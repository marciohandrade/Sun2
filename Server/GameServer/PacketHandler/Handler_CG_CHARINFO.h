#pragma once
#ifndef GAMESERVER_CG_CHARINFO_H
#define GAMESERVER_CG_CHARINFO_H

#include "./PacketHandler.h"



#define Handler_CG_CHARINFO_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_CHARINFO_IMPL(p) \
    void Handler_CG_CHARINFO::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)


class Handler_CG_CHARINFO
{
public:
    Handler_CG_CHARINFO();
    ~Handler_CG_CHARINFO();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_CG_CHARINFO_DECL(CG_CHARINFO_SELECT_INFO_SYN);
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    Handler_CG_CHARINFO_DECL(CG_CHARINFO_GET_ATTENDANCE_REWARD_PACKET);
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    Handler_CG_CHARINFO_DECL(CG_CHARINFO_GET_MINIGAME_BINGO_REWARD_PACKET);
#endif //_NA_008404_20150712_MINIGAME_SYSTEM
};

#endif //GAMESERVER_CG_CHARINFO_H