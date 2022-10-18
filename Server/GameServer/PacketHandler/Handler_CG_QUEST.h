#pragma once
#ifndef GAMESERVER_CG_QUEST_H
#define GAMESERVER_CG_QUEST_H

#include "./PacketHandler.h"




#define Handler_CG_QUEST_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_QUEST_IMPL(p) \
    void Handler_CG_QUEST::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)


class Handler_CG_QUEST
{
public:
    Handler_CG_QUEST(){}
    ~Handler_CG_QUEST(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_QUEST_DECL(CG_QUEST_ACCEPT_SYN);
    Handler_CG_QUEST_DECL(CG_QUEST_COMPLETE_SYN);
    Handler_CG_QUEST_DECL(CG_QUEST_REWARD_SYN);
    Handler_CG_QUEST_DECL(CG_QUEST_FAILED_SYN);
    Handler_CG_QUEST_DECL(CG_QUEST_ABANDON_SYN);
    Handler_CG_QUEST_DECL(CG_QUEST_EVENT_AREA_SYN);
    Handler_CG_QUEST_DECL(CG_QUEST_INCOMPLETE_SYN);
};

#endif //GAMESERVER_CG_QUEST_H