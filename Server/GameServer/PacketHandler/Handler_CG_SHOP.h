#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef HANDLER_CG_SHOP_H
#define HANDLER_CG_SHOP_H


#if _MSC_VER > 1000
#pragma once
#endif

#include "./PacketHandler.h"

#define Handler_CG_SHOP_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_SHOP_IMPL(p) \
    void Handler_CG_SHOP::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_SHOP
{
public:
    Handler_CG_SHOP(){}
    ~Handler_CG_SHOP(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_SHOP_DECL(CG_SHOP_VERSION_SYN);
    Handler_CG_SHOP_DECL(CG_SHOP_OPEN_SYN);
    Handler_CG_SHOP_DECL(CG_SHOP_CLOSE_NTF);
    Handler_CG_SHOP_DECL(CG_SHOP_CASH_SYN);
    Handler_CG_SHOP_DECL(CG_SHOP_BUY_STORAGE_LIST_SYN);
    Handler_CG_SHOP_DECL(CG_SHOP_GIFT_STORAGE_LIST_SYN);
    Handler_CG_SHOP_DECL(CG_SHOP_BUY_ITEM_SYN);
    Handler_CG_SHOP_DECL(CG_SHOP_SEND_GIFT_SYN);
    Handler_CG_SHOP_DECL(CG_SHOP_USE_STORAGE_ITEM_SYN);
    Handler_CG_SHOP_DECL(CG_SHOP_CHAR_NAME_CHECK_SYN);
    Handler_CG_SHOP_DECL(CG_SHOP_EXIST_GIFT_SYN);
};

#endif //HANDLER_CG_SHOP_H

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL