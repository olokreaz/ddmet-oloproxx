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

	CControls *m_pControls;

public:
	COxDummyControls();
	void OnInit() override;
	int Sizeof() const { return sizeof(*this); }
	void OnConsoleInit() override;
	void update(CNetObj_PlayerInput *pDummy);
	bool TakeDummy() const { return m_DummyFly || m_DummyHook || m_DummyJump || m_DummyDirectionLeft || m_DummyDirectionRight; }
};

#endif // GAME_CLIENT_COMPONENTS_OX_DUMMY_CONTROLS_H
