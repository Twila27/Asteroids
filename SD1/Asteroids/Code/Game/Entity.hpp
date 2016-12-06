#pragma once
#include "Engine/Math/Vector2.hpp"


//-----------------------------------------------------------------------------
class Entity 
{
protected:
	Vector2 m_position;
	Vector2 m_velocity;
	float m_angularVelocity;
	float m_orientation;
	Vector2 m_acceleration;

	float m_physicsRadius;
	float m_cosmeticRadius;

	float m_ageInSeconds;
	bool m_isAlive;
public:
	Entity();
	virtual void Update(float deltaSeconds);
	//virtual void Render();
	bool DoesOverlap( const Entity& ent ) const;
	Vector2 GetPosition() const { return m_position; }
	Vector2 GetVelocity() const { return m_velocity; }
	float GetOrientation() const { return m_orientation; }
	void SetPosition( float newX, float newY );
	void SetPosition( Vector2 newPos ) { m_position = newPos;  }
	void SetVelocity( float newX, float newY );
	void SetVelocity( Vector2 newVel ) { m_velocity = newVel; }
	void SetOrientation( float newOrientation );
	void SetAngularVelocity( float newAngularVelocity );
	void SetIsAlive( bool newStatus );
	virtual bool IsAlive() const { return m_isAlive; }
};