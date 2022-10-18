#pragma once
#ifndef GAMESERVER_AG_CONNECTION_H
#define GAMESERVER_AG_CONNECTION_H

#include "./PacketHandler.h"



#define Handler_AG_CONNECTION_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_AG_CONNECTION_IMPL(p) \
    void Handler_AG_CONNECTION::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)


class Handler_AG_CONNECTION
{
public:
    Handler_AG_CONNECTION();
    ~Handler_AG_CONNECTION();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_AG_CONNECTION_DECL(AG_CONNECTION_GROUND_CMD);
    Handler_AG_CONNECTION_DECL(AG_CONNECTION_DBCONNECT_UPDATE_CMD);
    //  Handler_AG_CONNECTION_DECL(AG_CONNECTION_UNREGISTER_SYN);

    //  Handler_AG_CONNECTION_DECL(AG_CONNECTION_ENTERVILLAGEREQ_SYN);
    Handler_AG_CONNECTION_DECL(AG_CONNECTION_DISCONNECT_CMD);

    Handler_AG_CONNECTION_DECL(AG_WORLDSERVER_INFO_CMD);

    //static void OnAG_CONNECTION_UNREGISTER_CMD(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    Handler_AG_CONNECTION_DECL(AG_CONNECTION_SERVER_INFO_CMD);
    Handler_AG_CONNECTION_DECL(AG_WORLDSERVER_ABUSING_USER_VERIFY_QUERY_CMD);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    Handler_AG_CONNECTION_DECL(AG_CONNECTION_SERVER_SELECT_CONTROL_SYN);
#endif
};

#endif //GAMESERVER_AG_CONNECTION_H
