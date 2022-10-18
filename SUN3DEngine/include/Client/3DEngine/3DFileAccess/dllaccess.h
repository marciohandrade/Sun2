// dllaccess.h

#ifndef _3DFILEACCESS_DLLACCESS_H_
#define _3DFILEACCESS_DLLACCESS_H_

#include "typedef.h"

enum ENUM_CONTACT_WZD
{
    CW_NEW_WZD_ANIMATION = 0,       // 애니메이션 생성
    CW_NEW_WZD_MESH_INFO,           // 사용 안함
    CW_NEW_WZD_MESH,                // 메시 생성
    CW_NEW_WZMESHINFO,              // 사용 안함
    CW_NEW_WZANIMATIONINFO,         // 애니메이션 정보 생성 (캐릭터툴에서만 사용)
    CW_NEW_WZUNIT,                  // 유닛 생성
    CW_NEW_WZUNITEDITABLE,          // 편집용 유닛 생성 (캐릭터툴에서만 사용)
    CW_NEW_WZUNITDRAW,              // 유닛 인스턴스 생성

    CW_DELETE_WZD = 16,             // Wzd 데이타 제거
    CW_DELETE_ARRAY_WZD,            // Wzd 데이타 배열 제거
    CW_DELETE_WZMESHINFO,           // 사용 안함
    CW_DELETE_WZANIMATIONINFO,      // 애니메이션 정보 제거
    CW_DELETE_WZUNIT,               // 유닛 제거
    CW_DELETE_WZUNITEDITABLE,       // 편집용 유닛 제거
    CW_DELETE_ARRAY_WZUNIT,         // 유닛 배열 제거
    CW_DELETE_WZUNITDRAW,           // 유닛 인스턴스 제거
    CW_DELETE_ARRAY_WZUNITDRAW,     // 유닛 인스턴스 배열 제거

    CW_SET_BRIGHTNESS_FUNC = 40,    // Gouraud Shading 의 계산식
    CW_GET_BRIGHTNESS_FUNC,
    CW_SET_LIGHTDIRECTION = 50,     // 빛 방향 설정
    CW_GET_BLINN_VECTORS = 53,      // Blinn vector 3 개 얻기
    CW_ON_CAMERADIR_CHANGE,         // 카메라 방향 변경시

    CW_SET_DISCACCESS = 1000,       // CDiscAccess 포인터 지정
    CW_SET_RENDERER = 1001,         // 렌더러(CDrawBase) 설정

    CW_SET_LOGHANDLER = 2000,       // 로그 핸들러 설정
    CW_GET_LOGSYSTEM,
};

extern "C" WZ3DA_API void* Func_ContactWzd( ENUM_CONTACT_WZD, void* );

// 명시적 링크시
typedef void* (*FUNC_ContactWzd)( ENUM_CONTACT_WZD, void* );

//------------------------------------------------------------------------------
/**
*/
inline HINSTANCE Load3DFileAccessLibrary( const char* lpszDll, FUNC_ContactWzd* pFunc_ContactWzd )
{
    WzAssert( lpszDll );
    WzAssert( pFunc_ContactWzd );

    HINSTANCE hDll = LoadLibraryA( lpszDll );
    if( !hDll )
    {
        return NULL;
    }

    *pFunc_ContactWzd = (FUNC_ContactWzd)GetProcAddress( hDll, "Func_ContactWzd" );
    if( !( *pFunc_ContactWzd ) )
    {
        FreeLibrary( hDll );
        return NULL;
    }

    return hDll;
}

//extern "C" WZ3DA_API void Func_SetLogSystem_3DFileAccess( WzLogHandler* );

#endif // _3DFILEACCESS_DLLACCESS_H_
