#include "vgui/ISurface.h"
#include "basepanel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

CBasePanel::CBasePanel( vgui::VPANEL hParent ) : BaseClass( NULL, "GameUI2_BasePanel" )
{
    // The hParent is possibly PANEL_GAMEUIDLL
    SetParent( hParent );
}

void CBasePanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
    BaseClass::ApplySchemeSettings( pScheme );

    // Resize the panel to the screen size, otherwise
    // it will be just a little dot in the top-left corner.
    int iWide, iTall;
    g_pVGuiSurface->GetScreenSize( iWide, iTall );
    SetBounds( 0, 0, iWide, iTall );
}
