#pragma once
#ifndef GAMESERVER_CG_GM_H
#define GAMESERVER_CG_GM_H

#include "./PacketHandler.h"

#define Handler_CG_GM_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_GM_IMPL(p) \
    void Handler_CG_GM::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)


//#include <GMCmdManager.h>



class Handler_CG_GM
{
public:
    Handler_CG_GM();
    ~Handler_CG_GM();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_GM_DECL(CG_GM_STRING_CMD_SYN);           // 스트링 명령어 처리
};


#endif //GAMESERVER_CG_GM_H