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
#define CWeaponMP40 C_WeaponMP40
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

	DECLARE_ACTTABLE();

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


acttable_t CWeaponThompson::m_acttable[] = 
{
	{ ACT_DOD_STAND_AIM,					ACT_DOD_STAND_AIM_TOMMY,				false },
	{ ACT_DOD_CROUCH_AIM,					ACT_DOD_CROUCH_AIM_TOMMY,				false },
	{ ACT_DOD_CROUCHWALK_AIM,				ACT_DOD_CROUCHWALK_AIM_TOMMY,			false },
	{ ACT_DOD_WALK_AIM,						ACT_DOD_WALK_AIM_TOMMY,					false },
	{ ACT_DOD_RUN_AIM,						ACT_DOD_RUN_AIM_TOMMY,					false },
	{ ACT_PRONE_IDLE,						ACT_DOD_PRONE_AIM_TOMMY,				false },
	{ ACT_PRONE_FORWARD,					ACT_DOD_PRONEWALK_IDLE_TOMMY,			false },
	{ ACT_DOD_STAND_IDLE,					ACT_DOD_STAND_IDLE_TOMMY,				false },
	{ ACT_DOD_CROUCH_IDLE,					ACT_DOD_CROUCH_IDLE_TOMMY,				false },
	{ ACT_DOD_CROUCHWALK_IDLE,				ACT_DOD_CROUCHWALK_IDLE_TOMMY,			false },
	{ ACT_DOD_WALK_IDLE,					ACT_DOD_WALK_IDLE_TOMMY,				false },
	{ ACT_DOD_RUN_IDLE,						ACT_DOD_RUN_IDLE_TOMMY,					false },
	{ ACT_SPRINT,							ACT_DOD_SPRINT_IDLE_TOMMY,				false },

	{ ACT_RANGE_ATTACK1,					ACT_DOD_PRIMARYATTACK_TOMMY,			false },
	{ ACT_DOD_PRIMARYATTACK_CROUCH,			ACT_DOD_PRIMARYATTACK_TOMMY,			false },
	{ ACT_DOD_PRIMARYATTACK_PRONE,			ACT_DOD_PRIMARYATTACK_PRONE_TOMMY,		false },
	{ ACT_RANGE_ATTACK2,					ACT_DOD_SECONDARYATTACK_TOMMY,			false },
	{ ACT_DOD_SECONDARYATTACK_CROUCH,		ACT_DOD_SECONDARYATTACK_CROUCH_TOMMY,	false },
	{ ACT_DOD_SECONDARYATTACK_PRONE,		ACT_DOD_SECONDARYATTACK_PRONE_TOMMY,	false },

	{ ACT_RELOAD,							ACT_DOD_RELOAD_TOMMY,					false },
	{ ACT_DOD_RELOAD_CROUCH,				ACT_DOD_RELOAD_CROUCH_TOMMY,			false },
	{ ACT_DOD_RELOAD_PRONE,					ACT_DOD_RELOAD_PRONE_TOMMY,				false },

	{ ACT_DOD_HS_IDLE,						ACT_DOD_HS_IDLE_TOMMY,				false },
	{ ACT_DOD_HS_CROUCH,					ACT_DOD_HS_CROUCH_TOMMY,			false },
};

IMPLEMENT_ACTTABLE(CWeaponThompson);



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

//=============================================================================
//
// CWeaponMP40
//
//=============================================================================

class CWeaponMP40 : public CWeaponDODSBase
{
	DECLARE_CLASS(CWeaponMP40, CWeaponDODSBase);

public:

	CWeaponMP40(void);

	void	PrimaryAttack(void);

	virtual DODWeaponType GetDODWeaponType(void) const
	{
		return DOD_WEAPON_TYPE_SMG;
	}

	virtual float GetDODFireRate(void) const
	{
		return 0.12f;
	}

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	DECLARE_ACTTABLE();

private:

	CWeaponMP40(const CWeaponMP40&);
};


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponMP40, DT_WeaponMP40)

BEGIN_NETWORK_TABLE(CWeaponMP40, DT_WeaponMP40)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponMP40)
END_PREDICTION_DATA()


LINK_ENTITY_TO_CLASS(weapon_mp40, CWeaponMP40);
PRECACHE_WEAPON_REGISTER(weapon_mp40);


acttable_t CWeaponMP40::m_acttable[] =
{
	{ ACT_DOD_STAND_AIM,					ACT_DOD_STAND_AIM_MP40,					false },
	{ ACT_DOD_CROUCH_AIM,					ACT_DOD_CROUCH_AIM_MP40,				false },
	{ ACT_DOD_CROUCHWALK_AIM,				ACT_DOD_CROUCHWALK_AIM_MP40,			false },
	{ ACT_DOD_WALK_AIM,						ACT_DOD_WALK_AIM_MP40,					false },
	{ ACT_DOD_RUN_AIM,						ACT_DOD_RUN_AIM_MP40,					false },
	{ ACT_PRONE_IDLE,						ACT_DOD_PRONE_AIM_MP40,					false },
	{ ACT_PRONE_FORWARD,					ACT_DOD_PRONEWALK_IDLE_MP40,			false },
	{ ACT_DOD_STAND_IDLE,					ACT_DOD_STAND_IDLE_MP40,				false },
	{ ACT_DOD_CROUCH_IDLE,					ACT_DOD_CROUCH_IDLE_MP40,				false },
	{ ACT_DOD_CROUCHWALK_IDLE,				ACT_DOD_CROUCHWALK_IDLE_MP40,			false },
	{ ACT_DOD_WALK_IDLE,					ACT_DOD_WALK_IDLE_MP40,					false },
	{ ACT_DOD_RUN_IDLE,						ACT_DOD_RUN_IDLE_MP40,					false },
	{ ACT_SPRINT,							ACT_DOD_SPRINT_IDLE_MP40,				false },

	{ ACT_RANGE_ATTACK1,					ACT_DOD_PRIMARYATTACK_MP40,			false },
	{ ACT_DOD_PRIMARYATTACK_CROUCH,			ACT_DOD_PRIMARYATTACK_MP40,			false },
	{ ACT_DOD_PRIMARYATTACK_PRONE,			ACT_DOD_PRIMARYATTACK_PRONE_MP40,	false },
	{ ACT_RANGE_ATTACK2,					ACT_DOD_SECONDARYATTACK_MP40,		false },
	{ ACT_DOD_SECONDARYATTACK_CROUCH,		ACT_DOD_SECONDARYATTACK_CROUCH_MP40,false },
	{ ACT_DOD_SECONDARYATTACK_PRONE,		ACT_DOD_SECONDARYATTACK_PRONE_MP40,	false },

	{ ACT_RELOAD,							ACT_DOD_RELOAD_MP40,				false },
	{ ACT_DOD_RELOAD_CROUCH,				ACT_DOD_RELOAD_CROUCH_MP40,			false },
	{ ACT_DOD_RELOAD_PRONE,					ACT_DOD_RELOAD_PRONE_MP40,			false },

	{ ACT_DOD_HS_IDLE,						ACT_DOD_HS_IDLE_MP44,				false },
	{ ACT_DOD_HS_CROUCH,					ACT_DOD_HS_CROUCH_MP44,				false },
};

IMPLEMENT_ACTTABLE(CWeaponMP40);



//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponMP40::CWeaponMP40(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponMP40::PrimaryAttack(void)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

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