// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
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
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#define SNAMPSHOT_CONFIG_FILE _T("SnapShot_Config.ini")

// ���� ���� ����
typedef struct tagProcessInfo
{
	DWORD	dwProcessID;				// ���μ��� ID
	DWORD	dwWorldNo;					// �����ȣ.
    BOOL    is_restore_;                // ���� ���μ��� ����
	TCHAR	strProcessFullName[MAX_PATH];
	tagProcessInfo()
	{
		dwProcessID = 0;
		dwWorldNo = 0;
		memset(strProcessFullName, 0, sizeof(strProcessFullName));
	}
}PROCESSINFO;