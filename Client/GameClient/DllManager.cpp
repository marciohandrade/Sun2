// sedllloader.cpp

#include "SunClientPrecompiledHeader.h"
#include "dllmanager.h"
#include "globaldata.h"
#include "globalfunc.h"

//------------------------------------------------------------------------------
/**
*/
DllManager::DllManager( void )
: dll_3d_file_access_( NULL )
, dll_3d_terrain_( NULL )
, dll_interface_( NULL )
, dll_effect_( NULL )
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
DllManager::~DllManager( void )
{
    FreeSeDlls();
}

//------------------------------------------------------------------------------
/**
*/
BOOL DllManager::LoadSeDlls( void )
{
    // xxx: �ٸ� dll�� �������� ControlsDll.dll�� ���� �Ϲ��� ��ũ��
    // Window2000���� ���� ���ȴ�. �׷��� �ε��� ControlsDll.dll�� 
    // ����ؼ� ����� ��ũ�� ����Ѵ�. �Ŀ� ��Ȯ�� ������ �˰� �Ǹ� ������ ��

#ifdef _DEBUG
	const char* interface_dll_name_ptr = "ControlsDLL_d.dll";
#else
	#ifdef LAST_RELEASE
		const char* interface_dll_name_ptr = "ControlsDLL.dll";
	#else
		const char* interface_dll_name_ptr = "ControlsDLL_p.dll";
	#endif
#endif

	// ControlsDLL.dll
	dll_interface_ = LoadInterfaceDll( interface_dll_name_ptr, &g_pContactDialog_FnPtr );
	if ( !dll_interface_ )
	{
		WRITELOG( eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("Failed to load ControlsDLL.dll - DLL load error") );
		assert( !"Failed to load ControlsDLL.dll - DLL load error" );

		return FALSE;
	}

	return TRUE;
} 

//------------------------------------------------------------------------------
/**
*/
void DllManager::FreeSeDlls( void )
{
	if ( dll_3d_file_access_ )
    {
        FreeLibrary( dll_3d_file_access_ );
        dll_3d_file_access_ = NULL;
    }

	if ( dll_3d_terrain_ )
    {
        FreeLibrary( dll_3d_terrain_ );
        dll_3d_terrain_ = NULL;
    }

    if ( dll_interface_ )
    {
        FreeLibrary( dll_interface_ );
        dll_interface_ = NULL;
    }

	if ( dll_effect_ )
    {
        FreeLibrary( dll_effect_ );
        dll_effect_ = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
HINSTANCE DllManager::Load3DFileAccessDll( const char* file_name_ptr, FUNC_ContactWzd* contact_wzd_ptr )
{
    assert( file_name_ptr );
    assert( contact_wzd_ptr );

    HINSTANCE hDll = LoadLibraryA( file_name_ptr );
    if ( !hDll )
    {
        return NULL;
    }

    *contact_wzd_ptr = (FUNC_ContactWzd)GetProcAddress( hDll, "Func_ContactWzd" );
    if ( !( *contact_wzd_ptr ) )
    {
        FreeLibrary( hDll );
        return NULL;
    }

    return hDll;
}

//------------------------------------------------------------------------------
/**
*/
HINSTANCE DllManager::LoadTerrainDll( const char* file_name_ptr, FUNC_ContactTerrain* contact_terrain_ptr )
{
    assert( file_name_ptr );
    assert( contact_terrain_ptr );

    HINSTANCE hDll = LoadLibraryA( file_name_ptr );
    if ( !hDll )
    {
        return NULL;
    }

    *contact_terrain_ptr = (FUNC_ContactTerrain)GetProcAddress( hDll, "Func_ContactTerrain" );
    if ( !(*contact_terrain_ptr) )
    {
        FreeLibrary( hDll );
        return NULL;
    }

    return hDll;
}

//------------------------------------------------------------------------------
/**
*/
HINSTANCE DllManager::LoadEffectDll( const char* file_name_ptr, FUNC_ContactEffect* contact_effect_ptr )
{
    assert( file_name_ptr );
    assert( contact_effect_ptr );

    HINSTANCE hDll = LoadLibraryA( file_name_ptr );
    if ( !hDll )
    {
        return NULL;
    }

    *contact_effect_ptr = (FUNC_ContactEffect)GetProcAddress( hDll, "Func_ContactEffect" );
    if ( !(*contact_effect_ptr) )
    {
        FreeLibrary( hDll );
        return NULL;
    }

    return hDll;
}

//------------------------------------------------------------------------------
/**
*/
HINSTANCE DllManager::LoadInterfaceDll( const char* file_name_ptr, FUNC_DialogManagerWZ* func_dlg_mg_ptr )
{
    assert( file_name_ptr );
    assert( func_dlg_mg_ptr );

    HINSTANCE hModule = LoadLibraryA( file_name_ptr );
    if ( !hModule )
    {
        return NULL;
    }

    *func_dlg_mg_ptr = (FUNC_DialogManagerWZ)GetProcAddress( hModule, "GFXDll_DialogManagerWZ" );
    if ( !(*func_dlg_mg_ptr) )
    {   
        FreeLibrary( hModule );
        return NULL;    
    }

    return hModule;
}
