#include "cbase.h"
#if defined( DODS_REMAKE ) && defined( HL2MP )
#include "hud.h"
#include "hudelement.h"
#include "iclientmode.h"
#include "hl2mp_gamerules.h"
#include <vgui/ISurface.h>
#include <vgui/IImage.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/Label.h>
#include "tier0/memdbgon.h"

class C_DODSControlPoint;
static CUtlVector<C_DODSControlPoint *> s_Points;
class C_DODSControlPoint : public C_BaseAnimating
{
 DECLARE_CLASS( C_DODSControlPoint, C_BaseAnimating );
 DECLARE_CLIENTCLASS();
public:
 C_DODSControlPoint()
 {
  m_Index = m_CaptureTeam = m_AlliesPresent = m_AxisPresent = 0;
  m_AlliesNeeded = m_AxisNeeded = 1;
  m_Disabled = m_CaptureEnabled = m_AlliesAllowed = m_AxisAllowed = false;
  m_Progress = 0;
  m_HudName[0] = 0;
  Q_memset( m_InArea, 0, sizeof( m_InArea ) );
  s_Points.AddToTail( this );
 }
 ~C_DODSControlPoint() { s_Points.FindAndRemove( this ); }
 int m_Index;
 bool m_Disabled;
 char m_HudName[128];
 int m_CaptureTeam;
 float m_Progress;
 int m_AlliesPresent, m_AxisPresent, m_AlliesNeeded, m_AxisNeeded;
 bool m_CaptureEnabled, m_AlliesAllowed, m_AxisAllowed;
 bool m_InArea[MAX_PLAYERS_ARRAY_SAFE];
};
IMPLEMENT_CLIENTCLASS_DT( C_DODSControlPoint, DT_DODSControlPoint, CDODSControlPoint )
 RecvPropInt( RECVINFO( m_Index ) ),
 RecvPropBool( RECVINFO( m_Disabled ) ),
 RecvPropString( RECVINFO( m_HudName ) ),
 RecvPropInt( RECVINFO( m_CaptureTeam ) ),
 RecvPropFloat( RECVINFO( m_Progress ) ),
 RecvPropInt( RECVINFO( m_AlliesPresent ) ),
 RecvPropInt( RECVINFO( m_AxisPresent ) ),
 RecvPropInt( RECVINFO( m_AlliesNeeded ) ),
 RecvPropInt( RECVINFO( m_AxisNeeded ) ),
 RecvPropBool( RECVINFO( m_CaptureEnabled ) ),
 RecvPropBool( RECVINFO( m_AlliesAllowed ) ),
 RecvPropBool( RECVINFO( m_AxisAllowed ) ),
 RecvPropArray3( RECVINFO_ARRAY( m_InArea ), RecvPropBool( RECVINFO( m_InArea[0] ) ) ),
END_RECV_TABLE()

namespace
{
const char *FlagImage( int team )
{
 return team == TEAM_AMERICANS ? "Flag_USArmy" : team == TEAM_GERMANS ? "Flag_Wermacht" : "Flag_Neutral";
}
C_DODSControlPoint *LocalCapturePoint()
{
 C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();
 if ( !local || !local->IsAlive() || local->IsObserver() ) return NULL;
 int slot = local->entindex();
 if ( slot <= 0 || slot >= MAX_PLAYERS_ARRAY_SAFE ) return NULL;
 for ( int i = 0; i < s_Points.Count(); ++i )
  if ( !s_Points[i]->m_Disabled && s_Points[i]->m_CaptureEnabled && s_Points[i]->m_InArea[slot] ) return s_Points[i];
 return NULL;
}
int ComparePoints( C_DODSControlPoint *const *a, C_DODSControlPoint *const *b )
{
 if ( (*a)->m_Index != (*b)->m_Index ) return (*a)->m_Index < (*b)->m_Index ? -1 : 1;
 return (*a)->entindex() - (*b)->entindex();
}
// Use the supplied ImagePanel controls, adding only the live progress/player rendering.
class CCaptureImage : public vgui::ImagePanel
{
 DECLARE_CLASS_SIMPLE( CCaptureImage, vgui::ImagePanel );
public:
 CCaptureImage( vgui::Panel *parent ) : BaseClass( parent, "" ), m_Fraction( 0 ), m_Filled( false ) {}
 void PaintBackground() OVERRIDE
 {
  if ( FStrEq( GetName(), "CapturePanelProgressBar" ) )
  {
   const int radius = MIN( GetWide(), GetTall() ) / 2;
   const int inner = MAX( 0, radius - MAX( 2, radius / 9 ) );
   for ( int degree = 0; degree < 360; ++degree )
   {
    float sn, cs;
    SinCos( DEG2RAD( (float)degree - 90.0f ), &sn, &cs );
    vgui::surface()->DrawSetColor( degree < m_Fraction * 360.0f ? m_Active : m_Inactive );
    vgui::surface()->DrawLine( GetWide()/2 + cs * inner, GetTall()/2 + sn * inner,
     GetWide()/2 + cs * radius, GetTall()/2 + sn * radius );
   }
   return;
  }
  if ( !Q_strncmp( GetName(), "CapturePanelPlayerIcon", 22 ) )
  {
   Color color = m_Filled ? Color( 255, 255, 255, 255 ) : Color( 100, 100, 100, 150 );
   CHudTexture *icon = gHUD.GetIcon( "capture_icon" );
   if ( icon ) icon->DrawSelf( 0, 0, GetWide(), GetTall(), color );
   else
   {
    vgui::surface()->DrawSetColor( color );
    vgui::surface()->DrawFilledRect( GetWide()/3, 0, GetWide()*2/3, GetTall()/4 );
    vgui::surface()->DrawFilledRect( GetWide()/4, GetTall()/3, GetWide()*3/4, GetTall()*3/4 );
    vgui::surface()->DrawLine( GetWide()/2, GetTall()/2, 0, GetTall() );
    vgui::surface()->DrawLine( GetWide()/2, GetTall()/2, GetWide(), GetTall() );
   }
   return;
  }
  BaseClass::PaintBackground();
 }
 float m_Fraction;
 bool m_Filled;
 CPanelAnimationVar( Color, m_Active, "color_active", "HudProgressBar.Active" );
 CPanelAnimationVar( Color, m_Inactive, "color_inactive", "HudProgressBar.InActive" );
};
}

class CHudDODSCapture : public CHudElement, public vgui::EditablePanel
{
 DECLARE_CLASS_SIMPLE( CHudDODSCapture, vgui::EditablePanel );
public:
 CHudDODSCapture( const char *name ) : CHudElement( "HudCapturePanel" ), BaseClass( g_pClientMode->GetViewport(), "HudCapturePanel" )
 {
  SetProportional( true );
  SetMouseInputEnabled( false );
  SetKeyBoardInputEnabled( false );
  SetPaintBackgroundEnabled( false );
  SetHiddenBits( HIDEHUD_PLAYERDEAD );
 }
 bool ShouldDraw() OVERRIDE { return LocalCapturePoint() && CHudElement::ShouldDraw(); }
 vgui::Panel *CreateControlByName( const char *name ) OVERRIDE
 {
  if ( FStrEq( name, "ImagePanel" ) ) return new CCaptureImage( this );
  return BaseClass::CreateControlByName( name );
 }
 void ApplySchemeSettings( vgui::IScheme *scheme ) OVERRIDE
 {
  BaseClass::ApplySchemeSettings( scheme );
  LoadControlSettings( "resource/ui/hudcapturepanel.res", "GAME" );
  vgui::Panel *background = FindChildByName( "CapturePanelBackground" );
  if ( background ) background->SetVisible( false );
 }
 void OnThink() OVERRIDE
 {
  C_DODSControlPoint *point = LocalCapturePoint();
  C_BasePlayer *local = C_BasePlayer::GetLocalPlayer();
  if ( !point || !local ) return;
  const char *flags[] = { "CapturePanelNeutralFlag", "CapturePanelAlliesFlag", "CapturePanelAxisFlag" };
  int flag = point->GetTeamNumber() == TEAM_AMERICANS ? 1 : point->GetTeamNumber() == TEAM_GERMANS ? 2 : 0;
  for ( int i = 0; i < 3; ++i )
  {
   vgui::Panel *panel = FindChildByName( flags[i] );
   if ( panel ) panel->SetVisible( i == flag );
  }
  CCaptureImage *progress = dynamic_cast<CCaptureImage *>( FindChildByName( "CapturePanelProgressBar" ) );
  if ( progress ) progress->m_Fraction = point->m_Progress;
  bool allies = local->GetTeamNumber() == TEAM_AMERICANS;
  int needed = allies ? point->m_AlliesNeeded : point->m_AxisNeeded;
  int present = allies ? point->m_AlliesPresent : point->m_AxisPresent;
  int shown = MIN( 5, needed );
  for ( int i = 0; i < 5; ++i )
  {
   char name[40];
   Q_snprintf( name, sizeof( name ), "CapturePanelPlayerIcon%d", i + 1 );
   CCaptureImage *icon = dynamic_cast<CCaptureImage *>( FindChildByName( name ) );
   if ( !icon ) continue;
   icon->SetVisible( i < shown );
   icon->m_Filled = i < present;
   int x, y;
   icon->GetPos( x, y );
   icon->SetPos( ( GetWide() - shown * ( icon->GetWide() + m_Spacing ) ) / 2 + i * ( icon->GetWide() + m_Spacing ), y );
  }
  vgui::Label *message = dynamic_cast<vgui::Label *>( FindChildByName( "CapturePanelMessage" ) );
  if ( !message ) return;
  char text[128];
  if ( point->m_AlliesPresent && point->m_AxisPresent ) Q_strncpy( text, "Capture blocked", sizeof( text ) );
  else if ( point->GetTeamNumber() == local->GetTeamNumber() ) Q_strncpy( text, "Point secured", sizeof( text ) );
  else if ( !( allies ? point->m_AlliesAllowed : point->m_AxisAllowed ) ) Q_strncpy( text, "Cannot capture", sizeof( text ) );
  else if ( present < needed ) Q_snprintf( text, sizeof( text ), "Need players: %d / %d", present, needed );
  else Q_snprintf( text, sizeof( text ), "Capturing: %d%%", (int)( point->m_Progress * 100 ) );
  message->SetText( text );
 }
 CPanelAnimationVarAliasType( int, m_Spacing, "icon_space", "2", "proportional_int" );
};
DECLARE_NAMED_HUDELEMENT( CHudDODSCapture, HudCapturePanel );

class CHudDODSObjectives : public CHudElement, public vgui::Panel
{
 DECLARE_CLASS_SIMPLE( CHudDODSObjectives, vgui::Panel );
public:
 CHudDODSObjectives( const char *name ) : CHudElement( "HudObjectiveIcons" ), BaseClass( g_pClientMode->GetViewport(), "HudObjectiveIcons" )
 {
  SetProportional( true );
  SetMouseInputEnabled( false );
  SetKeyBoardInputEnabled( false );
  SetPaintBackgroundEnabled( false );
 }
 bool ShouldDraw() OVERRIDE { return s_Points.Count() > 0 && CHudElement::ShouldDraw(); }
 void Paint() OVERRIDE
 {
  CUtlVector<C_DODSControlPoint *> points;
  for ( int i = 0; i < s_Points.Count(); ++i ) if ( !s_Points[i]->m_Disabled ) points.AddToTail( s_Points[i] );
  points.Sort( ComparePoints );
  int x = m_StartX, y = m_StartY;
  for ( int i = 0; i < points.Count(); ++i )
  {
   if ( x + m_Size > GetWide() ) { x = m_StartX; y += m_Size + m_Separator + 4; }
   C_DODSControlPoint *point = points[i];
   vgui::IImage *image = vgui::scheme()->GetImage( FlagImage( point->GetTeamNumber() ), true );
   if ( image )
   {
    int oldWide, oldTall;
    image->GetSize( oldWide, oldTall );
    image->SetPos( x, y );
    image->SetSize( m_Size, m_Size );
    image->SetColor( Color( 255, 255, 255, 255 ) );
    image->Paint();
    image->SetSize( oldWide, oldTall );
   }
   if ( point->m_CaptureTeam )
   {
    vgui::surface()->DrawSetColor( Color( 255, 210, 60, 255 ) );
    vgui::surface()->DrawFilledRect( x, y + m_Size, x + (int)( m_Size * point->m_Progress ), y + m_Size + 3 );
   }
   if ( point->m_AlliesPresent && point->m_AxisPresent )
   {
    vgui::surface()->DrawSetColor( Color( 255, 90, 60, 255 ) );
    vgui::surface()->DrawOutlinedRect( x, y, x + m_Size, y + m_Size );
   }
   x += m_Size + m_Separator;
  }
 }
 CPanelAnimationVarAliasType( int, m_Size, "iconsize", "24", "proportional_int" );
 CPanelAnimationVarAliasType( int, m_StartX, "icon_start_x", "15", "proportional_int" );
 CPanelAnimationVarAliasType( int, m_StartY, "icon_start_y", "15", "proportional_int" );
 CPanelAnimationVarAliasType( int, m_Separator, "separator_width", "5", "proportional_int" );
};
DECLARE_NAMED_HUDELEMENT( CHudDODSObjectives, HudObjectiveIcons );
#endif
