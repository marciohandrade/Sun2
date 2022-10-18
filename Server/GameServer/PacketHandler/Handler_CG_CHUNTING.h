#pragma once
#ifndef GAMESERVER_CG_CHUNTING_H
#define GAMESERVER_CG_CHUNTING_H

#include "./PacketHandler.h"




#define Handler_CG_CHUNTNG_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_CHUNTING_IMPL(p) \
    void Handler_CG_CHUNTING::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_CHUNTING
{
public:
    Handler_CG_CHUNTING() {}
    ~Handler_CG_CHUNTING() {}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_CHUNTNG_DECL(CG_CHUNTING_POINT_INFO_SYN);
};

#endif //GAMESERVER_CG_CHUNTING_H