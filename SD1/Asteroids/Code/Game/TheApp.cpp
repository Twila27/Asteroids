#include "Game/TheApp.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

//---------------------------------------------------------------------
TheApp* g_theApp = nullptr;


//---------------------------------------------------------------------
TheApp::TheApp()
{
	for ( int keyIndex = 0; keyIndex < NUM_KEYS; ++keyIndex )
	{
		m_keys[ keyIndex ].m_isKeyDown = false;
		m_keys[ keyIndex ].m_didKeyJustChange = false;
	}
	for ( int controllerSlot = 0; controllerSlot < NUM_CONTROLLERS; ++controllerSlot )
	{
		controllers[ controllerSlot ] = nullptr;
	}
}


//---------------------------------------------------------------------
TheApp::TheApp( const double screenWidth, const double screenHeight )
	: TheApp()
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}


//---------------------------------------------------------------------
void TheApp::SetKeyDownStatus( unsigned char asKey, bool isNowDown )
{
	m_keys[ asKey ].m_didKeyJustChange = ( m_keys[ asKey ].m_isKeyDown != isNowDown ? true : false );
	m_keys[ asKey ].m_isKeyDown = isNowDown;
}


//---------------------------------------------------------------------
bool TheApp::isKeyDown( unsigned char keyID )
{
	return m_keys[ keyID ].m_isKeyDown;
}


//---------------------------------------------------------------------
bool TheApp::WasKeyJustPressed( unsigned char keyID )
{
	return m_keys[ keyID ].m_didKeyJustChange;
}

//---------------------------------------------------------------------
void TheApp::Update()
{
	for ( int keyIndex = 0; keyIndex < NUM_KEYS; ++keyIndex )
		m_keys[ keyIndex ].m_didKeyJustChange = false;

	//Check for controller updates, drop-ins/outs.
	XINPUT_STATE xboxControllerState;
	memset( &xboxControllerState, 0, sizeof( xboxControllerState ) );
	for ( int controllerSlot = 0; controllerSlot < 3; controllerSlot++ )
	{
		DWORD errorStatus = XInputGetState( controllerSlot, &xboxControllerState );
		if ( errorStatus == ERROR_SUCCESS )
		{
			if ( controllers[ controllerSlot ] == nullptr )
			{
				controllers[ controllerSlot ] = new XboxController( controllerSlot );
			}
			controllers[ controllerSlot ]->Update();
		}
		else if ( errorStatus == ERROR_DEVICE_NOT_CONNECTED )
		{

			if ( controllers[ controllerSlot ] != nullptr )
			{
				delete controllers[ controllerSlot ];
				controllers[ controllerSlot ] = nullptr;
			}
		}
	}
}
