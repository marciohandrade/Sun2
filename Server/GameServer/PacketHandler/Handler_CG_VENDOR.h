#pragma once
#ifndef GAMESERVER_CG_VENDOR_H
#define GAMESERVER_CG_VENDOR_H

#include "./PacketHandler.h"




#define Handler_CG_VENDOR_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_VENDOR_IMPL(p) \
    void Handler_CG_VENDOR::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_VENDOR
{
public:
    Handler_CG_VENDOR();
    ~Handler_CG_VENDOR();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_VENDOR_DECL(CG_VENDOR_START_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_END_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_MODIFY_REQ_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_MODIFY_CANCEL_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_MODIFY_APPLY_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_INSERT_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_DELETE_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_BUY_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_MODIFY_TITLE_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_VIEW_START_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_VIEW_END_SYN);

    //Through Vendor List NPC
    Handler_CG_VENDOR_DECL(CG_VENDOR_LIST_START_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_REQUIRE_PAGE_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_SEARCH_SYN);
    Handler_CG_VENDOR_DECL(CG_VENDOR_LIST_END_SYN);

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    Handler_CG_VENDOR_DECL(CG_VENDOR_ENHANCED_SEARCH_SYN);
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
};

#endif //GAMESERVER_CG_VENDOR_H