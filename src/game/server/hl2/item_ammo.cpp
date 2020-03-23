//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: The various ammo types for HL2
//
//=============================================================================//

#include "cbase.h"
#include "player.h"
#include "gamerules.h"
#include "items.h"
#include "ammodef.h"
#include "eventlist.h"
#include "npcevent.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//---------------------------------------------------------
// Applies ammo quantity scale.
//---------------------------------------------------------
int ITEM_GiveAmmo( CBasePlayer *pPlayer, float flCount, const char *pszAmmoName, bool bSuppressSound = false )
{
    int iAmmoType = GetAmmoDef()->Index(pszAmmoName);
    if (iAmmoType == -1)
    {
        Msg("ERROR: Attempting to give unknown ammo type (%s)\n",pszAmmoName);
        return 0;
    }

    flCount *= g_pGameRules->GetAmmoQuantityScale(iAmmoType);

    // Don't give out less than 1 of anything.
    flCount = MAX( 1.0f, flCount );

    return pPlayer->GiveAmmo( flCount, iAmmoType, bSuppressSound );
}

// Base class for all ammo items.
// Someone please make valve programmers read OOP books.
class CItem_Ammo : public CItem {
public:
    DECLARE_CLASS( CItem_Ammo, CItem );

private:
    const char *m_szModelName;  //Model name of the ammo box
    const char *m_szAmmoName;   //Ammo definition of class
    float m_flAmmoCount;        //Ammo count which will be given to picker

public:
    //Constructor
    CItem_Ammo( const char *name, const float count, const char *model ) : m_szModelName( model ), m_szAmmoName( name ), m_flAmmoCount( count ) { }

    //Actions on spawn
    void Spawn()
    {
        Precache();
        SetModel( m_szModelName );
        BaseClass::Spawn();
    }

    //Precache stuff
    void Precache()
    {
        PrecacheModel( m_szModelName );
    }

    //Executes on player touch
    bool MyTouch( CBasePlayer *plr )
    {
        if( ITEM_GiveAmmo( plr, m_flAmmoCount, m_szAmmoName ) ) {
            if( g_pGameRules->ItemShouldRespawn( this ) == GR_ITEM_RESPAWN_NO ) {
                UTIL_Remove( this );
            }
            return true;
        }
        return false;
    }
};

// ========================================================================
//  >> BoxSRounds
// ========================================================================
class CItem_BoxSRounds : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_BoxSRounds, CItem_Ammo )
    CItem_BoxSRounds() : BaseClass( "Pistol", SIZE_AMMO_PISTOL, "models/items/boxsrounds.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_box_srounds, CItem_BoxSRounds);
LINK_ENTITY_TO_CLASS(item_ammo_pistol, CItem_BoxSRounds);

// ========================================================================
//  >> LargeBoxSRounds
// ========================================================================
class CItem_LargeBoxSRounds : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_LargeBoxSRounds, CItem_Ammo )
    CItem_LargeBoxSRounds() : BaseClass( "Pistol", SIZE_AMMO_PISTOL_LARGE, "models/items/boxsrounds.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_large_box_srounds, CItem_LargeBoxSRounds);
LINK_ENTITY_TO_CLASS(item_ammo_pistol_large, CItem_LargeBoxSRounds);

// ========================================================================
//  >> BoxMRounds
// ========================================================================
class CItem_BoxMRounds : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_BoxMRounds, CItem_Ammo );
    CItem_BoxMRounds() : BaseClass( "SMG1", SIZE_AMMO_SMG1, "models/items/boxmrounds.mdl" ) { }
};

LINK_ENTITY_TO_CLASS(item_box_mrounds, CItem_BoxMRounds);
LINK_ENTITY_TO_CLASS(item_ammo_smg1, CItem_BoxMRounds);

// ========================================================================
//  >> LargeBoxMRounds
// ========================================================================
class CItem_LargeBoxMRounds : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_LargeBoxMRounds, CItem_Ammo );
    CItem_LargeBoxMRounds() : BaseClass( "SMG1", SIZE_AMMO_SMG1_LARGE, "models/items/boxmrounds.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_large_box_mrounds, CItem_LargeBoxMRounds);
LINK_ENTITY_TO_CLASS(item_ammo_smg1_large, CItem_LargeBoxMRounds);

// ========================================================================
//  >> BoxLRounds
// ========================================================================
class CItem_BoxLRounds : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_BoxLRounds, CItem_Ammo );
    CItem_BoxLRounds() : BaseClass( "AR2", SIZE_AMMO_AR2, "models/items/combine_rifle_cartridge01.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_box_lrounds, CItem_BoxLRounds);
LINK_ENTITY_TO_CLASS(item_ammo_ar2, CItem_BoxLRounds);

// ========================================================================
//  >> LargeBoxLRounds
// ========================================================================
class CItem_LargeBoxLRounds : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_LargeBoxLRounds, CItem_Ammo );
    CItem_LargeBoxLRounds() : BaseClass( "AR2", SIZE_AMMO_AR2_LARGE, "models/items/combine_rifle_cartridge01.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_large_box_lrounds, CItem_LargeBoxLRounds);
LINK_ENTITY_TO_CLASS(item_ammo_ar2_large, CItem_LargeBoxLRounds);


// ========================================================================
//  >> CItem_Box357Rounds
// ========================================================================
class CItem_Box357Rounds : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_Box357Rounds, CItem_Ammo );
    CItem_Box357Rounds() : BaseClass( "357", SIZE_AMMO_357, "models/items/357ammo.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_ammo_357, CItem_Box357Rounds);


// ========================================================================
//  >> CItem_LargeBox357Rounds
// ========================================================================
class CItem_LargeBox357Rounds : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_LargeBox357Rounds, CItem_Ammo );
    CItem_LargeBox357Rounds() : BaseClass( "357", SIZE_AMMO_357, "models/items/357ammobox.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_ammo_357_large, CItem_LargeBox357Rounds);


// ========================================================================
//  >> CItem_BoxXBowRounds
// ========================================================================
class CItem_BoxXBowRounds : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_BoxXBowRounds, CItem_Ammo );
    CItem_BoxXBowRounds() : BaseClass( "XBowBolt", SIZE_AMMO_CROSSBOW, "models/items/crossbowrounds.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_ammo_crossbow, CItem_BoxXBowRounds);


// ========================================================================
//  >> FlareRound
// ========================================================================
class CItem_FlareRound : public CItem_Ammo
{
public:
    DECLARE_CLASS( CItem_FlareRound, CItem_Ammo );
    CItem_FlareRound() : BaseClass( "FlareRound", 1, "models/items/flare.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_flare_round, CItem_FlareRound);

// ========================================================================
//  >> BoxFlareRounds
// ========================================================================
#define SIZE_BOX_FLARE_ROUNDS 5

class CItem_BoxFlareRounds : public CItem_Ammo
{
public:
    DECLARE_CLASS( CItem_BoxFlareRounds, CItem_Ammo );
    CItem_BoxFlareRounds() : BaseClass( "FlareRound", SIZE_BOX_FLARE_ROUNDS, "models/items/boxflares.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_box_flare_rounds, CItem_BoxFlareRounds);

// ========================================================================
// RPG Round
// ========================================================================
class CItem_RPG_Round : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_RPG_Round, CItem_Ammo );
    CItem_RPG_Round() : BaseClass( "RPG_Round", SIZE_AMMO_RPG_ROUND, "models/weapons/w_missile_closed.mdl" ) { }
};
LINK_ENTITY_TO_CLASS( item_ml_grenade, CItem_RPG_Round );
LINK_ENTITY_TO_CLASS( item_rpg_round, CItem_RPG_Round );

// ========================================================================
//  >> AR2_Grenade
// ========================================================================
class CItem_AR2_Grenade : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_AR2_Grenade, CItem_Ammo );
    CItem_AR2_Grenade() : BaseClass( "SMG1_Grenade", SIZE_AMMO_SMG1_GRENADE, "models/items/ar2_grenade.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_ar2_grenade, CItem_AR2_Grenade);
LINK_ENTITY_TO_CLASS(item_ammo_smg1_grenade, CItem_AR2_Grenade);

// ========================================================================
//  >> BoxSniperRounds
// ========================================================================
#define SIZE_BOX_SNIPER_ROUNDS 10

class CItem_BoxSniperRounds : public CItem_Ammo
{
public:
    DECLARE_CLASS( CItem_BoxSniperRounds, CItem_Ammo );
    CItem_BoxSniperRounds() : BaseClass( "models/items/boxsniperrounds.mdl", SIZE_BOX_SNIPER_ROUNDS, "SniperRound" ) { }
};
LINK_ENTITY_TO_CLASS(item_box_sniper_rounds, CItem_BoxSniperRounds);


// ========================================================================
//  >> BoxBuckshot
// ========================================================================
class CItem_BoxBuckshot : public CItem_Ammo {
public:
    DECLARE_CLASS( CItem_BoxBuckshot, CItem_Ammo );
    CItem_BoxBuckshot() : BaseClass( "Buckshot", SIZE_AMMO_BUCKSHOT, "models/items/boxbuckshot.mdl" ) { }
};
LINK_ENTITY_TO_CLASS(item_box_buckshot, CItem_BoxBuckshot);

// ========================================================================
//  >> CItem_AR2AltFireRound
// ========================================================================
class CItem_AR2AltFireRound : public CItem_Ammo
{
public:
    DECLARE_CLASS( CItem_AR2AltFireRound, CItem_Ammo );
    CItem_AR2AltFireRound() : BaseClass( "models/items/combine_rifle_ammo01.mdl", SIZE_AMMO_AR2_ALTFIRE, "AR2AltFire" ) { }

    void Precache( void )
    {
        PrecacheParticleSystem( "combineball" );
        BaseClass::Precache();
    }
};

LINK_ENTITY_TO_CLASS( item_ammo_ar2_altfire, CItem_AR2AltFireRound );

// ==================================================================
// Ammo crate which will supply infinite ammo of the specified type
// ==================================================================

// Ammo types
enum
{
    AMMOCRATE_SMALL_ROUNDS,
    AMMOCRATE_MEDIUM_ROUNDS,
    AMMOCRATE_LARGE_ROUNDS,
    AMMOCRATE_RPG_ROUNDS,
    AMMOCRATE_BUCKSHOT,
    AMMOCRATE_GRENADES,
    AMMOCRATE_357,
    AMMOCRATE_CROSSBOW,
    AMMOCRATE_AR2_ALTFIRE,
    AMMOCRATE_SMG_ALTFIRE,
    NUM_AMMO_CRATE_TYPES,
};

// Ammo crate

class CItem_AmmoCrate : public CBaseAnimating
{
public:
    DECLARE_CLASS( CItem_AmmoCrate, CBaseAnimating );

    void    Spawn( void );
    void    Precache( void );
    bool    CreateVPhysics( void );

    virtual void HandleAnimEvent( animevent_t *pEvent );

    void    SetupCrate( void );
    void    OnRestore( void );

    //FIXME: May not want to have this used in a radius
    int     ObjectCaps( void ) { return (BaseClass::ObjectCaps() | (FCAP_IMPULSE_USE|FCAP_USE_IN_RADIUS)); };
    void    Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

    void    InputKill( inputdata_t &data );
    void    CrateThink( void );

    virtual int OnTakeDamage( const CTakeDamageInfo &info );

protected:

    int     m_nAmmoType;
    int     m_nAmmoIndex;

    static const char *m_lpzModelNames[NUM_AMMO_CRATE_TYPES];
    static const char *m_lpzAmmoNames[NUM_AMMO_CRATE_TYPES];
    static int m_nAmmoAmounts[NUM_AMMO_CRATE_TYPES];
    static const char *m_pGiveWeapon[NUM_AMMO_CRATE_TYPES];

    float   m_flCloseTime;
    COutputEvent    m_OnUsed;
    CHandle< CBasePlayer > m_hActivator;

    DECLARE_DATADESC();
};

LINK_ENTITY_TO_CLASS( item_ammo_crate, CItem_AmmoCrate );

BEGIN_DATADESC( CItem_AmmoCrate )

    DEFINE_KEYFIELD( m_nAmmoType,   FIELD_INTEGER, "AmmoType" ),

    DEFINE_FIELD( m_flCloseTime, FIELD_FLOAT ),
    DEFINE_FIELD( m_hActivator, FIELD_EHANDLE ),

    // These can be recreated
    //DEFINE_FIELD( m_nAmmoIndex,       FIELD_INTEGER ),
    //DEFINE_FIELD( m_lpzModelNames,    FIELD_ ),
    //DEFINE_FIELD( m_lpzAmmoNames, FIELD_ ),
    //DEFINE_FIELD( m_nAmmoAmounts, FIELD_INTEGER ),

    DEFINE_OUTPUT( m_OnUsed, "OnUsed" ),

    DEFINE_INPUTFUNC( FIELD_VOID, "Kill", InputKill ),

    DEFINE_THINKFUNC( CrateThink ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Animation events.
//-----------------------------------------------------------------------------

// Models names
const char *CItem_AmmoCrate::m_lpzModelNames[NUM_AMMO_CRATE_TYPES] =
{
    "models/items/ammocrate_pistol.mdl",    // Small rounds
    "models/items/ammocrate_smg1.mdl",      // Medium rounds
    "models/items/ammocrate_ar2.mdl",       // Large rounds
    "models/items/ammocrate_rockets.mdl",   // RPG rounds
    "models/items/ammocrate_buckshot.mdl",  // Buckshot
    "models/items/ammocrate_grenade.mdl",   // Grenades
    "models/items/ammocrate_smg1.mdl",      // 357
    "models/items/ammocrate_smg1.mdl",  // Crossbow

    //FIXME: This model is incorrect!
    "models/items/ammocrate_ar2.mdl",       // Combine Ball
    "models/items/ammocrate_smg2.mdl",      // smg grenade
};

// Ammo type names
const char *CItem_AmmoCrate::m_lpzAmmoNames[NUM_AMMO_CRATE_TYPES] =
{
    "Pistol",
    "SMG1",
    "AR2",
    "RPG_Round",
    "Buckshot",
    "Grenade",
    "357",
    "XBowBolt",
    "AR2AltFire",
    "SMG1_Grenade",
};

// Ammo amount given per +use
int CItem_AmmoCrate::m_nAmmoAmounts[NUM_AMMO_CRATE_TYPES] =
{
    300,    // Pistol
    300,    // SMG1
    300,    // AR2
    3,      // RPG rounds
    120,    // Buckshot
    5,      // Grenades
    50,     // 357
    50,     // Crossbow
    3,      // AR2 alt-fire
    5,
};

const char *CItem_AmmoCrate::m_pGiveWeapon[NUM_AMMO_CRATE_TYPES] =
{
    NULL,   // Pistol
    NULL,   // SMG1
    NULL,   // AR2
    NULL,       // RPG rounds
    NULL,   // Buckshot
    "weapon_frag",      // Grenades
    NULL,       // 357
    NULL,       // Crossbow
    NULL,       // AR2 alt-fire
    NULL,       // SMG alt-fire
};

#define AMMO_CRATE_CLOSE_DELAY  1.5f

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::Spawn( void )
{
    Precache();

    BaseClass::Spawn();

    SetModel( STRING( GetModelName() ) );
    SetMoveType( MOVETYPE_NONE );
    SetSolid( SOLID_VPHYSICS );
    CreateVPhysics();

    ResetSequence( LookupSequence( "Idle" ) );
    SetBodygroup( 1, true );

    m_flCloseTime = gpGlobals->curtime;
    m_flAnimTime = gpGlobals->curtime;
    m_flPlaybackRate = 0.0;
    SetCycle( 0 );

    m_takedamage = DAMAGE_EVENTS_ONLY;

}

//------------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------------
bool CItem_AmmoCrate::CreateVPhysics( void )
{
    return ( VPhysicsInitStatic() != NULL );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::Precache( void )
{
    SetupCrate();
    PrecacheModel( STRING( GetModelName() ) );

    PrecacheScriptSound( "AmmoCrate.Open" );
    PrecacheScriptSound( "AmmoCrate.Close" );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::SetupCrate( void )
{
    SetModelName( AllocPooledString( m_lpzModelNames[m_nAmmoType] ) );

    m_nAmmoIndex = GetAmmoDef()->Index( m_lpzAmmoNames[m_nAmmoType] );
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::OnRestore( void )
{
    BaseClass::OnRestore();

    // Restore our internal state
    SetupCrate();
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *pActivator -
//          *pCaller -
//          useType -
//          value -
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
    CBasePlayer *pPlayer = ToBasePlayer( pActivator );

    if ( pPlayer == NULL )
        return;

    m_OnUsed.FireOutput( pActivator, this );

    int iSequence = LookupSequence( "Open" );

    // See if we're not opening already
    if ( GetSequence() != iSequence )
    {
        Vector mins, maxs;
        trace_t tr;

        CollisionProp()->WorldSpaceAABB( &mins, &maxs );

        Vector vOrigin = GetAbsOrigin();
        vOrigin.z += ( maxs.z - mins.z );
        mins = (mins - GetAbsOrigin()) * 0.2f;
        maxs = (maxs - GetAbsOrigin()) * 0.2f;
        mins.z = ( GetAbsOrigin().z - vOrigin.z );

        UTIL_TraceHull( vOrigin, vOrigin, mins, maxs, MASK_SOLID, this, COLLISION_GROUP_NONE, &tr );

        if ( tr.startsolid || tr.allsolid )
             return;

        m_hActivator = pPlayer;

        // Animate!
        ResetSequence( iSequence );

        // Make sound
        CPASAttenuationFilter sndFilter( this, "AmmoCrate.Open" );
        EmitSound( sndFilter, entindex(), "AmmoCrate.Open" );

        // Start thinking to make it return
        SetThink( &CItem_AmmoCrate::CrateThink );
        SetNextThink( gpGlobals->curtime + 0.1f );
    }

    // Don't close again for two seconds
    m_flCloseTime = gpGlobals->curtime + AMMO_CRATE_CLOSE_DELAY;
}

//-----------------------------------------------------------------------------
// Purpose: allows the crate to open up when hit by a crowbar
//-----------------------------------------------------------------------------
int CItem_AmmoCrate::OnTakeDamage( const CTakeDamageInfo &info )
{
    // if it's the player hitting us with a crowbar, open up
    CBasePlayer *player = ToBasePlayer(info.GetAttacker());
    if (player)
    {
        CBaseCombatWeapon *weapon = player->GetActiveWeapon();

        if (weapon && !stricmp(weapon->GetName(), "weapon_crowbar"))
        {
            // play the normal use sound
            player->EmitSound( "HL2Player.Use" );
            // open the crate
            Use(info.GetAttacker(), info.GetAttacker(), USE_TOGGLE, 0.0f);
        }
    }

    // don't actually take any damage
    return 0;
}


//-----------------------------------------------------------------------------
// Purpose: Catches the monster-specific messages that occur when tagged
//          animation frames are played.
// Input  : *pEvent -
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::HandleAnimEvent( animevent_t *pEvent )
{
    if ( pEvent->event == AE_AMMOCRATE_PICKUP_AMMO )
    {
        if ( m_hActivator )
        {
            if ( m_pGiveWeapon[m_nAmmoType] && !m_hActivator->Weapon_OwnsThisType( m_pGiveWeapon[m_nAmmoType] ) )
            {
                CBaseEntity *pEntity = CreateEntityByName( m_pGiveWeapon[m_nAmmoType] );
                CBaseCombatWeapon *pWeapon = dynamic_cast<CBaseCombatWeapon*>(pEntity);
                if ( pWeapon )
                {
                    pWeapon->SetAbsOrigin( m_hActivator->GetAbsOrigin() );
                    pWeapon->m_iPrimaryAmmoType = 0;
                    pWeapon->m_iSecondaryAmmoType = 0;
                    pWeapon->Spawn();
                    if ( !m_hActivator->BumpWeapon( pWeapon ) )
                    {
                        UTIL_Remove( pEntity );
                    }
                    else
                    {
                        SetBodygroup( 1, false );
                    }
                }
            }

            if ( m_hActivator->GiveAmmo( m_nAmmoAmounts[m_nAmmoType], m_nAmmoIndex ) != 0 )
            {
                SetBodygroup( 1, false );
            }
            m_hActivator = NULL;
        }
        return;
    }

    BaseClass::HandleAnimEvent( pEvent );
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::CrateThink( void )
{
    StudioFrameAdvance();
    DispatchAnimEvents( this );

    SetNextThink( gpGlobals->curtime + 0.1f );

    // Start closing if we're not already
    if ( GetSequence() != LookupSequence( "Close" ) )
    {
        // Not ready to close?
        if ( m_flCloseTime <= gpGlobals->curtime )
        {
            m_hActivator = NULL;

            ResetSequence( LookupSequence( "Close" ) );
        }
    }
    else
    {
        // See if we're fully closed
        if ( IsSequenceFinished() )
        {
            // Stop thinking
            SetThink( NULL );
            CPASAttenuationFilter sndFilter( this, "AmmoCrate.Close" );
            EmitSound( sndFilter, entindex(), "AmmoCrate.Close" );

            // FIXME: We're resetting the sequence here
            // but setting Think to NULL will cause this to never have
            // StudioFrameAdvance called. What are the consequences of that?
            ResetSequence( LookupSequence( "Idle" ) );
            SetBodygroup( 1, true );
        }
    }
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : &data -
//-----------------------------------------------------------------------------
void CItem_AmmoCrate::InputKill( inputdata_t &data )
{
    UTIL_Remove( this );
}

