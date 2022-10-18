// dllmanager.h

#ifndef _SUN_DLLMANAGER_H_
#define _SUN_DLLMANAGER_H_

#include "dlltypedef.h"

class DllManager
{
public:
    // 인스턴스 얻기
    static DllManager* Instance( void );

    // 썬 엔진 DLL 로딩/해제
    BOOL LoadSeDlls( void );
    void FreeSeDlls( void );

private:
    // 생성자/소멸자
    DllManager( void );
    ~DllManager( void );

    // 3dfileaccess.dll 로드
    HINSTANCE Load3DFileAccessDll( const char* file_name_ptr, 
                                   FUNC_ContactWzd* contact_wzd_ptr );

    // 3dterrain.dll 로드
    HINSTANCE LoadTerrainDll( const char* file_name_ptr, 
                              FUNC_ContactTerrain* contact_terrain_ptr );

    // effect.dll 로드
    HINSTANCE LoadEffectDll( const char* file_name_ptr, 
                             FUNC_ContactEffect* contact_effect_ptr );

    // controls_dll.dll 로드
    HINSTANCE LoadInterfaceDll( const char* file_name_ptr, 
                                FUNC_DialogManagerWZ* func_dlg_mg_ptr );

private:
	HINSTANCE dll_3d_file_access_;
	HINSTANCE dll_3d_terrain_;
	HINSTANCE dll_effect_;
	HINSTANCE dll_interface_;
};

//------------------------------------------------------------------------------
/**
*/
inline DllManager* DllManager::Instance( void )
{
    static DllManager instance;

    return &instance;
}

#endif // _SUN_DLLMANAGER_H_
