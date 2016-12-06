#pragma once


//--------------------------------------------------
class TheGame;
class Asteroid;
class Bullet;
class Ship;


//--------------------------------------------------
const int MAX_NUMBER_OF_ASTEROIDS = 270;
const int MAX_NUMBER_OF_BULLETS = 400;
extern TheGame* g_theGame;


//--------------------------------------------------
class TheGame
{
private:
	int m_numAsteroidsAllocated;
	int m_numBulletsAllocated;
	int m_numBulletsFired;
	int m_currentWave;
	void SpawnBulletBurst( int numBullets, const Vector2& bulletPos, float bulletAngle );
public:
	Ship* m_ship;
	Asteroid* m_asteroids[ MAX_NUMBER_OF_ASTEROIDS ];
	Bullet* m_bullets[ MAX_NUMBER_OF_BULLETS ];
	TheGame();
	TheGame( double screenWidth, double screenHeight );
	~TheGame();
	void Update( float deltaSeconds );
	void Render();
};