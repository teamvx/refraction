//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "hudelement.h"
#include "hud_macros.h"
#include "hud_numericdisplay.h"
#include "iclientmode.h"
#include "iclientvehicle.h"
#include "vgui_controls/AnimationController.h"
#include "vgui/ILocalize.h"
#include "vgui/ISurface.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: Displays primary ammunition level.
//-----------------------------------------------------------------------------
class CHudPrimaryAmmo : public CHudNumericDisplay, public CHudElement {
    DECLARE_CLASS_SIMPLE( CHudPrimaryAmmo, CHudNumericDisplay );
public:
    CHudPrimaryAmmo( const char *pszElementName );
    void Init( void );
    void Reset( void );

    void SetAmmo( int iAmmo );
    void SetAmmo2( int iAmmo2 );
    void Paint( void );

protected:
    void OnThink( void );
    void UpdatePlayerAmmo( C_BasePlayer *pPlayer );
    void UpdateVehicleAmmo( C_BasePlayer *pPlayer, IClientVehicle *pVehicle );
    void UpdateAmmoDisplays( void );

private:
    CHandle<C_BaseCombatWeapon> m_hWeapon;
    CHandle<C_BaseEntity> m_hVehicle;
    CHudTexture *m_pAmmoIcon;
    int m_iAmmo, m_iAmmo2;

    CPanelAnimationVarAliasType( float, m_flDividerXPos, "divider_xpos", "0", "proportional_float" );
    CPanelAnimationVarAliasType( float, m_flDividerYPos, "divider_ypos", "0", "proportional_float" );
    CPanelAnimationVarAliasType( float, m_flDividerWide, "divider_wide", "0", "proportional_float" );
    CPanelAnimationVarAliasType( float, m_flDividerTall, "divider_tall", "0", "proportional_float" );
};
DECLARE_HUDELEMENT( CHudPrimaryAmmo );

//-----------------------------------------------------------------------------
// Purpose: Displays secondary (alternative, ie. smg grenades) ammunition level.
//-----------------------------------------------------------------------------
class CHudSecondaryAmmo : public CHudNumericDisplay, public CHudElement {
    DECLARE_CLASS_SIMPLE( CHudSecondaryAmmo, CHudNumericDisplay );
public:
    CHudSecondaryAmmo( const char *pszElementName );
    void Init( void ) { }
    void VidInit( void ) { }
    void SetAmmo( int iAmmo );
    void Reset( void );
    void Paint( void );

protected:
    void OnThink( void );
    void UpdateAmmoState( void );

private:
    CHandle<C_BaseCombatWeapon> m_hWeapon;
    CHudTexture *m_pAmmoIcon;
    int m_iAmmo;
};
DECLARE_HUDELEMENT( CHudSecondaryAmmo );

CHudPrimaryAmmo::CHudPrimaryAmmo( const char *pszElementName ) : BaseClass( NULL, "HudPrimaryAmmo" ), CHudElement( pszElementName )
{
    SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_WEAPONSELECTION );
    SetIndent( true );  // SetIndent1() ?
}

void CHudPrimaryAmmo::Init( void )
{
    m_iAmmo = -1;
    m_iAmmo2 = -1;
    m_pAmmoIcon = NULL;
}

void CHudPrimaryAmmo::Reset( void )
{
    BaseClass::Reset();
    m_hWeapon = NULL;
    m_hVehicle = NULL;
    m_pAmmoIcon = NULL;
    m_iAmmo = 0;
    m_iAmmo2 = 0;
    UpdateAmmoDisplays();
}

void CHudPrimaryAmmo::SetAmmo( int iAmmo )
{
    if( m_iAmmo != iAmmo ) {
        if( iAmmo == 0 )
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AmmoPrimaryEmpty" );
        else if( iAmmo < m_iAmmo )
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AmmoPrimaryDecreased" );
        else
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AmmoPrimaryIncreased" );
        m_iAmmo = iAmmo;
    }
    SetDisplayValue( m_iAmmo );
}

void CHudPrimaryAmmo::SetAmmo2( int iAmmo2 )
{
    if( m_iAmmo2 != iAmmo2 ) {
        if( iAmmo2 == 0 )
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AmmoPrimary2Empty" );
        else if( iAmmo2 < m_iAmmo2 )
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AmmoPrimary2Decreased" );
        else
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AmmoPrimary2Increased" );
        m_iAmmo2 = iAmmo2;
    }
    SetSecondaryValue( m_iAmmo2 );
}

void CHudPrimaryAmmo::Paint( void )
{
    BaseClass::Paint();
    if( m_hVehicle == NULL && m_pAmmoIcon )
        m_pAmmoIcon->DrawSelf( text_xpos, text_ypos, GetFgColor() );
    g_pVGuiSurface->DrawSetColor( GetFgColor() );
    g_pVGuiSurface->DrawFilledRect( m_flDividerXPos, m_flDividerYPos, m_flDividerXPos + m_flDividerWide, m_flDividerYPos + m_flDividerTall );
}

void CHudPrimaryAmmo::OnThink( void )
{
    UpdateAmmoDisplays();
}

void CHudPrimaryAmmo::UpdatePlayerAmmo( C_BasePlayer *pPlayer )
{
    m_hVehicle = NULL;
    C_BaseCombatWeapon *pWeapon = GetActiveWeapon();
    if( !pPlayer || !pWeapon || !pWeapon->UsesPrimaryAmmo() ) {
        SetPaintEnabled( false );
        SetPaintBackgroundEnabled( false );
        return;
    }

    SetPaintEnabled( true );
    SetPaintBackgroundEnabled( true );

    m_pAmmoIcon = gWR.GetAmmoIconFromWeapon( pWeapon->GetPrimaryAmmoType() );

    int iAmmo1 = pWeapon->Clip1();
    int iAmmo2 = 0;
    if( iAmmo1 < 0 )
        iAmmo1 = pPlayer->GetAmmoCount( pWeapon->GetPrimaryAmmoType() );
    else
        iAmmo2 = pPlayer->GetAmmoCount( pWeapon->GetPrimaryAmmoType() );

    SetAmmo( iAmmo1 );
    SetAmmo2( iAmmo2 );
    if( m_hWeapon != pWeapon ) {
        SetShouldDisplaySecondaryValue( pWeapon->UsesClipsForAmmo1() );
        g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "WeaponChanged" );
        m_hWeapon = pWeapon;
    }
}

void CHudPrimaryAmmo::UpdateVehicleAmmo( C_BasePlayer *pPlayer, IClientVehicle *pVehicle )
{
    m_hWeapon = NULL;
    C_BaseEntity *pVehicleEntity = pVehicle->GetVehicleEnt();
    if( !pVehicleEntity || pVehicle->GetPrimaryAmmoType() < 0 ) {
        SetPaintEnabled( false );
        SetPaintBackgroundEnabled( false );
        return;
    }

    SetPaintEnabled( true );
    SetPaintBackgroundEnabled( true );

    m_pAmmoIcon = gWR.GetAmmoIconFromWeapon( pVehicle->GetPrimaryAmmoType() );

    int iAmmo1 = pVehicle->GetPrimaryAmmoClip();
    int iAmmo2 = 0;
    if( iAmmo1 < 0 )
        iAmmo1 = pVehicle->GetPrimaryAmmoCount();
    else
        iAmmo2 = pVehicle->GetPrimaryAmmoCount();

    SetAmmo( iAmmo1 );
    SetAmmo2( iAmmo2 );
    if( pVehicleEntity != m_hVehicle ) {
        SetShouldDisplaySecondaryValue( false );
        g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "WeaponChanged" );
        m_hVehicle = pVehicleEntity;
    }
}

void CHudPrimaryAmmo::UpdateAmmoDisplays( void )
{
    C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
    IClientVehicle *pVehicle = pPlayer ? pPlayer->GetVehicle() : NULL;
    if( pVehicle )
        UpdateVehicleAmmo( pPlayer, pVehicle );
    else
        UpdatePlayerAmmo( pPlayer );
}

CHudSecondaryAmmo::CHudSecondaryAmmo( const char *pszElementName ) : BaseClass( NULL, "HudSecondaryAmmo" ), CHudElement( pszElementName )
{
    m_iAmmo = -1;
    SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_WEAPONSELECTION | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
    SetShouldDisplayValue( true );
    SetIndent( true );  // SetIndent1()?
}

void CHudSecondaryAmmo::SetAmmo( int iAmmo )
{
    if( m_iAmmo != iAmmo ) {
        if( iAmmo == 0 )
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AmmoSecondaryEmpty" );
        else if( iAmmo < m_iAmmo )
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AmmoSecondaryDecreased" );
        else
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "AmmoSecondaryIncreased" );
        m_iAmmo = iAmmo;
    }
    SetDisplayValue( m_iAmmo );
}

void CHudSecondaryAmmo::Reset( void )
{
    BaseClass::Reset();
    m_hWeapon = NULL;
    m_iAmmo = 0;
    SetAlpha( 0 );
    UpdateAmmoState();
}

void CHudSecondaryAmmo::Paint( void )
{
    BaseClass::Paint();
    if( m_pAmmoIcon )
        m_pAmmoIcon->DrawSelf( text_xpos, text_ypos, GetFgColor() );
}

void CHudSecondaryAmmo::OnThink( void )
{
    C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
    C_BaseCombatWeapon *pWeapon = GetActiveWeapon();
    IClientVehicle *pVehicle = pPlayer ? pPlayer->GetVehicle() : NULL;
    if( !pPlayer || !pWeapon || pVehicle ) {
        m_hWeapon = NULL;
        SetPaintEnabled( false );
        SetPaintBackgroundEnabled( false );
        return;
    }

    SetPaintEnabled( true );
    SetPaintBackgroundEnabled( true );
    UpdateAmmoState();
}

void CHudSecondaryAmmo::UpdateAmmoState( void )
{
    C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
    C_BaseCombatWeapon *pWeapon = GetActiveWeapon();
    
    if( pPlayer && pWeapon && pWeapon->UsesSecondaryAmmo() )
        SetAmmo( pPlayer->GetAmmoCount( pWeapon->GetSecondaryAmmoType() ) );

    if( m_hWeapon != pWeapon ) {
        if( pWeapon->UsesSecondaryAmmo() )
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "WeaponUsesSecondaryAmmo" );
        else
            g_pClientMode->GetViewportAnimationController()->StartAnimationSequence( "WeaponDoesNotUseSecondaryAmmo" );
        m_hWeapon = pWeapon;
        m_pAmmoIcon = m_hWeapon->GetWpnData().iconAmmo2;
    }
}

