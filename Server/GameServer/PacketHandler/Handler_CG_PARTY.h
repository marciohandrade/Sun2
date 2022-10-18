#pragma once
#ifndef GAMESERVER_CG_PARTY_H
#define GAMESERVER_CG_PARTY_H

#include "./PacketHandler.h"



class Handler_CG_PARTY
{
public:
    Handler_CG_PARTY();
    ~Handler_CG_PARTY();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    static void OnCG_PARTY_MEMBER_WARP_REQ_SYN(ServerSession* pPlayer, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_PARTY_MEMBER_SUMMON_REQ_SYN(ServerSession* pPlayer, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_PARTY_MEMBER_SUMMON_RES_SYN(ServerSession* pPlayer, MSG_BASE* pMsg, WORD wSize);

};

#endif //GAMESERVER_CG_PARTY_H