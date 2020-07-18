#include "gameui2_int.h"
#include "vgui/ISurface.h"
#include "tier1/KeyValues.h"
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CGameLogo : public CBaseUIElement {
    DECLARE_CLASS_SIMPLE( CGameLogo, CBaseUIElement );
public:
    CGameLogo();
    void Initialize();
    void Paint();

private:
    wchar_t m_szLabel[256];
    vgui::HFont m_hFont;
    Color m_color;
};
REGISTER_UI_ELEMENT( CGameLogo );

CON_COMMAND( ui_reloadlogo, "" )
{
    if( s_CGameLogo_reg.m_pElement ) {
        s_CGameLogo_reg.m_pElement->Initialize();
    }
}

CGameLogo::CGameLogo() : BaseClass( "GameLogo" )
{
    // ????
    SetPaintBackgroundEnabled( false );
}

void CGameLogo::Initialize()
{
    vgui::IScheme *pScheme = g_pVGuiSchemeManager->GetIScheme( GetScheme() );

    int iWidth  = Q_atoi( pScheme->GetResourceString( "GameLogo.Width" ) );
    int iHeight = Q_atoi( pScheme->GetResourceString( "GameLogo.Height" ) );
    int iXpos   = Q_atoi( pScheme->GetResourceString( "GameLogo.X" ) );
    int iYpos   = Q_atoi( pScheme->GetResourceString( "GameLogo.Y" ) );
    SetPropBounds( iXpos, iYpos, iWidth, iHeight );

    m_hFont = pScheme->GetFont( pScheme->GetResourceString( "GameLogo.Font" ), true );
    m_color = pScheme->GetColor( pScheme->GetResourceString( "GameLogo.Color" ), Color( 0, 0, 0, 255 ) );

    KeyValues *pGameinfoKV = new KeyValues( "Gameinfo" );
    pGameinfoKV->LoadFromFile( g_pFileSystem, "gameinfo.txt", "GAME" );
    Q_wcsncpy( m_szLabel, pGameinfoKV->GetWString( "gameui2_title", L"REFRACTION" ), sizeof( m_szLabel ) );
    pGameinfoKV->deleteThis();
}

void CGameLogo::Paint()
{
    g_pVGuiSurface->DrawSetTextColor( m_color );
    g_pVGuiSurface->DrawSetTextFont( m_hFont );
    g_pVGuiSurface->DrawSetTextPos( 0, 0 );
    g_pVGuiSurface->DrawPrintText( m_szLabel, Q_wcslen( m_szLabel ) );
}
