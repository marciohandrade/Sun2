#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "BattleGroundScene.h"
#include "HeroActionInput.h"
#include "Hero.h"
#include "MapObject.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#include "ChaosSystem/ChaosSystemFunction.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/uiChaosZoneInfo.h"
#include "ChaosSystem/uiChaosZoneResult.h"
#include "ChaosSystem/uiChaosZoneTimer.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "MapResMgr.h"
#include "ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h"
#include "GlobalFunc.h"
#include "Map.h"
#include "BGMSoundInfoParser.h"
#include "BattleGroundFlagInfoParser.h"
#include "ChatDialog.h"
#include "uiChatMan/uiChatMan_def.h"
#include "KillMessageDisplayManager.h"

///////////////////////////////////////////////////////////////

BattleGroundScene g_BattleGroundScene;

//-------------------------------------------------------------------------------------------
BattleGroundScene::BattleGroundScene() 
{
    m_SceneType = SCENE_TYPE_BATTLEGROUND;
}

//-------------------------------------------------------------------------------------------
BattleGroundScene::~BattleGroundScene()
{
}

//-------------------------------------------------------------------------------------------
BOOL BattleGroundScene::Init()
{
    BattleScene::Init();

    if (g_pMap)
    {
        GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_INSTANCE);
    }

    ChaosSystem::Function::SetBattleGroundReservation(false);
    ChaosSystem::Function::InitStrongPointAnimaion(g_pMap->GetCurrentFieldID()); // 거점들 애니메이션 초기화

    uiBattleScoreMan* battle_score_manager_ptr = GET_DIALOG_MANAGER(uiBattleScoreMan, UIMAN_BATTLE_SCORE);
    if (battle_score_manager_ptr)
    {
        if (battle_score_manager_ptr && 
            (battle_score_manager_ptr->GetIsInit() == false))
        {
            battle_score_manager_ptr->SetMode(uiBattleScoreMan::eBTS_BATTLE_GROUND);
            battle_score_manager_ptr->InitInfo();
        }
    }		

    uiChaosZoneInfo* chaos_zone_info = GET_CAST_DIALOG(uiChaosZoneInfo, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_INFO);
    if (chaos_zone_info)
    {
        chaos_zone_info->ShowInterface(true);
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        chaos_zone_info->BattleGroundSetting();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    }

    uiChaosZoneTimer* chaos_zone_timer = GET_CAST_DIALOG(uiChaosZoneTimer, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_TIMER);
    if (chaos_zone_timer)
    {
        chaos_zone_timer->ShowInterface(true);
    }

    //g_bObserverMode = FALSE;

    /*MainDialog* pMainDlg = GET_CAST_DIALOG(MainDialog, IM_MENU_MANAGER::MAIN_DLG);

    if (pMainDlg)
    {
        pMainDlg->ShowObserverButton(FALSE);
    }*/

    ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
    if (chat_dialog)
    {
        chat_dialog->OnPartyChat();
    }


    message_display_manager_.Init();

    return TRUE;
}

//-------------------------------------------------------------------------------------------
BOOL BattleGroundScene::Process(DWORD tick)
{

    BOOL bRet = BattleScene::Process(tick);

    message_display_manager_.Process(tick);

    return bRet;
}

//-------------------------------------------------------------------------------------------
BOOL BattleGroundScene::Render(DWORD tick)
{
    BOOL bRet = BattleScene::Render(tick);

    return bRet;
}

//-------------------------------------------------------------------------------------------
BOOL BattleGroundScene::PostInterfaceRender(DWORD tick)
{
    BOOL bRet = BattleScene::PostInterfaceRender(tick);

    message_display_manager_.Render();

    return bRet;
}

//-------------------------------------------------------------------------------------------
void BattleGroundScene::Release( void )
{
    BattleScene::Release();

    /*MainDialog* pMainDlg = GET_CAST_DIALOG(MainDialog, IM_MENU_MANAGER::MAIN_DLG);
    
        if (pMainDlg)
        {
            pMainDlg->ShowObserverButton(FALSE);
        }*/

    //g_bObserverMode = FALSE;
    //g_IsBattle		= FALSE;  (채팅을 막네)

    uiChaosZoneInfo* chaos_zone_info = GET_CAST_DIALOG(uiChaosZoneInfo, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_INFO);
    if (chaos_zone_info)
    {
        chaos_zone_info->ShowInterface(false);
    }
    uiChaosZoneTimer* chaos_zone_timer = GET_CAST_DIALOG(uiChaosZoneTimer, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_TIMER);
    if (chaos_zone_timer)
    {
        chaos_zone_timer->ShowInterface(false);
    }

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    ChaosSystem::GetChaosZoneData().Clear();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    ChaosSystem::GetBattleGroundData().Clear();

    uiBattleScoreMan* battle_score_manager_ptr 
        = static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
    if (battle_score_manager_ptr)
    {
        battle_score_manager_ptr->Reset();
    }

    ChatDialog* chat_dialog = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
    if (chat_dialog)
    {
        chat_dialog->OnNoramlChat();
    }
}

//-------------------------------------------------------------------------------------------
void BattleGroundScene::ProcessKeyInput_Hero(DWORD tick)
{
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if ((ChaosSystem::GetBattleGroundData().player_state() == kPlayerStateStandbyResurection) ||
        (ChaosSystem::GetBattleGroundData().player_state() == kPlayerStateStandbyResurectionRefuse)) //움직일 수 없는 상태일 때
    {
        if (g_Input.GetState(MOUSE_LBUTTON, KS_MOUSE_UP))
        {
            MapObject* map_object = g_HeroInput.GetPickedMapObject();
            if (map_object && (map_object->IsBattleGroundResurrectionObject() == TRUE))
            {
                WzVector offset = g_pHero->GetPosition() - map_object->GetPosition();
                float distance = VectorLength(&offset);

                if (distance < 50.0f)
                {
                    // 부활대기 재신청 / 부활대기 거부취소
                    ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN();
                }
                return;
            }
        }
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
 
    BattleScene::ProcessKeyInput_Hero(tick);

#ifdef _DEV_VER
    /*if (g_Input.GetState(DIK_F3, KS_UP))
    {
        BATTLE_GROUND_TEAM_MEMBER_INFO temp_member;
        temp_member.team_key = kRed;
        StrnCopy(temp_member.character_name, "레드팀", MAX_CHARNAME_LENGTH);
        ChaosSystem::GetBattleGroundData().AddBattleGroundMemberInfo(&temp_member);
        temp_member.team_key = kBlue;
        StrnCopy(temp_member.character_name, "블루팀", MAX_CHARNAME_LENGTH);
        ChaosSystem::GetBattleGroundData().AddBattleGroundMemberInfo(&temp_member);
    }
    if (g_Input.GetState(DIK_F4, KS_UP))
    {
        MSG_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD message;
        Snprintf(message.kill_payer_name, INTERFACE_STRING_LENGTH-1, "레드팀");
        Snprintf(message.death_payer_name, INTERFACE_STRING_LENGTH-1, "블루팀");
        message.is_series_kill = true;
        message.series_kill_count = 12;
        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD(&message);
    }
    if (g_Input.GetState(DIK_F5, KS_UP))
    {
        MSG_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD message;
        Snprintf(message.kill_payer_name, INTERFACE_STRING_LENGTH-1, "블루팀");
        Snprintf(message.death_payer_name, INTERFACE_STRING_LENGTH-1, "레드팀");
        message.is_series_kill = true;
        message.series_kill_count = 5;
        message.is_series_kill_block = true;
        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD(&message);
    }
    if(g_Input.GetState(DIK_F3, KS_UP))
    {		
        WzID temp_strongpoint_id = StrToWzID("P003");//858796112;
        ChaosZoneTeam temp_team_key = kBlue;
        BattleGroundStrongpointState temp_strongpoint_state = kStrongpointStateConquer;

        MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD temp_message;
        temp_message.strongpoint_info.strongpoint_id     = temp_strongpoint_id;
        temp_message.strongpoint_info.strongpoint_state  = temp_strongpoint_state;
        temp_message.strongpoint_info.team_key           = temp_team_key;

        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(&temp_message);
    }
    if(g_Input.GetState(DIK_F4, KS_UP))
    {		
        WzID temp_strongpoint_id = StrToWzID("P003");//858796112;
        ChaosZoneTeam temp_team_key = kBlue;
        BattleGroundStrongpointState temp_strongpoint_state = kStrongpointStateConquer;

        MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD temp_message;
        temp_message.strongpoint_info.strongpoint_id     = temp_strongpoint_id;
        temp_message.strongpoint_info.strongpoint_state  = temp_strongpoint_state;
        temp_message.strongpoint_info.team_key           = temp_team_key;

        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(&temp_message);
    }
    if(g_Input.GetState(DIK_F5, KS_UP))
    {		
        WzID temp_strongpoint_id = StrToWzID("P003");//858796112;
        ChaosZoneTeam temp_team_key = kBlue;
        BattleGroundStrongpointState temp_strongpoint_state = kStrongpointStateConquer;

        MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD temp_message;
        temp_message.strongpoint_info.strongpoint_id     = temp_strongpoint_id;
        temp_message.strongpoint_info.strongpoint_state  = temp_strongpoint_state;
        temp_message.strongpoint_info.team_key           = temp_team_key;

        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(&temp_message);
    }
    if (g_Input.GetState(DIK_F6, KS_UP))
    {		
        WzID temp_strongpoint_id = StrToWzID("P003");//858796112;
        ChaosZoneTeam temp_team_key = kRed;
        BattleGroundStrongpointState temp_strongpoint_state = kStrongpointStateConquer;

        MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD temp_message;
        temp_message.strongpoint_info.strongpoint_id     = temp_strongpoint_id;
        temp_message.strongpoint_info.strongpoint_state  = temp_strongpoint_state;
        temp_message.strongpoint_info.team_key           = temp_team_key;

        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(&temp_message);
    }
    if (g_Input.GetState(DIK_F7, KS_UP))
    {
        WzID temp_strongpoint_id = StrToWzID("P003");//858796112;
        ChaosZoneTeam temp_team_key = kBlue;
        BattleGroundStrongpointState temp_strongpoint_state = kStrongpointStateWait;

        MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD temp_message;
        temp_message.strongpoint_info.strongpoint_id     = temp_strongpoint_id;
        temp_message.strongpoint_info.strongpoint_state  = temp_strongpoint_state;
        temp_message.strongpoint_info.team_key           = temp_team_key;

        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(&temp_message);
    }
    if (g_Input.GetState(DIK_F8, KS_UP))
    {
        WzID temp_strongpoint_id = StrToWzID("P003");//858796112;
        ChaosZoneTeam temp_team_key = kRed;
        BattleGroundStrongpointState temp_strongpoint_state = kStrongpointStateWait;

        MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD temp_message;
        temp_message.strongpoint_info.strongpoint_id     = temp_strongpoint_id;
        temp_message.strongpoint_info.strongpoint_state  = temp_strongpoint_state;
        temp_message.strongpoint_info.team_key           = temp_team_key;

        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(&temp_message);
    }
    if (g_Input.GetState(DIK_F9, KS_UP))
    {
        WzID temp_strongpoint_id = StrToWzID("P003");//858796112;
        ChaosZoneTeam temp_team_key = kBlue;
        BattleGroundStrongpointState temp_strongpoint_state = kStrongpointStateWaitAttack;

        MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD temp_message;
        temp_message.strongpoint_info.strongpoint_id     = temp_strongpoint_id;
        temp_message.strongpoint_info.strongpoint_state  = temp_strongpoint_state;
        temp_message.strongpoint_info.team_key           = temp_team_key;

        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(&temp_message);
    }
    if(g_Input.GetState(DIK_F10, KS_UP))
    {		
        WzID temp_strongpoint_id = StrToWzID("P003");//858796112;
        ChaosZoneTeam temp_team_key = kRed;
        BattleGroundStrongpointState temp_strongpoint_state = kStrongpointStateWaitAttack;

        MSG_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD temp_message;
        temp_message.strongpoint_info.strongpoint_id     = temp_strongpoint_id;
        temp_message.strongpoint_info.strongpoint_state  = temp_strongpoint_state;
        temp_message.strongpoint_info.team_key           = temp_team_key;

        ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(&temp_message);
    }

    if(g_Input.GetState(DIK_F11, KS_UP))
    {
        uiChaosZoneResult* chaos_zone_result = GET_CAST_DIALOG(uiChaosZoneResult, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_RESULT);
        if (chaos_zone_result)
        {
            chaos_zone_result->ShowChaosZoneResult(kResultWin);
        }
    }
    if(g_Input.GetState(DIK_F12, KS_UP))
    {		
        uiChaosZoneResult* chaos_zone_result = GET_CAST_DIALOG(uiChaosZoneResult, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_RESULT);
        if (chaos_zone_result)
        {
            chaos_zone_result->ShowChaosZoneResult(kResultLose);
        }
    }*/
    ///////////////////////
#endif //_DEV_VER
}

//-------------------------------------------------------------------------------------------
RELATIONKIND::TYPE BattleGroundScene::GetRelationKindOfCharacter(Character* object, BOOL force_enemy)
{
    return GameFunc::GetRelationKindOfPlayerInBattleGround(object, force_enemy);
}

void BattleGroundScene::InsertKillMessage(TCHAR* first_user_id, DWORD first_user_color,
                                          TCHAR* second_user_id, DWORD second_user_color,
                                          TCHAR* format_string)
{
    KillMessage message_data;
    message_data.set_first_user_id(first_user_id);
    message_data.set_first_user_color(first_user_color);
    message_data.set_second_user_id(second_user_id);
    message_data.set_second_user_color(second_user_color);
    message_data.set_format_string(format_string);

    message_display_manager_.Insert(message_data);
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND