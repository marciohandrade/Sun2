#pragma once
#ifndef GAMESERVER_AG_GM_H
#define GAMESERVER_AG_GM_H

#include "./PacketHandler.h"

#define Handler_AG_GM_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_AG_GM_IMPL(p) \
    void Handler_AG_GM::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)




class Handler_AG_GM
{
public:
    Handler_AG_GM();
    ~Handler_AG_GM();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_AG_GM_DECL(AG_GM_TELEPORT_CMD);
    Handler_AG_GM_DECL(AG_GM_CHAR_INFO_SYN);
    Handler_AG_GM_DECL(AG_GM_TRACKING_REQUEST_INFO_SYN);
};

#endif //GAMESERVER_AG_GM_H
