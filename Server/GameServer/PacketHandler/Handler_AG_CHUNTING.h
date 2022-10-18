#pragma once
#ifndef GAMESERVER_AG_CHUNTING_H
#define GAMESERVER_AG_CHUNTING_H

#include "./PacketHandler.h"



#define Handler_AG_CHUNTING_DECL(Protocol) \
    static void On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_AG_CHUNTING_IMPL(Protocol) \
    void Handler_AG_CHUNTING::On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_AG_CHUNTING
{
public:
    Handler_AG_CHUNTING();
    ~Handler_AG_CHUNTING();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_AG_CHUNTING_DECL(AG_CHUNTING_ENTER_TEAM_CMD);
};

#endif //GAMESERVER_AG_CHUNTING_H
