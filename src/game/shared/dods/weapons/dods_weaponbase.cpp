//=============================================================================//
//
// Purpose: Weapon base for Day of Defeat: Source
//
//=============================================================================//

#include "cbase.h"
#include "dods_weaponbase.h"


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
