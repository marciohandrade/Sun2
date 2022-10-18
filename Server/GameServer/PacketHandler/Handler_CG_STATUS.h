#pragma once
#ifndef __Handler_CG_STATUS_H__
#define __Handler_CG_STATUS_H__

#include "./PacketHandler.h"



class Handler_CG_STATUS
{
public:
    Handler_CG_STATUS();
    ~Handler_CG_STATUS();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    static void OnCG_STATUS_STAT_SELECT_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_RESURRECTION_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_EMOTION_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_CONDITION_CHANGE_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_RENDER_OPTION_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_RESURRECTION_TO_SAFEZONE_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_END_DRAGON_FORMATION_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_REBIRTH_ON_VILLAGE_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_INTERCEPTION_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_FREE_INIT_STAT_UNDER_20_SYN (ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);    
    static void OnCG_STATUS_RENDER_EFFECT_ITEM_LIST_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
    static void OnCG_STATUS_DELETE_CHAR_STATE_SYN(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize);
#ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION   
    static void OnCG_STATUS_STAT_APPLY_SYN( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize );
#endif
};


#endif // __Handler_CG_STATUS_H__