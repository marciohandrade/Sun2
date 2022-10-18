/*!
 *	\file		winmain.cpp
 *	\brief		winmain 시작 점
 *	\image		html winmain.jpg	"흐름도"
 *	\bug		버그 내용 작성
 *	\todo		함수에 doxygen command 달기
 *	\author		김경표 (lammy@webzen.co.kr)
 *	\version	2008/12/19 김경표 - doxygen command 작성
 *	\version	2004/12/13 first release
 */

#include "SunClientPrecompiledHeader.h"
#include "stdafx.h"

#include <io.h>


#include "SceneBase.h"
#include "Application.h"
#include "Resource.h"
#include <SolarDump.h>
#include <Stackwalker.h>
#include <process.h>

#include "Version.h"
#include "ClientVer.h"
#include "GlobalFunc.h"
#include "GameOptionParser.h"
#include "InterfaceManager.h"
#include "uiLoginMan/uiLoginMan.h"

#if (WZENGINEVER >= 480)
    #include "LogManager.h"
#endif

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
    #include "AutoTestManager.h"
#endif

//------------------------------------------------------------------------------ 
#ifdef _NHN_USA_CHANNELING
	#include <HanAuth/HanReportForClient.h>	//지표모듈
	#include <HanAuth/HanAuthForClient.h>	//인증모듈
	#include <HanAuth/HanPollForClient.h>	//설문조사모듈
#endif//_NHN_USA_CHANNELING(북미채널링)

#ifdef	__NA000000_070913_HACKSHIELD_INSTALLATION__
    #include "HShield/HackShield.h"

    #pragma comment( lib, "../../lib/HShield/" MAKESTR(HACKSHIELD_VERSION) "/HShield.lib")
    #pragma comment( lib, "../../lib/HShield/" MAKESTR(HACKSHIELD_VERSION) "/HSUpChk.lib")
    #pragma comment( lib, "version.lib" )
#endif

#if USING_GAMEGUARD
#include "GameGuard/GameGuardApi.h"
#endif // USING_GAMEGUARD

#include "XignCode/XignCodeApi.h"

#pragma warning( push )
#pragma warning( disable : 4099 )


#if _MSC_VER >= 1500 // vs2008
    //! 북미용인데 지금은 사용하지 않음
    #pragma comment(linker, "/NODEFAULTLIB:HanAuthForClientD.lib")
    #pragma comment(linker, "/NODEFAULTLIB:HanAuthForClient.lib")
#endif

//------------------------------------------------------------------------------
#ifndef _ZLIBINCLUDED
    #define _ZLIBINCLUDED
    #pragma message("     _Adding library: zlib.lib (Please read License)" ) 
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/zlib.lib")
#endif

#ifndef _BINKLIBINCLUDED
    #define _BINKLIBINCLUDED
    #pragma message("     _Adding library: binkw32.lib (Please read License)" ) 
    #pragma comment(lib, "../lib/binkw32.lib")
#endif

// Comment out these definitions if you don't want to use it as a .lib
#ifndef _XML_PARSER
    #define _XML_PARSER
    #ifdef _DEBUG
        #pragma message("     _Adding library: XMLParserd.lib (Debug version - Please read License)" ) 
        #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/XMLParserd.lib")
    #else
        #pragma message("     _Adding library: XMLParser.lib (Release version - Please read License)" ) 
        #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/XMLParser.lib")
    #endif
#endif

//------------------------------------------------------------------------------------
#ifndef _IAF_EXPORT //#if'N'def
    #ifdef _DEBUG
        #pragma comment(lib, _T("../../lib/IAFD.lib"))
    #else//_DEBUG
        #pragma comment(lib, _T("../../lib/IAF.lib"))
    #endif//_DEBUG
#endif//_IAF_EXPORT

//------------------------------------------------------------------------------ 
// include sun engine library
//#define ENGINE_LIB_PATH  "../../../SUNEngine_trunk/lib"
//#define ENGINE_LIB_PATH  "../../../sunengine/lib"
#include "SunEngineLibInc.h"

//------------------------------------------------------------------------------ 
//! 통합샵
#include "SLMLibInc.h"

//------------------------------------------------------------------------------ 
#ifdef _DEBUG
    #pragma	comment(lib, "../lib/"VC_DIR_REDIRECT"/MemoryIni_d.lib" )

    #if _MSC_VER >= 1500 // vs2008 
        #ifdef _KOREA
            #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/MOLA_d.lib")
        #else
            #pragma comment(lib, "../../lib/"NATION_BUILD_NAME_OF_COUNTRY"/"MAKESTR(NATION_BUILD_VERSION)"/MOLA_d.lib")
        #endif
    #else
        #ifdef _KOREA
            #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/MOLA.lib")
        #else
            #pragma comment(lib, "../../lib/"NATION_BUILD_NAME_OF_COUNTRY"/"MAKESTR(NATION_BUILD_VERSION)"/MOLA.lib")
        #endif
    #endif
#else
    #pragma	comment(lib, "../lib/"VC_DIR_REDIRECT"/MemoryIni.lib" )

    #ifdef _KOREA
        #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/MOLA.lib")
    #else
        #pragma comment(lib, "../../lib/"NATION_BUILD_NAME_OF_COUNTRY"/"MAKESTR(NATION_BUILD_VERSION)"/MOLA.lib")
    #endif
#endif

//==================================================================================================  
/*-+-+-+-+- For Memory Alloc Hook -+-+-+-+-*/
//#if USING_SUN_SHARED_POOL
//    #include "./SharedMemoryLeakReporter.hxx"  
//#endif
//==================================================================================================  

#ifdef _DEV_VER 
    extern "C" int WinMainCRTStartup();

    #ifdef _CLIENT_LEAK_CHECK_DEFAULT_VER	
		#include "SunAllocHook.h"
    #endif

    int SunEntryPoint()
    {
        SharedMemoryPool::CreateInstance();  

    #ifdef _CLIENT_LEAK_CHECK_DEFAULT_VER
        //_CrtSetAllocHook(SunAllocHook);
    #endif

        return WinMainCRTStartup();
    }
#endif //_DEV_VER

/*-+-+-+-+- Global Variables -+-+-+-+-*/

#define MAX_LOADSTRING 256

Application *g_pApplication;

TCHAR	g_szSystemCheckTitle[MAX_LOADSTRING];
TCHAR	g_szSystemCheckMessage[MAX_LOADSTRING];
TCHAR   g_szSystemCheckPlzWait[MAX_LOADSTRING];


/*-+-+-+-+- Functions -+-+-+-+-*/
#ifdef _DEV_VER
static void RedirectAssertionHandler(char* msg, char* file_name, DWORD line_number);
#endif

LRESULT CALLBACK WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SystemProc(HWND, UINT, WPARAM, LPARAM);


/*-+-+-+-+- <><><> WinMain Function <><><> -+-+-+-+-*/

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	xxInitSceneLog(0);

	g_hInstance = hInstance;

#ifdef _CLIENT_LEAK_CHECK_DEFAULT_VER	
     _CrtSetDbgFlag( 
         _CRTDBG_ALLOC_MEM_DF | 
         _CRTDBG_LEAK_CHECK_DF
#	ifdef _CHECK_MEMORY_OVERRUN
         | _CRTDBG_DELAY_FREE_MEM_DF 
#	endif
         );
#endif

#ifdef _CLIENT_LEAK_CHECK_DEFAULT_VER
     //_CrtMemDumpAllObjectsSince(0);
     //_CrtSetBreakAlloc(516);
#endif

#ifdef _YMS_USING_FROST_LIBRARY_
     sun_network_api::set_network_security(sun_network_api::kNetworkSecurity_Frost);
     sun_network_api::sun_initialize();
#endif //_YMS_USING_FROST_LIBRARY_

#if USING_GAMEGUARD
     if (GameGuardApi::Instance()->Init(_CUSTOM_GAMEGUARD_NAME) == FALSE) // Version.h*/
     {
         GameGuardApi::DestroyInstance();
         TCHAR title[256] = {0, };
         if (GameGuardApi::error_code_ != 0)
         {
             _stprintf(title, _T("nProtect GameGuard : %u"), GameGuardApi::error_code_);
         }
         MessageBox(NULL, GameGuardApi::error_message_, title, MB_OK);
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
         TCHAR log_message[INTERFACE_STRING_LENGTH] = {0, };
         Snprintf(log_message, INTERFACE_STRING_LENGTH, _T("GameGuardApi::Init() 실패\n"));
         GlobalFunc::Write_GameGuard_Log(log_message);
#endif // _NA_000000_20140519_GAMEGUARD_LOG
         return 0;
     }
#endif // USING_GAMEGUARD

//-------------------------------------------------------------------------
// 게임 세팅 설정 시작
//-------------------------------------------------------------------------
	 // registry
#ifdef _DEBUG
	 g_pApplication->InitRegistry(hInstance);
#endif

     if(PACKER_MANAGER()->OpenPackFile("System\\System.wpk"))
     {
         GENERALPARAM->SetPackFileName("System\\System.wpk");
         LOGINPARAM->SetPackFileName("System\\System.wpk");
         CLIENTINFO->SetPackFileName("System\\System.wpk");
         GRAPHICPARAM->SetPackFileName("System\\System.wpk");
         PROGRAMPARAM->SetPackFileName("System\\System.wpk");

         TIMERPARAM->SetPackFileName("System\\System.wpk");
     }

     GENERALPARAM->SetSettingFileName( "System\\GENERAL.INI" );
     GENERALPARAM->Load();
     LOGINPARAM->SetSettingFileName( "System\\LOGIN.INI" );
     LOGINPARAM->Load();
     CLIENTINFO->SetSettingFileName( "System\\CLIENT_INFO.INI" );
     CLIENTINFO->Load();
     GRAPHICPARAM->SetSettingFileName("System\\GRAPHIC.INI");
     GRAPHICPARAM->Load();
     PROGRAMPARAM->SetSettingFileName("System\\PROGRAM.INI");
     PROGRAMPARAM->Load();

     TIMERPARAM->SetSettingFileName("System\\TIMER.INI");
     TIMERPARAM->Load();


//-------------------------------------------------------------------------
// 게임 세팅 설정 끝
//-------------------------------------------------------------------------


#ifdef	__NA000000_070913_HACKSHIELD_INSTALLATION__
    if (HackShield::Instance()->HS_Init() == FALSE)
    {
        HackShield::Instance()->HS_UnInit();
        HackShield::DestroyInstance();
        return 0;
    }
	
    if (HackShield::Instance()->HS_StartService() == FALSE)
    {
        HackShield::Instance()->HS_StopService();
        HackShield::Instance()->HS_UnInit();
        HackShield::DestroyInstance();
        return 0;
    }
#else
	CryptManager::InitInfo( NULL );
#endif

#if USING_XIGNCODE
    if (XignCodeApi::Init() == false)
    {
        MessageBox(NULL, "security module", "error", MB_OK);
        return 0;
    }
#endif//USING_XIGNCODE

#ifdef _DEV_VER
    FlowControl::RegisterAssertionHandler(RedirectAssertionHandler);
#endif

	char* pszDevOption = NULL;

#if defined(__KR) || defined(_KOREA)

#if defined(_DEV_VER)
	GlobalFunc::LoadDevIniFile();
	pszDevOption = GlobalFunc::GetDevOption();
#else
	char lpszFilePath[MAX_PATH];
	GetCurrentDirectoryA( MAX_PATH, lpszFilePath);
	strcat( lpszFilePath, "\\");
	strcat( lpszFilePath, "dev.ini");
	if(_access(lpszFilePath, 0) == 0)
	{
		//개발버전이 아닌데 dev.ini 파일이 존재하면 제거!!
		DeleteFile(lpszFilePath);
	}
#endif	//#if defined(_DEV_VER)

#endif	//#if defined(__KR) || defined(_KOREA)

    GlobalFunc::SetLogPath();


#ifdef LAST_RELEASE
    // no operation // 로그 시스템 - 유저 배포용 엔진에서는 적용하지 않음
#else 

    #if (WZENGINEVER >= 480)

    LogManager::Initialize();

    #endif
	
#endif // LAST_RELEASE

	if(pszDevOption && strlen(pszDevOption))
	{
		if(strstr(pszDevOption, "-loading"))	//log\LoadingTime.log 파일에 로딩시간 로그를 남기는 옵션.
		{
			GlobalFunc::SetLoadingTimeSaveMode(true);
		}
	}

	GlobalFunc::WriteLoadingTimeLog(_T("############ Game Start [%s]################"), c_tcBUILD_VERSION);


	//	인터페이스 엔진의 스트링 테이블을 로딩하기 전이기 때문에
	//	리소스에서 스트링을 가져 온다.
#ifdef	_KOREA
	LoadString( hInstance, IDS_SYSTEM_CHECK_KOR, g_szSystemCheckTitle, MAX_LOADSTRING );
	LoadString( hInstance , IDS_SYSTEM_CHECK_PLZ_WAIT_KOR , g_szSystemCheckPlzWait , MAX_LOADSTRING);
	LoadString( hInstance, IDS_SYSTEM_CHECK_MSG_KOR, g_szSystemCheckMessage, MAX_LOADSTRING );
#else
	LoadString( hInstance, IDS_SYSTEM_CHECK_FOREIGN, g_szSystemCheckTitle, MAX_LOADSTRING );
	LoadString( hInstance , IDS_SYSTEM_CHECK_PLZ_WAIT_FOREIGN, g_szSystemCheckPlzWait , MAX_LOADSTRING);
	LoadString( hInstance, IDS_SYSTEM_CHECK_MSG_FOREIGN, g_szSystemCheckMessage, MAX_LOADSTRING );
#endif


	
#if defined( _GAMECLIENT_VER ) || defined(_SUNGAME_DUMP_VER)
	#if defined( _CREATE_DUMP_ )
	    // 개발(GameClient.exe) or 사내(Sungame_Dump.exe) 버전 에서만 덤프를 남긴다
		SolarDump Dump;
		Dump.SetBuildVersion(c_tcBUILD_VERSION);
	#endif
#endif

	
    //--------------------------------------------------------------------------------------------------------------
    //
    // SYSTEM CHECK ROUTINE !!!
    //

    Application		App;
    g_pApplication = &App;


#ifdef _NHN_USA_CHANNELING
	//-------------------------------------------------------------------------
	//		북미 채널링 인증초기화
	//-------------------------------------------------------------------------
	g_szGameString = (char*)(lpCmdLine);

	if(!g_szGameString) return 0;

	//1.전달받은 게임스트링을 사용하여 인증모듈 초기화
	int nResultCode = HanAuthInitGameString(g_szGameString);
	
	if(nResultCode!=HAN_AUTHCLI_OK)
	{
		_asm nop; //TODO:인증실패시 적절한처리
	}

	//2.게임스트링을 전달받아 인증모듈내부에 저장한다.
	nResultCode = HanAuthForClientGameString(g_szGameString);

	//3.게임스트링에서 사용자 ID를 추출한다.
	char szMemberId[21]={0x00}; //21 is Max
	nResultCode = HanAuthGetId(g_szGameString,szMemberId,sizeof(szMemberId));

	//3.ID를통해 인증모듈 내부에 저장된 인증스트링을 갱신한다.
	HanAuthRefresh(szMemberId);

	//-------------------------------------------------------------------------
	//		북미 채널링 지표초기화
	//-------------------------------------------------------------------------
	
	//1.게임스트링을 읽어서 클라보고모듈을 초기화한다.
	nResultCode = HanReportInitGameString(g_szGameString);

	if(nResultCode!=0)
	{
		_asm nop; //TODO:지표초기화실패시 적절한처리
	}
	//-------------------------------------------------------------------------
	//		북미 채널링 설문조사모듈추가
	//-------------------------------------------------------------------------
	nResultCode = HanPollInitGameString(g_szGameString);
	
	if(nResultCode!=0)
	{
		_asm nop; //TODO:조사모듈실패시 적절한처리
	}

#endif//_NHN_USA_CHANNELING(북미채널링)

#if defined(_APPLY_JAPAN_GAMECHU_CHANNELING) || defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
    int commandline_arg_count = 0;
    wchar_t** command_line_arg_array = NULL;
    command_line_arg_array = ::CommandLineToArgvW(GetCommandLineW(), &commandline_arg_count);
    if (commandline_arg_count == 3)
    {
        uiLoginMan::SetParameterForAuth(command_line_arg_array);
    }

#ifdef _APPLY_RUSSIA_INGAMBA_CHANNELING
    #if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        if (uiLoginMan::IsUseParameterAuthInfo() != true)
        {
            return 0;
        }
    #endif //defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
#endif //_APPLY_RUSSIA_INGAMBA_CHANNELING

    #if defined(_JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_)

        #if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        // version.ini 를 통하여 auth control 및 loginfront 주소 세팅
        char ini_file_path[MAX_PATH] = {0, };
        char string_value[MAX_PATH] = {0, };
        GetCurrentDirectoryA(MAX_PATH, ini_file_path);
        strcat(ini_file_path, "\\version.ini");

        // loginfront 주소 세팅
        GetPrivateProfileStringA("SERVER", "URL1", "lf1.sunonline.jp", 
            string_value, MAX_PATH, ini_file_path);
        if (LOGINPARAM->GetLoginServerType() == 2)
        {
            LOGINPARAM->SetSelectedServiceLoginServerIP(string_value);
            LOGINPARAM->SetSelectedServiceLoginPort(LOGINPARAM->GetServiceLoginServerPort());
        }
        else
        {
            LOGINPARAM->SetLoginServerIP(string_value);
        }

        // 유저배포용 빌드로 개발망에서 테스트할 필요가 있음 
        //  - 현지에 패치 전달 전, 팩킹/업데이트 및 핵쉴드 테스트
        // INI 파일 설정으로 개발망에서 테스트 할 수 있도록 수정
        if (uiLoginMan::IsUseParameterAuthInfo() != true)
        {
            GetPrivateProfileStringA("AUTH", "AUTH_SELECT", "WEB_AUTH_FLOW", 
                string_value, MAX_PATH, ini_file_path);
            if (strcmp(string_value, "DEV_AUTH_FLOW") != 0)
            {
                return 0;
            }
        }  
        #endif

    #elif defined(_APPLY_JAPAN_GAMECHU_CHANNELING)
        // 2가지 인증방식지원하는 기간이 지나면 파라매터 값이 꼭 있어야 한다.
        if (uiLoginMan::IsUseParameterAuthInfo() != true)
        {
            return 0;
        }
    #endif //_JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

	GlobalFunc::ShowNetRecvLog(false);
	GlobalFunc::ShowNetSendLog(false);

#ifdef _DEBUG
    GlobalFunc::ShowPacketLog(true);
#else
    GlobalFunc::ShowPacketLog(false);
#endif

#ifdef _DEV_VER
	if (GENERALPARAM->GetSpecialMode() >= eSPECIAL_MODE_STEP1)
	{
		CustomAssertion::Instance()->ShowAssertMssage(true);
	}
#endif

#if defined(_DEV_VER) || defined(_DEBUG) || defined(_GAMECLIENT_VER)
	SetSaveParserLog(4);						// 디버그 모드 및 GameClient.exe 에서는 기본적으로 ProgramCommon 에러 로그를 남긴다.
	GlobalFunc::SetLogLevel(eLOG_LEVEL_INF);	// 디버그 모드 및 GameClient.exe 에서는 기본적으로 eLOG_LEVEL_ERR 수준의 로그를 남긴다.
#else
	SetSaveParserLog(0);
    GlobalFunc::SetLogLevel(eLOG_LEVEL_NONE);
#endif


	
	if(pszDevOption && strlen(pszDevOption))
	{
		char* pSpecialMode = strstr(pszDevOption, "-sp");
		if(pSpecialMode)
		{
			int iSp = atoi(pSpecialMode+3);
			GENERALPARAM->SetSpecialMode((eSPECIAL_MODE)iSp);
		}
		
		char* pParserLog = strstr(pszDevOption, "-parserlog");
		if(pParserLog)
		{
			BYTE bylevel = (BYTE)(atoi(pParserLog+10));
			SetSaveParserLog(bylevel);
		}

		char* ploglevel = strstr(pszDevOption, "-loglevel");
		if(ploglevel)
		{
			int iLoglevel = atoi(ploglevel+9);
			GlobalFunc::SetLogLevel((eLOG_LEVEL)iLoglevel);
		}

		if(strstr(pszDevOption, "-luaOn"))
		{
			GENERALPARAM->SetStartLuaConsole(TRUE);
		}

		if(strstr(pszDevOption, "-netrecvlog"))	// 이 커맨드 명령은 수시로 바꿔주자.. 유저에게 노출되었을경우에 대비하여.. 
		{
			GlobalFunc::ShowNetRecvLog(true);
		}

		if(strstr(pszDevOption, "-netsendlog"))	// 이 커맨드 명령은 수시로 바꿔주자.. 유저에게 노출되었을경우에 대비하여.. 
		{
			GlobalFunc::ShowNetSendLog(true);
		}

		// 리소스 테스트를 커맨드 명령을 이용해 할 수 있도록 한다.
		if(strstr(pszDevOption, "-resource"))
		{
			GENERALPARAM->SetTestResourceScene(TRUE);
			GENERALPARAM->SetTestResourceFailedOnly(TRUE);
			GENERALPARAM->SetTestResourceGraphic(TRUE);

			WRITELOG(eLOG_LEVEL_TRA, c_wOUTPUT_LOG_FILE, _T("resource check"));
		
		}

		if(strstr(pszDevOption, "-map"))
		{
			GENERALPARAM->SetTestResourceScene(TRUE);
			GENERALPARAM->SetTestResourceFailedOnly(TRUE);
			GENERALPARAM->SetTestResourceMap(TRUE);
			WRITELOG(eLOG_LEVEL_TRA, c_wOUTPUT_LOG_FILE, _T("map check"));
		}
		

	}

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
	#ifdef _KIKI_AUTOTEST

    AutoTestManagerEx::Instance()->LoadTestCase();

  //  if( AutoTestManager::Instance()->LoadEnableFlag() == true )
  //  {
  //      if( AutoTestManager::Instance()->LoadTestCase() == false )
  //      {
  //          return 0;
  //      }
  //  }
  //  else
  //  {
		//if(strstr(lpCmdLine, "-autotest"))
		//{
  //          char auto_test_file_name[256] = {NULL, };
  //          sscanf(lpCmdLine, "-autotest %s", auto_test_file_name);

  //          if (strlen(auto_test_file_name) > 0)
  //          {
  //              if(AutoTestManager::Instance()->LoadTestCase(auto_test_file_name) == false)
  //              {
  //                  return 0;
  //              }
  //          }
  //          else
  //          {
		//	    if(AutoTestManager::Instance()->LoadTestCase() == false)
		//	    {
		//		    return 0;
		//	    }
  //          }
		//}
  //  }
	#endif
#endif

#ifdef _CHINA

	LPTSTR cmdLine = GetCommandLine();
	char * pCmd = NULL;

	char * pStrLoginIP;
	char * pStrLoginPort;
	char strCmd[INTERFACE_STRING_LENGTH] = {0,};

	// 대문자 변환
	int length = _tcslen(cmdLine);
	for ( int i = 0; i < length ;i++ )
	{
		strCmd[i] = toupper(cmdLine[i]);
	}
	pCmd = strstr( strCmd , "-IP:");
	if( pCmd )
	{


		char * pToken;
		pToken = strtok(pCmd ," ");
		if (pToken )
		{	
			// -ip:xxx.xxx.xxx.xxx
			pStrLoginIP = &pToken[4];

			LOGINPARAM->SetSelectedServiceLoginServerIP(pStrLoginIP);

			pToken = strtok(NULL," ");
			
		}

		if( pToken )
		{
			if( strstr(pToken , "-PORT:") )
			{
				//-port:xxxx
				pStrLoginPort = &pToken[6];
				int port  = atoi(pStrLoginPort);
				LOGINPARAM->SetSelectedServiceLoginPort(port);
			}
		}

	}

#endif

	TCHAR	szBuildVersion[INTERFACE_STRING_LENGTH] ={0,};
	StrnCopy( szBuildVersion , c_tcBUILD_VERSION , INTERFACE_STRING_LENGTH -1);


	if (GENERALPARAM->IsNet())
    {
		GlobalFunc::WriteLoadingTimeLog(_T("############ open system check dialog ################"));

        // open system check dialog
        DialogBox(hInstance, (LPCTSTR)IDD_SYSTEM, NULL, (DLGPROC)SystemProc);
		
        // sys check dialog's message dispatch/translate
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.wParam == 1)
        {
            // 시스템 체크가 failed로 오는경우!!
            g_pApplication->ReleaseNet();
            return 0;
        }
    }

    //--------------------------------------------------------------------------------------------------------------

	TCHAR	szString[INTERFACE_STRING_LENGTH];
#ifdef	_JAPAN
	StrCopy( szString, "Loading..." );
#else
	StrCopy( szString, "Sun Online" );
#endif


	WORD wIconID = IDI_SUNICON;

	//_GS_ADD:국가별 아이콘 변경
#if defined( _CHINA)
    #ifdef _CN_CHANGE_ICON
    wIconID = IDI_SUNICON;
    #else
	wIconID = IDI_SUNICON_CN;
    #endif // _CN_CHANGE_ICON
//#elif defined( _JAPAN)
//	wIconID = IDI_SUNICON_JP;
#endif
	//_GS_ADD

	if( !g_pApplication->Create( hInstance, szString, WindowProc, (const TCHAR*)MAKEINTRESOURCE( wIconID ), nCmdShow ) )
	{
		exit( FALSE );
	}

    //-----------------------------------------------------------------------------------------------------------------------------
    // client version
	TCHAR	pAppName[INTERFACE_STRING_LENGTH] = {0,};

	if (GENERALPARAM->GetSpecialMode())	// 스페셜 모드인 경우에만 버전 표시
	{

#if defined( _DEBUG )
		Sprintf( pAppName, _T("%s - ver %s Debug ( Protocol ver : %d.%d.%d ) [%s]"),
			g_InterfaceManager.GetInterfaceString( 1661 ),
			CLIENTINFO->GetClientVersion(),
			C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, szBuildVersion);

#else


	if(LOGINPARAM->IsLiveTestServer())	// 테섭실행모드
	{
		Sprintf( pAppName, _T("%s - ver %s Test Release ( Protocol ver : %d.%d.%d ) [%s]"),
				g_InterfaceManager.GetInterfaceString( 1661 ),
				CLIENTINFO->GetClientVersion(),
				C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, szBuildVersion);

	}
	else	
	{
		Sprintf( pAppName, _T("%s - ver %s Release ( Protocol ver : %d.%d.%d ) [%s]"),
				g_InterfaceManager.GetInterfaceString( 1661 ),
				CLIENTINFO->GetClientVersion(),
				C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, szBuildVersion);

	}


#endif
	}
	else
	{
#if defined( _DEBUG )
		Sprintf( pAppName, _T("%s - Debug"), g_InterfaceManager.GetInterfaceString( 1661 ) );
#else

		if(LOGINPARAM->IsLiveTestServer())	// 테섭실행모드
		{
			Sprintf( pAppName, _T("%s - Test Release"), g_InterfaceManager.GetInterfaceString( 1661 ) );
		}
		else
		{
			Sprintf( pAppName, _T("%s"), g_InterfaceManager.GetInterfaceString( 1661 ) );
		}

#endif
	}
		
	::SetWindowText( g_hWndMain, pAppName );

#if USING_GAMEGUARD
    GameGuardApi::Instance()->SetClientHandle(g_hWndMain);
#endif // USING_GAMEGUARD
	

//-----------------------------------------------------------------------------------------------------------------------------

	xxWriteSceneLog("Application Run Start");
	xxWriteSceneLog("======================================");
    int nResult = g_pApplication->Run();
	xxWriteSceneLog("======================================");
	xxWriteSceneLog("Application Run End");
	


	g_pApplication->Destroy();

#ifdef LAST_RELEASE
    // no operation // 로그 시스템 - 유저 배포용 엔진에서는 적용하지 않음
#else

#if (WZENGINEVER >= 480)
    LogManager::Release();
#endif

#endif // LAST_RELEASE

#ifdef _NHN_USA_CHANNELING
	char szReportString[INTERFACE_STRING_LENGTH]={0x00};
	Snprintf (szReportString,INTERFACE_STRING_LENGTH-1, "id=%s&value=900",szMemberId);
	//2.클라이언트 종료보고(900)
	HanReportSendPosLog(szReportString);//게임종료(900)
	
	//3.설문조사모듈 팝업띄움
	HanPollProcess("0");
	
#endif//_NHN_USA_CHANNELING(북미채널링)


#ifdef	__NA000000_070913_HACKSHIELD_INSTALLATION__
	HackShield::Instance()->HS_StopService();
	HackShield::Instance()->HS_UnInit();
    HackShield::DestroyInstance();
#endif

#ifdef _YMS_USING_FROST_LIBRARY_
    sun_network_api::sun_finalize();
#endif //_YMS_USING_FROST_LIBRARY_

#if USING_XIGNCODE
    XignCodeApi::Release();
#endif//USING_XIGNCODE
#if USING_GAMEGUARD
    GameGuardApi::Instance()->LastMessagePrint();
    GameGuardApi::DestroyInstance();
#endif // USING_GAMEGUARD

	return ( nResult );
}


/*-+-+-+-+- <><><> Window Procedure <><><> -+-+-+-+-*/

LRESULT CALLBACK WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	if( g_pApplication)
	{
		return ( g_pApplication->WindowProc( hWnd, iMessage, wParam, lParam ) );
	}

	return ( DefWindowProc( hWnd, iMessage, wParam, lParam ) );
}


// 정보 대화 상자의 메시지 처리기입니다.
LRESULT CALLBACK SystemProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        {
		    //	Soul of the Ultimate Nation Online - 시스템 점검
            SetWindowText(hDlg, g_szSystemCheckTitle);
			SetDlgItemText(hDlg,IDC_STATIC , g_szSystemCheckPlzWait);

            SetTimer(hDlg,			// handle to main window 
                1,					// timer identifier 
                1000,				// 1-second interval 
                (TIMERPROC) NULL);	// no timer callback 
		}
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
        {
            EndDialog(hDlg, LOWORD(wParam));
            PostQuitMessage(0);
            return TRUE;
        }
        break;

    case WM_TIMER:
        {
            switch(wParam)
            {
            case 1:
                {
                    KillTimer(hDlg, 1);

                    g_pApplication->InitNet();
                    if (!g_pApplication->ConnectToAuthServer())
                    {
						//	Soul of the Ultimate Nation Online - 시스템 알림
						//	인증서버로부터 응답이 없습니다.\n\n네트워크가 연결되어 있지 않거나 방화벽에 막혀 접속할 수가 없습니다.\n네트워크 환경이나 방화벽 설정을 확인한 후 다시 시도해 주시길 바랍니다.
                        MessageBox(hDlg, g_szSystemCheckMessage, g_szSystemCheckTitle, 0);
                        EndDialog(hDlg, LOWORD(wParam));
                        PostQuitMessage(1);
                    }
                    else
                    {
                        if (g_pNetwork)
                        {
                            g_pNetwork->Disconnect(CI_AUTHSERVERIDX);
                        }

                        EndDialog(hDlg, LOWORD(wParam));
                        PostQuitMessage(0);
                    }
                }
                break;
            }
        }
        break;
    }
    return FALSE;
}

#ifdef _DEV_VER
static void
RedirectAssertionHandler(char* msg, char* file_name, DWORD line_number)
{
    CustomAssertion::Instance()->PushMsg(msg, file_name, line_number);
}
#endif

#pragma warning( pop )

/*-*-*-*                    End of Code                      *-*-*-*/
