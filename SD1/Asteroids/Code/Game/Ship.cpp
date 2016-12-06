#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Game/Ship.hpp"
#include "Game/TheApp.hpp"

#include <math.h>
#include <cstdlib>

const float SHIP_ACCELERATION_KNOB = 150.0f;
const float SHIP_ROTATION_KNOB = .005f;
const float SHIP_SIZE_KNOB = 30.f;
const float SHIP_HITBOX_SCALING_KNOB = .45f;
const float SECONDS_UNTIL_EXPLOSION = 3.f;


#define VK_LEFT 0x25
#define VK_UP 0x26
#define VK_RIGHT 0x27


//-----------------------------------------------------------------------------
Ship::Ship()
	: m_deathAnimationTimer( 0.f )
	, m_readyToExplode( false )
{
	m_cosmeticRadius = SHIP_SIZE_KNOB;
	m_physicsRadius = SHIP_HITBOX_SCALING_KNOB * m_cosmeticRadius;
	m_shipColor = new Rgba();
}


//-----------------------------------------------------------------------------
Ship::Ship( float x, float y )
	: Ship()
{
	m_position.SetXY( x, y );
}

//-----------------------------------------------------------------------------
Ship::~Ship()
{
	delete m_shipColor; 
	m_shipColor = nullptr;
}


//-----------------------------------------------------------------------------
//i.e. Tip of the ship.
Vector2 Ship::GetLocalTipPosition()
{
	return ( Vector2( cos( m_orientation ), sin( m_orientation ) ) * m_cosmeticRadius ); //Note polar form.
}


//-----------------------------------------------------------------------------
void Ship::StartDeathTimer( )
{
	m_deathAnimationTimer = SECONDS_UNTIL_EXPLOSION;
}


//-----------------------------------------------------------------------------
bool Ship::IsAlive() const
{
	if ( m_deathAnimationTimer != 0.f ) return false; //Appear dead when dying to prevent further collision.
	else return m_isAlive;
}


//-----------------------------------------------------------------------------
void Ship::RenderDeath()
{
	if ( m_deathAnimationTimer > 0.f )
	{
		float decayColor = RangeMap( m_deathAnimationTimer, 0.f, SECONDS_UNTIL_EXPLOSION, 0.f, 1.0f );
		m_shipColor->red = m_shipColor->green = m_shipColor->blue = decayColor;
	}
	else 
	{
		m_readyToExplode = true;
		m_isAlive = false;
		m_shipColor->red = m_shipColor->blue = m_shipColor->green = 1.0f;
		m_deathAnimationTimer = 0.f;
	}
}


//-----------------------------------------------------------------------------
//Assumes cosmeticRadius circumscribes the ship.
void Ship::Render()
{
	if ( !m_isAlive ) return;

	if ( m_deathAnimationTimer != 0.f ) RenderDeath();

	Vector2 rotatedTip = GetLocalTipPosition();
	const float ANGLE_SCALING_KNOB = 1.2f; //Keep ship from looking like an equilateral triangle.
	const float ANGLE_OF_SHIP_LEGS = ( 2.f*PI / 3.f ) * ANGLE_SCALING_KNOB;
	const float ANGLE_OF_SHIP_NOSE = ( PI / 12.f ) * ANGLE_SCALING_KNOB; //Scale '-' bit of the 'A' shape by same amount.

	g_theRenderer->PushView();

	g_theRenderer->TranslateView( m_position );
//	g_theRenderer->RotateViewByRadians( m_orientation );

	//Left '/' side of the 'A' shape.
	Vector2 pointOnUnitCircle = Vector2( cos( m_orientation - ANGLE_OF_SHIP_LEGS ), sin( m_orientation - ANGLE_OF_SHIP_LEGS ) );
	g_theRenderer->DrawLine( rotatedTip, ( pointOnUnitCircle * m_cosmeticRadius ), *m_shipColor, *m_shipColor );

	//Right '\' side of the 'A' shape.
	pointOnUnitCircle = Vector2( cos( m_orientation + ANGLE_OF_SHIP_LEGS ), sin( m_orientation + ANGLE_OF_SHIP_LEGS ) );
	g_theRenderer->DrawLine( rotatedTip, ( pointOnUnitCircle * m_cosmeticRadius ), *m_shipColor, *m_shipColor );

	//Center '-' part of the 'A' shape.
	pointOnUnitCircle = Vector2( cos( m_orientation - HALF_PI ), sin( m_orientation - HALF_PI ) );
	Vector2 oppositePointOnUnitCircle = Vector2( cos( m_orientation + HALF_PI ), sin( m_orientation + HALF_PI ) );
	float reducedRadius = m_cosmeticRadius * tan( ANGLE_OF_SHIP_NOSE );
	g_theRenderer->DrawLine( ( pointOnUnitCircle * reducedRadius ), ( oppositePointOnUnitCircle * reducedRadius ), *m_shipColor, *m_shipColor );

	if ( g_theApp->isKeyDown( 'W' ) || g_theApp->isKeyDown( VK_UP ) 
		|| (g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr && g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->GetRightTrigger( ) > 0.f)
		)
	{
		Vector2 flickerTip = Vector2( -m_cosmeticRadius*cos( m_orientation ), -m_cosmeticRadius*sin( m_orientation ) );
		float angleOfFlicker = rand() / static_cast<float>(RAND_MAX); //Use rand to vary the angle of the V.

		//Left '\' side of the 'V' shape.
		pointOnUnitCircle = Vector2( cos( m_orientation - angleOfFlicker ), sin( m_orientation - angleOfFlicker ) );
		g_theRenderer->DrawLine( flickerTip, ( pointOnUnitCircle * -m_cosmeticRadius ), *m_shipColor, Rgba( 0.f, 0.f, 1.f ) );

		//Right '/' side of the 'V' shape.
		oppositePointOnUnitCircle = Vector2( cos( m_orientation + angleOfFlicker ), sin( m_orientation + angleOfFlicker ) );
		g_theRenderer->DrawLine( flickerTip, ( oppositePointOnUnitCircle * -m_cosmeticRadius ), *m_shipColor, Rgba( 0.f, 0.f, 1.f ) );
	}

	g_theRenderer->PopView();
	
	g_theRenderer->SetDrawColor( 1.f, 1.f, 1.f, 1.f );
}


//-----------------------------------------------------------------------------
void Ship::Update( float deltaSeconds )
{
	if ( g_theApp->isKeyDown( 'W' ) || g_theApp->isKeyDown( VK_UP ) )
	{
		m_acceleration = Vector2( SHIP_ACCELERATION_KNOB*cos( m_orientation ), SHIP_ACCELERATION_KNOB*sin( m_orientation ) );
		m_velocity += m_acceleration * deltaSeconds;
	}
	else if ( g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr && g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->GetRightTrigger() > 0.f )
	{
		m_acceleration = Vector2( SHIP_ACCELERATION_KNOB*cos( m_orientation ), SHIP_ACCELERATION_KNOB*sin( m_orientation ) );
		m_velocity += m_acceleration * deltaSeconds * g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->GetRightTrigger();
	}


	if ( g_theApp->isKeyDown( 'A' ) || g_theApp->isKeyDown( VK_LEFT ) )
	{
		m_orientation += SHIP_ROTATION_KNOB;
	}
	else if ( g_theApp->isKeyDown( 'D' ) || g_theApp->isKeyDown( VK_RIGHT ) )
	{
		m_orientation -= SHIP_ROTATION_KNOB;
	}
	else
	{
		m_angularVelocity = 0;
	}

	if ( g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr )
	{
		Vector2 leftStickPos = g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->GetLeftStickPosition();
		if ( leftStickPos.x != 0.f && leftStickPos.y != 0.f )
			m_orientation = g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->GetLeftStickPositionAsAngleInRadians();
	}

	if ( m_deathAnimationTimer != 0.f ) m_deathAnimationTimer -= deltaSeconds;

	Entity::Update( deltaSeconds );
}
