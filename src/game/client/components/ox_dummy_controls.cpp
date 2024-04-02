//
// Created by olokreZ on 01.04.2024.
//

#include "ox_dummy_controls.h"

#include <engine/shared/config.h>

#include <game/client/gameclient.h>

#include <game/client/components/controls.h>

void ConOxDummyFly(IConsole::IResult *pResult, void *pUserData)
{
	bool *pdata = (bool *)pUserData;
	*pdata = pResult->GetInteger(0);
}

COxDummyControls::COxDummyControls()
{
	m_DummyFly = false;
	m_DummyHook = false;
	m_DummyJump = false;
	m_DummyDirectionLeft = false;
	m_DummyDirectionRight = false;

	m_DummyFire = false;
}

void COxDummyControls::OnConsoleInit()
{
	Console()->Register("+dummyjump", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyJump, "Dummy jump");
	Console()->Register("+dummyfly", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyFly, "Dummy HF (HammerFly)");
	Console()->Register("+dummyhook", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyHook, "Dummy HF (HammerFly)");
	Console()->Register("+dummyleft", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyDirectionLeft, "Dummy move left");
	Console()->Register("+dummyright", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyDirectionRight, "Dummy move right");
}

void COxDummyControls::OnInit()
{
	m_pControls = &GameClient()->m_Controls;
}
void COxDummyControls::update(CNetObj_PlayerInput *pDummy)
{
	// mem_zero(pDummy, sizeof *pDummy);

	// pDummy->m_WantedWeapon = m_pClient->m_DummyInput.m_WantedWeapon;
	// pDummy->m_TargetX = m_pClient->m_DummyInput.m_TargetX;
	// pDummy->m_TargetY = m_pClient->m_DummyInput.m_TargetY;

	if(m_DummyFly || (m_DummyHook && g_Config.m_OxDummyAutoAimHook))
	{
		vec2 main_tee = m_pClient->m_LocalCharacterPos;
		vec2 dummy_tee = m_pClient->m_aClients[m_pClient->m_aLocalIds[!g_Config.m_ClDummy]].m_Predicted.m_Pos;
		vec2 dir = main_tee - dummy_tee;
		pDummy->m_TargetX = dir.x;
		pDummy->m_TargetY = dir.y;
	}

	if(m_DummyFly)
	{
		if(!m_DummyFire)
		{
			m_DummyFire = true;
			pDummy->m_Fire = (pDummy->m_Fire + 1) | 1;
			pDummy->m_WantedWeapon = m_pClient->m_DummyInput.m_WantedWeapon = WEAPON_HAMMER + 1;
		}
		return;
	}

	if(m_DummyHook)
	{
		pDummy->m_Hook = 1;
	}
	else
		pDummy->m_Hook = 0;

	if(m_DummyJump)
		pDummy->m_Jump = 1;
	else
		pDummy->m_Jump = 0;

	if(m_DummyDirectionLeft != m_DummyDirectionRight)
	{
		if(m_DummyDirectionLeft)
			pDummy->m_Direction = -1;
		else if(m_DummyDirectionRight)
			pDummy->m_Direction = 1;
	}
	else
		pDummy->m_Direction = 0;
}
