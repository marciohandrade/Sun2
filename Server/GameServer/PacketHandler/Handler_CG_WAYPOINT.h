#pragma once
#ifndef GAMESERVER_CG_WAYPOINT_H
#define GAMESERVER_CG_WAYPOINT_H

#include "./PacketHandler.h"




#define Handler_CG_WAYPOINT_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_WAYPOINT_IMPL(p) \
    void Handler_CG_WAYPOINT::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_WAYPOINT
{
public:
    Handler_CG_WAYPOINT(){}
    ~Handler_CG_WAYPOINT(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_WAYPOINT_DECL(CG_WAYPOINT_INFO_LOAD_SYN);
    Handler_CG_WAYPOINT_DECL(CG_WAYPOINT_INFO_SAVE_SYN);

    Handler_CG_WAYPOINT_DECL(CG_CHAR_WAYPOINT_INFO_LOAD_SYN);
    Handler_CG_WAYPOINT_DECL(CG_CHAR_WAYPOINT_INFO_SAVE_SYN);
    Handler_CG_WAYPOINT_DECL(CG_CHAR_WAYPOINT_INFO_DEL_SYN);
    Handler_CG_WAYPOINT_DECL(CG_CHAR_WAYPOINT_INFO_EXE_CMD);
};

#endif //GAMESERVER_CG_WAYPOINT_H