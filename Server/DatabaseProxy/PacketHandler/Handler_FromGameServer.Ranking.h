#pragma once
#ifndef DBPROXY_HANDLER_FROM_GAMESERVER_RANKING_H
#define DBPROXY_HANDLER_FROM_GAMESERVER_RANKING_H

#include "./PacketHandler.h"

#ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
class ChildServerSession;

//==================================================================================================

#define Handler_P2D_DECL(p) static void On##p(ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_P2D_IMPL(p) void Handler_P2D::On##p(ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_P2D
{
public:
    Handler_P2D() {}
    ~Handler_P2D() {}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_P2D_DECL(DP_RANKING_INFO_REQ_SYN);
    Handler_P2D_DECL(DP_RANKING_INFO_REQ_DBR);
    Handler_P2D_DECL(DP_RANKING_MISSION_CLEAR_SYN);
    Handler_P2D_DECL(DP_RANKING_MISSION_CLEAR_DBR);
    Handler_P2D_DECL(DP_RANKING_MISSION_REWARD_SYN);
    Handler_P2D_DECL(DP_RANKING_MISSION_REWARD_DBR);
    Handler_P2D_DECL(DP_RANKING_REGISTER_MISSION_POINT_SYN);
    Handler_P2D_DECL(DP_RANKING_REGISTER_MISSION_POINT_DBR);
};

#define Handler_D2P_DECL(p) static void On##p(ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_D2P_IMPL(p) void Handler_D2P::On##p(ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_D2P
{
public:
    Handler_D2P() {}
    ~Handler_D2P() {}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_D2P_DECL(DP_RANKING_INFO_REQ_ACK);
    Handler_D2P_DECL(DP_RANKING_INFO_REQ_END_NTF);
    Handler_D2P_DECL(DP_RANKING_INFO_REQ_NAK);
    Handler_D2P_DECL(DP_RANKING_MISSION_CLEAR_ACK);
    Handler_D2P_DECL(DP_RANKING_MISSION_CLEAR_NAK);
    Handler_D2P_DECL(DP_RANKING_MISSION_REWARD_ACK);
    Handler_D2P_DECL(DP_RANKING_MISSION_REWARD_NAK);
    Handler_D2P_DECL(DP_RANKING_REGISTER_MISSION_POINT_ACK);
    Handler_D2P_DECL(DP_RANKING_REGISTER_MISSION_POINT_NAK);
};

#define Handler_GP_DECL(p) static void On##p(ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)
#define Handler_GP_IMPL(p) void Handler_GP::On##p(ChildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

class Handler_GP
{
public:
    Handler_GP() {}
    ~Handler_GP() {}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_GP_DECL(GP_RANKING_INFO_REQ_SYN);
    Handler_GP_DECL(GP_RANKING_MISSION_CLEAR_SYN);
    Handler_GP_DECL(GP_RANKING_MISSION_REWARD_SYN);

    Handler_GP_DECL(GP_RANKING_SETTLE_CMD);

    Handler_GP_DECL(GP_RANKING_REGISTER_MISSION_POINT_SYN);

    Handler_GP_DECL(GP_MISSION_RANKING_SETTLE_DATE_SYN);
};

#endif //_NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
#endif //DBPROXY_HANDLER_FROM_GAMESERVER_RANKING_H