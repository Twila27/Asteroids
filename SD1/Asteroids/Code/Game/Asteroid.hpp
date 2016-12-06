#pragma once
#include "Game/Entity.hpp"


//-----------------------------------------------------------------------------
const int ASTEROID_MAX_SIDES_KNOB = 16;
extern const int ASTEROID_SIZE_MAX;


//-----------------------------------------------------------------------------
class Asteroid : public Entity
{
public:
	Asteroid();
	Asteroid( int size );
	Asteroid( int screenWidth, int screenHeight );
	int GetSize() const { return m_size; }
	void Render();
	void DecreaseSize();
	void SetSize( int newSize );
private:
	enum Shape { LEAST_SIDES, LESS_SIDES, MORE_SIDES, MOST_SIDES };
	Shape m_shape;
	int m_size;
	float m_asteroidSides;
	float m_radiusScaleKnobs[ ASTEROID_MAX_SIDES_KNOB ];

};