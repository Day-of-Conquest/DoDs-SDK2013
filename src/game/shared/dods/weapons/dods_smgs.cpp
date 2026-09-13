//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: File that contains all submachine gun type weapons for DoD:Source and DoC
//
//=============================================================================//

#include "cbase.h"
#include "npcevent.h"
#include "in_buttons.h"
#include "dods_weaponbase.h"

#ifdef CLIENT_DLL
#include "c_hl2mp_player.h"
#include <prediction.h>
#else
#include "hl2mp_player.h"
#endif


#ifdef CLIENT_DLL
#define CWeaponThompson C_WeaponThompson
#endif


//=============================================================================
//
// CWeaponThompson
//
//=============================================================================

class CWeaponThompson : public CWeaponDODSBase
{
	DECLARE_CLASS(CWeaponThompson, CWeaponDODSBase);

public:

	CWeaponThompson(void);

	void	PrimaryAttack(void);

	virtual DODWeaponType GetDODWeaponType(void) const
	{
		return DOD_WEAPON_TYPE_SMG;
	}

	virtual float GetDODFireRate(void) const
	{
		return 0.085f;
	}

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

private:

	CWeaponThompson(const CWeaponThompson&);
};


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponThompson, DT_WeaponThompson)

BEGIN_NETWORK_TABLE(CWeaponThompson, DT_WeaponThompson)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponThompson)
END_PREDICTION_DATA()


LINK_ENTITY_TO_CLASS(weapon_thompson, CWeaponThompson);
PRECACHE_WEAPON_REGISTER(weapon_thompson);


#ifndef CLIENT_DLL

acttable_t CWeaponThompson::m_acttable[] =
{
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_SMG1,					false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_SMG1,						false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_SMG1,				false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_SMG1,				false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_SMG1,	false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_SMG1,			false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_SMG1,					false },
	{ ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_SMG1,					false },
};

IMPLEMENT_ACTTABLE(CWeaponThompson);

#endif


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponThompson::CWeaponThompson(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponThompson::PrimaryAttack(void)
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return;

	if (m_iClip1 <= 0)
	{
		if (!m_bFireOnEmpty)
		{
			Reload();
		}
		else
		{
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = gpGlobals->curtime + 0.2f;
		}

		return;
	}

	WeaponSound(SINGLE);

	pPlayer->DoMuzzleFlash();

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = gpGlobals->curtime + GetDODFireRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + GetDODFireRate();

	m_iClip1--;

	FireDODBullet();
	ApplyDODRecoil();
}