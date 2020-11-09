//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=====================================================================================//
#include "cbase.h"
#include "ifpspanel.h"
#include "vgui/IVGui.h"
#include "vgui/ISurface.h"
#include "vgui_controls/Panel.h"
#include "VGuiMatSurface/IMatSystemSurface.h"
#include "view.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar cl_showfps( "cl_showfps", "0", FCVAR_ARCHIVE, "Draw fps meter at top of screen" );
static ConVar cl_showpos( "cl_showpos", "0", FCVAR_ARCHIVE, "Draw current position at top of screen" );

extern bool g_bDisplayParticlePerformance;
int GetParticlePerformance();

//-----------------------------------------------------------------------------
// Purpose: Framerate indicator panel
//-----------------------------------------------------------------------------
class CFPSPanel : public vgui::Panel {
    DECLARE_CLASS_SIMPLE( CFPSPanel, vgui::Panel );
public:
    CFPSPanel( vgui::VPANEL parent );
    virtual ~CFPSPanel( void );
    virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
    virtual void Paint();
    virtual void OnTick( void );
    virtual bool ShouldDraw( void );

protected:
    MESSAGE_FUNC_INT_INT( OnScreenSizeChanged, "OnScreenSizeChanged", oldwide, oldtall );

private:
    void ComputeSize( void );

    vgui::HFont m_hFont;
    float m_flFPS;
    char m_szWatermark[256];
};

#define FPS_PANEL_WIDTH 400

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *parent -
//-----------------------------------------------------------------------------
CFPSPanel::CFPSPanel( vgui::VPANEL parent ) : BaseClass( NULL, "CFPSPanel" ), m_hFont( 0 ), m_flFPS( -1.0 )
{
    SetParent( parent );
    SetVisible( false );
    SetCursor( null );

    SetFgColor( Color( 0, 0, 0, 255 ) );
    SetPaintBackgroundEnabled( false );

    ComputeSize();
    g_pVGui->AddTickSignal( GetVPanel(), 250 );

#if MOD_IS_DEV_BUILD
    Q_snprintf( m_szWatermark, sizeof( m_szWatermark ), "%s %u.%u.%u devbuild (%s %s)", MOD_NAME, MOD_VERSION_MAJOR, MOD_VERSION_MINOR, MOD_VERSION_PATCH, __DATE__, __TIME__ );
#else
    Q_snprintf( m_szWatermark, sizeof( m_szWatermark ), "%s %u.%u.%u (%s %s)", MOD_NAME, MOD_VERSION_MAJOR, MOD_VERSION_MINOR, MOD_VERSION_PATCH, __DATE__, __TIME__ );
#endif
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
CFPSPanel::~CFPSPanel( void )
{

}

//-----------------------------------------------------------------------------
// Purpose: Updates panel to handle the new screen size
//-----------------------------------------------------------------------------
void CFPSPanel::OnScreenSizeChanged( int iOldWide, int iOldTall )
{
    BaseClass::OnScreenSizeChanged( iOldWide, iOldTall );
    ComputeSize();
}

//-----------------------------------------------------------------------------
// Purpose: Computes panel's desired size and position
//-----------------------------------------------------------------------------
void CFPSPanel::ComputeSize( void )
{
    int wide, tall;
    g_pVGuiPanel->GetSize( GetVParent(), wide, tall );
    
    int x = wide - FPS_PANEL_WIDTH;
    int y = 0;
    SetPos( x, y );
    SetSize( FPS_PANEL_WIDTH, 6 * g_pVGuiSurface->GetFontTall( m_hFont ) + 12 );
}

void CFPSPanel::ApplySchemeSettings( vgui::IScheme *pScheme )
{
    BaseClass::ApplySchemeSettings( pScheme );

    m_hFont = pScheme->GetFont( "DefaultFixedOutline" );
    Assert( m_hFont );

    ComputeSize();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CFPSPanel::OnTick( void )
{
    bool bVisible = ShouldDraw();
    if( IsVisible() != bVisible )
        SetVisible( bVisible );
}

//-----------------------------------------------------------------------------
// Purpose:
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CFPSPanel::ShouldDraw( void )
{
    // TODO: Uncomment this when development is done...
    //return g_bDisplayParticlePerformance || cl_showfps.GetBool() || cl_showpos.GetBool();
    return true;
}

static void UTIL_GetFPSColor( int iFPS, Color &c )
{
    if( iFPS >= 100 ) {
        c[0] = 0x00;
        c[1] = 0xFF;
        c[2] = 0xFF;
        return;
    }

    if( iFPS >= 50 ) {
        c[0] = 0x00;
        c[1] = 0xFF;
        c[2] = 0x00;
        return;
    }

    if( iFPS >= 30 ) {
        c[0] = 0xFF;
        c[1] = 0xFF;
        c[2] = 0x00;
        return;
    }

    c[0] = 0xFF;
    c[1] = 0x00;
    c[2] = 0x00;
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
//-----------------------------------------------------------------------------
void CFPSPanel::Paint()
{
    int ypos = 2;
    int iFontTall = g_pVGuiSurface->GetFontTall( m_hFont );
    
    // Draw mod/game watermark...
    g_pMatSystemSurface->DrawColoredText( m_hFont, 2, ypos, 255, 255, 255, 255, "%s", m_szWatermark );
    ypos += 2 + iFontTall;

    // Draw FPS
    float flFrametime = gpGlobals->absoluteframetime;
    if( cl_showfps.GetBool() ) {
        float flFPS = ( 1.0 / flFrametime );
        if( m_flFPS >= 0.0 ) {
            m_flFPS *= 0.9;
            m_flFPS += ( flFPS * 0.1 );
        }
        else m_flFPS = flFPS;

        Color c; UTIL_GetFPSColor( m_flFPS, c );
        g_pMatSystemSurface->DrawColoredText( m_hFont, 2, ypos, c[0], c[1], c[2], 255, "%.02f fps (%.02fms) at %s", m_flFPS, flFrametime * 1000.0, engine->GetLevelName() );
        ypos += 2 + iFontTall;
    }

    // Draw position, angles and speed
    int cl_showpos_int = cl_showpos.GetInt();
    if( cl_showpos_int > 0 ) {
        C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
        Vector origin = MainViewOrigin();
        QAngle angles = MainViewAngles();
        Vector velocity = Vector( 0 );
        if( pPlayer != NULL ) {
            if( cl_showpos_int >= 2 ) {
                origin = pPlayer->GetLocalOrigin();
                angles = pPlayer->GetLocalAngles();
            }
            velocity = pPlayer->GetAbsVelocity();
        }

        // Position
        g_pMatSystemSurface->DrawColoredText( m_hFont, 2, ypos, 255, 255, 255, 255, "pos: %.02f %.02f %.02f", origin.x, origin.y, origin.z );
        ypos += 2 + iFontTall;

        // Rotation
        g_pMatSystemSurface->DrawColoredText( m_hFont, 2, ypos, 255, 255, 255, 255, "ang: %.02f %.02f %.02f", angles.x, angles.y, angles.z );
        ypos += 2 + iFontTall;

        // Speed
        g_pMatSystemSurface->DrawColoredText( m_hFont, 2, ypos, 255, 255, 255, 255, "spd: %.04f", velocity.Length() );
        ypos += 2 + iFontTall;
    }
}

class CFPS : public IFPSPanel {
private:
    CFPSPanel *fpsPanel;
public:
    CFPS( void )
    {
        fpsPanel = NULL;
    }

    void Create( vgui::VPANEL parent )
    {
        fpsPanel = new CFPSPanel( parent );
    }

    void Destroy( void )
    {
        if( fpsPanel ) {
            fpsPanel->SetParent( ( vgui::Panel * )NULL );
            delete fpsPanel;
            fpsPanel = NULL;
        }
    }
};

static CFPS g_FPSPanel;
IFPSPanel *fps = (IFPSPanel *)&g_FPSPanel;
