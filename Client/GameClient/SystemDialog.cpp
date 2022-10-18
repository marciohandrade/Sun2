#include "SunClientPrecompiledHeader.h"
#include "SystemDialog.h"
#include "GameFramework.h"
#include "LoadScene.h"
#include "interfacemanager.h"
#include "MainDialog.h"
#include "BattleScene.h"
#include "VillageScene.h"
#include "CharacterScene.h"
#include "CastLeaveFieldDialog.h"
#include "Application.h"
#include "globalfunc.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiVendorMan/uiVendorMan_def.h"
#include "uiVendorSell/uiVendorSell.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemFunction.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "uiOptionMenuMan/uiOptionMenuMan.h"

#include "Map.h"

#include "uiMenuMan\uiMenuMan.h"

#ifdef _NA_000000_20130114_RENEWER_UI
	#include "uiOptionMan.h"
	#include "uiOptionView.h"
	#include "Interface/uiHotKeyDialog/uiHotKeyDialog.h"
	#include "HotKeyManager.h"
	#include "Input/KeyCommands.h"
	#include "input/HotKeyInfoParser.h"

#endif

#include "uiGuideSystemMan/uiGuideSystemMan.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiBattleScoreMan\uiBattleScoreMan.h"

WzID SystemDialog::m_wzId[System_MAX] =
{
    StrToWzID("TI00"),  //System_BACK_TO_THE_VILLAGE = 0,
    StrToWzID("TI01"),  //System_BACK_TO_THE_CHARACTER_SELECT,
#ifdef _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING
#else
	StrToWzID("TI03"),  //System_BACK_TO_THE_SERVER_SELECT,
#endif     
	StrToWzID("TI04"),  //System_OPTION_SETTING
	StrToWzID("TI05"),  //System_CREDIT
    StrToWzID("TI02"),  //System_EXIT,
    StrToWzID("B999"),  // Exit
	StrToWzID("TI06"),  //System_GO_OUT_DOMINATION,
};


//----------------------------------------------------------------------------
/**
*/
SystemDialog::SystemDialog(void)
{
}


//----------------------------------------------------------------------------
/**
*/
SystemDialog::~SystemDialog(void)
{
}


//----------------------------------------------------------------------------
/**
*/
VOID
SystemDialog::Init( CDrawBase * pDrawBase )
{
}


//----------------------------------------------------------------------------
/**
*/
VOID 
SystemDialog::OnShowWindow(BOOL val)
{
    if (val)
    {
		//UpdatePosition();
		
        CControlWZ* back_to_village_button = getControl(System_BACK_TO_THE_VILLAGE);

        if ((g_pMap != NULL) &&
            (back_to_village_button != NULL))
        {
            SCENE_TYPE current_scene_type = GameFramework::GetCurScene();
            if (current_scene_type == SCENE_TYPE_SSQ)
            {
                back_to_village_button->EnableWindowWZ(false);
            }
            else
            {
                back_to_village_button->EnableWindowWZ(true);
            }
            CControlWZ* go_out_domination_button = getControl(System_GO_OUT_DOMINATION);
            if (go_out_domination_button != NULL)
            {
                if (current_scene_type == SCENE_TYPE_DOMINATION
#ifdef _NA_008334_20150608_SONNENSCHEIN
                    || current_scene_type == SCENE_TYPE_SONNENSCHEIN
#endif //_NA_008334_20150608_SONNENSCHEIN
                    )
                {
                    back_to_village_button->ShowWindowWZ(WZ_HIDE);
                    go_out_domination_button->ShowWindowWZ(WZ_SHOW);
                }
                else
                {
                    back_to_village_button->ShowWindowWZ(WZ_SHOW);
                    go_out_domination_button->ShowWindowWZ(WZ_HIDE);
                }
            }            
        }

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType		= eDoType_dialog_System_Menu;
        msg.DoSomething	= GlobalFunc::DoSystemDialog;
        g_KeyQueueManager.PushBack(msg);
    }
	else
	{
		this->m_packetStatus = 0;

        g_KeyQueueManager.DeleteMsg(eDoType_dialog_System_Menu);
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
SystemDialog::Release()
{
}


//----------------------------------------------------------------------------
/**
*/
VOID 
SystemDialog::MessageProc( SI_MESSAGE * pMessage )
{
	uiVendorSell * pVendorSell = GET_CAST_DIALOG( uiVendorSell, IM_VENDOR_MANAGER::VENDOR_SELL );

	if ( GameFramework::GetCurrentScene() && 
		 ( SCENE_TYPE_CHARSELECT == GameFramework::GetCurrentScene()->GetType() || 
		   SCENE_TYPE_LOGIN == GameFramework::GetCurrentScene()->GetType()) )
	{
		// 캐릭터 씬에선 이런 일이 있으면 안된다.
		return;
	}

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
                // game exit
            case System_GAME_EXIT:
                {
                    uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
                    if (guide_manager != NULL)
                    {
#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
                        guide_manager->SaveData();
#else
                        guide_manager->SaveAndOut(uiGuideSystemMan::kGotoExit);
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
                    }

                    TCHAR szMessage[256];
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                    if (GlobalFunc::IsBattleGroundScene() &&
                        (ChaosSystem::GetBattleGroundData().battle_ground_progress() != ChaosSystem::BattleGroundData::PROGRESS_END))
                    {
                        // 2719 정말로 파티 떠남?
                        g_InterfaceManager.GetInterfaceString(2719, szMessage);
                    }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                    else if (GlobalFunc::IsGoldRushScene() &&
                        (ChaosSystem::GetGoldRushData().gold_rush_progress() != ChaosSystem::GoldRushData::PROGRESS_END))
                    {
                        // 2719 전장에서 이탈하게 되면 도망자 패널티를 받게 됩니다. 도망자 패널티를 받게 되면 일정시간동안 전장에 입장 할 수 없습니다. 정말 전장을 떠나시겠습니까?
                        g_InterfaceManager.GetInterfaceString(2719, szMessage);
                    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                    else
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                    {
                        //정말 게임을 종료하시겠습니까?
                        const DWORD dwIsLogOutStringIndex = 5099;
                        g_InterfaceManager.GetInterfaceString(dwIsLogOutStringIndex, szMessage, 256);
                    }
                    
					uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
					if(pSystemMan)
						pSystemMan->Sys_ConfirmLock(szMessage, GlobalFunc::SystemExitConfirm);


                    ShowDialog(FALSE);
                }
                break;

			case System_EXIT:	// exit
				{
					ShowDialog(FALSE);
				}
				break;

                // Select Another Game
            case System_BACK_TO_THE_VILLAGE:
                {
                    if( GENERALPARAM->IsNet())
                    {
						if (BIT_CHECK(this->m_packetStatus, BACK_TO_THE_VILLAGE))
						{
							return;
						}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                        if (GlobalFunc::IsBattleGroundScene() &&
                            (ChaosSystem::GetBattleGroundData().battle_ground_progress() != ChaosSystem::BattleGroundData::PROGRESS_END))
                        {
                            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                            // 2719 정말 파티 떠남?
                            g_InterfaceManager.GetInterfaceString(2719, temp_string, INTERFACE_STRING_LENGTH);					

                            uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                            if (system_manager)
                            {
                                system_manager->Sys_Confirm(temp_string, GlobalFunc::BattleGroundBackToTheVillageConfirm);
                            }
                        }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                        else if (GlobalFunc::IsGoldRushScene() &&
                            (ChaosSystem::GetGoldRushData().gold_rush_progress() != ChaosSystem::GoldRushData::PROGRESS_END))
                        {
                            TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                            // 2719 정말 파티 떠남?
                            g_InterfaceManager.GetInterfaceString(2719, temp_string, INTERFACE_STRING_LENGTH);					

                            uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                            if (system_manager)
                            {
                                system_manager->Sys_Confirm(temp_string, GlobalFunc::BattleGroundBackToTheVillageConfirm);
                            }
                        }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                        else
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                        {
                            BattleScene::BackToTheVillageTransact();
                        }
                    }
                    else
                    {
						BattleScene::BackToTheVillageTransact();
                    }

                    ShowDialog(FALSE);
                }
                break;
			case System_GO_OUT_DOMINATION:
				{
					if( GENERALPARAM->IsNet())
					{
						if (BIT_CHECK(this->m_packetStatus, BACK_TO_THE_VILLAGE))
						{
							return;
						}

#ifdef _NA_008334_20150608_SONNENSCHEIN
                        if  (GameFramework::GetCurScene() == SCENE_TYPE_SONNENSCHEIN)
                        {
                            BattleScene::BackToTheVillageTransact();
                        }
                        else
#endif //_NA_008334_20150608_SONNENSCHEIN
                        {
                            BattleScene::LeaveDominationTransact();
                        }
					}
					else
					{
						BattleScene::LeaveDominationTransact();
					}

					ShowDialog(FALSE);
				}
				break;
                // Character Selection
            case System_BACK_TO_THE_CHARACTER_SELECT:
				{
					if( pVendorSell && pVendorSell->IsStart() )
					{
						//5866	먼저 개인상점을 종료해야 합니다.
						g_InterfaceManager.GetInterfaceString(5866 , szMessage ,INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW , szMessage);
						ShowDialog(FALSE);
						break;
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

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                    if (ChaosSystem::GetBattleGroundData().is_on_reservation() == true)
                    {
                        //전장이 예약 중이면
                       // 2703: 전장 예약중엔 사용불가....
                        g_InterfaceManager.GetInterfaceString(2703 , szMessage ,INTERFACE_STRING_LENGTH);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW , szMessage);
                        ShowDialog(FALSE);
                        break;
                    }
                    else if (GlobalFunc::IsBattleGroundScene() &&
                        (ChaosSystem::GetBattleGroundData().battle_ground_progress() != ChaosSystem::BattleGroundData::PROGRESS_END))
                    {
                        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                        // 2719 정말 파티 떠남?
                        g_InterfaceManager.GetInterfaceString(2719, temp_string, INTERFACE_STRING_LENGTH);					

                        uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                        if (system_manager)
                        {
                            system_manager->Sys_Confirm(temp_string, GlobalFunc::BattleGroundCharacterSelectConfirm);
                        }
                    }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                    else if (GlobalFunc::IsGoldRushScene() &&
                        (ChaosSystem::GetGoldRushData().gold_rush_progress() != ChaosSystem::GoldRushData::PROGRESS_END))
                    {
                        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                        // 2719 정말 파티 떠남?
                        g_InterfaceManager.GetInterfaceString(2719, temp_string, INTERFACE_STRING_LENGTH);					

                        uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                        if (system_manager)
                        {
                            system_manager->Sys_Confirm(temp_string, GlobalFunc::BattleGroundCharacterSelectConfirm);
                        }
                    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                    else
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                    {
                        CharacterSelectStandingBy();
                    }
				}

                break;

#ifdef _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING
#else
            case System_BACK_TO_THE_SERVER_SELECT:
				{
					if( pVendorSell && pVendorSell->IsStart() )
					{
						//5866	먼저 개인상점을 종료해야 합니다.
						g_InterfaceManager.GetInterfaceString(5866 , szMessage ,INTERFACE_STRING_LENGTH );
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW , szMessage );
						ShowDialog(FALSE);
						break;
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

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                    if (ChaosSystem::GetBattleGroundData().is_on_reservation() == true)
                    {
                        //전장이 예약 중이면
                         // 2703: 전장 예약중엔 사용불가....
                        g_InterfaceManager.GetInterfaceString(2703 , szMessage ,INTERFACE_STRING_LENGTH);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW , szMessage );
                        ShowDialog(FALSE);
                        break;

                    }
                    else if (GlobalFunc::IsBattleGroundScene() &&
                        (ChaosSystem::GetBattleGroundData().battle_ground_progress() != ChaosSystem::BattleGroundData::PROGRESS_END))
                    {
                        // 전장 안이면
                        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                        // 2719 정말 파티 떠남?
                        g_InterfaceManager.GetInterfaceString(2719, temp_string, INTERFACE_STRING_LENGTH);					

                        uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                        if (system_manager)
                        {
                            system_manager->Sys_Confirm(temp_string, GlobalFunc::BattleGroundServerSelectConfirm);
                        }
                    }
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                    else if (GlobalFunc::IsGoldRushScene() &&
                        (ChaosSystem::GetGoldRushData().gold_rush_progress() != ChaosSystem::GoldRushData::PROGRESS_END))
                    {
                        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                        // 2719 정말 파티 떠남?
                        g_InterfaceManager.GetInterfaceString(2719, temp_string, INTERFACE_STRING_LENGTH);					

                        uiSystemMan* system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
                        if (system_manager)
                        {
                            system_manager->Sys_Confirm(temp_string, GlobalFunc::BattleGroundCharacterSelectConfirm);
                        }
                    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                    else
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                    {
                        ServerSelectStandingBy();
                    }
				}
                break;
#endif//_REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING

			case System_OPTION_SETTING:
				{
					//	옵션 다이얼로그
#ifdef _NA_000000_20130114_RENEWER_UI

					//temp key map dialog
// 					uiHotKeyDialog* hot_key_dialog = GET_CAST_DIALOG(uiHotKeyDialog, IM_OPTION_MENU_MANAGER::HOT_KEY_BINDING_DIALOG);
// 					if(hot_key_dialog)
// 					{
// 						BOOL b = hot_key_dialog->IsVisibleWindow();
// 						hot_key_dialog->ShowInterface(!b);
// 					}
					
					//BOOL b = GET_DIALOG( IM_OPTION_MANAGER::kOptionView_ID )->IsVisibleWindow();
					//GET_DIALOG( IM_OPTION_MANAGER::kOptionView_ID )->ShowInterface( !b );
#else
					BOOL b = GET_DIALOG( IM_OPTION_MENU_MANAGER::OPTION_MENU_DIALOG )->IsVisibleWindow();
					GET_DIALOG( IM_OPTION_MENU_MANAGER::OPTION_MENU_DIALOG )->ShowInterface( !b );
#endif

                    ShowDialog(FALSE);

				}
				break;
			case System_CREDIT:
				{
#ifdef _NA_000000_20130114_RENEWER_UI
					uiOptionMan::ToggleShowActionTap();
#endif
				}
				break;
			}
		}
		break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

VOID 
SystemDialog::ServerSelectStandingBy( VOID )
{
	ShowDialog( FALSE );

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
	if( GENERALPARAM->IsNet() )
	{

        //서버 끊기 전에 RepeatData 전송
        uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
        if (guide_manager != NULL)
        {
#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
            guide_manager->SaveData();
#else
            guide_manager->SaveAndOut(uiGuideSystemMan::kGotoServerSelect);
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
        }

		GlobalFunc::SendServerSelectRequestPacket( MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_SYN::eQuery_StartTransaction );
		
		keyMsg sendMsg;
		ZeroMemory( &sendMsg, sizeof( sendMsg ) );
		sendMsg.dwType		= eDoType_ServerSelectStandingBy;
		sendMsg.DoSomething	= GlobalFunc::DoLeaveFieldCancel;
		sendMsg.wParam		= eDoType_ServerSelectStandingBy;
		sendMsg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
		g_KeyQueueManager.PushBack( sendMsg );

#ifdef _DEV_VER
		StartLeaveFieldCastTimer( eDoType_ServerSelectStandingBy, 1000 );
#else //_DEV_VER
		StartLeaveFieldCastTimer( eDoType_ServerSelectStandingBy, DELAY_LEAVE_FIELD_TICK );
#endif //_DEV_VER
	}	
#else
	if( GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE )
	{
		ProcessServerSelect();
	}
	else
	{
		keyMsg sendMsg;
		ZeroMemory( &sendMsg, sizeof( sendMsg ) );
		sendMsg.dwType		= eDoType_ServerSelectStandingBy;
		sendMsg.DoSomething	= GlobalFunc::DoLeaveFieldCancel;
		sendMsg.wParam		= eDoType_ServerSelectStandingBy;
		sendMsg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
		g_KeyQueueManager.PushBack( sendMsg );

#ifdef _DEV_VER
		StartLeaveFieldCastTimer( eDoType_ServerSelectStandingBy, 1000 );
#else //_DEV_VER
		StartLeaveFieldCastTimer( eDoType_ServerSelectStandingBy, DELAY_LEAVE_FIELD_TICK );
#endif //_DEV_VER
	}
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__	
}

VOID 
SystemDialog::ProcessServerSelect(VOID)
{
    uiBattleScoreMan* battle_score_manager_ptr 
        = static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
    if (battle_score_manager_ptr)
    {
        battle_score_manager_ptr->Reset();
    }

	if (GENERALPARAM->IsNet())
	{
		g_isChangeServer = true;
		g_CharSelectScene.m_bConnected = false;

        if (g_pNetwork)
        {
		    g_pNetwork->Disconnect(CI_GAMESERVERIDX);
#if !defined(_NA002676_WORLD_SERVER_RENEWAL)
		    g_pNetwork->Disconnect(CI_CHATSERVERIDX);
		    g_pNetwork->SetEnableSendChatHeartBeat(FALSE);
#endif
		    g_pNetwork->SetEnableSendHeartBeat(FALSE);
        }

		g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_LOGIN, 11001, 0);
		GameFramework::ChangeScene(SCENE_TYPE_LOAD);

		uiPartyMan* partyMan =
			static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
		if (partyMan)
		{
			partyMan->DestroyParty();
		}
	}
	else
	{
		g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_LOGIN, 11001, 0);
		GameFramework::ChangeScene(SCENE_TYPE_LOAD);
	}		
}

VOID 
SystemDialog::CharacterSelectStandingBy( VOID )
{

    uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
    if (guide_manager != NULL)
    {
#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
        guide_manager->SaveData();
#else
        guide_manager->SaveAndOut(uiGuideSystemMan::kGotoCharacterSelect);
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
    }

	ShowDialog( FALSE );

	if( GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE )
	{
		ProcessCharacterSelect();
	}
	else
	{
		keyMsg sendMsg;
		ZeroMemory( &sendMsg, sizeof( sendMsg ) );
		sendMsg.dwType		= eDoType_CharacterSelectStandingBy;
		sendMsg.DoSomething	= GlobalFunc::DoLeaveFieldCancel;
		sendMsg.wParam		= eDoType_CharacterSelectStandingBy;
		sendMsg.lParam		= InterfaceManager::DIALOG_DELAY_CASTING;
		g_KeyQueueManager.PushBack( sendMsg );

#ifdef _DEV_VER
		StartLeaveFieldCastTimer( eDoType_CharacterSelectStandingBy, 1000 );
#else //_DEV_VER
		StartLeaveFieldCastTimer( eDoType_CharacterSelectStandingBy, DELAY_LEAVE_FIELD_TICK );
#endif //_DEV_VER
	}
}

VOID
SystemDialog::ProcessCharacterSelect( VOID )
{
    uiBattleScoreMan* battle_score_manager_ptr 
        = static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
    if (battle_score_manager_ptr)
    {
        battle_score_manager_ptr->Reset();
    }

    if( GENERALPARAM->IsNet() )
	{
		if (!BIT_CHECK(this->m_packetStatus, BACK_TO_THE_CHARACTER_SELECT))
		{
			BIT_ADD(m_packetStatus, BACK_TO_THE_CHARACTER_SELECT);

#ifdef _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
            MSG_CG_CONNECTION_BACKTOCHARSELECT_SYN SendPacket;
            GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
#else
            uiGuideSystemMan* guide_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
            if (guide_manager != NULL)
            {
                //타이머 체크 후 종료
                guide_manager->set_waiting_repeat_save_recv(true);
                guide_manager->set_waiting_library_save_recv(true);
            }
            //아래 SendPacket은 GuideSystemMan 에서 타이머 체크 후 실행합니다. (송찬종)

            //MSG_CG_CONNECTION_BACKTOCHARSELECT_SYN SendPacket;
            //GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket) );
#endif //_NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT
		}
	}
	else
	{
		g_LoadScene.SetNextSceneAfterLoaded(SCENE_TYPE_CHARSELECT, GENERALPARAM->GetCharSelectMapID(), 0 );
		g_LoadScene.ChangeImage(TRUE);
		GameFramework::ChangeScene( SCENE_TYPE_LOAD );
	}
}



//----------------------------------------------------------------------------
/**
*/
VOID 
SystemDialog::NetworkProc( MSG_BASE * pMsg )
{
	switch(pMsg->m_byCategory)
	{
	case CG_ZONE:
		{
			switch(pMsg->m_byProtocol)
			{
			case CG_ZONE_LOBBY_LEAVE_ACK:
			case CG_ZONE_LOBBY_LEAVE_NAK:
			case CG_ZONE_MISSION_LEAVE_ACK:
			case CG_ZONE_MISSION_LEAVE_NAK:
			case CG_ZONE_PVP_LEAVE_ACK:
			case CG_ZONE_PVP_LEAVE_NAK:
			case CG_ZONE_CHUNTING_LEAVE_ACK:
			case CG_ZONE_CHUNTING_LEAVE_NAK:
			case CG_ZONE_INSTANCE_DUNGEON_LEAVE_ACK:
			case CG_ZONE_INSTANCE_DUNGEON_LEAVE_NAK:
			case CG_ZONE_RETURN_VILLAGE_ACK:
			case CG_ZONE_RETURN_VILLAGE_NAK:
				{
					BIT_REMOVE( this->m_packetStatus, BACK_TO_THE_VILLAGE);
					g_KeyQueueManager.DeleteMsg(eDoType_Delay_Leave_Field);
				}
				break;

			default:
				break;
			}
        }
        break;
    case CG_ZONE_EX:
        {
            switch(pMsg->m_byProtocol)
            {
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
            case CG_ZONE_EX_SPA_LEAVE_ACK:
            case CG_ZONE_EX_SPA_LEAVE_NAK:
                {
                    BIT_REMOVE( this->m_packetStatus, BACK_TO_THE_VILLAGE);
                    g_KeyQueueManager.DeleteMsg(eDoType_Delay_Leave_Field);
                }
                break;
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
            default:
                break;
            }
        }
        break;
	case CG_CONNECTION:
		{
			switch(pMsg->m_byProtocol)
			{

            case CG_CONNECTION_BACKTOCHARSELECT_ACK:
				{
#ifdef _NHN_USA_CHANNELING
					GlobalFunc::SendHanReportPacket(600);//게임내에서캐릭선택으로이동(600)
#endif//_NHN_USA_CHANNELING
					ResetPacketStatus();	
				}
				break;

			case CG_CONNECTION_BACKTOCHARSELECT_NAK:
				{
					BIT_REMOVE( this->m_packetStatus, BACK_TO_THE_CHARACTER_SELECT);
				}
				break;


			default:
				break;
			}
		}
		break;

	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
SystemDialog::Update()
{
}


//----------------------------------------------------------------------------
/**
*/
CControlWZ * 
SystemDialog::getControl( POSTYPE AtPos )
{
	assert( AtPos < System_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}


//----------------------------------------------------------------------------
/**
*/
POSTYPE 
SystemDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < System_MAX; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return System_MAX;
}

//void SystemDialog::UpdatePosition()
//{
//	//RECT rc;
//	//GetDialogWZ()->GetDlgSize(&rc);
//
//	//long width = rc.right;
//	//long height = rc.bottom;
//
//	//float mw = (long)width * 0.5f;
//	//float mh = (long)height * 0.5f;
//
//	//
//
//	//long sw;
//	//long sh;
//
//	//POINT pt = g_pApplication->Get2DModeMapping();
//
//	//sw = pt.x;
//	//sh = pt.y;
//	//
//
//	//float pw = (float)(sw * 0.5f) - mw;
//	//float ph = (float)(sh * 0.5f) - mh;
//
//
//	//MoveWindowWZ(pw, ph);
//}

//----------------------------------------------------------------------------
/**
*/
void			
SystemDialog::AddPacketStatus(SYSTEM_PACKET_STATUS ePacketStatus)
{
	BIT_ADD(this->m_packetStatus, ePacketStatus);
}

//----------------------------------------------------------------------------
/**
*/
void			
SystemDialog::RemovePacketStatus(SYSTEM_PACKET_STATUS ePacketStatus)
{
	BIT_REMOVE(this->m_packetStatus, ePacketStatus);
}

void SystemDialog::CheckModifyOption()
{
	KEY_BIND_PAIR_MAP::iterator found_itr = 
		HotKeyManager::Instance()->current_using_key_bind_info().key_bind_pair_map().find(kOpen_SystemMenu_UI);

	KEY_BIND_PAIR temp_key_bind_pair = found_itr->second;

	STLX_STRING hot_key_string;
	DWORD string_code = 0;

	HOT_KEY_CODE hot_ket_code(temp_key_bind_pair.GetHotKeyCode(KEY_BIND_PAIR::kMainHotKey));
	if (hot_ket_code.key_code_ != 0)
	{
		if (hot_ket_code.ctrl_key_ == 1)
		{
			hot_key_string += g_InterfaceManager.GetInterfaceString(8218);
			hot_key_string += " + ";
		}

		if (hot_ket_code.alt_key_ == 1)
		{
			hot_key_string += g_InterfaceManager.GetInterfaceString(8219);
			hot_key_string += " + ";
		}

		if (hot_ket_code.shift_key_ == 1)
		{
			hot_key_string += g_InterfaceManager.GetInterfaceString(8217);
			hot_key_string += " + ";
		}

		string_code = HotKeyInfoParser::Instance()->GetStringCodeByKeyCode(hot_ket_code.key_code_);
		if(strstr(g_InterfaceManager.GetInterfaceString(string_code),"Back Slash"))
		{
			hot_key_string += "\\";
		}
		else
			hot_key_string += g_InterfaceManager.GetInterfaceString(string_code);
	}

	CControlWZ* Control_Key = getControl(System_CREDIT);
	if(Control_Key)
	{
		DWORD ControlTextID = Control_Key->GetTextID();

		TCHAR OutputText[256];
		g_InterfaceManager.GetInterfaceStringFormat(OutputText,256,ControlTextID,hot_key_string.c_str());

		Control_Key->SetTextWZA(OutputText);
	}

}