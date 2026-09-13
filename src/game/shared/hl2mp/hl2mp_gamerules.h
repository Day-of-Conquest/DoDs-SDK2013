//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//

#ifndef HL2MP_GAMERULES_H
#define HL2MP_GAMERULES_H
#pragma once

#include "gamerules.h"
#include "teamplay_gamerules.h"
#include "gamevars_shared.h"

#ifndef CLIENT_DLL
#include "hl2mp_player.h"
#endif


#ifdef DODS_REMAKE
#define DOD_AMMO_COLT "DOD_AMMO_COLT"
#define DOD_AMMO_P38 "DOD_AMMO_P38"
#define DOD_AMMO_C96 "DOD_AMMO_C96"
#define DOD_AMMO_SHOTGUN "DOD_AMMO_SHOTGUN"
#define DOD_AMMO_GARAND "DOD_AMMO_GARAND"
#define DOD_AMMO_K98 "DOD_AMMO_K98"
#define DOD_AMMO_M1CARBINE "DOD_AMMO_M1CARBINE"
#define DOD_AMMO_SPRING "DOD_AMMO_SPRING"
#define DOD_AMMO_SUBMG "DOD_AMMO_SUBMG"
#define DOD_AMMO_BAR "DOD_AMMO_BAR"
#define DOD_AMMO_30CAL "DOD_AMMO_30CAL"
#define DOD_AMMO_MG42 "DOD_AMMO_MG42"
#define DOD_AMMO_ROCKET "DOD_AMMO_ROCKET"
#define DOD_AMMO_HANDGRENADE "DOD_AMMO_HANDGRENADE"
#define DOD_AMMO_STICKGRENADE "DOD_AMMO_STICKGRENADE"
#define DOD_AMMO_HANDGRENADE_EX "DOD_AMMO_HANDGRENADE_EX"
#define DOD_AMMO_STICKGRENADE_EX "DOD_AMMO_STICKGRENADE_EX"
#define DOD_AMMO_SMOKEGRENADE_US "DOD_AMMO_SMOKEGRENADE_US"
#define DOD_AMMO_SMOKEGRENADE_GER "DOD_AMMO_SMOKEGRENADE_GER"
#define DOD_AMMO_SMOKEGRENADE_US_LIVE "DOD_AMMO_SMOKEGRENADE_US_LIVE"
#define DOD_AMMO_SMOKEGRENADE_GER_LIVE "DOD_AMMO_SMOKEGRENADE_GER_LIVE"
#define DOD_AMMO_RIFLEGRENADE_US "DOD_AMMO_RIFLEGRENADE_US"
#define DOD_AMMO_RIFLEGRENADE_GER "DOD_AMMO_RIFLEGRENADE_GER"
#define DOD_AMMO_RIFLEGRENADE_US_LIVE "DOD_AMMO_RIFLEGRENADE_US_LIVE"
#define DOD_AMMO_RIFLEGRENADE_GER_LIVE "DOD_AMMO_RIFLEGRENADE_GER_LIVE"
#define DMG_MACHINEGUN (DMG_LASTGENERICFLAG << 1)
#endif

#define VEC_CROUCH_TRACE_MIN	HL2MPRules()->GetHL2MPViewVectors()->m_vCrouchTraceMin
#define VEC_CROUCH_TRACE_MAX	HL2MPRules()->GetHL2MPViewVectors()->m_vCrouchTraceMax

enum
{
#ifdef DODS_REMAKE
	TEAM_AMERICANS = 2,
	TEAM_GERMANS,
	TEAM_COMBINE = TEAM_AMERICANS,
	TEAM_REBELS = TEAM_GERMANS,
#else
	TEAM_COMBINE = 2,
	TEAM_REBELS,
#endif
};


#ifdef CLIENT_DLL
	#define CHL2MPRules C_HL2MPRules
	#define CHL2MPGameRulesProxy C_HL2MPGameRulesProxy
#endif

class CHL2MPGameRulesProxy : public CGameRulesProxy
{
public:
	DECLARE_CLASS( CHL2MPGameRulesProxy, CGameRulesProxy );
	DECLARE_NETWORKCLASS();
};

class HL2MPViewVectors : public CViewVectors
{
public:
	HL2MPViewVectors( 
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight,
		Vector vCrouchTraceMin,
		Vector vCrouchTraceMax ) :
			CViewVectors( 
				vView,
				vHullMin,
				vHullMax,
				vDuckHullMin,
				vDuckHullMax,
				vDuckView,
				vObsHullMin,
				vObsHullMax,
				vDeadViewHeight )
	{
		m_vCrouchTraceMin = vCrouchTraceMin;
		m_vCrouchTraceMax = vCrouchTraceMax;
	}

	Vector m_vCrouchTraceMin;
	Vector m_vCrouchTraceMax;	
};

class CHL2MPRules : public CTeamplayRules
{
public:
	DECLARE_CLASS( CHL2MPRules, CTeamplayRules );

#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.

#else

	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.
#endif
	
	CHL2MPRules();
	virtual ~CHL2MPRules();

	virtual void Precache( void );
	virtual bool ShouldCollide( int collisionGroup0, int collisionGroup1 );
	virtual bool ClientCommand( CBaseEntity *pEdict, const CCommand &args );

	virtual float FlWeaponRespawnTime( CBaseCombatWeapon *pWeapon );
	virtual float FlWeaponTryRespawn( CBaseCombatWeapon *pWeapon );
	virtual Vector VecWeaponRespawnSpot( CBaseCombatWeapon *pWeapon );
	virtual int WeaponShouldRespawn( CBaseCombatWeapon *pWeapon );
	virtual void Think( void );
	virtual void CreateStandardEntities( void );
	virtual void ClientSettingsChanged( CBasePlayer *pPlayer );
	virtual int PlayerRelationship( CBaseEntity *pPlayer, CBaseEntity *pTarget );
	virtual void GoToIntermission( void );
	virtual void DeathNotice( CBasePlayer *pVictim, const CTakeDamageInfo &info );
	virtual const char *GetGameDescription( void );
	// derive this function if you mod uses encrypted weapon info files
	virtual const unsigned char *GetEncryptionKey( void ) { return (unsigned char *)"x9Ke0BY7"; }
	virtual const CViewVectors* GetViewVectors() const;
	const HL2MPViewVectors* GetHL2MPViewVectors() const;

	float GetMapRemainingTime();
	void CleanUpMap();
	void CheckRestartGame();
	void RestartGame();

	void OnNavMeshLoad( void );
	
#ifndef CLIENT_DLL
	virtual Vector VecItemRespawnSpot( CItem *pItem );
	virtual QAngle VecItemRespawnAngles( CItem *pItem );
	virtual float	FlItemRespawnTime( CItem *pItem );
	virtual bool	CanHavePlayerItem( CBasePlayer *pPlayer, CBaseCombatWeapon *pItem );
	virtual bool FShouldSwitchWeapon( CBasePlayer *pPlayer, CBaseCombatWeapon *pWeapon );

	void	AddLevelDesignerPlacedObject( CBaseEntity *pEntity );
	void	RemoveLevelDesignerPlacedObject( CBaseEntity *pEntity );
	void	ManageObjectRelocation( void );
	void    CheckChatForReadySignal( CHL2MP_Player *pPlayer, const char *chatmsg );
	const char *GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer );

#endif

	bool IsOfficialMap( void );

	virtual void ClientDisconnected( edict_t *pClient );

	bool CheckGameOver( void );
	bool IsIntermission( void );

	void PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info );

	
	bool	IsTeamplay( void ) { return m_bTeamPlayEnabled;	}
	void	CheckAllPlayersReady( void );

	virtual bool IsConnectedUserInfoChangeAllowed( CBasePlayer *pPlayer );
	
private:
	
	CNetworkVar( bool, m_bTeamPlayEnabled );
	CNetworkVar( float, m_flGameStartTime );
	CUtlVector<EHANDLE> m_hRespawnableItemsAndWeapons;
	float m_tmNextPeriodicThink;
	float m_flRestartGameTime;
	bool m_bCompleteReset;
	bool m_bAwaitingReadyRestart;
	bool m_bHeardAllPlayersReady;

#ifndef CLIENT_DLL
	bool m_bChangelevelDone;
#endif
};

inline CHL2MPRules* HL2MPRules()
{
	return static_cast<CHL2MPRules*>(g_pGameRules);
}

#endif //HL2MP_GAMERULES_H
