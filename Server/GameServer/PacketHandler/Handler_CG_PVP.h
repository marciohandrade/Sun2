#pragma once
#ifndef GAMESERVER_CG_PVP_H
#define GAMESERVER_CG_PVP_H

#include "./PacketHandler.h"




#define Handler_CG_PVP_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_PVP_IMPL(p) \
    void Handler_CG_PVP::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_PVP
{
public:
    Handler_CG_PVP();
    ~Handler_CG_PVP();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_PVP_DECL(CG_PVP_PRESS_KEY_SYN);
};

#endif //GAMESERVER_CG_PVP_H