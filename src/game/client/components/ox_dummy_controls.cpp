//
// Created by olokreZ on 01.04.2024.
//

#include "ox_dummy_controls.h"

#include <engine/shared/config.h>
#include <game/client/components/controls.h>
#include <game/client/gameclient.h>

COxDummyControls::COxDummyControls( )
{
	m_ControlFlag = 0;
	// m_DummyFly	      = false;
	// m_DummyHook	      = false;
	// m_DummyJump	      = false;
	// m_DummyDirectionLeft  = false;
	// m_DummyDirectionRight = false;
	//
	// m_DummyFire = 0;
}

static int  GetBit( int bit, uint8_t flag ) { return ( flag & ( 1 << bit ) ) != 0 ? 1 : 0; }
static void SetBit( int bit, uint8_t* flag ) { *flag |= 1 << bit; }
static void UnsetBit( int bit, uint8_t* flag ) { *flag &= ~( 1 << bit ); }

struct DummyControlState_t
{
	uint8_t* pData;
	int	 bit;
};

static void ConDummyControl( IConsole::IResult* pResult, void* pUserData )
{
	DummyControlState_t* pdata = static_cast< DummyControlState_t* >( pUserData );
	if ( pResult->GetInteger( 0 ) ) SetBit( pdata->bit, pdata->pData );
	else UnsetBit( pdata->bit, pdata->pData );
}

void COxDummyControls::OnConsoleInit( )
{
	{
		static DummyControlState_t sState = { &m_ControlFlag, CONTROL_BIT_FLY };
		Console( )->Register( "+dummyfly", "", CFGFLAG_CLIENT, ConDummyControl, &sState, "D HF" );
	}
	{
		static DummyControlState_t sState = { &m_ControlFlag, CONTROL_BIT_JUMP };
		Console( )->Register( "+dummyjump", "", CFGFLAG_CLIENT, ConDummyControl, &sState, "D jump" );
	}
	{
		static DummyControlState_t sState = { &m_ControlFlag, CONTROL_BIT_HOOK };
		Console( )->Register( "+dummyhook", "", CFGFLAG_CLIENT, ConDummyControl, &sState, "D HH" );
	}
	{
		static DummyControlState_t sState = { &m_ControlFlag, CONTROL_BIT_DIR_LEFT };
		Console( )->Register( "+dummyleft", "", CFGFLAG_CLIENT, ConDummyControl, &sState, "D move left" );
	}
	{
		static DummyControlState_t sState = { &m_ControlFlag, CONTROL_BIT_DIR_RIGHT };
		Console( )->Register( "+dummyright", "", CFGFLAG_CLIENT, ConDummyControl, &sState, "D move right" );
	}
}

void COxDummyControls::OnInit( ) { m_pControls = &GameClient( )->m_Controls; }

void COxDummyControls::update_dummy_controll( CNetObj_PlayerInput* pDummy )
{
	pDummy->m_Fire = m_pClient->m_DummyInput.m_Fire;

	pDummy->m_TargetX = m_pClient->m_DummyInput.m_TargetX;
	pDummy->m_TargetY = m_pClient->m_DummyInput.m_TargetY;

	pDummy->m_WantedWeapon = m_pClient->m_DummyInput.m_WantedWeapon;

	const vec2 main_tee_pos	 = m_pClient->m_LocalCharacterPos;
	const vec2 dummy_tee_pos = m_pClient->m_aClients[ m_pClient->m_aLocalIds[ !g_Config.m_ClDummy ] ].m_Predicted.m_Pos;

	const auto local_id = m_pClient->m_aLocalIds[ g_Config.m_ClDummy ];
	const auto dummy_id = m_pClient->m_aLocalIds[ !g_Config.m_ClDummy ];

	auto* local_player = &m_pClient->m_aClients[ local_id ];
	auto* local_dummy  = &m_pClient->m_aClients[ dummy_id ];

	if ( GetBit( CONTROL_BIT_FLY, m_ControlFlag ) || ( GetBit( CONTROL_BIT_HOOK, m_ControlFlag ) && g_Config.m_OxDummyAutoAimHook ) )
	{
		if ( GetBit( CONTROL_BIT_FLY, m_ControlFlag ) && m_DummyFire && !local_dummy->m_LiveFrozen ) goto skipAim;
		const vec2 d	  = main_tee_pos - dummy_tee_pos;
		pDummy->m_TargetX = d.x;
		pDummy->m_TargetY = d.y;
	}

skipAim:

	if ( g_Config.m_ClDummyCopyMoves ) goto skipMove;

	pDummy->m_Hook = 0;
	if ( GetBit( CONTROL_BIT_HOOK, m_ControlFlag ) || m_pClient->m_DummyInput.m_Hook )
	{
		pDummy->m_Hook	       = 1;
		pDummy->m_WantedWeapon = m_pClient->m_DummyInput.m_WantedWeapon;
	}

	pDummy->m_Jump	    = GetBit( CONTROL_BIT_JUMP, m_ControlFlag );
	pDummy->m_Direction = 0;
	if ( GetBit( CONTROL_BIT_DIR_LEFT, m_ControlFlag ) != GetBit( CONTROL_BIT_DIR_RIGHT, m_ControlFlag ) )
	{
		if ( GetBit( CONTROL_BIT_DIR_LEFT, m_ControlFlag ) ) pDummy->m_Direction = -1;
		else if ( GetBit( CONTROL_BIT_DIR_RIGHT, m_ControlFlag ) ) pDummy->m_Direction = 1;
	}

skipMove:

	if ( GetBit( CONTROL_BIT_FLY, m_ControlFlag ) )
	{
		if ( !m_DummyFire )
		{
			++m_DummyFire;
			pDummy->m_WantedWeapon = m_pClient->m_DummyInput.m_WantedWeapon = WEAPON_HAMMER + 1;
			pDummy->m_Fire = m_pClient->m_DummyInput.m_Fire = ( pDummy->m_Fire + 1 ) | 1;
		}
	}
	else { m_DummyFire = 0; }

	if ( g_Config.m_OxDummyZombi )
	{
		float f = random_float( );
		int   r = *reinterpret_cast< int* >( &f );

		//
		const auto local_id = m_pClient->m_aLocalIds[ g_Config.m_ClDummy ];
		const auto dummy_id = m_pClient->m_aLocalIds[ !g_Config.m_ClDummy ];

		auto* local_player = &m_pClient->m_aClients[ local_id ];
		auto* local_dummy  = &m_pClient->m_aClients[ dummy_id ];

		const auto local_tee_pos = local_player->m_Predicted.m_Pos;
		const auto dummy_tee_pos = local_dummy->m_Predicted.m_Pos;

		int reverese = ( r % 3 == 0 ? -1 : 1 );

		if ( dummy_tee_pos.x > local_tee_pos.x ) pDummy->m_Direction = -1 * reverese;
		else if ( dummy_tee_pos.x < local_tee_pos.x ) pDummy->m_Direction = 1 * reverese;
		else pDummy->m_Direction = 0;
		if ( r % 31 == 0 ) pDummy->m_Jump = true;
	}
}

void COxDummyControls::update_dummy_auto_shoot_unfreze( )
{
	auto* pd    = &m_pClient->m_DummyInput;
	auto* teech = &m_pClient->m_aClients[ m_pClient->m_aLocalIds[ g_Config.m_ClDummy ] ];
	auto* dummy = &m_pClient->m_aClients[ m_pClient->m_aLocalIds[ !g_Config.m_ClDummy ] ];

	static vec2 main_tee_pos;
	static vec2 dummy_tee_pos;

	main_tee_pos  = m_pClient->m_LocalCharacterPos;
	dummy_tee_pos = dummy->m_Predicted.m_Pos;
	if ( teech->m_FreezeEnd )
	{
		vec2 dir	   = main_tee_pos - dummy_tee_pos;
		pd->m_TargetX	   = dir.x;
		pd->m_TargetY	   = dir.y;
		pd->m_Fire	   = ( pd->m_Fire + 1 ) | 1;
		pd->m_WantedWeapon = WEAPON_LASER + 1;
	}
	else pd->m_Fire = ( pd->m_Fire + 1 ) & ~1;
}

bool COxDummyControls::TakeControllDummy( ) const
{
	return m_ControlFlag || g_Config.m_OxDummyZombi;
	// return m_DummyFly || m_DummyHook || m_DummyJump || m_DummyDirectionLeft || m_DummyDirectionRight;
}
