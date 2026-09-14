#include "cbase.h"
#ifdef DODS_REMAKE
#include "dods_classmenu.h"
#include "dods/dods_classes.h"
#include "hl2mp_gamerules.h"
#include "c_hl2mp_player.h"
#include "c_playerresource.h"
#include "IconPanel.h"
#include "hud.h"
#include "filesystem.h"
#include "KeyValues.h"
#include "IGameUIFuncs.h"
#include <vgui/IVGui.h>
#include <vgui_controls/CheckButton.h>
#include <vgui_controls/Label.h>
#include <vgui/IImage.h>
#include "tier0/memdbgon.h"
extern IGameUIFuncs *gameuifuncs;
extern ConVar hud_classautokill;
namespace
{
void LoadClassIcons()
{
 const char *sources[] = { "scripts/mod_textures", "scripts/doc/mod_textures" };
 for ( int source = 0; source < ARRAYSIZE( sources ); ++source )
 {
  CUtlDict<CHudTexture *, int> textures;
  LoadHudTextures( textures, sources[source], NULL );
  for ( int i = 0; i < textures.Count(); ++i )
   if ( !gHUD.GetIcon( textures.GetElementName( i ) ) ) gHUD.AddSearchableHudIconToList( *textures[i] );
  textures.PurgeAndDeleteElements();
 }
}
class CClassPreview : public vgui::EditablePanel
{
 DECLARE_CLASS_SIMPLE( CClassPreview, vgui::EditablePanel );
public:
 CClassPreview( vgui::Panel *parent ) : BaseClass( parent, "ClassInfoPanel" ) { SetProportional( true ); }
 vgui::Panel *CreateControlByName( const char *name ) OVERRIDE
 {
  if ( FStrEq( name, "CIconPanel" ) ) return new CIconPanel( this, "" );
  return BaseClass::CreateControlByName( name );
 }
};
class CClassChoice : public vgui::Button
{
 DECLARE_CLASS_SIMPLE( CClassChoice, vgui::Button );
public:
 CClassChoice( vgui::Panel *parent ) : BaseClass( parent, "", "" )
 {
  m_Command[0] = m_Active[0] = m_Inactive[0] = 0;
  m_ActiveImage = m_InactiveImage = NULL;
  SetMouseInputEnabled( true );
 }
 void ApplySettings( KeyValues *kv ) OVERRIDE
 {
  BaseClass::ApplySettings( kv );
  Q_strncpy( m_Command, kv->GetString( "command" ), sizeof( m_Command ) );
  Q_strncpy( m_Active, kv->GetString( "activeimage" ), sizeof( m_Active ) );
  Q_strncpy( m_Inactive, kv->GetString( "inactiveimage" ), sizeof( m_Inactive ) );
  LoadImages();
  SetMouseInputEnabled( true );
 }
 void OnCursorEntered() OVERRIDE { BaseClass::OnCursorEntered(); Preview(); }
 void OnSetFocus() OVERRIDE { BaseClass::OnSetFocus(); Preview(); }
 void ApplySchemeSettings( vgui::IScheme *scheme ) OVERRIDE
 {
  BaseClass::ApplySchemeSettings( scheme );
  LoadImages();
 }
 void PaintBackground() OVERRIDE
 {
  vgui::IImage *image = IsArmed() || HasFocus() ? m_ActiveImage : m_InactiveImage;
  if ( !image ) image = m_InactiveImage;
  if ( !image ) { BaseClass::PaintBackground(); return; }
  int oldWide, oldTall;
  image->GetSize( oldWide, oldTall );
  image->SetPos( 0, 0 );
  image->SetSize( GetWide(), GetTall() );
  image->SetColor( Color( 255, 255, 255, IsEnabled() ? 255 : 80 ) );
  image->Paint();
  image->SetSize( oldWide, oldTall );
 }

private:
 void LoadImages()
 {
  // These resource names refer to materials/vgui images, not the HUD icon dictionary.
  m_ActiveImage = m_Active[0] ? vgui::scheme()->GetImage( m_Active, true ) : NULL;
  m_InactiveImage = m_Inactive[0] ? vgui::scheme()->GetImage( m_Inactive, true ) : NULL;
 }
 vgui::IImage *m_ActiveImage, *m_InactiveImage; // Owned by the VGUI scheme.
 void Preview() { PostActionSignal( new KeyValues( "PreviewClass", "command", m_Command ) ); }
 char m_Command[64], m_Active[128], m_Inactive[128];
};
}
CDODSClassMenu::CDODSClassMenu( IViewPort *viewport, int team ) :
 BaseClass( viewport, DODSClassPanel( team ) ), m_Team( team ), m_ClassKey( BUTTON_CODE_INVALID ), m_Info( NULL ), m_ActivePreview( -1 )
{
 Q_memset( m_PreviewPages, 0, sizeof( m_PreviewPages ) );
 LoadClassIcons();
 // Panel::GetName supplies resource matching; our GetName override supplies
 // the separate viewport identifier. Keep the original resource roots intact.
 // Let VGUI load and own the KeyValues, including its shared resource cache.
 SetName( "class" );
 LoadControlSettings( "resource/ui/doc/classmenu.res", "GAME" );
 SetName( "ClassMenu" );
 const char *resource = team == TEAM_AMERICANS ? "resource/ui/doc/classmenu_allies.res" :
  "resource/ui/doc/classmenu_axis.res";
 KeyValues *layout = new KeyValues( "ClassMenu" );
 if ( layout->LoadFromFile( filesystem, resource, "GAME" ) )
 {
  KeyValues *frame = layout->FindKey( "ClassMenu" );
  if ( frame ) ApplySettings( frame );
  // Size the frame before constructing children: bottom-pinned buttons calculate
  // their offsets against the parent's current height in ApplySettings().
  InvalidateLayout( true );
  LoadControlSettings( resource, "GAME" );
  // Reapply button settings after the frame's layout is settled, so its final
  // dimensions are also used for the saved pin offsets.
  for ( KeyValues *entry = layout->GetFirstSubKey(); entry; entry = entry->GetNextKey() )
  {
   CClassChoice *button = dynamic_cast<CClassChoice *>( FindChildByName( entry->GetString( "fieldName", entry->GetName() ) ) );
   if ( button ) button->ApplySettings( entry );
  }
 }
 else Warning( "DODS class menu: could not load %s.\n", resource );
 // ApplySettings consumes values synchronously; this tree is never passed to VGUI's resource cache.
 layout->deleteThis();
 m_Info = dynamic_cast<vgui::EditablePanel *>( FindChildByName( "ClassInfoPanel" ) );
 vgui::ivgui()->AddTickSignal( GetVPanel(), 200 );
 SetVisible( false );
}
const char *CDODSClassMenu::GetName() { return DODSClassPanel( m_Team ); }
vgui::Panel *CDODSClassMenu::CreateControlByName( const char *name )
{
 if ( FStrEq( name, "ImageButton" ) || FStrEq( name, "DODButton" ) ||
   FStrEq( name, "DODRandomButton" ) || FStrEq( name, "DODMouseOverPanelButton" ) ) return new CClassChoice( this );
 if ( FStrEq( name, "CDODClassInfoPanel" ) ) return new CClassPreview( this );
 if ( FStrEq( name, "CIconPanel" ) ) return new CIconPanel( this, "" );
 return BaseClass::CreateControlByName( name );
}
void CDODSClassMenu::ShowPanel( bool show )
{
 if ( show )
 {
  DevMsg( "DODS class menu: preparing %s.\n", GetName() );
  m_ClassKey = gameuifuncs->GetButtonCodeForBind( "changeclass" );
  vgui::CheckButton *check = dynamic_cast<vgui::CheckButton *>( FindChildByName( "suicide_option" ) );
  if ( check ) check->SetSelected( hud_classautokill.GetBool() );
  const DODSClassInfo *info = DODSGetClass( m_Team, 0 );
  if ( info ) OnPreviewClass( info->command );
  Update();
 }
 if ( show )
 {
  // Resource decorations are not interactive. Keep them from winning mouse hit testing.
  for ( int i = GetChildCount() - 1; i >= 0; --i )
  {
   vgui::Panel *child = GetChild( i );
   vgui::Button *button = dynamic_cast<vgui::Button *>( child );
   child->SetMouseInputEnabled( button != NULL );
   if ( dynamic_cast<CClassChoice *>( child ) || dynamic_cast<vgui::CheckButton *>( child ) )
   {
    child->SetZPos( 10 );
    child->MoveToFront();
   }
  }
 }
 // Activate() requests focus only when keyboard input is already enabled.
 SetKeyBoardInputEnabled( show );
 BaseClass::ShowPanel( show );
 if ( show )
 {
  InvalidateLayout( true );
  MoveToCenterOfScreen();
  MoveToFront();
  int x, y, w, h;
  GetBounds( x, y, w, h );
  DevMsg( "DODS class menu: %s visible=%d bounds=%d,%d %dx%d children=%d\n",
   GetName(), IsVisible(), x, y, w, h, GetChildCount() );
  for ( int i = 0; i < GetChildCount(); ++i )
  {
   CClassChoice *button = dynamic_cast<CClassChoice *>( GetChild( i ) );
   if ( !button ) continue;
   button->GetBounds( x, y, w, h );
   DevMsg( "DODS class button: %s visible=%d enabled=%d bounds=%d,%d %dx%d\n",
    button->GetName(), button->IsVisible(), button->IsEnabled(), x, y, w, h );
  }
 }
}
void CDODSClassMenu::OnAutoKillChanged()
{
 vgui::CheckButton *check = dynamic_cast<vgui::CheckButton *>( FindChildByName( "suicide_option" ) );
 if ( check ) hud_classautokill.SetValue( check->IsSelected() );
}
void CDODSClassMenu::OnPreviewClass( const char *command )
{
 if ( !m_Info ) return;
 const int index = DODSFindClass( m_Team, command );
 if ( index == DODS_CLASS_NONE ) return;
 const int pageIndex = index == DODS_CLASS_RANDOM ? DODS_CLASS_COUNT : index;
 if ( pageIndex == m_ActivePreview ) return;
 // Focus/hover can request the same page again while VGUI processes layout.
 // Record the selection first, and retain each page rather than deleting focused controls.
 if ( m_ActivePreview >= 0 && m_PreviewPages[m_ActivePreview] )
  m_PreviewPages[m_ActivePreview]->SetVisible( false );
 m_ActivePreview = pageIndex;
 if ( !m_PreviewPages[pageIndex] )
 {
  CClassPreview *page = new CClassPreview( m_Info );
  m_PreviewPages[pageIndex] = page;
  page->SetBounds( 0, 0, m_Info->GetWide(), m_Info->GetTall() );
  page->SetKeyBoardInputEnabled( false );
  page->SetMouseInputEnabled( false );
  char path[MAX_PATH];
  Q_snprintf( path, sizeof( path ), "classes/%s.res", command + 4 );
  DevMsg( "DODS class menu: loading preview %s.\n", path );
  if ( filesystem->FileExists( path, "GAME" ) ) page->LoadControlSettings( path, "GAME" );
  else
  {
   const DODSClassInfo *info = DODSGetClass( m_Team, index );
   vgui::Label *description = new vgui::Label( page, "classInfo", info ? info->title : "Random class" );
   description->SetBounds( 0, 0, page->GetWide(), page->GetTall() );
  }
  // Preview resources contain display controls, not class-selection input.
  page->SetKeyBoardInputEnabled( false );
  page->SetMouseInputEnabled( false );
 }
 m_PreviewPages[pageIndex]->SetVisible( true );
 m_Info->SetKeyBoardInputEnabled( false );
 m_Info->SetMouseInputEnabled( false );
 m_Info->SetVisible( true );
}

void CDODSClassMenu::OnTick() { if ( IsVisible() ) Update(); }
void CDODSClassMenu::Update()
{
 C_HL2MP_Player *local = C_HL2MP_Player::GetLocalHL2MPPlayer();
 vgui::Panel *cancel = FindChildByName( "CancelButton" );
 if ( cancel ) cancel->SetVisible( local && local->GetTeamNumber() == m_Team && local->GetDesiredPlayerClass() != DODS_CLASS_NONE );
 C_PlayerResource *resource = dynamic_cast<C_PlayerResource *>( GameResources() );
 for ( int cls = 0; cls < DODS_CLASS_COUNT; ++cls )
 {
  int count = 0;
  if ( resource )
   for ( int slot = 1; slot <= gpGlobals->maxClients; ++slot )
    if ( resource->IsConnected( slot ) && resource->GetTeam( slot ) == m_Team && resource->GetDODSReservedClass( slot ) == cls ) ++count;
  int limit = DODSClassLimit( m_Team, cls );
  char name[64], text[32];
  Q_snprintf( name, sizeof( name ), "class_%d_num", cls + 1 );
  if ( limit < 0 ) Q_snprintf( text, sizeof( text ), "x %d", count );
  else Q_snprintf( text, sizeof( text ), "%d / %d", count, limit );
  SetLabelText( name, text );
  Q_snprintf( name, sizeof( name ), "class_%d_full", cls + 1 );
  vgui::Panel *full = FindChildByName( name );
  if ( full ) full->SetVisible( limit >= 0 && count >= limit );
 }
}
void CDODSClassMenu::OnCommand( const char *command )
{
 if ( FStrEq( command, "vguicancel" ) )
 {
  C_HL2MP_Player *local = C_HL2MP_Player::GetLocalHL2MPPlayer();
  if ( !local || local->GetTeamNumber() != m_Team || local->GetDesiredPlayerClass() == DODS_CLASS_NONE ) return;
 }
 else if ( DODSFindClass( m_Team, command ) != DODS_CLASS_NONE )
 {
  OnAutoKillChanged();
  engine->ClientCmd( command );
 }
 else { vgui::Frame::OnCommand( command ); return; }
 m_pViewPort->ShowPanel( this, false );
}
void CDODSClassMenu::OnKeyCodePressed( vgui::KeyCode code )
{
 if ( code == KEY_ESCAPE || code == m_ClassKey ) OnCommand( "vguicancel" );
 else BaseClass::OnKeyCodePressed( code );
}
CON_COMMAND_F( changeclass, "Open class selection for your team.", FCVAR_CLIENTCMD_CAN_EXECUTE | FCVAR_SERVER_CAN_EXECUTE )
{
 C_HL2MP_Player *player = C_HL2MP_Player::GetLocalHL2MPPlayer();
 if ( !player || !gViewPortInterface || engine->IsPlayingDemo() ) return;
 const char *name = DODSClassPanel( player->GetTeamNumber() );
 IViewPortPanel *panel = gViewPortInterface->FindPanelByName( name );
 if ( !panel )
 {
  Warning( "DODS changeclass: viewport panel '%s' is not registered (team %d).\n", name, player->GetTeamNumber() );
  return;
 }
 DevMsg( "DODS changeclass: opening '%s' for team %d.\n", name, player->GetTeamNumber() );
 gViewPortInterface->ShowPanel( panel, true );
}
#endif
