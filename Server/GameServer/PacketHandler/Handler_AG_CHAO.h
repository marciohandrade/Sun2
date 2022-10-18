#pragma once
#ifndef GAMESERVER_AG_CHAO_H
#define GAMESERVER_AG_CHAO_H

#include "./PacketHandler.h"

#define Handler_AG_CHAO_DECL(Protocol) \
    static void On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_AG_CHAO_IMPL(Protocol) \
    void Handler_AG_CHAO::On##Protocol(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)




class Handler_AG_CHAO
{
public:
    Handler_AG_CHAO() {}
    ~Handler_AG_CHAO() {}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_AG_CHAO_DECL(AG_CHAO_REQUEST_REVENGEINFO_ACK);
    Handler_AG_CHAO_DECL(AG_CHAO_REQUEST_REVENGEINFO_NAK);

    Handler_AG_CHAO_DECL(AG_CHAO_REMOVE_REVENGE_CHAR_CMD);

    Handler_AG_CHAO_DECL(AG_CHAO_REMOVE_REVENGE_CHAR_ACK);
    Handler_AG_CHAO_DECL(AG_CHAO_REMOVE_REVENGE_CHAR_NAK);

    Handler_AG_CHAO_DECL(AG_CHAO_ADD_REVENGE_CHAR_ACK);
    Handler_AG_CHAO_DECL(AG_CHAO_ADD_REVENGE_CHAR_NAK);

    Handler_AG_CHAO_DECL(AG_CHAO_TRACK_REVENGE_CHAR_ACK);
};

#endif //GAMESERVER_AG_CHAO_H