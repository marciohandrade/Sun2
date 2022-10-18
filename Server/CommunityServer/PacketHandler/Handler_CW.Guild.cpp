#include "WorldServerPch.hxx"
#include "./Handler_CW.h"

#include <PacketStruct_WZ.h>
#include <ProhibitionWordParser.h>

#include "./PacketHandler.h"
#include "WorldServer.h"

//#include "Sessions/ServerSessionInWorld.h"
//#include "Channel/WorldChannelManager.h"
//#include "Channel/WorldChannel.h"
#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
//#include "Services/ChattingService.h"
//#include "Services/Lotto/Lotto.h"
#include "Services/Group/GroupManager.h"
#include "Services/Guild/WorldGuildManager.h"

//==================================================================================================

namespace ns_world { namespace handlers {
;
BOOST_STATIC_ASSERT(RC::RC_GUILD_RESULT_COUNTS <= UCHAR_MAX);
}}; //end of namespace

//==================================================================================================
//==================================================================================================
//==================================================================================================

#undef HANDLER_NODE_INFO
#define HANDLER_NODE_INFO(_category_, _protocol_, _phase_) \
    void ns_world::handlers::Handler_CW::On##_protocol_(\
        ns_world::ServerSessionInWorld* const server_session, \
        const MSG_BASE* const msg_base, const uint16_t msg_size)

#define WAVERIX_WORLD_HANDLER_DEBUGGING() \
    WAVERIX_WRAPPER((WAVERIX_LC_CALL4("waverix_log_trace_cw", \
        __FUNCTION__, server_session, msg_base, msg_size)))

#if SUN_CODE_SAMPLE // to support visual assistx intelli-sense
ns_world::ServerSessionInWorld* server_session;
MSG_BASE* msg_base; uint16_t msg_size;
#endif

//==================================================================================================
//==================================================================================================
//==================================================================================================


HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_CHAT_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    // WARNING: f110826.1L, the client message has not null terminated string
    const MSG_CW_GUILD_CHAT_SYN* recv_msg = static_cast<const MSG_CW_GUILD_CHAT_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

    util::StrArrayForGenericChattingMsg chatting_msg;
    BOOST_STATIC_ASSERT(_countof(recv_msg->m_ptszCharMsg) == _countof(chatting_msg));
    const size_t chat_msg_length = \
        min(_countof(recv_msg->m_ptszCharMsg) - 1, recv_msg->m_byMsgLen);
    if (chat_msg_length == 0 ||
        chat_msg_length != recv_msg->m_byMsgLen ||
        recv_msg->GetSize() != msg_size)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=received invalid message, ")
               _T("expected=%d, received=%d, msg_len=%d"),
               recv_msg->GetSize(), msg_size, recv_msg->m_byMsgLen);
        return;
    };
    // rebuild chatting message, chat_msg_length does not contain null character
    strncpy(chatting_msg, recv_msg->m_ptszCharMsg, chat_msg_length);
    chatting_msg[chat_msg_length] = '\0';
    chatting_msg[_countof(chatting_msg) - 1] = '\0';
    //
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    if (char_info.guild_guid == 0)    // 길드 소속이 아니라고 나온다면?
    {
        // 길드 정보를 요청해야 한다
        MSG_CW_GUILD_CHAT_NAK msg_nak;
        msg_nak.byResult = msg_nak.ERR_WAIT_INFO; // 잠시 후에 다시 시도하십시오
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eCRITICAL_LOG, _T("[%s] Guild Chat Error - No Guild %u"),
               char_info.char_name, char_info.guild_guid);

        return;
        // 길드 멤버 복구 코드는 전부 접속 부분으로 옮겨갔다
/*
        // 길드 멤버 복구 요청
        MSG_WZ_GUILD_SELECT_SYN ToGuildMsg;
        ToGuildMsg.m_dwKey = user->GetGUID();
        ToGuildMsg.m_GuildGuid = recv_msg->m_GuildGuid;
        ToGuildMsg.m_CharGuid = user->GetCharGuid();
        ServerSession *pGuildServer = ServerSessionManager::Instance()->GetGuildServer();

        if (!pGuildServer)
        {
            SUNLOG(eCRITICAL_LOG, "GUILD_MEMBER_SYN 길드서버 없어 복구 요청 불가!");
            return;
        }
        pGuildServer->SendPacket(&ToGuildMsg, sizeof(MSG_WZ_GUILD_SELECT_SYN));

        SUNLOG(eFULL_LOG, "GUILD_CHAT_SYN 길드정보[%u] 없어서 요청[%s]", recv_msg->m_GuildGuid, user->GetCharName().c_str());
*/
    //----------------------------------------------------------------------------------------------
    }; // end 'if'
    //----------------------------------------------------------------------------------------------
    assert(char_info.guild_guid != 0);
    ns_world::WorldGuild* const guild = \
        ns_world::WorldGuildManager::Instance()->FindGuild(char_info.guild_guid);

    if (guild == NULL)
    {
        MSG_CW_GUILD_CHAT_NAK msg_nak;
        msg_nak.byResult = msg_nak.ERR_NOT_GUILD;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, _T("[%s] Guild Chat Error - No Guild %u"),
               char_info.char_name, char_info.guild_guid);
        return;
    }

    if (ProhibitionWordParser::Instance()->CheckFilter(chatting_msg) == false)
    {
        MSG_CW_CHAT_NORMAL_NAK msg_nak;
        msg_nak.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, _T("Guild Chat Error - USE BAN WORD[%s][CharGuid:%u]"),
               char_info.char_name, char_info.guild_guid);
        return;
    }

    // 길드원 전체에게 메시지 전송 - 이거 UserManager에서 처리해야 되지 않을라나 싶네?
    MSG_CW_GUILD_CHAT_BRD msg_brd;
    _tcsncpy(msg_brd.ptszCharName, char_info.char_name, _countof(msg_brd.ptszCharName));
    msg_brd.ptszCharName[_countof(msg_brd.ptszCharName) - 1] = _T('\0');
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(msg_brd.ptszChatMsg));
    msg_brd.byMsgLen = static_cast<uint8_t>(::strlen(chatting_msg));
    strncpy(msg_brd.ptszChatMsg, chatting_msg, _countof(msg_brd.ptszChatMsg));
    //
    guild->SendAllMember(&msg_brd, msg_brd.GetSize());

    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    GAMELOG->WriteGuildChat(user, server_session->GetChannelID(),
        guild, chatting_msg, msg_brd._MAX_CHATMSG_SIZE);

    SUNLOG(eFULL_LOG, _T("[%s] Send Guild Chat[%u]"),
           char_info.char_name, char_info.guild_guid);
}


HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_CHANGE_NOTICE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GUILD_CHANGE_NOTICE_SYN* recv_msg = \
        static_cast<const MSG_CW_GUILD_CHANGE_NOTICE_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    // NOTE: f110826.1L, the guild server same with the dbproxy server since version 1003
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        return;
    };

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    //
    size_t guild_notice_length_with_null = 0;
    // NOTE: f110826.1L, fixed string length problem from the custom version
    RC::eGUILD_RESULT notic_change_result = guild_manager->CanChangeNotice(\
        char_info.guild_guid, char_info.char_guid,
        recv_msg->m_tszGuildNotice, guild_notice_length_with_null);

    if (notic_change_result != RC::RC_GUILD_SUCCESS)
    {
        MSG_CW_GUILD_CHANGE_NOTICE_NAK  msg_nak;
        msg_nak.m_byErrorCode = static_cast<uint8_t>(notic_change_result);
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };

    // 권한 체크로 인해 길드서버를 통해야 함
    MSG_WZ_GUILD_CHANGE_NOTICE_SYN  msg_syn;
    msg_syn.m_GuildGuid = char_info.guild_guid;
    msg_syn.m_OperCharGuid = char_info.char_guid;
    msg_syn.m_wNoticeLen = static_cast<uint16_t>(guild_notice_length_with_null);
    _tcsncpy(msg_syn.m_szGuildNotice, recv_msg->m_tszGuildNotice,
             _countof(msg_syn.m_szGuildNotice));
    msg_syn.m_szGuildNotice[_countof(msg_syn.m_szGuildNotice) - 1] = _T('\0');
    //
    dbproxy->SendPacket(&msg_syn, msg_syn.GetSize());
}

HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_SELECT_NOTICE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GUILD_SELECT_NOTICE_SYN* recv_msg = \
        static_cast<const MSG_CW_GUILD_SELECT_NOTICE_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    // NOTE: f110826.1L, the guild server same with the dbproxy server since version 1003
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        return;
    };
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    if (FlowControl::FCAssert(char_info.char_guid != 0) == false) {
        return;
    };
    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    // CHANGES: f110923.1L, fixed the invalid usage case using the garbage guild guild.
    // WARNING: data fields that are extracted from the received message are unset.
    ns_world::WorldGuild* const guild = guild_manager->FindGuild(char_info.guild_guid);
    if (guild != NULL)
    {
        MSG_CW_GUILD_SELECT_NOTICE_ACK msg_ack;
        const util::StrArrayForGuildNotice& notice = guild->GetGuildNotice();
        msg_ack.m_wNoticeLen = static_cast<uint16_t>(::strlen(notice));
        BOOST_STATIC_ASSERT(_countof(msg_ack.m_ptszNotice) == _countof(notice));
        strncpy(msg_ack.m_ptszNotice, notice, _countof(msg_ack.m_ptszNotice));
        msg_ack.m_ptszNotice[_countof(msg_ack.m_ptszNotice) - 1] = _T('\0');
        user->SendPacket(&msg_ack, sizeof(msg_ack));
        return;
    };
    
    MSG_DW_GUILD_SELECT_NOTICE_SYN msg_dw_syn;
    msg_dw_syn.m_GuildGuid = char_info.guild_guid;
    msg_dw_syn.m_OperCharGuid = char_info.char_guid;
    msg_dw_syn.m_NoticeType = 0;
    dbproxy->SendPacket(&msg_dw_syn, sizeof(msg_dw_syn));

    // 요청 실패, ... waverix, thinking...
    MSG_CW_GUILD_SELECT_NOTICE_NAK msg_nak;
    msg_nak.m_dwErrorCode = RC::RC_GUILD_INVALID_STATE;
    user->SendPacket(&msg_nak, sizeof(msg_nak));
}

HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_WAREHOUSE_LOGLIST_REQ, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GUILD_WAREHOUSE_LOGLIST_REQ* recv_msg = \
        static_cast<const MSG_CW_GUILD_WAREHOUSE_LOGLIST_REQ*>(msg_base);
    __TOUCHED(recv_msg);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    // NOTE: f110826.1L, the guild server same with the dbproxy server since version 1003
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_MEMO_FAILED;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        //
        return;
    };

    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    MSG_DW_GUILD_WAREHOUSE_LOGLIST_REQ msg_syn;
    msg_syn.m_GuildGuid = char_info.guild_guid;
    msg_syn.m_ReqCharGuid = char_info.char_guid;
    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
}

//유저가 지역 점령전 쪽지를 확인하였다. DB에 해당 내용이 있는지 확인해본다.
HANDLER_NODE_INFO(CW_GUILD, CW_DOMINATION_RETURN_HEIM_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_DOMINATION_RETURN_HEIM_CMD* recv_msg = \
        static_cast<const MSG_CW_DOMINATION_RETURN_HEIM_CMD*>(msg_base);
    //
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]No GameDBProxy");
        return;
    };

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    eSYSTEM_MEMO_TYPE cur_system_memo_type;
    MAPCODE map_code;
    if (guild_manager->GetValueInMemo(\
            recv_msg->m_ptszMemo, cur_system_memo_type, map_code) == false)
    {
        return;
    };
    if (bool skipped = !((cur_system_memo_type == SYSMEMO_DOMINATION_RETURN_MONEY) ||
                         (cur_system_memo_type == SYSMEMO_DOMINATION_REWARD_MONEY)))
    {
        return;
    };

    MSG_DW_DOMINATION_FINDMONEY_SYN msg_syn;
    msg_syn.m_MapCode = map_code;
    msg_syn.m_RecvCharGuid = user->GetCharGuid();
    msg_syn.m_CostType = cur_system_memo_type;

    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
}
