#include "cbase.h"
#ifdef DODS_REMAKE
#include "dods_teammenu.h"
#include "c_team.h"
#include "hl2mp_gamerules.h"
#include "IconPanel.h"
#include "IGameUIFuncs.h"
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/HTML.h>
#include <vgui_controls/RichText.h>
#include "filesystem.h"
#include "hud.h"
#include "tier0/memdbgon.h"

extern IGameUIFuncs *gameuifuncs;

namespace
{
class CArmyChoiceButton : public vgui::Button
{
	DECLARE_CLASS_SIMPLE( CArmyChoiceButton, vgui::Button );
public:
	CArmyChoiceButton( vgui::Panel *parent ) : BaseClass( parent, "", "" ) {}
	virtual void OnCursorEntered() OVERRIDE
	{
		BaseClass::OnCursorEntered();
		Preview();
	}
	virtual void OnSetFocus() OVERRIDE
	{
		BaseClass::OnSetFocus();
		Preview();
	}
	virtual void PaintBackground() OVERRIDE
	{
		vgui::surface()->DrawSetColor( GetBgColor() );
		const int cut = GetTall() / 4;
		for ( int y = 0; y < GetTall(); ++y )
		{
			const int inset = MAX( 0, y - ( GetTall() - cut ) );
			vgui::surface()->DrawFilledRect( 0, y, GetWide() - inset, y + 1 );
		}
	}
	virtual void PaintBorder() OVERRIDE
	{
		int w = GetWide() - 1, h = GetTall() - 1, cut = GetTall() / 4;
		int x[] = { 0, w, w, w - cut, 0, 0 };
		int y[] = { 0, 0, h - cut, h, h, 0 };
		vgui::surface()->DrawSetColor( GetFgColor() );
		vgui::surface()->DrawPolyLine( x, y, ARRAYSIZE( x ) );
	}
	virtual void ApplySchemeSettings( vgui::IScheme *scheme ) OVERRIDE
	{
		BaseClass::ApplySchemeSettings( scheme );
		// Some supplied schemes omit the font referenced by TeamMenu.res.
		vgui::HFont font = scheme->GetFont( "ClassMenuDefault", IsProportional() );
		SetFont( font ? font : scheme->GetFont( "Default", IsProportional() ) );
	}
private:
	void Preview()
	{
		if ( IsEnabled() && ( FStrEq( GetName(), "allies" ) || FStrEq( GetName(), "axis" ) ) )
			PostActionSignal( new KeyValues( "PreviewTeam", "team", GetName() ) );
	}
};

class CArmyMenuBackdrop : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE( CArmyMenuBackdrop, vgui::EditablePanel );
public:
	CArmyMenuBackdrop( vgui::Panel *parent ) : BaseClass( parent, "MenuBackground" )
	{
		SetProportional( true );
		LoadControlSettings( "resource/ui/menubackground.res" );
		SetMouseInputEnabled( false );
		SetKeyBoardInputEnabled( false );
	}
	virtual void PaintBackground() OVERRIDE
	{
		vgui::Panel *title = GetParent()->FindChildByName( "joinTeam" );
		vgui::Panel *cancel = GetParent()->FindChildByName( "CancelButton" );
		vgui::Panel *autoButton = GetParent()->FindChildByName( "autobutton" );
		if ( !title || !cancel || !autoButton )
			return;
		int left, top, right, bottom;
		title->GetPos( left, top );
		cancel->GetPos( right, bottom );
		bottom += cancel->GetTall();
		autoButton->GetPos( right, top );
		right += autoButton->GetWide();
		int unused;
		title->GetPos( unused, top );
		const int margin = title->GetTall() / 2;
		vgui::IScheme *scheme = vgui::scheme()->GetIScheme( GetScheme() );
		vgui::surface()->DrawSetColor( scheme->GetColor( "ClassMenuDark", GetBgColor() ) );
		vgui::surface()->DrawFilledRect( left - margin, top - margin, right + margin, bottom + margin );
		vgui::surface()->DrawSetColor( scheme->GetColor( "TransparentWhite", GetFgColor() ) );
		vgui::surface()->DrawLine( left - margin, top - margin, right + margin, top - margin );
	}
};
}

CDODSTeamMenu::CDODSTeamMenu( IViewPort *viewport ) : BaseClass( viewport ),
	m_pPreview( NULL ), m_ChangeTeamKey( BUTTON_CODE_INVALID )
{
	g_pVGuiLocalize->AddFile( "resource/dod_%language%.txt" );
	g_pVGuiLocalize->AddFile( "resource/doc_%language%.txt" );
	LoadControlSettings( "resource/ui/teammenu.res" );
	m_pPreview = new vgui::EditablePanel( this, "ArmyPreview" );
	m_pPreview->SetProportional( true );
	m_pPreview->SetMouseInputEnabled( false );
	m_pPreview->SetKeyBoardInputEnabled( false );
	vgui::ivgui()->AddTickSignal( GetVPanel() );
	SetVisible( false );
}

vgui::Panel *CDODSTeamMenu::CreateControlByName( const char *name )
{
	if ( !Q_stricmp( name, "DODButton" ) || !Q_stricmp( name, "DODMouseOverPanelButton" ) )
		return new CArmyChoiceButton( this );
	if ( !Q_stricmp( name, "CDODMenuBackground" ) )
		return new CArmyMenuBackdrop( this );
	if ( !Q_stricmp( name, "CIconPanel" ) )
		return new CIconPanel( this, "" );
	return BaseClass::CreateControlByName( name );
}

void CDODSTeamMenu::OnPreviewTeam( const char *team )
{
	if ( !m_pPreview )
		return;
	if ( FStrEq( team, "allies" ) )
		m_pPreview->LoadControlSettings( "classes/allies.res" );
	else if ( FStrEq( team, "axis" ) )
		m_pPreview->LoadControlSettings( "classes/axis.res" );
	else
		return;
	m_pPreview->SetVisible( true );
}

void CDODSTeamMenu::PerformLayout()
{
	BaseClass::PerformLayout();
	vgui::Panel *placeholder = FindChildByName( "TeamImagePanel" );
	if ( placeholder && m_pPreview )
	{
		int x, y, w, h;
		placeholder->GetBounds( x, y, w, h );
		m_pPreview->SetBounds( x, y, w, h );
		placeholder->SetVisible( false );
	}
}

void CDODSTeamMenu::ShowPanel( bool show )
{
	if ( show )
	{
		const char *iconNames[] = { "icon_obj_allies", "icon_obj_axis" };
		const char *iconPanels[] = { "icon_allies", "icon_axis" };
		if ( !gHUD.GetIcon( iconNames[0] ) || !gHUD.GetIcon( iconNames[1] ) )
		{
			CUtlDict<CHudTexture *, int> textures;
			LoadHudTextures( textures, "scripts/doc/mod_textures", NULL );
			for ( int i = 0; i < ARRAYSIZE( iconNames ); ++i )
			{
				int index = textures.Find( iconNames[i] );
				if ( index != textures.InvalidIndex() && !gHUD.GetIcon( iconNames[i] ) )
					gHUD.AddSearchableHudIconToList( *textures[index] );
			}
			textures.PurgeAndDeleteElements();
		}
		for ( int i = 0; i < ARRAYSIZE( iconPanels ); ++i )
		{
			CIconPanel *icon = dynamic_cast<CIconPanel *>( FindChildByName( iconPanels[i] ) );
			if ( icon ) icon->SetIcon( iconNames[i] );
		}
		m_ChangeTeamKey = gameuifuncs->GetButtonCodeForBind( "changeteam" );
		Update();
		OnPreviewTeam( "allies" );
	}
	BaseClass::ShowPanel( show );
	SetKeyBoardInputEnabled( show );
	if ( show )
	{
		vgui::Panel *automatic = FindChildByName( "autobutton" );
		if ( automatic ) automatic->RequestFocus();
	}
}

void CDODSTeamMenu::Update()
{
	BaseClass::Update();
	OnTick();
}

void CDODSTeamMenu::LoadMapPage( const char *mapName )
{
	Q_strncpy( m_szMapName, mapName, sizeof( m_szMapName ) );
	vgui::HTML *html = dynamic_cast<vgui::HTML *>( FindChildByName( "MapInfoHTML" ) );
	char language[64], relative[MAX_PATH], absolute[MAX_PATH];
	engine->GetUILanguage( language, sizeof( language ) );
	Q_snprintf( relative, sizeof( relative ), "resource/maphtml/%s_%s.html", mapName, language );
	if ( !g_pFullFileSystem->FileExists( relative, "GAME" ) )
		Q_snprintf( relative, sizeof( relative ), "resource/maphtml/%s_english.html", mapName );
	if ( html && g_pFullFileSystem->FileExists( relative, "GAME" ) &&
		g_pFullFileSystem->RelativePathToFullPath( relative, "GAME", absolute, sizeof( absolute ) ) )
	{
		char url[MAX_PATH + 16];
		Q_FixSlashes( absolute, '/' );
		Q_snprintf( url, sizeof( url ), "file:///%s", absolute );
		html->OpenURL( url, NULL );
		html->SetVisible( true );
		m_pMapInfo->SetVisible( false );
		return;
	}
	BaseClass::LoadMapPage( mapName );
	if ( html ) html->SetVisible( false );
	m_pMapInfo->SetVisible( true );
}

void CDODSTeamMenu::OnTick()
{
	C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
	vgui::Panel *cancel = FindChildByName( "CancelButton" );
	if ( cancel ) cancel->SetVisible( player && player->GetTeamNumber() != TEAM_UNASSIGNED );
	ConVarRef spectators( "mp_allowspectators" );
	vgui::Panel *spec = FindChildByName( "specbutton" );
	if ( spec ) spec->SetVisible( spectators.IsValid() && spectators.GetBool() );
	const int teams[] = { TEAM_AMERICANS, TEAM_GERMANS };
	const char *labels[] = { "num_allies", "num_axis" };
	const char *singular[] = { "#teammenu_numAllies_1", "#teammenu_numAxis_1" };
	const char *plural[] = { "#teammenu_numAllies", "#teammenu_numAxis" };
	for ( int i = 0; i < ARRAYSIZE( teams ); ++i )
	{
		C_Team *team = GetGlobalTeam( teams[i] );
		vgui::Label *label = dynamic_cast<vgui::Label *>( FindChildByName( labels[i] ) );
		if ( !label ) continue;
		const int count = team ? team->Get_Number_Players() : 0;
		wchar_t number[16], text[128];
		V_snwprintf( number, ARRAYSIZE( number ), L"%d", count );
		const wchar_t *format = g_pVGuiLocalize->Find( count == 1 ? singular[i] : plural[i] );
		if ( format )
			g_pVGuiLocalize->ConstructString( text, sizeof( text ), format, 1, number );
		else
			V_snwprintf( text, ARRAYSIZE( text ), L"%ls: %d", i == 0 ? L"Americans" : L"Germans", count );
		label->SetText( text );
	}
}

void CDODSTeamMenu::OnCommand( const char *command )
{
	if ( FStrEq( command, "jointeam 0" ) || FStrEq( command, "jointeam 1" ) ||
		FStrEq( command, "jointeam 2" ) || FStrEq( command, "jointeam 3" ) )
	{
		engine->ClientCmd( command );
		m_pViewPort->ShowPanel( this, false );
	}
	else if ( FStrEq( command, "vguicancel" ) )
	{
		C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
		if ( player && player->GetTeamNumber() != TEAM_UNASSIGNED )
			m_pViewPort->ShowPanel( this, false );
	}
	else
		BaseClass::OnCommand( command );
}

void CDODSTeamMenu::OnKeyCodePressed( vgui::KeyCode code )
{
	if ( code == m_ChangeTeamKey || code == KEY_ESCAPE )
		m_pViewPort->ShowPanel( this, false );
	else
		BaseClass::OnKeyCodePressed( code );
}

CON_COMMAND_F( changeteam, "Open the team selection menu.", FCVAR_CLIENTCMD_CAN_EXECUTE | FCVAR_SERVER_CAN_EXECUTE )
{
	if ( C_BasePlayer::GetLocalPlayer() && gViewPortInterface && !engine->IsPlayingDemo() )
		gViewPortInterface->ShowPanel( PANEL_TEAM, true );
}
#endif
