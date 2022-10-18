// dllaccess.h

#ifndef _EFFECT_DLLACCESS_H_
#define _EFFECT_DLLACCESS_H_

#include "typedef.h"

enum ENUM_CONTACT_EFFECT
{
    CW_NEW_EFFECT_MANAGER = 0,
    CW_DELETE_EFFECT_MANAGER,

    CE_SET_LOGHANDLER,
    CE_GET_LOGSYSTEM,
};

extern "C" WZEFF_API void* Func_ContactEffect( ENUM_CONTACT_EFFECT, void* );

// 명시적 링크시
typedef void* (*FUNC_ContactEffect)( ENUM_CONTACT_EFFECT, void* );

//------------------------------------------------------------------------------
/**
*/
inline HINSTANCE LoadEffectLibrary( const char* lpszDll, FUNC_ContactEffect* pFunc_ContactEffect )
{
    WzAssert( lpszDll );
    WzAssert( pFunc_ContactEffect );

    HINSTANCE hDll = LoadLibraryA( lpszDll );
    if( !hDll )
    {
        return NULL;
    }

    *pFunc_ContactEffect = (FUNC_ContactEffect)GetProcAddress( hDll, "Func_ContactEffect" );
    if( !(*pFunc_ContactEffect) )
    {
        FreeLibrary( hDll );
        return NULL;
    }

    return hDll;
}

#endif // _EFFECT_DLLACCESS_H_