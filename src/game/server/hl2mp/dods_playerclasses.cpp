#include "cbase.h"
#ifdef DODS_REMAKE
#include "hl2mp_player.h"
#include "hl2mp_gamerules.h"
#include "dods/dods_classes.h"
#include "filesystem.h"
#include "ammodef.h"
#include "tier0/memdbgon.h"

bool CHL2MP_Player::CanChooseDODSClass( int index ) const
{
 if ( !DODSGetClass( GetTeamNumber(), index ) ) return false;
 int limit = DODSClassLimit( GetTeamNumber(), index );
 if ( limit < 0 ) return true;
 int count = 0;
 for ( int slot = 1; slot <= gpGlobals->maxClients; ++slot )
 {
  CHL2MP_Player *other = ToHL2MPPlayer( UTIL_PlayerByIndex( slot ) );
  if ( other && other != this && other->IsConnected() && other->GetTeamNumber() == GetTeamNumber() &&
    ( other->GetDesiredPlayerClass() == index ||
      ( other->GetDesiredPlayerClass() == DODS_CLASS_RANDOM && other->GetPlayerClass() == index ) ) ) ++count;
 }
 return count < limit;
}
void CHL2MP_Player::ShowClassSelectMenu()
{
 ShowViewPortPanel( DODSClassPanel( GetTeamNumber() ), true );
}
bool CHL2MP_Player::HandleCommand_JoinClass( int index )
{
 if ( GetTeamNumber() != TEAM_AMERICANS && GetTeamNumber() != TEAM_GERMANS )
 {
  ShowClassSelectMenu();
  return false;
 }
 bool allowed = false;
 if ( index == DODS_CLASS_RANDOM )
 {
  for ( int i = 0; i < DODS_CLASS_COUNT; ++i ) allowed |= CanChooseDODSClass( i );
 }
 else allowed = CanChooseDODSClass( index );
 if ( !allowed )
 {
  ClientPrint( this, HUD_PRINTTALK, "That class is unavailable or full." );
  ShowClassSelectMenu();
  return false;
 }
 if ( m_iDesiredPlayerClass == index && m_iPlayerClass != DODS_CLASS_NONE ) return true;
 m_iDesiredPlayerClass = index;
 ShowViewPortPanel( DODSClassPanel( GetTeamNumber() ), false );
 // Only initial selection spawns immediately; death keeps the regular respawn timing.
 if ( m_iPlayerClass == DODS_CLASS_NONE && IsObserver() )
 {
  StopObserverMode();
  State_Transition( STATE_ACTIVE );
  Spawn();
 }
 else if ( IsAlive() )
 {
  const char *autokill = engine->GetClientConVarValue( entindex(), "hud_classautokill" );
  if ( autokill && atoi( autokill ) != 0 ) CommitSuicide();
  else ClientPrint( this, HUD_PRINTTALK, "Your new class will take effect on respawn." );
 }
 return true;
}
bool CHL2MP_Player::PrepareDODSClass()
{
 if ( IsFakeClient() && m_iDesiredPlayerClass == DODS_CLASS_NONE ) m_iDesiredPlayerClass = DODS_CLASS_RANDOM;
 int chosen = m_iDesiredPlayerClass;
 if ( chosen == DODS_CLASS_RANDOM )
 {
  int choices[DODS_CLASS_COUNT], count = 0;
  for ( int i = 0; i < DODS_CLASS_COUNT; ++i )
   if ( CanChooseDODSClass( i ) ) choices[count++] = i;
  chosen = count ? choices[random->RandomInt( 0, count - 1 )] : DODS_CLASS_NONE;
  if ( IsFakeClient() )
  {
   int least = gpGlobals->maxClients + 1, ties = 0;
   for ( int c = 0; c < count; ++c )
   {
    int members = 0;
    for ( int slot = 1; slot <= gpGlobals->maxClients; ++slot )
    {
     CHL2MP_Player *other = ToHL2MPPlayer( UTIL_PlayerByIndex( slot ) );
     if ( !other || other == this || other->GetTeamNumber() != GetTeamNumber() ) continue;
     int reserved = other->GetDesiredPlayerClass();
     if ( reserved == DODS_CLASS_RANDOM ) reserved = other->GetPlayerClass();
     if ( reserved == choices[c] ) ++members;
    }
    if ( members < least ) { least = members; ties = 0; }
    if ( members == least && random->RandomInt( 0, ties++ ) == 0 ) chosen = choices[c];
   }
  }
 }
 else if ( !CanChooseDODSClass( chosen ) ) chosen = DODS_CLASS_NONE;
 m_iPlayerClass = chosen;
 if ( chosen == DODS_CLASS_NONE )
 {
  m_iDesiredPlayerClass = DODS_CLASS_NONE;
  return false;
 }
 return true;
}
void CHL2MP_Player::PrecacheDODSClasses()
{
 for ( int team = TEAM_AMERICANS; team <= TEAM_GERMANS; ++team )
  for ( int i = 0; i < DODS_CLASS_COUNT; ++i )
  {
   const DODSClassInfo *info = DODSGetClass( team, i );
   if ( !info ) continue;
   if ( info->model[0] && filesystem->FileExists( info->model, "GAME" ) ) PrecacheModel( info->model );
   for ( int w = 0; w < ARRAYSIZE( info->weapons ); ++w )
    if ( info->weapons[w][0] && EntityFactoryDictionary()->FindFactory( info->weapons[w] ) )
     UTIL_PrecacheOther( info->weapons[w] );
  }
}
void CHL2MP_Player::GiveDODSClassItems()
{
 const DODSClassInfo *info = DODSGetClass( GetTeamNumber(), m_iPlayerClass );
 if ( !info ) return;
 EquipSuit();
 CBaseCombatWeapon *primary = NULL;
 bool fallback = false;
 for ( int w = 0; w < ARRAYSIZE( info->weapons ); ++w )
 {
  const char *name = info->weapons[w];
  if ( w >= 3 && info->grenadeCount[w - 3] == 0 ) continue;
  if ( !name[0] || !EntityFactoryDictionary()->FindFactory( name ) )
  {
   if ( w != 0 ) continue;
   // Temporary equipment only: class identity and original scripts remain intact.
   name = "weapon_garand";
   fallback = true;
  }
  CBaseCombatWeapon *weapon = dynamic_cast<CBaseCombatWeapon *>( GiveNamedItem( name ) );
  if ( !weapon ) continue;
  if ( w == 0 ) primary = weapon;
  const int ammo = weapon->GetPrimaryAmmoType();
  if ( ammo >= 0 )
  {
   if ( w >= 3 ) SetAmmoCount( MIN( info->grenadeCount[w - 3], GetAmmoDef()->MaxCarry( ammo ) ), ammo );
   else GiveAmmo( MAX( 0, weapon->GetMaxClip1() ) * 3, ammo, true );
  }
 }
 if ( fallback ) ClientPrint( this, HUD_PRINTTALK, "This class uses a temporary Garand until its primary weapon is implemented." );
 if ( primary ) Weapon_Switch( primary );
 if ( info->model[0] && modelinfo->GetModelIndex( info->model ) > 0 )
 {
  SetModel( info->model );
  SetupPlayerSoundsByModel( info->model );
 }
}
#endif
