#pragma once
#ifndef GAMESERVER_AG_ITEM_H
#define GAMESERVER_AG_ITEM_H

#include "./PacketHandler.h"

#define Handler_AG_ITEM_DECL(Protocol) \
    static void On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_AG_ITEM_IMPL(Protocol) \
    void Handler_AG_ITEM::On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)




class Handler_AG_ITEM
{
public:
    Handler_AG_ITEM() {}
    ~Handler_AG_ITEM() {}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_AG_ITEM_DECL(AG_ITEM_PORTAL_ACK);

    Handler_AG_ITEM_DECL(AG_ITEM_UNLOCK_CMD);
    Handler_AG_ITEM_DECL(AG_ITEM_DECREASE_UNLOCK_CMD);
    Handler_AG_ITEM_DECL(AG_ITEM_USE_DECREE_ITEM_SYN);

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    Handler_AG_ITEM_DECL(AG_ITEM_USE_HEIM_LOTTO_TICKET_ACK);
#endif
};

#endif //GAMESERVER_AG_ITEM_H