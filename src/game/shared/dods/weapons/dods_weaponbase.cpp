//=============================================================================//
//
// Purpose: Weapon base for Day of Defeat: Source
//
//=============================================================================//

#include "cbase.h"
#include "dods_weaponbase.h"
#include "in_buttons.h"

IMPLEMENT_NETWORKCLASS_ALIASED(WeaponDODSBase, DT_WeaponDODSBase)

BEGIN_NETWORK_TABLE(CWeaponDODSBase, DT_WeaponDODSBase)
#ifdef DODS_REMAKE
#ifdef CLIENT_DLL
	RecvPropBool(RECVINFO(m_bScoped)),
	RecvPropBool(RECVINFO(m_bViewAnim)),
	RecvPropFloat(RECVINFO(m_flScopeChangeTime)),
#else
	SendPropBool(SENDINFO(m_bScoped)),
	SendPropBool(SENDINFO(m_bViewAnim)),
	SendPropFloat(SENDINFO(m_flScopeChangeTime), 0, SPROP_NOSCALE),
#endif
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA(CWeaponDODSBase)
#ifdef DODS_REMAKE
	DEFINE_PRED_FIELD(m_bScoped, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE),
	DEFINE_PRED_FIELD(m_bViewAnim, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE),
	DEFINE_PRED_FIELD(m_flScopeChangeTime, FIELD_FLOAT, FTYPEDESC_INSENDTABLE),
	DEFINE_PRED_FIELD(m_flScopeInTime, FIELD_FLOAT, 0),
	DEFINE_PRED_FIELD(m_flUnscopeTime, FIELD_FLOAT, 0),
	DEFINE_PRED_FIELD(m_flRescopeTime, FIELD_FLOAT, 0),
	DEFINE_PRED_FIELD(m_bRescopeAfterShot, FIELD_BOOLEAN, 0),
#endif
END_PREDICTION_DATA()
#endif // CLIENT_DLL

#ifdef GAME_DLL

BEGIN_DATADESC(CWeaponDODSBase)
END_DATADESC()

#endif


CWeaponDODSBase::CWeaponDODSBase()
{
#ifdef DODS_REMAKE
	m_bScoped = false;
	m_bViewAnim = false;
#endif
	m_flScopeInTime = -1;
	m_flScopeChangeTime = -1;
	m_flUnscopeTime = -1;
	m_flRescopeTime = -1;

	m_bRescopeAfterShot = false;

#ifdef CLIENT_DLL
	m_flScopePercent = 0.0f;
#endif
}

void CWeaponDODSBase::SecondaryAttack(void)
{
#ifdef DODS_REMAKE
	CBasePlayer *owner = GetPlayerOwner();

	if (!owner || !owner->IsAlive() || m_bInReload)
		return;

	switch (GetDODWeaponType())
	{
	case DOD_WEAPON_TYPE_RIFLE:
	case DOD_WEAPON_TYPE_SNIPER:
		if (owner->m_afButtonPressed & IN_ATTACK2)
		{
			ToggleScope();
		}
		break;

	case DOD_WEAPON_TYPE_SMG:
		SecondaryPunch();
		break;

	default:
		break;
	}
#else
	BaseClass::SecondaryAttack();
#endif
}

#define DOD_PUNCH_RANGE			64.0f
#define DOD_PUNCH_DAMAGE		25.0f
#define DOD_PUNCH_REFIRE_TIME	0.45f

void CWeaponDODSBase::SecondaryPunch(void)
{
	CBasePlayer *owner = GetPlayerOwner();

	if (!owner || !owner->IsAlive())
		return;

	if (m_flNextSecondaryAttack > gpGlobals->curtime)
		return;

	if (m_bInReload)
	{
		m_bInReload = false;
		owner->m_flNextAttack = gpGlobals->curtime;
	}

	SendWeaponAnim(ACT_VM_SECONDARYATTACK);
	owner->SetAnimation(PLAYER_ATTACK1);

	m_flNextPrimaryAttack = gpGlobals->curtime + DOD_PUNCH_REFIRE_TIME;
	m_flNextSecondaryAttack = gpGlobals->curtime + DOD_PUNCH_REFIRE_TIME;

#ifndef CLIENT_DLL

	Vector vecForward;
	owner->EyeVectors(&vecForward);

	Vector vecStart = owner->Weapon_ShootPosition();
	Vector vecEnd = vecStart + vecForward * 64.0f;

	trace_t tr;

	UTIL_TraceLine(
		vecStart,
		vecEnd,
		MASK_SHOT_HULL,
		owner,
		COLLISION_GROUP_NONE,
		&tr
	);

	if (tr.fraction == 1.0f)
	{
		Vector vecMins(-16.0f, -16.0f, -16.0f);
		Vector vecMaxs(16.0f, 16.0f, 16.0f);

		UTIL_TraceHull(
			vecStart,
			vecEnd,
			vecMins,
			vecMaxs,
			MASK_SHOT_HULL,
			owner,
			COLLISION_GROUP_NONE,
			&tr
		);
	}

	if (tr.m_pEnt)
	{
		CTakeDamageInfo info(
			owner,
			owner,
			this,
			25.0f,
			DMG_CLUB
		);

		CalculateMeleeDamageForce(
			&info,
			vecForward,
			tr.endpos,
			1.0f
		);

		tr.m_pEnt->TakeDamage(info);
	}

#endif
}

void CWeaponDODSBase::ToggleScope(void)
{
	CBasePlayer* pPlayer = GetPlayerOwner();

	if (pPlayer)
	{
		if (!IsScoped())
		{
			ScopeIn();
		}
		else
		{
			ScopeOut();
		}
	}
}

void CWeaponDODSBase::ScopeIn(void)
{
#ifdef DODS_REMAKE
	CBasePlayer *player = GetPlayerOwner();
	if (!player || !player->IsAlive() || m_bInReload || IsScoped())
		return;
	// The weapon owns the zoom, so it can release it without interfering with other FOV users.
	if (!player->SetFOV(this, (int)GetScopedFOV(), DOD_SNIPER_SCOPE_CHANGE_TIME))
		return;
#endif
	m_flScopeInTime = gpGlobals->curtime + DOD_SNIPER_SCOPE_CHANGE_TIME;

#if defined(DODS_REMAKE) || !defined(CLIENT_DLL)
	SetScoped(true);

	m_bViewAnim = !m_bViewAnim;
#endif

	m_flNextPrimaryAttack = MAX(gpGlobals->curtime + 0.5, m_flNextPrimaryAttack);
	m_flNextSecondaryAttack = MAX(gpGlobals->curtime + 0.5, m_flNextSecondaryAttack);
	m_flTimeWeaponIdle = gpGlobals->curtime + GetWpnData().m_flTimeToIdleAfterFire;

	m_flScopeChangeTime = gpGlobals->curtime;
}

void CWeaponDODSBase::ScopeOut(void)
{
	bool bWasScoped = IsScoped();

#ifdef DODS_REMAKE
	CBasePlayer *player = GetPlayerOwner();
	if (player && bWasScoped)
		player->SetFOV(this, 0, DOD_SNIPER_SCOPE_CHANGE_TIME);
	SetScoped(false);
	if (bWasScoped)
		m_bViewAnim = !m_bViewAnim;
#elif !defined(CLIENT_DLL)
	CBasePlayer* pPlayer = GetPlayerOwner();
	Assert(pPlayer);

	if (pPlayer)
	{
		pPlayer->SetFOV(pPlayer, 90, 0.1f);
	}

	SetScoped(false);

	m_bViewAnim = !m_bViewAnim;
#endif	

	if (bWasScoped)
	{
		m_flNextPrimaryAttack = MAX(gpGlobals->curtime + 0.5, m_flNextPrimaryAttack);
		m_flNextSecondaryAttack = MAX(gpGlobals->curtime + 0.5, m_flNextSecondaryAttack);
		m_flTimeWeaponIdle = gpGlobals->curtime + GetWpnData().m_flTimeToIdleAfterFire;
	}

	m_flScopeChangeTime = gpGlobals->curtime;

	m_flScopeInTime = -1;
	m_flUnscopeTime = -1;
	m_flRescopeTime = -1;
}

#define RESCOPE_TIME	1.2f

void CWeaponDODSBase::ScopeOutIn(void)
{
	ScopeOut();

	m_flRescopeTime = gpGlobals->curtime + 1.0f;
}

void CWeaponDODSBase::SetReScope(bool bReScope, float flDelay)
{
	m_flUnscopeTime = gpGlobals->curtime + flDelay;

	m_bRescopeAfterShot = bReScope;
}

bool CWeaponDODSBase::IsFullyScoped(void)
{
	return (IsScoped() == true &&
		((gpGlobals->curtime - m_flScopeChangeTime) > DOD_SNIPER_SCOPE_CHANGE_TIME));
}

bool CWeaponDODSBase::IsScopingIn(void)
{
	return (m_flScopeInTime > gpGlobals->curtime);
}

#ifdef CLIENT_DLL

float CWeaponDODSBase::GetScopedPercentage(void)
{
#ifdef DODS_REMAKE
	float progress = clamp((gpGlobals->curtime - m_flScopeChangeTime) / DOD_SNIPER_SCOPE_CHANGE_TIME, 0.0f, 1.0f);
	return IsScoped() ? progress : 1.0f - progress;
#else
	return m_flScopePercent;
#endif
}

#endif
#ifdef DODS_REMAKE
bool CWeaponDODSBase::Holster(CBaseCombatWeapon *pSwitchingTo)
{
	if (!BaseClass::Holster(pSwitchingTo))
		return false;
	ScopeOut();
	return true;
}

bool CWeaponDODSBase::Reload(void)
{
	if (!BaseClass::Reload())
		return false;
	ScopeOut();
	return true;
}

void CWeaponDODSBase::Drop(const Vector &velocity)
{
	ScopeOut();
	BaseClass::Drop(velocity);
}
#endif

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
DODWeaponType CWeaponDODSBase::GetDODWeaponType(void) const
{
	return DOD_WEAPON_TYPE_NONE;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CWeaponDODSBase::IsDODPistol(void) const
{
	return GetDODWeaponType() == DOD_WEAPON_TYPE_PISTOL;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CWeaponDODSBase::IsDODRifle(void) const
{
	return GetDODWeaponType() == DOD_WEAPON_TYPE_RIFLE;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CWeaponDODSBase::IsDODSMG(void) const
{
	return GetDODWeaponType() == DOD_WEAPON_TYPE_SMG;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CWeaponDODSBase::IsDODAssaultWeapon(void) const
{
	return GetDODWeaponType() == DOD_WEAPON_TYPE_ASSAULT;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CWeaponDODSBase::IsDODMachineGun(void) const
{
	return GetDODWeaponType() == DOD_WEAPON_TYPE_MG;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CWeaponDODSBase::IsDODSniper(void) const
{
	return GetDODWeaponType() == DOD_WEAPON_TYPE_SNIPER;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
float CWeaponDODSBase::GetDODFireRate(void) const
{
	switch (GetDODWeaponType())
	{
	case DOD_WEAPON_TYPE_PISTOL:
		return 0.15f;

	case DOD_WEAPON_TYPE_RIFLE:
		return 0.5f;

	case DOD_WEAPON_TYPE_SMG:
		return 0.1f;

	case DOD_WEAPON_TYPE_ASSAULT:
		return 0.1f;

	case DOD_WEAPON_TYPE_MG:
		return 0.08f;

	case DOD_WEAPON_TYPE_SNIPER:
		return 1.0f;

	default:
		return 0.2f;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
Vector CWeaponDODSBase::GetDODBulletSpread(void) const
{
	switch (GetDODWeaponType())
	{
	case DOD_WEAPON_TYPE_PISTOL:
		return VECTOR_CONE_3DEGREES;

	case DOD_WEAPON_TYPE_RIFLE:
		return VECTOR_CONE_1DEGREES;

	case DOD_WEAPON_TYPE_SMG:
		return VECTOR_CONE_4DEGREES;

	case DOD_WEAPON_TYPE_ASSAULT:
		return VECTOR_CONE_3DEGREES;

	case DOD_WEAPON_TYPE_MG:
		return VECTOR_CONE_4DEGREES;

	case DOD_WEAPON_TYPE_SNIPER:
		return VECTOR_CONE_1DEGREES;

	default:
		return VECTOR_CONE_4DEGREES;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
float CWeaponDODSBase::GetDODRecoilPitchMin(void) const
{
	switch (GetDODWeaponType())
	{
	case DOD_WEAPON_TYPE_PISTOL:
		return -1.0f;

	case DOD_WEAPON_TYPE_RIFLE:
		return -2.5f;

	case DOD_WEAPON_TYPE_SMG:
		return -1.3f;

	case DOD_WEAPON_TYPE_ASSAULT:
		return -1.7f;

	case DOD_WEAPON_TYPE_MG:
		return -1.5f;

	case DOD_WEAPON_TYPE_SNIPER:
		return -3.0f;

	default:
		return -1.0f;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
float CWeaponDODSBase::GetDODRecoilPitchMax(void) const
{
	switch (GetDODWeaponType())
	{
	case DOD_WEAPON_TYPE_PISTOL:
		return -0.5f;

	case DOD_WEAPON_TYPE_RIFLE:
		return -1.5f;

	case DOD_WEAPON_TYPE_SMG:
		return -0.7f;

	case DOD_WEAPON_TYPE_ASSAULT:
		return -0.9f;

	case DOD_WEAPON_TYPE_MG:
		return -0.8f;

	case DOD_WEAPON_TYPE_SNIPER:
		return -2.0f;

	default:
		return -0.5f;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
float CWeaponDODSBase::GetDODRecoilYaw(void) const
{
	switch (GetDODWeaponType())
	{
	case DOD_WEAPON_TYPE_PISTOL:
		return 0.35f;

	case DOD_WEAPON_TYPE_RIFLE:
		return 0.5f;

	case DOD_WEAPON_TYPE_SMG:
		return 0.45f;

	case DOD_WEAPON_TYPE_ASSAULT:
		return 0.5f;

	case DOD_WEAPON_TYPE_MG:
		return 0.55f;

	case DOD_WEAPON_TYPE_SNIPER:
		return 0.3f;

	default:
		return 0.3f;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponDODSBase::FireDODBullet(void)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return;

	Vector vecSrc = pPlayer->Weapon_ShootPosition();
	Vector vecAiming = pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	FireBulletsInfo_t info;

	info.m_iShots = 1;
	info.m_vecSrc = vecSrc;
	info.m_vecDirShooting = vecAiming;
	info.m_vecSpread = GetDODBulletSpread();
	info.m_flDistance = MAX_TRACE_LENGTH;
	info.m_iAmmoType = m_iPrimaryAmmoType;
	info.m_pAttacker = pPlayer;

	pPlayer->FireBullets(info);
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CWeaponDODSBase::ApplyDODRecoil(void)
{
	CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

	if (!pPlayer)
		return;

	pPlayer->ViewPunch(
		QAngle(
			random->RandomFloat(
				GetDODRecoilPitchMin(),
				GetDODRecoilPitchMax()
			),
			random->RandomFloat(
				-GetDODRecoilYaw(),
				GetDODRecoilYaw()
			),
			0.0f
		)
	);
}
