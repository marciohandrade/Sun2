#include "StdAfx.h"

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include <ChaosZoneSystem/Handler_AG_FREE_PVP.h>
#include <PacketStruct_AG_ChaosZoneSystem.h>
#include <ChaosZoneSystem/FreePVPManager.h>

void Handler_AG_FREE_PVP::OnAG_ProcessPacket(GameServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_AG_FREE_PVP_PACKET* recv_packet = static_cast<MSG_AG_FREE_PVP_PACKET*>(packet);
    switch(recv_packet->packet_type)
    {
    case AG_FREE_PVP_LEAVE_ROOM_CMD:
        {
            OnAG_FREE_PVP_LEAVE_ROOM_CMD(server_session, packet, size);
        }
        break;

    case AG_FREE_PVP_ACTIVE_CMD:
        {
            OnAG_FREE_PVP_ACTIVE_CMD(server_session, packet, size);
        }
        break;
    }
}

void Handler_AG_FREE_PVP::OnAG_FREE_PVP_LEAVE_ROOM_CMD(GameServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_AG_FREE_PVP_LEAVE_ROOM_CMD* recv_packet = 
        static_cast<MSG_AG_FREE_PVP_LEAVE_ROOM_CMD*>(packet);

    User* user = UserManager::Instance()->GetUser(recv_packet->m_dwKey);
    FreePVPManager::Instance()->LeaveRoom(user, recv_packet->zone_key);
}

void Handler_AG_FREE_PVP::OnAG_FREE_PVP_ACTIVE_CMD(GameServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_AG_FREE_PVP_ACTIVE_CMD* recv_packet = static_cast<MSG_AG_FREE_PVP_ACTIVE_CMD*>(packet);
    FreePVPManager::Instance()->SetFreePVPActive(recv_packet->is_free_pvp_active);
}
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP

