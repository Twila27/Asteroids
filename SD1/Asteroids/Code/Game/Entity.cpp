#include "Engine/Math/MathUtils.hpp"
#include "Game/Entity.hpp"
#include "Game/TheApp.hpp"

//--------------------------------------------------------------------------------------------------------------
Entity::Entity()
	: m_position( Vector2( 500.f, 500.f ) )
	, m_velocity( Vector2( 0.f, 0.f ) )
	, m_orientation( 0.f )
	, m_angularVelocity( 0.f )
	, m_acceleration( Vector2( 0.f, 0.f ) )
	, m_cosmeticRadius( 50.f )
	, m_physicsRadius( 35.f )
	, m_isAlive( true )
	, m_ageInSeconds( 0.f )
{

}


//--------------------------------------------------------------------------------------------------------------
void Entity::Update( float deltaSeconds )
{
	m_ageInSeconds += deltaSeconds;

	m_position += m_velocity * deltaSeconds;
	m_orientation += m_angularVelocity * deltaSeconds;

	if ( ( m_position.x + m_cosmeticRadius ) < 0.f ) m_position.x = static_cast<float>( g_theApp->screenWidth ) + m_cosmeticRadius;
	else if ( ( m_position.x - m_cosmeticRadius ) > g_theApp->screenWidth ) m_position.x = 0.f - m_cosmeticRadius;

	if ( ( m_position.y + m_cosmeticRadius ) < 0.f ) m_position.y = static_cast<float>( g_theApp->screenHeight ) + m_cosmeticRadius;
	else if ( ( m_position.y - m_cosmeticRadius ) > g_theApp->screenHeight ) m_position.y = 0.f - m_cosmeticRadius;
}


//--------------------------------------------------------------------------------------------------------------
bool Entity::DoesOverlap( const Entity& ent ) const
{
	return DoDiscsOverlap(m_position, m_physicsRadius, ent.m_position, ent.m_physicsRadius);
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetPosition( float newX, float newY )
{
	m_position.x = newX;
	m_position.y = newY;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetVelocity( float newX, float newY )
{
	m_velocity.x = newX;
	m_velocity.y = newY;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetOrientation( float newOrientation )
{
	m_orientation = newOrientation;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetAngularVelocity( float newAngularVelocity )
{
	m_angularVelocity = newAngularVelocity;
}


//--------------------------------------------------------------------------------------------------------------
void Entity::SetIsAlive( bool newStatus )
{
	m_isAlive = newStatus;
}
