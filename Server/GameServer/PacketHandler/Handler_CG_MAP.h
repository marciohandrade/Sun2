#pragma once
#ifndef GAMESERVER_CG_MAP_H
#define GAMESERVER_CG_MAP_H

#include "./PacketHandler.h"



class Handler_CG_MAP
{
public:
    Handler_CG_MAP();
    ~Handler_CG_MAP();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    static void OnCG_MAP_TELEPORT_SYN(ServerSession* pPlayer, MSG_BASE* pMsg, WORD wSize);

    //_NA_0_20100722_AUTO_SYSTEM
    static void OnCG_MAP_MOVE_LAST_RETURNSCROLL_POS_CMD(ServerSession* pPlayer, 
        MSG_BASE* pMsg, WORD wSize);
    // CHANGES: f110729.3L, _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    static void OnCG_MAP_KBMOVE_SERIAL_CHECK_ACK(ServerSession* pPlayer, 
        MSG_BASE* pMsg, WORD wSize);
};

#endif //GAMESERVER_CG_MAP_H