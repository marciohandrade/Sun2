#pragma once
#ifndef GAMESERVER_AG_EVENT_H
#define GAMESERVER_AG_EVENT_H

#include "./PacketHandler.h"



#define Handler_AG_EVENT_DECL(Protocol) \
    static void On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_AG_EVENT_IMPL(Protocol) \
    void Handler_AG_EVENT::On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_AG_EVENT
{
public:
    Handler_AG_EVENT();
    ~Handler_AG_EVENT();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    //D->W->A->G 模备 眠玫 格废 傈价
    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
    Handler_AG_EVENT_DECL(AG_EVENT_FRIENDINVITATE_INVITATELIST_CMD);
    Handler_AG_EVENT_DECL(AG_EVENT_RECALL_ACK);
    Handler_AG_EVENT_DECL(AG_EVENT_RECALL_NAK);
    //}
};

#endif //GAMESERVER_AG_EVENT_H