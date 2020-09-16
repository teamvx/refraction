/////////////////////////////////////////////////////////////
// Tracker scheme resource file
//
// sections:
//      Colors          - all the colors used by the scheme
//      BaseSettings    - contains settings for app to use to draw controls
//      Fonts           - list of all the fonts used by app
//      Borders         - description of all the borders
//
///////////////////////////////////////////////////////////
Scheme
{
    //////////////////////// COLORS ///////////////////////////
    // color details
    // this is a list of all the colors used by the scheme
    Colors
    {
        "White"             "197 200 198 255"
        "OffWhite"          "166 169 167 255"
        "DullWhite"         "138 140 138 255"
        "Orange"            "222 147 95 255"
        "TransparentBlack"  "0 0 0 128"
        "Black"             "0 0 0 255"
        "Blank"             "0 0 0 0"
        "Background"        "29 31 33 255"
        "OffBackground"     "29 31 33 190"
        "LightBackground"   "55 59 65 255"
        "DarkGray"          "40 42 46 255"
    }

    ///////////////////// BASE SETTINGS ////////////////////////
    //
    // default settings for all panels
    // controls use these to determine their settings
    BaseSettings
    {
        // vgui_controls color specifications
        Border.Bright                                   "LightBackground"   // the lit side of a control
        Border.Dark                                     "LightBackground"   // the dark/unlit side of a control
        Border.Selection                                "LightBackground"   // the additional border color for displaying the default/selected button

        Button.TextColor                                "White"
        Button.BgColor                                  "Blank"
        Button.ArmedTextColor                           "White"
        Button.ArmedBgColor                             "Blank"
        Button.DepressedTextColor                       "White"
        Button.DepressedBgColor                         "Blank"
        Button.FocusBorderColor                         "Black"
        
        CheckButton.TextColor                           "DullWhite"
        CheckButton.BgColor                             "TransparentBlack"
        CheckButton.Border1                             "Border.Dark"       // the left checkbutton border
        CheckButton.Border2                             "Border.Bright"     // the right checkbutton border
        CheckButton.Check                               "White"             // color of the check itself
        CheckButton.SelectedTextColor                   "White"
        CheckButton.DisabledFgColor                     "DarkGray"
        CheckButton.DisabledBgColor                     "LightBackground"
        CheckButton.ArmedBgColor                        "TransparentBlack"
        CheckButton.DepressedBgColor                    "TransparentBlack"
        CheckButton.HighlightFgColor                    "White"

        ComboBoxButton.ArrowColor                       "DullWhite"
        ComboBoxButton.ArmedArrowColor                  "White"
        ComboBoxButton.BgColor                          "Background"
        ComboBoxButton.DisabledBgColor                  "Background"

        Frame.TitleTextInsetX                           "16"
        Frame.ClientInsetX                              "8"
        Frame.ClientInsetY                              "6"
        Frame.BgColor                                   "Background"
        Frame.OutOfFocusBgColor                         "OffBackground"
        Frame.FocusTransitionEffectTime                 "0.3"   // time it takes for a window to fade in/out on focus/out of focus
        Frame.TransitionEffectTime                      "0.3"   // time it takes for a window to fade in/out on open/close
        Frame.AutoSnapRange                             "0"
        FrameGrip.Color1                                "DarkGray"
        FrameGrip.Color2                                "DarkGray"
        FrameTitleButton.FgColor                        "White"
        FrameTitleButton.BgColor                        "Blank"
        FrameTitleButton.DisabledFgColor                "OffWhite"
        FrameTitleButton.DisabledBgColor                "Blank"
        FrameSystemButton.FgColor                       "Blank"
        FrameSystemButton.BgColor                       "Blank"
        FrameSystemButton.Icon                          ""
        FrameSystemButton.DisabledIcon                  ""
        FrameTitleBar.Font                              "UiBold"
        FrameTitleBar.TextColor                         "White"
        FrameTitleBar.BgColor                           "Blank"
        FrameTitleBar.DisabledTextColor                 "255 255 255 192"
        FrameTitleBar.DisabledBgColor                   "Blank"

        GraphPanel.FgColor                              "White"
        GraphPanel.BgColor                              "TransparentBlack"

        Label.TextDullColor                             "DullWhite"
        Label.TextColor                                 "OffWhite"
        Label.TextBrightColor                           "White"
        Label.SelectedTextColor                         "White"
        Label.BgColor                                   "Blank"
        Label.DisabledFgColor1                          "117 117 117 255"
        Label.DisabledFgColor2                          "30 30 30 255"

        ListPanel.TextColor                             "White"
        ListPanel.TextBgColor                           "Blank"
        ListPanel.BgColor                               "TransparentBlack"
        ListPanel.SelectedTextColor                     "Black"
        ListPanel.SelectedBgColor                       "Orange"
        ListPanel.SelectedOutOfFocusBgColor             "255 155 0 128"
        ListPanel.EmptyListInfoTextColor                "OffWhite"

        Menu.TextColor                                  "White"
        Menu.BgColor                                    "DarkGray"
        Menu.ArmedTextColor                             "Black"
        Menu.ArmedBgColor                               "Orange"
        Menu.TextInset                                  "6"

        Panel.FgColor                                   "DullWhite"
        Panel.BgColor                                   "Blank"

        ProgressBar.FgColor                             "White"
        ProgressBar.BgColor                             "TransparentBlack"

        PropertySheet.TextColor                         "OffWhite"
        PropertySheet.SelectedTextColor                 "White"
        PropertySheet.TransitionEffectTime              "0.25"  // time to change from one tab to another

        RadioButton.TextColor                           "DullWhite"
        RadioButton.SelectedTextColor                   "White"

        RichText.TextColor                              "OffWhite"
        RichText.BgColor                                "TransparentBlack"
        RichText.SelectedTextColor                      "Black"
        RichText.SelectedBgColor                        "Orange"

        ScrollBar.Wide                                  "17"

        ScrollBarButton.FgColor                         "White"
        ScrollBarButton.BgColor                         "Blank"
        ScrollBarButton.ArmedFgColor                    "White"
        ScrollBarButton.ArmedBgColor                    "Blank"
        ScrollBarButton.DepressedFgColor                "White"
        ScrollBarButton.DepressedBgColor                "Blank"

        ScrollBarSlider.FgColor                         "Blank"             // nob color
        ScrollBarSlider.BgColor                         "255 255 255 64"    // slider background color

        SectionedListPanel.HeaderTextColor              "White"
        SectionedListPanel.HeaderBgColor                "Blank"
        SectionedListPanel.DividerColor                 "Black"
        SectionedListPanel.TextColor                    "DullWhite"
        SectionedListPanel.BrightTextColor              "White"
        SectionedListPanel.BgColor                      "TransparentBlack"
        SectionedListPanel.SelectedTextColor            "Black"
        SectionedListPanel.SelectedBgColor              "Orange"
        SectionedListPanel.OutOfFocusSelectedTextColor  "Black"
        SectionedListPanel.OutOfFocusSelectedBgColor    "255 155 0 128"

        Slider.NobColor                                 "108 108 108 255"
        Slider.TextColor                                "180 180 180 255"
        Slider.TrackColor                               "31 31 31 255"
        Slider.DisabledTextColor1                       "117 117 117 255"
        Slider.DisabledTextColor2                       "30 30 30 255"

        TextEntry.TextColor                             "OffWhite"
        TextEntry.BgColor                               "TransparentBlack"
        TextEntry.CursorColor                           "OffWhite"
        TextEntry.DisabledTextColor                     "DullWhite"
        TextEntry.DisabledBgColor                       "Blank"
        TextEntry.SelectedTextColor                     "Black"
        TextEntry.SelectedBgColor                       "Orange"
        TextEntry.OutOfFocusSelectedBgColor             "255 155 0 128"
        TextEntry.FocusEdgeColor                        "0 0 0 196"

        ToggleButton.SelectedTextColor                  "White"

        Tooltip.TextColor                               "0 0 0 196"
        Tooltip.BgColor                                 "Orange"

        TreeView.BgColor                                "TransparentBlack"

        WizardSubPanel.BgColor                          "Blank"

        // scheme-specific colors
        MainMenu.TextColor                              "White"
        MainMenu.ArmedTextColor                         "200 200 200 255"
        MainMenu.DepressedTextColor                     "192 186 80 255"
        MainMenu.MenuItemHeight                         "30"
        MainMenu.Inset                                  "32"
        MainMenu.Backdrop                               "0 0 0 156"

        Console.TextColor                               "OffWhite"
        Console.DevTextColor                            "White"

        NewGame.TextColor                               "White"
        NewGame.FillColor                               "0 0 0 255"
        NewGame.SelectionColor                          "Orange"
        NewGame.DisabledColor                           "128 128 128 196"
    }

    //
    //////////////////////// FONTS /////////////////////////////
    //
    // describes all the fonts
    Fonts
    {
        // fonts are used in order that they are listed
        // fonts listed later in the order will only be used if they fulfill a range not already filled
        // if a font fails to load then the subsequent fonts will replace
        
        "DebugFixed"
        {
            "1"
            {
                "name"      "Courier New"
                "tall"      "10"
                "weight"    "500"
                "antialias" "1"
            }
        }
        
        "DebugFixedSmall"
        {
            "1"
            {
                "name"      "Courier New"
                "tall"      "7"
                "weight"    "500"
                "antialias" "1"
            }
        }
        
        "DefaultFixedOutline"
        {
            "1"
            {
                "name"      "Lucida Console"
                "tall"      "10"
                "weight"    "0"
                "outline"   "1"
            }
        }
        
        "Default"
        {
            "1"
            {
                "name"      "Source Code Pro"
                "tall"      "15"
                "weight"    "0"
                "antialias" "1"
            }
        }
        
        "DefaultBold"
        {
            "1"
            {
                "name"      "Source Code Pro"
                "tall"      "15"
                "weight"    "550"
                "antialias" "1"
            }
        }
        
        "DefaultUnderline"
        {
            "1"
            {
                "name"      "Source Code Pro"
                "tall"      "15"
                "weight"    "0"
                "underline" "1"
                "antialias" "1"
            }
        }
        
        "DefaultSmall"
        {
            "1"
            {
                "name"      "Source Code Pro"
                "tall"      "13"
                "weight"    "550"
                "antialias" "1"
            }
        }
        
        "DefaultSmallDropShadow"
        {
            "1"
            {
                "name"      "Source Code Pro"
                "tall"      "13"
                "weight"    "400"
                "dropshadow" "1"
                "antialias" "1"
            }
        }
        
        "DefaultVerySmall"
        {
            "1"
            {
                "name"      "Source Code Pro"
                "tall"      "14"
                "weight"    "0"
                "antialias" "1"
            }
        }
        
        "DefaultLarge"
        {
            "1"
            {
                "name"      "Source Code Pro"
                "tall"      "18"
                "weight"    "0"
                "antialias" "1"
            }
        }
        
        "UiBold"
        {
            "1"
            {
                "name"      "Source Code Pro"
                "tall"      "12"
                "weight"    "1000"
                "antialias" "1"
                "antialias" "1"
            }
        }
        
        "ConsoleText"
        {
            "1"
            {
                "name"      "Source Code Pro"
                "tall"      "15"
                "weight"    "450"
                "antialias" "1"
            }
        }

        "Marlett"
        {
            "1"
            {
                "name"      "Marlett"
                "tall"      "14"
                "weight"    "0"
                "symbol"    "1"
            }
        }

        "Trebuchet24"
        {
            "1"
            {
                "name"      "Trebuchet MS"
                "tall"      "24"
                "weight"    "900"
            }
        }
        
        "Trebuchet20"
        {
            "1"
            {
                "name"      "Trebuchet MS"
                "tall"      "20"
                "weight"    "900"
            }
        }
        
        "Trebuchet18"
        {
            "1"
            {
                "name"      "Trebuchet MS"
                "tall"      "18"
                "weight"    "900"
            }
        }

        "DefaultFixed"
        {
            "1"
            {
                "name"      "Lucida Console"
                "tall"      "10"
                "weight"    "0"
            }
        }
        
        "DefaultFixedDropShadow"
        {
            "1"
            {
                "name"      "Lucida Console"
                "tall"      "10"
                "weight"    "0"
                "dropshadow" "1"
            }
        }

        "CloseCaption_Normal"
        {
            "1"
            {
                "name"      "Tahoma"
                "tall"      "16"
                "weight"    "500"
            }
        }
        
        "CloseCaption_Italic"
        {
            "1"
            {
                "name"      "Tahoma"
                "tall"      "16"
                "weight"    "500"
                "italic"    "1"
            }
        }
        
        "CloseCaption_Bold"
        {
            "1"
            {
                "name"      "Tahoma"
                "tall"      "16"
                "weight"    "900"
            }
        }

        "CloseCaption_BoldItalic"
        {
            "1"
            {
                "name"      "Tahoma"
                "tall"      "16"
                "weight"    "900"
                "italic"    "1"
            }
        }
    }

    //
    //////////////////// BORDERS //////////////////////////////
    //
    // describes all the border types
    Borders
    {
        BaseBorder          DepressedBorder
        ButtonBorder        RaisedBorder
        ComboBoxBorder      DepressedBorder
        MenuBorder          RaisedBorder
        BrowserBorder       DepressedBorder
        PropertySheetBorder RaisedBorder

        FrameBorder
        {
            // rounded corners for frames
            "backgroundtype" "0"
        }

        DepressedBorder
        {
            "inset" "0 0 1 1"
            Left
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 1"
                }
            }

            Right
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "1 0"
                }
            }

            Top
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }

            Bottom
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 0"
                }
            }
        }
        RaisedBorder
        {
            "inset" "0 0 1 1"
            Left
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 1"
                }
            }

            Right
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }

            Top
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 1"
                }
            }

            Bottom
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }
        }
        
        TitleButtonBorder
        {
            "backgroundtype" "0"
        }

        TitleButtonDisabledBorder
        {
            "backgroundtype" "0"
        }

        TitleButtonDepressedBorder
        {
            "backgroundtype" "0"
        }

        ScrollBarButtonBorder
        {
            "inset" "2 2 0 0"
            Left
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 1"
                }
            }

            Right
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "1 0"
                }
            }

            Top
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 0"
                }
            }

            Bottom
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }
        }
        
        ScrollBarButtonDepressedBorder
        {
            "inset" "2 2 0 0"
            Left
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 1"
                }
            }

            Right
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "1 0"
                }
            }

            Top
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }

            Bottom
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 0"
                }
            }
        }

        TabBorder
        {
            "inset" "0 0 1 1"
            Left
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 1"
                }
            }

            Right
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "1 0"
                }
            }

            Top
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 0"
                }
            }

        }

        TabActiveBorder
        {
            "inset" "0 0 1 0"
            Left
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 0"
                }
            }

            Right
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "1 0"
                }
            }

            Top
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 0"
                }
            }

        }


        ToolTipBorder
        {
            "inset" "0 0 1 0"
            Left
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }

            Right
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "1 0"
                }
            }

            Top
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }

            Bottom
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }
        }

        // this is the border used for default buttons (the button that gets pressed when you hit enter)
        ButtonKeyFocusBorder
        {
            "inset" "0 0 1 1"
            Left
            {
                "1"
                {
                    "color" "Border.Selection"
                    "offset" "0 0"
                }
                "2"
                {
                    "color" "Border.Bright"
                    "offset" "0 1"
                }
            }
            Top
            {
                "1"
                {
                    "color" "Border.Selection"
                    "offset" "0 0"
                }
                "2"
                {
                    "color" "Border.Bright"
                    "offset" "1 0"
                }
            }
            Right
            {
                "1"
                {
                    "color" "Border.Selection"
                    "offset" "0 0"
                }
                "2"
                {
                    "color" "Border.Dark"
                    "offset" "1 0"
                }
            }
            Bottom
            {
                "1"
                {
                    "color" "Border.Selection"
                    "offset" "0 0"
                }
                "2"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }
        }

        ButtonDepressedBorder
        {
            "inset" "2 1 1 1"
            Left
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 1"
                }
            }

            Right
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "1 0"
                }
            }

            Top
            {
                "1"
                {
                    "color" "Border.Dark"
                    "offset" "0 0"
                }
            }

            Bottom
            {
                "1"
                {
                    "color" "Border.Bright"
                    "offset" "0 0"
                }
            }
        }
    }

    //////////////////////// CUSTOM FONT FILES /////////////////////////////
    //
    // specifies all the custom (non-system) font files that need to be loaded to service the above described fonts
    CustomFontFiles
    {
        "1" "resource/HALFLIFE2.ttf"
        "2" "resource/sourcecodepro.ttf"
    }
}
