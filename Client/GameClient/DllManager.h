// dllmanager.h

#ifndef _SUN_DLLMANAGER_H_
#define _SUN_DLLMANAGER_H_

#include "dlltypedef.h"

class DllManager
{
public:
    // �ν��Ͻ� ���
    static DllManager* Instance( void );

    // �� ���� DLL �ε�/����
    BOOL LoadSeDlls( void );
    void FreeSeDlls( void );

private:
    // ������/�Ҹ���
    DllManager( void );
    ~DllManager( void );

    // 3dfileaccess.dll �ε�
    HINSTANCE Load3DFileAccessDll( const char* file_name_ptr, 
                                   FUNC_ContactWzd* contact_wzd_ptr );

    // 3dterrain.dll �ε�
    HINSTANCE LoadTerrainDll( const char* file_name_ptr, 
                              FUNC_ContactTerrain* contact_terrain_ptr );

    // effect.dll �ε�
    HINSTANCE LoadEffectDll( const char* file_name_ptr, 
                             FUNC_ContactEffect* contact_effect_ptr );

    // controls_dll.dll �ε�
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
