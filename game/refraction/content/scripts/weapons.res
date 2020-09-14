"WpnScripts"
{
    weapon_357
    {
        "printname"             "#Refraction_357Handgun"
        "viewmodel"             "models/weapons/v_357.mdl"
        "playermodel"           "models/weapons/w_357.mdl"
        "anim_prefix"           "python"
        "bucket"                "1"
        "bucket_position"       "1"
        "bucket_360"            "0"
        "bucket_position_360"   "1"

        "clip_size"             "6"
        "default_clip"          "6"
        "primary_ammo"          "357"
        "secondary_ammo"        "None"

        "weight"                "7"
        "rumble"                "2"
        "item_flags"            "0"

        "SoundData"
        {
            "empty"     "Weapon_Pistol.Empty"
            "primary"   "Weapon_357.Single"
        }

        
        "TextureData"
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "e"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "e"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "e"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "q"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_alyxgun
    {
        
        "printname"             "#Refraction_AlyxGun"
        "viewmodel"             "models/weapons/W_Alyx_Gun.mdl"
        "playermodel"           "models/weapons/W_Alyx_Gun.mdl" //FIXME: 
        "anim_prefix"           "alyxgun"
        "bucket"                "1"
        "bucket_position"       "4"
        
        "clip_size"             "30"
        "clip2_size"            "-1"
        "default_clip"          "30"
        "default_clip2"         "-1"
        "primary_ammo"          "AlyxGun"
        
        "weight"                "2"
        "item_flags"            "0"

        SoundData
        {

            "reload"        "Weapon_Pistol.Reload"
            "reload_npc"    "Weapon_Pistol.NPC_Reload"
            "empty"         "Weapon_Pistol.Empty"
            "primary"       "Weapon_Pistol.Single"
            "primary_npc"   "Weapon_Pistol.NPC_Single"
            "special1"      "Weapon_Pistol.Special1"
            "special2"      "Weapon_Pistol.Special2"
            "burst"         "Weapon_Pistol.Burst"
        }

        TextureData
        {
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "p"
            }
            "crosshair"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_annabelle
    {
        
        "printname"         "#Refraction_Annabelle"
        "viewmodel"         "models/weapons/v_shotgun.mdl"
        "playermodel"       "models/weapons/w_annabelle.mdl"
        "anim_prefix"       "shotgun"
        "bucket"            "2"
        "bucket_position"   "1"
        
        "clip_size"         "2"
        "primary_ammo"      "357"
        "secondary_ammo"    "None"

        "weight"            "2"
        "item_flags"        "0"

        SoundData
        {

            "empty"         "Weapon_Shotgun.Empty"
            "reload"        "Weapon_Shotgun.Reload"
            "special1"      "Weapon_Shotgun.Special1"
            "primary"       "Weapon_Shotgun.Single"
            "secondary"     "Weapon_Shotgun.Double"
            "reload_npc"    "Weapon_Shotgun.NPC_Reload"
            "primary_npc"   "Weapon_Shotgun.NPC_Single"
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "b"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "b"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "b"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "s"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_ar2
    {
        "printname"             "#Refraction_Pulse_Rifle"
        "viewmodel"             "models/weapons/v_irifle.mdl"
        "playermodel"           "models/weapons/w_irifle.mdl"
        "anim_prefix"           "ar2"
        "bucket"                "2"
        "bucket_position"       "1"
        "bucket_360"            "1"
        "bucket_position_360"   "2"

        "clip_size"             "30"
        "default_clip"          "30"
        "clip2_size"            "-1"
        "default_clip2"         "-1"
        "primary_ammo"          "AR2"
        "secondary_ammo"        "AR2AltFire"

        "weight"                "5"
        "rumble"                "4"
        "item_flags"            "0"

        
        SoundData
        {
            "special1"      "Weapon_CombineGuard.Special1"
            "empty"         "Weapon_IRifle.Empty"
            "secondary"     "Weapon_IRifle.Single"
            "reload"        "Weapon_AR2.Reload"
            "primary"       "Weapon_AR2.Single"

            // NPC SECTION
            "primary_npc"   "Weapon_AR2.NPC_Single"
            "reload_npc"    "Weapon_AR2.NPC_Reload"
            "secondary_npc" "Weapon_AR2.NPC_Double"
        }

        
        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "l"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "l"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "l"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "u"
            }
            "ammo2"
            {
                "font"      "WeaponIconsSmall"
                "character" "z"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_bugbait
    {
        "printname"             "#Refraction_Bugbait"
        "viewmodel"             "models/weapons/v_bugbait.mdl"
        "playermodel"           "models/weapons/w_bugbait.mdl"
        "anim_prefix"           "Grenade"
        "bucket"                "5"
        "bucket_position"       "0"
        "bucket_360"            "3"
        "bucket_position_360"   "2"

        "clip_size"             "0"
        "clip2_size"            "0"
        "primary_ammo"          "None"
        "secondary_ammo"        "None"
        "default_clip"          "0"
        "default_clip2"         "0"

        "autoswitchto"          "0"
        "autoswitchfrom"        "0"
        "weight"                "0"
        "item_flags"            "0"
        
        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "j"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "j"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "j"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "48"
                "y"         "72"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_citizenpackage
    {
        "printname"             "#Refraction_SMG1"
        "viewmodel"             "models/weapons/w_package.mdl"
        "playermodel"           "models/weapons/w_package.mdl"
        "anim_prefix"           "smg2"
        "bucket"                "2"
        "bucket_position"       "0"

        "clip_size"             "-1"
        "clip2_size"            "-1"
        "default_clip"          "-1"
        "default_clip2"         "-1"
        "primary_ammo"          "None"
        "secondary_ammo"        "None"

        "weight"                "0"
        "item_flags"            "0"

        SoundData
        {
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "b"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "b"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "b"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "s"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_citizensuitcase
    {
        "printname"             "#Refraction_SMG1"
        "viewmodel"             "models/weapons/w_suitcase_passenger.mdl"
        "playermodel"           "models/weapons/w_suitcase_passenger.mdl"
        "anim_prefix"           "smg2"
        "bucket"                "2"
        "bucket_position"       "0"

        "clip_size"             "-1"
        "clip2_size"            "-1"
        "default_clip"          "-1"
        "default_clip2"         "-1"
        "primary_ammo"          "None"
        "secondary_ammo"        "None"

        "weight"                "0"
        "item_flags"            "0"

        SoundData
        {
        }

        TextureData
        {
            "weapon"
            {
                "file"      "sprites/w_icons1"
                "x"         "128"
                "y"         "128"
                "width"     "128"
                "height"    "64"
            }
            "weapon_s"
            {
                "file"      "sprites/w_icons1b"
                "x"         "128"
                "y"         "128"
                "width"     "128"
                "height"    "64"
            }
            "ammo"
            {
                "file"      "sprites/a_icons1"
                "x"         "55"
                "y"         "60"
                "width"     "73"
                "height"    "15"
            }
            "crosshair"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_crossbow
    {
        "printname"             "#Refraction_Crossbow"
        "viewmodel"             "models/weapons/v_crossbow.mdl"
        "playermodel"           "models/weapons/w_crossbow.mdl"
        "anim_prefix"           "bow"
        "bucket"                "3"
        "bucket_position"       "1"
        "bucket_360"            "0"
        "bucket_position_360"   "2"

        "clip_size"             "1"
        "default_clip"          "5"
        "primary_ammo"          "XBowBolt"
        "secondary_ammo"        "None"

        "weight"                "6"
        "item_flags"            "0"

        SoundData
        {
            "primary"   "Weapon_Crossbow.Single"
            "reload"    "Weapon_Crossbow.Reload"
            "special1"  "Weapon_Crossbow.BoltElectrify"
            "special2"  "Weapon_Crossbow.BoltFly"
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "g"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "g"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "g"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "w"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }       
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"     "0"
                "y"     "96"
                "width"     "24"
                "height"    "24"
            }
            "zoom"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }       
            "zoom_autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"     "24"
                "y"     "112"
                "width"     "104"
                "height"    "16"
            }
        }
    }
    
    weapon_crowbar
    {
        "printname"             "#Refraction_Crowbar"
        "viewmodel"             "models/weapons/v_crowbar.mdl"
        "playermodel"           "models/weapons/w_crowbar.mdl"
        "anim_prefix"           "crowbar"
        "bucket"                "0"
        "bucket_position"       "0"
        "bucket_360"            "2"
        "bucket_position_360"   "0"

        "clip_size"             "-1"
        "primary_ammo"          "None"
        "secondary_ammo"        "None"

        "weight"                "0"
        "item_flags"            "0"

        SoundData
        {
            "primary"           "Weapon_Crowbar.Single"
            "melee_hit"         "Weapon_Crowbar.Melee_Hit"
            "melee_hit_world"   "Weapon_Crowbar.Melee_HitWorld"
        }
        
        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "c"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "c"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "c"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_cubemap
    {
        "printname"         "#Refraction_Cubemap"
        "viewmodel"         "models/shadertest/envballs.mdl"
        "playermodel"       "models/shadertest/envballs.mdl"
        "anim_prefix"       "envballs"
        "bucket"            "2"
        "bucket_position"   "0"

        "clip_size"         "-1"
        "clip2_size"        "-1"

        "default_clip"      "-1"
        "default_clip2"     "-1"

        "primary_ammo"      "None"
        "secondary_ammo"    "None"

        "weight"            "0"
        "item_flags"        "0"

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "b"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "b"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "b"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "s"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_frag
    {
        "printname"             "#Refraction_Grenade"
        "viewmodel"             "models/weapons/v_grenade.mdl"
        "playermodel"           "models/items/grenadeAmmo.mdl"
        "anim_prefix"           "Grenade"
        "bucket"                "4"
        "bucket_position"       "0"
        "bucket_360"            "3"
        "bucket_position_360"   "0"

        "clip_size"             "-1"
        "clip2_size"            "-1"
        "default_clip"          "1"
        "default_clip2"         "-1"

        "primary_ammo"          "grenade"
        "secondary_ammo"        "None"

        "weight"                "1"
        "item_flags"            "18"    // ITEM_FLAG_NOAUTORELOAD | ITEM_FLAG_EXHAUSTIBLE

        SoundData
        {
            "special1"  "WeaponFrag.Roll"
            "secondary" "common/null.wav"
            "primary"   "common/null.wav"
        }
        
        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "k"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "k"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "k"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "v"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "48"
                "y"         "72"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_physcannon
    {
        "printname"         "#Refraction_GravityGun"
        "viewmodel"         "models/weapons/v_physcannon.mdl"
        "playermodel"       "models/weapons/w_Physics.mdl"
        "anim_prefix"       "gauss"
        "bucket"            "0"
        "bucket_position"   "1"
        "bucket_360"            "2"
        "bucket_position_360"   "1"
        "clip_size"         "1"
        "clip2_size"        "0"
        "primary_ammo"      "None"
        "secondary_ammo"    "None"
        "default_clip"      "0"
        "default_clip2"     "0"
        "weight"            "0"
        "autoswitchto"      "0"
        "autoswitchfrom"    "0"
        "item_flags"        "2"

        SoundData
        {
            "primary"       "Weapon_PhysCannon.Launch"
            "reload"        "Weapon_PhysCannon.Charge"
            "empty"         "Weapon_PhysCannon.DryFire"
            "special1"      "Weapon_PhysCannon.Pickup"
            "special2"      "Weapon_PhysCannon.OpenClaws"
            "melee_hit"     "Weapon_PhysCannon.CloseClaws"
            "melee_miss"    "Weapon_PhysCannon.Drop"
            "special3"      "Weapon_PhysCannon.TooHeavy"
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "m"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "m"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "48"
                "y"         "72"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    // What the fuck is that? Does it even work?!
    weapon_physgun
    {
        "printname"             "#Refraction_Physgun"
        "viewmodel"             "models/weapons/v_Physics.mdl"
        "playermodel"           "models/weapons/w_Physics.mdl"
        "anim_prefix"           "physgun"
        "bucket"                "4"
        "bucket_position"       "4"

        "clip_size"             "1"
        "clip2_size"            "8"
        "primary_ammo"          "None"
        "secondary_ammo"        "Gravity"
        "default_clip2"         "8"

        "weight"                "0"
        "item_flags"            "2"

        SoundData
        {
            "primary"           "Weapon_Physgun.On"
            "reload"            "Weapon_Physgun.Off"
            "special1"          "Weapon_Physgun.Special1"
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "m"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "m"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "48"
                "y"         "72"
                "width"     "24"
                "height"    "24"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "48"
                "y"         "72"
                "width"     "24"
                "height"    "24"
            }
        }
    }

    weapon_pistol
    {
        "printname"             "#Refraction_Pistol"
        "viewmodel"             "models/weapons/v_pistol.mdl"
        "playermodel"           "models/weapons/w_pistol.mdl"
        "anim_prefix"           "pistol"
        "bucket"                "1"
        "bucket_position"       "0"
        "bucket_360"            "0"
        "bucket_position_360"   "0"

        "clip_size"             "18"
        "primary_ammo"          "Pistol"
        "secondary_ammo"        "None"

        "weight"                "2"
        "rumble"                "1"
        "item_flags"            "0"

        SoundData
        {
            "reload"        "Weapon_Pistol.Reload"
            "reload_npc"    "Weapon_Pistol.NPC_Reload"
            "empty"         "Weapon_Pistol.Empty"
            "primary"       "Weapon_Pistol.Single"
            "primary_npc"   "Weapon_Pistol.NPC_Single"
            "special1"      "Weapon_Pistol.Special1"
            "special2"      "Weapon_Pistol.Special2"
            "burst"         "Weapon_Pistol.Burst"
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "d"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "d"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "d"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "p"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_rpg
    {
        "printname"             "#Refraction_RPG"
        "viewmodel"             "models/weapons/v_rpg.mdl"
        "playermodel"           "models/weapons/w_rocket_launcher.mdl"
        "anim_prefix"           "missile launcher"
        "bucket"                "4"
        "bucket_position"       "1"
        "autoswitchfrom"        "0"
        "bucket_360"            "3"
        "bucket_position_360"   "1"

        "clip_size"             "-1"
        "clip2_size"            "-1"
        "default_clip"          "3"
        "default_clip2"         "-1"

        "primary_ammo"          "rpg_round"
        "secondary_ammo"        "None"

        "autoswitchto"          "0"
        "weight"                "0"
        "item_flags"            "2"

        SoundData
        {
            "primary"           "Weapon_RPG.Single"
            "primary_npc"       "Weapon_RPG.NPC_Single"
            "special1"          "Weapon_RPG.LaserOn"
            "special2"          "Weapon_RPG.LaserOff"
            "empty"             "Weapon_SMG1.Empty"
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "i"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "i"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "i"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "x"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "48"
                "y"         "72"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_shotgun  // weapon_shitgun
    {
        "printname"             "#Refraction_Shotgun"
        "viewmodel"             "models/weapons/v_shotgun.mdl"
        "playermodel"           "models/weapons/w_shotgun.mdl"
        "anim_prefix"           "shotgun"
        "bucket"                "3"
        "bucket_position"       "0"
        "bucket_360"            "1"
        "bucket_position_360"   "1"

        "clip_size"             "6"
        "primary_ammo"          "Buckshot"
        "secondary_ammo"        "None"

        "weight"                "4"
        "rumble"                "5"
        "item_flags"            "0"

        SoundData
        {
            "empty"             "Weapon_Shotgun.Empty"
            "reload"            "Weapon_Shotgun.Reload"
            "special1"          "Weapon_Shotgun.Special1"
            "primary"           "Weapon_Shotgun.Single"
            "secondary"         "Weapon_Shotgun.Double"
            "reload_npc"        "Weapon_Shotgun.NPC_Reload"
            "primary_npc"       "Weapon_Shotgun.NPC_Single"
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "b"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "b"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "b"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "s"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_smg1
    {
        "printname"             "#Refraction_SMG1"
        "viewmodel"             "models/weapons/v_smg1.mdl"
        "playermodel"           "models/weapons/w_smg1.mdl"
        "anim_prefix"           "smg2"
        "bucket"                "2"
        "bucket_position"       "0"
        "bucket_360"            "1"
        "bucket_position_360"   "0"

        "clip_size"             "45"
        "clip2_size"            "-1"
        "default_clip"          "45"
        "default_clip2"         "-1"
        "primary_ammo"          "SMG1"
        "secondary_ammo"        "SMG1_Grenade"

        "weight"                "3"
        "rumble"                "3"
        "item_flags"            "0"

        SoundData
        {
            "reload"            "Weapon_SMG1.Reload"
            "reload_npc"        "Weapon_SMG1.NPC_Reload"
            "empty"             "Weapon_SMG1.Empty"
            "primary"           "Weapon_SMG1.Single"
            "primary_npc"       "Weapon_SMG1.NPC_Single"
            "special1"          "Weapon_SMG1.Special1"
            "special2"          "Weapon_SMG1.Special2"
            "secondary"         "Weapon_SMG1.Double"
            "burst"             "Weapon_SMG1.Burst"
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "a"
            }
            "weapon_s"
            {   
                "font"      "WeaponIconsSelected"
                "character" "a"
            }
            "weapon_small"
            {
                "font"      "WeaponIconsSmall"
                "character" "a"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "r"
            }
            "ammo2"
            {
                "font"      "WeaponIconsSmall"
                "character" "t"
            }
            "crosshair"
            {
                "font"      "Crosshairs"
                "character" "Q"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
        }
    }
    
    weapon_stunstick
    {
        "printname"             "#Refraction_StunBaton"
        "viewmodel"             "models/weapons/v_stunbaton.mdl"
        "playermodel"           "models/weapons/w_stunbaton.mdl"
        "anim_prefix"           "stunbaton"
        "bucket"                "0"
        "bucket_position"       "3"

        "clip_size"             "-1"
        "primary_ammo"          "None"
        "secondary_ammo"        "None"

        "weight"                "0"
        "item_flags"            "0"

        SoundData
        {

            "primary"           "Weapon_StunStick.Swing"
            "melee_miss"        "Weapon_StunStick.Melee_Miss"
            "melee_hit"         "Weapon_StunStick.Melee_Hit"
            "melee_hit_world"   "Weapon_StunStick.Melee_HitWorld"
        }

        TextureData
        {
            "weapon"
            {
                "font"      "WeaponIcons"
                "character" "n"
            }
            "weapon_s"
            {
                "font"      "WeaponIconsSelected"
                "character" "n"
            }
            "ammo"
            {
                "font"      "WeaponIconsSmall"
                "character" "n"
            }
            "crosshair"
            {
                "file"      "sprites/crosshairs"
                "x"         "0"
                "y"         "48"
                "width"     "24"
                "height"    "24"
            }
            "autoaim"
            {
                "file"      "sprites/crosshairs"
                "x"         "48"
                "y"         "72"
                "width"     "24"
                "height"    "24"
            }
        }
    }
}
