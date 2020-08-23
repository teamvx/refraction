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

static ConVar cl_showfps( "cl_showfps", "0", 0, "Draw fps meter at top of screen" );
static ConVar cl_showpos( "cl_showpos", "0", 0, "Draw current position at top of screen" );
static ConVar cl_showmod( "cl_showmod", "1", 0, "Draw mod watermark and version at top of screen" );

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

    void InitAverages()
    {
        m_AverageFPS = -1;
        m_lastRealTime = -1;
        m_high = -1;
        m_low = -1;
    }

    vgui::HFont m_hFont;
    float       m_AverageFPS;
    float       m_lastRealTime;
    int         m_high;
    int         m_low;
    bool        m_bLastDraw;
    int         m_BatteryPercent;
    float       m_lastBatteryPercent;
};

#define FPS_PANEL_WIDTH 360

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *parent -
//-----------------------------------------------------------------------------
CFPSPanel::CFPSPanel( vgui::VPANEL parent ) : BaseClass( NULL, "CFPSPanel" )
{
    SetParent( parent );
    SetVisible( false );
    SetCursor( null );

    SetFgColor( Color( 0, 0, 0, 255 ) );
    SetPaintBackgroundEnabled( false );

    m_hFont = 0;
    m_BatteryPercent = -1;
    m_lastBatteryPercent = -1.0f;

    ComputeSize();

    vgui::ivgui()->AddTickSignal( GetVPanel(), 250 );
    m_bLastDraw = false;
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
    vgui::ipanel()->GetSize( GetVParent(), wide, tall );

    int x = wide - FPS_PANEL_WIDTH;
    int y = 0;
    SetPos( x, y );
    SetSize( FPS_PANEL_WIDTH, 6 * vgui::surface()->GetFontTall( m_hFont ) + 8 );
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
    if( IsVisible() != bVisible ) {
        SetVisible( bVisible );
    }
}

//-----------------------------------------------------------------------------
// Purpose:
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
bool CFPSPanel::ShouldDraw( void )
{
    if( g_bDisplayParticlePerformance ) {
        return true;
    }

    if( ( !cl_showfps.GetBool() || gpGlobals->absoluteframetime <= 0 ) && !cl_showpos.GetBool() && !cl_showmod.GetBool() ) {
        m_bLastDraw = false;
        return false;
    }

    if( !m_bLastDraw ) {
        m_bLastDraw = true;
        InitAverages();
    }
    return true;
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void GetFPSColor( int nFps, unsigned char ucColor[3] )
{
    // Red color - [00..29] fps
    ucColor[0] = 0xFF;
    ucColor[1] = 0x00;
    ucColor[2] = 0x00;
    
    // Cyan color - [100..N] fps
    if( nFps >= 100 ) {
        ucColor[0] = 0x00;
        ucColor[1] = 0xFF;
        ucColor[2] = 0xFF;
        return;
    }

    // Green color - [50..99] fps
    if( nFps >= 50 ) {
        ucColor[0] = 0x00;
        ucColor[1] = 0xFF;
        ucColor[2] = 0x00;
        return;
    }

    // Yellow color - [30..49] fps
    if( nFps >= 30 ) {
        ucColor[0] = 0xFF;
        ucColor[1] = 0xFF;
        ucColor[2] = 0x00;
        return;
    }
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  :
//-----------------------------------------------------------------------------
void CFPSPanel::Paint()
{
    int i = 0;
    int x = 2;
    int iFontTall = g_pVGuiSurface->GetFontTall( m_hFont );

    if( cl_showmod.GetBool() ) {
#ifdef _DEBUG
        const char *pszConfig = "DEBUG";
#else
        const char *pszConfig = "RELEASE";
#endif
        g_pMatSystemSurface->DrawColoredText( m_hFont, x, 2 + ( i++ * iFontTall ), 255, 255, 255, 255, "%s %u.%u.%u-%s", MOD_WATERMARK_STR, MOD_VERSION_MAJOR, MOD_VERSION_MINOR, MOD_VERSION_PATCH, pszConfig );
    }

    float frametime = gpGlobals->frametime;
    if( cl_showfps.GetBool() && frametime > 0.0 ) {
        if( m_lastRealTime != -1.0 ) {
            float fps = ( 1.0 / frametime );
            unsigned char ucColor[3] = { 0x00 };

            // Average FPS
            if( m_AverageFPS < 0.0 ) {
                m_AverageFPS = fps;
                m_high = (int)m_AverageFPS;
                m_low = (int)m_AverageFPS;
            }
            else {
                m_AverageFPS *= 0.9;
                m_AverageFPS += ( fps * 0.1 );
            }

            int fps_i = (int)fps;
            if( fps_i < m_low ) m_low = fps_i;
            if( fps_i > m_high ) m_high = fps_i;

            GetFPSColor( fps_i, ucColor );
            g_pMatSystemSurface->DrawColoredText( m_hFont, x, 2 + ( i++ * iFontTall ), ucColor[0], ucColor[1], ucColor[2], 0xFF, "%d fps (%d..%d), %.2fms at %s", fps_i, m_low, m_high, frametime * 1000.0, engine->GetLevelName() );
        }
    }
    m_lastRealTime = gpGlobals->realtime;

    int cl_showpos_v = cl_showpos.GetInt();
    if( cl_showpos_v > 0 ) {
        C_BasePlayer *player = C_BasePlayer::GetLocalPlayer();
        Vector origin = MainViewOrigin();
        QAngle angles = MainViewAngles();
        if( player && cl_showpos_v == 2 ) {
            origin = player->GetAbsOrigin();
            angles = player->GetAbsAngles();
        }

        // Print pos & angles
        g_pMatSystemSurface->DrawColoredText( m_hFont, x, 2 + ( i++ * iFontTall ), 255, 255, 255, 255, "pos:  %.02f %.02f %.02f", origin.x, origin.y, origin.z );
        g_pMatSystemSurface->DrawColoredText( m_hFont, x, 2 + ( i++ * iFontTall ), 255, 255, 255, 255, "ang:  %.02f %.02f %.02f", angles.x, angles.y, angles.z );

        // Velocity
        Vector velocity( 0 );
        if( player ) {
            velocity = player->GetLocalVelocity();
        }
        g_pMatSystemSurface->DrawColoredText( m_hFont, x, 2 + ( i++ * iFontTall ), 255, 255, 255, 255, "vel:  %.04f", velocity.Length() );
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
