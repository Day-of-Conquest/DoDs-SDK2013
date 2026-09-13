//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: File that contains all rifle type weapons for DoD:Source and DoC
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
#define CWeaponGarand C_WeaponGarand
#endif


//=============================================================================
//
// CWeaponGarand
//
//=============================================================================

class CWeaponGarand : public CWeaponDODSBase
{
	DECLARE_CLASS(CWeaponGarand, CWeaponDODSBase);

public:

	CWeaponGarand(void);

	void	PrimaryAttack(void);
	//void	SecondaryAttack(void);
	bool	Reload(void);

	virtual DODWeaponType GetDODWeaponType(void) const
	{
		return DOD_WEAPON_TYPE_RIFLE;
	}

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

private:

	CWeaponGarand(const CWeaponGarand&);
};


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponGarand, DT_WeaponGarand)

BEGIN_NETWORK_TABLE(CWeaponGarand, DT_WeaponGarand)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponGarand)
END_PREDICTION_DATA()


LINK_ENTITY_TO_CLASS(weapon_garand, CWeaponGarand);
PRECACHE_WEAPON_REGISTER(weapon_garand);


#ifndef CLIENT_DLL

acttable_t CWeaponGarand::m_acttable[] =
{
	{ ACT_HL2MP_IDLE,					ACT_HL2MP_IDLE_AR2,						false },
	{ ACT_HL2MP_RUN,					ACT_HL2MP_RUN_AR2,						false },
	{ ACT_HL2MP_IDLE_CROUCH,			ACT_HL2MP_IDLE_CROUCH_AR2,				false },
	{ ACT_HL2MP_WALK_CROUCH,			ACT_HL2MP_WALK_CROUCH_AR2,				false },
	{ ACT_HL2MP_GESTURE_RANGE_ATTACK,	ACT_HL2MP_GESTURE_RANGE_ATTACK_AR2,		false },
	{ ACT_HL2MP_GESTURE_RELOAD,			ACT_HL2MP_GESTURE_RELOAD_AR2,			false },
	{ ACT_HL2MP_JUMP,					ACT_HL2MP_JUMP_AR2,						false },
	{ ACT_RANGE_ATTACK1,				ACT_RANGE_ATTACK_AR2,					false },
};

IMPLEMENT_ACTTABLE(CWeaponGarand);

#endif


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponGarand::CWeaponGarand(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponGarand::PrimaryAttack(void)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return;

	if (m_iClip1 <= 0)
	{
		WeaponSound(EMPTY);
		m_flNextPrimaryAttack = gpGlobals->curtime + 0.2f;
		return;
	}

	WeaponSound(SINGLE);

	pPlayer->DoMuzzleFlash();

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = gpGlobals->curtime + 0.37f;
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.37f;

	m_iClip1--;

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	FireBulletsInfo_t info;

	info.m_iShots = 1;
	info.m_vecSrc = vecSrc;
	info.m_vecDirShooting = vecAiming;
	info.m_vecSpread = VECTOR_CONE_1DEGREES;
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_pAttacker = pPlayer;

	info.m_pAttacker = pPlayer;

	pPlayer->FireBullets(info);

	pPlayer->ViewPunch(
		QAngle(
			-2.0f,
			random->RandomFloat(-0.5f, 0.5f),
			0.0f
		)
	);

	if (m_iClip1 <= 0)
	{
		// Ping!!!
		WeaponSound(SPECIAL1);
	}


}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CWeaponGarand::Reload(void)
{
	// Garand cannot be reloaded until the current en-bloc clip is empty.
	if (m_iClip1 > 0)
	{
		CBasePlayer* pPlayer = ToBasePlayer(GetOwner());
		if (pPlayer)
		{
			pPlayer->HintMessage("#HINT_GARAND_RELOAD");
		}
		return false;
	}

	return BaseClass::Reload();
}