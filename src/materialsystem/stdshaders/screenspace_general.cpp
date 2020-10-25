// ----------------------------------------------------------------------------
// SCREENSPACE_GENERAL.CPP
//
// This file defines the C++ component of all screen-space effect shaders.
// ----------------------------------------------------------------------------
#include "BaseVSShader.h"
#include "screenspaceeffect_vs30.inc"

DEFINE_FALLBACK_SHADER( sdk_screenspace_general, sdk_screenspace_general_dx9 )
BEGIN_VS_SHADER_FLAGS( sdk_screenspace_general_dx9, "Common screen space shader.", SHADER_NOT_EDITABLE )
    BEGIN_SHADER_PARAMS
        SHADER_PARAM( C0_X,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C0_Y,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C0_Z,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C0_W,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C1_X,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C1_Y,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C1_Z,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C1_W,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C2_X,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C2_Y,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C2_Z,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C2_W,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C3_X,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C3_Y,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C3_Z,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( C3_W,                     SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( PIXSHADER,                SHADER_PARAM_TYPE_STRING,   "",     "Name of the pixel shader to use" )
        SHADER_PARAM( DISABLE_COLOR_WRITES,     SHADER_PARAM_TYPE_INTEGER,  "0",    "" )
        SHADER_PARAM( ALPHATESTED,              SHADER_PARAM_TYPE_FLOAT,    "0",    "" )
        SHADER_PARAM( TEXTURE1,                 SHADER_PARAM_TYPE_TEXTURE,  "",     "" )
        SHADER_PARAM( TEXTURE2,                 SHADER_PARAM_TYPE_TEXTURE,  "",     "" )
        SHADER_PARAM( TEXTURE3,                 SHADER_PARAM_TYPE_TEXTURE,  "",     "" )
        SHADER_PARAM( LINEARREAD_BASETEXTURE,   SHADER_PARAM_TYPE_INTEGER,  "0",    "" )
        SHADER_PARAM( LINEARREAD_TEXTURE1,      SHADER_PARAM_TYPE_INTEGER,  "0",    "" )
        SHADER_PARAM( LINEARREAD_TEXTURE2,      SHADER_PARAM_TYPE_INTEGER,  "0",    "" )
        SHADER_PARAM( LINEARREAD_TEXTURE3,      SHADER_PARAM_TYPE_INTEGER,  "0",    "" )
        SHADER_PARAM( LINEARWRITE,              SHADER_PARAM_TYPE_INTEGER,  "0",    "" )
        SHADER_PARAM( X360APPCHOOSER,           SHADER_PARAM_TYPE_INTEGER,  "0",    "Needed for movies in 360 launcher" )
    END_SHADER_PARAMS

    SHADER_INIT
    {
        if ( params[BASETEXTURE]->IsDefined() )
            LoadTexture( BASETEXTURE );
        if ( params[TEXTURE1]->IsDefined() )
            LoadTexture( TEXTURE1 );
        if ( params[TEXTURE2]->IsDefined() )
            LoadTexture( TEXTURE2 );
        if ( params[TEXTURE3]->IsDefined() )
            LoadTexture( TEXTURE3 );
    }

    SHADER_FALLBACK
    {
        if ( g_pHardwareConfig->GetDXSupportLevel() < 90 )
            return "screenspace_general_dx8";
        return 0;
    }

    SHADER_DRAW
    {
        SHADOW_STATE
        {
            pShaderShadow->EnableDepthWrites( false );

            if( params[BASETEXTURE]->IsDefined() ) {
                pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
                ITexture *pTexture = params[BASETEXTURE]->GetTextureValue();

                ImageFormat fmt = pTexture->GetImageFormat();
                if( fmt == IMAGE_FORMAT_RGBA16161616F || fmt == IMAGE_FORMAT_RGBA16161616 )
                    pShaderShadow->EnableSRGBRead( SHADER_SAMPLER0, false );
                else
                    pShaderShadow->EnableSRGBRead( SHADER_SAMPLER0, !params[LINEARREAD_BASETEXTURE]->IsDefined() || !params[LINEARREAD_BASETEXTURE]->GetIntValue() );
            }

            if( params[TEXTURE1]->IsDefined() ) {
                pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );
                ITexture *pTexture = params[TEXTURE1]->GetTextureValue();

                ImageFormat fmt = pTexture->GetImageFormat();
                if( fmt == IMAGE_FORMAT_RGBA16161616F || fmt == IMAGE_FORMAT_RGBA16161616 )
                    pShaderShadow->EnableSRGBRead( SHADER_SAMPLER1, false );
                else
                    pShaderShadow->EnableSRGBRead( SHADER_SAMPLER1, !params[LINEARREAD_TEXTURE1]->IsDefined() || !params[LINEARREAD_TEXTURE1]->GetIntValue() );
            }

            if( params[TEXTURE2]->IsDefined() ) {
                pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );
                ITexture *pTexture = params[TEXTURE2]->GetTextureValue();

                ImageFormat fmt = pTexture->GetImageFormat();
                if( fmt == IMAGE_FORMAT_RGBA16161616F || fmt == IMAGE_FORMAT_RGBA16161616 )
                    pShaderShadow->EnableSRGBRead( SHADER_SAMPLER2, false );
                else
                    pShaderShadow->EnableSRGBRead( SHADER_SAMPLER2, !params[LINEARREAD_TEXTURE2]->IsDefined() || !params[LINEARREAD_TEXTURE2]->GetIntValue() );
            }

            if( params[TEXTURE3]->IsDefined() ) {
                pShaderShadow->EnableTexture( SHADER_SAMPLER3, true );
                ITexture *pTexture = params[TEXTURE3]->GetTextureValue();

                ImageFormat fmt = pTexture->GetImageFormat();
                if( fmt == IMAGE_FORMAT_RGBA16161616F || fmt == IMAGE_FORMAT_RGBA16161616 )
                    pShaderShadow->EnableSRGBRead( SHADER_SAMPLER3, false );
                else
                    pShaderShadow->EnableSRGBRead( SHADER_SAMPLER3, !params[LINEARREAD_TEXTURE3]->IsDefined() || !params[LINEARREAD_TEXTURE3]->GetIntValue() );
            }

            int fmt = VERTEX_POSITION;
            if( IS_PARAM_DEFINED( X360APPCHOOSER ) && ( params[X360APPCHOOSER]->GetIntValue() ) ) {
                fmt |= VERTEX_COLOR;
                EnableAlphaBlending( SHADER_BLEND_SRC_ALPHA, SHADER_BLEND_ONE_MINUS_SRC_ALPHA );
            }
            pShaderShadow->VertexShaderVertexFormat( fmt, 1, 0, 0 );

            // maybe convert from linear to gamma on write.
            pShaderShadow->EnableSRGBWrite( params[LINEARWRITE]->GetFloatValue() == 0.0 );

            // Pre-cache shaders
            DECLARE_STATIC_VERTEX_SHADER( screenspaceeffect_vs30 );
            SET_STATIC_VERTEX_SHADER( screenspaceeffect_vs30 );

            if( params[DISABLE_COLOR_WRITES]->GetIntValue() )
                pShaderShadow->EnableColorWrites( false );

            pShaderShadow->EnableAlphaTest( true );
            pShaderShadow->AlphaFunc( SHADER_ALPHAFUNC_GREATER, 0.0 );

            if( IS_FLAG_SET( MATERIAL_VAR_ADDITIVE ) )
                EnableAlphaBlending( SHADER_BLEND_ONE, SHADER_BLEND_ONE );

            pShaderShadow->SetPixelShader( params[PIXSHADER]->GetStringValue(), 0 );
        }

        DYNAMIC_STATE
        {
            if( params[BASETEXTURE]->IsDefined() )
                BindTexture( SHADER_SAMPLER0, BASETEXTURE, -1 );
            if( params[TEXTURE1]->IsDefined() )
                BindTexture( SHADER_SAMPLER1, TEXTURE1, -1 );
            if( params[TEXTURE2]->IsDefined() )
                BindTexture( SHADER_SAMPLER2, TEXTURE2, -1 );
            if( params[TEXTURE3]->IsDefined() )
                BindTexture( SHADER_SAMPLER3, TEXTURE3, -1 );
            
            float c0[] = {
                params[C0_X]->GetFloatValue(),
                params[C0_Y]->GetFloatValue(),
                params[C0_Z]->GetFloatValue(),
                params[C0_W]->GetFloatValue(),
                params[C1_X]->GetFloatValue(),
                params[C1_Y]->GetFloatValue(),
                params[C1_Z]->GetFloatValue(),
                params[C1_W]->GetFloatValue(),
                params[C2_X]->GetFloatValue(),
                params[C2_Y]->GetFloatValue(),
                params[C2_Z]->GetFloatValue(),
                params[C2_W]->GetFloatValue(),
                params[C3_X]->GetFloatValue(),
                params[C3_Y]->GetFloatValue(),
                params[C3_Z]->GetFloatValue(),
                params[C3_W]->GetFloatValue()
            };

            pShaderAPI->SetPixelShaderConstant( 0, c0, ARRAYSIZE( c0 ) / 4 );

            float eyePos[4];
            pShaderAPI->GetWorldSpaceCameraPosition( eyePos );
            pShaderAPI->SetPixelShaderConstant( 10, eyePos, 1 );

            pShaderAPI->SetVertexShaderIndex( 0 );
            pShaderAPI->SetPixelShaderIndex( 0 );

            DECLARE_DYNAMIC_VERTEX_SHADER( screenspaceeffect_vs30 );
            SET_DYNAMIC_VERTEX_SHADER( screenspaceeffect_vs30 );
        }
        Draw();
    }
END_SHADER
