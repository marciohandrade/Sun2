// dllaccess.h

#ifndef _3DFILEACCESS_DLLACCESS_H_
#define _3DFILEACCESS_DLLACCESS_H_

#include "typedef.h"

enum ENUM_CONTACT_WZD
{
    CW_NEW_WZD_ANIMATION = 0,       // �ִϸ��̼� ����
    CW_NEW_WZD_MESH_INFO,           // ��� ����
    CW_NEW_WZD_MESH,                // �޽� ����
    CW_NEW_WZMESHINFO,              // ��� ����
    CW_NEW_WZANIMATIONINFO,         // �ִϸ��̼� ���� ���� (ĳ������������ ���)
    CW_NEW_WZUNIT,                  // ���� ����
    CW_NEW_WZUNITEDITABLE,          // ������ ���� ���� (ĳ������������ ���)
    CW_NEW_WZUNITDRAW,              // ���� �ν��Ͻ� ����

    CW_DELETE_WZD = 16,             // Wzd ����Ÿ ����
    CW_DELETE_ARRAY_WZD,            // Wzd ����Ÿ �迭 ����
    CW_DELETE_WZMESHINFO,           // ��� ����
    CW_DELETE_WZANIMATIONINFO,      // �ִϸ��̼� ���� ����
    CW_DELETE_WZUNIT,               // ���� ����
    CW_DELETE_WZUNITEDITABLE,       // ������ ���� ����
    CW_DELETE_ARRAY_WZUNIT,         // ���� �迭 ����
    CW_DELETE_WZUNITDRAW,           // ���� �ν��Ͻ� ����
    CW_DELETE_ARRAY_WZUNITDRAW,     // ���� �ν��Ͻ� �迭 ����

    CW_SET_BRIGHTNESS_FUNC = 40,    // Gouraud Shading �� ����
    CW_GET_BRIGHTNESS_FUNC,
    CW_SET_LIGHTDIRECTION = 50,     // �� ���� ����
    CW_GET_BLINN_VECTORS = 53,      // Blinn vector 3 �� ���
    CW_ON_CAMERADIR_CHANGE,         // ī�޶� ���� �����

    CW_SET_DISCACCESS = 1000,       // CDiscAccess ������ ����
    CW_SET_RENDERER = 1001,         // ������(CDrawBase) ����

    CW_SET_LOGHANDLER = 2000,       // �α� �ڵ鷯 ����
    CW_GET_LOGSYSTEM,
};

extern "C" WZ3DA_API void* Func_ContactWzd( ENUM_CONTACT_WZD, void* );

// ����� ��ũ��
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
