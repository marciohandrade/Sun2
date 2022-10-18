#pragma once
#ifndef GAMESERVER_CG_CONVERSATION_H
#define GAMESERVER_CG_CONVERSATION_H

#include "./PacketHandler.h"



class Handler_CG_CONVERSATION
{
public:
    Handler_CG_CONVERSATION();
    ~Handler_CG_CONVERSATION();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    static void OnCG_CONVERSATION_FIELDCHAT_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
};

#endif //GAMESERVER_CG_CONVERSATION_H