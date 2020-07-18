#ifndef GAMEUI2_INT_H
#define GAMEUI2_INT_H
#ifdef _WIN32
#pragma once
#endif
#include "GameUI/IGameUI.h"
#include "GameUI/IGameUI2.h"
#include "vgui_controls/AnimationController.h"
#include "vgui_controls/Panel.h"
#include "basepanel.h"

#define GameUI2_GetAnimationController() vgui::GetAnimationController()
void GameUI2_GetLocalizedString( const char *pszSource, wchar_t *szDestination, int iDestChars );

class CGameUI2 : public IGameUI2 {
public:
    void Initialize( CreateInterfaceFn appSystemFactory, CGlobalVarsBase *pGlobals );
    void Shutdown( void );
    void VidInit( void );

    void PostInit( void );
    void PreShutdown( void );
    void OnUpdate( void );
    void LevelInitPreEntity( const char *pMapName );
    void LevelInitPostEntity( void );
    void LevelShutdown( void );

    CBasePanel * GetBasePanel( void );

private:
    CBasePanel * m_pBasePanel = NULL;
};

class CBaseUIElement : public vgui::Panel {
    DECLARE_CLASS_SIMPLE( CBaseUIElement, vgui::Panel );

public:
    CBaseUIElement( const char *pszElementName );
    virtual void Initialize();
    virtual void Shutdown();

    void SetPropBounds( int xpos, int ypos, int wide, int tall );
    void SetPropSize( int wide, int tall );
    void SetPropPos( int xpos, int ypos );
};

// Used to internally register UI elements.
class CUIElementReg {
public:
    typedef void( *CreateElementFn )( CBaseUIElement ** );
    typedef void( *DeleteElementFn )( CBaseUIElement ** );

public:
    static CUIElementReg *s_pElements;
    CUIElementReg *m_pNext;

    CreateElementFn m_pCreateFn;
    DeleteElementFn m_pDeleteFn;
    CBaseUIElement *m_pElement = NULL;

public:
    CUIElementReg( CreateElementFn pCreateFn, DeleteElementFn pDeleteFn );
    void Create();
    void Delete();
};

// Declares UI element...
#define REGISTER_UI_ELEMENT(className)                      \
static void className##_Create(CBaseUIElement **pElement)   \
{                                                           \
    *pElement = (CBaseUIElement *)(new className);          \
}                                                           \
static void className##_Delete(CBaseUIElement **pElement)   \
{                                                           \
    delete *pElement;                                       \
    *pElement = nullptr;                                    \
}                                                           \
static CUIElementReg s_##className##_reg(className##_Create, className##_Delete)

extern class IVEngineClient *           g_pEngine;
extern class IEngineSound *             g_pEngineSound;
extern class IEngineVGui *              g_pEngineVGui;
extern class ISoundEmitterSystemBase *  g_pSoundEmitterSystem;
extern class IVRenderView *             g_pRenderView;
extern class IMaterialSystem *          g_pMaterialSystem;
extern class IMatSystemSurface *        g_pMatSystemSurface;
extern class IFileSystem *              g_pFileSystem;
extern class CGlobalVarsBase *          g_pGlobals;
extern class IGameUI *                  g_pGameUI1;
extern class CGameUI2 *                 g_pGameUI2;

#endif
