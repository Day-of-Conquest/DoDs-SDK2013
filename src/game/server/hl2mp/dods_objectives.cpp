#include "cbase.h"
#if defined( DODS_REMAKE ) && defined( HL2MP )
#include "triggers.h"
#include "hl2mp_gamerules.h"
#include "team.h"
#include "filesystem.h"
#include "engine/IEngineTrace.h"
#include "tier0/memdbgon.h"

namespace
{
bool PlayingTeam( int team ) { return team == TEAM_AMERICANS || team == TEAM_GERMANS; }
}

class CDODSControlPoint : public CBaseAnimating
{
 DECLARE_CLASS( CDODSControlPoint, CBaseAnimating );
 DECLARE_SERVERCLASS();
 DECLARE_DATADESC();
public:
 CDODSControlPoint() : m_DefaultOwner( 0 ), m_Group( NULL_STRING )
 {
  m_Title = NULL_STRING;
  m_Index = 0;
  m_Disabled = false;
  for ( int i = 0; i < 3; ++i ) { m_Models[i] = m_Sounds[i] = NULL_STRING; m_Bodygroups[i] = 0; }
  m_HudName.GetForModify()[0] = 0;
  m_CaptureTeam = m_AlliesPresent = m_AxisPresent = 0;
  m_AlliesNeeded = m_AxisNeeded = 1;
  m_Progress = 0;
  m_CaptureEnabled = m_AlliesAllowed = m_AxisAllowed = false;
  for ( int i = 0; i < MAX_PLAYERS_ARRAY_SAFE; ++i ) m_InArea.Set( i, false );
 }
 void Precache() OVERRIDE
 {
  BaseClass::Precache();
  for ( int i = 0; i < 3; ++i )
  {
   if ( m_Models[i] != NULL_STRING && filesystem->FileExists( STRING( m_Models[i] ), "GAME" ) ) PrecacheModel( STRING( m_Models[i] ) );
   if ( m_Sounds[i] != NULL_STRING ) PrecacheScriptSound( STRING( m_Sounds[i] ) );
  }
 }
 void Spawn() OVERRIDE
 {
  Precache();
  SetSolid( SOLID_NONE );
  SetMoveType( MOVETYPE_NONE );
  Q_strncpy( m_HudName.GetForModify(), STRING( m_Title ), 128 );
  SetOwnerTeam( m_DefaultOwner, this, false, false );
 }
 void SetOwnerTeam( int team, CBaseEntity *activator, bool captured, bool outputs = true )
 {
  if ( !PlayingTeam( team ) ) team = TEAM_UNASSIGNED;
  const bool changed = team != GetTeamNumber();
  ChangeTeam( team );
  int slot = team == TEAM_AMERICANS ? 1 : team == TEAM_GERMANS ? 2 : 0;
  if ( m_Models[slot] != NULL_STRING && modelinfo->GetModelIndex( STRING( m_Models[slot] ) ) > 0 )
  {
   SetModel( STRING( m_Models[slot] ) );
   SetBodygroup( 0, m_Bodygroups[slot] );
   RemoveEffects( EF_NODRAW );
  }
  else AddEffects( EF_NODRAW );
  if ( !outputs || !changed ) return;
  DevMsg( "DODS objective %s: owner changed to team %d.\n", GetDebugName(), team );
  if ( m_Sounds[slot] != NULL_STRING ) EmitSound( STRING( m_Sounds[slot] ) );
  if ( slot == 1 ) { m_Allies.FireOutput( activator, this ); if ( captured ) m_AlliesCaptured.FireOutput( activator, this ); }
  else if ( slot == 2 ) { m_Axis.FireOutput( activator, this ); if ( captured ) m_AxisCaptured.FireOutput( activator, this ); }
  else m_Reset.FireOutput( activator, this );
 }
 void InputOwner( inputdata_t &data ) { SetOwnerTeam( data.value.Int(), data.pActivator, false ); }
 void InputReset( inputdata_t &data ) { SetOwnerTeam( m_DefaultOwner, data.pActivator, false ); }
 void InputShow( inputdata_t &data ) { RemoveEffects( EF_NODRAW ); }
 void InputHide( inputdata_t &data ) { AddEffects( EF_NODRAW ); }
 void InputEnable( inputdata_t &data ) { m_Disabled = false; }
 void InputDisable( inputdata_t &data ) { m_Disabled = true; }
 bool IsActive() const { return !m_Disabled; }
 int UpdateTransmitState() OVERRIDE { return SetTransmitState( FL_EDICT_ALWAYS ); }
 void ClearCapture()
 {
  m_CaptureTeam = 0;
  m_Progress = 0;
  m_AlliesPresent = m_AxisPresent = 0;
  m_CaptureEnabled = false;
  for ( int i = 0; i < MAX_PLAYERS_ARRAY_SAFE; ++i ) m_InArea.Set( i, false );
 }
 void SetParticipant( int index ) { if ( index > 0 && index < MAX_PLAYERS_ARRAY_SAFE ) m_InArea.Set( index, true ); }
 void CaptureStatus( int team, float progress, int allies, int axis, int alliesNeeded, int axisNeeded, bool alliesAllowed, bool axisAllowed )
 {
  m_CaptureTeam = team;
  m_Progress = clamp( progress, 0.0f, 1.0f );
  m_AlliesPresent = allies;
  m_AxisPresent = axis;
  m_AlliesNeeded = MAX( 1, alliesNeeded );
  m_AxisNeeded = MAX( 1, axisNeeded );
  m_CaptureEnabled = true;
  m_AlliesAllowed = alliesAllowed;
  m_AxisAllowed = axisAllowed;
 }
 int PointIndex() const { return m_Index; }
 int DefaultOwner() const { return m_DefaultOwner; }
private:
 int m_DefaultOwner;
 string_t m_Group;
 CNetworkVar( int, m_Index );
 CNetworkVar( bool, m_Disabled );
 CNetworkString( m_HudName, 128 );
 CNetworkVar( int, m_CaptureTeam );
 CNetworkVar( float, m_Progress );
 CNetworkVar( int, m_AlliesPresent );
 CNetworkVar( int, m_AxisPresent );
 CNetworkVar( int, m_AlliesNeeded );
 CNetworkVar( int, m_AxisNeeded );
 CNetworkVar( bool, m_CaptureEnabled );
 CNetworkVar( bool, m_AlliesAllowed );
 CNetworkVar( bool, m_AxisAllowed );
 CNetworkArray( bool, m_InArea, MAX_PLAYERS_ARRAY_SAFE );
 int m_Bodygroups[3];
 string_t m_Title, m_Models[3], m_Sounds[3];
 COutputEvent m_Allies, m_Axis, m_Reset, m_AlliesCaptured, m_AxisCaptured;
};
IMPLEMENT_SERVERCLASS_ST( CDODSControlPoint, DT_DODSControlPoint )
 SendPropInt( SENDINFO( m_Index ), 16 ),
 SendPropBool( SENDINFO( m_Disabled ) ),
 SendPropString( SENDINFO( m_HudName ) ),
 SendPropInt( SENDINFO( m_CaptureTeam ), 3, SPROP_UNSIGNED ),
 SendPropFloat( SENDINFO( m_Progress ), 10, 0, 0.0f, 1.0f ),
 SendPropInt( SENDINFO( m_AlliesPresent ), 8, SPROP_UNSIGNED ),
 SendPropInt( SENDINFO( m_AxisPresent ), 8, SPROP_UNSIGNED ),
 SendPropInt( SENDINFO( m_AlliesNeeded ), 8, SPROP_UNSIGNED ),
 SendPropInt( SENDINFO( m_AxisNeeded ), 8, SPROP_UNSIGNED ),
 SendPropBool( SENDINFO( m_CaptureEnabled ) ),
 SendPropBool( SENDINFO( m_AlliesAllowed ) ),
 SendPropBool( SENDINFO( m_AxisAllowed ) ),
 SendPropArray3( SENDINFO_ARRAY3( m_InArea ), SendPropBool( SENDINFO_ARRAY( m_InArea ) ) ),
END_SEND_TABLE()
LINK_ENTITY_TO_CLASS( dod_control_point, CDODSControlPoint );
LINK_ENTITY_TO_CLASS( team_control_point, CDODSControlPoint );
BEGIN_DATADESC( CDODSControlPoint )
 DEFINE_KEYFIELD( m_DefaultOwner, FIELD_INTEGER, "point_default_owner" ),
 DEFINE_KEYFIELD( m_Group, FIELD_STRING, "point_group" ),
 DEFINE_KEYFIELD( m_Index, FIELD_INTEGER, "point_index" ),
 DEFINE_KEYFIELD( m_Title, FIELD_STRING, "point_printname" ),
 DEFINE_KEYFIELD( m_Disabled, FIELD_BOOLEAN, "StartDisabled" ),
 DEFINE_KEYFIELD( m_Bodygroups[0], FIELD_INTEGER, "point_reset_model_bodygroup" ),
 DEFINE_KEYFIELD( m_Bodygroups[1], FIELD_INTEGER, "point_allies_model_bodygroup" ),
 DEFINE_KEYFIELD( m_Bodygroups[2], FIELD_INTEGER, "point_axis_model_bodygroup" ),
 DEFINE_KEYFIELD( m_Models[0], FIELD_STRING, "point_reset_model" ),
 DEFINE_KEYFIELD( m_Models[1], FIELD_STRING, "point_allies_model" ),
 DEFINE_KEYFIELD( m_Models[2], FIELD_STRING, "point_axis_model" ),
 DEFINE_KEYFIELD( m_Sounds[0], FIELD_STRING, "point_resetsound" ),
 DEFINE_KEYFIELD( m_Sounds[1], FIELD_STRING, "point_allies_capsound" ),
 DEFINE_KEYFIELD( m_Sounds[2], FIELD_STRING, "point_axis_capsound" ),
 DEFINE_INPUTFUNC( FIELD_INTEGER, "SetOwner", InputOwner ),
 DEFINE_INPUTFUNC( FIELD_VOID, "RoundInit", InputReset ),
 DEFINE_INPUTFUNC( FIELD_VOID, "ShowModel", InputShow ),
 DEFINE_INPUTFUNC( FIELD_VOID, "HideModel", InputHide ),
 DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
 DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
 DEFINE_OUTPUT( m_Allies, "OnAlliesCap" ),
 DEFINE_OUTPUT( m_Axis, "OnAxisCap" ),
 DEFINE_OUTPUT( m_Reset, "OnCapReset" ),
 DEFINE_OUTPUT( m_AlliesCaptured, "AlliesCapturePoint" ),
 DEFINE_OUTPUT( m_AxisCaptured, "AxisCapturePoint" ),
END_DATADESC()

class CDODSCaptureArea : public CBaseTrigger
{
 DECLARE_CLASS( CDODSCaptureArea, CBaseTrigger );
 DECLARE_DATADESC();
public:
 CDODSCaptureArea() : m_AlliesRequired( 1 ), m_AxisRequired( 1 ), m_Duration( 5.0f ),
  m_AlliesAllowed( true ), m_AxisAllowed( true ), m_Capturing( 0 ), m_Elapsed( 0 ), m_LastThink( 0 ) { m_PointName = NULL_STRING; }
 void Spawn() OVERRIDE
 {
  AddSpawnFlags( SF_TRIGGER_ALLOW_CLIENTS );
  BaseClass::Spawn();
  InitTrigger();
  m_LastThink = gpGlobals->curtime;
  SetThink( &CDODSCaptureArea::CaptureThink );
  SetNextThink( gpGlobals->curtime + 0.1f );
 }
 void Activate() OVERRIDE
 {
  BaseClass::Activate();
  m_Point = dynamic_cast<CDODSControlPoint *>( gEntList.FindEntityByName( NULL, m_PointName ) );
  if ( !m_Point ) Warning( "dod_capture_area '%s': missing control point '%s'.\n", GetDebugName(), STRING( m_PointName ) );
 }
 void BreakCapture()
 {
  const int old = m_Capturing;
  m_Capturing = 0;
  m_Elapsed = 0;
  if ( old == TEAM_AMERICANS ) m_AlliesBreak.FireOutput( this, this );
  if ( old == TEAM_GERMANS ) m_AxisBreak.FireOutput( this, this );
  if ( old ) m_Break.FireOutput( this, this );
 }
 void InputDisable( inputdata_t &data ) OVERRIDE { BaseClass::InputDisable( data ); BreakCapture(); if ( m_Point ) m_Point->ClearCapture(); }
 void InputReset( inputdata_t &data ) { BreakCapture(); m_LastThink = gpGlobals->curtime; }
 CDODSControlPoint *BotPoint() const { return m_Point.Get(); }
 bool CanBotCapture( CBasePlayer *player )
 {
  return player && !m_bDisabled && m_Point && m_Point->IsActive() &&
   m_Point->GetTeamNumber() != player->GetTeamNumber() && PassesTriggerFilters( player ) &&
   ( player->GetTeamNumber() == TEAM_AMERICANS ? m_AlliesAllowed :
     player->GetTeamNumber() == TEAM_GERMANS && m_AxisAllowed );
 }
 bool ContainsBot( CBasePlayer *player )
 {
  Ray_t hull; trace_t overlap;
  hull.Init( player->GetAbsOrigin(), player->GetAbsOrigin(), player->WorldAlignMins(), player->WorldAlignMaxs() );
  enginetrace->ClipRayToCollideable( hull, MASK_ALL, CollisionProp(), &overlap );
  return overlap.startsolid || overlap.allsolid;
 }
 void CaptureThink()
 {
  float delta = MAX( 0.0f, gpGlobals->curtime - m_LastThink );
  m_LastThink = gpGlobals->curtime;
  SetNextThink( gpGlobals->curtime + 0.1f );
  if ( m_Point ) m_Point->ClearCapture();
  if ( m_bDisabled || !m_Point || !m_Point->IsActive() || ( HL2MPRules()->IsIntermission() || HL2MPRules()->IsDODSRoundOver() ) ) { BreakCapture(); return; }
  int allies = 0, axis = 0;
  CBaseEntity *activator = this;
  for ( int i = 1; i <= gpGlobals->maxClients; ++i )
  {
   CBasePlayer *p = UTIL_PlayerByIndex( i );
   if ( !p || !p->IsAlive() || p->IsObserver() || !PassesTriggerFilters( p ) ) continue;
   Ray_t hull;
   trace_t overlap;
   hull.Init( p->GetAbsOrigin(), p->GetAbsOrigin(), p->WorldAlignMins(), p->WorldAlignMaxs() );
   enginetrace->ClipRayToCollideable( hull, MASK_ALL, CollisionProp(), &overlap );
   if ( !overlap.startsolid && !overlap.allsolid ) continue;
   m_Point->SetParticipant( p->entindex() );
   if ( p->GetTeamNumber() == TEAM_AMERICANS ) { ++allies; activator = p; }
   if ( p->GetTeamNumber() == TEAM_GERMANS ) { ++axis; activator = p; }
  }
  int team = 0;
  if ( allies >= MAX( 1, m_AlliesRequired ) && !axis && m_AlliesAllowed ) team = TEAM_AMERICANS;
  if ( axis >= MAX( 1, m_AxisRequired ) && !allies && m_AxisAllowed ) team = TEAM_GERMANS;
  if ( !team || team == m_Point->GetTeamNumber() )
  {
   BreakCapture();
   m_Point->CaptureStatus( 0, 0, allies, axis, m_AlliesRequired, m_AxisRequired, m_AlliesAllowed, m_AxisAllowed );
   return;
  }
  if ( m_Capturing != team )
  {
   BreakCapture();
   m_Capturing = team;
   if ( team == TEAM_AMERICANS ) m_AlliesStart.FireOutput( activator, this );
   else m_AxisStart.FireOutput( activator, this );
   m_Start.FireOutput( activator, this );
   m_Point->CaptureStatus( team, 0, allies, axis, m_AlliesRequired, m_AxisRequired, m_AlliesAllowed, m_AxisAllowed );
   return;
  }
  m_Elapsed += delta;
  m_Point->CaptureStatus( team, m_Duration > 0 ? m_Elapsed / m_Duration : 1.0f,
   allies, axis, m_AlliesRequired, m_AxisRequired, m_AlliesAllowed, m_AxisAllowed );
  if ( m_Elapsed < MAX( 0.0f, m_Duration ) ) return;
  m_Capturing = 0;
  m_Elapsed = 0;
  m_Point->SetOwnerTeam( team, activator, true );
  m_Point->CaptureStatus( 0, 0, allies, axis, m_AlliesRequired, m_AxisRequired, m_AlliesAllowed, m_AxisAllowed );
  if ( team == TEAM_AMERICANS ) m_AlliesEnd.FireOutput( activator, this );
  else m_AxisEnd.FireOutput( activator, this );
  m_End.FireOutput( activator, this );
 }
private:
 string_t m_PointName;
 CHandle<CDODSControlPoint> m_Point;
 int m_AlliesRequired, m_AxisRequired;
 float m_Duration;
 bool m_AlliesAllowed, m_AxisAllowed;
 int m_Capturing;
 float m_Elapsed, m_LastThink;
 COutputEvent m_AlliesStart, m_AlliesBreak, m_AlliesEnd, m_AxisStart, m_AxisBreak, m_AxisEnd, m_Start, m_Break, m_End;
};
LINK_ENTITY_TO_CLASS( dod_capture_area, CDODSCaptureArea );
BEGIN_DATADESC( CDODSCaptureArea )
 DEFINE_KEYFIELD( m_PointName, FIELD_STRING, "area_cap_point" ),
 DEFINE_KEYFIELD( m_AlliesRequired, FIELD_INTEGER, "area_allies_numcap" ),
 DEFINE_KEYFIELD( m_AxisRequired, FIELD_INTEGER, "area_axis_numcap" ),
 DEFINE_KEYFIELD( m_Duration, FIELD_FLOAT, "area_time_to_cap" ),
 DEFINE_KEYFIELD( m_AlliesAllowed, FIELD_BOOLEAN, "area_allies_cancap" ),
 DEFINE_KEYFIELD( m_AxisAllowed, FIELD_BOOLEAN, "area_axis_cancap" ),
 DEFINE_FIELD( m_Point, FIELD_EHANDLE ),
 DEFINE_FIELD( m_Capturing, FIELD_INTEGER ),
 DEFINE_FIELD( m_Elapsed, FIELD_FLOAT ),
 DEFINE_FIELD( m_LastThink, FIELD_TIME ),
 DEFINE_THINKFUNC( CaptureThink ),
 DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
 DEFINE_INPUTFUNC( FIELD_VOID, "RoundInit", InputReset ),
 DEFINE_OUTPUT( m_AlliesStart, "OnAlliesStartCap" ),
 DEFINE_OUTPUT( m_AlliesBreak, "OnAlliesBreakCap" ),
 DEFINE_OUTPUT( m_AlliesEnd, "OnAlliesEndCap" ),
 DEFINE_OUTPUT( m_AxisStart, "OnAxisStartCap" ),
 DEFINE_OUTPUT( m_AxisBreak, "OnAxisBreakCap" ),
 DEFINE_OUTPUT( m_AxisEnd, "OnAxisEndCap" ),
 DEFINE_OUTPUT( m_Start, "OnStartCap" ),
 DEFINE_OUTPUT( m_Break, "OnBreakCap" ),
 DEFINE_OUTPUT( m_End, "OnEndCap" ),
END_DATADESC()

// Minimal map-I/O win detector. HL2MP retains responsibility for round lifecycle.
bool DODSBotCaptureGoal( CBasePlayer *player, CBaseEntity *candidate, Vector &goal, bool &inside )
{
 CDODSCaptureArea *area = dynamic_cast<CDODSCaptureArea *>( candidate );
 if ( !area || !area->CanBotCapture( player ) ) return false;
 goal = area->WorldSpaceCenter();
 inside = area->ContainsBot( player );
 return true;
}

int DODSBotCaptureOrder( CBasePlayer *player, CBaseEntity *candidate )
{
 CDODSCaptureArea *area = dynamic_cast<CDODSCaptureArea *>( candidate );
 if ( !area || !area->BotPoint() ) return 0;
 float sums[2] = { 0, 0 }; int counts[2] = { 0, 0 };
 for ( CBaseEntity *ent = gEntList.FirstEnt(); ent; ent = gEntList.NextEnt( ent ) )
 {
  CDODSControlPoint *point = dynamic_cast<CDODSControlPoint *>( ent );
  if ( !point || !point->IsActive() || !PlayingTeam( point->DefaultOwner() ) ) continue;
  int side = point->DefaultOwner() == TEAM_AMERICANS ? 0 : 1;
  sums[side] += point->PointIndex(); ++counts[side];
 }
 bool americansAscending = true;
 if ( counts[0] && counts[1] ) americansAscending = sums[0] / counts[0] <= sums[1] / counts[1];
 bool ascending = player->GetTeamNumber() == TEAM_AMERICANS ? americansAscending : !americansAscending;
 return ascending ? area->BotPoint()->PointIndex() : -area->BotPoint()->PointIndex();
}

class CDODSPointMaster : public CPointEntity
{
 DECLARE_CLASS( CDODSPointMaster, CPointEntity );
 DECLARE_DATADESC();
public:
 CDODSPointMaster() : m_Disabled( false ), m_Winner( 0 ) {}
 void Spawn() OVERRIDE { SetThink( &CDODSPointMaster::CheckPoints ); SetNextThink( gpGlobals->curtime + 0.5f ); }
 void InputEnable( inputdata_t &data ) { m_Disabled = false; }
 void InputDisable( inputdata_t &data ) { m_Disabled = true; }
 void InputReset( inputdata_t &data )
 {
  m_Winner = 0;
  for ( CBaseEntity *ent = gEntList.FirstEnt(); ent; ent = gEntList.NextEnt( ent ) )
  {
   CDODSControlPoint *point = dynamic_cast<CDODSControlPoint *>( ent );
   CDODSCaptureArea *area = dynamic_cast<CDODSCaptureArea *>( ent );
   if ( point ) point->InputReset( data );
   if ( area ) area->InputReset( data );
  }
 }
 void CheckPoints()
 {
  SetNextThink( gpGlobals->curtime + 0.5f );
  if ( m_Disabled || m_Winner ) return;
  int owner = 0, count = 0;
  for ( CBaseEntity *ent = gEntList.FirstEnt(); ent; ent = gEntList.NextEnt( ent ) )
  {
   CDODSControlPoint *point = dynamic_cast<CDODSControlPoint *>( ent );
   if ( !point || !point->IsActive() ) continue;
   int team = point->GetTeamNumber();
   if ( !PlayingTeam( team ) || ( count && owner != team ) ) return;
   owner = team;
   ++count;
  }
  if ( !count ) return;
  m_Winner = owner;
  HL2MPRules()->EndDODSRound( owner );
  if ( owner == TEAM_AMERICANS ) m_AlliesWin.FireOutput( this, this );
  else m_AxisWin.FireOutput( this, this );
 }
private:
 bool m_Disabled;
 int m_Winner;
 COutputEvent m_AlliesWin, m_AxisWin;
};
LINK_ENTITY_TO_CLASS( dod_control_point_master, CDODSPointMaster );
LINK_ENTITY_TO_CLASS( team_control_point_master, CDODSPointMaster );
BEGIN_DATADESC( CDODSPointMaster )
 DEFINE_KEYFIELD( m_Disabled, FIELD_BOOLEAN, "StartDisabled" ),
 DEFINE_FIELD( m_Winner, FIELD_INTEGER ),
 DEFINE_THINKFUNC( CheckPoints ),
 DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
 DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
 DEFINE_INPUTFUNC( FIELD_VOID, "RoundInit", InputReset ),
 DEFINE_OUTPUT( m_AlliesWin, "OnAlliesWin" ),
 DEFINE_OUTPUT( m_AxisWin, "OnAxisWin" ),
END_DATADESC()
#endif
