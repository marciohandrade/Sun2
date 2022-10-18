#pragma once
#ifndef GAMESERVER_HANDLER_GM_H
#define GAMESERVER_HANDLER_GM_H

#include "./PacketHandler.h"


class RemoteServerSession;

//-------------------------------------------------------------------------------------------------
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

#define HandlerFromMasterServer_DECL(fnName) \
    static void On##fnName(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

#define HandlerFromMasterServer_IMPL(fnName) \
    void Handler_GM::On##fnName(ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//-------------------------------------------------------------------------------------------------


class Handler_GM
{
public:
    Handler_GM();
    ~Handler_GM();

    // changes, f100819.1L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    static void OnGM_CONNECTION_GROUND_CMD(ServerSession *pMasterServer, MSG_BASE* pMsg, WORD wSize);
    static void OnGM_CONNECTION_SERVER_INFO_CMD(ServerSession *pMasterServer, MSG_BASE* pMsg, WORD wSize);
    static void OnGM_RELOAD_DATA_CMD(ServerSession *pMasterServer, MSG_BASE* pMsg, WORD wSize);
    static void OnSERVERCOMMON_SERVERSHUTDOWN_REQ(ServerSession *pMasterServer, MSG_BASE* pMsg, WORD wSize);
    static void OnGM_OPERATION_USERMONEY_REQ(ServerSession *pMasterServer, MSG_BASE* pMsg, WORD wSize);
    static void OnGM_OPERATION_FUNCRESTRICT_CMD(ServerSession *pMasterServer, MSG_BASE* pMsg, WORD wSize);
    static void OnGM_OPERATION_STATISTICS_REQ(ServerSession *pMasterServer, MSG_BASE* pMsg, WORD wSize);

    //-------------------------------------------------------------------------------------------------
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    HandlerFromMasterServer_DECL(GM_CONNECTION_DISCONNECT_CMD);
    HandlerFromMasterServer_DECL(GM_CONNECTION_PHASE_SHIFT_CMD);
    HandlerFromMasterServer_DECL(GM_CONNECTION_BLOCK_CMD);
    //
    //HandlerFromMasterServer_DECL(AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN);
    HandlerFromMasterServer_DECL(SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK);
    HandlerFromMasterServer_DECL(SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK);
    HandlerFromMasterServer_DECL(SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD);
    //HandlerFromMasterServer_DECL(AM_DATA_TRANSFER_SENDDATA_ACK);
    //HandlerFromMasterServer_DECL(AM_DATA_TRANSFER_SENDDATA_NAK);
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    //-------------------------------------------------------------------------------------------------
};

#endif //GAMESERVER_HANDLER_GM_H