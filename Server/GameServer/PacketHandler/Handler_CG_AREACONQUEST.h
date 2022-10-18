#pragma once
#ifndef GAMESERVER_CG_AREACONQUEST_H
#define GAMESERVER_CG_AREACONQUEST_H

#include "./PacketHandler.h"




#define Handler_CG_AREACONQUEST_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_AREACONQUEST_IMPL(p) \
    void Handler_CG_AREACONQUEST::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_AREACONQUEST
{
public:
    Handler_CG_AREACONQUEST(){}
    ~Handler_CG_AREACONQUEST(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_AREACONQUEST_DECL(CG_AREACONQUEST_REWARD_SYN);
};

#endif //GAMESERVER_CG_AREACONQUEST_H