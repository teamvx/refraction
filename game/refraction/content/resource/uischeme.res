///////////////////////////////////////////////////////////
// sections:
//      Colors          - all the colors used by the scheme
//      BaseSettings    - contains settings for app to use to draw controls
//      Fonts           - list of all the fonts used by app
//
///////////////////////////////////////////////////////////
Scheme
{
    //////////////////////// COLORS ///////////////////////////
    // color details
    // this is a list of all the colors used by the scheme
    Colors
    {
        "MenuText"      "255 255 255 255"
        "MenuBack"      "112 120 128 200"
        "LogoText"      "255 255 255 200"
    }
    
    ///////////////////// BASE SETTINGS ////////////////////////
    //
    // default settings for all panels
    // controls use these to determine their settings
    BaseSettings
    {
        "GameLogo.X"                    "48"
        "GameLogo.Y"                    "168"
        "GameLogo.Width"                "592"
        "GameLogo.Height"               "64"
        "GameLogo.Font"                 "GameTitle"
        "GameLogo.Color"                "LogoText"
        
        "GameMenu.X"                    "53"
        "GameMenu.Y"                    "240"
        "GameMenu.Width"                "128"
        
        "GameMenu.Button.FgColor"       "MenuText"
        "GameMenu.Button.BgColor"       "MenuBack"
        "GameMenu.Button.Font"          "MenuButton"
        "GameMenu.Button.TextX"         "5"
        "GameMenu.Button.TextY"         "3"
        "GameMenu.Button.Width"         "128"
        "GameMenu.Button.Height"        "20"
        "GameMenu.Button.RolloverSound" "ui/buttonrollover.wav"
        "GameMenu.Button.ClickSound"    "ui/buttonclickrelease.wav"
    }
    
    //
    //////////////////////// FONTS /////////////////////////////
    //
    // describes all the fonts
    Fonts
    {
        "Default"
        {
            "1"
            {
                "name"      "Tahoma"
                "tall"      "16"
                "weight"    "500"
            }
        }

        "MenuButton"
        {
            "1"
            {
                "name"      "Verdana"
                "tall"      "15"
                "weight"    "500"
                "antialias" "1"
            }
        }

        "GameTitle"
        {
            "1"
            {
                "name"          "K12HL2"
                "tall"          "64"
                "weight"        "400"
                "antialias"     "1"
                "custom"        "1"
                "proportional"  "1"
            }
        }
    }

    //////////////////////// CUSTOM FONT FILES /////////////////////////////
    //
    // specifies all the custom (non-system) font files that need to be loaded to service the above described fonts
    CustomFontFiles
    {
        "1"     "resource/K12HL2.ttf"       //Logo
        "2"     "resource/HALFLIFE2.ttf"
    }
}