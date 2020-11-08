// ----------------------------------------------------------------------------
// POSTPROC_DESATURATE.CPP
//
// This file defines the C++ component of the desaturation post processing shader.
// ----------------------------------------------------------------------------
#include "BaseVSShader.h"
#include "postproc_vs30.inc"
#include "postproc_desaturate_ps30.inc"

BEGIN_SHADER( PostProc_Desaturate, "Makes bright pixels to have less colors" )
    BEGIN_SHADER_PARAMS
        SHADER_PARAM( FACTOR, SHADER_PARAM_TYPE_FLOAT, "0.45", "Grayscale lerp factor." )
    END_SHADER_PARAMS

    SHADER_INIT
    {
        if( !params[FACTOR]->IsDefined() )
            params[FACTOR]->SetFloatValue( 0.45 );
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

            DECLARE_STATIC_VERTEX_SHADER( postproc_vs30 );
            SET_STATIC_VERTEX_SHADER( postproc_vs30 );

            if( g_pHardwareConfig->SupportsShaderModel_3_0() ) {
                DECLARE_STATIC_PIXEL_SHADER( postproc_desaturate_ps30 );
                SET_STATIC_PIXEL_SHADER( postproc_desaturate_ps30 );
            }
        }

        DYNAMIC_STATE
        {
            pShaderAPI->BindStandardTexture( SHADER_SAMPLER0, TEXTURE_FRAME_BUFFER_FULL_TEXTURE_0 );

            const float C0 = params[FACTOR]->GetFloatValue();
            pShaderAPI->SetPixelShaderConstant( 0, &C0, 1 );

            DECLARE_DYNAMIC_VERTEX_SHADER( postproc_vs30 );
            SET_DYNAMIC_VERTEX_SHADER( postproc_vs30 );

            if( g_pHardwareConfig->SupportsShaderModel_3_0() ) {
                DECLARE_DYNAMIC_PIXEL_SHADER( postproc_desaturate_ps30 );
                SET_DYNAMIC_PIXEL_SHADER( postproc_desaturate_ps30 );
            }
        }

        Draw();
    }

END_SHADER
