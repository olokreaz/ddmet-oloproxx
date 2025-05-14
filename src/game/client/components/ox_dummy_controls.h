//
// Created by olokreZ on 01.04.2024.
//

#ifndef GAME_CLIENT_COMPONENTS_OX_DUMMY_CONTROLS_H
#define GAME_CLIENT_COMPONENTS_OX_DUMMY_CONTROLS_H

#include <engine/shared/console.h>

#include <game/client/component.h>
#include <game/client/components/controls.h>

class COxDummyControls : public CComponent
{
public:
	enum
	{
		CONTROL_BIT_FLY = 0,
		CONTROL_BIT_HOOK,
		CONTROL_BIT_JUMP,
		CONTROL_BIT_DIR_LEFT,
		CONTROL_BIT_DIR_RIGHT,

		CONTROL_BIT_MAX,
	};

	uint8_t m_ControlFlag;
	int	m_DummyFire;

private:
	// bool m_DummyFly;
	// bool m_DummyHook;
	// bool m_DummyJump;
	// bool m_DummyDirectionLeft;
	// bool m_DummyDirectionRight;

	CControls *m_pControls;

public:
	COxDummyControls( );
	void OnInit( ) override;
	int  Sizeof( ) const override { return sizeof( *this ); }
	void OnConsoleInit( ) override;
	void update_dummy_controll( CNetObj_PlayerInput *pDummy );
	void update_dummy_auto_shoot_unfreze( );

	bool TakeControllDummy( ) const;
};

#endif	  // GAME_CLIENT_COMPONENTS_OX_DUMMY_CONTROLS_H
