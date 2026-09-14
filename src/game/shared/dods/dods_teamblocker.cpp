#include "cbase.h"
#if defined( DODS_REMAKE ) && defined( HL2MP )
#include "hl2mp_gamerules.h"
#include "tier0/memdbgon.h"
#ifdef CLIENT_DLL
#define CDODSTeamBlocker C_DODSTeamBlocker
#endif

class CDODSTeamBlocker : public CBaseEntity
{
 DECLARE_CLASS( CDODSTeamBlocker, CBaseEntity );
 DECLARE_NETWORKCLASS();
public:
 CDODSTeamBlocker()
 {
  m_bDisabled = false;
  m_iBlockTeam = 0;
#ifndef CLIENT_DLL
  m_Mins.Init();
  m_Maxs.Init();
#endif
 }
 bool ShouldCollide( int group, int mask ) const OVERRIDE
 {
  if ( m_bDisabled || ( group != COLLISION_GROUP_PLAYER_MOVEMENT && group != COLLISION_GROUP_PLAYER ) ) return false;
  int blocked = m_iBlockTeam;
  if ( !blocked ) blocked = GetTeamNumber();
  return ( blocked == TEAM_AMERICANS && ( mask & CONTENTS_TEAM1 ) ) ||
   ( blocked == TEAM_GERMANS && ( mask & CONTENTS_TEAM2 ) );
 }
#ifndef CLIENT_DLL
 DECLARE_DATADESC();
 void Spawn() OVERRIDE
 {
  BaseClass::Spawn();
  SetMoveType( MOVETYPE_NONE );
  if ( GetModelName() != NULL_STRING )
  {
   SetModel( STRING( GetModelName() ) );
   SetSolid( SOLID_BSP );
  }
  else
  {
   SetSolid( SOLID_BBOX );
   UTIL_SetSize( this, m_Mins, m_Maxs );
  }
  AddEffects( EF_NODRAW );
 }
 int UpdateTransmitState() OVERRIDE { return SetTransmitState( FL_EDICT_ALWAYS ); }
 void InputEnable( inputdata_t &data ) { m_bDisabled = false; }
 void InputDisable( inputdata_t &data ) { m_bDisabled = true; }
 void InputToggle( inputdata_t &data ) { m_bDisabled = !m_bDisabled; }
 void InputBlockTeam( inputdata_t &data ) { m_iBlockTeam = data.value.Int(); }
 Vector m_Mins, m_Maxs;
#endif
 CNetworkVar( bool, m_bDisabled );
 CNetworkVar( int, m_iBlockTeam );
};
IMPLEMENT_NETWORKCLASS_ALIASED( DODSTeamBlocker, DT_DODSTeamBlocker )
BEGIN_NETWORK_TABLE( CDODSTeamBlocker, DT_DODSTeamBlocker )
#ifdef CLIENT_DLL
 RecvPropBool( RECVINFO( m_bDisabled ) ),
 RecvPropInt( RECVINFO( m_iBlockTeam ) ),
#else
 SendPropBool( SENDINFO( m_bDisabled ) ),
 SendPropInt( SENDINFO( m_iBlockTeam ), 4 ),
#endif
END_NETWORK_TABLE()
#ifndef CLIENT_DLL
LINK_ENTITY_TO_CLASS( func_teamblocker, CDODSTeamBlocker );
LINK_ENTITY_TO_CLASS( func_team_wall, CDODSTeamBlocker );
BEGIN_DATADESC( CDODSTeamBlocker )
 DEFINE_KEYFIELD( m_bDisabled, FIELD_BOOLEAN, "StartDisabled" ),
 DEFINE_KEYFIELD( m_iBlockTeam, FIELD_INTEGER, "blockteam" ),
 DEFINE_KEYFIELD( m_Mins, FIELD_VECTOR, "mins" ),
 DEFINE_KEYFIELD( m_Maxs, FIELD_VECTOR, "maxs" ),
 DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
 DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
 DEFINE_INPUTFUNC( FIELD_VOID, "Toggle", InputToggle ),
 DEFINE_INPUTFUNC( FIELD_INTEGER, "SetBlockTeam", InputBlockTeam ),
END_DATADESC()
#endif
#endif
