//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Draws the normal TF2 or HL2 HUD.
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "clientmode_hl2mpnormal.h"
#include "vgui_int.h"
#include "hud.h"
#include <vgui/IInput.h>
#include <vgui/IPanel.h>
#include <vgui/ISurface.h>
#include <vgui_controls/AnimationController.h>
#include "iinput.h"
#include "hl2mpclientscoreboard.h"
#include "hl2mptextwindow.h"
#include "ienginevgui.h"
#ifdef DODS_REMAKE
#include "dods/ui/dods_teammenu.h"
#include "dods/ui/dods_classmenu.h"
#include "dods/dods_classes.h"
#include "hl2mp_gamerules.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
vgui::HScheme g_hVGuiCombineScheme = 0;


// Instance the singleton and expose the interface to it.
IClientMode *GetClientModeNormal()
{
	static ClientModeHL2MPNormal g_ClientModeNormal;
	return &g_ClientModeNormal;
}

ClientModeHL2MPNormal* GetClientModeHL2MPNormal()
{
	Assert( dynamic_cast< ClientModeHL2MPNormal* >( GetClientModeNormal() ) );

	return static_cast< ClientModeHL2MPNormal* >( GetClientModeNormal() );
}

//-----------------------------------------------------------------------------
// Purpose: this is the viewport that contains all the hud elements
//-----------------------------------------------------------------------------
class CHudViewport : public CBaseViewport
{
private:
	DECLARE_CLASS_SIMPLE( CHudViewport, CBaseViewport );

protected:
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme )
	{
		BaseClass::ApplySchemeSettings( pScheme );

		gHUD.InitColors( pScheme );

		SetPaintBackgroundEnabled( false );
	}

	virtual IViewPortPanel *CreatePanelByName( const char *szPanelName );
#ifdef DODS_REMAKE
	virtual void CreateDefaultPanels() OVERRIDE
	{
		BaseClass::CreateDefaultPanels();
		AddNewPanel( CreatePanelByName( PANEL_TEAM ), "PANEL_TEAM" );
		AddNewPanel( CreatePanelByName( PANEL_CLASS_ALLIES ), "PANEL_CLASS_ALLIES" );
		AddNewPanel( CreatePanelByName( PANEL_CLASS_AXIS ), "PANEL_CLASS_AXIS" );
	}
#endif
};

int ClientModeHL2MPNormal::GetDeathMessageStartHeight( void )
{
	return m_pViewport->GetDeathMessageStartHeight();
}

IViewPortPanel* CHudViewport::CreatePanelByName( const char *szPanelName )
{
	IViewPortPanel* newpanel = NULL;

#ifdef DODS_REMAKE
	if ( Q_strcmp( PANEL_TEAM, szPanelName ) == 0 )
		return new CDODSTeamMenu( this );
	if ( FStrEq( szPanelName, PANEL_CLASS_ALLIES ) ) return new CDODSClassMenu( this, TEAM_AMERICANS );
	if ( FStrEq( szPanelName, PANEL_CLASS_AXIS ) ) return new CDODSClassMenu( this, TEAM_GERMANS );
#endif

	if ( Q_strcmp( PANEL_SCOREBOARD, szPanelName) == 0 )
	{
		newpanel = new CHL2MPClientScoreBoardDialog( this );
		return newpanel;
	}
	else if ( Q_strcmp(PANEL_INFO, szPanelName) == 0 )
	{
		newpanel = new CHL2MPTextWindow( this );
		return newpanel;
	}
	else if ( Q_strcmp(PANEL_SPECGUI, szPanelName) == 0 )
	{
		newpanel = new CHL2MPSpectatorGUI( this );	
		return newpanel;
	}

	
	return BaseClass::CreatePanelByName( szPanelName ); 
}

//-----------------------------------------------------------------------------
// ClientModeHLNormal implementation
//-----------------------------------------------------------------------------
ClientModeHL2MPNormal::ClientModeHL2MPNormal()
{
	m_pViewport = new CHudViewport();
	m_pViewport->Start( gameuifuncs, gameeventmanager );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
ClientModeHL2MPNormal::~ClientModeHL2MPNormal()
{
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void ClientModeHL2MPNormal::Init()
{
	BaseClass::Init();

	// Load up the combine control panel scheme
	g_hVGuiCombineScheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), "resource/CombinePanelScheme.res", "CombineScheme" );
	if (!g_hVGuiCombineScheme)
	{
		Warning( "Couldn't load combine panel scheme!\n" );
	}
}



