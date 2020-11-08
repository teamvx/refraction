#include "cbase.h"
#include "c_weapons.h"
#include "basehlcombatweapon_shared.h"
#include "c_basehlcombatweapon.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//
// UNUSED WEAPONS
// These weapons *exists* but in the "unused" subdirectory.
// They would somehow be re-used, I guess...
//
#if 0
STUB_CLIENTWEAPON( weapon_cguard,           CWeaponCGuard,          C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_flaregun,         Flaregun,               C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_hopwire,          WeaponHopwire,          C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_slam,             Weapon_SLAM,            C_BaseHLCombatWeapon );
#endif

//
// NPC-ONLY WEAPONS
// These weapons are supposed to be used by NPCs and not by the player.
// However it seems that the player can also pick them up :)
//
STUB_CLIENTWEAPON( weapon_annabelle,        WeaponAnnabelle,        C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_alyxgun,          WeaponAlyxGun,          C_HLSelectFireMachineGun );
STUB_CLIENTWEAPON( weapon_citizenpackage,   WeaponCitizenPackage,   C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_citizensuitcase,  WeaponCitizenSuitcase,  C_WeaponCitizenPackage );

//
// PLAYER AND NPC WEAPONS
// These weapons are supposed to be used by both NPCs and the player.
// I am not entirely sure about weapon_bugbait, but...
//
STUB_CLIENTWEAPON( weapon_357,              Weapon357,              C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_ar2,              WeaponAR2,              C_HLMachineGun );
STUB_CLIENTWEAPON( weapon_bugbait,          WeaponBugBait,          C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_crossbow,         WeaponCrossbow,         C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_crowbar,          WeaponCrowbar,          C_BaseHLBludgeonWeapon );
STUB_CLIENTWEAPON( weapon_frag,             WeaponFrag,             C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_pistol,           WeaponPistol,           C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_rpg,              WeaponRPG,              C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_shotgun,          WeaponShotgun,          C_BaseHLCombatWeapon );
STUB_CLIENTWEAPON( weapon_smg1,             WeaponSMG1,             C_HLSelectFireMachineGun );
