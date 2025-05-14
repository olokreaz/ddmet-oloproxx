/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_CLIENT_COMPONENTS_CONTROLS_H
#define GAME_CLIENT_COMPONENTS_CONTROLS_H

#include <base/vmath.h>

#include <engine/client.h>

#include <game/client/component.h>
#include <game/generated/protocol.h>

class CControls : public CComponent
{
	float GetMaxMouseDistance( ) const;

	CNetObj_PlayerInput m_DummyEmpty;

public:
	vec2  m_aMousePos[ NUM_DUMMIES ];
	vec2  m_aMousePosOnAction[ NUM_DUMMIES ];
	vec2  m_aTargetPos[ NUM_DUMMIES ];
	float m_OldMouseX;
	float m_OldMouseY;

	int m_aAmmoCount[ NUM_WEAPONS ];

	CNetObj_PlayerInput m_aInputData[ NUM_DUMMIES ];
	CNetObj_PlayerInput m_aLastData[ NUM_DUMMIES ];
	int		    m_aInputDirectionLeft[ NUM_DUMMIES ];
	int		    m_aInputDirectionRight[ NUM_DUMMIES ];
	int		    m_aShowHookColl[ NUM_DUMMIES ];
	int		    m_aQLastWeapon[ NUM_DUMMIES ];
	int		    m_LastDummy;
	int		    m_OtherFire;

	CControls( );
	int Sizeof( ) const override { return sizeof( *this ); }

	void OnReset( ) override;
	void OnRelease( ) override;
	void OnRender( ) override;
	void OnMessage( int MsgType, void *pRawMsg ) override;
	bool OnCursorMove( float x, float y, IInput::ECursorType CursorType ) override;
	void OnConsoleInit( ) override;
	void OnPlayerDeath( );

	int CurrentTeeSnapInput( int *pData );

	int DummySnapInput( int *pData, bool Force );

	void ClampMousePos( );
	void ResetInput( int Dummy );
};
#endif
