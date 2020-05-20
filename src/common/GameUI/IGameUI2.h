#ifndef IREGAMEUI_H
#define IREGAMEUI_H
#ifdef _WIN32
#pragma once
#endif

#include "IGameUI.h"
#ifdef GAMEUI2_DLL
#include "tier1/interface.h"
#else
#include "interface.h"
#endif

#include "vgui/VGUI.h"
#include "mathlib/vector.h"
#include "ivrenderview.h"

// Base interface for GameUI2 (ReGameUI)
class IGameUI2 : public IBaseInterface {
public:
    virtual void Initialize( CreateInterfaceFn appSystemFactory ) = 0;
    virtual void Shutdown() = 0;
    virtual void VidInit() = 0;

    virtual void PostInit() = 0;
    virtual void PreShutdown() = 0;
    virtual void OnUpdate() = 0;
    virtual void LevelInitPreEntity( const char *pMapName ) = 0;
    virtual void LevelInitPostEntity() = 0;
    virtual void LevelShutdown() = 0;
};
#define GAMEUI2_INTERFACE_VERSION   "GameUI2v001"

// Some macros...
#define GameUI2_Message(fmt, ...)   ConColorMsg(Color(0x00, 0xFF, 0xFF, 0xFF), fmt, __VA_ARGS__)
#define GameUI2_Warning(fmt, ...)   ConColorMsg(Color(0xFF, 0xFF, 0x00, 0xFF), fmt, __VA_ARGS__)

#endif
