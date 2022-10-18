#pragma once
#ifndef GAMESERVER_CG_CONNECTION_H
#define GAMESERVER_CG_CONNECTION_H

#include "./PacketHandler.h"



#define Handler_CG_CONNECTION_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_CONNECTION_IMPL(p) \
    void Handler_CG_CONNECTION::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_CONNECTION
{
public:
    Handler_CG_CONNECTION() {}
    ~Handler_CG_CONNECTION() {}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    //  Handler_CG_CONNECTION_DECL(CG_CONNECTION_NOTIFY_LOGOUT_SYN);
    Handler_CG_CONNECTION_DECL(CG_CONNECTION_CLIENT_TICK_TEST_SYN);
};

#endif //GAMESERVER_CG_CONNECTION_H