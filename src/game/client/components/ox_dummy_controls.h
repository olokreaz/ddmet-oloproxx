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
	bool m_DummyFly;
	bool m_DummyHook;
	bool m_DummyJump;
	bool m_DummyDirectionLeft;
	bool m_DummyDirectionRight;

	uint8_t m_DummyHookTick; // 3 ticks

	CControls *m_pControls;

public:
	int m_DummyFire;

	COxDummyControls();
	void OnInit() override;
	int Sizeof() const override { return sizeof(*this); }
	void OnConsoleInit() override;
	void update_dummy_controll(CNetObj_PlayerInput *pDummy);
	void update_dummy_auto_shoot_unfreze();

	bool TakeDummy() const
	{
		return m_DummyFly || m_DummyHook || m_DummyJump || m_DummyDirectionLeft || m_DummyDirectionRight;
	}
};

#endif // GAME_CLIENT_COMPONENTS_OX_DUMMY_CONTROLS_H
