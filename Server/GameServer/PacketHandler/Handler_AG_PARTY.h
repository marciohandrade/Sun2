#pragma once
#ifndef GAMESERVER_AG_PARTY_H
#define GAMESERVER_AG_PARTY_H

#include "./PacketHandler.h"

#define Handler_AG_PARTY_DECL(Protocol) \
    static void On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_AG_PARTY_IMPL(Protocol) \
    void Handler_AG_PARTY::On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)




class Handler_AG_PARTY
{
public:
    Handler_AG_PARTY();
    ~Handler_AG_PARTY();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_AG_PARTY_DECL(AG_PARTY_CREATE_CMD);
    Handler_AG_PARTY_DECL(AG_PARTY_DESTROY_CMD);
    Handler_AG_PARTY_DECL(AG_PARTY_JOIN_CMD);
    Handler_AG_PARTY_DECL(AG_PARTY_LEAVE_CMD);
    Handler_AG_PARTY_DECL(AG_PARTY_CHANGE_MASTER_CMD);
    Handler_AG_PARTY_DECL(AG_PARTY_ITEM_DISTRIBUTION_CHANGE_CMD);
    Handler_AG_PARTY_DECL(AG_PARTY_JOIN_SYN);
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    Handler_AG_PARTY_DECL(AG_PARTY_CAN_CHANGE_OPTION_SYN);
    Handler_AG_PARTY_DECL(AG_PARTY_CAN_CREATE_ROOM_SYN);
    Handler_AG_PARTY_DECL(AG_PARTY_READY_STATE_SYN);
    Handler_AG_PARTY_DECL(AG_PARTY_NOTIFY_PARTY_MEMBER_INFO_CMD);
    Handler_AG_PARTY_DECL(AG_PARTY_PARTYROOM_ITEM_DELETE_CMD);
    Handler_AG_PARTY_DECL(AG_PARTY_RESET_READY_STATE_CMD);
    Handler_AG_PARTY_DECL(AG_PARTY_CAN_JOIN_ROOM_SYN);
    Handler_AG_PARTY_DECL(AG_PARTY_SEND_MEMBER_RENDER_INFO_CMD);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    Handler_AG_PARTY_DECL(AG_CHAOS_ZONE_PACKET);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
};

#endif //GAMESERVER_AG_PARTY_H