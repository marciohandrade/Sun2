#pragma once
#ifndef WORLDSERVER_HANDLER_CW_H
#define WORLDSERVER_HANDLER_CW_H

#include <PacketStruct_CW.h>

#include "./PacketHandler.h"

namespace ns_world {
;
class ServerSessionInWorld;

}; //end of namespace

//==================================================================================================

namespace ns_world { namespace handlers {
;

class Handler_CW
{
public:
    static const ns_world::PacketHandlerInfo GetPacketHandlerNodes();
    //----------------------------------------------------------------------------------------------
    // internal utility
    static ns_world::WorldUser* last_handled_user;
    // NOTE: f110823.1L, if the server_session field is null,
    // it means process using the session of user
    static bool CheckEnabledChattingState(ns_world::WorldUser* user,
        ns_world::ServerSessionInWorld* server_session);

    #define FIND_USER_FROM_AGENT_MSG(_msg_base_) \
        (((ns_world::handlers::Handler_CW::last_handled_user = \
            ns_world::WorldUserManager::Instance()->FindUser((_msg_base_)->m_dwKey)) != NULL) \
            ? (ns_world::handlers::Handler_CW::last_handled_user) \
            : (FlowControl::FCAssert(!"can't find user"), NULL))
    //----------------------------------------------------------------------------------------------
#undef HANDLER_NODE_INFO
#define HANDLER_NODE_INFO(_category_, _protocol_, phase) \
    static void Handler_CW::On##_protocol_(\
        ns_world::ServerSessionInWorld* const server_session, \
        const MSG_BASE* const msg, const uint16_t msg_size)

    //----------------------------------------------------------------------------------------------
    // Handler_CW.cpp
    //----------------------------------------------------------------------------------------------
    // CHANGES: f110823.1L, removed. do not use anymore
    //HANDLER_NODE_INFO(CW_CONNECTION, CW_HEARTBEAT, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_GM, CW_GM_NOTICE_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_GM, CW_GM_STRING_CMD_SYN, PHASE_SERVICE);
    // CHANGES: f110907.2L, removed, does not use anymore
    //HANDLER_NODE_INFO(CW_VIEWPORT, CW_VIEWPORT_CHARSTATE, PHASE_SERVICE);
    // CHANGES: f110823.1L, removed. do not use anymore
    //HANDLER_NODE_INFO(CW_ARMORKIT, CW_ARMORKIT_FINDHACK_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_COMMUNITY, CW_COMM_USERID_REQ, PHASE_SERVICE);
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_INFO_SYN, PHASE_SERVICE);         // (, 내) 하임 로또 정보 요청
    HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_ENTRY_SYN, PHASE_SERVICE);        // 하임 로또 응모 요청
    HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_NOTICE_LIST_SYN, PHASE_SERVICE);  // 당첨 안내 쪽지 목록 요청
    HANDLER_NODE_INFO(CW_LOTTO, CW_LOTTO_NOTICE_CONFIRM, PHASE_SERVICE);   // 당첨 안내 쪽지 읽음 확인
    //}
    //----------------------------------------------------------------------------------------------
    // Handler_CW.Chatting.cpp
    //----------------------------------------------------------------------------------------------
    HANDLER_NODE_INFO(CW_CHAT, CW_WHISPER_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_CHAT, CW_NOTICE_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_CHAT, CW_WHISPER_SET_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_NORMAL_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_SHOUT_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_PARTY_SYN, PHASE_SERVICE);
    // CHANGES: f110823.1L, removed. do not use anymore
    //HANDLER_NODE_INFO(CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_ACK, PHASE_SERVICE);
    //HANDLER_NODE_INFO(CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_NAK, PHASE_SERVICE);
    // CHANGES: f110907.2L, the voice chatting contents removed. do not use anymore
    //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_REQUEST_SYN, PHASE_SERVICE);
    //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_ACCEPT_SYN, PHASE_SERVICE);
    //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_REJECT_SYN, PHASE_SERVICE);
    //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_DISCONNECT_SYN, PHASE_SERVICE);
    //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_VOICE_MIC_STATUS_SYN, PHASE_SERVICE);
    //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_PARTY_MIC_STATUS_REQ, PHASE_SERVICE);
    //HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_PLAYER_MIC_STATUS_REQ, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_JOIN_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_SEND_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_INVITE_SYN, PHASE_SERVICE);
    //_NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL
    HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_INVITE_NAK, PHASE_SERVICE); // 초대 대상자가 채널 초대를 거부하거나 차단했을 때 호출된다.
    HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_OUT_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_CHAT, CW_CHAT_CHANNEL_INFO_REQ, PHASE_SERVICE);
    //----------------------------------------------------------------------------------------------
    // Handler_CW.Friend.cpp
    //----------------------------------------------------------------------------------------------
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_NEWLIST_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_ADD_SYN, PHASE_SERVICE);
    //HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_OK_SYN, PHASE_SERVICE);
    //HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_NO_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_DEL_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_BLOCK_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_BLOCK_FREE_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_CHAT_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_CHATBLOCK_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_MEMO_UPDATE_SYN, PHASE_SERVICE);   //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_STATUS_REQ, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_FRIEND, CW_FRIEND_STATUS_JOIN_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_REVENGE, CW_REVENGE_DEL_SYN, PHASE_SERVICE);
    //----------------------------------------------------------------------------------------------
    // Handler_CW.Memo.cpp
    //----------------------------------------------------------------------------------------------
    HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_SEND_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_LIST_REQ, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_VIEW_REQ, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_MEMO, CW_MEMO_DELETE_SYN, PHASE_SERVICE);
    //----------------------------------------------------------------------------------------------
    // Handler_CW.Guild.cpp
    //----------------------------------------------------------------------------------------------
    HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_CHAT_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_WAREHOUSE_LOGLIST_REQ, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_SELECT_NOTICE_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_GUILD, CW_GUILD_CHANGE_NOTICE_SYN, PHASE_SERVICE);
    HANDLER_NODE_INFO(CW_GUILD, CW_DOMINATION_RETURN_HEIM_CMD, PHASE_SERVICE);
    //----------------------------------------------------------------------------------------------
    // Handler_CW.Group.cpp
    //----------------------------------------------------------------------------------------------
    // implemented by __KR_001355_20090608_GROUPINGSYSTEM_CW = {
    HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADD_SYN, PHASE_SERVICE);         //그룹 멤버 추가 요청
    HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_GROUPLIST_REQ, PHASE_SERVICE);   //그룹 리스트 요청
    HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADD_RESPONSE_ACK, PHASE_SERVICE);//그룹 멤버 추가 요청 성공 응답
    HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADD_RESPONSE_NAK, PHASE_SERVICE);//그룹 멤버 추가 요청 실패 응답
    HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_STATUS_REQ, PHASE_SERVICE);     //그룹 멤버 상세 정보 요청
    HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_DEL_SYN, PHASE_SERVICE);        //그룹 멤버 삭제 요청
    HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADDBLOCK_SYN, PHASE_SERVICE);   //그룹 맴버 추가 거부 토글 요청
    HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_RESTOR_CMD, PHASE_SERVICE);     //그룹 정보 재요청
    // }; end of '__KR_001355_20090608_GROUPINGSYSTEM_CW'
};

}}; //end of namespace

//==================================================================================================

#endif //WORLDSERVER_HANDLER_CW_H