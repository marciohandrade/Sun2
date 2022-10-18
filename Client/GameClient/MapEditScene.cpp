//------------------------------------------------------------------------------
// PVPScene.cpp
//
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "GameFramework.h"
#include "Player.h"
#include "Monster.h"
#include "mapeditscene.h"
#include "TerrainObject.h"
#include "varpacket.h"
#include "Camera.h"
#include "ItemManager.h"
//#include "HotKeyManager.h"
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
#include "PlayerAttributes.h"
#include <SCItemSlot.h>
#include "VillageScene.h"
#include "NPC.h"
#include "EventInfoParser.h"
#include "uiPartyMember/uiPartyMember.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiGuildCreate/uiGuildCreate.h"
#include "uiGuildWindow/uiGuildWindow.h"
#include "uiGuildMenu/uiGuildMenu.h"
#include "uiGuildMain/uiGuildMain.h"
#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiChaoSystemMan/uiChaoSystemMan.h"
#include "TargetDialog.h"
#include "PlayerMonster.h"
#include "tiredsystemdialog.h"
#include "uiMissionRelayMan/uiMissionRelayMan.h"
#include "MissionRelay.h"
#include "BattleLobby.h"
#include "Global.h"
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
#include "SkillEffectInfoParser.h"
#include "MissionScene.h"
#include "AppearanceManager.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "InstanceDungeonScene.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiLoginMan/uiLoginMan.h"
#include "AreaTileLoadHelper.h"
///////////////////////////////////////////////////////////////

MapEditScene		g_mapEditScene;
PlayerMonster*      g_pMon = NULL;

//----------------------------------------------------------------------------
/**
*/
MapEditScene::MapEditScene(void)
{
	m_SceneType = SCENE_TYPE_MAPEDIT;
}



//----------------------------------------------------------------------------
/**
*/
MapEditScene::~MapEditScene(void)
{
}


#ifdef MAP_EDIT_MODE 

BOOL g_bResetShow=FALSE;
BOOL g_bAutoCalculate=FALSE;
BOOL g_bSelectResetShow=FALSE;

//------------------------------------------------------------------------------
/**
*/
void _CallBack_Reset(bool bYes)
{
	g_bResetShow=FALSE;

	if(bYes)
		g_mapEditScene.Reset();

}



//------------------------------------------------------------------------------
/**
*/
void _CallBack_SelectedReset(bool bYes)
{
	g_bSelectResetShow=FALSE;

	if(bYes)
		g_mapEditScene.ResetSelected();

}

//------------------------------------------------------------------------------
/**
*/
void _CallBack_Auto_Calculate(bool bYes)
{
	g_bAutoCalculate=FALSE;

	if(bYes)
	{
		g_mapEditScene.Reset();
		g_mapEditScene.AutoCalCulate();
	}
}

//----------------------------------------------------------------------------
/** 초기화, 로딩
*/
BOOL 
MapEditScene::Init( void )
{
	uiPartyMan* partyMan =
		static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
	if (partyMan)
	{
		partyMan->uiShowAll();
	}

	if (g_pMap)
		g_pMap->SetRenderFlag(TRUE);

#ifdef _PERF_DEBUG
	if(g_pSunRenderer->x_pManagerMultiLoader)
		g_pSunRenderer->x_pManagerMultiLoader->TurnOn_MultiThreadedLoading(FALSE);
#else
	m_bFristRender=TRUE;


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


	//	TCHAR		szMessage[MAX_MESSAGE_LENGTH];

	SceneBase::InitLoadingImg();
	m_dwTestMonsterId=0;
	m_bShowAllTile=FALSE;

	m_GoupId = 0;
	m_iDbgTextType = eDebugTextType_None;
	m_bComebackFromLobby = FALSE;
	m_bCameraDetech = false;
	m_bPlayerAck = FALSE;
	m_bHideObject=FALSE;
	m_bShowAllLayer=FALSE;

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
	m_bBlind = FALSE;

	m_dwBattleTimeSerial = 0;
	m_iBrightDelay = 0;
	m_GrayScaleColor = WzColor_RGBA(255,255,255,255);
	m_iGrayScaleDelay = 0;
	g_CurTime = 0;
	m_dwMissionClearTick = 0;

	m_ExpList.clear();


	
#ifdef _MOD_CHAGNE_DELETE_HERO
	g_ObjectManager.DeleteHero();
#endif
	g_pHero = (Hero *)g_ObjectManager.Add(
		g_ObjectManager.GenerateKeyAtSingleMode(PLAYER_OBJECT), 
		PLAYER_OBJECT,
		xGetHeroData()->GetObjectKey(),						
		TRUE);

	g_InterfaceManager.InitSkillInfos();

	g_pHero->SetHeroInfo(BattleScene::GetHeroInfo(),NULL,NULL);
	g_pHero->GetCharInfo()->m_byClassCode = xGetHeroData()->GetClassCode();

	WzVector wzVec;

	// 캐릭터의 기본 마을 ID에 있는 필드 넘버를 넘겨줘야함.
	g_pMap->GetHeroPositionOnlySingleVersion(&wzVec);
	// 캐릭터가 스폿될 위치를 잡아준다.
	g_pHero->SetPosition( wzVec );

	g_pHero->Rotate(0.0f);		
	g_pMap->SetUseAutoCamera(FALSE);
	g_pMap->SetAutoCameraTargetObjectKey( g_pHero->GetObjectKey() );

	// 테스트 코드들.
	int iCount = 0;
	int iBaboCount = 0;
	while(1) 
	{
		int tile = rand() % g_pSunTerrain->x_pPathFinder->GetNumPathTile();
		WzVector vTestPos = g_pSunTerrain->GetRandomPosInTile(tile);

		g_pSunTerrain->PathExp_Reset(g_pHero->GetPathExplorer(), &vTestPos, -1);

		assert(g_pHero->GetPathExplorer()->GetTile() != -1);
		iCount++;

		if (g_pHero->GetPathExplorer()->GetTile() != tile) {
			iBaboCount++;
		}

		break;
	}


	WzVector vTestPos = g_pSunTerrain->GetRandomPosInTile(1);
	g_pHero->SetPosition(vTestPos,1);

	vTestPos.x=111.83109f;
	vTestPos.y=335.43338f;
	vTestPos.z=9.8169489f;


	vTestPos.x = 7.1181059f;
	vTestPos.y = 23.331070f;
	vTestPos.z = 14.0f;


	int tile = 100;
	WzVector vMonPos = g_pSunTerrain->GetRandomPosInTile(tile);

	// 임시로 버서커에게 칼을 들려준다.
	if (g_pHero->GetClass() == eCHAR_BERSERKER) 
	{
		SCItemSlot ItemSlot;
		ItemSlot.SetCode(1);

		EquipmentContainer* pContainer = static_cast<EquipmentContainer>(g_pHero->GetSlotContainer(SI_EQUIPMENT));
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	
		g_pHero->SetCurrentAttackStyle(eSTYLE_TWOHANDAXE_NORMAL);

		pContainer = g_pHero->GetSlotContainer(SI_EQUIPMENT);
		ItemSlot.SetCode(51000); // 		
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

		ItemSlot.SetCode(51028); // 		
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

		ItemSlot.SetCode(51002); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

		ItemSlot.SetCode(51003); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

		ItemSlot.SetCode(51004); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

		ItemSlot.SetCode(51005); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

		ItemSlot.SetCode(51006); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);	

		ItemSlot.SetCode(60000); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_RING1, ItemSlot);	

		ItemSlot.SetCode(51001); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	

        g_pHero->RefreshEquipData();

		g_pHero->SetCurrentAttackStyle(eSTYLE_ROLLANDS_DOWN);

	}

	// 드래곤도 칼주자
	if (g_pHero->GetClass() == eCHAR_DRAGON) 
	{
		SCItemSlot ItemSlot;
		ItemSlot.SetCode(31);

		EquipmentContainer* pContainer = static_cast<EquipmentContainer>(g_pHero->GetSlotContainer(SI_EQUIPMENT));
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);			


		ItemSlot.SetCode(51007); // 		
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

		ItemSlot.SetCode(51029); // 		
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

		ItemSlot.SetCode(51008); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	


		ItemSlot.SetCode(51009); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

		ItemSlot.SetCode(51010); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

		ItemSlot.SetCode(51011); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

		ItemSlot.SetCode(51012); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

		ItemSlot.SetCode(51013); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);	
        
        g_pHero->RefreshEquipData();

		g_pHero->SetCurrentAttackStyle(eSTYLE_HACKSTER_KNOCKBACK_TWOHAND);

	}
	// 발키리 석궁주기
	if (g_pHero->GetClass() == eCHAR_VALKYRIE) 
	{

		SCItemSlot ItemSlot;
		//ItemSlot.SetCode(91); // 석궁
		ItemSlot.SetCode(112); // 에테르

		EquipmentContainer* pContainer = static_cast<EquipmentContainer>(g_pHero->GetSlotContainer(SI_EQUIPMENT));
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

		ItemSlot.SetCode(51021); // 		
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

		ItemSlot.SetCode(51031); // 		
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

		ItemSlot.SetCode(51022); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	

		ItemSlot.SetCode(51023); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

		ItemSlot.SetCode(51024); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

		ItemSlot.SetCode(51025); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

		ItemSlot.SetCode(51026); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

		ItemSlot.SetCode(51027); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);	

        g_pHero->RefreshEquipData();

		//g_pHero->SetCurrentAttackStyle(eSTYLE_ONEHANDCROSSBOW_NORMAL,FALSE);
		g_pHero->SetCurrentAttackStyle(eSTYLE_ETHER_NORMAL);
	}

	if (g_pHero->GetClass() == eCHAR_MAGICIAN) 
	{
		SCItemSlot ItemSlot;
		ItemSlot.SetCode(154);
		g_pHero->SetCurrentNomalAttackStyle(eWEAPONTYPE_ORB);

		EquipmentContainer* pContainer = static_cast<EquipmentContainer>(g_pHero->GetSlotContainer(SI_EQUIPMENT));
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);		

		ItemSlot.SetCode(51014); // 		
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

		ItemSlot.SetCode(51030); // 		
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

		ItemSlot.SetCode(51015); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	


		ItemSlot.SetCode(51016); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

		ItemSlot.SetCode(51017); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

		ItemSlot.SetCode(51018); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

		ItemSlot.SetCode(51019); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

		ItemSlot.SetCode(51020); // 
		pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);

        g_pHero->RefreshEquipData();

		ItemSlot.SetCode(138); // 
		pContainer = g_pHero->GetSlotContainer(SI_INVENTORY);
		pContainer->InsertSlot(0, ItemSlot);	

	}

	g_pHero->SetHP(100);
	g_pHero->SetMP(100);

	g_pHero->SetPartVariation(PLAYER_VARIATION_HAIR,1);
	g_pHero->SetPartVariation(PLAYER_VARIATION_FACE,1);
    g_pHero->RefreshAppearRender();

	g_InterfaceManager.RegisterSkillEffect(1,1,0);
	g_InterfaceManager.RegisterSkillEffect(1,2,0);
	g_InterfaceManager.RegisterSkillEffect(1,3,0);
	g_InterfaceManager.RegisterSkillEffect(1,4,0);
	g_InterfaceManager.RegisterSkillEffect(1,5,0);
	g_InterfaceManager.RegisterSkillEffect(1,6,0);
	g_InterfaceManager.RegisterSkillEffect(1,7,0);

	CursorChangeTransact::Instance()->ForceChange(MouseCursor::eMouseCursor_Default);

	g_InterfaceManager.ShowGameInterface(TRUE);

	SettingNPC(g_pMap->GetMapID(),g_pMap->GetCurrentFieldID());


	// 인터페이스 랜더링 온
	g_InterfaceManager.SetRenderState( TRUE );
	g_InterfaceManager.ResetDamageList();
	g_InterfaceManager.InitializeSkillEffect();
	// 미션 스위치 초기화
	InitializeMissionSwitch();
	g_ProjectileManagerEx.Clear();
	g_CloneManager.Clear();
	g_pSunRenderer->EnableAreaLight(TRUE);

	m_oldBlur         = g_pSunRenderer->x_pManagerPostEffect->GetTechBlur();
	m_oldFogParameter = g_pSunRenderer->GetFogParameter();

	uiBase = GET_DIALOG(IM_CHAT_MANAGER::CHAT_DIALOG);//(ChatDialog *)g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_CHAT );
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
	g_Camera.SetProjection(g_Camera.GetNearClip(),g_pMap->GetMapFarClip()*2.0f,BATTLESCENE_FOV);

	WzVector wzVector;
	g_pMap->GetPlayerDirection(&wzVector );
	g_pMap->ApplyOptionSetting();


	m_iScreenShotState = eSCREEN_SHOT_FULL;

	g_KeyQueueManager.Reset();
	g_KeyQueueManager.SetKey(DIK_ESCAPE);
	g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);

	m_iEventTick = 0;
	m_bEventState = FALSE;
	m_iEventCurScript = 0;
	m_iEventCurPage = 0;
	m_iEventCurLine = 0;
	m_iEventVoiceHandle = -1;
	for (int a = 0; a < MAX_EVENT_LINE; a++)
	{
		ZeroMemory(m_szEventLine[a],sizeof(TCHAR) * MAX_MESSAGE_LENGTH);
	}

	int fogValue= GRAPHICPARAM->GetFogValue();
	int blurValue = GRAPHICPARAM->GetBlurValue();
	m_fFog_weight = (float) fogValue / 100.0f > 1.0f ? 1.0f   :   (float) fogValue / 100.0f;
	m_fBlur_weight = (float) blurValue / 100.0f > 1.0f ? 1.0f :   (float) blurValue / 100.0f;

	InitQuestAreaInfo();

	g_InterfaceManager.HideAllWindows();
	m_iCurSelectObject=m_iNextSelectObject=m_iPreSelectObject=-1;

	//파일이 없을땐 자동 계산 
	if(!g_pMap->IsExsistWAT())
	{
		Reset();
		AutoCalCulate();
		SaveWAT();
	}
	else
	{
		char szBufStr[_MAX_PATH] = { 0, };
		char szDrive[_MAX_PATH] = { 0, };
		char szDir[_MAX_PATH] = { 0, };
		char szFName[_MAX_FNAME] = { 0, };
		char szExt[_MAX_EXT] = { 0, };
		char szWATFileName[MAX_PATH]= { 0, };
		char szFilePath[MAX_PATH]= { 0, };	

		_splitpath(g_pMap->GetFullFileName(), szDrive, szDir, NULL, NULL);
		ZeroMemory(szFilePath,sizeof(szFilePath));

		if (szDrive[0] == 0)
		{
			_stprintf(szFilePath,_T("%s"),szDir);
		}
		else 
		{
			_stprintf(szFilePath,_T("%s\\%s"),szDrive,szDir);
		}

		StrCopy(szWATFileName, g_pMap->GetFullFileName());	
		szWATFileName[strlen(g_pMap->GetFullFileName())- 3] = 0;
		strcat(szWATFileName, "wat");

		char temp[1024];
		sprintf(temp,"로딩 완료%s",szWATFileName);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp);

	}
	
	uiBase * pDlg = GET_DIALOG(IM_CHAT_MANAGER::SYSTEM_CHAT_DIALOG);
	if ( pDlg )
	{
		pDlg->ShowWindow(TRUE);
	}

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
MapEditScene::Process( DWORD dwTick )
{

	static int iCount = 0;
	static BOOL bOld = FALSE;

	g_CurTime	+= dwTick;

	g_pMap->Update( dwTick );
	ProcessSpecialEffect(dwTick);
	
	
	g_CloneManager.Process(dwTick);
	g_ProjectileManagerEx.Process(dwTick);

	LPDIMOUSESTATE2	pMouserState = g_Input.GetMouseBuffer();

	if(  g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_DOWN) )
	{
		m_bIsMouseLBClick = TRUE;
	}
	else
	{
		m_bIsMouseLBClick = FALSE;
	}

	ProcessUnits(dwTick);

	if (g_pMap->GetUseAutoCamera())
	{
		g_pMap->AutoCameraPlay( dwTick );
	}
	else
	{
		ProcessCamera(pMouserState,dwTick);
	}
	ProcessKeyInput_Hero(dwTick);

	ProcessEffect(dwTick);
 	
	m_bEnableRender = TRUE;

	if ( m_bWaitEnterSync == FALSE)
	{
		m_bDrawWait = FALSE;
	}

	ProcessTrigger();

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
MapEditScene::Render( DWORD dwTick )
{
	m_eFadeState=eFADE_STATE_NONE;
	
	if( m_bEnableRender == FALSE )
	{
		g_pSunRenderer->x_pManagerPostEffect->Apply();
		return TRUE;
	}

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

	g_pSunRenderer->SetCullFaceMode( CF_DRAW_FRONT );
	g_pMap->Render(dwTick);

	SUNPROFILE_BEGIN( _T("RenderOthers") );
	//	임시 : 잔상 효과

	RenderClone();
	g_ProjectileManagerEx.Render();	

	g_InterfaceManager.RenderMovePointer(dwTick);  // 이동포인터 렌더링
	g_InterfaceManager.RenderAreaPointer(dwTick);  // 지역지정 포인터 렌더링
	g_InterfaceManager.RenderTerrainTargetPointer(dwTick);  

	SUNPROFILE_END( _T("RenderOthers") );

	SUNPROFILE_BEGIN( _T("FlushRenderOtherAndApplyPostEffect") );

	if(g_pEffectManager)
		g_pEffectManager->Move( dwTick );

	g_pSunRenderer->x_pManagerPostEffect->Apply();

	SUNPROFILE_END( _T("FlushRenderOtherAndApplyPostEffect") );

	RenderDamage(dwTick);                         

	RenderBlindEffect();

	RenderMissionClearMessage();

	if (GENERALPARAM->GetSpecialMode()) 
	{
		if (g_bShowDebugInfo)
		{
			RenderMoveInfo();
		}

		RenderLightDirection();
	}

	RenderEditText();

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
void 
MapEditScene::Release( void )
{
	BattleScene::Release();
	m_iTerrainObjIndexArray.Clear();
}

//----------------------------------------------------------------------------
/**
*/
void	
MapEditScene::ProcessKeyInput_Hero(DWORD dwTick)
{


	if (g_pHero)
	{
		if (g_pHero->GetCurState() != STATE::MOVE)
		{
			g_InterfaceManager.SetMovePointer(FALSE,NULL);
		}
	}


	if( g_Input.GetState( DIK_LCONTROL, KS_KEY )&& 
		g_Input.GetState( DIK_Z, KS_DOWN ) )
	{
		while(1)
		{
			BYTE bCharType=rand()%5+1;
			if(bCharType!=eCHAR_SHADOW)
			{
				GameFunc::SpawnPlayer(bCharType);
				break;
			}
		}

	}

	if(g_Input.GetState(DIK_1, KS_UP))
	{
		if(g_pMon&&g_pMon->IsKindOfObject(MONSTER_OBJECT))
		{
			// 몬스터도 큐큐큐!
			PLAYER_ACTION action;
			ZeroMemory(&action,sizeof(action));
			action.ActionID = ACTION_ATTACK;
			action.ATTACK.byAttackType =g_pMon->GetComboCount();		// 몬스터는 특별히 attack type이 없다.			 
			action.ATTACK.dwTargetID =g_pHero->GetObjectKey();			
			action.ATTACK.vCurPos.wvPos = g_pMon->GetPosition(); 
			action.ATTACK.vCurPos.wTile = -1;
			action.ATTACK.vDestPos.wvPos = g_pMon->GetPosition();
			action.ATTACK.vDestPos.wTile = -1;
			action.ATTACK.dwAttackSerial = 0;
			action.ATTACK.bAttackMove = FALSE;
			action.ATTACK.dwAttackStyle = g_pMon->GetCurrentAttackStyle();

			g_pMon->PutAction(&action);

			g_pMon->SetComboCount(g_pMon->IncreaseComboCount());


		}
	}


	if(g_Input.GetState(DIK_2, KS_UP))
	{
		if(g_pMon&&g_pMon->IsKindOfObject(MONSTER_OBJECT))
		{
			// 몬스터도 큐큐큐!
			PLAYER_ACTION action;
			GameFunc::GetPlayerSkill(g_pMon->GetClass(),(Character *)g_pMon,(Character *)g_pHero,action);

			BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(action.SKILL.dwSkillID);

			if(pInfo)
			{
				g_InterfaceManager.ShowMessageBox(pInfo->m_szSkillName,2000,NULL,0);

				g_pMon->PutAction(&action);

			}
		}
	}



	if(g_Input.GetState(DIK_H, KS_DOWN))
	{
		uiBase* pDlg = GET_DIALOG(IM_CHAT_MANAGER::SYSTEM_CHAT_DIALOG);
		if ( pDlg )
		{
			BOOL bShow=pDlg->IsVisibleWindow();
			pDlg->ShowWindow(!bShow);
		}
	}

	//작업한 전체 타일보기
	if(g_Input.GetState(DIK_B, KS_UP))
	{
		m_bShowAllTile=!m_bShowAllTile;
	}
	//오브젝트 숨기기
	if(g_Input.GetState(DIK_N, KS_KEY))
	{
		m_bHideObject=TRUE;
	}
	else
	{
		m_bHideObject=FALSE;
	}

	//레이어 보기 
	if(g_Input.GetState(DIK_L, KS_UP))
	{
		m_bShowAllLayer=!m_bShowAllLayer;
	}

	//선택취소하기
	if(g_Input.GetState(DIK_C, KS_UP))
	{
		g_HeroInput.SetEditMapObjKey(0);
		m_iCurSelectObject=-1;
	}

	////낙뢰 
	//if(g_Input.GetState(DIK_T, KS_UP))
	//{
	//	AddBattleTimeSerial();

	//	SetBrightDelay(400);


 //       cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
 //       pProjectile->SetTargetPosition( g_pHero->GetVisiblePos() );
 //       pProjectile->SetLifeTime( 3000 );
 //       pProjectile->SetHitData( g_pHero->GetObjectKey(), g_pHero->GetCurrentAttackSerial(), 0, FALSE, 350, TRUE );
 //       pProjectile->CreateEffect( StrToWzID("0025"), g_pHero->GetObjectKey(), g_pHero->GetObjectKey(), 300 );
 //       g_ProjectileManagerEx.InsertProjectile( pProjectile );


	//	//PROJECTILE_PARAM param;

	//	//ZeroMemory(&param,sizeof(param));
	//	//param.fHeight = 0.0f;

	//	//param.vStartPos = g_pHero->GetVisiblePos();
	//	//param.vTargetPos=param.vStartPos;
	//
	//	//param.dwLifeTime = 3000;
	//	//param.dwMoveTime = 3000;

	//	//param.fSpeed = 1.0f;
	//	//param.dwType=PROJECTILEID_COMMON;
	//	//param.dwImageCode=0;
	//	//param.dwAttackSerial = g_pHero->GetCurrentAttackSerial();
	//	//param.dwOwner = g_pHero->GetObjectKey();		
	//	//param.effectID = StrToWzID("O025");
	//	//param.bAddNextSerial=TRUE;
	//	//param.dwDamageTime=350;
	//	//param.bAllDamage=TRUE;
	//	//param.dwActiveDelayTime=300;
	//	//param.bNotMove=TRUE;

	//	//g_ProjectileManager.CreateProjectile(param);

	//	ATTACK_RESULT attret;
	//	ZeroMemory(&attret,sizeof(attret));

	//	attret.dwAttackSerial = g_pHero->GetCurrentAttackSerial();
	//	attret.dwTimeSerial =  GetBattleTimeSerial();
	//	attret.TargetInfo.dwTargetHP = 80;
	//	attret.TargetInfo.dwTargetID = g_pHero->GetObjectKey();
	//	attret.TargetInfo.wDamage = 20;
	//	attret.dwSkillID=7061;
	//	attret.dwGroupId=1;

	//	g_pHero->AddAttackResult(&attret);
	//}


	//떨어뜨리기
	if(g_Input.GetState(DIK_F1, KS_UP))
	{

		TerrainObject *pTerrain=g_HeroInput.GetTargetTerrainObject();
		if(pTerrain)
		{
			if(pTerrain->GetCurState()!=STATE::EARTH_QUAKE)
			{
				pTerrain->SetNextState( STATE::EARTH_QUAKE, g_CurTime);
				pTerrain->ForceStateChange(g_CurTime);
			}
			else 
			{	
				if(!pTerrain->IsFallDown())
					pTerrain->SetFallDown(TRUE);
				else
				{
					pTerrain->SetFallDown(FALSE);
					pTerrain->SetNextState( STATE::IDLE, g_CurTime);
					pTerrain->ForceStateChange(g_CurTime);
					
				}
			}

		}
		
	}//복원
	
	if(g_Input.GetState(DIK_F2, KS_UP))//전부 복원
	{
		for(int i=0;i<m_iTerrainObjIndexArray.Size();i++)
		{
			TerrainObject *pObj=(TerrainObject *)g_ObjectManager.GetObject(m_iTerrainObjIndexArray[i]);
			if(pObj)
			{
				pObj->SetFallDown(FALSE);
				pObj->SetNextState( STATE::IDLE, g_CurTime);
				pObj->ForceStateChange(g_CurTime);
				
			}
		}
	}

	if(!g_bResetShow&&!g_bAutoCalculate&&!g_bSelectResetShow)
	{

		if(g_Input.GetState(DIK_F12, KS_UP))
		{
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			TCHAR szTemp[INTERFACE_STRING_LENGTH] ="작업을 초기화 하시겠습니까?";

			g_bResetShow=TRUE;

			if(pSystemMan)
				pSystemMan->Sys_ConfirmUnLock(szTemp,_CallBack_Reset);						
		}

		if(g_Input.GetState(DIK_F11, KS_UP)&&m_iCurSelectObject!=-1)
		{
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			TCHAR szTemp[INTERFACE_STRING_LENGTH] ="선택된 지형의 작업을 초기화 하시겠습니까?";

			g_bSelectResetShow=TRUE;

			if(pSystemMan)
				pSystemMan->Sys_ConfirmUnLock(szTemp,_CallBack_SelectedReset);						
		}

		if(g_Input.GetState(DIK_F10, KS_UP))
		{
			uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			TCHAR szTemp[INTERFACE_STRING_LENGTH] ="자동 계산 하시겠습니까?기존의 작업이 초기화 됩니다.";

			g_bAutoCalculate=TRUE;

			if(pSystemMan)
				pSystemMan->Sys_ConfirmUnLock(szTemp,_CallBack_Auto_Calculate);						
		}

	}

	int iSize=m_iTerrainObjIndexArray.Size();
	if(iSize)
	{
		if(g_Input.GetState(DIK_UP, KS_UP))
		{
			m_iCurSelectObject++;
			m_iCurSelectObject=min(iSize-1,m_iCurSelectObject);

			Object *pObj=g_ObjectManager.GetObject(m_iTerrainObjIndexArray[m_iCurSelectObject]);
			if(g_pHero&&pObj)
			{
				WzVector vPos=pObj->GetPosition();
				vPos.z+=pObj->GetMeshHeight();

				g_pHero->SetPosition(vPos);
			}
			
			g_HeroInput.SetEditMapObjKey(m_iTerrainObjIndexArray[m_iCurSelectObject]);
		}
		else if(g_Input.GetState(DIK_DOWN, KS_UP))
		{
			m_iCurSelectObject--;
			m_iCurSelectObject=max(0,m_iCurSelectObject);

			Object *pObj=g_ObjectManager.GetObject(m_iTerrainObjIndexArray[m_iCurSelectObject]);
			if(g_pHero&&pObj)
			{
				WzVector vPos=pObj->GetPosition();
				vPos.z+=pObj->GetMeshHeight();

				g_pHero->SetPosition(vPos);

			}
			g_HeroInput.SetEditMapObjKey(m_iTerrainObjIndexArray[m_iCurSelectObject]);
		}
		else if(g_Input.GetState(DIK_HOME, KS_UP))
		{
			m_iCurSelectObject=0;

			Object *pObj=g_ObjectManager.GetObject(m_iTerrainObjIndexArray[m_iCurSelectObject]);
			if(g_pHero&&pObj)
			{
				WzVector vPos=pObj->GetPosition();
				vPos.z+=pObj->GetMeshHeight();

				g_pHero->SetPosition(vPos);

			}
			g_HeroInput.SetEditMapObjKey(m_iTerrainObjIndexArray[m_iCurSelectObject]);
		}
		else if(g_Input.GetState(DIK_END, KS_UP))
		{
			m_iCurSelectObject=iSize-1;

			Object *pObj=g_ObjectManager.GetObject(m_iTerrainObjIndexArray[m_iCurSelectObject]);
			if(g_pHero&&pObj)
			{
				WzVector vPos=pObj->GetPosition();
				vPos.z+=pObj->GetMeshHeight();

				g_pHero->SetPosition(vPos);

			}
			g_HeroInput.SetEditMapObjKey(m_iTerrainObjIndexArray[m_iCurSelectObject]);
		}
	}

	if(g_Input.GetState(DIK_LCONTROL, KS_KEY)&&g_Input.GetState(DIK_S, KS_DOWN))
	{
		SaveWAT();
	}


	g_HeroInput.Process(dwTick);

}

//----------------------------------------------------------------------------
/**
*/

void MapEditScene::SaveWAT()
{
	CAreaTileHelper helper;

	char szBufStr[_MAX_PATH] = { 0, };
	char szDrive[_MAX_PATH] = { 0, };
	char szDir[_MAX_PATH] = { 0, };
	char szFName[_MAX_FNAME] = { 0, };
	char szExt[_MAX_EXT] = { 0, };
	char szWATFileName[MAX_PATH]= { 0, };
	char szFilePath[MAX_PATH]= { 0, };	

	_splitpath(g_pMap->GetFullFileName(), szDrive, szDir, NULL, NULL);
	ZeroMemory(szFilePath,sizeof(szFilePath));

	if (szDrive[0] == 0)
	{
		_stprintf(szFilePath,_T("%s"),szDir);
	}
	else 
	{
		_stprintf(szFilePath,_T("%s\\%s"),szDrive,szDir);
	}

	StrCopy(szWATFileName, g_pMap->GetFullFileName());	
	szWATFileName[strlen(g_pMap->GetFullFileName())- 3] = 0;
	strcat(szWATFileName, "wat");

	for(int i=0;i<m_iTerrainObjIndexArray.Size();i++)
	{
		TerrainObject *pObj=(TerrainObject *)g_ObjectManager.GetObject(m_iTerrainObjIndexArray[i]);

		if(pObj)
		{
			AREA_TILE_INFO * pAreaInfo = new AREA_TILE_INFO;

			pAreaInfo->iAreaID=pObj->GetID();				//공간 번호 
			pAreaInfo->wNumberOfTiles=pObj->GetTileNum();		//타일 갯수 
			pAreaInfo->piTileIndex = new int[pAreaInfo->wNumberOfTiles];

			for(int j=0;j<pAreaInfo->wNumberOfTiles;j++)
			{
				pAreaInfo->piTileIndex[j]=pObj->GetTileIndex(j);
			}
			helper.AddAreaTileInfo(pAreaInfo,pObj->GetID());
		}
	}

	if(helper.SaveAreaTile(szWATFileName))
	{
		//Todo:세이브 성공 
		static int i=0;
		i++;
		char temp[1024];
		sprintf(temp,"%s 세이브완료 %d",szWATFileName,i);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp);

		
	}
	else
	{
		char temp[1024];
		sprintf(temp,"%s 세이브 실패!! 읽기 전용 확인!",szWATFileName);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp);

	}
}

//----------------------------------------------------------------------------
/**
*/
void MapEditScene::SetSelectObject(int iObjKey)
{
	for(int i=0;i<m_iTerrainObjIndexArray.Size();i++)
	{
		if(m_iTerrainObjIndexArray[i]==iObjKey)
		{
			m_iCurSelectObject=i;
			break;
		}
	}

}

//----------------------------------------------------------------------------
/**
*/
void  
MapEditScene::AddTerrainObjIndex(int iIndex)
{
	m_iTerrainObjIndexArray.Append(iIndex);
}

//----------------------------------------------------------------------------
/**
*/
void MapEditScene::RenderEditText()
{
	

	g_pSunRenderer->Set2DMode( TRUE );

	int iSize=m_iTerrainObjIndexArray.Size();

	char lpszText[256] = {0,};
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("BL24") );

	g_pSunRenderer->x_pManagerTextOut->DrawText( "MAP EDIT MODE", 20, 10, WzColor_RGB(235,192,38 ) );		

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

	int iGap=17;
	int iY=50;
	if(m_iCurSelectObject>=0&&m_iCurSelectObject<iSize)
	{
		MapObject *pObj=(MapObject *)g_ObjectManager.GetObject(m_iTerrainObjIndexArray[m_iCurSelectObject]);

		if(pObj)
		{
			sprintf(lpszText,"선택된 레이어:%d",pObj->GetID());
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, 20, iY, WzColor_RGB( 235,192,38) );		
			iY+=iGap;
			sprintf(lpszText,"선택된 인덱스:%d",m_iCurSelectObject);
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, 20, iY, WzColor_RGB( 152,206,142) );		
			iY+=iGap;
			sprintf(lpszText,"총 갯수:%d",iSize);
			g_pSunRenderer->x_pManagerTextOut->DrawText( lpszText, 20, iY, WzColor_RGB( 152,206,142) );		
			iY+=iGap;
		}
	}

	if(m_bShowAllTile)
	{
		g_pSunRenderer->x_pManagerTextOut->DrawText( "레이어에 속한 타일 보기", 20, iY, WzColor_RGB(152,206,142 ) );		
		iY+=iGap;
	}

	if(m_bShowAllLayer)
	{
		g_pSunRenderer->x_pManagerTextOut->DrawText( "레이어 보기", 20, iY, WzColor_RGB( 152,206,142 ) );		

	}

	// 마무리
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
	g_pSunRenderer->Set2DMode( FALSE );
}

//----------------------------------------------------------------------------
/**
*/
void MapEditScene::Reset()
{
	for(int i=0;i<m_iTerrainObjIndexArray.Size();i++)
	{
		TerrainObject *pObj=(TerrainObject *)g_ObjectManager.GetObject(m_iTerrainObjIndexArray[i]);
		if(pObj)
			pObj->DeleteAllTileIndex();
	}
}


//----------------------------------------------------------------------------
/**
*/
void MapEditScene::ResetSelected()
{
		TerrainObject *pObj=(TerrainObject *)g_ObjectManager.GetObject(m_iTerrainObjIndexArray[m_iCurSelectObject]);
		if(pObj)
			pObj->DeleteAllTileIndex();
}

//----------------------------------------------------------------------------
/**
*/
void MapEditScene::AutoCalCulate()
{
	int iTileNum=0;

	if(g_pMap)
	{
		iTileNum=g_pMap->GetWorldBase()->x_pPathFinder->GetNumPathTile();

		for(int i=0;i<m_iTerrainObjIndexArray.Size();i++)
		{
			TerrainObject *pObj=(TerrainObject *)g_ObjectManager.GetObject(m_iTerrainObjIndexArray[i]);
			if(pObj)
			{
				WzBoundingVolume *pBv=pObj->GetRenderPart()->GetBoundingVolumePtr();
				pBv->m_aabb.m_wvMax.z+=2.0f;

				pBv->m_aabb.m_wvMax.x+=2.0f;
				pBv->m_aabb.m_wvMax.y+=2.0f;

				pBv->m_aabb.m_wvMin.x-=2.0f;
				pBv->m_aabb.m_wvMin.y-=2.0f;

				for(int j=0;j<iTileNum;j++)
				{
					WzVector vOut[3];
					BOOL     bFlag[3]={FALSE,FALSE,FALSE};
					WzVector *pOut=(WzVector *)&vOut;
					int iCount=0;

					g_pMap->GetWorldBase()->x_pPathFinder->GetTileVertexInfo(pOut,j);

					if ((vOut[0].x >=pBv->m_aabb.m_wvMin.x) && (vOut[0].x <= pBv->m_aabb.m_wvMax.x) &&
						(vOut[0].y >=pBv->m_aabb.m_wvMin.y) && (vOut[0].y <= pBv->m_aabb.m_wvMax.y))
					{
						bFlag[0]=TRUE;
					}

					if ((vOut[1].x >=pBv->m_aabb.m_wvMin.x) && (vOut[1].x <= pBv->m_aabb.m_wvMax.x) &&
						(vOut[1].y >=pBv->m_aabb.m_wvMin.y) && (vOut[1].y <= pBv->m_aabb.m_wvMax.y) )
					{
						bFlag[1]=TRUE;
					}

					if ((vOut[1].x >=pBv->m_aabb.m_wvMin.x) && (vOut[1].x <= pBv->m_aabb.m_wvMax.x) &&
						(vOut[1].y >=pBv->m_aabb.m_wvMin.y) && (vOut[1].y <= pBv->m_aabb.m_wvMax.y))
					{
						bFlag[1]=TRUE;
					}

					if ((vOut[2].x >=pBv->m_aabb.m_wvMin.x) && (vOut[2].x <= pBv->m_aabb.m_wvMax.x) &&
						(vOut[2].y >=pBv->m_aabb.m_wvMin.y) && (vOut[2].y <= pBv->m_aabb.m_wvMax.y))
					{
						bFlag[2]=TRUE;
					}

					if(bFlag[0]&&bFlag[1]&&bFlag[2])
					{
						pObj->AddTileIndex(j);
					}
				}
			}
		}
		char temp[1024];
		sprintf(temp,"자동계산 완료");
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp);

	}
}

#endif 
