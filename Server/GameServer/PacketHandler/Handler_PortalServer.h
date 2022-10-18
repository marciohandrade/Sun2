#pragma once
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#ifndef GAMESERVER_PORTALSERVER_H
#define GAMESERVER_PORTALSERVER_H

#include "./PacketHandler.h"

// NOTE: f110511.1L, after _NA002635_GP_DP_PROTOCOL_UINIFICATION application,
// PortalServer does not used. the ipc object related to the ranking system is replaced by
// DBProxyServerSession so that the below instance of class is used in DBProxyServerSession
// despite the name of Handler_PortalServer.

#define Handler_RankingServer_DECL(p) \
    static void On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)
#define Handler_RankingServer_IMPL(p) \
    void Handler_RankingServer::On##p(ServerSession* pServerSession, MSG_BASE* pRecvMsg, WORD wSize)



class Handler_RankingServer
{
public:
    Handler_RankingServer(){}
    ~Handler_RankingServer(){}

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_RankingServer_DECL(GP_RANKING_SESSION_CONTROL_CMD);

    Handler_RankingServer_DECL(GP_RANKING_INFO_REQ_ACK);
    Handler_RankingServer_DECL(GP_RANKING_INFO_REQ_NAK);
    Handler_RankingServer_DECL(GP_RANKING_INFO_REQ_END_NTF);

    Handler_RankingServer_DECL(GP_RANKING_MISSION_SUCCESS_ACK);
    Handler_RankingServer_DECL(GP_RANKING_MISSION_SUCCESS_BRD);
    Handler_RankingServer_DECL(GP_RANKING_MISSION_FAILED_NAK);

    Handler_RankingServer_DECL(GP_RANKING_MISSION_REWARD_ACK);
    Handler_RankingServer_DECL(GP_RANKING_MISSION_REWARD_BRD);
    Handler_RankingServer_DECL(GP_RANKING_MISSION_REWARD_NAK);

    Handler_RankingServer_DECL(GP_RANKING_SETTLED_INFO_CMD);
    Handler_RankingServer_DECL(GP_RANKING_SETTLED_INFO_END_NTF);
    Handler_RankingServer_DECL(GP_RANKING_CHAGED_STATUS_NTF);

    Handler_RankingServer_DECL(GP_MISSION_RANKING_SETTLE_DATE_ACK);
};

#endif //GAMESERVER_PORTALSERVER_H
#endif // _NA_008012_20150130_RANKING_SYSTEM