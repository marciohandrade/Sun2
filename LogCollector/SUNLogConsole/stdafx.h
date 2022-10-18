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
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.
#include <afxdtctl.h>		// Internet Explorer 4의 공용 컨트롤에 대한 MFC 지원입니다.
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 공용 컨트롤에 대한 MFC 지원입니다.
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

// 국가별 디파인
#ifdef _CHINA
    #define _RENAME // 로그파일 이름 앞에 버전 번호 포함
    #define USE_SEPERATED_CHANNEL // 채널별로 로그 콜렉터를 실행 할때
#endif
//==================================================================================================

// 로그 상태
enum eLOG_STATE
{
	LOGSTATE_LOADING=0,			// 파일 로딩 중.. 파일 마지막까지 읽지 못했음.
	LOGSTATE_END,				// 파일 마지막까지 로드.
	LOGSTATE_REALTIME_SAME_OFFSET,			// 다음 날짜 파일 로드.
	LOGSTATE_INVALIED_OFFSET,	// 로그 파일에 카테고리 정보가 잘 못 저장되어 있음.
	LOGSTATE_INSERT_FAIL,		// DB 입력 실패
	LOGSTATE_NETWORK_LOGON_FAIL,			// 파일 오픈 실패(네트워크 로그온 실패.)
	LOGSTATE_INVALID_PATH,		// 경로를 찾을 수 없음.
	LOGSTATE_ACCESS_DENY,		// 액세스 거부
	LOGSTATE_INVALID_NETWORK_PATH,		// 네트워크 경로를 찾을 수 없음. Sunbackup/fhrm!1 계정으로 공유 폴더로 연결을 하지 않았음.
	LOGSTATE_NOT_EXIST_FILE,    // 파일이 존재하지 않는다.
	LOGSTATE_UNKNOWN_OPEN_FAIL,	// 알 수 없는 이유로 파일 오픈 실패 - T_ErrorInfo 테이블 참조
	LOGSTATE_FIND_OFFET,
    LOGSTATE_IO_PENDING,        // IO 겹침
};

// 서버 타입
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
	GAME_SERVER				= 13,	//< FIELD_SERVER와 BATTLE_SERVER를 통칭
};

// 게임 서버 정보
typedef struct tagServerInfo
{
	ULONGLONG	ulOffset;				// 오프셋
	ULONGLONG	ulBeforeBreakOffset;    // 이전 오프셋.
	int			nServerType;			// 서버 타입
	int			nServerCode;			// 서버 코드
	int			nChannelID;				// 채널 아이디
	TCHAR		szServerIP[32];			// 아이피
	TCHAR		szPrefix[24];			// prefix (AGENT, BATTLE, FIELD, WORLD)
	TCHAR		szFileName[MAX_PATH];	// 파일명
	TCHAR		szPath[MAX_PATH];		// 파일 경로
	TCHAR		szFullPath[MAX_PATH];	// IP를 포함한 전체 경로
	TCHAR       szLocalPrefixPath[24];  //
	eLOG_STATE	eLogState;				// 로그 상태
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