#pragma once
#include "Game/Entity.hpp"

struct Rgba;


//-----------------------------------------------------------------------------
class Ship : public Entity
{
private: 
	float m_deathAnimationTimer;
	Rgba* m_shipColor;
	void RenderDeath();
public: 
	bool m_readyToExplode;
	Ship();
	~Ship();
	Ship( float x, float y );
	void Render();
	void Update( float deltaSeconds ) override;
	void StartDeathTimer();
	bool IsAlive() const override;
	Vector2 GetLocalTipPosition();
};