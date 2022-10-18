// CryptINI.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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


	//	현재 실행파일을 디폴트 경로로 하여 Ini파일에 대한 경로를 구함
	//

	HMODULE hMod = GetModuleHandle( NULL );

    GetModuleFileName( hMod, CurrentPath, MAX_PATH );

    *(_tcsrchr( CurrentPath, _T('\\') ) ) = _T('\0');
#if (_MSC_VER >= 1500) // vs2008 이상
    sprintf_s( IniPath, MAX_PATH, _T("%s\\%s"), CurrentPath, IniName );
#else
    StringCchPrintf( IniPath, MAX_PATH, _T("%s\\%s"), CurrentPath, IniName );
#endif

	

	//
	//	Step 1: ini 객체 생성
	//

	MemoryIni mini;


	//
	//	Step 2: 값을 putString, putInteger함수로 값을 등록
	//

	mini.putInteger( _T("NameList"), _T("NameCount"), 4 );
	mini.putString ( _T("NameList"), _T("Name0"), _T("Seyool Park") );
	mini.putString ( _T("NameList"), _T("Name1"), _T("박세율") );
	mini.putString ( _T("NameList"), _T("Name2"), _T("朴世律") );
	mini.putString ( _T("NameList"), _T("Name3"), _T("その他") );


	//
	//	Step 3: 설정된 값을 파일로 저장, NULL이면 평문으로 저장함
	//

	if ( 0 > mini.SaveToFile( IniPath, SimpleXorCryption ) )
	{
		_ASSERTE( !"SaveToFile fail" );
		return -1;
	}


	//
	//	Step 4: IniPath로부터 Ini내용을 로드함, NULL이면 평문으로 간주함
	//

	if ( 0 > mini.LoadFromFile( IniPath, SimpleXorCryption ) )
	{
		_ASSERTE( !"LoadFromFile fail" );
		return -1;
	}


	//
	//	Step 5: 필요한 값을 추출해 냄
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
#if (_MSC_VER >= 1500) // vs2008 이상
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

