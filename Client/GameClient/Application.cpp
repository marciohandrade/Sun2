/**
 *	@file		Application.cpp
 *	@brief		Application class
 *  @author		Unknown
 *	@version	Unknown
 */

// AppSample.cpp: implementation of the Application class.
//
//////////////////////////////////////////////////////////////////////
#include "SunClientPrecompiledHeader.h"
#include <iterator>
#ifndef _WIN32_DCOM
#define _WIN32_DCOM
#endif

#include <conio.h>
#include "Application.h"
#include "DllManager.h"
#include "GameFramework.h"
#include "varpacket.h"
#include "Resource.h"
#include "ResourceManager.h"
#include "MouseHandler.h"
#include "Camera.h"
#include "Mouse.h"
#include "SoundBGM.h"
#include <ItemInfoParser.h>
#include <NPCInfoParser.h>
#include <SkillInfoParser.h>
#include <SkillInventoryStoreParser.h>
#include "ItemManager.h"
#include "Map.h"
#include "ObjectManager.h"
#include "InterfaceManager.h"
#include "MapInfoParser.h"
#include "ShopInfoParser.h"
#include "GraphicResourceList.h"
#include "message.h"
#include "ProjectileManagerEx.h"
#include "MapResMgr.h"
#include "StateInfoParser.h"
#include "ItemcompositeParser.h"
#include "MissionRewardParser.h"
#include "HotkeyManager.h"
#include "appearanceManager.h"
#include "SlotKeyGenerator.h"
#include "CharSoundInfoParser.h"
#include "CharInfoParser.h"
#include "Hero.h"
#include "dlgPosInfoParser.h"
#include "BGMSoundInfoParser.h"
#include "RequireFieldLevelInfoParser.h"
#include "StyleQuickRegistInfoParser.h"
#include "EnvironmentInfoParser.h"
#include "ProjectileInfoParser.h"
#include "uiLoginAccount/uiLoginAccount.h"
#include "RadarInfoParser.h"
#include "NPCVoiceInfoParser.h"
#include "HeroVariationInfoParser.h"
#include "HeroHeightInfoParser.h"
#include "MonsterVoiceInfoParser.h"
#include "EventInfoParser.h"
#include "ItemSoundInfoParser.h"
#include "LimitedMapPortParser.h"
#include "HeadMeshInfoParser.h"
#include "EnchantParser.h"
#include "uiLoginMan/uiLoginMan.h"
#include "WrapArmorKit.h"
#include "KeyMapMgr.h"
#include "CursorChangeTransact.h"
#include "globalfunc.h"
#include "SceneBase.h"
#include "HeroActionInput.h"
#include "OptimizationManager.h"
#include "CharacterScene.h"
#include "LoadScene.h"
#include "SkillTextInfoParser.h"
#include "HelpInfoParser.h"
#include "ItemComposeListParser.h"
#include "ItemCrystalizeListParser.h"
#include "CompositeNPCInfoParser.h"	
#include "QuestInfoManagerEx.h"
#include "QuestTextInfoParser.h"
#include "QuestManager_Concrete.h"
#include "MissionManager_Concrete.h"
#include "ExpValueInfoParser.h"
#include "ItemTypeList.h"
#include "RewardInfoList.h"
#include "SunEventInfoParser.h"
#include "OptionListParser.h"
#include "SkillEffectInfoParser.h"
#include "AttachmentOffsetInfoParser.h"
#include "PacketRequestManager.h"
#include "cLua.h"

    #include "SpecialTexutureInfoParser.h"
#ifdef _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
    #include "ResourceVersionParser.h"
#endif //_RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT



#include "BattleScene.h"

#include <SolarFileReader.h>


//#ifdef HAIR_COLOR_VARIATION
//#include "HeadColorInfoParser.h"
//#endif

#include "uiSystemMan/uiSystemMan.h"

#ifdef GGUO_CRASH_DUMP
#ifdef _DH_DEBUG_REPORT
    #include "CrashReporter.h"
    #include "MD5Encrypt.h"
#else
    #include "Tlhelp32.h"
#endif
#endif//GGUO_CRASH_DUMP

#include "ClientVer.h"
#include "SolarDateTime.h"
#include "uiSystemVerify/uiSystemVerify.h"

#ifdef __NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
	#include "SCSlotFactory.h"
#endif

#include "ScreenShotSaver.h"

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
#include "uiPartyMan/uiPartyMan.h"
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__

#include "DelayLoadManager.h"
#include "ItemCooltimeManager.h"

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
#include "AutoTestManager.h"
#endif

#ifdef _YMS_AUTOBOT_GAME
    #include "Autobot/Autobot.h"
    #include "Autobot/Condition/AutobotConditionManager.h"
    #include "Autobot/Passpoint/AutobotPasspointManager.h"
#endif //_YMS_AUTOBOT_GAME

#ifdef _DEBUG //#ifdef _LTJ_PACKET_SHOW_FOR_DEBUG
    CPShowFNTable* g_pPacketShow = NULL;
#endif //_LTJ_PACKET_SHOW_FOR_DEBUG

#ifdef __NA000000_070913_HACKSHIELD_INSTALLATION__
#include "HShield/HackShield.h"
#endif //__NA000000_070913_HACKSHIELD_INSTALLATION__

#include "TreeMenuGroup.h"
#include "TextureListInfoHandleManager.h"
#include "XignCode/XignCodeApi.h"
#include "KBMoveCheckManagerInClient.h"
#include <new.h>
#include "HeroData.h"
#include "CooltimeManager.h"

#define MULTI_LOAD_COUNT_PER_SECOND		30

#pragma warning ( push )
#pragma warning(disable : 4995)

#pragma warning ( push )
#pragma warning(disable : 4800)
                              
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//		전역
//---------------------------------------------------------------------------------------
int		g_iUpdateFrameCount = 0;
int		g_iRenderFrameCount = 0;
WORD	g_wFrameSkipping = 0;

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"
#include "CreateCharCustomDialog.h"
bool is_slow_ = false;
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP

VOID NetworkRecv( DWORD dwIndex, BYTE * pData, DWORD dwSize );
VOID NetworkDisconnect( DWORD dwIndex );
VOID NetworkError( char * pszMsg );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace CheckMultiClient
{
    HANDLE m_hSemaphore = NULL;

    BOOL CreateHandle()
    {
#if defined(_SUNGAME_VER) || defined(_SUNGAME_DUMP_VER)
        LONG s_max_client_count = 3;
#else // defined(DEBUG) || defined(CLIENT_VER)
        LONG s_max_client_count = 10;
#endif 

        m_hSemaphore = CreateSemaphore( NULL, s_max_client_count, s_max_client_count, _T("SUN_GAME") );	
        if ( m_hSemaphore == NULL )
        {
            return FALSE;
        }

        DWORD dwWaitResult = WaitForSingleObject( m_hSemaphore, 100 );
        if( dwWaitResult == WAIT_TIMEOUT )
        {
            m_hSemaphore = NULL;
            return FALSE;
        }

        if( dwWaitResult == WAIT_OBJECT_0 )
        {
            return TRUE;
        }

        return FALSE;
    }

    void ReleaseHandle()
    {
        if( m_hSemaphore != NULL )
        {
            LONG previous_count;
            ReleaseSemaphore( m_hSemaphore, 1, &previous_count );
        }
    }
};


Application::Application() 
{

#ifdef GGUO_CRASH_DUMP
    ::_set_new_handler(NewHandler);
    ::_set_purecall_handler(PureCallHandler);
#if _MSC_VER == 1310
    // vs2003 전용
    ::_set_security_error_handler(BufferOverflowHandler);
#endif // 1310
	::SetUnhandledExceptionFilter( ExceptionFilter );
#endif

	Clear();

	m_hConsole = INVALID_HANDLE_VALUE;

#ifdef _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
#else // _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
    m_hSemaphore = NULL; 
#endif // _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT

	m_iDebugInfo = 0;
	m_bRunningGame = FALSE;

    m_dwInitialWndStyle = m_dwInitialWndStyle & (~(WS_SYSMENU |WS_MAXIMIZE | WS_MINIMIZE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME));

	for(int i = 0; i < 100; i++)
	{
		m_iProfile[i] = 0;
		ZeroMemory(m_szProfile[i], 100);
	}

	for(int i = 0; i < MAX_PRELOAD_RESOURCE; ++i)
	{
		l_pPreloadResource[i] = NULL;
	}

    l_dwPreloadResource[0] = 59051 + ENGINE_RESOURCE_ID_OFFSET;//용변신
	l_dwPreloadResource[1] = 59001 + ENGINE_RESOURCE_ID_OFFSET;//배틀러
	l_dwPreloadResource[2] = 59002 + ENGINE_RESOURCE_ID_OFFSET;//정령
	l_dwPreloadResource[3] = 59003 + ENGINE_RESOURCE_ID_OFFSET;//디스토리어

    //캐릭터의 기본스킨을 미리 로드한다
    ResourceManager* load_mgr = ResourceManager::Instance();

    if (load_mgr == NULL)
    {
        return;
    }
    int offset_index = 4;
    
    //step1: 캐릭터 PreLoad
    for(int index = 0; index < 5; ++index)
    {
        // 클래스
        eCHAR_TYPE class_type = (eCHAR_TYPE)(index + eCHAR_BERSERKER);
        // 남성
        l_dwPreloadResource[offset_index] = load_mgr->GetCharacterBoneResourceCode(class_type, GenderType::kMale);
        ++offset_index;
        // 여성
        l_dwPreloadResource[offset_index] = load_mgr->GetCharacterBoneResourceCode(class_type, GenderType::kFemale);
        ++offset_index;
    }

    //step2: 탈것 PreLoad
    for(int index = 0; index < 10; ++index)
    {
        l_dwPreloadResource[offset_index] = 5301 + index + ENGINE_RESOURCE_ID_OFFSET;
        ++offset_index;
    }
}

Application::~Application()
{
}

int Application::NewHandler(size_t)
{
    //SASSERT(NULL, "Memory allocate is fail.");
    int* break_point = NULL;
    *break_point = 200;
    return 0;
}

void Application::PureCallHandler()
{
    //SASSERT(NULL, "Pure function is called.");
    int* break_point = NULL;
    *break_point = 100;
}

void Application::BufferOverflowHandler(int code, void* unused)
{
    //SASSERT(NULL, "Buffer is overflow.");
    int* break_point = NULL;
    *break_point = 300;
}

void Application::Clear( void )
{
	m_VirtualPacket.clear();
	m_RecvVirtualPacket.clear();

	m_GameFramework = NULL;
	m_bGameInput = FALSE;
}


void Application::InitSound()
{
	GlobalFunc::WriteLoadingTimeLog(_T("Application::InitSound Start"));
	DWORD dwStartTime = GlobalFunc::StartTimeCheck();

	//-------------------------------------------------------------------------
	//		웹젠 사운드 엔진 초기화
	//-------------------------------------------------------------------------
	WZSNDINFO info;

	info.use_secure_filesystem = (bool)GENERALPARAM->IsUsePackFile();	// 압축 암호화 파일 시스템을 사용할지 여부


	info.enable_debugLog = FALSE;

	if (GENERALPARAM->GetSoundLogFlag())
	{
		info.enable_debugLog = TRUE;	// 비쥬얼 스튜디오 디버그 창 또는  _bin 폴더에 들어있는  DebugView로 
										// 디버깅 로그를 출력할지를 지정 ( true  or  false )
	}									//  정식 릴리즈 에서는 false 로 할것을 권장합니다.
	
	const int	c_iVeryLongPathLength = 65536;
	char szCurDir[c_iVeryLongPathLength];

	::GetCurrentDirectoryA( c_iVeryLongPathLength, szCurDir );
	
	if (info.use_secure_filesystem)
	{
        sprintf(info.pszSecureFileSystem, "%s", "data\\sound\\sound.wpk");

		sprintf(info.pszConfigFile, "%s", "wzsound_config.txt");
		sprintf(info.pszMusicFile, "%s", "wzsound_music.txt");
		sprintf(info.pszSoundFile, "%s", "wzsound_sound.txt");
		sprintf(info.pszVoiceFile, "%s", "wzsound_voice.txt");
	}
	else
	{
		sprintf(info.pszLocalFileSystem, "%s\\%s", szCurDir, "data\\sound\\");

		sprintf(info.pszConfigFile, "%s\\%s", szCurDir, "data\\sound\\wzsound_config.txt");
		sprintf(info.pszMusicFile, "%s\\%s", szCurDir, "data\\sound\\wzsound_music.txt");
		sprintf(info.pszSoundFile, "%s\\%s", szCurDir, "data\\sound\\wzsound_sound.txt");
		sprintf(info.pszVoiceFile, "%s\\%s", szCurDir, "data\\sound\\wzsound_voice.txt");
	}

    Sound::Instance()->SetUseSoundLibrary(FALSE);
	
	BOOL bUseEffectSound	= GENERALPARAM->ReadInt(_T("SOUND_EFFECT"), 1);
	BOOL bUseBGMSound		= GENERALPARAM->ReadInt(_T("SOUND_BGM"), 1);

	Sound::Instance()->SetUseEffectSound(bUseEffectSound );
	Sound::Instance()->SetUseBGMSound(bUseBGMSound);

	if (bUseEffectSound || bUseBGMSound)
	{
		Sound::Instance()->SetUseSoundLibrary(TRUE);
	}

	BOOL bRet = Sound::Instance()->Create(&info);

	if (bRet)
	{
		_asm nop;
	}

	GlobalFunc::EndTimeCheck(dwStartTime, true, _T("Application::InitSound End"));
}


void Application::ReleaseSound()
{
	//	웹젠 사운드 라이브러리 해제
	Sound::Instance()->Destroy();
	Sound::Instance()->DestroyInstance();
}


BOOL Application::LoadGameInfo()
{
	char PACK_FILE_NAME[MAX_PATH] = {0,};

	if( GENERALPARAM->IsUsePackFile() )
	{
		strcpy( PACK_FILE_NAME, "Data\\Resource.wpk" );
		PACKER_MANAGER()->OpenPackFile("Data\\Resource.wpk");
	}
	else
	{
		ZeroMemory( PACK_FILE_NAME, MAX_PATH );
	}

	g_pQMGR_Real = new QuestManager_Concrete(&g_QuestInfoManager);
	g_pQMGR_Temp = new QuestManager_Concrete(&g_QuestInfoManager);
	g_pQMGR_Real->Init(100);
	g_pQMGR_Temp->Init(100);  
	g_pMission_Concrete = new MissionManager_Concrete;
	
	return TRUE;
}

void Application::LoadGameInfoForSriptParserMng()
{
	GlobalFunc::WriteLoadingTimeLog(_T("Script(For ScriptVerCheck) Loding Start"));
	DWORD dwStartTime = GlobalFunc::StartTimeCheck();

	g_pClientScriptParserManager = new ClientScriptParserManager;
	g_pClientScriptParserManager->RegisterParser();
	g_pClientScriptParserManager->InitParserPool( NULL );
	g_pClientScriptParserManager->Load();

#ifdef _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
    #if defined( _KOREA )
    TCHAR* nation_code_string = "KOREA";
    #elif defined( _JAPAN )
    TCHAR* nation_code_string = "JAPAN";
    #elif defined( _CHINA ) || defined( _TAIWAN )
    TCHAR* nation_code_string = "CHINA";
    #elif defined( _GLOBAL )
    TCHAR* nation_code_string = "GLOBAL";
    #elif defined( _RUSSIA )
    TCHAR* nation_code_string = "RUSSIA";
    #endif

    if (StrnCmp(nation_code_string, ResourceVersionParser::Instance()->nation_code().c_str(), 6) != 0)
    {
        MessageBox(g_hWndMain, "Invalid Client Data Files", NULL, MB_OK);
        exit(0);
    }
#endif //_RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT

	GlobalFunc::EndTimeCheck(dwStartTime, true, _T("Script(For ScriptVerCheck) Loding End"));

    // 스크립트를 읽은 후에 스크립트 내용에 맞춰 메모리풀을 생성하기 때문에.. 
    xGetCooltimeManager()->InitializeCooltimeTable();
}


void Application::UnloadGameInfo()
{
	ResourceManager::Instance()->Release();
	ResourceManager::Instance()->DestroyInstance();

 
	ItemManager::Instance()->Release();
	ItemManager::Instance()->DestroyInstance();

    if (g_pQMGR_Temp)
    {
	    g_pQMGR_Temp->DestroyAll();
    }

    if (g_pQMGR_Real)
    {
	    g_pQMGR_Real->DestroyAll();
    }

	SAFE_DELETE( g_pQMGR_Temp );  
	SAFE_DELETE( g_pQMGR_Real );	
	SAFE_DELETE( g_pMission_Concrete );

	if( g_pClientScriptParserManager )
	{
		g_pClientScriptParserManager->Release();
		g_pClientScriptParserManager->DestroyInstanceEx();
		SAFE_DELETE( g_pClientScriptParserManager );
	}
}


//------------------------------------------------------------------------------
// Method : Create
// [2010-1-28 by i4u4me] 수정 내용 : 함수 순서 바꿈(게임옵션을 먼저 읽고 리소스 로딩)
//  사유 : 게임옵션에 국가별 변수를 참고해서 리소스 로딩
//      디파인 : _YMS_GAMEOPTION_LANGUAGETYPE
//  기존 : 
//      ResourceManager::Instance()->Init();
//      g_InterfaceManager.LoadFontAndStringTable();
//      m_GameFramework = new GameFramework;
//      GAMEOPTION->Load();
//------------------------------------------------------------------------------
BOOL Application::Create( HINSTANCE hInstance, LPCTSTR lpszAppName, WNDPROC WndProc, LPCTSTR lpszIconName, int nCmdShow )
{
//#ifdef _DEBUG
//    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//    _CrtSetBreakAlloc(3801);
//#endif

#ifdef _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
    if( CheckMultiClient::CreateHandle() == FALSE )
    {
        return FALSE;
    }

#else //_NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT

#ifdef _NA_20120109_KIKI_ENABLE_MULTI_CLIENT
    // no operation // 이 디파인이 활성화 되는 경우, 멀티 클라이언트 관련 체크를 하지 않음.(멀티 클라 허용 상태)
#else
	if (GENERALPARAM->GetSpecialMode() == eSPECIAL_MODE_NONE)
	{
		if ( !CheckOverlapExecute() )
		{
#ifdef _DEV_VER
			MessageBox( g_hWndMain, "CheckOverlapExecute failed", NULL, MB_OK );
#endif
			return FALSE;
		}
	}
#endif

#endif //_NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT

	m_bRunningGame = TRUE;
    this->m_dwCurrentTick = 0;
    this->m_bCheckToWorldServerReconnect = false;

#ifdef _DEV_VER
	g_Profiler.Init();	
	g_ProcessProfiler.Init();
#endif
	//-------------------------------------------------------------------------
	//		파일 시스템 초기화
	//-------------------------------------------------------------------------
	m_pDiscAccess = new CDiscAccess;
	m_pDiscAccess->Create();

	if( GENERALPARAM->IsSystemPackFileName() )
	{
		DWORD dwStart = GlobalFunc::StartTimeCheck();
		GlobalFunc::WriteLoadingTimeLog(_T("OpenDisc_Packer Start [System.wpk]"));

		// 패스워드를 넣어야한다!! - 패쓰워드가 없으면 NULL
		if( !m_pDiscAccess->OpenDisc_Packer( ePFN_SYSTEM, "System\\System.wpk" , NULL ) )
		{
			GlobalFunc::WriteLoadingTimeLog(_T("System\\System.wpk 파일 로딩 에러"));
			assert( !"System\\System.wpk 파일 로딩 에러" );
#ifdef _DEV_VER
			MessageBox( g_hWndMain, "System\\System.wpk 파일 로딩 에러!", NULL, MB_OK );
#endif
			return FALSE;
		}
		GlobalFunc::EndTimeCheck(dwStart, true, _T("OpenDisc_Packer End [System.wpk]"));
	}

	if( GENERALPARAM->IsUsePackFile() )
	{
		DWORD dwStart = GlobalFunc::StartTimeCheck();
		GlobalFunc::WriteLoadingTimeLog(_T("OpenDisc_Packer Start [Resource.wpk]"));

		// 패스워드를 넣어야한다!! - 패쓰워드가 없으면 NULL
		if( !m_pDiscAccess->OpenDisc_Packer( ePFN_RESOURCE, "Data\\Resource.wpk" , NULL ) )
		{
			GlobalFunc::WriteLoadingTimeLog(_T("Data\\Resource.wpk 파일 로딩 에러"));
			assert( !"Data\\Resource.wpk 파일 로딩 에러" );
#ifdef _DEV_VER
			MessageBox( g_hWndMain, "Data\\Resource.wpk 파일 로딩 에러!", NULL, MB_OK );
#endif
			return FALSE;
		}
		GlobalFunc::EndTimeCheck(dwStart, true, _T("OpenDisc_Packer End [Resource.wpk]"));

	}
	//-------------------------------------------------------------------------
	//		프레임웍 초기화, 랜더러 생성
	//-------------------------------------------------------------------------
	St_AppCreateParam AppCreateParam;

	if(strlen(GENERALPARAM->GetEngineRoot()) > 0)
	{
		sprintf(AppCreateParam.m_lpszDllPath,"%s\\dll\\",GENERALPARAM->GetEngineRoot());
	}
	else 
	{
		memset( AppCreateParam.m_lpszDllPath, 0, sizeof( AppCreateParam.m_lpszDllPath ) );
	}

	strcpy(AppCreateParam.m_lpszShaderPath, "System\\");
	strcpy(AppCreateParam.m_lpszIrradiancePath, "System" );

	AppCreateParam.m_dwDrawCreateOption = DCO_USE_PHYSICS | DCO_SET_IRRADIANCE_PATH;
	AppCreateParam.m_pDiscAccess = m_pDiscAccess;

	if( GENERALPARAM->IsSystemPackFileName() )
    {
		m_pDiscAccess->SetActiveDisc(ePFN_SYSTEM);
    }

    //! 스크린 사이즈 저장
    set_desktop_screen_size(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	if( !CAppLast::Create( hInstance, lpszAppName, WndProc, lpszIconName, nCmdShow, &AppCreateParam))
	{
		MessageBox( g_hWndMain, "Failed to create application.", NULL, MB_OK );
		return ( FALSE );
	}

#ifdef _HACKSHIELD_CHECK_TIMER_API_FUNCTION_
    m_pDraw->x_Clock.SetClockFunction(_AhnHS_QueryPerformanceCounter, 
                                      _AhnHS_QueryPerformanceFrequency, 
                                      _AhnHS_GetTickCount);
#endif //_HACKSHIELD_CHECK_TIMER_API_FUNCTION_

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
    if (uiLoginMan::IsUseParameterAuthInfo() == true)
    {
        if (web_in_client.Initialize() == TRUE)
        {
            web_in_client.SetHGame(m_hWnd);
            BOOL return_value = web_in_client.TransferMyHWND();
            if (return_value == FALSE)
            {
                MessageBox(NULL, "Failed to Transfer HWND", NULL, MB_OK);
            }
        }
    }
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

	if( GENERALPARAM->IsUsePackFile() )
	{
		m_pDiscAccess->SetActiveDisc(ePFN_RESOURCE);
	}
	else
	{
		m_pDiscAccess->SetActiveDisc(ePFN_NOT_USE_PACKFILE);
	}
	//-------------------------------------------------------------------------
	//		윈도우 핸들과 랜더러 초기화
	//-------------------------------------------------------------------------
	g_hWndMain = m_hWnd;
	g_pSunRenderer = m_pDraw;
	g_pSunRenderer->SetClearColor(0,0,0);

	//	화면 해상도 조절
	float fRate = (float)m_ScreenParam.m_ScreenFormat.m_dwWidth / (float)m_ScreenParam.m_ScreenFormat.m_dwHeight;
    g_Camera.SetAspectRatio(SR_AUTO);

	POINT pt = Get2DModeMapping();

	float fWidth = pt.x / (float)m_ScreenParam.m_ScreenFormat.m_dwWidth;
	float fHeight = pt.y / (float)m_ScreenParam.m_ScreenFormat.m_dwHeight;

	//-------------------------------------------------------------------------
	//		SUN 랜더러 외 나머지 엔진 로딩
	//-------------------------------------------------------------------------
    if( !DllManager::Instance()->LoadSeDlls() )
	{
		MessageBox( g_hWndMain, "Dll Load failed!", NULL, MB_OK );
		return FALSE;
	}

    //	타이머 초기화
#ifdef _DEV_VER
	g_pSunRenderer->x_Clock.SetFramePerSecond( 200, FALSE , 10 , FALSE );
#else 
	g_pSunRenderer->x_Clock.SetFramePerSecond( 100, FALSE , 10 , FALSE );
#endif 

	if(g_pSunRenderer->x_pManagerMultiLoader)
	{
		g_pSunRenderer->x_pManagerMultiLoader->SetLoadCountPerSecond( MULTI_LOAD_COUNT_PER_SECOND );
	}

    // LOD처리
    // 60미터 이상 떨어진 오브젝트에대해 노멀맵을 끄고
    // 30미터 이상 떨어진 오브젝트에대해 스키닝을 끈다.
#ifdef _PERF_DEBUG
	float fNormalMapDistance[2] = {10.0f,11.0f};
    float fSkinningDistance[2] = {10.0f,11.0f};
#else
    float fNormalMapDistance[2] = {50.0f,51.0f};
    float fSkinningDistance[2] = {40.0f,41.0f};
#endif

    g_pSunRenderer->x_pManagerUnitLOD->AutoInvalidNormalMap(TRUE,fNormalMapDistance);
    g_pSunRenderer->x_pManagerUnitLOD->AutoInvalidSkinning(TRUE,fSkinningDistance);
    g_pSunRenderer->x_pManagerUnitLOD->EnableUnitLOD(TRUE);

#ifdef _PERF_DEBUG
    float fLODDistance[4] = { 5.0f, 6.0f, 10.0f, 11.0f};	

	WzColor shadowcolor = WzColor_RGBA(155,155,155,255);
    g_pSunRenderer->SetColor(200,200,200,255,WRIC_AMBIENT);

    g_pSunRenderer->SetShadowColor(shadowcolor);
    g_pSunRenderer->x_pManagerUnitLOD->SetDistanceOfLOD( fLODDistance, 4);
    //g_pSunRenderer->x_pManagerShadowMap->SetDepthBias(2.0e-04f);
	g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(FALSE);

	g_pSunRenderer->EnableOcclusionCulling(FALSE);
	g_pSunRenderer->x_pManagerShadowMap->SetShadowRange(30.0f);
#else
	float fLODDistance[4] = { 20.0f, 21.0f, 40.0f, 41.0f};

	WzColor shadowcolor = WzColor_RGBA(155,155,155,255);
    g_pSunRenderer->SetColor(200,200,200,255,WRIC_AMBIENT);

    g_pSunRenderer->SetShadowColor(shadowcolor);
    g_pSunRenderer->x_pManagerUnitLOD->SetDistanceOfLOD( fLODDistance, 4);
    g_pSunRenderer->x_pManagerUnitLOD->EnableUnitSubTypeLOD(TRUE);

	g_pSunRenderer->EnableOcclusionCulling(FALSE);
	g_pSunRenderer->x_pManagerShadowMap->SetShadowRange(30.0f);
#endif	


    // 게임 옵션 로딩 및 초기화( GameData.Dat )
    GAMEOPTION->Load();

    // 아이템 3d Renderer
    ResourceManager::Instance()->Init();
    g_InterfaceManager.LoadFontAndStringTable();

    // 메인 프레임 웍 생성
    m_GameFramework = new GameFramework;


	bool bSkipMoviePlay = false;

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) // 디버그/개발배포 모드
    if( AutoTestManagerEx::Instance()->IsActive() == true )
    {
        bSkipMoviePlay = true;
    }

	//#ifdef _KIKI_AUTOTEST
	//	if(AutoTestManager::Instance()->IsEnable())
	//		bSkipMoviePlay = true;	// 자동테스트모드 활성화시에는 동영상 skip
	//#endif
#endif

	SCENE_TYPE iSceneType;

	if(bSkipMoviePlay)
	{
		iSceneType = SCENE_TYPE_START;
	}
	else
	{
		if( GAMEOPTION->GetShowMoviePlay() )
			iSceneType = SCENE_TYPE_MOVIEPLAY;
		else
			iSceneType = SCENE_TYPE_START;
	}


	if( FALSE == m_GameFramework->Create( iSceneType) )
	{
#ifdef _DEV_VER
		MessageBox( g_hWndMain, "m_GameFramework->Create() failed", NULL, MB_OK );	
#endif
		return FALSE;
	}

	//	다이렉트 인풋
	if( FAILED( g_Input.Create( m_hWnd ) ) )
	{
		//	다이렉트 인풋 생성 실패
#ifdef _DEV_VER
		MessageBox( g_hWndMain, "g_Input.Create() failed", NULL, MB_OK );
#endif
		return FALSE;
	}

	// 테스트용 변수 초기화
	InitializeTempVariable();

#ifdef _USE_LUA_SCRIPT
    // 루아 싱글톤 객체 생성, 초기화
    new cLuaMain();
    cLuaMain::instance()->InitLua();

#ifdef _DEV_VER
    if ( GENERALPARAM->IsStartLuaConsole() )
    {
        cLuaMain::instance()->OpenLuaConsole();
    }
#endif //_DEV_VER

#endif //_USE_LUA_SCRIPT

	Mouse::Instance()->Init( m_hWnd );
	Mouse::Instance()->SetScreenRes( fWidth, fHeight );
	m_ScreenSize.x = (LONG)m_ScreenParam.m_ScreenFormat.m_dwWidth;
	m_ScreenSize.y = (LONG)m_ScreenParam.m_ScreenFormat.m_dwHeight;

#ifdef __NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
	SCSlotFactory::Instance()->Init( 1, 1, 1, 1, 1 );
#endif

    new cHeroData();
    xGetHeroData()->Initialize();
    new cCooltimeManager();
    xGetCooltimeManager()->Initialize();

	ItemManager::Instance()->Init();
	ICONMGR()->Create(MAX_ICON_IMAGE, g_pSunRenderer);
	MAPRESMGR()->Create(MAX_MAP_OBJECT);
	g_SlotKeyGenerator.Init();

	RECT rcWindow;
	rcWindow.left = 0;
	rcWindow.top = 0;
	rcWindow.right = m_ScreenParam.m_ScreenFormat.m_dwWidth;
	rcWindow.bottom = m_ScreenParam.m_ScreenFormat.m_dwHeight;
	g_InterfaceManager.SetWindowRC(&rcWindow);


	CheckScreenSize();
	g_pSunRenderer->GetGammaRamp(g_D3dGammaRamp.red,g_D3dGammaRamp.green,g_D3dGammaRamp.blue);

	for(int i=0;i<256;i++)
	{
		g_D3dGammaRamp.red[i]*=(65535/255);
		g_D3dGammaRamp.green[i]*=(65535/255);
		g_D3dGammaRamp.blue[i]*=(65535/255);
	}

	m_hTextureOnHead = g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\Interface\\guild_back.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	if(INVALID_HANDLE_VALUE == m_hTextureOnHead)
	{
		assert(!"INVALID_HANDLE_VALUE == m_hTextureOnHead");
	}

#ifdef _DEBUG //#ifdef _LTJ_PACKET_SHOW_FOR_DEBUG
    if (g_pPacketShow == NULL)
    {
        g_pPacketShow = (CPShowFNTable*)GetCPShowHandler();
        g_pPacketShow->fnInit(C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, CATEGORY_ODD_NUMBER, PROTOCOL_ODD_NUMBER);
    }
#endif

#ifdef _NA_000000_20120410_SPEEDHACK
    WGPL::SetCallbackWGPL(BattleScene::CallbackWGPL);
#endif //_NA_000000_20120410_SPEEDHACK


#ifdef _NA_008316_20150604_DEV_VER_SHOW_DAMAGE_INFO
  #ifdef _DEV_VER
    TestModule::Instance()->SetModuleName(TestModule::TestMode_Show_Damage_info, "show_damage_info");
    TestModule::Instance()->SetTestMode(TestModule::TestMode_Show_Damage_info, false);
  #endif //_DEV_VER
#endif //_NA_008316_20150604_DEV_VER_SHOW_DAMAGE_INFO

    return TRUE;
}
//------------------------------------------------------------------------------
/** GS_BUG_FIX:특정해상도에서 정확하게 원하는사이즈를 변경시킬수없다.
*/
BOOL	
Application::CheckScreenSize()
{
	if(!g_pSunRenderer || g_pSunRenderer->IsFullScreen()) return FALSE;

	//설정된영역사이즈
	St_ScreenFormat		ScreenFormat;
	g_pSunRenderer->GetScreenFormat( &ScreenFormat );
	GAMEOPTION->SetExceptionFlag(FALSE);

	//클라이언트 영역체크
	RECT rcRealRect;
	GetClientRect(this->GetHWnd(),&rcRealRect);
	DWORD dwWidth = (rcRealRect.right-rcRealRect.left);
	DWORD dwHeight = (rcRealRect.bottom-rcRealRect.top);
	
	//실제사이즈와 비교했을때 정상적으로 변경되지않았다면,
	if(ScreenFormat.m_dwWidth != dwWidth || ScreenFormat.m_dwHeight != dwHeight)
	{
		//원본사이즈는 옵션에 따로 저장
		GAMEOPTION->SetExceptionSize(ScreenFormat);
		GAMEOPTION->SetExceptionFlag(TRUE);

		//다르다면 다시 재설정하자
		ScreenFormat.m_dwWidth	= dwWidth;
		ScreenFormat.m_dwHeight	= dwHeight;
		g_pSunRenderer->SelectMode( ScreenFormat );

		POINT pt = {(LONG)dwWidth,(LONG)dwHeight};
		OnChangeRes( &pt );

		RECT rcWindow = {0,0,(LONG)dwWidth,(LONG)dwHeight};
		g_InterfaceManager.SetWindowRC(&rcWindow);

		return TRUE;
	}
	return FALSE;

}

//------------------------------------------------------------------------------
/**
*/
void
Application::InitRegistry(HINSTANCE hInstance)
{
    //-----------------------------------------------------------------------------------
    //	게임 데이터 경로 설정
    //	* 레지스트리 관리로 변경 예정
    //-----------------------------------------------------------------------------------
	// 송정헌 : 각 국가별 실행경로 디파인이였으나 작업 단일화로 _bin 으로 실행경로를 맞춘다

	LoadStringA( hInstance, IDS_GAME_PATH, g_szRunPath, MAX_PATH );

    //	실행 폴더 설정
    SetCurrentDirectoryA( g_szRunPath );
}

//------------------------------------------------------------------------------
/**
*/
void
Application::InitNet()
{
    if( GENERALPARAM->IsNet() )
    {
        //	네트웍 객체 생성
        g_pNetwork = new Network();
        if (g_pNetwork)
        {
            g_pNetwork->Init(NetworkRecv, NetworkDisconnect, NetworkError);
        }
    }
    else
    {
        g_pNetwork = NULL;
    }
}


//------------------------------------------------------------------------------
/**
*/
BOOL
Application::ConnectToAuthServer()
{
    if (GENERALPARAM->IsNet())
    {
        if (g_pNetwork == NULL)
        {
            return FALSE;
        }

        GlobalFunc::InitRandom();

        //  Auth Server 연결
        BOOL bConnectRet = FALSE;
        int i;

        if (LOGINPARAM->GetLoginServerType() == 2)
        {
            const int c_iMAX_LOGIN_SEREVR_NUM = LOGINPARAM->GetServiceLoginServerNum();

			std::vector<int> tryidx;
			for (i=0; i<c_iMAX_LOGIN_SEREVR_NUM; ++i)
			{
				tryidx.push_back(i);
			}

			int bakId = GlobalFunc::GetRandom(c_iMAX_LOGIN_SEREVR_NUM);

			for (i=0; i<10000; ++i)
			{
				int Id = GlobalFunc::GetRandom(c_iMAX_LOGIN_SEREVR_NUM);

				if (bakId != Id)
				{
					int tempId = tryidx[bakId];
					tryidx[bakId] = tryidx[Id];
					tryidx[Id] = tempId;
					bakId = Id;
				}
			}

			for (i=0; i<c_iMAX_LOGIN_SEREVR_NUM; ++i)
			{
#ifdef _CHINA
                if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)
                {
                    //! 대만일경우
                    TCHAR   szLoginServer[128];
                    Sprintf (szLoginServer, _T("%s%d.%s"), LOGINPARAM->GetServiceLoginServerIP_Head(),
                        tryidx[i] + 1,
                        LOGINPARAM->GetServiceLoginServerIP_Tail());
                    // connect1.sunonline.co.kr 형식으로 ip 저장 
                    LOGINPARAM->SetSelectedServiceLoginServerIP(szLoginServer);
                    LOGINPARAM->SetSelectedServiceLoginPort(LOGINPARAM->GetServiceLoginServerPort());
                }
#elif defined(_APPLY_JAPAN_GAMECHU_CHANNELING)
                // no operation
#else
                TCHAR   szLoginServer[128];
                Sprintf (szLoginServer, _T("%s%d.%s"), LOGINPARAM->GetServiceLoginServerIP_Head(),
                    tryidx[i] + 1,
                    LOGINPARAM->GetServiceLoginServerIP_Tail());
				// connect1.sunonline.co.kr 형식으로 ip 저장 
                LOGINPARAM->SetSelectedServiceLoginServerIP(szLoginServer);
                LOGINPARAM->SetSelectedServiceLoginPort(LOGINPARAM->GetServiceLoginServerPort());
#endif
                bConnectRet = g_pNetwork->Connect(
                    CI_AUTHSERVERIDX,
                    LOGINPARAM->GetSelectedServiceLoginServerIP(),
                    LOGINPARAM->GetSelectedServiceLoginServerPort() );

				if (bConnectRet)
				{
					break;
				}
			}
        }

        if (FALSE == bConnectRet)
        {
            bConnectRet = g_pNetwork->Connect(
                CI_AUTHSERVERIDX,
                LOGINPARAM->GetLoginServerIP(),
                LOGINPARAM->GetLoginServerPort() );

        }

		uiLoginMan* loginMan = static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));

		if (loginMan && bConnectRet) 
			loginMan->UnsetTryReconnect();

		return bConnectRet;
    }
	else	
	{
		// 싱글모드는 무조건 TRUE 리턴
		return TRUE;
	}
}
//#endif

//------------------------------------------------------------------------------
/**
*/
void
Application::ReleaseNet()
{
    // socket close
    if (g_pNetwork && (GENERALPARAM->IsNet() == TRUE) && (g_pNetwork->IsConnected(CI_GAMESERVERIDX) == TRUE))
    {
        g_pNetwork->Disconnect(CI_GAMESERVERIDX);
    }
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    if (g_pNetwork && (GENERALPARAM->IsNet() == TRUE) && (g_pNetwork->IsConnected(CI_CHATSERVERIDX) == TRUE))
    {
        g_pNetwork->Disconnect(CI_CHATSERVERIDX);
    }
#endif
    if (g_pNetwork && (GENERALPARAM->IsNet() == TRUE))
    {
        //	네트웍 연결 끊기
        g_pNetwork->Disconnect(CI_GAMESERVERIDX);
        g_pNetwork->Release();

    }

    SAFE_DELETE(g_pNetwork);

    LOGINPARAM->DestroyInstance();
}

void Application::Destroy()
{
    // 현재 사용되고 있는 설정을 CAppLast::m_ScreenParam에 넣고, 이것은 CAppLast::Destroy에서 3dsetup.ini 에 저장된다
    g_pSunRenderer->GetScreenFormat( &m_ScreenParam.m_ScreenFormat );
    g_pSunRenderer->GetRenderOption( &m_ScreenParam.m_RenderOption );



#ifdef _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
    CheckMultiClient::ReleaseHandle();
#else //_NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT

	if (m_hSemaphore) 
	{
        LONG previous_count;
        ReleaseSemaphore( m_hSemaphore, 1, &previous_count );
		m_hSemaphore = NULL;
	}
#endif //_NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT

	if (!m_bRunningGame)
	{
		return;
	}

	g_KeyQueueManager.Release();
	
	InitializeVirtualPacket();

    //// socket close
    if (g_pNetwork && (GENERALPARAM->IsNet() == TRUE) && (g_pNetwork->IsConnected(CI_GAMESERVERIDX) == TRUE))
    {
        g_pNetwork->Disconnect(CI_GAMESERVERIDX);
    }
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    if (g_pNetwork && (GENERALPARAM->IsNet() == TRUE) && (g_pNetwork->IsConnected(CI_CHATSERVERIDX) == TRUE))
    {
        g_pNetwork->Disconnect(CI_CHATSERVERIDX);
    }
#endif
    if (g_pNetwork && (GENERALPARAM->IsNet() == TRUE))
    {
        //	네트웍 연결 끊기
        g_pNetwork->Disconnect( CI_GAMESERVERIDX );
        g_pNetwork->Release();
    }

	if (g_pSunRenderer && g_pSunRenderer->x_pManagerMultiLoader)
	{
		g_pSunRenderer->x_pManagerMultiLoader->ExitMultiThreadedLoading();
	}

	UnloadPreloadResource();


	g_ProjectileManagerEx.Destroy();
	//	현재 Scene 파괴
	m_GameFramework->Destroy();
	SAFE_DELETE( m_GameFramework );

	//	카메라 파괴
	g_Camera.Destroy();

#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.DeleteHero();
#endif

	//	인터페이스 메니져 종료
	g_InterfaceManager.Release();

	if ( g_pMap )
	{
		g_pMap->Destroy();
		SAFE_DELETE(g_pMap);
	}

	// 오브젝트 삭제..
	g_ObjectManager.Destroy();
    g_SlotKeyGenerator.Release();

	ResourceManager::Instance()->FreeAllCachedData();

	// 마우스 포인터 릴리즈
	MouseHandler::Instance()->ReleaseUnitRenderer();
	MouseHandler::Instance()->DestroyInstance();

	MouseCursor::Instance()->Release();
	MouseCursor::Instance()->DestroyInstance();

	CursorChangeTransact::Instance()->Release();
	CursorChangeTransact::Instance()->DestroyInstance();
		
	Mouse::Instance()->Release();
	Mouse::Instance()->DestroyInstance();
    
	KeyMapMgr::Instance()->DestroyInstance();

	// 사운드
	ReleaseSound();

#ifdef _USE_LUA_SCRIPT
    delete cLuaMain::instance();
#endif //_USE_LUA_SCRIPT


	// 기타인스턴스 삭제
	ReleaseManager();

	// 파싱 자료 소각
	UnloadGameInfo();

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
	m_RankManager.Release();
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

	if (m_pDiscAccess)
	{
		if( GENERALPARAM->IsSystemPackFileName() )
		{
			m_pDiscAccess->CloseDisc( ePFN_SYSTEM );
		}

		if( GENERALPARAM->IsUsePackFile() )
		{
			m_pDiscAccess->CloseDisc( ePFN_RESOURCE );
		}

		m_pDiscAccess->Destroy();
		SAFE_DELETE(m_pDiscAccess);
	}

	PACKER_MANAGER()->Release();
	PACKER_MANAGER()->DestroyInstance();

	// 이펙트 매니저 해제
	if ( g_pEffectManager)
	{
		g_pEffectManager->Destroy();

		Func_ContactEffect( CW_DELETE_EFFECT_MANAGER, g_pEffectManager);
		g_pEffectManager = NULL;
	}

	if(g_pPathExplorer)
	{
		//	길찾기 기능 파괴

        Func_ContactTerrain( CT_DELETE_PATH_EXPLORER, g_pPathExplorer );
		g_pPathExplorer = NULL;
	}

	//	SUN 3D 엔진 라이브러리 해제
    DllManager::Instance()->FreeSeDlls();

	m_bRunningGame = FALSE;

	GENERALPARAM->DestroyInstance();
	GAMEOPTION->DestroyInstance();
    LOGINPARAM->DestroyInstance();
	CLIENTINFO->DestroyInstance();
	GRAPHICPARAM->DestroyInstance();
	PROGRAMPARAM->DestroyInstance();

    TIMERPARAM->DestroyInstance();


	SAFE_DELETE(g_pNetwork);

#ifdef MAKE_TEST_MODULE_MACRO
	TestModule::Instance()->DestroyInstance();
#endif

	HotKeyManager::Instance()->DestroyInstance();
	ScreenShotSaver::Instance()->DestroyInstance();
	
//#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
//#ifdef _KIKI_AUTOTEST
//	AutoTestManager::Instance()->CompleteCurTestCase();
//	AutoTestManager::Instance()->DestroyInstance();
//#endif
//#endif

	g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hTextureOnHead );
	m_hTextureOnHead = INVALID_HANDLE_VALUE;

    DelayLoadManager::Instance()->DestroyInstance();
    ItemCooltimeManager::Instance()->DestroyInstance();

#ifdef _DEV_VER
	CustomAssertion::Instance()->DestroyInstance();
#endif

    PacketRequestManager::Instance()->DestroyInstance();

#ifdef _YMS_AUTOBOT_GAME
    Autobot::Instance()->DestroyInstance();
    AutobotConditionManager::Instance()->DestroyInstance();
    AutobotPasspointManager::Instance()->DestroyInstance();
#endif //_YMS_AUTOBOT_GAME

    KBMoveCheckManagerInClient::Instance()->DestroyInstance();

    xGetCooltimeManager()->Release();
    delete xGetCooltimeManager();
    xGetHeroData()->Release();
    delete xGetHeroData();

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
    AutoTestManagerEx::DestroyInstance();
#endif // defined(_DEBUG) || defined(_GAMECLIENT_VER)

	//	SUN 엔진 프레임 웍 종료
	CAppLast::Destroy();
}

LRESULT Application::WindowProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{	
	if (!m_bRunningGame)
	{
		return TRUE;
	}

	Mouse::Instance()->MouseMessageProc( hWnd, iMessage, wParam, lParam );

	//  인터페이스 처리
	SI_MESSAGE stMessage;
	ZeroMemory(&stMessage,sizeof(stMessage));

	BOOL bPreCheck = FALSE;
	BOOL bRet = FALSE;
	if (GameFramework::GetCurrentScene()) 
    {
		bPreCheck = GameFramework::GetCurrentScene()->SceneWndProc(hWnd, iMessage, wParam, lParam ,&stMessage);
	}

	if ( FALSE == bPreCheck )
	{
		bRet = g_InterfaceManager.InterfaceWndProc( hWnd, iMessage, wParam, lParam ,&stMessage);
	}

#ifdef _YMS_AUTOBOT_GAME
    Autobot::Instance()->MessageProc(hWnd, iMessage, wParam, lParam);
#endif //_YMS_AUTOBOT_GAME

	// Window 메시지 처리
	switch( iMessage )
	{
    case WM_NCRBUTTONDOWN:
    case WM_NCRBUTTONUP:
    case WM_NCRBUTTONDBLCLK:
    case WM_NCMBUTTONDOWN:
    case WM_NCMBUTTONUP:
    case WM_NCMBUTTONDBLCLK:
    //case WM_WINDOWPOSCHANGING:
    //case WM_WINDOWPOSCHANGED:
        // 캡션바 마우스 오른쪽 버튼 클릭시 
        return 0;
    case WM_INITMENUPOPUP:
    case 0x0313:
        // 작업 표시줄 마우스 오른쪽 클릭시
        return 0;

	case WM_SYSCOMMAND:
        {
		//	return 0;
            switch (wParam)
            {
            case SC_PREVWINDOW:
			case SC_NEXTWINDOW:
			case SC_KEYMENU:
                return 0;
            case SC_CLOSE:
#if WZENGINEVER >= 380
                SCENE_TYPE scene_type = GameFramework::GetCurScene();
                if (scene_type == SCENE_TYPE_NONE
                    || scene_type == SCENE_TYPE_MOVIEPLAY
                    || scene_type == SCENE_TYPE_START
                    || scene_type == SCENE_TYPE_CHARSELECT
                    || scene_type == SCENE_TYPE_LOGIN
                    || scene_type == SCENE_TYPE_LOAD)
                {
                }
                else
                {
                    if (g_InterfaceManager.IsLockDlg() == FALSE)
                    {
                        //! 인터페이스에 락 걸린 상태가 아니면 메뉴를 띄운다.
                        GlobalFunc::DoSystemDialog(0, 0);
                    }
                }
#endif //WZENGINEVER >= 380
                return 0; 
            }
        }
        break;

    case WM_PAINT:
        {			
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 그리기 코드를 추가합니다.

            EndPaint(hWnd, &ps);
			return TRUE;
        }
        break;

	//-----------------------------------------------------------------------------------
	//		마우스 입력
	//-----------------------------------------------------------------------------------
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
		{
			if ( iMessage != WM_MOUSEMOVE)
			{
				_asm nop;
			}
			POINT	Point;
			Point.x = (int)LOWORD( lParam );
			Point.y = (int)HIWORD( lParam );

			Mouse::Instance()->SetPoint(Point);
			POINT res = Mouse::Instance()->ConvertPoint(Point);

			g_Input.m_bUIMouseInput = g_InterfaceManager.IsIntersectRect( res );			
		}
		break;


	//-----------------------------------------------------------------------------------
	//		키보드 입력
	//-----------------------------------------------------------------------------------
	case WM_KEYDOWN:
		{
		}
		break;
	case WM_KEYUP:
		{	
		}
		break;
	case WM_SETCURSOR:
		{
			MouseCursor::Instance()->Update();
		}
		return 0;
	case WM_SETFOCUS:
		{
			MouseCursor::Instance()->Update();
            m_bFocus = TRUE;
            
            ::UpdateWindow(g_hWndMain);
		}
		break;
        
    case WM_KILLFOCUS:
        {
            m_bFocus = FALSE;
        }
        break;

    case WM_ACTIVATE:
        {
            switch (wParam)
            {
            case WA_INACTIVE:
                break;
            }
        }
        break;

    case WM_CANCELMODE:
        {
            if ( g_pHero )
            {
                // 이동하고 있을때는 이동을 멈춤. 그 외의 상태는 유지
                if (g_pHero->IsMove() == TRUE)
                {
                    g_HeroInput.SetAutoRun(FALSE);
                    g_pHero->StopMove();
                    g_pHero->SendStopMovePacket();
                }               
            }
        }
        break;  

	case WM_ACTIVATEAPP:
		{
			if(wParam == TRUE)
			{
				g_Input.Reset();						
			}
			else 
			{
				g_Input.Reset();
			}
		}
		break;

	case WM_SYSKEYDOWN:
		{
			switch ( wParam )
            {
            case VK_F4:
                {
                    return 0;
                }
                break;

            case VK_MENU:
                {
                    return 0;
                }
                break;

            case VK_TAB:
                {
                    _asm nop;
                }
                break;                
            }           
		}
		break;
    }

	// 엔진 업데이트 문서에 true가 리턴됐을때만 기본 wndproc을 호출하라고 되어있음.
	if (bRet) 
	{
        if( iMessage == WM_KEYDOWN && (int)wParam == VK_RETURN && GetKeyState( VK_CONTROL ) & 0x8000 )
        {
            CGameOption::Instance()->ToggleFullScreenMode();
            return 0;
        }
        else
        {
            return ( CAppLast::WindowProc( hWnd, iMessage, wParam, lParam ) );
        }
	}
	else
	{
		return 0;
	}
}

void Application::SendToChatHeartbeatSyn()
{
	MSG_CW_HEARTBEAT m_CW_HeartbeatMsg;
	GlobalFunc::SendPacket(CI_CHATSERVERIDX, &m_CW_HeartbeatMsg, sizeof(m_CW_HeartbeatMsg));
}

int Application::HeartBeat( void )
{   
	// 핵쉴드 해킹툴 감지시 클라 종료
	if (g_bExitFlag)
	{
		xxWriteSceneLog("Application Exit Code: 3280");

		return HBR_EXIT;
	}

    // 네트워크 업데이트
	// LoadScene 일 때 네트워크 업데이트를 호출하면 안된다.  - 이태준 09.07.15
	// LoadScene 에서는 패킷을 처리하지 않음. 따라서 패킷을 꺼내와 파싱하면 안됨.
	// LOAD_SCENE_BLOCK_NETWORK_UPDATE
	if ( SCENE_TYPE_LOAD != m_GameFramework->GetCurScene())
	{		
		if (g_pNetwork && (GENERALPARAM->IsNet() == TRUE))
		{			
			g_pNetwork->Update();
		}
		else
		{
			ParseVirtualPacket();
		}
	}

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
    if ((uiLoginMan::IsUseParameterAuthInfo() == true) && 
        (IsInitWIC() == TRUE))
    {
        web_in_client.CheckCBEvent();
    }    
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

	//	타이머 업데이트
#ifdef _HACKSHIELD_CHECK_TIMER_API_FUNCTION_
    if ((DWORD)g_pSunRenderer->x_Clock.GetFuncPerformanceCount() != (DWORD)_AhnHS_QueryPerformanceCounter)
    {
		xxWriteSceneLog("Application Exit Code: 3281");
        return HBR_EXIT;
    }
    else if ((DWORD)g_pSunRenderer->x_Clock.GetFuncPerformanceFrequency() != (DWORD)_AhnHS_QueryPerformanceFrequency)
    {
		xxWriteSceneLog("Application Exit Code: 3282");
        return HBR_EXIT;
    }
    else if ((DWORD)g_pSunRenderer->x_Clock.GetFuncGetTickCount() != (DWORD)_AhnHS_GetTickCount)
    {
		xxWriteSceneLog("Application Exit Code: 3283");
        return HBR_EXIT;
    }
#endif //_HACKSHIELD_CHECK_TIMER_API_FUNCTION_

	DWORD dwTick;
	g_wFrameSkipping = g_pSunRenderer->x_Clock.CheckFrameSkipping( &dwTick );
    this->m_dwCurrentTick = clock_function::GetTickCount();

    this->CheckReconnectToWorldServer();
	
//#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
//	#ifdef _KIKI_AUTOTEST
//		if(GENERALPARAM->IsNet() && AutoTestManager::Instance()->IsEnable())
//		{
//			AutoTestManager::Instance()->Run(dwTick);
//		}
//	#endif
//#endif

	//-------------------------------------------------------------------------
	//	Scene 업데이트
	//  Animate, 계산에 관련된거 이안에 다 넣어야 함!!!! taiyo
	//-------------------------------------------------------------------------
	if( g_wFrameSkipping & CFS_MOVE )
	{
		// 테스트 코드

		++g_iUpdateFrameCount;

		SUNPROCESSPROFILE_BEGIN( _T("Process") );

		UpdateGlobalVariable();

		MouseHandler::Instance()->Update();

		//	다이렉트 인풋 입력 상태 업데이트
		g_Input.Update( g_Input.m_bUIMouseInput );

#ifdef MAP_EDIT_MODE
		// no operation
#else
	#ifdef _DEV_VER
        if (GENERALPARAM->GetSpecialMode())
		{
			ProcessDebugInput();
		}
	#endif
#endif 

		g_InterfaceManager.Process(dwTick);
		g_OptimizationManager.Process(dwTick);
		
		ResourceManager::Instance()->Process(dwTick);

		SUNPROCESSPROFILE_BEGIN(_T("ProessCamera"));
        g_Camera.Process(dwTick);
		SUNPROCESSPROFILE_END(_T("ProessCamera"));

#ifdef MAKE_TEST_MODULE_MACRO
		TestModule::Instance()->ProceeInput(dwTick);
#endif //MAKE_TEST_MODULE_MACRO

#ifdef _YMS_AUTOBOT_GAME
        if ((g_Input.GetState(MOUSE_LBUTTON, KS_MOUSE_DOWN) == FALSE) && 
            (g_Input.GetState(MOUSE_LBUTTON, KS_MOUSE_KEY) == FALSE) && 
            (g_Input.GetState(MOUSE_LBUTTON, KS_MOUSE_UP) == FALSE) && 
            (g_Input.IsPressAnyKey() == FALSE))
        {
            Autobot::Instance()->Update(dwTick);
        }
#endif //_YMS_AUTOBOT_GAME

        //	각 Scene 프로세싱
		if( FALSE == m_GameFramework->Update( dwTick ) )
		{
			xxWriteSceneLog("Application Exit Code: 3284");
			WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("HBR_EXIT: 엔진에서 종료하게 되므로 클라이언트도 함께 종료"));
            assert (!"HBR_EXIT: 엔진에서 종료하게 되므로 클라이언트도 함께 종료");
            return HBR_EXIT;
        }
		SUNPROCESSPROFILE_BEGIN(_T("InterfaceUpdate"));

		// 인터페이스 업데이트
		g_InterfaceManager.Update();

		SUNPROCESSPROFILE_END(_T("InterfaceUpdate"));

		SUNPROCESSPROFILE_BEGIN( _T("EffectSound") );

		ProcessEffectSound();

		SUNPROCESSPROFILE_END( _T("EffectSound") );

        Sound::Instance()->Update();

		SUNPROCESSPROFILE_END( _T("Process") );
	}

	//-------------------------------------------------------------------------
	//	Scene 랜더링
	//-------------------------------------------------------------------------
	if( g_wFrameSkipping & CFS_RENDER )
	{
		++g_iRenderFrameCount;

		SUNPROFILE_BEGIN( _T("Render") );

		SUNPROFILE_BEGIN(_T("SetViewport"));	

		m_GameFramework->SetViewport();

		SUNPROFILE_END(_T("SetViewport"));
		// 매 Frame 당 Render 처리

		SUNPROFILE_BEGIN(_T("BeginRender"));
		if( TRUE == g_pSunRenderer->BeginRender() )
		{
			if (g_pSunRenderer->IsReseted())
			{
				m_GameFramework->OnDeviceReset();
			}

			SUNPROFILE_END(_T("BeginRender"));
			
			//	각 Scene 랜더링
			m_GameFramework->Render( dwTick );	

			m_GameFramework->PreInterfaceRender( dwTick );

			SUNPROFILE_BEGIN( _T("RenderInterface") );
			// 인터페이스 랜더링
			g_InterfaceManager.Render();

			// Mouse Item Render
			MouseHandler::Instance()->Render();			

			m_GameFramework->PostInterfaceRender( dwTick );	

			SUNPROFILE_END( _T("RenderInterface") );

#ifdef MAKE_TEST_MODULE_MACRO
			TestModule::Instance()->TestStatusTextOut();
#endif //MAKE_TEST_MODULE_MACRO

#ifdef _DEV_VER
			_renderPerfInfo();
			CustomAssertion::Instance()->RenderAssertMessage();
#endif

			BOOL b2D,bRealSize;
			g_pSunRenderer->Check2DMode( &b2D, &bRealSize);
			if( b2D)
			{
				g_pSunRenderer->Set2DMode( FALSE);
			}

			g_pSunRenderer->SetAlphaTestMode(0);

	

			SUNPROFILE_BEGIN(_T("EndRender"));
			g_pSunRenderer->EndRender();
			SUNPROFILE_END(_T("EndRender"));

			m_GameFramework->OnEndRendering( dwTick );
		}
		else
		{
			SUNPROFILE_END(_T("BeginRender"));
		}		

		SUNPROFILE_END( _T("Render") );
	}


#ifdef __NA_0_20080415_PROCESS_END_PROECSS
	if( g_wFrameSkipping & CFS_MOVE )
	{
		m_GameFramework->OnEndProcess( dwTick );
	}
#endif


	//do
	{
		ISecureWorkNode* pWorkNode = CryptManager::GetISecureWorkNode();
		BYTE* pSendMsg;
		WORD wSendSize;
		if( pWorkNode )
		{
			pWorkNode->DoEarlyProcess( pSendMsg, wSendSize );
			if( wSendSize )
				GlobalFunc::SendPacket( CI_GAMESERVERIDX, pSendMsg, wSendSize );

			pWorkNode->DoLatelyProcess( pSendMsg, wSendSize );
			if( wSendSize )
				GlobalFunc::SendPacket( CI_GAMESERVERIDX, pSendMsg, wSendSize );
		}
		pWorkNode = NULL;
		pSendMsg = NULL;
		wSendSize = 0;
	}// while(0);

	ISecureWorkNode::Update( dwTick );

#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    {
        ////! KBMoveCheckManagerInClient::Instance()->CheckLocalTestRequest
        ////! 함수 내엣 자체적으로 tick를 계산하기때문에 특별히 tick를 계산하지 않는다.
        ////! 일정 간격(틈을 주고)으로 호출하기 위함
        //MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN syn;
        //if (KBMoveCheckManagerInClient::Instance()->CheckLocalTestRequest<1>(&syn) == true)
        //{
        //    GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));
        //}
    }
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

	return ( 0 );
}

void Application::PreloadResource()
{
#ifdef _DEBUG
    return;
#endif

	for (int a = 0; a < MAX_PRELOAD_RESOURCE; a++)
	{
		l_pPreloadResource[a] = ResourceManager::Instance()->AllocResourceUnitDraw(l_dwPreloadResource[a], -1.0f, false);
	}


    TextureTable& rTable = SpecialTexutureInfoParser::Instance()->GetTextrueInfo();
    TextureTable::iterator it = rTable.begin();
    
    for(;it!=rTable.end();++it)
    {
        Specail_Texture& dest = it->second;
        
        dest.hSpecialTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture(dest.szFilename,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);

        if (dest.hSpecialTexture  != INVALID_HANDLE_VALUE) 
        {
            g_pSunRenderer->x_pManagerTexture->AddSpecialTexture(dest.m_TextureID,dest.hSpecialTexture);
        }
        
    }
}

void Application::UnloadPreloadResource()
{
#ifdef _DEBUG
    return;
#endif

	for (int a = 0; a < MAX_PRELOAD_RESOURCE; a++)
	{	
		if (l_pPreloadResource[a]) 
		{
			ResourceManager::Instance()->FreeResourceUnitDraw(l_pPreloadResource[a]);
			l_pPreloadResource[a] = NULL;
		}		
	}	

    TextureTable& rTable = SpecialTexutureInfoParser::Instance()->GetTextrueInfo();
    TextureTable::iterator it = rTable.begin();

    for(;it!=rTable.end();++it)
    {
        Specail_Texture& dest = it->second;

        if (dest.hSpecialTexture  != INVALID_HANDLE_VALUE) 
        {
            g_pSunRenderer->x_pManagerTexture->RemoveSpecialTexture(dest.m_TextureID);
            g_pSunRenderer->x_pManagerTexture->DestroyTexture(dest.hSpecialTexture);
        }
    }

}

CDiscAccess * Application::GetDiscAccess()
{
	return m_pDiscAccess;
}
	
//------------------------------------------------------------------------------
/**
    Network recv callback function

    CI_GAMESERVERIDX: 게임 서버로부터 받은 패킷
    CI_CHATSERVERIDX: 채팅 서버로부터 받은 패킷
    CI_AUTHSERVERIDX: 인증 서버로부터 받은 패킷

    FIXME: 패킷이 서로다른 종류라는것은 알겠는데, 구분해서 받는 이유는?
*/
VOID NetworkRecv( DWORD dwIndex, BYTE * pData, DWORD dwSize )
{
//#if _DEV_VER
//	if(GlobalFunc::IsShowNetRecvLog())
//	{
//		TCHAR szText[1024] = {0,};
//
//		Sprintf(szText, _T("RECV : Server Idx : %d, Catagory : %d, Protocol : %d, Size: %d\n\r"), 
//			dwIndex, ((MSG_BASE *)pData)->m_byCategory, ((MSG_BASE *)pData)->m_byProtocol, dwSize );
//		
//		OutputDebugString(szText);
//	}
//
#ifdef _DEBUG //#ifdef _LTJ_PACKET_SHOW_FOR_DEBUG
    if ((g_pPacketShow) && GlobalFunc::IsShowPacketLog())
    {
        MSG_BASE* packet = (MSG_BASE*)pData;
        if( const CHAR* msg_str = g_pPacketShow->fnQuery(packet->m_byCategory, packet->m_byProtocol, NULL) )
        {
            // 너무 자주오는 패킷은 표시하지말자
            if( (packet->m_byCategory == CG_SYNC && packet->m_byProtocol == CG_SYNC_COMPOSED_STREAM_CMD ) == false
                && (packet->m_byCategory == CG_STATUS && packet->m_byProtocol == CG_STATUS_CHANGE_HP_BRD ) == false 
                )
                GlobalFunc::Write_Packet_Log("[RECV] %s\n", msg_str);

#if (WZENGINEVER >= 480)
            WZLOG( WZPKT1, msg_str );
#endif
        }
    }
#endif //_LTJ_PACKET_SHOW_FOR_DEBUG

	switch( dwIndex )
	{
        // NOTE: f110914.1L, CI_GAMESERVERIDX value same with CI_CHATSERVERIDX
        //since _NA002676_WORLD_SERVER_RENEWAL
	case CI_GAMESERVERIDX: //CI_CHATSERVERIDX (_NA002676_WORLD_SERVER_RENEWAL)
		{
			g_pApplication->m_GameFramework->ParsePacket( (MSG_BASE * )pData );
		}
		break;
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
	case CI_CHATSERVERIDX:
		{
			g_pApplication->m_GameFramework->ParsePacketInChat( (MSG_BASE *)pData );
		}
		break;
#endif
    case CI_AUTHSERVERIDX:
        {
            g_pApplication->m_GameFramework->ParsePacket ((MSG_BASE*)pData);
		}
        break;
	}
}


void CallBackLogOutChoice( bool b )
{
	if( b )
	{
		GlobalFunc::GoToServerList();
	}
	else
	{
		GlobalFunc::LogOutGame();
	}
}

VOID NetworkDisconnect( DWORD dwIndex )
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( dwIndex )
	{
	case CI_GAMESERVERIDX:
        if (g_isChangeServer == false)
        {
            g_InterfaceManager.GetInterfaceString( eST_GAMESERVER_DISCONNECT_MSG, szMessage, INTERFACE_STRING_LENGTH );
#ifdef _COMMON_CHANNELING_LOGIC_DEFINE
			GlobalFunc::SysMessageBox(szMessage, TRUE);
#else
    #ifdef _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
            if (uiLoginMan::IsUseParameterAuthInfo() == true)
            {
                GlobalFunc::SysMessageBox(szMessage, TRUE);
            }
            else
    #endif _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_
            {
                GlobalFunc::SysConfirmBox( szMessage, CallBackLogOutChoice, NULL, NULL, 99, 3, true );
            }
#endif //_COMMON_CHANNELING_LOGIC_DEFINE
			WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, _T("NetworkDisconnect Callback - GameServer"));
        }
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
		else
		{
            if (g_pNetwork)
            {
			    g_pNetwork->SetEnableSendChatHeartBeat(FALSE);
			    g_pNetwork->SetEnableSendHeartBeat(FALSE);
            }

			g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_LOGIN, 11001, 0);
			GameFramework::ChangeScene( SCENE_TYPE_LOAD );

			uiPartyMan* partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY);				
			if(partyMan && partyMan->IsParty())
			{
				partyMan->DestroyParty();
			}
		}
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__
		break;
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
	case CI_CHATSERVERIDX:
		{
			// 월드서버 프로토콜 암호화 정보 초기화
            if (g_pNetwork)
            {
			    g_pNetwork->InitCryptographyInfo( CI_CHATSERVERIDX );
            }

			// 서버 재접속
			if (g_isChangeServer == false)
			{
				g_pApplication->OnReconnectToWorldServer();
			}
		}
		break;
#endif
    case CI_AUTHSERVERIDX:
        {
			uiLoginMan* loginMan =
                static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
            
            if (loginMan && g_pNetwork && (g_pNetwork->IsRqstDisconnectAuth() == FALSE))
            {
                loginMan->SetTryReconnect();
            }
			
            
        }
        break;
	}	
}

VOID NetworkError( char * pszMsg )
{
#if defined _DEV_VER && !defined _DEBUG
	FILE * fp = fopen( "networkErrorMsg.txt", "a+t" );
	if( fp )
	{
		fwrite( pszMsg, strlen(pszMsg), 1, fp );
		fclose( fp );
	}
#endif
}


void Application::AddVirtualPacket(DWORD dwServerIndex,BYTE *pPacket,int iSize)
{
	VIRTUAL_PACKET *pVirtualPacket = new VIRTUAL_PACKET;

	pVirtualPacket->dwServerIndex = dwServerIndex;
	memcpy(pVirtualPacket->byData,pPacket,iSize);

	m_RecvVirtualPacket.push_back(pVirtualPacket);
}


void Application::ParseVirtualPacket()
{
	m_VirtualPacket.clear();

	std::copy(m_RecvVirtualPacket.begin(),m_RecvVirtualPacket.end(),std::back_insert_iterator< std::vector<VIRTUAL_PACKET*> >(m_VirtualPacket));

	m_RecvVirtualPacket.clear();

	vector<VIRTUAL_PACKET *>::iterator iter = m_VirtualPacket.begin();

	while (iter != m_VirtualPacket.end())
	{
		VIRTUAL_PACKET *pVirtualPacket = *iter;
		NetworkRecv(pVirtualPacket->dwServerIndex,pVirtualPacket->byData, 0);

		delete pVirtualPacket;
		iter++;
	};

	m_VirtualPacket.clear();
}


void Application::InitializeVirtualPacket()
{
	vector<VIRTUAL_PACKET *>::iterator iter = m_VirtualPacket.begin();

	while (iter != m_VirtualPacket.end())
	{
		VIRTUAL_PACKET *pVirtualPacket = *iter;
		delete pVirtualPacket;
		iter++;
	};

	m_VirtualPacket.clear();
}


void Application::ReleaseManager()
{
    TextureListInfoHandleManager::Instance()->RemoveAll();
    TextureListInfoHandleManager::Instance()->DestroyInstance();

	APPEARANCEMGR->DestroyInstance();
	ICONMGR()->Destroy();
	ICONMGR()->DestroyInstance();
	MAPRESMGR()->Destory();
	MAPRESMGR()->DestroyInstance();
}

#ifdef _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
#else //_NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT
BOOL Application::CheckOverlapExecute()
{
    m_hSemaphore = CreateSemaphore(NULL, 0, 1, _T("SUN_GAME") );	
    if ( m_hSemaphore != NULL && GetLastError() & ERROR_ALREADY_EXISTS )
    {
        CloseHandle( m_hSemaphore );
        m_hSemaphore = NULL;
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
#endif //_NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT


void Application::ProcessEffectSound()
{
	
	if (g_pEffectManager) 
	{
        CSoundInfoManager* pSndMan = g_pEffectManager->GetSoundInfoManager();
        SUN_ASSERT(pSndMan);

        if (!pSndMan)
        {
            return;
        }

        while (!pSndMan->IsEmpty())
        {
            St_SoundInfo* pSound = pSndMan->Pop();

			if (pSound)
			{
				switch (pSound->m_eSMassage)
				{
				case ENUM_START_EVENT:
					{
	#if !defined(_DEBUG)
						// 디버그에서는 이걸 플레이해서 잡아보자.
						if (2550 != pSound->m_nSoundID)
	#endif
						{
							pSound->m_nResultHandle = g_SoundEffect.Play(pSound->m_nSoundID,&pSound->m_wvPos);
						}
					}
					break;
				case ENUM_STOP_EVENT:
					{
						wzsnd_stop(pSound->m_nResultHandle);
					}
					break;
				case ENUM_UPDATE_POS_EVENT:
					{
						D3DVECTOR VecPos, VecVeclocity;
						VecPos.x = pSound->m_wvPos.x;
						VecPos.y = pSound->m_wvPos.z;
						VecPos.z = pSound->m_wvPos.y;
						
						VecVeclocity.x = 0.0f;
						VecVeclocity.y = 0.0f;
						VecVeclocity.z = 0.0f;
						
						wzsnd_update_3d_source(
							pSound->m_nResultHandle,
							&VecPos,
							&VecVeclocity
							);
					}
					break;
				}
			}
			
		}
	}
}

void Application::CreateConsole()
{
#ifdef _DEBUG
	AllocConsole();
	DWORD fdwMode, fdwSaveOldMode; 

	m_hConsole = GetStdHandle(STD_INPUT_HANDLE); 
	if (m_hConsole != INVALID_HANDLE_VALUE) 
	{
		if (! GetConsoleMode(m_hConsole, &fdwSaveOldMode) ) 
		{
			fdwMode = ENABLE_WINDOW_INPUT; 
			SetConsoleMode(m_hConsole, fdwMode);
		}
	}
#endif
}

void Application::ProcessConsole()
{
	 DWORD cNumRead, i; 
    INPUT_RECORD irInBuf[128]; 


	if (m_hConsole == INVALID_HANDLE_VALUE)
	{
		return;
	}

	if (ReadConsoleInput(m_hConsole,irInBuf,128, &cNumRead) ) 
	{
		_cprintf("%d\n",cNumRead);

		for (i = 0; i < cNumRead; i++) 
		{
			switch(irInBuf[i].EventType) 
			{ 
			case KEY_EVENT: // keyboard input 
				__asm nop;
				break; 
			case MOUSE_EVENT: // mouse input 		
				break; 
			case WINDOW_BUFFER_SIZE_EVENT: 
				break; 
			case FOCUS_EVENT:  // disregard focus events 
			case MENU_EVENT:   // disregard menu events 
				break; 
			default: 				
				break; 
			} 
		}

	}
}

void Application::_renderPerformanceStatus()
{
#ifdef _DEV_VER

	if (!g_pSunRenderer)
	{
		return;
	}

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "mi16" );

	LARGE_INTEGER Freq;
	QueryPerformanceFrequency(&Freq);
	int y = 50;
	int x = 600;

	// FPS
	DWORD dwTick;
	DWORD dwTicks2;
	DWORD dwTick3;
	LONG lCount, lPpsCount, lDcpsCount;
	g_pSunRenderer->GetFpsValues( &dwTick, &lCount );
	g_pSunRenderer->GetPpsValues( &dwTicks2, &lPpsCount);
	g_pSunRenderer->GetDcpsValues( &dwTick3, &lDcpsCount);

	TCHAR lpszText[512] = {0,};

	Snprintf( lpszText, 512-1, _T("%ld(M) Availabletexmem"),g_pSunRenderer->GetAvailableTextureMem() / (1024*1024));
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("%ld(M) UsedTextureMem"),g_pSunRenderer->GetUsedTextureMem() / (1024*1024));
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;
	
	float fUsedMem=( float)g_pSunRenderer->GetUsedTextureMem() / 1024.0f;
	float fRemainMem=( float)g_pSunRenderer->GetAvailableTextureMem()/ 1024.0f;

	float fTotalMem= fRemainMem+fUsedMem;
	float fPercent=(fUsedMem/fTotalMem)*100.0f;


	TCHAR	string[INTERFACE_STRING_LENGTH];
	// 총메모리:%d 퍼센트:%d 6301
	g_InterfaceManager.GetInterfaceString( 6301, string, INTERFACE_STRING_LENGTH );
	Snprintf( lpszText, 512-1, string, (DWORD)fTotalMem/(1024*1024),(DWORD)fPercent );
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );

	if(fTotalMem<=300000.0f)
	{
		y += 20;
		// 비디오 메모리 128MB 6302
		g_InterfaceManager.GetInterfaceString( 6302, string, INTERFACE_STRING_LENGTH );
		g_pSunRenderer->x_pManagerTextOut->DrawText( string, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	}

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();

#endif
}

void Application::_renderProfile()
{	
#ifdef _DEV_VER
	if (!g_pSunRenderer)
	{
		return;
	}

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "mi16" );
	g_Profiler.GenerateResult();
	
	// profile 결과
	for ( int i = 0; i < g_Profiler.GetNumSample(); ++i)
	{
		if(0 == m_iProfile[i])
		{
			g_Profiler.GetResultText( i, m_szProfile[i]);
		}
		int iDepth = g_Profiler.GetDepth(i);
		g_pSunRenderer->x_pManagerTextOut->DrawText( m_szProfile[i], 600 + 20 * iDepth, 50 + i * 20, WzColor_RGB( 0x0F, 255, 0x0f ));

		if(5 < m_iProfile[i])
			m_iProfile[i] = 0;
		else
			m_iProfile[i]++;
	}
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
#endif 
}

void Application::_renderProcessProfile()
{	
#ifdef _DEV_VER
	if (!g_pSunRenderer)
	{
		return;
	}

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( "mi16" );
	g_ProcessProfiler.GenerateResult();
	
	// profile 결과
	for ( int i = 0; i < g_ProcessProfiler.GetNumSample(); ++i)
	{
		if(0 == m_iProfile[i])
		{
			g_ProcessProfiler.GetResultText( i, m_szProfile[i]);
		}
		int iDepth = g_ProcessProfiler.GetDepth(i);
		g_pSunRenderer->x_pManagerTextOut->DrawText( m_szProfile[i], 600 + 20 * iDepth, 50 + i * 20, WzColor_RGB( 0x0F, 255, 0x0f ));

		if(5 < m_iProfile[i])
			m_iProfile[i] = 0;
		else
			m_iProfile[i]++;
	}
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
#endif 
}

void Application::_renderPerfInfo()
{
	if (GENERALPARAM->GetSpecialMode() >= eSPECIAL_MODE_STEP2)
	{		
		switch(m_iDebugInfo)
		{
		case 1:
			_renderPerformanceStatus();
			break;

		case 2:
			_renderProfile();
			break;

		case 3:
			_renderProcessProfile();
			break;
		}
		return;
	}
}

//------------------------------------------------------------------------------ 
POINT Application::Get2DModeMapping( void )
{
    POINT pt;

#if WZENGINEVER >= 620
    pt.x = g_pSunRenderer->Get2DModeWidth();
    pt.y = g_pSunRenderer->Get2DModeHeight();
#else // WZENGINEVER >= 620

    if (screen_ratio() == SR_4_3)
    {
        pt.x = static_cast<LONG>(STD_2D_WIDTH);    
        pt.y = static_cast<LONG>(STD_2D_HEIGHT);
    }
    else if (screen_ratio() == SR_16_9)
    {
        pt.x = static_cast<LONG>(STD_2D_WIDTH_WIDE);    
        pt.y = static_cast<LONG>(STD_2D_HEIGHT_WIDE);
    }
    else //if (screen_ratio() == SR_16_10)
    {
        pt.x = static_cast<LONG>(STD_2D_WIDTH_HALF_WIDE);    
        pt.y = static_cast<LONG>(STD_2D_HEIGHT_HALF_WIDE);
    }

#endif // WZENGINEVER >= 620

    return pt;
}

void Application::OnChangeRes(POINT *newsize)
{

	POINT pt = Get2DModeMapping();

	m_ScreenSize.x = newsize->x;
	m_ScreenSize.y = newsize->y;

	float fWidth = pt.x / (float)m_ScreenSize.x;
	float fHeight = pt.y / (float)m_ScreenSize.y;

	Mouse::Instance()->SetScreenRes( fWidth, fHeight );

}

//------------------------------------------------------------------------------
/**
    월드서버로 부터 끊겼을 경우 c_iRECONNECT_INVERVAL 후에 재접속을 시도한다.
*/
void
Application::OnReconnectToWorldServer()
{
    if (this->m_bCheckToWorldServerReconnect)
        return;

    if (g_pNetwork && (g_pNetwork->IsConnected(CI_GAMESERVERIDX) == FALSE))
    {
        return;
    }
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    if (g_pNetwork && (g_pNetwork->IsConnected(CI_CHATSERVERIDX) == TRUE))
    {
        return;
    }

    this->m_iTryCountToWroldServerReconnect = 0;

    if (g_pNetwork)
    {
        g_pNetwork->SetEnableSendChatHeartBeat(FALSE);
    }

    this->m_bCheckToWorldServerReconnect = true;
    this->m_dwStartTickToWorldServerReconnect = this->m_dwCurrentTick;
    this->m_dwOverTickToWorldServerReconnect = this->m_dwStartTickToWorldServerReconnect + GENERALPARAM->GetIntervalToWorldServerReconnect();
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
Application::OffReconnectToWorldServer()
{
    this->m_bCheckToWorldServerReconnect = false;
}

//------------------------------------------------------------------------------
/**
*/
void
Application::CheckReconnectToWorldServer()
{
    if (!this->m_bCheckToWorldServerReconnect)
    {
        return;
    }
    if (g_CharSelectScene.m_bConnected == false)
    {
        return;
    }

    if (g_pNetwork && (g_pNetwork->IsConnected(CI_GAMESERVERIDX) == FALSE))
    {
        return;
    }
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    if (g_pNetwork && (g_pNetwork->IsConnected(CI_CHATSERVERIDX) == TRUE))
    {
        this->OffReconnectToWorldServer();
        return;
    }

    if (this->m_dwCurrentTick >= this->m_dwOverTickToWorldServerReconnect)
    {
        this->m_dwStartTickToWorldServerReconnect = this->m_dwCurrentTick;
        this->m_dwOverTickToWorldServerReconnect = this->m_dwStartTickToWorldServerReconnect + GENERALPARAM->GetIntervalToWorldServerReconnect();

        MSG_CG_PREPARE_WORLD_CONNECT_SYN SendPacket;
        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );

        this->m_iTryCountToWroldServerReconnect++;
    }
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
Application::ForceReconnectToWorldServer()
{
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
    if (g_pNetwork)
    {
        g_pNetwork->Disconnect(CI_CHATSERVERIDX,TRUE);
    }
#endif
}

//------------------------------------------------------------------------------
/**
*/
void
Application::ProcessDebugInput()
{
#ifdef _DEV_VER
	if (g_Input.GetState(DIK_F3, KS_DOWN))
	{
		m_iDebugInfo++;
		if (m_iDebugInfo >= 4)
		{
			m_iDebugInfo = 0;
		}
	}

	if (g_Input.GetState(DIK_F4, KS_DOWN))
	{
		CustomAssertion::Instance()->ToggleAssertMssage();
	}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    float pitch = 0.0f;
    if (is_slow_)
    {
        pitch = 0.001f;
    }
    else
    {
        pitch = 0.01f;
    }

    if (g_Input.GetState(DIK_F6, KS_DOWN))
    {
        is_slow_ = !is_slow_;
    }
    //else if ((!is_slow_ && g_Input.GetState(DIK_HOME, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_HOME, KS_DOWN)))
    //{
    //   g_CharSelectScene.m_camera.y += pitch;
    //   g_CharSelectScene.m_target.y += pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_END, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_END, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_camera.y -= pitch;
    //    g_CharSelectScene.m_target.y -= pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_NEXT, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_NEXT, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_camera.x += pitch;
    //    g_CharSelectScene.m_target.x += pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_DELETE, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_DELETE, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_camera.x -= pitch;
    //    g_CharSelectScene.m_target.x -= pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_PRIOR, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_PRIOR, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_camera.z += pitch;
    //    g_CharSelectScene.m_target.z += pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_INSERT, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_INSERT, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_camera.z -= pitch;
    //    g_CharSelectScene.m_target.z -= pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_W, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_W, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_character.y += pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_S, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_S, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_character.y -= pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_D, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_D, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_character.x += pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_A, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_A, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_character.x -= pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_Q, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_Q, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_character.z += pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_Z, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_Z, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_character.z -= pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_NUMPAD8, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_NUMPAD8, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_target.y += pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_NUMPAD2, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_NUMPAD2, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_target.y -= pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_NUMPAD6, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_NUMPAD6, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_target.x += pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_NUMPAD4, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_NUMPAD4, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_target.x -= pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_NUMPAD9, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_HOME, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_target.z += pitch;
    //}
    //else if ((!is_slow_ && g_Input.GetState(DIK_NUMPAD3, KS_KEY)) || 
    //    (is_slow_ && g_Input.GetState(DIK_NUMPAD3, KS_DOWN)))
    //{
    //    g_CharSelectScene.m_target.z -= pitch;
    //}
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP

#ifdef _DEBUG
    if (g_Input.GetState(DIK_PAUSE, KS_DOWN))   
    {
        GlobalFunc::TogglePacketLog();
    }

#endif

#endif
}

void 
Application::UpdateGlobalVariable()
{
	g_fSpecularVariation = 1.0f + sin(g_CurTime * 0.003f);
}

void Application::Set2DMode(BOOL bFlag)
{
    g_pSunRenderer->Set2DMode(bFlag);
}
//------------------------------------------------------------------------------ 
BOOL Application::IsWideModeFlag()
{
    return (screen_ratio() != SR_4_3);
}
//------------------------------------------------------------------------------ 
ENUM_SCREEN_RATIO Application::screen_ratio()
{
    //!< SR_4_3     : 0.75f     : 1.3333333333333333333333333333333f
    //!< SR_16_10   : 0.625f    : 1.6f
    //!< SR_16_9    : 0.5625f   : 1.7777777777777777777777777777778f

    if (g_pSunRenderer->GetCameraInfo()->m_fAspect <= 1.4f)  
    {
        return SR_4_3;
    }
    else if (g_pSunRenderer->GetCameraInfo()->m_fAspect <= 1.65f) 
    {
        return SR_16_10;
    }
    return SR_16_9;
}

#ifdef GGUO_CRASH_DUMP
#ifdef _DH_DEBUG_REPORT
typedef BOOL (WINAPI * fnMINIDUMPWRITEDUMP)(
    HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
    CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
    );

MINIDUMP_TYPE	Application::s_eDumpType			= MiniDumpNormal;
TCHAR			Application::s_pszDumpFileDir[]	= {0,};
TCHAR			Application::s_pszFilePrefix[]	= {0,};
TCHAR			Application::s_pszBuildVersion[]  = _T("0000");

TCHAR szFile1[256], szFile2[256], szFile3[256], szFile4[256], szFile5[256],szFile6[256];
char szDumpPath[MAX_PATH*2] = {0,};
char szDumpIamgePath[MAX_PATH*2] = {0,};
TCHAR crash_message[3][1024] = {0,};
static _EXCEPTION_POINTERS* exception_info_ptr = NULL;
_TCHAR version_string[64] = {0,};

_TCHAR* GetVersion(_TCHAR* filename)
{
    _TCHAR* string_buffer;
    const int kVersion_Depth = 15;      //strlen("120104_0410.dmp")

    string_buffer = _tcsstr(_tcslwr(filename), "sungame_");

    if (string_buffer)
    {
        int string_length = _tcslen(string_buffer);
        if (string_length - kVersion_Depth >= 0)
        {
            _TCHAR* string_pointer = string_buffer + (string_length - kVersion_Depth);
            sprintf(version_string, "webzen@%d", _tstoi(string_pointer));

            return version_string;
        }

        //::MessageBox(NULL, _T(version_string), _T("Message"), MB_OK);
        //printf(version_string);
        /*printf("Version (%s)\n", version_string);*/
    }

    sprintf(version_string, "webzen@version");
    return version_string;
}

static INT CreateMiniDump( struct _EXCEPTION_POINTERS * pExceptionInfo )
{
    LONG rt = EXCEPTION_CONTINUE_SEARCH;

    if (exception_info_ptr == NULL)
    {
        return rt;
    }

    // 현재 경로에서 dll로드
    const TCHAR* dbghelp_name = _T("dbghelp.dll");
    HANDLE hDLL = ::LoadLibrary(dbghelp_name);

    if( NULL == hDLL )
    {
        assert(!"DBGHELP.DLL을 로드할 수 없습니다." );
        return rt;
    }

    fnMINIDUMPWRITEDUMP pfnMiniDumpWriteDump = (fnMINIDUMPWRITEDUMP)GetProcAddress( (HMODULE)hDLL, "MiniDumpWriteDump" );
    if( NULL == pfnMiniDumpWriteDump )
    {
        assert(!"오래된 버전의 DBGHELP.DLL입니다. 최신버전으로 업데이트 하시기 바랍니다." );
        return rt;
    }

    HANDLE hFile = CreateFile( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( INVALID_HANDLE_VALUE == hFile )
    {
        assert(!"[dump]파일을 열 수 없습니다.");
        return rt;
    }

    MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId				= GetCurrentThreadId();
    eInfo.ExceptionPointers		= pExceptionInfo;
    eInfo.ClientPointers		= FALSE;

    MINIDUMP_TYPE dump_type = MiniDumpNormal;
    if (GENERALPARAM->GetSendDumpType() == 1)
    {
        dump_type = MiniDumpWithDataSegs;
    }

    BOOL bResult = pfnMiniDumpWriteDump( GetCurrentProcess(), 
        GetCurrentProcessId(), 
        hFile, 
        dump_type, 
        (pExceptionInfo?&eInfo:NULL), 
        NULL, NULL);

    if( !bResult )
    {
        assert(!"MiniDumpWriteDump() 호출이 실패했습니다." );
        CloseHandle(hFile);
        return ( rt = EXCEPTION_EXECUTE_HANDLER );
    }
    FlushFileBuffers(hFile);
    CloseHandle(hFile);

    hFile = CreateFile(szDumpPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if( INVALID_HANDLE_VALUE == hFile)
    {
        assert(!"[dump]파일을 열 수 없습니다.");
        return rt;
    }
    DWORD file_size = 0;
    DWORD read_size = 0;
    DWORD write_size = 0;

    file_size = GetFileSize(hFile, NULL);

    char* buffer = new char[file_size];
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    ReadFile(hFile, buffer, file_size, &read_size, NULL);
    FlushFileBuffers(hFile);
    CloseHandle(hFile);

    TCHAR pass_key[64] = "webzen@version";//webzen@120100 이런식 파일이름이용
    sprintf(pass_key, GetVersion(szDumpPath));
    MD5Encrypt* md5 = new MD5Encrypt();
    md5->EncryptString(pass_key, buffer, (unsigned int)file_size);
    if (md5 != NULL)
    {
        delete md5;
        md5 = NULL;
    }

    hFile = CreateFile(szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    if( INVALID_HANDLE_VALUE == hFile)
    {
        assert(!"[dump]파일을 열 수 없습니다.");
        if (buffer != NULL)
        {
            delete [] buffer;
        }
        return rt;
    }
    WriteFile(hFile, buffer, file_size, &write_size, NULL);
    CloseHandle(hFile);
    //CloseHandle(hMD5File);

    if (buffer != NULL)
    {
        delete [] buffer;
    }
    return rt;
}

static INT CreateFullDump( struct _EXCEPTION_POINTERS * pExceptionInfo )
{
    LONG rt = EXCEPTION_CONTINUE_SEARCH;

    if (pExceptionInfo == NULL)
    {
        return rt;
    }

    // 현재 경로에서 dll로드
    const TCHAR* dbghelp_name = _T("dbghelp.dll");
    HANDLE hDLL = ::LoadLibrary(dbghelp_name);

    if( NULL == hDLL )
    {
        assert(!"DBGHELP.DLL을 로드할 수 없습니다." );
        return rt;
    }

    fnMINIDUMPWRITEDUMP pfnMiniDumpWriteDump = (fnMINIDUMPWRITEDUMP)GetProcAddress( (HMODULE)hDLL, "MiniDumpWriteDump" );
    if( NULL == pfnMiniDumpWriteDump )
    {
        assert(!"오래된 버전의 DBGHELP.DLL입니다. 최신버전으로 업데이트 하시기 바랍니다." );
        return rt;
    }


    TCHAR file_prefix[MAX_PATH]	= {0,};
    TCHAR file_name[MAX_PATH] = {0,};
    GetModuleFileName(NULL, file_name, MAX_PATH);

    TCHAR* slash = strrchr(file_name, '\\');
    TCHAR* dot = strrchr(slash+1, '.');
    lstrcpyn(file_prefix, slash+1, (int)(dot - slash));

    TCHAR build_version[MAX_PATH]  = _T("0000");
    lstrcpyn(build_version, c_tcBUILD_VERSION, MAX_PATH);

    struct tm* today;
    time_t ltime;
    time(&ltime);
    today = localtime(&ltime);

    char dump_path[MAX_PATH*2];
    wsprintf(dump_path, 
        ".\\%s%02d-%02d-%02d=%02dh%02dm%02ds_%s.dmp",
        file_prefix,
        today->tm_year+1900,
        today->tm_mon+1,
        today->tm_mday,
        today->tm_hour,
        today->tm_min,
        today->tm_sec,
        build_version);

    if( FILE* fp = fopen("naklutest.txt","a+") )
    {
        fprintf(fp,"%s\n",dump_path);
        fclose(fp);
    }
    HANDLE hFile = CreateFile( dump_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( INVALID_HANDLE_VALUE == hFile )
    {
        assert(!"[dump]파일을 열 수 없습니다.");
        return rt;
    }

    MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId				= GetCurrentThreadId();
    eInfo.ExceptionPointers		= pExceptionInfo;
    eInfo.ClientPointers		= FALSE;

    BOOL bResult = pfnMiniDumpWriteDump( GetCurrentProcess(), 
        GetCurrentProcessId(), 
        hFile, 
        MiniDumpWithFullMemory, 
        (pExceptionInfo?&eInfo:NULL), 
        NULL, NULL);

    if( !bResult )
    {
        assert(!"MiniDumpWriteDump() 호출이 실패했습니다." );
        CloseHandle(hFile);
        return ( rt = EXCEPTION_EXECUTE_HANDLER );
    }
    FlushFileBuffers(hFile);
    CloseHandle(hFile);

    return rt;
}

DWORD crash_report_strings[3] = { IDS_REPORT_STRING_FOREIGN, 0, 0 };
BOOL WINAPI CrashCallbackBefore()
{
    /*5641	Soul of the Ultimate Nation에 문제가 있어 프로그램을 종료합니다.\r\n불편을 끼쳐드려 대단히 죄송합니다.\r\n\r\n오류를 보고하시면, 빠른 문제 해결에 도움이 될 수 있습니다.\r\n오류를 보고 하시겠습니까?
    5642	오류를 보고해주셔서 대단히 감사합니다.\r\n빠른 해결을 위해 최선을 다하겠습니다.\r\n\r\n<Soul of the Ultimate Nation>
    5643	오류가 발생한 상황에 대한 자세한 내용을 기재하여 주십시오.\r\n내용을 상세히 기재해주실 수록 빠른 해결에 도움이 됩니다.
    */
    /*
    5641	The program is shutting down because of a problem with Soul of the Ultimate Nation.\r\nWe apologize for any inconvenience.\r\n\r\nBy reporting this problem, you can help us solve it as soon as possible.\r\nDo you want to report the error?
    5642	Thank you for reporting this error.\r\nWe will do our best to solve this problem as soon as possible.\r\n\r\n<Soul of the Ultimate Nation>
    5643	Please enter as much information about the error as you can.\r\nThe more detailed the information is, the more it will help us fix the problem.
    */

#ifdef	_KOREA
    crash_report_strings[0] = IDS_REPORT_STRING_KOR;
    LoadString( NULL, crash_report_strings[0], crash_message[0], 1024 );
#elif defined(_JAPAN)
    //crash_report_strings[0] = IDS_REPORT_STRING_JP;
    //LoadString( NULL, crash_report_strings[0], crash_message[0], 1024 );
    
    //일본 멀티 바이트가 깨져서 아스키 코드로 강제 입력하여 출력 하도록 수정
    //5641
    const char japan_text[] = {83,111,117,108,32,111,102,32,116,104,101,32,85,108,116,105,109,97,116,101,
        32,78,97,116,105,111,110,-126,-55,-106,-30,-111,-24,-126,-86,-108,-83,-112,-74,-126,-75,-127,
        65,-125,118,-125,-115,-125,79,-125,-119,-125,-128,-126,-16,-113,73,-105,-71,-126,-75,-126,-36,
        -126,-73,-127,66,13,10,-126,-78,-106,-64,-104,102,-126,-16,-126,-88,-126,-87,-126,-81,
        -126,-75,-112,-67,-126,-55,-112,92,-126,-75,-106,-13,-126,-96,-126,-24,-126,-36,-126,-71,-126,
        -15,-127,66,13,10,13,10,-125,71,-125,-119,-127,91,-126,-16,-111,-105,-112,77,
        -126,-73,-126,-23,-126,-58,-106,-30,-111,-24,-119,-16,-116,-120,-126,-55,-106,-16,-105,-89,
        -126,-65,-126,-36,-126,-73,-127,66,13,10,-125,71,-125,-119,-127,91,-126,-16,-111,
        -105,-112,77,-126,-75,-126,-36,-126,-73,-126,-87,-127,66,0};

    sprintf(crash_message[0], japan_text);
    /*5642
        -125,71,-125,-119,-127,91,-126,-52,-111,-105,-112,77,-112,-67,-126,-55,-126,-96,-126,-24,-126,-86,-126,-58,-126,-92,-126,-78,-126,-76,-126,-94,-126,-36,-126,-73,-127,66,92,114,92,110,-111,-127,-126,-94,-119,-16,-116,-120,-126,-52,-126,-67,-126,-33,-126,-55,-115,-59,-111,80,-126,-16,-112,115,-126,-83,-126,-75,-126,-36,-126,-73,-127,66,92,114,92,110,92,114,92,110,60,83,111,117,108,32,111,102,32,116,104,101,32,85,108,116,105,109,97,116,101,32,78,97,116,105,111,110,62,
    */
    /*5643
        -125,71,-125,-119,-127,91,-126,-86,-108,-83,-112,-74,-126,-75,-126,-67,-113,-13,-117,-75,-126,-55,-126,-62,-126,-94,-126,-60,-113,-38,-126,-75,-126,-94,-109,-32,-105,101,-126,-16,-113,-111,-126,-94,-126,-60,-126,-83,-126,-66,-126,-77,-126,-94,-127,66,92,114,92,110,-113,-38,-115,-41,-126,-56,-109,-32,-105,101,-126,-16,-113,-111,-126,-94,-126,-60,-126,-94,-126,-67,-126,-66,-126,-81,-126,-23,-126,-58,-111,-127,-126,-94,-119,-16,-116,-120,-126,-55,-116,113,-126,-86,-126,-24,-126,-36,-126,-73,-127,66,
    */
#else
    crash_report_strings[0] = IDS_REPORT_STRING_FOREIGN;
    LoadString( NULL, crash_report_strings[0], crash_message[0], 1024 );
#endif

    int result = ::MessageBox(NULL, _T(crash_message[0]), 
        _T("Sungame.exe"), MB_YESNO|MB_ICONERROR);

    if (result == IDYES)
    {
        //CreateMiniDump(exception_info_ptr);
        //::MessageBox(NULL, _T(crash_message[1]), _T("Message"), MB_OK);
        Crash::AddFile(szFile1);
        Crash::AddFile(szFile2);
        Crash::AddFile(szFile3);
        Crash::AddFile(szFile4);
        Crash::AddFile(szFile5);
        Crash::AddFile(szFile6);
    }


    return TRUE;
}

BOOL WINAPI CrashCallbackAfter()
{
    //::MessageBox(NULL, _T("After handle exception"), _T("Message"), MB_OK);

    //생성한 파일 삭제
    //remove(szDumpPath);

    return TRUE;
}

typedef union _BYTES
{
    DWORD dowrd;
    BYTE bytes[4];
}DWORD_TO_BYTES;

LONG WINAPI Application::ExceptionFilter( struct _EXCEPTION_POINTERS * pExceptionInfo )
{
    LONG rt = EXCEPTION_CONTINUE_SEARCH;


    // version.ini 를 통하여 풀덤프 생성 여부를 설정
    TCHAR ini_file_path[MAX_PATH] = {0, };
    TCHAR string_value[MAX_PATH] = {0, };
    GetCurrentDirectory(MAX_PATH, ini_file_path);
    _tcscat(ini_file_path, _T("\\version.ini"));

    GetPrivateProfileString("DUMPTYPE", "DUMP_TYPE", "none",
        string_value, MAX_PATH, ini_file_path);
    
    if (_tcscmp(string_value, _T("MAKE_FULL_DUMP")) == 0)
    {
        CreateFullDump(pExceptionInfo);
        return 1;
    }


    exception_info_ptr = pExceptionInfo;

    // temp 폴더 주소 만들기 =================================
    LPSTR path_string_ptr;
    TCHAR string[1024] = {0,};
    DWORD_TO_BYTES data[2];
    DWORD string_address = (DWORD)(void*)string;
    data[0].dowrd = string_address;
    string_address = (DWORD)(void*)exception_info_ptr;
    data[1].dowrd = string_address;

    path_string_ptr = string;
    GetTempPath(1024, path_string_ptr);

    Snprintf(s_pszDumpFileDir, MAX_PATH-1, "%s{%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X}\\", path_string_ptr, 
        data[0].bytes[0], data[1].bytes[0], data[0].bytes[1], data[1].bytes[1],
        data[0].bytes[2], data[1].bytes[2], data[0].bytes[3], data[1].bytes[3]);
    //========================================================
    //lstrcpyn(s_pszDumpFileDir, "%s", MAX_PATH);
    //lstrcpyn(s_pszDumpFileDir, ".", MAX_PATH);
    TCHAR szFilename[MAX_PATH];
    GetModuleFileName( NULL, szFilename, MAX_PATH);

    TCHAR * slash = strrchr( szFilename, '\\' );
    TCHAR * dot = strrchr( slash+1, '.' );
    ::lstrcpyn( s_pszFilePrefix, slash+1, (int)(dot - slash) );

    TCHAR szClientVersion[64], szHttpUrl[256], szFtpUrl[256], szFtpID[256], szFtpPW[256];
    int nFtpPort, nProjectID, nCount;
    bool bCumulative = true;
    bool bDeleteDumpfile = true;
    bool bStackTrace = false;

    sprintf(szClientVersion, TEXT(s_pszBuildVersion));
    sprintf(szHttpUrl, TEXT("202.14.103.223:10080"));
    sprintf(szFtpUrl, TEXT("202.14.103.223"));
    sprintf(szFtpID, TEXT("webzen"));
    sprintf(szFtpPW, TEXT("webFTP!34"));
    nFtpPort = 10021;
    nProjectID = 14;
    nCount = 3;

    struct tm * today;
    time_t		ltime;
    time( &ltime );
    today = localtime( &ltime );
    
    wsprintf(s_pszBuildVersion, c_tcBUILD_VERSION);
    wsprintf(szDumpPath, "%s%s_%s.dmp", s_pszDumpFileDir, s_pszFilePrefix, s_pszBuildVersion);
    sprintf(szFile1, szDumpPath);//dmp 파일 추가

    // addfile는 \\ 가 아닌 / 을 사용한다.
    int i = 0;
    while(szFile1[i] != '\0')
    {
        if (szFile1[i] == '\\')
        {
            szFile1[i] = '/';
        }
        ++i;
    }
    //sprintf(szFile1, "CrashReport.dmp");//dmp 파일 추가


    CreateDirectory(s_pszDumpFileDir, NULL);
    CreateMiniDump(exception_info_ptr);

    if (!Crash::Install(
        CrashCallbackBefore,
        CrashCallbackAfter,
        nProjectID,
        szClientVersion,
        MiniDumpNormal,
        szHttpUrl,
        szFtpUrl,
        nFtpPort,
        szFtpID,
        szFtpPW,
        nCount,
        bCumulative,
        bDeleteDumpfile,
        bStackTrace,
        szDumpPath))
    {
        return 1;
    }

#ifdef _NA_000000_20130426_DUMPTOSCREENSHOT
    wsprintf(szFile6, "%s%s_%s.jpg", s_pszDumpFileDir, s_pszFilePrefix, s_pszBuildVersion);
    //해당 덤프파일명으로 파일 저장.
    if(g_pSunRenderer)
    {
        g_pSunRenderer->SaveScreen( szFile6 );
    }

    i = 0;
    while(szFile6[i] != '\0')
    {
        if (szFile6[i] == '\\')
        {
            szFile6[i] = '/';
        }
        ++i;
    }
#endif


    static BOOL first = FALSE;
    if (first)
    {
        return rt;
    }
    first = TRUE;

    __try 
    {
        RaiseException( STILL_ACTIVE, 0, 0, NULL );
    } 
    __except( Crash::HandleExceptionWithInfo(GetExceptionInformation()) ) 
    {
        Crash::Uninstall();
        remove("CrashLog.txt");
        remove(szDumpPath);
        remove(szFile6);
        remove(szDumpIamgePath);
        RemoveDirectory(s_pszDumpFileDir);

        return 1;// 오류 보고창을 뜨지 않게 함
    }

    return rt;
}
#else//_DH_DEBUG_REPORT

LONG WINAPI		Application::ExceptionFilter( struct _EXCEPTION_POINTERS * pExceptionInfo )
{
	LONG rt = EXCEPTION_CONTINUE_SEARCH;

	if( pExceptionInfo == NULL )
	{
		static BOOL first = FALSE;
		if( first ) return rt;
		first = TRUE;

		__try 
		{
			RaiseException( EXCEPTION_BREAKPOINT, 0, 0, NULL );
		} 
		__except( ExceptionFilter( GetExceptionInformation() ), EXCEPTION_CONTINUE_EXECUTION ) 
		{

		}
		return rt;
	}
	else
	{

		St_SystemAndDriverInfo sysInfo;
		GetSystemAndDriverInfo("3DRendererD3D.dll", &sysInfo);
		
		TCHAR szFileName[INTERFACE_STRING_LENGTH] = {0,};
		GlobalFunc::GetCrashReportFileName(szFileName);
		SetFileAttributes( szFileName, FILE_ATTRIBUTE_NORMAL );


		

		TCHAR lpszParamBuffer[16] = {0,};
		//0: 보내기 전, 1:보냄, -1: 보내기 싫음, -2: Default
		WritePrivateProfileString( _T("SEND_REPORT"), _T("Send"), ParamInt2Str( 0, lpszParamBuffer), szFileName);

		FILE *fp = fopen("MiniCrashDump.txt","wt");

		if (fp)
		{

			DWORD exceptionAddress = (DWORD)pExceptionInfo->ExceptionRecord->ExceptionAddress;	


			TCHAR szValue[MAX_PATH] = {0,};
			Sprintf(szValue, _T("%ld"), exceptionAddress);
			WritePrivateProfileString( _T("CRASH_INFO"), _T("CrashAddress"), szValue, szFileName);

			// 일단 뻗은 주소를 기록~
			fprintf(fp,"Crash Address : %X\n",exceptionAddress);


			MODULEENTRY32 me32;

			ZeroMemory(&me32,sizeof(me32));
			me32.dwSize = sizeof(me32);

			// 어느 모듈인지 찾을수 있으면 모듈 정보도 기록한다.
			HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());

			if (hModuleSnap != (HANDLE)-1)
			{
				if (Module32First(hModuleSnap,&me32))
				{
					do
					{
						DWORD dwModStart = (DWORD)me32.modBaseAddr;
						DWORD dwModEnd = (DWORD)me32.modBaseAddr + me32.modBaseSize;

						// 모듈을 찾았다~
						if (exceptionAddress >= dwModStart && exceptionAddress < dwModEnd)
						{

							//CRASH_INFO
							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%ld"), dwModStart);
							WritePrivateProfileString( _T("CRASH_INFO"), _T("ModuleStartAddress"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%ld"), dwModEnd);
							WritePrivateProfileString( _T("CRASH_INFO"), _T("ModuleEndAddress"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%ld"), exceptionAddress - dwModStart);
							WritePrivateProfileString( _T("CRASH_INFO"), _T("ModuleCrashOffset"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%s"), me32.szModule);
							WritePrivateProfileString( _T("CRASH_INFO"), _T("ModuleName"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%s"), c_tcBUILD_VERSION);
							WritePrivateProfileString( _T("CRASH_INFO"), _T("CompileVer"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							DWORD64 sysTime;
							util::GetDateTime_YYYYMMDDHHMMSS(sysTime);
							_ui64tot(sysTime, szValue, 10);
							WritePrivateProfileString( _T("CRASH_INFO"), _T("CrashTime"), szValue, szFileName);

							//PC_INFO
							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%s"), sysInfo.m_szOSEnglish);
							WritePrivateProfileString( _T("PC_INFO"), _T("OS"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%s"), sysInfo.m_szProcessorEnglish);
							WritePrivateProfileString( _T("PC_INFO"), _T("Process"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%s"), sysInfo.m_szPhysicalMemoryEnglish);
							WritePrivateProfileString( _T("PC_INFO"), _T("PhysicalMemory"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%s"), sysInfo.m_szDirectXVersionLongEnglish);
							WritePrivateProfileString( _T("PC_INFO"), _T("DirectXVersion"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%s"), sysInfo.m_szDescription);
							WritePrivateProfileString( _T("PC_INFO"), _T("VGADescription"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%s"), sysInfo.m_szDisplayMemoryEnglish);
							WritePrivateProfileString( _T("PC_INFO"), _T("DisplayMemory"), szValue, szFileName);

							ZeroMemory(szValue, sizeof(szValue));
							Sprintf(szValue, _T("%s"), sysInfo.m_szDriverVersion);
							WritePrivateProfileString( _T("PC_INFO"), _T("DriverVersion"), szValue, szFileName);

							// 이름과 매핑주소 그리고 오프셋을 기록
							fprintf(fp,"Module Name : %s\n",me32.szModule);
							fprintf(fp,"Module Base Address : %X - %X\n",dwModStart,dwModEnd);
							fprintf(fp,"Module Crash Offset : %X\n",exceptionAddress - dwModStart);							
							break;
						}
						
					}
					while(Module32Next(hModuleSnap,&me32));
				}
			}

			fclose(fp);
		}
	}

	return rt;

}

#endif//_DH_DEBUG_REPORT

#endif//GGUO_CRASH_DUMP


#pragma warning ( pop )
#pragma warning ( pop )
