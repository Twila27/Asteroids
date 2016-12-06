#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/Asteroid.hpp"

#include <math.h>
#include <cstdlib>

int ASTEROID_FIXED_SPEED_KNOB = 110;
const int ASTEROID_MAX_ORIENTATION_KNOB = 6; //<int>(2pi).
const int ASTEROID_MAX_ROTATION_SPEED_KNOB = 10;
const float ASTEROID_SIZE_KNOB = 50.f; //Refers directly to rendering size.
const int ASTEROID_SIZE_MAX = 3; //Refers to the breaking of asteroids, not render size. But note Bossteroid grows beyond this!
const float ASTEROID_HITBOX_SCALING_KNOB = .65f;


//--------------------------------------------------------------------------------------------------------------
Asteroid::Asteroid()
	: m_shape( static_cast<Shape>( rand() % 4 ) )
	, m_size( ASTEROID_SIZE_MAX )
{
	m_cosmeticRadius = ASTEROID_SIZE_KNOB;
	m_physicsRadius = ASTEROID_HITBOX_SCALING_KNOB * m_cosmeticRadius;

	m_position.SetXY( static_cast<float>( rand() % 1600 ), static_cast<float>( rand() % 900 ) );
	m_velocity.SetXY( static_cast<float>( ASTEROID_FIXED_SPEED_KNOB ), static_cast<float>( ASTEROID_FIXED_SPEED_KNOB ) );
	if ( rand() % 2 ) m_velocity.x *= -1.f;
	if ( rand() % 2 ) m_velocity.y *= -1.f;
	m_acceleration.SetXY( 0.f, 0.f );
	m_orientation = static_cast<float>( rand() % ASTEROID_MAX_ORIENTATION_KNOB );
	m_angularVelocity = static_cast<float>( rand() % ASTEROID_MAX_ROTATION_SPEED_KNOB );
	if ( rand() % 2 ) m_angularVelocity *= -1.f;

	switch ( m_shape ) //Set parameters for the below rendering.
	{
		case LEAST_SIDES: m_asteroidSides = ASTEROID_MAX_SIDES_KNOB*0.70f; break;
		case LESS_SIDES: m_asteroidSides = ASTEROID_MAX_SIDES_KNOB*0.80f; break;
		case MORE_SIDES: m_asteroidSides = ASTEROID_MAX_SIDES_KNOB*0.90f; break;
		case MOST_SIDES: m_asteroidSides = ASTEROID_MAX_SIDES_KNOB; break;
	}

	for ( int i = 0; i < m_asteroidSides; i++ )
	{
		float normalizedRandNum = rand() / static_cast<float>( RAND_MAX );
		m_radiusScaleKnobs[ i ] = ASTEROID_HITBOX_SCALING_KNOB > normalizedRandNum ? ASTEROID_HITBOX_SCALING_KNOB : normalizedRandNum;
	}
}

//--------------------------------------------------------------------------------------------------------------
Asteroid::Asteroid( int size )
	: Asteroid()
{
	while ( m_size != size ) DecreaseSize();
}


//--------------------------------------------------------------------------------------------------------------
Asteroid::Asteroid( int screenWidth, int screenHeight ) 
	: Asteroid()
{
	if ( rand() % 2 ) m_position.SetXY( -m_cosmeticRadius, static_cast<float>( rand() % screenHeight ) ); //Along left.
	else m_position.SetXY( static_cast<float>( rand( ) % screenWidth), -m_cosmeticRadius ); //Along bottom.
}


//--------------------------------------------------------------------------------------------------------------
void Asteroid::Render()
{
	if ( !m_isAlive ) return;

	const float radiansTotal = 2.f * PI;
	const float radiansPerSide = radiansTotal / m_asteroidSides;
	float lineThickness = ( m_size > 3 ? 5.f : 1.f ); //Bossteroid thicker.
	Rgba asteroidStartColor = ( m_size > 3 ? Rgba( .8f, 0.f, 0.f ) : Rgba() ); //Bossteroid's dark red-brown color.
	Rgba asteroidEndColor = ( m_size > 3 ? Rgba( 1.f, 0.f, 0.f ) : Rgba() ); //Bossteroid's dark red-brown color.

	//Deconstructing what was a GL_LINE_LOOP to work with the GL_LINES-based Renderer class by caching last iteration.
	Vector2* firstPos = nullptr; //Saved for the final line draw request back to close the shape where it started.
	Vector2* savedPos = nullptr;
	for ( float radians = 0.f; radians < radiansTotal; radians += radiansPerSide ) {
			
			
		float rotatedRadians = radians + ( DegreesToRadians(m_orientation) ); 
		float reducedRadius = m_radiusScaleKnobs[ static_cast<int>(radians / radiansPerSide) ] * m_cosmeticRadius;
			
			
		float x = m_position.x + ( reducedRadius * cos( rotatedRadians ) );
		float y = m_position.y + ( reducedRadius * sin( rotatedRadians ) );


		if ( savedPos != nullptr ) g_theRenderer->DrawLine( *savedPos, Vector2( x, y ), asteroidStartColor, asteroidEndColor, lineThickness );
		else {
			firstPos = new Vector2( x, y );
			savedPos = new Vector2( x, y ); 
		}
		*savedPos = Vector2( x, y );
	}

	g_theRenderer->DrawLine( *firstPos, *savedPos, asteroidStartColor, asteroidEndColor, lineThickness );
	delete firstPos;
	delete savedPos;
	firstPos = nullptr;
	savedPos = nullptr;
}


//-----------------------------------------------------------------------------
void Asteroid::DecreaseSize()
{
	--m_size; 
	m_cosmeticRadius = ASTEROID_SIZE_KNOB * ( static_cast<float>( m_size ) / static_cast<float>( ASTEROID_SIZE_MAX ) );
	m_physicsRadius = ASTEROID_HITBOX_SCALING_KNOB * m_cosmeticRadius;
}


//-----------------------------------------------------------------------------
void Asteroid::SetSize( int newSize )
{
	m_size = newSize; 
	m_cosmeticRadius = ASTEROID_SIZE_KNOB * ( static_cast<float>( m_size ) / static_cast<float>( ASTEROID_SIZE_MAX ) );
	m_physicsRadius = ASTEROID_HITBOX_SCALING_KNOB * m_cosmeticRadius;
}
