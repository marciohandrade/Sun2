#pragma once
#ifndef __HANDLER_CG_STYLE_H__
#define __HANDLER_CG_STYLE_H__

#include "./PacketHandler.h"




#define Handler_CG_STYLE_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_STYLE_IMPL(p) \
    void Handler_CG_STYLE::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)


class Handler_CG_STYLE
{
public:
    Handler_CG_STYLE();
    ~Handler_CG_STYLE();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    static void OnCG_STYLE_SELECT_STYLE_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    Handler_CG_STYLE_DECL(CG_STYLE_LINK_SYN);
    Handler_CG_STYLE_DECL(CG_STYLE_UNLINK_SYN);
    Handler_CG_STYLE_DECL(CG_STYLE_LINKMOVE_SYN);
};


#endif // __HANDLER_CG_STYLE_H__
