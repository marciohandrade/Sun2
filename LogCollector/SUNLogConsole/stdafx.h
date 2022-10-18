/*------------------------------------------------------------------------------
 * stdafx.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#pragma once



#include <iostream>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include <afx.h>
#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.
#include <afxdtctl.h>		// Internet Explorer 4�� ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

//#ifdef _DEBUG
//#pragma  comment(lib, "CrashHandler_vsnetd.lib")
//#else
//#pragma  comment(lib, "CrashHandler_vsnet.lib")
//#endif

#include <strsafe.h>
#include <process.h>

#include <sql.h>
#include <sqltypes.h> 
#include <sqlext.h>

// CHANGES:f101206.2L, prevent a duplicated definition about 'DBCHAR'
#ifdef DBCHAR
    #undef DBCHAR
#endif
#include <odbcss.h>

#pragma comment(lib, "odbc32.lib")

#define MAX_DB_PW					128
#define DEFAULT_WRITE_THREAD_NUM	12
//#define DEFAULT_WRITE_THREAD_NUM	6
#define CONFIG_FILE_NAME	        _T("config.ini")
#define READINI_FILE_NAME           _T("logfile.ini")
#define READ_FILE_PATH				_T("Log")

#define REALTIME_PROC_NAME _T("SUNRealTimeLogConsole.exe")
#define TIMESPAN_PROC_NAME _T("SUNLogConsole.exe")

extern TCHAR g_szCurrentPath[MAX_PATH];

//==================================================================================================
// Version content macros for this project
#define _GUILD_LOG
#define _POINT_LOG
#define _CHAT_USERID
//==================================================================================================

#define _SERVER

#include "Deployment.h"
#include "Define.h"

#include "LogArchive.h"
#include "LogCommonDefine.h"
#include "LogDataStruct.h"
#include "LogTimeFormat.h"
#include "BaseLog.h"
#include "BaseLogData.h"

#include "ActionData.h"
#include "AdminData.h"

#include "ChatData.h"
#include "ExpData.h"
#include "InventoryData.h"
#include "ItemData.h"

#include "AccumulatePointData.h"
#include "MoneyData.h"
#include "SessionData.h"
#include "SkillData.h"
#include "SnapShotData.h"
#include "WareHouseData.h"
#include "WriteLogMng.h"
#include "GuildData.h"

#include "LogClassExtension.h"

#ifdef _DEBUG
    #pragma comment(lib, "../../Server/lib/"VC_DIR_REDIRECT"/SunWriteLogMng_d.lib")
#else
    #pragma comment(lib, "../../Server/lib/"VC_DIR_REDIRECT"/SunWriteLogMng.lib")
#endif

//#define YOUNGMOON_TEST
//#define NAMKA_TEST

// ������ ������
#ifdef _CHINA
    #define _RENAME // �α����� �̸� �տ� ���� ��ȣ ����
    #define USE_SEPERATED_CHANNEL // ä�κ��� �α� �ݷ��͸� ���� �Ҷ�
#endif
//==================================================================================================

// �α� ����
enum eLOG_STATE
{
	LOGSTATE_LOADING=0,			// ���� �ε� ��.. ���� ���������� ���� ������.
	LOGSTATE_END,				// ���� ���������� �ε�.
	LOGSTATE_REALTIME_SAME_OFFSET,			// ���� ��¥ ���� �ε�.
	LOGSTATE_INVALIED_OFFSET,	// �α� ���Ͽ� ī�װ� ������ �� �� ����Ǿ� ����.
	LOGSTATE_INSERT_FAIL,		// DB �Է� ����
	LOGSTATE_NETWORK_LOGON_FAIL,			// ���� ���� ����(��Ʈ��ũ �α׿� ����.)
	LOGSTATE_INVALID_PATH,		// ��θ� ã�� �� ����.
	LOGSTATE_ACCESS_DENY,		// �׼��� �ź�
	LOGSTATE_INVALID_NETWORK_PATH,		// ��Ʈ��ũ ��θ� ã�� �� ����. Sunbackup/fhrm!1 �������� ���� ������ ������ ���� �ʾ���.
	LOGSTATE_NOT_EXIST_FILE,    // ������ �������� �ʴ´�.
	LOGSTATE_UNKNOWN_OPEN_FAIL,	// �� �� ���� ������ ���� ���� ���� - T_ErrorInfo ���̺� ����
	LOGSTATE_FIND_OFFET,
    LOGSTATE_IO_PENDING,        // IO ��ħ
};

// ���� Ÿ��
enum eSERVER_TYPE
{
	UNKNOWN_SERVER 			= 0,
	AGENT_SERVER			= 1,
	FIELD_SERVER			= 2,
	BATTLE_SERVER			= 3,
	GAME_DBPROXY			= 4,
	WORLD_SERVER			= 5,
	MASTER_SERVER			= 6,
	GUILD_SERVER            = 7,
	ACCOUNT_DBPROXY			= 8,
	TOURNAMENT_SERVER		= 9,
	AUTH_AGENT_SERVER		= 10,
	OP_SERVER				= 11,   // WOPS(Webzen Online Portal Solution) Agent Server
	TEMP_SERVER				= 12,
	GAME_SERVER				= 13,	//< FIELD_SERVER�� BATTLE_SERVER�� ��Ī
};

// ���� ���� ����
typedef struct tagServerInfo
{
	ULONGLONG	ulOffset;				// ������
	ULONGLONG	ulBeforeBreakOffset;    // ���� ������.
	int			nServerType;			// ���� Ÿ��
	int			nServerCode;			// ���� �ڵ�
	int			nChannelID;				// ä�� ���̵�
	TCHAR		szServerIP[32];			// ������
	TCHAR		szPrefix[24];			// prefix (AGENT, BATTLE, FIELD, WORLD)
	TCHAR		szFileName[MAX_PATH];	// ���ϸ�
	TCHAR		szPath[MAX_PATH];		// ���� ���
	TCHAR		szFullPath[MAX_PATH];	// IP�� ������ ��ü ���
	TCHAR       szLocalPrefixPath[24];  //
	eLOG_STATE	eLogState;				// �α� ����
	INT			nRepeatCount;
	ULONGLONG	ulFileSize;
	INT			nWorldNo;

	tagServerInfo()
	{
		ulOffset = 0;
		ulBeforeBreakOffset = 0;
		ulFileSize = 0;
		nServerType = 0;
		nServerCode = 0;
		nChannelID = 0;
		nRepeatCount = 0;
		memset(szServerIP, 0, sizeof(szServerIP));
		memset(szPrefix, 0, sizeof(szPrefix));
		memset(szFileName, 0, sizeof(szFileName));
		memset(szPath, 0, sizeof(szPath));
		memset(szFullPath, 0, sizeof(szFullPath));
		memset(szLocalPrefixPath, 0, sizeof(szLocalPrefixPath));
		eLogState = LOGSTATE_LOADING;
		nWorldNo = 0;
	}
}SERVERINFO;



#define USER_WHITE	    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define USER_RED         (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define USER_GREEN       (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define USER_BLUE        (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define USER_YELLOW      (USER_RED | USER_GREEN)
#define USER_PINK        (USER_RED | USER_BLUE)
#define USER_SKYBLUE     (USER_GREEN | USER_BLUE)

#define USER_RED_BG      (BACKGROUND_RED | BACKGROUND_INTENSITY)
#define USER_GREEN_BG    (BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define USER_BLUE_BG     (BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define USER_YELLOW_BG   (USER_RED_BG | USER_GREEN_BG)
#define USER_PINK_BG     (USER_RED_BG | USER_BLUE_BG)
#define USER_SKYBLUE_BG  (USER_GREEN_BG | USER_BLUE_BG)
#define USER_WHITE_BG    (USER_RED_BG | USER_GREEN_BG | USER_BLUE_BG)

void PrintUserColorString(HANDLE hStdOut, WORD Attribute, TCHAR* str);