// dllaccess.h

#ifndef _3DTERRAIN_DLLACCESS_H_
#define _3DTERRAIN_DLLACCESS_H_

#include "typedef.h"

// xxx: TODO: ���� �����Ͻ� ���� �߻����� ���� �ӽ÷� �߰�
// ���� �Ʒ� include ���� ������ ��
#include "pathexplorer.h"
#include "worldbase.h"

enum ENUM_CONTACT_TERRAIN
{
    CT_NEW_WORLD = 0,
    CT_DELETE_WORLD,
    CT_DELETE_ARRAY_WORLD,

    CT_NEW_PATH_EXPLORER = 10,
    CT_DELETE_PATH_EXPLORER,
    CT_DELETE_ARRAY_PATH_EXPLORER,

    CT_NEW_AUTOCAMERA = 20,
    CT_DELETE_AUTOCAMERA,
    CT_DELETE_ARRAY_AUTOCAMERA,

    CT_NEW_MAPLOADER = 30,
    CT_DELETE_MAPLOADER,

    CT_SET_LOGHANDLER = 40,
    CT_GET_LOGSYSTEM,
};

extern "C" WZTER_API void* Func_ContactTerrain( ENUM_CONTACT_TERRAIN, void* );

// ����� ��ũ��
typedef void* (*FUNC_ContactTerrain)( ENUM_CONTACT_TERRAIN, void* );

//------------------------------------------------------------------------------
/**
*/
inline HINSTANCE Load3DTerrainLibrary( const char* lpszDll, FUNC_ContactTerrain* pFunc_ContactTerrain )
{
    WzAssert( lpszDll );
    WzAssert( pFunc_ContactTerrain );

    HINSTANCE hDll = LoadLibraryA( lpszDll );
    if( !hDll )
    {
        return NULL;
    }

    *pFunc_ContactTerrain = (FUNC_ContactTerrain)GetProcAddress( hDll, "Func_ContactTerrain" );
    if( !(*pFunc_ContactTerrain) )
    {
        FreeLibrary( hDll );
        return NULL;
    }

    return hDll;
}

#endif // _3DTERRAIN_DLLACCESS_H_