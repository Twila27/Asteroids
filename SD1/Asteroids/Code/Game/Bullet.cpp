#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Game/Entity.hpp"
#include "Game/Bullet.hpp"

#include <math.h>

const float SECONDS_BEFORE_DISAPPEARING = 2.f;
const float BULLET_SIZE_KNOB = 1.f;
const float BULLET_SPEED_KNOB = 400.f;

//-----------------------------------------------------------------------------
Bullet::Bullet( )
{
	m_timeToLive = SECONDS_BEFORE_DISAPPEARING;
	m_cosmeticRadius = BULLET_SIZE_KNOB;
	m_physicsRadius = m_cosmeticRadius;
}


//-----------------------------------------------------------------------------
Bullet::Bullet( const Vector2& position, const float m_orientation, const BulletType type /*= STANDARD */ )
	: Bullet()
{
	m_position = position;
	m_velocity.x = BULLET_SPEED_KNOB * cos( m_orientation );
	m_velocity.y = BULLET_SPEED_KNOB * sin( m_orientation );
	m_type = type;
}


//-----------------------------------------------------------------------------
void Bullet::Render()
{
	if ( !m_isAlive ) return;
	
	Vector2 startPos = Vector2( m_cosmeticRadius + m_position.x, m_cosmeticRadius + m_position.y );
	Vector2 endPos = Vector2( startPos.x + BULLET_SIZE_KNOB, startPos.y + BULLET_SIZE_KNOB );

	Rgba bulletColor = Rgba( 0.f, 1.f, 0.f, 1.f );
	float bulletThickness = 1.f;

	switch ( m_type )
	{
	case BulletType::STANDARD:
		bulletColor = Rgba( 1.f, 1.f, 1.f, 1.f ); 
		bulletThickness = 1.f;
		break; //Full white
	case BulletType::CLUSTER:
		bulletColor = Rgba( 1.f, 0.75f, 0.75f, 1.f ); 
		bulletThickness = 2.f;
		break;
	case BulletType::MEGA_CLUSTER:
		bulletColor = Rgba( 1.f, 0.25f, 0.25f, 1.f ); 
		bulletThickness = 3.f;
		break;
	case BulletType::MEGA_MEGA_CLUSTER:
		bulletColor = Rgba( 1.f, 0.f, 0.f, 1.f ); 
		bulletThickness = 4.f;
		break; //Full red
	}

	g_theRenderer->DrawLine( startPos, endPos, bulletColor, bulletColor, bulletThickness );
}
void Bullet::Update(float deltaSeconds)
{
	Entity::Update(deltaSeconds);
	m_timeToLive -= deltaSeconds;
	if ( m_timeToLive <= 0.f ) m_isAlive = false;
}