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
#define CWeaponK98 C_WeaponK98
#define CWeaponK98Scoped C_WeaponK98Scoped
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

	DECLARE_ACTTABLE();

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


acttable_t CWeaponGarand::m_acttable[] =
{
	{ ACT_DOD_STAND_AIM,					ACT_DOD_STAND_AIM_RIFLE,				false },
	{ ACT_DOD_CROUCH_AIM,					ACT_DOD_CROUCH_AIM_RIFLE,				false },
	{ ACT_DOD_CROUCHWALK_AIM,				ACT_DOD_CROUCHWALK_AIM_RIFLE,			false },
	{ ACT_DOD_WALK_AIM,						ACT_DOD_WALK_AIM_RIFLE,					false },
	{ ACT_DOD_RUN_AIM,						ACT_DOD_RUN_AIM_RIFLE,					false },
	{ ACT_PRONE_IDLE,						ACT_DOD_PRONE_AIM_RIFLE,				false },
	{ ACT_PRONE_FORWARD,					ACT_DOD_PRONEWALK_IDLE_RIFLE,			false },
	{ ACT_DOD_STAND_IDLE,					ACT_DOD_STAND_IDLE_RIFLE,				false },
	{ ACT_DOD_CROUCH_IDLE,					ACT_DOD_CROUCH_IDLE_RIFLE,				false },
	{ ACT_DOD_CROUCHWALK_IDLE,				ACT_DOD_CROUCHWALK_IDLE_RIFLE,			false },
	{ ACT_DOD_WALK_IDLE,					ACT_DOD_WALK_IDLE_RIFLE,				false },
	{ ACT_DOD_RUN_IDLE,						ACT_DOD_RUN_IDLE_RIFLE,					false },
	{ ACT_SPRINT,							ACT_DOD_SPRINT_IDLE_RIFLE,				false },

	{ ACT_DOD_IDLE_ZOOMED,					ACT_DOD_STAND_ZOOM_RIFLE,				false },
	{ ACT_DOD_CROUCH_ZOOMED,				ACT_DOD_CROUCH_ZOOM_RIFLE,				false },
	{ ACT_DOD_CROUCHWALK_ZOOMED,			ACT_DOD_CROUCHWALK_ZOOM_RIFLE,			false },
	{ ACT_DOD_WALK_ZOOMED,					ACT_DOD_WALK_ZOOM_RIFLE,				false },
	{ ACT_DOD_PRONE_ZOOMED,					ACT_DOD_PRONE_ZOOM_RIFLE,				false },
	{ ACT_DOD_PRONE_FORWARD_ZOOMED,			ACT_DOD_PRONE_ZOOM_FORWARD_RIFLE,		false },

	{ ACT_RANGE_ATTACK1,					ACT_DOD_PRIMARYATTACK_RIFLE,			false },
	{ ACT_DOD_PRIMARYATTACK_CROUCH,			ACT_DOD_PRIMARYATTACK_RIFLE,			false },
	{ ACT_DOD_PRIMARYATTACK_PRONE,			ACT_DOD_PRIMARYATTACK_PRONE_RIFLE,		false },

	{ ACT_RELOAD,							ACT_DOD_RELOAD_RIFLE,					false },
	{ ACT_DOD_RELOAD_CROUCH,				ACT_DOD_RELOAD_CROUCH_RIFLE,			false },
	{ ACT_DOD_RELOAD_PRONE,					ACT_DOD_RELOAD_PRONE_RIFLE,				false },

	{ ACT_DOD_HS_IDLE,						ACT_DOD_HS_IDLE_K98,					false },
	{ ACT_DOD_HS_CROUCH,					ACT_DOD_HS_CROUCH_K98,					false },
};

IMPLEMENT_ACTTABLE(CWeaponGarand);


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

//=============================================================================
//
// CWeaponK98
//
//=============================================================================

class CWeaponK98 : public CWeaponDODSBase
{
	DECLARE_CLASS(CWeaponK98, CWeaponDODSBase);

public:

	CWeaponK98(void);

	void	PrimaryAttack(void);

	virtual DODWeaponType GetDODWeaponType(void) const
	{
		return DOD_WEAPON_TYPE_RIFLE;
	}

	virtual float GetDODFireRate(void) const
	{
		return 1.1f;
	}

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	DECLARE_ACTTABLE();

private:

	CWeaponK98(const CWeaponK98&);
};


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponK98, DT_WeaponK98)

BEGIN_NETWORK_TABLE(CWeaponK98, DT_WeaponK98)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponK98)
END_PREDICTION_DATA()


LINK_ENTITY_TO_CLASS(weapon_k98, CWeaponK98);
PRECACHE_WEAPON_REGISTER(weapon_k98);


acttable_t CWeaponK98::m_acttable[] =
{
	{ ACT_DOD_STAND_AIM,					ACT_DOD_STAND_AIM_BOLT,					false },
	{ ACT_DOD_CROUCH_AIM,					ACT_DOD_CROUCH_AIM_BOLT,				false },
	{ ACT_DOD_CROUCHWALK_AIM,				ACT_DOD_CROUCHWALK_AIM_BOLT,			false },
	{ ACT_DOD_WALK_AIM,						ACT_DOD_WALK_AIM_BOLT,					false },
	{ ACT_DOD_RUN_AIM,						ACT_DOD_RUN_AIM_BOLT,					false },
	{ ACT_PRONE_IDLE,						ACT_DOD_PRONE_AIM_BOLT,					false },
	{ ACT_PRONE_FORWARD,					ACT_DOD_PRONEWALK_IDLE_BOLT,			false },
	{ ACT_DOD_STAND_IDLE,					ACT_DOD_STAND_IDLE_BOLT,				false },
	{ ACT_DOD_CROUCH_IDLE,					ACT_DOD_CROUCH_IDLE_BOLT,				false },
	{ ACT_DOD_CROUCHWALK_IDLE,				ACT_DOD_CROUCHWALK_IDLE_BOLT,			false },
	{ ACT_DOD_WALK_IDLE,					ACT_DOD_WALK_IDLE_BOLT,					false },
	{ ACT_DOD_RUN_IDLE,						ACT_DOD_RUN_IDLE_BOLT,					false },
	{ ACT_SPRINT,							ACT_DOD_SPRINT_IDLE_BOLT,				false },

	{ ACT_DOD_IDLE_ZOOMED,					ACT_DOD_STAND_ZOOM_BOLT,				false },
	{ ACT_DOD_CROUCH_ZOOMED,				ACT_DOD_CROUCH_ZOOM_BOLT,				false },
	{ ACT_DOD_CROUCHWALK_ZOOMED,			ACT_DOD_CROUCHWALK_ZOOM_BOLT,			false },
	{ ACT_DOD_WALK_ZOOMED,					ACT_DOD_WALK_ZOOM_BOLT,					false },
	{ ACT_DOD_PRONE_ZOOMED,					ACT_DOD_PRONE_ZOOM_BOLT,				false },
	{ ACT_DOD_PRONE_FORWARD_ZOOMED,			ACT_DOD_PRONE_ZOOM_FORWARD_BOLT,		false },

	{ ACT_RANGE_ATTACK1,					ACT_DOD_PRIMARYATTACK_BOLT,				false },
	{ ACT_DOD_PRIMARYATTACK_CROUCH,			ACT_DOD_PRIMARYATTACK_BOLT,				false },
	{ ACT_DOD_PRIMARYATTACK_PRONE,			ACT_DOD_PRIMARYATTACK_PRONE_BOLT,		false },

	{ ACT_RELOAD,							ACT_DOD_RELOAD_BOLT,					false },
	{ ACT_DOD_RELOAD_CROUCH,				ACT_DOD_RELOAD_CROUCH_BOLT,				false },
	{ ACT_DOD_RELOAD_PRONE,					ACT_DOD_RELOAD_PRONE_BOLT,				false },

	{ ACT_DOD_HS_IDLE,						ACT_DOD_HS_IDLE_K98,					false },
	{ ACT_DOD_HS_CROUCH,					ACT_DOD_HS_CROUCH_K98,					false },
};

IMPLEMENT_ACTTABLE(CWeaponK98);


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponK98::CWeaponK98(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponK98::PrimaryAttack(void)
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

	m_flNextPrimaryAttack = gpGlobals->curtime + GetDODFireRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + GetDODFireRate();

	m_iClip1--;

	FireDODBullet();
	ApplyDODRecoil();
}


//=============================================================================
//
// CWeaponK98Scoped
//
//=============================================================================

class CWeaponK98Scoped : public CWeaponDODSBase
{
	DECLARE_CLASS(CWeaponK98Scoped, CWeaponDODSBase);

public:

	CWeaponK98Scoped(void);

	void	PrimaryAttack(void);
	void	SecondaryAttack(void);

	virtual DODWeaponType GetDODWeaponType(void) const
	{
		return DOD_WEAPON_TYPE_SNIPER;
	}

	virtual float GetDODFireRate(void) const
	{
		return 1.1f;
	}

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	DECLARE_ACTTABLE();

private:

	CWeaponK98Scoped(const CWeaponK98Scoped&);
};


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponK98Scoped, DT_WeaponK98Scoped)

BEGIN_NETWORK_TABLE(CWeaponK98Scoped, DT_WeaponK98Scoped)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponK98Scoped)
END_PREDICTION_DATA()


LINK_ENTITY_TO_CLASS(weapon_k98_scoped, CWeaponK98Scoped);
PRECACHE_WEAPON_REGISTER(weapon_k98_scoped);


acttable_t CWeaponK98Scoped::m_acttable[] =
{
	{ ACT_DOD_STAND_AIM,					ACT_DOD_STAND_AIM_BOLT,					false },
	{ ACT_DOD_CROUCH_AIM,					ACT_DOD_CROUCH_AIM_BOLT,				false },
	{ ACT_DOD_CROUCHWALK_AIM,				ACT_DOD_CROUCHWALK_AIM_BOLT,			false },
	{ ACT_DOD_WALK_AIM,						ACT_DOD_WALK_AIM_BOLT,					false },
	{ ACT_DOD_RUN_AIM,						ACT_DOD_RUN_AIM_BOLT,					false },
	{ ACT_PRONE_IDLE,						ACT_DOD_PRONE_AIM_BOLT,					false },
	{ ACT_PRONE_FORWARD,					ACT_DOD_PRONEWALK_IDLE_BOLT,			false },
	{ ACT_DOD_STAND_IDLE,					ACT_DOD_STAND_IDLE_BOLT,				false },
	{ ACT_DOD_CROUCH_IDLE,					ACT_DOD_CROUCH_IDLE_BOLT,				false },
	{ ACT_DOD_CROUCHWALK_IDLE,				ACT_DOD_CROUCHWALK_IDLE_BOLT,			false },
	{ ACT_DOD_WALK_IDLE,					ACT_DOD_WALK_IDLE_BOLT,					false },
	{ ACT_DOD_RUN_IDLE,						ACT_DOD_RUN_IDLE_BOLT,					false },
	{ ACT_SPRINT,							ACT_DOD_SPRINT_IDLE_BOLT,				false },

	{ ACT_DOD_IDLE_ZOOMED,					ACT_DOD_STAND_ZOOM_BOLT,				false },
	{ ACT_DOD_CROUCH_ZOOMED,				ACT_DOD_CROUCH_ZOOM_BOLT,				false },
	{ ACT_DOD_CROUCHWALK_ZOOMED,			ACT_DOD_CROUCHWALK_ZOOM_BOLT,			false },
	{ ACT_DOD_WALK_ZOOMED,					ACT_DOD_WALK_ZOOM_BOLT,					false },
	{ ACT_DOD_PRONE_ZOOMED,					ACT_DOD_PRONE_ZOOM_BOLT,				false },
	{ ACT_DOD_PRONE_FORWARD_ZOOMED,			ACT_DOD_PRONE_ZOOM_FORWARD_BOLT,		false },

	{ ACT_RANGE_ATTACK1,					ACT_DOD_PRIMARYATTACK_BOLT,				false },
	{ ACT_DOD_PRIMARYATTACK_CROUCH,			ACT_DOD_PRIMARYATTACK_BOLT,				false },
	{ ACT_DOD_PRIMARYATTACK_PRONE,			ACT_DOD_PRIMARYATTACK_PRONE_BOLT,		false },

	{ ACT_RELOAD,							ACT_DOD_RELOAD_BOLT,					false },
	{ ACT_DOD_RELOAD_CROUCH,				ACT_DOD_RELOAD_CROUCH_BOLT,				false },
	{ ACT_DOD_RELOAD_PRONE,					ACT_DOD_RELOAD_PRONE_BOLT,				false },

	{ ACT_DOD_HS_IDLE,						ACT_DOD_HS_IDLE_K98,					false },
	{ ACT_DOD_HS_CROUCH,					ACT_DOD_HS_CROUCH_K98,					false },
};


IMPLEMENT_ACTTABLE(CWeaponK98Scoped);


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponK98Scoped::CWeaponK98Scoped(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponK98Scoped::PrimaryAttack(void)
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

	m_flNextPrimaryAttack = gpGlobals->curtime + GetDODFireRate();
	m_flNextSecondaryAttack = gpGlobals->curtime + GetDODFireRate();

	m_iClip1--;

	FireDODBullet();
	ApplyDODRecoil();
}


//-----------------------------------------------------------------------------
// Purpose: Scoped functionality will be added later.
//-----------------------------------------------------------------------------
void CWeaponK98Scoped::SecondaryAttack(void)
{
	BaseClass::SecondaryAttack();
}