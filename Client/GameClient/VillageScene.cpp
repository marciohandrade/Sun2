#include "SunClientPrecompiledHeader.h"


#include "SceneBase.h"
#include "BattleScene.h"
#include "GameFramework.h"
#include "VillageScene.h"
#include "SoundEffect.h"
#include "ItemManager.h"
#include "SCSlotHeader.h"
#include "Hero.h"
#include "Item.h"
#include "SoundBGM.h"
#include "ObjectManager.h"
#include "ShopDialog.h"
#include "InterfaceManager.h"
#include "NPC.h"
#include "NpcInfoParser.h"
#include "BattleLobby.h"
#include "Map.h"
#include "BGMSoundInfoParser.h"
#include "Mouse.h"
#include "globalfunc.h"
#include "OptimizationManager.h"
#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"    
#include "OptionDialog.h"
#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#endif //_NA_000000_20121210_REFACTORY_GAME_USER
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

VillageScene	g_VillageScene;

VillageScene::VillageScene() 
{
	m_SceneType = SCENE_TYPE_VILLAGE;
	BattleScene::SetLobbyFlag(FALSE);
	BattleScene::SetLobbyInPending(FALSE);
	BattleScene::SetLobbyOutPending(FALSE);
	
	SetRqstFieldMoveMapCode(0);
}

VillageScene::~VillageScene()
{
}

void VillageScene::Activate( void )
{
	BattleScene::Activate();
}


BOOL VillageScene::Init( void )
{
	BattleScene::Init();

	GlobalFunc::PLAY_BGM(
		MAP_TYPE_DEFAULT, 
		g_pMap->GetCurrentFieldID(), 
		eZONETYPE_VILLAGE );

	// 넷모드에서는 Player Enter Ack를 받은후에 Npc를 세우는것으로 했다.

	g_KeyQueueManager.Reset();
	g_KeyQueueManager.SetKey(DIK_ESCAPE);
	g_KeyQueueManager.SetDefaultFun(GlobalFunc::DoSystemDialog);


	g_OptimizationManager.SetPerformanceMode(TRUE);

#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
	//이전씬
	SCENE_TYPE postSceneType = GameFramework::GetPostSceneType();

	if(postSceneType == SCENE_TYPE_MISSION)
	{
		uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);

		if (ui_battlezone2_manager_ != NULL)
		{
			BYTE isStateBattleZone2 = ui_battlezone2_manager_->battlezone_room_state();

			//현재 배틀존 입장 상태가 로비 상태와 같다면
			if(isStateBattleZone2 == uiBattlezone2Man::ROOM_STATE_LOBBY)
			{
				ui_battlezone2_manager_->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_LOBBY,true);
			}
		}

	}
#endif //_NA_000000_20121210_REFACTORY_GAME_USER

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    // 피성 오브젝트가 뜨지 않도록
    uiDominationTowerMan* domination_manager_ptr = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (domination_manager_ptr && false == domination_manager_ptr->IsShowDominationObject())
    {        
        domination_manager_ptr->set_domination_object_show(FALSE);
    }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	return TRUE;
}

BOOL VillageScene::Process( DWORD dwTick )
{
    BattleScene::Process(dwTick);

    int i;
    for (i = 0; i < 10; ++i)
    {
        TCHAR   szText[INTERFACE_STRING_LENGTH];

        Sprintf(szText, _T("PRT%d"), i);
        this->CheckHeroInShortcutBattleArea(szText, StrToWzID(szText));
        Sprintf(szText, _T("OUT%d"), i);
        this->CheckHeroInShortcutBattleArea(szText, StrToWzID(szText));
		Sprintf(szText, _T("MP0%d"), i);
		this->CheckHeroInShortcutBattleArea(szText, StrToWzID(szText));	

    }
	SendHeroSectorIndex();


	return TRUE;
}

BOOL VillageScene::Render( DWORD dwTick )
{
	BattleScene::Render(dwTick);
	
	
	return TRUE;
}
void VillageScene::Release( void )
{
	BattleScene::Release();

	g_SoundBGM.Stop();

	BattleScene::SetLobbyFlag(FALSE);
	BattleScene::SetLobbyInPending(FALSE);
	BattleScene::SetLobbyOutPending(FALSE);

	g_OptimizationManager.SetPerformanceMode(FALSE);

    // 옵션설정 LOD 거리로 세팅
#ifdef _NA_000000_20130114_RENEWER_UI
#else
    OptionDialog* option_dialog_ptr = 
        GET_CAST_DIALOG(OptionDialog, IM_OPTION_MENU_MANAGER::OPTION_DIALOG);
    if (option_dialog_ptr != NULL)
    {
        int lod_option = option_dialog_ptr->GetOptionParam()->GetOptionValue( nOptionType::LOD_LEVEL );
        GameFunc::ApplyLODDistance(lod_option);
    }
#endif
}

void VillageScene::ProcessKeyInput_GM()
{
	BattleScene::ProcessKeyInput_GM();

#if defined ( _ZONE_TEST_LOBBY_IN_SINGLE )
	if (g_Input.GetState(DIK_MINUS, KS_DOWN) )
	{
		// 미션 로비 생성
		BattleRoomEntry Entry;
		Entry.Init();
        
		Entry.SetPublic(eZONEPUBLIC_PUBLIC);
		Entry.SetType(eZONETYPE_MISSION);
		Entry.SetMapCode(1);
		Entry.SetFieldIndex(0);	// 미션은 무조건 0번 인덱스를 선택한다.
		Entry.SetMaxLevel(4);
		Entry.SetMaxPlayer(8);
		Entry.SetPassword("");

		BattleLobby * pBattleReady = 
			(BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);

		BattleScene::SetPendingBattleRoomEntry( &Entry );

		uiBattleZoneMan* pZoneMan = static_cast<uiBattleZoneMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));
		if(pZoneMan)
		{
			pZoneMan->SetLobbyMasterPlayerKey(g_pHero->GetObjectKey()); 
		}
		pBattleReady->ShowWindow(TRUE);

		BattleScene::SetLobbyFlag(TRUE);

		UserSlot EnterUser;
		EnterUser.SetPlayerKey( g_pHero->GetObjectKey() );
		pBattleReady->AddPlayer( EnterUser );

		MSG_CG_SYNC_PLAYER_ENTER_ACK	SendPacketAck;

		SendPacketAck.Encode();

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
	}
#ifdef	_JAPAN
	else if ( g_Input.GetState( DIK_CIRCUMFLEX, KS_DOWN ))
#else
	else if ( g_Input.GetState( DIK_EQUALS, KS_DOWN ))
#endif
	{
		// 헌팅 로비 생성      
		BattleRoomEntry Entry;
		Entry.Init();
    
		Entry.SetPublic(eZONEPUBLIC_PUBLIC);
		Entry.SetType(eZONETYPE_CHUNTING);
		Entry.SetMapCode(1001);
		Entry.SetFieldIndex(0);	
		Entry.SetMaxLevel(8);
		Entry.SetMaxPlayer(8);
		Entry.SetPassword("");

		BattleLobby * pBattleReady = 
			(BattleLobby *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_BATTLE_LOBBY);

		uiBattleZoneMan* pZoneMan = static_cast<uiBattleZoneMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLEZONE));
		if(pZoneMan)
		{
			pZoneMan->SetBattleRoomEntry( &Entry );
			BattleScene::SetPendingBattleRoomEntry(&Entry );

			pZoneMan->SetLobbyMasterPlayerKey(g_pHero->GetObjectKey()); 
		}
		pBattleReady->ShowWindow(TRUE);

		BattleScene::SetLobbyFlag(TRUE);

		UserSlot EnterUser;
		EnterUser.SetPlayerKey( g_pHero->GetObjectKey() );
		pBattleReady->AddPlayer( EnterUser );
		
		MSG_CG_SYNC_PLAYER_ENTER_ACK	SendPacketAck;

		SendPacketAck.Encode();

		g_pApplication->AddVirtualPacket(CI_GAMESERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
	}
#endif
}

BOOL  
VillageScene::SceneWndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam , SI_MESSAGE *stMessage)
{

	return FALSE;
}


void VillageScene::OnConnectWorldServer()
{
	m_iPrevSectorIndex = -1;
	SceneBase::OnConnectWorldServer();
}

RELATIONKIND::TYPE VillageScene::GetRelationKindOfCharacter( Character * pObject, BOOL bForceEnemy )
{
	return GameFunc::GetRelationKindOfPlayerInVillage( pObject, bForceEnemy );
}