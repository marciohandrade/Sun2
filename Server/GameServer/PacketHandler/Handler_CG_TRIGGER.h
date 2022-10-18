#pragma once
#ifndef __HANDLER_CG_TRIGGER_H__
#define __HANDLER_CG_TRIGGER_H__

#include "./PacketHandler.h"






#define Handler_CG_TRIGGER_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_TRIGGER_IMPL(p) \
    void Handler_CG_TRIGGER::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)


class Handler_CG_TRIGGER
{
public:
    Handler_CG_TRIGGER();
    ~Handler_CG_TRIGGER();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    static void OnCG_TRIGGER_CLICK_OBJECT_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_TRIGGER_ENTER_AREA_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_TRIGGER_LEAVE_AREA_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);

    Handler_CG_TRIGGER_DECL(CG_TRIGGER_REWARD_SYN);
};


#endif // __HANDLER_CG_TRIGGER_H__
