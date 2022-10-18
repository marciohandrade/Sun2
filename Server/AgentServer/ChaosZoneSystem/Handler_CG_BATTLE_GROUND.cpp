#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ChaosZoneSystem/Handler_CG_BATTLE_GROUND.h>
#include <PacketStruct_AG_ChaosZoneSystem.h>
#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <ChaosZoneSystem/BattleGroundManager.h>
//#include <ChaosZoneSystem/BattleGroundMatchingSystem.h>
#include <TimeCheck/BattleGroundOpenTimeManager.h>
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#include <ServerOptionParserEx.h>
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM

void Handler_CG_BATTLE_GROUND::OnCG_ProcessPacket(User* user, MSG_BASE* packet, WORD size)
{
#ifdef _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
    MSG_CG_BATTLE_GROUND_PACKET* recv_packet = static_cast<MSG_CG_BATTLE_GROUND_PACKET*>(packet);

    switch(recv_packet->packet_type)
    {
    case CG_BATTLE_GROUND_RESERVATION_SYN:   // 전장 예약..
        {
            OnCG_BATTLE_GROUND_RESERVATION_SYN(user, packet, size);
        }break;

    case CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN:   // 전장 취소..
        {
            OnCG_BATTLE_GROUND_RESERVATION_CANCEL_SYN(user, packet, size);
        }break;

    case CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN:  // 비정상 접속 종료시 예약 취소
        {
            OnCG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN(user, packet, size);
        }break;
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    case CG_BATTLE_GROUND_GET_OPEN_TIME_SYN:
        {
            OnCG_BATTLE_GROUND_GET_OPEN_TIME_SYN(user, packet, size);
        }break;
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    default:
        {
            user->SendToLinkedServer(static_cast<MSG_BASE_FORWARD*>(packet), size);
        }
        break;
    }
#endif // _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION
}

void Handler_CG_BATTLE_GROUND::OnCG_BATTLE_GROUND_RESERVATION_SYN(User* user, MSG_BASE* packet, WORD size)
{
    MSG_CG_BATTLE_GROUND_RESERVATION_SYN* recv_msg = 
        static_cast<MSG_CG_BATTLE_GROUND_RESERVATION_SYN*>(packet);

    const MAPCODE map_code = recv_msg->map_code;
    MSG_CG_BATTLE_GROUND_RESERVATION_ACK ack_msg;

    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    if (battle_ground_manager == NULL)
    {
        ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    // 입장 제한 확인..
    RC::eBATTLE_GROUND_RESULT result_code = battle_ground_manager->CanReservation(user, map_code);
    if (result_code != RC::RC_BATTLE_GROUND_RESERVATION_SUCCESS) 
    {
        ack_msg.result_code = result_code;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    //Agent->GameServer에서 검증
    MSG_AG_BATTLE_GROUND_CAN_RESERVATION_SYN msg;
    msg.map_code = map_code;
    msg.is_healer = recv_msg->is_healer;
    user->SendToLinkedServer(&msg, sizeof(msg));
}

void Handler_CG_BATTLE_GROUND::OnCG_BATTLE_GROUND_RESERVATION_CANCEL_SYN(User* user, MSG_BASE* packet, WORD size)
{
    MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN* recv_msg = 
        static_cast<MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_SYN*>(packet);

    USERGUID guid = user->GetUserKey();
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();

    MSG_CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK ack_msg;
    ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_CANCEL_FAIL;
    if (battle_ground_manager->IsValidReservationMember(guid))
    {
        BattleGroundMemberInfo* member = battle_ground_manager->FindBattleGroundMember(guid);
        if (member) 
        {
            if (member->group_key == 0) // 1차매칭 하지 않은 유저.
            {
                battle_ground_manager->RemoveMemberStandbyGroup(member);
                battle_ground_manager->RemoveReservationMember(guid);
                ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_CANCEL_SUCCESS;
            }
            else
            {
                ack_msg.result_code = RC::RC_BATTLE_GROUND_STATE_INCLUDE_MATCHING_GROUPS;
            }
        }
    }
    user->SendPacket(&ack_msg, sizeof(ack_msg));

    // 상태변경..
    ;{
        MSG_AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD cmd_msg;
        cmd_msg.behave_sate = PLAYER_BEHAVE_IDLE_STATE;
        user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));
    };
}

void Handler_CG_BATTLE_GROUND::OnCG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN(User* user, MSG_BASE* packet, WORD size)
{
    MSG_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN* recv_msg = 
        static_cast<MSG_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_REMOVE_SYN*>(packet);
}

#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
void Handler_CG_BATTLE_GROUND::OnCG_BATTLE_GROUND_GET_OPEN_TIME_SYN(User* user, MSG_BASE* packet, WORD size)
{
    MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN* recv_msg = 
        static_cast<MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_SYN*>(packet);

    MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK ack_msg;

    ServerOptionParserEx* const server_option = ServerOptionParserEx::Instance();
    const BYTE time_control_system_active =
        server_option->GetServerOption().battle_ground_time_control_system;
    if (time_control_system_active == 0)
    {
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    util::TimeInfoPerFrame time_info;
    util::TimeSync::UpdateTimeInfoPerFrame(&time_info);

    BattleGroundOpenTimeManager* const time_manager = BattleGroundOpenTimeManager::Instance();
    time_manager->SetupMessage(time_info, &ack_msg);

    user->SendPacket(&ack_msg, sizeof(ack_msg));
}
#endif //_NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

