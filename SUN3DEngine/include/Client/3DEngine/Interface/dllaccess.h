#pragma once

#include "typedef.h"

enum ENUM_USE_TYPE
{
    WZ_NEW_DLG_MANAGER,
    WZ_DELETE_DLG_MANAGER,

    WZ_SET_LOGHANDLER,
    WZ_GET_LOGSYSTEM,
};

extern "C" WZCTL_API void* GFXDll_DialogManagerWZ( ENUM_USE_TYPE, void* );

// 명시적 링크시
typedef void* (*FUNC_DialogManagerWZ)( ENUM_USE_TYPE, void* );

//------------------------------------------------------------------------------
/**
*/
inline HINSTANCE FUNC_LoadDialogManagerAccess( const char* pszDllName, FUNC_DialogManagerWZ* pFuncDlgMG )
{
    WzAssert( pszDllName );
    WzAssert( pFuncDlgMG );

    HINSTANCE hModule = LoadLibraryA( pszDllName );
    if( !hModule )
    {
        return NULL;
    }

    *pFuncDlgMG = (FUNC_DialogManagerWZ)GetProcAddress( hModule, "GFXDll_DialogManagerWZ" );
    if( !(*pFuncDlgMG) )
    {   
        FreeLibrary( hModule );
        return NULL;    
    }

    return hModule;
}

