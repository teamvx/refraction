// ----------------------------------------------------------------------------
// CHROMATICABERRATION.CPP
//
// This file defines the C++ component of the chromatic aberration post processing shader.
// ----------------------------------------------------------------------------
#include "BaseVSShader.h"
#include "screenspaceeffect_vs30.inc"
#include "post_chromatic_aberration_ps30.inc"

BEGIN_SHADER( post_chromatic_aberration, "Adds a little color displacement at the screen borders" )
    BEGIN_SHADER_PARAMS
        SHADER_PARAM( AMOUNT, SHADER_PARAM_TYPE_FLOAT, "0.2", "Amount of aberration." )
    END_SHADER_PARAMS

    SHADER_INIT
    {
        if( !params[AMOUNT]->IsDefined() )
            params[AMOUNT]->SetFloatValue( 0.4 );
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
                DECLARE_STATIC_PIXEL_SHADER( post_chromatic_aberration_ps30 );
                SET_STATIC_PIXEL_SHADER( post_chromatic_aberration_ps30 );
            }
        }

        DYNAMIC_STATE
        {
            pShaderAPI->BindStandardTexture( SHADER_SAMPLER0, TEXTURE_FRAME_BUFFER_FULL_TEXTURE_0 );

            float c0 = params[AMOUNT]->GetFloatValue();
            pShaderAPI->SetPixelShaderConstant( 0, &c0, 1 );

            DECLARE_DYNAMIC_VERTEX_SHADER( screenspaceeffect_vs30 );
            SET_DYNAMIC_VERTEX_SHADER( screenspaceeffect_vs30 );

            if( g_pHardwareConfig->SupportsShaderModel_3_0() ) {
                DECLARE_DYNAMIC_PIXEL_SHADER( post_chromatic_aberration_ps30 );
                SET_DYNAMIC_PIXEL_SHADER( post_chromatic_aberration_ps30 );
            }
        }

        Draw();
    }

END_SHADER
