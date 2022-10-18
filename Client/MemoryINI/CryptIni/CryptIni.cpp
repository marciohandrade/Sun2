// CryptINI.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include <conio.h>
#include "MemoryIni.h"
#include "CryptFunc.h"

int test_mem_leak()
{	
	LPCTSTR IniName = _T("akcfg.ini");
	TCHAR	IniPath[MAX_PATH] = {0};
	TCHAR	CurrentPath[MAX_PATH] = {0};


	//	���� ���������� ����Ʈ ��η� �Ͽ� Ini���Ͽ� ���� ��θ� ����
	//

	HMODULE hMod = GetModuleHandle( NULL );

    GetModuleFileName( hMod, CurrentPath, MAX_PATH );

    *(_tcsrchr( CurrentPath, _T('\\') ) ) = _T('\0');
#if (_MSC_VER >= 1500) // vs2008 �̻�
    sprintf_s( IniPath, MAX_PATH, _T("%s\\%s"), CurrentPath, IniName );
#else
    StringCchPrintf( IniPath, MAX_PATH, _T("%s\\%s"), CurrentPath, IniName );
#endif

	

	//
	//	Step 1: ini ��ü ����
	//

	MemoryIni mini;


	//
	//	Step 2: ���� putString, putInteger�Լ��� ���� ���
	//

	mini.putInteger( _T("NameList"), _T("NameCount"), 4 );
	mini.putString ( _T("NameList"), _T("Name0"), _T("Seyool Park") );
	mini.putString ( _T("NameList"), _T("Name1"), _T("�ڼ���") );
	mini.putString ( _T("NameList"), _T("Name2"), _T("�����") );
	mini.putString ( _T("NameList"), _T("Name3"), _T("������") );


	//
	//	Step 3: ������ ���� ���Ϸ� ����, NULL�̸� ������ ������
	//

	if ( 0 > mini.SaveToFile( IniPath, SimpleXorCryption ) )
	{
		_ASSERTE( !"SaveToFile fail" );
		return -1;
	}


	//
	//	Step 4: IniPath�κ��� Ini������ �ε���, NULL�̸� ������ ������
	//

	if ( 0 > mini.LoadFromFile( IniPath, SimpleXorCryption ) )
	{
		_ASSERTE( !"LoadFromFile fail" );
		return -1;
	}


	//
	//	Step 5: �ʿ��� ���� ������ ��
	//

	TCHAR StrKey[MAX_LINE] = {0};
	TCHAR StrValue[MAX_LINE] = {0};
	LONG  NameCount = 0;

	if ( 0 > mini.getInteger(_T("NameList"), _T("NameCount"), &NameCount) )
	{
		_ASSERTE( !"getInteger fail" );
		return -1;
	}

	for ( LONG i = 0; i < NameCount; i++ )
	{
#if (_MSC_VER >= 1500) // vs2008 �̻�
        sprintf_s( StrKey, MAX_LINE, _T("Name%d"), i );
#else
        StringCchPrintf( StrKey, MAX_LINE, _T("Name%d"), i );
#endif
		mini.getString( _T("NameList"), StrKey, StrValue, MAX_LINE );
		
		_tprintf( _T("[NameList] %s = %s\n"), StrKey, StrValue );
	}

	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER( argc );
	UNREFERENCED_PARAMETER( argv );

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF  |
					_CRTDBG_LEAK_CHECK_DF |
					_CRTDBG_REPORT_FLAG );

	test_mem_leak();

	_CrtDumpMemoryLeaks();

	return 0;
}

