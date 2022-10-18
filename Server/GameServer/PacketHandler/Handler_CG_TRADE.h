#pragma once
#ifndef __HANDLER_CG_TRADE_H__
#define __HANDLER_CG_TRADE_H__

#include "./PacketHandler.h"





#define Handler_CG_TRADE_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_TRADE_IMPL(p) \
    void Handler_CG_TRADE::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)


class Handler_CG_TRADE
{
public:
    Handler_CG_TRADE(){}
    ~Handler_CG_TRADE(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_TRADE_DECL(CG_TRADE_OPEN_SYN);
    Handler_CG_TRADE_DECL(CG_TRADE_PUT_SYN);
    Handler_CG_TRADE_DECL(CG_TRADE_PUT_MONEY_SYN);
    Handler_CG_TRADE_DECL(CG_TRADE_GET_SYN);
    Handler_CG_TRADE_DECL(CG_TRADE_GET_MONEY_SYN);
    Handler_CG_TRADE_DECL(CG_TRADE_PROPOSAL_SYN);
    Handler_CG_TRADE_DECL(CG_TRADE_MODIFY_SYN);
    Handler_CG_TRADE_DECL(CG_TRADE_ACCEPT_SYN);
    Handler_CG_TRADE_DECL(CG_TRADE_CANCEL_SYN);
    Handler_CG_TRADE_DECL(CG_TRADE_MODIFY_RESPONSE_SYN);

};


#endif // __HANDLER_CG_TRADE_H__
