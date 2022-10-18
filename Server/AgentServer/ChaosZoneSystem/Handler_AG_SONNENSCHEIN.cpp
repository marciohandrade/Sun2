#include "stdafx.h"

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "Handler_AG_SONNENSCHEIN.h"
#include "SonnenScheinManager.h"
#include "SonnenScheinRoom.h"
#include "PacketStruct_AG_ChaosZoneSystem.h"
#include "ServerSessions/GameServerSession.h"

void Handler_AG_SONNENSCHEIN::OnAG_ProcessPacket( GameServerSession* server_session, MSG_BASE* packet, WORD size )
{
    MSG_AG_SONNENSCHEIN_PACKET* received_msg = static_cast<MSG_AG_SONNENSCHEIN_PACKET*>(packet);

    switch(received_msg->packet_type)
    {
    case AG_SONNENSCHEIN_REQUEST_GUID_SYN:
        {
            OnAG_SONNENSCHEIN_REQUEST_GUID_SYN(server_session, packet, size);
        }
        break;
    case AG_SONNENSCHEIN_KICKUSER_CMD:
        {
            OnAG_SONNENSCHEIN_KICKUSER_CMD(server_session, packet, size);
        } break;
    case AG_SONNENSCHEIN_ROOM_DESTROY_CMD:
        {
            OnAG_SONNENSCHEIN_ROOM_DESTROY_CMD(server_session, packet, size);
        } break;
    case AG_SONNENSCHEIN_STAMP_COMPLETE_CMD:
        {
            OnAG_SONNENSCHEIN_STAMP_COMPLETE_CMD(server_session, packet, size);
        } break;
    case AG_SONNENSCHEIN_ENTER_CHECK_ACK:
        {
            OnAG_SONNENSCHEIN_ENTER_CHECK_ACK(server_session, packet, size);
        } break;
    }
}

Handler_AG_SONNENSCHEIN_IMPL(AG_SONNENSCHEIN_REQUEST_GUID_SYN)
{
    const MSG_AG_SONNENSCHEIN_REQUEST_GUID_SYN* recv_msg = 
        static_cast<MSG_AG_SONNENSCHEIN_REQUEST_GUID_SYN*>(packet);

    User* const user = UserManager::Instance()->GetUser(packet->m_dwKey);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Cannot Find User! [UserKey:%d]", packet->m_dwKey);
        return;
    }

    SonnenScheinManager* sonnenschein_mgr = SonnenScheinManager::Instance();
    SonnenScheinRoom* room = sonnenschein_mgr->FindRoomUsingRoomKey(recv_msg->room_key);

    ;{
        MSG_AG_SONNENSCHEIN_REQUEST_GUID_ACK ack_msg;

        ack_msg.room_key = recv_msg->room_key;
        ack_msg.creater_char_guid_ = user->GetSelectedCharGuid();
        ack_msg.creater_guild_guid_ = user->GetSelectedCharGuildGuid();

        server_session->SendPacket(&ack_msg, sizeof(ack_msg));
    };
}

Handler_AG_SONNENSCHEIN_IMPL(AG_SONNENSCHEIN_KICKUSER_CMD)
{
    const MSG_AG_SONNENSCHEIN_KICKUSER_CMD* recv_msg = 
        static_cast<MSG_AG_SONNENSCHEIN_KICKUSER_CMD*>(packet);

    SonnenScheinManager* manager = SonnenScheinManager::Instance();
    if (recv_msg->room_key == 0) {
        manager->KickAllPlayers();
    }
    else {
        manager->KickRoomPlayers(recv_msg->room_key);
    }
        
}

Handler_AG_SONNENSCHEIN_IMPL(AG_SONNENSCHEIN_ROOM_DESTROY_CMD)
{
    const MSG_AG_SONNENSCHEIN_ROOM_DESTROY_CMD* recv_msg = 
        static_cast<MSG_AG_SONNENSCHEIN_ROOM_DESTROY_CMD*>(packet);

    SonnenScheinManager* manager = SonnenScheinManager::Instance();
    manager->DestroyRoom(recv_msg->room_key);
}

Handler_AG_SONNENSCHEIN_IMPL(AG_SONNENSCHEIN_STAMP_COMPLETE_CMD)
{
    const MSG_AG_SONNENSCHEIN_STAMP_COMPLETE_CMD* recv_msg = 
        static_cast<MSG_AG_SONNENSCHEIN_STAMP_COMPLETE_CMD*>(packet);

    if (recv_msg->room_key != 0) {
        SonnenScheinManager* manager = SonnenScheinManager::Instance();
        manager->StampComplete(recv_msg->room_key);
    }
}

Handler_AG_SONNENSCHEIN_IMPL(AG_SONNENSCHEIN_ENTER_CHECK_ACK)
{
    const MSG_AG_SONNENSCHEIN_ENTER_CHECK_ACK* recv_msg = \
        static_cast<MSG_AG_SONNENSCHEIN_ENTER_CHECK_ACK*>(packet);

    SonnenScheinManager* sonnenschein_mgr = SonnenScheinManager::Instance();
    User* const user = UserManager::Instance()->GetUser(packet->m_dwKey);

    //½ÇÆÐ
    if (recv_msg->result_code != RC::RC_SONNENSCHEIN_SUCCESS)
    {
        MSG_CG_SONNENSCHEIN_ENTER_CHECK_ACK nak_msg;
        nak_msg.result_code = recv_msg->result_code;
        user->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    //MoveZone
    BOOL move_result = sonnenschein_mgr->MoveToSonnenScheinRoom(user);

    if (move_result == TRUE)
    {
        MSG_CG_SONNENSCHEIN_ENTER_CHECK_ACK ack_msg;
        ack_msg.result_code = RC::RC_SONNENSCHEIN_SUCCESS;
        ack_msg.sonnenschein_mapcode = sonnenschein_mgr->GetSonnenScheinMapCode();
        user->SendPacket(&ack_msg, sizeof(ack_msg));
    }
    else
    {
        MSG_CG_SONNENSCHEIN_ENTER_CHECK_ACK nak_msg;
        nak_msg.result_code = RC::RC_SONNENSCHEIN_FAILED;
        user->SendPacket(&nak_msg, sizeof(nak_msg));
    }
}
#endif //_NA_008334_20150608_SONNENSCHEIN
