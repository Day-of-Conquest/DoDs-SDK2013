#ifndef DODS_CLASSMENU_H
#define DODS_CLASSMENU_H
#pragma once
#ifdef DODS_REMAKE
#include "classmenu.h"
class CDODSClassMenu : public CClassMenu
{
 DECLARE_CLASS_SIMPLE( CDODSClassMenu, CClassMenu );
public:
 CDODSClassMenu( IViewPort *viewport, int team );
 const char *GetName() OVERRIDE;
 void ShowPanel( bool show ) OVERRIDE;
 void Reset() OVERRIDE {}
 void Update() OVERRIDE;
 void OnTick() OVERRIDE;
 vgui::Panel *CreateControlByName( const char *name ) OVERRIDE;
protected:
 void OnCommand( const char *command ) OVERRIDE;
 void OnKeyCodePressed( vgui::KeyCode code ) OVERRIDE;
 MESSAGE_FUNC_CHARPTR( OnPreviewClass, "PreviewClass", command );
 MESSAGE_FUNC( OnAutoKillChanged, "CheckButtonChecked" );
private:
 int m_Team;
 ButtonCode_t m_ClassKey;
 vgui::EditablePanel *m_Info;
 vgui::EditablePanel *m_PreviewPages[9]; // Eight classes plus random.
 int m_ActivePreview;
};
#endif
#endif
