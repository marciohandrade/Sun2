#include "WorldServerPch.hxx"
#include "./Handler_CW.h"

#include "./PacketHandler.h"
#include "WorldServer.h"

//#include "Sessions/ServerSessionInWorld.h"
#include "Channel/WorldChannelManager.h"
//#include "Channel/WorldChannel.h"
#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
//#include "Services/ChattingService.h"
#include "Services/Lotto/Lotto.h"
//#include "Services/Group/GroupManager.h"
//#include "Services/Guild/WorldGuildManager.h"
#include "Functions/GMCmdManager.h"

//==================================================================================================

namespace ns_world { namespace handlers {
;

}}; //end of namespace

const ns_world::PacketHandlerInfo ns_world::handlers::Handler_CW::GetPacketHandlerNodes()
{
#undef HANDLER_NODE_INFO
#undef HANDLER_NODE_INFO_EOR
#define HANDLER_NODE_INFO(_category_, _protocol_, _phase_) \
    { MAKEWORD(_category_, _protocol_), _phase_, &ns_world::handlers::Handler_CW::On##_protocol_ }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const ns_world::PacketHandlerNode handlers_static_[] =
    {
        //------------------------------------------------------------------------------------------
        // Handler_CW.cpp
        //------------------------------------------------------------------------------------------
        // CHANGES: f110823.1L, removed. do not use anymore
        //HANDLER_NODE_INFO(CW_CONNECTION, CW_HEARTBEAT, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_GM, CW_GM_NOTICE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_GM, CW_GM_STRING_CMD_SYN, PHASE_SERVICE),
        // CHANGES: f110907.2L, removed, does not use anymore
        //HANDLER_NODE_INFO(CW_VIEWPORT, CW_VIEWPORT_CHARSTATE, PHASE_SERVICE),
        // CHANGES: f110823.1L, removed. does not use anymore
        //HANDLER_NODE_INFO(CW_ARMORKIT, CW_ARMORKIT_FINDHACK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_COMMUNITY, CW_COMM_USERID_REQ, PHASE_SERVICE),
        //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
        HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_INFO_SYN, PHASE_SERVICE),         // (, 내) 하임 로또 정보 요청
        HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_ENTRY_SYN, PHASE_SERVICE),        // 하임 로또 응모 요청
        HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_NOTICE_LIST_SYN, PHASE_SERVICE),  // 당첨 안내 쪽지 목록 요청
        HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_NOTICE_CONFIRM, PHASE_SERVICE),   // 당첨 안내 쪽지 읽음 확인
        //}
        //------------------------------------------------------------------------------------------
        // Handler_CW.Chatting.cpp
        //------------------------------------------------------------------------------------------
        HANDLER_NODE_INFO(CW_CHAT, CW_WHISPER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_CHAT, CW_NOTICE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_CHAT, CW_WHISPER_SET_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_NORMAL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_SHOUT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_PARTY_SYN, PHASE_SERVICE),
        // CHANGES: f110823.1L, removed. do not use anymore
        //HANDLER_NODE_INFO(CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_ACK, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_NAK, PHASE_SERVICE),
        // CHANGES: f110907.2L, the voice chatting contents removed. do not use anymore
        //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_REQUEST_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_ACCEPT_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_REJECT_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_DISCONNECT_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_MIC_STATUS_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_PARTY_MIC_STATUS_REQ, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_PLAYER_MIC_STATUS_REQ, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_JOIN_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_SEND_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_INVITE_SYN, PHASE_SERVICE),
        //_NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL
        HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_INVITE_NAK, PHASE_SERVICE), // 초대 대상자가 채널 초대를 거부하거나 차단했을 때 호출된다.
        HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_OUT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_INFO_REQ, PHASE_SERVICE),
        //------------------------------------------------------------------------------------------
        // Handler_CW.Friend.cpp
        //------------------------------------------------------------------------------------------
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_NEWLIST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_ADD_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_OK_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_NO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_DEL_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_BLOCK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_BLOCK_FREE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_CHAT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_CHATBLOCK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_MEMO_UPDATE_SYN, PHASE_SERVICE),   //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_STATUS_REQ, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_STATUS_JOIN_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_REVENGE, CW_REVENGE_DEL_SYN, PHASE_SERVICE),
        //------------------------------------------------------------------------------------------
        // Handler_CW.Memo.cpp
        //------------------------------------------------------------------------------------------
        HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_SEND_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_LIST_REQ, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_VIEW_REQ, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_DELETE_SYN, PHASE_SERVICE),
        //------------------------------------------------------------------------------------------
        // Handler_CW.Guild.cpp
        //------------------------------------------------------------------------------------------
        HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_CHAT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_WAREHOUSE_LOGLIST_REQ, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_SELECT_NOTICE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_CHANGE_NOTICE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CW_GUILD, CW_DOMINATION_RETURN_HEIM_CMD, PHASE_SERVICE),
        //------------------------------------------------------------------------------------------
        // Handler_CW.Group.cpp
        //------------------------------------------------------------------------------------------
        // implemented by __KR_001355_20090608_GROUPINGSYSTEM_CW = {
        HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADD_SYN, PHASE_SERVICE),         //그룹 멤버 추가 요청
        HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_GROUPLIST_REQ, PHASE_SERVICE),   //그룹 리스트 요청
        HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADD_RESPONSE_ACK, PHASE_SERVICE),//그룹 멤버 추가 요청 성공 응답
        HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADD_RESPONSE_NAK, PHASE_SERVICE),//그룹 멤버 추가 요청 실패 응답
        HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_STATUS_REQ, PHASE_SERVICE),     //그룹 멤버 상세 정보 요청
        HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_DEL_SYN, PHASE_SERVICE),        //그룹 멤버 삭제 요청
        HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADDBLOCK_SYN, PHASE_SERVICE),   //그룹 맴버 추가 거부 토글 요청
        HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_RESTOR_CMD, PHASE_SERVICE),     //그룹 정보 재요청
        // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_CW'
        //------------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------------
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    ns_world::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

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

//added by __NA001094_080928_TASK_SURVIVAL_SOLOING_QUEST_ETC__

//==================================================================================================

ns_world::WorldUser* ns_world::handlers::Handler_CW::last_handled_user = NULL;

// NOTE: f110823.1L, if the server_session field is null, it means process using the session of user
bool ns_world::handlers::Handler_CW::CheckEnabledChattingState(
    ns_world::WorldUser* user, ns_world::ServerSessionInWorld* server_session)
{
    assert(user != NULL);
    ns_world::community::WorldChannel* channel = NULL;
    if (server_session != NULL)
    {
        channel = server_session->GetChannel();
    }
    else
    {
        channel = user->GetUserAccountInfo().channel;
    };
    //
#if SUN_CODE_BACKUP
    // will be called in user->EnabledChatting()
    if (channel == NULL) {
        return false;
    };
    if (channel->EnabledChat() == false) {
        return false;
    };
#endif
    if (bool disabled = (user->EnabledChatting() == false)) {
        user->NotifyChattingDisabledState(RC::RC_CHAT_BELONG_ZONE_BLOCK);
    };
    return true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

// GM 공지
HANDLER_NODE_INFO(CW_GM, CW_GM_NOTICE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GM_NOTICE_SYN* recv_msg = static_cast<const MSG_CW_GM_NOTICE_SYN*>(msg_base);
    //
    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    // #1. check buffer boundary with string length
    // specification: fixed size
    if (!FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) ||
        !FlowControl::FCAssert(recv_msg->dwNoticeLength < MAX_NOTICE_LEN))
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=unexpected message format, requester=%d|"),
               user_info.user_guid);
        MSG_CW_GM_NOTICE_NAK msg_nak;
        msg_nak.dwResultCode = msg_nak.ERR_NOTICE_DEFAULT;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };
    if (recv_msg->dwNoticeLength == 0) {
        // skip
        return;
    };

    // #2. check authorization
    if (user_info.gm_grade > eGM_GRADE_BASE &&
        user_info.gm_grade < eGM_GRADE_MAX)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
               _T("Msg=abused action! unauthorized user request the GM facility, requester=%d|"),
               user_info.user_guid);
        return;
    };

    //----------------------------------------------------------------------------------------------
    MSG_CW_NOTICE_BRD msg_notice;
    msg_notice.m_byRed = 0xFF;
    msg_notice.m_byGreen = 0xFF;
    msg_notice.m_byBlue = 0xFF;
    // (WARNING) buffer overrun 위험성 존재하고 있다. 우선 위험 방지 처리 수행한다.
    // (NOTE) MAX_NOTICE_LEN = 1024, _MAX_CHATMSG_SIZE = 255
    const size_t MessageLength = min(_countof(msg_notice.szMsg), _countof(recv_msg->szNotice));
    BOOST_STATIC_ASSERT(MessageLength < USHRT_MAX);
    strncpy(msg_notice.szMsg, recv_msg->szNotice, MessageLength);
    msg_notice.szMsg[MessageLength - 1] = '\0';
    // CHANGES: f101210.2L, consider length with null-terminated position
    if (size_t str_len = strlen(msg_notice.szMsg) + 1) {
        // NOTE: always true, because wLen need consider with null-terminated position
        msg_notice.wLen = static_cast<uint16_t>(str_len);
    };

    const TCHAR* notice_tag_msg = NULL;
    switch (recv_msg->byNoticeType) // MSG_CW_GM_NOTICE_SYN::__unnamed enumeration
    {
    case recv_msg->NOTICE_WORLD:   // 자신이 속한 월드 전체 공지
        {   // 월드 전체 공지-갖고 있는 모든 유저들에게 날린다
            ns_world::WorldUserManager* user_manager = ns_world::WorldUserManager::Instance();
            user_manager->SendToAll(&msg_notice, msg_notice.GetSize());
            notice_tag_msg = _T("World Notice");
        };
        break;
    case recv_msg->NOTICE_CHANNEL: // 자신이 속한 채널 전체 공지
        {   // 해당 유저가 접속한 채널에만 날려준다
            user->SendPacketBelongToChannel(&msg_notice, msg_notice.GetSize());
            notice_tag_msg = _T("Channel Notice");
        };
        break;
    case recv_msg->NOTICE_ZONE:    // 자기가 들어와 있는 지역에만 공지
        {   // 내가 접속한 지역에만 공지
            user->SendPacketBelongToZone(&msg_notice, msg_notice.GetSize());
            notice_tag_msg = _T("Zone Notice");
        };
        break;
    default:
        // NOTE: at this point, this handler received any undefined message types.
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
               _T("Msg=can't notify GM message. invalid notice type ")
               _T("(user_guid=%d, name='%s', notice_type=%d|"),
               user_info.user_guid, user->GetCharName(), recv_msg->byNoticeType);
        return;
    }; //end 'switch'

    //----------------------------------------------------------------------------------------------
    SUNLOG(eCRITICAL_LOG,
        _T("-----------------------------<< %s >>-----------------------------"), notice_tag_msg);
    SUNLOG(eCRITICAL_LOG,
        _T("|Len=%d,Message=[%s]\r\n"), msg_notice.wLen, msg_notice.szMsg);
    SUNLOG(eCRITICAL_LOG,
        _T("---------------------------------------------------------------------------"));

    return;
}

// 스트링 파싱을 통한 GM 명령
HANDLER_NODE_INFO(CW_GM, CW_GM_STRING_CMD_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GM_STRING_CMD_SYN* recv_msg = \
        static_cast<const MSG_CW_GM_STRING_CMD_SYN*>(msg_base);
    //
    // with null-terminated string
    TCHAR copied_buffer[_countof(recv_msg->m_szStringCmd)]; // MAX_STRING_CMD_LENGTH + 1
    _tcsncpy(copied_buffer, recv_msg->m_szStringCmd, _countof(copied_buffer));
    copied_buffer[_countof(copied_buffer) - 1] = _T('\0');
    //
    ns_world::GMCmdManager::Instance()->ParseCommand(recv_msg->m_dwKey, copied_buffer);
}

// TODO: deletion reservation
// 다른 사람의 계정ID 요청
HANDLER_NODE_INFO(CW_COMMUNITY, CW_COMM_USERID_REQ, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_COMM_USERID_REQ* recv_msg = static_cast<const MSG_CW_COMM_USERID_REQ*>(msg_base);
    //
    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };
    ns_world::WorldUser* search_user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharName(recv_msg->m_ptszCharName);
    if (search_user == NULL)
    {
        MSG_CW_COMM_USERID_NAK msg_nak;
        msg_nak.m_wErrorCode = msg_nak._ERR_NOTCONNUSER;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=can't find community user='%s'|"),
               recv_msg->m_ptszCharName);
        return;
    };
    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::UserAccountInfo& search_user_info = search_user->GetUserAccountInfo();
    MSG_CW_COMM_USERID_ACK msg_ack;
    _tcsncpy(msg_ack.m_ptszAccountID, search_user_info.account_id,
             _countof(msg_ack.m_ptszAccountID));
    msg_ack.m_ptszAccountID[_countof(msg_ack.m_ptszAccountID) - 1] = _T('\0');
    _tcsncpy(msg_ack.m_ptszCharName, search_user->GetCharName(),
             _countof(msg_ack.m_ptszCharName));
    user->SendPacket(&msg_ack, sizeof(msg_ack));

    SUNLOG(eFULL_LOG, _T("[GUID:%u][C:%u] UserID Req[GUID:%u][UserID:%s]"),
           user_info.user_guid, server_session->GetChannelID(),
           search_user_info.user_guid, search_user_info.account_id);
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
//--- Heim Lotto System ---
//==================================================================================================
//==================================================================================================
//==================================================================================================

HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_INFO_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_LOTTO_INFO_SYN* recv_msg = static_cast<const MSG_CW_LOTTO_INFO_SYN*>(msg_base);
    //
    Lotto::GetInstance().OnInfoRequest(user->GetCharGuid(), recv_msg);
}

HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_ENTRY_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_LOTTO_ENTRY_SYN* recv_msg = static_cast<const MSG_CW_LOTTO_ENTRY_SYN*>(msg_base);

    Lotto::GetInstance().OnEntryRequest(user->GetCharGuid(), recv_msg);
}

HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_NOTICE_LIST_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_LOTTO_NOTICE_LIST_SYN* recv_msg = \
        static_cast<const MSG_CW_LOTTO_NOTICE_LIST_SYN*>(msg_base);
    //
    Lotto::GetInstance().OnNoticeRequest(user->GetCharGuid(), recv_msg);
}

HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_NOTICE_CONFIRM, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_LOTTO_NOTICE_CONFIRM* recv_msg = \
        static_cast<const MSG_CW_LOTTO_NOTICE_CONFIRM*>(msg_base);
    //
    Lotto::GetInstance().OnNoticeConfirm(user->GetCharGuid(), recv_msg);
}

