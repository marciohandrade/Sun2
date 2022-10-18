#include "stdafx.h"
#include "PacketHandler.h"

//==================================================================================================

#include <Protocol_ServerCommon.h>
#include <PacketStruct_DG.h>
#include <PacketStruct_MX.h>
#include <PacketStruct_AD.h>
#include <PacketStruct_DM.h>
#include <PacketStruct_DW.h>
#include <PacketStruct_DZ.h>
#include <PacketStruct_DP.h>
#include <PacketStruct_SX.h>

//==================================================================================================

#include "Sessions/ChildServerSession.h"

#include "Handler_Common.h"
#include "Handler_FromMasterServer.h"
#include "Handler_FromAgentServer.h"
#include "Handler_FromGuildServer.h"
#include "Handler_FromWorldServer.h"
#include "Handler_FromPortalServer.h"
#include "Handler_FromGameServer.h"
#include "Handler_FromGameServer.Ranking.h"
#include "Handler_FromSolarAuthServer.h"
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
#include "Handler_Guild.DbpToGuild.h"
#include "Handler_Guild.GameToGuild.h"
#include "Handler_Guild.GuildToDbp.h"
#include "Handler_Guild.WorldToGuild.h"
#endif
//==================================================================================================

PacketHandler::PacketHandler()
{
    bool must_be_register_all = Register();
    ASSERT(must_be_register_all);
}

PacketHandler::~PacketHandler()
{
}

// NOTE: f110518.2L, a batched process for a partial protocol group
bool PacketHandler::_RegisterPartialBlock(
    const TCHAR* group_name, ePACKET_HANDLER handler_type,
    const nsPacket::PacketHandlerInfo& regist_info)
{
    util::FunctionMap* message_map = &function_maps_[handler_type];
    util::FunctionMap* db_result_map = &function_maps_[PACKET_HANDLER_DBR];
    const nsPacket::PacketHandlerNode* it = regist_info.node_array_,
                                     * end = regist_info.node_array_ + regist_info.number_of_array_;
    for (; it != end; ++it)
    {
        HANDLER_FUNC* func_info = new HANDLER_FUNC;
        func_info->m_dwFunctionKey = it->category_and_protocol_;
        func_info->m_fnHandler = it->handler_;
        util::FunctionMap* function_map = (it->is_db_result_handler_ == false)
                                        ?   message_map : db_result_map;
        bool is_added = (function_map->Add(func_info) != false);
        if (is_added == false) {
            break;
        }
    }

    if (it != end)
    {   //occurred fail
        const int order = static_cast<int>(it - regist_info.node_array_);
        static const char* order_postfix[4] = { _T("st"), _T("nd"), _T("rd"), _T("th") };
        const TCHAR* postfix = order < 3 ? order_postfix[order] : order_postfix[3];
                                
        SUNLOG(eCRITICAL_LOG,
               _T("|Msg=conflict protocol (%3d:%3d)|Pos=%3d%s in '%s'|Action=stop registering|"),
               it->category_, it->protocol_,
               order + 1, postfix,
               group_name);
        __debugbreak(); // assert
        return false;
    }
    return true;
}

bool PacketHandler::Register()
{
//==================================================================================================
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
    // NOTE: f110518.2L, the batch unit process has not message handlers based on session
    return true;
#endif
//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
#define BATCH_REGISTERING(group_name, handler_type, nodes) \
    if (!_RegisterPartialBlock((group_name), (handler_type), (nodes))) { \
        return false; \
    }
//==================================================================================================
//==================================================================================================
//==================================================================================================
#if (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_ACCOUNT)
    //
    BATCH_REGISTERING("Master", PACKET_HANDLER_MASTER,
                      Handler_FromMasterServer::GetPacketHandlerNodes());
    return true;
#endif
//==================================================================================================
//==================================================================================================
//==================================================================================================
#if (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT)
    //
    BATCH_REGISTERING("MasterServer", PACKET_HANDLER_MASTER,
                      Handler_FromMasterServer::GetPacketHandlerNodes()); //O
    //
    BATCH_REGISTERING("AgentServer", PACKET_HANDLER_AGENT,
                      Handler_FromAgentServer::GetPacketHandlerNodes()); //O
    BATCH_REGISTERING("GameServer", PACKET_HANDLER_GAME,
                      Handler_FromGameServer::GetPacketHandlerNodes());
    BATCH_REGISTERING("WorldServer", PACKET_HANDLER_WORLD,
                      Handler_FromWorldServer::GetPacketHandlerNodes());
  #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
    BATCH_REGISTERING("GameServer", PACKET_HANDLER_GAME,
                      Handler_Guild_GameToGuild::GetPacketHandlerNodes());
    BATCH_REGISTERING("GameServer", PACKET_HANDLER_GAME,
                      Handler_Guild_GuildToDbp::GetPacketHandlerNodes());
    BATCH_REGISTERING("WorldServer", PACKET_HANDLER_WORLD,
                      Handler_Guild_WorldToGuild::GetPacketHandlerNodes());
  #else
    BATCH_REGISTERING("GuildServer", PACKET_HANDLER_GUILD,
                      Handler_FromGuildServer::GetPacketHandlerNodes());
  #endif
  #ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
    BATCH_REGISTERING("GameServer", PACKET_HANDLER_GAME,
                      Handler_GP::GetPacketHandlerNodes());
    BATCH_REGISTERING("GameServer", PACKET_HANDLER_GAME,
                      Handler_P2D::GetPacketHandlerNodes());
  #else
    BATCH_REGISTERING("PortalServer", PACKET_HANDLER_PORTAL,
                      Handler_FromPortalServer::GetPacketHandlerNodes());
  #endif
    BATCH_REGISTERING("SolarAuth", PACKET_HANDLER_SOLARAUTH,
                      Handler_FromSolarAuthServer::GetPacketHandlerNodes()); //O
#endif
//==================================================================================================
//==================================================================================================
//==================================================================================================
    return true;
}

bool PacketHandler::AddHandler(ePACKET_HANDLER handler_type,
    BYTE category, BYTE protocol, fnHandler fnHandler)
{
    HANDLER_FUNC* handler_info = new HANDLER_FUNC;
    handler_info->m_dwFunctionKey = MAKEWORD(category, protocol);
    handler_info->m_fnHandler = fnHandler;
    return (function_maps_[handler_type].Add(handler_info) != false);
}

void PacketHandler::ParsePacket(ePACKET_HANDLER handler_type,
    ChildServerSession* server_session, MSG_BASE* msg_base, WORD msg_size)
{
    //if(msg_base->m_byCategory == 0xff){}
    HANDLER_FUNC* handler_info = (HANDLER_FUNC*)function_maps_[handler_type].Find(
        MAKEWORD(msg_base->m_byCategory, msg_base->m_byProtocol));
    if (handler_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
            _T("|["__FUNCTION__"]|[%02u][%-16s]|Msg=invalid packet!|C:P={%03u:%03u}|"),
            handler_type, server_session->GetConnectIP().c_str(),
            msg_base->m_byCategory, msg_base->m_byProtocol);
        return;
    }

    handler_info->m_fnHandler(server_session, msg_base, msg_size);
}
