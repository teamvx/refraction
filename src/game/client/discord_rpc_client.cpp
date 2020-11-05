#include "cbase.h"
#include "discord_rpc_client.h"
#include <time.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class CDiscordRPC : public IDiscordRPC {
public:
    CDiscordRPC( void );

    virtual const char * Name(void);
    virtual bool Init( void );
    virtual void Shutdown( void );
    virtual void LevelInitPreEntity( void );
    virtual void LevelShutdownPostEntity( void );
    virtual void Update( float flDelta );

    virtual void SetPresenceState( const char *pszFmt, ... );
    virtual void SetPresenceDetails( const char *pszFmt, ... );

private:
    int64_t m_llRPCStartTimestamp;
    char m_szPresenceState[1024];
    char m_szPresenceDetails[1024];
    DiscordRichPresence m_Presence;

private:
    static void HandleReady( const DiscordUser *pUser );
    static void HandleDisconnected( int iCode, const char *pszMessage );
    static void HandleError( int iCode, const char *pszMessage );
    static void HandleJoin( const char *pszSecret );
    static void HandleSpectate( const char *pszSecret );
    static void HandleJoinRequest( const DiscordUser *pUser );
};
static CDiscordRPC s_DiscordRPC;
IDiscordRPC *g_pDiscordRPC = &s_DiscordRPC;

// Discord AppID
static ConVar discord_appid( "discord_appid", "773971191350296606", FCVAR_DEVELOPMENTONLY );

static inline void Discord_UpdatePresenceEx( const DiscordRichPresence *pPrecense )
{
    if( !g_bTextMode )
        Discord_UpdatePresence( pPrecense );
}

CDiscordRPC::CDiscordRPC( void ) : m_llRPCStartTimestamp( 0 ), m_Presence()
{
    Q_memset( m_szPresenceState, 0, sizeof( m_szPresenceState ) );
    Q_memset( m_szPresenceDetails, 0, sizeof( m_szPresenceDetails ) );
}

const char * CDiscordRPC::Name( void )
{
    // GameSystem Name.
    return "Discord-RPC";
}

bool CDiscordRPC::Init( void )
{
    DiscordEventHandlers handlers;
    Q_memset( &handlers, 0, sizeof( DiscordEventHandlers ) );

    handlers.ready = CDiscordRPC::HandleReady;
    handlers.disconnected = CDiscordRPC::HandleDisconnected;
    handlers.errored = CDiscordRPC::HandleError;
    handlers.joinGame = CDiscordRPC::HandleJoin;
    handlers.spectateGame = CDiscordRPC::HandleSpectate;
    handlers.joinRequest = CDiscordRPC::HandleJoinRequest;

    char szAppId[256] = { 0 };
    Q_snprintf( szAppId, sizeof( szAppId ), "%d", engine->GetAppID() );
    m_llRPCStartTimestamp = time( 0 );
    Discord_Initialize( discord_appid.GetString(), &handlers, true, szAppId );

    Q_memset( &m_Presence, 0, sizeof( DiscordRichPresence ) );
    m_Presence.startTimestamp = m_llRPCStartTimestamp;
    m_Presence.largeImageKey = "logo1";
    SetPresenceState( "Main Menu" );

    return true;
}

void CDiscordRPC::Shutdown( void )
{
    Discord_Shutdown();
}

void CDiscordRPC::LevelInitPreEntity( void )
{
    m_Presence.startTimestamp = time( 0 );
    SetPresenceState( "Map - %s", MapName() );
}

void CDiscordRPC::LevelShutdownPostEntity( void )
{
    m_Presence.startTimestamp = m_llRPCStartTimestamp;
    SetPresenceState( "Main Menu" );
}

void CDiscordRPC::Update( float flDelta )
{
    // We have to dispatch callbacks every frame/game tick.
    (void)( flDelta );
    Discord_RunCallbacks();
}

void CDiscordRPC::SetPresenceState( const char *pszFmt, ... )
{
    if( !pszFmt ) {
        m_Presence.state = NULL;
        Discord_UpdatePresenceEx( &m_Presence );
        return;
    }

    Q_memset( m_szPresenceState, 0, sizeof( m_szPresenceState ) );
    va_list ap;
    va_start( ap, pszFmt );
    Q_vsnprintf( m_szPresenceState, sizeof( m_szPresenceState ), pszFmt, ap );
    va_end( ap );

    m_Presence.state = m_szPresenceState;
    Discord_UpdatePresenceEx( &m_Presence );
}

void CDiscordRPC::SetPresenceDetails( const char *pszFmt, ... )
{
    if( !pszFmt ) {
        m_Presence.details = NULL;
        Discord_UpdatePresenceEx( &m_Presence );
        return;
    }

    Q_memset( m_szPresenceDetails, 0, sizeof( m_szPresenceDetails ) );
    va_list ap;
    va_start( ap, pszFmt );
    Q_vsnprintf( m_szPresenceDetails, sizeof( m_szPresenceDetails ), pszFmt, ap );
    va_end( ap );

    m_Presence.details = m_szPresenceDetails;
    Discord_UpdatePresenceEx( &m_Presence );
}

void CDiscordRPC::HandleReady( const DiscordUser *pUser )
{
    ConColorMsg( LOG_COLOR_MAGENTA, "Discord: Connected to %s#%s (%s)\n", pUser->username, pUser->discriminator, pUser->userId );
}

void CDiscordRPC::HandleDisconnected( int iCode, const char *pszMessage )
{
    ConColorMsg( LOG_COLOR_MAGENTA, "Discord: Disconnected - %d: %s\n", iCode, pszMessage );
}

void CDiscordRPC::HandleError( int iCode, const char *pszMessage )
{
    ConColorMsg( LOG_COLOR_MAGENTA, "Discord: Error - %d: %s\n", iCode, pszMessage );
}

void CDiscordRPC::HandleJoin( const char *pszSecret )
{
    (void)( pszSecret );
}

void CDiscordRPC::HandleSpectate( const char *pszSecret )
{
    (void)( pszSecret );
}

void CDiscordRPC::HandleJoinRequest( const DiscordUser *pUser )
{
    (void)( pUser );
}
