#ifndef GAMEUI_BASEPANEL_H
#define GAMEUI_BASEPANEL_H
#ifdef _WIN32
#pragma once
#endif
#include "vgui_controls/Panel.h"

class CBasePanel : public vgui::Panel {
    DECLARE_CLASS_SIMPLE( CBasePanel, vgui::Panel );
public:
    CBasePanel( vgui::VPANEL hParent );
    void ApplySchemeSettings( vgui::IScheme *pScheme );
};

#endif
