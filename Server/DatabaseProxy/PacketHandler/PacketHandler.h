#pragma once
#ifndef DBPROXY_PACKET_HANDLER_H
#define DBPROXY_PACKET_HANDLER_H

#include <Singleton.h>
#include <FunctionMap.h>

//==================================================================================================
class ChildServerSession;

enum ePACKET_HANDLER
{
    PACKET_HANDLER_AGENT,
    PACKET_HANDLER_GAME,
    PACKET_HANDLER_WORLD,
    PACKET_HANDLER_MASTER,
    PACKET_HANDLER_GUILD,
    PACKET_HANDLER_PORTAL,
    PACKET_HANDLER_SOLARAUTH,
    PACKET_HANDLER_DBR,
    PACKET_HANDLER_MAX,
};

//==================================================================================================
// NOTE: f110518.2L, guild server unification
namespace nsPacket {
;
struct PacketHandlerNode;
struct PacketHandlerInfo;

}; //end of namespace
//==================================================================================================
//
class PacketHandler : public util::Singleton<PacketHandler>
{
public:
    typedef void (*fnHandler)(ChildServerSession* pSession, MSG_BASE* pMsg, WORD wSize);

    PacketHandler();
    ~PacketHandler();

    bool Register();
    bool AddHandler(ePACKET_HANDLER handler_type, BYTE category, BYTE protocol, fnHandler fnHandler);
    void ParsePacket(ePACKET_HANDLER handler_type,
        ChildServerSession* server_session, MSG_BASE* msg_base, WORD msg_size);

private:
    struct HANDLER_FUNC : public util::BASE_FUNC
    {
        fnHandler m_fnHandler;
    };
    // NOTE: f110518.2L, a batched process for a partial protocol group
    bool _RegisterPartialBlock(const TCHAR* group_name, ePACKET_HANDLER handler_type,
                               const nsPacket::PacketHandlerInfo& regist_info);
    //
    util::FunctionMap function_maps_[PACKET_HANDLER_MAX];
};

//==================================================================================================

namespace nsPacket {
;
// NOTE: f110518.2L, PacketHandlerNode
struct PacketHandlerNode
{
    union {
        uint16_t category_and_protocol_;
        struct {
            uint8_t category_;
            uint8_t protocol_;
        };
    };
    bool is_db_result_handler_;
    ePHASE_SHIFT phase_shift_;
    PacketHandler::fnHandler handler_;
};

// NOTE: f110518.2L, PacketHandlerInfo
struct PacketHandlerInfo
{
    const PacketHandlerNode* node_array_;
    int number_of_array_;
};

}; //end of namespace

#define WAVERIX_GUILD_DEBUGGING() \
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace_domination", "|"__FUNCTION__"|{}=")))

//==================================================================================================

#endif //DBPROXY_PACKET_HANDLER_H