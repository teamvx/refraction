//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//

// C callable material system interface for the utils.
// und: fuck ton of winapi? or why there was a windows.h?

#include "materialsystem/imaterialsystem.h"
#include "materialsystem/imaterial.h"
#include "materialsystem/imaterialvar.h"
#include "utilmatlib.h"
#include "tier0/dbg.h"
#include "filesystem.h"
#include "materialsystem/materialsystem_config.h"

void LoadMaterialSystemInterface( CreateInterfaceFn fileSystemFactory )
{
    // Already loaded
    if( g_pMaterialSystem ) {
        return;
    }

    // materialsystem.dll should be in the path, it's in bin along with vbsp.
    const char *pDllName = "materialsystem.dll";
    CSysModule *materialSystemDLLHInst;
    materialSystemDLLHInst = g_pFullFileSystem->LoadModule( pDllName );
    if( !materialSystemDLLHInst ) {
        Error( "Can't load MaterialSystem.dll\n" );
    }

    CreateInterfaceFn clientFactory = Sys_GetFactory( materialSystemDLLHInst );
    if( clientFactory ) {
        g_pMaterialSystem = (IMaterialSystem *)clientFactory( MATERIAL_SYSTEM_INTERFACE_VERSION, NULL );
        if( !g_pMaterialSystem ) {
            Error( "Could not get the material system interface from materialsystem.dll (" __FILE__ ")" );
        }
    }
    else {
        Error( "Could not find factory interface in library MaterialSystem.dll" );
    }

    if( !g_pMaterialSystem->Init( "shaderapiempty.dll", 0, fileSystemFactory ) ) {
        Error( "Could not start the empty shader (shaderapiempty.dll)!" );
    }

    // See valvesoftware/source-sdk-2013#200
    g_pMaterialSystem->ModInit();
}

void InitMaterialSystem( const char *materialBaseDirPath, CreateInterfaceFn fileSystemFactory )
{
    LoadMaterialSystemInterface( fileSystemFactory );
    MaterialSystem_Config_t config;
    g_pMaterialSystem->OverrideConfig( config, false );
}

void ShutdownMaterialSystem()
{
    if( g_pMaterialSystem ) {
        g_pMaterialSystem->Shutdown();
        g_pMaterialSystem = NULL;
    }
}

MaterialSystemMaterial_t FindMaterial( const char *materialName, bool *pFound, bool bComplain )
{
    IMaterial *pMat = g_pMaterialSystem->FindMaterial( materialName, TEXTURE_GROUP_OTHER, bComplain );
    MaterialSystemMaterial_t matHandle = pMat;

    if( pFound ) {
        *pFound = true;
        if( IsErrorMaterial( pMat ) )
            *pFound = false;
    }

    return matHandle;
}

void GetMaterialDimensions( MaterialSystemMaterial_t materialHandle, int *width, int *height )
{
    PreviewImageRetVal_t retVal;
    ImageFormat dummyImageFormat;
    IMaterial *material = (IMaterial *)materialHandle;
    bool translucent;
    retVal = material->GetPreviewImageProperties( width, height, &dummyImageFormat, &translucent );
    if( retVal != MATERIAL_PREVIEW_IMAGE_OK ) {
        *width = 128;
        *height = 128;
    }
}

void GetMaterialReflectivity( MaterialSystemMaterial_t materialHandle, float *reflectivityVect )
{
    IMaterial *material = (IMaterial *)materialHandle;
    const IMaterialVar *reflectivityVar;

    bool found;
    reflectivityVar = material->FindVar( "$reflectivity", &found, false );
    if( !found ) {
        Vector tmp;
        material->GetReflectivity( tmp );
        VectorCopy( tmp.Base(), reflectivityVect );
    }
    else {
        reflectivityVar->GetVecValue( reflectivityVect, 3 );
    }
}

int GetMaterialShaderPropertyBool( MaterialSystemMaterial_t materialHandle, int propID )
{
    IMaterial *material = (IMaterial *)materialHandle;

    if( propID == UTILMATLIB_NEEDS_BUMPED_LIGHTMAPS ) {
        return material->GetPropertyFlag( MATERIAL_PROPERTY_NEEDS_BUMPED_LIGHTMAPS );
    }

    if( propID == UTILMATLIB_NEEDS_LIGHTMAP ) {
        return material->GetPropertyFlag( MATERIAL_PROPERTY_NEEDS_LIGHTMAP );
    }

    Assert( 0 );
    return 0;
}

int GetMaterialShaderPropertyInt( MaterialSystemMaterial_t materialHandle, int propID )
{
    IMaterial *material = (IMaterial *)materialHandle;

    if( propID == UTILMATLIB_OPACITY ) {
        if( material->IsTranslucent() ) {
            return UTILMATLIB_TRANSLUCENT;
        }

        if( material->IsAlphaTested() ) {
            return UTILMATLIB_ALPHATEST;
        }

        return UTILMATLIB_OPAQUE;
    }

    Assert( 0 );
    return 0;
}

const char *GetMaterialVar( MaterialSystemMaterial_t materialHandle, const char *propertyName )
{
    IMaterial *material = (IMaterial *)materialHandle;
    IMaterialVar *var;
    bool found;
    var = material->FindVar( propertyName, &found, false );
    if( found ) {
        return var->GetStringValue();
    }
    else {
        return NULL;
    }
}

const char *GetMaterialShaderName( MaterialSystemMaterial_t materialHandle )
{
    IMaterial *material = (IMaterial *)materialHandle;
    return material->GetShaderName();
}
