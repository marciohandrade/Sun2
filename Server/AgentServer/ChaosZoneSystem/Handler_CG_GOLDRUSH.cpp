#include <stdafx.h>

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "ChaosZoneSystem/Handler_CG_GOLDRUSH.h"
#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <PacketStruct_AG_ChaosZoneSystem.h>
#include <ChaosZoneSystem/BattleGroundManager.h>

void Handler_CG_GOLDRUSH::OnCG_ProcessPacket( User* user, MSG_BASE* packet, WORD size )
{
    MSG_CG_GOLDRUSH_PACKET* recv_packet = static_cast<MSG_CG_GOLDRUSH_PACKET*>(packet);

    switch(recv_packet->packet_type)
    {
    case CG_GOLDRUSH_RESERVATION_SYN:       //전장 예약
        {
            OnCG_GOLDRUSH_RESERVATION_SYN(user, packet, size);
        }break;
    case CG_GOLDRUSH_RESERVATION_CANCEL_SYN:    //예약취소
        {
            OnCG_GOLDRUSH_RESERVATION_CANCEL_SYN(user,packet,size);
        }break;
    default:
        {
            user->SendToLinkedServer(static_cast<MSG_BASE_FORWARD*>(packet),size);
        }break;
    }
}

void Handler_CG_GOLDRUSH::OnCG_GOLDRUSH_RESERVATION_SYN( User* user, MSG_BASE* packet, WORD size )
{
    MSG_CG_GOLDRUSH_RESERVATION_SYN* recv_packet = \
        static_cast<MSG_CG_GOLDRUSH_RESERVATION_SYN*>(packet);

    const MAPCODE map_code = recv_packet->map_code;
    MSG_CG_GOLDRUSH_RESERVATION_ACK ack_packet;

    BattleGroundManager* const battleground_manager = BattleGroundManager::Instance();
    if (battleground_manager == NULL)
    {
        ack_packet.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        user->SendPacket(&ack_packet, sizeof(ack_packet));
        return;
    }

    // 입장 제한 확인
    RC::eBATTLE_GROUND_RESULT result_code = battleground_manager->CanReservation(user, map_code);
    if (result_code != RC::RC_BATTLE_GROUND_RESERVATION_SUCCESS )
    {
        ack_packet.result_code = result_code;
        user->SendPacket(&ack_packet, sizeof(ack_packet));
        return;
    }

    //Agent->GameServer에서 검증
    MSG_AG_GOLDRUSH_CAN_RESERVATION_SYN msg;
    msg.map_code = map_code;
    msg.is_healer = recv_packet->is_healer;
    user->SendToLinkedServer(&msg, sizeof(msg));
}

void Handler_CG_GOLDRUSH::OnCG_GOLDRUSH_RESERVATION_CANCEL_SYN( User* user, MSG_BASE* packet, WORD size )
{
    MSG_CG_GOLDRUSH_RESERVATION_CANCEL_SYN* recv_packet = \
        static_cast<MSG_CG_GOLDRUSH_RESERVATION_CANCEL_SYN*>(packet);

    USERGUID guid = user->GetUserKey();
    BattleGroundManager* const battleground_manager = BattleGroundManager::Instance();

    MSG_CG_GOLDRUSH_RESERVATION_CANCEL_ACK ack_packet;
    ack_packet.result_code = RC::RC_BATTLE_GROUND_RESERVATION_CANCEL_FAIL;

    BattleGroundMemberInfo* member = battleground_manager->FindBattleGroundMember(guid);
    if (member)
    {
        if (member->group_key == 0) // 1차매칭 하지 않은 유저
        {
            battleground_manager->RemoveMemberStandbyGroup(member);
            battleground_manager->RemoveReservationMember(guid);
            ack_packet.result_code = RC::RC_BATTLE_GROUND_RESERVATION_CANCEL_SUCCESS;
        }
        else {
            ack_packet.result_code = RC::RC_BATTLE_GROUND_STATE_INCLUDE_MATCHING_GROUPS;
        }
    }
    user->SendPacket(&ack_packet, sizeof(ack_packet));

    // 상태변경
    ;{
        MSG_AG_GOLDRUSH_STATUS_CHANGE_STATE_CMD cmd_packet;
        cmd_packet.behave_sate = PLAYER_BEHAVE_IDLE_STATE;
        user->SendToLinkedServer(&cmd_packet, sizeof(cmd_packet));
    };
}

void Handler_CG_GOLDRUSH::OnCG_GOLDRUSH_ABNORMAL_EXIT_USER_REMOVE_SYN( User* user, MSG_BASE* packet, WORD size )
{
    MSG_CG_GOLDRUSH_ABNORMAL_EXIT_USER_REMOVE_SYN* recv_msg = 
        static_cast<MSG_CG_GOLDRUSH_ABNORMAL_EXIT_USER_REMOVE_SYN*>(packet);
}
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH