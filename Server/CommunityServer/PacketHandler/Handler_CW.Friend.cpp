#include "WorldServerPch.hxx"
#include "./Handler_CW.h"

#include <PacketStruct_WZ.h>
#include <ProhibitionWordParser.h>
//
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
//#include "Services/Guild/WorldGuildManager.h"
#include "Services/Friends/FriendMap.h"
#include "Services/Friends/FriendInvitation.h"

//==================================================================================================

namespace ns_world { namespace handlers {
;

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

// 친구 목록 요청
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_NEWLIST_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_NEWLIST_SYN* recv_msg = \
        static_cast<const MSG_CW_FRIEND_NEWLIST_SYN*>(msg_base);
    //
    if (FlowControl::FCAssert(ns_world::FriendMap::PreparedFriendService(user)) == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=unexpected flow, ")
               _T("user_guid=%u, char_name=%s|"), recv_msg->m_dwKey, user->GetCharName());
        return;
    };

    MSG_CW_FRIEND_FRIENDLIST_ACK msg_friends;
    MSG_CW_FRIEND_BLOCKLIST_ACK msg_blockchars;
    MSG_CW_REVENGE_LIST_CMD msg_revenges;

    ns_world::FriendMap* const friend_map = ns_world::FriendMap::Instance();
    friend_map->LoadFriendList(user, msg_friends.sFriendStat, &msg_friends.byFriendStatNum);
    friend_map->LoadBlockList(user, msg_blockchars.sBlockStat, &msg_blockchars.byBlockStatNum);
    friend_map->LoadRevengeList(user, msg_revenges.sRevengeStat, &msg_revenges.byRevengeStatNum,
        &msg_revenges.byRevengePoint);

    user->SendPacket(&msg_friends, msg_friends.GetSize());
    //_KR_0_20091021_FIRENDINVITATION_EXTEND //에이전트 서버에 친구추천 목록을 전달한다. (W->A->G)
    if (ns_world::FriendInvitationInWorld* event = ns_world::FriendInvitationInWorld::Instance())
    {
        event->NotifyTotalFriendInvitationList(user, event->eMsgForward_ToClient);
    };
    user->SendPacket(&msg_blockchars, msg_blockchars.GetSize());
    user->SendPacket(&msg_revenges, msg_revenges.GetSize());

    SUNLOG(eFULL_LOG, _T("[%s][Guid:%u] Friend/Block/Revenge List Send"),
           user->GetCharName(), user->GetUserGuid());
}

// 친구 요청
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_ADD_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_ADD_SYN* recv_msg = static_cast<const MSG_CW_FRIEND_ADD_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    if (ns_world::FriendMap::IsFriendTransaction(user)) {
        return;
    };
    // NOTE: check whether own character addition request.
    util::StrArrayForCharName finding_target_char_name;
    _tcsncpy(finding_target_char_name, recv_msg->ptszCharNameTo,
             _countof(finding_target_char_name));
    finding_target_char_name[_countof(finding_target_char_name) - 1] = _T('\0');
    if (_tcsnicmp(finding_target_char_name, user->GetCharName(),
                  _countof(finding_target_char_name)) == 0)
    {
        MSG_CW_FRIEND_ADD_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.FRIEND_IS_ME;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=disallow that own character add to friend list, ")
               _T("char_name='%s'|"), finding_target_char_name);
        return;
    };
    if (ns_world::FriendMap::FindFriendByCharName(user, finding_target_char_name) != NULL)
    {
        MSG_CW_FRIEND_ADD_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.ALREADY_FRIEND;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg='%s' already contains '%s' in friend list|"),
               user->GetCharName(), finding_target_char_name);
        return;
    };

    // game rules: 친구 최대 개수는 MAX_FRIEND_STAT_NUM 개 (90->200)
    if (ns_world::FriendMap::GetNumberOfFriends(user) >= MAX_FRIEND_STAT_NUM)
    {
        MSG_CW_FRIEND_ADD_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.ERR_DEFAULT;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eFULL_LOG,
               _T("|["__FUNCTION__"]|Msg='%s' can't add to friend list anymore|"),
               user->GetCharName());
        return;
    };

    // 일단 바~로 DB로 전송~
    MSG_DW_FRIEND_ADD_SYN msg_syn;
    msg_syn.m_CharGuidFrom = user->GetCharGuid();
    _tcsncpy(msg_syn.m_ptszCharNameTo, finding_target_char_name,
             _countof(msg_syn.m_ptszCharNameTo));
    msg_syn.m_ptszCharNameTo[_countof(msg_syn.m_ptszCharNameTo) - 1] = _T('\0');

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        // NOTE: f110906.4L, the world server will have been auto-closed
        // when the database is disconnected... maybe...
        return;
    };

    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
    ns_world::FriendMap::StartFriendTransaction(user);

    SUNLOG(eFULL_LOG, _T("Friend ADD to GameDBProxy(%s->%s)"),
           user->GetCharName(), finding_target_char_name);
}

// 친구 삭제
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_DEL_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_DEL_SYN* recv_msg = static_cast<const MSG_CW_FRIEND_DEL_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    if (ns_world::FriendMap::IsFriendTransaction(user)) {
        return;
    };

    util::StrArrayForCharName finding_target_char_name;
    _tcsncpy(finding_target_char_name, recv_msg->ptszFriendName,
             _countof(finding_target_char_name));
    finding_target_char_name[_countof(finding_target_char_name) - 1] = _T('\0');
    //
    const STRUCT_FRIEND_INFO* friend_info = \
        ns_world::FriendMap::FindFriendByCharName(user, finding_target_char_name);
    if (friend_info == NULL)
    {
        MSG_CW_FRIEND_DEL_NAK   msg_nak;
        msg_nak.m_byResult = msg_nak.NOT_FRIEND_USER;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=can't find '%s' friend in friend list of '%s',CharGuid=%d|"),
               finding_target_char_name, user->GetCharName(), user->GetCharGuid());
        return;
    };

    MSG_DW_FRIEND_DEL_SYN msg_syn;
    msg_syn.m_CharGuidFrom = user->GetCharGuid();
    msg_syn.m_CharGuidTo = friend_info->dwFriendGuid;
    // added data field to support _KR_0_20091021_FIRENDINVITATION_EXTEND //친구 삭제 
    msg_syn.m_Sts = ns_world::FriendInvitationInWorld::Instance()->GetFriendSts(\
        user, friend_info->dwFriendGuid);

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        // NOTE: f110906.4L, the world server will have been auto-closed
        // when the database is disconnected... maybe...
        return;
    };
    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
    ns_world::FriendMap::StartFriendTransaction(user);

    SUNLOG(eFULL_LOG, _T("Friend NO to GameDBProxy(%s->%s)"),
           user->GetCharName(), finding_target_char_name);
}

// 차단 요청
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_BLOCK_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_BLOCK_SYN* recv_msg = static_cast<const MSG_CW_FRIEND_BLOCK_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    if (ns_world::FriendMap::IsFriendTransaction(user)) {
        return;
    };

    // 그냥 차단하면 끝~
    // NOTE: check whether own character addition request.
    util::StrArrayForCharName finding_target_char_name;
    _tcsncpy(finding_target_char_name, recv_msg->ptszBlockName,
             _countof(finding_target_char_name));
    finding_target_char_name[_countof(finding_target_char_name) - 1] = _T('\0');
    if (_tcsnicmp(finding_target_char_name, user->GetCharName(),
                  _countof(finding_target_char_name)) == 0)
    {
        MSG_CW_FRIEND_BLOCK_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.BLOCKED_ME;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=disallow that own character delete from friend list, ")
               _T("char_name='%s'|"), finding_target_char_name);
        return;
    };

    const STRUCT_BLOCK_INFO* block_char_info = \
        ns_world::FriendMap::FindBlockCharByCharName(user, finding_target_char_name);
    if (block_char_info != NULL)
    {
        MSG_CW_FRIEND_BLOCK_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.ALREADY_BLOCKED;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg='%s' is already blocked in friend list of ''%s'|"),
               finding_target_char_name, user->GetCharName());
        return;
    };

    // game rules: 블록 최대 개수는 MAX_BLOCK_STAT_NUM 개 (90->200)
    if (ns_world::FriendMap::GetNumberOfBlocks(user) >= MAX_BLOCK_STAT_NUM)
    {
        MSG_CW_FRIEND_BLOCK_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.ERR_DEFAULT;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eFULL_LOG,
               _T("|["__FUNCTION__"]|Msg='%s' can't add from blocking list anymore|"),
               user->GetCharName());
        return;
    };

    MSG_DW_FRIEND_BLOCK_SYN msg_syn;
    msg_syn.m_CharGuidFrom  = user->GetCharGuid();
    _tcsncpy(msg_syn.m_ptszCharNameTo, finding_target_char_name,
             _countof(msg_syn.m_ptszCharNameTo));
    msg_syn.m_ptszCharNameTo[_countof(msg_syn.m_ptszCharNameTo) - 1] = _T('\0');

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        // NOTE: f110906.4L, the world server will have been auto-closed
        // when the database is disconnected... maybe...
        return;
    };

    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
    ns_world::FriendMap::StartFriendTransaction(user);
    
    SUNLOG(eFULL_LOG, _T("Friend Block to GameDBProxy(%s->%s)"),
           user->GetCharName(), finding_target_char_name);
}

// 차단 삭제 요청
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_BLOCK_FREE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_BLOCK_FREE_SYN* recv_msg = \
        static_cast<const MSG_CW_FRIEND_BLOCK_FREE_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    if (ns_world::FriendMap::IsFriendTransaction(user)) {
        return;
    };
    // NOTE: check whether own character addition request.
    util::StrArrayForCharName finding_target_char_name;
    _tcsncpy(finding_target_char_name, recv_msg->ptszBlockName,
             _countof(finding_target_char_name));
    finding_target_char_name[_countof(finding_target_char_name) - 1] = _T('\0');

    const STRUCT_BLOCK_INFO* block_char_info = \
        ns_world::FriendMap::FindBlockCharByCharName(user, finding_target_char_name);
    if (block_char_info == NULL)
    {
        MSG_CW_FRIEND_BLOCK_FREE_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.NOT_BLOCKED;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };

    // 정확한 차단 정보가 있으면 차단 해제 요청을 DB에 보낸다
    MSG_DW_FRIEND_BLOCK_FREE_SYN msg_syn;
    msg_syn.m_CharGuidFrom = user->GetCharGuid();
    msg_syn.m_CharGuidTo = block_char_info->dwBlockGuid;

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        // NOTE: f110906.4L, the world server will have been auto-closed
        // when the database is disconnected... maybe...
        return;
    };

    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
    ns_world::FriendMap::StartFriendTransaction(user);

    SUNLOG(eFULL_LOG, _T("Friend Block Free to GameDBProxy(%s->%s)"),
           user->GetCharName(), finding_target_char_name);
}

// 친구 채팅
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_CHAT_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_CHAT_SYN* recv_msg = static_cast<const MSG_CW_FRIEND_CHAT_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

    util::StrArrayForGenericChattingMsg chatting_msg;
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(recv_msg->ptszChatMsg));
    const size_t chat_msg_length = min(_countof(recv_msg->ptszChatMsg) - 1, recv_msg->byMsgLength);
    if (chat_msg_length == 0 ||
        chat_msg_length != recv_msg->byMsgLength ||
        recv_msg->GetSize() != msg_size)
    {
        return;
    };
    //
    MSG_CW_FRIEND_CHAT_NAK msg_nak;
    BOOST_STATIC_ASSERT(_countof(msg_nak.ptszFriendNameTo) == _countof(recv_msg->ptszFriendNameTo));
    strncpy(msg_nak.ptszFriendNameTo, recv_msg->ptszFriendNameTo,
            _countof(msg_nak.ptszFriendNameTo));
    msg_nak.ptszFriendNameTo[_countof(msg_nak.ptszFriendNameTo) - 1] = '\0';

    // 모든 채팅에서 크기가 0이거나 실제 내용이 없으면 무시
//  if (!recv_msg->byMsgLength || !_tcslen(recv_msg->ptszChatMsg))
//  {
//      SUNLOG(eFULL_LOG, "채팅 내용 없음");
//      return;
//  }

    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    ns_world::WorldUser* const friend_user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharName(msg_nak.ptszFriendNameTo);
    //__KR_001355_20090608_GROUPINGSYSTEM_CW
    const STRUCT_GROUP_MEMBER_INFO* group_member_info = (friend_user == NULL) ?
        NULL : group_manager->FindGroupMember(user, friend_user->GetCharGuid());
    // Check online
    const bool not_entered_state = \
        (friend_user == NULL || friend_user->GetConnectedCharInfo().zone_type == eZONETYPE_LOBBY);
    if (not_entered_state && group_member_info == NULL)
    {
        msg_nak.m_byResult = msg_nak.OFFLINE_FRIEND;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eFULL_LOG,
               _T("|["__FUNCTION__"]|Msg='%s' does not enable service, reason=offline|"),
               msg_nak.ptszFriendNameTo);
        return;
    };

    const STRUCT_FRIEND_INFO* friend_info = \
        ns_world::FriendMap::FindFriend(user, friend_user->GetCharGuid());
    if (friend_info == NULL && group_member_info == NULL)
    {
        if (ns_world::FriendMap::FindFriend(friend_user, user->GetCharGuid()) == NULL)
        {   // 상대에게도 친구 아이다!!
            msg_nak.m_byResult = msg_nak.NOT_FRIEND;
            user->SendPacket(&msg_nak, sizeof(msg_nak));
            SUNLOG(eFULL_LOG,
                   _T("|["__FUNCTION__"]|Msg='%s' is not friend with requester ('%s')|"),
                   msg_nak.ptszFriendNameTo, user->GetCharName());
            return;
        };
    };

    const bool enabled_chatting = \
        (friend_user->GetChattingMode().friend_chatting_blocked == false) &&
        (ns_world::FriendMap::FindBlockChar(friend_user, user->GetCharGuid()) == NULL);
    if (enabled_chatting == false && group_member_info == NULL)
    {
        msg_nak.m_byResult = msg_nak.BLOCK_CHAT;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eFULL_LOG,
               _T("|["__FUNCTION__"]|Msg='%s' can not communicate, reason=chatting blocked|"),
               msg_nak.ptszFriendNameTo);
        return;
    };

    // rebuild chatting message, chat_msg_length does not contain null character
    assert(chat_msg_length < _countof(chatting_msg)); // already calculated
    strncpy(chatting_msg, recv_msg->ptszChatMsg, chat_msg_length);
    chatting_msg[chat_msg_length] = '\0';
    chatting_msg[_countof(chatting_msg) - 1] = '\0';
    //
    if (ProhibitionWordParser::Instance()->CheckFilter(chatting_msg) == false)
    {
        MSG_CW_CHAT_NORMAL_NAK chat_msg_nak;
        chat_msg_nak.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;
        user->SendPacket(&chat_msg_nak, sizeof(chat_msg_nak));
        SUNLOG(eFULL_LOG,
               _T("|["__FUNCTION__"]|Msg='%s' use banned words|"),
               user->GetCharName());
        return;
    };
    //
    MSG_CW_FRIEND_CHAT_BRD msg_brd;
    strncpy(msg_brd.ptszFriendNameFrom, user->GetCharName(), _countof(msg_brd.ptszFriendNameFrom));
    msg_brd.ptszFriendNameFrom[_countof(msg_brd.ptszFriendNameFrom) - 1] = '\0';
    //
    BOOST_STATIC_ASSERT(_countof(msg_brd.ptszChatMsg) == _countof(chatting_msg));
    msg_brd.byMsgLength = static_cast<uint8_t>(::strlen(chatting_msg));
    strncpy(msg_brd.ptszChatMsg, chatting_msg, _countof(msg_brd.ptszChatMsg));
    //
    friend_user->SendPacket(&msg_brd, msg_brd.GetSize());
    //
    GAMELOG->WriteFriendChat(user, server_session->GetChannelID(),
        friend_user, chatting_msg, _countof(chatting_msg) - 1);
    SUNLOG(eFULL_LOG, _T("Friend Chat[%s -> %s]: %s"),
        user->GetCharName(), friend_user->GetCharName(), chatting_msg);
}

// 1:1 대화 거부 체크
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_CHATBLOCK_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_CHATBLOCK_SYN* recv_msg = \
        static_cast<const MSG_CW_FRIEND_CHATBLOCK_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    // 해당 유저의 1:1대화를 거부한다
    ns_world::ChattingMode chatting_mode = user->GetChattingMode();
    ;{  // 이 유저의 귓말 켬/끔 상태 세팅
        chatting_mode.friend_chatting_blocked = (recv_msg->m_bBlock != false);
        ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
        user_manager->ChangeChattingMode(user, chatting_mode);
    };

    MSG_CW_FRIEND_CHATBLOCK_ACK msg_ack;
    msg_ack.m_bNowBlock = chatting_mode.friend_chatting_blocked;
    user->SendPacket(&msg_ack, sizeof(msg_ack));

    SUNLOG(eFULL_LOG, _T("[%s] Friend Chat Block(%u)"),
           user->GetCharName(), chatting_mode.friend_chatting_blocked);
}

// 친구/리벤지/길드원/그룹원 상세정보 요청
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_STATUS_REQ, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_STATUS_REQ* recv_msg = \
        static_cast<const MSG_CW_FRIEND_STATUS_REQ*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        // NOTE: f110906.4L, the world server will have been auto-closed
        // when the database is disconnected... maybe...
        return;
    };
    // NOTE: updates logic by the _NA_0_20100527_ADD_CHAR_RELATION_MEMO contents
    RC::eFRIEND_RESULT friend_result = \
        ns_world::FriendMap::CheckAccessibleFriendInfo(user, recv_msg->m_dwToCharGuid);

    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    if (friend_result == RC::RC_FRIEND_SUCCESS)
    {
        MSG_DW_FRIEND_DETAIL_INFO_SYN msg_syn;
        msg_syn.m_dwKey = char_info.char_guid;
        msg_syn.m_dwToCharGuid = recv_msg->m_dwToCharGuid;
        dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
    }
    else if (friend_result == RC::RC_FRIEND_NOTGUILDMEMBER)
    {
        MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_SYN msg_syn;
        msg_syn.m_dwKey = char_info.char_guid;
        msg_syn.m_guildGuid = char_info.guild_guid;
        msg_syn.m_dwToCharGuid = recv_msg->m_dwToCharGuid;
        dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
    }
    else
    {
        MSG_CW_FRIEND_STATUS_NAK nak_msg;
        nak_msg.m_byErrorCode = static_cast<uint8_t>(friend_result);
        user->SendPacket(&nak_msg, sizeof(nak_msg));
    };
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_MEMO_UPDATE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_MEMO_UPDATE_SYN* recv_msg = \
        static_cast<const MSG_CW_FRIEND_MEMO_UPDATE_SYN*>(msg_base);
    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    RC::eFRIEND_RESULT friend_result = \
        ns_world::FriendMap::CheckAccessibleFriendInfo(user, recv_msg->m_dwToCharGuid);

    switch (friend_result)
    {
    case RC::RC_FRIEND_SUCCESS:
    case RC::RC_FRIEND_NOTGUILDMEMBER:          // 한번 이상 접속 하지 않은 오프라인 길드원
        {                                       // 여기까지 왔다면, 상세 정보 창을 열었다는 뜻이며, 길드서버에 선 체크를 한 상황이므로 통과 한다.
            MSG_DW_FRIEND_MEMO_UPDATE_SYN msg_syn;
            BOOST_STATIC_ASSERT(_countof(msg_syn.m_ptszMemo) == _countof(recv_msg->m_ptszMemo));
            //
            msg_syn.m_dwKey = user->GetCharGuid();
            msg_syn.m_dwToCharGuid = recv_msg->m_dwToCharGuid;

            _tcsncpy(msg_syn.m_ptszMemo, recv_msg->m_ptszMemo, _countof(msg_syn.m_ptszMemo));
            msg_syn.m_ptszMemo[_countof(msg_syn.m_ptszMemo) - 1] = _T('\0');

            ns_world::ServerSessionInWorld* dbproxy = \
                ns_world::WorldServer::Instance()->GetGameDBProxy();
            if (dbproxy != NULL) {
                dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
            }
            else {
                friend_result = RC::RC_FRIEND_FAIL;
            };
        }
        break;
    };

    if (friend_result != RC::RC_FRIEND_SUCCESS)
    {
        MSG_CW_FRIEND_MEMO_UPDATE_NAK nak_msg;
        nak_msg.m_byErrorCode = static_cast<uint8_t>(friend_result);
        user->SendPacket(&nak_msg, sizeof(nak_msg));
    };
}

// 친구 따라가기
HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_STATUS_JOIN_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_FRIEND_STATUS_JOIN_SYN* recv_msg = \
        static_cast<const MSG_CW_FRIEND_STATUS_JOIN_SYN*>(msg_base);
    //
    MSG_CW_FRIEND_STATUS_JOIN_NAK msg_nak;
    ;{
        ZeroMemory(&msg_nak, sizeof(msg_nak));
        new (&msg_nak) MSG_CW_FRIEND_STATUS_JOIN_NAK;
    };
    strncpy(msg_nak.m_ptszCharName, recv_msg->m_ptszCharName, _countof(msg_nak.m_ptszCharName));
    msg_nak.m_ptszCharName[_countof(msg_nak.m_ptszCharName) - 1] = '\0';


    ns_world::WorldUser* const target_user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharName(msg_nak.m_ptszCharName);
    if (target_user == NULL)
    {   // not found, not connected, etc...
        msg_nak.m_byErrorCode = RC::RC_FRIEND_NOTCONNECTED;  // 해당 유저 미접속
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=target('%s') is not connecting|"),
               msg_nak.m_ptszCharName);
        return;
    };

    const ns_world::ConnectedCharInfo& target_char_info = target_user->GetConnectedCharInfo();
    // __KR_001355_20090608_GROUPINGSYSTEM_CW
    const STRUCT_GROUP_MEMBER_INFO* group_member_info = \
        ns_world::GroupManager::Instance()->FindGroupMember(user, target_char_info.char_guid);
    // History
    //  BUG:5045 문상현, remove the check condition whether group_member_info is null

    if (user->GetUserAccountInfo().channel != target_user->GetUserAccountInfo().channel)
    {
        msg_nak.m_byErrorCode = RC::RC_FRIEND_DIFFCHANNEL;   // 채널 달라요~
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        int target_channel = target_user->GetUserAccountInfo().channel ?
            target_user->GetUserAccountInfo().channel->GetChannelID() : 0;
        SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=target('%s') is located on other channel(%d), ")
               _T("current channel(%d)|"),
               msg_nak.m_ptszCharName, target_channel,
               user->GetUserAccountInfo().channel->GetChannelID());
        return;
    };
    //
    const STRUCT_FRIEND_INFO* friend_info = \
        ns_world::FriendMap::FindFriend(user, target_char_info.char_guid);
    if ((friend_info == NULL || friend_info->dwFriendGuid != target_char_info.char_guid) &&
        (group_member_info == NULL))
    {
        // check whether the revenging target
        const STRUCT_REVENGE_INFO* revenge_info = \
            ns_world::FriendMap::FindRevengeChar(user, target_char_info.char_guid);
        if (revenge_info == NULL ||
            revenge_info->dwRevengeCharGuid != target_char_info.char_guid)
        {
            msg_nak.m_byErrorCode = RC::RC_FRIEND_NOTFRIEND; // 친구가 아니거나 잘못된 정보임
            user->SendPacket(&msg_nak, sizeof(msg_nak));
            SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=target('%s')'s revenge info. is invalid|"),
                   msg_nak.m_ptszCharName);
            return;
        };
    };
    // 방 타입이 잘못되었으면
    if (target_char_info.zone_type != eZONETYPE_LOBBY &&
        target_char_info.zone_type != eZONETYPE_CHUNTING)
    {
        msg_nak.m_byErrorCode = RC::RC_FRIEND_INVALIDZONE;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=target('%s')'s is located on invalid zone(%d)"),
               msg_nak.m_ptszCharName, target_char_info.zone_type);
        return;
    };

    // 현재 배틀존 정보 넣는다
    MSG_CW_FRIEND_STATUS_JOIN_ACK msg_ack;
    msg_ack.m_byZoneType = target_char_info.zone_type;
    msg_ack.m_dwZoneKey = target_char_info.zone_key;
    BOOST_STATIC_ASSERT(_countof(msg_ack.m_ptszCharName) == _countof(msg_nak.m_ptszCharName));
    strncpy(msg_ack.m_ptszCharName, msg_nak.m_ptszCharName, _countof(msg_ack.m_ptszCharName));
    user->SendPacket(&msg_ack, sizeof(msg_ack));

    SUNLOG(eFULL_LOG, _T("CW_FRIEND_STATUS_JOIN_ACK send[requester char:%u]")
           _T("[Target:%s][ZT:%u][ZK:%u]"),
           user->GetCharGuid(),
           msg_ack.m_ptszCharName, 
           target_char_info.zone_type,
           target_char_info.zone_key);
}

// 리벤지 리스트에서 삭제
HANDLER_NODE_INFO(CW_REVENGE, CW_REVENGE_DEL_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_REVENGE_DEL_SYN* recv_msg = static_cast<const MSG_CW_REVENGE_DEL_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

    if (ns_world::FriendMap::IsFriendTransaction(user)) {
        return;
    };

    // 혹시 있을지 모르는 문자열 길이 오류 보정
    // NOTE: fixed, prevent changes about cg network buffer stream
    util::StrArrayForCharName char_name;
    strncpy(char_name, recv_msg->m_ptszCharName, _countof(char_name));
    char_name[_countof(char_name) - 1] = '\0';

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        return;
    };

    // 나쁜친구 맞아?
    const STRUCT_REVENGE_INFO* revenge_info = \
        ns_world::FriendMap::FindRevengeCharByCharName(user, char_name);
    if (revenge_info == NULL)
    {
        MSG_CW_REVENGE_DEL_NAK msg_nak;
        CopyMemory(msg_nak.m_ptszCharName, char_name, sizeof(msg_nak.m_ptszCharName));
        msg_nak.m_byErrorCode = RC::RC_REVENGE_INVALID_CHAR;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, _T("Revenge Del Error - Not Revenge[%s][CharGuid:%u]->[%s]"),
               user->GetCharName(), user->GetCharGuid(), char_name);
        return;
    };

    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    MSG_DW_CHAO_REMOVE_REVENGE_CHAR_SYN msg_dg_syn;
    msg_dg_syn.m_dwKey = server_session->GetChannelID(); // ... m_dwKey usage warnings
    msg_dg_syn.m_CharGuidFrom  = char_info.char_guid;
    msg_dg_syn.m_RevengeGuidTo = revenge_info->dwRevengeCharGuid;
    //
    dbproxy->SendPacket(&msg_dg_syn, sizeof(msg_dg_syn));

    ns_world::FriendMap::StartFriendTransaction(user);

    SUNLOG(eFULL_LOG, _T("Revenge Del to GameDBProxy(%s->%s)"),
           char_info.char_name, revenge_info->sPacketInfo.ptszRevengeName);
}
