#include "cbase.h"
#ifdef DODS_REMAKE
#include "dods_classes.h"
#include "hl2mp_gamerules.h"
#include "viewport_panel_names.h"
#include "filesystem.h"
#include "KeyValues.h"
#include "igamesystem.h"
#include "tier0/memdbgon.h"

static ConVar mp_limit_allies_assault( "mp_limit_allies_assault", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_allies_mg( "mp_limit_allies_mg", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_allies_rifleman( "mp_limit_allies_rifleman", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_allies_rocket( "mp_limit_allies_rocket", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_allies_sergeant( "mp_limit_allies_sergeant", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_allies_sniper( "mp_limit_allies_sniper", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_allies_support( "mp_limit_allies_support", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_axis_assault( "mp_limit_axis_assault", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_axis_mg( "mp_limit_axis_mg", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_axis_rifleman( "mp_limit_axis_rifleman", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_axis_rocket( "mp_limit_axis_rocket", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_axis_sniper( "mp_limit_axis_sniper", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
static ConVar mp_limit_axis_support( "mp_limit_axis_support", "-1", FCVAR_REPLICATED, "Class capacity: -1 unlimited, 0 disabled." );
namespace
{
class CClassCatalog : public CAutoGameSystem
{
public:
 CClassCatalog() : CAutoGameSystem( "DODS class catalog" ), loaded( false ) {}
 void LevelInitPreEntity() OVERRIDE { loaded = false; Load(); }
 void LevelShutdownPostEntity() OVERRIDE { loaded = false; }
 void Load()
 {
  if ( loaded ) return;
  loaded = true;
  Q_memset( info, 0, sizeof( info ) );
  const char *files[2][DODS_CLASS_COUNT] = {
   { "us_garand", "us_tommy", "us_bar", "us_spring", "us_30cal", "us_bazooka", "us_m3", "us_medic" },
   { "axis_k98", "axis_mp40", "axis_mp44", "axis_k98s", "axis_mg42", "axis_pschreck", "axis_m3", "axis_medic" }
  };
  const char *weaponKeys[] = { "primaryweapon", "secondaryweapon", "meleeweapon", "grenadetype", "grenadetype2" };
  for ( int t = 0; t < 2; ++t )
   for ( int i = 0; i < DODS_CLASS_COUNT; ++i )
   {
    char path[MAX_PATH];
    Q_snprintf( path, sizeof( path ), "scripts/playerclass_%s.txt", files[t][i] );
    KeyValues *kv = new KeyValues( "PlayerClassDatafile" );
    if ( kv->LoadFromFile( filesystem, path, "GAME" ) )
    {
     DODSClassInfo &entry = info[t][i];
     Q_strncpy( entry.command, kv->GetString( "selectcmd" ), sizeof( entry.command ) );
     Q_strncpy( entry.title, kv->GetString( "printname" ), sizeof( entry.title ) );
     Q_strncpy( entry.model, kv->GetString( "playermodel" ), sizeof( entry.model ) );
     Q_strncpy( entry.limit, kv->GetString( "limitcvar" ), sizeof( entry.limit ) );
     for ( int w = 0; w < ARRAYSIZE( weaponKeys ); ++w )
     {
      const char *name = kv->GetString( weaponKeys[w] );
      if ( name[0] ) Q_snprintf( entry.weapons[w], sizeof( entry.weapons[w] ), "weapon_%s", name );
     }
     entry.grenadeCount[0] = MAX( 0, kv->GetInt( "numgrens" ) );
     entry.grenadeCount[1] = MAX( 0, kv->GetInt( "numgrens2" ) );
     entry.valid = !Q_strncmp( entry.command, "cls_", 4 ) && entry.weapons[0][0];
    }
    else Warning( "DODS: missing class script %s\n", path );
    kv->deleteThis();
   }
 }
 DODSClassInfo info[2][DODS_CLASS_COUNT];
 bool loaded;
} catalog;
}
const DODSClassInfo *DODSGetClass( int team, int index )
{
 if ( ( team != TEAM_AMERICANS && team != TEAM_GERMANS ) || index < 0 || index >= DODS_CLASS_COUNT ) return NULL;
 catalog.Load();
 const DODSClassInfo &info = catalog.info[team == TEAM_AMERICANS ? 0 : 1][index];
 return info.valid ? &info : NULL;
}
int DODSFindClass( int team, const char *command )
{
 if ( FStrEq( command, "cls_random" ) ) return DODS_CLASS_RANDOM;
 for ( int i = 0; i < DODS_CLASS_COUNT; ++i )
 {
  const DODSClassInfo *info = DODSGetClass( team, i );
  if ( info && FStrEq( info->command, command ) ) return i;
 }
 return DODS_CLASS_NONE;
}
int DODSClassLimit( int team, int index )
{
 const DODSClassInfo *info = DODSGetClass( team, index );
 if ( !info ) return 0;
 ConVarRef limit( info->limit, true );
 return limit.IsValid() ? limit.GetInt() : -1;
}
const char *DODSClassPanel( int team )
{
 return team == TEAM_AMERICANS ? PANEL_CLASS_ALLIES : team == TEAM_GERMANS ? PANEL_CLASS_AXIS : PANEL_TEAM;
}
#endif
