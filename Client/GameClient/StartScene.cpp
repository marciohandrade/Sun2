#include "SunClientPrecompiledHeader.h"
#include "Application.h"
#include "startscene.h"
#include "objectmanager.h"
#include "ItemManager.h"
#include "InterfaceManager.h"
#include "SoundBGM.h"
#include "LoadScene.h"
#include "MouseCursor.h"
#include "MouseHandler.h"
#include "Camera.h"
#include "Mouse.h"
#include "Sound.h"
#include "SoundBGM.h"
#include "MapResMgr.h"
#include "SlotKeyGenerator.h"
#include "ProjectileManagerEx.h"
#include "Map.h"
#include "CursorChangeTransact.h"
#include "GlobalFunc.h"

#include "SystemDialog.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GlobalFunc.h"
#include "BattleScene.h"
#include "OptimizationManager.h"

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
#include "AutoTestManager.h"
#endif

#ifdef _APPLY_NHN_FILTERFING
	#include <IAF_Export.h>	//lib ���	
#endif//_APPLY_NHN_FILTERFING(NHN ���͸�)

StartScene g_StartScene;


StartScene::StartScene(void)
{
	m_iRenderCount = 0;
	m_bInitialized = FALSE;

	m_bInitInterface = FALSE;
	m_bRenderLoadImg = FALSE;
}

StartScene::~StartScene(void)
{
    m_SceneType = SCENE_TYPE_START;
}


void StartScene::Activate()
{

}

BOOL StartScene::Init()
{
	//	�ε� ��� ȭ�� �̹��� �ε�
	bool bSkipMoviePlay = false;

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) // �����/���߹��� ���

    if( AutoTestManagerEx::Instance()->IsActive() == true )
    {
        bSkipMoviePlay = true;
    }

	//#ifdef _KIKI_AUTOTEST
	//	if(AutoTestManager::Instance()->IsEnable())
	//		bSkipMoviePlay = true;	// �ڵ��׽�Ʈ��� Ȱ��ȭ�ÿ��� ������ skip
	//#endif
#endif

	if(bSkipMoviePlay)
	{
		SceneBase::LoadingCI();
	}
	else
	{
		if( GAMEOPTION->GetShowMoviePlay() )
		{
			SceneBase::LoadingImg(TRUE);
			
		}
		else
		{
			SceneBase::LoadingCI();//�ΰ� ����
		}
	}

	m_bFristRender=TRUE;

	m_bChangeImage=FALSE;
	m_dwCurFadeTime=0;
	m_eFadeState=eFADE_STATE_IN;

	IncreaseLoadingProcess();



#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.Create();
#endif

	

	// ���� �ʱ�ȭ.
	g_pApplication->InitSound();

	//	�������̽� ������ ��
	g_InterfaceManager.SetRenderState( TRUE );

	m_bRenderLoadImg = TRUE;
	IncreaseLoadingProcess();
	
	g_pApplication->LoadGameInfoForSriptParserMng();

#ifdef _DH_OS_LANGUAGETYPE
    g_pClientScriptParserManager->UpdateMessageScript();
#endif//_DH_OS_LANGUAGETYPE

	IncreaseLoadingProcess();
	GlobalFunc::WriteLoadingTimeLog(_T("UI Loading Start"));
	DWORD dwStartTime = GlobalFunc::StartTimeCheck();
	
	//	�������̽� ��ü ����
	g_InterfaceManager.Init( g_hWndMain, g_pSunRenderer);

	m_bInitInterface = TRUE;
	GlobalFunc::EndTimeCheck(dwStartTime, true, _T("UI Loading End"));

	IncreaseLoadingProcess();
	loadGameInfo();
	g_OptimizationManager.Init();
	IncreaseLoadingProcess();


	dwStartTime = GlobalFunc::StartTimeCheck();

	if (g_pMap)
	{// �ʿ������ �ϴ�..
		delete g_pMap;
		g_pMap = NULL;
	}
	else
	{
		g_pMap = new Map;
	}

	IncreaseLoadingProcess();

	g_pEffectManager = ( CEffectManager*)Func_ContactEffect( CW_NEW_EFFECT_MANAGER, 0);

	assert(g_pEffectManager);
	
	IncreaseLoadingProcess();

	BOOL bRet = FALSE;

	bRet = g_pEffectManager->Create( g_pSunRenderer, NULL);
	assert(bRet && "����Ʈ �ε� ���� 1");


#ifdef _KST_DISABLE_AUTO_LEVEL_EFFECT
    g_pEffectManager->ClearAutoLvlCtlParams();
#endif

    g_pEffectManager->SetDiscAccess( g_pApplication->GetDiscAccess() );

    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step1")); 

	IncreaseLoadingProcess();

    dwStartTime = GlobalFunc::StartTimeCheck();

	bRet = g_pEffectManager->LoadTextureFiles("Data\\Effect\\","Effect.txt");	

    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step2 [Effect.txt]")); 

	IncreaseLoadingProcess();

    dwStartTime = GlobalFunc::StartTimeCheck();

	bRet = g_pEffectManager->LoadTextureFiles("Data\\Effect\\","EffectTexture_BG.txt");	

    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step3 [EffectTexture_BG.txt]")); 

	IncreaseLoadingProcess();

	//assert(bRet && "����Ʈ �ε� ���� 2");
    dwStartTime = GlobalFunc::StartTimeCheck();

	bRet = g_pEffectManager->LoadMeshFiles("Data\\Effect\\","Effect-Mesh.txt");

    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step4 [Effect-Mesh.txt]")); 
	IncreaseLoadingProcess();

    dwStartTime = GlobalFunc::StartTimeCheck();

	bRet = g_pEffectManager->LoadMeshFiles("Data\\Effect\\","Effect-Mesh_BG.txt");

    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step5 [Effect-Mesh_BG.txt]")); 
	IncreaseLoadingProcess();

    dwStartTime = GlobalFunc::StartTimeCheck();


    int nEffectQuality = GAMEOPTION->GetGameOption().GetOptionValue( nOptionType::EFFECT_QUALITY );

    // ����Ʈ ��
    if( nEffectQuality <= 3 )
    {
        g_pEffectManager->LoadEffectFromFile( "Data\\Effect\\effectresource0.txt","Data\\Effect\\", EELT_ADD, 0 );
    }

    // ����
    if( nEffectQuality <= 2 )
    {
        g_pEffectManager->LoadEffectFromFile( "Data\\Effect\\effectresource1.txt","Data\\Effect\\", EELT_ADD, 1 );
    }

    // �߰�
    if( nEffectQuality <= 1 )
    {
        g_pEffectManager->LoadEffectFromFile( "Data\\Effect\\effectresource2.txt","Data\\Effect\\", EELT_ADD, 2 );
    }

    // ����
    if( nEffectQuality <= 0 )
    {
        g_pEffectManager->LoadEffectFromFile( "Data\\Effect\\effectresource3.txt","Data\\Effect\\", EELT_ADD, 3 );
    }

    // �⺻ ����Ʈ ���� ����
    g_pEffectManager->SetBaseEffectLevel( 3 - nEffectQuality );

    // ����Ʈ ���� �ڵ� ���� 
    g_pEffectManager->SetOption(EOPT_AUTOEFFLVLCTL | EOPT_APPLYEFFLOD | EOPT_APPLYLOD_LOOPONLY);


    // �ڵ� ����Ʈ ���� ���� ����
    if (PROGRAMPARAM->GetAutoEffectRate() >= 1)
    {
        g_pEffectManager->SetOption( EOPT_AUTOEFFRATECTL );
    }
    else
    {
        g_pEffectManager->UnsetOption( EOPT_AUTOEFFRATECTL );
    }

    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step6")); 

#ifdef _DEBUG
    g_pEffectManager->UnsetOption( EOPT_AUTOEFFLVLCTL );
    g_pEffectManager->UnsetOption( EOPT_AUTOEFFRATECTL );
#endif 

	IncreaseLoadingProcess();


    dwStartTime = GlobalFunc::StartTimeCheck();

	//	assert(bRet && "����Ʈ �ε� ���� 4");

#ifdef	_KOREA
    g_slangFilter.SetNationType( FILTER_KOREA );
    g_slangNameFilter.SetNationType( FILTER_KOREA );
#endif

#ifdef	_JAPAN
	g_slangFilter.SetNationType( FILTER_JAPAN );
	g_slangNameFilter.SetNationType( FILTER_JAPAN );
#endif


#ifdef _CHINA
    if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)       //!< 2:�߱�, 3:�븸
    {
        g_slangFilter.SetNationType( FILTER_TAIWAN );
        g_slangNameFilter.SetNationType( FILTER_TAIWAN );
    }
    else
    {
	    g_slangFilter.SetNationType( FILTER_CHINA );
	    g_slangNameFilter.SetNationType( FILTER_CHINA );
    }
#endif

#ifdef	_RUSSIA
    g_slangFilter.SetNationType( FILTER_RUSSIA );
    g_slangNameFilter.SetNationType( FILTER_RUSSIA );
    g_slangNameFilter.SetCheckType(kCheckFullString);
#endif

#ifdef _APPLY_NHN_FILTERFING
	int nRet = IAF::LoadAbuseDB();

#ifdef _DEBUG
	switch(nRet)
	{
	case -1:assert(!"IAF Load:Error(Can't find DB file)");break;
	case -2:assert(!"IAF Load:Error(Inappropriate DB file - checksum invalid)");break;
	case -3:assert(!"IAF Load:Error(Inappropriate DB file - can't decrypt db file)");break; 
	case -4:assert(!"IAF Load:Error(Unexpected Error)");break;
	}
#endif//_DEBUG

#endif//_APPLY_NHN_FILTERFING(NHN ���͸�)


    GlobalFunc::EndTimeCheck(dwStartTime, true,_T("StartScene::Init Step7"));


    dwStartTime = GlobalFunc::StartTimeCheck();

	if( GENERALPARAM->IsUsePackFile() )
	{
		bRet = g_slangFilter.LoadFromPack(GlobalFunc::AddNationName("Data\\Filter.txt"),false);
		assert(bRet && "Filter.txt �ε� ����");

		bRet = g_slangNameFilter.LoadFromPack(GlobalFunc::AddNationName("Data\\FilterName.txt"));
		assert(bRet && "Data\\FilterName.txt �ε� ����");
	}
	else
	{
		bRet = g_slangFilter.Load(GlobalFunc::AddNationName("Data\\Filter.txt"),false);
		assert(bRet && "Filter.txt �ε� ����");

		bRet = g_slangNameFilter.Load(GlobalFunc::AddNationName("Data\\FilterName.txt"));
		assert(bRet && "Data\\FilterName.txt �ε� ����");
	}


    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step8 [Data/Filter.txt, Data/FilterName.txt]"));

    dwStartTime = GlobalFunc::StartTimeCheck();

	IncreaseLoadingProcess();

	g_ProjectileManagerEx.Init();

	IncreaseLoadingProcess();

	g_pSunRenderer->SetEffectManager(g_pEffectManager);

	IncreaseLoadingProcess();

	g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect( PET_REFRACTION );

	g_pSunRenderer->x_pManagerPostEffect->TurnOnPostEffect(PET_DEPTH_OF_FIELD);

	IncreaseLoadingProcess();

	IncreaseLoadingProcess();

	//  ���� ��ã�� ��� 
    g_pPathExplorer=( CPathExplorer* )Func_ContactTerrain( CT_NEW_PATH_EXPLORER, 0 );
    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step9")); 

	IncreaseLoadingProcess();

    dwStartTime = GlobalFunc::StartTimeCheck();

	if ( !GENERALPARAM->IsTestUIScene() && !GENERALPARAM->IsTestResourceScene() )
	{
		g_pApplication->PreloadResource();
	}

    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step10 PreloadResource")); 

	IncreaseLoadingProcess();

    dwStartTime = GlobalFunc::StartTimeCheck();

	//	���� �ɼ� ����
	//	TRUE �� �־� �ָ� ���� ���� üũ�ؼ� �ٲ� �κи� ����
	//	FALSE �� �־� �ָ� ������ ���� �ɼ� ���·� ��� ����
	g_InterfaceManager.ApplyOption( FALSE ,TRUE);

	IncreaseLoadingProcess();

	g_KeyQueueManager.Init();
	g_KeyQueueManager.SetKey(DIK_ESCAPE);

	g_KeyQueueManager.SetDefaultFun(NULL);


    CGameOption::Instance()->ApplyOption();

	m_bInitialized = TRUE;

	IncreaseLoadingProcess();
	
	

	IncreaseLoadingProcess();

	//	ī�޶� ����
	g_Camera.Create( CAMERA_DEFAULT_NEAR_CLIP, CAMERA_DEFAULT_FAR_CLIP ,CAMERA_DEFAULT_FOV);

#ifdef _RUSSIA
    // ���þƱ��� ���� - ���þƹ����Է��� ���� ���������ν��� ������ȯ [1/7/2011 iforall]
    // �ش� �ڵ带 ���Ͽ� WM_CHAR �� ���� �����ڵ带 2����Ʈ�� ���� �� �ֵ�����.
    // (NOTE) �ش� �ڵ� ���� WINDOW API �� �������� �� �� ���� (ex. SetWindowText())
    SetWindowLongW(g_hWndMain, GWL_WNDPROC, GetWindowLong(g_hWndMain, GWL_WNDPROC));
#endif //_RUSSIA
	

    GlobalFunc::EndTimeCheck(dwStartTime, true, _T("StartScene::Init Step11")); 

	return TRUE;
}



//----------------------------------------------------------------------------
/**
*/
void  StartScene::IncreaseLoadingProcess()
{

	m_fMaxCount=20.0f;
	SceneBase::IncreaseLoadingProcess();

	if( TRUE == g_pSunRenderer->BeginRender() )
	{
		SceneBase::RenderLoadingImg();
		loadInitMessage();

		g_pSunRenderer->x_pManagerPostEffect->Apply();
	}
	g_pSunRenderer->EndRender();

}

BOOL StartScene::Process( DWORD dwTick )
{
	DecideScene();
	return TRUE;
}


BOOL StartScene::Render( DWORD dwTick )
{

	return TRUE;
}

void StartScene::Release()
{
	m_iRenderCount = 0;


	//	������ �÷��� ����
	m_bEnableRender = FALSE;

	SceneBase::ReleaseLoadingImg();

	g_pSunRenderer->x_Clock.Reset();
}

void StartScene::ParsePacket( MSG_BASE * pMsg )
{
}


void StartScene::DecideScene()
{
	g_pApplication->Set2DMode(FALSE);
	SCENE_TYPE iSceneType = SCENE_TYPE_LOGIN;

    if ( GENERALPARAM->IsTestResourceScene() )
    {
        iSceneType = SCENE_TYPE_RESOURCETEST;
    }
    else
    if (GENERALPARAM->IsTestUIScene())
    {
        iSceneType = SCENE_TYPE_TESTUI;
    }
    else
    {
        if (GENERALPARAM->IsNet())
        {
            // multi
            iSceneType = SCENE_TYPE_LOAD;
            g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_LOGIN, 11001, 0);
            g_LoadScene.ChangeImage(TRUE);
        }
        else
        {
            // single
#ifdef MAP_EDIT_MODE
            iSceneType = SCENE_TYPE_LOAD;	
            xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
			g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_MAPEDIT, 12001, 61001);

			BattleScene::SetHeroInfo(eCHAR_ELEMENTALIST);

			g_LoadScene.ChangeImage(TRUE);
#else
			iSceneType = SCENE_TYPE_LOAD;	
			g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT, GENERALPARAM->GetCharSelectMapID(), 0);
			g_LoadScene.ChangeImage(TRUE);
#endif 

        }
    }

	GameFramework::ChangeScene(iSceneType);
}

void StartScene::loadGameInfo()
{
	GlobalFunc::WriteLoadingTimeLog(_T("Script Loding Start"));
	DWORD dwStartTime = GlobalFunc::StartTimeCheck();

	g_pApplication->LoadGameInfo();

	GlobalFunc::EndTimeCheck(dwStartTime, true, _T("Script Loding End"));

	MouseCursor::Instance()->Init( g_hInstance );
	CursorChangeTransact::Instance()->Init();
	MouseHandler::Instance()->InitUnitRenderer(g_pSunRenderer);
}

void StartScene::RenderText( void )
{

}

