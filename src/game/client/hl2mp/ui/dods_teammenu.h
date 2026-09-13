#ifndef DODS_TEAMMENU_H
#define DODS_TEAMMENU_H
#pragma once

#include "teammenu.h"

#ifdef DODS_REMAKE
class CDODSTeamMenu : public CTeamMenu
{
	DECLARE_CLASS_SIMPLE( CDODSTeamMenu, CTeamMenu );
public:
	explicit CDODSTeamMenu( IViewPort *viewport );
	virtual void ShowPanel( bool show ) OVERRIDE;
	virtual void Update() OVERRIDE;
	virtual void OnTick() OVERRIDE;
	virtual void PerformLayout() OVERRIDE;
	virtual vgui::Panel *CreateControlByName( const char *name ) OVERRIDE;

protected:
	virtual void OnCommand( const char *command ) OVERRIDE;
	virtual void OnKeyCodePressed( vgui::KeyCode code ) OVERRIDE;
	virtual void LoadMapPage( const char *mapName ) OVERRIDE;
	virtual void PaintBackground() OVERRIDE {}
	MESSAGE_FUNC_CHARPTR( OnPreviewTeam, "PreviewTeam", team );

private:
	vgui::EditablePanel *m_pPreview;
	ButtonCode_t m_ChangeTeamKey;
};
#endif
#endif
