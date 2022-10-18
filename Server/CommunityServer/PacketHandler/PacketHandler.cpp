#include "WorldServerPch.hxx"
#include "./PacketHandler.h"

#include "./Handler_AW.h"
#include "./Handler_CW.h"
#include "./Handler_DW.h"
#include "./Handler_MW.h"

ns_world::PacketHandler* ns_world::PacketHandler::instance_static_ = NULL;

ns_world::PacketHandler* ns_world::PacketHandler::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(PacketHandler** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        PacketHandler** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    PacketHandler* provider = new PacketHandler;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

ns_world::PacketHandler::PacketHandler()
{
    bool result = this->Register();
    ASSERT(result);
}

ns_world::PacketHandler::~PacketHandler()
{
}

// f100819.1L, a batched process for a partial protocol group
bool ns_world::PacketHandler::_RegisterPartialBlock(
    const TCHAR* group_name, ePACKET_HANDLER handler_type,
    const ns_world::PacketHandlerInfo& regist_info)
{
    util::FunctionMap* function_map = &function_table_[handler_type];
    const ns_world::PacketHandlerNode* it = regist_info.node_array_,
        * end = regist_info.node_array_ + regist_info.number_of_array_;
    for (; it != end; ++it)
    {
        HANDLER_FUNC* func_info = new HANDLER_FUNC;
        func_info->m_dwFunctionKey = it->category_and_protocol_;
        func_info->m_fnHandler = it->handler_;
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
        __asm { int 3 }; // assert
        return false;
    }
    return true;
}

bool ns_world::PacketHandler::AddHandler(
    ns_world::ePACKET_HANDLER handler_type,
    BYTE category, BYTE protocol, fnHandler fnHandler)
{
    HANDLER_FUNC* func_info = new HANDLER_FUNC;
    func_info->m_dwFunctionKey	= MAKEWORD(category, protocol);
    func_info->m_fnHandler		= fnHandler;
    return function_table_[handler_type].Add(func_info) != false;
}

void ns_world::PacketHandler::ParsePacket(
    ns_world::ePACKET_HANDLER handler_type,
    ServerSessionInWorld* server_session, MSG_BASE* msg, WORD msg_size)
{
    HANDLER_FUNC* func_info = (HANDLER_FUNC*)function_table_[handler_type].Find(
        MAKEWORD(msg->m_byCategory, msg->m_byProtocol));

    if (func_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "|["__FUNCTION__"]|Msg=Invalid packet! Category[%u] Protocol[%u]",
               msg->m_byCategory, msg->m_byProtocol);
        ASSERT(!"Invalid packet from Server!");
        return;
    }

    func_info->m_fnHandler(server_session, msg, msg_size);
}

bool ns_world::PacketHandler::Register()
{
#define BATCH_REGISTERING(group_name, handler_type, nodes) \
    if (!_RegisterPartialBlock((group_name), (handler_type), (nodes))) { \
        return false; \
    }

    BATCH_REGISTERING("C->A->W", ns_world::PACKET_HANDLER_AGENT,
                      ns_world::handlers::Handler_CW::GetPacketHandlerNodes());
    BATCH_REGISTERING("D->W", ns_world::PACKET_HANDLER_DBPROXY,
                      ns_world::handlers::Handler_DW::GetPacketHandlerNodes());
    BATCH_REGISTERING("A->W", ns_world::PACKET_HANDLER_AGENT,
                      ns_world::handlers::Handler_AW::GetPacketHandlerNodes());
    BATCH_REGISTERING("M->W", ns_world::PACKET_HANDLER_MASTER,
                      ns_world::handlers::Handler_MW::GetPacketHandlerNodes());
    //
    return true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================






#if SUN_CODE_BACKUP
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
// 파티장이 변경되면 호출된다.
HANDLER_NODE_INFO(AW_PARTY_CHANGE_MASTER_SYN, PHASE_SERVICE),(AgentServerSession* agent_server, 
                                                             MSG_BASE* receive_msg, 
                                                             WORD receive_msg_size);

// 파티 모집에서 파티 참가 신청에 대한 응답이 오면 호출된다.
HANDLER_NODE_INFO(AW_PARTY_FINDING_JOIN_RESULT_ACK, PHASE_SERVICE),(AgentServerSession* agent_server, 
                                                                   MSG_BASE* receive_msg, 
                                                                   WORD receive_msg_size);
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
#endif //SUN_CODE_BACKUP
