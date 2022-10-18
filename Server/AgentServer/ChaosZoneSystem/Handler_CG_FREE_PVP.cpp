#include "StdAfx.h"

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include <ChaosZoneSystem/Handler_CG_FREE_PVP.h>
#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <ChaosZoneSystem/FreePVPManager.h>
#include <ChaosZoneSystem/FreePVPRoom.h>
#include <Zone/ZoneManager.h>
#include <Zone/InstanceDungeon.h>
#include <PacketHandler/Handler_FromClient.h>
#include <ServerOptionParserEx.h>
#include <ChaosZoneSystem/BattleGroundManager.h>

void Handler_CG_FREE_PVP::OnCG_ProcessPacket(User* user, MSG_BASE* packet, WORD size)
{
    MSG_CG_FREE_PVP_PACKET* recv_packet = static_cast<MSG_CG_FREE_PVP_PACKET*>(packet);

    switch(recv_packet->packet_type)
    {
    case CG_FREE_PVP_ROOM_LIST_SYN:
        {
            OnCG_FREE_PVP_ROOM_LIST_SYN(user, packet, size);
        }
        break;
    case CG_FREE_PVP_CREATE_ROOM_SYN:
        {
            OnCG_FREE_PVP_CREATE_ROOM_SYN(user, packet, size);
        }
        break;

    case CG_FREE_PVP_JOIN_ROOM_SYN:
        {
            OnCG_FREE_PVP_JOIN_ROOM_SYN(user, packet, size);
        }
        break;
    default:
        {
            user->SendToLinkedServer(static_cast<MSG_BASE_FORWARD*>(packet), size);
        }
        break;
    }
}

void Handler_CG_FREE_PVP::OnCG_FREE_PVP_ROOM_LIST_SYN(User* user, MSG_BASE* packet, WORD size)
{
    const MSG_CG_FREE_PVP_ROOM_LIST_SYN* recv_packet = 
        static_cast<MSG_CG_FREE_PVP_ROOM_LIST_SYN*>(packet);

    FreePVPManager* free_pvp_manager = FreePVPManager::Instance();

    MSG_CG_FREE_PVP_ROOM_LIST_ACK ack_msg;
    ack_msg.count_of_list = free_pvp_manager->ArchiveRoomList(ack_msg.free_pvp_room_info,
                                                            _countof(ack_msg.free_pvp_room_info));

    user->SendPacket(&ack_msg, ack_msg.GetSize());
}

void Handler_CG_FREE_PVP::OnCG_FREE_PVP_CREATE_ROOM_SYN(User* user, MSG_BASE* packet, WORD size)
{
    MSG_CG_FREE_PVP_CREATE_ROOM_SYN* recv_packet = \
        static_cast<MSG_CG_FREE_PVP_CREATE_ROOM_SYN*>(packet);

    MSG_CG_FREE_PVP_CREATE_ROOM_ACK ack_msg;
    ack_msg.result_code = RC::RC_FREE_PVP_CREATE_SUCCESS;

    FreePVPManager* free_pvp_manager = FreePVPManager::Instance();

    // 자유전투실 활성화 여부..
    if (free_pvp_manager->GetFreePVPActive() == false)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL_INACTIVE;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

	// 다른 행동 중 일때..(거래, 상점)
    if (user->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL_TRANSACTION;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    // 전장 입장 예약 중 일때..
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    if (battle_ground_manager)
    {
        if (battle_ground_manager->IsValidReservationMember(user->GetUserKey()))
        {
            ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL_ALREADY_BATTLE_GROUND_RESERVATION;
            user->SendPacket(&ack_msg, sizeof(ack_msg));
            return;
        }
    }

    SERVER_COMMON_OPTION& server_option_info = ServerOptionParserEx::Instance()->GetServerOption();
    // 자유전투실 방 갯수 제한..
    if (free_pvp_manager->GetRoomCount() > server_option_info.free_pvp_max_room_count) //script..
    {
        ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL_MAX_ROOM;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    // 현재 방 입장을 대기 중인 유저가 방을 만들려고 할때..
    if (free_pvp_manager->IstMoveZoneWaitPlayer(user->GetUserGUID()) == true)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL_WAIT_MOVE_ZONE;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    // 이미 트랜잭션중이면
    if(user->IsBeginTransaction())
    {
        ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL_TRANSACTION;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    user->BeginTransaction(TR_CREATE_INSTANCE_DUNGEON);

    // 인스턴스 던전 생성..
    const MAPCODE map_code = recv_packet->base_free_pvp_info.map_code;
    InstanceDungeon* instance_dungeon = NULL;
    const RC::eROOM_RESULT rt = g_ZoneManager.CreateInstanceDungeon( \
        0, NULL,
        map_code, (InstanceDungeon *&)instance_dungeon,
        INDUN_KIND_CHAOS_ZONE, FALSE);

    if (!instance_dungeon || rt != RC::RC_ROOM_SUCCESS)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL_ZONE;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        user->EndTransaction();
        return;
    }
    instance_dungeon->SetRoomState(eROOMSTATE_GENERNAL);

    // 리스트 생성
    KEYTYPE room_key = 0;
    if (free_pvp_manager->CreateRoom(recv_packet->base_free_pvp_info, room_key) == false)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        user->EndTransaction();
        return;
    }

    // 룸에 존 키 셋팅
    FreePVPRoom* const free_pvp_room = free_pvp_manager->FindRoom(room_key);
    if (free_pvp_room == NULL)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        user->EndTransaction();
        return;
    }

    free_pvp_room->SetZoneKey(instance_dungeon->GetKey());
    SUNLOG(eCRITICAL_LOG, " [%s|%d] Create FreePVP Room Guid = %d | Room Key(%d) ", 
        __FUNCTION__,
        __LINE__,
        user->GetUserGUID(),
        free_pvp_room->GetZoneKey());
    user->EndTransaction();

    // 바로 조인..
    MSG_CG_FREE_PVP_JOIN_ROOM_SYN request;
    {
        request.join_room_key = room_key;
        _tcsncpy(request.password, recv_packet->base_free_pvp_info.room_password, 
            _countof(request.password));   
    }
 
    OnCG_FREE_PVP_JOIN_ROOM_SYN(user, &request, request.GetSize());
}

void Handler_CG_FREE_PVP::OnCG_FREE_PVP_JOIN_ROOM_SYN(User* user, MSG_BASE* packet, WORD size)
{
    MSG_CG_FREE_PVP_JOIN_ROOM_SYN* recv_packet = static_cast<MSG_CG_FREE_PVP_JOIN_ROOM_SYN*>(packet);

    const KEYTYPE room_key = recv_packet->join_room_key;
    const bool is_private_room = recv_packet->is_private_room;

    MSG_CG_FREE_PVP_JOIN_ROOM_ACK ack_msg;
    FreePVPManager* free_pvp_manager = FreePVPManager::Instance();
    if (free_pvp_manager == NULL)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 자유전투실 활성화 여부..
    if (free_pvp_manager->GetFreePVPActive() == false)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_INACTIVE;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

	// 다른 행동 중 일때..(거래, 상점)
    if (user->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
        return;
    }

    // 전장 입장 예약 중 일때..
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    if (battle_ground_manager)
    {
        if (battle_ground_manager->IsValidReservationMember(user->GetUserKey()))
        {
            ack_msg.result_code = RC::RC_FREE_PVP_CREATE_FAIL_ALREADY_BATTLE_GROUND_RESERVATION;
            user->SendPacket(&ack_msg, sizeof(ack_msg));
            return;
        }
    }

    // 방이 없을 경우..
    FreePVPRoom* const free_pvp_room = free_pvp_manager->FindRoom(room_key);
    if (free_pvp_room == NULL)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_ISNOT_ROOM;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 해당 존이 존재하지 않을 경우..
    InstanceDungeon* const instance_dugeon = \
        static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(free_pvp_room->GetZoneKey()));
    if (instance_dugeon == NULL) 
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_ISNOT_ROOM;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        free_pvp_manager->DestroyRoom(room_key);
        return;
    }

    // 해당 존 상태가 일반적이지 않을 경우..
    if (instance_dugeon->GetRoomState() != eROOMSTATE_GENERNAL)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }
    
    // 정원이 다 찼을경우..
    if (free_pvp_room->CanJoinRoomByMemberCount() == false)
    {
        ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_MAX_USER;
        user->SendPacket(&ack_msg,sizeof(ack_msg));
        return;
    }

    // 비번방일 경우..
    if (recv_packet->is_private_room)
    {
        if (free_pvp_room->CheckPassword(recv_packet->password) == false)
        {
            ack_msg.result_code = RC::RC_FREE_PVP_JOIN_FAIL_DIFFER_PASSWORD;
            user->SendPacket(&ack_msg,sizeof(ack_msg));
            return;
        }
    }

    // 필드에서 파티중일 경우.. 파티를 탈퇴하고 입장한다..
    MSG_CG_PARTY_LEAVE_SYN request;
    Handler_CG::OnCG_PARTY_LEAVE_SYN(user, &request, sizeof(request));

    MSG_CG_FREE_PVP_JOIN_ROOM_COUNT_CMD cmd_msg;
    user->SendPacket(&cmd_msg,sizeof(cmd_msg));
    free_pvp_manager->InsertFreePVPUser(user->GetUserGUID(), room_key);
}
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

