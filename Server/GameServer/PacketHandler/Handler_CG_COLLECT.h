#pragma once
#ifndef GAMESERVER_CG_COLLECT_H
#define GAMESERVER_CG_COLLECT_H

#include "./PacketHandler.h"

class Handler_CG_COLLECT
{
public:
    Handler_CG_COLLECT();
    ~Handler_CG_COLLECT();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    static void OnCG_COLLECT_ACTION_SYN(ServerSession* pPlayer, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_COLLECT_CANCEL_SYN(ServerSession* pPlayer, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_COLLECT_ALLCOLLECTIVE_SYN(ServerSession* pPlayer, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_COLLECT_SUCCESS_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
};

#endif //GAMESERVER_CG_COLLECT_H