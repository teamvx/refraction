// ----------------------------------------------------------------------------
// POST_CUBICDISTORTION.CPP
//
// This file defines the C++ component of the cubic distortion post processing shader.
// ----------------------------------------------------------------------------
#include "BaseVSShader.h"
#include "post_generic_vs30.inc"
#include "post_cubic_distortion_ps30.inc"

BEGIN_SHADER( Post_CubicDistortion, "Adds lens effect..." )
    BEGIN_SHADER_PARAMS
        SHADER_PARAM( AMOUNT, SHADER_PARAM_TYPE_FLOAT, "0.2", "Amount of displacement." )
    END_SHADER_PARAMS

    SHADER_INIT
    {
        if( !params[AMOUNT]->IsDefined() )
            params[AMOUNT]->SetFloatValue( 0.2 );
    }

    bool NeedsFullFrameBufferTexture( IMaterialVar **params, bool bCheckSpecificToThisFrame ) const
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

            pShaderShadow->VertexShaderVertexFormat( VERTEX_POSITION, 1, 0, 0 );

            DECLARE_STATIC_VERTEX_SHADER( post_generic_vs30 );
            SET_STATIC_VERTEX_SHADER( post_generic_vs30 );

            if( g_pHardwareConfig->SupportsShaderModel_3_0() ) {
                DECLARE_STATIC_PIXEL_SHADER( post_cubic_distortion_ps30 );
                SET_STATIC_PIXEL_SHADER( post_cubic_distortion_ps30 );
            }
        }

        DYNAMIC_STATE
        {
            pShaderAPI->BindStandardTexture( SHADER_SAMPLER0, TEXTURE_FRAME_BUFFER_FULL_TEXTURE_0 );

            float c0 = params[AMOUNT]->GetFloatValue();
            pShaderAPI->SetPixelShaderConstant( 0, &c0, 1 );

            DECLARE_DYNAMIC_VERTEX_SHADER( post_generic_vs30 );
            SET_DYNAMIC_VERTEX_SHADER( post_generic_vs30 );

            if( g_pHardwareConfig->SupportsShaderModel_3_0() ) {
                DECLARE_DYNAMIC_PIXEL_SHADER( post_cubic_distortion_ps30 );
                SET_DYNAMIC_PIXEL_SHADER( post_cubic_distortion_ps30 );
            }
        }

        Draw();
    }

END_SHADER
