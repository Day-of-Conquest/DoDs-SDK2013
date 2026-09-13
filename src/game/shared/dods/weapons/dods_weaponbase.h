//=============================================================================//
//
// Purpose: Weapon base for Day of Defeat: Source
//
//=============================================================================//

#ifndef WEAPON_DODBASE_H
#define WEAPON_DODBASE_H
#ifdef _WIN32
#pragma once
#endif

#include "weapon_hl2mpbasehlmpcombatweapon.h"
#include "weapon_parse.h"

#if defined( CLIENT_DLL )
#define CWeaponDODSBase C_WeaponDODSBase
#endif

#define DOD_AMMO_SUBMG				"DOD_AMMO_SUBMG"
#define DOD_AMMO_ROCKET				"DOD_AMMO_ROCKET"
#define DOD_AMMO_COLT				"DOD_AMMO_COLT"
#define DOD_AMMO_P38				"DOD_AMMO_P38"
#define DOD_AMMO_C96				"DOD_AMMO_C96"
#define DOD_AMMO_WEBLEY				"DOD_AMMO_WEBLEY"
#define DOD_AMMO_GARAND				"DOD_AMMO_GARAND"
#define DOD_AMMO_K98				"DOD_AMMO_K98"
#define DOD_AMMO_M1CARBINE			"DOD_AMMO_M1CARBINE"
#define DOD_AMMO_ENFIELD			"DOD_AMMO_ENFIELD"
#define DOD_AMMO_SPRING				"DOD_AMMO_SPRING"
#define DOD_AMMO_FG42				"DOD_AMMO_FG42"
#define DOD_AMMO_BREN				"DOD_AMMO_BREN"
#define DOD_AMMO_BAR				"DOD_AMMO_BAR"
#define DOD_AMMO_30CAL				"DOD_AMMO_30CAL"
#define DOD_AMMO_MG34				"DOD_AMMO_MG34"
#define DOD_AMMO_MG42				"DOD_AMMO_MG42"

#define DOD_AMMO_HANDGRENADE			"DOD_AMMO_HANDGRENADE"
#define DOD_AMMO_HANDGRENADE_EX		"DOD_AMMO_HANDGRENADE_EX"
#define DOD_AMMO_STICKGRENADE			"DOD_AMMO_STICKGRENADE"
#define DOD_AMMO_STICKGRENADE_EX		"DOD_AMMO_STICKGRENADE_EX"

#define DOD_AMMO_SMOKEGRENADE_US		"DOD_AMMO_SMOKEGRENADE_US"
#define DOD_AMMO_SMOKEGRENADE_GER		"DOD_AMMO_SMOKEGRENADE_GER"
#define DOD_AMMO_SMOKEGRENADE_US_LIVE		"DOD_AMMO_SMOKEGRENADE_US_LIVE"
#define DOD_AMMO_SMOKEGRENADE_GER_LIVE		"DOD_AMMO_SMOKEGRENADE_GER_LIVE"

#define DOD_AMMO_RIFLEGRENADE_US		"DOD_AMMO_RIFLEGRENADE_US"
#define DOD_AMMO_RIFLEGRENADE_GER		"DOD_AMMO_RIFLEGRENADE_GER"
#define DOD_AMMO_RIFLEGRENADE_US_LIVE		"DOD_AMMO_RIFLEGRENADE_US_LIVE"
#define DOD_AMMO_RIFLEGRENADE_GER_LIVE		"DOD_AMMO_RIFLEGRENADE_GER_LIVE"

#define DOD_AMMO_SHOTGUN			"DOD_AMMO_SHOTGUN"

#define DOD_SNIPER_SCOPE_CHANGE_TIME 0.3f


enum DODWeaponType
{
	DOD_WEAPON_TYPE_NONE = 0,

	DOD_WEAPON_TYPE_PISTOL,
	DOD_WEAPON_TYPE_RIFLE,
	DOD_WEAPON_TYPE_SMG,
	DOD_WEAPON_TYPE_ASSAULT,
	DOD_WEAPON_TYPE_MG,
	DOD_WEAPON_TYPE_SNIPER,
	DOD_WEAPON_TYPE_ROCKET,
	DOD_WEAPON_TYPE_GRENADE,
	DOD_WEAPON_TYPE_MELEE
};


class CWeaponDODSBase : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS(CWeaponDODSBase, CBaseHL2MPCombatWeapon);
	DECLARE_NETWORKCLASS();
	DECLARE_PREDICTABLE();

	CWeaponDODSBase();

#ifdef DODS_REMAKE
	virtual bool Holster(CBaseCombatWeapon *pSwitchingTo = NULL) OVERRIDE;
	virtual bool Reload(void) OVERRIDE;
	virtual void Drop(const Vector &velocity) OVERRIDE;
#endif

	//-----------------------------------------------------------------------------
	// Weapon type
	//-----------------------------------------------------------------------------

	virtual DODWeaponType GetDODWeaponType(void) const;

	bool IsDODPistol(void) const;
	bool IsDODRifle(void) const;
	bool IsDODSMG(void) const;
	bool IsDODAssaultWeapon(void) const;
	bool IsDODMachineGun(void) const;
	bool IsDODSniper(void) const;

	//-----------------------------------------------------------------------------
	// Weapon firing
	//-----------------------------------------------------------------------------

	virtual float GetDODFireRate(void) const;
	virtual Vector GetDODBulletSpread(void) const;

	virtual float GetDODRecoilPitchMin(void) const;
	virtual float GetDODRecoilPitchMax(void) const;
	virtual float GetDODRecoilYaw(void) const;

	void FireDODBullet(void);
	void ApplyDODRecoil(void);

	//-----------------------------------------------------------------------------
	// Scope
	//-----------------------------------------------------------------------------

	void SetScoped(bool bScoped) { m_bScoped = bScoped; }
	bool IsScoped(void) { return m_bScoped; }

	virtual float GetScopedFOV(void) { return 20; }

	virtual bool ShouldScopeOutBetweenShots(void) { return true; }
	virtual bool ShouldReScopeAfterReload(void) { return false; }

	virtual void SecondaryAttack(void) OVERRIDE;

	void SecondaryPunch(void);

	void ToggleScope(void);
	void ScopeIn(void);
	void ScopeOut(void);
	void ScopeOutIn(void);

	void SetReScope(bool bReScope, float flDelay);

	bool IsFullyScoped(void);
	bool IsScopingIn(void);

#ifdef CLIENT_DLL
	float GetScopedPercentage(void);
#endif

	CNetworkVar(bool, m_bScoped);
	CNetworkVar(bool, m_bViewAnim);

protected:
	float m_flScopeInTime;

#ifdef DODS_REMAKE
	CNetworkVar(float, m_flScopeChangeTime);
#else
	float m_flScopeChangeTime;
#endif

	float m_flUnscopeTime;
	float m_flRescopeTime;

	bool m_bRescopeAfterShot;

#ifdef CLIENT_DLL
	float m_flScopePercent;
#endif

#ifdef GAME_DLL
	DECLARE_DATADESC();
#endif
};


#endif // WEAPON_DODBASE_H