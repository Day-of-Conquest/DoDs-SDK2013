//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Game movement for Half-Life 2: Deathmatch
//
//=============================================================================//

#include "cbase.h"
#include "hl2mp_gamemovement.h"
#include "in_buttons.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CHL2MPGameMovement::CHL2MPGameMovement()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CHL2MPGameMovement::CheckJumpButton( void )
{
	bool bJump = BaseClass::CheckJumpButton();

	CHL2MP_Player *pPlayer = GetHL2MPPlayer();

	if ( !pPlayer )
		return bJump;

	if ( bJump )
		pPlayer->DoAnimationEvent( PLAYERANIMEVENT_JUMP );

	return bJump;
}

#if defined( DODS_REMAKE ) && defined( HL2MP )
void CDODSGameMovement::Duck(void)
{
	CHL2MP_Player* pPlayer = static_cast<CHL2MP_Player*>(player);
	if (!player->IsAlive() || player->IsObserver() || player->GetMoveType() != MOVETYPE_WALK || player->GetWaterLevel() >= WL_Waist)
	{
		if (pPlayer->IsCrawling())
		{
			pPlayer->SetCrawling(false, true);
			if (player->IsAlive() && !player->IsObserver())
				player->SetViewOffset(GetPlayerViewOffset(player->m_Local.m_bDucked));
		}
		BaseClass::Duck();
		return;
	}

	const bool pressed = (mv->m_nButtons & IN_ALT1) && !(mv->m_nOldButtons & IN_ALT1);
	if (pressed && player->GetGroundEntity() != NULL)
	{
		if (pPlayer->IsCrawling())
		{
			if (CanUnduck())
			{
				pPlayer->SetCrawling(false, true);
				FinishUnDuck();
			}
		}
		else if (player->GetWaterLevel() < WL_Waist && !(player->GetFlags() & FL_ONTRAIN))
		{
			FinishDuck();
			player->m_Local.m_bDucking = false;
			player->m_Local.m_bInDuckJump = false;
			player->m_Local.m_flDucktime = 0;
			player->m_Local.m_flDuckJumpTime = 0;
			player->m_Local.m_flJumpTime = 0;
			pPlayer->SetCrawling(true, true);
		}
	}

	if (!pPlayer->IsCrawling())
	{
		BaseClass::Duck();
		return;
	}

	player->SetViewOffset(GetPlayerViewOffset(true) * 0.5f);
	mv->m_flForwardMove *= 0.25f;
	mv->m_flSideMove *= 0.25f;
	mv->m_flUpMove *= 0.25f;
}

bool CDODSGameMovement::CheckJumpButton(void)
{
	if (static_cast<CHL2MP_Player*>(player)->IsCrawling())
	{
		mv->m_nOldButtons |= IN_JUMP;
		return false;
	}
	return BaseClass::CheckJumpButton();
}

bool CDODSGameMovement::LadderMove(void)
{
	if (static_cast<CHL2MP_Player*>(player)->IsCrawling())
		return false;
	return BaseClass::LadderMove();
}
#endif

#ifdef DODS_REMAKE
	CDODSGameMovement g_GameMovement;
#else
static CHL2MPGameMovement g_GameMovement;
#endif
IGameMovement *g_pGameMovement = (IGameMovement *)&g_GameMovement;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGameMovement, IGameMovement, INTERFACENAME_GAMEMOVEMENT, g_GameMovement );
