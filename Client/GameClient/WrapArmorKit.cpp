//------------------------------------------------------------------------------
//  WrapArmorKit.cpp
//  (C) 2006 ckbang, jjumbo
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/*
    revision histroy:

	- date: 2007-9-19 ��
	- KTW 
	- version : 2.0.0.7

	- date: 2007-9-4 ȭ
	- KTW 
	- version : 2.0.0.6

	- date: 2007-8-9 ��
	- KTW 
	- version : 2.0.0.5

	- date: 2007-7-18 ��
	- KTW 
	- version : 2.0.0.4

	- date: 2007-4-18 ��
	- KTW 
	- version : 2.0.0.3
	- Ŀ�� ����̺� ����

	- date: 2007-4-11 ȭ
	- KTW 
	- version : 2.0.0.2
	- �Ƹ�Ŷ 2.0 ����

	- date: 2007-3-30 ��
	- KTW 
	- version : 2.0.0.1
	- �Ƹ�Ŷ 2.0 ����

	- date: 2007-2-23 ��
	- y2jinc (at build pc)
	- version : 1.3.0.3
	- ���� ����Ʈ ����(AK v2.0�� ȣȯ�� ���)
	- Ž�� �������� ���� ���θ� üũ�ϴ� ��� ����
	- GenerateSid() ���� �α�

	- date: 2007-2-1 ��
	- y2jinc (at build pc)
	- version : 1.3.0.2
	- ������ seed ����ȯ �ϴ� ��� �߰�.( GenerateSid() ) 

	- date: 2007-1-3 ��
	- y2jinc (at build pc)
	- version : 1.3.0.0
	- binary packing ������ ���� ����.

	- date: 2006-12-12 ��
	- y2jinc (at build pc)
	- version : 1.3.0.0
	- binary packing ������ ���� ����.

	- date: 2006-12-06 ��
	- y2jinc (at build pc)
	- version : 1.2.0.6
	- binary packing ���� ��������.

	- date: 2006-11-30 ��
	- y2jinc (at build pc)
	- version : 1.2.0.5
	- �޸𸮸� ����� �κ� ����.

	- date: 2006-11-02 ��
	- y2jinc (at build pc)
	- version : 1.2.0.4
	- Ghost API ���� ����.

	- date: 2006-10-31 ȭ
	- y2jinc (at build pc)
	- version : 1.2.0.3
	- IGhost API �����ϰ�
	- ��ŷ�� ����Ʈ ������Ʈ

	- date: 2006-10-19 ��
	- y2jinc (at build pc)
	- version : 1.2.0.1
	- ��ŷ�� ����Ʈ ������Ʈ

	- date: 2006-09-28 ��
	- y2jinc (at build pc)
	- version : 1.2.0.0
	- ��ŷ�� ����Ʈ ������Ʈ

	- date: 2006-09-14 THU
	- ckbang (at build pc)
	- version : 1.1.0.2
		- ��ŷ�� ����Ʈ ������Ʈ

	- date: 2006-08-31 THU
	- ckbang (at build pc)
	- version : 1.1.0.1
		- ��ŷ�� ����Ʈ ������Ʈ

	- date: 2006-08-23 WED
	- ckbang (at build pc)
	- version : 1.1.0.0
		- ��ŷ�� ����Ʈ ������Ʈ

	- date: 2006-08-16 WED
	- ckbang (at build pc)
	- version : 1.0.4.5
		- ��ŷ�� ����Ʈ ������Ʈ

	- date: 2006-08-09 WED
	- ckbang (at build pc)
	- version : 1.0.4.4
		- ��ŷ�� ����Ʈ ������Ʈ
		- ��������� �߰������ ���� �������̽� �߰� (setpatternlist, �����ڵ� �̱���)

	- date: 2006-08-02 WED
	- ckbang
	- version : 1.0.4.3
	  - ��ŷ ����Ʈ Ÿ�� �߰�2��

	- date: 2006-07-20 THU
	- ckbang
	- version : 1.0.4.2
	  - ��ŷ�� ����Ʈ ������Ʈ

	- date: 2006-07-12 WED
	- jjumbo
	- version : 1.0.4.1
	  - ��ŷ�� ����Ʈ ������Ʈ

    - date: 2006-07-06 THU
    - ckbang
    - version: 1.0.4.0
      - ms crypto library �ʱ�ȭ�� Űȹ�� ���п� ���� ����ó�� �߰�
      - �����Ű���� ������ ���ϸ� �ӽ�Ű������ ��ü�ϰ� ����

    - date: 2006-06-28 WED
    - ckbang
    - version: 1.0.2.2
      - �������콺 1.4.5 2006-06-27 Ez_Sun.exe : Auto Mouse, Keyboard
      - �׽��� 1.04 2006-06-27 games.exe : Auto Mouse, Keyboard (�׽����� �ڵ� ������Ʈ ������)
      - �÷����� 1.17 2006-06-27 plus_sun.exe : Auto Mouse, Keyboard (�ڵ� ������Ʈ ��� ������)

*/




#include "SunClientPrecompiledHeader.h"


#	include <CryptManager_4Client.h>


enum ARMORKIT_ZONE_TYPE 
{	
	LIVE_ZONE_TYPE	= 1,
	DEV_ZONE_TYPE	= 2,
	INTRA_ZONE_TYPE	= 3,
};

#ifdef _CK_IGNORE_ARMOKIT

bool ArmorKit_Init(HWND hWnd, HINSTANCE hInst)
{
	CryptManager::InitInfo( NULL );

	return true;
}
void ArmorKit_Update()
{
}
bool ArmorKit_Suspended()
{
	return true;
}
void ArmorKit_Shut()
{
}
#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
INT ArmorKit_SetPatternList(char *list, int length)
#else
INT ArmorKit_SetPatternList(TCHAR *list, int length)
#endif // TEMP_WORK_ARMORKIT_CONVERT_ASCII
{
	return -1;
}

INT ArmorKit_GenerateSid(int seed, char *sid )
{
	return -1;
}

#else


#ifdef _KTW_ARMORKIT_2
//-----------------------------------------------------------------------------
//	��Ƽ�� ����(Front Sever IP)�� �����մϴ�.
//
/*
#define SUN_KR_SERVER						100
#define	SUN_KR_DEV							(SUN_KR_SERVER + 1)		//	���߸� �׽�Ʈ ����
#define	SUN_KR_INTRA						(SUN_KR_SERVER + 2)		//	�系�� �׽�Ʈ ����
#define	SUN_KR_LIVE							(SUN_KR_SERVER + 3)		//	���̺� ����

#ifdef __CHINA_OPEN_BETA_SERVICE
// For Sun Client China Version
#define SUN_CN_PART							200
#define SUN_CN_IBT							(SUN_CN_PART)			//	172.18.235.17
#define SUN_CN_PART_01						(SUN_CN_PART + 1)		//	cn1.sun.the9.com
#define SUN_CN_PART_02						(SUN_CN_PART + 2)		//	cn2.sun.the9.com
#define SUN_CN_PART_03						(SUN_CN_PART + 3)		//	cn3.sun.the9.com
#define SUN_CN_PART_04						(SUN_CN_PART + 4)		//	cn4.sun.the9.com
#define SUN_CN_PART_05						(SUN_CN_PART + 5)		//	cn5.sun.the9.com
#define SUN_CN_PART_06						(SUN_CN_PART + 6)		//	cn6.sun.the9.com
#endif	//	__CHINA_OPEN_BETA_SERVICE
*/

#define SUN_CN_PART_MAX_COUNT	7
const TCHAR c_tsSvr[SUN_CN_PART_MAX_COUNT][MAX_PATH+1] =
{ 
_T("172.18.235.17"),
_T("cn1.sun.the9.com"),
_T("cn2.sun.the9.com"),
_T("cn3.sun.the9.com"),
_T("cn4.sun.the9.com"),
_T("cn5.sun.the9.com"),
_T("cn6.sun.the9.com"),
};

//-----------------------------------------------------------------------------
// ArmorKit v2 �� ����ϱ� ���� ��������� �߰��մϴ�.
//
#include "AKLib.h"
#include "typedef_AKv1HackDetect.h"
#include "servers_sun.inc"

//-----------------------------------------------------------------------------
// Armorkit DLL ���� ������ ��� DLL ����� �߰��մϴ�.
//
#ifdef _AKV2_DLL
#include "AKLib_dll.h"
#endif//_AKV2_DLL

#else	//	_KTW_ARMORKIT_2

// for armorkit
#ifdef _AK_DLL_
#include "ArmorKitDll.h"
#else
#include "CArmorKit.h"
#endif
#include "AKError.h"
#include "WrapArmorKit.h"
#endif	//	_KTW_ARMORKIT_2

// for sun client
#include "interfacemanager.h"
#include "globalfunc.h"
#include "Resource.h"

// prototypes
INT SunArmorKitCallBackProc (PHACK_DETECT_INFO pHDI);
LRESULT CALLBACK SunArmorKitAlertWndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SunArmorKitAlertWndProcSub(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL SunArmorKitAlertInitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM SunArmorKitAlertMyRegisterClass(HINSTANCE hInstance);
BOOL SunArmorKitAlertInit(HINSTANCE hInstance, int nCmdShow);

class CArmorKit_Interface;
class CArmorKit_Check;
class CArmorKit_Detect;
class CArmorKit_Report;

typedef struct SUNCLIENT_HACK_DETECT_INFO
{
    INT		HackType;		// Ž���� ��ŷ Ÿ��  (e.g. AKDETECT_SPEEDHACK)
    INT		Size;			// Buffer �� ����
    TCHAR	Buf[1024];		// ������ ����-1024�� ������Ʈ���� ���޹��� �Ϲ��� �ִ밪(����â)
} SUNCLIENT_HACK_DETECT_INFO;

#ifdef _KTW_ARMORKIT_2

// [ STEP 1 ]
//	- AKLib ��ü �ν��Ͻ� ����
//
#ifdef _AKV2_DLL
RSCM_IAKLib_dll pArmorKit;
#pragma comment(lib, "../../lib/AKv2_dll.lib")
#else	//	_AKV2_DLL
RSCM_IAKLib pArmorKit;		
#ifndef	_JAPAN
#pragma comment(lib, "../../lib/AKv2.lib")
#endif
#endif	//	_AKV2_DLL

#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
const char          c_tcARMORKIT_VERSION[] = "2.0.0.7";     // Now, we don't have external communicatable armorKit's integrity.
#else
const TCHAR         c_tcARMORKIT_VERSION[] = _T("2.0.0.7"); 
#endif // TEMP_WORK_ARMORKIT_CONVERT_ASCII
// must hand-write because of Updater is know that armorkit's version correctly.

IAKLib* AKv2 = pArmorKit.get();

#else	//	_KTW_ARMORKIT_2

#ifdef _AK_DLL_

// const
#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
const char             c_tcARMORKIT_VERSION[] = "1.3.0.4";     // Now, we don't have external communicatable armorKit's integrity.
#else
const TCHAR             c_tcARMORKIT_VERSION[] = _T("1.3.0.4"); 
#endif // TEMP_WORK_ARMORKIT_CONVERT_ASCII
// must hand-write because of Updater is know that armorkit's version correctly.

#pragma comment(lib, "../../lib/ArmorKitDll.lib")

CArmorKitDll	cAK;


#else

// const
#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
const char             c_tcARMORKIT_VERSION[] = "1.3.0.4";     // Now, we don't have external communicatable armorKit's integrity.
#else
const TCHAR             c_tcARMORKIT_VERSION[] = _T("1.3.0.4");
#endif // TEMP_WORK_ARMORKIT_CONVERT_ASCII
// must hand-write because of Updater is know that armorkit's version correctly.
// must hand-write because of Updater is know that armorkit's version correctly.

#pragma comment(lib, "../../lib/ArmorKit.lib")


CArmorKit		cAK;

#endif

#endif	//	_KTW_ARMORKIT_2

#if defined(_USER_RELEASE_)
#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
    const char             c_tcARMORKIT_LAUNCHERNAME[] = "SunGame.exe";
#else
	const TCHAR             c_tcARMORKIT_LAUNCHERNAME[] = _T("SunGame.exe");
#endif //TEMP_WORK_ARMORKIT_CONVERT_ASCII
    const bool              c_bARMORKIT_TRACE_LOG       = false;

#elif defined(_PERF_DEBUG)
#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
    const char             c_tcARMORKIT_LAUNCHERNAME[] = "GameClient_Perf_Debug.exe";
#else
	const TCHAR             c_tcARMORKIT_LAUNCHERNAME[] = _T("GameClient_Perf_Debug.exe");
#endif //TEMP_WORK_ARMORKIT_CONVERT_ASCII
    const bool              c_bARMORKIT_TRACE_LOG       = true;

#elif defined(_DEBUG)
#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
    const char             c_tcARMORKIT_LAUNCHERNAME[] = "GameClient_Debug.exe";
#else
	const TCHAR             c_tcARMORKIT_LAUNCHERNAME[] = _T("GameClient_Debug.exe");
#endif //const char             c_tcARMORKIT_LAUNCHERNAME[] = "GameClient_Perf_Debug.exe";
    const bool              c_bARMORKIT_TRACE_LOG       = true;

#else

#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
    const char             c_tcARMORKIT_LAUNCHERNAME[] = "GameClient.exe";
#else
	const TCHAR             c_tcARMORKIT_LAUNCHERNAME[] = _T("GameClient.exe");
#endif //TEMP_WORK_ARMORKIT_CONVERT_ASCII
    const bool              c_bARMORKIT_TRACE_LOG       = true;

#endif

const DWORD             c_dwARMORKIT_EXIT_SEC = 10;
const DWORD             c_dwARMORKIT_ACTION_CHECK_CYCLE =   10 * SEC_PER_TICK;  // 10sec
const DWORD             c_dwARMORKIT_STATINGBY_SHUTDOWN =   c_dwARMORKIT_EXIT_SEC * SEC_PER_TICK;  // 10sec

DWORD                   g_dwForcedExitStartTick = 0;

// enums
enum
{
    eARMORKIT_REPORT_NOTICE=0,
    eARMORKIT_REPORT_ALERT,
    eARMORKIT_REPORT_SILENCE_ALERT, // just to do report to WOPS center
    eARMORKIT_REPORT_MAX,
};

//------------------------------------------------------------------------------
/**
    @class cArmorKit_Report

    hack report to WOPS center

    (C) 2005 ckbang
*/
class CArmorKit_Report
{
public:
    CArmorKit_Report() {m_iReportType = eARMORKIT_REPORT_MAX;}

    inline void SetStartTick()
    {
        m_dwStartTick = GetTickCount();
    }

    inline DWORD GetStartTick()
    {
        return m_dwStartTick;
    }

    inline void SetInfo(PHACK_DETECT_INFO pHDI)
    {
        m_hackInfo.HackType = pHDI->HackType;
        m_hackInfo.Size     = pHDI->Size;
        ZeroMemory (&m_hackInfo.Buf, 1024);
#ifdef _KTW_ARMORKIT_2
		if( pHDI->Size )
			memcpy ((TCHAR *)&m_hackInfo.Buf, (TCHAR *)pHDI->TypedBuf, pHDI->Size);
#else	//	_KTW_ARMORKIT_2
        if( pHDI->Size )
            memcpy ((TCHAR *)&m_hackInfo.Buf, (TCHAR *)pHDI->Buf, pHDI->Size);
#endif	//	_KTW_ARMORKIT_2
    }

	inline const SUNCLIENT_HACK_DETECT_INFO & GetInfo()
    {
        return m_hackInfo;
    }

    inline void SetReportType(BYTE byReportType)
    {
        m_iReportType = byReportType;
    }

    inline BYTE GetReportType()
    {
        return m_iReportType;
    }

    inline void SetHeadline(const util::_tstring& strHead)
    {
        m_strHead = strHead;
    }

    inline const util::_tstring&    GetHeadline()
    {
        return m_strHead;
    }

    inline void SetContents(const util::_tstring& strContents)
    {
        m_strContents = strContents;
    }

    inline const util::_tstring&    GetContents()
    {
        return m_strContents;
    }

    void OnShow();

    CArmorKit_Report& operator=(const CArmorKit_Report& rhs);

private:
    INT                 m_iReportType;
    SUNCLIENT_HACK_DETECT_INFO m_hackInfo;

    util::_tstring      m_strHead;
    util::_tstring      m_strContents;

    DWORD               m_dwStartTick;
};

//------------------------------------------------------------------------------
/**
*/
void CArmorKit_Report::OnShow()
{
}

//------------------------------------------------------------------------------
/**
*/
inline
CArmorKit_Report&
CArmorKit_Report::operator=(const CArmorKit_Report& rhs)
{
    m_hackInfo.HackType = rhs.m_hackInfo.HackType;
    m_hackInfo.Size = rhs.m_hackInfo.Size;
    ZeroMemory (&m_hackInfo.Buf, 1024);
    if( rhs.m_hackInfo.Size )
        memcpy ((TCHAR *)&m_hackInfo.Buf, (TCHAR *)&rhs.m_hackInfo.Buf, rhs.m_hackInfo.Size);

    this->m_iReportType         = rhs.m_iReportType;
    this->m_strHead             = rhs.m_strHead;
    this->m_strContents         = rhs.m_strContents;
    this->m_dwStartTick         = rhs.m_dwStartTick;

    return *this;
}

//------------------------------------------------------------------------------
/**
    @class cArmorKit_Interface

    ArmorKit Library Interface

    (C) 2005 ckbang
*/
class CArmorKit_Interface
{
public:
    CArmorKit_Interface() :
      m_bAllowReportDuplicate(false),   // to allow report duplicate
      m_bHackDetected(false),           // detected flag
      m_bStandingByShutdown(false)
    {}

    bool Init(HWND hWnd, HINSTANCE hInstance);

    inline void SetStartTick()
    {
        m_dwStartTick = GetTickCount();
    }

    void insertReport(const CArmorKit_Report& report);
    bool sendReports();
    bool checkArmorKit_KeepAlive();
    bool standingShutdown();
    
    inline bool isHackDetected()
    {
        return m_bHackDetected;
    }

    inline bool isStandingByShutdown()
    {
        return m_bStandingByShutdown;
    }

    inline HWND GetHWND() { return m_hWnd; }
    inline HINSTANCE GetHINSTANCE() { return m_hInstance; }

    inline const CArmorKit_Report&     GetLatestReport()
    {
        return m_latestReport;
    }

private:
    bool                                m_bAllowReportDuplicate;
    bool                                m_bHackDetected;
    bool                                m_bStandingByShutdown;  // was reported for waiting shutdown.

    DWORD                               m_dwStartTick;
    std::vector<CArmorKit_Report>       m_vReports;
    HWND                                m_hWnd;
    HINSTANCE                           m_hInstance;

    CArmorKit_Report                    m_latestReport;
};

//------------------------------------------------------------------------------
/**
*/
bool CArmorKit_Interface::Init(HWND hWnd, HINSTANCE hInstance)
{
    m_hWnd = hWnd;
    m_hInstance = hInstance;

#ifdef _KTW_ARMORKIT_2
	// [ STEP 2] 
	//	- AKLib ���� �÷��� ����
	// 
#ifdef _DEBUG
	DWORD svc_flag = AKFLAG_FOR_SUN_DBG;
#else
	DWORD svc_flag = AKFLAG_FOR_SUN;
#endif//_DEBUG

//	svc_flag = svc_flag & ~AKUSE_VIRGINE_SVC;
//////////////////////////////////////////////////////////////////////////
// �Ƹ�Ŷ ������Ʈ ���� IP
	DWORD			c_dwSvcIp;
#ifdef _CHINA

#else	//	__CHINA_OPEN_BETA_SERVICE

#ifdef _USER_RELEASE_
	if(StrnCmp(_T("10.1.47.6"), LOGINPARAM->GetLoginServerIP(), MAX_PATH) == NULL)
		c_dwSvcIp = SUN_KR_INTRA;
	else
		c_dwSvcIp = SUN_KR_LIVE;
#else	//	_USER_RELEASE_
		c_dwSvcIp = SUN_KR_DEV;
#endif	//	_USER_RELEASE_

#endif	//	__CHINA_OPEN_BETA_SERVICE
//////////////////////////////////////////////////////////////////////////

#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
	if (AKv2->preStart(svc_flag, SunArmorKitCallBackProc, const_cast<char *>(c_tcARMORKIT_VERSION), c_dwSvcIp) != 0)
#else	//	TEMP_WORK_ARMORKIT_CONVERT_ASCII
	if (AKv2->preStart(svc_flag, SunArmorKitCallBackProc, const_cast<TCHAR *>(c_tcARMORKIT_VERSION), c_dwSvcIp) != 0)
#endif	//	TEMP_WORK_ARMORKIT_CONVERT_ASCII
	{
		assert (!"ArmorKit failed to initialize");
#ifndef _USER_RELEASE_
		MessageBox (NULL, _T("ArmorKit failed to initialize"), "ArmorKit Failed", 0);
#endif	//	_USER_RELEASE_
		return false;
	}

#else	//	_KTW_ARMORKIT_2

#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
    if (cAK.Init(SunArmorKitCallBackProc, const_cast<char *>(c_tcARMORKIT_VERSION)) != AK_SUCCESS)
#else //TEMP_WORK_ARMORKIT_CONVERT_ASCII
	if (cAK.Init(SunArmorKitCallBackProc, const_cast<TCHAR *>(c_tcARMORKIT_VERSION)) != AK_SUCCESS)
#endif //TEMP_WORK_ARMORKIT_CONVERT_ASCII
    {
		//TCHAR szError[512];
		//sprintf( szError, "initiallizeing ArmorKit failed : %s\n", GetErrorString() );
		//assert( szError );
        assert (!"ArmorKit failed to initialize");
		
		//MessageBox (NULL, _T("ArmorKit version mismatched"), "ArmorKit Failed", 0);
        return false;
    }

#endif	//	_KTW_ARMORKIT_2
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void CArmorKit_Interface::insertReport(const CArmorKit_Report& report)
{
    CArmorKit_Report _report = report;
    
    if (m_bAllowReportDuplicate)
    {
        _report.SetStartTick();
        m_vReports.push_back(_report);
    }
    else
    {
        bool isHave = false;

        for (int i=0; i<m_vReports.size(); ++i)
        {
            if ( m_vReports[i].GetInfo().HackType == _report.GetInfo().HackType )
            {
                isHave = true;
                break;
            }
        }

        if (isHave == false)
        {
            _report.SetStartTick();
            m_vReports.push_back(_report);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
bool CArmorKit_Interface::sendReports()
{
    for (int i=0; i<m_vReports.size(); ++i)
    {
        if (m_vReports[i].GetReportType() != eARMORKIT_REPORT_NOTICE)
            m_bHackDetected = true;
    }

	for (int i=0; i<m_vReports.size(); ++i)
    {
        MSG_CG_ARMORKIT_FINDHACK_SYN    sync;
        
        sync.HackType = m_vReports[i].GetInfo().HackType;
        sync.Size = m_vReports[i].GetInfo().Size;
        ZeroMemory (&sync.Buf, 1024);
        if( m_vReports[i].GetInfo().Size )
            memcpy ((TCHAR *)&sync.Buf, (TCHAR *)&m_vReports[i].GetInfo().Buf, m_vReports[i].GetInfo().Size);

		GlobalFunc::SendPacketToGameServer(&sync, sizeof(sync));

        if (m_bHackDetected)
        {
            if (m_vReports[i].GetReportType() != eARMORKIT_REPORT_NOTICE)
                m_latestReport = m_vReports[i];
        }
        else
        {
            m_latestReport = m_vReports[i];
        }
    }

    m_vReports.clear();

    return (!m_bHackDetected);
}

//------------------------------------------------------------------------------
/**
*/
bool CArmorKit_Interface::checkArmorKit_KeepAlive()
{
    DWORD currentTick = GetTickCount();
    DWORD deltaTick = (currentTick - m_dwStartTick);

	if (deltaTick > (10 * SEC_PER_TICK))
    {
        // reset
        this->SetStartTick();
#ifdef _KTW_ARMORKIT_2
		if(AKv2->Start() != 0)
#else	//	_KTW_ARMORKIT_2
        // check keepalive
        if (cAK.Start() != AK_SUCCESS)
#endif	//	_KTW_ARMORKIT_2
        {
            m_bHackDetected = true;
            m_latestReport.SetHeadline(_T("��ŷ����Ʈ��� Ž���Ǿ����ϴ�.\n"));
            m_latestReport.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
            m_latestReport.SetStartTick();
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool CArmorKit_Interface::standingShutdown()
{
    assert (this->isHackDetected());

    DWORD currentTick = GetTickCount();
    DWORD deltaTick = (currentTick - m_latestReport.GetStartTick());

    if (deltaTick > c_dwARMORKIT_STATINGBY_SHUTDOWN)
        return true;

    return false;
}

// global
CArmorKit_Interface     cAK_Interface;


//------------------------------------------------------------------------------
/**
*/
#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
INT	 ArmorKit_SetPatternList(char *list, int length)
#else //TEMP_WORK_ARMORKIT_CONVERT_ASCII
INT	 ArmorKit_SetPatternList(TCHAR *list, int length)
#endif //TEMP_WORK_ARMORKIT_CONVERT_ASCII
{
#ifdef _KTW_ARMORKIT_2
	return AKv2->SUN_SetFileHashList(list, length);
#else	//	_KTW_ARMORKIT_2
	return cAK.SetPatternList(list, length);
#endif	//	_KTW_ARMORKIT_2
}


INT ArmorKit_GenerateSid(int seed, char *sid )
{
#ifdef _KTW_ARMORKIT_2
	return AKv2->SUN_GenerateSID(seed, sid, MSG_CG_ARMORKIT_SEED_TEST_ACK::eARMORKIT_AKS_KEYLENGTH);
#else	//	_KTW_ARMORKIT_2
	return cAK.GenerateSid(seed, sid);
#endif	//	_KTW_ARMORKIT_2
}

//------------------------------------------------------------------------------
/**
*/
bool ArmorKit_Init(HWND hWnd, HINSTANCE hInst)
{
    if (!cAK_Interface.Init(hWnd, hInst))
        return false;

#ifdef _KTW_ARMORKIT_2
	int ret = AKv2->Start();
	if ( ret != 0)
	{
		assert (!"ArmorKit failed to start");
#ifndef _USER_RELEASE_
		MessageBox (NULL, _T("ArmorKit failed to start"), "ArmorKit Failed", 0);
#endif	//	_USER_RELEASE_
		return false;
	}


	CryptManager::InitInfo( &AKv2 );


#else	//	_KTW_ARMORKIT_2

#ifndef _AK_DLL_
#else
#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
    cAK.SetLauncherName( const_cast<char *>(c_tcARMORKIT_LAUNCHERNAME) );
#else //TEMP_WORK_ARMORKIT_CONVERT_ASCII
	cAK.SetLauncherName( const_cast<TCHAR *>(c_tcARMORKIT_LAUNCHERNAME) );
#endif //TEMP_WORK_ARMORKIT_CONVERT_ASCII
#endif

    // action config
    // ckFIXME: just security rules are here i think
    DWORD dwActionflags = AKCHECK_ALL;

#if defined (_DEBUG)    // only exception for debugging purpose
    dwActionflags = 0;
#endif 

    cAK.SetActionFlags (dwActionflags);
    cAK.SetActionMode (AKMODE_TIMER, c_dwARMORKIT_ACTION_CHECK_CYCLE);

    // trace log
    cAK.SetUseFileLog(c_bARMORKIT_TRACE_LOG);

    if (cAK.Start() != AK_SUCCESS)
    {
        assert (!"ArmorKit failed to start");
        return false;
    }


	CryptManager::InitInfo( &cAK );


#endif	//	_KTW_ARMORKIT_2

    cAK_Interface.SetStartTick();

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool ArmorKit_Suspended()
{
    if (cAK_Interface.isHackDetected())
    {
        // system init
        SunArmorKitAlertInit(cAK_Interface.GetHINSTANCE(), 0);

        // open system check dialog
        DialogBox(cAK_Interface.GetHINSTANCE(), (LPCTSTR)IDD_ARMORKIT, NULL, (DLGPROC)SunArmorKitAlertWndProcMain);

        // sys check dialog's message dispatch/translate
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) 
        {
            if (g_dwForcedExitStartTick)
            {
                DWORD currentTick = GetTickCount();
                DWORD deltaTick = (currentTick - g_dwForcedExitStartTick);
                if (deltaTick > (10 * SEC_PER_TICK))
                {
                    SendMessage(g_hWndMain, WM_COMMAND, IDOK, NULL);
                    g_dwForcedExitStartTick = 0;
                }
            }
            
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void ArmorKit_Shut()
{
#ifdef _KTW_ARMORKIT_2
	AKv2->Stop();
#else	//	_KTW_ARMORKIT_2
    cAK.Release();
#endif	//	_KTW_ARMORKIT_2
}

//------------------------------------------------------------------------------
/**
*/
void ArmorKit_Update()
{
    if (cAK_Interface.isHackDetected())
    {
        if (cAK_Interface.standingShutdown())
        {
            GlobalFunc::LogOutGame();
        }            

        return;
    }

	if(!cAK_Interface.sendReports())
	{
		if (!ArmorKit_Suspended())
		{
			ArmorKit_Shut();
			exit (0);
		}
	}

//    cAK_Interface.checkArmorKit_KeepAlive();
}

//------------------------------------------------------------------------------
INT SunArmorKitCallBackProc( PHACK_DETECT_INFO pHDI )
{
    if (cAK_Interface.isHackDetected())
        return 0;

    INT iDetect = pHDI->HackType;

    if ( iDetect < 0 ) {
        // ckFIXME: ignored
        return 0;
    }

    CArmorKit_Report    _report;

#ifdef _KTW_ARMORKIT_2

	switch( iDetect )
	{

	case V1_AKDETECT_SPEEDHACK:
		{
			_report.SetHeadline(_T("���ǵ����� Ž���Ǿ����ϴ�.\n"));
			_report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
			_report.SetInfo(pHDI);
			_report.SetReportType(eARMORKIT_REPORT_ALERT);
			cAK_Interface.insertReport(_report);

			// ckFIXME: to be Debug message?
			//PSPEED_HACK_CHECK_INFO pSHCI = (PSPEED_HACK_CHECK_INFO) pHDI->Buf;
			//printf(_T("timeGetTime(): %ld, GetTickCount(): %ld, QueryPerformanceCounter(): %ld, RDTSC: %ld    \n"), pSHCI->dwTGT, pSHCI->dwGTC , pSHCI->dwQPC, pSHCI->dwTSC);
			break;
		}
	case V1_AKDETECT_MALWARE :
		{
			_report.SetHeadline(_T("��ŷ����Ʈ��� Ž���Ǿ����ϴ�.\n���� �Ǵ� ���α׷��� ������ ���� �� �ٽ� �����ϼ���.\n"));
			_report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
			_report.SetInfo(pHDI);
			_report.SetReportType(eARMORKIT_REPORT_ALERT);
			cAK_Interface.insertReport(_report);

			//PPROCESS_SCAN_INFO pPSI = (PPROCESS_SCAN_INFO) pHDI->Buf;
			//printf(_T("PID: %ld, PRCSNAME: %s, SIGNAME: %s\n"), pPSI->ProcessId, pPSI->ProcessName, pPSI->SignatureName);         
			break;
		}
	case V1_AKDETECT_ERROR:
		{
			_report.SetHeadline(_T("�ý��� ������ �߻��߽��ϴ�.\n"));
			_report.SetContents(_T("�ý����� ���̷����� �Ǽ��ڵ忡 ���� �����Ǿ� ���� �� �ֽ��ϴ�.\n���̷����� �Ǽ��ڵ带 �����Ͻð� �ٽ� ������ �ֽʽÿ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
			_report.SetInfo(pHDI);
			_report.SetReportType(eARMORKIT_REPORT_ALERT);
			cAK_Interface.insertReport(_report);

			//PPE_CHECKSUM_INFO pPCI = (PPE_CHECKSUM_INFO) pHDI->Buf;
			//printf(_T("Embeded check sum: %ld, calculated check sum: %ld\n"), pPCI->HeaderSum, pPCI->CalcSum);
			break;
		}
		/*		
		case V1_AKDETECT_HOOK_DETECT:
		{
		_report.SetHeadline(_T("�������� API �ּҺ����� Ž���Ǿ����ϴ�.\n"));
		_report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
		_report.SetInfo(pHDI);
		_report.SetReportType(eARMORKIT_REPORT_ALERT);
		cAK_Interface.insertReport(_report);
		break;
		}
		*/
	case V1_AKDETECT_DEBUG:
		{
			_report.SetHeadline(_T("���μ����� ������� �����Ǿ����ϴ�.\n"));
			_report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
			_report.SetInfo(pHDI);
			_report.SetReportType(eARMORKIT_REPORT_ALERT);
			cAK_Interface.insertReport(_report);
			break;
		}

	case V1_AKDETECT_RUNCODE:
		{
			_report.SetHeadline(_T("���μ����� �ڵ尡 �����Ǿ����ϴ�.\n"));
			_report.SetContents(_T("�ý����� ���̷����� �Ǽ��ڵ忡 ���� �����Ǿ� ���� �� �ֽ��ϴ�.\n���̷����� �Ǽ��ڵ带 �����Ͻð� �ٽ� ������ �ֽʽÿ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
			_report.SetInfo(pHDI);
			_report.SetReportType(eARMORKIT_REPORT_ALERT);
			cAK_Interface.insertReport(_report);
			break;
		}

	case V1_AKDETECT_HOOK_RESTORE_FAIL:
		{
			_report.SetHeadline(_T("�ý����� �����ϴµ� �����Ͽ����ϴ�.\n"));
			_report.SetContents(_T("�ý����� ���̷����� �Ǽ��ڵ忡 ���� �����Ǿ� ���� �� �ֽ��ϴ�.\n���̷����� �Ǽ��ڵ带 �����Ͻð� �ٽ� ������ �ֽʽÿ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
			_report.SetInfo(pHDI);
			_report.SetReportType(eARMORKIT_REPORT_ALERT);
			cAK_Interface.insertReport(_report);
			break;
		}

	default:
		{
			_report.SetHeadline(_T("�ý��ۿ��� ���ǵ��� ���� ������ �߰ߵǾ����ϴ�.\n"));
			_report.SetContents(_T("�ý����� ���̷����� �Ǽ��ڵ忡 ���� �����Ǿ� ���� �� �ֽ��ϴ�.\n���̷����� �Ǽ��ڵ带 �����Ͻð� �ٽ� ������ �ֽʽÿ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
			_report.SetInfo(pHDI);
			_report.SetReportType(eARMORKIT_REPORT_ALERT);
			cAK_Interface.insertReport(_report);
			break ;
		}
	}

	
#else	//	_KTW_ARMORKIT_2

    switch( iDetect )
    {

        case AKDETECT_SPEEDHACK:
        {
            _report.SetHeadline(_T("���ǵ����� Ž���Ǿ����ϴ�.\n"));
            _report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
            _report.SetInfo(pHDI);
            _report.SetReportType(eARMORKIT_REPORT_ALERT);
            cAK_Interface.insertReport(_report);

            // ckFIXME: to be Debug message?
            //PSPEED_HACK_CHECK_INFO pSHCI = (PSPEED_HACK_CHECK_INFO) pHDI->Buf;
            //printf(_T("timeGetTime(): %ld, GetTickCount(): %ld, QueryPerformanceCounter(): %ld, RDTSC: %ld    \n"), pSHCI->dwTGT, pSHCI->dwGTC , pSHCI->dwQPC, pSHCI->dwTSC);
            break;
        }
        case AKDETECT_MALWARE :
        {
            _report.SetHeadline(_T("��ŷ����Ʈ��� Ž���Ǿ����ϴ�.\n���� �Ǵ� ���α׷��� ������ ���� �� �ٽ� �����ϼ���.\n"));
            _report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
            _report.SetInfo(pHDI);
            _report.SetReportType(eARMORKIT_REPORT_ALERT);
            cAK_Interface.insertReport(_report);

            //PPROCESS_SCAN_INFO pPSI = (PPROCESS_SCAN_INFO) pHDI->Buf;
            //printf(_T("PID: %ld, PRCSNAME: %s, SIGNAME: %s\n"), pPSI->ProcessId, pPSI->ProcessName, pPSI->SignatureName);         
            break;
        }
        case AKDETECT_PEINTEGRITY :
        {
            _report.SetHeadline(_T("���Ӽ���Ʈ������ ������ Ž���Ǿ����ϴ�.\n"));
            _report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
            _report.SetInfo(pHDI);
            _report.SetReportType(eARMORKIT_REPORT_ALERT);
            cAK_Interface.insertReport(_report);
            
            //PPE_CHECKSUM_INFO pPCI = (PPE_CHECKSUM_INFO) pHDI->Buf;
            //printf(_T("Embeded check sum: %ld, calculated check sum: %ld\n"), pPCI->HeaderSum, pPCI->CalcSum);
            break;
        }
		case AKDETECT_HOOK_DETECT:
		{
            _report.SetHeadline(_T("�������� API �ּҺ����� Ž���Ǿ����ϴ�.\n"));
            _report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
            _report.SetInfo(pHDI);
            _report.SetReportType(eARMORKIT_REPORT_NOTICE);
            cAK_Interface.insertReport(_report);
			break;
		}
		case AKDETECT_HOOK_RESTORE_FAIL:
		{
            _report.SetHeadline(_T("�������� API �ּҺ����� Ž���Ǿ����ϴ�.\n"));
            _report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
            _report.SetInfo(pHDI);
            _report.SetReportType(eARMORKIT_REPORT_ALERT);
            cAK_Interface.insertReport(_report);
			break;
		}
        case AKDETECT_APIHOOK:
        {
            _report.SetHeadline(_T("�������� API �ּҺ����� Ž���Ǿ����ϴ�.\n"));
            _report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
            _report.SetInfo(pHDI);
            _report.SetReportType(eARMORKIT_REPORT_ALERT);
            cAK_Interface.insertReport(_report);

            //PHOOKED_API_INFO pHAI = (PHOOKED_API_INFO)pHDI->Buf;
            //switch(pHAI->API_TYPE)
            //{
            //  case AT_EXPORTED:
            //  {
            //      printf(_T("EAT hook ==  \n"));
            //      printf(_T("%s.%s    \n"), pHAI->API_INFO.HEA.DLLName, pHAI->API_INFO.HEA.ApiName);                  
            //      switch (pHAI->API_INFO.HEA.HOOKED_CODE)
            //      {
            //      case AH_EAT_MODIFIED:
            //          {
            //              printf(_T("AH_EAT_MODIFIED  \n"));                          
            //              break;
            //          }
            //      case AH_EAT_INLINE_HOOKED:
            //          {
            //              printf(_T("AH_EAT_INLINE_HOOKED \n"));
            //              break;
            //          }
            //      case AH_EAT_COUNT_ERROR:
            //          {
            //              printf(_T("AH_EAT_COUNT_ERROR   \n"));
            //              break;
            //          }
            //      case AH_EAT_COUNT_ZERO:
            //          {
            //              printf(_T("AH_EAT_COUNT_ZERO    \n"));
            //              break;
            //          }
            //      default:
            //          {
            //              printf(_T("oops! unknown hook code !! %d    \n"), pHAI->API_INFO.HEA.HOOKED_CODE);
            //              break;
            //          }
            //      }
            //      printf(_T("current addr: 0x%x   \n"), pHAI->API_INFO.HEA.HookdAddr);
            //      printf(_T("origin  addr: 0x%x   \n"), pHAI->API_INFO.HEA.OriginalAddr);
            //      break;
            //  }
            //  
            //  case AT_IMPORTED:
            //  {
            //      printf(_T("IAT hook ==  \n"));
            //      printf(_T("%s.%s    \n"), pHAI->API_INFO.HIA.DLLName, pHAI->API_INFO.HIA.ApiName);                  
            //      switch (pHAI->API_INFO.HIA.HOOKED_CODE)
            //      {
            //          case AH_IAT_MODIFIED:
            //          {
            //              printf(_T("AH_IAT_MODIFIED  \n"));                          
            //              break;
            //          }
            //          case AH_IAT_INLINE_HOOKED:
            //          {
            //              printf(_T("AH_IAT_INLINE_HOOKED \n"));
            //              break;
            //          }
            //          default:
            //          {
            //              printf(_T("oops! unknown hook code !! %d    \n"), pHAI->API_INFO.HIA.HOOKED_CODE);
            //              break;
            //          }
            //      }
            //      printf(_T("DetourAddr: 0x%x \n"), pHAI->API_INFO.HIA.DetourAddr);
            //      printf(_T("valid addr: 0x%x - 0x%x  \n"), pHAI->API_INFO.HIA.dwStart, pHAI->API_INFO.HIA.dwEnd);
            //      printf(_T("Hooked addr: 0x%x    \n"), pHAI->API_INFO.HIA.HookdAddr);
            //      break;
            //  }
            //  case AT_INTEGRITY:
            //  {
            //      printf(_T("Import table modified == \n"));
            //      printf(_T("original import dll count: %d \ncurrent import dll count: %d \n"), 
            //          pHAI->API_INFO.IMI.OrgImpModCount,
            //          pHAI->API_INFO.IMI.CurImpModCount);
            //      break;
            //  }
            //  default:
            //  {
            //      _ASSERTE(!"Oops! never reach here!");
            //      printf(TEXT("Oops! never reach here!\n"));
            //      break;
            //  }                   
            //}// switch(pHAI->API_TYPE)

            break;
        }
        case AKDETECT_DEBUG:
        {
            _report.SetHeadline(_T("���� ���μ����� �������°� �����Ǿ����ϴ�.\n"));
            _report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
            _report.SetInfo(pHDI);
            _report.SetReportType(eARMORKIT_REPORT_ALERT);
            cAK_Interface.insertReport(_report);

            //PANTI_DEBUG_CHECK_INFO pADCI = (PANTI_DEBUG_CHECK_INFO) pHDI->Buf;
            //
            //printf(_T("by spcail api: %d, by prcs info: %d, by pprcs info: %d, by device: %d, by seh: %d    \n"),
            //    pADCI->bResSpecificAPI,
            //    pADCI->bResProcessInfo,
            //    pADCI->bResParentProcessInfo,
            //    pADCI->bResActiveDevices,
            //    pADCI->bResSEH);
   
            break;
        }
        case AKDETECT_RUNCODE:
        {
            _report.SetHeadline(_T("���� ��Ÿ�� ���μ����� �ڵ尡 �����Ǿ����ϴ�.\n"));
            _report.SetContents(_T("�ҹ� ���α׷� ���� ���� �̿뿡 ������ ������ �� �ֽ��ϴ�.\n���� ������ http://www.sunonline.co.kr/customer/inquiry/ ���� ���� ��Ź �帳�ϴ�."));
            _report.SetInfo(pHDI);
            _report.SetReportType(eARMORKIT_REPORT_ALERT);
            cAK_Interface.insertReport(_report);
            break;
        }   
        case AKDETECT_TOUCH : 
        {
#ifdef _AK_DLL_
            return 0;
#else
            int value = GetTouchValue(pHDI->Size);
            return value;           
#endif
        }
        default:
        {
            assert (!"SunArmorKitCallBackProc() : unknown hack type - never reach here!");
            break;
        }
    
    }
#endif	//	_KTW_ARMORKIT_2
    return 0;
}


BOOL SunArmorKitAlertInit(HINSTANCE hInstance, int nCmdShow)
{
    SunArmorKitAlertMyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!SunArmorKitAlertInitInstance (hInstance, nCmdShow)) 
    {
        return FALSE;
    }

    return TRUE;
}

//
// �Լ�: MyRegisterClass()
//
// ����: â Ŭ������ ����մϴ�.
//
// ����:
//
// Windows 95���� �߰��� 'RegisterClassEx' �Լ����� ����
// �ش� �ڵ尡 Win32 �ý��۰� ȣȯ�ǵ���
// �Ϸ��� ��쿡�� �� �Լ��� ����մϴ�. �� �Լ��� ȣ���ؾ�
// �ش� ���� ���α׷��� �����
// '�ùٸ� ������' ���� �������� ������ �� �ֽ��ϴ�.
//
ATOM SunArmorKitAlertMyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

	WORD wIconID = IDI_SUNICON;

	//_GS_ADD:������ ������ ����
#if defined( _TAIWAN )
	wIconID = IDI_SUNICON_TW;
#elif defined( _CHINA)
	wIconID = IDI_SUNICON_CN;
//#elif defined( _JAPAN)
//	wIconID = IDI_SUNICON_JP;
#endif
	//_GS_ADD

    wcex.cbSize			= sizeof(WNDCLASSEX); 
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= (WNDPROC)SunArmorKitAlertWndProcSub;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)wIconID);
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= NULL;
    wcex.lpszClassName	= _T("ARMORKIT SYSTEM POPUP CLASS");
    wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)wIconID);

    return RegisterClassEx(&wcex);
}

//
// �Լ�: InitInstance(HANDLE, int)
//
// ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
// ����:
//
// �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
// �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL SunArmorKitAlertInitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd;

    hWnd = CreateWindow(_T("ARMORKIT SYSTEM POPUP CLASS"), _T("ARMORKIT System check"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    g_hWndMain = hWnd;
    return TRUE;
}

//
// �Լ�: WndProc(HWND, unsigned, WORD, LONG)
//
// ����: �� â�� �޽����� ó���մϴ�.
//
// WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
// WM_PAINT	- �� â�� �׸��ϴ�.
// WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK SunArmorKitAlertWndProcSub(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message) 
    {
    case WM_COMMAND:
        wmId    = LOWORD(wParam); 
        wmEvent = HIWORD(wParam); 
        // �޴��� ���� ������ ���� �м��մϴ�.
        switch (wmId)
        {
        case 0:
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

#ifndef MAX_LOADSTRING
#define MAX_LOADSTRING 256
#endif

extern TCHAR	g_szSystemCheckTitle[MAX_LOADSTRING];
extern TCHAR	g_szSystemCheckMessage[MAX_LOADSTRING];

LRESULT CALLBACK SunArmorKitAlertWndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
            //	Soul of the Ultimate Nation Online - �ý��� ����
            SetWindowText(hWnd, g_szSystemCheckTitle);

            CArmorKit_Report    latestReport = cAK_Interface.GetLatestReport();

            util::_tstring      strHeadline = latestReport.GetHeadline();
            util::_tstring      strContents = latestReport.GetContents();
            SetDlgItemText(hWnd, IDC_ARMORKITALERT_HEADELINE, strHeadline.c_str());
            SetDlgItemText(hWnd, IDC_ARMORKITALERT_CONTENT, strContents.c_str());

            g_dwForcedExitStartTick = GetTickCount();
        }
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
        {
            EndDialog(hWnd, LOWORD(wParam));
            PostQuitMessage(0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

#endif
