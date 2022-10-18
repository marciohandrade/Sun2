#include <stdafx.h>
#ifdef _NA_008334_20150608_SONNENSCHEIN

#include "Handler_CG_SONNENSCHEIN.h"

#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <PacketStruct_AG_ChaosZoneSystem.h>
#include <ChaosZoneSystem/SonnenScheinManager.h>


void Handler_CG_SONNENSCHEIN::OnCG_ProcessPacket( User* user, MSG_BASE* packet, WORD size )
{
    MSG_CG_SONNENSCHEIN_PACKET* received_msg = static_cast<MSG_CG_SONNENSCHEIN_PACKET*>(packet);

    switch(received_msg->packet_type)
    {
    case CG_SONNENSCHEIN_CREATE_SYN:
        {
            OnCG_SONNENSCHEIN_CREATE_SYN(user, packet, size);
        }
        break;
    case CG_SONNENSCHEIN_ENTER_CHECK_SYN:
        {
            OnCG_SONNENSCHEIN_ENTER_CHECK_SYN(user, packet, size);
        }
        break;
    case CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN:
        {
            OnCG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN(user, packet, size);
        }
        break;
    default:
        {
            user->SendToLinkedServer(static_cast<MSG_BASE_FORWARD*>(packet),size);
        }break;
    }
}

Handler_CG_SONNENSCHEIN_IMPL(CG_SONNENSCHEIN_CREATE_SYN)
{
    MSG_CG_SONNENSCHEIN_CREATE_SYN* recved_msg = static_cast<MSG_CG_SONNENSCHEIN_CREATE_SYN*>(packet);
    
    SonnenScheinManager* sonnenschein_mgr = SonnenScheinManager::Instance();

    RC::eSONNENSCHEIN_RESULT create_result = sonnenschein_mgr->CreateSonnenScheinRoom(user, FALSE);

    ;{
        MSG_CG_SONNENSCHEIN_CREATE_ACK ack_msg;
        ack_msg.result_code = create_result;
        user->SendPacket(&ack_msg, sizeof(ack_msg));
    };
}

Handler_CG_SONNENSCHEIN_IMPL(CG_SONNENSCHEIN_ENTER_CHECK_SYN)
{
    MSG_CG_SONNENSCHEIN_ENTER_CHECK_SYN* recv_msg = 
        static_cast<MSG_CG_SONNENSCHEIN_ENTER_CHECK_SYN*>(packet);

    SonnenScheinManager* sonnenschein_mgr = SonnenScheinManager::Instance();
    //입장가능검사
    RC::eSONNENSCHEIN_RESULT check_result = sonnenschein_mgr->CheckUserEnterSonnenSchein(user, FALSE);
   if (check_result != RC::RC_SONNENSCHEIN_SUCCESS) {
       MSG_CG_SONNENSCHEIN_ENTER_CHECK_ACK ack_msg;
       ack_msg.result_code = check_result;
       user->SendPacket(&ack_msg, sizeof(ack_msg));
       return;
   }

   //방이 생성되어있나?
   SonnenScheinRoom* room =
       sonnenschein_mgr->FindRoomUsingGuildGuid(user->GetSelectedCharGuildGuid());
   if (room == NULL) {
       MSG_CG_SONNENSCHEIN_ENTER_CHECK_ACK ack_msg;
       ack_msg.result_code = RC::RC_SONNENSCHEIN_ROOM_NO_EXIST;
       user->SendPacket(&ack_msg, sizeof(ack_msg));
       return;
   }

    MSG_AG_SONNENSCHEIN_ENTER_CHECK_SYN ag_syn_msg;
    ag_syn_msg.entrance_item_code = recv_msg->entrance_item_code;
    ag_syn_msg.m_dwKey = user->GetUserKey();
    ag_syn_msg.map_code = sonnenschein_mgr->GetSonnenScheinMapCode();
    user->SendToLinkedServer(&ag_syn_msg, sizeof(ag_syn_msg));
}

Handler_CG_SONNENSCHEIN_IMPL(CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN)
{
    MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN* recv_msg = \
        static_cast<MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_SYN*>(packet);

    SonnenScheinManager* sonnenschein_mgr = SonnenScheinManager::Instance();

    GUILDGUID guild_guid = user->GetSelectedCharGuildGuid();

    MSG_CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK ack_msg;
    BOOL result = sonnenschein_mgr->GetEnterPlayerList_CharGuid(guild_guid, \
                                    ack_msg.enter_player_list, &ack_msg.enter_player_count);

    user->SendPacket(&ack_msg, sizeof(ack_msg));
}
#endif //_NA_008334_20150608_SONNENSCHEIN