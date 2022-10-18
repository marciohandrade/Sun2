#include "stdafx.h"

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "ChaosZoneSystem/Handler_AG_GOLDRUSH.h"
#include "ChaosZoneSystem/Handler_CG_GOLDRUSH.h"
#include "PacketStruct_AG_ChaosZoneSystem.h"

#include "ChaosZoneSystem/BattleGroundManager.h"
#include "ChaosZoneSystem/BattleGroundMatchingSystem.h"
#include "AgentParty/AgentPartyManager.h"
#include "AgentParty/PartyRoomFinder.h"

void Handler_AG_GOLDRUSH::OnAG_ProcessPacket( GameServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_CG_GOLDRUSH_PACKET* recv_packet = static_cast<MSG_CG_GOLDRUSH_PACKET*>(packet);
    switch(recv_packet->packet_type)
    {
    case AG_BATTLE_GROUND_LEAVE_CMD:
        {
            OnAG_GOLDRUSH_LEAVE_CMD(server_session, packet, size);
        }
        break;
    case AG_BATTLE_GROUND_CAN_RESERVATION_ACK:
        {
            OnAG_GOLDRUSH_CAN_RESERVATION_ACK(server_session, packet, size);
        }
        break;
    }
}

void Handler_AG_GOLDRUSH::OnAG_GOLDRUSH_LEAVE_CMD( GameServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_AG_GOLDRUSH_LEAVE_CMD* recv_packet = static_cast<MSG_AG_GOLDRUSH_LEAVE_CMD*>(packet);
    User* const user = UserManager::Instance()->GetUser(packet->m_dwKey);

    BattleGroundManager::Instance()->LeaveRoom(user,recv_packet->zone_key);
}

void Handler_AG_GOLDRUSH::OnAG_GOLDRUSH_CAN_RESERVATION_ACK( GameServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_AG_GOLDRUSH_CAN_RESERVATION_ACK* recv_packet = \
        static_cast<MSG_AG_GOLDRUSH_CAN_RESERVATION_ACK*>(packet);

    BattleGroundManager* battle_ground_manager = BattleGroundManager::Instance();
    User* const user = UserManager::Instance()->GetUser(packet->m_dwKey);
    if (user == NULL)
    {
        return;
    }

    MSG_CG_GOLDRUSH_RESERVATION_ACK ack_msg;
    ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_SUCCESS;

    // 
    if (recv_packet->result_code != RC::RC_BATTLE_GROUND_RESERVATION_SUCCESS)
    {
        ack_msg.result_code = recv_packet->result_code;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    const MAPCODE map_code = recv_packet->map_code;
    int equip_item_score = recv_packet->equip_item_score;
    eCHAR_TYPE class_code = recv_packet->class_code;
    bool is_healer = recv_packet->is_healer;

    if (battle_ground_manager->InsertReservationMember(user->GetUserKey(), map_code, 
        equip_item_score, class_code, is_healer) == false) 
    {
        // �Ŵ����� �������
        ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    BattleGroundMemberInfo* const member = \
        battle_ground_manager->FindBattleGroundMember(user->GetUserGUID());
    if (member == NULL)
    {
        //InsertReservationMember() ���� ����ó�������� ����Ʈ�� ���� ���� ���
        ack_msg.result_code = RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    // ��Ʋ�� ���� ��� ���
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomFinder* room_finder = party_manager->GetPartyRoomFinder();
    if (room_finder != NULL)
    {
        room_finder->DeleteAllCharGuidFromMissionList(user->GetSelectedCharGuid());
    }

    battle_ground_manager->InsertMemberStandbyGroup(member);
    ack_msg.goldrush_reservation_key = member->reservation_key;
    user->SendPacket(&ack_msg, sizeof(ack_msg));

    // ������ ������ �Ҷ����� üũ..
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    battle_ground_manager->CheckMatching(map_code);

    //
    ;{
        MSG_CG_GOLDRUSH_RESERVATION_BRD brd_msg;
        BattleGroundMemberList* const member_list = battle_ground_manager->FindStandbyGroupMemberList(map_code);
        if (member_list == NULL)
        {
            return;
        }
        brd_msg.current_reservation_memeber_count = member_list->size();

        BattleGroundMemberList::const_iterator const_itr = member_list->begin();
        for (; const_itr != member_list->end(); ++const_itr)
        {
            BattleGroundMemberInfo* const member = *const_itr;
            if (member == NULL)
            {
                continue;
            }
            User* const user = UserManager::Instance()->GetUser(member->user_guid);
            if (user == NULL)
            {
                continue;
            }
            user->SendPacket(&brd_msg, sizeof(brd_msg));
        }
    };

    // ���º���..
    ;{
        MSG_AG_GOLDRUSH_STATUS_CHANGE_STATE_CMD cmd_msg;
        cmd_msg.behave_sate = PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE;
        user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));
    };
}

#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH