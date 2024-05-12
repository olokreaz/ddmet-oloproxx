//
// Created by olokreZ on 01.04.2024.
//

#include "ox_dummy_controls.h"

#include <engine/shared/config.h>

#include <game/client/gameclient.h>

#include <game/client/components/controls.h>

void
ConOxDummyFly( IConsole::IResult* pResult, void* pUserData )
{
	bool* pdata = ( bool* )pUserData;
	*pdata	    = pResult->GetInteger( 0 );
}

COxDummyControls::
COxDummyControls( )
{
	m_DummyFly	      = false;
	m_DummyHook	      = false;
	m_DummyJump	      = false;
	m_DummyDirectionLeft  = false;
	m_DummyDirectionRight = false;

	m_DummyFire	      = 0;
	m_DummyHookTick	      = 0;
}

void
COxDummyControls::OnConsoleInit( )
{
	Console( )->Register( "+dummyjump", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyJump, "Dummy jump" );
	Console( )->Register( "+dummyfly", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyFly, "Dummy HF (HammerFly)" );
	Console( )->Register( "+dummyhook", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyHook, "Dummy HF (HammerFly)" );
	Console( )->Register( "+dummyleft", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyDirectionLeft, "Dummy move left" );
	Console( )->Register( "+dummyright", "", CFGFLAG_CLIENT, ConOxDummyFly, &m_DummyDirectionRight, "Dummy move right" );
}

void
COxDummyControls::OnInit( )
{
	m_pControls = &GameClient( )->m_Controls;
}

void
COxDummyControls::update_dummy_controll( CNetObj_PlayerInput* pDummy )
{
	pDummy->m_Fire	       = m_pClient->m_DummyInput.m_Fire;

	pDummy->m_TargetX      = m_pClient->m_DummyInput.m_TargetX;
	pDummy->m_TargetY      = m_pClient->m_DummyInput.m_TargetY;

	pDummy->m_WantedWeapon = m_pClient->m_DummyInput.m_WantedWeapon;

	pDummy->m_Hook	       = 0;
	pDummy->m_Direction    = 0;
	pDummy->m_Jump	       = 0;

	static vec2 main_tee_pos;
	static vec2 dummy_tee_pos;

	main_tee_pos  = m_pClient->m_LocalCharacterPos;
	dummy_tee_pos = m_pClient->m_aClients[ m_pClient->m_aLocalIds[ !g_Config.m_ClDummy ] ].m_Predicted.m_Pos;

	if ( m_DummyFly || ( m_DummyHook && g_Config.m_OxDummyAutoAimHook ) ) {
		const vec2 dir	  = main_tee_pos - dummy_tee_pos;
		pDummy->m_TargetX = dir.x;
		pDummy->m_TargetY = dir.y;
	}

	if ( m_DummyFly ) {
		if ( !m_DummyFire ) {
			if ( m_DummyFire ) return;
			++m_DummyFire;
			pDummy->m_WantedWeapon = m_pClient->m_DummyInput.m_WantedWeapon = WEAPON_HAMMER + 1;
			pDummy->m_Fire = m_pClient->m_DummyInput.m_Fire = ( pDummy->m_Fire + 1 ) | 1;
		}

		return;
	}

	if ( m_DummyHook ) {
		pDummy->m_Hook	       = 1;
		pDummy->m_WantedWeapon = m_pClient->m_DummyInput.m_WantedWeapon;
	}

	pDummy->m_Jump	    = m_DummyJump;

	pDummy->m_Direction = 0;
	if ( m_DummyDirectionLeft != m_DummyDirectionRight ) {
		if ( m_DummyDirectionLeft ) pDummy->m_Direction = -1;
		else if ( m_DummyDirectionRight ) pDummy->m_Direction = 1;
	}
}
void
COxDummyControls::update_dummy_auto_shoot_unfreze( )
{
	auto* pd    = &m_pClient->m_DummyInput;
	auto* teech = &m_pClient->m_aClients[ m_pClient->m_aLocalIds[ g_Config.m_ClDummy ] ];
	auto* dummy = &m_pClient->m_aClients[ m_pClient->m_aLocalIds[ !g_Config.m_ClDummy ] ];

	static vec2 main_tee_pos;
	static vec2 dummy_tee_pos;

	main_tee_pos  = m_pClient->m_LocalCharacterPos;
	dummy_tee_pos = dummy->m_Predicted.m_Pos;
	if ( teech->m_FreezeEnd ) {
		vec2 dir	   = main_tee_pos - dummy_tee_pos;
		pd->m_TargetX	   = dir.x;
		pd->m_TargetY	   = dir.y;
		pd->m_Fire	   = ( pd->m_Fire + 1 ) | 1;
		pd->m_WantedWeapon = WEAPON_LASER + 1;
	} else pd->m_Fire = ( pd->m_Fire + 1 ) & ~1;
}
