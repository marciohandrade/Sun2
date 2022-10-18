// dllaccess.h

#ifndef _3DRENDERER_DLLACCESS_H_
#define _3DRENDERER_DLLACCESS_H_

#include "typedef.h"



enum ENUM_CONTACT_RENDERER
{
    CR_SET_LOGHANDLER = 0,       // 로그 핸들러 설정
    CR_GET_LOGSYSTEM,
};

extern "C" WZRND_API void* Func_ContactRenderer( ENUM_CONTACT_RENDERER eWork, void* pPtr );


class CDrawBase;

extern "C" WZRND_API CDrawBase* New_DrawInstance();
extern "C" WZRND_API void Delete_DrawInstance( CDrawBase* );

// 명시적 링크시
typedef CDrawBase* (*FUNC_New_DrawLastInstance)();
typedef void (*FUNC_Delete_DrawLastInstance)( CDrawBase* );

//------------------------------------------------------------------------------
/**
*/
inline HINSTANCE Load3DRendererLibrary( const char* lpszDll, 
                                        FUNC_New_DrawLastInstance* pNew_DrawLastInstance,
                                        FUNC_Delete_DrawLastInstance* pDelete_DrawLastInstance )
{
    WzAssert( lpszDll );
    WzAssert( pNew_DrawLastInstance );
    WzAssert( pDelete_DrawLastInstance );

    HINSTANCE hDll = LoadLibraryA( lpszDll );
    if( !hDll )
    {
        return NULL;
    }

    *pNew_DrawLastInstance = (FUNC_New_DrawLastInstance)GetProcAddress( hDll, "New_DrawInstance" );
    *pDelete_DrawLastInstance = (FUNC_Delete_DrawLastInstance)GetProcAddress( hDll, "Delete_DrawInstance" );
    if( !(*pNew_DrawLastInstance) || !(*pDelete_DrawLastInstance) )
    {
        FreeLibrary( hDll );
        return NULL;
    }

    return hDll;
}

#endif // _3DRENDERER_DLLACCESS_H_