//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: File that contains all pistol type weapons for DoD:Source and DoC
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


#define PISTOL_FASTEST_REFIRE_TIME				0.1f
#define PISTOL_FASTEST_DRY_REFIRE_TIME			0.2f

#define PISTOL_ACCURACY_SHOT_PENALTY_TIME		0.2f
#define PISTOL_ACCURACY_MAXIMUM_PENALTY_TIME	1.5f


#ifdef CLIENT_DLL
#define CWeaponRevolver C_WeaponRevolver
#define CWeaponP38 C_WeaponP38
#define CWeaponColt C_WeaponColt
#endif


//=============================================================================
//
// CWeaponRevolver
//
//=============================================================================

class CWeaponRevolver : public CWeaponDODSBase
{
	DECLARE_CLASS(CWeaponRevolver, CWeaponDODSBase);

public:

	CWeaponRevolver(void);

	void	PrimaryAttack(void);

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	virtual DODWeaponType GetDODWeaponType(void) const
	{
		return DOD_WEAPON_TYPE_PISTOL;
	}

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

private:

	CWeaponRevolver(const CWeaponRevolver&);
};


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponRevolver, DT_WeaponRevolver)

BEGIN_NETWORK_TABLE(CWeaponRevolver, DT_WeaponRevolver)
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA(CWeaponRevolver)
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS(weapon_revolver, CWeaponRevolver);
PRECACHE_WEAPON_REGISTER(weapon_revolver);


#ifndef CLIENT_DLL

acttable_t CWeaponRevolver::m_acttable[] =
{
	{ ACT_DOD_STAND_AIM,					ACT_DOD_STAND_AIM_PISTOL,				false },
	{ ACT_DOD_CROUCH_AIM,					ACT_DOD_CROUCH_AIM_PISTOL,				false },
	{ ACT_DOD_CROUCHWALK_AIM,				ACT_DOD_CROUCHWALK_AIM_PISTOL,			false },
	{ ACT_DOD_WALK_AIM,						ACT_DOD_WALK_AIM_PISTOL,				false },
	{ ACT_DOD_RUN_AIM,						ACT_DOD_RUN_AIM_PISTOL,					false },
	{ ACT_PRONE_IDLE,						ACT_DOD_PRONE_AIM_PISTOL,				false },
	{ ACT_PRONE_FORWARD,					ACT_DOD_PRONEWALK_IDLE_PISTOL,			false },
	{ ACT_DOD_STAND_IDLE,					ACT_DOD_STAND_IDLE_PISTOL,				false },
	{ ACT_DOD_CROUCH_IDLE,					ACT_DOD_CROUCH_IDLE_PISTOL,				false },
	{ ACT_DOD_CROUCHWALK_IDLE,				ACT_DOD_CROUCHWALK_IDLE_PISTOL,			false },
	{ ACT_DOD_WALK_IDLE,					ACT_DOD_WALK_IDLE_PISTOL,				false },
	{ ACT_DOD_RUN_IDLE,						ACT_DOD_RUN_IDLE_PISTOL,				false },
	{ ACT_SPRINT,							ACT_DOD_SPRINT_IDLE_PISTOL,				false },

	{ ACT_RANGE_ATTACK1,					ACT_DOD_PRIMARYATTACK_PISTOL,			false },
	{ ACT_DOD_PRIMARYATTACK_CROUCH,			ACT_DOD_PRIMARYATTACK_PISTOL,			false },
	{ ACT_DOD_PRIMARYATTACK_PRONE,			ACT_DOD_PRIMARYATTACK_PRONE_PISTOL,		false },

	{ ACT_RELOAD,							ACT_DOD_RELOAD_PISTOL,					false },
	{ ACT_DOD_RELOAD_CROUCH,				ACT_DOD_RELOAD_CROUCH_PISTOL,			false },
	{ ACT_DOD_RELOAD_PRONE,					ACT_DOD_RELOAD_PRONE_PISTOL,			false },

	// Hand Signals
	{ ACT_DOD_HS_IDLE,						ACT_DOD_HS_IDLE_PISTOL,					false },
	{ ACT_DOD_HS_CROUCH,					ACT_DOD_HS_CROUCH_PISTOL,				false },
};

IMPLEMENT_ACTTABLE(CWeaponRevolver);

#endif


//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponRevolver::CWeaponRevolver(void)
{
	m_bReloadsSingly = false;
	m_bFiresUnderwater = false;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponRevolver::PrimaryAttack(void)
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
	{
		return;
	}

	if (m_iClip1 <= 0)
	{
		if (!m_bFireOnEmpty)
		{
			Reload();
		}
		else
		{
			WeaponSound(EMPTY);
			m_flNextPrimaryAttack = gpGlobals->curtime + 0.15f;
		}

		return;
	}

	WeaponSound(SINGLE);
	pPlayer->DoMuzzleFlash();

	SendWeaponAnim(ACT_VM_PRIMARYATTACK);
	pPlayer->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = gpGlobals->curtime + 0.75f;
	m_flNextSecondaryAttack = gpGlobals->curtime + 0.75f;

	m_iClip1--;

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	FireBulletsInfo_t info(1, vecSrc, vecAiming, vec3_origin, MAX_TRACE_LENGTH, m_iPrimaryAmmoType);
	info.m_pAttacker = pPlayer;

	pPlayer->FireBullets(info);

#ifdef CLIENT_DLL

	if (prediction->IsFirstTimePredicted())
	{
		QAngle angles;

		engine->GetViewAngles(angles);

		angles.x += random->RandomInt(-1, 1);
		angles.y += random->RandomInt(-1, 1);
		angles.z += 0.0f;

		engine->SetViewAngles(angles);
	}

#endif

	pPlayer->ViewPunch(QAngle(-8.0f, random->RandomFloat(-2.0f, 2.0f), 0.0f));

	if (!m_iClip1 && pPlayer->GetAmmoCount(m_iPrimaryAmmoType) <= 0)
	{
		pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
	}
}


//=============================================================================
//
// CWeaponP38
//
//=============================================================================

class CWeaponP38 : public CWeaponDODSBase
{
public:
	DECLARE_CLASS(CWeaponP38, CWeaponDODSBase);

	CWeaponP38(void);

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	void	Precache(void);
	void	ItemPostFrame(void);
	void	ItemPreFrame(void);
	void	ItemBusyFrame(void);
	void	PrimaryAttack(void);
	void	AddViewKick(void);
	void	DryFire(void);

	void	UpdatePenaltyTime(void);

	Activity GetIdleActivity(void);
	Activity GetPrimaryAttackActivity(void);
	Activity GetDrawActivity(void);

	virtual bool Reload(void);

	virtual DODWeaponType GetDODWeaponType(void) const
	{
		return DOD_WEAPON_TYPE_PISTOL;
	}

	virtual const Vector& GetBulletSpread(void)
	{
		static Vector cone;

		float ramp = RemapValClamped(
			m_flAccuracyPenalty,
			0.0f,
			PISTOL_ACCURACY_MAXIMUM_PENALTY_TIME,
			0.0f,
			1.0f);

		VectorLerp(VECTOR_CONE_1DEGREES, VECTOR_CONE_6DEGREES, ramp, cone);

		return cone;
	}

	virtual int GetMinBurst()
	{
		return 1;
	}

	virtual int GetMaxBurst()
	{
		return 3;
	}

	virtual float GetFireRate(void)
	{
		return 0.5f;
	}

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

private:

	CNetworkVar(float, m_flSoonestPrimaryAttack);
	CNetworkVar(float, m_flLastAttackTime);
	CNetworkVar(float, m_flAccuracyPenalty);
	CNetworkVar(int, m_nNumShotsFired);

private:

	CWeaponP38(const CWeaponP38&);
};


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponP38, DT_WeaponP38)

BEGIN_NETWORK_TABLE(CWeaponP38, DT_WeaponP38)

#ifdef CLIENT_DLL

	RecvPropTime(RECVINFO(m_flSoonestPrimaryAttack)),
	RecvPropTime(RECVINFO(m_flLastAttackTime)),
	RecvPropFloat(RECVINFO(m_flAccuracyPenalty)),
	RecvPropInt(RECVINFO(m_nNumShotsFired)),

#else

	SendPropTime(SENDINFO(m_flSoonestPrimaryAttack)),
	SendPropTime(SENDINFO(m_flLastAttackTime)),
	SendPropFloat(SENDINFO(m_flAccuracyPenalty)),
	SendPropInt(SENDINFO(m_nNumShotsFired)),

#endif

END_NETWORK_TABLE()


#ifdef CLIENT_DLL

BEGIN_PREDICTION_DATA(CWeaponP38)

	DEFINE_PRED_FIELD(m_flSoonestPrimaryAttack, FIELD_FLOAT, FTYPEDESC_INSENDTABLE),
	DEFINE_PRED_FIELD(m_flLastAttackTime, FIELD_FLOAT, FTYPEDESC_INSENDTABLE),
	DEFINE_PRED_FIELD(m_flAccuracyPenalty, FIELD_FLOAT, FTYPEDESC_INSENDTABLE),
	DEFINE_PRED_FIELD(m_nNumShotsFired, FIELD_INTEGER, FTYPEDESC_INSENDTABLE),

END_PREDICTION_DATA()

#endif


LINK_ENTITY_TO_CLASS(weapon_p38, CWeaponP38);
PRECACHE_WEAPON_REGISTER(weapon_p38);


#ifndef CLIENT_DLL

acttable_t CWeaponP38::m_acttable[] =
{
	{ ACT_DOD_STAND_AIM,					ACT_DOD_STAND_AIM_PISTOL,				false },
	{ ACT_DOD_CROUCH_AIM,					ACT_DOD_CROUCH_AIM_PISTOL,				false },
	{ ACT_DOD_CROUCHWALK_AIM,				ACT_DOD_CROUCHWALK_AIM_PISTOL,			false },
	{ ACT_DOD_WALK_AIM,						ACT_DOD_WALK_AIM_PISTOL,				false },
	{ ACT_DOD_RUN_AIM,						ACT_DOD_RUN_AIM_PISTOL,					false },
	{ ACT_PRONE_IDLE,						ACT_DOD_PRONE_AIM_PISTOL,				false },
	{ ACT_PRONE_FORWARD,					ACT_DOD_PRONEWALK_IDLE_PISTOL,			false },
	{ ACT_DOD_STAND_IDLE,					ACT_DOD_STAND_IDLE_PISTOL,				false },
	{ ACT_DOD_CROUCH_IDLE,					ACT_DOD_CROUCH_IDLE_PISTOL,				false },
	{ ACT_DOD_CROUCHWALK_IDLE,				ACT_DOD_CROUCHWALK_IDLE_PISTOL,			false },
	{ ACT_DOD_WALK_IDLE,					ACT_DOD_WALK_IDLE_PISTOL,				false },
	{ ACT_DOD_RUN_IDLE,						ACT_DOD_RUN_IDLE_PISTOL,				false },
	{ ACT_SPRINT,							ACT_DOD_SPRINT_IDLE_PISTOL,				false },

	{ ACT_RANGE_ATTACK1,					ACT_DOD_PRIMARYATTACK_PISTOL,			false },
	{ ACT_DOD_PRIMARYATTACK_CROUCH,			ACT_DOD_PRIMARYATTACK_PISTOL,			false },
	{ ACT_DOD_PRIMARYATTACK_PRONE,			ACT_DOD_PRIMARYATTACK_PRONE_PISTOL,		false },

	{ ACT_RELOAD,							ACT_DOD_RELOAD_PISTOL,					false },
	{ ACT_DOD_RELOAD_CROUCH,				ACT_DOD_RELOAD_CROUCH_PISTOL,			false },
	{ ACT_DOD_RELOAD_PRONE,					ACT_DOD_RELOAD_PRONE_PISTOL,			false },

	// Hand Signals
	{ ACT_DOD_HS_IDLE,						ACT_DOD_HS_IDLE_PISTOL,					false },
	{ ACT_DOD_HS_CROUCH,					ACT_DOD_HS_CROUCH_PISTOL,				false },
};

IMPLEMENT_ACTTABLE(CWeaponP38);

#endif

Activity CWeaponP38::GetIdleActivity(void)
{
	Activity actIdle;

	if (m_iClip1 <= 0)
		actIdle = ACT_VM_IDLE_EMPTY;
	else
		actIdle = ACT_VM_IDLE;

	return actIdle;
}

Activity CWeaponP38::GetPrimaryAttackActivity(void)
{
	Activity actPrim;

	if (m_iClip1 <= 0)
		actPrim = ACT_VM_PRIMARYATTACK_EMPTY;
	else
		actPrim = ACT_VM_PRIMARYATTACK;

	return actPrim;
}

Activity CWeaponP38::GetDrawActivity(void)
{
	Activity actDraw;

	if (m_iClip1 <= 0)
		actDraw = ACT_VM_DRAW_EMPTY;
	else
		actDraw = ACT_VM_DRAW;

	return actDraw;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponP38::CWeaponP38(void)
{
	m_flSoonestPrimaryAttack = gpGlobals->curtime;
	m_flLastAttackTime = gpGlobals->curtime;
	m_flAccuracyPenalty = 0.0f;
	m_nNumShotsFired = 0;

	m_fMinRange1 = 24;
	m_fMaxRange1 = 1500;
	m_fMinRange2 = 24;
	m_fMaxRange2 = 200;

	m_bReloadsSingly = false;
	m_bFiresUnderwater = true;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponP38::Precache(void)
{
	BaseClass::Precache();
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponP38::DryFire(void)
{
	WeaponSound(EMPTY);
	SendWeaponAnim(ACT_VM_DRYFIRE);

	m_flSoonestPrimaryAttack = gpGlobals->curtime + PISTOL_FASTEST_DRY_REFIRE_TIME;
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponP38::PrimaryAttack(void)
{
	if ((gpGlobals->curtime - m_flLastAttackTime) > 0.5f)
	{
		m_nNumShotsFired = 0;
	}
	else
	{
		m_nNumShotsFired++;
	}

	m_flLastAttackTime = gpGlobals->curtime;
	m_flSoonestPrimaryAttack = gpGlobals->curtime + PISTOL_FASTEST_REFIRE_TIME;

	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	if (pOwner)
	{
		pOwner->ViewPunchReset();
	}

	BaseClass::PrimaryAttack();

	m_flAccuracyPenalty += PISTOL_ACCURACY_SHOT_PENALTY_TIME;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponP38::UpdatePenaltyTime(void)
{
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	if (pOwner == NULL)
		return;

	if (((pOwner->m_nButtons & IN_ATTACK) == false) &&
		(m_flSoonestPrimaryAttack < gpGlobals->curtime))
	{
		m_flAccuracyPenalty -= gpGlobals->frametime;
		m_flAccuracyPenalty = clamp(
			m_flAccuracyPenalty,
			0.0f,
			PISTOL_ACCURACY_MAXIMUM_PENALTY_TIME);
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponP38::ItemPreFrame(void)
{
	UpdatePenaltyTime();

	BaseClass::ItemPreFrame();
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponP38::ItemBusyFrame(void)
{
	UpdatePenaltyTime();

	BaseClass::ItemBusyFrame();
}


//-----------------------------------------------------------------------------
// Purpose: Allows firing as fast as button is pressed
//-----------------------------------------------------------------------------
void CWeaponP38::ItemPostFrame(void)
{
	BaseClass::ItemPostFrame();

	if (m_bInReload)
		return;

	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	if (pOwner == NULL)
		return;

	if (pOwner->m_nButtons & IN_ATTACK2)
	{
		m_flLastAttackTime = gpGlobals->curtime + PISTOL_FASTEST_REFIRE_TIME;
		m_flSoonestPrimaryAttack = gpGlobals->curtime + PISTOL_FASTEST_REFIRE_TIME;
		m_flNextPrimaryAttack = gpGlobals->curtime + PISTOL_FASTEST_REFIRE_TIME;
	}

	if (((pOwner->m_nButtons & IN_ATTACK) == false) &&
		(m_flSoonestPrimaryAttack < gpGlobals->curtime))
	{
		m_flNextPrimaryAttack = gpGlobals->curtime - 0.1f;
	}
	else if ((pOwner->m_nButtons & IN_ATTACK) &&
		(m_flNextPrimaryAttack < gpGlobals->curtime) &&
		(m_iClip1 <= 0))
	{
		DryFire();
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CWeaponP38::Reload(void)
{
	bool fRet = DefaultReload(GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD);

	if (fRet)
	{
		WeaponSound(RELOAD);
		m_flAccuracyPenalty = 0.0f;
	}

	return fRet;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponP38::AddViewKick(void)
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	QAngle viewPunch;

	viewPunch.x = SharedRandomFloat("p38pax", 0.25f, 0.5f);
	viewPunch.y = SharedRandomFloat("p38pay", -0.6f, 0.6f);
	viewPunch.z = 0.0f;

	pPlayer->ViewPunch(viewPunch);
}


//=============================================================================
//
// CWeaponColt
//
//=============================================================================

class CWeaponColt : public CWeaponDODSBase
{
public:
	DECLARE_CLASS(CWeaponColt, CWeaponDODSBase);

	CWeaponColt(void);

	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	void	Precache(void);
	void	ItemPostFrame(void);
	void	ItemPreFrame(void);
	void	ItemBusyFrame(void);
	void	PrimaryAttack(void);
	void	AddViewKick(void);
	void	DryFire(void);

	virtual DODWeaponType GetDODWeaponType(void) const
	{
		return DOD_WEAPON_TYPE_PISTOL;
	}

	void	UpdatePenaltyTime(void);

	Activity GetIdleActivity(void);
	Activity GetPrimaryAttackActivity(void);
	Activity GetDrawActivity(void);

	Activity GetReloadActivity(void);

	virtual bool Reload(void);

	virtual const Vector& GetBulletSpread(void)
	{
		static Vector cone;

		float ramp = RemapValClamped(
			m_flAccuracyPenalty,
			0.0f,
			PISTOL_ACCURACY_MAXIMUM_PENALTY_TIME,
			0.0f,
			1.0f);

		VectorLerp(VECTOR_CONE_1DEGREES, VECTOR_CONE_6DEGREES, ramp, cone);

		return cone;
	}

	virtual int GetMinBurst()
	{
		return 1;
	}

	virtual int GetMaxBurst()
	{
		return 3;
	}

	virtual float GetFireRate(void)
	{
		return 0.5f;
	}

#ifndef CLIENT_DLL
	DECLARE_ACTTABLE();
#endif

private:

	CNetworkVar(float, m_flSoonestPrimaryAttack);
	CNetworkVar(float, m_flLastAttackTime);
	CNetworkVar(float, m_flAccuracyPenalty);
	CNetworkVar(int, m_nNumShotsFired);

private:

	CWeaponColt(const CWeaponColt&);
};


IMPLEMENT_NETWORKCLASS_ALIASED(WeaponColt, DT_WeaponColt)

BEGIN_NETWORK_TABLE(CWeaponColt, DT_WeaponColt)

#ifdef CLIENT_DLL

	RecvPropTime(RECVINFO(m_flSoonestPrimaryAttack)),
	RecvPropTime(RECVINFO(m_flLastAttackTime)),
	RecvPropFloat(RECVINFO(m_flAccuracyPenalty)),
	RecvPropInt(RECVINFO(m_nNumShotsFired)),

#else

	SendPropTime(SENDINFO(m_flSoonestPrimaryAttack)),
	SendPropTime(SENDINFO(m_flLastAttackTime)),
	SendPropFloat(SENDINFO(m_flAccuracyPenalty)),
	SendPropInt(SENDINFO(m_nNumShotsFired)),

#endif

END_NETWORK_TABLE()


#ifdef CLIENT_DLL

BEGIN_PREDICTION_DATA(CWeaponColt)

	DEFINE_PRED_FIELD(m_flSoonestPrimaryAttack, FIELD_FLOAT, FTYPEDESC_INSENDTABLE),
	DEFINE_PRED_FIELD(m_flLastAttackTime, FIELD_FLOAT, FTYPEDESC_INSENDTABLE),
	DEFINE_PRED_FIELD(m_flAccuracyPenalty, FIELD_FLOAT, FTYPEDESC_INSENDTABLE),
	DEFINE_PRED_FIELD(m_nNumShotsFired, FIELD_INTEGER, FTYPEDESC_INSENDTABLE),

END_PREDICTION_DATA()

#endif


LINK_ENTITY_TO_CLASS(weapon_colt, CWeaponColt);
PRECACHE_WEAPON_REGISTER(weapon_colt);


#ifndef CLIENT_DLL

acttable_t CWeaponColt::m_acttable[] =
{
	{ ACT_DOD_STAND_AIM,					ACT_DOD_STAND_AIM_PISTOL,				false },
	{ ACT_DOD_CROUCH_AIM,					ACT_DOD_CROUCH_AIM_PISTOL,				false },
	{ ACT_DOD_CROUCHWALK_AIM,				ACT_DOD_CROUCHWALK_AIM_PISTOL,			false },
	{ ACT_DOD_WALK_AIM,						ACT_DOD_WALK_AIM_PISTOL,				false },
	{ ACT_DOD_RUN_AIM,						ACT_DOD_RUN_AIM_PISTOL,					false },
	{ ACT_PRONE_IDLE,						ACT_DOD_PRONE_AIM_PISTOL,				false },
	{ ACT_PRONE_FORWARD,					ACT_DOD_PRONEWALK_IDLE_PISTOL,			false },
	{ ACT_DOD_STAND_IDLE,					ACT_DOD_STAND_IDLE_PISTOL,				false },
	{ ACT_DOD_CROUCH_IDLE,					ACT_DOD_CROUCH_IDLE_PISTOL,				false },
	{ ACT_DOD_CROUCHWALK_IDLE,				ACT_DOD_CROUCHWALK_IDLE_PISTOL,			false },
	{ ACT_DOD_WALK_IDLE,					ACT_DOD_WALK_IDLE_PISTOL,				false },
	{ ACT_DOD_RUN_IDLE,						ACT_DOD_RUN_IDLE_PISTOL,				false },
	{ ACT_SPRINT,							ACT_DOD_SPRINT_IDLE_PISTOL,				false },

	{ ACT_RANGE_ATTACK1,					ACT_DOD_PRIMARYATTACK_PISTOL,			false },
	{ ACT_DOD_PRIMARYATTACK_CROUCH,			ACT_DOD_PRIMARYATTACK_PISTOL,			false },
	{ ACT_DOD_PRIMARYATTACK_PRONE,			ACT_DOD_PRIMARYATTACK_PRONE_PISTOL,		false },

	{ ACT_RELOAD,							ACT_DOD_RELOAD_PISTOL,					false },
	{ ACT_DOD_RELOAD_CROUCH,				ACT_DOD_RELOAD_CROUCH_PISTOL,			false },
	{ ACT_DOD_RELOAD_PRONE,					ACT_DOD_RELOAD_PRONE_PISTOL,			false },

	// Hand Signals
	{ ACT_DOD_HS_IDLE,						ACT_DOD_HS_IDLE_PISTOL,					false },
	{ ACT_DOD_HS_CROUCH,					ACT_DOD_HS_CROUCH_PISTOL,				false },
};

IMPLEMENT_ACTTABLE(CWeaponColt);

#endif

Activity CWeaponColt::GetIdleActivity(void)
{
	Activity actIdle;

	if (m_iClip1 <= 0)
		actIdle = ACT_VM_IDLE_EMPTY;
	else
		actIdle = ACT_VM_IDLE;

	return actIdle;
}

Activity CWeaponColt::GetPrimaryAttackActivity(void)
{
	Activity actPrim;

	if (m_iClip1 <= 0)
		actPrim = ACT_VM_PRIMARYATTACK_EMPTY;
	else
		actPrim = ACT_VM_PRIMARYATTACK;

	return actPrim;
}

Activity CWeaponColt::GetDrawActivity(void)
{
	Activity actDraw;

	if (m_iClip1 <= 0)
		actDraw = ACT_VM_DRAW_EMPTY;
	else
		actDraw = ACT_VM_DRAW;

	return actDraw;
}

Activity CWeaponColt::GetReloadActivity(void)
{
	Activity actReload;

	if (m_iClip1 <= 0)
		actReload = ACT_VM_RELOAD_EMPTY;
	else
		actReload = ACT_VM_RELOAD;

	return actReload;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CWeaponColt::CWeaponColt(void)
{
	m_flSoonestPrimaryAttack = gpGlobals->curtime;
	m_flLastAttackTime = gpGlobals->curtime;
	m_flAccuracyPenalty = 0.0f;
	m_nNumShotsFired = 0;

	m_fMinRange1 = 24;
	m_fMaxRange1 = 1500;
	m_fMinRange2 = 24;
	m_fMaxRange2 = 200;

	m_bReloadsSingly = false;
	m_bFiresUnderwater = true;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponColt::Precache(void)
{
	BaseClass::Precache();
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponColt::DryFire(void)
{
	WeaponSound(EMPTY);
	SendWeaponAnim(ACT_VM_DRYFIRE);

	m_flSoonestPrimaryAttack = gpGlobals->curtime + PISTOL_FASTEST_DRY_REFIRE_TIME;
	m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponColt::PrimaryAttack(void)
{
	if ((gpGlobals->curtime - m_flLastAttackTime) > 0.5f)
	{
		m_nNumShotsFired = 0;
	}
	else
	{
		m_nNumShotsFired++;
	}

	m_flLastAttackTime = gpGlobals->curtime;
	m_flSoonestPrimaryAttack = gpGlobals->curtime + PISTOL_FASTEST_REFIRE_TIME;

	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	if (pOwner)
	{
		pOwner->ViewPunchReset();
	}

	BaseClass::PrimaryAttack();

	m_flAccuracyPenalty += PISTOL_ACCURACY_SHOT_PENALTY_TIME;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponColt::UpdatePenaltyTime(void)
{
	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	if (pOwner == NULL)
		return;

	if (((pOwner->m_nButtons & IN_ATTACK) == false) &&
		(m_flSoonestPrimaryAttack < gpGlobals->curtime))
	{
		m_flAccuracyPenalty -= gpGlobals->frametime;
		m_flAccuracyPenalty = clamp(
			m_flAccuracyPenalty,
			0.0f,
			PISTOL_ACCURACY_MAXIMUM_PENALTY_TIME);
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponColt::ItemPreFrame(void)
{
	UpdatePenaltyTime();

	BaseClass::ItemPreFrame();
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponColt::ItemBusyFrame(void)
{
	UpdatePenaltyTime();

	BaseClass::ItemBusyFrame();
}


//-----------------------------------------------------------------------------
// Purpose: Allows firing as fast as button is pressed
//-----------------------------------------------------------------------------
void CWeaponColt::ItemPostFrame(void)
{
	BaseClass::ItemPostFrame();

	if (m_bInReload)
		return;

	CBasePlayer *pOwner = ToBasePlayer(GetOwner());

	if (pOwner == NULL)
		return;

	if (pOwner->m_nButtons & IN_ATTACK2)
	{
		m_flLastAttackTime = gpGlobals->curtime + PISTOL_FASTEST_REFIRE_TIME;
		m_flSoonestPrimaryAttack = gpGlobals->curtime + PISTOL_FASTEST_REFIRE_TIME;
		m_flNextPrimaryAttack = gpGlobals->curtime + PISTOL_FASTEST_REFIRE_TIME;
	}

	if (((pOwner->m_nButtons & IN_ATTACK) == false) &&
		(m_flSoonestPrimaryAttack < gpGlobals->curtime))
	{
		m_flNextPrimaryAttack = gpGlobals->curtime - 0.1f;
	}
	else if ((pOwner->m_nButtons & IN_ATTACK) &&
		(m_flNextPrimaryAttack < gpGlobals->curtime) &&
		(m_iClip1 <= 0))
	{
		DryFire();
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CWeaponColt::Reload(void)
{
	bool fRet = DefaultReload(GetMaxClip1(), GetMaxClip2(), ACT_VM_RELOAD);

	if (fRet)
	{
		WeaponSound(RELOAD);
		m_flAccuracyPenalty = 0.0f;
	}

	return fRet;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponColt::AddViewKick(void)
{
	CBasePlayer *pPlayer = ToBasePlayer(GetOwner());

	if (pPlayer == NULL)
		return;

	QAngle viewPunch;

	viewPunch.x = SharedRandomFloat("coltpax", 0.25f, 0.5f);
	viewPunch.y = SharedRandomFloat("coltpay", -0.6f, 0.6f);
	viewPunch.z = 0.0f;

	pPlayer->ViewPunch(viewPunch);
}