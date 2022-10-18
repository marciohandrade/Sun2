/*-----------------------------------------------------------------------------
 * EnumProc.cpp
 *-----------------------------------------------------------------------------
 * process enumerating module 
 *-----------------------------------------------------------------------------
 * Copyright (c) , 2006, WEBZEN Inc.
 * All rights reserved.
 *
 * Author : Noh Yong Hwan, 2006, WEBZEN Inc. Research & Development Dept.
 *          fixbrain@gmail.com , somma@webzen.co.kr
**---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "EnumProc.h"


#include <crtdbg.h>
#include <tlhelp32.h>
#include <tchar.h>

#include <strsafe.h>
#pragma warning(disable : 4995)	

#include <vector>
using namespace std;


#ifdef _DEBUG
#pragma warning(disable : 4100)		
void DebugString(LPCTSTR fmt, ...)
{
#ifdef _DEBUG	
	HRESULT hRes; 

	va_list args;	
	TCHAR temp[2048] = {0};

	va_start(args,fmt);

#ifdef _VC60
	if (vsprintf(temp, fmt, args) > 0) {
		va_end(args);
		OutputDebugString(temp);
	}
#else
	hRes = StringCbVPrintf(
		temp, 
		(sizeof(temp)) -1,
		fmt,
		args);

	if (hRes == S_OK)
	{
		va_end(args);
		OutputDebugString(temp);
	}		
#endif	// _VC60

#endif // _DEBUG
}
#pragma warning( default : 4100 )

#endif	// _DEBUG



//-----------------------------------------------------------------------------
// 내부 전역 변수 
//
WORD	gWinVersion = 0;			
TCHAR	gErrorMessage[2048];
TCHAR	gTmpErrorMessage[2048];


/**	-----------------------------------------------------------------------
	\brief	GetLastError() 가 반환하는 에러코드를 읽을 수 있는 문자열로 리턴해 주는 함수

	\param	
	\return	
			성공시 TRUE, 실패시 0
	\code
			if(ret ==FALSE)
			{
				char err[256];
				if ( FormatErrorString(err, sizeof(err), GetLastError()) == TRUE) 
				{
					WriteLog("Error VirtualProtect:%s", err);	
				}
				else
				{
					WriteLog("Error VirtualProtect: %s", "undefined");
				}

				return false;
			}	
	\endcode		
-------------------------------------------------------------------------*/
BOOL FormatErrorString(TCHAR *msg, DWORD msg_size, DWORD err)
{
	*msg = NULL;

	if(FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, 
		err, 
		0, 
		msg, 
		msg_size, 
		NULL) == 0) 
	{
		return FALSE;		// 0
	}
	else
	{
		return TRUE;		// 1
	}
}



/**	-----------------------------------------------------------------------
	\brief	gErrorMessage 전역변수에 에러메세지를 세팅하는 함수 (fmt 포맷 문자열 : 시스템 에러메세지 형식)			

	\param	
			IncludeGetLastError		GetLastError() 메세지를 끝에 추가할 건지 말건지..
			fmt						에러 메세지 포맷 문자열

	\return	
			세팅한 에러 메세지 문자열 
-------------------------------------------------------------------------*/
LPCTSTR SetErrorString(BOOL IncludeGetLastError, TCHAR *fmt,...)
{   	
	TCHAR	temp[1024] = {'\0'};			
	TCHAR	errMsg[512] = {'\0'};
	DWORD	curPos = 0;
	va_list args;

	// 에러메세지 prefix 세팅
	//
	//	E: can not open file : xxxxxxblah..blah....
	//
	curPos += _stprintf(&temp[curPos], _TEXT("%s [%d]: "), _TEXT("E"), GetLastError());		
	va_start(args, fmt);
	curPos += _vstprintf(&temp[curPos], fmt, args);
	va_end(args);

	// system error message
	//
	if (IncludeGetLastError == TRUE)
	{
		if ( FormatErrorString(errMsg, sizeof(errMsg), GetLastError()) )
		{
			curPos += _stprintf(&temp[curPos], _TEXT(": %s"), errMsg);
		}
		else
		{
			curPos += _stprintf(&temp[curPos], _TEXT(": %s"), _TEXT("unknown error"));
		}	
	} 

	MoveString(gErrorMessage, temp, sizeof(gErrorMessage));	
	return gErrorMessage;
}


/**	-----------------------------------------------------------------------
	\brief	에러메세지 문자열을 담고 있는 포인터 반환	
-------------------------------------------------------------------------*/
LPCTSTR GetErrorString(void)
{
	MoveString(gTmpErrorMessage, gErrorMessage, sizeof(gTmpErrorMessage));	
	return gTmpErrorMessage;
}



/**	-----------------------------------------------------------------------
	\brief	OS 시스템 정보를 가져오는 함수
			% MSDN 을 참고했음 예제 "Getting the System Version"


	\param	
	\return	
			성공시 Windows 버전 넘버
			실패시 FALSE
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
WORD	__getWinVersion( VOID ) 
{

	OSVERSIONINFOEX osvi;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.
	//
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);


	BOOL bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi);
	if( bOsVersionInfoEx == FALSE )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}

	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
			return _WIN2003 ;
			
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
			return _WINXP ;

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
			return _WIN2000 ;

		if ( osvi.dwMajorVersion <= 4 )
			return _WINNT ;			

	case VER_PLATFORM_WIN32_WINDOWS:

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			return _WIN95 ;
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			
			if ( osvi.szCSDVersion[1] == 'A' ) {
				return _WINSE ;			
			}
			
			return _WIN98 ;
		} 

		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			return _WINME ;
		} 
		break;

	case VER_PLATFORM_WIN32s:
		break;

	}
	return FALSE; 
}


/**	-----------------------------------------------------------------------
	\brief	__getWinVersion() 함수의 프론트 엔드  
-------------------------------------------------------------------------*/
WORD	GetWinVersion( VOID ) 
{
	if (gWinVersion	== 0)
	{
		gWinVersion = __getWinVersion();

		return gWinVersion;
	}
	else
	{
		return gWinVersion;
	}
}

/**	-----------------------------------------------------------------------
	\brief	권한상승 함수

	\param	
	\return			
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
BOOL SetPrivilege(
				  HANDLE hToken,  // token handle
				  LPCTSTR Privilege,  // Privilege to enable/disable
				  BOOL bEnablePrivilege  // TRUE to enable. FALSE to disable
				  )
{
	TOKEN_PRIVILEGES tp = { 0 };
	LUID luid;
	DWORD cb=sizeof(TOKEN_PRIVILEGES);
	if(!LookupPrivilegeValue( NULL, Privilege, &luid ))
	{	
		_ASSERTE(!"LookupPrivilegeValue");
		SetErrorString(TRUE, _T("%s: LookupPrivilegeValue()"), __FUNCTION__);
		return FALSE;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if(bEnablePrivilege) {
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	} else {
		tp.Privileges[0].Attributes = 0;
	}
	AdjustTokenPrivileges( hToken, FALSE, &tp, cb, NULL, NULL );
	if (GetLastError() != ERROR_SUCCESS)
	{		
		_ASSERTE(!"AdjustTokenPrivileges");
		SetErrorString(TRUE, _T("%s: AdjustTokenPrivileges()"), __FUNCTION__);
		return FALSE;
	}

	return TRUE;
}




/** \brief	문자열 복사를 위한 함수(null-terminated string 에 대해서만 사용해야 한다.!!!)
	\param	Dest	문자열을 복사할 SizeOfDest 크기의 버퍼
	\param	Src		원본 NULL-Terminated 문자열	
*/
void MoveString(LPTSTR Dest, LPCTSTR Src, SIZE_T SizeOfDest)
{
	_ASSERTE(Dest != NULL);
	_ASSERTE(Src != NULL);
	_ASSERTE(SizeOfDest != 0);
	_ASSERTE( IsBadStringPtr(Dest, SizeOfDest) != TRUE );

	
	SIZE_T srcLen = 0;
	if ( (Dest != NULL) && (Src != NULL) )
	{
		memset(Dest, 0x00, SizeOfDest);
		srcLen = _tcslen((TCHAR *)Src);
		if (srcLen >= SizeOfDest)
		{
			memmove(Dest, Src, SizeOfDest -1);		// for Null terminated
		}
		else
		{
			memmove(Dest, Src, srcLen);
		}
	
	}	
}




/**	-----------------------------------------------------------------------
	\brief	

	\param	
	\return			
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
INT EnumProcs(std::vector<RUNNING_PROC_INFO> *vectProc)
{
	_ASSERTE(NULL != vectProc);
	if (NULL == vectProc)
	{
		SetErrorString(FALSE, TEXT("%s >> invalid parameter"), __FUNCTION__);
		return -1;
	}

	RUNNING_PROC_INFO run_proc_info = {0};
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe32 = {0};	

	// Take a snapshot of all processes in the system.
	//
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	_ASSERTE(INVALID_HANDLE_VALUE != hProcessSnap);
	if(INVALID_HANDLE_VALUE == hProcessSnap)
	{
		SetErrorString(TRUE, TEXT("%s >> CreateToolhelp32Snapshot"), __FUNCTION__);
		return -1;
	}

	// Set the size of the structure before using it.
	//
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	//
	BOOL bRet = Process32First(hProcessSnap, &pe32);
	_ASSERTE(FALSE != bRet);
	if(FALSE == bRet)
	{	
		SetErrorString(TRUE, TEXT("%s >> Process32First"), __FUNCTION__);
		CloseHandle( hProcessSnap );     // Must clean up the snapshot object!
		return -1;
	}

	// Now walk the snapshot of processes
	//
	do
	{
		memset(&run_proc_info, 0x00, sizeof(run_proc_info));

		run_proc_info.ProcessID = pe32.th32ProcessID;
		MoveString(run_proc_info.ExeFile, pe32.szExeFile, sizeof(run_proc_info.ExeFile));
		
#define ex_pr_nm1 "wscript.exe"
#define ex_pr_nm2 "tail.exe"

		if(strncmp(ex_pr_nm1, pe32.szExeFile, sizeof(ex_pr_nm1))==0)
		{
			continue;
		}

		if(strncmp(ex_pr_nm2, pe32.szExeFile, sizeof(ex_pr_nm2))==0)
		{
			continue;
		}

		// 실행 이미지의 full path 를 구한다. 
		//
		BOOL bRet = GetImageFullPath(pe32.th32ProcessID,  run_proc_info.FullExeFile);		
		if (FALSE == bRet)
		{

			SetErrorString(FALSE, TEXT("%s >> %s "), __FUNCTION__, GetErrorString());
#ifdef _DEBUG
			DebugString(TEXT("%s >> %s\n"), __FUNCTION__, GetErrorString());
#endif
			continue;	// system process 의 경우 createToolHelp32SnapShot 이 실패하는 경우가 있으므로..
		}
		

		//---------------------------------------------------------------------
		// add to vector~
		//
		vectProc->push_back(run_proc_info);
		
		

	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
	return 0;
}

/**	-----------------------------------------------------------------------
	\brief	dwPID 프로세스의 풀 패스를 구하는 함수

	\param	
			dwPID		프로세스 아이디
			szFullPath	MAX_PATH 만큼의 문자열 버퍼
	\return			
	\code
	
	\endcode		
-------------------------------------------------------------------------*/
BOOL GetImageFullPath(DWORD dwPID, LPTSTR szFullPath)
{
	_ASSERTE(NULL != szFullPath);
	_ASSERTE(TRUE != IsBadWritePtr(szFullPath, sizeof(_MAX_PATH)));
	if (NULL == szFullPath)
	{
		SetErrorString(FALSE, TEXT("%s >> null string ptr(2nd param)"), __FUNCTION__);
		return FALSE;
	}
	if (TRUE == IsBadWritePtr(szFullPath, sizeof(_MAX_PATH)))
	{
		SetErrorString(FALSE, TEXT("%s >> invalid string ptr(2nd param - insufficient buf)"), __FUNCTION__);
		return FALSE;
	}

	// pid == 0 은 idle 프로세스 이므로 패스 
	// 
	if (0 == dwPID)
	{
#define SYS_IDLE_PROC_NAME	"system idle process"
		MoveString(szFullPath, TEXT(SYS_IDLE_PROC_NAME), _MAX_PATH);
		return TRUE;
	}



	//HANDLE hToken = NULL;

	MODULEENTRY32 me32 = {0};



	//if ( ISNTFAMILY(GetWinVersion()) )
	//{
	//	// 디버그 권한으로 바꿈
	//	//	
	//	if ( OpenThreadToken(GetCurrentThread(), 
	//		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken)  != TRUE )
	//	{
	//		if (GetLastError() == ERROR_NO_TOKEN)
	//		{
	//			if ( ImpersonateSelf(SecurityImpersonation)	!= TRUE ) 
	//			{
	//				_ASSERTE(!"ImpersonateSelf");
	//				SetErrorString(TRUE, TEXT("%s >> ImpersonateSelf"), __FUNCTION__);
	//				return FALSE;			
	//			}

	//			if ( OpenThreadToken(
	//					GetCurrentThread(),
	//					TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
	//					FALSE,
	//					&hToken) != TRUE)
	//			{
	//				_ASSERTE(!"OpenThreadToken");
	//				SetErrorString(TRUE, TEXT("%s >> OpenThreadToken"), __FUNCTION__);
	//				return FALSE;
	//			}
	//		}
	//		else
	//		{
	//			_ASSERTE(!"OpenThreadToken");
	//			SetErrorString(TRUE, TEXT("%s >> OpenThreadToken"), __FUNCTION__);			
	//			return FALSE;
	//		}
	//	}  
	//	if ( SetPrivilege(hToken, SE_DEBUG_NAME, TRUE) != TRUE )
	//	{
	//		_ASSERTE(!"SetPrivilege");
	//		SetErrorString(FALSE, TEXT("%s >> SetPrivilege"), __FUNCTION__, GetErrorString());		
	//		CloseHandle(hToken);
	//		return FALSE;
	//	}
	//}

	
	// Take a snapshot of all modules in the specified process.
	//
	//	system process 인 경우 실패하는 경우도 있다. 
	// 

	HANDLE hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID );

	if( hModuleSnap == INVALID_HANDLE_VALUE )
	{
		SetErrorString(TRUE, TEXT("%s >> CreateToolhelp32Snapshot"), __FUNCTION__);

		//if (ISNTFAMILY(GetWinVersion()))	
		//{
		//	// disable SeDebugPrivilege
		//	//
		//	if (hToken != NULL)
		//	{
		//		SetPrivilege(hToken, SE_DEBUG_NAME, FALSE);
		//		CloseHandle(hToken);
		//	}
		//}

		return( FALSE );
	}

	// Set the size of the structure before using it.
	//
	me32.dwSize = sizeof( MODULEENTRY32 );

	// Retrieve information about the first module,
	// and exit if unsuccessful
	//
	BOOL bRet = Module32First( hModuleSnap, &me32 );
	_ASSERTE(FALSE != bRet);
	if(FALSE == bRet)
	{
		SetErrorString(TRUE, TEXT("%s >> Module32First"), __FUNCTION__);
		CloseHandle( hModuleSnap );     // Must clean up the snapshot object!

		//if (ISNTFAMILY(GetWinVersion()))	
		//{
		//	// disable SeDebugPrivilege
		//	//
		//	if (hToken != NULL)
		//	{
		//		SetPrivilege(hToken, SE_DEBUG_NAME, FALSE);
		//		CloseHandle(hToken);
		//	}
		//}

		return( FALSE );
	}

	// 맨 처음 모듈이 자기 자신이다. !!!
	//
	MoveString(szFullPath, me32.szExePath, MAX_PATH);	
	CloseHandle( hModuleSnap );
	
	//if (ISNTFAMILY(GetWinVersion()))	
	//{
	//	// disable SeDebugPrivilege
	//	//
	//	if (hToken != NULL)
	//	{
	//		SetPrivilege(hToken, SE_DEBUG_NAME, FALSE);
	//		CloseHandle(hToken);
	//	}
	//}

	return( TRUE );
}


















