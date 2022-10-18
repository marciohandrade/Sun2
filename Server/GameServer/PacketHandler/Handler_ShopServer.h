#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL



#pragma once
#ifndef GAMESERVER_SHOPSERVER_H
#define GAMESERVER_SHOPSERVER_H

#include "./PacketHandler.h"

#define Handler_ShopServer_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)
#define Handler_ShopServer_IMPL(p) \
    void Handler_ShopServer::On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)



class Handler_ShopServer
{
public:
    Handler_ShopServer();
    ~Handler_ShopServer();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_ShopServer_DECL(GS_SHOP_USER_CASH_REQ_ACK);
    Handler_ShopServer_DECL(GS_SHOP_ITEM_LIST_REQ_ACK);
    Handler_ShopServer_DECL(GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK);

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    Handler_ShopServer_DECL(GS_SHOP_MILEAGE_LIST_REQ_ACK);
#endif
    Handler_ShopServer_DECL(GS_SHOP_PACK_LIST_REQ_ACK);
    Handler_ShopServer_DECL(GS_SHOP_BRANCH_LIST_REQ_ACK);
    Handler_ShopServer_DECL(GS_SHOP_BUY_ITEM_ACK);
    Handler_ShopServer_DECL(GS_SHOP_USER_BUY_HISTORY_ACK);
    Handler_ShopServer_DECL(GS_SHOP_RELOAD_DATA_NTF);

    Handler_ShopServer_DECL(GS_SHOP_SEND_GIFT_ACK);
    Handler_ShopServer_DECL(GS_SHOP_ACTION_RECV_GIFT_ACK);
    Handler_ShopServer_DECL(GS_SHOP_IS_RECV_GIFT_ACK);
    Handler_ShopServer_DECL(GS_SHOP_RECV_GIFT_LIST_ACK);
    Handler_ShopServer_DECL(GS_SHOP_SEND_GIFT_LIST_ACK);
};

#endif //GAMESERVER_SHOPSERVER_H

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
