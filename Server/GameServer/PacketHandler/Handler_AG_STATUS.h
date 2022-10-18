#pragma once
#ifndef GAMESERVER_AG_STATUS_H
#define GAMESERVER_AG_STATUS_H

#include "./PacketHandler.h"

#define Handler_AG_STATUS_DECL(Protocol) \
    static void On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_AG_STATUS_IMPL(Protocol) \
    void Handler_AG_STATUS::On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)




class Handler_AG_STATUS
{
public:
    Handler_AG_STATUS() {}
    ~Handler_AG_STATUS() {}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_AG_STATUS_DECL(AG_STATUS_DECREASE_DURA_CMD);
    Handler_AG_STATUS_DECL(AG_STATUS_INTERCEPTION_ACK);
    Handler_AG_STATUS_DECL(AG_STATUS_INTERCEPTION_NAK);
    // __KR_001355_20090608_GROUPINGSYSTEM_GW = {
    Handler_AG_STATUS_DECL(AG_STATUS_GROUP_SET_MEMBERINFO_CMD);
    Handler_AG_STATUS_DECL(AG_STATUS_GROUP_ADDEXP_CMD);
    Handler_AG_STATUS_DECL(AG_STATUS_GROUP_ADDMONEY_CMD);
    // }
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    Handler_AG_STATUS_DECL(AG_STATUS_LOTTO_REWARD_SYN);
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    Handler_AG_STATUS_DECL(AG_STATUS_LOTTO_TICKET_PURCHASE_SYN);
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    //}

    Handler_AG_STATUS_DECL(AG_USER_DOMINATION_RETURNHIEM_CMD);
};

#endif //GAMESERVER_AG_STATUS_H