#pragma once
#ifndef GAMESERVER_CG_CHAO_H
#define GAMESERVER_CG_CHAO_H

#include "./PacketHandler.h"




#define Handler_CG_CHAO_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_CHAO_IMPL(p) \
    void Handler_CG_CHAO::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_CHAO
{
public:
    Handler_CG_CHAO(){}
    ~Handler_CG_CHAO(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_CHAO_DECL(CG_CHAO_ADD_REVENGE_CHAR_SYN);

    Handler_CG_CHAO_DECL(CG_CHAO_TRACK_REVENGE_CHAR_SYN);
};

#endif //GAMESERVER_CG_CHAO_H