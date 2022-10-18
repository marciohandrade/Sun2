// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once


#include <iostream>
#include <tchar.h>
#include <list>

#include <afx.h>
#include <tlhelp32.h>
#include <shellapi.h>

#include <aclapi.h>
#include <accctrl.h>

#include <strsafe.h>


void LOGF(const char* pszFormat,...);

#ifdef _SUN
//#define SNAPSHOT_PROC_NAME _T("SunLogManagerConsole.exe")
#define SNAPSHOT_PROC_NAME _T("SUNRealTimeLogConsole.exe")
#else
#define SNAPSHOT_PROC_NAME _T("")
#endif //_SUN
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#define SNAMPSHOT_CONFIG_FILE _T("SnapShot_Config.ini")

// 게임 서버 정보
typedef struct tagProcessInfo
{
	DWORD	dwProcessID;				// 프로세스 ID
	DWORD	dwWorldNo;					// 월드번호.
    BOOL    is_restore_;                // 복구 프로세스 여부
	TCHAR	strProcessFullName[MAX_PATH];
	tagProcessInfo()
	{
		dwProcessID = 0;
		dwWorldNo = 0;
		memset(strProcessFullName, 0, sizeof(strProcessFullName));
	}
}PROCESSINFO;