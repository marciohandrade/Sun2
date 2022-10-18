#include "SunClientPreCompiledHeader.h"

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "GoldRushScene.h"
#include "InterfaceManager.h"
#include "uiChatMan/uiChatMan_def.h"
#include "ChatDialog.h"
#include "Map.h"
#include "GlobalFunc.h"
#include "MapObject.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#include "ChaosSystem/ChaosSystemFunction.h"
#include "ChaosSystem/ChaosSystemMan.h"
#include "ChaosSystem/uiChaosZoneInfo.h"
#include "ChaosSystem/uiChaosZoneResult.h"
#include "ChaosSystem/uiChaosZoneTimer.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "KillMessageDisplayManager.h"
#include "BGMSoundInfoParser.h"

GoldRushScene g_GoldRushScene;
//-------------------------------------------------------------------------------------------
GoldRushScene::GoldRushScene( void )
{
    m_SceneType = SCENE_TYPE_GOLDRUSH;
}
//-------------------------------------------------------------------------------------------
GoldRushScene::~GoldRushScene( void )
{

}
//-------------------------------------------------------------------------------------------
BOOL GoldRushScene::Init()
{
    BattleScene::Init();

    if (g_pMap)
    {
        GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_INSTANCE);
    }

    ChaosSystem::Function::SetGoldRushReservation(false);

    uiBattleScoreMan* battle_score_manager_ptr = GET_DIALOG_MANAGER(uiBattleScoreMan, UIMAN_BATTLE_SCORE);
    if (battle_score_manager_ptr)
    {
        if (battle_score_manager_ptr && 
            (battle_score_manager_ptr->GetIsInit() == false))
        {
            battle_score_manager_ptr->SetMode(uiBattleScoreMan::eBTS_GOLD_RUSH);
            battle_score_manager_ptr->InitInfo();
        }
    }

    uiChaosZoneInfo* chaos_zone_info = GET_CAST_DIALOG(uiChaosZoneInfo, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_INFO);
    if (chaos_zone_info)
    {
        chaos_zone_info->ShowInterface(true);
        chaos_zone_info->GoldRushSetting();
    }

    uiChaosZoneTimer* chaos_zone_timer = GET_CAST_DIALOG(uiChaosZoneTimer, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_TIMER);
    if (chaos_zone_timer)
    {
        chaos_zone_timer->ShowInterface(true);
    }

    ChatDialog* chat_dialog_ptr = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
    if (chat_dialog_ptr)
    {
        chat_dialog_ptr->OnPartyChat();        
    }

    message_display_manager_.Init();

    return TRUE;
}
//-------------------------------------------------------------------------------------------
BOOL GoldRushScene::Process( DWORD tick )
{
    BOOL bRet = BattleScene::Process(tick);

    message_display_manager_.Process(tick);

    return bRet;
}
//-------------------------------------------------------------------------------------------
BOOL GoldRushScene::Render( DWORD tick )
{
    BOOL bRet = BattleScene::Render(tick);

    return bRet;
}
//-------------------------------------------------------------------------------------------
BOOL GoldRushScene::PostInterfaceRender( DWORD tick )
{
    BOOL bRet = BattleScene::PostInterfaceRender(tick);

    message_display_manager_.Render();

    return bRet;
}
//-------------------------------------------------------------------------------------------
void GoldRushScene::Release()
{
    BattleScene::Release();

    uiChaosZoneInfo* chaos_zone_info_ptr = GET_CAST_DIALOG(uiChaosZoneInfo, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_INFO);
    if (chaos_zone_info_ptr)
    {
        chaos_zone_info_ptr->ShowInterface(false);
    }
    uiChaosZoneTimer* chaos_zone_timer_ptr = GET_CAST_DIALOG(uiChaosZoneTimer, IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_TIMER);
    if (chaos_zone_timer_ptr)
    {
        chaos_zone_timer_ptr->ShowInterface(false);
    }
    ChaosSystem::GetChaosZoneData().Clear();
    ChaosSystem::GetGoldRushData().Clear();

    uiBattleScoreMan* battle_score_manager_ptr
        = static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
    if (battle_score_manager_ptr)
    {
        battle_score_manager_ptr->Reset();
    }

    ChatDialog* chat_dialog_ptr = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
    if (chat_dialog_ptr)
    {
        chat_dialog_ptr->OnNoramlChat();
    }
}

void GoldRushScene::ProcessKeyInput_Hero( DWORD tick )
{
    BattleScene::ProcessKeyInput_Hero(tick);
}

RELATIONKIND::TYPE GoldRushScene::GetRelationKindOfCharacter( Character* object_ptr, BOOL force_enemy )
{
    return GameFunc::GetRelationKindOfPlayerInGoldRush(object_ptr, force_enemy);
}

void GoldRushScene::InsertKillMessage( TCHAR* first_user_id, DWORD first_user_color, TCHAR* second_user_id, DWORD second_user_color, TCHAR* format_string )
{
    KillMessage message_data;
    message_data.set_first_user_id(first_user_id);
    message_data.set_first_user_color(first_user_color);
    message_data.set_second_user_id(second_user_id);
    message_data.set_second_user_color(second_user_color);
    message_data.set_format_string(format_string);

    message_display_manager_.Insert(message_data);
}

void GoldRushScene::InsertMonsterKillMessage( TCHAR* first_user_id, DWORD first_user_color, TCHAR* monster_id, DWORD second_user_color, TCHAR* format_string )
{
    KillMessage message_data;
    message_data.set_first_user_id(first_user_id);
    message_data.set_first_user_color(first_user_color);
    message_data.set_monster_id(monster_id);
    message_data.set_second_user_color(second_user_color);
    message_data.set_format_string(format_string);

    message_display_manager_.Insert(message_data);
}
//-------------------------------------------------------------------------------------------
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
