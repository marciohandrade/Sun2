#pragma once
#ifndef GAMESERVER_HANDLER_CLIENT_H
#define GAMESERVER_HANDLER_CLIENT_H

#include "./PacketHandler.h"




#define Handler_Client_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)
#define Handler_Client_IMPL(p) \
    void Handler_Client::On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)

class Handler_Client
{
public:
    Handler_Client(){}
    ~Handler_Client(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_Client_DECL(CG_MISSION_RANKING_INFO_SYN);
    Handler_Client_DECL(CG_MISSION_RANKING_MEMBER_INFO_SYN);
    Handler_Client_DECL(CG_MISSION_RANKING_REWARD_SYN);

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    // ¼¥ ÆÐÅ¶
    Handler_Client_DECL(CG_SHOP_CASH_REQ_SYN);
    Handler_Client_DECL(CG_SHOP_ITEM_LIST_REQ_SYN);
    Handler_Client_DECL(CG_SHOP_ITEM_REQ_SYN);          //< 2006/12/20 Ãß°¡
    Handler_Client_DECL(CG_SHOP_ITEM_BUY_SYN);
    Handler_Client_DECL(CG_SHOP_BUY_HISTORY_SYN);

    //Handler_Client_DECL(CG_SHOP_PACKAGE_BUY_SYN);
    //Handler_Client_DECL(CG_SHOP_CHECK_CHAR_NAME_SYN);
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        Handler_Client_DECL(CG_SHOP_SEND_GIFT_RELAY_SYN);
    #else
        Handler_Client_DECL(CG_SHOP_SEND_GIFT_SYN);
    #endif

    Handler_Client_DECL(CG_SHOP_ACTION_RECV_GIFT_SYN);
    Handler_Client_DECL(CG_SHOP_IS_RECV_GIFT_SYN);
    Handler_Client_DECL(CG_SHOP_RECV_GIFT_LIST_SYN  );
#endif // !defined _NA_0_20110613_GAME_SHOP_RENEWAL



    //³¬½Ã



// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
    Handler_Client_DECL(CG_ZONE_PLAYER_BATTLE_RESULT_SYN);

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    Handler_Client_DECL(CG_HONORSYSTEM_PACKET);
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    Handler_Client_DECL(CG_POINTWALLET_PACKET);
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    Handler_Client_DECL(CG_CHAOS_ZONE_PACKET);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    Handler_Client_DECL(CG_MISSION_RANKING_SETTLEDDATE_INFO_SYN);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    Handler_Client_DECL(CG_CHARINFO_SMARTNPC_PACKET);
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    Handler_Client_DECL(CG_ETC_SUN_RANKING_SYN);
    Handler_Client_DECL(CG_ETC_SUN_RANKING_SETTLE_INFO_SYN);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

};

#endif //GAMESERVER_HANDLER_CLIENT_H