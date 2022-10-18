#pragma once
#ifndef __HANDLER_CG_SYNC_H__
#define __HANDLER_CG_SYNC_H__

#include "./PacketHandler.h"




#define Handler_CG_SYNC_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_CG_SYNC_IMPL(p) \
    void Handler_CG_SYNC::On##p(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_CG_SYNC
{
public:
    Handler_CG_SYNC();
    ~Handler_CG_SYNC();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    // dwOption = [0]:non-ThrustMove, [1]:ThrustMove, [2]:JumpNak, [3]:JumpStop
    static void Send_SYNC_MOVE_NAK(Player* pPlayer, RC::eSYNC_RESULT eErrorCode, DWORD IN dwErrorLevel, BOOL bDefNak);

    Handler_CG_SYNC_DECL(AG_SYNC_PLAYER_ENTER_SYN);
    Handler_CG_SYNC_DECL(AG_SYNC_PLAYER_WARP_SYN);

    Handler_CG_SYNC_DECL(CG_SYNC_MOVE_SYN);
    Handler_CG_SYNC_DECL(CG_SYNC_TARGET_MOVE_SYN);
    Handler_CG_SYNC_DECL(CG_SYNC_KBMOVE_SYN);
    Handler_CG_SYNC_DECL(CG_SYNC_STOP_SYN);
    Handler_CG_SYNC_DECL(CG_SYNC_FAST_KBMOVE_SYN);

    Handler_CG_SYNC_DECL(CG_SYNC_JUMP_SYN);
    Handler_CG_SYNC_DECL(CG_SYNC_JUMP_STOP_SYN);
    // CHANGES: f110315.2L, _NA001955_110210_WING_COSTUME
    Handler_CG_SYNC_DECL(CG_SYNC_WINGS_CONTROL_SYN);
};

#endif // __HANDLER_CG_SYNC_H__