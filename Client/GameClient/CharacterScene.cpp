//------------------------------------------------------------------------------
//  CharacterScene.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "characterscene.h"
#include "objectmanager.h"
#include "Application.h"
#include "LoadScene.h"
#include "Camera.h"
#include "AppearanceManager.h"
#include "BattleScene.h"
#include "Character.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "GameFunc.h"
#include "SoundBGM.h"
#include "CloneManager.h"
#include "InventoryDialog.h"
#include "Version.h"
#include "EquipmentContainer.h"
#include <SCItemSlot.h>
#include "uiLoginAccount/uiLoginAccount.h"
#include "uiLoginServerList/uiLoginServerList.h"
#include "uiLoginMan/uiLoginMan.h"
#include "CreateCharBottomDialog.h"
#include "CreateCharLeftDialog.h"
#include "CreateCharSelect.h"
#include "BGMSoundInfoParser.h"
#include "uiSystemIdNumberConfirm/uiSystemIdNumberConfirm.h"
#include "Nstring.h"
#include "LoadScene.h"
#include "LoginScene.h"
#include "MouseCursor.h"
#include "SoundEffect.h"
#include "Chatdialog.h"
#include "HeroActionInput.h"
#include "HeroVariationInfoParser.h"
#include "HeroHeightInfoParser.h"
#include "MapInfoParser.h"
#include "ResultCode.h"
#include "QuestManager_Concrete.h"
#include "MissionManager_Concrete.h"
#include "uiTiredSystemMan/uiTiredSystemMan.h"
#include "TiredSystemDialog.h"
#include "uiHelpMan/uiHelpMan.h"
#include "Map.h"
#include "ResourceManager.h"
#include "uiQuestMan/uiQuestMan.h"
#include "CursorChangeTransact.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "PcbangPlayTimeDialog.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "ShopDialog.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "OptionGameDialog.h"
#include "OptimizationManager.h"
#include "CreateCharRotateButton.h"
#include "uiCreateCharacterMan/uiCreateCharacterMan.h"
#include "CreateCharCustomDialog.h"
#include "uiPartyMan/uiPartyMan.h"
#include "Pet.h"
#include "PetInfoParser.h"
#include "uiPersonalWayPointMan/uiPersonalWayPointMan.h"
#include "uiChatMan/uiChatMan_def.h"
#include "Monster.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "uiLoginBack/uiLoginBackTop.h"

#include "uiSkillMan/uiNewSkillMan.h"
#include "uiQuestMan/uiQuestMan_def.h"
#include "uiquestlistdlg/uiQuestListDlg.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"

#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "Mouse.h"

#ifdef _NHN_USA_CHANNELING
#include <HanAuth/HanReportForClient.h>	//지표모듈
#endif//_NHN_USA_CHANNELING(북미 채널링)

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
#include "AutoTestManager.h"
#endif

#ifdef _YMS_AUTOBOT_GAME
    #include "Autobot/Condition/AutobotConditionManager.h"
#endif //_YMS_AUTOBOT_GAME

#include "uiSkillMan/uiNewSkillMan.h"
#include "CreateCharCustomCameraInfoParser.h"

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "uiCharacterRenameDialog/uiCharacterRenameDialog.h"
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE

#include "AppearanceResourceCode.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "uiCharacterSelectList/uiCharacterSelectList.h"

#ifdef _NA_000000_20130114_RENEWER_UI 
#include "uiCharacterSelectList/uiCharacterSelectList2.h"
#endif//#ifdef _NA_000000_20130114_RENEWER_UI 

#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiSelectClass2/uiSelectClass2.h"
#else
#include "uiSelectClass/uiSelectClass.h"
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
#include "CharacterCreationInfoParser.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "CharSelectDialog.h"

#include "CastDialog.h"

#ifdef _NA_20100307_BETAKEY_SYSTEM
#include "BetaKeyImpl.h"
#include "BetaKey.h"
#endif//_NA_20100307_BETAKEY_SYSTEM

#include "uiGuideSystemMan/uiGuideSystemMan.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "HonorSystem/HonorSystem.h"
#include "uiCharacterStatus/uiCharacterStatusHonorInfo.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
#include "uiCharacterStatus/uiCharacterStatusCurrencyInfo.h"
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#endif//_DH_BATTLEZONE2_
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "XignCode/XignCodeApi.h"
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiConvenienceStore/CashShop.h"
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
#include "uiTiredSystemMan/uiTiredSystemMan.h"
#include "TiredSystemDialog.h"
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
#ifdef _NA_008405_20150716_MINIGAME_BINGO
#include "uiMenuMan/uiMenuMan.h"
#include "MainDialog2.h"
#endif // _NA_008405_20150716_MINIGAME_BINGO

#include "HotKeyManager.h"
#include "HeroData.h"
#include "CooltimeManager.h"
#include "CharacterCreate.h"

#pragma comment( lib, "Iphlpapi.lib" )

#define SELECT_MOVE_DISTANCE    1.0f    //선택시 이동할 최대 거리 
#define CHAR_MOVE_SPEED         0.3f 
#define WALK_SPEED              0.001f
#define DOUBLE_CLICK_TIME       500     // 0.5초 시간안에 입력은 더블클릭이다.
#define PERFECT                 100.0f  // 퍼센트
CharacterScene	                g_CharSelectScene;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
cCharacterCreate				g_CharCreate;
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT


const DWORD INTERFACE_SCROLL_TIME = 1;
const float BOTTOM_INTERFACE_SIZE = 80.0f;
const float BOTTOM_INTERFACE_MARGIN = 92.0f;
const float LEFT_INTERFACE_SIZE = 338.0f;
const float LEFT_INTERFACE_TOP = 120.0f;
const float LEFT_INTERFACE_MARGIN = 24.0f;
const float l_fCharacterSceneFarClip = 300.0f;
const int CHARACTER_MAP_RESOURCE_ID = SCRIPT_RESOURCE_ID_OFFSET + 130000;


TCHAR	g_szCrashLogFileName[INTERFACE_STRING_LENGTH] = {0,};
DWORD	g_dwStartTime = 0;




using namespace RC;


void _ProcessCrashReport(bool bYes)
{
	TCHAR lpszParamBuffer[16] = {0,};

	if(bYes)
	{
		//0: 보내기 전, 1:보냄, -1: 보내기 싫음, -2: Default
		WritePrivateProfileString( _T("SEND_REPORT"), _T("Send"), ParamInt2Str( 1, lpszParamBuffer), g_szCrashLogFileName);

		MSG_CG_CONNECTION_CRASH_REPORT_SYN sync;
		ZeroMemory(&sync.m_CrashInfo, sizeof(CRASH_INFO));
		ZeroMemory(&sync.m_ComputerInfo, sizeof(COMPUTER_INFO));

		TCHAR szTempBuffer[MAX_PATH] = {0,};
		TCHAR* pszStopBuffer = NULL;
		GetPrivateProfileString( _T("CRASH_INFO"), _T("CrashAddress"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		_tcstod(szTempBuffer, &pszStopBuffer);
		sync.m_CrashInfo.m_dwCrahAddress = _tcstoul(szTempBuffer, &pszStopBuffer, 10);

		GetPrivateProfileString( _T("CRASH_INFO"), _T("ModuleStartAddress"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		_tcstod(szTempBuffer, &pszStopBuffer);
		sync.m_CrashInfo.m_dwModuleStartAddress = _tcstoul(szTempBuffer, &pszStopBuffer, 10);

		GetPrivateProfileString( _T("CRASH_INFO"), _T("ModuleEndAddress"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		_tcstod(szTempBuffer, &pszStopBuffer);
		sync.m_CrashInfo.m_dwModuleEndAddress = _tcstoul(szTempBuffer, &pszStopBuffer, 10);

		GetPrivateProfileString( _T("CRASH_INFO"), _T("ModuleCrashOffset"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		_tcstod(szTempBuffer, &pszStopBuffer);
		sync.m_CrashInfo.m_dwModuleCrashOffset = _tcstoul(szTempBuffer, &pszStopBuffer, 10);

		GetPrivateProfileString( _T("CRASH_INFO"), _T("ModuleName"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		StrnCopy(sync.m_CrashInfo.m_szModuleName, szTempBuffer, MAX_MODULE_NAME_SIZE);

		GetPrivateProfileString( _T("CRASH_INFO"), _T("CompileVer"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		StrnCopy(sync.m_CrashInfo.m_szBuildVer, szTempBuffer, MAX_BUILD_VERSION_SIZE);

		GetPrivateProfileString( _T("CRASH_INFO"), _T("CrashTime"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		_tcstod(szTempBuffer, &pszStopBuffer);
		sync.m_CrashInfo.m_dw64CrashTime = _tcstoui64(szTempBuffer, &pszStopBuffer, 10);
		

		GetPrivateProfileString( _T("PC_INFO"), _T("OS"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		StrnCopy(sync.m_ComputerInfo.m_szOS, szTempBuffer, MAX_OS_NAME_SIZE);

		GetPrivateProfileString( _T("PC_INFO"), _T("Process"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		StrnCopy(sync.m_ComputerInfo.m_szProcess, szTempBuffer, MAX_PROCESS_NAME_SIZE);

		GetPrivateProfileString( _T("PC_INFO"), _T("PhysicalMemory"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		StrnCopy(sync.m_ComputerInfo.m_szPhysicalMemory, szTempBuffer, MAX_PHYSICAL_MEMORY_SIZE);

		GetPrivateProfileString( _T("PC_INFO"), _T("DirectXVersion"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		StrnCopy(sync.m_ComputerInfo.m_szDirectXVersion, szTempBuffer, MAX_DIRECTX_NAME_SIZE);

		GetPrivateProfileString( _T("PC_INFO"), _T("VGADescription"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		StrnCopy(sync.m_ComputerInfo.m_szDescription, szTempBuffer, MAX_DESCRIPT_SIZE);

		GetPrivateProfileString( _T("PC_INFO"), _T("DisplayMemory"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		StrnCopy(sync.m_ComputerInfo.m_szDisplayMemory, szTempBuffer, MAX_DISPLAY_MEMORY_SIZE);

		GetPrivateProfileString( _T("PC_INFO"), _T("DriverVersion"), _T("0"), szTempBuffer, MAX_PATH-1, g_szCrashLogFileName );
		StrnCopy(sync.m_ComputerInfo.m_szDriverVersion, szTempBuffer, MAX_DRIVER_VERSION_SIZE);

		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
		
	}
	else
	{
		//0: 보내기 전, 1:보냄, -1: 보내기 싫음, -2: Default
		WritePrivateProfileString( _T("SEND_REPORT"), _T("Send"), ParamInt2Str( -1, lpszParamBuffer), g_szCrashLogFileName);

	}
}


//------------------------------------------------------------------------------
/**
*/
CharacterScene::CharacterScene(void)
{
    m_hTexture = INVALID_HANDLE_VALUE;
	m_bConnected = 0;
    m_bWorldServerConnected = false;
    m_SceneType = SCENE_TYPE_CHARSELECT;

    Movie.is_show_movie = false;
    Movie.bink = NULL;
    Movie.bink_texture = NULL;
    Movie.backup_clear_color = 0;
}

//------------------------------------------------------------------------------
/**
*/
CharacterScene::~CharacterScene(void)
{

}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::Activate()
{

}

//------------------------------------------------------------------------------
/**
    캐릭터 위치 세팅 
*/
void CharacterScene::InitSlotForPositionAndAni()
{
    //float fDistanceX = 0.15f;
    //float fDistanceY = 1.50f;

    //float xPos=-22.20f;
    //float yPos=54.7f;

    for(int i = 0; i < MAX_CHARACTER_LIST_NUM; ++i)
    {
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
		//m_PosInfoArray[i].wzVecPosition = xGetCharacterCreate()->GetCharacterPosition();
        //SetVector(&m_PosInfoArray[i].wzVecPosition, m_character.y, m_character.z);
#else

        SetVector(&m_PosInfoArray[i].wzVecPosition, -5.054004f, 1.2010005f, 10.538147f);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    }

    for(int i = 0; i < MAX_CHARACTER_LIST_NUM; i++)
    {
        m_PosInfoArray[i].dwObjectKey = INVALID_OBJECT_KEY;
    }
}







//------------------------------------------------------------------------------
/**
*/
BOOL CharacterScene::Init()
{
	if ( uiPersonalWayPointMan* pWaypointMan = GET_DIALOG_MANAGER(uiPersonalWayPointMan , UIMAN_PERSONAL_WAYPOINT) )
	{
		pWaypointMan->InitWayPoint();
	}

	if ( uiDominationTowerMan* pDominationMan = GET_DIALOG_MANAGER(uiDominationTowerMan , UIMAN_DOMINATIONWAR_TOWER) )
	{
		pDominationMan->Clear();
	}
#ifdef _NHN_USA_CHANNELING
	GlobalFunc::SendHanReportPacket(601);//캐릭터선택창이동완료(601)
#endif//_NHN_USA_CHANNELING(북미 채널링:지표)

    BattleScene::m_bPendingBackToSomeWhere = false;

	m_bVerifyLock = FALSE;

	if (g_pNetwork)
	{
		g_pNetwork->SetEnableSendHeartBeat(FALSE);
	}		

    CursorChangeTransact::Instance()->ForceChange( MouseCursor::eMouseCursor_Default );

	g_HeroInput.Init();

	m_eFadeState=eFADE_STATE_NONE;
	m_dwCurFadeTime=0;

	g_InterfaceManager.ShowWindow( InterfaceManager::DIALOG_SYSTEM, FALSE );

    LoadCinematicCamera();
    is_loaded_my_character_ = true;
    if(g_pSunRenderer->x_pManagerMultiLoader)
        g_pSunRenderer->x_pManagerMultiLoader->TurnOn_MultiThreadedLoading(TRUE);

	TCHAR	szMessage[INTERFACE_STRING_LENGTH];


    SceneBase::Init();

	m_bChangeSelectedCharacter=TRUE;

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	xGetCharacterCreate()->Initialize();
	//m_zoom = 0.0f;
 //   SetVector(&m_camera, -47.31f, -4.914f, 1.2f);
 //   SetVector(&m_character, -47.286f, 0.f, 0.42f);
 //   SetVector(&m_target, -47.31f, 0.f, 1.2f);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

#ifndef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.Create();
#endif
    m_CharList.clear();
    InitSlotForPositionAndAni();
  
	m_bPickCharLock = FALSE;
    m_dwSelectOid = INVALID_OBJECT_KEY;
    m_dwSelectCharacter = INVALID_OBJECT_KEY;

    //	인터페이스 랜더링 온
    g_InterfaceManager.SetRenderState( TRUE );
    m_curDelOid = INVALID_OBJECT_KEY;
	m_fRt=0.0f;

	// 배경음악 플레이
	GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_CHARSELECT);
    
    m_bInitCam=FALSE;
    m_preTime=0;


    if (g_pQMGR_Real)
    {
	    g_pQMGR_Real->DestroyAll();
    }

    if (g_pQMGR_Temp)
    {
	    g_pQMGR_Temp->DestroyAll();
    }

	g_pMission_Concrete->Clear();

	if ( uiQuestMan * pQuestMan = (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager( UIMAN_QUEST ) )
	{
		pQuestMan->ClearQuestInfos();
		pQuestMan->ClearAcceptAuto();

		pQuestMan->ClearPendingQuestCompletePacket();
	}

    if ( uiPartyMan* partyMan = static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY)) )
    {
        partyMan->ClearParty();
    }

    if( GENERALPARAM->IsNet() )
    {
		if (m_bConnected)
		{
			MSG_CG_CONNECTION_REENTERSERVER_SYN	SendPacket;

			if( FALSE == GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) ) )
			{
				assert("MSG_CG_CONNECTION_REENTERSERVER_SYN - CharacterScene");
				return FALSE;
			}        
		}

        if(!m_bConnected && BIT_CHECK(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_CONECT) == FALSE)
        {
            BIT_ADD(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_CONECT);
            // 서버 접속 요청을 하자.
            MSG_CG_CONNECTION_ENTERSERVER_SYN ConnectRegPacket;
            ConnectRegPacket.m_byCategory = CG_CONNECTION;
            ConnectRegPacket.m_byProtocol = CG_CONNECTION_ENTERSERVER_SYN;
			ConnectRegPacket.m_byNationCode = NATION_CODE;

#ifdef _NA_0_20110915_CLIENT_MAC_ADDRESS
            ZeroMemory(ConnectRegPacket.mac_address_, 30);
            g_pNetwork->GetMACAddress(OUT ConnectRegPacket.mac_address_, 30);
#endif //_NA_0_20110915_CLIENT_MAC_ADDRESS

            uiLoginMan* loginMan =
                static_cast<uiLoginMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
            assert (loginMan);

            StrCopy(ConnectRegPacket.m_szID, loginMan->GetID());
            ConnectRegPacket.m_dwAuthID	  = loginMan->GetAuthUserKey();
            memcpy ((BYTE *)ConnectRegPacket.m_szSerialKey, loginMan->GetSerialKeys(), AUTH_SERIAL_KEY_LEN);

			ConnectRegPacket.m_byHighVerNo		= C2S_HIGH_VERSION_NO;
			ConnectRegPacket.m_byMiddleVerNo	= C2S_MIDDLE_VERSION_NO;
			ConnectRegPacket.m_byLowVerNo		= C2S_LOW_VERSION_NO;


			ConnectRegPacket.m_dwMOLAProtocolCRC = CryptManager::GetMolaProtocolCRC();





			ConnectRegPacket.Encode();

			if(GENERALPARAM->GetSpecialMode())
			{
				g_dwStartTime = GlobalFunc::StartTimeCheck();
				GlobalFunc::WriteLoadingTimeLog(_T("캐릭터 정보 전송 요청"));
			}

			if( FALSE == GlobalFunc::SendPacket(CI_GAMESERVERIDX, &ConnectRegPacket, sizeof(ConnectRegPacket) ) )
            {
				WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("SendPacket return FALSE: MSG_CA_CHARINFO_CHARLIST_SYN"));
				assert(!"MSG_CA_CHARINFO_CHARLIST_SYN - CharacterScene");
				return FALSE;
            }
        }           

		//	캐릭터 정보 전송중
		const int	c_iCharInfoSending = 70256;
		g_InterfaceManager.GetInterfaceString( c_iCharInfoSending, szMessage, INTERFACE_STRING_LENGTH );
		g_InterfaceManager.ShowMessageBox( szMessage, -1, NULL, 0 );

        g_pApplication->OnReconnectToWorldServer();
    }
    else //싱글 모드 캐릭터 생성 
    {
		srand(clock_function::GetTickCount());
		
        BYTE ClassCodeType[6] = { eCHAR_BERSERKER, eCHAR_DRAGON, eCHAR_SHADOW, eCHAR_VALKYRIE, eCHAR_MAGICIAN, eCHAR_ELEMENTALIST };
        char CharacterName[6][16] = { "테스트1", "테스트2", "테스트3", "테스트4", "테스트5", "테스트6" };

        for( size_t i=0;i!=6;++i)
        {
		    CharListInfo* pCharListInfo = new CharListInfo;
            pCharListInfo->dwObjectKey = g_ObjectManager.GenerateKeyAtSingleMode(PLAYER_OBJECT);
            StrCopy(pCharListInfo->CharacterInfo.m_tszCharName, CharacterName[i]);

        	
            CLIENT_CHARACTER_PART part;
            ZeroMemory(&pCharListInfo->CharacterInfo, sizeof(part));
            pCharListInfo->CharacterInfo.m_byFace=1;
            pCharListInfo->CharacterInfo.m_byHair=1;
            pCharListInfo->CharacterInfo.m_byClass = ClassCodeType[i];
            pCharListInfo->CharacterInfo.m_bySlot = i;
            pCharListInfo->CharacterInfo.m_ChangeOfClassStep = ClassCodeType[i];
            pCharListInfo->CharacterInfo.gender = 0;

		    MakeMyChar( pCharListInfo->dwObjectKey, pCharListInfo->CharacterInfo );
            m_CharList.push_back( pCharListInfo );
        }
    } 

    g_pMap->SetUseAutoCamera(TRUE);
    //	로딩 배경 화면 이미지 로딩
	m_hTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\Interface\\Loading.jpg" 
																,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);


	 m_fOriginalFarClip = g_Camera.GetFarClip();
	 g_Camera.SetProjection(g_Camera.GetNearClip(),l_fCharacterSceneFarClip,g_Camera.GetFOV());

     /*WzVector wvPos;
     wvPos.x = -47.31f;
     wvPos.y = -3.784f;
     wvPos.z = 1.2f;
     g_Camera.SetCamera(&wvPos);*/

	 // 캐릭터 생성관련 테스트
	 
	m_bCreateCharacter = FALSE;
//#ifdef _YJ_RENEWAL_CHARACTER_CREATE_SCENE
//    //no operation
//#else //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
	m_pCreateMapUnitDraw = NULL;
	m_pCreateMapUnit = NULL;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//#endif //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
	m_bySelectedClass = 0;
    m_bySelectedModel = 0;
    m_byCharacterSlot = MaxCharListConfig::kDefaultSlotSize;

	m_bLoadCreateCharacterRoom = false;

	g_pSunRenderer->EnableAreaLight(TRUE);
	 
	g_KeyQueueManager.Reset();
	g_KeyQueueManager.SetKey(DIK_ESCAPE);
	g_KeyQueueManager.SetDefaultFun(NULL);

	m_bMouseChange = FALSE;
	if(g_Input.GetMouseChange())
	{
		g_Input.SetMouseChange(FALSE);
		m_bMouseChange = TRUE;
	}


	if( uiCommunityMan* pCommMan = static_cast<uiCommunityMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY)) )
	{
		pCommMan->Reset();
	}


    m_stFogCharCreate.m_wcFogColor = GRAPHICPARAM->m_wcCharCreateSceneFogColor;
	
	m_stFogCharCreate.m_fFogStart=40.0f;
	m_stFogCharCreate.m_fFogEnd=150.0f;
	m_stFogCharCreate.m_fMaxFogRate=45.3f;


	m_stBlurCharCreate.m_fFinalColorBlend=0.39f;
	m_stBlurCharCreate.m_fMinLuminance=0.6f;
	m_stBlurCharCreate.m_fSelectHighLight=0.48f;
	m_stBlurCharCreate.m_iBlurCount=4;
	m_stBlurCharCreate.m_fOriginalColorBlend=0.77f;
	m_stBlurCharCreate.m_wcSelectColor=WzColor_RGB(255,255,255);

	g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_DEPTH_OF_FIELD);
	g_pMap->EnableBlur(GAMEOPTION->GetBlurState());

	g_pSunRenderer->EnableFog(GAMEOPTION->GetFogState());

    xGetHeroData()->ReleaseGuildInfo();

    // 캐릭터가 생성/변경되면 모든 쿨타임을 리셋
    xGetCooltimeManager()->ResetSkillCooltime();
    xGetCooltimeManager()->ResetItemCooltime();


#ifdef _YMS_AUTOBOT_GAME
    AutobotConditionManager::Instance()->Clear();
#endif //_YMS_AUTOBOT_GAME
    
    //! 2011.6.21 / naklu / 캐릭터 선택화면에 돌아 올때 AC 완료 리스트 정보 초기화 
    if ( uiAreaConquestMan* area_conquest_manager = GET_DIALOG_MANAGER(uiAreaConquestMan, UIMAN_AREACONQUEST) )
    {
        area_conquest_manager->DeleteComplimentList();
    }

#if USING_XIGNCODE
    ZeroMemory(XignCodeApi::xigncode_user_info(), sizeof(XignCodeApi::XignCode_UserInfo));
#endif //USING_XIGNCODE

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    HonorSystem::GetHonorSystemData().ClearTitleCodeList();
    HonorSystem::GetHonorSystemData().InitializeHonorPointTable();
    if ( uiCharacterStatusHonorInfo* ui_honor_info = GET_CAST_DIALOG(uiCharacterStatusHonorInfo, uiCharacterStatusHonorInfo::kDialog_ID) )
    {
        ui_honor_info->InitHonorListInfo();
    }
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    if ( uiCharacterStatusCurrencyInfo* ui_currency_info = GET_CAST_DIALOG(uiCharacterStatusCurrencyInfo, uiCharacterStatusCurrencyInfo::kDialog_ID) )
    {
        ui_currency_info->InitCurrencyInfo();
    }
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _DH_BATTLEZONE2_
    if ( uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER) )
    {
        ui_battlezone2_manager_->Clear();
    }
#endif//_DH_BATTLEZONE2_

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    ChaosSystem::ChaosZoneData::ReleaseMutexHandle();
#else
    ChaosSystem::GetBattleGroundData().ReleaseMutexHandle();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    if ( !GENERALPARAM->IsNet() )
    {
        // 캐릭터 선택 관련 gui 보이기
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CHARSELECT,TRUE);

        uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
        if (login_back_top != NULL)
        {
            login_back_top->ShowInterface(TRUE);
        }
    }

#ifdef _NA_008405_20150716_MINIGAME_BINGO
	MainDialog2* main_dialog = GET_CAST_DIALOG(MainDialog2, IM_MENU_MANAGER::MAIN_DLG_2);
	if (main_dialog)
	{
		main_dialog->MiniGameToDisAbleButton(false);
	}
#endif // _NA_008405_20150716_MINIGAME_BINGO

	return TRUE;
}	

//------------------------------------------------------------------------------
/**
	싱글용 빈슬롯을 구한다. 
*/
int  CharacterScene::GetEmptySlot()
{
	for(int i=0;i<MAX_CHARACTER_LIST_NUM;i++)
	{
		if(m_PosInfoArray[i].dwObjectKey==INVALID_OBJECT_KEY)
			return i;
	}

	return -1;
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::Release()
{
    if(m_bCreateCharacter)
    {
        //캐릭터씬에서 릴리즈시에는 ViewPort 원상복귀시켜야됨
        SetCreateCharacterMode(FALSE);
    }

	if(g_pSunRenderer->x_pManagerMultiLoader)
		g_pSunRenderer->x_pManagerMultiLoader->TurnOn_MultiThreadedLoading(FALSE);

	if (m_dwCameraObjectKey)
	{
		g_ObjectManager.Delete(m_dwCameraObjectKey);
	}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
	g_Camera.SetProjection(g_Camera.GetNearClip(),m_fOriginalFarClip,g_Camera.GetFOV());
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    // 캐릭터 선택 관련 gui 감추기
    g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CHARSELECT,FALSE);
    uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
    if (login_back_top != NULL)
    {
        login_back_top->ShowInterface(FALSE);
    }

    if (m_hTexture != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(m_hTexture);
        m_hTexture = INVALID_HANDLE_VALUE;

    }
    
    //	랜더링 플래그 끄기
    m_bEnableRender = FALSE;

#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.DeleteAllObjectWithoutHero();
#else
	g_ObjectManager.Destroy();
#endif

    // 맵 해제..
    g_pMap->Destroy();

    CHARLIST_ITR itr;

    for (itr=  m_CharList.begin(); itr!= m_CharList.end() ;)
    {
        delete static_cast<CharListInfo *>(*itr);
        m_CharList.erase(itr++);
    }

	if(m_bMouseChange)
	{
		m_bMouseChange = FALSE;
		g_Input.SetMouseChange(TRUE);
	}


    m_CharList.clear();

    for( size_t i=0;i!=m_StatusArray.size();++i)
    {
        delete m_StatusArray[i];
    }
    m_StatusArray.clear();

	g_InterfaceManager.HideAllWindows();

//#ifdef _YJ_RENEWAL_CHARACTER_CREATE_SCENE
//    //no operation
//#else //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
	ReleaseCreateCharacterResource();
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//#endif //_YJ_RENEWAL_CHARACTER_CREATE_SCENE

    g_InterfaceManager.ResetUIManager();
    g_InterfaceManager.ResetSolarDialog();

    //------------------------------------------------------------------------------
    // 중국 캐릭터 생성후 동영상 보기
    if (Movie.bink)
    {
        BinkClose(Movie.bink);
        Movie.bink = NULL;
    }
    if (Movie.bink_texture != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(Movie.bink_texture);
        Movie.bink_texture = INVALID_HANDLE_VALUE;
    }
    //------------------------------------------------------------------------------ 
}			

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::CreatePlayerList()
{
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::ProcessKeyInput( DWORD dwTick )
{
}

//------------------------------------------------------------------------------
/**
*/
Character* CharacterScene::GetPickedChar(WzRay *pRay)
{
	if(m_bPickCharLock)
		return NULL;

    //스타트 패킷을 안보냈을때
    if (VectorLength(&pRay->m_wvDirection) >= 0.2f&& BIT_CHECK(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_SELCHAR)==FALSE) 
    {
        float fT = 0.f;
        Object * pTargetObject = NULL;
        Character *pChar = NULL;
        DWORD dwKey = 0;

        float fSelectDist = 1000.f;		// 현재 선택된 캐릭터와의 거리: 기본값은 max로 설정

        CHARLIST_ITR itr = m_CharList.begin();
        CharListInfo * pCharListInfo = NULL;

        for (; itr != m_CharList.end(); ++itr)
        {
            pCharListInfo = static_cast<CharListInfo *>(*itr);
            dwKey = pCharListInfo->dwObjectKey;
            pTargetObject = g_ObjectManager.GetObject(dwKey);

            if ((pTargetObject->GetSCObject().GetObjectType() & CHARACTER_OBJECT) == CHARACTER_OBJECT) 
            {
                if (pTargetObject == g_pHero)
                {
                    continue;
                }
                WzBoundingVolume * pVolume=pTargetObject->GetBoundingVolume();


				WORD wResult = NTCT_OUTSIDE;

				if (pTargetObject->GetNumPickingBoundingVolume())
				{
					WzBoundingVolume bv = *pTargetObject->GetPickingBoundingVolume(0);
					wResult = Intersect_Ray_Bv( pRay, &bv, &fT );

				}
				else
				{
					WzBoundingVolume bv = *pTargetObject->GetBoundingVolume();
					wResult = Intersect_Ray_Bv( pRay, &bv, &fT );
				}

        
                if (NTCT_OUTSIDE == wResult)  
                {
                    pTargetObject = NULL;
                    continue;
                }
                else
                {				
                    // 이전에 선택한 캐릭터보다 가까운 캐릭터를 선택한다.
                    if (fT < fSelectDist)
                    {
                        Character* pSelectChar = (Character *)pTargetObject;
                        pChar = pSelectChar;
                        fSelectDist = fT;
                    }			
                }
            }
        }

         return pChar;
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
    캐릭터 선택
*/
void CharacterScene::SelectCharecter(Character * pChar)
{
    Player* pPlayer = static_cast<Player *>(pChar);

    CHARLIST_ITR itr = m_CharList.begin();
    CharListInfo* pCharListInfo = NULL;

    m_dwSelectCharacter = INVALID_OBJECT_KEY;
    for (; itr != m_CharList.end(); ++itr)
    {
        pCharListInfo = static_cast<CharListInfo *>(*itr);
        Player* pOtherPlayer = (Player*)g_ObjectManager.GetObject(pCharListInfo->dwObjectKey);
        if(pOtherPlayer->GetObjectKey()==pPlayer->GetObjectKey())
        {
            //현재 선택된 캐릭터를 저장 
            m_dwSelectCharacter = pPlayer->GetCharInfo()->m_bySlot;

            // 이전 선택 된놈 처리 
            if( m_dwSelectOid != INVALID_OBJECT_KEY && m_dwSelectOid != pPlayer->GetObjectKey() )
            {
                if( ObjectStatus* pOldObject = GetStatus(m_dwSelectOid) )
                {
                    pOldObject->SetSelect( FALSE );
                    pOldObject->status_time_backup = pOldObject->status_time;
                    pOldObject->status_time += 2400;

                    if( Player* pNoneSelPlayer = (Player*)g_ObjectManager.GetObject(m_dwSelectOid) )
                    {
                        pNoneSelPlayer->SetAnimation ("CE50", FALSE);
                    }
                }
            }

            DWORD preSelectOid = m_dwSelectOid;
            m_dwSelectOid = pPlayer->GetObjectKey();
            if(preSelectOid!=m_dwSelectOid)//이전 선택한 놈이랑 다를때 
            {
                ObjectStatus* pCurrentObject = GetStatus( m_dwSelectOid );
                pCurrentObject->SetSelect( TRUE );
            }
            else //이미 선택한 놈일때 더블 클릭 검사 
            {
                DWORD diff = g_pSunRenderer->x_Clock.GetCurrentTick()-m_preTime;
                if( diff < DOUBLE_CLICK_TIME)//더블클릭 처리 
                {
                    SendCharSelectPacket();
                }
            }

            m_preTime = g_pSunRenderer->x_Clock.GetCurrentTick();
            if( GENERALPARAM->IsNet()==FALSE )
            {
                ((Player*)pChar)->SetMoveSpeedRatio(100);
                ((Player*)pChar)->SetAttackSpeedRatio(100);
				BattleScene::SetHeroInfo( ((Player *)pChar)->GetClass() );
            }
            break;
        }

		

    }
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::ProcessMouse(DWORD dwTick)
{
	if( m_bVerifyLock )
		return;

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    if (NeedRenameProcess() == true)
    {
        // 이름변경 프로세스가 필요하면(진행중이면) 리턴
        return;
    }
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE

    if (m_bCreateCharacter)
    {
        ProcessMouseCreateCharacterMode(dwTick);
        return;
    }

    //Todo:캐릭터 삭제 진행시 선택 못바꾸게
    if(g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_DOWN) )
    {
        SelectCharecter();
    }

}

void CharacterScene::ProcessMouseCreateCharacterMode( DWORD dwTick )
{
	bool RotNotis = false;
	uiCreateCharacterMan* pCreateMan = GET_DIALOG_MANAGER(uiCreateCharacterMan, UIMAN_CREATECHARACTER);
	if(pCreateMan)
	{
		int posx = Mouse::Instance()->GetMouseX();
		int posy = Mouse::Instance()->GetMouseY();
		RotNotis = pCreateMan->OnOverWindowInCreate(posx,posy);
	}

	if(RotNotis)	
	{
		return;
	}

    ProcessCreateCharacterMouse();

    if (GET_DIALOG(IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG)->IsVisible())
    {
        LPDIMOUSESTATE2	pMouseState = g_Input.GetMouseBuffer();
        
#ifdef	_DEBUG
        if( g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_KEY ) )
        {
            g_Camera.Rotate( pMouseState->lX, pMouseState->lY, Camera::CAMERA_ROTATE_RMOUSE );
        }
#endif

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

		if( pMouseState->lZ < 0 )
		{
			ZoomCamera( -0.2f );
		}
		else if( pMouseState->lZ > 0 )
		{
			ZoomCamera( 0.2f );
		}

#else
        g_Camera.MoveFrontBack( pMouseState->lZ );

        Object* pCamera = g_ObjectManager.GetObject(m_dwCameraObjectKey);
        if (pCamera != NULL)
        {
            float min_distance = 1.0f;
            if (min_distance > g_Camera.GetCameraDistance())
            {
                g_Camera.SetCameraDistance(min_distance);
            }
            float max_distance = 4.0f;
            if (max_distance < g_Camera.GetCameraDistance())
            {
                g_Camera.SetCameraDistance(max_distance);
            }

            float camera_distance = g_Camera.GetCameraDistance();
            camera_distance -= 1.0f;
            camera_distance = max(camera_distance, 0.0f);
            WzVector from_position = pCamera->GetBonePos(34);
            WzVector to_position = pCamera->GetBonePos(35);

            switch (m_bySelectedModel)
            {
            case eCHAR_BERSERKER:
                {
                    from_position.x += (camera_distance * 0.5f);
                    from_position.y += (camera_distance * 0.5f);
                    from_position.z += (camera_distance * 0.5f);

                    to_position.x -= (camera_distance * 0.5f);
                    to_position.y -= (camera_distance * 0.2f);
                    to_position.z -= (camera_distance * 0.5f);
                }
                break;
            case eCHAR_DRAGON:
                {
                    from_position.x += (camera_distance * 0.45f);
                    from_position.y += (camera_distance * 0.45f);
                    from_position.z += (camera_distance * 0.1f);

                    to_position.x -= (camera_distance * 0.45f);
                    to_position.y -= (camera_distance * 0.2f);
                    to_position.z -= (camera_distance * 0.2f);
                }
                break;
            case eCHAR_SHADOW:
                {
                    from_position.x += (camera_distance * 0.45f);
                    from_position.y += (camera_distance * 0.5f);
                    from_position.z += (camera_distance * 0.2f);

                    to_position.x -= (camera_distance * 0.45f);
                    to_position.y -= (camera_distance * 0.2f);
                    to_position.z -= (camera_distance * 0.2f);
                }
                break;
            case eCHAR_VALKYRIE:
            case eCHAR_MAGICIAN:
                {
                    from_position.x += (camera_distance * 0.5f);
                    from_position.y += (camera_distance * 0.5f);
                    from_position.z += (camera_distance * 0.25f);

                    to_position.x -= (camera_distance * 0.5f);
                    to_position.y -= (camera_distance * 0.2f);
                    to_position.z -= (camera_distance * 0.3f);
                }
                break;
            }

            WzVector direction = from_position - to_position;
            VectorNormalize(&direction, &direction);
            from_position = from_position + (direction * camera_distance);
            g_Camera.SetCameraFromTo(from_position, to_position);
        }

#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        g_Camera.UpdateCamera();
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL CharacterScene::SendCharSelectPacket()
{
    if (BIT_CHECK(m_packetStatus, PACKET_STATUS_CHARACTERSCENE_SELCHAR) == FALSE)
    {
#ifdef _NA_000000_20130114_RENEWER_UI 
		uiCharacterSelectList2* select_list = GET_CAST_DIALOG(uiCharacterSelectList2, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#else
		uiCharacterSelectList* select_list = GET_CAST_DIALOG(uiCharacterSelectList, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#endif


        if (select_list == NULL)
        {
            return FALSE;
        }

        BYTE slot_index = static_cast<BYTE>(select_list->GetSelectedCharacterSlotIndex());
        CharListInfo* character_info = GetCharacterInfoBySlotIndex(slot_index);
        if (character_info == NULL)
        {
            return FALSE;
        }

        DWORD character_object_key = character_info->dwObjectKey;
        if ( Player* select_character = static_cast<Player*>(g_ObjectManager.GetObject(character_object_key)) )
        {
            if( Pet* pet= static_cast<Pet*>(g_ObjectManager.GetObject(select_character->GetPetObjectKey())) )
            {
                pet->SetPetLock(FALSE);
            }

            if (select_character->m_DelChk != 0)
            {
                CharSelectDialog* charater_select_dialog = GET_CAST_DIALOG(CharSelectDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG);
                if (charater_select_dialog)
                {
                    charater_select_dialog->OnDeleteCharacter();
                }

                return FALSE;
            }
        }

        if (GENERALPARAM->IsNet() == TRUE)
        {
            m_MsgCharSelectSync.m_bySelectedCharIndex = slot_index;
            m_dwSelectCharacter = slot_index;

            GlobalFunc::SendPacket( CI_GAMESERVERIDX, &m_MsgCharSelectSync, sizeof(m_MsgCharSelectSync));

            BIT_ADD(m_packetStatus, PACKET_STATUS_CHARACTERSCENE_SELCHAR);
        }
        else
        {
            CharListInfo* char_info = GetCurSelectCharacterInfo();
            BattleScene::SetHeroInfo(char_info->CharacterInfo.m_byClass);
#ifdef MAP_EDIT_MODE

            xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
            g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_MAPEDIT, GENERALPARAM->GetStartMapID(), GENERALPARAM->GetStartFieldID());
            GameFramework::ChangeScene(SCENE_TYPE_LOAD);
#else       
            xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
            const sMAPINFO* mapInfo = MapInfoParser::Instance()->FindMapInfo(GENERALPARAM->GetStartMapID());
            SCENE_TYPE eSceneType;
            if(eZONETYPE_INSTANCE == mapInfo->byMKind)
                eSceneType = GlobalFunc::ConvertClientZoneType((eZONETYPE)mapInfo->byMKind, (eINSTANCE_DUNGEON_KIND)mapInfo->byMType);
            else 
                eSceneType = GlobalFunc::ConvertClientZoneType((eZONETYPE)mapInfo->byMKind);
            g_LoadScene.SetNextSceneAfterLoaded(eSceneType, GENERALPARAM->GetStartMapID(), GENERALPARAM->GetStartFieldID());
            GameFramework::ChangeScene(SCENE_TYPE_LOAD);
#endif
        }
    }

#ifdef _DH_LAST_SELECTED_CHARACTER
    uiCharacterSelectMan* select_manager = GET_DIALOG_MANAGER(uiCharacterSelectMan, UIMAN_CHARACTERSELECT);
    if (select_manager)
    {
        select_manager->SetLastSelectCharacter(select_manager->last_selected_header().last_select_server, 
            static_cast<WORD>(m_dwSelectCharacter));
        select_manager->SaveLastSelectedData();
    }
#endif//_DH_LAST_SELECTED_CHARACTER
    return TRUE;
}

void CharacterScene::OnStartGame( int MapCode )
{
    g_InterfaceManager.ReadySkillInfos();

    // 캐릭터 이동할 멥 설정
    if (GENERALPARAM->IsNet())
    {
        g_Camera.SetCameraDistance(7.5f);

        g_LoadScene.SetNextSceneAfterLoaded( MapCode, 0 );
        GameFramework::ChangeScene( SCENE_TYPE_LOAD );
    }
    else
    {

        g_Camera.SetCameraDistance(7.5f);


#ifdef MAP_EDIT_MODE
        xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
        g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_MAPEDIT, GENERALPARAM->GetStartMapID(), GENERALPARAM->GetStartFieldID());
        GameFramework::ChangeScene(SCENE_TYPE_LOAD);

#else
        xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );

        const sMAPINFO* mapInfo = MapInfoParser::Instance()->FindMapInfo(GENERALPARAM->GetStartMapID());
        SCENE_TYPE eSceneType;
        if(eZONETYPE_INSTANCE == mapInfo->byMKind)
            eSceneType = GlobalFunc::ConvertClientZoneType((eZONETYPE)mapInfo->byMKind, (eINSTANCE_DUNGEON_KIND)mapInfo->byMType);
        else 
            eSceneType = GlobalFunc::ConvertClientZoneType((eZONETYPE)mapInfo->byMKind);
        g_LoadScene.SetNextSceneAfterLoaded(eSceneType, GENERALPARAM->GetStartMapID(), GENERALPARAM->GetStartFieldID());

        GameFramework::ChangeScene(SCENE_TYPE_LOAD);
#endif
    }	
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::ProcessUnits(DWORD dwTick)
{
    //프로세스는 전부 돈다.
    Object_Map_Itr	itr = g_ObjectManager.GetBegin();
    while( g_ObjectManager.GetEnd() != itr )
    {
        Object *pObj = (Object *)itr->second;
        if(pObj->GetSCObject().GetObjectType()==PLAYER_OBJECT)
        {
            Player *pPlayer=(Player *)pObj;

            ProcessState(pPlayer,dwTick);

            pPlayer->ProcessInterpolation( dwTick );

            float ft=pPlayer->ProcessAnimation( dwTick );

			ObjectStatus *pStatus=  GetStatus(pPlayer->GetObjectKey());
			if(pStatus)
			{
				pStatus->animation_rate=ft;
			}

            if( ft>= PERFECT && m_dwSelectCharacter == pPlayer->GetCharInfo()->m_bySlot && BIT_CHECK(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_SELCHAR) )
            {
                OnStartGame( pPlayer->GetCharInfo()->m_iRegion );
            }							 
        
        }
        else
        {
			if (pObj->IsKindOfObject(CAMERA_OBJECT))
			{
				m_fCameraRate = pObj->ProcessAnimation( dwTick );
			}
			else			
			{
				pObj->Process(dwTick);
			}
            
        }
        ++itr;
    }
}

//------------------------------------------------------------------------------
/**
캐릭터 상태 변화 연출 체크 
*/
ObjectStatus * CharacterScene::GetStatus(DWORD ObjectKey)
{
    for( size_t i=0;i!=m_StatusArray.size();++i)
    {
        if(m_StatusArray[i]->object_key == ObjectKey)
        {
            return m_StatusArray[i];
        }
    }

    return NULL;
}

void ObjectStatus::SetMovingBackStatus()
{
    m_CurrentStatus = CHARACTER_STATUS_MOVE_BACK; 
}

void ObjectStatus::SetIdleStatus()
{
    m_CurrentStatus = CHARACTER_STATUS_IDLE;  

    if( Player* pPlayer = static_cast<Player*> ( g_ObjectManager.GetObject( object_key ) ) )
    {
        pPlayer->SetAnimation( StrToWzID("N001"), TRUE );	
    }
    status_time = 0;

    SetDefaultStatus();
}

//------------------------------------------------------------------------------ 
WzID GetCreateCharacterPositionAniCode()
{
    return (g_pApplication->IsWideModeFlag() ? StrToWzID("N001") : StrToWzID("N101"));
}


void ObjectStatus::SetSelectIdleStatus()
{
    if( IsSelected() )
    {
        if( Player* pPlayer = static_cast<Player*> ( g_ObjectManager.GetObject( object_key ) ) )
        {
            char szAniCode[5];
            ZeroMemory(szAniCode,sizeof(szAniCode));

            szAniCode[0] = 'N';
            szAniCode[1] = pPlayer->GetWeaponAnimationCode();
            szAniCode[2] = '0';
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
            szAniCode[3] = '1';
#else
            szAniCode[3] = '3';
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

            pPlayer->SetAnimation( szAniCode, TRUE );
        }
    }
    else
    {
        if( Player* pPlayer = static_cast<Player*> ( g_ObjectManager.GetObject( object_key ) ) )
        {
            pPlayer->SetAnimation( GetCreateCharacterPositionAniCode() /*"N001"*/, TRUE );
        }
    }

    SetDefaultStatus();
}

void ObjectStatus::SetMovingFrontStatus() 
{
    m_CurrentStatus = CHARACTER_STATUS_MOVE_FRONT;
}

void ObjectStatus::SetStartMotion()
{
    if( Player* pPlayer = static_cast<Player*> ( g_ObjectManager.GetObject( object_key ) ) )
    {

#ifdef _NA_000000_20120920_STARTMOTION_CODE
		pPlayer->SetAnimation( "CE00", FALSE );	
#else
        if ((pPlayer->GetClass() == eCHAR_BERSERKER) || 
            (pPlayer->GetClass() == eCHAR_DRAGON) )
            pPlayer->SetAnimation( "CE08", FALSE );	
        else
            pPlayer->SetAnimation( "CE00", FALSE );	
#endif// _NA_000000_20120920_STARTMOTION_CODE
    }

    SetDefaultStatus();
}

//------------------------------------------------------------------------------
/**
        캐릭터 상태 변화 연출 체크 
        Todo:이동 시간 단위로 맞출것,소스 정리-작업중  
*/
void CharacterScene::ProcessState( Player * pPlayer,DWORD dwTick )
{
    if( ObjectStatus* pObjectState = GetStatus(pPlayer->GetObjectKey()) )
    {
        if( pObjectState->IsSelected() == FALSE )
        {
            if( pObjectState->IsSelectIdleStatus() == TRUE )
            {
                // 모션을 취하는등의 행동중이라면 애니메이션이 종료된 후에 돌아가도록
                if(pObjectState->animation_rate >= PERFECT)
                {
                    pObjectState->status_time = pObjectState->status_time_backup;
                    pObjectState->SetMovingBackStatus();
                }
            }
            else if( pObjectState->IsSelected() == FALSE && pObjectState->IsIdleStatus() == FALSE )
            {
                // 그외의 경우에는 바로 돌아가도록
                pObjectState->status_time = pObjectState->status_time_backup;
                pObjectState->SetMovingBackStatus();
            }
        }


        
        if( pObjectState->IsSelected() )
        {
            if( pObjectState->IsIdleStatus() )
            {
                // 앞으로 걸어가도록 한다
                pObjectState->SetMovingFrontStatus();
            }
        }

        if( pObjectState->IsMovingFrontStatus() )
        {
            //시간차가 열라 클경우 
            int twRemainder = 0;
            pPlayer->SetAnimation( "N006", TRUE );	

            pObjectState->status_time += dwTick;

            //2.8초 동안 앞으로 걸어간다
            int iTotalTime = 2800;

            if(pObjectState->status_time>=iTotalTime)
            {
                twRemainder = pObjectState->status_time - iTotalTime;
                pObjectState->status_time=iTotalTime;
                pObjectState->SetSelectIdleStatus();
            }

            WzVector curPos = pPlayer->GetVisiblePos();
            WzVector dir = pPlayer->GetDirection();
            //Todo:이동 거리 시간 처리 할것 
            dir.x = dir.x*WALK_SPEED*(dwTick-twRemainder);
            dir.y = dir.y*WALK_SPEED*(dwTick-twRemainder);
            curPos += dir;
            pPlayer->SetPosition(curPos);
        }
        else if( pObjectState->IsMovingBackStatus() )
        {
            int twRemainder = 0;
            //Todo:뒤로 걷기 처리 
            pPlayer->SetAnimation( "N011", TRUE );	

            pObjectState->status_time -= dwTick;
            if(pObjectState->status_time <= 0)
            {
                twRemainder = pObjectState->status_time;
                pObjectState->status_time = 0;
                pObjectState->SetIdleStatus();
            }

            WzVector curPos = pPlayer->GetVisiblePos();
            WzVector dir = pPlayer->GetDirection();

            //Todo:이동 거리 시간 처리 할것
            dir.x=dir.x*WALK_SPEED*(dwTick+twRemainder);
            dir.y=dir.y*WALK_SPEED*(dwTick+twRemainder);

            curPos -= dir;
            pPlayer->SetPosition(curPos);
        }
        
        if( pObjectState->IsSelectIdleStatus() )
        {
		    if(m_dwSelectOid==pPlayer->GetObjectKey())
		    {
			    char szAniCode[5];
			    ZeroMemory(szAniCode,sizeof(szAniCode));

			    szAniCode[0] = 'N';
			    szAniCode[1] = pPlayer->GetWeaponAnimationCode();
			    szAniCode[2] = '0';
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
                szAniCode[3] = '1';
#else
			    szAniCode[3] = '3';
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
			    pPlayer->SetAnimation( szAniCode, TRUE );
		    }
		    else
		    {
			    pPlayer->SetAnimation( GetCreateCharacterPositionAniCode() /*"N001"*/, TRUE );
		    }

            pObjectState->SetDefaultStatus();
        }
    }
}

void CharacterScene::SetCinematicCharShow( BOOL b )
{
    if (b != FALSE)
    {
        ShowBaseCinematicCharacter();
    }
    else
    {
        // 선택된 클래스를 제외하고 숨김
        ShowOnlyCinematicCharacter(m_dwSelectedObjectKey);  
    }
}

//------------------------------------------------------------------------------
/**
    카메라 설정 : 히어로( 내 캐릭터 ) 중심으로 위치
*/
void CharacterScene::ProcessCamera(LPDIMOUSESTATE2 pMouseState,DWORD dwTick)
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

		xGetCharacterCreate()->ProcessCamera( (eCHAR_TYPE)m_bySelectedClass );
#else

	if (m_bCreateCharacter)
	{
		if (GET_DIALOG(IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG)->IsVisible())	// 커스텀 윈도우 활성화 시엔 애니카메라를 작동시키지 않는다.
        {
			return;
        }

		if ( Object* pCamera = g_ObjectManager.GetObject(m_dwCameraObjectKey) )
        {

#ifdef _KST_CREATECHARACTER_SPRINGCAMERA_
			
			g_Camera.SetSpringCameraPosition( pCamera->GetBonePos(34), pCamera->GetBonePos(35), dwTick );
			g_Camera.UpdateCamera();	// 직접 업데이트 해준다.

#else // _KST_CREATECHARACTER_SPRINGCAMERA_

			g_Camera.AttachToBone( pCamera->GetBonePos(34), pCamera->GetBonePos(35) );

#endif // _KST_CREATECHARACTER_SPRINGCAMERA_
			return;
		}
	}

    // 카메라 초기화 
	WzVector wzCamera, wzTarget;
    SetVector(&wzCamera, -5.443278f, 6.583461f, 10.761790f);
    SetVector(&wzTarget, -5.358036f, 5.587751f, 10.697770f);

	g_Camera.SetCameraFromTo(wzCamera,wzTarget);

	g_Camera.SetForceSpringCamera( FALSE );
	g_Camera.UpdateCamera();

#endif

}

//------------------------------------------------------------------------------
/**
*/
BOOL CharacterScene::Process( DWORD dwTick )
{  
    //------------------------------------------------------------------------------
    //! 중국 캐릭터 생성후 동영상 보기
    if (Movie.is_show_movie  == true)
    {
        bool is_end = (!Movie.bink ||	
            g_Input.IsPressAnyKey() ||
            (!BinkWait(Movie.bink) && !NextFrame(Movie.bink)));	

        if (is_end == true)
        {
            set_show_movie(false);
        }
        return TRUE;
    }
    //------------------------------------------------------------------------------


    g_CurTime = g_pSunRenderer->x_Clock.GetCurrentTick();
    g_pMap->Update(dwTick);
    LPDIMOUSESTATE2	pMouserState = g_Input.GetMouseBuffer();
	if (m_bCreateCharacter)
	{
		ProcessScrollInterface(dwTick);
	}
    
    ProcessKeyInput( dwTick );
    ProcessMouse( dwTick );
    ProcessUnits( dwTick );
	ProcessCamera( pMouserState ,dwTick);
    ProcessLoadCharacter();

	m_bEnableRender = TRUE;

    return TRUE;
}

//------------------------------------------------------------------------------
/**
    마우스 피킹을 위해 마우스 커서 레이를 얻어오는데,
	현재 업데이트 프레임에서 구해야 하는데( 초당 60 회 )
	여기 말고는 제대로 된 값이 얻어지지 않는다.
*/
BOOL CharacterScene::Render( DWORD dwTick )
{
    //------------------------------------------------------------------------------
    //! 중국 캐릭터 생성후 동영상 보기
    if (Movie.is_show_movie  == true && Movie.bink != NULL)
    {
        POINT pt = g_pApplication->Get2DModeMapping(); 

        FRECT area;
        float screen_ratio = static_cast<float>(pt.y) / static_cast<float>(pt.x);
        float movie_ratio = static_cast<float>(Movie.bink->Height) / static_cast<float>(Movie.bink->Width);
        if (screen_ratio > movie_ratio)
        {
            // 위아래 여백
            float size_ratio = static_cast<float>(pt.x) / static_cast<float>(Movie.bink->Width);
            float w_size = Movie.bink->Width * size_ratio;
            float h_size = Movie.bink->Height * size_ratio;

            area.left	= 0;
            area.right	= w_size;
            area.top	= (pt.y - h_size) * 0.5f;
            area.bottom	= h_size;
        }
        else
        {
            // 좌우 여백
            float size_ratio = static_cast<float>(pt.y) / static_cast<float>(Movie.bink->Height);
            float w_size = Movie.bink->Width * size_ratio;
            float h_size = Movie.bink->Height * size_ratio;

            area.left	= (pt.x - w_size) * 0.5f;
            area.right	= w_size;
            area.top	= 0;
            area.bottom	= h_size;
        }

        g_pSunRenderer->ClearRender();  
        g_pSunRenderer->Set2DMode(TRUE, FALSE);
        {
            g_pSunRenderer->RenderTexture(Movie.bink_texture, area.left, area.top, area.right, area.bottom - 1, 0.0f, 0.0f);

#ifdef _DEV_VER
            g_pSunRenderer->x_pManagerTextOut->StartToUseFont("DFLT");
            {
                TCHAR pText[256] = {NULL, };
                Snprintf(pText, 256-1, _T("Current Frame : %d"), Movie.bink->FrameNum );
                g_pSunRenderer->x_pManagerTextOut->DrawText(pText, 30, (pt.y - 30), WzColor_RGB(0x0F, 255, 0x0f));
            }
            g_pSunRenderer->x_pManagerTextOut->EndUseFont();
#endif //_dev_ver
        }
        g_pSunRenderer->Set2DMode(FALSE);

        if(g_pEffectManager)
        {
            g_pEffectManager->Move( dwTick );
        }
        g_pSunRenderer->x_pManagerPostEffect->Apply();

        return TRUE;
    }
    //------------------------------------------------------------------------------

    if( m_bEnableRender == FALSE )
    {
        g_pSunRenderer->RenderTexture( m_hTexture, 0.0f, 0.0f, 1024.0f, 768.0f, 0.0f, 0.0f );
		g_pSunRenderer->x_pManagerPostEffect->Apply();
        return TRUE;
    }

	if(m_bFristRender)
	{
		m_bFristRender=FALSE;
        float fGmmma = GAMEOPTION->GetGameOption().GetOptionValueFloat( nOptionType::GAMMA_VALUE );
		SetGamma(DWORD( fGmmma ) );
	}

	if(g_pEffectManager)
		g_pEffectManager->Move( dwTick );

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
	if (m_bCreateCharacter)
	{
//#ifdef _YJ_RENEWAL_CHARACTER_CREATE_SCENE
//        St_FogParameter old_fog_param;
//        BOOL ret_fog = g_pSunRenderer->IsFogEnabled();
//        g_pSunRenderer->EnableFog(TRUE);
//        old_fog_param=g_pMap->GetWorldBase()->GetFogParameter();
//        g_pMap->GetWorldBase()->SetFogParameter(g_pSunRenderer,m_stFogCharCreate);
//        g_pSunRenderer->GetMouseCursorRay( &g_wrMouseRay );
//
//        g_pMap->Render(dwTick);
//        RenderCinematicCharacter();
//        g_pSunRenderer->x_pManagerPostEffect->Apply();
//
//        g_pMap->GetWorldBase()->SetFogParameter(g_pSunRenderer,old_fog_param);
//        g_pSunRenderer->EnableFog(ret_fog);
//#else //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
		RenderCharacterCreate();
//#endif //_YJ_RENEWAL_CHARACTER_CREATE_SCENE

		return TRUE;
	}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
 
    g_pSunRenderer->GetMouseCursorRay( &g_wrMouseRay );
    g_pSunRenderer->SetCullFaceMode( CF_DRAW_FRONT );

    //배경 출력,오브젝트 출력  
    g_pMap->Render(dwTick);

    SUNPROFILE_BEGIN( _T("RenderUnitText") );
    //이펙트와의 Alpha문제로 이펙트 렌더 이후로 호출 시점 조정
    g_pMap->RenderUnitInfo();
    SUNPROFILE_END( _T("RenderUnitText") );

	Object_Map_Itr tempitr;
	Object_Map_Itr itr = g_ObjectManager.GetBegin();
	while(itr != g_ObjectManager.GetEnd() )
	{
		Object * pObject = static_cast<Object *>(itr->second);
		if (0 == pObject)
		{
			assert(!"Object Is NULL!!");
			return FALSE;
		
		}
		itr++;	
	}

	g_pSunRenderer->x_pManagerPostEffect->Apply();

	SceneBase::Render(dwTick);

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
CharListInfo * CharacterScene::GetCurSelectCharacterInfo( )
{
    CHARLIST_ITR itr = m_CharList.begin();
    CharListInfo * pCharListInfo = NULL;

    for (; itr != m_CharList.end(); ++itr)
    {
        pCharListInfo = static_cast<CharListInfo *>(*itr);
        if (pCharListInfo && 
            pCharListInfo->CharacterInfo.m_bySlot == (BYTE)m_dwSelectCharacter)
        {
            return pCharListInfo;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
CharListInfo * CharacterScene::GetCharacterInfoBySlotIndex(int slot_index)
{
    CHARLIST_ITR itr = m_CharList.begin();
    CharListInfo * pCharListInfo = NULL;

    for (; itr != m_CharList.end(); ++itr)
    {
        pCharListInfo = static_cast<CharListInfo *>(*itr);
        if ((pCharListInfo != NULL) && 
            pCharListInfo->CharacterInfo.m_bySlot == static_cast<BYTE>(slot_index))
        {
            return pCharListInfo;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::OnRecvCG_SYNC(MSG_BASE * pMsg)
{
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::OnRecvCG_CHARINFO(MSG_BASE * pMsg)
{
    switch( pMsg->m_byProtocol )
    {

	case CG_SECURE_QUERY_RESOURCE_CHK_T2_CMD:
		{
			ISecureWorkNode::OnMsgRecvSignal::ResourceCheckT2( pMsg, 0 );
		}
		break;

	case CG_CHARINFO_CHAR_ITEM_CMD:
		{
			// 캐릭정보 + 인벤 + 임시 인벤
			MSG_CG_CHARINFO_CHAR_ITEM_CMD * pRecvMsg = (MSG_CG_CHARINFO_CHAR_ITEM_CMD *)pMsg;
			pRecvMsg->Decode();

			CharListInfo* pSelectCharInfo = GetCurSelectCharacterInfo();

            xGetHeroData()->SetHeroInfo( pSelectCharInfo->CharacterInfo, pRecvMsg->m_CharacterInfo );
            xGetHeroData()->SetEquipItemInfo( &pSelectCharInfo->CharacterInfo.m_EquipItemInfo );
            xGetHeroData()->SetItemStream( pRecvMsg->m_BitStreamBuffer, pRecvMsg->m_Size );


            const HERO_INFO* pHeroInfo = xGetHeroData()->GetHeroInfo();
#ifdef _NA_000000_20130114_RENEWER_UI
#else
			if( OptionGameDialog * pOptionDialog = GET_CAST_DIALOG( OptionGameDialog, IM_OPTION_MENU_MANAGER::OPTION_GAME_DIALOG ) )
            {
				pOptionDialog->InitBlockCheck( pHeroInfo->m_CharInfo.m_bTradeFlag, pHeroInfo->m_CharInfo.m_bWhisperFlag );
            }
#endif

#if !defined ( __CN_OO0510__FATIGUE_MATCHING_UPDATE )
            TiredSystemDialog* pTiredDlg = GET_CAST_DIALOG(TiredSystemDialog, IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM);
            pTiredDlg->SetTime( pHeroInfo->m_CharInfo.m_PlayLimitedTime );
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE


			if (GENERALPARAM->GetSpecialMode()) 
			{
				if(pRecvMsg->m_CharacterInfo.m_ChaoState==eCHAO_STATE_CHAO)
				{
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,_T("카오 상태"));
				}
				else if(pRecvMsg->m_CharacterInfo.m_ChaoState==eCHAO_STATE_PREV_CHAO)
				{
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,_T("프리 카오 상태"));
				}
			}
		}
		break;
	case CG_CHARINFO_SKILL_CMD:
		{
			// 스킬
			MSG_CG_CHARINFO_SKILL_CMD * pRecvMsg = (MSG_CG_CHARINFO_SKILL_CMD *)pMsg;
            xGetHeroData()->SetSkillTotalInfo( &pRecvMsg->m_SkillInfo );
		}
		break;

	case CG_CHARINFO_QUICK_CMD:
		{
			// 퀵
			MSG_CG_CHARINFO_QUICK_CMD * pRecvMsg = (MSG_CG_CHARINFO_QUICK_CMD *)pMsg;
            xGetHeroData()->SetQuickTotalInfo( &pRecvMsg->m_QuickInfo );
		}
		break;

	case CG_CHARINFO_STYLE_CMD:
		{
			// 스타일 퀵
			_asm nop;
		}
		break;

    case CG_CHARINFO_CREATE_ACK:
        {
            //Todo:캐릭터 생성 처리 
#ifdef _NHN_USA_CHANNELING
            GlobalFunc::SendHanReportPacket(302);//캐릭생성(302)
#endif//_NHN_USA_CHANNELING(북미 채널링:지표)      
            BIT_REMOVE(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_CREATECHAR);
            MSG_CG_CHARINFO_CREATE_ACK * pRecvPacket = (MSG_CG_CHARINFO_CREATE_ACK *)pMsg;

            CLIENT_CHARACTER_PART CharacterListForRecv;
            ZeroMemory(&CharacterListForRecv, sizeof(CharacterListForRecv));
            pRecvPacket->ConvertForRecv(CharacterListForRecv);

            // 캐릭터 추가
            if( CharListInfo * pCharListInfo = new CharListInfo )
            {
                DWORD ObjectKey = CHARACTERSCENE::PLAYER_LIST + CharacterListForRecv.m_bySlot;
                pCharListInfo->CharacterInfo = CharacterListForRecv;
                pCharListInfo->dwObjectKey = ObjectKey;

                MakeMyChar( ObjectKey, CharacterListForRecv );
                m_CharList.push_back( pCharListInfo );
            }

            // 생성에 성공했을때만 창을 닫는다.
            g_CharSelectScene.SetCreateCharacterMode(FALSE);

            g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CHARSELECT,TRUE);
            uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
            if (login_back_top != NULL)
            {
                login_back_top->ShowInterface(TRUE);
            }

#ifdef _NA_000000_20130114_RENEWER_UI 
			uiCharacterSelectList2* select_list_dialog = GET_CAST_DIALOG(uiCharacterSelectList2, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#else
			uiCharacterSelectList* select_list_dialog = GET_CAST_DIALOG(uiCharacterSelectList, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#endif
            if (select_list_dialog != NULL)
            {
                select_list_dialog->SetSelectedCharacterListIndex(CharacterListForRecv.m_bySlot);
            }            

#ifdef _YJW_CREATECHARACTER_MOVIE
            set_show_movie(true);
#endif //_YJW_CREATECHARACTER_MOVIE
        }
        break;

    case CG_CHARINFO_CREATE_NAK:
        {

            MSG_CG_CHARINFO_CREATE_NAK * pRecvPacket = (MSG_CG_CHARINFO_CREATE_NAK *)pMsg;
            //Todo:캐릭터 생성 실패
            BIT_REMOVE(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_CREATECHAR);

			ShowErrorCode(pRecvPacket->m_dwErrorCode,TRUE);

        }
        break;

        
    case CG_CHARINFO_DESTROY_ACK:
        {
            //Todo:캐릭터 삭제 처리 
            BIT_REMOVE(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_DELCHAR);	
            if(m_curDelOid!=INVALID_OBJECT_KEY)
            {
                CHARLIST_ITR itr = m_CharList.begin();
                CharListInfo * pCharListInfo = NULL;

                for (; itr != m_CharList.end(); ++itr)
                {
                    pCharListInfo = static_cast<CharListInfo *>(*itr);
					
                    if(pCharListInfo->CharacterInfo.m_bySlot==m_curDelOid)
                    {
						uiHelpMan* helpMan =
								static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

						if(helpMan)
						{
							helpMan->ResetReadInfo(pCharListInfo->CharacterInfo.m_tszCharName);
						}

						if( pCharListInfo->CharacterInfo.m_LV >= CHAR_DESTROY_LEVEL)
						{
							// 시간 포멧 2006-10-12 24:03 ( 분단위 )
							// 삭제된 시간을 임의로 넣어준다.
							Player * pPlayer = static_cast<Player *> ( g_ObjectManager.GetObject(pCharListInfo->dwObjectKey) );
							
							if( pPlayer)
							{
								SYSTEMTIME LTime;
								util::TimeSync::GetLocalTime(&LTime);
								TCHAR szMonth[5] , szDay[5] , szHour[5] , szMinute[5];
								// 숫자 포멧에 맞추기 위해 0보다 작은 숫자는 앞에 0을 붙인다. ex) 9 -> 09
								AddZeroWhenUnder10(szMonth , LTime.wMonth);
								AddZeroWhenUnder10(szDay , LTime.wDay);
								AddZeroWhenUnder10(szHour , LTime.wHour);
								AddZeroWhenUnder10(szMinute , LTime.wMinute);
								Sprintf(pPlayer->m_tszDelDate , _T("%d-%s-%s %s:%s"),
									LTime.wYear , szMonth , szDay, szHour , szMinute );
								pPlayer->m_DelChk = 3;
								uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
								TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

								TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
								// 5039 %s 캐릭터를 삭제 처리했습니다. 7일 후에 완전히 삭제됩니다.
								g_InterfaceManager.GetInterfaceString(5039, szTemp ,INTERFACE_STRING_LENGTH);
								Snprintf( szMessage ,INTERFACE_STRING_LENGTH -1, szTemp , pPlayer->GetName() );
								if(pSystemMan)
									pSystemMan->Sys_VerifyLock(szMessage);

                                pCharListInfo->CharacterInfo.m_DelChk = 3;

#ifdef _NA_000000_20130114_RENEWER_UI 
								uiCharacterSelectList2* select_list = GET_CAST_DIALOG(uiCharacterSelectList2, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#else
								uiCharacterSelectList* select_list = GET_CAST_DIALOG(uiCharacterSelectList, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#endif
                                if (select_list != NULL)
                                {
                                    select_list->CharacterListUp();
                                }
							}
							break;
							
						}

                        DeleteMyChar(pCharListInfo->dwObjectKey);

						delete pCharListInfo;
                        m_CharList.erase(itr);

#ifdef _NA_000000_20130114_RENEWER_UI 
						uiCharacterSelectList2* select_list = GET_CAST_DIALOG(uiCharacterSelectList2, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#else
						uiCharacterSelectList* select_list = GET_CAST_DIALOG(uiCharacterSelectList, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#endif
                        if (select_list != NULL)
                        {
                            select_list->CharacterListUp();
                        }

						if(m_dwSelectCharacter==m_curDelOid)
							m_dwSelectCharacter=INVALID_OBJECT_KEY;
                        m_curDelOid=INVALID_OBJECT_KEY;
                        break;
                    }
                }
            }
        }
        break;

    case CG_CHARINFO_DESTROY_NAK:
        {
			MSG_CG_CHARINFO_DESTROY_NAK * pRecvPacket = (MSG_CG_CHARINFO_DESTROY_NAK *)pMsg;

            BIT_REMOVE(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_DELCHAR);
            m_curDelOid = INVALID_OBJECT_KEY;
			//	CG_CHARINFO_DESTROY_NAK!!
			//	캐릭터 삭제 실패
			ShowErrorCode(pRecvPacket->m_dwErrorCode);

        }
        break;

	case CG_CHARINFO_RECOVER_ACK:
		{
			BIT_REMOVE(m_packetStatus , PACKET_STATUS_CHARACTERSCENE_RECOVERY);
		
			Player * pPlayer = ( Player *) g_ObjectManager.GetObject( GetCurrentSelectedPlayerKey() ) ;
			if( pPlayer )
			{
				pPlayer->m_DelChk = 0;
			}
            CharListInfo* char_info = g_CharSelectScene.GetCurSelectCharacterInfo();
            if (char_info)
            {
                char_info->CharacterInfo.m_DelChk = 0;
            }

#ifdef _NA_000000_20130114_RENEWER_UI 
			uiCharacterSelectList2* select_list = GET_CAST_DIALOG(uiCharacterSelectList2, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#else
			uiCharacterSelectList* select_list = GET_CAST_DIALOG(uiCharacterSelectList, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#endif
            if (select_list != NULL)
            {
                select_list->CharacterListUp();
            }
		}
		break;

	case CG_CHARINFO_RECOVER_NAK:
		{
			MSG_CG_CHARINFO_RECOVER_NAK * pRecvPacket = ( MSG_CG_CHARINFO_RECOVER_NAK *) pMsg;
			BIT_REMOVE(m_packetStatus , PACKET_STATUS_CHARACTERSCENE_RECOVERY);
			ShowRecoveryErrorCode(pRecvPacket->m_dwErrorCode);
		}	
		break;

	case CG_CHARINFO_CHARACTERID_CHECK_ACK:
	case CG_CHARINFO_CHARACTERID_CHECK_NAK:
		{
            OnRecvCharacterIDCheck(pMsg);
		}
		break;

    case CG_CHARINFO_ACCUMULATE_POINT_CMD:
        {
            SceneBase::OnRecvCG_CHARINFO(pMsg);
        }
        break;
    case CG_CHARINFO_SKILL_PERK_CMD:
        {
            uiBase* perk_dialog = GET_DIALOG(IM_SKILL_MANAGER::SKILL_PERK_DLG);
            if (perk_dialog != NULL)
            {
                perk_dialog->NetworkProc(pMsg);
            }            
        }
        break;
    }
}

void CharacterScene::OnRecvCharacterIDCheck(MSG_BASE* pMsg)
{
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    if (NeedRenameProcess() == true)
    {
        uiBase* rename_input_dialog = GET_DIALOG(IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME_INPUT);
        if (rename_input_dialog != NULL)
        {
            rename_input_dialog->NetworkProc(pMsg);
        }
        return;
    }
#endif

    switch (pMsg->m_byProtocol)
    {
    case CG_CHARINFO_CHARACTERID_CHECK_ACK:
        {
            g_InterfaceManager.ShowMessageBox(g_InterfaceManager.GetInterfaceString(5053), 2000, NULL, 0);
        }
        break;

    case CG_CHARINFO_CHARACTERID_CHECK_NAK:
        {
            g_InterfaceManager.ShowMessageBox(g_InterfaceManager.GetInterfaceString(5052), 2000, NULL, 0);
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
	Todo:스트링 테이블 요청 
*/
 void CharacterScene::ShowErrorCode(DWORD dwErrorCode,BOOL bCreate)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	if ( bCreate )
	{

		switch(dwErrorCode)
		{
		case RC_CHAR_CREATE_MINIMUM_CHARACTER:
			{
				//5025	이름은 영문 4자 한글 2자 이상 입력해주세요
				g_InterfaceManager.GetInterfaceString(5025 , szMessage ,INTERFACE_STRING_LENGTH );
			}
			break;

		case RC_CHAR_CREATE_CANNOT_CREATE_TO_SERVICE:
			{
				// 5009	해당 월드의 혼잡으로 신규 캐릭터 생성이 가능하지 않습니다. 다른 월드를 이용해 주시기 바랍니다.
				g_InterfaceManager.GetInterfaceString( 5009 , szMessage ,INTERFACE_STRING_LENGTH);
			}
			break;

		case RC_CHAR_CREATE_EXIST_SAME_NAME:
			{
				// 5230	동일한 이름의 캐릭터가 존재합니다.
				g_InterfaceManager.GetInterfaceString( 5230 , szMessage ,INTERFACE_STRING_LENGTH);
			}
			break;

		case RC_CHAR_CREATE_SLOT_FULL:
			{
				//70009	캐릭터 생성 개수가 초과되었습니다.
				g_InterfaceManager.GetInterfaceString(70009 , szMessage ,INTERFACE_STRING_LENGTH);
			}
			break;
		
		case RC_CHAR_CREATE_CHARNAME_BAN_WORD:
			{
				//	금지 단어 사용
				g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, MAX_MESSAGE_LENGTH );
			}
			break;
#ifdef _NA_20100307_BETAKEY_SYSTEM
        case RC_CHAR_CREATE_CANNOT_BETAKEY:
            {
                //사용자가 가진 베타키로는 해당 캐릭터를 생성할수 없습니다.
                // 70540: 조건이 맞지 않아 선택하신 클래스를 생성 할 수 없습니다.
                g_InterfaceManager.GetInterfaceString(70540, szMessage, MAX_MESSAGE_LENGTH );
            }
            break;
#endif//_NA_20100307_BETAKEY_SYSTEM
#ifdef _DEBUG
		case RC_CHAR_CREATE_ALREADY_DOING_TRANSACTION:																								
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] 이미 다른 트랜잭션을 진행중이다."));
			}
			break;

		case RC_CHAR_CREATE_INVALID_PARAM:
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH -1, _T("[DEBUG] 패킷이 잘못된 값입니다.") );
			}
			break;

		case RC_CHAR_CREATE_DISCONNECTED_DBPROXY:
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] DBP가 연결이 되지 않았습니다.(잠시뒤 서비스 이용바람)"));
			}
			break;

		case RC_CHAR_CREATE_TRANSACTION_ERROR:
			{
				Snprintf( szMessage ,INTERFACE_STRING_LENGTH -1, _T("[DEBUG] 트랜젝션 오류(쿼리오류,기타오류)") );
			}
			break;

		case RC_CHAR_CREATE_QUERY_ERROR:
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH - 1, _T("[DEBUG] DB쿼리 실패"));
			}
			break;

		case RC_CHAR_CREATE_DBUSER_NOT_EXIST:
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] DBProxy의 DB유저가 존재하지 않는다.") );
			}
			break;

		case RC_CHAR_CREATE_DBCHAR_ALREADY_CREATED:
			{
				Snprintf( szMessage ,INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] DBP의 캐릭터가 이미 만들어져 있다.") );
			}
			break;
#endif

		default:
			{
				g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CREATE_FAIL, szMessage, INTERFACE_STRING_LENGTH );
			}
			break;
		}
	}
	else
	{
		switch ( dwErrorCode )
		{
		case RC_CHAR_DESTROY_DESTROIED:
			{
				Snprintf( szMessage ,INTERFACE_STRING_LENGTH -1 , _T("이미 삭제된 캐릭터 입니다.") );
			}
			break;

		case RC_CHAR_DESTROY_INCORRECT_SSN:
			{
				// 5845	주민등록번호가 일치하지 않습니다.
				g_InterfaceManager.GetInterfaceString(5845 , szMessage ,INTERFACE_STRING_LENGTH );
			}
			break;

		case RC_CHAR_DESTROY_GUILD_MASTER:
			{
				// 5178	길드장 캐릭터입니다. 삭제가 가능하지 않습니다.
				g_InterfaceManager.GetInterfaceString(5178 , szMessage ,INTERFACE_STRING_LENGTH);
			}
			break;

		case RC_CHAR_DESTROY_GROUP_MEMBER:
			{
				// 7057	그룹 멤버를 모두 해제한 후 캐릭터 삭제가 가능합니다.
				g_InterfaceManager.GetInterfaceString(7057 , szMessage ,INTERFACE_STRING_LENGTH);
			}
			break;

#ifdef _DEBUG
		case RC_CHAR_RECOVER_ALREADY_DOING_TRANSACTION:
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH - 1, _T("[DEBUG] 이미 다른 트랜잭션을 진행중이다.") );
			}
			break;

		case RC_CHAR_DESTROY_INVALID_PARAM:
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] 패킷이 잘못된 값입니다.") );
			}
			break;
			
		case RC_CHAR_DESTROY_TRANSACTION_ERROR:
			{
				Snprintf ( szMessage , INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] 트랜젝션 오류(쿼리오류,기타오류)" ) ); 
			}
			break;

		case RC_CHAR_DESTROY_DISCONNECTED_DBPROXY:		
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] DBP가 연결이 되지 않았습니다.(잠시뒤 서비스 이용바람)"));
			}
			break;

		case RC_CHAR_DESTROY_DBUSER_ALREADY_CREATED:
			{
				Snprintf( szMessage ,INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] DBP의 유저가 이미 만들어져 있다. ") );
			}
			break;

		case RC_CHAR_DESTROY_QUERY_ERROR:
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] DB쿼리 실패") );
			}
			break;

		case RC_CHAR_DESTROY_DBUSER_DONOT_EXIST:
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] DBProxy의 DB유저가 존재하지 않는다." ) );
			}
			break;
		case RC_CHAR_DESTROY_DBCHAR_DONOT_EXIST:
			{
				Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , _T("[DEBUG] 삭제할 캐릭터가 없다.") );
			}
			break;
#endif
		default:
			{
				g_InterfaceManager.GetInterfaceString( eST_CHARACTER_DELETE_FAIL, szMessage, INTERFACE_STRING_LENGTH );
			}
			break;
		
		}
	}

	if( _tcslen(szMessage ) > 0 )
		g_InterfaceManager.ShowMessageBox( szMessage, 2000, NULL, 0 );
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::OnRecvCA_CHARINFO( MSG_BASE * pMsg )
{
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::ParsePacket( MSG_BASE * pMsg )
{
    switch (pMsg->m_byCategory)
    {
    case CG_CHARINFO:
        OnRecvCG_CHARINFO(pMsg);
		SceneBase::OnRecvCG_CHARINFO( pMsg );
        break;
    case CG_SYNC:
        OnRecvCG_SYNC(pMsg);
        break;
	case CG_AREACONQUEST:
        OnRecvCG_AREACONQUEST(pMsg);
        break;

    case CG_CONNECTION:
        OnRecvCG_CONNECTION(pMsg);
        break;

	case CG_EVENT:
		OnRecvCG_EVENT(pMsg);
		break;
	default:
		SceneBase::ParsePacket(pMsg);
		break;
	};
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::OnRecvCG_CONNECTION(MSG_BASE * pMsg)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};


    switch ( pMsg->m_byProtocol)
    {
    case CG_PREPARE_WORLD_CONNECT_ACK:
		OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_ACK( pMsg );
        break;

    case CG_PREPARE_WORLD_CONNECT_NAK:
		OnRecvCG_CONNECTION__CG_PREPARE_WORLD_CONNECT_NAK( pMsg );
        break;

    case CG_CONNECTION_ENTERSERVER_ACK:
        {
			if (g_pNetwork)
			{
				g_pNetwork->SetEnableSendHeartBeat(TRUE);
			}

			if(GENERALPARAM->GetSpecialMode())
			{
				GlobalFunc::EndTimeCheck(g_dwStartTime, true, _T("캐릭터 정보 받음"));
				GlobalFunc::PrintTotalLoadingTime();
			}

            MSG_CG_CONNECTION_ENTERSERVER_ACK * pRecvPacket = (MSG_CG_CONNECTION_ENTERSERVER_ACK *)pMsg;
			CLIENT_CHARACTER_PART pCharacterListForRecv[MAX_CHARACTER_LIST_NUM];
			ZeroMemory(&pCharacterListForRecv, sizeof(pCharacterListForRecv));

			pRecvPacket->ConvertForRecv(pCharacterListForRecv);		


            m_byCharacterSlot = pRecvPacket->char_slot_size;

            for( int i = 0; i < pRecvPacket->m_byCount; ++i )
            {

#ifdef _NA_004035_20120227_COSTUME_ITEM 
                // 장착 해제중인데 코드가 남아있는경우에 클라에서 직접 0을 넣어줌
                if( pCharacterListForRecv[i].is_equipped_costume == false )
                {
                    pCharacterListForRecv[i].costume_item_pos = INVALID_SLOT_POS;
                    pCharacterListForRecv[i].costume_item_code = 0;
                }
#endif//_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
				// 장착 해제중인데 코드가 남아있는경우에 클라에서 직접 0을 넣어줌
				if( pCharacterListForRecv[i].is_equipped_decorate_costume == false )
				{
					pCharacterListForRecv[i].decorate_costume_item_pos = INVALID_SLOT_POS;
					pCharacterListForRecv[i].decorate_costume_item_code = 0;
				}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME


                // 캐릭터 리스트 설정
                CharListInfo* pCharListInfo = new CharListInfo;
                pCharListInfo->CharacterInfo = pCharacterListForRecv[i];    // Copy
                pCharListInfo->dwObjectKey = CHARACTERSCENE::PLAYER_LIST + pCharacterListForRecv[i].m_bySlot;

                if(GENERALPARAM->GetSpecialMode())
                {
                    g_dwStartTime = GlobalFunc::StartTimeCheck();
                    GlobalFunc::WriteLoadingTimeLog(_T("캐릭터 세팅 시작"));
                }


                MakeMyChar(pCharListInfo->dwObjectKey, pCharacterListForRecv[i] );
                m_CharList.push_back( pCharListInfo );

                if(GENERALPARAM->GetSpecialMode())
                {
                    GlobalFunc::EndTimeCheck(g_dwStartTime, true, _T("캐릭터 세팅 완료"));
                    GlobalFunc::PrintTotalLoadingTime();
                }
            }

            m_bConnected = TRUE;
            
            g_isPkServer = pRecvPacket->m_bPVP;

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
            RecvRenameStatus(&(pRecvPacket->m_ReNameStatus[0]));
#endif _JP_20110222_INGAME_CHARNAME_CHANGE

            is_loaded_my_character_ = false;
            HideAllCharacter();
			
			//	캐릭터 정보 전송중
			const int	c_iCharInfoSending = 70256;
			g_InterfaceManager.GetInterfaceString( c_iCharInfoSending, szMessage, INTERFACE_STRING_LENGTH );
			g_InterfaceManager.ShowMessageBox( szMessage, 1, NULL, 0 );

			CursorChangeTransact::Instance()->ForceChange( MouseCursor::eMouseCursor_Default );

			GlobalFunc::GetCrashReportFileName(g_szCrashLogFileName);
			SetFileAttributes(g_szCrashLogFileName, FILE_ATTRIBUTE_NORMAL);
			TCHAR szTempBuffer[16] = {0,};
			GetPrivateProfileString( _T("SEND_REPORT"), _T("Send"), _T("-2"), szTempBuffer, 15, g_szCrashLogFileName );

			int iSendReport = _ttoi(szTempBuffer);
			if(iSendReport == 0)
			{
				// 5029 :  크래쉬 리포트를 보내시겠습니까?	
				TCHAR szTempMessage[INTERFACE_STRING_LENGTH] = {0,};
				g_InterfaceManager.GetInterfaceString(5029, szTempMessage, INTERFACE_STRING_LENGTH);
				
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_Confirm(szTempMessage, _ProcessCrashReport);
			}

#ifdef _DH_LAST_SELECTED_CHARACTER
    #if defined(_DEBUG) || defined(_GAMECLIENT_VER) 

            if( AutoTestManagerEx::Instance()->IsAutoLogin() == false )
            //if( AutoTestManager::Instance()->IsEnable() == false )
            {
                //마지막 접속 정보 요청
                uiCharacterSelectMan* select_manager = GET_DIALOG_MANAGER(uiCharacterSelectMan, UIMAN_CHARACTERSELECT);
                if (select_manager)
                {
                    select_manager->SEND_SELECT_CHARACTER_DATA_GAMEOPTION_LOAD_SYN();
                }
            }
    #else
            //마지막 접속 정보 요청
            uiCharacterSelectMan* select_manager = GET_DIALOG_MANAGER(uiCharacterSelectMan, UIMAN_CHARACTERSELECT);
            if (select_manager)
            {
                select_manager->SEND_SELECT_CHARACTER_DATA_GAMEOPTION_LOAD_SYN();
            }
    #endif//defined(_DEBUG) || defined(_GAMECLIENT_VER) 

#endif//_DH_LAST_SELECTED_CHARACTER

        }
        break;

    case CG_CONNECTION_ENTERSERVER_NAK:
        {
			//	CG_CONNECTION_ENTERSERVER_NAK!!
			m_bConnected = FALSE;

			MSG_CG_CONNECTION_ENTERSERVER_NAK * pRecvMsg = (MSG_CG_CONNECTION_ENTERSERVER_NAK *)pMsg;

			switch ((eLOGOUT_RESULT)pRecvMsg->m_dwErrorCode )
			{
			case RC_LOGOUT_WRONG_VERSION:	
				{
					//< 버전이 일치하지 않습니다.
					const int	c_iAuthLogout = 70258;
					g_InterfaceManager.GetInterfaceString( c_iAuthLogout, szMessage, INTERFACE_STRING_LENGTH );

					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
						pSystemMan->Sys_VerifyLock(szMessage, TRUE);

				}
				break;

			case RC_LOGOUT_AUTH_REQUEST:		
				{
					//< 강제 로그아웃 요청으로 로그아웃 되었습니다.
					//	인증서버로 부터 로그아웃 처리됐습니다!
					const int	c_iAuthLogout = 70258;
					g_InterfaceManager.GetInterfaceString( c_iAuthLogout, szMessage, INTERFACE_STRING_LENGTH );

					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
						pSystemMan->Sys_VerifyLock(szMessage, TRUE);

				}
				break;

			case RC_LOGOUT_WRONG_AUTHKEY:
				{
					//< 올바른 인증절차를 거치지 않아 로그아웃 됩니다. 5013
					g_InterfaceManager.GetInterfaceString(eST_LOG_OUT_BECAUSE_INVALID_AUTH , szMessage , INTERFACE_STRING_LENGTH);
					g_InterfaceManager.ShowMessageBox( szMessage , 2000, NULL, 0 );
				}
				break;

			case RC_LOGOUT_HACKING_USER:
				{
					//< 서버로 잘못된 데이터를 전송해서 로그아웃 되었습니다. 5014
					g_InterfaceManager.GetInterfaceString(eST_LOG_OUT_BECAUSE_INVALID_REQUEST , szMessage , INTERFACE_STRING_LENGTH );
					g_InterfaceManager.ShowMessageBox(szMessage , 2000, NULL, 0 );
				}
				break;

			case RC_LOGOUT_DB_ERROR:			
				{
					//< 데이터에 오류가 발생해서 로그아웃 되었습니다.
					//	캐릭터 리스트를 받지 못하였습니다.
					const int	c_iNotRecvCharInfo = 70263;
					g_InterfaceManager.GetInterfaceString( c_iNotRecvCharInfo, szMessage, INTERFACE_STRING_LENGTH );
					g_InterfaceManager.ShowMessageBox( szMessage, 2000, NULL, 0 );
				}
				break;

			case RC_LOGOUT_CHAR_BLOCK:
				{
					// 계정 제재(블럭)중인 유저입니다. : 5015
					g_InterfaceManager.GetInterfaceString(eST_BLOCK_ACCOUNT_USER , szMessage , INTERFACE_STRING_LENGTH );
					g_InterfaceManager.ShowMessageBox( szMessage, 2000, NULL, 0 );
				}
				break;

			case RC_LOGOUT_DUPLICATED_USER:
				{
					//< 같은 계정으로 접속을 시도하여 로그아웃 됩니다.
					g_InterfaceManager.GetInterfaceString(eST_LOG_OUT_BECAUSE_SAME_LOGIN, szMessage , INTERFACE_STRING_LENGTH);
					g_InterfaceManager.ShowMessageBox( szMessage , 2000, NULL, 0 );
				}
				break;
			}
        }
        break;

    case CG_CONNECTION_ENTERVILLAGEREQ_ACK:
        {
			g_InterfaceManager.HideAllWindows();
			// UI 상태 초기화
			Init_UIState();

			//채널 채팅 종료 처리
			ChatDialog * pChatDialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);//(ChatDialog * ) g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT);	
            if (pChatDialog) 
            {
                if (pChatDialog->GetExistChannel())
                {
                    pChatDialog->ExitChannelChat();
                }
                pChatDialog->set_is_first_enter(true);   // HELP 채널채팅 관련 초기화
            }

			ShopDialog * pShopDlg = (ShopDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_SHOP );
			if( pShopDlg )
			{
				pShopDlg->ClearAllHistorySlot();
			}

			//	실제 사용할, 선택한 캐릭터 데이터가 딸려 온다
            BYTE byCategory = 0;
            BYTE byProtocol = 0;
            DWORD dwObjectKey = 0;
            MSG_CG_CONNECTION_ENTERVILLAGEREQ_ACK * pRecvPacket = (MSG_CG_CONNECTION_ENTERVILLAGEREQ_ACK *)pMsg;

            xGetHeroData()->SetObjectKey( pRecvPacket->m_dwPlayerKey );

			if(IsSendSelectCharPacket())
			{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
                if( ConvenienceStore::CashShop* pInstance = ConvenienceStore::CashShop::instance() )
                {
                    pInstance->SetHasNewPresent(false);
                }
#else
				uiCashItemShopMan *pCashMan = static_cast<uiCashItemShopMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
				if( pCashMan && pCashMan->GetNewPresentRcvCount() )
				{
					pCashMan->ClearNewPresentRcvCount();
				}
#endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


			}

			CharListInfo * pSelectCharInfo = GetCurSelectCharacterInfo();
            //g_LoadScene.SetGoingToMap( pSelectCharInfo->CharacterInfo.m_iRegion );
            //g_LoadScene.SetNextSceneAfterLoaded( SCENE_TYPE_VILLAGE );
            //	선택한 놈 파이팅 포즈를 취한 다음 로딩씬으로 넘어간다. 파이팅 포즈 셋팅
            m_dwSelectCharacter = BattleScene::GetHeroInfo()->m_CharInfo.m_bySlot;

#ifdef _YMS_AUTOBOT_GAME
            AutobotConditionManager::Instance()->Init(pSelectCharInfo->CharacterInfo.m_tszCharName);
            MSG_CG_ETC_GAMEOPTION_LOAD_SYN autobot_syn;
            autobot_syn.option_type_ = GameOption::kAutoHunting;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &autobot_syn, sizeof(autobot_syn));
#endif //_YMS_AUTOBOT_GAME

            CHARLIST_ITR itr = m_CharList.begin();
            CharListInfo * pCharListInfo = NULL;

            for (; itr != m_CharList.end(); ++itr)
            {
                pCharListInfo = static_cast<CharListInfo *>(*itr);

                if(pCharListInfo->CharacterInfo.m_bySlot==m_dwSelectCharacter)
                {
                    if( ObjectStatus* pObjectStatus = GetStatus( pCharListInfo->dwObjectKey ) )
                    {
                        pObjectStatus->SetStartMotion();

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) // 디버그/개발배포 모드
                        if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
                        {
                            OnStartGame(pCharListInfo->CharacterInfo.m_iRegion);
                        }
    //#ifdef _KIKI_AUTOTEST
    //                    if( AutoTestManager::Instance()->IsSkipLobbyLoading() )
    //                    {
    //                        OnStartGame(pCharListInfo->CharacterInfo.m_iRegion);
    //                    }
    //#endif
#endif
                        break;
                    }
                    //if( Object* pObject =g_ObjectManager.GetObject(pCharListInfo->dwObjectKey) )
                    //{
                    //    switch ( pObject->GetSCObject().GetObjectType() )
                    //    {
                    //    case MAP_OBJECT:
                    //        break;

                    //    case PLAYER_OBJECT:
                    //        {
                    //            Player * pPlayer = static_cast<Player *>(pObject);
                    //            if( pPlayer->GetCharInfo()->m_bySlot == m_dwSelectCharacter )
                    //            {
                    //                if( ObjectStatus* pObjectStatus = GetStatus( pPlayer->GetObjectKey()) )
                    //                {
                    //                    pObjectStatus->SetStartMotion();
                    //                }
                    //                return;
                    //            }
                    //        }
                    //    }
                    //}
                }
            }
        }
        break;

    case CG_CONNECTION_ENTERVILLAGEREQ_NAK:
        {
			//	CG_CONNECTION_ENTERVILLAGEREQ_NAK!!

			MSG_CG_CONNECTION_ENTERVILLAGEREQ_NAK * pRecvMsg = (MSG_CG_CONNECTION_ENTERVILLAGEREQ_NAK *)pMsg;
			BIT_REMOVE(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_SELCHAR);

			TCHAR szMessage[INTERFACE_STRING_LENGTH];
			g_InterfaceManager.GetStringEnterVillageError(
				pRecvMsg->m_dwErrorCode, 
				szMessage, 
				INTERFACE_STRING_LENGTH);

			GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_TOP_SHOW, szMessage);
        }
        break;

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    case CG_CHARNAME_CHANGE_ACK:
    case CG_CHARNAME_CHANGE_NAK:
        {
            if (NeedRenameProcess() == true)
            {
                uiBase* rename_input_dialog = GET_DIALOG(IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME_INPUT);
                if (rename_input_dialog != NULL)
                {
                    rename_input_dialog->NetworkProc(pMsg);
                }
            }
        }
        break;
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE

    default:
        {
			SceneBase::OnRecvCG_CONNECTION(pMsg);
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void CharacterScene::MakeMyChar( DWORD ObjectKey, const CLIENT_CHARACTER_PART& CharacterInfo )
{
    DWORD class_code = CharacterInfo.m_byClass;
    ClassCodeEx class_code_ex;
    class_code_ex.class_type = CharacterInfo.m_byClass;
    class_code_ex.gender = CharacterInfo.gender;
    class_code = class_code_ex.class_code;

    BYTE SlotIndex = CharacterInfo.m_bySlot;

    WzVector stand_position = m_PosInfoArray[SlotIndex].wzVecPosition;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	stand_position = xGetCharacterCreate()->GetStandPosition(GetRace(class_code));
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

    ObjectStatus* pObjectStatus = new ObjectStatus( ObjectKey, stand_position );
    m_StatusArray.push_back( pObjectStatus );


#if defined(_DEBUG) || defined(_GAMECLIENT_VER) // 디버그/개발배포 모드
    if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
    {
        return;
    }
    //#ifdef _KIKI_AUTOTEST
    //    if( AutoTestManager::Instance()->IsSkipLobbyLoading() )
    //    {
    //        return;
    //    }
    //#endif
#endif

    // 플레이어를 생성
    if( CAMERA_EXTRA_INFO* camExtra = g_pMap->GetExtraCameraInfo() )
    {
        WzVector wzCameraPos;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        SetVector(&wzCameraPos, stand_position.x, xGetCharacterCreate()->GetCameraPosition().y, xGetCharacterCreate()->GetCameraPosition().z);
#else
        SetVector(&wzCameraPos, camExtra->WzPos.x, camExtra->WzPos.y, camExtra->WzPos.z);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

        if( Player* pPlayer = (Player*)g_ObjectManager.Add(ObjectKey, PLAYER_OBJECT, class_code, FALSE) )
        {
            m_PosInfoArray[SlotIndex].dwObjectKey = ObjectKey;

            pPlayer->SetDirection(wzCameraPos, stand_position, TRUE);	 
            pPlayer->SetPosition(stand_position);
            pPlayer->SetAnimation(StrToWzID("N001"), TRUE);
            pPlayer->SetChaoState(0);
            pPlayer->SetAttackBeginPlayer(FALSE);
            pPlayer->SetShowName(TRUE);


            pPlayer->SetClientChar(CharacterInfo);
            pPlayer->GetCharInfo()->m_bySlot = SlotIndex;

            if ( Pet* pPet = static_cast<Pet*>(g_ObjectManager.GetObject(pPlayer->GetPetObjectKey())) )
            {
                //safe_code:혹 서버에서 잘못된슬롯을 넘겨보냈는지 체크
                if ( SlotIndex >= (MAX_CHARACTER_LIST_NUM))
                {
                    assert(!"캐릭슬롯 Error");
                    pPet->SetModifyPosition(330.f, 0.6f);
                }
                else
                {
                    const float kDefaultOffest = 0.45f;
                    const float kMaxOffest = 1.0f;
                    float fAngle = 195.f;
                    float fOffset = kDefaultOffest;
                    float bounding_size = 0.0f;
                    WzBoundingVolume* pet_bounding_volume = pPet->GetBoundingVolume();
                    if (pet_bounding_volume != NULL)
                    {
                        bounding_size = (pet_bounding_volume->m_aabb.m_wvMin.x - pet_bounding_volume->m_aabb.m_wvMax.x)
                            * (pet_bounding_volume->m_aabb.m_wvMin.y - pet_bounding_volume->m_aabb.m_wvMax.y);
                        fOffset += bounding_size;
                    }
                    fOffset = max(kDefaultOffest, fOffset);
                    fOffset = min(1.0f, fOffset);

                    if (pPet->GetPet()->pet_script_ptr_->m_byPpossition != 0)
                    {
                        fAngle = 175.0f;
                        pPet->SetScale(0.8f);
                    }
                    fAngle *= (PI / 180.0f);
                    pPet->SetModifyPosition(fAngle, fOffset);
                }

                pPet->SetPetLock(TRUE);

            }
        }
    }

}


//------------------------------------------------------------------------------
/**
*/
DWORD CharacterScene::GetSelectCharLevel()
{
	DWORD dwLv=0;
	Player *pPlayer=(Player *)g_ObjectManager.GetObject(m_dwSelectOid);
	if(pPlayer)
		dwLv=pPlayer->GetLevel();

	return dwLv;
}

//------------------------------------------------------------------------------
/**
*/
BOOL CharacterScene::SendDeleteCharPacket(const char *pString)
{   
 
    CHARLIST_ITR itr = m_CharList.begin();
    CharListInfo * pCharListInfo = NULL;
	nString str=pString;

    for (; itr != m_CharList.end(); ++itr)
    {
        pCharListInfo = static_cast<CharListInfo *>(*itr);

        if(pCharListInfo->CharacterInfo.m_bySlot==m_dwSelectCharacter)
        {
            m_dwSelectCharacter=INVALID_OBJECT_KEY;//선택 해제
            //Todo:패킷 날리자.
            if( GENERALPARAM->IsNet() )
            {
#ifdef _GS_GSP_DELETE_CHAR_CHK
				
				TCHAR szMsg[INTERFACE_STRING_LENGTH] = {0,};
				
				//영문:삭제체크 키워(delete)
				g_InterfaceManager.GetInterfaceString(2119,szMsg);
				nString szlhs= szMsg;
			
				//둘다 문자 대소문자 맞춰줌
				szlhs.ToLower();
				str.ToLower();

				//불일치할경우 메시지 메시지 찍어주기
				//일치할경우에는 서버에서는 무조건삭제한다(협의)
				if(szlhs != str)
				{
					g_InterfaceManager.GetInterfaceString(2120,szMsg);
					g_InterfaceManager.ShowMessageBox( szMsg , 2000 , NULL, 0);	
					return TRUE;
				}
#endif//_GS_GSP_DELETE_CHAR_CHK
                if(BIT_CHECK(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_DELCHAR)==FALSE)
                {

                    BIT_ADD(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_DELCHAR);
                    m_curDelOid=pCharListInfo->CharacterInfo.m_bySlot;
                    // 캐릭터 삭제 패킷 날리기  
                    MSG_CG_CHARINFO_DESTROY_SYN SendPacket;
                    SendPacket.m_byCategory = CG_CHARINFO;
                    SendPacket.m_byProtocol = CG_CHARINFO_DESTROY_SYN;

					int numString = strlen(pString);
					numString = min(numString , 10);
					memset(SendPacket.m_LastSocialSecurityNumber , 0 , sizeof(SendPacket.m_LastSocialSecurityNumber));

					for( int i = 0; i < numString; i++ )
					{
						char ch = pString[i];
						SendPacket.m_LastSocialSecurityNumber[i] = (int) ch;
					}
                    SendPacket.m_bySelectedCharSlotIndex=pCharListInfo->CharacterInfo.m_bySlot;

                    if( FALSE == GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) ) )
                    {
						//	MSG_CHARINFO_DESTROY_SYN
                        WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
                        g_InterfaceManager.ShowMessageBox( _T("MSG_CHARINFO_DESTROY_SYN"), 10000, &Color, 0 );
                        return FALSE;
                    }
                }
            }
            else//싱글은 직접 지운다.
            {
                DeleteMyChar(pCharListInfo->dwObjectKey);

				delete pCharListInfo;
                m_CharList.erase(itr);
            }
            break;
        }
    }

	return TRUE;
}

//------------------------------------------------------------------------------
/**
     캐릭터 삭제 
*/
VOID CharacterScene::DeleteMyChar(DWORD ObjectKey)
{
	if(m_dwSelectOid == ObjectKey)
		m_dwSelectOid = INVALID_OBJECT_KEY;

	for(int i=0;i<MAX_CHARACTER_LIST_NUM;i++)
	{
		if(m_PosInfoArray[i].dwObjectKey==ObjectKey)
		{
			m_PosInfoArray[i].dwObjectKey=INVALID_OBJECT_KEY;
			break;
		}
	}
	g_ObjectManager.Delete(ObjectKey);

    for( size_t i=0;i!=m_StatusArray.size();++i)
    {
        ObjectStatus* pObjectStatus = m_StatusArray[i];
        if( pObjectStatus->object_key == ObjectKey )
        {
            m_StatusArray.erase( m_StatusArray.begin() + i );
            delete pObjectStatus;
            break;
        }
    }
}

//------------------------------------------------------------------------------
/**
    캐릭터 생성 패킷 쏘기
*/
BOOL CharacterScene::SendMakeCharPacket(DWORD classCode,const char *pName)
{

	if(m_CharList.size()>=MAX_CHARACTER_LIST_NUM)
	{
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];

		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_NUMBER_FULL, szMessage, INTERFACE_STRING_LENGTH );
		g_InterfaceManager.ShowMessageBox( szMessage, 2000, NULL, 0 );

		return FALSE;
	}

    // 캐릭터 생성 패킷 
    if(BIT_CHECK(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_CREATECHAR)==FALSE)
    {
        BIT_ADD(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_CREATECHAR);
    
        m_SendCharCreatePacket.m_byCategory = CG_CHARINFO;
        m_SendCharCreatePacket.m_byProtocol = CG_CHARINFO_CREATE_SYN;
        ClassCodeEx class_code_ex;
        class_code_ex.class_code = classCode;
        m_SendCharCreatePacket.m_byClass = static_cast<BYTE>(class_code_ex.class_type);
        m_SendCharCreatePacket.gender = static_cast<BYTE>(class_code_ex.gender);
    /*
    #ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP
        if (mystic_create_mode_ == true)
        {
            m_SendCharCreatePacket.m_byClass = static_cast<BYTE>(eCHAR_MYSTIC);
            m_SendCharCreatePacket.gender = static_cast<BYTE>(GenderType::kFemale);
        }

    #endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP
    */
        
		m_SendCharCreatePacket.m_byFace   = m_iFaceVariation;
		m_SendCharCreatePacket.m_byHair   = m_iHairVariation;
		m_SendCharCreatePacket.m_byHeight = m_iHeightVariation;
		
		memset(m_SendCharCreatePacket.m_pszCharName,0,sizeof(m_SendCharCreatePacket.m_pszCharName));
        StrnCopy(m_SendCharCreatePacket.m_pszCharName,pName,sizeof(m_SendCharCreatePacket.m_pszCharName));

        if(GlobalFunc::SendPacket(CI_GAMESERVERIDX, &m_SendCharCreatePacket, sizeof(m_SendCharCreatePacket) )==FALSE)
        {
#if defined( _DEBUG )
			//	MSG_CG_CHARINFO_CREATE_SYN
            WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
            g_InterfaceManager.ShowMessageBox( _T("MSG_CG_CHARINFO_CREATE_SYN"), 10000, &Color, 0 );
#endif
            return FALSE;
        }
    }

    return TRUE;
}

void CharacterScene::SetViewport()
{
}

//#ifdef _YJ_RENEWAL_CHARACTER_CREATE_SCENE
//#else //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
void CharacterScene::RenderCharacterCreate()
{
	g_pSunRenderer->GetMouseCursorRay( &g_wrMouseRay );
	m_pCreateMapUnitDraw->SetPosition(0.0f,0.0f,0.0f);
	g_pSunRenderer->RenderUnit(m_pCreateMapUnitDraw);

	RenderCinematicCharacter();
	g_pSunRenderer->x_pManagerPostEffect->Apply();
}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//#endif //_YJ_RENEWAL_CHARACTER_CREATE_SCENE

void CharacterScene::SetCreateCharacterMode(BOOL bMode)
{
	static St_PE_BLUR s_OldBlur;
	static WzColor s_OldAmbient;
	static St_FogParameter s_OldFog;
	m_bCreateCharacter = bMode;
	static FLOAT	fMin, fMax, fFov;

	if (m_bCreateCharacter)
	{
		GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, 11111, eZONETYPE_LOBBY);
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        g_CharSelectScene.ShowSelectCharacter(-1);
#else
		PlayCameraAnimation(GetCreateCharacterPositionAniCode() /*StrToWzID("N001")*/,FALSE);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT


		m_bySelectedClass = 0;
		m_dwInterfaceScroll = 0;		
		m_fCameraRate = 0.0f;
		m_dwPickedObjectKey = 0;

        ShowBaseCinematicCharacter();
        //GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO)->ShowDialog( TRUE );
		GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG)->ShowDialog( TRUE );
		GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_SELECT)->ShowDialog( TRUE );
		
		g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_CHARSELECT,FALSE);
        uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
        if (login_back_top != NULL)
        {
            login_back_top->ShowInterface(TRUE);
            login_back_top->SetSceneTitle(189);
        }

        fMin = g_Camera.GetNearClip();
		fMax = g_Camera.GetFarClip();
		fFov = g_Camera.GetFOV();
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
		g_Camera.SetProjection(0.1f,200.0f,WZ_PI / 6.0f);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

		s_OldBlur = g_pSunRenderer->x_pManagerPostEffect->GetTechBlur();
		St_PE_BLUR blur;
		blur.m_iBlurCount = 8;
		blur.m_fSelectHighLight = 0.55f;
		
		blur.m_fFinalColorBlend = 0.70f;		
		blur.m_fMinLuminance = 0.32f;
		blur.m_fOriginalColorBlend = 1.f;

		blur.m_wcSelectColor = WzColor_RGBA( 255, 231, 157, 255 );
		WzColor	FogColor	 = WzColor_RGBA( 100, 99, 55, 255);
		WzColor AmbientColor = WzColor_RGBA( 159, 137, 96, 255 );

		GlobalFunc::PLAY_BGM(1, 11111, (eZONETYPE)0);

		InitVariation();


#ifdef _KST_CREATECHARACTER_SPRINGCAMERA_
		if ( Object* pCamera = g_ObjectManager.GetObject(m_dwCameraObjectKey) )
		{
			g_Camera.InitSpringCameraPosition( pCamera->GetBonePos(34), pCamera->GetBonePos(35) );
			g_Camera.UpdateCamera();	// 직접 업데이트 해준다.
		}
#endif

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
        SelectClass2* select_class = NULL;
        uiCreateCharacterMan* create_character_manager_ptr = GET_DIALOG_MANAGER(uiCreateCharacterMan , UIMAN_CREATECHARACTER);
        if (create_character_manager_ptr)
        {
            select_class = create_character_manager_ptr->get_select_class_ptr();
        }

        if (select_class)
        {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            OnSelectedCharacter(eCHAR_WITCHBLADE);
            select_class->set_current_class_code(eCHAR_WITCHBLADE);
#else
            OnSelectedCharacter(eCHAR_HELLROID);
            select_class->set_current_class_code(eCHAR_HELLROID);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

            select_class->OnExcute();
        }
#else
        SelectClass* select_class = static_cast<SelectClass*>(GET_DIALOG(IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG));
        if (select_class)
        {
            OnSelectedCharacter(eCHAR_MYSTIC);
            select_class->SetCurrentClassCode(eCHAR_MYSTIC);
            select_class->OnExcute();
        }
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	}
	else
	{
		ReleaseCinematicCharacter();

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
        GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO)->ShowDialog( FALSE );
        GET_DIALOG(IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG)->ShowDialog( FALSE );
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
		GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG)->ShowDialog( FALSE );
		GET_DIALOG(IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG)->ShowWindow( FALSE );
        GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_SELECT)->ShowDialog( FALSE );
		GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_ROTATEBUTTON)->ShowDialog( FALSE );

		g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_CHARSELECT,TRUE);
        uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
        if (login_back_top != NULL)
        {
            login_back_top->ShowInterface(TRUE);
            m_dwSelectedObjectKey = 0;
        }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
        g_Camera.SetProjection(fMin, fMax, fFov);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

		if (g_pMap)
		{
			GlobalFunc::PLAY_BGM(1, g_pMap->GetCurrentFieldID(), (eZONETYPE)0);
		}

		InitVariation();

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        OnSelectedCharacter(eCHAR_WITCHBLADE);
#else
        OnSelectedCharacter(eCHAR_HELLROID);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
        SelectClass* select_class = static_cast<SelectClass*>(GET_DIALOG(IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG));
        if (select_class)
        {
            select_class->ShowDialog(FALSE);
        }
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
        GET_DIALOG(IM_CREATE_CHAR_MANAGER::EXPLAIN_RACE_DIALOG)->ShowDialog(FALSE);
        GET_DIALOG(IM_CREATE_CHAR_MANAGER::EXPLAIN_CLASS_DIALOG)->ShowDialog(FALSE);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	}
}

//#ifdef _YJ_RENEWAL_CHARACTER_CREATE_SCENE
//#else //_YJ_RENEWAL_CHARACTER_CREATE_SCENE
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
void  CharacterScene::ReleaseCreateCharacterResource()
{
	if ( m_pCreateMapUnitDraw)
	{
		Func_ContactWzd( CW_DELETE_WZUNITDRAW, m_pCreateMapUnitDraw );
		m_pCreateMapUnitDraw = NULL;

	}
	if ( m_pCreateMapUnit )
	{
		g_pSunRenderer->x_pManagerWzUnitLoader->DestroyWzUnit(CHARACTER_MAP_RESOURCE_ID);
		m_pCreateMapUnit = NULL;

	}
}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//#endif //_YJ_RENEWAL_CHARACTER_CREATE_SCENE

void  CharacterScene::SetObjectPosToCinematicMatrix(Object *pObject,int iboneindex)
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	xGetCharacterCreate()->SetObjectPosToCinematicMatrix( pObject, iboneindex );
	return;
//
//    eCHAR_TYPE class_type = eCHAR_NONE;
//    Character* character = static_cast<Character*>(pObject);
//    if (character)
//    {
//        class_type = character->GetClass();//character->GetClass();
//
//        DWORD class_code = eCHAR_NONE;
//        ClassCodeEx class_code_ex;
//        class_code_ex.class_type = class_type;
//        class_code_ex.gender = character->GetGender();
//        class_code = class_code_ex.class_code;
//        m_character.x = GetCharacterX(GetRace(class_code));
//        m_character.y = GetCharacterY(GetRace(class_code));
//        m_camera.x = GetCameraX(GetRace(class_code));
//        m_target.x = m_camera.x;
//		m_zoom = 0.0f;
//    }
//    WzVector vPos = m_character;
#else
	WzMatrix matBers = GetCinematicBoneMatrix(iboneindex);

	WzVector vPos;

	vPos.x =  matBers._41;
    vPos.y =  matBers._42; 
    vPos.z =  matBers._43;

	WzVector vAngle;

	vAngle.x =  matBers._11;
    vAngle.y =  matBers._12; 
    vAngle.z =  matBers._13;

    float fAngle = Math_GetAngleFromVector(&vAngle);

	if( pObject )
	{
		pObject->SetPosition( vPos );
//#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//        pObject->SetDirection(m_camera, vPos);
//        //pObject->SetAnimation( StrToWzID("N001"), TRUE );
//#else
		pObject->SetAngle( fAngle,TRUE );
        pObject->SetAnimation( StrToWzID("N203"), TRUE );
//#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	}

#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

SCSlotContainer * CharacterScene::GetEquipContainer( DWORD objectkey )
{
	Player * pPlayer = (Player *)g_ObjectManager.GetObject( objectkey );

	if( pPlayer == NULL )
		return NULL;

	return pPlayer->GetSlotContainer( SI_EQUIPMENT );
}

void CharacterScene::ShowCharacterSelectSceneUI()
{
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
    uiCharacterRenameDialog* rename_dialog = 
        GET_CAST_DIALOG(uiCharacterRenameDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME);

    if ((NeedRenameProcess() == true) &&
        (rename_dialog != NULL))
    {
        //서 있는 케릭터 숨기기
        CHARLIST_ITR char_list_itr = m_CharList.begin();
        CharListInfo * char_list_info = NULL;

        for (; char_list_itr != m_CharList.end(); ++char_list_itr)
        {
            char_list_info = static_cast<CharListInfo *>(*char_list_itr);
            Player* character_object = static_cast<Player*>(g_ObjectManager.GetObject(char_list_info->dwObjectKey));
            character_object->SetShow(false);
            Pet* pet_object = NULL;
            if(character_object->IsPetReturn())
            {
                pet_object = (Pet*) g_ObjectManager.GetObject(character_object->GetPetObjectKey());
                if(pet_object)
                {
                    pet_object->SetStatePetHide(_HIDE_SKILL_INFO::ALL_HIDE);
                }
            }
        }
        // 이름 변경 관련 UI 출력
        rename_dialog->ShowInterface(TRUE);
        rename_dialog->RefreshNeedRenameList();
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CHARSELECT,FALSE);
        uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
        if (login_back_top != NULL)
        {
            login_back_top->ShowInterface(FALSE);
        }
    }
    else
#endif //_JP_20110222_INGAME_CHARNAME_CHANGE
    {
        // 캐릭터 선택 관련 gui 보이기
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CHARSELECT,TRUE);
        uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
        if (login_back_top != NULL)
        {
            login_back_top->ShowInterface(TRUE);
        }
    }
}

void CharacterScene::AddCinematicCharacter(DWORD class_code, DWORD object_key, int bone_index_for_position)
{
    Player* cinematic_character = static_cast<Player*>(g_ObjectManager.Add(object_key, PLAYER_OBJECT, class_code, FALSE));

    cinematic_character->RefreshAppearRender();

//#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
//#else
    SetObjectPosToCinematicMatrix(cinematic_character, bone_index_for_position);
//#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

void CharacterScene::ProcessLoadCharacter()
{
    const size_t kNumOfCinematicCharacter = (kTempClassTypeMax - 1) * 2;

    if (m_bCreateCharacter == TRUE)
    {
        return;
    }

    Object* load_character = NULL;
    size_t num_of_loaded_character = 0;
    size_t num_of_character = 0;
    CastDialog* cast_dialog = static_cast<CastDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING));

    // 자신의 캐릭터 로드상태 체크
    if (is_loaded_my_character_ == false)
    {
        // 체크 캐릭터 로드상태
        num_of_character = m_CharList.size();
        CHARLIST_ITR char_list_itr = m_CharList.begin();
        CharListInfo * char_list_info = NULL;
        for ( ; char_list_itr != m_CharList.end(); ++char_list_itr)
        {
            char_list_info = static_cast<CharListInfo *>(*char_list_itr);
            load_character = g_ObjectManager.GetObject(char_list_info->dwObjectKey);

            if ((load_character != NULL) &&
                (load_character->IsCompletelyFullLoaded() == TRUE))
            {
                num_of_loaded_character++;
            }
            else
            {
#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
                if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
                {
                    // 로드된것으로 적용
                    ++num_of_loaded_character;
                }
    //#ifdef _KIKI_AUTOTEST
    //            if(AutoTestManager::Instance()->IsSkipLobbyLoading())
    //            {
    //                // 로드된것으로 적용
    //                ++num_of_loaded_character;
    //            }
    //#endif
#endif
            }
        }

        if (num_of_loaded_character >= num_of_character)
        {
            ShowCharacterSelectSceneUI();
            is_loaded_my_character_ = true;
        }
    }
    else
    {
        // 시네마틱캐릭터 로드상태 체크
        for (int index = 0; index < kNumOfCinematicCharacter; ++index)
        {
            load_character = g_ObjectManager.GetObject(CHARACTERSCENE::kBaseCinematicCharacter[index]);
            if (load_character == NULL)
            {
                continue;
            }

            if (load_character->IsCompletelyFullLoaded() == TRUE)
            {
                num_of_loaded_character++;
            }
            num_of_character++;
        }

            if ((num_of_character > 0) &&
                (num_of_loaded_character >= num_of_character) && 
                (IsLoadedCreateRoom() == true))
            {
                num_of_character = 0;
                g_InterfaceManager.UnLockDlgs();
                g_CharSelectScene.SetCreateCharacterMode(TRUE);
            }
    }

    // 로딩 상태에 따른 게이지 출력
    if (cast_dialog != NULL)
    {
        if (num_of_character > 0)
        {
            cast_dialog->ShowWindow(TRUE);
            cast_dialog->SetGage(100.f * num_of_loaded_character / num_of_character);
        }
        else
        {
            if (cast_dialog->IsVisible() == TRUE)
            {
                cast_dialog->ShowWindow(FALSE);

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
                if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
                {
                    AutoTestManagerEx::Instance()->RunStartGame();
                }
#endif
            }
        }
    }
}

bool CharacterScene::IsLoadedCreateRoom()
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
    if (m_pCreateMapUnitDraw == NULL)
    {
        return false;
    }

    CWzUnit* pUnit = m_pCreateMapUnitDraw->GetWzUnit();
    if ((pUnit == NULL) || 
        (pUnit->GetLoadState() != eLOAD_FULL))
    {
        return FALSE;
    }			
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    return true;
}


void  CharacterScene::MakeCinematicCharacter()
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	xGetCharacterCreate()->MakeCinematicCharacter();
#else

    // 10개의 캐릭터를 생성
    const int kBoneIndexForCinematic[kTempClassTypeMax - 1] = {2, 3, 38, 33, 4};

    DWORD object_key = CHARACTERSCENE::BERSERKER;
    eCHAR_TYPE class_Image_code = eCHAR_NONE;
    Player* cinematic_character = NULL;
    ClassCodeEx class_code_ex;
    for (BYTE class_type = eCHAR_BERSERKER; class_type < kTempClassTypeMax; ++class_type)
    {
        // 남성
        class_code_ex.class_type = class_type;
        class_code_ex.gender = GenderType::kMale;
        class_Image_code = ResourceCode::GetImageClassType(
            static_cast<eCHAR_TYPE>(class_code_ex.class_type), 
            static_cast<GenderType::Value>(class_code_ex.gender));
        AddCinematicCharacter(class_code_ex.class_code, object_key++, kBoneIndexForCinematic[class_Image_code - 1]);
        
        //여성
        class_code_ex.class_type = class_type;
        class_code_ex.gender = GenderType::kFemale;
        class_Image_code = ResourceCode::GetImageClassType(
            static_cast<eCHAR_TYPE>(class_code_ex.class_type), 
            static_cast<GenderType::Value>(class_code_ex.gender));
        AddCinematicCharacter(class_code_ex.class_code, object_key++, kBoneIndexForCinematic[class_Image_code - 1]);
    }
	
	DWORD dwMonsterCode = 2301;

#ifdef __CN_CREATE_BLOCK_SHADOW
	dwMonsterCode = 2301;
#endif//__CN_CREATE_BLOCK_SHADOW(중국쉐도우생성막기:요청(추후제거))


	m_CustomCharValue[0].fYaw	 = 6.0450134f;
	m_CustomCharValue[0].fPitch	 = 0.15635738f;
	m_CustomCharValue[0].fDis	 = 2.7520750f;
	m_CustomCharValue[0].fMinDis = 2.7520750f;
	m_CustomCharValue[0].fMaxDis = 6.0f;

	m_CustomCharValue[1].fYaw	 = 5.8756204f;
	m_CustomCharValue[1].fPitch	 = 0.017406138f;
	m_CustomCharValue[1].fDis	 = 1.9037653f;
	m_CustomCharValue[1].fMinDis = 1.9037653f;
	m_CustomCharValue[1].fMaxDis = 6.0f;

	m_CustomCharValue[2].fYaw	 = 0.019497f;
	m_CustomCharValue[2].fPitch	 = -0.158528f;
	m_CustomCharValue[2].fDis	 = 1.8483721f;
	m_CustomCharValue[2].fMinDis = 1.8483721f;
	m_CustomCharValue[2].fMaxDis = 6.0f;

	m_CustomCharValue[3].fYaw	 = 6.069417f;
	m_CustomCharValue[3].fPitch	 = -0.058915f;
	m_CustomCharValue[3].fDis	 = 1.8483721f;
	m_CustomCharValue[3].fMinDis = 1.8483721f;
	m_CustomCharValue[3].fMaxDis = 6.0f;

	m_CustomCharValue[4].fYaw	 = 5.9076447f;
	m_CustomCharValue[4].fPitch	 = -0.089062095f;
	m_CustomCharValue[4].fDis	 = 1.8467708f;
	m_CustomCharValue[4].fMinDis = 1.8467708f;
	m_CustomCharValue[4].fMaxDis = 5.24f;


#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

}

void CharacterScene::RenderCinematicCharacter()
{
	//카메라 이동중이아니거나 선택된캐릭이 아닌경우 피킹실루엣 그리기
	if ((m_bySelectedClass == FALSE) && 
		(IsCameraMoving() == FALSE))
	{
		Object* picked_obejct = g_ObjectManager.GetObject(m_dwPickedObjectKey);

        if (picked_obejct)
        {
            picked_obejct->RenderOutline(WzColor_RGBA(255,255,255,100), 0.01f);
        }

        Object* selected_obejct = g_ObjectManager.GetObject(create_selected_character_object_key_);
        if (selected_obejct && picked_obejct != selected_obejct)
        {
            selected_obejct->RenderOutline(WzColor_RGBA(235,192,38,100), 0.013f);
        }
	}	

    Object* object = NULL;

	// Player 그리기
    for (int object_key = CHARACTERSCENE::BERSERKER; object_key < CHARACTERSCENE::PLAYER_LIST; ++object_key) 
    {
        object = g_ObjectManager.GetObject(object_key);
        if (object != NULL)
        {
            object->Render();
        }
    }
}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
void CharacterScene::PlayCameraAnimation(WzID anim,BOOL bLoop)
{
	Object *pCamera = g_ObjectManager.GetObject(m_dwCameraObjectKey);

	if (pCamera)
	{
		pCamera->SetAnimation(anim,bLoop);
		pCamera->ProcessAnimation(0);
		pCamera->AnimateUnit();
		m_fCameraRate = 0.0f;
	}
}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

WzMatrix  CharacterScene::GetCinematicBoneMatrix(int index)
{
	WzMatrix mat;
	Object *pCamera = g_ObjectManager.GetObject(m_dwCameraObjectKey);

	if (pCamera)
	{
		mat = pCamera->GetBoneMatrix(index);
	}

	return mat;
}

eCHAR_TYPE CharacterScene::GetRace(DWORD code)
{
	eCHAR_TYPE class_type = eCHAR_NONE;

	ClassCodeEx class_code;
    class_code.class_code = code;
    switch (class_code.class_type)
    {
    case eCHAR_BERSERKER:
        if (class_code.gender== GenderType::kMale)
        {
            class_type = eCHAR_BERSERKER;
        }
        else
        {
            class_type = eCHAR_VALKYRIE;
        }
        break;
    case eCHAR_DRAGON:
        if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_DRAGON;
        }
        else
        {
            class_type = eCHAR_VALKYRIE;
        }
        break;
    case eCHAR_SHADOW:
        if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_SHADOW;
        }
        else
        {
            class_type = eCHAR_ELEMENTALIST;
        }
        break;
    case eCHAR_VALKYRIE:
        if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_DRAGON;
        }
        else
        {
            class_type = eCHAR_VALKYRIE;
        }
        break;
    case eCHAR_ELEMENTALIST:   
        if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_SHADOW;
        }
        else
        {
            class_type = eCHAR_ELEMENTALIST;
        }
        break;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    case eCHAR_MYSTIC:   
        /*if (class_code.gender == GenderType::kMale)
        {
            class_type = eCHAR_MYSTIC;
        }
        else*/
        {
            class_type = eCHAR_MYSTIC;
        }
        break;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    case eCHAR_HELLROID:
        {
            //_NA_000000_20130812_NEW_CHARACTER_HELLROID
            class_type = eCHAR_HELLROID;
        }
        break;
    case eCHAR_WITCHBLADE:
        {
            //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            class_type = eCHAR_WITCHBLADE;
        }
        break;
    }
    return class_type;
}

void CharacterScene::ProcessCreateCharacterMouse()
{
	float fLength = 999999.9f;
	float fT = 0.0f;
	Player *pSelected = NULL;
    WzBoundingVolume* pBV = NULL;
    Wz2DLine line_2d;
    WzVector temp_vector;
    eCHAR_TYPE class_type = eCHAR_NONE;

    for (int a = 0; a < CHARACTERSCENE::MAX_CINEMATIC_CHARACTER; ++a)
    {
        DWORD character_object_key = CHARACTERSCENE::kBaseCinematicCharacter[a];

        Player *pPlayer = (Player *)g_ObjectManager.GetObject(character_object_key);
        if (pPlayer && pPlayer->GetShow())
        {
            if (pPlayer->GetNumPickingBoundingVolume())
            {
                pBV = pPlayer->GetPickingBoundingVolume(0);

            }
            else
            {
                pBV = pPlayer->GetBoundingVolume();
            }

            if (pBV == NULL)
            {
                continue;
            }

            WORD wResult = Intersect_Ray_Bv( &g_wrMouseRay, pBV, &fT );

            if (NTCT_OUTSIDE != wResult)
            {
                // 와이드모드에서 발키리의 바운딩박스가 좌/우의 섀도우/엘리의 바운딩 박스에 가려져 피킹이 안됨
                // 바운딩 볼륨과 레이의 충돌점이 아닌 바운딩 볼륨의 중점과 레이와의 거리로 판단하도록 수정
                temp_vector = g_wrMouseRay.m_wvOrigin + g_wrMouseRay.m_wvDirection;
                As2D_GetLine(&line_2d, &g_wrMouseRay.m_wvOrigin, &temp_vector);
                WzVector temp_vector = (pBV->m_aabb.m_wvMin + pBV->m_aabb.m_wvMax) * 0.5;
                fT = As2D_Distance_Line_Vertex(&line_2d, temp_vector);

                if (fT <= fLength)
                {
                    fLength = fT;
                    pSelected = pPlayer;
                }
            }
        }
    }

	if (pSelected)
	{
		m_dwPickedObjectKey = pSelected->GetObjectKey();
	}
	else
	{
		m_dwPickedObjectKey = 0;
	}

	if (IsCameraMoving())
	{
		return;
	}
	
	if (g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_UP ))
	{
		if (!m_bySelectedClass) 
        {
			if (pSelected)
			{
                if (m_dwSelectedObjectKey != m_dwPickedObjectKey)
                {
                    m_dwSelectedObjectKey = m_dwPickedObjectKey;
                }
                else
                {
                    return; //같은 캐릭터면 선택 UI 오픈 및 애니메이션 안함
                }

                class_type = GetRace(pSelected->GetClassCode());
                OnSelectedCharacter(class_type);
				Object* pObject = g_ObjectManager.GetObject(m_dwSelectedObjectKey);
                if (pObject)
                {
				    m_fOldAngle = pObject->GetAngle();
                }

                GET_CAST_DIALOG( CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG )->LoadParts(m_dwSelectedObjectKey);
			}
		}
	}

	SelectedCharacterRotate();
}


void CharacterScene::SelectedCharacterRotate( void )
{
	if (!m_bySelectedClass)
    {
		return;
    }

	LPDIMOUSESTATE2	pMouserState = g_Input.GetMouseBuffer();

	if (g_Input.m_bUIMouseInput)
    {
		return;
    }

	CreateCharRotateButton* pDlg = GET_CAST_DIALOG(CreateCharRotateButton, IM_CREATE_CHAR_MANAGER::CREATECHAR_ROTATEBUTTON);
	if (pDlg == NULL || pDlg->GetRotateType() != CreateCharRotateButton::NONE_BUTTON)
    {
		return;
    }

	static BOOL bLbuttonDown = FALSE;
    const int kMouseButton = MOUSE_RBUTTON;
    

	if (g_Input.GetState(kMouseButton, KS_MOUSE_DOWN))
	{
		bLbuttonDown = TRUE;
	}

	if (g_Input.GetState(kMouseButton, KS_MOUSE_UP))
	{
		bLbuttonDown = FALSE;
	}

	if (bLbuttonDown == TRUE)
	{
        if (g_Input.GetState( kMouseButton, KS_MOUSE_KEY))
        {
            float rotate_angle = static_cast<float>(pMouserState->lX) * 0.1f;
            if (rotate_angle < -0.1f)
            {
                rotate_angle = -0.1f;
            }

            if (rotate_angle > 0.1f)
            {
                rotate_angle = 0.1f;
            }
            RotateCinematicCharacter(rotate_angle);
        }        
	}
}

void CharacterScene::OnSelectedCharacter(eCHAR_TYPE class_code)
{
    // 선택된 외형에 따른 캐릭터 오브젝트키를 컨테이너에 담는다.
    image_class_object_key_list_.clear();

    switch(class_code)
    {
    case eCHAR_BERSERKER:
        {
            if (IsMakeCharacterBetakey(eCHAR_BERSERKER, eCHAR_BERSERKER))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::BERSERKER);
            }            
        }
        break;
    case eCHAR_DRAGON:
        {
            if (IsMakeCharacterBetakey(eCHAR_DRAGON, eCHAR_DRAGON))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::DRAGON);
            }

            if (IsMakeCharacterBetakey(eCHAR_VALKYRIE, eCHAR_DRAGON))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::VALKYRIE);
            }
        }
        break;
    case eCHAR_SHADOW:
        {		
            if (IsMakeCharacterBetakey(eCHAR_SHADOW, eCHAR_SHADOW))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::SHADOW);
            }

            if (IsMakeCharacterBetakey(eCHAR_ELEMENTALIST, eCHAR_SHADOW))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::ELEMENTALIST);
            }
        }
        break;
    case eCHAR_VALKYRIE:
        {
            if (IsMakeCharacterBetakey(eCHAR_DRAGON, eCHAR_VALKYRIE))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::DRAGON_W);
            }

            if (IsMakeCharacterBetakey(eCHAR_VALKYRIE, eCHAR_VALKYRIE))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::VALKYRIE_W);
            }

            if (IsMakeCharacterBetakey(eCHAR_BERSERKER, eCHAR_VALKYRIE))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::BERSERKER_W);
            }
        }
        break;
    case eCHAR_MAGICIAN:
        {
            if (IsMakeCharacterBetakey(eCHAR_SHADOW, eCHAR_MAGICIAN))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::SHADOW_W);
            }

            if (IsMakeCharacterBetakey(eCHAR_ELEMENTALIST, eCHAR_MAGICIAN))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::ELEMENTALIST_W);
            }
        }
        break;   
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    case eCHAR_MYSTIC:
        {
            if (IsMakeCharacterBetakey(eCHAR_MYSTIC, eCHAR_MYSTIC))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::MYSTIC_W);
            }
/*
            if (IsMakeCharacterBetakey(eCHAR_MYSTIC, eCHAR_MYSTIC))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::MYSTIC);
            }
            */
        }
        break; 
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eCHAR_HELLROID:
        {
            if (IsMakeCharacterBetakey(eCHAR_HELLROID, eCHAR_HELLROID))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::HELLROID);
            }
/*
            if (IsMakeCharacterBetakey(eCHAR_MYSTIC, eCHAR_MYSTIC))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::MYSTIC);
            }
            */
        }
        break; 
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAR_WITCHBLADE:
        {
            if (IsMakeCharacterBetakey(eCHAR_WITCHBLADE, eCHAR_WITCHBLADE))
            {
                image_class_object_key_list_.push_back(CHARACTERSCENE::WITCHBLADE_W);
            }
        }
        break; 
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }

#if defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
    SelectClass2* ui_select_class = NULL;
    uiCreateCharacterMan* create_character_manager_ptr = GET_DIALOG_MANAGER(uiCreateCharacterMan , UIMAN_CREATECHARACTER);
    if (create_character_manager_ptr) {
        ui_select_class = create_character_manager_ptr->get_select_class_ptr();
    }

    if (ui_select_class) {
        ui_select_class->set_current_class_type(class_code);
    }    
#else
    SelectClass* ui_select_class = static_cast<SelectClass*>(GET_DIALOG(IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG));
    if (ui_select_class)    
    {
        ui_select_class->ShowDialog(FALSE);
    #ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        ui_select_class->SetCurrentClassType(class_code);
    #else
        ui_select_class->SetCurrentClassCode(class_code);
    #endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        ui_select_class->ShowInterface(TRUE);
    } 
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
}

void  CharacterScene::OnClickedChar(BYTE byClassCode, BYTE byModelCode)
{
    bool is_wide = (g_pApplication->IsWideModeFlag() == TRUE);
    WzID ani_code = 0;

    create_selected_character_object_key_ = 0;
    m_bySelectedModel = byModelCode;
    switch(byModelCode)
    {
        case eCHAR_BERSERKER:   ani_code = is_wide ? StrToWzID("N002") : StrToWzID("N102"); break;
        case eCHAR_DRAGON:      ani_code = is_wide ? StrToWzID("N004") : StrToWzID("N104"); break;
        case eCHAR_VALKYRIE:    ani_code = is_wide ? StrToWzID("N008") : StrToWzID("N108"); break;
        case eCHAR_MAGICIAN:    ani_code = is_wide ? StrToWzID("N006") : StrToWzID("N106"); break;
        case eCHAR_SHADOW:      ani_code = is_wide ? StrToWzID("N010") : StrToWzID("N110"); break;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        case eCHAR_MYSTIC:      ani_code = is_wide ? StrToWzID("N010") : StrToWzID("N110"); break;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eCHAR_HELLROID:    ani_code = is_wide ? StrToWzID("N010") : StrToWzID("N110"); break;
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_WITCHBLADE:  ani_code = is_wide ? StrToWzID("N010") : StrToWzID("N110"); break;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
    PlayCameraAnimation(ani_code, FALSE);	
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

	m_bySelectedClass = byClassCode;

	CreateCharBottomDialog* pBottomDlg = GET_CAST_DIALOG(CreateCharBottomDialog, IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG);
	CreateCharCustomDialog* pCustomDlg = GET_CAST_DIALOG(CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG);
	
    pCustomDlg->InitText();

	if (GET_DIALOG( IM_CREATE_CHAR_MANAGER::CREATECHAR_SELECT )->IsVisible())
	{		
		GET_DIALOG( IM_CREATE_CHAR_MANAGER::CREATECHAR_SELECT )->ShowDialog( FALSE );
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
        GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO)->ShowDialog( FALSE );
        GET_DIALOG(IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG)->ShowDialog( FALSE );
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
	}

	InitVariation();

	SetCinematicCharShow( FALSE );
}

BOOL  CharacterScene::IsCameraMoving()
{
   if (m_fCameraRate >= 100.0f)
   {
	   return FALSE;
   }
   else
   {
	   return TRUE;
   }
}

void CharacterScene::SetCharOldAngle( void )
{
	Object * obj = g_ObjectManager.GetObject( m_dwSelectedObjectKey );

	if( obj )
	{
		obj->SetAngle( m_fOldAngle, TRUE );
	}
}

void  CharacterScene::OnCancelCreateCharacter()
{
	if (m_bySelectedClass)
	{
		SetCinematicCharShow( TRUE );
		m_dwSelectedObjectKey = 0;

		// 기본형으로 돌려준다.
        SetBackCinematicCharacter();

        bool is_wide = (g_pApplication->IsWideModeFlag() == TRUE);
        WzID ani_code = 0;
        switch(m_bySelectedModel)
        {
            case eCHAR_BERSERKER:   ani_code = is_wide ? StrToWzID("N003") : StrToWzID("N103"); break;
            case eCHAR_DRAGON:      ani_code = is_wide ? StrToWzID("N005") : StrToWzID("N105"); break;
            case eCHAR_VALKYRIE:    ani_code = is_wide ? StrToWzID("N009") : StrToWzID("N109"); break;
            case eCHAR_MAGICIAN:    ani_code = is_wide ? StrToWzID("N007") : StrToWzID("N107"); break;
            case eCHAR_SHADOW:      ani_code = is_wide ? StrToWzID("N011") : StrToWzID("N111"); break;
        }
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
        PlayCameraAnimation(ani_code, FALSE);	
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        m_bySelectedClass = 0;
        m_dwInterfaceScroll = 0;

		InitVariation();
	}

}

void CharacterScene::ProcessScrollInterface(DWORD dwTick)
{
	CreateCharBottomDialog* pBottomDlg = GET_CAST_DIALOG(CreateCharBottomDialog, IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG);

	if (m_bySelectedClass)
	{
		if (!IsCameraMoving())
		{
			m_dwInterfaceScroll += dwTick;
			m_dwInterfaceScroll = min(INTERFACE_SCROLL_TIME,m_dwInterfaceScroll);				
		}

	}
	else
	{
		if (m_dwInterfaceScroll <= dwTick)
		{
			m_dwInterfaceScroll = 0;
		}
		else
		{
			m_dwInterfaceScroll -= dwTick;
		}

	}

	float fRate = (float)m_dwInterfaceScroll / (float)INTERFACE_SCROLL_TIME;

	fRate = 1.0f;

	float fScreenXSize;
	float fScreenYSize;

	POINT pt = g_pApplication->Get2DModeMapping();

	fScreenXSize = (float)pt.x;
	fScreenYSize = (float)pt.y;	

	float fLeft = LerfFloat(fScreenXSize,fScreenXSize - LEFT_INTERFACE_SIZE,fRate);
	float fBottom = LerfFloat(fScreenYSize,fScreenYSize - BOTTOM_INTERFACE_SIZE,fRate);

	if (m_dwInterfaceScroll == INTERFACE_SCROLL_TIME)
	{
		if (!(GET_DIALOG( IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG)->IsVisible()))
		{
			GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG)->ShowDialog(TRUE);
            GET_DIALOG( IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG)->ShowWindow(TRUE);
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
            GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_CLASS_INFO)->ShowDialog( FALSE );
            GET_DIALOG(IM_CREATE_CHAR_MANAGER::SELECTCHAR_CLASS_DIALOG)->ShowDialog( FALSE );
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES
            g_Camera.SetCameraDistance(1.0f);

			GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_ROTATEBUTTON)->ShowDialog(TRUE);
			GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_SELECT)->ShowDialog(FALSE);
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
            GET_DIALOG(IM_CREATE_CHAR_MANAGER::EXPLAIN_RACE_DIALOG)->ShowDialog(TRUE);
            GET_DIALOG(IM_CREATE_CHAR_MANAGER::EXPLAIN_CLASS_DIALOG)->ShowDialog(TRUE);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

            CreateCharCustomDialog* createcharcustomdialog_ptr = GET_CAST_DIALOG(CreateCharCustomDialog, IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG);
            if (createcharcustomdialog_ptr != NULL)
            {
                createcharcustomdialog_ptr->first_setting_reset();
            }
		}

	}
	else
	{
		if (GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG)->IsVisible())
		{		
			GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_BOTTOM_DIALOG)->ShowDialog(FALSE);
			GET_DIALOG(IM_CREATE_CHAR_MANAGER::CUSTOM_DIALOG)->ShowWindow(FALSE);
			GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_ROTATEBUTTON)->ShowDialog(FALSE);
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
            GET_DIALOG(IM_CREATE_CHAR_MANAGER::EXPLAIN_RACE_DIALOG)->ShowDialog(FALSE);
            GET_DIALOG(IM_CREATE_CHAR_MANAGER::EXPLAIN_CLASS_DIALOG)->ShowDialog(FALSE);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
		}

		if (!IsCameraMoving())
		{
			if (!(GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_SELECT)->IsVisible()))
			{		
				GET_DIALOG(IM_CREATE_CHAR_MANAGER::CREATECHAR_SELECT)->ShowDialog(TRUE);
			}
		}
	}
}

void CharacterScene::ReleaseCinematicCharacter()
{
	// Player 제거
    for (int object_key = CHARACTERSCENE::BERSERKER; object_key < CHARACTERSCENE::PLAYER_LIST; ++object_key) 
	{
		g_ObjectManager.Delete(object_key);
	}
}

void  CharacterScene::OnBackCreateCharacter()
{
}

DWORD CharacterScene::GetSelectedClass()
{
	return m_bySelectedClass;
}

BOOL  CharacterScene::PostInterfaceRender( DWORD dwTick )
{
	if (m_bCreateCharacter)
	{
		g_pSunRenderer->Set2DMode(TRUE);
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont("ti35");

		int iClassName = 0;

		if(m_bySelectedClass==eCHAR_BERSERKER)
		{
			iClassName = eST_CHARACTER_CLASS_NAME_BERSERKER;
		}
		else if(m_bySelectedClass==eCHAR_DRAGON)
		{
			iClassName = eST_CHARACTER_CLASS_NAME_DRAGON;
		}
		else if(m_bySelectedClass==eCHAR_VALKYRIE)
		{
			iClassName = eST_CHARACTER_CLASS_NAME_VALKYRIE;
		}
		else if(m_bySelectedClass==eCHAR_MAGICIAN)
		{
			iClassName = eST_CHARACTER_CLASS_NAME_MAGICIAN;
		}
		else if(m_bySelectedClass==eCHAR_SHADOW)
		{
			iClassName = eST_CHARACTER_CLASS_NAME_SHADOW;
		}
        else if(m_bySelectedClass==eCHAR_MYSTIC) // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {
            iClassName = eST_CHARACTER_CLASS_NAME_MYSTIC;
        }
        else if(m_bySelectedClass==eCHAR_HELLROID) //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        {
            iClassName = eST_CHARACTER_CLASS_NAME_HELLROID;
        }
		else
		{			
			iClassName = eST_CHARACTER_CREATE;
		}

		TCHAR szCharName[INTERFACE_STRING_LENGTH];

		//	캐릭터 클래스 이름 얻어오기
		g_InterfaceManager.GetInterfaceString( iClassName, szCharName, INTERFACE_STRING_LENGTH );

		RECT rc;

		rc.left = 34;
		rc.top = 28;
		rc.right = 375;
		rc.bottom = 150;

        BOOL is_show = FALSE;
        if (m_dwInterfaceScroll == INTERFACE_SCROLL_TIME && m_bySelectedClass )
        {
            is_show = TRUE;
        }
        else if (!m_bySelectedClass && !IsCameraMoving())
        {
            is_show = TRUE;
        }
        uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
        if (login_back_top != NULL && (login_back_top->IsVisibleWindow() != is_show))
        {
            login_back_top->ShowInterface(is_show);
            if (m_bySelectedClass == 0)
            {
                login_back_top->SetSceneTitle(iClassName);
            }
            else
            {
                login_back_top->SetSceneTitleClassName(m_bySelectedClass, iClassName);
            }
            
        }

		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
		g_pSunRenderer->Set2DMode(FALSE);
	}

	return TRUE;
}

void CharacterScene::InitVariation()
{
	m_iFaceVariation = 1;
	m_iHairVariation = 1;
	m_iHeightVariation = 2;
}

Player *CharacterScene::GetCinematicCharacter(BYTE byClass)
{
	Player *pRet = NULL;

	switch(byClass)
	{
	case eCHAR_BERSERKER:
		pRet = (Player *)g_ObjectManager.GetObject(CHARACTERSCENE::BERSERKER);
		break;
	case eCHAR_VALKYRIE:
		pRet = (Player *)g_ObjectManager.GetObject(CHARACTERSCENE::VALKYRIE);
		break;
	case eCHAR_DRAGON:
		pRet = (Player *)g_ObjectManager.GetObject(CHARACTERSCENE::DRAGON);
		break;
	case eCHAR_ELEMENTALIST:
		pRet = (Player *)g_ObjectManager.GetObject(CHARACTERSCENE::ELEMENTALIST);
		break;
	case eCHAR_SHADOW:
		pRet = (Player *)g_ObjectManager.GetObject(CHARACTERSCENE::SHADOW);
		break;
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    case eCHAR_MYSTIC:
        pRet = (Player *)g_ObjectManager.GetObject(CHARACTERSCENE::MYSTIC);
        break;
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	}

	return pRet;
}

void  CharacterScene::OnClickChangeHeight(BOOL bFoward)
{
	if (!m_bySelectedClass)
	{
		return;
	}

	if (bFoward) 
	{
		m_iHeightVariation++;
	}
	else
	{
		m_iHeightVariation--;
	}

	if (m_iHeightVariation >= MAX_HEIGHT_VARIATION)
	{
		m_iHeightVariation = 0;
	}
	else if ( m_iHeightVariation < 0)
	{
		m_iHeightVariation = MAX_HEIGHT_VARIATION - 1;
	}

    Player *pPlayer = static_cast<Player*>(g_ObjectManager.GetObject(GetSelectedObjectKey()));
	

	if (pPlayer)
	{
		pPlayer->SetHeightVariation(m_iHeightVariation);
		pPlayer->RefreshAppearRender();
	}

}

void  CharacterScene::OnClickChangeHair(BOOL bFoward)
{
	if (!m_bySelectedClass)
	{
		return;
	}

	if (bFoward) 
	{
		m_iHairVariation++;
	}
	else
	{
		m_iHairVariation--;
	}

    Player *pPlayer = static_cast<Player*>(g_ObjectManager.GetObject(GetSelectedObjectKey()));
    if (pPlayer == NULL)
    {
        return;
    }

    BYTE selected_class = pPlayer->GetImageClass();

	int maxcount = HeroVariationInfoParser::Instance()->GetVariationCount(selected_class, PLAYER_VARIATION_HAIR);

	// index가 1부터 시작하기때문에 maxcount 자체가 최대 index임
	if (m_iHairVariation > maxcount)
	{
		m_iHairVariation = 1;
	}
	else if ( m_iHairVariation < 1)
	{
		m_iHairVariation = maxcount;
	}

	pPlayer->SetPartVariation(PLAYER_VARIATION_HAIR,m_iHairVariation);
    pPlayer->RefreshAppearRender();
}

void  CharacterScene::OnClickChangeFace(BOOL bFoward)
{
	if (!m_bySelectedClass)
	{
		return;
	}

	if (bFoward) 
	{
		m_iFaceVariation++;
	}
	else
	{
		m_iFaceVariation--;
	}

    Player *pPlayer = static_cast<Player*>(g_ObjectManager.GetObject(GetSelectedObjectKey()));
    if (pPlayer == NULL)
    {
        return;
    }

    BYTE selected_class = pPlayer->GetImageClass();

	int maxcount = HeroVariationInfoParser::Instance()->GetVariationCount(selected_class, PLAYER_VARIATION_FACE);
	// index가 1부터 시작하기때문에 maxcount 자체가 최대 index임
	if (m_iFaceVariation > maxcount)
	{
		m_iFaceVariation = 1;
	}
	else if ( m_iFaceVariation < 1)
	{
		m_iFaceVariation = maxcount;
	}

    pPlayer->SetPartVariation(PLAYER_VARIATION_FACE,m_iFaceVariation);
    pPlayer->RefreshAppearRender();
}

int   CharacterScene::GetCurrentHeightVariation()
{
	return m_iHeightVariation;
}
int   CharacterScene::GetCurrentHairVariation()
{
	return m_iHairVariation;
}
int   CharacterScene::GetCurrentFaceVariation()
{
	return m_iFaceVariation;
}

void CharacterScene::OnRecvCG_AREACONQUEST(MSG_BASE *pMsg)
{
}


void CharacterScene::OnRecvCG_EVENT( MSG_BASE * pMsg)
{
	switch( pMsg->m_byProtocol)
	{
	case CG_EVENT_TIME_CMD:
		{
			MSG_CG_EVENT_TIME_CMD * pRecvPacket = (MSG_CG_EVENT_TIME_CMD * ) pMsg;

			PcbangPlayTimeDialog * pDlg = ( PcbangPlayTimeDialog * ) 
				g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_PCBANG_EVENT_TIME);
			if( pDlg)
			{
				pDlg->SetTime(pRecvPacket->m_EventUseTime);
			}
		}
		break;

#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	case CG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD:
		{
			MSG_CG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD* recv_packet = static_cast<MSG_CG_GMEVENT_PEAKTIME_PROGRESS_EVENTINFO_CMD*>(pMsg);

			TiredSystemDialog* tired_system_dialog = static_cast<TiredSystemDialog*>(GET_DIALOG( IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM ));
			if ( recv_packet->exp_ratio > 0.0001f )
			{
				if ( tired_system_dialog )
				{
					tired_system_dialog->set_gm_event_exp_ratio_( recv_packet->exp_ratio );
				}
			}
			else
			{
				if ( tired_system_dialog )
				{
					tired_system_dialog->set_gm_event_exp_ratio_( 0.0f );
				}
			}
		}
		break;
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	}
}


void CharacterScene::AddZeroWhenUnder10(TCHAR * pszNumber , int inputNum)
{
	if( inputNum < 10 )
	{
		Sprintf(pszNumber , _T("0%d"), inputNum);
	}
	else
	{
		Sprintf(pszNumber, _T("%d") , inputNum);
	}
}

void CharacterScene::ForceDelChar()
{
	if( ! m_bConnected )
		return;
	if( ! m_bEnableRender)
		return;
	CHARLIST_ITR itr = m_CharList.begin();
	CharListInfo * pCharListInfo = NULL;

	for (; itr != m_CharList.end(); ++itr)
	{
		pCharListInfo = static_cast<CharListInfo *>(*itr);

		if( pCharListInfo->CharacterInfo.m_DelChk > 0 )
		{
		
			Player * pPlayer =	( Player *) g_ObjectManager.GetObject(pCharListInfo->dwObjectKey);
			if( pPlayer) 
			{
				if( pPlayer->GetCharDelRemainTime() <= 0)
				{
					uiHelpMan* helpMan =
						static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

					if(helpMan)
					{
						helpMan->ResetReadInfo(pCharListInfo->CharacterInfo.m_tszCharName);
					}
					DeleteMyChar(pCharListInfo->dwObjectKey);

					delete pCharListInfo;
					m_CharList.erase(itr);
					break;
				}
			}
		}
	}
}

void CharacterScene::SEND_CHARINFO_RECOVER_SYN()
{
	if (BIT_CHECK(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_RECOVERY)==FALSE)
	{
		BIT_ADD(m_packetStatus,PACKET_STATUS_CHARACTERSCENE_RECOVERY);
		MSG_CG_CHARINFO_RECOVER_SYN	SendPacket;
		SendPacket.m_bySelectedCharSlotIndex = (BYTE)this->GetSelectCharOId();
		GlobalFunc::SendPacket(CI_GAMESERVERIDX , &SendPacket , sizeof( SendPacket) );
	}
}

void CharacterScene::ShowRecoveryErrorCode(DWORD errorCode)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH ] = {0,};
	switch ( errorCode )
	{
	case RC_CHAR_RECOVER_NOTDESTROYED:
		{
			//5035	삭제되지 않은 캐릭터입니다.
			g_InterfaceManager.GetInterfaceString(5035 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_CHAR_RECOVER_DBCHAR_DONOT_EXIST:
		{
			// 5036	복구할 캐릭터가 존재하지 않습니다.
			g_InterfaceManager.GetInterfaceString(5036 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

#ifdef _DEBUG
	case RC_CHAR_RECOVER_ALREADY_DOING_TRANSACTION:
		{
			Snprintf(szMessage ,INTERFACE_STRING_LENGTH -1, _T("[DEBUG] 이미 다른 트랜잭션을 진행중이다.") );
		}
		break;

	case RC_CHAR_RECOVER_INVALID_PARAM:
		{
			Snprintf(szMessage ,INTERFACE_STRING_LENGTH -1, _T("[DEBUG] 패킷이 잘못된 값입니다.") );
		}
		break;

	case RC_CHAR_RECOVER_DISCONNECTED_DBPROXY:
		{
			Snprintf(szMessage ,INTERFACE_STRING_LENGTH -1, _T("[DEBUG] DBP가 연결이 되지 않았습니다.(잠시뒤 서비스 이용바람)") );
		}
		break;

	case RC_CHAR_RECOVER_TRANSACTION_ERROR:
		{
			Snprintf(szMessage ,INTERFACE_STRING_LENGTH -1, _T("[DEBUG] 트랜젝션 오류(쿼리오류,기타오류) ") );
		}
		break;

	case RC_CHAR_RECOVER_DBUSER_DONOT_EXIST:
		{
			Snprintf(szMessage ,INTERFACE_STRING_LENGTH -1, _T("[DEBUG]  DBProxy의 DB유저가 존재하지 않는다.") );
		}
		break;
#endif
	default:
		{
			// 5034	캐릭터 복구에 실패했습니다.
			g_InterfaceManager.GetInterfaceString(5034 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;
	}

	if( _tcslen(szMessage ) > 0 )
		g_InterfaceManager.ShowMessageBox( szMessage, 2000, NULL, 0 );
}

BOOL CharacterScene::SendCharacterIdCheckSyn( LPCTSTR szName )
{
	MSG_CG_CHARINFO_CHARACTERID_CHECK_SYN	syn;

	_tcsncpy(syn.m_szCharName, szName, MAX_CHARNAME_LENGTH);
    syn.m_szCharName[_countof(syn.m_szCharName) - 1] = '\0';
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &syn, sizeof(MSG_CG_CHARINFO_CHARACTERID_CHECK_SYN));

	return TRUE;
}

void CharacterScene::LoadCreateCharacterRoom()
{
	// 맵로딩
	if( m_bLoadCreateCharacterRoom == false )
	{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
		g_pSunRenderer->x_pManagerWzUnitLoader->LoadWzUnit( CHARACTER_MAP_RESOURCE_ID, "Data\\Interface\\characterselect\\Room\\make_room.wzu" );

		m_pCreateMapUnit = g_pSunRenderer->x_pManagerWzUnitLoader->GetWzUnit(CHARACTER_MAP_RESOURCE_ID);

        m_pCreateMapUnitDraw = (CWzUnitDraw *)Func_ContactWzd( CW_NEW_WZUNITDRAW, 0 );

		m_pCreateMapUnitDraw->SetWzUnit( m_pCreateMapUnit,FALSE );

		PlayCameraAnimation(GetCreateCharacterPositionAniCode()/*StrToWzID("N001")*/,FALSE);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        m_fCameraRate = 0.0f;

		m_dwPickedObjectKey = 0;
		m_bLoadCreateCharacterRoom = true;
	}
}

void CharacterScene::LoadCinematicCamera()
{
    m_dwCameraObjectKey = ResourceCode::GetCinematicCameraInCharacterScene();
    Object* pCamera = g_ObjectManager.Add(m_dwCameraObjectKey, CAMERA_OBJECT, 0, 0);
    if (pCamera != NULL)
    {
        WzVector vPos;
        ZeroMemory(&vPos, sizeof(vPos));
        pCamera->SetPosition(vPos);
    }
}

void CharacterScene::Init_UIState()
{
    uiSkillMan* skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);
    if (skill_man)
    {
        skill_man->SetCurrentDialogKey(IM_SKILL_MANAGER::SKILL_TREE_DLG);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        // PerkList를 요청
        skill_man->SEND_Perk_List_SYN();
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    }

	uiQuestListDlg* pQuestListDlg = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
	if (pQuestListDlg)
	{
		pQuestListDlg->SetDlgState(QUEST_CATEGORY_MISSION);
	}

    uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
    if (guide_manager != NULL)
    {
        //캐릭터 별 사용하는 변수 초기화
        guide_manager->set_is_have_notice_once(false);
        //도움말 Repeat 데이터를 요청
        guide_manager->SEND_GUIDE_DATA_GAMEOPTION_LOAD_SYN();
        //열람실 데이터를 요청
        guide_manager->SEND_LIBRARY_DATA_GAMEOPTION_LOAD_SYN();
    }
    HotKeyManager::Instance()->LoadHotKeyInfoFromServer();
}

//------------------------------------------------------------------------------ 
void CharacterScene::SetCustomViewCamera()
{
    Object* obj = g_ObjectManager.GetObject(m_dwSelectedObjectKey);
    BASE_CreateCharCustomCameraInfo* info = CreateCharCustomCameraInfoParser::Instance()->GetCustomCameraInfo(static_cast<eCHAR_TYPE>(m_bySelectedClass));

    if ((obj != NULL) && (info != NULL))
    {
        obj->SetAngle(info->char_rot, TRUE);
    }
}

//------------------------------------------------------------------------------ 
#if defined(_DEBUG) || defined(_GAMECLIENT_VER)
BOOL CharacterScene::SetSelectCharacterAutoTestByName(LPCTSTR CharacterName)
{
    CHARLIST_ITR it = m_CharList.begin();
    DWORD index = 0;

    while( it != m_CharList.end() )
    {
        CharListInfo* info = *it;
        if( _tcscmp( info->CharacterInfo.m_tszCharName, CharacterName ) == 0 )
        {
            SetSelectCharacterAutoTest( info->CharacterInfo.m_bySlot );
            return TRUE;
        }
        ++it;
    }

    return FALSE;
}

void CharacterScene::SetSelectCharacterAutoTest(DWORD dwSelectCharacter)
{
#if defined(_DEBUG) || defined(_GAMECLIENT_VER) // 디버그/개발배포 모드

    if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
    {
        if( 
#ifdef _NA_000000_20130114_RENEWER_UI 
			uiCharacterSelectList2* select_list = GET_CAST_DIALOG(uiCharacterSelectList2, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST)
#else
			uiCharacterSelectList* select_list = GET_CAST_DIALOG(uiCharacterSelectList, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST)
#endif
		  )
        {
            select_list->SetSelectedCharacterSlotIndex( dwSelectCharacter );
        }

		m_dwSelectCharacter = dwSelectCharacter;
	}
#endif
}

#endif

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
// 이름 변경 프로세스 중에는 하단UI 는 숨기는 것으로 한다.
void CharacterScene::InitRenameProcess()
{
    for (BYTE index = 0; index < MAX_CHARACTER_LIST_NUM; ++index)
    {
        rename_status_[index] = false;
    }
}

bool CharacterScene::NeedRenameProcess()
{
    for (BYTE index = 0; index < m_CharList.size()/*m_byCharacterSlot*/; ++index)
    {
        if (rename_status_[index] == true)
        {
            return true;
        }
    }
    return false;
}

int CharacterScene::GetNumOfNeedRename()
{
    BYTE index = 0;
    int number = 0;
    for ( ; index < m_CharList.size()/*m_byCharacterSlot*/; ++index)
    {
        if (rename_status_[index] == true)
        {
            ++number;
        }
    }
    return number;
}

bool CharacterScene::NeedRenameByIndex(BYTE index)
{
    if (index < 0 || index >= m_CharList.size()/*m_byCharacterSlot*/)
    {
        return false;
    }

    return rename_status_[index];
}

void CharacterScene::UnsetRenameStatusByIndex(BYTE index)
{
    if (index < 0 || index >= m_CharList.size()/*m_byCharacterSlot*/)
    {
        return;
    }
    rename_status_[index] = false;
}

void CharacterScene::RecvRenameStatus(BYTE* rename_status)
{
    if (rename_status == NULL)
    {
        return;
    }

    for (BYTE index = 0; index < m_CharList.size()/*m_byCharacterSlot*/; ++index)
    {
        rename_status_[index] = ((rename_status[index] == 1) ? true : false);
    }
}

#endif //_JP_20110222_INGAME_CHARNAME_CHANGE

void CharacterScene::ReleaseAllParts(DWORD object_key)
{
    // 플레이어 및 이큅컨테이너 얻기
    Player* cinematic_character = static_cast<Player*>(g_ObjectManager.GetObject(object_key));
    if (cinematic_character == NULL)
    {
        return;
    }

    SCSlotContainer* equip_container = cinematic_character->GetEquipmentContainer();
    if (equip_container == NULL)
    {
        return;
    }
    
    // 순회하면 DeleteSlot 호출
    const POSTYPE kEquipPosition[MAX_ARMORCODE] = { 
        EquipmentContainer::EQUIP_ARMOR,
        EquipmentContainer::EQUIP_PROTECTOR,
        EquipmentContainer::EQUIP_HELMET,
        EquipmentContainer::EQUIP_PANTS,
        EquipmentContainer::EQUIP_BOOTS,
        EquipmentContainer::EQUIP_GLOVE,
        EquipmentContainer::EQUIP_BELT,
        EquipmentContainer::EQUIP_SHIRTS
    };

    SCItemSlot delete_item_solt;
    // 무기해제
    equip_container->DeleteSlot(EquipmentContainer::EQUIP_WEAPON1, &delete_item_solt);
    // 방어구파츠해제
    for (int index = 0; index < MAX_ARMORCODE; ++index)
    {
        equip_container->DeleteSlot(kEquipPosition[index], &delete_item_solt);
    }

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    //cinematic_character->SetAnimation(StrToWzID("N001"), TRUE);
#else
    cinematic_character->SetAnimation(StrToWzID("N203"), TRUE);
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

void CharacterScene::ResetCameraPosition(eCHAR_TYPE class_type )
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
	xGetCharacterCreate()->ResetCameraPosition( class_type );
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

bool CharacterScene::CheckEnableSelectClass(eCHAR_TYPE check_class_type)
{
    std::vector<DWORD>::iterator object_key_itr = image_class_object_key_list_.begin();
    std::vector<DWORD>::iterator object_key_itr_end = image_class_object_key_list_.end();
    for ( ; object_key_itr != object_key_itr_end; ++object_key_itr)
    {
        DWORD object_key = *object_key_itr;
        Player* cinematic_character = static_cast<Player*>(g_ObjectManager.GetObject(object_key));

        if (cinematic_character == NULL)
        {
            continue;
        }
        
        if (cinematic_character->GetClass() == check_class_type)
        {
            return true;
        }
    }
    return false;
}

DWORD CharacterScene::ChangeCreateClass(eCHAR_TYPE change_class_type)
{
    DWORD object_key = GetCreateClassObjectKey(change_class_type);
    if (object_key)
    {
        m_dwSelectedObjectKey = object_key;
        ShowOnlyCinematicCharacter(object_key);
        return object_key;
    }
    return 0;
}

DWORD CharacterScene::GetCreateClassObjectKey(eCHAR_TYPE change_class_type)
{
    std::vector<DWORD>::iterator object_key_itr = image_class_object_key_list_.begin();
    std::vector<DWORD>::iterator object_key_itr_end = image_class_object_key_list_.end();
    for ( ; object_key_itr != object_key_itr_end; ++object_key_itr)
    {
        DWORD object_key = *object_key_itr;
        Player* cinematic_character = static_cast<Player*>(g_ObjectManager.GetObject(object_key));

        if (cinematic_character == NULL)
        {
            continue;
        }

        if (cinematic_character->GetClass() == change_class_type)
        {
            DWORD object_key = cinematic_character->GetObjectKey();
            m_dwSelectedObjectKey = object_key;
            return object_key;
        }
    }
    return 0;
}

void CharacterScene::ShowBaseCinematicCharacter()
{
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#else
    // 각 클래스 대표 캐릭터를 세팅
    // 대표 캐릭터만 렌더링하며 나머지는 숨김.
    for (int object_key = CHARACTERSCENE::BERSERKER; object_key < CHARACTERSCENE::PLAYER_LIST; ++object_key) 
    {
        Player* cinematic_character = static_cast<Player*>(g_ObjectManager.GetObject(object_key));
        if (cinematic_character == NULL)
        {
            continue;
        }

        DWORD cinematic_character_class_type = static_cast<DWORD>(cinematic_character->GetClass());
        BOOL is_show = (CHARACTERSCENE::kBaseCinematicCharacter[cinematic_character_class_type - 1] == object_key) ? TRUE : FALSE;
        cinematic_character->SetShow(is_show);

        ReleaseAllParts(object_key);
    }
#endif // _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
}

void CharacterScene::ChangeSelectCharacter(DWORD show_object_key)
{
    Player* cinematic_character = NULL;
    std::vector<DWORD>::iterator object_key_itr = image_class_object_key_list_.begin();
    std::vector<DWORD>::iterator object_key_itr_end = image_class_object_key_list_.end();
    for ( ; object_key_itr != object_key_itr_end; ++object_key_itr)
    {
        DWORD object_key = *object_key_itr;
        cinematic_character = static_cast<Player*>(g_ObjectManager.GetObject(object_key));

        if (cinematic_character == NULL)
        {
            continue;
        }
        cinematic_character->SetShow(FALSE);
    }

    for (int object_key = CHARACTERSCENE::BERSERKER; object_key < CHARACTERSCENE::PLAYER_LIST; ++object_key) 
    {
        cinematic_character = static_cast<Player*>(g_ObjectManager.GetObject(object_key));
        if (cinematic_character == NULL)
        {
            continue;
        }

        //BOOL is_show = (show_object_key == object_key) ? TRUE : FALSE;

        if (show_object_key == object_key)
        {
            cinematic_character->SetShow(TRUE);
            m_dwPickedObjectKey = object_key;
            create_selected_character_object_key_ = show_object_key;
        }        
    }
}

void CharacterScene::ShowOnlyCinematicCharacter(DWORD show_object_key)
{
    for (int object_key = CHARACTERSCENE::BERSERKER; object_key < CHARACTERSCENE::PLAYER_LIST; ++object_key) 
    {
        Player* cinematic_character = static_cast<Player*>(g_ObjectManager.GetObject(object_key));
        if (cinematic_character == NULL)
        {
            continue;
        }

        BOOL is_show = (show_object_key == object_key) ? TRUE : FALSE;
        cinematic_character->SetShow(is_show);
    }
}

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
void CharacterScene::ZoomCamera( float rate)
{
	xGetCharacterCreate()->ZoomCamera( rate );
}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

void CharacterScene::RotateCinematicCharacter(float angle)
{
    std::vector<DWORD>::iterator object_key_itr = image_class_object_key_list_.begin();
    std::vector<DWORD>::iterator object_key_itr_end = image_class_object_key_list_.end();
    for ( ; object_key_itr != object_key_itr_end; ++object_key_itr)
    {
        Object* cinematic_character = g_ObjectManager.GetObject(*object_key_itr);

        if (cinematic_character == NULL)
        {
            continue;
        }

        float character_angle = cinematic_character->GetAngle();
        character_angle += angle;
        cinematic_character->SetAngle(character_angle, TRUE);
    }
}

void CharacterScene::SetBackCinematicCharacter()
{
    // 각 캐릭터 앵글 복구 및 장비 해제
    std::vector<DWORD>::iterator object_key_itr = image_class_object_key_list_.begin();
    std::vector<DWORD>::iterator object_key_itr_end = image_class_object_key_list_.end();
    for ( ; object_key_itr != object_key_itr_end; ++object_key_itr)
    {
        DWORD object_key = *object_key_itr;
        Player* cinematic_character = static_cast<Player*>(g_ObjectManager.GetObject(object_key));

        if (cinematic_character == NULL)
        {
            continue;
        }
        ReleaseAllParts(object_key);
        cinematic_character->SetAngle(m_fOldAngle, TRUE);
        cinematic_character->SetHeightVariation(2);
        cinematic_character->SetPartVariation(PLAYER_VARIATION_FACE,1);
        cinematic_character->SetPartVariation(PLAYER_VARIATION_HAIR,1);
        cinematic_character->RefreshAppearRender();
    }
}

void CharacterScene::SelectCharecter()
{
    if (BIT_CHECK(m_packetStatus, PACKET_STATUS_CHARACTERSCENE_SELCHAR) == FALSE) 
    {
#ifdef _NA_000000_20130114_RENEWER_UI 
		uiCharacterSelectList2* select_list = GET_CAST_DIALOG(uiCharacterSelectList2, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#else
		uiCharacterSelectList* select_list = GET_CAST_DIALOG(uiCharacterSelectList, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_LIST);
#endif

        if (select_list == NULL)
        {
            return;
        }

        CharListInfo* character_info = GetCharacterInfoBySlotIndex(select_list->GetSelectedCharacterSlotIndex());
        if (character_info == NULL)
        {
            return;
        }

        Player* selected_character = static_cast<Player*>(g_ObjectManager.GetObject(character_info->dwObjectKey));
        if (selected_character == NULL)
        {
            return;
        }

        WORD intersect_result = NTCT_OUTSIDE;
        float distance = 0.f;

        if (selected_character->GetNumPickingBoundingVolume() > 0)
        {
            WzBoundingVolume bounding_volume = *selected_character->GetPickingBoundingVolume(0);
            intersect_result = Intersect_Ray_Bv(&g_wrMouseRay, &bounding_volume, &distance);

        }
        else
        {
            WzBoundingVolume bounding_volume = *selected_character->GetBoundingVolume();
            intersect_result = Intersect_Ray_Bv(&g_wrMouseRay, &bounding_volume, &distance);
        }

        if (intersect_result != NTCT_OUTSIDE)  
        {
            DWORD time_diff = g_CurTime - m_preTime;

            if(time_diff < DOUBLE_CLICK_TIME)//더블클릭 처리 
            {
                SendCharSelectPacket();
            }

            m_preTime = g_CurTime;
        }
    }   
}

void CharacterScene::ShowSelectCharacter(int slot_index)
{
    m_dwSelectCharacter = INVALID_OBJECT_KEY;
    m_dwSelectOid = INVALID_OBJECT_KEY;

    create_selected_character_object_key_ = 0;

    CHARLIST_ITR character_itr = m_CharList.begin();
    CHARLIST_ITR character_itr_end = m_CharList.end();
    CharListInfo * character_info = NULL;
    for ( ; character_itr != character_itr_end; ++character_itr)
    {
        character_info = (*character_itr);
        if (character_info == NULL)
        {
            continue;
        }

        Player* character_ptr = static_cast<Player*>(g_ObjectManager.GetObject(character_info->dwObjectKey));
        if (character_ptr == NULL)
        {
            continue;
        }

        BOOL is_show = (character_info->CharacterInfo.m_bySlot == slot_index) ? TRUE : FALSE;
        character_ptr->SetShow(is_show);

        Pet* pPet = NULL;
        if(character_ptr->IsPetReturn())
        {
            pPet = (Pet*) g_ObjectManager.GetObject(character_ptr->GetPetObjectKey());
            if(pPet)
            {
                pPet->SetStatePetHide((is_show == TRUE) ? _HIDE_SKILL_INFO::NONE_HIDE : _HIDE_SKILL_INFO::ALL_HIDE);
            }
        }

        if (is_show == TRUE)
        {
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
			xGetCharacterCreate()->ResetCameraPosition( GetRace(character_ptr->GetClassCode()) );
			//m_camera.x = GetCameraX(GetRace(character_ptr->GetClassCode()));
			//m_target.x = m_camera.x;
			//m_zoom = 0.0f;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

            m_dwSelectCharacter = character_info->CharacterInfo.m_bySlot;
            m_dwSelectOid = character_info->dwObjectKey;
        }
    }
    CharSelectDialog* charater_select_dialog = GET_CAST_DIALOG(CharSelectDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG);
    if (charater_select_dialog != NULL)
    {
        if (m_dwSelectCharacter != INVALID_OBJECT_KEY)
        {
            CharListInfo* char_info = g_CharSelectScene.GetCurSelectCharacterInfo();
            if (char_info && char_info->CharacterInfo.m_DelChk == 0)
            {
                charater_select_dialog->ShowDeleteButton(true);
                charater_select_dialog->EnableButton(true, CharSelectDialog::CharSelect_Delete);
                charater_select_dialog->EnableButton(true, CharSelectDialog::CharSelect_Start);
            }
            else
            {
                charater_select_dialog->ShowDeleteButton(false);
                charater_select_dialog->EnableButton(true, CharSelectDialog::CharSelect_UnDelete);
                charater_select_dialog->EnableButton(false, CharSelectDialog::CharSelect_Start);
            }
        }
        else
        {
            charater_select_dialog->EnableButton(false, CharSelectDialog::CharSelect_Delete);
            charater_select_dialog->EnableButton(false, CharSelectDialog::CharSelect_Start);
            charater_select_dialog->ShowDeleteButton(true);
        }
    }
}

void CharacterScene::HideAllCharacter()
{
    CHARLIST_ITR char_list_itr = m_CharList.begin();
    CharListInfo * char_list_info = NULL;

    for (; char_list_itr != m_CharList.end(); ++char_list_itr)
    {
        char_list_info = static_cast<CharListInfo *>(*char_list_itr);
        if( Player* character_object = static_cast<Player*>(g_ObjectManager.GetObject(char_list_info->dwObjectKey)) )
        {
            character_object->SetShow(false);
            Pet* pet_object = NULL;
            if(character_object->IsPetReturn())
            {
                pet_object = (Pet*) g_ObjectManager.GetObject(character_object->GetPetObjectKey());
                if(pet_object)
                {
                    pet_object->SetStatePetHide(_HIDE_SKILL_INFO::ALL_HIDE);
                }
            }
        }
    }
}

bool CharacterScene::IsMakeCharacterBetakey(BYTE class_code, eCHAR_TYPE base_gender_of_class)
{
#ifdef _SCJ_REMOVE_GENDER_CREATE_SCEANE
    return (class_code == base_gender_of_class);
#endif //_SCJ_REMOVE_GENDER_CREATE_SCEANE

#ifdef _NA_20100307_BETAKEY_SYSTEM
    uiLoginMan* login_manager = static_cast<uiLoginMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
    BetaKeyInfo* betakey_info = NULL;
    if (login_manager)
    {
        betakey_info = login_manager->GetBetaKeyInfo();

        BYTE base_gender = static_cast<BYTE>(ResourceCode::GetBaseGenderOfClass(base_gender_of_class));
        bool is_can_create = BetaKeyImpl::CanCreateCharacter(betakey_info, base_gender_of_class, base_gender);

        if (betakey_info && is_can_create)
        {
            return true;
        }
    }
    return false;
#else
    return true; // 베타키가 포함된 것이 아니면 무조건 true;
#endif//_NA_20100307_BETAKEY_SYSTEM
}



//------------------------------------------------------------------------------ 
// NextFrame - advances to the next Bink frame.
bool CharacterScene::NextFrame(HBINK bink)
{
    if (bink == NULL)
    {
        return false;
    }

    // Decompress the Bink frame.
    BinkDoFrame(bink);

    // Are we at the end of the movie?
    if (bink->FrameNum == bink->Frames)
    {
        return FALSE;
    }

    // Lock the BinkBuffer so that we can copy the decompressed frame into it.
    long lPitch;
    int iComponent;
    VOID* pByte = g_pSunRenderer->x_pManagerTexture->LockTexture(Movie.bink_texture, &lPitch, &iComponent);
    if (pByte == NULL)
    {
        return FALSE;
    }
    BinkCopyToBuffer(bink, pByte, lPitch, bink->Height, 0,0, BINKSURFACE32);
    g_pSunRenderer->x_pManagerTexture->UnlockTexture(Movie.bink_texture);

    BinkNextFrame(bink);
    return TRUE;
}
//------------------------------------------------------------------------------ 
void CharacterScene::set_show_movie(bool val)
{ 
    if (val  == true)
    {
        if (Movie.bink == NULL)
        {
            Movie.bink = BinkOpen(GENERALPARAM->characterscene_cinematic_filename(), 0);
            if (Movie.bink == NULL)
            {
                return;
            }
            Movie.bink_texture = g_pSunRenderer->x_pManagerTexture->GenerateEmptyTexture(Movie.bink->Width, Movie.bink->Height, 4,
                        TLS_NO_MIPMAP | TLS_EMPTY_TEXTURE);
            if (Movie.bink_texture == NULL)
            {
                BinkClose(Movie.bink);
                Movie.bink = NULL;
                return;
            }
        }

        BinkGoto(Movie.bink, 0, 0); 
        Movie.backup_clear_color = g_pSunRenderer->GetClearColor();

        g_pSunRenderer->SetClearColor(0, 0, 0);
        g_InterfaceManager.SaveAppearWindows();
        g_InterfaceManager.HideAllWindows();
        ShowCursor(FALSE);
    }
    else
    {
        g_pSunRenderer->SetClearColor(Red_WzColor( Movie.backup_clear_color), 
                                        Blue_WzColor( Movie.backup_clear_color ),
                                        Green_WzColor( Movie.backup_clear_color), 
                                        Alpha_WzColor( Movie.backup_clear_color));

        g_InterfaceManager.LoadAppearWindows();
        ShowCursor(TRUE);
    }
    Movie.is_show_movie = val;
}