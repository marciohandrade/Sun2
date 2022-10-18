#include "WorldServerPch.hxx"
#include "./Handler_CW.h"

#include <ProhibitionWordParser.h>
#include <GameOptionParser.h>

#include "./PacketHandler.h"
#include "WorldServer.h"

//#include "Sessions/ServerSessionInWorld.h"
#include "Channel/WorldChannelManager.h"
//#include "Channel/WorldChannel.h"
#include "Channel/ChatRoomCommunity.h"
#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
#include "Services/ChattingService.h"
//#include "Services/Lotto/Lotto.h"
//#include "Services/Group/GroupManager.h"
//#include "Services/Guild/WorldGuildManager.h"
#include "Services/Friends/FriendMap.h"

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

HANDLER_NODE_INFO(CW_CHAT, CW_WHISPER_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_WHISPER_SYN* recv_msg = static_cast<const MSG_CW_WHISPER_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

    util::StrArrayForChatting chatting_msg;
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(recv_msg->szWhisperMsg));
    const size_t chat_msg_length = min(_countof(recv_msg->szWhisperMsg) - 1, recv_msg->byMsgLen);
    if (chat_msg_length == 0 ||
        chat_msg_length != recv_msg->byMsgLen ||
        recv_msg->GetSize() != msg_size)
    {
        return;
    };

    // rebuild chatting message, chat_msg_length contains null character
    strncpy(chatting_msg, recv_msg->szWhisperMsg, chat_msg_length);
    chatting_msg[chat_msg_length] = '\0';
    chatting_msg[_countof(chatting_msg) - 1] = '\0';
    //
    util::StrArrayForCharName target_char_name;
    BOOST_STATIC_ASSERT(_countof(target_char_name) == _countof(recv_msg->szCharNameTo));
    strncpy(target_char_name, recv_msg->szCharNameTo, _countof(target_char_name));
    target_char_name[_countof(target_char_name) - 1] = '\0';

    // 모든 채팅에서 크기가 0이거나 실제 내용이 없으면 무시
//  if (!recv_msg->byMsgLen || !_tcslen(recv_msg->szWhisperMsg))
//  {
//      SUNLOG(eFULL_LOG, "채팅 내용 없음");
//      return;
//  }

    ns_world::WorldUser* const target_user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharName(target_char_name);

    if (user == target_user)
    {
        SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg='%s' does not enable service, ")
               _T("reason=can't send to me|"), target_char_name);
        return;
    };

    if (const bool disabled_state = (target_user == NULL) ||
            (target_user->GetConnectedCharInfo().zone_type == eZONETYPE_LOBBY))
    {
        MSG_CW_WHISPER_NAK msg_nak;
        msg_nak.byReason = msg_nak.USER_NOT_FOUND;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg='%s' does not enable service, reason=offline|"),
               target_char_name);
        return;
    };

    if (ns_world::FriendMap::FindBlockChar(target_user, user->GetCharGuid()))
    {
        MSG_CW_WHISPER_NAK msg_nak;
        msg_nak.byReason = msg_nak.USER_WHISPER_OFF;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };
    // DONE: replace to ns_world::ChattingService::EnabledWhisperingIntercepted(user)
    if (target_user->GetChattingMode().whispering_intercepted)
    {
        // NOTE: GM 유저 맞으면 귓말은 정상적으로 보내지만, 유저에게는 귓말 보내지 못한다고 알려준다
        const bool is_gm = (target_user->GetUserAccountInfo().gm_grade >= eGM_GRADE_FIRST);

        MSG_CW_WHISPER_NAK msg_nak;
        msg_nak.byReason = msg_nak.USER_WHISPER_OFF;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        //
        eSUN_LOG_LEVEL log_level = is_gm ? eCRITICAL_LOG : eFULL_LOG;
        if (is_gm)
        {
            MSG_CW_WHISPER_ACK msg_ack;
            strncpy(msg_ack.szCharNameFrom, user->GetCharName(), _countof(msg_ack.szCharNameFrom));
            msg_ack.szCharNameFrom[_countof(msg_ack.szCharNameFrom) - 1] = '\0';
            BOOST_STATIC_ASSERT(_countof(msg_ack.szWhisperMsg) == _countof(chatting_msg));
            msg_ack.byMsgLen = static_cast<uint8_t>(::_tcslen(chatting_msg));
            strncpy(msg_ack.szWhisperMsg, chatting_msg, _countof(msg_ack.szWhisperMsg));
            target_user->SendPacket(&msg_ack, msg_ack.GetSize());
        };

        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg='%s' wisper to '%s', gm_state=%d, ")
               _T("whispered msg='%s'|"),
               user->GetCharName(), target_char_name, is_gm, chatting_msg);
        return;
    };

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

    // 대상에게 귓말 전송
    ;{
        MSG_CW_WHISPER_ACK msg_ack;
        strncpy(msg_ack.szCharNameFrom, user->GetCharName(), _countof(msg_ack.szCharNameFrom));
        msg_ack.szCharNameFrom[_countof(msg_ack.szCharNameFrom) - 1] = '\0';
        BOOST_STATIC_ASSERT(_countof(msg_ack.szWhisperMsg) == _countof(chatting_msg));
        msg_ack.byMsgLen = static_cast<uint8_t>(::_tcslen(chatting_msg));
        strncpy(msg_ack.szWhisperMsg, chatting_msg, _countof(msg_ack.szWhisperMsg));
        target_user->SendPacket(&msg_ack, msg_ack.GetSize());
    };

    // GAMELOG
    GAMELOG->WriteWhisperChat(user, server_session->GetChannelID(),
        target_user, chatting_msg, _countof(chatting_msg) - 1);

    SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg='%s' wisper to '%s', ")
           _T("whispered msg='%s'|"),
           user->GetCharName(), target_char_name, chatting_msg);
}

HANDLER_NODE_INFO(CW_CHAT, CW_NOTICE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_NOTICE_SYN* recv_msg = static_cast<const MSG_CW_NOTICE_SYN*>(msg_base);
    //

    // 길이가 최대값을 넘기면 접속을 끊는다.
/*  if (recv_msg->byLen > MAX_NOTICE_LEN)
    {
        user->Disconnect();
        return;
    }

    // TODO: 공지 권한이 없으면 리턴한다.

    char szNotice[MAX_NOTICE_LEN + 1];
    ZeroMemory(szNotice, sizeof(szNotice));
    strncpy(szNotice, recv_msg->szMsg, recv_msg->byLen);

    WorldServer::Instance()->SendNoticeMsg(szNotice);*/
}

// 귓말 켜고 끔(차단) 처리
HANDLER_NODE_INFO(CW_CHAT, CW_WHISPER_SET_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_WHISPER_SET_SYN* recv_msg = static_cast<const MSG_CW_WHISPER_SET_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

    // 이 유저의 귓말 켬/끔 상태 세팅
    ns_world::ChattingService::EnableWhisperingIntercepted(user, (recv_msg->bWhisper != 0));

    // 성공했음을 알린다
    MSG_CW_WHISPER_SET_ACK msg_ack;
    msg_ack.bWhisper = recv_msg->bWhisper;
    user->SendPacket(&msg_ack, sizeof(msg_ack));

    SUNLOG(eCRITICAL_LOG,  "(%s) Whisper Set(%d)", user->GetCharName(), recv_msg->bWhisper);
}

// 채팅 요청
HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_NORMAL_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_CHAT_NORMAL_SYN* recv_msg = static_cast<const MSG_CW_CHAT_NORMAL_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

#if WORLD_CONTENTS
#ifndef _DEBUG
    // 부하 방지를 위해 1/10초 이하의 시간에 연달아 보낸 채팅은 무시
    if (!user->CheckChatTime())
    {
        return;
    }
#endif
#endif
    util::StrArrayForGenericChattingMsg chatting_msg;
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(recv_msg->m_pszChatMsg));
    const size_t chat_msg_length = min(_countof(recv_msg->m_pszChatMsg) - 1, recv_msg->m_byMsgLength);
    if (chat_msg_length == 0 ||
        chat_msg_length != recv_msg->m_byMsgLength ||
        recv_msg->GetSize() != msg_size)
    {
        return;
    };

    // rebuild chatting message, chat_msg_length does not contain null character
    strncpy(chatting_msg, recv_msg->m_pszChatMsg, chat_msg_length);
    chatting_msg[chat_msg_length] = '\0';
    chatting_msg[_countof(chatting_msg) - 1] = '\0';

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
#if WORLD_CONTENTS
    //채팅 도배인지 체크
    //1. 같은 메세지면..
    if (strcmp(recv_msg->m_pszChatMsg, user->GetLastChatMsg()) == 0)    
    {
        //2. 시간이 5초를 지났는지..
        if (!user->IsExpiredChatTimer())
        {
            MSG_CW_CHAT_NORMAL_NAK  sendMsg;
            sendMsg.m_wErrorCode = RC::RC_CHAT_ANTI_DOBAE;
            user->SendPacket(&sendMsg, sizeof(MSG_CW_CHAT_NORMAL_NAK));
            return;         
        }
    }
    else
    {
        //메세지가 다르면.. 타이머와 마지막 메세지를 새로 세팅한다.
        user->SetLastChatMsg(recv_msg->m_pszChatMsg);
        user->SetChatTimer(5 * 1000);        //5초
    }
#endif

    // 채팅 메시지 패킷 조립
    MSG_CW_CHAT_NORMAL_BRD msg_brd;
    strncpy(msg_brd.m_ptszCharName, user->GetCharName(), _countof(msg_brd.m_ptszCharName));
    msg_brd.m_ptszCharName[_countof(msg_brd.m_ptszCharName) - 1] = '\0';
    msg_brd.m_byMsgLength = static_cast<uint8_t>(::strlen(chatting_msg));
    strncpy(msg_brd.m_ptszChatMsg, chatting_msg, _countof(msg_brd.m_ptszChatMsg));
    user->SendNormalChat(&msg_brd, msg_brd.GetSize());

    GAMELOG->WriteNormalChat(user, server_session->GetChannelID(),
        chatting_msg, _countof(chatting_msg) - 1);

    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    SUNLOG(eFULL_LOG, _T("CW_CHAT_NORMAL_BRD[%s][C:%u][ZT:%u][ZK:%u]:%s"),
        user->GetCharName(), server_session->GetChannelID(),
        char_info.zone_type, char_info.zone_key, chatting_msg);
}

HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_SHOUT_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_CHAT_SHOUT_SYN* recv_msg = static_cast<const MSG_CW_CHAT_SHOUT_SYN*>(msg_base);
    //
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
#if WORLD_CONTENTS_BLOCK
#ifndef _DEBUG
    // 부하 방지를 위해 1/10초 이하의 시간에 연달아 보낸 채팅은 무시
    if (!user->CheckChatTime())
    {
        return;
    }
#endif
#endif

    util::StrArrayForGenericChattingMsg chatting_msg;
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(recv_msg->m_pszChatMsg));
    const size_t chat_msg_length = min(_countof(recv_msg->m_pszChatMsg) - 1, recv_msg->m_byMsgLength);
    if (chat_msg_length == 0 ||
        chat_msg_length != recv_msg->m_byMsgLength ||
        recv_msg->GetSize() != msg_size)
    {
        return;
    };

    // rebuild chatting message, chat_msg_length does not contain null character
    strncpy(chatting_msg, recv_msg->m_pszChatMsg, chat_msg_length);
    chatting_msg[chat_msg_length] = '\0';
    chatting_msg[_countof(chatting_msg) - 1] = '\0';

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

    // 채팅 메시지 패킷 조립
    MSG_CW_CHAT_SHOUT_BRD msg_brd;
    strncpy(msg_brd.m_szCharName, user->GetCharName(), _countof(msg_brd.m_szCharName));
    msg_brd.m_szCharName[_countof(msg_brd.m_szCharName) - 1] = '\0';
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(msg_brd.m_pszChatMsg));
    msg_brd.m_byMsgLength = static_cast<uint8_t>(::strlen(chatting_msg));
    strncpy(msg_brd.m_pszChatMsg, chatting_msg, _countof(msg_brd.m_pszChatMsg));
    //
    MSG_CW_CHAT_SHOUT_NAK msg_nak;

    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    //
    bool is_shouting_item = false;
    bool is_super_master_item = false;
    bool deleted_item_post_shouting = false;
    bool is_pc_room = (user_info.pc_room_id != 0);
#if WORLD_CONTENTS_BLOCK
    if (user->GetItemOwnFlag().FindItemInfo(eITEMTYPE_CHAT_SHOUT_CONSUME) != NULL)  deleted_item_post_shouting = true;
    if (user->GetItemOwnFlag().FindItemInfo(eITEMTYPE_CHAT_SHOUT) != NULL)  is_shouting_item = true;
    if (user->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER) != NULL) is_super_master_item = true;
#endif

    if (recv_msg->m_byMessageType == recv_msg->GENERAL_SHOUT) //@
    {
        const GAME_OPTION& game_option = GameOptionParser::Instance()->GetGameOption();
        if (game_option.m_byNormalShoutLevel == 0) { //0이면 블럭상태이다.
            return;
        };
        if (char_info.char_level < game_option.m_byNormalShoutLevel)
        {
            msg_nak.m_wErrorCode = RC::RC_GENERAL_SHOUT_LIMIT_LEVEL;
            user->SendPacket(&msg_nak, sizeof(msg_nak));
            return;
        };

        msg_brd.m_byMessageType = msg_brd.GENERAL_SHOUT;
        if (is_super_master_item || is_pc_room)
        {
            switch (char_info.zone_type)
            {
            case eZONETYPE_LOBBY:
            case eZONETYPE_HUNTING:
            case eZONETYPE_MISSION:
            case eZONETYPE_CHUNTING:
                msg_brd.m_byMessageType = msg_brd.SUPER_MASTER_SHOUT;
                break;
            };
        };
        user->SendPacketShouting(&msg_brd, msg_brd.GetSize(), NULL);
    }
    else if (recv_msg->m_byMessageType == recv_msg->WORLD_SHOUT) //@@
    {
        const GAME_OPTION& game_option = GameOptionParser::Instance()->GetGameOption();
        // NOTE: ...unused?
    #ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
        if (game_option.m_byWorldShoutLevel == 0)         //0이면 블럭상태이다.
            return;

        if (char_info.char_level < game_option.m_byWorldShoutLevel)
        {
            msg_nak.m_wErrorCode = RC::RC_WORLD_SHOUT_LIMIT_LEVEL;
            user->SendPacket(&msg_nak, sizeof(msg_nak));
            return;
        };
    #endif
        // 소모성 외치기 아이템이 있거나(기간제가 아니다.), PC방인데 PC방에서 외치기가 가능한 옵션이면.
        bool right_now = false;

        // 기간제 외치기 아이템이 있거나, PC방인데 PC방에서 외치기가 가능한 옵션이면.
        if (is_shouting_item || (is_pc_room && game_option.m_byWorldShoutPCBANG))
        {
            right_now = true;
        }
        else if (deleted_item_post_shouting) // 소모성 외치기 아이템이 있을시.
        {
#if WORLD_CONTENTS_BLOCK
            user->SetTmpDecreeMsg(msg_brd.m_pszChatMsg);
#endif
        }
        else
        {
            msg_nak.m_wErrorCode = RC::RC_CHAT_NOT_DECREE_ITEM;
            user->SendPacket(&msg_nak, sizeof(msg_nak));
            return;
        };

        msg_brd.m_byMessageType = msg_brd.DECREE_SHOUT;
        //
        ns_world::WorldShoutingArg shouting_arg = { 0, };
        shouting_arg.whole_shouting = true;
        shouting_arg.using_item = right_now;
        user->SendPacketShouting(&msg_brd, msg_brd.GetSize(), &shouting_arg);
    };

    GAMELOG->WriteShoutChat(user, server_session->GetChannelID(),
        chatting_msg, _countof(chatting_msg) - 1);
    //
    SUNLOG(eFULL_LOG, _T("CW_CHAT_SHOUT_SYN[%s][C:%u][ZK:%u]:%s"),
           user->GetCharName(), server_session->GetChannelID(),
           char_info.zone_key, chatting_msg);
}

// 파티 채팅 처리
HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_PARTY_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_CHAT_PARTY_SYN* recv_msg = static_cast<const MSG_CW_CHAT_PARTY_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    //
    util::StrArrayForGenericChattingMsg chatting_msg;
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(recv_msg->m_ptszChatMsg) &&
                        sizeof(chatting_msg) == sizeof(recv_msg->m_ptszChatMsg));
    const size_t chat_msg_length = min(_countof(recv_msg->m_ptszChatMsg) - 1, recv_msg->m_byMsgLength);
    if (chat_msg_length == 0 ||
        chat_msg_length != recv_msg->m_byMsgLength ||
        recv_msg->GetSize() != msg_size)
    {
        return;
    };
    // rebuild chatting message, chat_msg_length does not contain null character
    strncpy(chatting_msg, recv_msg->m_ptszChatMsg, chat_msg_length);
    chatting_msg[chat_msg_length] = '\0';
    chatting_msg[_countof(chatting_msg) - 1] = '\0';

    // 모든 채팅에서 크기가 0이거나 실제 내용이 없으면 무시
//  if (!recv_msg->m_byMsgLength || !_tcslen(recv_msg->m_ptszChatMsg))
//  {
//      SUNLOG(eFULL_LOG, "채팅 내용 없음");
//      return;
//  }

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

    MSG_CW_CHAT_PARTY_BRD msg_brd;
    strncpy(msg_brd.m_ptszCharName, user->GetCharName(), _countof(msg_brd.m_ptszCharName));
    msg_brd.m_ptszCharName[_countof(msg_brd.m_ptszCharName) - 1] = '\0';
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(msg_brd.m_ptszChatMsg));
    msg_brd.m_byMsgLength = static_cast<uint8_t>(::strlen(chatting_msg));
    strncpy(msg_brd.m_ptszChatMsg, chatting_msg, _countof(msg_brd.m_ptszChatMsg));

    user->SendPacketToParty(&msg_brd, msg_brd.GetSize());

    GAMELOG->WritePartyChat(user, server_session->GetChannelID(),
        chatting_msg, _countof(chatting_msg) - 1);
    SUNLOG(eFULL_LOG, _T("[%s] Send Party Chat, chat_msg='%s'"),
           user->GetCharName(), chatting_msg);
}

// 채널채팅방 입장 요청
HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_JOIN_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_CHAT_CHANNEL_JOIN_SYN* recv_msg = \
        static_cast<const MSG_CW_CHAT_CHANNEL_JOIN_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

    util::StrArrayForRoomTitle room_title;
    BOOST_STATIC_ASSERT(_countof(recv_msg->m_ptszRoomName) == _countof(room_title));
    _tcsncpy(room_title, recv_msg->m_ptszRoomName, _countof(room_title));
    room_title[_countof(room_title) - 1] = _T('\0');

    if (ProhibitionWordParser::Instance()->CheckFilterName(room_title) == false)
    {
        MSG_CW_CHAT_NORMAL_NAK chat_msg_nak;
        chat_msg_nak.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;
        user->SendPacket(&chat_msg_nak, sizeof(chat_msg_nak));
        return;
    };

    ns_world::WorldChannelManager* const channel_manager = \
        ns_world::WorldChannelManager::Instance();
    bool entered = channel_manager->EnterChatRoomCommunity(user, room_title);
    SUNLOG(eFULL_LOG, _T("CHAT_CHANNEL_JOIN_SYN[CharGuid:%u],entered=%d,RoomName='%s'"),
           user->GetCharGuid(), entered, room_title);
}

// 채널채팅 보냄
HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_SEND_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_CHAT_CHANNEL_SEND_SYN* recv_msg = \
        static_cast<const MSG_CW_CHAT_CHANNEL_SEND_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    const ns_world::community::ChatRoomCommunity* const chat_room = \
        ns_world::WorldChannelManager::GetLinkedChatRoom(user);
    if (chat_room == NULL) {
        return;
    };

    util::StrArrayForChatting chatting_msg;
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(recv_msg->m_ptszChat));
    const size_t chat_msg_length = min(_countof(recv_msg->m_ptszChat) - 1, recv_msg->m_byChatLength);
    if (chat_msg_length == 0 ||
        chat_msg_length != recv_msg->m_byChatLength ||
        recv_msg->GetSize() != msg_size)
    {
        return;
    };
    // rebuild chatting message, chat_msg_length does not contain null character
    strncpy(chatting_msg, recv_msg->m_ptszChat, chat_msg_length);
    chatting_msg[chat_msg_length] = '\0';
    chatting_msg[_countof(chatting_msg) - 1] = '\0';

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

    MSG_CW_CHAT_CHANNEL_SEND_BRD msg_brd;
    msg_brd.m_byChannelID = static_cast<uint8_t>(server_session->GetChannelID());
    _tcsncpy(msg_brd.m_ptszCharName, user->GetCharName(), _countof(msg_brd.m_ptszCharName));
    msg_brd.m_ptszCharName[_countof(msg_brd.m_ptszCharName) - 1] = _T('\0');
    BOOST_STATIC_ASSERT(_countof(chatting_msg) == _countof(msg_brd.m_ptszChat));
    msg_brd.m_byChatLength = static_cast<uint8_t>(::strlen(chatting_msg));
    strncpy(msg_brd.m_ptszChat, chatting_msg, _countof(msg_brd.m_ptszChat));
    // NOTE: currently, f110908, msg_brd.m_ptszChat != chatting_msg
    //
    if (chat_room->SendToAll(&msg_brd, msg_brd.GetSize()) == false)
    {
        MSG_CW_CHAT_CHANNEL_SEND_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_CHAT_NOROOM;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=can't send message to chatroom|"));
        return;
    };

    GAMELOG->WriteChannelChat(user, server_session->GetChannelID(),
        chatting_msg, _countof(chatting_msg) - 1);
    SUNLOG(eFULL_LOG, _T("CHAT_CHANNEL_SEND BRD[CharGuid:%u]"), user->GetCharGuid());
}

// 채널채팅방 초대
HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_INVITE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_CHAT_CHANNEL_INVITE_SYN* recv_msg = \
        static_cast<const MSG_CW_CHAT_CHANNEL_INVITE_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };

    // _NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL
    MSG_CW_CHAT_CHANNEL_INVITE_NAK msg_nak;

    BOOST_STATIC_ASSERT(\
        _countof(msg_nak.invite_target_name_) == _countof(recv_msg->m_ptszInviteName));
    strncpy(msg_nak.invite_target_name_, recv_msg->m_ptszInviteName,
            _countof(msg_nak.invite_target_name_));
    msg_nak.invite_target_name_[_countof(msg_nak.invite_target_name_) - 1] = '\0';

    ns_world::WorldUser* const target_user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharName(msg_nak.invite_target_name_);
    if (target_user == NULL)
    {
        MSG_CW_CHAT_CHANNEL_INVITE_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_CHAT_NOCHAR;
        SUNLOG(eFULL_LOG,
               _T("|["__FUNCTION__"]|Msg=this message cannot send to '%s', reason=offline|"),
               msg_nak.invite_target_name_);
        return;
    };
    strncpy(msg_nak.invite_target_name_, target_user->GetCharName(),
            _countof(msg_nak.invite_target_name_));
    msg_nak.invite_target_name_[_countof(msg_nak.invite_target_name_) - 1] = '\0';
    //
    // added by _NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL
    // 초대한 대상이 이미 채널 채팅 중인지 검사한다.
    const ns_world::community::ChatRoomCommunity* const target_chat_room = \
        ns_world::WorldChannelManager::GetLinkedChatRoom(target_user);
    if (target_chat_room != NULL)
    {
        msg_nak.m_byErrorCode = RC::RC_CHAT_ALREADYINROOM;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eFULL_LOG, 
               _T("|%s|초대한 대상이 이미 채널 채팅 중입니다.|신청자 = %s, 대상자 = %s|"), 
               __FUNCTION__, user->GetCharName(), msg_nak.invite_target_name_);
        return;
    };

    // 어떤 방에 초대하는가?
    const ns_world::community::ChatRoomCommunity* const requester_chat_room = \
        ns_world::WorldChannelManager::GetLinkedChatRoom(user);
    if (requester_chat_room == NULL)
    {
        msg_nak.m_byErrorCode = RC::RC_CHAT_NOROOM;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg='%s' is not joined on chat_room|"),
               user->GetCharName());
        return;
    };

    MSG_CW_CHAT_CHANNEL_INVITE_ACK msg_ack;
    MSG_CW_CHAT_CHANNEL_INVITE_CMD msg_cmd;
    { // 초대하는 놈 이름
        BOOST_STATIC_ASSERT(_countof(msg_cmd.m_ptszInviteName) ==
                            _countof(msg_nak.invite_target_name_));
        strncpy(msg_cmd.m_ptszInviteName, user->GetCharName(),
                _countof(msg_cmd.m_ptszInviteName));
        msg_cmd.m_ptszInviteName[_countof(msg_cmd.m_ptszInviteName) - 1] = '\0';
    };
    { // 방제
        BOOST_STATIC_ASSERT(sizeof(msg_cmd.m_ptszRoomName) == sizeof(util::StrArrayForRoomTitle));
        strncpy(msg_cmd.m_ptszRoomName, requester_chat_room->GetChatRoomName(),
                _countof(msg_cmd.m_ptszRoomName));
        msg_cmd.m_ptszRoomName[_countof(msg_cmd.m_ptszRoomName) - 1] = '\0';
        msg_cmd.m_byRoomNameLength = static_cast<uint8_t>(strlen(msg_cmd.m_ptszRoomName) + 1);
    };
    user->SendPacket(&msg_ack, sizeof(msg_ack));
    target_user->SendPacket(&msg_cmd, msg_cmd.GetSize());

    SUNLOG(eFULL_LOG, _T("CHAT_CHANNEL_INVITE SEND[CharGuid:%u][TargetCharGuid:%u]"),
           user->GetCharGuid(), target_user->GetCharGuid());
}

// implemented by _NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL
// 초대 대상자가 채널 초대를 거부하거나 차단했을 때 호출된다.
HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_INVITE_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_CHAT_CHANNEL_INVITE_NAK* recv_msg = \
        static_cast<const MSG_CW_CHAT_CHANNEL_INVITE_NAK*>(msg_base);
    //
    if (sizeof(*recv_msg) != msg_size) {
        return;
    };
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    // 초대 거부 요청이 왔는지 검사한다.
    if (recv_msg->m_byErrorCode != RC::RC_CHAT_DENY_INVITATION && 
        recv_msg->m_byErrorCode != RC::RC_CHANNEL_CHAT_BLOCKED)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|채널 초대 거부나 차단 상태가 아닙니다.|요청자 = %s, 요청코드 = %d|"), 
               __FUNCTION__, user->GetCharName(), recv_msg->m_byErrorCode);
        return;
    };

    MSG_CW_CHAT_CHANNEL_INVITE_NAK relay_msg_nak = *recv_msg;
    relay_msg_nak.invite_target_name_[_countof(relay_msg_nak.invite_target_name_) - 1] = _T('\0');

    // 초대 신청자를 검사한다.
    ns_world::WorldUser* const invite_request_user = 
        ns_world::WorldUserManager::Instance()->FindUserByCharName(relay_msg_nak.invite_target_name_);
    if (invite_request_user == NULL || invite_request_user == user)
    {
        // 초대 신청자를 찾을 수 없다면 응답 대상자가 존재하지 않으므로 여기서 끝낸다.
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|초대 신청자를 찾을 수 없습니다.|요청자 = %s, 신청자 = %s|"), 
               __FUNCTION__,  user->GetCharName(), relay_msg_nak.invite_target_name_);
        return;
    };

    // 초대 대상자를 설정한다.
    ;{
        ::_tcsncpy(relay_msg_nak.invite_target_name_, user->GetCharName(),
                   _countof(relay_msg_nak.invite_target_name_));
        relay_msg_nak.invite_target_name_[
            _countof(relay_msg_nak.invite_target_name_) - 1] = _T('\0');
    };
    // 초대 신청자에게 응답 메시지를 중계한다.
    invite_request_user->SendPacket(&relay_msg_nak, sizeof(relay_msg_nak));

    SUNLOG(eFULL_LOG, 
           _T("|%s|초대 신청자에게 초대 거부나 차단 상태 알림 메시지를 보냈습니다.|신청자 = %s, 대상자 = %s|"), 
           __FUNCTION__, invite_request_user->GetCharName(), user->GetCharName());
}

// 채널채팅방 퇴장 요청
HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_OUT_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_CHAT_CHANNEL_OUT_SYN* recv_msg = \
        static_cast<const MSG_CW_CHAT_CHANNEL_OUT_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    //
    ns_world::WorldChannelManager* const channel_manager = \
        ns_world::WorldChannelManager::Instance();
    channel_manager->LeaveChatRoomCommunity(user);
    //
    SUNLOG(eFULL_LOG, _T("CHAT_CHANNEL_OUT[CharGuid:%u]"), user->GetCharGuid());
}

// 채널채팅방 정보 요청
HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_INFO_REQ, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_CHAT_CHANNEL_INFO_REQ* recv_msg = \
        static_cast<const MSG_CW_CHAT_CHANNEL_INFO_REQ*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    const ns_world::community::ChatRoomCommunity* const chat_room = \
        ns_world::WorldChannelManager::GetLinkedChatRoom(user);
    if (chat_room == NULL)
    {
        MSG_CW_CHAT_CHANNEL_INFO_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_CHAT_NOROOM;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        return;
    };

    // NOTE: f110310.5L, this message processing seems to dangerous...
    MSG_CW_CHAT_CHANNEL_INFO_ANS msg_ack;
#ifdef _DEBUG
    const ns_functor::ChatRoomMemberListFilter::MemberInfoArray& test_chat_room_members = \
        msg_ack.m_ptszCharName; __TOUCHED(test_chat_room_members);
    BOOST_STATIC_ASSERT(_countof(test_chat_room_members) == MAX_CHATZONE_USER);
#endif
    ns_world::WorldChannelManager* const channel_manager = \
        ns_world::WorldChannelManager::Instance();
    if (channel_manager->LoadChatRoomMembers(user,
            msg_ack.m_ptszCharName, &msg_ack.m_byUserNum) == false)
    {
        MSG_CW_CHAT_CHANNEL_INFO_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_CHAT_UNKNOWN;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        return;
    };
    user->SendPacket(&msg_ack, msg_ack.GetSize());

    SUNLOG(eFULL_LOG, _T("ChannelChatZone Info Req[%u][Num:%u]"),
           user->GetCharGuid(), msg_ack.m_byUserNum);
}
