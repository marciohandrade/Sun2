#pragma once
#ifndef __HANDLER_CG_SKILL_H__
#define __HANDLER_CG_SKILL_H__

#include "./PacketHandler.h"




#define Handler_CG_SKILL_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_SKILL_IMPL(p) \
    void Handler_CG_SKILL::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_SKILL
{
public:
    Handler_CG_SKILL();
    ~Handler_CG_SKILL();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_SKILL_DECL(CG_SKILL_ACTION_SYN);
    Handler_CG_SKILL_DECL(CG_SKILL_SELECT_SKILLPOINT_SYN);
    // Casting Cancel
    Handler_CG_SKILL_DECL(CG_SKILL_CASTING_CANCEL_SYN);
    //_NA002217_100728_EP2_PERK_SYSTEM_
    Handler_CG_SKILL_DECL(CG_SKILL_PERK_CONTROL_SYN);
    //                    CG_SKILL_PERK_CONTROL_CMD <- answer
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    Handler_CG_SKILL_DECL(CG_SKILL_PERK_GROUP_CONTROL_SYN);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
};


#endif // __HANDLER_CG_SKILL_H__
