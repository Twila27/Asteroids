#include "Engine/Math/MathUtils.hpp"
#include "Game/TheGame.hpp"
#include "Game/TheApp.hpp"
#include "Game/Asteroid.hpp"
#include "Game/Ship.hpp"
#include "Game/Bullet.hpp"

#include <cstdlib>
#include <math.h>

#define VK_SPACE 0x20
#define VK_NUMPAD1 0x61
#define VK_NUMPAD2 0x62
#define VK_NUMPAD3 0x63
#define VK_NUMPAD4 0x64

//-----------------------------------------------------------------------------
TheGame* g_theGame = nullptr;
const int INITIAL_NUMBER_OF_ASTEROIDS = 6;
const int NUMBER_OF_BULLETS_SHIP_EXPLODES_INTO_KNOB = 100;
const int BULLET_EXPLOSION_SPEED_KNOB = 1000;
const float ASTEROID_SPEED_CHANGE_ON_BREAKUP_SCALING_KNOB = 1.25f;


//-----------------------------------------------------------------------------
TheGame::TheGame()
	: m_ship( nullptr )
	, m_numBulletsAllocated( 0 )
	, m_numAsteroidsAllocated( INITIAL_NUMBER_OF_ASTEROIDS )
	, m_currentWave( 0 )
	, m_numBulletsFired( 0 )
{
	for ( int i = 0; i < MAX_NUMBER_OF_ASTEROIDS; i++ ) m_asteroids[ i ] = nullptr;
	for ( int i = 0; i < MAX_NUMBER_OF_BULLETS; i++ ) m_bullets[ i ] = nullptr;
}


//-----------------------------------------------------------------------------
TheGame::TheGame( double screenWidth, double screenHeight ) 
	: TheGame()
{
	m_ship = new Ship( static_cast<float>( screenWidth ) / 2.f, static_cast<float>( screenHeight ) / 2.f );
	for ( int i = 0; i < INITIAL_NUMBER_OF_ASTEROIDS; i++ ) m_asteroids[ i ] = new Asteroid( static_cast<int>( screenWidth ), static_cast<int>( screenHeight ) );
}


//-----------------------------------------------------------------------------
TheGame::~TheGame()
{
	delete m_ship;
	for ( int i = 0; i < m_numAsteroidsAllocated; i++ ) delete m_asteroids[ i ];
}


//-----------------------------------------------------------------------------
void TheGame::Update( float deltaSeconds )
{
	m_ship->Update( deltaSeconds );


	for ( int i = 0; i < m_numAsteroidsAllocated; i++ )
	{
		if ( m_asteroids[ i ]->IsAlive() )
		{
			m_asteroids[ i ]->Update( deltaSeconds );
			if ( m_ship->IsAlive() && m_ship->DoesOverlap( *m_asteroids[ i ] ) )
			{
				m_ship->StartDeathTimer();
				m_asteroids[ i ]->SetIsAlive( false );
				delete m_asteroids[ i ];
				m_asteroids[ i ] = nullptr;
				if ( m_numAsteroidsAllocated > 1 ) //Fill gap in array.
				{
					m_asteroids[ i ] = m_asteroids[ m_numAsteroidsAllocated - 1 ];
					m_asteroids[ m_numAsteroidsAllocated - 1 ] = nullptr;
				}
				--m_numAsteroidsAllocated;
			}
			for ( int j = 0; j < m_numBulletsAllocated; j++ )
			{
				if ( m_bullets[ j ]->IsAlive() && m_asteroids[ i ] != nullptr && m_bullets[ j ]->DoesOverlap( *m_asteroids[ i ] ) )
				{
					m_bullets[ j ]->SetIsAlive( false );
					if ( m_asteroids[ i ]->GetSize() == 1 ) //Smallest size.
					{
						m_asteroids[ i ]->SetIsAlive( false );
						delete m_asteroids[ i ];
						m_asteroids[ i ] = nullptr;
						if ( m_numAsteroidsAllocated > 1 ) //Fill gap in array.
						{
							m_asteroids[ i ] = m_asteroids[ m_numAsteroidsAllocated - 1 ];
							m_asteroids[ m_numAsteroidsAllocated - 1 ] = nullptr;
						}
						--m_numAsteroidsAllocated;
					}
					else if (m_numAsteroidsAllocated < MAX_NUMBER_OF_ASTEROIDS )
					{
						m_asteroids[ i ]->DecreaseSize();
						if (m_asteroids[i]->GetSize() < 3 ) m_asteroids[ i ]->SetVelocity( m_asteroids[ i ]->GetVelocity() * ASTEROID_SPEED_CHANGE_ON_BREAKUP_SCALING_KNOB );
						//Break asteroid by making one a tinier version of itself, and spawning off a new one with opposite velocity.
						//Maybe lower its velocity?
						//Make new asteroid.
						m_asteroids[ m_numAsteroidsAllocated ] = new Asteroid();
						m_asteroids[ m_numAsteroidsAllocated ]->SetPosition( m_asteroids[ i ]->GetPosition() );
						m_asteroids[ m_numAsteroidsAllocated ]->SetVelocity( m_asteroids[ i ]->GetVelocity() * -1.f);
						m_asteroids[ m_numAsteroidsAllocated ]->SetSize(m_asteroids[i]->GetSize());
						++m_numAsteroidsAllocated;
					}
				}
			}
		}
	}
	if ( m_numAsteroidsAllocated == 0 )
	{
		++m_currentWave;
		for ( int i = 0; i < INITIAL_NUMBER_OF_ASTEROIDS * m_currentWave; i++ )
		{
			if ( m_numAsteroidsAllocated < MAX_NUMBER_OF_ASTEROIDS ) 
				m_asteroids[ i ] = new Asteroid( static_cast<int>( g_theApp->screenWidth ), static_cast<int>( g_theApp->screenHeight ) );
		}
		m_numAsteroidsAllocated = m_currentWave * INITIAL_NUMBER_OF_ASTEROIDS;
		if ( m_numAsteroidsAllocated < MAX_NUMBER_OF_ASTEROIDS )
		{
			m_asteroids[ m_numAsteroidsAllocated ] = new Asteroid( static_cast<int>( g_theApp->screenWidth ), static_cast<int>( g_theApp->screenHeight ) );
			m_asteroids[ m_numAsteroidsAllocated ]->SetSize( m_currentWave + ASTEROID_SIZE_MAX );
			++m_numAsteroidsAllocated;
		}
	}


	for ( int i = 0; i < m_numBulletsAllocated; i++ )
	{
		m_bullets[ i ]->Update( deltaSeconds );
		if ( !m_bullets[ i ]->IsAlive() )
		{
			if ( m_bullets[ i ]->GetBulletType() != Bullet::STANDARD ) SpawnBulletBurst(m_bullets[i]->GetBulletType() * Bullet::CLUSTER_BULLET_STORM_SIZE, m_bullets[i]->GetPosition(), m_bullets[i]->GetOrientation());
			delete m_bullets[ i ];
			m_bullets[ i ] = nullptr;
			if ( m_numBulletsAllocated > 1 ) //Fill gap in array.
			{
				m_bullets[ i ] = m_bullets[ m_numBulletsAllocated - 1 ];
				m_bullets[ m_numBulletsAllocated - 1 ] = nullptr;
			}
			--m_numBulletsAllocated;
		}
	}


	if ( m_ship->m_readyToExplode )
	{
		m_ship->m_readyToExplode = false;
		//Goal is to spawn bullets in all directions from the position and with the velocity of the ship.SpawnBulletBurst();
		SpawnBulletBurst( NUMBER_OF_BULLETS_SHIP_EXPLODES_INTO_KNOB, m_ship->GetLocalTipPosition( ) + m_ship->GetPosition( ), m_ship->GetOrientation( ) );
	}


	if ( //Controller.
			( g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr &&
			g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->IsStartButtonDown() &&
			g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->WasStartButtonJustPressed() )
		|| //Keyboard.
			(g_theApp->isKeyDown( 'P' ) && m_ship->IsAlive() == false) 
		)
	{
		m_ship->SetPosition( static_cast<float>(g_theApp->screenWidth) / 2.f, static_cast<float>(g_theApp->screenHeight) / 2.f );
		m_ship->SetVelocity( 0.f, 0.f );
		m_ship->SetOrientation( 0.f );
		m_ship->SetAngularVelocity( 0.f );
		m_ship->SetIsAlive( true );
	}

	if ( g_theApp-> isKeyDown('O') && g_theApp->WasKeyJustPressed( 'O' ) && m_numAsteroidsAllocated < MAX_NUMBER_OF_ASTEROIDS )
	{
		m_asteroids[ m_numAsteroidsAllocated ] = new Asteroid();
		++m_numAsteroidsAllocated;
	}

	if ( g_theApp->isKeyDown( 'L' ) && g_theApp->WasKeyJustPressed( 'L' ) && m_numAsteroidsAllocated > 0 ) //Destroy.
	{
		delete m_asteroids[ m_numAsteroidsAllocated-1 ]; //-1 makes the count 0-based.
		m_asteroids[ m_numAsteroidsAllocated-1 ] = nullptr;
		--m_numAsteroidsAllocated;
	}

	if ( m_ship->IsAlive() == false ) return; //Dead ships don't shoot.

	if ( m_numBulletsAllocated < MAX_NUMBER_OF_BULLETS &&
		( //Controller.
		( g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr &&
		g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->IsAButtonDown() &&
		g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->WasAButtonJustPressed() )
		|| //Keyboard.
		( g_theApp->isKeyDown( VK_SPACE ) && g_theApp->WasKeyJustPressed( VK_SPACE ) ) ||
		( g_theApp->isKeyDown( VK_NUMPAD1 ) && g_theApp->WasKeyJustPressed( VK_NUMPAD1 ) ) ||
		( g_theApp->isKeyDown( '1' ) && g_theApp->WasKeyJustPressed( '1' ) ) 
		)
		)
	{
		m_bullets[ m_numBulletsAllocated ] = new Bullet( m_ship->GetLocalTipPosition() + m_ship->GetPosition(), m_ship->GetOrientation() );
		++m_numBulletsAllocated;
		m_numBulletsFired += 1;
		return; //No firing multiple bullet types at once.
	}

	if ( m_numBulletsFired >= Bullet::NUMBER_SHOTS_BEFORE_CLUSTER_READY &&
		( //Controller.
		( g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr &&
		g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->IsBButtonDown( ) &&
		g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->WasBButtonJustPressed( ) )
		|| //Keyboard.
		( g_theApp->isKeyDown( VK_NUMPAD2 ) && g_theApp->WasKeyJustPressed( VK_NUMPAD2 ) ) ||
		( g_theApp->isKeyDown( '2' ) && g_theApp->WasKeyJustPressed( '2' ) )
		)
		)
	{
		m_bullets[ m_numBulletsAllocated ] = new Bullet( m_ship->GetLocalTipPosition( ) + m_ship->GetPosition( ), m_ship->GetOrientation( ), Bullet::CLUSTER );
		++m_numBulletsAllocated;
		m_numBulletsFired -= Bullet::NUMBER_SHOTS_BEFORE_CLUSTER_READY;
		return;
	}

	if ( m_numBulletsFired >= 2 * Bullet::NUMBER_SHOTS_BEFORE_CLUSTER_READY &&
		( //Controller.
		( g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr &&
		g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->IsXButtonDown() &&
		g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->WasXButtonJustPressed() )
		|| //Keyboard.
		( g_theApp->isKeyDown( VK_NUMPAD3 ) && g_theApp->WasKeyJustPressed( VK_NUMPAD3 ) ) ||
		( g_theApp->isKeyDown( '3' ) && g_theApp->WasKeyJustPressed( '3' ) )
		)
		)
	{
		m_bullets[ m_numBulletsAllocated ] = new Bullet( m_ship->GetLocalTipPosition() + m_ship->GetPosition(), m_ship->GetOrientation(), Bullet::MEGA_CLUSTER );
		++m_numBulletsAllocated;
		m_numBulletsFired -= 2 * Bullet::NUMBER_SHOTS_BEFORE_CLUSTER_READY;
		return;
	}

	if ( m_numBulletsFired >= 3 * Bullet::NUMBER_SHOTS_BEFORE_CLUSTER_READY &&
		( //Controller.
		( g_theApp->controllers[ TheApp::CONTROLLER_ONE ] != nullptr &&
		g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->IsYButtonDown() &&
		g_theApp->controllers[ TheApp::CONTROLLER_ONE ]->WasYButtonJustPressed() )
		|| //Keyboard.
		( g_theApp->isKeyDown( VK_NUMPAD4 ) && g_theApp->WasKeyJustPressed( VK_NUMPAD4 ) ) ||
		( g_theApp->isKeyDown( '4' ) && g_theApp->WasKeyJustPressed( '4' ) )
		)
		)
	{
		m_bullets[ m_numBulletsAllocated ] = new Bullet( m_ship->GetLocalTipPosition() + m_ship->GetPosition(), m_ship->GetOrientation(), Bullet::MEGA_MEGA_CLUSTER );
		++m_numBulletsAllocated;
		m_numBulletsFired -= 3 * Bullet::NUMBER_SHOTS_BEFORE_CLUSTER_READY;
		return;
	}
}


void TheGame::SpawnBulletBurst(int numBullets, const Vector2& bulletPos, float bulletAngle)
{
	for ( int i = 0; i < numBullets; i++ )
	{
		if ( m_numBulletsAllocated >= MAX_NUMBER_OF_BULLETS ) break;
		m_bullets[ m_numBulletsAllocated ] = new Bullet( bulletPos, bulletAngle );
		m_bullets[ m_numBulletsAllocated ]->SetVelocity(
			static_cast<float>( rand() % BULLET_EXPLOSION_SPEED_KNOB )*cos( RangeMap( static_cast<float>( i ), 0.f, static_cast<float>( numBullets ), 0.f, 2.f * PI ) ),
			static_cast<float>( rand() % BULLET_EXPLOSION_SPEED_KNOB )*sin( RangeMap( static_cast<float>( i ), 0.f, static_cast<float>( numBullets ), 0.f, 2.f * PI ) ) );
		++m_numBulletsAllocated;
	}
}

//-----------------------------------------------------------------------------
void TheGame::Render()
{
	m_ship->Render();
	for ( int i = 0; i < m_numAsteroidsAllocated; i++ )
	{
		m_asteroids[ i ]->Render();
	}
	for ( int i = 0; i < m_numBulletsAllocated; i++ )
	{
		m_bullets[ i ]->Render();
	}

}
