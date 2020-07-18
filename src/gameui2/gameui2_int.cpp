#include "cdll_int.h"
#include "engine/IEngineSound.h"
#include "ienginevgui.h"
#include "SoundEmitterSystem/isoundemittersystembase.h"
#include "ivrenderview.h"
#include "materialsystem/imaterialsystem.h"
#include "VGuiMatSurface/IMatSystemSurface.h"
#include "filesystem.h"
#include "vgui/ILocalize.h"
#include "gameui2_int.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CDllDemandLoader s_GameUIDll( "GameUI" );
static CGameUI2 s_GameUI2;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGameUI2, IGameUI2, GAMEUI2_INTERFACE_VERSION, s_GameUI2 );

void GameUI2_GetLocalizedString( const char *pszSource, wchar_t *szDestination, int iDestChars )
{
    const wchar_t *pszValue = g_pVGuiLocalize->Find( pszSource );
    if( pszValue ) {
        Q_wcsncpy( szDestination, pszValue, sizeof( wchar_t ) * iDestChars );
        return;
    }
    mbstowcs( szDestination, pszSource, iDestChars );
}

// IMPORTANT NOTE:
// When you add an interface, extern it in the gameui2_int.h
IVEngineClient *            g_pEngine = NULL;
IEngineSound *              g_pEngineSound = NULL;
IEngineVGui *               g_pEngineVGui = NULL;
ISoundEmitterSystemBase *   g_pSoundEmitterSystem = NULL;
IVRenderView *              g_pRenderView = NULL;
IMaterialSystem *           g_pMaterialSystem = NULL;
IMatSystemSurface *         g_pMatSystemSurface = NULL;
IFileSystem *               g_pFileSystem = NULL;
CGlobalVarsBase *           g_pGlobals = NULL;
IGameUI *                   g_pGameUI1 = NULL;
CGameUI2 *                  g_pGameUI2 = &s_GameUI2;

void CGameUI2::Initialize( CreateInterfaceFn appFactory, CGlobalVarsBase *pGlobals )
{
    MEM_ALLOC_CREDIT();
    ConnectTier1Libraries( &appFactory, 1 );
    ConnectTier2Libraries( &appFactory, 1 );
    ConnectTier3Libraries( &appFactory, 1 );
    ConVar_Register( FCVAR_CLIENTDLL );
    bool bInterfacesGood = true;

    // Setup common interfaces
    g_pEngine               = (IVEngineClient *)            appFactory( VENGINE_CLIENT_INTERFACE_VERSION, NULL );
    g_pEngineSound          = (IEngineSound *)              appFactory( IENGINESOUND_CLIENT_INTERFACE_VERSION, NULL );
    g_pEngineVGui           = (IEngineVGui *)               appFactory( VENGINE_VGUI_VERSION, NULL );
    g_pSoundEmitterSystem   = (ISoundEmitterSystemBase *)   appFactory( SOUNDEMITTERSYSTEM_INTERFACE_VERSION, NULL );
    g_pRenderView           = (IVRenderView *)              appFactory( VENGINE_RENDERVIEW_INTERFACE_VERSION, NULL );
    g_pMaterialSystem       = (IMaterialSystem *)           appFactory( MATERIAL_SYSTEM_INTERFACE_VERSION, NULL );
    g_pMatSystemSurface     = (IMatSystemSurface*)          appFactory( MAT_SYSTEM_SURFACE_INTERFACE_VERSION, NULL );
    g_pFileSystem           = (IFileSystem *)               appFactory( FILESYSTEM_INTERFACE_VERSION, NULL );
    g_pGlobals = pGlobals;

    // Setup GameUI011
    CreateInterfaceFn gameuiFactory = s_GameUIDll.GetFactory();
    if( gameuiFactory )
        g_pGameUI1 = (IGameUI *)gameuiFactory( GAMEUI_INTERFACE_VERSION, NULL );

    // Setup VGUI interfaces
    bInterfacesGood = ( bInterfacesGood && vgui::VGui_InitInterfacesList( GAMEUI_INTERFACE_VERSION, &appFactory, 1 ) );

    // Check if something failed
    bInterfacesGood = ( bInterfacesGood && g_pEngine );
    bInterfacesGood = ( bInterfacesGood && g_pEngineSound );
    bInterfacesGood = ( bInterfacesGood && g_pEngineVGui );
    bInterfacesGood = ( bInterfacesGood && g_pSoundEmitterSystem );
    bInterfacesGood = ( bInterfacesGood && g_pRenderView );
    bInterfacesGood = ( bInterfacesGood && g_pMaterialSystem );
    bInterfacesGood = ( bInterfacesGood && g_pMatSystemSurface );
    bInterfacesGood = ( bInterfacesGood && g_pFileSystem );
    bInterfacesGood = ( bInterfacesGood && g_pGlobals );
    bInterfacesGood = ( bInterfacesGood && g_pGameUI1 );

    if( !bInterfacesGood ) {
        Error( "CGameUI2::Initialize() failed to get necessary interfaces\n" );
        // Uh oh, there's a crash :(
    }

    m_pBasePanel = new CBasePanel( g_pEngineVGui->GetPanel( PANEL_GAMEUIDLL ) );
    g_pGameUI1->SetMainMenuOverride( m_pBasePanel->GetVPanel() );
    GameUI2_GetAnimationController()->SetParent( m_pBasePanel );
}

void CGameUI2::Shutdown( void )
{
    g_pGameUI1->SetMainMenuOverride( ( vgui::VPANEL )0 );
    GameUI2_GetAnimationController()->SetParent( ( vgui::VPANEL )0 );
    delete m_pBasePanel;

    ConVar_Unregister();
    DisconnectTier3Libraries();
    DisconnectTier2Libraries();
    DisconnectTier1Libraries();
}

void CGameUI2::VidInit( void )
{
    int iUpdated = 0;
    for( CUIElementReg *pItem = CUIElementReg::s_pElements; pItem; pItem = pItem->m_pNext ) {
        if( pItem->m_pElement ) {
            // Initialize() usually loads data from scheme, so here we are.
            pItem->m_pElement->Initialize();
            iUpdated++;
        }
    }

    GameUI2_Warning( "GameUI2: VidInit() called, %d panels should be updated...\n", iUpdated );
}

void CGameUI2::PostInit( void )
{
    vgui::HScheme hScheme = g_pVGuiSchemeManager->LoadSchemeFromFile( "resource/uischeme.res", "UIScheme" );
    GameUI2_GetAnimationController()->SetScheme( hScheme );

    // Setup the panels
    int iFound = 0;
    for( CUIElementReg *pItem = CUIElementReg::s_pElements; pItem; pItem = pItem->m_pNext ) {
        pItem->Create();
        pItem->m_pElement->SetParent( m_pBasePanel );
        pItem->m_pElement->SetScheme( hScheme );
        pItem->m_pElement->Initialize();
        iFound++;
    }

    GameUI2_Message( "GameUI2: Initialized %d panels.\n", iFound );
}

void CGameUI2::PreShutdown( void )
{
    // Shutdown the panels
    for( CUIElementReg *pItem = CUIElementReg::s_pElements; pItem; pItem = pItem->m_pNext ) {
        pItem->m_pElement->Shutdown();
        pItem->Delete();    // <-- Automatically unparents
    }
}

void CGameUI2::OnUpdate( void )
{
    GameUI2_GetAnimationController()->UpdateAnimations( g_pGlobals->curtime );
}

void CGameUI2::LevelInitPreEntity( const char *pMapName )
{

}

void CGameUI2::LevelInitPostEntity( void )
{

}

void CGameUI2::LevelShutdown( void )
{
    GameUI2_GetAnimationController()->UpdateAnimations( g_pGlobals->curtime );
    GameUI2_GetAnimationController()->RunAllAnimationsToCompletion();
}

CBaseUIElement::CBaseUIElement( const char *pszElementName ) : BaseClass( NULL, pszElementName )
{

}

void CBaseUIElement::Initialize()
{

}

void CBaseUIElement::Shutdown()
{

}

void CBaseUIElement::SetPropBounds( int xpos, int ypos, int wide, int tall )
{
    xpos = g_pVGuiSchemeManager->GetProportionalScaledValue( xpos );
    ypos = g_pVGuiSchemeManager->GetProportionalScaledValue( ypos );
    wide = g_pVGuiSchemeManager->GetProportionalScaledValue( wide );
    tall = g_pVGuiSchemeManager->GetProportionalScaledValue( tall );
    SetBounds( xpos, ypos, wide, tall );
}

void CBaseUIElement::SetPropSize( int wide, int tall )
{
    wide = g_pVGuiSchemeManager->GetProportionalScaledValue( wide );
    tall = g_pVGuiSchemeManager->GetProportionalScaledValue( tall );
    SetSize( wide, tall );
}

void CBaseUIElement::SetPropPos( int xpos, int ypos )
{
    xpos = g_pVGuiSchemeManager->GetProportionalScaledValue( xpos );
    ypos = g_pVGuiSchemeManager->GetProportionalScaledValue( ypos );
    SetPos( xpos, ypos );
}

CUIElementReg * CUIElementReg::s_pElements = NULL;

CUIElementReg::CUIElementReg( CreateElementFn pCreateFn, DeleteElementFn pDeleteFn ) : m_pCreateFn( pCreateFn ), m_pDeleteFn( pDeleteFn )
{
    m_pNext = s_pElements;
    s_pElements = this;
}

void CUIElementReg::Create()
{
    if( !m_pElement ) {
        m_pCreateFn( &m_pElement );
    }
}

void CUIElementReg::Delete()
{
    if( m_pElement ) {
        m_pElement->SetParent( ( vgui::VPANEL )0 );
        m_pDeleteFn( &m_pElement );
    }
}
