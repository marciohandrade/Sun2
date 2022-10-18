#pragma once
#ifndef WORLDSERVER_HANDLER_MW_H
#define WORLDSERVER_HANDLER_MW_H

#include <PacketStruct_MW.h>

#include "./PacketHandler.h"

namespace ns_world {
;
class ServerSessionInWorld;

}; //end of namespace

namespace ns_world { namespace handlers {
;

class Handler_MW
{
public:
    static const ns_world::PacketHandlerInfo GetPacketHandlerNodes();
    //
#undef HANDLER_NODE_INFO
#define HANDLER_NODE_INFO(_category_, _protocol_, phase) \
    static void Handler_MW::On##_protocol_(\
        ns_world::ServerSessionInWorld* const server_session, \
        const MSG_BASE* const msg, const uint16_t msg_size)

    HANDLER_NODE_INFO(MW_CONNECTION, MW_SERVER_INFO_CMD, PHASE_SERVICE);
    HANDLER_NODE_INFO(MW_OPERATION, MW_NOTICE_CMD, PHASE_SERVICE);
    HANDLER_NODE_INFO(MW_OPERATION, MW_BATTLEZONEINFO_REQ, PHASE_SERVICE);
    //HANDLER_NODE_INFO(SERVERCOMMON_SERVERSHUTDOWN_REQ, PHASE_SERVICE);
    //-------------------------------------------------------------------------------------------------
    //#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    HANDLER_NODE_INFO(MW_CONNECTION, MW_CONNECTION_DISCONNECT_CMD, PHASE_SERVICE);
    HANDLER_NODE_INFO(MW_CONNECTION, MW_CONNECTION_PHASE_SHIFT_CMD, PHASE_SERVICE);
    HANDLER_NODE_INFO(MW_CONNECTION, MW_CONNECTION_BLOCK_CMD, PHASE_SERVICE);
    //
    HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, PHASE_SERVICE);
    HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, PHASE_SERVICE);
    HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD, PHASE_SERVICE);
    //#endif //
    //__NA001254_20090109_RTTG_FUNC_ADD
    HANDLER_NODE_INFO(MW_OPERATION, MW_MULTIMEMO_SEND_SYN, PHASE_SERVICE);
    //
    __DISABLE_COPY(Handler_MW);
};

}}; //end of namespace

//==================================================================================================

#endif //WORLDSERVER_HANDLER_MW_H