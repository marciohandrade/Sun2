
//------------------------------------------------------------------------------
//  BattleScene.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "GameFramework.h"
#include "Player.h"
#include "Monster.h"
#include "BattleScene.h"
#include "varpacket.h"
#include "Camera.h"
#include "ItemManager.h"
#include "HotKeyManager.h"
#include "HeroActionInput.h"
#include "Battle.h"
#include "SoundBGM.h"
#include "SolarDialog.h"
#include "LoadScene.h"
#include "Map.h"
#include "GameFunc.h"
#include "ObjectManager.h"
#include "RewardDialog.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "Clone.h"
#include "CloneManager.h"
#include "ProjectileManagerEx.h"
#include "ChatDialog.h"

#include "HeroGaugeDialog.h"
#include "TriggerParser.h"
#include <TriggerGroupInfo.h>
#include <TriggerInfoManager.h>
#include "TriggerManEx.h"
#include "OptimizationManager.h"
#include "HelpMessageParser.h"
#include "Message.h"
#include "MapUnitCacher.h"
#include "NPCInfoParser.h"
#include "SkillEffectInfoParser.h"
#include "MouseHandler.h"
#include "SystemDialog.h"
#include "EquipmentContainer.h"
#include "SkillInfoParser.h"
#include "CommonCharacterFormula.h"
#include "Mouse.h"
#include "ItemInfoParser.h"
#include "GameOptionParser.h"
#include "PlayerAttributes.h"
#include <SCItemSlot.h>
#include "VillageScene.h"
#include "NPC.h"
#include "EventInfoParser.h"
#include "uiPartyMember/uiPartyMember.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemFunction.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
    #include "uiGuildCreate/uiGuildCreate.h"
    #include "uiGuildWindow/uiGuildWindow.h"
    #include "uiGuildMenu/uiGuildMenu.h"
    #include "uiGuildMain/uiGuildMain.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiChaoSystemMan/uiChaoSystemMan.h"
#include "TargetDialog.h"
#include "tiredsystemdialog.h"
#include "uiMissionRelayMan/uiMissionRelayMan.h"
#include "MissionRelay.h"
#include "ChatPersonalOption.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "BattleLobby.h"
#include "Global.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "BGMSoundInfoParser.h"
#include "MouseCursor.h"
#include "KeyQueueManager.h"
#include "StateInfoParser.h"
#include "RadarInfoParser.h"
#include "radardialog.h"
#include "radarplusdialog.h"
#include "RadarFullDialog.h"
#include "CastDialog.h"
#include "CastLeaveFieldDialog.h"
#include "uiHelpMan/uiHelpMan.h"
#include "MapInfoParser.h"
#include "uiPartyMember/uiPartyMember.h"
#include "uiQuestMan/uiQuestMan.h"
#include "QuestManager_Concrete.h"
#include "MissionManager_Concrete.h"
#include "uiPartyMan/uiPartyMan.h"
#include "ResourceManager.h"
#include "CursorChangeTransact.h"
#include "Application.h"
#include "GlobalFunc.h"
#include "SkillEffectFunc.h"
#include "SkillEffectInfoParser.h"
#include "MissionScene.h"
#include "AppearanceManager.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "InstanceDungeonScene.h"
#include "uiLoginMan/uiLoginMan.h"
#include "DelayLoadManager.h"

#ifdef _YJ_FISHING_SYSTEM_000933
#include "uiFishingShop/uiFishingShop.h"
#include "uiFishingTrade/uiFishingTrade.h"
#include "uiFishingAction/uiFishingAction.h"
#endif //_YJ_FISHING_SYSTEM_000933

#include "uiItemStatChangeSysMan/uiItemStatChangeSysMan.h"
#include "uiItemStatChangeSys/uiItemStatChangeSys.h"
#include "ScreenShotSaver.h"
#include <ssq/SSQInfoStruct.h>
#include <ssq/SSQInfoParser.h>
#include "SSQScene.h"
#include "uiSSQMan/uiSSQManager.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiTiredSystemMan/uiTiredSystemMan.h"
#include "uiMenuMan/uiMenuMan.h"
#include "MainDialog.h"
#include "uiherogaugeman/uiherogaugeman.h"
#include "uiquickcontainerman/uiquickcontainerman.h"
#include "uiChatMan/uiChatMan_def.h"

#include "uiQuestListDlg/uiQuestListDlg.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "uiDominationTimer/uiDominationTimer.h"
#include "uiAssistMan/uiAssistMan.h"

#include "Pet.h"

#include "uiBattleScoreMan\uiBattleScoreMan.h"

#include "Interface/uiCommunityDlg/uiCommunityDlg.h"

#ifdef _YMS_AUTOBOT_GAME
    #include "Autobot/Autobot.h"
#endif //_YMS_AUTOBOT_GAME

#include "QuickContainer.h"

#include "uiSkillMan/uiNewSkillMan.h"
#include "uiPerkDialog/uiPerkDialog.h"
#include "state/SkillStateFactory.h"
#include "Rider.h"
#include "uiNoticeMan/uiNoticeMan.h"
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiConvenienceStore/uiConvenienceStoreMan.h"
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL

#ifdef _DH_BATTLEZONE2_
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#include "uiBattlezone2List/uiBattlezone2List.h"
#include "uiBattlezone2Lobby/uiBattlezone2Lobby.h"
#endif//_DH_BATTLEZONE2_
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "HeroAttributes.h"
#include "HeroData.h"
#include "State/SkillState.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildSystem/GuildSystemPacketFunction.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
#include "InDunOptionParser.h"
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY

///////////////////////////////////////////////////////////////
// static 변수 초기화
DWORD			BattleScene::ms_RoomKey = 0;
BattleRoomEntry BattleScene::ms_BattleRoomEntry;
int				BattleScene::m_iScreenShotState = eSCREEN_SHOT_FULL;
BOOL			BattleScene::m_bMissionReward = FALSE;
bool            BattleScene::m_bPendingBackToSomeWhere = false;
int				BattleScene::m_SceneCount=0;

BOOL			BattleScene::m_bLobbyFlag			= FALSE;	// 현재 로비인지 check 해주는 변수 플레그
BOOL			BattleScene::m_bLobbyPendingFlag	= FALSE;	// 로비로 들어가기전 전 플레그
BOOL			BattleScene::m_bLobbyOutPendingFlag = FALSE;	// 로비를 빠져나오기 전 플레그
BOOL		    BattleScene::m_bWaitEnterSync		= FALSE;
BOOL            BattleScene::m_bDrawWait			= FALSE;

KEYTYPE			BattleScene::m_CurLobbyKey		= 0;
DWORD			BattleScene::m_dwVillCheckSum	= 0;
DWORD           BattleScene::m_dwVillCheckSum_WMO = 0;

util::Timer		BattleScene::m_StayTimer;
int				BattleScene::m_iGameConnectedTime = 0;

#ifdef _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
WzID BattleScene::last_loaded_submapid_ = 0;
#endif //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
///////////////////////////////////////////////////////////////

#define		FREE_CAMERA_DEFAULT_SPEED			4.0f
#define		FREE_CAMERA_SPEED_UP				0.2f
#define		FREE_CAMERA_MIN_SPEED_UP			0.2f
#define		FREE_CAMERA_MAX_SPEED_UP			50.0f
#define		TEN_MINUTE							600000
float		g_fFreeCameraSpeed = FREE_CAMERA_DEFAULT_SPEED;



// 임시 타이머
util::Timer * g_pDelayLeaveFieldTimer = NULL;

const int BLACK_BLIND_TIME = 1000;
const int WHITE_BLIND_TIME = 1500;


//------------------------------------------------------------------------------
/**
*/
BattleScene::BattleScene()
{
	m_iScreenShotState = eSCREEN_SHOT_FULL;
	m_ExpList.clear();
	m_fMonsterHPMultiplier = 1.0f;
	m_fBlur_weight = 1.0f;
	m_fFog_weight = 1.0f;
	m_iPrevSectorIndex = -1;

    m_bPendingBackToSomeWhere = false;
	m_dwOldSelectId = 0;
	m_StayTimer.SetTimer(TEN_MINUTE);

	m_pTriggerMan = NULL;
	m_bTriggerInit = FALSE;

	m_SceneCount++;
}
//------------------------------------------------------------------------------
/**
*/
BattleScene::~BattleScene()
{
	m_SceneCount--;
}

//------------------------------------------------------------------------------
/**
*/

void BattleScene::SetHeroInfo( BYTE byClassCode )
{
    xGetHeroData()->SetClassCode( byClassCode );
}


//----------------------------------------------------------------------------
/**
*/
void BattleScene::SendPlayerEnterSyn(BOOL bWaitDraw /*= TRUE*/)
{
	assert(g_pMap);

	if(NULL == g_pMap)
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		//70170	%s: 알려지지 않은 오류입니다. GM에게 문의하십시오.
		Snprintf(szMessage, INTERFACE_STRING_LENGTH -1, g_InterfaceManager.GetInterfaceString(70170) , _T("Current Map Info Error"));
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE, szMessage);

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
		{
			pSystemMan->Sys_VerifyLock(szMessage, TRUE);	// 지금 상황에서  g_pMap 이 NULL이면 종료할 수 밖에 없다.
		}

		return;
	}

	MSG_CG_SYNC_PLAYER_ENTER_SYN		SendPacket;
    
	SendPacket.m_byCategory		= CG_SYNC;
	SendPacket.m_byProtocol		= CG_SYNC_PLAYER_ENTER_SYN;
	
	if(g_pMap->IsVill() || g_pMap->IsTutorialField()) 
    {
        //최근마을이나 튜토리얼 필드의 체크썸 저장.
		m_dwVillCheckSum = g_pMap->GetWorldBase()->GetCheckSum();
        m_dwVillCheckSum_WMO = g_pMap->checksum_wmo();
    }

	//미션 재시도후 로비에서 나갈경우 빌리지 체크썸 사용.
	if(BattleScene::IsLobbyOutPending())
	{
		SendPacket.m_dwCheckSum		= m_dwVillCheckSum;
        SendPacket.checksum_wmo_    = m_dwVillCheckSum_WMO;
	}
	else
	{
		SendPacket.m_dwCheckSum	 = g_pMap->GetWorldBase()->GetCheckSum();
        SendPacket.checksum_wmo_ = g_pMap->checksum_wmo();
	}
	
    

	if( FALSE == GlobalFunc::SendPacket( CI_GAMESERVERIDX, 
		&SendPacket, 
		sizeof(MSG_CG_SYNC_PLAYER_ENTER_SYN) ) )
	{
		assert( !"패킷 Send 에러 - MSG_CG_SYNC_PLAYER_ENTER_SYN" );
		return;
	}

	m_bWaitEnterSync = TRUE;
	m_bDrawWait = bWaitDraw;


	g_InstanceDungeonScene.Send_MSG_CG_ETC_GET_BATTLE_OPEN_TIME_SYN();
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN();
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::SetRoomKey(DWORD dwKey)
{
	ms_RoomKey = dwKey;
}

//------------------------------------------------------------------------------
/**
*/
DWORD BattleScene::GetCurRoomKey()
{
	return ms_RoomKey;
}


//------------------------------------------------------------------------------
/**
*/
void BattleScene::SetPendingBattleRoomEntry(BattleRoomEntry * IN pBattleRoomEntry)
{
	memcpy(&ms_BattleRoomEntry, pBattleRoomEntry, sizeof(ms_BattleRoomEntry));
}


//------------------------------------------------------------------------------
/**
*/
BattleRoomEntry* BattleScene::GetPendingBattleRoomEntry()
{
	return &ms_BattleRoomEntry;
}

BOOL BattleScene::IsComeBackFromLobby() const
{
	return BOOL(this->m_bComebackFromLobby);
}

//------------------------------------------------------------------------------
/**
*/
const HERO_INFO * BattleScene::GetHeroInfo()
{
    return xGetHeroData()->GetHeroInfo();
}

void BattleScene::SaveHero(Hero * pHero)
{
    xGetHeroData()->SaveHero( pHero );
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::Activate()
{
}

//------------------------------------------------------------------------------
/**.
*/
BOOL BattleScene::Init()
{
#ifdef _DH_BATTLEZONE2_
    uiBattlezone2Lobby* battlezone2_lobby = GET_CAST_DIALOG(uiBattlezone2Lobby, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LOBBY);
    if (battlezone2_lobby)
    {
        battlezone2_lobby->Unlocks();
    }
#endif//_DH_BATTLEZONE2_
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
    if (chaos_system_manager != NULL)
    {
        chaos_system_manager->Unlocks();
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	if(!PROGRAMPARAM->GetEffectBatch())
	{
		g_pSunRenderer->nType = 0;
	}
	else 
	{
		g_pSunRenderer->nType = 1;
	}

#ifdef _INFINITE_MAP_LOADING_TEST
	m_bReadyMapLoading = FALSE;
	m_dwStartTick = clock_function::GetTickCount();
#endif
    uiPartyMan* partyMan =
        static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
    if (partyMan)
    {
        partyMan->uiShowAll();
    }

    uiDominationTowerMan* ui_domination_manager = 
        GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager)
    {
        ui_domination_manager->SetDominationGuildInfo(NULL);
    }

	g_fFreeCameraSpeed = FREE_CAMERA_DEFAULT_SPEED;

	if (g_pMap)
		g_pMap->SetRenderFlag(TRUE);
	

	m_eFadeState=eFADE_STATE_NONE;
	m_dwCurFadeTime=0;

#ifdef _PERF_DEBUG
	if(g_pSunRenderer->x_pManagerMultiLoader)
		g_pSunRenderer->x_pManagerMultiLoader->TurnOn_MultiThreadedLoading(FALSE);
#else


	//게임 옵션으로 멀티쓰레드 옵션 빼기.
	if (GAMEOPTION->GetOperateMultiThread())
	{
		if(g_pSunRenderer->x_pManagerMultiLoader)
			g_pSunRenderer->x_pManagerMultiLoader->TurnOn_MultiThreadedLoading(TRUE);
	}
	else
	{
		if(g_pSunRenderer->x_pManagerMultiLoader)
            g_pSunRenderer->x_pManagerMultiLoader->TurnOn_MultiThreadedLoading(FALSE);
	}


#endif

    g_pSunTerrain->SetDepthApplyOff(false);

    DelayLoadManager::Instance()->clear();
	
	SceneBase::InitLoadingImg();
	m_dwTestMonsterId=0;
	m_bFristRender=TRUE;

    m_GoupId = 0;
	m_iDbgTextType = eDebugTextType_None;
	m_bComebackFromLobby = FALSE;
    m_bCameraDetech = false;
	m_bPlayerAck = FALSE;

#ifndef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.Create();
#endif

	m_bIsMouseLBClick = FALSE;
	m_eTrigerState		= TRIGGER_STATE_LEAVE_AREA;
	m_eShorcutAreaState = SHORTCUT_AREA_STATE_LEAVE_AREA;
	m_iMapObjectID		= 0;

	m_bUseAutoCamera	= FALSE;

	//	로딩 배경 화면 이미지 로딩
	m_hTexture = g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\Interface\\Loading.jpg",TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	// mission clear 이미지 로딩
	m_hMissionClearTexture =g_pSunRenderer->x_pManagerTexture->LoadTexture( "Data\\Interface\\etc\\m_complete.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	g_CloneManager.Clear();	

	// 블라인드처리
	m_iCurBlindRate = 0;
	m_BlindType = BLIND_TYPE_NONE;

	m_dwBattleTimeSerial = 0;
	m_iBrightDelay = 0;
	m_GrayScaleColor = WzColor_RGBA(255,255,255,255);
	m_iGrayScaleDelay = 0;
	g_CurTime = 0;
	m_dwMissionClearTick = 0;
	m_dwCurFadeTime=0;
	m_eFadeState=eFADE_STATE_IN;

	m_ExpList.clear();

    m_iPrevSectorIndex = -1;

	//-------------------------------------------------------------------------------
	//		* 임시 : 몬스터 4마리 로딩
	//-------------------------------------------------------------------------------

	if( GENERALPARAM->IsNet() )
	{
		BattleScene::SendPlayerEnterSyn();

        DWORD class_code = xGetHeroData()->GetClassCode();
        ClassCodeEx class_code_ex;
        class_code_ex.class_type = xGetHeroData()->GetClassCode();
        class_code_ex.gender = xGetHeroData()->GetGender();
        class_code = class_code_ex.class_code;

        g_pHero = (Hero *)g_ObjectManager.Add(
            xGetHeroData()->GetObjectKey(),
            PLAYER_OBJECT,
            class_code,
            TRUE);

		assert(g_pHero);

		g_InterfaceManager.InitSkillInfos();

		g_pHero->SetHeroInfo( xGetHeroData()->GetHeroInfo());
		g_pHero->SetObserverMode(false,true);
	}
	else
	{
#ifdef _MOD_CHAGNE_DELETE_HERO
		g_ObjectManager.DeleteHero();
#endif
		g_pHero = (Hero *)g_ObjectManager.Add(
			g_ObjectManager.GenerateKeyAtSingleMode(PLAYER_OBJECT), 
			PLAYER_OBJECT,
			xGetHeroData()->GetClassCode(),
			TRUE);

        g_InterfaceManager.InitSkillInfos();

        g_pHero->SetHeroInfo( xGetHeroData()->GetHeroInfo());
		g_pHero->GetCharInfo()->m_byClassCode = xGetHeroData()->GetClassCode();

		WzVector wzVec;

		// 캐릭터의 기본 마을 ID에 있는 필드 넘버를 넘겨줘야함.
		BOOL bPos = g_pMap->GetHeroPositionOnlySingleVersion(&wzVec);
		if(bPos)
			g_pHero->SetPosition( wzVec );

		g_pHero->Rotate(0.0f);		
		g_pMap->SetUseAutoCamera(FALSE);
		g_pMap->SetAutoCameraTargetObjectKey( g_pHero->GetObjectKey() );

				
		if(FALSE == bPos)
		{
			WzVector vTestPos = g_pSunTerrain->GetRandomPosInTile(1);
			g_pHero->SetPosition( vTestPos, 1);
		}

		g_pHero->SetHP(100);
		g_pHero->SetMP(100);
		g_pHero->SetMoveSpeed(3.0f);
		g_pHero->SetLevel(20);

		g_pHero->SetPartVariation(PLAYER_VARIATION_HAIR,1);
		g_pHero->SetPartVariation(PLAYER_VARIATION_FACE,1);
        g_pHero->RefreshAppearRender();

		CursorChangeTransact::Instance()->ForceChange(MouseCursor::eMouseCursor_Default);
		
		g_InterfaceManager.ShowGameInterface(TRUE);
		
		SettingNPC(g_pMap->GetMapID(),g_pMap->GetCurrentFieldID());
	}

	// 인터페이스 랜더링 온
	g_InterfaceManager.SetRenderState( TRUE );
	g_InterfaceManager.ResetDamageList();
	g_ProjectileManagerEx.Clear();
	g_CloneManager.Clear();
	g_pSunRenderer->EnableAreaLight(TRUE);

	m_oldBlur         = g_pSunRenderer->x_pManagerPostEffect->GetTechBlur();
	m_oldFogParameter = g_pSunRenderer->GetFogParameter();

	m_pChatDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
	assert(m_pChatDlg);



	// 임시 변수
	m_MoveInfoList.clear();

    m_pTestMoveInfoExplorer = ( CPathExplorer* )Func_ContactTerrain( CT_NEW_PATH_EXPLORER, 0 );

	m_bRenderLightDirection = FALSE;
	m_vTestDir.x = 1.0f;
	m_vTestDir.y = 0.0f;
	m_vTestDir.z = 0.0f;

	//	동영상 촬영용 프리 카메라
	if( GENERALPARAM->IsFreeCameraMode() )
	{
		GENERALPARAM->SetFreeCameraState( FALSE );
	}

	const float BATTLESCENE_FOV = (WZ_PI / 4.0f);
	g_Camera.SetProjection(g_pMap->GetMapNearClip(),g_pMap->GetMapFarClip(),BATTLESCENE_FOV);
	 
	WzVector wzVector;

    g_pMap->GetPlayerDirection(&wzVector );
	g_pMap->ApplyOptionSetting();


	m_iScreenShotState = eSCREEN_SHOT_FULL;

	g_KeyQueueManager.Reset();
	g_KeyQueueManager.SetKey(DIK_ESCAPE);
	g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);


    m_EventText.Init();


	int fogValue= GRAPHICPARAM->GetFogValue();
	int blurValue = GRAPHICPARAM->GetBlurValue();
	m_fFog_weight = (float) fogValue / 100.0f > 1.0f ? 1.0f   :   (float) fogValue / 100.0f;
	m_fBlur_weight = (float) blurValue / 100.0f > 1.0f ? 1.0f :   (float) blurValue / 100.0f;

	InitQuestAreaInfo();

	if(NULL == m_pTriggerMan)
		m_pTriggerMan = new TriggerManEx;

	MouseHandler::Instance()->InitConfirmState();


#ifdef _JBH_ADD_AGE_LIMIT_MARK
	SceneBase::LoadingGameRatingMarkTexture();
#endif

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
		TiredSystemDialog* pTiredDlg = GET_CAST_DIALOG( TiredSystemDialog, IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM ); 
		if( pTiredDlg )
			pTiredDlg->OffLineTimeOfFatigueMessage();
#endif


	if(IsEnablePkScene())
	{

		CEnvObjectManager	* pManager = g_pMap->GetWorldBase()->GetEnvObjectManager();

		if(pManager)
		{
			St_EnvObject	New;
	
			WzBoundingVolume wbv;
			wbv.m_eBvType=BVT_TILE_ATT;

	
			New.m_WzID		= StrToWzID("PK01");
			New.m_fElements	= ENVOBJ_ELEM_FOG; 
			New.m_BoundVolume = wbv;
			New.m_wcFog		= WzColor_RGBA(255,0,0,255);//자유분쟁
			New.m_fFogBegin	= GRAPHICPARAM->GetPkFogBegin();
			New.m_fFogEnd	= GRAPHICPARAM->GetPkFogEnd();
			New.m_fFogRate	= GRAPHICPARAM->GetPkFogRate();
			New.m_wTileAtt	= PTA_FREE_CONFLICT_TILE;

			
			pManager->MakeStEnvObject(&New);


			New.m_WzID		= StrToWzID("PK02");
			New.m_fElements	= ENVOBJ_ELEM_FOG; 
			New.m_BoundVolume = wbv;
			New.m_wcFog		= WzColor_RGBA(255,128,0,255);//분쟁
			New.m_fFogBegin	= GRAPHICPARAM->GetPkFogBegin();
			New.m_fFogEnd	= GRAPHICPARAM->GetPkFogEnd();
			New.m_fFogRate	= GRAPHICPARAM->GetPkFogRate();
			New.m_wTileAtt	= PTA_CONFLICT_TILE;

			pManager->MakeStEnvObject(&New);

		}


	}

    g_HeroInput.ResetSpecialMove();

#ifdef _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
    last_loaded_submapid_ = 0;
#endif //_NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP

#ifdef _NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE
    // 섀도우 소환체에 붙일 렌더객체 초기화
    summon_render_info[0].m_dwRenderType |= 0x4; //라이트닝
    //summon_render_info.m_dwRenderType |= 0x40; //글로우

    //summon_render_info.m_dwRenderType |= 0x100; //EWRT_STREAM  
    //summon_render_info.m_dwRenderType |= 0x8000; //EWRT_UV1DNORMAL         
    summon_render_info[0].m_wcRender[WRIC_DIFFUSE] = WzColor_RGBA(35, 96, 197, 255);
    summon_render_info[0].m_wcRender[WRIC_SPECULAR] = WzColor_RGBA(128, 255, 255, 255);
    summon_render_info[0].m_wcRender[WRIC_AMBIENT] = WzColor_RGBA(142, 142, 142, 255);
    //summon_render_info.m_wiTextureToUse = StrToWzID("eeee");
    //summon_render_info.m_fStreamFactor[0] = 2;
    //summon_render_info.m_fStreamFactor[1] = 0;
    //summon_render_info.m_fSpecularIntensity = 100;
    //summon_render_info.m_wiTextureFilter = StrToWzID("##00");
    //summon_render_info.m_bUseAlphaFilter = TRUE;
    //summon_render_info.m_fGlowFactor = 1;
    //summon_render_info.m_bChangeGlowFactor = TRUE;       // 글로우 맵 강도를 변화 시킬건지
    //summon_render_info.m_fMinGlowFactor = 0;         // 글로우 맵 강도 최소값dd
    //summon_render_info.m_fMaxGlowFactor = 1;         // 글로우 맵 강도 최대값
    //summon_render_info.m_fTimeToChangeGlowFactor = 1; // 글로우 맵 강도 변화 시간
    
    summon_render_info[1].m_dwRenderType |= 0x40; //글로우
#endif //_NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE

#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
    InitRadarLayer(g_pMap->GetMapID());
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessFade(DWORD dwTick)
{

}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::Release()
{
	//SceneBase::LoadingImg();
    SceneBase::ReleaseLoadingImg();

	GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, "BattleScene::Release() - InterfaceManager::ReleaseDialogTexture()");
	g_InterfaceManager.ReleaseDialogTexture();

	this->m_bDeadFlag = FALSE;

	m_ExpList.clear();

	if(g_pSunRenderer->x_pManagerMultiLoader)
		g_pSunRenderer->x_pManagerMultiLoader->TurnOn_MultiThreadedLoading(FALSE);

	Sound::Instance()->StopEffectSoundAll();

	if (m_pTestMoveInfoExplorer) 
	{
        Func_ContactTerrain( CT_DELETE_PATH_EXPLORER, m_pTestMoveInfoExplorer );
		m_pTestMoveInfoExplorer = NULL;
		m_MoveInfoList.clear();
	}

	//	랜더링 플래그 끄기
	m_bEnableRender = FALSE;
	
	m_bPlayerAck = FALSE;

	if (m_pChatDlg)
	{
		SCENE_TYPE NextScene =  g_LoadScene.GetNextSceneAfterLoaded();

		//로그인창,캐릭터창에서만 채팅내용을 제거한다.
		if (NextScene==SCENE_TYPE_LOGIN || NextScene==SCENE_TYPE_CHARSELECT)
		{
			m_pChatDlg->ExhaustChatMsgALL();
		}
	}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    GuildSystem::Function::CloseGuildWareHouse();
#else
    uiGuildMan* pGuildMan = (uiGuildMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
    if(pGuildMan)
    {
        if(pGuildMan->IsGuildWareHouseWindow())
        {
            pGuildMan->SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN();
        }
    }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

	uiQuestListDlg* pQuestListDlg = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
	if(pQuestListDlg)
	{
		pQuestListDlg->SaveNaviQCode();
	}

	g_InterfaceManager.ShowGameInterface( FALSE );
	g_InterfaceManager.HideMessageBox();
	g_InterfaceManager.HideCenterMessageBox();
	g_InterfaceManager.HideAllWindows();

	g_ProjectileManagerEx.Clear();
	g_pEffectManager->DestroyAllEffect();

	RestoreRenderState();
	ReleaseInterfaceResource();
	// 주의!
	// octree에서 정상적으로 object를 제거하기위하여 반드시 map보다 먼저 object를 release해주어야함
	// 오브젝트를 릴리즈 하기 전에 히어로 정보 세이브
	if( g_pHero )
		BattleScene::SaveHero(g_pHero);
	ReleaseObjects();
	

	if (g_pMap)
	{
		g_pMap->Destroy();
	}

	

    uiPartyMan* partyMan =
        static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
    if (partyMan)
    {
        partyMan->uiClearAll();
    }
	
	m_dwMissionClearTick = 0;

	//	동영상 촬영용 프리 카메라
	//	동영상 촬영용 프리 카메라
	if( GENERALPARAM->IsFreeCameraMode() )
	{
		GENERALPARAM->SetFreeCameraState( FALSE );
	}

	g_Camera.SetProjection(g_Camera.GetNearClip(),g_Camera.GetFarClip(),CAMERA_DEFAULT_FOV);

	SAFE_DELETE( g_pDelayLeaveFieldTimer );

	uiQuestMan * pQuestMan = 
		(uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
	if ( pQuestMan )
	{
		pQuestMan->ClearAcceptAuto();
	}

#ifdef _JBH_ADD_AGE_LIMIT_MARK
	ReleaseGameRatingMarkTexture();
#endif //_JBH_ADD_AGE_LIMIT_MARK

	m_bTriggerInit = FALSE; //씬 진입시 매번 Init
	if (m_pTriggerMan)
    {
		m_pTriggerMan->Release();
    }

	SAFE_DELETE(m_pTriggerMan);

	SceneBase::ReleaseLoadingImg();
}

//------------------------------------------------------------------------------
/**
*/
BOOL BattleScene::Process( DWORD dwTick )
{
	UpdateGameStayTime();
	PrintPcBangBanMessage();

#ifndef	_CHINA
	PrintYoungManProtectMessage();
#endif


	ProcessQuestArea();
	ProcessQuestTime();

#ifdef _INFINITE_MAP_LOADING_TEST
	if( clock_function::GetTickCount() - m_dwStartTick > 15000 )
	{
		sMAPPORTAL_HASH * pMapPortalHash =  MapInfoParser::Instance()->GetMapPortalHashTable();
		sMAPPORTAL_HASH::iterator itr = pMapPortalHash->begin();


		int iMapCode = 0;
		while( itr != pMapPortalHash->end() )
		{
			BOOL bFound = FALSE;
			sMAP_ENTERENCE_PORTAL * pMapInfo = ( *itr);
			for(int i = 0; i < m_vecLoadingIndex.size(); i++)
			{
				if( pMapInfo->wIndex == m_vecLoadingIndex[i] )
				{
					// 이미 탔던 포탈 이다.
					bFound = TRUE;
				}
			}
			if( !bFound )
			{
				// 처음 타는 포탈이면 타도 좋다.
				m_vecLoadingIndex.push_back(pMapInfo->wIndex);
				iMapCode = pMapInfo->wIndex;
				break;
			}
			itr++;
		}

		// 모두 다 탔으면 처음으로 다시 반복한다.
		if( pMapPortalHash->size() == m_vecLoadingIndex.size() )
		{
			m_vecLoadingIndex.clear();
		}
		if( iMapCode)
		{
			m_dwStartTick = 0;
			MSG_CG_ZONE_PORTAL_FIELD_MOVE_ACK SendPacketAck;
			SendPacketAck.m_wIndex = iMapCode;
			g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
		}

	}
#endif


	SUNPROCESSPROFILE_BEGIN(_T("ProcessInput"));
	g_CurTime	+= dwTick;

    //------------------------------------------------------------------------------

	ProcessExpList();
	g_pMap->Update( dwTick );

	m_EventText.ProcessEventScript(dwTick);    
    ProcessBlind(dwTick);
	ProcessSpecialEffect(dwTick);
	ProcessKeyInput();
	ProcessKeyInput_GM();

	m_TextDisplayManager.Process( dwTick );

	g_CloneManager.Process(dwTick);
	g_ProjectileManagerEx.Process(dwTick);
	//-----------------------------------------------------------------------------------
	//		마우스 좌표 갱신( 다이렉트 인풋 )
	//-----------------------------------------------------------------------------------

	LPDIMOUSESTATE2	pMouserState = g_Input.GetMouseBuffer();

	if(  g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_DOWN) )
	{
		m_bIsMouseLBClick = TRUE;
	}
	else
	{
		m_bIsMouseLBClick = FALSE;
	}

	SUNPROCESSPROFILE_END(_T("ProcessInput"));

	SUNPROCESSPROFILE_BEGIN(_T("ProcessUnits"));
	ProcessUnits(dwTick);
	SUNPROCESSPROFILE_END(_T("ProcessUnits"));

	SUNPROCESSPROFILE_BEGIN(_T("ProcessCamera"));
		//	자동 카메라 사용
	if (g_pMap->GetUseAutoCamera())
	{
		g_pMap->AutoCameraPlay( dwTick );
	}
	else
	{
		ProcessCamera(pMouserState,dwTick);
	}
	SUNPROCESSPROFILE_END(_T("ProcessCamera"));

	ProcessEffect(dwTick);

	ProcessKeyInput_HotKey();
    ProcessKeyInput_Hero(dwTick);

	//	랜더링 플래그 켜기
	m_bEnableRender = TRUE;

	if ( m_bWaitEnterSync == FALSE)
	{
		m_bDrawWait = FALSE;
	}
	//	트리거 처리
	SUNPROCESSPROFILE_BEGIN(_T("ProcessTrigger"));
	ProcessTrigger();
	SUNPROCESSPROFILE_END(_T("ProcessTrigger"));

	// 디버그용
	ProcessMoveInfo(dwTick);
	ProcessRenderOption(pMouserState);

	UpdateItemShareSystem();
    UpdateItemOwner();

	if(g_pMap && IsEnablePkScene())
	{
		CEnvObjectManager	* pManager = g_pMap->GetWorldBase()->GetEnvObjectManager();
		if(pManager && g_pHero)
		{
			DWORD dwAtt = g_pHero->GetCurrentTileAtt(FALSE);
			if(BIT_CHECK(dwAtt, PTA_CONFLICT_TILE) || BIT_CHECK(dwAtt, PTA_FREE_CONFLICT_TILE))//자유분쟁,분쟁 지역만 안개효과를 낸다
			{
				pManager->SetActorType(EAT_BY_ACTORPOSITION);
			}
			else
			{
				pManager->SetActorType(EAT_BY_CAMERA);
			}

			pManager->SetActorPos(g_pHero->GetPosition());
			pManager->SetActorTileAtt((WORD)dwAtt);
		}
	}


#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    {
        //! KBMoveCheckManagerInClient::Instance()->CheckLocalTestRequest
        //! 함수 내엣 자체적으로 tick를 계산하기때문에 특별히 tick를 계산하지 않는다.
        //! 일정 간격(틈을 주고)으로 호출하기 위함
        //MSG_CG_CONNECTION_CLIENT_TICK_TEST_SYN syn;
        //if (KBMoveCheckManagerInClient::Instance()->CheckLocalTestRequest<1>(&syn) == true)
        //{
        //    // 일단 보내지 않는다.
        //    GlobalFunc::SendPacketToGameServer(&syn, sizeof(syn));
        //}
    }
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL BattleScene::Render( DWORD dwTick )
{	

	if(m_bFristRender)
	{
		m_bFristRender=FALSE;
        float fGamma = GAMEOPTION->GetGameOption().GetOptionValueFloat( nOptionType::GAMMA_VALUE );

        SetGamma(DWORD( fGamma ) );
	}

	//	랜더링 플래그가 꺼져 있으면 랜더링 하지 않음
	if( m_bEnableRender == FALSE )
	{
		if(g_pEffectManager)
			g_pEffectManager->Move( dwTick );


		g_pSunRenderer->x_pManagerPostEffect->Apply();
		return TRUE;
	}

	//	임시 : 마우스 피킹을 위해 마우스 커서 레이를 얻어오는데,
	//	현재 업데이트 프레임에서 구해야 하는데( 초당 60 회 )
	//	여기 말고는 제대로 된 값이 얻어지지 않는다.	
	g_pSunRenderer->GetMouseCursorRay( &g_wrMouseRay );

    if (!g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_SYSTEM_VERIFY)
        && !g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_SYSTEM_CONFIRM)
		&& !g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_BATTLEZONE_LIST)
		&& !g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_BATTLEZONE_CREATE)
		&& !g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_BATTLE_LOBBY)
        && m_bDrawWait)
	{
		SceneBase::RenderLoadingImg();
		return TRUE;
	}

	SetBrightBlur();

	//-----------------------------------------------------------------------------------
	//		배경 필드 랜더링
	//-----------------------------------------------------------------------------------
	//	배경
	g_pSunRenderer->SetCullFaceMode( CF_DRAW_FRONT );

	SUNPROFILE_BEGIN( _T("MapRender") );
	g_pMap->Render(dwTick);
	SUNPROFILE_END( _T("MapRender") );

    SUNPROFILE_BEGIN( _T("Move Effect") );
    if(g_pEffectManager)
    {
        g_pEffectManager->Move( dwTick );
    }
    SUNPROFILE_END( _T("Move Effect") );

	SUNPROFILE_BEGIN( _T("RenderOthers") );
	//	임시 : 잔상 효과

	RenderClone();
	g_ProjectileManagerEx.Render();	

	g_InterfaceManager.RenderMovePointer(dwTick);   // 이동포인터 렌더링
	g_InterfaceManager.RenderAreaPointer(dwTick);   // 지역지정 포인터 렌더링
	g_InterfaceManager.RenderTargetPointer(dwTick); // attack포인터 렌더링
	g_InterfaceManager.RenderTargetEffect();	    // 타겟팅 이펙트

	SUNPROFILE_END( _T("RenderOthers") );
	
	SUNPROFILE_BEGIN( _T("ApplyPostEffect") );
	g_pSunRenderer->x_pManagerPostEffect->Apply();
	SUNPROFILE_END( _T("ApplyPostEffect") );

	RenderDamage(dwTick);                         

    SUNPROFILE_BEGIN( _T("RenderUnitText") );
    //이펙트와의 Alpha문제로 이펙트 렌더 이후로 호출 시점 조정
    g_pMap->RenderUnitInfo();
    SUNPROFILE_END( _T("RenderUnitText") );

    // 블라인드이펙트의 경우 RenderUnitInfo() 이후에 호출
    // 그렇지 않으면 캐릭터명을 통한 위치파악이 가능함
    RenderBlindEffect();

	//-----------------------------------------------------------------------------------
	//		임시 : 2D 택스트( 현재는 정보 출력만 하고 있다 )
	//-----------------------------------------------------------------------------------
	RenderText(dwTick );

	m_TextDisplayManager.Render();

    RenderMissionClearMessage();
	
	if (GENERALPARAM->GetSpecialMode()) 
    {
		if (g_bShowDebugInfo)
		{
			RenderMoveInfo();
		}

		RenderLightDirection();
	}
	
#ifdef _GS_ADD_BIGEVENT_MSG_1122
	if(PROGRAMPARAM->IsBigEvent())
	{
		g_InterfaceManager.ShowBigEventMsg();
	}
#endif//_GS_ADD_BIGEVENT_MSG_1122

    RenderDbgMonsterRange();

#ifdef _YMS_AUTOBOT_GAME
    Autobot::Instance()->Render_Checkpoint();
#endif

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void BattleScene::RenderDbgMonsterRange()
{
#ifdef _DEV_VER
    if (g_pHero && m_iDbgTextType == eDebugTextType_Monster)
    {
        Object *obj_ptr = g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());

        if (obj_ptr && obj_ptr->IsKindOfObject(MONSTER_OBJECT) == TRUE)
        {
            Monster* target_mon_ptr = static_cast<Monster*>(obj_ptr);
            BASE_NPCINFO* npc_info_ptr = target_mon_ptr->GetMonsterInfo();
            if(npc_info_ptr == NULL){return;}

            BvSphere bv;

            bv.m_fRadius = npc_info_ptr->m_fAttRange + npc_info_ptr->m_fBodyRadius;
            bv.m_wvCenter = target_mon_ptr->GetPosition();

            WzColor colorback = g_pSunRenderer->GetDiffuseColor();

            if (bv.m_fRadius > 0.0001f)
            {
                WzColor color_sph = WzColor_RGBA(255,0,0,0);
                g_pSunRenderer->SetColor(color_sph);
                g_pSunRenderer->RenderBvSphere(&bv);
            }

            bv.m_fRadius = target_mon_ptr->GetMonsterInfo()->m_fViewRange;
            if (bv.m_fRadius > 0.0001f)
            {
                WzColor color_sph = WzColor_RGBA(0,0,255,0);
                g_pSunRenderer->SetColor(color_sph);
                g_pSunRenderer->RenderBvSphere(&bv);
            }

            g_pSunRenderer->SetColor(colorback);
        }
    }
#endif //_DEV_VER
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::RenderText( DWORD dwTick )
{
	
	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

	if (m_pChatDlg)
	{
		if (g_InterfaceManager.GetShowFlag())
		{
			m_pChatDlg->RenderChatMsgWithoutDialog(dwTick);
		}
	}

	// 마무리
    g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessKeyInput()
{
    if (GlobalFunc::BeforeLoadingInputLock())
    {
        return;
    }

    if (g_Input.GetState(DIK_RETURN, KS_DOWN) || g_Input.GetState(DIK_NUMPADENTER, KS_DOWN))
    {
        ChatDialog* pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
        if (pDlg && !pDlg->IsAlwaysOnTop())
        {			  
            if (!pDlg->m_preShow && !pDlg->IsVisibleWindow())			
            {
                pDlg->ShowDialog(TRUE);
            }
            else
            {
                pDlg->m_preShow=FALSE;
            }
        }
    }

#ifndef _CHINA //#if'N'def
    //! 2011.5.3 / i4u4me / 중국 텟섭에서 동영상 찰영 하기 위해 
    //! 중국은 프리카메라 사용여부를 스페셜모드 체크 하지 않는다.
	if( GENERALPARAM->GetSpecialMode() )
#endif //_CHINA
	{
        //	동영상 촬영용 프리 카메라
		if( GENERALPARAM->IsFreeCameraMode() )
		{
			if(g_Input.GetState (DIK_LCONTROL, KS_KEY) && g_Input.GetState( DIK_F, KS_DOWN ) )
			{
				if(g_pHero)
				{
					g_Camera.MoveFrontBack( -9999 );
					m_FreeCameraVector = g_pHero->GetVisiblePos();
					BOOL	bState = GENERALPARAM->GetFreeCameraState() ^ TRUE;
					GENERALPARAM->SetFreeCameraState( bState );
				}
			}
		}
	}

#ifdef _NA_000000_20150604_BUG_FIND_TEST
    // 찬종 테스트
    if (g_Input.GetState( DIK_LCONTROL, KS_KEY ))
    {
        if( g_Input.GetState( DIK_F10, KS_DOWN) || g_Input.GetState( DIK_F10, IKS_DOWN ) )
        {
            if( uiQuestMan* pMan = GET_DIALOG_MANAGER( uiQuestMan, UIMAN_QUEST ) )
            {
                //pMan->SetQuestViewProgressRect(); //반응없음
                g_InterfaceManager.MoveDefaultWindowPos();
            }
        }
    }
#endif //_NA_000000_20150604_BUG_FIND_TEST

}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessKeyInput_HotKey()
{
    g_HeroInput.set_jump_key_pressed(false);

	if ( m_EventText.IsActived() )
	{
        HotKeyManager::Instance()->ProcessHotkeyFunction(kOpen_SystemMenu_N_Cancel);
        HotKeyManager::Instance()->ProcessHotkeyFunction(kMove_Forward);
        HotKeyManager::Instance()->ProcessHotkeyFunction(kMove_Backward);
        HotKeyManager::Instance()->ProcessHotkeyFunction(kMove_LeftSide);
        HotKeyManager::Instance()->ProcessHotkeyFunction(kMove_RightSide);
        HotKeyManager::Instance()->ProcessHotkeyFunction(kTurn_Left);
        HotKeyManager::Instance()->ProcessHotkeyFunction(kTurn_Right);
        HotKeyManager::Instance()->ProcessHotkeyFunction(kJump);
        HotKeyManager::Instance()->ProcessHotkeyFunction(kAutoRun);

		return;
	}

    HotKeyManager::Instance()->Process();
}


//------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessKeyInput_GM()
{
#ifdef _DEV_VER

	if ( g_Input.GetState(DIK_F2, KS_DOWN))
	{
		m_iDbgTextType++;
		if (m_iDbgTextType >= eDebugTextType_Max)
		{
			m_iDbgTextType = 0;
		}
	}

#ifdef _UI_VERTEXBUFFER_MOD
	if ( g_Input.GetState(DIK_F5, KS_DOWN))
	{
		g_pSunRenderer->TouggleMode();
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT, "TouggleMode : %d", g_pSunRenderer->GetVertexMode());
	}
#endif //_UI_VERTEXBUFFER_MOD

	if (g_Input.GetState( DIK_LCONTROL, KS_KEY ))
	{
        if(g_Input.GetState(DIK_Q, KS_DOWN))
        {
            if( false == g_pMap->m_bRenderWireTerrain)
                g_pMap->m_bRenderWireTerrain = true;
            else
                g_pMap->m_bRenderWireTerrain = false;
        }

		if(g_Input.GetState( DIK_P, KS_DOWN ))
		{
			if(g_pMap)
			{
				if(false == g_pMap->GetbRenderTile())
					g_pMap->SetbRenderTile(true);
				else
					g_pMap->SetbRenderTile(false);
			}
		}
	}
  
#ifdef _YJ_TEST_TILE_CONVEX
	if(g_Input.GetState(DIK_9, KS_DOWN))
	{
		CWzArchive WMOArchive;

		for(int i = 0; i < 70000; ++i)
		{
			bool bYes = true;

			const sWMOINFO* pWMO = MapInfoParser::Instance()->FindWMOInfo(i);
			if(pWMO)
			{
				WMOArchive.SetDiscAccess(g_pApplication->GetDiscAccess());
				if(FALSE == WMOArchive.LoadFile(pWMO->pszPath))
				{
					assert(!"!WMOArchive.LoadFile");
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "[%d]%s파일 없음", pWMO->Code, pWMO->pszPath);
					continue;
				}

				UINT uiHead = 0;
				WMOArchive.Read(&uiHead, 4);
				WORD wVersion = 0;
				WMOArchive >> wVersion;
				WMOArchive.SetVersion(wVersion);

				if ( WMOArchive.GetVersion() > 170 )
				{
					WORD wSecondVersion = 0;
					WMOArchive.Read(&wSecondVersion, sizeof(WORD));
					DWORD dwCheckSum = 0;
					WMOArchive.Read(&dwCheckSum, sizeof(DWORD));
				}

				St_ChunkInfo ci = WMOArchive.ReadChunk();
				WMOArchive.SkipCurrentChunk( ci);

				ci = WMOArchive.ReadChunk();
				int iNumPathVtx;
				WMOArchive >> iNumPathVtx;
				WzVector* wvTemp = new WzVector [iNumPathVtx];
				WMOArchive.Read(wvTemp, iNumPathVtx*sizeof(WzVector));
				int iNumPathTile;
				WMOArchive >> iNumPathTile;
				WORD wTemp;
				int iTemp;
				float fTemp;
				DWORD dwTemp;
				int iTemp2[3];
				BYTE btTemp2[3];
				WzVector wvTemp2;
				/**/
				int iConvex = -1;
				for(int j = 0; j < iNumPathTile; ++j)
				{
					WMOArchive >> wTemp;
					WMOArchive >> wTemp;
					for(int k = 0; k < wTemp; ++k)
					{
						WMOArchive >> iTemp;
						WMOArchive >> fTemp;
					}

					WMOArchive >> dwTemp;
					WMOArchive >> iConvex;

					WMOArchive.Read( iTemp2, 3 * sizeof ( int));
					WMOArchive >> wvTemp2;
					WMOArchive.Read( iTemp2, 3 * sizeof ( int));
					WMOArchive.Read( iTemp2, 3 * sizeof ( int));
					WMOArchive.Read( btTemp2, 3 * sizeof ( BYTE));

					if(-1 == iConvex)
					{
						bYes = false;
					}
				}
				
				if(false == bYes)
				{
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "잘못된 WMO:[%d]%s", pWMO->Code, pWMO->pszPath);
				}
			}
		}
	}
#endif //_YJ_TEST_TILE_CONVEX 

#endif //_DEV_VER

	if (GENERALPARAM->GetSpecialMode() == eSPECIAL_MODE_NONE) 
	{
		return;
	}

	if (g_Input.GetState( DIK_LCONTROL, KS_KEY ))
	{
		if( g_Input.GetState( DIK_F12, KS_DOWN) || g_Input.GetState( DIK_F12, IKS_DOWN ) )
		{
			g_pMap->SetUseAutoCamera(m_bUseAutoCamera ^= TRUE);
			if (g_pHero)
			{
				g_pMap->SetAutoCameraTargetObjectKey( g_pHero->GetObjectKey() );
			}
		}
	}


	if( GENERALPARAM->IsNet()==FALSE)
	{
		Character *pChr = (Character *)g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());

		if(pChr && m_dwOldSelectId != pChr->GetObjectKey())
		{
			g_TestSkillIndex=0;
			m_dwOldSelectId=pChr->GetObjectKey();
		}


		if(g_Input.GetState(DIK_B, KS_UP))
		{
			Character *pMon=(Character *)g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());
			
			if(pMon && pMon->IsKindOfObject(MONSTER_OBJECT))
			{
				// 몬스터도 큐큐큐!
			  	PLAYER_ACTION action;
				ZeroMemory(&action,sizeof(action));
				action.ActionID = ACTION_ATTACK;
				action.ATTACK.byAttackType =rand()%2;		// 몬스터는 특별히 attack type이 없다.			 
				action.ATTACK.dwTargetID =g_pHero->GetObjectKey();			
				action.ATTACK.vCurPos.wvPos = pMon->GetPosition(); 
				action.ATTACK.vCurPos.wTile = -1;
				action.ATTACK.vDestPos.wvPos = pMon->GetPosition();
				action.ATTACK.vDestPos.wTile = -1;
				action.ATTACK.dwAttackSerial = 0;
				action.ATTACK.bAttackMove = FALSE;
				action.ATTACK.dwAttackStyle = 9999;

				pMon->DoAction(&action);	
				pMon->CommitChangeState(g_CurTime);	

			}
		}

		PLAYER_ACTION action;
		ZeroMemory(&action,sizeof(action));

		action.SKILL.dwSkillID=0;


		if(g_Input.GetState(DIK_UP, KS_UP))
		{
			g_TestSkillIndex--;
		}

		if(g_Input.GetState(DIK_DOWN, KS_UP))
		{
			g_TestSkillIndex++;
		}

		if(g_TestSkillIndex>9)
		{
			g_TestSkillIndex=-3;
		}

		if(g_TestSkillIndex<-3)
		{
			g_TestSkillIndex=9;
		}

		if(pChr && pChr->IsKindOfObject(MONSTER_OBJECT))
		{
			Monster *pMon = (Monster *)pChr;

			if(g_TestSkillIndex==-1)
			{
				action.SKILL.dwSkillID=pMon->GetMonsterInfo()->m_wReviveCode;
			}
			else if(g_TestSkillIndex==-2)
			{
				action.SKILL.dwSkillID=pMon->GetMonsterInfo()->m_wHealCode;
			}
			else if(g_TestSkillIndex==-3)
			{
				action.SKILL.dwSkillID=pMon->GetMonsterInfo()->m_wSummonCode;
			}
			else 
			{
				action.SKILL.dwSkillID=pMon->GetMonsterInfo()->m_wSkillCode[g_TestSkillIndex];
			}

			char temp[512] = {0,};

			BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)action.SKILL.dwSkillID);

			if(pInfo)
			{
				Snprintf(temp, 512-1, _T("%d Code:%d Name:%s"),g_TestSkillIndex,pInfo->m_SkillClassCode,pInfo->m_szSkillName);
			}
			else
			{
				Snprintf(temp, 512-1, _T("%d Skill:%s"),g_TestSkillIndex,"스킬없음");				
			}

			g_InterfaceManager.ShowMessageBox(temp,1000,NULL,0);
		}

		if(g_Input.GetState(DIK_V, KS_UP))
		{
			if(action.SKILL.dwSkillID)
			{
				Monster *pMon=(Monster *)g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());

				if(pMon)
				{
					action.ActionID = ACTION_SKILL;				
					action.SKILL.dwTargetID = g_pHero->GetObjectKey();
					action.SKILL.dwAttackSerial = 0;
					action.SKILL.vCurPos.wvPos =pMon->GetPosition();					
					action.SKILL.vCurPos.wTile = -1;
					action.SKILL.vDestPos.wvPos =pMon->GetPosition();
					action.SKILL.vDestPos.wTile = -1;
					action.SKILL.vTargePos=g_pHero->GetVisiblePos();

					QueueSkillAction(&action,pMon);
			
				}	
			}
			else
			{
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,"사용할수 없는 스킬입니다.");
			}
		}
	}


	if (!GENERALPARAM->IsNet() &&g_Input.GetState(DIK_LCONTROL, KS_KEY))
	{
		if (g_Input.GetState(DIK_1, KS_DOWN))
		{
			Monster *pMon=GameFunc::SpawnMonster(27010	);

			if(pMon)
			{
				m_dwTestMonsterId=pMon->GetObjectKey();

				WzVector wzTemp=g_pHero->GetPosition();

				wzTemp+=g_pHero->GetDirection();

				pMon->SetPosition(wzTemp);

			}
		}

		if (g_Input.GetState(DIK_2, KS_DOWN))
		{
			Monster *pMon=GameFunc::SpawnMonster(6504);

			if(pMon)
			{
				m_dwTestMonsterId=pMon->GetObjectKey();
			}
		}

		if (g_Input.GetState(DIK_3, KS_DOWN))
		{
			Monster *pMon=GameFunc::SpawnMonster(6505);

			if(pMon)
			{
				m_dwTestMonsterId=pMon->GetObjectKey();
			}
		}

		if (g_Input.GetState(DIK_4, KS_DOWN))
		{
			Monster *pMon=GameFunc::SpawnMonster(6506);

			if(pMon)
			{
				m_dwTestMonsterId=pMon->GetObjectKey();
			}
		}

		if (g_Input.GetState(DIK_5, KS_DOWN))
		{
			Monster *pMon=GameFunc::SpawnMonster(1);

			if(pMon)
			{
				m_dwTestMonsterId=pMon->GetObjectKey();
			}
		}
	}


	if ( g_Input.GetState(DIK_SLASH, KS_DOWN) )
	{
		if( g_Input.GetState( DIK_LCONTROL, KS_KEY) )
		{
			static BOOL bFlag = FALSE;
			bFlag = !bFlag;
			MSG_CG_GM_OBSERVER_SYN SendPacket;
			SendPacket.m_byCategory = CG_GM;
			SendPacket.m_byProtocol = CG_GM_OBSERVER_SYN;
			SendPacket.m_byObserverOn = !bFlag;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

			if(g_pHero)
				g_pHero->SetShow(bFlag);
		}
	}
	

	if( g_Input.GetState( DIK_LCONTROL, KS_KEY ) )
	{

		if (g_Input.GetState( DIK_N, KS_DOWN ))
		{
		}
		else if (g_Input.GetState( DIK_G, KS_DOWN))
		{
			//bgm on/off
			static BOOL bBGMSound = FALSE;
			bBGMSound  = !bBGMSound ;
			g_SoundBGM.Stop();
			Sound::Instance()->SetUseBGMSound(bBGMSound );
		}
		else if (g_Input.GetState( DIK_E, KS_DOWN ))
		{
			//effect on/off
			static BOOL bEffectSound = FALSE;
			bEffectSound = !bEffectSound;
			Sound::Instance()->SetUseEffectSound(bEffectSound);
		}

	}


// 	if( g_Input.GetState( DIK_F8, KS_DOWN ) )
// 	{
// 		static int count = 0;
// 		char Temp[MAX_PATH + 1];
// 
// 		Sprintf( Temp, "%d %s", count++, "번 아이템을 습득하였습니다");
// 
// 		TEXT_MEMBER_SHARED_PTR_TYPE data( new Text_Member( Temp ) );
// 
// 		m_TextDisplayManager.Insert( data );	
// 	}
}

//------------------------------------------------------------------------------
/**
	주인공 인터페이스에 관해선 여기서 처리한다.
*/
void BattleScene::ProcessKeyInput_Hero(DWORD dwTick)
{
    if (GlobalFunc::BeforeLoadingInputLock())
    {
        g_HeroInput.Process(dwTick);
        return;
    }

    if (g_pHero)
	{
		if (g_pHero->GetCurState() != STATE::MOVE)
		{
			g_InterfaceManager.SetMovePointer(FALSE,NULL);
		}
	}

	g_HeroInput.Process(dwTick);
}

void BattleScene::ProcessKeyInput_Quick()
{
    if (g_Input.IsPressQuickExtKey())
    {
        return;
    }

    int quickslot_tab_number = 0;
    QuickContainer* quick_container = NULL;
    if (g_pHero == NULL)
    {
        return;
    }
    quick_container = g_pHero->GetQuickContainer();
    if (quick_container && quick_container->double_size())
    {
        quickslot_tab_number = quick_container->GetCurTabNum();
    }

    const int kQuickKeyNum = 10;
    const BYTE input_key[kQuickKeyNum] = {DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, 
                                          DIK_6, DIK_7, DIK_8, DIK_9, DIK_0};
    
    BOOL is_key_down = FALSE;
    int index = 0;
    for ( ; index < kQuickKeyNum; ++index)
    {
        is_key_down = g_Input.GetState(input_key[index], KS_DOWN);
        if (is_key_down)
        {
            if (g_Input.GetState(DIK_LALT, KS_KEY))
            {
                index += kQuickKeyNum;
            }
            //index += quickslot_tab_number;
            MouseHandler::Instance()->ItemUseTransaction(SI_QUICK, index);
            break;
        }
    }
}

void BattleScene::ProcessKeyInput_PartyMember(int index)
{
    SCENE_TYPE current_scene_type = GameFramework::GetCurrentScene()->GetType();
    switch (current_scene_type)
    {
    case SCENE_TYPE_VILLAGE:
    case SCENE_TYPE_FIELD:
    case SCENE_TYPE_MISSION:
    case SCENE_TYPE_PVP:
    case SCENE_TYPE_INSTANCE_DUNGEON:
    case SCENE_TYPE_CHUNTING:
    case SCENE_TYPE_SSQ:
    case SCENE_TYPE_DOMINATION:
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    case SCENE_TYPE_FREEPVP:
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    case SCENE_TYPE_BATTLEGROUND:
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    case SCENE_TYPE_GOLDRUSH:
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case SCENE_TYPE_SONNENSCHEIN:
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    case SCENE_TYPE_GUILDMISSION:
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        {
            // 음수는 자기자신
            if (index < 0)
            {
                if (g_pHero != NULL)
                {
                    g_HeroInput.SetCurrentTarget(g_pHero->GetObjectKey());
                }   
            }
            else
            {
                uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
                if (party_manager != NULL)
                {
                    party_manager->SetTargetingMember(index);                      
                }                    
            }
        }
        break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessUnits(DWORD dwTick)
{
#ifdef _K2P_OBJECT_MANAGER_MODIFIDY
	g_ObjectManager.ProcessUnits( dwTick );
#else
	
	g_dwCharProcessCount=0;
	float		fDistance = 0.f;
	Object_Map_Itr tempitr;
	Object_Map_Itr itr = g_ObjectManager.GetBegin();
	while(itr != g_ObjectManager.GetEnd() )
	{		
		Object * pObject = static_cast<Object *>(itr->second);
		if (0 == pObject)
		{
			assert(!"Object Is NULL!!");
			return;
		}
		
		BOOL bLive = itr->second->Process(dwTick);
		if (bLive)
		{
			itr++;
		}
		else
		{
			tempitr = itr;
			itr++;

			g_ObjectManager.Delete(tempitr->first);
		}
	}
#endif
}

//------------------------------------------------------------------------------
/**
	카메라 설정 : 히어로( 내 캐릭터 ) 중심으로 위치
*/
void BattleScene::ProcessCamera( LPDIMOUSESTATE2 pMouseState,DWORD dwTick)
{
	if (!g_pHero)
	{
		return;
	}

	//	동영상 촬영용 프리 카메라
	if( GENERALPARAM->IsFreeCameraMode() )
	{
		//	프리 카메라
		if( GENERALPARAM->GetFreeCameraState() )
		{
			if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_KEY) )
			{
				g_Camera.Rotate( pMouseState->lX, pMouseState->lY );
			}

			if( g_Input.GetState( DIK_RBRACKET, KS_DOWN ) )
			{
				g_fFreeCameraSpeed += FREE_CAMERA_SPEED_UP;
				if( g_fFreeCameraSpeed >= FREE_CAMERA_MAX_SPEED_UP )
				{
					g_fFreeCameraSpeed = FREE_CAMERA_MAX_SPEED_UP;
				}
			}
			else if( g_Input.GetState( DIK_LBRACKET, KS_DOWN ) )
			{
				g_fFreeCameraSpeed -= FREE_CAMERA_SPEED_UP;
				if( g_fFreeCameraSpeed <= FREE_CAMERA_MIN_SPEED_UP )
				{
					g_fFreeCameraSpeed = FREE_CAMERA_MIN_SPEED_UP;
				}
			}

			if( g_Input.GetState( DIK_W, KS_KEY ) )
			{
				m_FreeCameraVector -= (g_Camera.GetCameraDirection() * (GENERALPARAM->GetFreeCameraSpeed() * g_fFreeCameraSpeed) );
			}
			else if( g_Input.GetState( DIK_S, KS_KEY ) )
			{
				m_FreeCameraVector += (g_Camera.GetCameraDirection() * (GENERALPARAM->GetFreeCameraSpeed() * g_fFreeCameraSpeed) );
			}
			if(  g_Input.GetState( DIK_A, KS_KEY ) )
			{
				g_Camera.RotateYaw( GENERALPARAM->GetFreeCameraTurnSpeed() );
			}
			else if( g_Input.GetState( DIK_D, KS_KEY ) )
			{
				g_Camera.RotateYaw( -GENERALPARAM->GetFreeCameraTurnSpeed() );
			}
			if( g_Input.GetState( DIK_Q, KS_KEY ) )
			{
				WzVector RightVector;
				WzVector UpVector;
				SetVector( &UpVector, 0.0f, 0.0f, 1.0f );
				VectorCrossProduct( &RightVector, &g_Camera.GetCameraDirection(), &UpVector );
				VectorNormalize( &RightVector, &RightVector );
				m_FreeCameraVector += (RightVector * (GENERALPARAM->GetFreeCameraSpeed() * g_fFreeCameraSpeed) );
			}
			else if( g_Input.GetState( DIK_E, KS_KEY ) )
			{
				WzVector CameraDirection = g_Camera.GetCameraDirection();
				WzVector RightVector;
				WzVector UpVector;
				SetVector( &UpVector, 0.0f, 0.0f, 1.0f );
				VectorCrossProduct( &RightVector, &g_Camera.GetCameraDirection(), &UpVector );
				VectorNormalize( &RightVector, &RightVector );
				m_FreeCameraVector -= (RightVector * (GENERALPARAM->GetFreeCameraSpeed() * g_fFreeCameraSpeed) );
			}
			g_Camera.SetCamera( &(m_FreeCameraVector), FALSE,dwTick);
			g_Camera.UpdateCamera();


			return;
		}
	}

	WzVector vDirection = g_pHero->GetDirection();	
	g_Camera.SetAngle( &vDirection );
	

    // 주의: 하드코딩
    // 플레이어가 맵에 들어왔을때 한번만 실행하기 위한 코드
    // -> 플레이어가 바라보는 방향으로 카메라를 보정한다.
    if (m_bCameraDetech)
    {
        g_Camera.SetYaw(g_pHero->GetAngle());
        g_Camera.Rotate(0,-50);	
        m_bCameraDetech = false;
	}

    //	마우스로 카메라 회전
	if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_KEY) )
	{
        g_Camera.Rotate( pMouseState->lX, pMouseState->lY,Camera::CAMERA_ROTATE_LMOUSE );
	}
	else if( g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_KEY ) )
	{
		g_Camera.Rotate( pMouseState->lX, pMouseState->lY, Camera::CAMERA_ROTATE_RMOUSE );
	}
	else if (g_Input.GetState( MOUSE_MBUTTON, KS_MOUSE_DOWN ))
	{
		if( !m_bRenderLightDirection )
		{
#ifdef _JBH_MOD_MOUSE_MBUTTON_IN_BATTLESCENE
			g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom(), FALSE );
#else
			if( g_Input.IsPressCtrlKey() )
			{
				g_Camera.SetYaw( g_pHero->GetAngle() + WZ_PI );
			}
			else
			{
				g_Camera.SetYaw( g_pHero->GetAngle() );
			}
#endif	
			g_Camera.Rotate(0,0);	
		}
	}
	else
	{
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        if( g_pHero->GetCurState() == STATE::SKILL )
        {
            if( g_pHero->GetCurSkillState()->GetSkillClassCode() == eSKILL_HELLROID_1015 )
            {
                g_Camera.KeyboardModeInterpolateAngle(g_pHero->GetAngle());
            }
        }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID

        if( g_pHero->GetCurState() == STATE::KEYBOARDMOVE||g_pHero->GetCurState() == STATE::KEYBOARDJUMP) 
        {
            g_Camera.KeyboardModeInterpolateAngle(g_pHero->GetAngle());
        }
        else
        {
            if( g_pHero->IsMoving() )
            {
                // 일반 이동시 카메라 자동회전 온/오프
                if( GENERALPARAM->GetGetCameraRotaion() )
                {
                    g_Camera.InterpolateAngle();
                }
            }
        }

		g_Camera.Rotate(0,0);
		g_Camera.SetRotateState( FALSE );
		g_Camera.RotateAngleCheck( 0 );
		g_Camera.SetStartRotateCheck( FALSE );
	}


	WzVector vPos = g_pHero->GetVisiblePos();

	if ( g_bObserverMode == FALSE )
	{
		DWORD dwPlayerAtt = g_pHero->GetCurrentTileAtt(FALSE);

		if (BIT_CHECK(dwPlayerAtt, PTA_PLAYER_FALLDOWN_TILE))
		{
			WzVector vTempPos = g_pHero->GetPosition();
			vPos.z = max( vTempPos.z, vPos.z);
		}

		float default_eye_height = GlobalFunc::GetCameraHeight( g_pHero->GetClass() );

		//restore default eye height
		g_Camera.set_eye_height( default_eye_height );

        if ((g_pHero->GetCurState() == STATE::WINGS_SPREAD) || (g_pHero->GetCurState() == STATE::WINGS_FURL))
        {
            const float kFloorToBody_Length = 1.442f;

            float camera_eye_height = default_eye_height + (g_pHero->GetBodyPos().z - vPos.z - kFloorToBody_Length);

            g_Camera.set_eye_height(camera_eye_height);
        }
        else if (g_pHero->is_spread_wings())
        {
            g_Camera.set_eye_height(CGraphicParam::Instance()->camera_eye_height_wings_spread());
        }

        if (g_pHero->rider() != NULL) 
        {
            const float kFloorToLinkDummy_Stand_Length = 2.156f;

            float camera_eye_height = default_eye_height;
            if ((g_pHero->GetCurState() == STATE::RIDER_FURL) || (g_pHero->GetCurState() == STATE::RIDER_SPREAD))
            {
                camera_eye_height += (g_pHero->rider()->GetLinkPos()->z - vPos.z - kFloorToLinkDummy_Stand_Length);
            }
            else if (g_pHero->rider()->is_spread() == true)
            {
                camera_eye_height += (CGraphicParam::Instance()->camera_eye_height_rider_spread() - kFloorToLinkDummy_Stand_Length);
            }
            g_Camera.set_eye_height(camera_eye_height);
        }
	}
	

	g_Camera.SetCamera( &vPos, TRUE ,dwTick);	

	g_Camera.UpdateCamera();
	
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessEffect(DWORD dwTick)
{
     
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::SetFieldID(DWORD dwFieldID)
{
	m_dwFieldID = dwFieldID;
}


//------------------------------------------------------------------------------
/**
*/
void BattleScene::RenderDamage(DWORD dwTick)
{
	if (GENERALPARAM->GetScreenShotMode()) 
	{	
		return;
	}

	g_InterfaceManager.RenderDamageList(dwTick);
	
}

//------------------------------------------------------------------------------
/**
*/
DWORD BattleScene::AddBattleTimeSerial()
{
	DWORD ret = m_dwBattleTimeSerial;
	m_dwBattleTimeSerial++;
	return ret;
}

//------------------------------------------------------------------------------
/**
*/
DWORD BattleScene::GetBattleTimeSerial()
{
	return m_dwBattleTimeSerial;
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::RenderClone()
{
	Clone *pClone = NULL;
	Clone_Map_Itr itr = g_CloneManager.GetBegin();
	for (; itr != g_CloneManager.GetEnd(); itr++)
	{
		pClone = static_cast<Clone *>(itr->second);
		assert(pClone);				
		if(pClone)
		{
			pClone->Render();
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessTrigger()
{
	if (!g_pMap)	return;
	
	DWORD dwGroupID = 0;
	DWORD dwCategoryID = 0;
	TCHAR  szTriggerGroupName[MAX_TRIGGER_GROUPNAME_LENGTH] = {0,};

	g_pMap->GetTriggerGroupName(szTriggerGroupName);
	if('0' == szTriggerGroupName[0])
		return;
	if(NULL == m_pTriggerMan)
		return;

	if(FALSE == m_bTriggerInit)
	{
		TriggerGroupInfo* pTriggerGroupInfo = g_pMap->GetTriggerInfoMan()->FindTriggerGruopInfo(szTriggerGroupName);
		if(pTriggerGroupInfo)
		{
			//-----------------------------------//
			m_pTriggerMan->Init(*pTriggerGroupInfo);
			//-----------------------------------//
			m_bTriggerInit = TRUE;

			g_pMap->SetTriggerMan(m_pTriggerMan);
		}
		else 
		{
			g_pMap->SetTriggerMan(NULL);
		}
	}
	m_pTriggerMan->OnMsg(NULL);
}

//------------------------------------------------------------------------------
/**
*/
int BattleScene::GetCharacterHP(DWORD dwID)
{
	Character *pChr = (Character *)g_ObjectManager.GetObject(dwID);
	if (pChr)
	{
		return pChr->GetHP();
	}

	return 0;
}


void BattleScene::ProcessBlind(DWORD dwTick)
{
	bool isblind = false;
    if (g_pHero)
    {
		if( g_pHero->IsBlind() || g_pHero->IsBlind2() )
		{
			isblind = true;

			if( g_pHero->IsBlind() == TRUE )
				m_BlindType = BLIND_TYPE_BLACK;
			else if( g_pHero->IsBlind2() == TRUE )
				m_BlindType = BLIND_TYPE_WHITE;
		}
    }

	if( isblind == true )
    {
        //if( g_pHero->GetTargetObject() != NULL )
        //{
        //    g_pHero->SetNextState(STATE::IDLE,g_CurTime);
        //    g_pHero->SetTargetID(0);
        //}

        //g_HeroInput.SetCurrentTarget(0);

        m_iCurBlindRate += dwTick;

        if( m_BlindType == BLIND_TYPE_BLACK && m_iCurBlindRate >= BLACK_BLIND_TIME)
        {
            m_iCurBlindRate = BLACK_BLIND_TIME;
        }
        else if( m_BlindType == BLIND_TYPE_WHITE && m_iCurBlindRate >= WHITE_BLIND_TIME )
        {
            m_iCurBlindRate = WHITE_BLIND_TIME;
        }

    }
    else
    {
        if (m_iCurBlindRate)
        {
            m_iCurBlindRate -= dwTick;

            if (m_iCurBlindRate <= 0)
            {
                m_iCurBlindRate = 0;
				m_BlindType = BLIND_TYPE_NONE;

                g_pSunRenderer->SetLinearFog(
                    m_oldFogParameter.m_wcFogColor,
                    m_oldFogParameter.m_fFogStart,
                    m_oldFogParameter.m_fFogEnd,
                    m_oldFogParameter.m_fMaxFogRate );
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessSpecialEffect(DWORD dwTick)
{


	if (m_iBrightDelay > 0)
	{
		m_iBrightDelay -= dwTick;

		if (m_iBrightDelay <= 0)
		{
			m_iBrightDelay = 0;
			g_pSunRenderer->x_pManagerPostEffect->SetTechBlur(&m_oldBlur);
			g_pSunRenderer->SetLinearFog(m_oldFogParameter.m_wcFogColor,m_oldFogParameter.m_fFogStart,m_oldFogParameter.m_fFogEnd,m_oldFogParameter.m_fMaxFogRate);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessGrayScaleDelay(DWORD dwTick)
{
	if (m_iGrayScaleDelay > 0)
	{
		m_iGrayScaleDelay -= dwTick;

		if (m_iGrayScaleDelay <= 0)
		{
			m_iGrayScaleDelay = 0;			
			g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_GRAYSCALE);
		}
	}
}


//------------------------------------------------------------------------------
/**
*/
void BattleScene::SetBrightBlur()
{
	int maxlight = 1000;

	if (m_iBrightDelay > 0)
	{
		float fRate = (float)m_iBrightDelay / (float)maxlight;
		if (fRate >= 1.0f)
		{
			fRate = 1.0f;
		}

		if (fRate <= 0.0f)
		{
			fRate = 0.0f;
		}
		St_PE_BLUR blur = m_oldBlur;

		float fLumRate = 1.0f - fRate;
		blur.m_fMinLuminance = m_oldBlur.m_fMinLuminance * fLumRate;
		blur.m_fSelectHighLight = m_oldBlur.m_fSelectHighLight * (1.0f - fRate)  + 2.0f * fRate;
		blur.m_fFinalColorBlend = m_oldBlur.m_fFinalColorBlend * (1.0f - fRate)  + 2.0f * fRate;


		int iBlurR = Red_WzColor(blur.m_wcSelectColor) ;
		int iBlurG = Green_WzColor(blur.m_wcSelectColor) ;
		int iBlurB = Blue_WzColor(blur.m_wcSelectColor) ;

		int iNewBlurR = int( (255 * fRate ) + iBlurR * fLumRate );
		int iNewBlurG = int( (255 * fRate ) + iBlurG * fLumRate );
		int iNewBlurB = int( (255 * fRate ) + iBlurB * fLumRate );


		WzColor newblurcolor = WzColor_RGBA((int)(iBlurR*m_fBlur_weight),(int)(iBlurG *m_fBlur_weight),(int)(iBlurB *m_fBlur_weight),255);
		blur.m_wcSelectColor = newblurcolor;
		g_pSunRenderer->x_pManagerPostEffect->SetTechBlur(&blur);



		int iR = Red_WzColor(m_oldFogParameter.m_wcFogColor) ;
		int iG = Green_WzColor(m_oldFogParameter.m_wcFogColor);
		int iB = Blue_WzColor(m_oldFogParameter.m_wcFogColor);

	
		int iNewR = int( (255 * fRate) + iR * fLumRate );
		int iNewG = int( (255 * fRate) + iG * fLumRate );
		int iNewB = int( (255 * fRate) + iB * fLumRate );

		float fStart = m_oldFogParameter.m_fFogStart * fLumRate;
		float fEnd = 1.0f * fRate + m_oldFogParameter.m_fFogEnd * fLumRate;

		WzColor newfog;

		newfog = WzColor_RGBA((int)(iNewR * m_fFog_weight),(int)(iNewG * m_fFog_weight),(int)(iNewB * m_fFog_weight) ,255);
		g_pSunRenderer->SetLinearFog(newfog,fStart,fEnd,m_oldFogParameter.m_fMaxFogRate);
		
	}

}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::SetBrightDelay(int delay)
{
	if (m_iBrightDelay <= 0 )
	{
		m_oldFogParameter = g_pSunRenderer->GetFogParameter();
		m_iBrightDelay = delay;
	}	
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::SetGrayScaleDelay(int delay,WzColor color)
{
	m_iGrayScaleDelay = delay;
	m_GrayScaleColor = color;
}



//------------------------------------------------------------------------------
/** 
    마을로 돌아가는 루틴
*/
void BattleScene::BackToTheVillage(CODETYPE VillageCode /*= 0*/)
{
	if (GENERALPARAM->IsNet())
	{
		assert(g_pHero);

		if (!g_pHero)
        {
			return;
        }

		BattleScene::SetRoomKey(0);
        
		SCENE_TYPE eBackToScene = SCENE_TYPE_VILLAGE;

        MAPCODE MapCode;;
		if (0 == VillageCode)
		{
            MapCode = g_pHero->GetCharInfo()->m_iRegion;
			
			//GS_BUG_FIX:마을로(필드,마을)돌아갈곳을체크
			const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(g_pHero->GetCharInfo()->m_iRegion);
			
			if (pMapInfo && (pMapInfo->byMKind == eZONETYPE_FIELD))
            {
				eBackToScene = SCENE_TYPE_FIELD;
            }
		}
		else
		{
            MapCode = (MAPCODE)VillageCode;
        }
			
		g_LoadScene.SetNextSceneAfterLoaded(eBackToScene, MapCode, 0);
		GameFramework::ChangeScene(SCENE_TYPE_LOAD);

	}
	else
	{
		g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT, GENERALPARAM->GetCharSelectMapID(), 0);
		GameFramework::ChangeScene(SCENE_TYPE_LOAD);
	}

	uiBattleScoreMan* battle_score_manager_ptr 
        = static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
	if (battle_score_manager_ptr)
	{
		battle_score_manager_ptr->Reset();
	}
}

//----------------------------------------------------------------------------
/**
*/
void				
BattleScene::BackToTheVillageSyn()
{
    if (BattleScene::m_bPendingBackToSomeWhere)
        return;

	SystemDialog * pSystemDialog = (SystemDialog * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SYSTEM );

    switch (GameFramework::GetCurScene() )
    {
    case SCENE_TYPE_MISSION:
        {
            BattleScene::m_bPendingBackToSomeWhere = true;

			if ( pSystemDialog )
			{
				pSystemDialog->AddPacketStatus(SystemDialog::BACK_TO_THE_VILLAGE);
			}

            MSG_CG_ZONE_MISSION_LEAVE_SYN SendPacket;


			if(g_MissionScene.IsLeavePartyBan())	// 강퇴
				SendPacket.m_Reason = MSG_CG_ZONE_MISSION_LEAVE_SYN::REASON_EXPULSION;
			else	// 스스로 나간것
				SendPacket.m_Reason = MSG_CG_ZONE_MISSION_LEAVE_SYN::REASON_INTEND;


            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;

    case SCENE_TYPE_LOBBY:
        {
            BattleScene::m_bPendingBackToSomeWhere = true;

            if ( pSystemDialog )
			{
				pSystemDialog->AddPacketStatus(SystemDialog::BACK_TO_THE_VILLAGE);
			}

            MSG_CG_ZONE_LOBBY_LEAVE_SYN SendPacket;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;

	case SCENE_TYPE_VILLAGE:
		{
            BattleScene::m_bPendingBackToSomeWhere = true;

			if(BattleScene::IsLobby())
			{
				if ( pSystemDialog )
				{
					pSystemDialog->AddPacketStatus(SystemDialog::BACK_TO_THE_VILLAGE);
				}

				MSG_CG_ZONE_LOBBY_LEAVE_SYN	SendPacket;
				GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
			}
		}
		break;

    case SCENE_TYPE_PVP:
        {
            BattleScene::m_bPendingBackToSomeWhere = true;

            if ( pSystemDialog )
			{
				pSystemDialog->AddPacketStatus(SystemDialog::BACK_TO_THE_VILLAGE);
			}

            MSG_CG_ZONE_PVP_LEAVE_SYN SendPacket;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;


	case SCENE_TYPE_INSTANCE_DUNGEON:
		{
			BattleScene::m_bPendingBackToSomeWhere = true;

			if ( pSystemDialog )
			{
				pSystemDialog->AddPacketStatus(SystemDialog::BACK_TO_THE_VILLAGE);
			}

			MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_SYN SendPacket;

			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
		}
		break;



	case SCENE_TYPE_FIELD:
		{
            BattleScene::m_bPendingBackToSomeWhere = true;

            if ( pSystemDialog )
			{
				pSystemDialog->AddPacketStatus(SystemDialog::BACK_TO_THE_VILLAGE);
			}

            MSG_CG_ZONE_RETURN_VILLAGE_SYN SendPacket;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;

    case SCENE_TYPE_CHUNTING:
        {
            BattleScene::m_bPendingBackToSomeWhere = true;

            if ( pSystemDialog )
            {
                pSystemDialog->AddPacketStatus(SystemDialog::BACK_TO_THE_VILLAGE);
            }

            MSG_CG_ZONE_CHUNTING_LEAVE_SYN SendPacket;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;

    case SCENE_TYPE_BATTLEGROUND:
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    case SCENE_TYPE_GOLDRUSH:
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    case SCENE_TYPE_FREEPVP:
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case SCENE_TYPE_SONNENSCHEIN:
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    case SCENE_TYPE_GUILDMISSION:
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        {
            BattleScene::m_bPendingBackToSomeWhere = true;

            if ( pSystemDialog )
            {
                pSystemDialog->AddPacketStatus(SystemDialog::BACK_TO_THE_VILLAGE);
            }

            MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_SYN SendPacket;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    case SCENE_TYPE_SPA:
        {
            BattleScene::m_bPendingBackToSomeWhere = true;

            if ( pSystemDialog )
            {
                pSystemDialog->AddPacketStatus(SystemDialog::BACK_TO_THE_VILLAGE);
            }

            MSG_CG_ZONE_EX_SPA_LEAVE_SYN SendPacket;

            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
        }
        break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void BattleScene::BackToTheVillageTransact()
{
    SCENE_TYPE current_scene_type = GameFramework::GetCurScene();
    // 타락한 사원에서는 마을로 돌아가기를 지원하지 않음
    if (current_scene_type == SCENE_TYPE_SSQ)
    {
        return;
    }

#ifdef _NA_000000_20120527_CHAOS_ZONE
    if ((current_scene_type == SCENE_TYPE_BATTLEGROUND) ||
        (current_scene_type == SCENE_TYPE_FREEPVP)
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        || (current_scene_type == SCENE_TYPE_GOLDRUSH)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
        || (current_scene_type == SCENE_TYPE_SONNENSCHEIN)
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        || (current_scene_type == SCENE_TYPE_GUILDMISSION)
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        )
    {
        // 카오스존 나가기
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType		= eDoType_Delay_Leave_ChaosZone;
        msg.DoSomething	= GlobalFunc::DoLeaveFieldCancel;
        msg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
        msg.wParam		= eDoType_Delay_Leave_ChaosZone;
        g_KeyQueueManager.PushBack(msg);

        // 딜레이 캐스트 창을 띄우고 타이머를 세팅한다.
        StartLeaveFieldCastTimer(eDoType_Delay_Leave_ChaosZone, DELAY_LEAVE_FIELD_TICK);
        return;
    }
#endif //_NA_000000_20120527_CHAOS_ZONE

	if (g_pHero && g_pHero->IsDead())
	{
		// 죽은상태에서는 무조건 나가게 해주자.
		BackToTheVillageSyn();
		return;
	}

    if ((current_scene_type == SCENE_TYPE_VILLAGE) ||
		(current_scene_type == SCENE_TYPE_FIELD))
    {
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(eST_U_ARE_IN_VILLAGE_OR_FIELD,szMessage,INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, szMessage);
		return;
	}

	if( GENERALPARAM->IsNet()==FALSE)
	{
        xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
		g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_VILLAGE, GENERALPARAM->GetStartMapID(), GENERALPARAM->GetStartFieldID() );
		GameFramework::ChangeScene( SCENE_TYPE_LOAD );

	}

	keyMsg msg;
	ZeroMemory(&msg,sizeof(msg));
	msg.dwType		= eDoType_Delay_Leave_Field;
	msg.DoSomething	= GlobalFunc::DoLeaveFieldCancel;
	msg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
	msg.wParam		= eDoType_Delay_Leave_Field;
	g_KeyQueueManager.PushBack(msg);

	// 딜레이 캐스트 창을 띄우고 타이머를 세팅한다.
	StartLeaveFieldCastTimer( eDoType_Delay_Leave_Field, DELAY_LEAVE_FIELD_TICK );
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::LeaveDominationTransact()
{
	if (g_pHero && g_pHero->IsDead())
	{
		// 죽은상태에서는 무조건 나가게 해주자.
		//uiDominationTowerMan::SEND_LEAVE_DOMINATION();
        
        // 2011.08.09 / 송찬종 / 리스폰 캐스팅 바와 마을로 가기 캐스팅 바가 곂치면서 다이얼로그 락이 안풀리는 현상 수정
        // 3464 죽은 상태에서는 지상으로 내려갈 수 없습니다.
        //#define _NA_000000_121015_LEAVE_CANCEL_TO_REVIVE_RE_CASTING
        TCHAR message[INTERFACE_STRING_LENGTH];
        g_InterfaceManager.GetInterfaceString(3464, message, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, message);
		return;
	}

	if ( SCENE_TYPE_VILLAGE == GameFramework::GetCurScene() 
		|| SCENE_TYPE_FIELD  == GameFramework::GetCurScene() )
	{		
		return;
	}

	if( GENERALPARAM->IsNet()==FALSE)
	{
        xGetHeroData()->SetObjectKey( INVALID_OBJECT_KEY );
		g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_VILLAGE, GENERALPARAM->GetStartMapID(), GENERALPARAM->GetStartFieldID());
		GameFramework::ChangeScene( SCENE_TYPE_LOAD );
	}

	keyMsg msg;
	ZeroMemory(&msg,sizeof(msg));
	msg.dwType		= eDoType_Delay_Leave_Domination;
	msg.DoSomething	= GlobalFunc::DoLeaveFieldCancel;
	msg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
	msg.wParam		= eDoType_Delay_Leave_Domination;
	g_KeyQueueManager.PushBack(msg);

	// 딜레이 캐스트 창을 띄우고 타이머를 세팅한다.
	StartLeaveFieldCastTimer( eDoType_Delay_Leave_Domination, DELAY_LEAVE_FIELD_TICK );
}

//------------------------------------------------------------------------------
/**
    미션 클리어 메시지를 출력한다.
*/
void BattleScene::RenderMissionClearMessage()
{
	if (m_dwMissionClearTick != 0)
	{
		// 5초 동안 렌더한다.
		
		g_pSunRenderer->Set2DMode( TRUE, FALSE );
		g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA );
		g_pSunRenderer->SetZBufferTest(FALSE);
		g_pSunRenderer->SetColor(255,255,255,255);
		g_pSunRenderer->RenderTexture(m_hMissionClearTexture,257.0f,0.0f,491.0f,84.0f);
		g_pSunRenderer->Set2DMode( FALSE );
		g_pSunRenderer->SetZBufferTest(TRUE);

		if (m_dwMissionClearTick + 10000 < g_pSunRenderer->x_Clock.GetCurrentTick() )
		{
			m_dwMissionClearTick = 0;
		}
	}

}


//------------------------------------------------------------------------------
/**
    최초의 renderstate로 돌려준다.
*/
void BattleScene::RestoreRenderState()
{
	g_pMap->EnableBlur(FALSE);
	g_pSunRenderer->x_pManagerPostEffect->SetTechBlur(&m_oldBlur);
	g_pSunRenderer->SetLinearFog(m_oldFogParameter.m_wcFogColor,m_oldFogParameter.m_fFogStart,m_oldFogParameter.m_fFogEnd,m_oldFogParameter.m_fMaxFogRate);
}

//------------------------------------------------------------------------------
/**
*/
void  BattleScene::SetFogParam(BYTE bR,BYTE bG,BYTE bB,float fFogStart,float fFogEnd,float fFogRate)
{
	WzColor color;
	color = WzColor_RGBA(bR,bG,bB,255);

	m_oldFogParameter.m_wcFogColor = color;
	m_oldFogParameter.m_fFogStart = fFogStart;
	m_oldFogParameter.m_fFogEnd = fFogEnd;
	m_oldFogParameter.m_fMaxFogRate = fFogRate;
	g_pSunRenderer->SetLinearFog(m_oldFogParameter.m_wcFogColor,m_oldFogParameter.m_fFogStart,m_oldFogParameter.m_fFogEnd,m_oldFogParameter.m_fMaxFogRate);
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ReleaseInterfaceResource()
{

    // 목표지점 표시 데칼을 보이지 않게 한다.
	g_InterfaceManager.SetMovePointer(FALSE,NULL);

	if (m_hTexture != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hTexture );
		m_hTexture = INVALID_HANDLE_VALUE;
	}

	if (m_hMissionClearTexture != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture( m_hMissionClearTexture  );
		m_hMissionClearTexture = INVALID_HANDLE_VALUE;
	}
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::ReleaseObjects()
{
	g_CloneManager.Clear();

#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.DeleteAllObjectWithoutHero();
#else
	g_ObjectManager.Destroy();
    uiBattlezone2Man* man = static_cast<uiBattlezone2Man*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_ZONE_MANAGER));
    if (man != NULL)
    {
        man->ClearObjectKey();
    }
#endif
}



//------------------------------------------------------------------------------
/**
*/
void BattleScene::_renderDbgInfoRenderStates(int& x, int& y)
{
#ifdef _DEV_VER

	if(NULL == g_pMap)
		return;
	// FPS
	DWORD dwTick;
	DWORD dwTicks2;
	DWORD dwTick3;
	LONG lCount, lPpsCount, lDcpsCount;
	g_pSunRenderer->GetFpsValues( &dwTick, &lCount );
	float fFps = ( float )lCount * 1000.0f / ( float )dwTick;

	g_pSunRenderer->GetPpsValues( &dwTicks2, &lPpsCount);
	g_pSunRenderer->GetDcpsValues( &dwTick3, &lDcpsCount);
	LONG lPolygon = lCount > 0 ? lPpsCount / lCount : 0;
	LONG lDrawCall = lCount > 0 ? lDcpsCount / lCount : 0;

	TCHAR lpszText[512] = {0,};
	
    SYSTEMTIME time;
    util::TimeSync::GetLocalTime(&time);
    Snprintf(lpszText, 512-1, _T("%4d-%2d-%2d  %2d:%2d:%2d"), time.wYear, time.wMonth, time.wDay,
             time.wHour, time.wMinute, time.wSecond);
    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
    y += 20;

	Snprintf( lpszText, 512-1, _T(" %f fps [ %ld polygons, %ld drawcalls(%ld terrain drawcalls) ]"), 
		fFps, lPolygon, lDrawCall, g_pMap->GetWorldBase()->terrain_drawcalls());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;
	
	Snprintf( lpszText, 512-1, _T(" VRam %ld/%ld(M) "), (g_pSunRenderer->GetLocalVidMem() - g_pSunRenderer->GetAvailableVidMem()) / (1024*1024), g_pSunRenderer->GetLocalVidMem() / (1024*1024));
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("Available Texture memory: %ld(M)"),g_pSunRenderer->GetAvailableTextureMem() / (1024*1024));
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	MEMORYSTATUS MemStatus;
	MemStatus.dwLength = sizeof(MemStatus);
	GlobalMemoryStatus(&MemStatus);
	Snprintf( lpszText, 512-1, _T(" SRam %ld/%ld(M) "), (MemStatus.dwTotalPhys - MemStatus.dwAvailPhys) / (1024*1024), MemStatus.dwTotalPhys / (1024*1024));
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T(" Near : %f Far : %f "),g_Camera.GetNearClip(),g_Camera.GetFarClip());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("ProcessCount : %d"),g_dwCharProcessCount);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("RenderCount : %d"),g_dwCharRenderCount);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	//////////////////////////////////////////////////////////////////////////
	y += 20;
	Snprintf( lpszText, 512-1, _T("VisMapObjCount : %d"),g_pMap->m_iVisMapObjCnt);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
	y += 20;
	Snprintf( lpszText, 512-1, _T("VisMonCount : %d"),g_pMap->m_iVisMonCnt);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
	y += 20;
	Snprintf( lpszText, 512-1, _T("VisPlayerCount : %d"),g_pMap->m_iVisPlayerCnt);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
	y += 20;
	Snprintf( lpszText, 512-1, _T("VisNPCCount : %d"),g_pMap->m_iNPCCnt);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
    y += 20;
    Snprintf( lpszText, 512-1, _T("VisGrassCount : %d"),g_pMap->grass_cnt_);
    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
	y += 20;
	Snprintf( lpszText, 512-1, _T("OctNode(Object) : %d"), g_pMap->GetWorldBase()->x_pOctree->GetNodeCount());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
	y += 20;
    Snprintf( lpszText, 512-1, _T("OctNode(Terrain) : %d"), g_pMap->GetWorldBase()->x_pTerrainOctree->GetNodeCount());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
	y += 20;
    Snprintf( lpszText, 512-1, _T("Terrain PolyCnt : %d / %d"), g_pMap->rendering_terrain_cnt_, g_pMap->GetWorldBase()->GetTerrainMeshCount());
    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
    y += 20;
    Snprintf( lpszText, 512-1, _T("Total Terrain Texture Count : %d"), g_pMap->GetWorldBase()->GetTextureCount());
    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
    y += 20;

	if(g_pEffectManager)
	{
#ifdef LAST_RELEASE
        // no operations
#else
        // 생성 이펙트 수 / 시뮬레이션 안 한 이펙트 수
        Snprintf( lpszText, 512-1, _T("Skipped/Created effects : %d / %d"), 
            g_effDbgInfo.numSkippedEffect, g_pEffectManager->GetNumCreatedEffects() );
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
		y += 20;

        // 렌더링된 이펙트 수
        Snprintf( lpszText, 512-1, _T("Rendered effects : %d ( %d )"), 
            g_effDbgInfo.numRenderedEffect, g_effDbgInfo.maxNumRenderedEffect );
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
		y += 20;

        // 렌더링된 엘리멘트 수
        Snprintf( lpszText, 512-1, _T("Rendered Elements : %d ( %d )"), 
            g_effDbgInfo.numRenderedSprite + g_effDbgInfo.numRenderedMesh + g_effDbgInfo.numRenderedChain + 
            g_effDbgInfo.numRenderedFlatChain + g_effDbgInfo.numRenderedDecal + g_effDbgInfo.numRenderedWeather , 
            g_effDbgInfo.maxNumRenderedSprite + g_effDbgInfo.maxNumRenderedMesh + g_effDbgInfo.maxNumRenderedChain +
            g_effDbgInfo.maxNumRenderedFlatChain + g_effDbgInfo.maxNumRenderedDecal + g_effDbgInfo.maxNumRenderedWeather);

		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
		y += 20;
#endif // LAST_RELEASE

	}

	Snprintf( lpszText, 512-1, _T("SceneType: %d / MAPCode:%d / WMOCode:%d"), GameFramework::GetCurrentScene()->GetType(), g_pMap->GetCurrentFieldID(), g_pMap->GetCurrentWMOCode());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 255 ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("VisibleRange: %f, AlphaRange: %f"), g_pMap->GetVisibleRange(), g_pMap->GetAlphaRange());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText,x, y, WzColor_RGB( 255, 255, 255 ) );
	y += 20;
	//////////////////////////////////////////////////////////////////////////


    ////------------------------------------------------------------------------------ 
    ////_NA000000_SPEED_HACK_PREVENTION_SUPPORT
    //if (g_pHero != NULL)
    //{
    //    x = 400;
    //    y = 600;

    //    Snprintf(lpszText, 256-1, _T("kbmove_skip_count = %d"), g_pHero->KBMove.debug_kbmove_skip_count);
    //    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
    //    y += 20;

    //    Snprintf(lpszText, 256-1, _T("hero speed = %f"), g_pHero->KBMove.debug_move_speed);
    //    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
    //    y += 20;

    //    Snprintf(lpszText, 256-1, _T("kbmove_gab = %f"), g_pHero->KBMove.debug_kbmove_gab);
    //    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
    //    y += 20;
    //}
#endif //#ifdef _DEV_VER
}

//------------------------------------------------------------------------------
/**
*/
static const char *GetActionString(PLAYER_ACTION *pAction)
{
	 //	디버그용 출력 메시지
	switch(pAction->ActionID)
	{
	case ACTION_IDLE:
		return "ACTION_IDLE";
	case ACTION_ATTACK:		
		return "ACTION_ATTACK";
	case ACTION_SKILL:		
		return "ACTION_SKILL";
	case ACTION_MOVE:		
		return "ACTION_MOVE";
	case ACTION_KEYBOARDMOVE:
		return "ACTION_KEYBOARDMOVE";
	case ACTION_KNOCKBACK:	
		return "ACTION_KNOCKBACK";
	case ACTION_TOAIR:		
		return "ACTION_TOAIR";
	case ACTION_DOWN:		
		return "ACTION_DOWN";
	case ACTION_GETUP:		
		return "ACTION_GETUP";
	case ACTION_NPCMEET:		
		return "ACTION_NPCMEET";
	case ACTION_GETITEM:		
		return "ACTION_GETITEM";
	case ACTION_JUMP:		
		return "ACTION_JUMP";
	case ACTION_EVENTJUMP:    
		return "ACTION_EVENTJUMP";
	case ACTION_STOP:         
		return "ACTION_STOP";
	case ACTION_VALKYRIESHOT: 
		return "ACTION_VALKYRIESHOT";
	case ACTION_PLAYERMEET: 
		return "ACTION_PLAYERMEET";
	case ACTION_SIT: 
		return "ACTION_SIT";
    case ACTION_ATTACKJUMP:
        return "ACTION_ATTACKJUMP";

	default :
		return "INVALID_ACTION";
	}
}

void BattleScene::_renderDbgInfoMonsterInfo(int& x, int& y)
{
#ifdef _DEV_VER
	Object *pObject = g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());
	if (pObject && pObject->IsKindOfObject(CHARACTER_OBJECT))
	{
		Character *pChar = (Character*)pObject;

		TCHAR lpszText[256] = {0,};
		int		x = 400;
		int		y = 80;
		int		Line = 18;

		Snprintf( lpszText, 256-1, _T("ObjectKey: %d"),pChar->GetObjectKey());
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
		y += Line;

		for( size_t i = 0 ; i < pChar->GetPartCount() ; ++i )
		{
			if ( CWzUnitDraw* pPart = pChar->GetPart(i) )
			{	
				Snprintf( lpszText, 256-1, _T("LOD[%d] Lv: %d"), i, pPart->GetWzdLOD());

				g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
				y += Line;
			}
		}

		St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
		if(pCameraInfo)
		{
			WzVector wvLen = pCameraInfo->m_wvFrom - pChar->GetVisiblePos();
			float fLen = VectorLength(&wvLen);
			Snprintf( lpszText, 256-1, _T("CamToChar Len: %f"), fLen);
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
			y += Line;
		}

		Snprintf( lpszText, 256-1, _T("HeroToChar Z: %f"), pChar->GetVisiblePos().z - g_pHero->GetVisiblePos().z);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
		y += Line;

		PLAYER_ACTION CurAction		= *pChar->GetCurrentAction();
		Snprintf( lpszText, 256-1, _T("Current Action : %s") , GetActionString(&CurAction) );
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
		y += Line;
		
		if(pChar->GetNextAction())
		{
			PLAYER_ACTION NextAction	= *pChar->GetNextAction();
			Snprintf( lpszText, 256-1, _T("Queue Action : %s") , GetActionString(&NextAction) );
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ));
			y += Line;
		}
		else
		{
			Snprintf( lpszText, 256-1, _T("Queue Action : ACTION_NULL") );
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
			y += Line;
		}
		
		Snprintf( lpszText, 256-1, _T("ActionQueueCount: %d"),pChar->GetActionQueueCount());
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
		y += Line;
	
		
		WzVector vVisiblePos;
		vVisiblePos=pChar->GetVisiblePos();
		
		Snprintf( lpszText, 256-1, _T("VisiblePos: %.2f, %.2f, %.2f"),vVisiblePos.x,vVisiblePos.y,vVisiblePos.z);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
		y += Line;

		
		WzVector vPos;
        vPos= pChar->GetPosition();
		Snprintf( lpszText, 256-1, _T("LogicPos: %.2f, %.2f, %.2f"),vPos.x,vPos.y,vPos.z);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
		y += Line;


		if( CWzUnitDraw* pBasePart = pChar->GetBasePart())
		{
			WzVector vMeshPos = pBasePart->GetPosition();

			Snprintf( lpszText, 256-1, _T("MeshPos: %.2f, %.2f, %.2f"),vMeshPos.x,vMeshPos.y,vMeshPos.z);
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
			y += Line;
		}
		else
		{
			Snprintf( lpszText, 256-1, _T("랜더파트없음"));
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
			y += Line;

		}

		BYTE bAlpha;
		bAlpha=Alpha_WzColor(pChar->GetColor());

		Snprintf( lpszText, 256-1, _T("Alpha:%d"),bAlpha);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0  ) );
		y += Line;

		BOOL bShow;
		bShow=pChar->GetShow();

		Snprintf( lpszText, 256-1, _T("Show:%d"),bShow);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
		y += Line;


		BOOL bObserver;
		bObserver=pChar->IsObserverMode();

		Snprintf( lpszText, 256-1, _T("bObserver:%d"),bObserver);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
		y += Line;

		


		BOOL CompletelyLoaded;

		CompletelyLoaded=pChar->IsCompletelyLoaded();

		Snprintf( lpszText, 256-1, _T("CompletelyLoaded:%d"),CompletelyLoaded);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
		y += Line;

		

		BOOL bCurShowFlag;
		bCurShowFlag=pChar->GetCurShowFlag();

		Snprintf( lpszText, 256-1, _T("CurShowFlag:%d"),bCurShowFlag);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0  ) );
		y += Line;


		TCHAR ani_code[5] = {0,};
        WzAnimationInfo *ani_info_ptr = pChar->GetAnimationInfo(pChar->GetCurrentAnimation());
        if (ani_info_ptr)
        {
            WzIDToString(ani_code, pChar->GetCurrentAnimation());
            int attack_speed = static_cast<int>((((ani_info_ptr->m_nMaxFrame/30.f) * 1000.f) + 1000.f));
            Snprintf( lpszText, 256-1, _T("AniCode: %s(AttSpeed: %d)"), ani_code, attack_speed);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0 ) );
            y += Line;
        }

		int  iTile;
		iTile=pChar->GetPathExplorer()->GetTile();

		Snprintf( lpszText, 256-1, _T("Tile:%d"),iTile);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0  ) );
		y += Line;


		DWORD dwAtt=0;

		if(g_pMap)
		{
			if(iTile>=0)
				dwAtt=g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iTile); 
		}


		Snprintf( lpszText, 256-1, _T("TileAtt:%o"),dwAtt);
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 255, 255, 0  ) );
		y += Line;

		if(BIT_CHECK(dwAtt,PTA_CONFLICT_TILE))
		{
			g_pSunRenderer->x_pManagerTextOut->DrawText( "PTA_CONFLICT_TILE", x, y, WzColor_RGB( 255, 0, 0  ) );
			y += Line;
		}


		if(BIT_CHECK(dwAtt,PTA_FREE_CONFLICT_TILE  ))
		{
			g_pSunRenderer->x_pManagerTextOut->DrawText( "PTA_FREE_CONFLICT_TILE", x, y, WzColor_RGB( 255, 0, 0  ) );
			y += Line;

		}

		if(BIT_CHECK(dwAtt,PTA_PK_TILE))
		{
			g_pSunRenderer->x_pManagerTextOut->DrawText( "PTA_PK_TILE", x, y, WzColor_RGB( 255, 0, 0  ) );
			y += Line;
		}

		if(BIT_CHECK(dwAtt,PTA_NO_WALK))
		{
			g_pSunRenderer->x_pManagerTextOut->DrawText( "PTA_NO_WALK", x, y, WzColor_RGB( 255, 0, 0  ) );
			y += Line;
		}

        if(BIT_CHECK(dwAtt,PTA_DOMINATION_WAR_TILE))
        {
            g_pSunRenderer->x_pManagerTextOut->DrawText( "PTA_DOMINATION_WAR_TILE", x, y, WzColor_RGB( 255, 0, 0  ) );
            y += Line;
        }

        if(BIT_CHECK(dwAtt,PTA_PLAYER_JUMP_TILE))
        {
            g_pSunRenderer->x_pManagerTextOut->DrawText( "PTA_PLAYER_JUMP_TILE", x, y, WzColor_RGB( 255, 0, 0  ) );
            y += Line;
        }

        if(BIT_CHECK(dwAtt,PTA_JUMP_CONTROL_A))
        {
            g_pSunRenderer->x_pManagerTextOut->DrawText( "PTA_JUMP_CONTROL_A", x, y, WzColor_RGB( 255, 0, 0  ) );
            y += Line;
        }

        if(BIT_CHECK(dwAtt,PTA_JUMP_CONTROL_B))
        {
            g_pSunRenderer->x_pManagerTextOut->DrawText( "PTA_JUMP_CONTROL_B", x, y, WzColor_RGB( 255, 0, 0  ) );
            y += Line;
        }


        //////////////////////////////////////////////////////////////////////////
        //NpcInfo
        x = 600;
        y = 80;

        if (pChar->IsKindOfObject(MONSTER_OBJECT) == TRUE)
        {
            Monster* monster_ptr = static_cast<Monster*>(pChar);
            BASE_NPCINFO* npc_info_ptr = monster_ptr->GetMonsterInfo();
            TCHAR desc_str[INTERFACE_STRING_LENGTH + 1] = {0,};

            GlobalFunc::GetMonGradeDesc(static_cast<eNPC_GRADE>(npc_info_ptr->m_byGrade), desc_str);
            Snprintf( lpszText, 256-1, _T("Grade: %d(%s)"), npc_info_ptr->m_byGrade, desc_str);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;

            GlobalFunc::GetAICodeDesc(npc_info_ptr->m_wAICode, desc_str);
            Snprintf( lpszText, 256-1, _T("AICode(세부 AI): %d(%s)"), npc_info_ptr->m_wAICode, desc_str);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;

            GlobalFunc::GetAttackAttitudeDesc(static_cast<eATTACK_ATTITUDE>(npc_info_ptr->m_byAttitude), desc_str);
            Snprintf( lpszText, 256-1, _T("Attitude(공격성향): %d(%s)"), 
                      npc_info_ptr->m_byAttType, 
                      desc_str);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;

            GlobalFunc::GetAttTypeDesc(static_cast<eATTACK_TYPE>(npc_info_ptr->m_byAttType), desc_str);
            Snprintf( lpszText, 256-1, _T("AttType(공격타입): %d(%s)"), 
                npc_info_ptr->m_byAttType, 
                desc_str);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;

            GlobalFunc::GetMoveAttitudeDesc(static_cast<eNPC_MOVE_ATTITUDE>(npc_info_ptr->m_byMoveAttitude), desc_str);
            Snprintf( lpszText, 256-1, _T("MoveAttitude(이동성향): %d(%s)"), 
                npc_info_ptr->m_byAttType, 
                desc_str);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;

            Snprintf( lpszText, 256-1, _T("PhyAttRate(공격 성공률): %d"), npc_info_ptr->m_wPhyAttRate );
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;

            Snprintf( lpszText, 256-1, _T("PhyAttAvoid(공격 회피률): %d"), npc_info_ptr->m_wPhyAttAvoid );
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;

            Snprintf( lpszText, 256-1, _T("WalkSpeed: %f"), npc_info_ptr->m_fWalkSpeed );
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;

            Snprintf( lpszText, 256-1, _T("RunSpeed: %f"), npc_info_ptr->m_fRunSpeed );
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;
            Snprintf( lpszText, 256-1, _T("AttackPower(최소/최대): %d / %d"), npc_info_ptr->m_dwMinAttackPower, npc_info_ptr->m_dwMaxAttackPower );
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;
            Snprintf( lpszText, 256-1, _T("SkillPower: %d"), npc_info_ptr->skill_power );
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;
            Snprintf( lpszText, 256-1, _T("PhysicalDefense: %d"), npc_info_ptr->m_dwPhyDef );
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 255, 255));
            y += Line;
            Snprintf( lpszText, 256-1, _T("Fire(불) Attack/Resist: %d / %d%%"), npc_info_ptr->element_fire_attack, npc_info_ptr->element_fire_resist);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 64, 0));
            y += Line;
            Snprintf( lpszText, 256-1, _T("Water(물) Attack/Resist: %d / %d%%"), npc_info_ptr->element_water_attack, npc_info_ptr->element_water_resist);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(0, 128, 255));
            y += Line;
            Snprintf( lpszText, 256-1, _T("Earth(대지) Attack/Resist: %d / %d%%"), npc_info_ptr->element_earth_attack, npc_info_ptr->element_earth_resist);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 192, 0));
            y += Line;
            Snprintf( lpszText, 256-1, _T("Wind(바람) Attack/Resist: %d / %d%%"), npc_info_ptr->element_wind_attack, npc_info_ptr->element_wind_resist);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(0, 255, 128));
            y += Line;
            Snprintf( lpszText, 256-1, _T("Dark(암흑) Attack/Resist: %d / %d%%"), npc_info_ptr->element_dark_attack, npc_info_ptr->element_dark_resist);
            g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(192, 192, 192));
            y += Line;
        }

        if (pChar->IsKindOfObject(MONSTER_OBJECT) == TRUE)
        {            
            //if (aggro_player_count > 0)
            //{
            //    int index = 0;
            //    int gap_count = 2;
            //    if (aggro_player_count > 40)
            //    {
            //        gap_count = 3;
            //    }
            //    TCHAR filed_string[3][255] = { "%-16s: %-6d", "%-16s: %-6d %-16s: %-6d",
            //        "%-16s: %-6d %-16s: %-6d %-16s: %-6d" };
            //    for (index = gap_count; index <= aggro_player_count; index = index+gap_count)
            //    {
            //        Snprintf(lpszText, 256-1, filed_string[gap_count-1], 
            //            aggro_value_info[index-2].character_name_, aggro_value_info[index-2].aggro_value_,
            //            aggro_value_info[index-1].character_name_, aggro_value_info[index-1].aggro_value_,
            //            aggro_value_info[index-3].character_name_, aggro_value_info[index-3].aggro_value_);
            //        g_pSunRenderer->x_pManagerTextOut->DrawText(lpszText, x, y, WzColor_RGB(255, 128, 0));
            //        y += Line;
            //    }
            //    int remain_count = (aggro_player_count%gap_count);
            //    if (remain_count == 1)// 1개 남은 경우
            //    {
            //        Snprintf(lpszText, 256-1, filed_string[0], 
            //            aggro_value_info[aggro_player_count-1].character_name_, aggro_value_info[0].aggro_value_);
            //        g_pSunRenderer->x_pManagerTextOut->DrawText(lpszText, x, y, WzColor_RGB(255, 192, 0));
            //        y += Line;
            //    }
            //    else if (remain_count == 2)// 2개 남은 경우
            //    {
            //        Snprintf(lpszText, 256-1, filed_string[gap_count-1], 
            //            aggro_value_info[index-2].character_name_, aggro_value_info[index-2].aggro_value_,
            //            aggro_value_info[index-1].character_name_, aggro_value_info[index-1].aggro_value_);
            //        g_pSunRenderer->x_pManagerTextOut->DrawText(lpszText, x, y, WzColor_RGB(255, 192, 0));
            //        y += Line;
            //    }
            //}
            //else
            //{
            //    Snprintf( lpszText, 256-1, _T("Aggro: 없음"));
            //    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB(255, 192, 0));
            //    y += Line;
            //}

            if (pChar->GetObjectKey() != pre_object_key)
            {//갱신 정보를 보내자
                pre_object_key = pChar->GetObjectKey();
                TCHAR string_result[MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH] = {0,};
                sprintf(string_result, "aggrodisplay %u", pre_object_key);
                MSG_CG_GM_STRING_CMD_SYN msg;
                ZeroMemory(msg.m_szStringCmd,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
                StrnCopy(msg.m_szStringCmd, string_result, MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
                GlobalFunc::SendPacket(CI_GAMESERVERIDX,&msg,sizeof(msg));
            }
        }
        //////////////////////////////////////////////////////////////////////////
	}
#endif //#ifdef _DEV_VER
}

void BattleScene::_renderDbgInfoKeyQueueNum(int& x, int& y)
{
#ifdef _DEV_VER
	TCHAR lpszText[256] = {0,};
	Snprintf( lpszText, 256-1, _T("Current key Queue Num : %d"),g_KeyQueueManager.Size());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );		
    y += 20;
#endif //#ifdef _DEV_VER
}

void BattleScene::_renderDbgInfoSkillQueueNum(int& x, int& y)
{
#ifdef _DEV_VER
	TCHAR lpszText[256] = {0,};
	Snprintf( lpszText, 256-1, _T("Current skill Queue Num : %d"),g_HeroInput.GetSkillQueueSize());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );		
    y += 20;
#endif //#ifdef _DEV_VER
}

void BattleScene::_renderDbgInfoCameraSpring(int& x, int& y)
{
#ifdef _DEV_VER
	TCHAR message[256] = {0,};
    
    Snprintf(message, 256-1, _T("카메라기본:1.3 \n\n카메라 현재위치:%f  "), g_Camera.CameraHeight());
    g_pSunRenderer->x_pManagerTextOut->DrawText( message, x, y, WzColor_RGB(0, 255, 0));	
    
    y += 100;

	Snprintf(message, 256-1, 
             _T("SPRING_CONST:%f\nDAMP_CONST:%f\nSPRING_LEN:%f"),
             g_Camera.GetSpringConst(),
             g_Camera.GetDampConst(),
             g_Camera.GetSpringLen());

	g_pSunRenderer->x_pManagerTextOut->DrawText(message, x, y, WzColor_RGB( 255, 0, 0 ));		
    y += 20;
#endif //#ifdef _DEV_VER
}

//------------------------------------------------------------------------------
/**
*/
void BattleScene::_renderDbgInfoHeroStatus(int& x, int& y)
{
#ifdef _DEV_VER
	if (!g_pHero) return;
	if (!g_pSunTerrain) return;

	TCHAR lpszText[512] = {0,};

	WzVector vPos = g_pHero->GetVisiblePos();

	g_pSunTerrain->PathExp_Reset(g_pPathExplorer,&vPos, -1);

	Snprintf( lpszText, 512-1, _T("Current Tile : %d[%d] Convex: [%d]")
		, g_pHero->GetPathExplorer()->GetTile() 
		,g_pPathExplorer->GetTile()
		,g_pSunTerrain->x_pPathFinder->GetConvexOfTile(g_pPathExplorer->GetTile()));
	
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("Current Pos : x : %f y: %f z : %f"), vPos.x,vPos.y,vPos.z );
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	PLAYER_ACTION curact = *g_pHero->GetCurrentAction();
	PLAYER_ACTION queueact = g_HeroInput.GetQueueAction();

	Snprintf( lpszText, 512-1, _T("Current Action : %s") , GetActionString(&curact) );
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("Queue Action : %s") , GetActionString(&queueact) );
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("Remain Attack Result : %d") , g_pHero->GetAttackResultListSize());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;


#ifdef _NA_000000_20120410_SPEEDHACK

#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
    float move_speed_modifier = g_pHero->GetMoveSpeedModifier();
#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
    float move_speed_modifier = 0.0f;
    if (g_pHero->IsRiding() || g_pHero->m_bCustomMoveSpeed)
    {
        move_speed_modifier =  g_pHero->GetMoveSpeedModifier();
    }
    else
    {
        HeroAttributes* const hero_attr = (HeroAttributes*)(g_pHero->GetPlayerAttribute());
        move_speed_modifier = hero_attr->move_speed_modifier_;
    }
#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER


    float fMoveSpeed = g_pHero->move_speed_;
    Snprintf( lpszText, 512-1, _T("Current Move Speed : %f(%f(%f), %f)") , 
        fMoveSpeed * move_speed_modifier,  
        fMoveSpeed, g_pHero->Character::GetMoveSpeed(),
        move_speed_modifier);
#else
    float fMoveSpeed = g_pHero->GetMoveSpeed() * g_pHero->GetMoveSpeedModifier() ;
    Snprintf( lpszText, 512-1, _T("Current Move Speed : %f") , fMoveSpeed );
#endif //_NA_000000_20120410_SPEEDHACK

	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	float fDistance=10.0f;
	if(fMoveSpeed)
	{
		fDistance=min(10.f,500.0f*fMoveSpeed);
	}

	Snprintf( lpszText, 512-1, _T("MoveDistance: %f") , fDistance  );
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("Fighting Energy : %d") , g_pHero->GetFightingEnergy());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	int iMaxTile[64];
	ZeroMemory(iMaxTile,sizeof(iMaxTile));
	if (g_pHero->GetPathExplorer()->IsMoving()) 
    {
		int iRouteNum = g_pHero->GetPathExplorer()->GetTileRoute(iMaxTile);

		if (iRouteNum > 0)
		{
			TCHAR szTile[32] = {0,};

			int iVisibleRouteNum = iRouteNum;
			// 10개까지만 보여준다.
			if (iVisibleRouteNum > 10)
			{
				iVisibleRouteNum = 10;
			}
			ZeroMemory(lpszText,sizeof(lpszText));

			Snprintf(szTile, 32-1, _T("Route Count(%d) "), iRouteNum);
			_tcscat(lpszText,szTile);

			for (int a = 0; a < iVisibleRouteNum; a++) {
				ZeroMemory(szTile,sizeof(szTile));
				Snprintf(szTile, 32-1, _T("[%d] "), iMaxTile[a]);
				_tcscat(lpszText,szTile);				
			}

			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
			y += 20;
		}
	}

	Snprintf( lpszText, 512-1, _T("TempVariable : %d %d %d"), g_TempVariable[0], g_TempVariable[1], g_TempVariable[2]);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	int iCurTile = g_pHero->GetPathExplorer()->GetTile();

	int iNearNode[3];

	if(iCurTile>=0)
		g_pSunTerrain->x_pPathFinder->GetNearNodes(iCurTile , iNearNode , 3);

	Snprintf( lpszText, 512-1, _T("NearNodes : %d %d %d"), iNearNode[0], iNearNode[1], iNearNode[2]);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 512-1, _T("Picked Tile : %d (%f %f %f)")
			, g_HeroInput.GetPickedTileIndex()
			, g_HeroInput.GetPickedPosition().x
			, g_HeroInput.GetPickedPosition().y
			, g_HeroInput.GetPickedPosition().z);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

    Snprintf( lpszText, 512-1, _T("Exp : %ld / %ld"), g_pHero->GetExp(), g_pHero->GetNextExp());
    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );		
	y += 20;

	//////////////////////////////////////////////////////////////////////////
	if(g_pHero && g_pHero->IsPetReturn())
	{
		Pet* pPet = (Pet*)g_ObjectManager.GetObject(g_pHero->GetPetObjectKey());
		if(pPet)
		{
			WzColor ColTemp = pPet->GetColor();
			Snprintf( lpszText, 512-1, _T("Pet Color : %x"),pPet->GetColor() );
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
			y += 20;
		}
		else
		{
			Snprintf( lpszText, 512-1, "pPet == NULL" );
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
			y += 20;
		}
	}
	else
	{
		Snprintf( lpszText, 512-1, "펫소환X" );
		g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
		y += 20;
	}

	//////////////////////////////////////////////////////////////////////////
	WzVector vNormal =g_pHero->GetNormal();
	WzVector vUp={0.f,0.f,1.f};
	float fAngle = n_rad2deg( acosf( VectorDotProduct( &vNormal, &vUp)));
	Snprintf( lpszText, 512-1, "지형노말각: %f", fAngle);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );	
	y += 20;

	float fCameraAngle = g_Camera.GetAngle();
	float fHeroAngle   = g_pHero->GetAngle();

	Snprintf( lpszText, 512-1, "카메라(%f) 주인공(%f)", fCameraAngle,fHeroAngle);
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );	
	y += 20;

    DWORD load_time = DelayLoadManager::Instance()->delay_time(kLoadDelay);
    DWORD process_time = DelayLoadManager::Instance()->delay_time(kProcessDelay);
    DWORD quick_load_count = g_pSunRenderer->x_pManagerMultiLoader->GetNumberOfLoadObject(QUEUE_TYPE_QUICK);
    DWORD normal_load_count = g_pSunRenderer->x_pManagerMultiLoader->GetNumberOfLoadObject(QUEUE_TYPE_NORMAL); 

    Snprintf(lpszText, 512-1, "멀티쓰레드-QuickLoad:%d NormalLoad:%d", quick_load_count, normal_load_count);
    g_pSunRenderer->x_pManagerTextOut->DrawText(lpszText, x, y, WzColor_RGB(0, 255,0));	
    y += 20;

    DWORD refresh_count = DelayLoadManager::Instance()->size();
    Snprintf(lpszText, 512-1, "로드할갯수(%d)-load:%d process:%d", refresh_count, load_time, process_time);
    g_pSunRenderer->x_pManagerTextOut->DrawText(lpszText, x, y, WzColor_RGB(0, 255,0));	
    y += 20;


	//////////////////////////////////////////////////////////////////////////
#endif //#ifdef _DEV_VER
}


void BattleScene::_renderDbgInfoResourceStatus(int& x, int& y)
{
#ifdef _DEV_VER
	TCHAR lpszText[256] = {0,};

	Snprintf( lpszText, 256-1, _T("Player Count : %d"), g_ObjectManager.GetObjectCount(PLAYER_OBJECT) );
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;
	
	Snprintf( lpszText, 256-1, _T("Monster Count : %d"), g_ObjectManager.GetObjectCount(MONSTER_OBJECT) );
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 256-1, _T("Item Count : %d"), g_ObjectManager.GetObjectCount(ITEM_OBJECT)  );
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 256-1, _T("Loaded Resource Count : %d"), ResourceManager::Instance()->GetLoadedResourceCount());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

	Snprintf( lpszText, 256-1, _T("Wait For Free Resource Count : %d"), ResourceManager::Instance()->GetWaitForFreeResourceCount());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;
	
	Snprintf( lpszText, 256-1, _T("ObjectManager.size = %d"), g_ObjectManager.GetAllObjectCnt());
	g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
	y += 20;

#endif //#ifdef _DEV_VER
}
//------------------------------------------------------------------------------
void  BattleScene::_renderDbgInfoSound(int& x, int& y)
{
#ifdef _DEV_VER
    if(NULL == g_pMap)
        return;

    // FPS
    DWORD dwTick;
    DWORD dwTicks2;
    DWORD dwTick3;
    LONG lCount, lPpsCount, lDcpsCount;
    g_pSunRenderer->GetFpsValues( &dwTick, &lCount );
    float fFps = ( float )lCount * 1000.0f / ( float )dwTick;

    g_pSunRenderer->GetPpsValues( &dwTicks2, &lPpsCount);
    g_pSunRenderer->GetDcpsValues( &dwTick3, &lDcpsCount);
    LONG lPolygon = lCount > 0 ? lPpsCount / lCount : 0;
    LONG lDrawCall = lCount > 0 ? lDcpsCount / lCount : 0;

    TCHAR lpszText[512] = {0,};

    Snprintf( lpszText, 512-1, _T(" %f fps (%ld polygons, %ld drawcalls)"), 
        fFps, lPolygon, lDrawCall);
    g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
    y += 20;


    //if (wzsnd_cache_is())
    {
        Snprintf( lpszText, 256-1, _T("wzsnd_cache_is = %s"), wzsnd_cache_is() ? "TRUE" : "FALSE" );
        g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
        y += 20;

        Snprintf( lpszText, 256-1, _T("wzsnd_cache_size = %d"), wzsnd_cache_size());
        g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
        y += 20;

        Snprintf( lpszText, 256-1, _T("wzsnd_cache_maintain_min = %d"), wzsnd_cache_maintain_milliseconds_get() / 1000 / 60);
        g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
        y += 20;

        Snprintf( lpszText, 256-1, _T("calltick_cache sec = %d"), Sound::Instance()->calltick_cache() / 1000);
        g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
        y += 20;

        Snprintf( lpszText, 256-1, _T("calltick_updatelistener sec = %d"), Sound::Instance()->calltick_updatelistener() / 1000);
        g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, x, y, WzColor_RGB( 0x0F, 255, 0x0f ) );
        y += 20;
    }

#endif //#ifdef _DEV_VER
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void  BattleScene::_renderDbgInfoAutobot(int& x, int& y)
{
#ifdef _DEV_VER
    #ifdef _YMS_AUTOBOT_GAME
        Autobot::Instance()->Render_DebugInfo();
    #endif //_YMS_AUTOBOT_GAME
#endif //#ifdef _DEV_VER
}

void BattleScene::AddAttackResultQueueAction( cAttackResultData& ResultData, DWORD dwTimeSerial, DWORD dwAttackId )
{
	Character* pTarget = (Character*)g_ObjectManager.GetObject(ResultData.GetTargetObjectKey());
	if( pTarget == NULL )
	{
		return;
	}

	if( pTarget->IsKindOfObject(MONSTER_OBJECT) == TRUE )
	{
		Monster* pMon = (Monster*)pTarget;
		if(MONSTER_TYPE_SETTING_MAPOBJECT == pMon->GetMonsterType())
			return;
	}


	if( ResultData.CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DOWN ) && ResultData.CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK ) )
	{
		pTarget->SetKnockBackDown( ResultData.GetCurrentPosition().wvPos, ResultData.GetDestPosition().wvPos, ResultData.GetEndDownTime() );
		return;
	}

	// 스턴, 다운, 띄우기등 좌표를 변경하지 않는 애들을 먼저 한 후,
	if( ResultData.CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_STUN ) || 
		ResultData.CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_FREEZE ) 
		)
	{
		pTarget->SetStun( ResultData.GetCurrentPosition().wvPos );
	}

	if( ResultData.CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DOWN) ) // 엎어지기
	{
		pTarget->SetDown( ResultData.GetEndDownTime() );
	}

	if( ResultData.CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_AIR) )
	{				
		if(!pTarget->CannotAir())
		{
			PLAYER_ACTION action;
			action.ActionID = ACTION_TOAIR;
			action.dwTimeSerial =dwTimeSerial;
			action.TOAIR.fDownForceLimit = -40.0f;
			action.TOAIR.fGravity = 70.0f;
			action.TOAIR.fUpperForce = 50.0;
			action.TOAIR.bForceDown=TRUE;


			action.TOAIR.fHeightLimit = 3.4f;

			action.TOAIR.dwAirTime   = ResultData.GetAirTime();
			action.TOAIR.dwGetupTime =  ResultData.GetGetUpTime();			
			pTarget->PutAction(&action);
			pTarget->SetLatestQueueAttack(dwTimeSerial);
		}
	}

	// 좌표를 이동시키는걸 뒤에 한다
	if( ResultData.CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK2) ) 
	{
		pTarget->SetKnockBack2( ResultData.GetCurrentPosition().wvPos, ResultData.GetDestPosition().wvPos );
	}

	if( ResultData.CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK3) )
	{
		pTarget->SetKnockBack3( ResultData.GetCurrentPosition().wvPos, ResultData.GetDestPosition().wvPos );
	}

	// 밀어내기
	if( ResultData.CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK) )
	{
		pTarget->SetKnockBack( ResultData.GetCurrentPosition().wvPos, ResultData.GetDestPosition().wvPos );
	}
}

BOOL BattleScene::PreInterfaceRender(DWORD dwTick)
{
	g_InterfaceManager.PreRenderInterfaceManager(dwTick );
	return TRUE;
}

BOOL BattleScene::PostInterfaceRender(DWORD dwTick)
{
#ifdef _DEV_VER
	_renderDbgText(dwTick);
#endif

    m_EventText.RenderEventScript();

#ifdef _JBH_ADD_AGE_LIMIT_MARK
	RenderGameRatingMark();
#endif //_JBH_ADD_AGE_LIMIT_MARK
	
	g_InterfaceManager.UpdateRenderTarget();
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    g_InterfaceManager.RenderToDiceTexture();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
		
	return TRUE;
}

void BattleScene::AddSplitDamageResult( Character* character, ATTACK_RESULT* attack_result, int hit_count )
{
	int group_id = 1;

	ATTACK_RESULT temp;
    temp = *attack_result;	// copy
    temp.SetGroupID(group_id);

    assert(hit_count>1); // 멍청하면 그냥 죽어야징

    DWORD damage_per_count = attack_result->GetTargetData().GetDamage() / hit_count;

    for (int b = 0; b < hit_count - 1; b++ )
    {
        temp.GetTargetData().SetDamage( damage_per_count );
        temp.GetTargetData().SetTargetHP( attack_result->GetTargetData().GetTargetHP() + attack_result->GetTargetData().GetDamage() - (b*damage_per_count) );

        character->AddAttackResult(&temp);		

		++group_id;
		temp.SetGroupID(group_id);
    }

    //마지막 타 이다.

    temp.GetTargetData().SetDamage( attack_result->GetTargetData().GetDamage() - (damage_per_count*(hit_count-1)));
    temp.GetTargetData().SetTargetHP( attack_result->GetTargetData().GetTargetHP() );

    character->AddAttackResult(&temp);
}

void BattleScene::QueueSkillAction(PLAYER_ACTION *pAction,Character *pChar)
{
	assert(pChar);

	if(!pChar)
		return;

	if (GENERALPARAM->IsServerDebug()) 
	{
		pChar->DoAction(pAction);
		pChar->CommitChangeState(g_CurTime);

	}
	else
	{
		pChar->PutAction(pAction);
	}			
}

// 클라이언트에서 데미지를 여러단계로 나누어서 표시하는 스킬의 갯수를 가져온다
bool BattleScene::GetSplitHitCount( SLOTCODE skill_class_code, int& hit_count )
{
    switch( skill_class_code )
    {
        //2타
    //case eSKILL_MYSTIC_904: // 플라잉비

    case eSKILL_FIRE_DESTROYER_DOUBLE_HEAD:
#ifdef _NA_006679_20130423_1302_SKILL_RENEWAL
#else
    case eSKILL_FILL_OFF:
#endif // _NA_006679_20130423_1302_SKILL_RENEWAL
    case eSKILL_HEAVY_WHISTLE_SHOT:	
    case eSKILL_DOUBLE_SPINKICK:
    case eSKILL_BLADE_FURY:
    //case eSKILL_ARCANE_BLADE:
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
#else
    case eSKILL_FIST_OF_FIRE:
#endif //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    case eSKILL_FELLEN_WARRIOR_XDANCE:
#ifdef _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST
	case eSKILL_WITCHBLADE_1104: // 트위스트플랩킥
	case eSKILL_WITCHBLADE_1106: // 네일스크래치
	case eSKILL_WITCHBLADE_1108: // 데드폴
	case eSKILL_WITCHBLADE_1109: // 드래곤댄스
	case eSKILL_WITCHBLADE_1110: // 스피닝하츠
	case eSKILL_WITCHBLADE_1118: // 크로스콤비네이션
#endif // _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST
        {
            hit_count = 2; 
        }
        break;

        // 3타
    case eSKILL_DARK_KNIGHT_DARK_SWORD:
    case eSKILL_BLOOD_RAIN:
    case eSKILL_TRIPLE_PIERCE:
    case eSKILL_FIRE_FILA:
    case eSKILL_REDAMOR_WARRIOR_SPIN_CRUSH:
    case eSKILL_DARK_F_INFANTRY_BASH:
    case eSKILL_HEAVY_NOIZE_SHOT:
    case eSKILL_DARK_SWORDMAN_TRIPLE_ATTACK:
        {
            hit_count = 3;
        }
        break;

        //4타:
    case eSKILL_DARKNESS_OBSERVER_WHEEL_ATTACK:
    case eSKILL_POISON_RAIN:
    case eSKILL_SWORDDANCING:
    case eSKILL_LIGHTING_WAVE: 
    case eSKILL_FLAME_RULER_EXPLORZEN_KNOCKLE:
    case eSKILL_EVIL_SORCERER_PRESSING_WALL:
    case eSKILL_CHASE_SHOT:
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
#else
    case eSKILL_AIRBLOW:
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
        {
            hit_count = 4;
        }
        break;

        //5타
    case eSKILL_SACRED_FIRE:
    case eSKILL_ILLUSION_DANCE:
    case eSKILL_DARK_SLASH:
        {	
            hit_count = 5;
        }
        break;

        

        //6타
    case eSKILL_FAST_SHOT:
        {
            hit_count = 6;
        }
        break;

    case eSKILL_MYSTIC_909: // 템페스트블로우
        {
            hit_count = 6;
        }
        break;

        //10타
    case eSKILL_SPINING_WAVE:
    case eSKILL_WILD_SHOT:
        {
            hit_count = 10;
        }
        break;
        //15타

    case eSKILL_EVIL_SORCERER_FIRE_BALL:
        {
            hit_count = 15;
        }
        break;

    default:

        // 나머지는 나누지않는다
        return false;
    }

    return true;
}


void BattleScene::AddSkillResult( Character* pChr, SkillFacade::ResultData* result_data )
{
    SkillScriptInfo* skill_info = result_data->GetSkillScriptInfo();
    assert(skill_info && "적용할수없는 스킬의 결과입니다");
    assert(result_data->GetTargetResultCount() < MAX_TARGET_NUM);

//#ifdef _MILKY_DEBUG
//    // 자동 쿨타임 초기화
//    if( g_pHero != NULL && pChr != NULL )
//    {
//        GlobalFunc::ProcessChatMessage( "//쿨타임" );
//    }
//#endif

#ifdef _DEV_VER
    {
        TCHAR message[1024];
        wsprintf(message, "[%s(%d, %d), %d]",
            skill_info->m_szSkillName, 
            skill_info->m_SkillClassCode,
            skill_info->m_SkillCode,
            result_data->GetTargetResultCount() );
        g_InterfaceManager.AddSkillResultMessage( message, 0xFFFFFFFF );
    }
#endif

    if ( result_data->GetTargetResultCount() == 0) //자신 지정스킬의 경우 타겟이 없다.
    {
        return;
    }

    int targetcount = 0;
    int iResultSize = 0;
	cAttackResultData  TargetInfo[MAX_TARGET_NUM];
    //ATTACK_RESULT_TARGET_INFO  TargetInfo[MAX_TARGET_NUM];

	ATTACK_RESULT attackresult;
    DWORD projectileId = NONE_ID;
    DWORD mainTargetIndex = NONE_ID;
    eWEAPONSOUNDKIND eSoundKind;

    if (pChr->IsKindOfObject(PLAYER_OBJECT)) 
    {
        Player * pPlayer = (Player *)pChr;
        eSoundKind = GameFunc::GetWeaponSoundKind((eWEAPONTYPE)pPlayer->GetWeaponKind());
    }
    else
    {
        eSoundKind = (eWEAPONSOUNDKIND)0;
    }


    // 타겟 수 만큼 타겟 결과를 만든다.
    for (int a = 0; a < result_data->GetTargetResultCount(); a++)
    {
        SkillFacade::TargetResult* target_result = result_data->GetTargetResult(a);
        if( target_result == NULL )
            continue;

        // 대상이 없으면 패스
        Character* pTarget = (Character*)g_ObjectManager.GetObject( target_result->GetTargetKey() );
        if( pTarget == NULL )
            continue;

#ifdef _DEV_VER
        {
            TCHAR message[1024];
            wsprintf(message, "  [%d] [%s] [%d]", a+1, pTarget->GetName(), pTarget->GetObjectKey() );
            g_InterfaceManager.AddSkillResultMessage( message, 0xFFFFFFFF );
        }
#endif


        ZeroMemory(&attackresult,sizeof(attackresult));
        attackresult.SetSkillCode( result_data->GetSkillCode() );
        attackresult.SetAttackSerial( result_data->GetClientSerial() );
        attackresult.SetTimeSerial( GetBattleTimeSerial() );
        attackresult.SetWeaponSound( eSoundKind );

        //////////////////////////////////////////////////////////////////////////
        // AddTargetResultInfo 부분
        attackresult.GetTargetData().SetTargetObjectKey( target_result->GetTargetKey() );
        attackresult.GetTargetData().SetAdditionalEffect( ConvertServerToClientEffect( target_result->GetSkillEffect() ));//클라이언트용으로 변환 	
        attackresult.GetTargetData().SetNoDamage(TRUE);// 데미지 Ability가 있을때 FALSE로 변한다.
		attackresult.GetTargetData().SetPosition( pTarget->GetPosition(), pTarget->GetPosition() );


        // 대상에 스킬이펙트를 처리
        if( pTarget != pChr )
        {
            SkillEffectFunc::CreateReceiverSkillEffect(result_data->GetSkillCode(), a, pTarget, pChr );
        }

        // 대상에 어빌리티 내용을 적용 (데미지, 스턴, 힐 등등..)

        int hit_count = 0;
        for ( size_t i=0;i!=target_result->GetResultCount();++i)
        {
            SKILL_RESULT_CODE* result_code = target_result->GetResultCode(i);
            BASE_ABILITYINFO* pAbilityInfo = target_result->GetResultAbillityInfo(i);

            if( result_code == NULL || pAbilityInfo == NULL )
                continue;

			//attackresult.GetTargetData().Init();

            AddAbilityResult( pChr, pTarget, result_data->GetSkillCode(), result_code, attackresult.GetTargetData() );

            // 데미지를 등록
            if( pAbilityInfo->m_wAbilityID == eABILITY_DAMAGE )
            {

#ifdef _DEV_VER
                {
                    TCHAR message[1024];
                    wsprintf(message, "    [DAMAGE : %d]", attackresult.GetTargetData().GetDamage() );
                    g_InterfaceManager.AddSkillResultMessage( message, 0xFFFF88A7 );
                }
#endif

                if( GetSplitHitCount( skill_info->m_SkillClassCode, hit_count ) == true )
                {
                    // 데미지를 클라에서 나누는 경우
                    AddSplitDamageResult( pChr, &attackresult, hit_count );
                }
                else
                {
                    attackresult.SetGroupID(hit_count + 1 );

                    pChr->AddAttackResult( &attackresult );

                    ++hit_count;
                }

                //	맞는 놈이 주인공이면 타겟 설정
                if( g_pHero && ( pTarget->GetObjectKey() == g_pHero->GetObjectKey() ) && attackresult.GetTargetData().IsNoDamage() == FALSE )
                {
                    //	주인공이 타겟을 잡고 있지 않다면 설정
                    if( g_HeroInput.GetCurrentTarget() == 0 )
                    {
                        g_HeroInput.SetCurrentTarget( pChr->GetObjectKey() );
                    }
                }
            }
            else if( pAbilityInfo->m_wAbilityID == eABILITY_CUR_HP_INCREASE )
            {
#ifdef _DEV_VER
                TCHAR message[1024];
                wsprintf(message, "    [ABILITY : %d] [PARAM: %d, %d, %d, %d]", 
                    pAbilityInfo->m_wAbilityID,
                    pAbilityInfo->m_iParam[0],
                    pAbilityInfo->m_iParam[1],
                    pAbilityInfo->m_iParam[2],
                    pAbilityInfo->m_iParam[3] );
                g_InterfaceManager.AddSkillResultMessage( message, 0xFFFF7050 );
#endif
                bool Is_monster = false;
                Player* player_ptr = NULL;
                if (pChr->GetObjectType() == MONSTER_OBJECT)
                {
                    Monster* monster_ptr = static_cast<Monster*> (pChr);

                    if (monster_ptr)
                    {
                        DWORD monster_ptr_parent_id = monster_ptr->GetParentId();

                        if (monster_ptr_parent_id)
                        {
                            Object* object_ptr = NULL;

                            object_ptr = g_ObjectManager.GetObject(monster_ptr->GetParentId() );

                            if (object_ptr)
                            {
                                if (object_ptr->GetObjectType() == PLAYER_OBJECT)
                                {
                                    player_ptr = static_cast<Player*> (object_ptr);
                                }
                            }

                            if (player_ptr)
                            {
                                Is_monster = player_ptr->GetObjectKey() == g_pHero->GetObjectKey();
                            }
                        }
                    }
                }
                if(g_pHero && ( pTarget->GetObjectKey() == g_pHero->GetObjectKey() ) 
                    && (pChr->GetObjectKey() == g_pHero->GetObjectKey() || Is_monster == true ))
                {
                    // 위 조건문은 g_pHero가 존재 할 때 타겟이 내가 되고 만약 다른이가 써줬을 때 
                    //다른 클래스의 어빌리티와 내 스킬 어빌리티가 섞여 나에게 스킬을 쓰지 않을 때는 이상한 수치가 나오게 된다. 
                    attackresult.GetTargetData().SetHeal( TRUE );
                    attackresult.GetTargetData().SetNoDamage( FALSE );

                    PlayerAttributes* player_attributes = g_pHero->GetPlayerAttribute();

                    int skill_percent_damage = player_attributes->GetSkillPercentDamage();

                    if (skill_percent_damage > 0)
                    {
                        int iTempAbility = pAbilityInfo->m_iParam[1];

                        iTempAbility = int(iTempAbility * (skill_percent_damage / 100.0f));

                        attackresult.GetTargetData().SetDamage(pAbilityInfo->m_iParam[1] + iTempAbility);
                    }
                    else
                    {
                        attackresult.GetTargetData().SetDamage(pAbilityInfo->m_iParam[1]);
                    }

                    pChr->AddAttackResult( &attackresult );
                }
            }
            else
            {
#ifdef _DEV_VER
                TCHAR message[1024];
                wsprintf(message, "    [ABILITY : %d] [PARAM: %d, %d, %d, %d]", 
                    pAbilityInfo->m_wAbilityID,
                    pAbilityInfo->m_iParam[0],
                    pAbilityInfo->m_iParam[1],
                    pAbilityInfo->m_iParam[2],
                    pAbilityInfo->m_iParam[3] );
                g_InterfaceManager.AddSkillResultMessage( message, 0xFFFF8432 );
#endif
            }
        }

        AddAttackResultQueueAction( attackresult.GetTargetData(), GetBattleTimeSerial(), pChr->GetObjectKey() );


#ifdef _KST_DISPLAY_SKILL_RESULT_MESSAGE
        char temp[1024];
        wsprintf( temp, "AddAttackResult : AbillityCount[%d] HitCount[%d]", target_result->GetResultCount(), hit_count );
        GlobalFunc::OutputMsg_SysMsg( temp );
#endif


        if( hit_count < 2 )
        {
            //! 연타처리 형태가 아니면 즉시 처리한다
            //! 서버측에서 결과를 싱크 맞게 보내주기때문에 클라이언트는 오면 즉시 처리
            //! skill.info.txt :: FlyingObjectCode 필드를 참고해서 서버측에서 패킷이 늦게 옴.
            //! MSGSUB_SKILL_PERIODIC_DAMAGE_BRD등 주기적으로 오는것도 적시 처리(발사체는 제외)
            if (g_SkillStateFactory.GetResultProcessingType(result_data->GetSkillCode()) == SkillStateFactory::kProcessing_PacketRecvImmediate)
            {
                pChr->ProcessAttackResult(pChr->GetCurrentAttackSerial(), FALSE); 
            }
        }

        //////////////////////////////////////////////////////////////////////////



        TargetInfo[a] = attackresult.GetTargetData();

        if( result_data->GetPrimaryTargetKey() == TargetInfo[a].GetTargetObjectKey() )
        {
            mainTargetIndex = a; //메인타겟 인덱스 보관 
        }

        if (GENERALPARAM->GetSpecialMode()) 
        {
            if (g_pHero && (g_pHero->GetObjectKey() == TargetInfo[a].GetTargetObjectKey()))
            {
                if (TargetInfo[a].GetTargetHP() == 0)
                {
                    TCHAR szString[256];
                    Sprintf(szString,_T("플레이어 %d 스킬로 사망"),result_data->GetSkillCode());
                    GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,szString);							
                }
            }
        }


        // 모든 대상에게서 이펙트
        if( skill_info->m_SkillClassCode == eSKILL_VITAL_SUCTION )
        {
            WzVector StartPosition = pTarget->GetArrowTargetPos();
            WzVector ControlPosition = StartPosition;
            ControlPosition.z += 3.0f;

            cProjectileBezierUnit* pProjectile = new cProjectileBezierUnit();
            pProjectile->SetLifeTime(800);
            pProjectile->SetBezierCourseSingleControl( StartPosition, ControlPosition, pChr->GetObjectKey() );
            pProjectile->CreateEffect( StrToWzID("K797"), pChr->GetObjectKey(), pTarget->GetObjectKey() );
            pProjectile->SetHitData( pChr->GetObjectKey(), 0, StrToWzID("K798"), FALSE, 0, FALSE );
            g_ProjectileManagerEx.InsertProjectile( pProjectile );
        }
    }

    // 서버 디버그모드일경우는 바로 데미지 처리.
    if (GENERALPARAM->IsServerDebug())
    {
        pChr->ProcessAttackResult( result_data->GetClientSerial(), TRUE);//true일때 시리얼에 맞는 결과를 전부 제거 
    }

    // 현재 AttackSerial보다 늦게 온 놈이면 바로 처리해버린다.
    // 딜레이 스킬은 남도 늦게 온다!
    if ( result_data->GetClientSerial() < pChr->GetNextAttackSerial() )
    {
        pChr->ProcessAttackResult(result_data->GetClientSerial(), TRUE);//true일때 시리얼에 맞는 결과를 전부 제거 
    }
}

void BattleScene::AddAbilityResult( Character* pChr, Character* pTarget, DWORD dwSkillCode, SKILL_RESULT_CODE* pResultCode, cAttackResultData& ResultData )
{
    // ABILITY에 딸린 상태이상이 있으면 적용해준다.
    SkillScriptInfo* pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillCode);
    if ( pSkillInfo == NULL )
        return;

    BASE_ABILITYINFO* pAbilityInfo = pSkillInfo->GetAbilityInfoByIndex((BYTE)pResultCode->m_wAbilityIndex);	
    if( pAbilityInfo == NULL )
        return;

    switch(pAbilityInfo->m_wAbilityID)
    {
#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
    case kAbilityActiveComboSkill:
        {
            if( pChr == g_pHero )
            {
                int skill_class_code1 = pAbilityInfo->m_iOption1;
                int skill_class_code2 = pAbilityInfo->m_iOption2;
                int skill_class_code3 = pAbilityInfo->m_iParam[0];
                int skill_class_code4 = pAbilityInfo->m_iParam[1];
                DWORD duration = pAbilityInfo->m_iParam[2]; // 유지시간
                g_pHero->AddActivedComboSkill( skill_class_code1, duration );
                g_pHero->AddActivedComboSkill( skill_class_code2, duration );
                g_pHero->AddActivedComboSkill( skill_class_code3, duration );
                g_pHero->AddActivedComboSkill( skill_class_code4, duration );
            }
        }
        break;
#endif

    case eABILITY_MONSTER_TRANSFORMATION:
        {
            SKILL_RESULT_MONSTER_TRANSFORMATION* pResult = (SKILL_RESULT_MONSTER_TRANSFORMATION *)pResultCode;
            pTarget->SetFreeMove(FALSE);
            pTarget->SetHeight(0.0f);
            DWORD dwCloneKey = g_CloneManager.CreateClone(pTarget);
            Clone *pClone = g_CloneManager.GetClone(dwCloneKey);
            if (pClone) 
            {

                int iR = Red_WzColor(pTarget->GetColor());
                int iG = Green_WzColor(pTarget->GetColor());
                int iB = Blue_WzColor(pTarget->GetColor());

                WzColor color=WzColor_RGBA(iR,iG,iB,255);
                pClone->SetAlphaBlend(AB_ALPHA);
                pClone->SetColor(color);

                pClone->SetLifeTime( 2100 );
                pClone->SetDisappear( 2000 );
                pClone->SetPlayAni( TRUE );
            }

            PLAYER_ACTION action;
            ZeroMemory(&action,sizeof(action));
            action.ActionID = ACTION_TRANSFORM;
            action.TRANSFORM.rcCode=pAbilityInfo->m_iOption1;
            action.TRANSFORM.transFormType=(eTRANSFORM_TYPE)pAbilityInfo->m_iOption1;
            pTarget->DoAction(&action);	
            pTarget->CommitChangeState(g_CurTime);	

            pTarget->SetHP( pResult->m_dwHP );				
            pTarget->SetMaxHP(pResult->m_dwMaxHP);//초기 mp값은 없음으로 서버에서 세팅하라고함-지완

            //변신중 몬스터가 타겟되어있다면 풀어준다.
            if(pTarget->GetObjectKey() == g_HeroInput.GetCurrentTarget())
            {
                g_HeroInput.SetCurrentTarget(0);
            }

        }
        break;

    case eABILITY_DAMAGE:
        {
            SKILL_RESULT_DAMAGE *pResult = (SKILL_RESULT_DAMAGE *)pResultCode;
            ResultData.SetTargetHP( pResult->m_dwTargetHP );
            ResultData.SetDamage( pResult->m_wDamage );
            ResultData.SetNoDamage( FALSE );
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            ResultData.SetTargetSD(pResult->target_sd);
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

            if( pResult->m_byEffect & SKILL_EFFECT_ETHERDEVICE_METAL)
            {
                ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_METAL );
            }

            if( pResult->m_byEffect & SKILL_EFFECT_ETHERDEVICE_ETHER)
            {
                ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_ETHER );
            }

            if (ResultData.GetDamage() == 0 )
            {
                //	SKILL MISS
                ResultData.SetMiss( TRUE );

#ifdef _KST_DISPLAY_SKILL_RESULT_MESSAGE
                TCHAR		szMessage[INTERFACE_STRING_LENGTH];
                g_InterfaceManager.GetInterfaceString( eST_SKILL_MISS, szMessage, INTERFACE_STRING_LENGTH );    // "스킬이 빗나갔습니다."
                g_InterfaceManager.ShowMessageBox( szMessage, 3000, 0 );
#endif
            }
        }
        break;

        //치유의 손길  
    case eABILITY_CUR_HP_INCREASE:// 현재 HP 증가
        {
            if( pAbilityInfo->m_iParam[1] > 0 )//회복 어빌리티일때
            {
                cTargetPosition Position;
                Position.SetTargetVisiblePosition( pTarget->GetObjectKey(), TRUE );

                if( pSkillInfo->m_SkillClassCode == eSKILL_HEALING_HAND)
                {
                    g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), StrToWzID("K410"), 5000, Position );
                }
                else if( pSkillInfo->m_SkillClassCode == eSKILL_HEAL_MAGIC_ARRAY)
                {
                    g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), StrToWzID("k420"), 5000, Position );
                }
                else if( pSkillInfo->m_SkillClassCode == eSKILL_NATURAL_FORCE)
                {
                    g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), StrToWzID("k430"), 5000, Position );
                }
                else
                {
                    g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), StrToWzID("k410"), 5000, Position );
                }
            }
        }
        break;

        // 자신의 HP소모류의 스킬은 바로 HPMP를 빼준다.
    case eABILITY_EXHAUST_MP:
    case eABILITY_EXHAUST_HP:
        {
            SKILL_RESULT_EXHAUST *pResult = (SKILL_RESULT_EXHAUST *)pResultCode;
            pTarget->SetHP(pResult->m_dwTargetHP);
            pTarget->SetMP(pResult->m_dwTargetMP);
        }
        break;		

    case eABILITY_STUN_STATUS:
        {
            // 오타아님 STUN 맞음
            SKILL_RESULT_STUN *pResult = (SKILL_RESULT_STUN *)pResultCode;

			if( pResultCode->m_wStatusCode == eCHAR_STATE_DOWN )
			{
                if( pSkillInfo->m_SkillClassCode != eSKILL_AIRBLOW )
                {
				    ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DOWN );
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
					ResultData.SetEndDownTime(pResultCode->m_iStatusTime);
#else
				    ResultData.SetEndDownTime( pAbilityInfo->m_iParam[2] );
#endif // _NA_008472_20150903_IMPROVED_BALANCE
				    ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvCurPos );
                }
            }
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
            else if ( pResultCode->m_wStatusCode == eCHAR_STATE_UPPERDOWN )
            {
                // 어퍼다운 상태가 걸리면 2.5초간 띄운다.
                pTarget->SetAirBlowAir( 2500 );
            }
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
			else if( pResultCode->m_wStatusCode == eCHAR_STATE_STUN )
			{
				ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_STUN );
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
				ResultData.SetEndDownTime( pResultCode->m_iStatusTime );
#else
				ResultData.SetEndDownTime( pAbilityInfo->m_iParam[2] );
#endif // _NA_008472_20150903_IMPROVED_BALANCE
				ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvCurPos );
			}
			else if( pResultCode->m_wStatusCode == eCHAR_STATE_STUN2 )
			{
				ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_STUN );
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
				ResultData.SetEndDownTime( pResultCode->m_iStatusTime );
#else
				ResultData.SetEndDownTime( pAbilityInfo->m_iParam[2] );
#endif // _NA_008472_20150903_IMPROVED_BALANCE
				ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvCurPos );
			}
			else if( pResultCode->m_wStatusCode == eCHAR_STATE_FROZEN )
			{
				ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_FREEZE );
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
				ResultData.SetEndDownTime( pResultCode->m_iStatusTime );
#else
				ResultData.SetEndDownTime( pAbilityInfo->m_iParam[2] );
#endif // _NA_008472_20150903_IMPROVED_BALANCE
				ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvCurPos );
			}
			else if( pResultCode->m_wStatusCode == eCHAR_STATE_SLEEP )
			{
				ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_STUN );
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
				ResultData.SetEndDownTime( pResultCode->m_iStatusTime );
#else
				ResultData.SetEndDownTime( pAbilityInfo->m_iParam[2] );
#endif // _NA_008472_20150903_IMPROVED_BALANCE
				ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvCurPos );
			}
			//else if( pResultCode->m_wStatusCode == eCHAR_STATE_STONE )
			//{
			//	ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_STUN );
			//	ResultData.SetEndDownTime( pAbilityInfo->m_iParam[2] );
			//	ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvCurPos );
			//}
			else
			{
				WzVector vZeroVector;
				ZeroMemory(&vZeroVector,sizeof(vZeroVector));

				// 바로멈춘다!
				if (pResult->m_wvCurPos != vZeroVector)
				{				
				    pTarget->OnParalyzed();
				    pTarget->SetPosition(pResult->m_wvCurPos);
				}
			}
        }
        break;		

    case eABILITY_RESURRECTION://부활 Todo:Mp 세팅 
        {
            SKILL_RESULT_RESURRECTION *pResult = (SKILL_RESULT_RESURRECTION *)pResultCode;

            pTarget->SetHP(pResult->m_dwTargetHP);
            pTarget->SetMP(pResult->m_dwTargetMP);
            pTarget->SetPosition(pResult->m_wvCurPos);

            cTargetPosition Position;
            Position.SetTargetVisiblePosition( pTarget->GetObjectKey(), TRUE );

            if(pTarget->IsKindOfObject( PLAYER_OBJECT))
            {
                g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), StrToWzID("A022"), 5000, Position );
            }
            else
            {
                g_ProjectileManagerEx.CreateEffect( pTarget->GetObjectKey(), StrToWzID("A023"), 5000, Position );
            }
        }
        break;

    case eABILITY_SELF_DESTRUCTION:
    case eABILITY_KNOCKBACK:
        {
            SKILL_RESULT_POSITION* pResult = (SKILL_RESULT_POSITION*)pResultCode;

			ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK );

			if( pTarget->IsMyHero() == TRUE )
            {
				ResultData.SetPosition( pTarget->GetPosition(), pResult->m_wvDestPos );
            }
            else
            {
				ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvDestPos );
            }

			ResultData.SetForceSetLastestAttack( TRUE );
        }
        break;

		// 몬트샤인악몽의기사 돌려찍기
    case eABILITY_KNOCKBACK2:
        {
			ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK2 );

            SKILL_RESULT_POSITION* pResult = (SKILL_RESULT_POSITION*)pResultCode;
			if( pTarget->IsMyHero() == TRUE )
			{
				ResultData.SetPosition( pTarget->GetPosition(), pResult->m_wvDestPos );
			}
			else
			{
				ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvDestPos );
			}

			ResultData.SetForceSetLastestAttack( TRUE );
        }
        break;

    case eABILITY_TELEPORT:
        {
            SKILL_RESULT_POSITION* pResult = (SKILL_RESULT_POSITION*)pResultCode;
            pResult = pResult;

            //if( pTarget->IsMyHero() == TRUE )
            //{
            //    ResultData.SetPosition( pTarget->GetPosition(), pResult->m_wvDestPos );
            //}
            //else
            //{
            //    ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvDestPos );
            //}
            break;
        }

	case eABILITY_PULLING:
		{
			ResultData.AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_KNOCKBACK3 );

			SKILL_RESULT_POSITION* pResult = (SKILL_RESULT_POSITION*)pResultCode;
			if( pTarget->IsMyHero() == TRUE )
			{
				ResultData.SetPosition( pTarget->GetPosition(), pResult->m_wvDestPos );
			}
			else
			{
				ResultData.SetPosition( pResult->m_wvCurPos, pResult->m_wvDestPos );
			}

			ResultData.SetForceSetLastestAttack( TRUE );
		}
		break;

    case eABILITY_FIGHTING_ENERGY_NUM_INCREASE:
    case eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY:
        {
            SKILL_RESULT_FIGHTING_ENERGY *pResult = (SKILL_RESULT_FIGHTING_ENERGY *)pResultCode;
            pTarget->SetFightingEnergy(pResult->m_sFightingEnergyCount);
        }
        break;

    case eABILITY_BLOCK_SKILL:
        if(pTarget == g_pHero)
        {
            g_pHero->SetLockSkill(TRUE);
        }
        break;
    }

    Character* pRealTarget = pTarget;

    if(SKILL_TARGET_ENEMY_AND_ME == pSkillInfo->m_byTarget)
        pRealTarget = pChr;

    // 상태효과(버프)를 생성한다
    if ( pResultCode->m_wStatusCode != 0 ) 
    {
        if( pResultCode->m_wStatusCode == 307 )
        {
            int current_count = pTarget->GetSkillEffectCount( 307 );
            while( current_count < pTarget->GetFightingEnergy() )
            {
                if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateSkillStatusEffect( pResultCode->m_wStatusCode, dwSkillCode, pResultCode->m_iStatusTime, pAbilityInfo ) )
                {
                    pEffect->CreateEffect( pRealTarget, pChr->GetObjectKey() );
                    pRealTarget->AddSkillEffect(pEffect);
                }

                ++current_count;
            }
        }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        else if( pResultCode->overlap_status_count > 0)
        {
            if ( pResultCode->overlap_status_count == 1) // 처음
            {
                if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateSkillStatusEffect( pResultCode->m_wStatusCode, dwSkillCode, pResultCode->m_iStatusTime, pAbilityInfo ) )
                {
                    pEffect->CreateEffect( pRealTarget, pChr->GetObjectKey() );
                    pRealTarget->AddSkillEffect(pEffect);
                }
            }
            else
            {
                // 중첩버프
                SKILL_EFFECT* skill_effect_ptr = 
                    pRealTarget->FindSkillEffect(static_cast<eCHAR_STATE_TYPE>(pResultCode->m_wStatusCode));
                if (skill_effect_ptr)
                {
                    skill_effect_ptr->CreateEffect( pRealTarget, pChr->GetObjectKey() );
                    skill_effect_ptr->overlap_status_count_ = pResultCode->overlap_status_count;
                    skill_effect_ptr->iRemainTime = pResultCode->m_iStatusTime;
                }
            }
        }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        else
        {
            if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateSkillStatusEffect( pResultCode->m_wStatusCode, dwSkillCode, pResultCode->m_iStatusTime, pAbilityInfo ) )
            {
                pEffect->CreateEffect( pRealTarget, pChr->GetObjectKey() );
                pRealTarget->AddSkillEffect(pEffect);
            }
        }
    }
}

void BattleScene::AddReflectDamageResult(char *pPacket,Character *pChr)
{
    nsSync::MSGSUB_SKILL_REFLECT_DAMAGE_BRD* pResultCode = (nsSync::MSGSUB_SKILL_REFLECT_DAMAGE_BRD*)pPacket;

	if( pResultCode->m_SkillCode == 0 )//0이면 어빌리티 없음 
		return;

	SkillScriptInfo *pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(pResultCode->m_SkillCode);
	assert(pSkillInfo);
	if(!pSkillInfo)
	{
		FILE *fp = fopen("CrashLog.txt","wt");
		if (fp)
		{
			fprintf(fp,"AddAbilityResult : 서버가 보내준 SkillCode 이상 SkillCode : %d",pResultCode->m_SkillCode);
			fclose(fp);
		}

		return;
	}

	DWORD dwState = 0;

	// ABILITY에 딸린 상태이상이 있으면 적용해준다.
	BASE_ABILITYINFO *pAbilityInfo = pSkillInfo->GetAbilityInfo(pResultCode->m_wAbilityCode);
	if(!pAbilityInfo)
	{
		FILE *fp = fopen("CrashLog.txt","wt");
		if (fp)
		{
			fprintf(fp,"AddAbilityResult : 서버가 보내준 AbilityIndex 이상 SkillCode : %d AbilityIndex : %d",pResultCode->m_SkillCode,pResultCode->m_wAbilityCode);
			fclose(fp);
		}

		return;
	}

	switch(pResultCode->m_wAbilityCode)
	{
	case eABILITY_REFLECT_DAMAGE:
		{

		}
		break;

	case eABILITY_REFLECT_FEAR:
		{
			dwState = eCHAR_STATE_FEAR;
		}
		break;

	case eABILITY_REFLECT_FROZEN:
		{
			dwState = eCHAR_STATE_FROZEN;
		}
		break;

	case eABILITY_REFLECT_SLOW:
		{
			dwState = eCHAR_STATE_FETTER;
		}
		break;
			
	case eABILITY_REFLECT_SLOWDOWN:
		{
			dwState = eCHAR_STATE_SLOWDOWN;
		}
		break;

	case eABILITY_REFLECT_STUN:
		{
			dwState = eCHAR_STATE_STUN;
		}
		break;
	}

	if (pChr)
	{	
		// 어빌리티 아이디가 잘못 왔을경우 인포가 없다!
		if (pAbilityInfo && pAbilityInfo->m_wStateID != 0) 
        {
            if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateSkillStatusEffect( dwState, pResultCode->m_SkillCode, pAbilityInfo->m_iParam[1], pAbilityInfo ) )
            {

			    for (int a = 0; a < 4; a++)
			    {
				    pEffect->AbilityInfo.m_iParam[0] = pAbilityInfo->m_iOption2;	
				    pEffect->AbilityInfo.m_iParam[1] = pAbilityInfo->m_iParam[0];	
				    pEffect->AbilityInfo.m_iParam[2] = pAbilityInfo->m_iParam[1];	
				    pEffect->AbilityInfo.m_iParam[3] = 0;						
			    }

                pEffect->CreateEffect( pChr, pChr->GetObjectKey() );

                pChr->AddSkillEffect(pEffect);
            }
        }
	}
}

float BattleScene::GetMonsterHPIncreaseRatio()
{
	return m_fMonsterHPMultiplier;
}

void BattleScene::SetMonsterHPMultiplier(float fMultiplier)
{
    // TODO : 여기서 몬스터 관련 메시지 처리를 하자. - j0
    
    if (m_fMonsterHPMultiplier > fMultiplier)
    {
        TCHAR szMessage[INTERFACE_STRING_LENGTH];
        // 5621	몬스터가 약해집니다.
        g_InterfaceManager.GetInterfaceString(5621, szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        // 5622	몬스터로부터 받을 수 있는 경험치가 감소합니다.
        g_InterfaceManager.GetInterfaceString(5622, szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }

	m_fMonsterHPMultiplier = fMultiplier;


	Object_Map_Itr itr = g_ObjectManager.GetBegin();
	for (; itr != g_ObjectManager.GetEnd(); itr++)
	{
		Object* pObject = static_cast<Object*>(itr->second);
		if (0 == pObject)
		{
			assert(!"Object Is NULL!!");
			return;
		}
		
		if (pObject->IsKindOfObject(MONSTER_OBJECT))
		{
			Monster* pMon = (Monster*)pObject;
			if (pMon->IsShadowSummon())
            {
				continue;
            }

			pMon->ChangeHPRatio(fMultiplier);
		}
	}
}

void				
BattleScene::SettingNPC(MAPID MapCode ,FIELDID FieldID)
{
	// NPC 위치시킨당.
	WzVector wzPos;
	WzVector wzDir;

	EXTRA_NPCINFO_SET * pInfoSet =  
		NPCInfoParser::Instance()->GetExtraInfoSetFromFieldID(MapCode , FieldID);

	vector<EXTRA_NPCINFO*>::iterator itr;

	EXTRA_NPCINFO * pInfo = NULL;

	DWORD object_key = NPC_OBJECT_KEY;

	for ( itr = pInfoSet->GetBegin(); itr != pInfoSet->GetEnd(); itr ++)
	{
		pInfo = *itr;

		assert(pInfo);

        BASE_NPCINFO* base_npcinfo_ptr = NPCInfoParser::Instance()->GetNPCInfo(pInfo->m_NPCCODE);
        if (base_npcinfo_ptr == NULL)
        {
            continue;
        }

        if (base_npcinfo_ptr->m_byGrade == eNPC_MOVE_SYNC_MERCHANT)
        {
            continue;
        }

		NPC * pNPC = NULL;
		if (pInfo->m_eNPCTYPE==eNPC_TYPE_LOTTO)
		{
			pNPC = (NPC *)g_ObjectManager.Add( ++object_key, LOTTO_NPC_OBJECT, pInfo->m_NPCCODE );
		}
		else
		{
			pNPC = (NPC *)g_ObjectManager.Add( ++object_key, NPC_OBJECT, pInfo->m_NPCCODE );	
		}
		if (pNPC) {
			SetVector(&wzPos,pInfo->m_fNPCPos[0],pInfo->m_fNPCPos[1],pInfo->m_fNPCPos[2]);
			SetVector(&wzDir,pInfo->m_fNPCDir[0],pInfo->m_fNPCDir[1],pInfo->m_fNPCDir[2]);
            pNPC->SetPosition( wzPos );
			pNPC->SetAngle( Math_GetAngleFromVector(&wzDir) );
			pNPC->SetExtraInfo(pInfo);
            pNPC->SetVisiblePos(wzPos);
            NPC::InitNpcImageInfo(pNPC, pNPC->GetMonsterInfo());

            if( CWzUnitDraw* pBasePart = pNPC->GetBasePart() )
            {
                WzVector wzEffPos, wzEffRot, wzEffScale;
                SetVector( &wzEffRot, 0.0f, 0.0f, 0.0f );
                SetVector( &wzEffScale, 1.0f, 1.0f, 1.0f );

	            int nNumEffect = pBasePart->GetNumUnitEffectInfo();
	            for( int i=0; i<nNumEffect; ++i )
	            {
		            WzUnitEffectInfo* pei = pBasePart->GetUnitEffectInfo( i );
		            CEffectBody* pEffectBody = g_pEffectManager->GetEffectPtrHandle( pei->m_hEffect );
		            if( pEffectBody )
                    {
                        wzEffPos = wzPos + pei->m_wvPosition;
                        pEffectBody->SetResultMatrix( wzEffPos, wzEffRot, wzEffScale );
                    }
                }
            }
        }
	}

	RefreshNPCStateForQuest();
}

BOOL BattleScene::CheckHeroInArea(DWORD areaID)
{ 
    if (g_pMap == NULL)
    {
        return FALSE;
    }

    if (g_pHero == NULL)
    {
        return FALSE;
    }

    eTRIGGER_STATE TrigerState, HeroTrigerState;
    WzSpecialArea* pAreaInfo = g_pMap->GetMapAreaByIndex(areaID);
    if (pAreaInfo == NULL)
    {
        return FALSE;
    }

    TrigerState = TRIGGER_STATE_LEAVE_AREA;
    switch (pAreaInfo->m_wbvRegion.m_eBvType)
    {
    case BVT_AABB:
        {
            if (TRUE == Contain_AABB_Vertex(&pAreaInfo->m_wbvRegion.m_aabb, g_pHero->GetPosition()))			
            {
                TrigerState = TRIGGER_STATE_ENTER_AREA;			
            }
        }
        break;
    case BVT_PATH_TILE_INDEX:
        {
            int iTile = g_pHero->GetPathExplorer()->GetTile();
            for (int i = 0; i < static_cast<int>(pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount); ++i)
            {
                if (iTile == pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData[i])
                {
                    TrigerState = TRIGGER_STATE_ENTER_AREA;
                    break;
                }
            }
        }
        break;
    case BVT_SPHERE:
        {
            if (TRUE == Contain_Sphere_Vertex(&pAreaInfo->m_wbvRegion.m_BvSphere, g_pHero->GetPosition()))
            {
                TrigerState = TRIGGER_STATE_ENTER_AREA;			
            }
        }
        break;
    default:
        {
            return FALSE;
        }
    }

    HeroTrigerState = g_pHero->GetTriggerState(areaID);
    if( HeroTrigerState != TrigerState )
    {
        // 들어갔을땐 언제나 패킷을 보낸다!!!
        if( TrigerState == TRIGGER_STATE_ENTER_AREA )
        {
            //< 플레이어가 들어왔다.
            //	패킷 보내기
            MSG_CG_TRIGGER_ENTER_AREA_SYN	SendPacket;
            SendPacket.m_byCategory		= CG_TRIGGER;
            SendPacket.m_byProtocol		= CG_TRIGGER_ENTER_AREA_SYN;
            SendPacket.m_iAreaID		= areaID;

#ifdef _DEBUG
            TCHAR szMessage[INTERFACE_STRING_LENGTH];
            Sprintf(szMessage, _T("EnterArea( %d ), AreaType( %d )\n"), areaID, pAreaInfo->m_wbvRegion.m_eBvType);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_OUPUT_DEBUG, szMessage);
#endif

            if( FALSE == GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(MSG_CG_TRIGGER_ENTER_AREA_SYN) ) )
            {
                //	MSG_CG_TRIGGER_ENTER_AREA_SYN - BattleScene
                WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
                GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("Failed Send MSG_CG_TRIGGER_ENTER_AREA_SYN - BattleScene"));
            }
        }	
        else if ( TrigerState == TRIGGER_STATE_LEAVE_AREA )
        {
            // 나왔을땐 이전에 들어갔을 경우에만 보낸다.
            if (HeroTrigerState == TRIGGER_STATE_ENTER_AREA)
            {
                MSG_CG_TRIGGER_LEAVE_AREA_SYN	SendPacket;
                SendPacket.m_byCategory		= CG_TRIGGER;
                SendPacket.m_byProtocol		= CG_TRIGGER_LEAVE_AREA_SYN;
                SendPacket.m_iAreaID		= areaID;

#ifdef _DEBUG
                TCHAR szMessage[INTERFACE_STRING_LENGTH];
                Sprintf(szMessage, _T("LeaveArea( %d ), AreaType( %d )\n"), areaID, pAreaInfo->m_wbvRegion.m_eBvType);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_OUPUT_DEBUG, szMessage);
#endif

                if( FALSE == GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(MSG_CG_TRIGGER_LEAVE_AREA_SYN) ) )
                {
                    WzColor	Color = WzColor_RGBA( 255, 80, 80, 255 );
                    GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,_T("Failed Send MSG_CG_TRIGGER_LEAVE_AREA_SYN - BattleScene"));
                }
            }
        }
        g_pHero->SetTriggerState( areaID,TrigerState );
    }

	if (TrigerState == TRIGGER_STATE_ENTER_AREA)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//----------------------------------------------------------------------------
/**
*/
BOOL BattleScene::CheckHeroInShortcutBattleArea(TCHAR* sAreaID, DWORD dwAreaID)
{
    if (g_pHero == NULL)
    {
        return FALSE;
    }

    WzSpecialArea* pAreaInfo = g_pMap->GetMapArea(dwAreaID);

    if (!pAreaInfo)
    {
        return FALSE;
    }

    eSHORTCUT_AREA_STATE ShortcutSate, HeroShortcutSate;
    ShortcutSate = SHORTCUT_AREA_STATE_LEAVE_AREA;
    switch(pAreaInfo->m_wbvRegion.m_eBvType)
    {
    case BVT_AABB:
        {
            if (TRUE == Contain_AABB_Vertex(&pAreaInfo->m_wbvRegion.m_aabb, g_pHero->GetPosition()))
            {
                ShortcutSate = SHORTCUT_AREA_STATE_ENTER_AREA;			
            }
        }
        break;
    case BVT_PATH_TILE_INDEX:
        {
            int iTile = g_pHero->GetPathExplorer()->GetTile();
            for (int i = 0 ;i < static_cast<int>(pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount); ++i)
            {
                if (iTile == pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData[i])
                {
                    ShortcutSate = SHORTCUT_AREA_STATE_ENTER_AREA;
                    break;
                }
            }
        }
        break;
    case BVT_SPHERE:
        {
            if (TRUE == Contain_Sphere_Vertex(&pAreaInfo->m_wbvRegion.m_BvSphere, g_pHero->GetPosition()))
            {
                ShortcutSate = SHORTCUT_AREA_STATE_ENTER_AREA;			
            }
        }
        break;
    }
    
    HeroShortcutSate = g_pHero->GetShortcutState(dwAreaID);
    if( HeroShortcutSate != ShortcutSate )
    {
        // 들어갔을땐 언제나 패킷을 보낸다!!!
        if( ShortcutSate == SHORTCUT_AREA_STATE_ENTER_AREA )
        {
            WORD wIndex = MapInfoParser::Instance()->GetCurrentMapPortalIndex(sAreaID, g_pMap->GetCurrentFieldID());

            if (wIndex)
            {
                sMAP_ENTERENCE_PORTAL* entPortal = MapInfoParser::Instance()->GetMapPortal( wIndex );

                //미션포탈일경우
                if(entPortal && entPortal->byMapType!=0)
                {
                    //룸정보를 초기화:미션포탈은 로비를 거치지않고 바로가기때문
                    uiBattleZoneMan* pZoneMan = 
                        static_cast<uiBattleZoneMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));
                    if(pZoneMan)
                    {
                        pZoneMan->GetBattleRoomEntry().Init();
                    }

                    //Client->Server: (퀘스트or미션)포탈에 진입하겠다는SYN
                    MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN sync;
                    sync.syncType	= MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN::eSYNC_FIXED_PORTAL;			
                    sync.mapEnterancePortalIndex	= wIndex;	
                    GlobalFunc::SendPacket( CI_GAMESERVERIDX, (BYTE *)&sync, sizeof(sync));	
                }
                else
                {
                    MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN sync;
                    sync.m_wIndex = wIndex;
                    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sync, sizeof(sync));	
                }
            }
        }
        g_pHero->SetShortcutState( dwAreaID,ShortcutSate );
    }

    if (ShortcutSate == SHORTCUT_AREA_STATE_ENTER_AREA)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void BattleScene::ProcessMoveInfo(DWORD dwTick)
{
	MOVE_INFO_LIST::iterator iter,tempiter;

	iter = m_MoveInfoList.begin();
	while(iter != m_MoveInfoList.end())
	{
		(*iter).m_dwDelay += dwTick;

		if ((*iter).m_dwDelay >= 20000)
		{
			tempiter = iter;
			iter++;
			m_MoveInfoList.erase(tempiter);			
		}
		else
		{
			iter++;
		}
	}
}

void BattleScene::RenderMoveInfo()
{
	MOVE_INFO_LIST::iterator iter;

	TCHAR szTemp[256] = {0,};

	iter = m_MoveInfoList.begin();
	while(iter != m_MoveInfoList.end())
	{
		POINT pt2;
		RECT rc;
		g_pSunRenderer->GetScreenPos( &pt2, &iter->vFrom );		
		Snprintf(szTemp, 256-1, _T("ID : %ld"),iter->m_dwKey);
		
		rc.left = pt2.x;
		rc.right = rc.left + 200;
		rc.top = pt2.y;
		rc.bottom = rc.top + 20;

		g_pSunRenderer->SetZBufferTest(FALSE);
		g_pSunRenderer->x_pManagerTextOut->DrawText(szTemp,&rc,WzColor_RGBA(0,255,255,255));		
		g_pSunRenderer->SetColor(255,255,255);
		if (iter->bSuccess) 
        {
			g_pSunRenderer->RenderLine(iter->vFrom,iter->vTo,WzColor_RGBA(30,30,255,255),1.0f);
			g_pSunRenderer->RenderLine(iter->vFrom,iter->vActualTo,WzColor_RGBA(255,255,30,255),1.0f);
		}
		else
		{
			g_pSunRenderer->RenderLine(iter->vFrom,iter->vTo,WzColor_RGBA(255,30,30,255),1.0f);
		}

		g_pSunRenderer->SetZBufferTest(TRUE);

		iter++;

	}
}

void BattleScene::RegisterMoveInfo(HONGPOS Cur,HONGPOS Dest,DWORD dwKey,BOOL bThrust)
{

	MOVE_INFO info;

	info.bSuccess = TRUE;
	info.m_dwDelay = 0;
	info.m_dwKey = dwKey;
	info.vFrom = Cur.wvPos;
	info.vTo = Dest.wvPos;
	info.vActualTo = info.vFrom;

	m_pTestMoveInfoExplorer->Stop();

	if (Cur.wTile == -1) 
	{		
		g_pSunTerrain->PathExp_Reset(m_pTestMoveInfoExplorer,&Cur.wvPos, -1);		
		if (m_pTestMoveInfoExplorer->GetTile() == -1)
		{
			info.bSuccess =  FALSE;
		}		
	}
	else
	{
		g_pSunTerrain->PathExp_Reset(m_pTestMoveInfoExplorer,&Cur.wvPos,Cur.wTile);
	}

	if (m_pTestMoveInfoExplorer->GetTile() != -1)
	{
		WzVector vDist = Dest.wvPos - Cur.wvPos;
		vDist.z = 0.0f;

		g_pSunTerrain->PathExp_Thrust(m_pTestMoveInfoExplorer,&vDist,PTA_NO_WALK);

		if (!m_pTestMoveInfoExplorer->IsMoving())
		{
			info.bSuccess =  FALSE;			
		}
		else
		{
			info.vActualTo = m_pTestMoveInfoExplorer->GetTargetPos();
		}
	}

	if (!info.bSuccess && bThrust)
	{
		TCHAR szTemp[256];
		Sprintf(szTemp,_T("ID : %ld Thrust 이동 실패"),dwKey);
		GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_CHAT,szTemp);
	}

	m_MoveInfoList.push_back(info);
}

void BattleScene::AddSkillFieldEffect( Character* pChr, SkillFacade::ResultData* result_data )
{
    for (int a = 0; a < result_data->GetEffectResultCount(); ++a )
    {
        SkillFacade::EffectResult* effect_result = result_data->GetEffectResult(a);
        if( effect_result == NULL )
            continue;

        SKILL_RESULT_EFFECT* pResult = effect_result->m_pResult;

        DWORD lifeTime = 0;
        SkillScriptInfo* pInfo = SkillInfoParser::Instance()->GetSkillInfo( result_data->GetSkillCode() );
        assert(pInfo);
        if( pInfo == NULL )
            continue;

        //SkillEffectInfo* pEffectInfo = SkillEffectInfoParser::Instance()->GetSkillEffectInfo( pInfo->m_SkillClassCode );
        BASE_ABILITYINFO* pAInfo=pInfo->GetAbilityInfoByIndex((BYTE)pResult->m_wAbilityIndex);
        assert(pAInfo);

        if( pAInfo == NULL )
        {
#ifdef _DEV_VER
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "[assertion]pResult->m_wAbilityIndex: %d", pResult->m_wAbilityIndex);
#endif //_DEV_VER
            return;
        }

        if(pAInfo)
        {
            lifeTime = pAInfo->m_iParam[2];//세번째 파라미터가 유지 시간이다.
        }
        else
        {
            lifeTime = 10000;
        }

        //랜덤 투하 

        for(int j=0;j<pResult->m_byCount;j++)
        {
            if(pResult->m_byCount==0||pResult->m_byCount>=MAX_EFFECT_COUNT)
            {

                break;
            }

            //Todo:스킬 별로 이펙트 속성이 다르게 갈때 처리 현재는 하나로 간다.
            if(pAInfo->m_wAbilityID==eABILITY_RANDOM_AREA_ATTACK)
            {

                pResult->m_EffectInfo[j].m_Time = max(pResult->m_EffectInfo[j].m_Time,200);

                WzVector vVisiblePos = pResult->m_EffectInfo[j].m_wvEffectPos;
                vVisiblePos.z += 3.0;

                float fT = 0.0f;
                int iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vVisiblePos, &fT, -1.0f, 10.0f );

                if( iTileFocussing >= 0 )
                {
                    vVisiblePos.z -= fT;
                }

                WzID EffectID, EndEffectID;

                EffectID = StrToWzID("L071");
                EndEffectID = StrToWzID("L072");

                DWORD LifeTime = pResult->m_EffectInfo[j].m_Time+200;
                DWORD EndEffectTime = pResult->m_EffectInfo[j].m_Time;

                g_ProjectileManagerEx.CreateFieldEffect( vVisiblePos, EffectID, LifeTime, EndEffectID, EndEffectTime, 0 );

            }
            else
            {
                //SkillEffectFunc::CreateSkillEventEffectByFieldSkill( pInfo->m_SkillClassCode, pResult->m_EffectInfo[j].m_wvEffectPos );


                switch(pInfo->m_SkillClassCode)  
                {
                case eSKILL_STAR_FALL:
                case eSKILL_FIRE_FILA:
                    {
                        g_ProjectileManagerEx.CreateFieldEffect( pResult->m_EffectInfo[j].m_wvEffectPos, StrToWzID("I060"), lifeTime );
                    }
                    break;

                case eSKILL_POISON_RAIN:
                    {
                        g_ProjectileManagerEx.CreateFieldEffect( pResult->m_EffectInfo[j].m_wvEffectPos, StrToWzID("I020"), lifeTime );
                    }
                    break;

                case eSKILL_ITEM_MINE: // 폭탄(마인 스킬)
                    {
                        GlobalFunc::CreateEffectBeUpToOption( StrToWzID("E320"), pResult->m_EffectInfo[j].m_wvEffectPos);
                    }
                    break;

                }
            }
        }
    }
}

void BattleScene::RenderLightDirection()
{
	if (m_bRenderLightDirection) 
    {
		if (g_pHero)
		{
			WzVector vPos = g_pHero->GetVisiblePos();
			vPos.z += 1.0f;
			WzVector vPos2;
			WzVector vDir;

			vDir = g_pMap->GetWorldBase()->GetSunLightDir();
			
			vPos2 =  vPos + m_vTestDir;

			WzColor color = g_pMap->GetWorldBase()->GetSunLightColor();

			g_pSunRenderer->RenderPyramid(vPos2,vPos,color);

			
			
			for (int a = 0; a < MAX_NUM_LIGHT; a++)
			{
				const St_LightInformation *pInfo = g_pSunRenderer->GetLightInfo(a);
				if (pInfo)
				{
					if (pInfo->m_bValid)
					{
						if (pInfo->m_eltType == ELT_POINT) 
						{
							BvSphere bv;

							bv.m_fRadius = pInfo->m_fRange;
							bv.m_wvCenter = pInfo->m_wvPosition;

							WzColor colorback = g_pSunRenderer->GetDiffuseColor();								
							g_pSunRenderer->SetColor(pInfo->m_wcColor);
							g_pSunRenderer->RenderBvSphere(&bv);
							g_pSunRenderer->SetColor(colorback);
						}
					}
				}
			}					
		}
	}
}

void BattleScene::ShowLightDirection()
{
	m_bRenderLightDirection ^= 1;
}

void  BattleScene::ProcessRenderOption(LPDIMOUSESTATE2 pMouseState)
{
	if (m_bRenderLightDirection)
	{
		if ( g_Input.GetState( MOUSE_MBUTTON, KS_MOUSE_DOWN) )
		{
			m_vTestDir = g_pMap->GetWorldBase()->GetSunLightDir();
		}
		else if( g_Input.GetState( MOUSE_MBUTTON, KS_MOUSE_KEY) )
		{
			WzVector vDir;
			WzVector vNewDir;

			vDir = g_pMap->GetWorldBase()->GetSunLightDir();
			vDir = m_vTestDir;

			WzVector vDiff = vDir;
			vDiff.z = 0.0f;

			float fBase = VectorLength(&vDiff);
			float fPitch = atan2(vDir.z,fBase);			
			float fAngle = pMouseState->lX * 0.005f;

			vNewDir.x = vDir.x * cos(fAngle) + vDir.y * -sin(fAngle);
			vNewDir.y = vDir.x * sin(fAngle) + vDir.y * cos(fAngle);
			vNewDir.z = 0.0f;

			VectorNormalize(&vNewDir,&vNewDir);

			float fNewPitch = fPitch - pMouseState->lY * 0.005f;
			if (fNewPitch >= WZ_PI / 2.0f)
			{
				fNewPitch = WZ_PI / 2.0f - 0.001f;
			}
			else if (fNewPitch <= -(WZ_PI / 2.0f) )
			{
				fNewPitch = -(WZ_PI / 2.0f) + 0.001f;
			}

			vNewDir.z = sin(fNewPitch);

			vNewDir.x *= cos(fNewPitch);
			vNewDir.y *= cos(fNewPitch);

			VectorNormalize(&vNewDir,&vNewDir);

			m_vTestDir = vNewDir;
			
		}
		else if ( g_Input.GetState( MOUSE_MBUTTON, KS_MOUSE_UP) )
		{
			WzColor color = g_pMap->GetWorldBase()->GetSunLightColor();
			g_pMap->GetWorldBase()->SetSunLightDir(m_vTestDir);
			g_pMap->GetWorldBase()->SetSunLightColor(color);
		}
	}
}

void BattleScene::_renderDbgText(DWORD dwTick)
{
#ifdef _DEV_VER
	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

	if (m_iDbgTextType)
	{
        int x = 400;
        int y = 5;

		// 준비 과정
		switch(m_iDbgTextType) 
		{
		case eDebugTextType_RenderState:
			_renderDbgInfoRenderStates(x,y);
            _renderDbgInfoResourceStatus(x,y);
			break;

		case eDebugTextType_HeroStatus:
			_renderDbgInfoHeroStatus(x,y);
			break;	

		case eDebugTextType_Resource:
			_renderDbgInfoResourceStatus(x,y);
			break;	

		case eDebugTextType_KeyQueue:
			_renderDbgInfoKeyQueueNum(x,y);
			break;

		case eDebugTextType_SkillQueue:
			_renderDbgInfoSkillQueueNum(x,y);
			break;

		case eDebugTextType_Camera:
			_renderDbgInfoCameraSpring(x,y);
			break;

		case eDebugTextType_Monster:
			_renderDbgInfoMonsterInfo(x,y);
			break;

        case eDebugTextType_Sound:
            _renderDbgInfoSound(x,y);
            break;

        case eDebugTextType_Autobot:
            _renderDbgInfoAutobot(x,y);
            break;
		}
	}

	// 마무리
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
#endif //_DEV_VER
}



void BattleScene::SetScreenShotState( int iState )
{
	m_iScreenShotState = iState;
}

int BattleScene::GetScreenShotState( void )
{
	return m_iScreenShotState;
}

void BattleScene::RenderBlindEffect()
{
	if (m_iCurBlindRate)
	{
		g_pSunRenderer->Set2DMode(TRUE);

        POINT pt = g_pApplication->Get2DModeMapping();
		RECT rc;

		rc.top = 0;
		rc.left = 0;
		rc.right = pt.x;
		rc.bottom = pt.y;

		if( m_BlindType == BLIND_TYPE_BLACK )
        {
            float fRate = (float)m_iCurBlindRate / (float)BLACK_BLIND_TIME;
            int iAlpha = (int)(255.0f * fRate);

		    WzColor color = WzColor_RGBA(0,0,0,iAlpha);
            g_InterfaceManager.RenderRect(rc,color);
        }
		else if( m_BlindType == BLIND_TYPE_WHITE )
        {
            float fRate = (float)m_iCurBlindRate / (float)WHITE_BLIND_TIME;
            static float s_max_rate = 0.96f;
            if( fRate > s_max_rate )
                fRate = s_max_rate;

            int iAlpha = (int)(255.0f * fRate);

            WzColor color = WzColor_RGBA(255,255,255,iAlpha);
            g_InterfaceManager.RenderRect(rc,color);
        }

		g_pSunRenderer->Set2DMode(FALSE);
	}
}

void BattleScene::ProcessExpList()
{
	EXP_INFO_LIST::iterator iter,tempiter;

	iter = m_ExpList.begin();
	while(iter != m_ExpList.end() )
	{				
		BOOL bPop = FALSE;

		Character * pChr = (Character *)g_ObjectManager.GetObject(iter->m_dwCharacterKey);
		if (!pChr)
		{			
			bPop = TRUE;
		}
		else
		{
			if (pChr->IsDead())
			{
				bPop = TRUE;
			}
		}

		if (!bPop)
		{
			iter++;
		}
		else
		{
			tempiter = iter;
			iter++;

			PopupExp(tempiter->m_dwExpOffset , tempiter->m_nBonusExpRatio);

			m_ExpList.erase(tempiter);
		}		
	}
}

void BattleScene::PopupExp(EXPTYPE expoffset , int bonusExpRatio /* = 0  */, int ExpType /* = eNormalExp */)
{
    if (g_InterfaceManager.GetShowFlag() == FALSE)
    {
        return;
    }

	// 보너스 경험치를 구한다.
	EXPTYPE bonusExp = 0;
	if( bonusExpRatio > 0)
	{
		bonusExp =  (EXPTYPE) ceil( (double)expoffset *  ( (double) bonusExpRatio / (100.f + (double) bonusExpRatio) ) );
	}

	if (g_pHero) 
	{
		TCHAR szResult[MAX_MESSAGE_LENGTH] ={ '\0', };
		TCHAR szMessage[MAX_MESSAGE_LENGTH] = { '\0', };
		TCHAR szText[MAX_MESSAGE_LENGTH] = { '\0', };
		TCHAR szExpMessage[MAX_MESSAGE_LENGTH] = { '\0', };

		if (expoffset > 0)
		{
			if(ExpType == eGroupExp)
			{
				// 7041: 그룹 멤버로부터 %d 경험치를 보상 받았습니다.
				g_InterfaceManager.GetInterfaceString( 7041, szMessage, MAX_MESSAGE_LENGTH );
			}
			else
			{
				g_InterfaceManager.GetInterfaceString( eST_GET_EXP, szMessage, MAX_MESSAGE_LENGTH );
			}

			if (bonusExp > 0)
			{
				Sprintf( szResult, szMessage, expoffset - bonusExp);
				// 9050  %s (추가 경험치 %d획득)
				g_InterfaceManager.GetInterfaceString( 9050, szText, MAX_MESSAGE_LENGTH );
				Sprintf( szExpMessage, szText, szResult, bonusExp );
			}

			else
			{
				Sprintf( szExpMessage, szMessage, expoffset );
			}
		}
		else
		{

			if ( g_pHero->IsReachMaxLevel() )
			{
				//5028	최고 제한 레벨에 도달하여 더 이상 경험치를 획득 하실 수 없습니다.
				g_InterfaceManager.GetInterfaceString(5028, szResult, INTERFACE_STRING_LENGTH);
			}
			else
			{
				g_InterfaceManager.GetInterfaceString(eST_TIRED_SYSTEM_ITEM_NOT_GET, szResult, MAX_MESSAGE_LENGTH);
			}
		}

		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szExpMessage, CHAT_TYPE_SYSTEM );

		if( GAMEOPTION->GetPickUpItemNotify() )
		{
			InserCenterTextMessage( szExpMessage, PICK );
		}
			
		g_pHero->CreatePopupMesh(0, expoffset, eDAMAGEPOPUP_EXP, WzColor_RGBA(255,255,255,255), FALSE, FALSE);
	}
}


//-------------------------------------------------------------------------------------------
/**
*/
void BattleScene::InitQuestAreaInfo()
{
	m_VecQuestArea.clear();
	if ( !g_pMap )
	{
		return;
	}
	
	int Count = g_pMap->GetNumberOfSpecialArea();
	WzSpecialArea* pArea = NULL; 

	for (int Index = 0; Index < Count; ++Index )
	{
		pArea = g_pMap->GetMapAreaByIndex(Index);
		if (pArea)
		{
			m_VecQuestArea.push_back( pArea->m_wiIdentity );
		}
	}

}

//-------------------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessQuestArea()
{
	for (size_t i = 0; i < m_VecQuestArea.size(); ++i)
	{
		GameFunc::CheckHeroInAreaForQuest( m_VecQuestArea[i] );
	}	
}





//-------------------------------------------------------------------------------------------
/**
*/
void BattleScene::ProcessQuestTime()
{
	uiQuestMan * pQuestMan = 
		(uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);
	if ( pQuestMan )
	{
		pQuestMan->UpdateQuestTime();
	}	
}





BOOL BattleScene::SceneWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam ,SI_MESSAGE *stMessage)
{
	if ( m_EventText.IsActived() )
	{
		return TRUE;
	}
	
	return SceneBase::SceneWndProc(hWnd,iMessage,wParam,lParam,stMessage);
}

//-------------------------------------------------------------------------------------------
/**
*/
void BattleScene::KeyManagerProc(DWORD wParam,DWORD lParam)
{
	switch(wParam)
	{
	case eSceneEventSkip_EventScript:
		{
            m_EventText.SkipEventScript();
		}
		break;
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void BattleScene::InitQuestInfo()
{
	if ( g_pQMGR_Real == NULL )
        return;

    Quest * pQuest = NULL;

    uiQuestMan * pQuestMan = 
        (uiQuestMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);

    // 자동 수락된 목록 등록
    for( int i = 1; i < MAX_QUEST_COMPL_NUM; ++i)
    {
        pQuest = g_pQMGR_Real->FindQuest(i);

        if ( !pQuest )
            continue; 

        //완료조건이 아무것도 없는 퀘스트를 위해서 무조건 한번 이벤트를 호출한다.
        g_pQMGR_Real->OnEventForClient(QUEST_EVENT_ITEM, NULL, pQuest);					
    }

    UpdateAutoRewardRequest();
    RefreshNPCStateForQuest();
}



//-------------------------------------------------------------------------------------------
/**
*/
void BattleScene::OnDeviceReset()
{
	g_InterfaceManager.UpdateMyHeadTexture();
	for (int a = 0; a < MAX_PARTYMEMBER_NUM; a++)
	{
		g_InterfaceManager.UpdatePartyHeadTexture(a);
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void BattleScene::SendHeroSectorIndex()
{
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
	if (g_pHero && g_pMap)
	{
		if (!IsLobby()) 
        {
			int iIndex = -1;
			WzVector vPos = g_pHero->GetPosition();
			iIndex = g_pMap->GetSectorIndex(&vPos);

			//if (iIndex != -1) 
            if (iIndex >= 0) 
			{
				if (iIndex != m_iPrevSectorIndex)
				{
					MSG_CW_VIEWPORT_CHARSTATE SendPacket;
					SendPacket.m_byCategory = CW_VIEWPORT;
					SendPacket.m_byProtocol = CW_VIEWPORT_CHARSTATE;
					SendPacket.dwFieldCode = g_pMap->GetCurrentFieldID();
					SendPacket.dwSectorIndex = (DWORD)iIndex;

					if (GlobalFunc::SendPacket(CI_CHATSERVERIDX, &SendPacket, sizeof(SendPacket) ) == TRUE)
					{
						m_iPrevSectorIndex = iIndex;
					}
				}
			}
		}
	}
#endif
}

//----------------------------------------------------------------------------
/**
*/
BOOL BattleScene::IsLobby()
{
	return BOOL(BattleScene::m_bLobbyFlag);
}


//----------------------------------------------------------------------------
/**
*/
BOOL BattleScene::IsLobbyInPending()
{
	return BOOL(BattleScene::m_bLobbyPendingFlag);
}


//----------------------------------------------------------------------------
/**
*/
BOOL BattleScene::IsLobbyOutPending()
{
	return BOOL(BattleScene::m_bLobbyOutPendingFlag);
}


//----------------------------------------------------------------------------
/**
*/
VOID BattleScene::SetLobbyFlag(BOOL bFlag)
{	
	BattleScene::m_bLobbyFlag = bFlag;
}


//----------------------------------------------------------------------------
/**
*/
VOID BattleScene::SetLobbyInPending(BOOL bFlag)
{
	BattleScene::m_bLobbyPendingFlag = bFlag;
}

VOID BattleScene::SetLobbyOutPending(BOOL bFlag)
{
	BattleScene::m_bLobbyOutPendingFlag = bFlag;
}
//
//BOOL BattleScene::CreateProjectileSkillResult(DAMAGETYPE wDamage,DWORD dwTargetKey,DWORD dwTargetHP,DWORD timeSerial,DWORD dwSkillCode,DWORD dwAttackerKey)
//{
//	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillCode);
//	Character *pTarget = (Character *)g_ObjectManager.GetObject(dwTargetKey);
//	Character *pAttacker = (Character *)g_ObjectManager.GetObject(dwAttackerKey);
//
//
//	if(pInfo && pTarget && pAttacker)
//	{
//		switch(pInfo->m_SkillClassCode)
//		{
//		case eSKILL_SACRED_FIRE:
//			{
//				ATTACK_RESULT attret;
//				attret.SetSkillCode(dwSkillCode);
//				attret.SetAttackSerial(pAttacker->GetCurrentAttackSerial());
//				attret.SetTimeSerial(timeSerial);
//				attret.GetTargetData().SetTargetHP( dwTargetHP );
//				attret.GetTargetData().SetTargetObjectKey( dwTargetKey );
//				attret.GetTargetData().SetDamage( wDamage );
//				pAttacker->AddAttackResult(&attret);
//
//
//                //WzVector StartPosition = pTarget->GetVisiblePos();
//                //StartPosition.z += 10.0f;
//                //WzVector TargetPosition = pTarget->GetVisiblePos();
//
//                //g_ProjectileManagerEx.CreateMoveEffect( StartPosition, TargetPosition, StrToWzID("K501"), 500, pAttacker->GetObjectKey(), pAttacker->GetCurrentAttackSerial() );
//
//                //g_ProjectileManagerEx.CreateFieldEffect( pTarget->GetVisiblePos(), StrToWzID("K502"), 2000, 0, 0, 600 );
//			}
//			return TRUE;
//		}
//	}
//
//	return FALSE;
//}


void BattleScene::PrintYoungManProtectMessage()
{
	static int iHour = 0;
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,} , szResult[INTERFACE_STRING_LENGTH] ={0,};

	int curHour = m_iGameConnectedTime / 60;

	if( iHour < curHour )
	{
		// 청소년 보호 메세지 출력.
		iHour = curHour;

		if( iHour < 3)
		{
			g_InterfaceManager.GetInterfaceString(eST_CONNECT_TIME_UNDER_3,szMessage,INTERFACE_STRING_LENGTH);
			Sprintf(szResult,szMessage,iHour);
		}
		else
		{
			g_InterfaceManager.GetInterfaceString(eST_CONNECT_TIME_OVER_3,szMessage,INTERFACE_STRING_LENGTH);
			Sprintf(szResult,szMessage,iHour);
		}
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szResult);	

	}
}



void BattleScene::PrintPcBangBanMessage()
{
	static int iMinute = 0;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	// m_iTime이 10분 단위로 갱신된다. 10분에 한번씩 뿌려준다.
	if( iMinute <  m_iGameConnectedTime )
	{
		iMinute = m_iGameConnectedTime;
		if( g_pHero && g_pHero->GetBlockPCBang() )
		{
			// 5865	PC방 요금 미납자로 신고가 접수되었습니다. 게임 이용에 불이익을 받으실 수 있습니다.
			g_InterfaceManager.GetInterfaceString( 5865 , szMessage ,INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_PCBang_Notice(szMessage);//함수내에서 시간 조절.

		}	
	}
}


void BattleScene::UpdateGameStayTime()
{
	if( m_StayTimer.IsExpired() )
	{
		m_StayTimer.SetTimer(TEN_MINUTE);
		int iSecondToMinute = TEN_MINUTE / 60000;
		m_iGameConnectedTime += iSecondToMinute;
	}
}

#ifdef _JBH_ADD_AGE_LIMIT_MARK
void BattleScene::RenderGameRatingMark()
{
	static	int		iHour = 0;
	static	DWORD	dwTick = 0;
	// m_iGameConnectedTime 의 갱신 주기는 10분 이다.
	if( iHour < m_iGameConnectedTime / 60 )
	{
		iHour = m_iGameConnectedTime / 60;
		dwTick = clock_function::GetTickCount();
	}

	if( dwTick + 3000 > clock_function::GetTickCount() )
	{
		SceneBase::RenderGameRatingMark();
	}
}
#endif //_JBH_ADD_AGE_LIMIT_MARK

void BattleScene::UpdateItemOwner()
{
    ItemManager::Instance()->UpdateOwnerList();
}

void BattleScene::UpdateItemShareSystem()
{
	uiPartyMan * pPartyMan = ( uiPartyMan *) g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY);
	if( pPartyMan )
	{
		pPartyMan->UpdatePartyItemShareSystem();
	}
}

void BattleScene::NoticeBattleZoneItemShareSystem()
{
	if( GameFramework::GetCurrentScene() &&
		GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FIELD || 
		GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE )
		return;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	//5220	배틀존에서의 아이템 분배방식은 특정 아이템만 적용됩니다.
	g_InterfaceManager.GetInterfaceString(5220 , szMessage ,INTERFACE_STRING_LENGTH);
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
}


int BattleScene::OnEndRendering( DWORD /*dwTick*/ )
{
	if( ScreenShotSaver::Instance()->IsPrepared() )
	{
		ScreenShotSaver::Instance()->Save();
	}

	return 0;
}

void BattleScene::OnRecvCG_SSQ_CTRL_CMD(const WAR_TAG_P IN pMsg, const WAR_MSG_HDR IN header)
{
	using namespace nsSSQ;
	using namespace nsSSQ::Observer;


	TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = { '\0', };
	int	  MessageType = eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT;
	WAR_TAG_P pIt	= const_cast<WAR_TAG_P>(pMsg);
	WAR_TAG_P pEnd	= pIt + header.size;

	for (WORD size = 0; pIt < pEnd; pIt += size)
	{
		size	= 10000;	// 미 설정에 대한 스킵 설정 (buffer-overrun protector)
		switch (pIt->_TAG)
		{
			//─────────────────────────────────────────
			//	C<->G SSQ_MESSAGE_HANDLERs
			//	각 프로토콜을 구현하는 메시지는 MSGSUB_으로 시작한다.
			//	(protocol) SSQ_PROTOCOL_blah~blah~ --> (structure) MSGSUB_SSQ_PROTOCOL_blah~blah~
			//	이와 같은 샘플 방식대로 구현시 아래 예제와 같은 방식으로 각 case문을 시작할 것
			//	이렇게 하지 않을 경우 컴파일 에러 또는 런타임 경고 등이 발생하게 되어 있지만
			//	고려되지 않는 사항도 있을 수 있다.
			//
			//	case SSQ_PROTOCOL_blah~blah~:
			//		{
			//			const MSGSUB_SSQ_PROTOCOL_blah~blah~& rMsg = static_cast<MSGSUB_SSQ_PROTOCOL_blah~blah~&>(*pIt);
			//			size = rMsg.GetSize();
			//			...
			//─────────────────────────────────────────
		case SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN );

				printf( "[SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN] \n" );

				g_InterfaceManager.GetInterfaceString( 6412, szMessage );

				TCHAR szBuff[INTERFACE_STRING_LENGTH];

				Sprintf( szBuff, szMessage, rMsg.Countdown );

				GlobalFunc::OutputMsg_Notice( MessageType, szBuff );

			}
			break;

		case SSQ_PROTOCOL_NOTICE_ENABLE_ENTER:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_ENABLE_ENTER );

				printf( "[SSQ_PROTOCOL_NOTICE_ENABLE_ENTER] \n" );

				g_InterfaceManager.GetInterfaceString( 6418, szMessage );
				GlobalFunc::OutputMsg_Notice( MessageType, szMessage );

#ifdef _DH_BATTLEZONE2_
                uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
                if (battlezone2_list)
                {
                    battlezone2_list->SetIsSSQOpen(true);
                }
#endif//_DH_BATTLEZONE2_
			}
			break;
		case SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN );

				printf( "[SSQ_PROTOCOL_NOTICE_CLOSE_ENTER_COUNTDOWN] \n" );

				g_InterfaceManager.GetInterfaceString( 6413, szMessage );

				TCHAR szBuff[INTERFACE_STRING_LENGTH];
				Sprintf( szBuff, szMessage, rMsg.Countdown );

				GlobalFunc::OutputMsg_Notice( MessageType, szBuff );
			}
			break;
			//	다음 메시지는 가변 스트림의 마지막에 붙는 메시지, 스트림 검증용이지만 
			//	현 시점 실질적인 사용은 하지 않는다. (하지만 구현은 해 둘 것.)
		case SSQ_PROTOCOL_CTRL_TAG_TAIL:
			{
				printf( "[SSQ_PROTOCOL_CTRL_TAG_TAIL] \n" );
			}
			break;
		case SSQ_PROTOCOL_NOTICE_CLOSE_PORTAL:
			{	
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_CLOSE_PORTAL );
			
				g_InterfaceManager.GetInterfaceString( 6414, szMessage );
				GlobalFunc::OutputMsg_Notice( MessageType, szMessage );

#ifdef _DH_BATTLEZONE2_
                uiBattlezone2List* battlezone2_list = GET_CAST_DIALOG(uiBattlezone2List, IM_BATTLEZONE2_MANAGER::BATTLEZONE2_LIST);
                if (battlezone2_list)
                {
                    battlezone2_list->SetIsSSQOpen(false);
                }
#endif//_DH_BATTLEZONE2_
			}
			break;
		case SSQ_PROTOCOL_NOTICE_CLOSE_ENTER:
			{				
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_CLOSE_ENTER );
			}
			break;
		case SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED );
			}
			break;
		case SSQ_PROTOCOL_NOTICE_GAMBLE_START:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_GAMBLE_START );

				m_dwGambleTime = 0;
				m_bGambleStart = TRUE;
	
				TCHAR szMessage[INTERFACE_STRING_LENGTH +1];
				g_InterfaceManager.GetInterfaceString( 6419, szMessage );
				GlobalFunc::default_OutputMsg( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, szMessage );

			}
			break;
		case SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT );

				if( g_pHero == NULL )
					break;

				TCHAR szTemp[INTERFACE_STRING_LENGTH +1];
				TCHAR szMessage[INTERFACE_STRING_LENGTH +1];

				g_InterfaceManager.GetInterfaceString( 1769, szTemp );

				const std::string strMoney = GlobalFunc::CalculateThousandDigit( rMsg.RewardHeim );
				
				Sprintf( szMessage, szTemp, (TCHAR*)rMsg.CharName, strMoney.c_str() );

				if( _tcscmp( (TCHAR*)rMsg.CharName, g_pHero->GetName() ) == 0 )
				{
					MONEY	money = g_pHero->GetMoney() + rMsg.RewardHeim;
					g_pHero->SetMoney( money );
				}

				GlobalFunc::default_OutputMsg( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, szMessage );
			}
			break;
		case SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN );

				uiSSQSystemWindow * pDlg = GET_CAST_DIALOG( uiSSQSystemWindow, IM_SSQ_MANAGER::SSQ_SYSTEMTIME );

				if( pDlg )
				{
					pDlg->SetStart( TRUE );
					pDlg->SetStartCount( 10 );
				}
			}
			break;
		case SSQ_PROTOCOL_NOTICE_BATTLE_START:	
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_BATTLE_START );
					
				// 전투 시작시 BGM 교체
				g_SoundBGM.Stop();
				g_SoundBGM.Play( 29 );
				g_IsBattle = TRUE;
			}
			break;
		case SSQ_PROTOCOL_NOTICE_BATTLE_END:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_BATTLE_END );
				g_IsBattle = FALSE;
			}
			break;
		case SSQ_PROTOCOL_NOTICE_1TO1_PORTAL_CLOSED:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_NOTICE_1TO1_PORTAL_CLOSED );

				g_ProjectileManagerEx.DeleteProjectile( g_dwSSQPortalKey );
			}
			break;
		case SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION );

				const SSQPattern * pPattern = SSQInfoParser::Instance()->FindSSQPattern( rMsg.PatternIndex );
				if( pPattern == NULL )
					break;

				for( int i = 0; i < pPattern->m_StepGroup[rMsg.StepArrayIndex].m_ListSize; i++ )
				{
					g_SsqScene.AddEarthQuakeIndex( pPattern->m_StepGroup[rMsg.StepArrayIndex].m_List[i] );
				}

			}
			break;
		case SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE );

				g_SsqScene.ProcessEarthQuake();
			}
			break;
		case SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE );

				for( int i = 0; i < rMsg.Count; i++ )
				{
					g_SsqScene.CreateThunderBolt( rMsg.Position[i] );
				}

			}
			break;
		case SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE );

				for( int i = 0; i < rMsg.Count; i++ )
				{
					g_SsqScene.ProcessDamage( rMsg.Damages[i] );
				}
			}
			break;
		case SSQ_PROTOCOL_TRAP_SELF_DESTRUCTION:
			{
			}
			break;
		case SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS );

				uiSSQSystemWindow * pDlg = GET_CAST_DIALOG( uiSSQSystemWindow, IM_SSQ_MANAGER::SSQ_SYSTEMTIME );

				if( pDlg )
				{
					pDlg->SetRemainUser( rMsg.Remains, 100 );
				}
			}
			break;
		case SSQ_PROTOCOL_INFORMATION_REMAINED_TIME:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_REMAINED_TIME );

				uiSSQSystemWindow * pDlg = GET_CAST_DIALOG( uiSSQSystemWindow, IM_SSQ_MANAGER::SSQ_SYSTEMTIME );

				if( pDlg )
				{
					pDlg->SetRemainTime( rMsg.Minute, rMsg.Second );
				}
			}
			break;
		case SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_REWARD_ON_PLAYING );


				TCHAR szTemp[INTERFACE_STRING_LENGTH +1];
				TCHAR szMessage[INTERFACE_STRING_LENGTH +1];

				g_InterfaceManager.GetInterfaceString( 6423, szTemp );
				Sprintf( szMessage, szTemp, (TCHAR*)rMsg.CharName );

				GlobalFunc::default_OutputMsg( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, szMessage );				
			}
			break;
		case SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION );

				for( int i = 0; i < rMsg.Count; i++ )
				{
					Object * pObj = g_ObjectManager.GetObject( rMsg.PositionInfo[i].ObjectKey );

					if( pObj == NULL )
						continue;

					WzVector vTemp = rMsg.PositionInfo[i].Position;
					vTemp.z = 20.0f;
					WzVector pos;

					GameFunc::PickTerrainMesh( vTemp, pos );
					
					pObj->SetPosition( pos );
				}
			}
			break;
		case SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS );
				
				for( int i = 0; i < rMsg.Count; i++ )
				{
					g_SsqScene.ProcessDamage( rMsg.Damages[i], 7061 );
				}
			}
			break;
		case SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED:
			{	
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED );

				g_SsqScene.DeleteDoomEffect();
			}
			break;
		case SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_AMOUNT_DEDUCTED );

				if( g_pHero == NULL )
					break;

				MONEY money = g_pHero->GetMoney();
				money -= rMsg.Cost;
				g_pHero->SetMoney( money );
			}
			break;

		case SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_G2C_SELECT:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_G2C_SELECT );

				uiAssistMan* pUIMan = GET_DIALOG_MANAGER(uiAssistMan, UIMAN_ASSIST);
				if(pUIMan)
				{
					pUIMan->ShowAskSSQObserverDialog();
				}				
			}
			break;
		case SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT );

				using namespace nsSSQ::Observer;

				if( rMsg.Result == MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT::eNtfCmd_ResultByRequest_Success )
				{
					if( rMsg.Coordinates == Info::eCoord_Slots )
						break;

					const WzVector vAngle = GlobalFunc::GetObserverAngle( rMsg.Coordinates );

					g_Camera.SetAngleYaw( vAngle.x );
					g_Camera.SetAnglePitch( vAngle.y );

					g_pHero->SetAngle( vAngle.x );

					if( g_bObserverMode == FALSE )
					{
						g_bObserverMode = TRUE;
						MainDialog * pDlg = GET_CAST_DIALOG( MainDialog, IM_MENU_MANAGER::MAIN_DLG );

						if( pDlg )
						{
							pDlg->ShowObserverButton( TRUE );
						}

						if( g_pHero )
							g_pHero->SetShow( FALSE );

						SetShowInterfaceForObserverMode( FALSE );
					}
				}
				else
				{
					
				}
			}
			break;
		case SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT );

				TCHAR szMessage[INTERFACE_STRING_LENGTH +1];

				if( rMsg.bIsNotStarted )
				{
					g_InterfaceManager.GetInterfaceString( 5598, szMessage );
				}
				else
				{
					g_InterfaceManager.GetInterfaceString( 6426, szMessage );
				}

				GlobalFunc::default_OutputMsg( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, szMessage );				

			}
			break;
		case SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT );

				TCHAR szTemp[INTERFACE_STRING_LENGTH +1];
				TCHAR szMessage[INTERFACE_STRING_LENGTH +1];

                if (strlen(static_cast<TCHAR*>(rMsg.guild_name)) != 0)
                {
                    // 6431 : 이번 '타락한 사원' 최고의 투사는 "%s 길드의 %s"님 입니다! 최고의 투사에게 '빛나는 투사의 상자'가 수여됩니다.
                    g_InterfaceManager.GetInterfaceString( 6431, szTemp );
                    Sprintf( szMessage, szTemp, static_cast<TCHAR*>(rMsg.guild_name), static_cast<TCHAR*>(rMsg.WinnerName));
                }
                else
                {
                    // 6427 : 이번 '타락한 사원' 최고의 투사는 "%s"님 입니다! 최고의 투사에게 '빛나는 투사의 상자'가 수여됩니다.
                    g_InterfaceManager.GetInterfaceString( 6427, szTemp );
                    Sprintf( szMessage, szTemp, static_cast<TCHAR*>(rMsg.WinnerName) );
                }

                uiNoticeMan* pUIMan = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
                if (pUIMan != NULL)
                {
                    pUIMan->ShowNoticeMessage(eGM_Notice, szMessage, 10000);
                }
			}
			break;

		default:
			{
				WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER | c_wOUTPUT_LOG_SYSCHAT, "Can't Readable SSQ Msg Format (Tag = %d)\n ", pIt->_TAG );
				
			}
			break;
		}
	}

	ASSERT( pIt==pEnd );

}


void BattleScene::InserCenterTextMessage( char * szText, DWORD eType )
{
	WzColor color;

	if ( eType == PICK )
		color = WzColor_RGB(255,255,255);
	else
		color = WzColor_RGB(0,255,0);


	TEXT_MEMBER_SHARED_PTR_TYPE data( new Text_Member( szText, color ) );
	m_TextDisplayManager.Insert( data );
}

void BattleScene::SetShowInterfaceForObserverMode( BOOL bShow )
{
#ifdef _NA_000000_20130114_RENEWER_UI
	DWORD dwDialogID[4] = { IM_HERO_GAUGE_MANAGER::HERO_GAUGE_DLG, 
		IM_QUICK_CONTAINER_MANAGER::QUICK_CONTAINER,
		IM_QUICK_CONTAINER_MANAGER::STYLE_CONTAINER,
		IM_HERO_GAUGE_MANAGER::HERO_GAUGE_DLGCONTROL
	};

	for( int i = 0; i < 4; i++ )
	{
		uiBase * pDlg = GET_DIALOG( dwDialogID[i] );

		if( pDlg )
			pDlg->ShowWindow( bShow );
	}
#else
	DWORD dwDialogID[3] = { IM_HERO_GAUGE_MANAGER::HERO_GAUGE_DLG, 
		IM_QUICK_CONTAINER_MANAGER::QUICK_CONTAINER,
		IM_QUICK_CONTAINER_MANAGER::STYLE_CONTAINER };

	for( int i = 0; i < 3; i++ )
	{
		uiBase * pDlg = GET_DIALOG( dwDialogID[i] );

		if( pDlg )
			pDlg->ShowWindow( bShow );
	}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
}

void BattleScene::ShowRadirDialog()
{
    if (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_SSQ)
    {
		static DWORD countMapOpen = 0;

        if (g_pHero && (g_pHero->IsShowFullMap() == FALSE))
        {
#ifdef _NA_000000_20130114_RENEWER_UI
			RadarDialog* radar_dialog = 
				(RadarDialog*)GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG);
			RadarPlusDialog* radar_plus = 
				(RadarPlusDialog*)GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);

			if (!radar_dialog || !radar_plus)
			{
				//단 하나라도 포인트가 없다면 처리 하지 않음;
				goto OUT_RESULT;
			}

			int radarLevel = 0;
			//레이더 단계 계산
			{
				if(radar_dialog->IsVisible() && radar_dialog->gethidecheck() == false)
				{
					radarLevel = 1;
				}
				if(radar_dialog->IsVisible() && radar_dialog->gethidecheck())
				{
					radarLevel = 0;
				}
				if(radar_plus->IsVisible() && radar_plus->gethidecheck() == false)
				{
					radarLevel = 2;
				}

				if(radar_plus->IsVisible() && radar_plus->gethidecheck())
				{
					radarLevel = 3;
				}
			}

			//level 단계
			switch(radarLevel)
			{
			case 0:
				{
					radar_plus->ShowDialog(FALSE);
					radar_dialog->ShowDialog(TRUE);
					radar_dialog->HideAllControl(true);
					
				}
				break;
			case 1:
				{
					radar_dialog->ShowDialog(FALSE);
					radar_plus->ShowDialog(TRUE);
					radar_plus->HideAllControl(true);
				}
				break;
			case 2:
				{
					radar_dialog->ShowDialog(FALSE);
					radar_plus->ShowDialog(TRUE);
					radar_plus->HideAllControl(false);
				}
				break;
			case 3:
				{
					radar_plus->ShowDialog(FALSE);
					radar_dialog->ShowDialog(TRUE);
					radar_dialog->HideAllControl(false);
					countMapOpen = 0;
				}
				break;

			}

#else
			if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->IsVisible())
			{
				GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->ShowDialog(FALSE);
				GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->ShowDialog(TRUE);
			}
			else if (GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->IsVisible())
			{
				GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->ShowDialog(FALSE);
				GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->ShowDialog(FALSE);
			}
			else
			{
				GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->ShowDialog(TRUE);
				GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->ShowDialog(FALSE);
			}
#endif

OUT_RESULT:
            uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);
            if (ui_battlezone2_manager_ != NULL)
            {
                ui_battlezone2_manager_->MoveBattleZoneIcon();
            }

        }
    }
}

//------------------------------------------------------------------------------
//_NA_000000_20120410_SPEEDHACK
void BattleScene::CallbackWGPL(void)
{
    static bool is_sended = false;
    if (is_sended == true)
    {
        return;
    }
    if (g_pHero == NULL)
    {
        return;
    }

    MSG_CG_ETC_MEMORY_HACK_CMD cmd;

    if(GlobalFunc::SendPacketToGameServer(&cmd, sizeof(cmd)) == TRUE)
    {
        is_sended = true;
    }
    else
    {
        assert( !"패킷 Send 에러 - MSG_CG_SYNC_PLAYER_ENTER_SYN" );
    }
}
//------------------------------------------------------------------------------
#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
void BattleScene::SetCurrentRadarLayer(int current_layer)
{
    RadarDialog * pRadarDlg = GET_CAST_DIALOG(RadarDialog, IM_RADAR_MANAGER::RADAR_DIALOG);
    if(pRadarDlg)
    {
        pRadarDlg->SetCurLayer(current_layer);
    }

    RadarPlusDialog * pRadarPlusDlg = GET_CAST_DIALOG(RadarPlusDialog, IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
    if(pRadarPlusDlg)
    {
        pRadarPlusDlg->SetCurLayer(current_layer);
    }
}
//------------------------------------------------------------------------------
void BattleScene::InitRadarLayer(MAPCODE map_code)
{
    int iMaxLayer = 1;

    INDUN_OPTION* pOption = InDunOptionParser::Instance()->GetInDunOption(map_code);
    if (pOption)
    {
        iMaxLayer = pOption->mdwLayer;
    }


    RadarDialog* pRadarDlg = GET_CAST_DIALOG(RadarDialog, IM_RADAR_MANAGER::RADAR_DIALOG);
    if (pRadarDlg)
    {
        pRadarDlg->SetMaxLayer(iMaxLayer);
        pRadarDlg->SetCurLayer();
    }

    RadarPlusDialog* pRadarPlusDlg = GET_CAST_DIALOG(RadarPlusDialog, IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
    if (pRadarPlusDlg)
    {
        pRadarPlusDlg->SetMaxLayer(iMaxLayer);
        pRadarPlusDlg->SetCurLayer();
    }
}
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY
//------------------------------------------------------------------------------