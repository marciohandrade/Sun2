#pragma once
#ifndef GAMESERVER_CG_WAREHOUSE_H
#define GAMESERVER_CG_WAREHOUSE_H

#include "./PacketHandler.h"




#define Handler_CG_WAREHOUSE_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_WAREHOUSE_IMPL(p) \
    void Handler_CG_WAREHOUSE::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_WAREHOUSE
{
public:
    Handler_CG_WAREHOUSE(){}
    ~Handler_CG_WAREHOUSE(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_WAREHOUSE_DECL(CG_WAREHOUSE_START_SYN);
    Handler_CG_WAREHOUSE_DECL(CG_WAREHOUSE_MONEY_SYN);
    Handler_CG_WAREHOUSE_DECL(CG_WAREHOUSE_END_SYN);

    Handler_CG_WAREHOUSE_DECL(CG_WAREHOUSE_PWD_SETTING_SYN);
    Handler_CG_WAREHOUSE_DECL(CG_WAREHOUSE_PWD_CHECK_SYN);
    Handler_CG_WAREHOUSE_DECL(CG_WAREHOUSE_PWD_LOCKING_SYN);
};

#endif //GAMESERVER_CG_WAREHOUSE_H