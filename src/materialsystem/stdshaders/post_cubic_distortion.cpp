// ----------------------------------------------------------------------------
// POST_CUBICDISTORTION.CPP
//
// This file defines the C++ component of the cubic distortion post processing shader.
// ----------------------------------------------------------------------------
#include "BaseVSShader.h"
#include "screenspaceeffect_vs30.inc"
#include "post_cubic_distortion_ps30.inc"

BEGIN_SHADER( post_cubic_distortion, "Adds lens effect..." )
    BEGIN_SHADER_PARAMS
        SHADER_PARAM( FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "Framebuffer texture." )
        SHADER_PARAM( DISTAMOUNT, SHADER_PARAM_TYPE_FLOAT, "0.15", "Amount of displacement." )
    END_SHADER_PARAMS

    SHADER_INIT
    {
        if( params[FBTEXTURE]->IsDefined() )
            LoadTexture( FBTEXTURE );
        if( !params[DISTAMOUNT]->IsDefined() )
            params[DISTAMOUNT]->SetFloatValue( 0.15 );
    }

    bool NeedsFullFrameBufferTexture( IMaterialVar **params, bool bCheckSpecificToThisFrame /* = true */ ) const
    {
        return true;
    }

    SHADER_FALLBACK
    {
        // Requires DX9 + above
        if( g_pHardwareConfig->GetDXSupportLevel() < 90 ) {
            Assert( 0 );
            return "Wireframe";
        }
        return 0;
    }

    SHADER_DRAW
    {
        SHADOW_STATE
        {
            pShaderShadow->EnableDepthWrites( false );
            pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );

            int fmt = VERTEX_POSITION;
            pShaderShadow->VertexShaderVertexFormat( fmt, 1, 0, 0 );

            // Pre-cache shaders
            DECLARE_STATIC_VERTEX_SHADER( screenspaceeffect_vs30 );
            SET_STATIC_VERTEX_SHADER( screenspaceeffect_vs30 );

            if( g_pHardwareConfig->SupportsShaderModel_3_0() ) {
                DECLARE_STATIC_PIXEL_SHADER( post_cubic_distortion_ps30 );
                SET_STATIC_PIXEL_SHADER( post_cubic_distortion_ps30 );
            }
        }

        DYNAMIC_STATE
        {
            // $FBTEXTURE
            BindTexture( SHADER_SAMPLER0, FBTEXTURE, -1 );

            // $AMOUNT
            float c0 = params[DISTAMOUNT]->GetFloatValue();
            pShaderAPI->SetPixelShaderConstant( 0, &c0, 1 );

            DECLARE_DYNAMIC_VERTEX_SHADER( screenspaceeffect_vs30 );
            SET_DYNAMIC_VERTEX_SHADER( screenspaceeffect_vs30 );

            if( g_pHardwareConfig->SupportsShaderModel_3_0() ) {
                DECLARE_DYNAMIC_PIXEL_SHADER( post_cubic_distortion_ps30 );
                SET_DYNAMIC_PIXEL_SHADER( post_cubic_distortion_ps30 );
            }
        }

        Draw();
    }

END_SHADER
