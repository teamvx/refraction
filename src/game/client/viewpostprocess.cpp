//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//===========================================================================

#include "cbase.h"
#include "materialsystem/imaterialsystem.h"
#include "materialsystem/itexture.h"
#include "materialsystem/imaterialvar.h"
#include "materialsystem/imaterialsystemhardwareconfig.h"
#include "materialsystem/materialsystem_config.h"
#include "tier1/callqueue.h"
#include "colorcorrectionmgr.h"
#include "clienteffectprecachesystem.h" // refraction stores precache shit right here
#include "view_scene.h"
#include "c_world.h"
#include "bitmap/tgawriter.h"
#include "filesystem.h"
#include "tier0/vprof.h"
#include "proxyentity.h"

enum PP_Condition {
    PPC_ALWAYS,
    PPC_IF_CVAR,
    PPC_IF_NOT_CVAR,
};

struct PP_Pass_t {
    PP_Condition cond;
    const char *mat_name;
    ConVar const *cvar;
    CMaterialReference mat;
};
#define PPP_PROCESS( mat_name )                         { PPC_ALWAYS, (mat_name), NULL }
#define PPP_PROCESS_IF_CVAR( mat_name, cvar_ptr )       { PPC_IF_CVAR, (mat_name), (cvar_ptr) }
#define PPP_PROCESS_IF_NOT_CVAR( mat_name, cvar_ptr )   { PPC_IF_NOT_CVAR, (mat_name), (cvar_ptr) }

// mapmaker controlled autoexposure
bool    g_bUseCustomAutoExposureMin = false;
bool    g_bUseCustomAutoExposureMax = false;
float   g_flCustomAutoExposureMin = 0;
float   g_flCustomAutoExposureMax = 0;
bool    g_bFlashlightIsOn = false;

// hdr parameters
ConVar          mat_hdr_level                           ( "mat_hdr_level",                              "2",    FCVAR_ARCHIVE   );
static ConVar   debug_postproc                          ( "mat_debug_postprocessing_effects",           "0"                     );  // 0 = off, 1 = show post-processing passes in quadrants of the screen, 2 = only apply post-processing to the centre of the screen
static ConVar   split_postproc                          ( "mat_debug_process_halfscreen",               "0",    FCVAR_CHEAT     );
static ConVar   mat_postprocessing_combine              ( "mat_postprocessing_combine",                 "1"                     );  // Combine bloom, software anti-aliasing and color correction into one post-processing pass
static ConVar   mat_dynamic_tonemapping                 ( "mat_dynamic_tonemapping",                    "1",    FCVAR_CHEAT     );
static ConVar   mat_show_ab_hdr                         ( "mat_show_ab_hdr",                            "0"                     );
static ConVar   mat_tonemapping_occlusion_use_stencil   ( "mat_tonemapping_occlusion_use_stencil",      "0"                     );
ConVar          mat_debug_autoexposure                  ( "mat_debug_autoexposure",                     "0",    FCVAR_CHEAT     );
static ConVar   mat_autoexposure_max                    ( "mat_autoexposure_max",                       "2"                     );
static ConVar   mat_autoexposure_min                    ( "mat_autoexposure_min",                       "0.5"                   );
static ConVar   mat_show_histogram                      ( "mat_show_histogram",                         "0"                     );
ConVar          mat_hdr_tonemapscale                    ( "mat_hdr_tonemapscale",                       "1.0",  FCVAR_CHEAT     );
ConVar          mat_hdr_uncapexposure                   ( "mat_hdr_uncapexposure",                      "0",    FCVAR_CHEAT     );
ConVar          mat_colorcorrection                     ( "mat_colorcorrection",                        "0"                     );
ConVar          mat_accelerate_adjust_exposure_down     ( "mat_accelerate_adjust_exposure_down",        "3.0",  FCVAR_CHEAT     );
ConVar          mat_hdr_manual_tonemap_rate             ( "mat_hdr_manual_tonemap_rate",                "1.0"                   );
ConVar          mat_exposure_center_region_x            ( "mat_exposure_center_region_x",               "0.9",  FCVAR_CHEAT     );
ConVar          mat_exposure_center_region_y            ( "mat_exposure_center_region_y",               "0.85", FCVAR_CHEAT     );
ConVar          mat_exposure_center_region_x_flashlight ( "mat_exposure_center_region_x_flashlight",    "0.9",  FCVAR_CHEAT     );
ConVar          mat_exposure_center_region_y_flashlight ( "mat_exposure_center_region_y_flashlight",    "0.85", FCVAR_CHEAT     );
ConVar          mat_tonemap_algorithm                   ( "mat_tonemap_algorithm",                      "1"                     );  // 0 = Original Algorithm 1 = New Algorithm
ConVar          mat_tonemap_percent_target              ( "mat_tonemap_percent_target",                 "60.0", FCVAR_CHEAT     );
ConVar          mat_tonemap_percent_bright_pixels       ( "mat_tonemap_percent_bright_pixels",          "2.0",  FCVAR_CHEAT     );
ConVar          mat_tonemap_min_avglum                  ( "mat_tonemap_min_avglum",                     "3.0",  FCVAR_CHEAT     );
ConVar          mat_fullbright                          ( "mat_fullbright",                             "0",    FCVAR_CHEAT     );
extern ConVar   localplayer_visionflags;

// Refraction postproc cvars
static ConVar mat_post_chromatic_aberration( "mat_post_chromatic_aberration", "1", FCVAR_ARCHIVE, "Chromatic aberration post-effect" );
static ConVar mat_post_cubic_distortion( "mat_post_cubic_distortion", "1", FCVAR_ARCHIVE, "Cubic distortion post-effect" );

// Post processing materials precache
CLIENTEFFECT_REGISTER_BEGIN( PrecachePostProcessingEffects )
    CLIENTEFFECT_MATERIAL( "dev/bloomadd" )
    CLIENTEFFECT_MATERIAL( "dev/no_pixel_write" )
    CLIENTEFFECT_MATERIAL( "dev/lumcompare" )
    CLIENTEFFECT_MATERIAL( "dev/copyfullframefb_vanilla" )
    CLIENTEFFECT_MATERIAL( "dev/copyfullframefb" )
    CLIENTEFFECT_MATERIAL( "dev/engine_post" )
    CLIENTEFFECT_MATERIAL( "dev/motion_blur" )

    // TODO: Precache your post-processing effects here.
    CLIENTEFFECT_MATERIAL( "shaders/post_chromatic_aberration" )
    CLIENTEFFECT_MATERIAL( "shaders/post_cubic_distortion" )
CLIENTEFFECT_REGISTER_END_CONDITIONAL( engine->GetDXSupportLevel() >= 90 )

// TODO: Place your post-processing effects here.
// To make them controllable by cvar, use PPP_PROCESS_IF[_NOT]_CVAR()
// To make them permanently enabled, use PPP_PROCESS()
PP_Pass_t PP_Final[] = {
    PPP_PROCESS_IF_CVAR( "shaders/post_chromatic_aberration", &mat_post_chromatic_aberration ),
    PPP_PROCESS_IF_CVAR( "shaders/post_cubic_distortion", &mat_post_cubic_distortion ),
};

void DrawPostEffects( PP_Pass_t *pl, int count, int x, int y, int w, int h ) _NOEXCEPT
{
    // Loop through the pl
    for( int i = 0; i < count; i++ ) {
        bool cond = true; // PPC_ALWAYS
        switch( pl[i].cond ) {
            case PPC_IF_CVAR:       cond = pl[i].cvar->GetBool();   break;
            case PPC_IF_NOT_CVAR:   cond = !pl[i].cvar->GetBool();  break;
        }

        if( cond ) {
            // Setup the material to draw
            IMaterial *src_mat = pl[i].mat;
            if( !src_mat ) {
                src_mat = materials->FindMaterial( pl[i].mat_name, TEXTURE_GROUP_OTHER, true );
                if( src_mat )
                    pl[i].mat.Init( src_mat );
            }

            DrawScreenEffectMaterial( src_mat, x, y, w, h );
        }
    }
}

static void SetRenderTargetAndViewPort( ITexture *rt )
{
    tmZone( TELEMETRY_LEVEL0, TMZF_NONE, "%s", __FUNCTION__ );

    CMatRenderContextPtr pRenderContext( materials );
    pRenderContext->SetRenderTarget( rt );
    pRenderContext->Viewport( 0, 0, rt->GetActualWidth(), rt->GetActualHeight() );
}

#define FILTER_KERNEL_SLOP 20

enum Histogram_entry_state_t {
    HESTATE_INITIAL = 0,
    HESTATE_FIRST_QUERY_IN_FLIGHT,
    HESTATE_QUERY_IN_FLIGHT,
    HESTATE_QUERY_DONE,
};

#define N_LUMINANCE_RANGES 31
#define N_LUMINANCE_RANGES_NEW 17
#define MAX_QUERIES_PER_FRAME 1

class CHistogram_entry_t {
public:
    Histogram_entry_state_t         m_state;
    OcclusionQueryObjectHandle_t    m_occ_handle;                       // the occlusion query handle
    int                             m_frame_queued;                     // when this query was last queued
    int                             m_npixels;                          // # of pixels this histogram represents
    int                             m_npixels_in_range;
    float                           m_min_lum, m_max_lum;               // the luminance range this entry was queried with
    float                           m_minx, m_miny, m_maxx, m_maxy;     // range is 0..1 in fractions of the screen

    bool ContainsValidData( void )
    {
        return ( m_state == HESTATE_QUERY_DONE ) || ( m_state == HESTATE_QUERY_IN_FLIGHT );
    }

    void IssueQuery( int frm_num );
};

void CHistogram_entry_t::IssueQuery( int frm_num )
{
    CMatRenderContextPtr pRenderContext( materials );
    if( !m_occ_handle ) {
        m_occ_handle = pRenderContext->CreateOcclusionQueryObject();
    }

    int xl, yl, dest_width, dest_height;
    pRenderContext->GetViewport( xl, yl, dest_width, dest_height );

    // Find min and max gamma-space text range
    float flTestRangeMin = m_min_lum;
    float flTestRangeMax = ( m_max_lum == 1.0f ) ? 10000.0f : m_max_lum; // Count all pixels >1.0 as 1.0

    // First, set stencil bits where the colors match
    IMaterial *test_mat = materials->FindMaterial( "dev/lumcompare", TEXTURE_GROUP_OTHER, true );
    IMaterialVar *pMinVar = test_mat->FindVar( "$C0_X", NULL ); pMinVar->SetFloatValue( flTestRangeMin );
    IMaterialVar *pMaxVar = test_mat->FindVar( "$C0_Y", NULL ); pMaxVar->SetFloatValue( flTestRangeMax );

    int scrx_min = FLerp( xl, ( xl + dest_width - 1 ), 0, 1, m_minx );
    int scrx_max = FLerp( xl, ( xl + dest_width - 1 ), 0, 1, m_maxx );
    int scry_min = FLerp( yl, ( yl + dest_height - 1 ), 0, 1, m_miny );
    int scry_max = FLerp( yl, ( yl + dest_height - 1 ), 0, 1, m_maxy );

    float exposure_width_scale, exposure_height_scale;

    // now, shrink region of interest if the flashlight is on
    if( g_bFlashlightIsOn ) {
        exposure_width_scale = ( 0.5f * ( 1.0f - mat_exposure_center_region_x_flashlight.GetFloat() ) );
        exposure_height_scale = ( 0.5f * ( 1.0f - mat_exposure_center_region_y_flashlight.GetFloat() ) );
    }
    else {
        exposure_width_scale = ( 0.5f * ( 1.0f - mat_exposure_center_region_x.GetFloat() ) );
        exposure_height_scale = ( 0.5f * ( 1.0f - mat_exposure_center_region_y.GetFloat() ) );
    }
    int skip_edgex = ( 1 + scrx_max - scrx_min ) * exposure_width_scale;
    int skip_edgey = ( 1 + scry_max - scry_min ) * exposure_height_scale;

    // now, do luminance compare
    float tscale = 1.0;
    if( g_pMaterialSystemHardwareConfig->GetHDRType() == HDR_TYPE_FLOAT ) {
        tscale = pRenderContext->GetToneMappingScaleLinear().x;
    }

    IMaterialVar *use_t_scale = test_mat->FindVar( "$C0_Z", NULL ); use_t_scale->SetFloatValue( tscale );
    m_npixels = ( 1 + scrx_max - scrx_min ) * ( 1 + scry_max - scry_min );

    if( mat_tonemapping_occlusion_use_stencil.GetInt() ) {
        pRenderContext->SetStencilWriteMask( 1 );

        // AV - We don't need to clear stencil here because it's already been cleared at the beginning of the frame
        //pRenderContext->ClearStencilBufferRectangle( scrx_min, scry_min, scrx_max, scry_max, 0 );

        pRenderContext->SetStencilEnable( true );
        pRenderContext->SetStencilPassOperation( STENCILOPERATION_REPLACE );
        pRenderContext->SetStencilCompareFunction( STENCILCOMPARISONFUNCTION_ALWAYS );
        pRenderContext->SetStencilFailOperation( STENCILOPERATION_KEEP );
        pRenderContext->SetStencilZFailOperation( STENCILOPERATION_KEEP );
        pRenderContext->SetStencilReferenceValue( 1 );
    }
    else {
        pRenderContext->BeginOcclusionQueryDrawing( m_occ_handle );
    }

    scrx_min += skip_edgex;
    scry_min += skip_edgey;
    scrx_max -= skip_edgex;
    scry_max -= skip_edgey;
    pRenderContext->DrawScreenSpaceRectangle( test_mat, scrx_min, scry_min, 1 + scrx_max - scrx_min, 1 + scry_max - scry_min, scrx_min, scry_min, scrx_max, scry_max, dest_width, dest_height );

    if( mat_tonemapping_occlusion_use_stencil.GetInt() ) {
        // now, start counting how many pixels had their stencil bit set via an occlusion query
        pRenderContext->BeginOcclusionQueryDrawing( m_occ_handle );

        // now, issue an occlusion query using stencil as the mask
        pRenderContext->SetStencilEnable( true );
        pRenderContext->SetStencilTestMask( 1 );
        pRenderContext->SetStencilPassOperation( STENCILOPERATION_KEEP );
        pRenderContext->SetStencilCompareFunction( STENCILCOMPARISONFUNCTION_EQUAL );
        pRenderContext->SetStencilFailOperation( STENCILOPERATION_KEEP );
        pRenderContext->SetStencilZFailOperation( STENCILOPERATION_KEEP );
        pRenderContext->SetStencilReferenceValue( 1 );

        IMaterial *stest_mat = materials->FindMaterial( "dev/no_pixel_write", TEXTURE_GROUP_OTHER, true );
        pRenderContext->DrawScreenSpaceRectangle( stest_mat, scrx_min, scry_min, 1 + scrx_max - scrx_min, 1 + scry_max - scry_min, scrx_min, scry_min, scrx_max, scry_max, dest_width, dest_height );
        pRenderContext->SetStencilEnable( false );
    }

    pRenderContext->EndOcclusionQueryDrawing( m_occ_handle );
    if( m_state == HESTATE_INITIAL ) {
        m_state = HESTATE_FIRST_QUERY_IN_FLIGHT;
    }
    else {
        m_state = HESTATE_QUERY_IN_FLIGHT;
    }

    m_frame_queued = frm_num;
}

#define HISTOGRAM_BAR_SIZE 200

class CLuminanceHistogramSystem {
    CHistogram_entry_t CurHistogram[N_LUMINANCE_RANGES];
    int cur_query_frame;

public:
    float FindLocationOfPercentBrightPixels( float flPercentBrightPixels, float flPercentTarget );
    float GetTargetTonemapScalar( bool bGetIdealTargetForDebugMode );
    void Update( void );
    void DisplayHistogram( void );
    void UpdateLuminanceRanges( void );

    CLuminanceHistogramSystem( void )
    {
        UpdateLuminanceRanges();
    }
};

void CLuminanceHistogramSystem::Update( void )
{
    UpdateLuminanceRanges();

    // find which histogram entries should have something done this frame
    int n_queries_issued_this_frame = 0;
    cur_query_frame++;

    int nNumRanges = N_LUMINANCE_RANGES;
    if( mat_tonemap_algorithm.GetInt() == 1 ) {
        nNumRanges = N_LUMINANCE_RANGES_NEW;
    }

    for( int i = 0; i < nNumRanges; i++ ) {
        switch( CurHistogram[i].m_state ) {
            case HESTATE_INITIAL: {
                if( n_queries_issued_this_frame < MAX_QUERIES_PER_FRAME ) {
                    CurHistogram[i].IssueQuery( cur_query_frame );
                    n_queries_issued_this_frame++;
                }
            } break;

            case HESTATE_FIRST_QUERY_IN_FLIGHT:
            case HESTATE_QUERY_IN_FLIGHT: {
                if( cur_query_frame > CurHistogram[i].m_frame_queued + 2 ) {
                    CMatRenderContextPtr pRenderContext( materials );
                    int np = pRenderContext->OcclusionQuery_GetNumPixelsRendered( CurHistogram[i].m_occ_handle );
                    if( np != -1 ) {    // -1=query not finished. wait until next time
                        CurHistogram[i].m_npixels_in_range = np;
                        CurHistogram[i].m_state = HESTATE_QUERY_DONE;
                    }
                }
            } break;
        }
    }

    // now, issue queries for the oldest finished queries we have
    while( n_queries_issued_this_frame < MAX_QUERIES_PER_FRAME ) {
        int nNumRanges = N_LUMINANCE_RANGES;
        int oldest_so_far = -1;

        if( mat_tonemap_algorithm.GetInt() == 1 ) {
            nNumRanges = N_LUMINANCE_RANGES_NEW;
        }

        for( int i = 0; i < nNumRanges; i++ ) {
            if( ( CurHistogram[i].m_state == HESTATE_QUERY_DONE ) && ( ( oldest_so_far == -1 ) || ( CurHistogram[i].m_frame_queued < CurHistogram[oldest_so_far].m_frame_queued ) ) )
                oldest_so_far = i;
        }

        if( oldest_so_far == -1 ) { // nothing to do
            break;
        }

        CurHistogram[oldest_so_far].IssueQuery( cur_query_frame );
        n_queries_issued_this_frame++;
    }
}

float CLuminanceHistogramSystem::FindLocationOfPercentBrightPixels( float flPercentBrightPixels, float flPercentTargetToSnapToIfInSameBin = -1.0f )
{
    if( mat_tonemap_algorithm.GetInt() == 1 ) { // New algorithm
        int nTotalValidPixels = 0;
        for( int i = 0; i < N_LUMINANCE_RANGES_NEW - 1; i++ ) {
            if( CurHistogram[i].ContainsValidData() ) {
                nTotalValidPixels += CurHistogram[i].m_npixels_in_range;
            }
        }

        if( nTotalValidPixels == 0 ) {
            return -1.0f;
        }

        // Find where percent range border is
        float flTotalPercentRangeTested = 0.0f;
        float flTotalPercentPixelsTested = 0.0f;
        for( int i = N_LUMINANCE_RANGES_NEW - 2; i >= 0; i-- ) {    // Start at the bright end
            if( !CurHistogram[i].ContainsValidData() ) {
                return -1.0f;
            }

            float flPixelPercentNeeded = ( flPercentBrightPixels / 100.0f ) - flTotalPercentPixelsTested;
            float flThisBinPercentOfTotalPixels = float( CurHistogram[i].m_npixels_in_range ) / float( nTotalValidPixels );
            float flThisBinLuminanceRange = CurHistogram[i].m_max_lum - CurHistogram[i].m_min_lum;
            if( flThisBinPercentOfTotalPixels >= flPixelPercentNeeded ) {   // We found the bin needed
                if( flPercentTargetToSnapToIfInSameBin >= 0.0f ) {
                    if( ( CurHistogram[i].m_min_lum <= ( flPercentTargetToSnapToIfInSameBin / 100.0f ) ) && ( CurHistogram[i].m_max_lum >= ( flPercentTargetToSnapToIfInSameBin / 100.0f ) ) ) {
                        // Sticky bin...We're in the same bin as the target so keep the tonemap scale where it is
                        return ( flPercentTargetToSnapToIfInSameBin / 100.0f );
                    }
                }

                float flPercentOfThesePixelsNeeded = flPixelPercentNeeded / flThisBinPercentOfTotalPixels;
                float flPercentLocationOfBorder = 1.0f - ( flTotalPercentRangeTested + ( flThisBinLuminanceRange * flPercentOfThesePixelsNeeded ) );
                flPercentLocationOfBorder = MAX( CurHistogram[i].m_min_lum, MIN( CurHistogram[i].m_max_lum, flPercentLocationOfBorder ) ); // Clamp to this bin just in case
                return flPercentLocationOfBorder;
            }

            flTotalPercentPixelsTested += flThisBinPercentOfTotalPixels;
            flTotalPercentRangeTested += flThisBinLuminanceRange;
        }

        return -1.0f;
    }
    else {
        // Don't know what to do for other algorithms yet
        return -1.0f;
    }
}

float CLuminanceHistogramSystem::GetTargetTonemapScalar( bool bGetIdealTargetForDebugMode = false )
{
    if( mat_tonemap_algorithm.GetInt() == 1 ) { // New algorithm
        float flPercentLocationOfTarget;
        if( bGetIdealTargetForDebugMode == true ) {
            flPercentLocationOfTarget = FindLocationOfPercentBrightPixels( mat_tonemap_percent_bright_pixels.GetFloat() ); // Don't pass in the second arg so the scalar doesn't snap to a bin
        }
        else {
            flPercentLocationOfTarget = FindLocationOfPercentBrightPixels( mat_tonemap_percent_bright_pixels.GetFloat(), mat_tonemap_percent_target.GetFloat() );
        }

        if( flPercentLocationOfTarget < 0.0f ) {    // This is the return error code
            flPercentLocationOfTarget = mat_tonemap_percent_target.GetFloat() / 100.0f; // Pretend we're at the target
        }

        // Make sure this is > 0.0f
        flPercentLocationOfTarget = MAX( 0.0001f, flPercentLocationOfTarget );

        // Compute target scalar
        float flTargetScalar = ( mat_tonemap_percent_target.GetFloat() / 100.0f ) / flPercentLocationOfTarget;

        // Compute secondary target scalar
        float flAverageLuminanceLocation = FindLocationOfPercentBrightPixels( 50.0f );
        if( flAverageLuminanceLocation > 0.0f ) {
            float flTargetScalar2 = ( mat_tonemap_min_avglum.GetFloat() / 100.0f ) / flAverageLuminanceLocation;

            // Only override it if it's trying to brighten the image more than the primary algorithm
            if( flTargetScalar2 > flTargetScalar ) {
                flTargetScalar = flTargetScalar2;
            }
        }

        // Apply this against last frames scalar
        CMatRenderContextPtr pRenderContext( materials );
        float flLastScale = pRenderContext->GetToneMappingScaleLinear().x;
        flTargetScalar *= flLastScale;

        flTargetScalar = MAX( 0.001f, flTargetScalar );
        return flTargetScalar;
    }
    else {  // Original tonemapping
        float average_luminance = 0.5f;
        float total = 0;
        int total_pixels = 0;
        float scale_value = 1.0;
        if( CurHistogram[N_LUMINANCE_RANGES - 1].ContainsValidData() ) {
            scale_value = CurHistogram[N_LUMINANCE_RANGES - 1].m_npixels * ( 1.0f / CurHistogram[N_LUMINANCE_RANGES - 1].m_npixels_in_range );
            if( mat_debug_autoexposure.GetInt() ) {
                engine->Con_NPrintf( 20, "Scale value = %f", scale_value );
            }
        }
        else {
            average_luminance = 0.5;
        }

        if( !IsFinite( scale_value ) ) {
            scale_value = 1.0f;
        }

        for( int i = 0; i < N_LUMINANCE_RANGES - 1; i++ ) {
            if( CurHistogram[i].ContainsValidData() ) {
                total += scale_value * CurHistogram[i].m_npixels_in_range * AVG( CurHistogram[i].m_min_lum, CurHistogram[i].m_max_lum );
                total_pixels += CurHistogram[i].m_npixels;
            }
            else {
                average_luminance = 0.5; // always return 0.5 until we've queried a whole frame
            }
        }

        if( total_pixels > 0 ) {
            average_luminance = total * ( 1.0 / total_pixels );
        }
        else {
            average_luminance = 0.5;
        }

        // Make sure this is > 0.0f
        average_luminance = MAX( 0.0001f, average_luminance );

        // Compute target scalar
        float flTargetScalar = 0.005 / average_luminance;

        return flTargetScalar;
    }
}

static void GetExposureRange( float *flAutoExposureMin, float *flAutoExposureMax )
{
    // Get min
    if( ( g_bUseCustomAutoExposureMin ) && ( g_flCustomAutoExposureMin > 0.0f ) ) {
        *flAutoExposureMin = g_flCustomAutoExposureMin;
    }
    else {
        *flAutoExposureMin = mat_autoexposure_min.GetFloat();
    }

    // Get max
    if( ( g_bUseCustomAutoExposureMax ) && ( g_flCustomAutoExposureMax > 0.0f ) ) {
        *flAutoExposureMax = g_flCustomAutoExposureMax;
    }
    else {
        *flAutoExposureMax = mat_autoexposure_max.GetFloat();
    }

    // Override
    if( mat_hdr_uncapexposure.GetInt() ) {
        *flAutoExposureMax = 20.0f;
        *flAutoExposureMin = 0.0f;
    }

    // Make sure min <= max
    if( *flAutoExposureMin > *flAutoExposureMax ) {
        *flAutoExposureMax = *flAutoExposureMin;
    }
}

void CLuminanceHistogramSystem::UpdateLuminanceRanges( void )
{
    // Only update if our mode changed
    static int s_nCurrentBucketAlgorithm = -1;
    if( s_nCurrentBucketAlgorithm == mat_tonemap_algorithm.GetInt() ) {
        return;
    }
    s_nCurrentBucketAlgorithm = mat_tonemap_algorithm.GetInt();

    int nNumRanges = N_LUMINANCE_RANGES;
    if( mat_tonemap_algorithm.GetInt() == 1 ) {
        nNumRanges = N_LUMINANCE_RANGES_NEW;
    }

    cur_query_frame = 0;
    for( int bucket = 0; bucket < nNumRanges; bucket++ ) {
        int idx = bucket;
        CHistogram_entry_t & e = CurHistogram[idx];
        e.m_state = HESTATE_INITIAL;
        e.m_minx = 0;
        e.m_maxx = 1;
        e.m_miny = 0;
        e.m_maxy = 1;
        if( bucket != nNumRanges - 1 ) {    // Last bucket is special
            if( mat_tonemap_algorithm.GetInt() == 0 ) { // Original algorithm
                // Use a logarithmic ramp for high range in the low range
                e.m_min_lum = -0.01 + exp( FLerp( log( .01 ), log( .01 + 1 ), 0, nNumRanges - 1, bucket ) );
                e.m_max_lum = -0.01 + exp( FLerp( log( .01 ), log( .01 + 1 ), 0, nNumRanges - 1, bucket + 1 ) );
            }
            else {
                // Use even distribution
                e.m_min_lum = float( bucket ) / float( nNumRanges - 1 );
                e.m_max_lum = float( bucket + 1 ) / float( nNumRanges - 1 );

                // Use a distribution with slightly more bins in the low range
                e.m_min_lum = e.m_min_lum > 0.0f ? powf( e.m_min_lum, 1.5f ) : e.m_min_lum;
                e.m_max_lum = e.m_max_lum > 0.0f ? powf( e.m_max_lum, 1.5f ) : e.m_max_lum;
            }
        }
        else {
            // The last bucket is used as a test to determine the return range for occlusion
            // queries to use as a scale factor. some boards (nvidia) have their occlusion
            // query return values larger when using AA.
            e.m_min_lum = 0;
            e.m_max_lum = 100000.0;
        }
    }
}

void CLuminanceHistogramSystem::DisplayHistogram( void )
{
    bool bDrawTextThisFrame = true;
    CMatRenderContextPtr pRenderContext( materials );
    pRenderContext->PushRenderTargetAndViewport();

    int nNumRanges = N_LUMINANCE_RANGES - 1;
    if( mat_tonemap_algorithm.GetInt() == 1 ) {
        nNumRanges = N_LUMINANCE_RANGES_NEW - 1;
    }

    int nMaxValidPixels = 0;
    int nTotalValidPixels = 0;
    int nTotalGraphPixelsWide = 0;
    for( int l = 0; l < nNumRanges; l++ ) {
        CHistogram_entry_t &e = CurHistogram[l];
        if( e.ContainsValidData() ) {
            nTotalValidPixels += e.m_npixels_in_range;
            if( e.m_npixels_in_range > nMaxValidPixels ) {
                nMaxValidPixels = e.m_npixels_in_range;
            }
        }

        int width = MAX( 1, 500 * ( e.m_max_lum - e.m_min_lum ) );
        nTotalGraphPixelsWide += width + 2;
    }

    int xl, yl, dest_width, dest_height;
    pRenderContext->GetViewport( xl, yl, dest_width, dest_height );

    if( bDrawTextThisFrame == true ) {
        engine->Con_NPrintf( 17, "(All values in linear space)" );
        engine->Con_NPrintf( 21, "AvgLum @ %4.2f%%  mat_tonemap_min_avglum = %4.2f%%  Using %d pixels of %d pixels on screen (%3d%%)", MAX( 0.0f, FindLocationOfPercentBrightPixels( 50.0f ) ) * 100.0f, mat_tonemap_min_avglum.GetFloat(), nTotalValidPixels, ( dest_width * dest_height ), int( float( nTotalValidPixels ) * 100.0f / float( dest_width * dest_height ) ) );
        engine->Con_NPrintf( 23, "mat_hdr_manual_tonemap_rate = %4.2f  mat_accelerate_adjust_exposure_down = %4.2f", mat_hdr_manual_tonemap_rate.GetFloat(), mat_accelerate_adjust_exposure_down.GetFloat() );
    }

    // New algorithm only
    if( mat_tonemap_algorithm.GetInt() == 1 ) {
        float vTotalPixelsAndHigher[N_LUMINANCE_RANGES];
        for( int i = 0; i < nNumRanges; i++ ) {
            vTotalPixelsAndHigher[i] = CurHistogram[nNumRanges - 1 - i].m_npixels_in_range;
            if( i > 0 ) {
                vTotalPixelsAndHigher[i] += vTotalPixelsAndHigher[i - 1];
            }
        }
    }

    int xpStart = dest_width - nTotalGraphPixelsWide - 10;
    int xp = xpStart;
    for( int l = 0; l < nNumRanges; l++ ) {
        int np = 0;
        CHistogram_entry_t &e = CurHistogram[l];
        if( e.ContainsValidData() ) {
            np += e.m_npixels_in_range;
        }
        int width = MAX( 1, 500 * ( e.m_max_lum - e.m_min_lum ) );

        if( np ) {
            int height = MAX( 1, MIN( HISTOGRAM_BAR_SIZE, ( (float)np / (float)nMaxValidPixels ) * HISTOGRAM_BAR_SIZE ) );
            pRenderContext->ClearColor3ub( 255, 0, 0 );
            pRenderContext->Viewport( xp, 4 + HISTOGRAM_BAR_SIZE - height, width, height );
            pRenderContext->ClearBuffers( true, true );
        }
        else {
            int height = 1;
            pRenderContext->ClearColor3ub( 0, 0, 255 );
            pRenderContext->Viewport( xp, 4 + HISTOGRAM_BAR_SIZE - height, width, height );
            pRenderContext->ClearBuffers( true, true );
        }
        xp += width + 2;
    }

    if( mat_tonemap_algorithm.GetInt() == 1 ) { // New algorithm only
        float flYellowTargetPixelStart = ( xpStart + ( float( nTotalGraphPixelsWide ) * mat_tonemap_percent_target.GetFloat() / 100.0f ) );
        float flYellowAveragePixelStart = ( xpStart + ( float( nTotalGraphPixelsWide ) * mat_tonemap_min_avglum.GetFloat() / 100.0f ) );

        float flTargetPixelStart = ( xpStart + ( float( nTotalGraphPixelsWide ) * FindLocationOfPercentBrightPixels( mat_tonemap_percent_bright_pixels.GetFloat(), mat_tonemap_percent_target.GetFloat() ) ) );
        float flAveragePixelStart = ( xpStart + ( float( nTotalGraphPixelsWide ) * FindLocationOfPercentBrightPixels( 50.0f ) ) );

        // Draw target yellow border bar
        int height = HISTOGRAM_BAR_SIZE;

        // Green is current percent target location
        pRenderContext->Viewport( flYellowTargetPixelStart, 4 + HISTOGRAM_BAR_SIZE - height, 4, height );
        pRenderContext->ClearColor3ub( 200, 200, 0 );
        pRenderContext->ClearBuffers( true, true );

        pRenderContext->Viewport( flTargetPixelStart, 4 + HISTOGRAM_BAR_SIZE - height, 4, height );
        pRenderContext->ClearColor3ub( 0, 255, 0 );
        pRenderContext->ClearBuffers( true, true );

        // Blue is average luminance location
        pRenderContext->Viewport( flYellowAveragePixelStart, 4 + HISTOGRAM_BAR_SIZE - height, 4, height );
        pRenderContext->ClearColor3ub( 200, 200, 0 );
        pRenderContext->ClearBuffers( true, true );

        pRenderContext->Viewport( flAveragePixelStart, 4 + HISTOGRAM_BAR_SIZE - height, 4, height );
        pRenderContext->ClearColor3ub( 0, 200, 200 );
        pRenderContext->ClearBuffers( true, true );
    }

    // Show actual tonemap value
    float flAutoExposureMin;
    float flAutoExposureMax;
    GetExposureRange( &flAutoExposureMin, &flAutoExposureMax );

    float flBarWidth = 600.0f;
    float flBarStart = dest_width - flBarWidth - 10.0f;

    pRenderContext->Viewport( flBarStart, 4 + HISTOGRAM_BAR_SIZE - 4 + 75, flBarWidth, 4 );
    pRenderContext->ClearColor3ub( 200, 200, 200 );
    pRenderContext->ClearBuffers( true, true );

    pRenderContext->Viewport( flBarStart, 4 + HISTOGRAM_BAR_SIZE - 4 + 75 + 1, flBarWidth, 2 );
    pRenderContext->ClearColor3ub( 0, 0, 0 );
    pRenderContext->ClearBuffers( true, true );

    pRenderContext->Viewport( flBarStart + ( flBarWidth * ( ( pRenderContext->GetToneMappingScaleLinear().x - flAutoExposureMin ) / ( flAutoExposureMax - flAutoExposureMin ) ) ), 4 + HISTOGRAM_BAR_SIZE - 4 + 75 - 6, 4, 16 );
    pRenderContext->ClearColor3ub( 255, 0, 0 );
    pRenderContext->ClearBuffers( true, true );

    if( bDrawTextThisFrame == true ) {
        engine->Con_NPrintf( 26, "%.2f %.2f %.2f", flAutoExposureMin, ( flAutoExposureMax + flAutoExposureMin ) / 2.0f, flAutoExposureMax );
    }

    // Last bar doesn't clear properly so draw an extra pixel
    pRenderContext->Viewport( 0, 0, 1, 1 );
    pRenderContext->ClearColor3ub( 0, 0, 0 );
    pRenderContext->ClearBuffers( true, true );

    pRenderContext->PopRenderTargetAndViewport();
}


static CLuminanceHistogramSystem g_HDR_HistogramSystem;

static float s_MovingAverageToneMapScale[10] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
static int s_nInAverage = 0;

void ResetToneMapping( float value )
{
    CMatRenderContextPtr pRenderContext( materials );
    s_nInAverage = 0;
    pRenderContext->ResetToneMappingScale( value );
}

static ConVar mat_force_tonemap_scale( "mat_force_tonemap_scale", "0.0", FCVAR_CHEAT );

static void SetToneMapScale( IMatRenderContext *pRenderContext, float newvalue, float minvalue, float maxvalue )
{
    Assert( IsFinite( newvalue ) );
    if( !IsFinite( newvalue ) ) {
        return;
    }

    float flForcedTonemapScale = mat_force_tonemap_scale.GetFloat();

    if( mat_fullbright.GetInt() == 1 ) {
        flForcedTonemapScale = 1.0f;
    }

    if( flForcedTonemapScale > 0.0f ) {
        mat_hdr_tonemapscale.SetValue( flForcedTonemapScale );
        pRenderContext->ResetToneMappingScale( flForcedTonemapScale );
        return;
    }

    mat_hdr_tonemapscale.SetValue( newvalue );
    pRenderContext->SetGoalToneMappingScale( newvalue );

    if( s_nInAverage < ARRAYSIZE( s_MovingAverageToneMapScale ) ) {
        s_MovingAverageToneMapScale[s_nInAverage++] = newvalue;
    }
    else {
        // scroll, losing oldest
        for( int i = 0; i < ARRAYSIZE( s_MovingAverageToneMapScale ) - 1; i++ ) {
            s_MovingAverageToneMapScale[i] = s_MovingAverageToneMapScale[i + 1];
        }

        s_MovingAverageToneMapScale[ARRAYSIZE( s_MovingAverageToneMapScale ) - 1] = newvalue;
    }

    // now, use the average of the last tonemap calculations as our goal scale
    if( s_nInAverage == ARRAYSIZE( s_MovingAverageToneMapScale ) ) {    // got full buffer yet?
        float avg = 0.0;
        float sumweights = 0;
        int sample_pt = ARRAYSIZE( s_MovingAverageToneMapScale ) / 2;
        for( int i = 0; i < ARRAYSIZE( s_MovingAverageToneMapScale ); i++ ) {
            float weight = abs( i - sample_pt ) * ( 1.0 / ( ARRAYSIZE( s_MovingAverageToneMapScale ) / 2 ) );
            sumweights += weight;
            avg += weight * s_MovingAverageToneMapScale[i];
        }
        avg *= ( 1.0 / sumweights );
        avg = MIN( maxvalue, MAX( minvalue, avg ) );
        pRenderContext->SetGoalToneMappingScale( avg );
        mat_hdr_tonemapscale.SetValue( avg );
    }
}

class CEnginePostMaterialProxy : public CEntityMaterialProxy {
public:
    CEnginePostMaterialProxy();
    virtual ~CEnginePostMaterialProxy();
    virtual bool Init( IMaterial *pMaterial, KeyValues *pKeyValues );
    virtual void OnBind( C_BaseEntity *pEntity );
    virtual IMaterial *GetMaterial();

private:
    IMaterialVar *m_pMaterialParam_AAValues;
    IMaterialVar *m_pMaterialParam_AAValues2;
    IMaterialVar *m_pMaterialParam_ColCorrectEnable;
    IMaterialVar *m_pMaterialParam_ColCorrectNumLookups;
    IMaterialVar *m_pMaterialParam_ColCorrectDefaultWeight;
    IMaterialVar *m_pMaterialParam_ColCorrectLookupWeights;

public:
    static IMaterial * SetupEnginePostMaterial( const Vector4D & fullViewportFBUVs, const Vector2D &destTexSize, bool bPerformColCorrect );
};

CEnginePostMaterialProxy::CEnginePostMaterialProxy()
{
    m_pMaterialParam_AAValues = NULL;
    m_pMaterialParam_AAValues2 = NULL;
    m_pMaterialParam_ColCorrectEnable = NULL;
    m_pMaterialParam_ColCorrectNumLookups = NULL;
    m_pMaterialParam_ColCorrectDefaultWeight = NULL;
    m_pMaterialParam_ColCorrectLookupWeights = NULL;
}

CEnginePostMaterialProxy::~CEnginePostMaterialProxy()
{
    // Do nothing
}

bool CEnginePostMaterialProxy::Init( IMaterial *pMaterial, KeyValues *pKeyValues )
{
    bool bFoundVar = false;

    m_pMaterialParam_AAValues = pMaterial->FindVar( "$AAInternal1", &bFoundVar, false );
    m_pMaterialParam_AAValues2 = pMaterial->FindVar( "$AAInternal3", &bFoundVar, false );
    m_pMaterialParam_ColCorrectEnable = pMaterial->FindVar( "$colCorrectEnable", &bFoundVar, false );
    m_pMaterialParam_ColCorrectNumLookups = pMaterial->FindVar( "$colCorrect_NumLookups", &bFoundVar, false );
    m_pMaterialParam_ColCorrectDefaultWeight = pMaterial->FindVar( "$colCorrect_DefaultWeight", &bFoundVar, false );
    m_pMaterialParam_ColCorrectLookupWeights = pMaterial->FindVar( "$colCorrect_LookupWeights", &bFoundVar, false );

    return true;
}

void CEnginePostMaterialProxy::OnBind( C_BaseEntity *pEnt )
{
    // sorse
}

IMaterial * CEnginePostMaterialProxy::GetMaterial()
{
    if( !m_pMaterialParam_AAValues ) {
        return NULL;
    }

    return m_pMaterialParam_AAValues->GetOwningMaterial();
}

IMaterial * CEnginePostMaterialProxy::SetupEnginePostMaterial( const Vector4D & fullViewportFBUVs, const Vector2D & destTexSize, bool bPerformColCorrect )
{
    // Shouldn't get here if none of the effects are enabled
    Assert( bPerformColCorrect );

    if( bPerformColCorrect ) {
        return materials->FindMaterial( "dev/engine_post", TEXTURE_GROUP_OTHER, true );
    }
    else {
        // Just use the old bloomadd material (which uses additive blending, unlike engine_post)
        // NOTE: this path is what gets used for DX8 (which cannot enable AA or col-correction)
        return materials->FindMaterial( "dev/bloomadd", TEXTURE_GROUP_OTHER, true );
    }
}

EXPOSE_INTERFACE( CEnginePostMaterialProxy, IMaterialProxy, "engine_post" IMATERIAL_PROXY_INTERFACE_VERSION );

// Control for dumping render targets to files for debugging
static ConVar mat_dump_rts( "mat_dump_rts", "0" );
static int s_nRTIndex = 0;
bool g_bDumpRenderTargets = false;

// Dump a rendertarget to a TGA.  Useful for looking at intermediate render target results.
void DumpTGAofRenderTarget( const int width, const int height, const char *pFilename )
{
    // Ensure that mat_queue_mode is zero
    static ConVarRef mat_queue_mode( "mat_queue_mode" );
    if( mat_queue_mode.GetInt() != 0 ) {
        DevMsg( "Error: mat_queue_mode must be 0 to dump debug rendertargets\n" );
        mat_dump_rts.SetValue( 0 );     // Just report this error once and stop trying to dump images
        return;
    }

    CMatRenderContextPtr pRenderContext( materials );

    // Get the data from the render target and save to disk bitmap bits
    unsigned char *pImage = (unsigned char *)malloc( width * 4 * height );

    // Get Bits from the material system
    pRenderContext->ReadPixels( 0, 0, width, height, pImage, IMAGE_FORMAT_RGBA8888 );

    // allocate a buffer to write the tga into
    int iMaxTGASize = 1024 + ( width * height * 4 );
    void *pTGA = malloc( iMaxTGASize );
    CUtlBuffer buffer( pTGA, iMaxTGASize );

    if( !TGAWriter::WriteToBuffer( pImage, buffer, width, height, IMAGE_FORMAT_RGBA8888, IMAGE_FORMAT_RGBA8888 ) ) {
        Error( "Couldn't write bitmap data snapshot.\n" );
    }

    free( pImage );

    // async write to disk (this will take ownership of the memory)
    char szPathedFileName[_MAX_PATH];
    Q_snprintf( szPathedFileName, sizeof( szPathedFileName ), "//MOD/%d_%s_%s.tga", s_nRTIndex++, pFilename, IsOSX() ? "OSX" : "PC" );

    FileHandle_t fileTGA = filesystem->Open( szPathedFileName, "wb" );
    filesystem->Write( buffer.Base(), buffer.TellPut(), fileTGA );
    filesystem->Close( fileTGA );

    free( pTGA );
}

static bool s_bScreenEffectTextureIsUpdated = false;
static void DoTonemapping( IMatRenderContext *pRenderContext, int nX, int nY, int nWidth, int nHeight, float flAutoExposureMin, float flAutoExposureMax )
{
    // Update HDR histogram before bloom
    if( mat_dynamic_tonemapping.GetInt() || mat_show_histogram.GetInt() ) {
        tmZone( TELEMETRY_LEVEL0, TMZF_NONE, "%s", __FUNCTION__ );

        if( s_bScreenEffectTextureIsUpdated == false ) {
            // FIXME: nX/nY/nWidth/nHeight are used here, but the equivalent parameters are ignored in Generate8BitBloomTexture
            UpdateScreenEffectTexture( 0, nX, nY, nWidth, nHeight, true );
            s_bScreenEffectTextureIsUpdated = true;
        }

        g_HDR_HistogramSystem.Update();
        if( mat_dynamic_tonemapping.GetInt() || mat_show_histogram.GetInt() ) {
            float flTargetScalar = g_HDR_HistogramSystem.GetTargetTonemapScalar();
            float flTargetScalarClamped = MAX( flAutoExposureMin, MIN( flAutoExposureMax, flTargetScalar ) );
            flTargetScalarClamped = MAX( 0.001f, flTargetScalarClamped ); // Don't let this go to 0!
            if( mat_dynamic_tonemapping.GetInt() ) {
                SetToneMapScale( pRenderContext, flTargetScalarClamped, flAutoExposureMin, flAutoExposureMax );
            }

            if( mat_debug_autoexposure.GetInt() || mat_show_histogram.GetInt() ) {
                bool bDrawTextThisFrame = true;

                if( bDrawTextThisFrame == true ) {
                    if( mat_tonemap_algorithm.GetInt() == 0 ) {
                        engine->Con_NPrintf( 19, "(Original algorithm) Target Scalar = %4.2f  Min/Max( %4.2f, %4.2f )  Final Scalar: %4.2f  Actual: %4.2f", flTargetScalar, flAutoExposureMin, flAutoExposureMax, mat_hdr_tonemapscale.GetFloat(), pRenderContext->GetToneMappingScaleLinear().x );
                    }
                    else {
                        engine->Con_NPrintf( 19, "%.2f%% of pixels above %d%% target @ %4.2f%%  Target Scalar = %4.2f  Min/Max( %4.2f, %4.2f )  Final Scalar: %4.2f  Actual: %4.2f", mat_tonemap_percent_bright_pixels.GetFloat(), mat_tonemap_percent_target.GetInt(), ( g_HDR_HistogramSystem.FindLocationOfPercentBrightPixels( mat_tonemap_percent_bright_pixels.GetFloat(), mat_tonemap_percent_target.GetFloat() ) * 100.0f ), g_HDR_HistogramSystem.GetTargetTonemapScalar( true ), flAutoExposureMin, flAutoExposureMax, mat_hdr_tonemapscale.GetFloat(), pRenderContext->GetToneMappingScaleLinear().x );
                    }
                }
            }
        }
    }
}

static void DisplayHistogram( IMatRenderContext *pRenderContext, int nX, int nY, int nWidth, int nHeight, float flAutoExposureMin, float flAutoExposureMax )
{
    if( mat_show_histogram.GetInt() && ( engine->GetDXSupportLevel() >= 90 ) ) {
        g_HDR_HistogramSystem.DisplayHistogram();
    }
}

static void CenterScaleQuadUVs( Vector4D & quadUVs, const Vector2D & uvScale )
{
    Vector2D uvMid = 0.5f*Vector2D( ( quadUVs.z + quadUVs.x ), ( quadUVs.w + quadUVs.y ) );
    Vector2D uvRange = 0.5f*Vector2D( ( quadUVs.z - quadUVs.x ), ( quadUVs.w - quadUVs.y ) );
    quadUVs.x = uvMid.x - uvScale.x*uvRange.x;
    quadUVs.y = uvMid.y - uvScale.y*uvRange.y;
    quadUVs.z = uvMid.x + uvScale.x*uvRange.x;
    quadUVs.w = uvMid.y + uvScale.y*uvRange.y;
}

static ConVar r_queued_post_processing( "r_queued_post_processing", "0" );

// How much to dice up the screen during post-processing on 360
// This has really marginal effects, but 4x1 does seem vaguely better for post-processing
static ConVar mat_postprocess_x( "mat_postprocess_x", "4" );
static ConVar mat_postprocess_y( "mat_postprocess_y", "1" );

void DoEnginePostProcessing( int x, int y, int w, int h, bool bFlashlightIsOn, bool bPostVGui )
{
    tmZone( TELEMETRY_LEVEL0, TMZF_NONE, "%s", __FUNCTION__ );

    CMatRenderContextPtr pRenderContext( materials );

    if( g_bDumpRenderTargets ) {
        g_bDumpRenderTargets = false;   // Turn off from previous frame
    }

    if( mat_dump_rts.GetBool() ) {
        g_bDumpRenderTargets = true;    // Dump intermediate render targets this frame
        s_nRTIndex = 0;                 // Used for numbering the TGA files for easy browsing
        mat_dump_rts.SetValue( 0 );     // We only want to capture one frame, on rising edge of this convar

        DumpTGAofRenderTarget( w, h, "BackBuffer" );
    }

    if( r_queued_post_processing.GetInt() ) {
        ICallQueue *pCallQueue = pRenderContext->GetCallQueue();
        if( pCallQueue ) {
            pCallQueue->QueueCall( DoEnginePostProcessing, x, y, w, h, bFlashlightIsOn, bPostVGui );
            return;
        }
    }

    HDRType_t hdrType = g_pMaterialSystemHardwareConfig->GetHDRType();
    g_bFlashlightIsOn = bFlashlightIsOn;

    // Use the appropriate autoexposure min / max settings.
    // Mapmaker's overrides the convar settings.
    float flAutoExposureMin;
    float flAutoExposureMax;
    GetExposureRange( &flAutoExposureMin, &flAutoExposureMax );

    switch( hdrType ) {
        case HDR_TYPE_NONE:
        case HDR_TYPE_INTEGER:
        {
            s_bScreenEffectTextureIsUpdated = false;

            if( hdrType != HDR_TYPE_NONE ) {
                DoTonemapping( pRenderContext, x, y, w, h, flAutoExposureMin, flAutoExposureMax );
            }

            bool bPerformColCorrect = !bPostVGui && ( g_pMaterialSystemHardwareConfig->GetDXSupportLevel() >= 90 ) && ( g_pMaterialSystemHardwareConfig->GetHDRType() != HDR_TYPE_FLOAT ) && g_pColorCorrectionMgr->HasNonZeroColorCorrectionWeights() && mat_colorcorrection.GetInt();
            bool bSplitScreenHDR = mat_show_ab_hdr.GetInt();

            pRenderContext->EnableColorCorrection( bPerformColCorrect );
            if( bPerformColCorrect ) {
                tmZone( TELEMETRY_LEVEL0, TMZF_NONE, "ColorCorrection" );

                ITexture *pSrc = materials->FindTexture( "_rt_FullFrameFB", TEXTURE_GROUP_RENDER_TARGET );
                int nSrcWidth = pSrc->GetActualWidth();
                int nSrcHeight = pSrc->GetActualHeight();

                ITexture *dest_rt1 = materials->FindTexture( "_rt_SmallFB1", TEXTURE_GROUP_RENDER_TARGET );

                if( !s_bScreenEffectTextureIsUpdated ) {
                    // NOTE: UpdateScreenEffectTexture() uses StretchRect, so _rt_FullFrameFB is always 100%
                    //       filled, even when the viewport is not fullscreen (e.g. with 'mat_viewportscale 0.5')
                    UpdateScreenEffectTexture( 0, x, y, w, h, true );
                    s_bScreenEffectTextureIsUpdated = true;
                }

                // Now add bloom (dest_rt0) to the framebuffer and perform software anti-aliasing and
                // colour correction, all in one pass (improves performance, reduces quantization errors)
                //
                // First, set up texel coords (in the bloom and fb textures) at the centres of the outer pixel of the viewport:
                Vector4D fullViewportPostSrcCorners( 0.0f, -0.5f, nSrcWidth / 4 - 1, nSrcHeight / 4 - 1 );
                Vector4D fullViewportPostDestCorners( 0.0f, 0.0f, nSrcWidth - 1, nSrcHeight - 1 );
                Rect_t   fullViewportPostDestRect = { x, y, w, h };
                Vector2D destTexSize( nSrcWidth, nSrcHeight );

                // When the viewport is not fullscreen, the UV-space size of a pixel changes
                // (due to a stretchrect blit being used in UpdateScreenEffectTexture()), so
                // we need to adjust the corner-pixel UVs sent to our drawrect call:
                Vector2D uvScale( ( nSrcWidth - ( nSrcWidth / (float)w ) ) / ( nSrcWidth - 1 ), ( nSrcHeight - ( nSrcHeight / (float)h ) ) / ( nSrcHeight - 1 ) );
                CenterScaleQuadUVs( fullViewportPostSrcCorners, uvScale );
                CenterScaleQuadUVs( fullViewportPostDestCorners, uvScale );

                Rect_t   partialViewportPostDestRect = fullViewportPostDestRect;
                Vector4D partialViewportPostSrcCorners = fullViewportPostSrcCorners;
                if( debug_postproc.GetInt() == 2 ) {
                    // Restrict the post effects to the centre quarter of the screen
                    // (we only use a portion of the bloom texture, so this *does* affect bloom texture UVs)
                    partialViewportPostDestRect.x += 0.25f*fullViewportPostDestRect.width;
                    partialViewportPostDestRect.y += 0.25f*fullViewportPostDestRect.height;
                    partialViewportPostDestRect.width -= 0.50f*fullViewportPostDestRect.width;
                    partialViewportPostDestRect.height -= 0.50f*fullViewportPostDestRect.height;

                    // This math interprets texel coords as being at corner pixel centers (*not* at corner vertices):
                    Vector2D uvScale( 1.0f - ( ( w / 2 ) / (float)( w - 1 ) ), 1.0f - ( ( h / 2 ) / (float)( h - 1 ) ) );
                    CenterScaleQuadUVs( partialViewportPostSrcCorners, uvScale );
                }

                // Temporary hack... Color correction was crashing on the first frame
                // when run outside the debugger for some mods (DoD). This forces it to skip
                // a frame, ensuring we don't get the weird texture crash we otherwise would.
                // FIXME: This will be removed when the true cause is found [added: Main CL 144694]
                static bool bFirstFrame = !IsX360();
                if( !bFirstFrame || !bPerformColCorrect ) {
                    bool bFBUpdated = false;

                    if( mat_postprocessing_combine.GetInt() ) {
                        // Perform post-processing in one combined pass
                        IMaterial *post_mat = CEnginePostMaterialProxy::SetupEnginePostMaterial( fullViewportPostDestCorners, destTexSize, bPerformColCorrect );

                        if( bSplitScreenHDR ) {
                            pRenderContext->SetScissorRect( partialViewportPostDestRect.width / 2, 0, partialViewportPostDestRect.width, partialViewportPostDestRect.height, true );
                        }

                        // thicc call
                        pRenderContext->DrawScreenSpaceRectangle( post_mat, 0, 0, partialViewportPostDestRect.width, partialViewportPostDestRect.height, partialViewportPostSrcCorners.x, partialViewportPostSrcCorners.y, partialViewportPostSrcCorners.z, partialViewportPostSrcCorners.w, dest_rt1->GetActualWidth(), dest_rt1->GetActualHeight(), GetClientWorldEntity()->GetClientRenderable(), mat_postprocess_x.GetInt(), mat_postprocess_y.GetInt() );

                        if( bSplitScreenHDR ) {
                            pRenderContext->SetScissorRect( -1, -1, -1, -1, false );
                        }
                        bFBUpdated = true;
                    }
                    else if( bPerformColCorrect ) {
                        if( bFBUpdated ) {
                            Rect_t actualRect;
                            UpdateScreenEffectTexture( 0, x, y, w, h, false, &actualRect );
                        }

                        IMaterial *colcorrect_mat = CEnginePostMaterialProxy::SetupEnginePostMaterial( fullViewportPostDestCorners, destTexSize, bPerformColCorrect );

                        if( bSplitScreenHDR ) {
                            pRenderContext->SetScissorRect( partialViewportPostDestRect.width / 2, 0, partialViewportPostDestRect.width, partialViewportPostDestRect.height, true );
                        }

                        // thicc call
                        pRenderContext->DrawScreenSpaceRectangle( colcorrect_mat, partialViewportPostDestRect.x, partialViewportPostDestRect.y, partialViewportPostDestRect.width, partialViewportPostDestRect.height, partialViewportPostSrcCorners.x, partialViewportPostSrcCorners.y, partialViewportPostSrcCorners.z, partialViewportPostSrcCorners.w, dest_rt1->GetActualWidth(), dest_rt1->GetActualHeight(), GetClientWorldEntity()->GetClientRenderable() );

                        if( bSplitScreenHDR ) {
                            pRenderContext->SetScissorRect( -1, -1, -1, -1, false );
                        }
                        bFBUpdated = true;
                    }

                    if( g_bDumpRenderTargets ) {
                        DumpTGAofRenderTarget( partialViewportPostDestRect.width, partialViewportPostDestRect.height, "EnginePost" );
                    }
                }
                bFirstFrame = false;
            }

            if( hdrType != HDR_TYPE_NONE ) {
                DisplayHistogram( pRenderContext, x, y, w, h, flAutoExposureMin, flAutoExposureMax );
            }
        } break;

// Seems that the mat_hdr_level 3 is broken so float backbuffer is unused.
#if 0
        case HDR_TYPE_FLOAT:
        {
            int dest_width, dest_height;
            pRenderContext->GetRenderTargetDimensions( dest_width, dest_height );
            if( mat_dynamic_tonemapping.GetInt() || mat_show_histogram.GetInt() ) {
                g_HDR_HistogramSystem.Update();
                if( mat_dynamic_tonemapping.GetInt() ) {
                    float avg_lum = MAX( 0.0001, g_HDR_HistogramSystem.GetTargetTonemapScalar() );
                    float scalevalue = MAX( flAutoExposureMin, MIN( flAutoExposureMax, 0.18 / avg_lum ) );
                    pRenderContext->SetGoalToneMappingScale( scalevalue );
                    mat_hdr_tonemapscale.SetValue( scalevalue );
                }
            }

            //DrawPostEffects( HDR_Final_F, x, y, dest_width, dest_height );

            pRenderContext->SetRenderTarget( NULL );
            if( mat_show_histogram.GetInt() && ( engine->GetDXSupportLevel() >= 90 ) ) {
                g_HDR_HistogramSystem.DisplayHistogram();
            }

            if( mat_dynamic_tonemapping.GetInt() ) {
                float avg_lum = MAX( 0.0001, g_HDR_HistogramSystem.GetTargetTonemapScalar() );
                float scalevalue = MAX( flAutoExposureMin, MIN( flAutoExposureMax, 0.023 / avg_lum ) );
                SetToneMapScale( pRenderContext, scalevalue, flAutoExposureMin, flAutoExposureMax );
            }
            pRenderContext->SetRenderTarget( NULL );
        } break;
#endif
    }

    // Draw refraction post effects
    DrawPostEffects( PP_Final, ARRAYSIZE(PP_Final), x, y, w, h );
}

// Motion Blur Material Proxy =========================================================================================
static float g_vMotionBlurValues[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
class CMotionBlurMaterialProxy : public CEntityMaterialProxy {
public:
    CMotionBlurMaterialProxy();
    virtual ~CMotionBlurMaterialProxy();
    virtual bool Init( IMaterial *pMaterial, KeyValues *pKeyValues );
    virtual void OnBind( C_BaseEntity *pEntity );
    virtual IMaterial *GetMaterial();

private:
    IMaterialVar *m_pMaterialParam;
};

CMotionBlurMaterialProxy::CMotionBlurMaterialProxy()
{
    m_pMaterialParam = NULL;
}

CMotionBlurMaterialProxy::~CMotionBlurMaterialProxy()
{
    // Do nothing
}

bool CMotionBlurMaterialProxy::Init( IMaterial *pMaterial, KeyValues *pKeyValues )
{
    bool bFoundVar = false;

    m_pMaterialParam = pMaterial->FindVar( "$MotionBlurInternal", &bFoundVar, false );
    if( bFoundVar == false )
        return false;

    return true;
}

void CMotionBlurMaterialProxy::OnBind( C_BaseEntity *pEnt )
{
    if( m_pMaterialParam != NULL ) {
        m_pMaterialParam->SetVecValue( g_vMotionBlurValues, 4 );
    }
}

IMaterial *CMotionBlurMaterialProxy::GetMaterial()
{
    if( m_pMaterialParam == NULL )
        return NULL;

    return m_pMaterialParam->GetOwningMaterial();
}

EXPOSE_INTERFACE( CMotionBlurMaterialProxy, IMaterialProxy, "MotionBlur" IMATERIAL_PROXY_INTERFACE_VERSION );

//=====================================================================================================================
// Image-space Motion Blur ============================================================================================
//=====================================================================================================================
ConVar mat_motion_blur_enabled( "mat_motion_blur_enabled", "1", FCVAR_ARCHIVE );
ConVar mat_motion_blur_forward_enabled( "mat_motion_blur_forward_enabled", "0" );
ConVar mat_motion_blur_falling_min( "mat_motion_blur_falling_min", "10.0" );
ConVar mat_motion_blur_falling_max( "mat_motion_blur_falling_max", "20.0" );
ConVar mat_motion_blur_falling_intensity( "mat_motion_blur_falling_intensity", "1.0" );
ConVar mat_motion_blur_rotation_intensity( "mat_motion_blur_rotation_intensity", "1.0" );
ConVar mat_motion_blur_strength( "mat_motion_blur_strength", "1.0" );

void DoImageSpaceMotionBlur( const CViewSetup &view, int x, int y, int w, int h )
{
    if( ( !mat_motion_blur_enabled.GetInt() ) || ( g_pMaterialSystemHardwareConfig->GetDXSupportLevel() < 90 ) ) {
        return;
    }

    //======================================================================================================//
    // Get these convars here to make it easier to remove them later and to default each client differently //
    //======================================================================================================//
    float flMotionBlurRotationIntensity = mat_motion_blur_rotation_intensity.GetFloat() * 0.15f; // The default is to not blur past 15% of the range
    float flMotionBlurRollIntensity = 0.3f; // * mat_motion_blur_roll_intensity.GetFloat(); // The default is to not blur past 30% of the range
    float flMotionBlurFallingIntensity = mat_motion_blur_falling_intensity.GetFloat();
    float flMotionBlurFallingMin = mat_motion_blur_falling_min.GetFloat();
    float flMotionBlurFallingMax = mat_motion_blur_falling_max.GetFloat();
    float flMotionBlurGlobalStrength = mat_motion_blur_strength.GetFloat();

    //===============================================================================//
    // Set global g_vMotionBlurValues[4] values so material proxy can get the values //
    //===============================================================================//
    if( true ) {
        //=====================//
        // Previous frame data //
        //=====================//
        static float s_flLastTimeUpdate = 0.0f;
        static float s_flPreviousPitch = 0.0f;
        static float s_flPreviousYaw = 0.0f;
        static float s_vPreviousPositon[3] = { 0.0f, 0.0f, 0.0f };
        static matrix3x4_t s_mPreviousFrameBasisVectors;
        static float s_flNoRotationalMotionBlurUntil = 0.0f;
        //float vPreviousSideVec[3] = { s_mPreviousFrameBasisVectors[0][1], s_mPreviousFrameBasisVectors[1][1], s_mPreviousFrameBasisVectors[2][1] };
        //float vPreviousForwardVec[3] = { s_mPreviousFrameBasisVectors[0][0], s_mPreviousFrameBasisVectors[1][0], s_mPreviousFrameBasisVectors[2][0] };
        //float vPreviousUpVec[3] = { s_mPreviousFrameBasisVectors[0][2], s_mPreviousFrameBasisVectors[1][2], s_mPreviousFrameBasisVectors[2][2] };

        float flTimeElapsed = gpGlobals->realtime - s_flLastTimeUpdate;

        //===================================//
        // Get current pitch & wrap to +-180 //
        //===================================//
        float flCurrentPitch = view.angles[PITCH];
        while( flCurrentPitch > 180.0f ) {
            flCurrentPitch -= 360.0f;
        }
        while( flCurrentPitch < -180.0f ) {
            flCurrentPitch += 360.0f;
        }

        //=================================//
        // Get current yaw & wrap to +-180 //
        //=================================//
        float flCurrentYaw = view.angles[YAW];
        while( flCurrentYaw > 180.0f ) {
            flCurrentYaw -= 360.0f;
        }
        while( flCurrentYaw < -180.0f ) {
            flCurrentYaw += 360.0f;
        }

        //===========================//
        // Get current basis vectors //
        //===========================//
        matrix3x4_t mCurrentBasisVectors;
        AngleMatrix( view.angles, mCurrentBasisVectors );

        float vCurrentSideVec[3] = { mCurrentBasisVectors[0][1], mCurrentBasisVectors[1][1], mCurrentBasisVectors[2][1] };
        float vCurrentForwardVec[3] = { mCurrentBasisVectors[0][0], mCurrentBasisVectors[1][0], mCurrentBasisVectors[2][0] };

        //======================//
        // Get current position //
        //======================//
        float vCurrentPosition[3] = { view.origin.x, view.origin.y, view.origin.z };

        //===============================================================//
        // Evaluate change in position to determine if we need to update //
        //===============================================================//
        float vPositionChange[3] = { 0.0f, 0.0f, 0.0f };
        VectorSubtract( s_vPreviousPositon, vCurrentPosition, vPositionChange );
        if( ( VectorLength( vPositionChange ) > 30.0f ) && ( flTimeElapsed >= 0.5f ) ) {
            //=======================================================//
            // If we moved a far distance in one frame and more than //
            // half a second elapsed, disable motion blur this frame //
            //=======================================================//
            g_vMotionBlurValues[0] = 0.0f;
            g_vMotionBlurValues[1] = 0.0f;
            g_vMotionBlurValues[2] = 0.0f;
            g_vMotionBlurValues[3] = 0.0f;
        }
        else if( flTimeElapsed > ( 1.0f / 15.0f ) ) {
            //==========================================//
            // If slower than 15 fps, don't motion blur //
            //==========================================//
            g_vMotionBlurValues[0] = 0.0f;
            g_vMotionBlurValues[1] = 0.0f;
            g_vMotionBlurValues[2] = 0.0f;
            g_vMotionBlurValues[3] = 0.0f;
        }
        else if( VectorLength( vPositionChange ) > 50.0f ) {
            //================================================================================//
            // We moved a far distance in a frame, use the same motion blur as last frame     //
            // because I think we just went through a portal (should we ifdef this behavior?) //
            //================================================================================//
            s_flNoRotationalMotionBlurUntil = gpGlobals->realtime + 1.0f; // Wait a second until the portal craziness calms down
        }
        else {
            //====================//
            // Normal update path //
            //====================//
            // Compute horizontal and vertical fov
            float flHorizontalFov = view.fov;
            float flVerticalFov = ( view.m_flAspectRatio <= 0.0f ) ? ( view.fov ) : ( view.fov / view.m_flAspectRatio );

            //=====================//
            // Forward motion blur //
            //=====================//
            float flViewDotMotion = DotProduct( vCurrentForwardVec, vPositionChange );
            if( mat_motion_blur_forward_enabled.GetBool() ) {   // Want forward and falling
                g_vMotionBlurValues[2] = flViewDotMotion;
            }
            else {  // Falling only
                g_vMotionBlurValues[2] = flViewDotMotion * fabs( vCurrentForwardVec[2] ); // Only want this if we're looking up or down;
            }

            //====================================//
            // Yaw (Compensate for circle strafe) //
            //====================================//
            float flSideDotMotion = DotProduct( vCurrentSideVec, vPositionChange );
            float flYawDiffOriginal = s_flPreviousYaw - flCurrentYaw;
            if( ( ( s_flPreviousYaw - flCurrentYaw > 180.0f ) || ( s_flPreviousYaw - flCurrentYaw < -180.0f ) ) && ( ( s_flPreviousYaw + flCurrentYaw > -180.0f ) && ( s_flPreviousYaw + flCurrentYaw < 180.0f ) ) ) {
                flYawDiffOriginal = s_flPreviousYaw + flCurrentYaw;
            }

            float flYawDiffAdjusted = flYawDiffOriginal + ( flSideDotMotion / 3.0f ); // Yes, 3.0 is a magic number, sue me

            // Make sure the adjustment only lessens the effect, not magnify it or reverse it
            if( flYawDiffOriginal < 0.0f ) {
                flYawDiffAdjusted = clamp( flYawDiffAdjusted, flYawDiffOriginal, 0.0f );
            }
            else {
                flYawDiffAdjusted = clamp( flYawDiffAdjusted, 0.0f, flYawDiffOriginal );
            }

            // Use pitch to dampen yaw
            float flUndampenedYaw = flYawDiffAdjusted / flHorizontalFov;
            g_vMotionBlurValues[0] = flUndampenedYaw * ( 1.0f - ( fabs( flCurrentPitch ) / 90.0f ) ); // Dampen horizontal yaw blur based on pitch

            //=======================================//
            // Pitch (Compensate for forward motion) //
            //=======================================//
            float flPitchCompensateMask = 1.0f - ( ( 1.0f - fabs( vCurrentForwardVec[2] ) ) * ( 1.0f - fabs( vCurrentForwardVec[2] ) ) );
            float flPitchDiffOriginal = s_flPreviousPitch - flCurrentPitch;
            float flPitchDiffAdjusted = flPitchDiffOriginal;

            if( flCurrentPitch > 0.0f ) {
                flPitchDiffAdjusted = flPitchDiffOriginal - ( ( flViewDotMotion / 2.0f ) * flPitchCompensateMask ); // Yes, 2.0 is a magic number, sue me
            }
            else {
                flPitchDiffAdjusted = flPitchDiffOriginal + ( ( flViewDotMotion / 2.0f ) * flPitchCompensateMask ); // Yes, 2.0 is a magic number, sue me
            }

            // Make sure the adjustment only lessens the effect, not magnify it or reverse it
            if( flPitchDiffOriginal < 0.0f ) {
                flPitchDiffAdjusted = clamp( flPitchDiffAdjusted, flPitchDiffOriginal, 0.0f );
            }
            else {
                flPitchDiffAdjusted = clamp( flPitchDiffAdjusted, 0.0f, flPitchDiffOriginal );
            }

            g_vMotionBlurValues[1] = flPitchDiffAdjusted / flVerticalFov;

            //========================================================//
            // Roll (Enabled when we're looking down and yaw changes) //
            //========================================================//
            g_vMotionBlurValues[3] = flUndampenedYaw; // Roll starts out as undampened yaw intensity and is then scaled by pitch
            g_vMotionBlurValues[3] *= ( fabs( flCurrentPitch ) / 90.0f ) * ( fabs( flCurrentPitch ) / 90.0f ) * ( fabs( flCurrentPitch ) / 90.0f ); // Dampen roll based on pitch^3

            //==============================================================//
            // Time-adjust falling effect until we can do something smarter //
            //==============================================================//
            if( flTimeElapsed > 0.0f ) {
                g_vMotionBlurValues[2] /= flTimeElapsed * 30.0f; // 1/30th of a second?
            }
            else {
                g_vMotionBlurValues[2] = 0.0f;
            }

            // Scale and bias values after time adjustment
            g_vMotionBlurValues[2] = clamp( ( fabs( g_vMotionBlurValues[2] ) - flMotionBlurFallingMin ) / ( flMotionBlurFallingMax - flMotionBlurFallingMin ), 0.0f, 1.0f ) * ( g_vMotionBlurValues[2] >= 0.0f ? 1.0f : -1.0f );
            g_vMotionBlurValues[2] /= 30.0f; // To counter-adjust for time adjustment above

            //=================//
            // Apply intensity //
            //=================//
            g_vMotionBlurValues[0] *= flMotionBlurRotationIntensity * flMotionBlurGlobalStrength;
            g_vMotionBlurValues[1] *= flMotionBlurRotationIntensity * flMotionBlurGlobalStrength;
            g_vMotionBlurValues[2] *= flMotionBlurFallingIntensity * flMotionBlurGlobalStrength;
            g_vMotionBlurValues[3] *= flMotionBlurRollIntensity * flMotionBlurGlobalStrength;

            //===============================================================//
            // Dampen motion blur from 100%-0% as fps drops from 50fps-30fps //
            //===============================================================//
            if( !IsX360() ) {   // I'm not doing this on the 360 yet since I can't test it
                float flSlowFps = 30.0f;
                float flFastFps = 50.0f;
                float flCurrentFps = ( flTimeElapsed > 0.0f ) ? ( 1.0f / flTimeElapsed ) : 0.0f;
                float flDampenFactor = clamp( ( ( flCurrentFps - flSlowFps ) / ( flFastFps - flSlowFps ) ), 0.0f, 1.0f );

                g_vMotionBlurValues[0] *= flDampenFactor;
                g_vMotionBlurValues[1] *= flDampenFactor;
                g_vMotionBlurValues[2] *= flDampenFactor;
                g_vMotionBlurValues[3] *= flDampenFactor;

                //engine->Con_NPrintf( 6, "Dampen: %.2f", flDampenFactor );
            }

            //engine->Con_NPrintf( 6, "Final values: { %6.2f%%, %6.2f%%, %6.2f%%, %6.2f%% }", g_vMotionBlurValues[0]*100.0f, g_vMotionBlurValues[1]*100.0f, g_vMotionBlurValues[2]*100.0f, g_vMotionBlurValues[3]*100.0f );
        }

        //============================================//
        // Zero out blur if still in that time window //
        //============================================//
        if( gpGlobals->realtime < s_flNoRotationalMotionBlurUntil ) {
            // Zero out rotational blur but leave forward/falling blur alone
            g_vMotionBlurValues[0] = 0.0f; // X
            g_vMotionBlurValues[1] = 0.0f; // Y
            g_vMotionBlurValues[3] = 0.0f; // Roll
        }
        else {
            s_flNoRotationalMotionBlurUntil = 0.0f;
        }

        //====================================//
        // Store current frame for next frame //
        //====================================//
        VectorCopy( vCurrentPosition, s_vPreviousPositon );
        s_mPreviousFrameBasisVectors = mCurrentBasisVectors;
        s_flPreviousPitch = flCurrentPitch;
        s_flPreviousYaw = flCurrentYaw;
        s_flLastTimeUpdate = gpGlobals->realtime;
    }

    //=============================================================================================//
    // Render quad and let material proxy pick up the g_vMotionBlurValues[4] values just set above //
    //=============================================================================================//
    if( true ) {
        CMatRenderContextPtr pRenderContext( materials );
        ITexture *pSrc = materials->FindTexture( "_rt_FullFrameFB", TEXTURE_GROUP_RENDER_TARGET );
        int nSrcWidth = pSrc->GetActualWidth();
        int nSrcHeight = pSrc->GetActualHeight();
        int dest_width, dest_height, nDummy;
        pRenderContext->GetViewport( nDummy, nDummy, dest_width, dest_height );

        if( g_pMaterialSystemHardwareConfig->GetHDRType() != HDR_TYPE_FLOAT ) {
            UpdateScreenEffectTexture( 0, x, y, w, h, true ); // Do we need to check if we already did this?
        }

        // Get material pointer
        IMaterial *pMatMotionBlur = materials->FindMaterial( "dev/motion_blur", TEXTURE_GROUP_OTHER, true );

        if( pMatMotionBlur != NULL ) {
            pRenderContext->DrawScreenSpaceRectangle( pMatMotionBlur, 0, 0, dest_width, dest_height, 0, 0, nSrcWidth - 1, nSrcHeight - 1, nSrcWidth, nSrcHeight, GetClientWorldEntity()->GetClientRenderable() );

            if( g_bDumpRenderTargets ) {
                DumpTGAofRenderTarget( dest_width, dest_height, "MotionBlur" );
            }
        }
    }
}
