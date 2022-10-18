#pragma once
#ifndef __HANDLER_AG_CHARINFO_H__
#define __HANDLER_AG_CHARINFO_H__

#include "./PacketHandler.h"

#define Handler_AG_CHARINFO_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_AG_CHARINFO_IMPL(p) \
    void Handler_AG_CHARINFO::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)



class Handler_AG_CHARINFO
{
public:
    Handler_AG_CHARINFO();
    ~Handler_AG_CHARINFO();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_AG_CHARINFO_DECL(AG_CHARINFO_PCBANG_BILL_CMD);
};

#endif // __HANDLER_AG_CHARINFO_H__