//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//

#ifndef IGAMESYSTEM_H
#define IGAMESYSTEM_H
#ifdef _WIN32
#pragma once
#endif

//-----------------------------------------------------------------------------
// Game systems are singleton objects in the client + server codebase responsible for
// various tasks
// The order in which the server systems appear in this list are the
// order in which they are initialized and updated. They are shut down in
// reverse order from which they are initialized.
//-----------------------------------------------------------------------------


// UNDONE: Do these need GameInit/GameShutdown as well?
// UNDONE: Remove the Pre/Post entity semantics and rely on system ordering?
// FIXME: Remove all ifdef CLIENT_DLL if we can...
abstract_class IGameSystem
{
public:
    // GameSystems are expected to implement these methods.
    virtual const char * Name( void ) = 0;

    // Init, shutdown
    // return true on success. false to abort DLL init!
    virtual bool Init( void ) = 0;
    virtual void PostInit( void ) = 0;
    virtual void Shutdown( void ) = 0;
#ifdef CLIENT_DLL
    virtual void VidInit( void ) = 0;
#endif

    // Level init, shutdown
    virtual void LevelInitPreEntity( void ) = 0;
    // entities are created / spawned / precached here
    virtual void LevelInitPostEntity( void ) = 0;

    virtual void LevelShutdownPreClearSteamAPIContext( void ) { };
    virtual void LevelShutdownPreEntity( void ) = 0;
    // Entities are deleted / released here...
    virtual void LevelShutdownPostEntity( void ) = 0;
    // end of level shutdown

    // Called during game save
    virtual void OnSave( void ) = 0;

    // Called during game restore, after the local player has connected and entities have been fully restored
    virtual void OnRestore( void ) = 0;

    // Called every frame. It's safe to remove an igamesystem from within this callback.
    virtual void SafeRemoveIfDesired( void ) = 0;

    virtual bool IsPerFrame( void ) = 0;

    // destructor, cleans up automagically....
    virtual ~IGameSystem( void );

    // Client systems can use this to get at the map name
    static const char * MapName( void );

    // These methods are used to add and remove server systems from the
    // main server loop. The systems are invoked in the order in which
    // they are added.
    static void Add( IGameSystem* pSys );
    static void Remove( IGameSystem* pSys );
    static void RemoveAll( void );

    // These methods are used to initialize, shutdown, etc all systems
    static bool InitAllSystems( void );
    static void PostInitAllSystems( void );
    static void ShutdownAllSystems( void );
#ifdef CLIENT_DLL
    static void VidInitAllSystems( void );
#endif
    static void LevelInitPreEntityAllSystems( char const* pMapName );
    static void LevelInitPostEntityAllSystems( void );
    static void LevelShutdownPreClearSteamAPIContextAllSystems( void ); // Called prior to steamgameserverapicontext->Clear( void )
    static void LevelShutdownPreEntityAllSystems( void );
    static void LevelShutdownPostEntityAllSystems( void );

    static void OnSaveAllSystems( void );
    static void OnRestoreAllSystems( void );

    static void SafeRemoveIfDesiredAllSystems( void );

#ifdef CLIENT_DLL
    static void PreRenderAllSystems( void );
    static void UpdateAllSystems( float frametime );
    static void PostRenderAllSystems( void );
#else
    static void FrameUpdatePreEntityThinkAllSystems( void );
    static void FrameUpdatePostEntityThinkAllSystems( void );
    static void PreClientUpdateAllSystems( void );

    // Accessors for the above function
    static CBasePlayer *RunCommandPlayer( void );
    static CUserCmd *RunCommandUserCmd( void );
#endif
};

class IGameSystemPerFrame : public IGameSystem {
public:
    // destructor, cleans up automagically....
    virtual ~IGameSystemPerFrame( void );

#ifdef CLIENT_DLL
    // Called before rendering
    virtual void PreRender( void ) = 0;

    // Gets called each frame
    virtual void Update( float frametime ) = 0;

    // Called after rendering
    virtual void PostRender( void ) = 0;
#else
    // Called each frame before entities think
    virtual void FrameUpdatePreEntityThink( void ) = 0;
    // called after entities think
    virtual void FrameUpdatePostEntityThink( void ) = 0;
    virtual void PreClientUpdate( void ) = 0;
#endif
};

// Quick and dirty server system for users who don't care about precise ordering
// and usually only want to implement a few of the callbacks
class CBaseGameSystem : public IGameSystem {
public:
    virtual const char * Name( void ) { return "unnamed"; }

    // Init, shutdown
    // return true on success. false to abort DLL init!
    virtual bool Init( void ) { return true; }
    virtual void PostInit( void ) { }
    virtual void Shutdown( void ) { }
#ifdef CLIENT_DLL
    virtual void VidInit( void ) { }
#endif

    // Level init, shutdown
    virtual void LevelInitPreEntity( void ) { }
    virtual void LevelInitPostEntity( void ) { }
    virtual void LevelShutdownPreClearSteamAPIContext( void ) { }
    virtual void LevelShutdownPreEntity( void ) { }
    virtual void LevelShutdownPostEntity( void ) { }

    virtual void OnSave( void ) { }
    virtual void OnRestore( void ) { }
    virtual void SafeRemoveIfDesired( void ) { }

    virtual bool IsPerFrame( void ) { return false; }
private:

    // Prevent anyone derived from CBaseGameSystem from implementing these, they need
    //  to derive from CBaseGameSystemPerFrame below!!!
#ifdef CLIENT_DLL
    // Called before rendering
    virtual void PreRender( void ) { }

    // Gets called each frame
    virtual void Update( float frametime ) { }

    // Called after rendering
    virtual void PostRender( void ) { }
#else
    // Called each frame before entities think
    virtual void FrameUpdatePreEntityThink( void ) { }
    // called after entities think
    virtual void FrameUpdatePostEntityThink( void ) { }
    virtual void PreClientUpdate( void ) { }
#endif
};

// Quick and dirty server system for users who don't care about precise ordering
// and usually only want to implement a few of the callbacks
class CBaseGameSystemPerFrame : public IGameSystemPerFrame {
public:
    virtual const char * Name( void ) { return "unnamed"; }

    // Init, shutdown
    // return true on success. false to abort DLL init!
    virtual bool Init( void ) { return true; }
    virtual void PostInit( void ) { }
    virtual void Shutdown( void ) { }
#ifdef CLIENT_DLL
    virtual void VidInit( void ) { }
#endif

    // Level init, shutdown
    virtual void LevelInitPreEntity( void ) { }
    virtual void LevelInitPostEntity( void ) { }
    virtual void LevelShutdownPreClearSteamAPIContext( void ) { }
    virtual void LevelShutdownPreEntity( void ) { }
    virtual void LevelShutdownPostEntity( void ) { }

    virtual void OnSave( void ) { }
    virtual void OnRestore( void ) { }
    virtual void SafeRemoveIfDesired( void ) { }

    virtual bool IsPerFrame( void ) { return true; }

#ifdef CLIENT_DLL
    // Called before rendering
    virtual void PreRender( void ) { }

    // Gets called each frame
    virtual void Update( float frametime ) { }

    // Called after rendering
    virtual void PostRender( void ) { }
#else
    // Called each frame before entities think
    virtual void FrameUpdatePreEntityThink( void ) { }
    // called after entities think
    virtual void FrameUpdatePostEntityThink( void ) { }
    virtual void PreClientUpdate( void ) { }
#endif
};

// Quick and dirty server system for users who don't care about precise ordering
// and usually only want to implement a few of the callbacks
class CAutoGameSystem : public CBaseGameSystem {
public:
    CAutoGameSystem( const char *name = NULL ); // hooks in at startup, no need to explicitly add
    CAutoGameSystem *m_pNext;

    virtual const char * Name( void ) { return m_pszName ? m_pszName : "unnamed"; }

private:
    const char *m_pszName;
};

//-----------------------------------------------------------------------------
// Purpose: This is a CAutoGameSystem which also cares about the "per frame" hooks
//-----------------------------------------------------------------------------
class CAutoGameSystemPerFrame : public CBaseGameSystemPerFrame {
public:
    CAutoGameSystemPerFrame( const char *name = NULL );
    CAutoGameSystemPerFrame *m_pNext;

    virtual const char * Name( void ) { return m_pszName ? m_pszName : "unnamed"; }

private:
    const char *m_pszName;
};


//-----------------------------------------------------------------------------
// Purpose: This interface is here to add more hooks than IGameSystemPerFrame exposes,
// so we don't pollute it with hooks that only the tool cares about
//-----------------------------------------------------------------------------
class IToolFrameworkServer {
public:
    virtual void PreSetupVisibility( void ) = 0;
};

#endif // IGAMESYSTEM_H
