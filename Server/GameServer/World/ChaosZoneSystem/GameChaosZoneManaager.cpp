#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <../ProgramCommon/ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <World/ChaosZoneSystem/GameChaosZoneManaager.h>
#include <PacketStruct_AG_ChaosZoneSystem.h>
#include <GameZoneManager.h>
#include <GameZone.h>
#include <GameInstanceDungeon.h>
#include <World/ChaosZoneSystem/ChaosZoneManager.h>
#include <MapInfoParser.h>
#include <World/ChaosZoneSystem/BattleGround.h>
#include <World/ChaosZoneSystem/FreePVP.h>
#include <BattleGroundInfoParser.h>
#include <EquipmentSlotContainer.h>
#include <ItemManager.h>
#include <ChaoState.h>
#include <BaseStatus.h>
#include <StatusManager.h>
#include <MissionManager.h>

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "World/ChaosZoneSystem/GoldRush.h"
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "SonnenSchein.h"
#include "ChaosZoneInfoParser.h"
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include "OneDaysGuildMission.h"
	#ifndef _NA_008334_20150608_SONNENSCHEIN
	#else
		#include "ChaosZoneInfoParser.h"
	#endif //(_NA_008334_20150608_SONNENSCHEIN)
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

//-------------------------------------------------------------------------------------------------
#ifdef _NA_008334_20150608_SONNENSCHEIN
VOID SendAckPacket_SonnenScheinEnterCheck(const DWORD& user_key, Player* player, 
                                          const RC::eSONNENSCHEIN_RESULT& result_code)
{
    MSG_AG_SONNENSCHEIN_ENTER_CHECK_ACK ack_msg;
    ack_msg.m_dwKey = user_key;
    ack_msg.result_code = result_code;

    player->SendPacket(&ack_msg, sizeof(ack_msg));
}
#endif //_NA_008334_20150608_SONNENSCHEIN

//--------------------------------------------------------------------------------------------------
// BattleGround
//--------------------------------------------------------------------------------------------------
void GameChaosZoneManager::ParseCGPacket(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_CG_CHAOS_ZONE_PACKET* recv_packet = static_cast<MSG_CG_CHAOS_ZONE_PACKET*>(packet);
    switch(recv_packet->category_type)
    {
    case kCategoryBattleGround:
        {
            OnCG_ProcessPacketBattleGround(server_session, packet, size);
        }
        break;
    case kCategoryFreePVP:
        {
            OnCG_ProcessPacketFreePVP(server_session, packet, size);
        }
        break;
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    case eCHAOSZONE_CATEGORY_GOLDRUSH:
        {
            OnCG_ProcessPacketGoldRush(server_session, packet, size);
        }
        break;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case eCHAOSZONE_CATEGORY_SONNENSCHEIN:
        {
            OnCG_ProcessPacketSonnenSchein(server_session, packet, size);
        }
        break;
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	case eCHAOSZONE_CATEGORY_ONEDAYSGUILDMISSION:
		{
			OnCG_ProcessPacketOneDaysGuildMission(server_session, packet, size);
		}
		break;
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
    }
}

void GameChaosZoneManager::OnCG_ProcessPacketBattleGround(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_CG_BATTLE_GROUND_PACKET* recv_packet = static_cast<MSG_CG_BATTLE_GROUND_PACKET*>(packet);
    switch(recv_packet->packet_type)
    {
    case CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN:
        {
            OnCG_BATTLE_GROUND_RESURECTION_REFUSE_SYN(server_session, packet, size);
        }
        break;
        
    case CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN:
        {
            OnCG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN(server_session, packet, size);
        }
        break;
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    case CG_BATTLE_GROUND_INITIATE_VOTE_SYN:
        {
            OnCG_BATTLE_GROUND_INITIATE_VOTE_SYN(server_session, packet, size);
        }
        break;

    case CG_BATTLE_GROUND_VOTE_SYN:
        {
            OnCG_BATTLE_GROUND_VOTE_SYN(server_session, packet, size);
        }
        break;
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    }
}

void GameChaosZoneManager::OnCG_BATTLE_GROUND_RESURECTION_REFUSE_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN* recv_packet = \
        static_cast<MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN*>(packet);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player == NULL) {
        return;
    }

    if (player->IsThereBattleGroundZone() == false)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in battle ground"), 
            __FUNCTION__, __LINE__);
        return;
    }

    player->GetBattleGroundPlayerInfo().SetPlayerState(kPlayerStateStandbyResurectionRefuse);

    BattleGround* const battle_ground = static_cast<BattleGround*>(player->GetCurrentChaosZone());
    if (battle_ground == NULL) {
        return;
    }

    MSG_CG_BATTLE_GROUND_STATE_RESURECTION_CMD send_msg;
    send_msg.player_state = kPlayerStateStandbyResurectionRefuse;
    send_msg.resurection_time = battle_ground->GetRemainTicks();
    player->SendPacket(&send_msg, sizeof(send_msg));
}

void GameChaosZoneManager::OnCG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN* recv_packet = \
        static_cast<MSG_CG_BATTLE_GROUND_RESURECTION_REFUSE_CANCEL_SYN*>(packet);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player == NULL) {
        return;
    }

    if (player->IsThereBattleGroundZone() == false)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in battle ground"), 
            __FUNCTION__, __LINE__);
        return;
    }

    player->GetBattleGroundPlayerInfo().SetPlayerState(kPlayerStateStandbyResurection);

    BattleGround* const battle_ground = static_cast<BattleGround*>(player->GetCurrentChaosZone());
    if (battle_ground == NULL)
    {
        return;
    }

    MSG_CG_BATTLE_GROUND_STATE_RESURECTION_CMD send_msg;
    send_msg.player_state = kPlayerStateStandbyResurection;
    send_msg.resurection_time = battle_ground->GetRemainTicks();
    player->SendPacket(&send_msg, sizeof(send_msg));
}

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
void GameChaosZoneManager::OnCG_BATTLE_GROUND_INITIATE_VOTE_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_CG_BATTLE_GROUND_INITIATE_VOTE_SYN* recv_packet = \
        static_cast<MSG_CG_BATTLE_GROUND_INITIATE_VOTE_SYN*>(packet);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player == NULL) {
        return;
    }

    if (player->IsThereBattleGroundZone() == false)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in battle ground"), 
            __FUNCTION__, __LINE__);
        return;
    }

    BattleGround* const battle_ground = static_cast<BattleGround*>(player->GetCurrentChaosZone());
    if (battle_ground == NULL) {
        return;
    }

    Player* van_player = PlayerManager::Instance()->FindPlayerByName(recv_packet->van_payer_name);
    if (van_player == NULL) {
        return;
    }
    
    if (battle_ground->FindBattleGroundMember(van_player->GetCharGuid()) == NULL) {
        return;
    }

    RC::eBATTLE_GROUND_RESULT result = RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL;
    result = battle_ground->OnInitiateVote(player, van_player);

    if (result != RC::RC_BATTLE_GROUND_INITIATE_VOTE_SUCCESS)
    {
        MSG_CG_BATTLE_GROUND_INITIATE_VOTE_NAK nak_msg;
        nak_msg.result = result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

void GameChaosZoneManager::OnCG_BATTLE_GROUND_VOTE_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_CG_BATTLE_GROUND_VOTE_SYN* recv_packet = \
        static_cast<MSG_CG_BATTLE_GROUND_VOTE_SYN*>(packet);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player == NULL) {
        return;
    }

    if (player->IsThereBattleGroundZone() == false)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in battle ground"), 
            __FUNCTION__, __LINE__);
        return;
    }

    BattleGround* const battle_ground = static_cast<BattleGround*>(player->GetCurrentChaosZone());
    if (battle_ground == NULL) {
        return;
    }
    
    battle_ground->OnVote(player, recv_packet->vote_result);

}
#endif // _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

//--------------------------------------------------------------------------------------------------
void GameChaosZoneManager::ParseAGPacket(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_AG_CHAOS_ZONE_PACKET* recv_packet = static_cast<MSG_AG_CHAOS_ZONE_PACKET*>(packet);
    switch(recv_packet->category_type)
    {
    case kCategoryBattleGround:
        {
            OnAG_ProcessPacketBattleGround(server_session, packet, size);
        }
        break;
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    case eCHAOSZONE_CATEGORY_GOLDRUSH:
        {
            OnAG_ProcessPacketGoldRush(server_session, packet, size);
        } break;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case eCHAOSZONE_CATEGORY_SONNENSCHEIN:
        {
            OnAG_ProcessPacketSonnenSchein(server_session, packet, size);
        }
        break;
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	case eCHAOSZONE_CATEGORY_ONEDAYSGUILDMISSION:
		{
			OnAG_ProcessPacketOneDaysGuildMission(server_session, packet, size);
		}
		break;
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
    }
}

void GameChaosZoneManager::OnAG_ProcessPacketBattleGround(ServerSession* server_session, 
                                                          MSG_BASE* packet, WORD size)
{
    MSG_AG_BATTLE_GROUND_PACKET* recv_packet = static_cast<MSG_AG_BATTLE_GROUND_PACKET*>(packet);
    switch(recv_packet->packet_type)
    {
    case AG_BATTLE_GROUND_ENTER_TEAM_CMD:
        {
            OnAG_BATTLE_GROUND_ENTER_TEAM_CMD(server_session, packet, size);
        }
        break;
    case AG_BATTLE_GROUND_CAN_RESERVATION_SYN:
        {
            OnAG_BATTLE_GROUND_CAN_RESERVATION_SYN(server_session, packet, size);
        }
        break;
    case AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD:
        {
            OnAG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD(server_session, packet, size);
        }
        break;
    }
}

void GameChaosZoneManager::OnAG_BATTLE_GROUND_ENTER_TEAM_CMD(ServerSession* server_session, 
                                                             MSG_BASE* packet, WORD size)
{
    MSG_AG_BATTLE_GROUND_ENTER_TEAM_CMD* recv_packet = \
        static_cast<MSG_AG_BATTLE_GROUND_ENTER_TEAM_CMD*>(packet);

    GameZone* const zone = g_GameZoneManager.GetZone(recv_packet->zone_key);
    if(zone == NULL || zone->GetZoneType() != eZONETYPE_INSTANCE )
    {
        SUNLOG(eCRITICAL_LOG,  
            TEXT("[GameChaosZoneManager::OnAG_BATTLE_GROUND_ENTER_TEAM_CMD]존이 존재하지 않는다[%d]\n"),
            recv_packet->zone_key);
        return;
    }

    GameInstanceDungeon* const instance_dungeon = static_cast<GameInstanceDungeon*>(zone);

    instance_dungeon->GetChaosZoneManager()->EnterTeam(recv_packet->team_key, 
                                                       recv_packet->user_count, 
                                                       recv_packet->user_key_list);

}

void GameChaosZoneManager::OnAG_BATTLE_GROUND_CAN_RESERVATION_SYN(ServerSession* server_session, 
                                                                  MSG_BASE* packet, WORD size)
{
    MSG_AG_BATTLE_GROUND_CAN_RESERVATION_SYN* recv_packet = \
        static_cast<MSG_AG_BATTLE_GROUND_CAN_RESERVATION_SYN*>(packet);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player == NULL) 
        return;

    const MAPCODE map_code = recv_packet->map_code;
    
    MSG_AG_BATTLE_GROUND_CAN_RESERVATION_ACK ack_msg;
    ack_msg.map_code = map_code;
    ack_msg.class_code = player->GetCharType();
    ack_msg.is_healer = recv_packet->is_healer;
    ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_SUCCESS;

    const BattleGroundInfo* const battle_ground_info = \
                                            BattleGroundInfoParser::Instance()->FindData(map_code);
    if (battle_ground_info == NULL)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 보상창이 띄워져 있을 경우..
    MissionManager* const mission_manager = player->GetMissionManager();
    if (mission_manager)
    {
        mission_manager->CheckEnableWindowOpen(); // 그럴리 없다고 단정지어진 것이지만, 중복된 보상이 존재할 경우를 대비
        if (mission_manager->EnableWindowOpen() == true) // 보상창이 열려있을 경우.
        {
            ack_msg.result_code = RC::RC_BATTLE_GROUND_REWARD_WINDOW_OPEN;
            player->SendPacket(&ack_msg,sizeof(ack_msg));
            return;
        }
    }

    // 도망자 버프 확인
    const BaseStatus* const status = player->GetStatusManager()->FindStatus(eCHAR_STATE_FUGITIVE);
    if (status != NULL) // 도망자 버프가 존재하면..
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_EXIST_STATE_FUGITIVE_BUF;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 카오상태 확인
    if (player->GetChaoState().IsChaoState() == TRUE)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_PLAYER_IS_CHAO_STATE;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 레벨 제한
    if (player->GetLevel() < battle_ground_info->limit_level)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_IS_NOT_QUALIFIED_LEVEL;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 장비 평점 제한..
    CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer* const>(
        player->GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));
    if (equip_container == NULL)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    const ns_formula::EquipSlotsManageInfo* const equip_manage_info = \
        equip_container->GetEquipmentManageInfo();
    if (equip_manage_info == NULL)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    const ns_formula::EquipSlotsManageInfo::ItemScoreInfo& item_score_info = \
        equip_manage_info->item_score_info;

    ack_msg.equip_item_score = item_score_info.GetTotalScore();
    if (battle_ground_info->equipment_rating > ack_msg.equip_item_score)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_IS_NOT_QUALIFIED_ITEM_SCORE;
    }

    player->SendPacket(&ack_msg,sizeof(ack_msg));
}

void GameChaosZoneManager::OnAG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD* recv_packet = \
        static_cast<MSG_AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD*>(packet);
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player) 
        player->SetBehave(recv_packet->behave_sate);
}

//--------------------------------------------------------------------------------------------------
// FreePVP
//--------------------------------------------------------------------------------------------------
void GameChaosZoneManager::OnCG_ProcessPacketFreePVP(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_CG_FREE_PVP_PACKET* recv_packet = static_cast<MSG_CG_FREE_PVP_PACKET*>(packet);
    switch(recv_packet->packet_type)
    {
    case CG_FREE_PVP_RESURRECTION_SYN:
        {
            OnCG_FREE_PVP_RESURRECTION_SYN(server_session, packet, size);
        }
        break;
    }
}

void GameChaosZoneManager::OnCG_FREE_PVP_RESURRECTION_SYN(ServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_CG_FREE_PVP_RESURRECTION_SYN* recv_packet = static_cast<MSG_CG_FREE_PVP_RESURRECTION_SYN*>(packet);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player == NULL)
    {
        return;
    }

    GameField* const game_field = player->GetField();
    if (game_field == NULL)
    {
        return;
    }

    GameZone* const game_zone = game_field->GetGameZone();
    if (game_zone == NULL || 
        game_zone->GetZoneType() != eZONETYPE_INSTANCE)
    {
        return;
    }

    GameInstanceDungeon* const instance_dungeon = static_cast<GameInstanceDungeon*>(game_zone);
    if (instance_dungeon == NULL ||
        instance_dungeon->GetChaosZoneType() != kFreePVP)
    {
        return;
    }
    
    ChaosZoneManager* const zone_manager = instance_dungeon->GetChaosZoneManager();
    if (zone_manager == NULL)
    {
        return;
    }

    ChaosZone* const chaos_zone = zone_manager->GetCurrentZone();
    if (chaos_zone == NULL)
    {
        return;
    }

    FreePVP* const free_pvp = static_cast<FreePVP*>(chaos_zone);
    if (free_pvp == NULL)
    {
        return;
    }
    
    free_pvp->Resurrection(player);
}

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
void GameChaosZoneManager::OnAG_ProcessPacketGoldRush( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_AG_GOLDRUSH_PACKET* recv_packet = static_cast<MSG_AG_GOLDRUSH_PACKET*>(packet);
    switch(recv_packet->packet_type)
    {
    case AG_GOLDRUSH_ENTER_TEAM_CMD:
        {
            OnAG_GOLDRUSH_ENTER_TEAM_CMD(server_session, packet, size);
        }
        break;
    case AG_GOLDRUSH_CAN_RESERVATION_SYN:
        {
            OnAG_GOLDRUSH_CAN_RESERVATION_SYN(server_session, packet, size);
        }
        break;
    case AG_GOLDRUSH_STATUS_CHANGE_STATE_CMD:
        {
            OnAG_GOLDRUSH_STATUS_CHANGE_STATE_CMD(server_session, packet, size);
        }
        break;
    }
}

void GameChaosZoneManager::OnAG_GOLDRUSH_ENTER_TEAM_CMD( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_AG_GOLDRUSH_ENTER_TEAM_CMD* recv_packet = \
        static_cast<MSG_AG_GOLDRUSH_ENTER_TEAM_CMD*>(packet);

    GameZone* const zone = g_GameZoneManager.GetZone(recv_packet->zone_key);
    if(zone == NULL || zone->GetZoneType() != eZONETYPE_INSTANCE )
    {
        SUNLOG(eCRITICAL_LOG,  
            TEXT("[GameChaosZoneManager::OnAG_GOLDRUSH_ENTER_TEAM_CMD]존이 존재하지 않는다[%d]\n"),
            recv_packet->zone_key);
        return;
    }

    GameInstanceDungeon* const instance_dungeon = static_cast<GameInstanceDungeon*>(zone);

    instance_dungeon->GetChaosZoneManager()->EnterTeam(recv_packet->team_key, 
        recv_packet->user_count, 
        recv_packet->user_key_list);
}

void GameChaosZoneManager::OnAG_GOLDRUSH_CAN_RESERVATION_SYN( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_AG_GOLDRUSH_CAN_RESERVATION_SYN* recv_packet = \
        static_cast<MSG_AG_GOLDRUSH_CAN_RESERVATION_SYN*>(packet);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player == NULL) 
        return;

    const MAPCODE map_code = recv_packet->map_code;

    MSG_AG_GOLDRUSH_CAN_RESERVATION_ACK ack_msg;
    ack_msg.map_code = map_code;
    ack_msg.class_code = player->GetCharType();
    ack_msg.is_healer = recv_packet->is_healer;
    ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_SUCCESS;
    
    const BattleGroundInfo* const battle_ground_info = \
        BattleGroundInfoParser::Instance()->FindData(map_code);
    if (battle_ground_info == NULL)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 보상창이 띄워져 있을 경우..
    MissionManager* const mission_manager = player->GetMissionManager();
    if (mission_manager)
    {
        mission_manager->CheckEnableWindowOpen(); // 그럴리 없다고 단정지어진 것이지만, 중복된 보상이 존재할 경우를 대비
        if (mission_manager->EnableWindowOpen() == true) // 보상창이 열려있을 경우.
        {
            ack_msg.result_code = RC::RC_BATTLE_GROUND_REWARD_WINDOW_OPEN;
            player->SendPacket(&ack_msg,sizeof(ack_msg));
            return;
        }
    }

    // 도망자 버프 확인
    const BaseStatus* const status = player->GetStatusManager()->FindStatus(eCHAR_STATE_FUGITIVE);
    if (status != NULL) // 도망자 버프가 존재하면..
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_EXIST_STATE_FUGITIVE_BUF;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 카오상태 확인
    if (player->GetChaoState().IsChaoState() == TRUE)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_PLAYER_IS_CHAO_STATE;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 레벨 제한
    if (player->GetLevel() < battle_ground_info->limit_level)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_IS_NOT_QUALIFIED_LEVEL;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 장비 평점 제한..
    CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer* const>(
        player->GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));
    if (equip_container == NULL)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    const ns_formula::EquipSlotsManageInfo* const equip_manage_info = \
        equip_container->GetEquipmentManageInfo();
    if (equip_manage_info == NULL)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        player->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    const ns_formula::EquipSlotsManageInfo::ItemScoreInfo& item_score_info = \
        equip_manage_info->item_score_info;

    ack_msg.equip_item_score = item_score_info.GetTotalScore();
    if (battle_ground_info->equipment_rating > ack_msg.equip_item_score)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_IS_NOT_QUALIFIED_ITEM_SCORE;
    }

    player->SendPacket(&ack_msg,sizeof(ack_msg));
}

void GameChaosZoneManager::OnAG_GOLDRUSH_STATUS_CHANGE_STATE_CMD( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_AG_GOLDRUSH_STATUS_CHANGE_STATE_CMD* recv_packet = \
        static_cast<MSG_AG_GOLDRUSH_STATUS_CHANGE_STATE_CMD*>(packet);
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player) 
        player->SetBehave(recv_packet->behave_sate);
}

void GameChaosZoneManager::OnCG_GOLDRUSH_INITIATE_VOTE_SYN( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_CG_GOLDRUSH_INITIATE_VOTE_SYN* recv_packet = \
        static_cast<MSG_CG_GOLDRUSH_INITIATE_VOTE_SYN*>(packet);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player == NULL) {
        return;
    }

    if (player->GetChaosZoneType() != eCHAOSZONE_TYPE_GOLDRUSH)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in goldrush"), 
            __FUNCTION__, __LINE__);
        return;
    }

    GoldRush* const goldrush = static_cast<GoldRush*>(player->GetCurrentChaosZone());
    if (goldrush == NULL) {
        return;
    }

    Player* van_player = PlayerManager::Instance()->FindPlayerByName(recv_packet->van_payer_name);
    if (van_player == NULL) {
        return;
    }

    if (goldrush->FindGoldRushMember(van_player->GetCharGuid()) == NULL) {
        return;
    }

    RC::eBATTLE_GROUND_RESULT result = RC::RC_BATTLE_GROUND_INITIATE_VOTE_FAIL;
    result = goldrush->OnInitiateVote(player, van_player);

    if (result != RC::RC_BATTLE_GROUND_INITIATE_VOTE_SUCCESS)
    {
        MSG_CG_GOLDRUSH_INITIATE_VOTE_NAK nak_msg;
        nak_msg.result = result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}

void GameChaosZoneManager::OnCG_GOLDRUSH_VOTE_SYN( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_CG_GOLDRUSH_VOTE_SYN* recv_packet = \
        static_cast<MSG_CG_GOLDRUSH_VOTE_SYN*>(packet);

    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_packet->m_dwKey);
    if (player == NULL) {
        return;
    }

    if (player->GetChaosZoneType() != eCHAOSZONE_TYPE_GOLDRUSH)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("[%s|%d] player is not in battle ground"), 
            __FUNCTION__, __LINE__);
        return;
    }

    GoldRush* const goldrush = static_cast<GoldRush*>(player->GetCurrentChaosZone());
    if (goldrush == NULL) {
        return;
    }

    goldrush->OnVote(player, recv_packet->vote_result);
}

void GameChaosZoneManager::OnCG_ProcessPacketGoldRush( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_CG_GOLDRUSH_PACKET* recv_packet = static_cast<MSG_CG_GOLDRUSH_PACKET*>(packet);
    switch(recv_packet->packet_type)
    {
    case CG_GOLDRUSH_INITIATE_VOTE_SYN:
        {
            OnCG_GOLDRUSH_INITIATE_VOTE_SYN(server_session, packet, size);
        }
        break;

    case CG_GOLDRUSH_VOTE_SYN:
        {
            OnCG_GOLDRUSH_VOTE_SYN(server_session, packet, size);
        }
        break;
    }
}

#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_008334_20150608_SONNENSCHEIN
VOID GameChaosZoneManager::OnCG_ProcessPacketSonnenSchein( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    //[ Client -> GameServer ] Packets
    MSG_CG_SONNENSCHEIN_PACKET* sonnenschein_packet = static_cast<MSG_CG_SONNENSCHEIN_PACKET*>(packet);

    switch(sonnenschein_packet->packet_type)
    {
        //
    }
}

VOID GameChaosZoneManager::OnAG_ProcessPacketSonnenSchein( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    //[ AgentServer -> GameServer ] Packets
    MSG_AG_SONNENSCHEIN_PACKET* sonnenschein_packet = static_cast<MSG_AG_SONNENSCHEIN_PACKET*>(packet);

    switch(sonnenschein_packet->packet_type)
    {
    case AG_SONNENSCHEIN_REQUEST_GUID_ACK:
        {
            OnAG_SONNENSCHEIN_REQUEST_GUID_ACK(server_session, packet, size);
        }
        break;
    case AG_SONNENSCHEIN_ENTER_CHECK_SYN:
        {
            OnAG_SONNENSCHEIN_ENTER_CHECK_SYN(server_session, packet, size);
        }
        break;
    }
}

VOID GameChaosZoneManager::OnAG_SONNENSCHEIN_REQUEST_GUID_ACK( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_AG_SONNENSCHEIN_REQUEST_GUID_ACK* recv_msg = 
        static_cast<MSG_AG_SONNENSCHEIN_REQUEST_GUID_ACK*>(packet);

    GameZone* zone = g_GameZoneManager.GetZone(recv_msg->room_key);
    if (zone == NULL) 
    {
        //Zone검사
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Error! GameZone is Not Exist![RoomKey: %d]", recv_msg->room_key);
        return;
    }
    if(zone->GetZoneType() != eZONETYPE_INSTANCE)
    {
        //ZoneType검사
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Error! GameZone is Not Instance![RoomKey: %d]", recv_msg->room_key);
        return;
    }

    GameInstanceDungeon* instance_dungeon = static_cast<GameInstanceDungeon*>(zone);
    if (instance_dungeon->GetChaosZoneType() != eCHAOSZONE_TYPE_SONNENSCHEIN)
    {
        //SONNENSCHEIN인지 검사
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] This dungeon is NOT SONNENSCHEIN! [RoomKey: %d]", recv_msg->room_key);
        return;
    }
    ChaosZoneManager* chaoszone_mgr = instance_dungeon->GetChaosZoneManager();
    if (chaoszone_mgr == NULL)
    {
        //chaoszonemanager 검사
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] ChaosZoneManager is NULL!");
        return;
    }
    ChaosZone* chaos_zone = chaoszone_mgr->GetCurrentZone();
    if (chaos_zone == NULL)
    {
        //chaoszone 검사
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] ChaosZone is NULL!");
        return;
    }
    SonnenSchein* sonnen_schein = static_cast<SonnenSchein*>(chaos_zone);
    sonnen_schein->SetInitialCreater(recv_msg->creater_char_guid_, recv_msg->creater_guild_guid_);
}

VOID GameChaosZoneManager::OnAG_SONNENSCHEIN_ENTER_CHECK_SYN( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_AG_SONNENSCHEIN_ENTER_CHECK_SYN* recv_msg = \
        static_cast<MSG_AG_SONNENSCHEIN_ENTER_CHECK_SYN*>(packet);

    // 입장 아이템 체크
    Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
    if (player == NULL) {
        return;
    }
    
    //1. 길드미션의 입장아이템 정보 가져오기
    ChaosZoneInfoParser* chaoszone_parser = ChaosZoneInfoParser::Instance();
    const ChaosZoneInfo* mission_info = chaoszone_parser->FindData(recv_msg->map_code);
    if (mission_info == NULL)
    {
        //Nak
        SendAckPacket_SonnenScheinEnterCheck(recv_msg->m_dwKey, player, 
                                             RC::RC_SONNENSCHEIN_NOTEXIST_MISSIONINFO);
        return;
    }

    MSG_CG_ITEM_LOSE_ACK item_msg;
    POSTYPE item_count = 1;    //1개 고정 (스크립트에도 수량정보는 포함되어있지 않다)
    WORD item_code = recv_msg->entrance_item_code;
    SCItemSlot deleted_item_slot;

    //2. 전달받은 아이템이 해당 룸의 입장아이템이 맞는가

    //입장아이템이 필요한 미션인가
    if (chaoszone_parser->IsMissionOfRequireItem(mission_info->map_code, eCHAOSZONE_TYPE_SONNENSCHEIN) == false)
    {
        //필요없으면 바로 ACK
        //Ack
        SendAckPacket_SonnenScheinEnterCheck(recv_msg->m_dwKey, player, RC::RC_SONNENSCHEIN_SUCCESS);
        return;
    }

    if (false == chaoszone_parser->IsMissionItem(mission_info->map_code, eCHAOSZONE_TYPE_SONNENSCHEIN, item_code))
    {
        //Nak
        SendAckPacket_SonnenScheinEnterCheck(recv_msg->m_dwKey, player, 
            RC::RC_SONNENSCHEIN_CHECK_ENTRANCE_ITEM_FAILED);
        return;
    }

    //3. 플레이어의 해당아이템 소지여부 검사
    INT having_item_num = player->GetItemManager()->ExistItem(SI_INVENTORY, item_code, item_count);
    if (having_item_num < item_count)
    {
        //Nak
        SendAckPacket_SonnenScheinEnterCheck(recv_msg->m_dwKey, player, 
                                             RC::RC_SONNENSCHEIN_NOTEXIST_ENTERANCE_ITEM);
        return;
    }
    
    //충분히 소지하고 잇으면
    //4. 아이템 소모시키고 ACK
    if ( RC::RC_ITEM_SUCCESS != player->GetItemManager()->DeleteAllKindOfItem( \
        1, &item_code, &item_count, &(item_msg.m_ItemInfo), &deleted_item_slot ) )
    {
        //Nak
        SendAckPacket_SonnenScheinEnterCheck(recv_msg->m_dwKey, player, 
                                             RC::RC_SONNENSCHEIN_CHECK_ENTRANCE_ITEM_FAILED);
        return;
    }

    //RC_ITEM_SUCCESS
    player->SendPacket(&item_msg, sizeof(item_msg));
    GAMELOG->LogItem(ITEM_USE, player, &deleted_item_slot, 0, item_count);

    //Ack
    SendAckPacket_SonnenScheinEnterCheck(recv_msg->m_dwKey, player, RC::RC_SONNENSCHEIN_SUCCESS);
}

#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION

void GameChaosZoneManager::OnCG_ProcessPacketOneDaysGuildMission( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
	//[ Client -> GameServer ] Packets
	MSG_CG_ONEDAYSGUILDMISSION_PACKET* onedaysguildmission_packet = static_cast<MSG_CG_ONEDAYSGUILDMISSION_PACKET*>(packet);

	switch(onedaysguildmission_packet->packet_type)
	{
		//
	}
}

void GameChaosZoneManager::OnAG_ProcessPacketOneDaysGuildMission( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
	//[ AgentServer -> GameServer ] Packets
	MSG_AG_ONEDAYSGUILDMISSION_PACKET* onedaysguildmission_packet = static_cast<MSG_AG_ONEDAYSGUILDMISSION_PACKET*>(packet);

	switch(onedaysguildmission_packet->packet_type)
	{
	case AG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN:
		{
			OnAG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN(server_session, packet, size);
		} break;
	}
}

void SendAckPacket_OneDaysGuildMissionEnterCheck(const DWORD& user_key, Player* player, 
																	   const RC::eONEDAYSGUILDMISSION_RESULT& result_code)
{
	MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK ack_msg;
	ack_msg.m_dwKey = user_key;
	ack_msg.result_code = result_code;

	player->SendPacket(&ack_msg, sizeof(ack_msg));
}

void GameChaosZoneManager::OnAG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN( ServerSession* server_session, MSG_BASE* packet, WORD size )
{
	MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN* recv_msg = \
		static_cast<MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN*>(packet);

	// 유저 유효성 검사 체크
	Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(recv_msg->m_dwKey);
	if (player == NULL) {
		return;
	}

	// 1. 길드미션의 입장아이템 정보 가져오기
	ChaosZoneInfoParser* chaoszone_parser = ChaosZoneInfoParser::Instance();
	const ChaosZoneInfo* mission_info = chaoszone_parser->FindData(recv_msg->map_code);
	if (mission_info == NULL)
	{
		//Nak
		SendAckPacket_OneDaysGuildMissionEnterCheck(recv_msg->m_dwKey, player, 
			RC::RC_ONEDAYSGUILDMISSION_NOTEXIST_MISSIONINFO);
		return;
	}

	MSG_CG_ITEM_LOSE_ACK item_msg;
	POSTYPE item_count = 1;    //1개 고정 (스크립트에도 수량정보는 포함되어있지 않다)
	WORD item_code = recv_msg->entrance_item_code;
	SCItemSlot deleted_item_slot;

	//2. 전달받은 아이템이 해당 룸의 입장아이템이 맞는가

	//입장아이템이 필요한 미션인가
	if (chaoszone_parser->IsMissionOfRequireItem(mission_info->map_code, eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION) == false)
	{
		//필요없으면 바로 ACK
		//Ack
		SendAckPacket_OneDaysGuildMissionEnterCheck(recv_msg->m_dwKey, player, RC::RC_ONEDAYSGUILDMISSION_SUCCESS);
		return;
	}

	if (false == chaoszone_parser->IsMissionItem(mission_info->map_code, eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION, item_code))
	{
		//Nak
		SendAckPacket_OneDaysGuildMissionEnterCheck(recv_msg->m_dwKey, player, 
			RC::RC_ONEDAYSGUILDMISSION_CHECK_ENTRANCE_ITEM_FAILED);
		return;
	}

	//3. 플레이어의 해당아이템 소지여부 검사
	INT having_item_num = player->GetItemManager()->ExistItem(SI_INVENTORY, item_code, item_count);
	if (having_item_num < item_count)
	{
		if(!recv_msg->pass_require_item_check)
		{
			//Nak
			SendAckPacket_OneDaysGuildMissionEnterCheck(recv_msg->m_dwKey, player, 
				RC::RC_ONEDAYSGUILDMISSION_NOTEXIST_ENTERANCE_ITEM);
			return;
		}
	}

	//Ack
	SendAckPacket_OneDaysGuildMissionEnterCheck(recv_msg->m_dwKey, player, RC::RC_ONEDAYSGUILDMISSION_SUCCESS);
}

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND