#pragma once
#ifndef __HANDLER_DG_CONNECTION_H__
#define __HANDLER_DG_CONNECTION_H__

#include "./PacketHandler.h"

//==================================================================================================
#define Handler_DG_CONNECTION_DECL(p) \
    static void On##p(ServerSession* server_session, MSG_BASE* msg, WORD msg_size)
#define Handler_DG_CONNECTION_IMPL(p) \
    void Handler_DG_CONNECTION::On##p(ServerSession* server_session, MSG_BASE* msg, WORD msg_size)
//==================================================================================================

class Handler_DG_CONNECTION
{
public:
    Handler_DG_CONNECTION() {}
    ~Handler_DG_CONNECTION() {}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();
    Handler_DG_CONNECTION_DECL(DG_CONNECTION_GROUND_CMD);
    // NOTE: f110426.2L, added to prevent connectless db user problems.
    Handler_DG_CONNECTION_DECL(DG_CHARINFO_CONNECT_CHAR_CMD);
};


#endif // __HANDLER_DG_CONNECTION_H__