#include "WorldServerPch.hxx"
#include "./Handler_DW.h"

#include <PacketStruct_AW.h>
#include <PacketStruct_MW.h>

#include "./PacketHandler.h"
#include "WorldServer.h"
#include "Sessions/ServerSessionInWorld.h"
#include "Channel/WorldChannelManager.h"
#include "Channel/WorldChannel.h"
#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
#include "Services/ChattingService.h"
#include "Services/Friends/FriendMap.h"
#include "Services/Friends/FriendInvitation.h"
#include "Services/Group/GroupManager.h"
#include "Services/Lotto/Lotto.h"
#include "Services/Memo/WorldSystemMemo.h"
#include "Services/Guild/WorldGuildManager.h"
#include "Services/Guild/WorldGuild.h"

//==================================================================================================

namespace ns_world { namespace handlers {
;
}}; //end of namespace

const ns_world::PacketHandlerInfo ns_world::handlers::Handler_DW::GetPacketHandlerNodes()
{
#undef HANDLER_NODE_INFO
#undef HANDLER_NODE_INFO_EOR
#define HANDLER_NODE_INFO(_category_, _protocol_, _phase_) \
    { MAKEWORD(_category_, _protocol_), _phase_, &ns_world::handlers::Handler_DW::On##_protocol_ }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const ns_world::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(DW_CONNECTION, DW_CONNECTION_SERVER_INFO_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_GETLIST_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_GETLIST_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_ADD_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_ADD_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DEL_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DEL_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_FREE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_FREE_NAK, PHASE_SERVICE),
        //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DETAIL_INFO_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DETAIL_INFO_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_MEMO_UPDATE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_MEMO_UPDATE_NAK, PHASE_SERVICE),

        //채팅금지/허용
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCK_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCK_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATALLOW_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATALLOW_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCK_UPDATE_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCKTIME_ANS, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCKTIME_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_SEND_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_SEND_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_LIST_ANS, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_LIST_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_VIEW_ANS, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_VIEW_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_DELETE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_DELETE_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_STATUS, DW_STATUS_INTERCEPTION_ANS, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_STATUS, DW_STATUS_INTERCEPTION_NAK, PHASE_SERVICE),
        //__NA001254_20090109_RTTG_FUNC_ADD
        HANDLER_NODE_INFO(DW_MEMO, DW_MULTIMEMO_SEND_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_ADD_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_ADD_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_DEL_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_DEL_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_LIST_ACK, PHASE_SERVICE),
        //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_LOAD_ENTRY_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_LOAD_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_WINNER_LIST_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_SAVE_ENTRY_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_ROLLOVER_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_ADD_TICKET_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_REWARD_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_NOTICE_LIST_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_NOTICE_DEL_ACK, PHASE_SERVICE),
        //}
        HANDLER_NODE_INFO(DW_GUILD, DW_DOMINATION_FINDMONEY_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_ANS, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_SELECT_NOTICE_ACK, PHASE_SERVICE),
        //HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_SELECT_NOTICE_NAK, PHASE_SERVICE),
        //------------------------------------------------------------------------------------------
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_DESTROY_GUILD_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_JOIN_GUILD_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_WITHDRAW_GUILD_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_NOTICE_CMD, PHASE_SERVICE),
    #ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHUNTING_PVP_RESULT_CMD, PHASE_SERVICE),
    #endif
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_DUTY_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_MASTER_CMD, PHASE_SERVICE),

        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_COMMANDER_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_CORPS_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CREATE_REAR_CAMP_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_DESTROY_REAR_CAMP_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CREATE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_RESTORE_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_LOGIN_CHAR_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_LOGOUT_CHAR_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_KICK_PLAYER_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_MEMBER_INFO_REQ_ACK, PHASE_SERVICE),

        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_RECOVERY_GUILD_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_DESTROY_WAIT_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_GUILDMARK_REGISTER_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_GUILDMARK_DELETE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_NAK, PHASE_SERVICE),

        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_NAME_CHANGE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_DOMINATION_RETURNHIME_CMD, PHASE_SERVICE),
        //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK, PHASE_SERVICE),
        //------------------------------------------------------------------------------------------
        //
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
    void ns_world::handlers::Handler_DW::On##_protocol_(\
        ns_world::ServerSessionInWorld* const server_session, \
        const MSG_BASE* const msg_base, const uint16_t msg_size)

#define WAVERIX_WORLD_HANDLER_DEBUGGING() \
    WAVERIX_WRAPPER((WAVERIX_LC_CALL4("waverix_log_trace_dw", \
        __FUNCTION__, server_session, msg_base, msg_size)))

#if SUN_CODE_SAMPLE // to support visual assistx intelli-sense
ns_world::ServerSessionInWorld* server_session;
MSG_BASE* msg_base; uint16_t msg_size;
#endif
//==================================================================================================

HANDLER_NODE_INFO(DW_CONNECTION, DW_CONNECTION_SERVER_INFO_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    SUNLOG(eCRITICAL_LOG, "|["__FUNCTION__"]|");
}

// 친구 목록 수신
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_GETLIST_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_GETLIST_ACK* const recv_msg = \
        static_cast<const MSG_DW_FRIEND_GETLIST_ACK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(recv_msg->m_CharGuid);
    if (bool cant_found = (user == NULL))
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't found user(CharGuid=%d)|"),
               __FUNCTION__, recv_msg->m_CharGuid);
        return;
    };
    const DWORD char_guid = recv_msg->m_CharGuid;
    if (bool mismatched = (user->GetConnectedCharInfo().char_guid != char_guid))
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|[%s]|Msg=can't found user's character(current:reply=%d:%d)|"),
               __FUNCTION__, user->GetConnectedCharInfo().char_guid, char_guid);
        return;
    };
    if (_countof(recv_msg->m_sFriendInfo) < recv_msg->number_of_friends)
    {
        assert(!"index out of ranges");
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=index out of ranges (max:num=%d:%d)|"),
               __FUNCTION__, _countof(recv_msg->m_sFriendInfo), recv_msg->number_of_friends);
        return;
    };

    ns_world::FriendMap* const friend_map = ns_world::FriendMap::Instance();
    friend_map->StoreTotalFriendContentsList(user, \
        recv_msg->m_sFriendInfo, recv_msg->number_of_friends);

    if (user->EnabledSendMessage() == false)
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=target user unlinked, channel=%d|"),
               __FUNCTION__, user->GetUserAccountInfo().channel->GetChannelID());
        return;
    };

    MSG_AW_CHAO_REQUEST_REVENGEINFO_ACK msg_ack;
    msg_ack.m_dwCharGuid = char_guid;
    friend_map->LoadRevengeList(user, \
        msg_ack.m_sRevengeUserInfo, &msg_ack.m_byRevengeStatNum, &msg_ack.m_byRevengePoint);
    //
    user->SendPacket(&msg_ack, msg_ack.GetSize());
    //
    SUNLOG(eCRITICAL_LOG,
           _T("|[%s]|Msg=send to the agent server the '%s''s revenge list, numbers=%d, point=%d|"),
           __FUNCTION__, user->GetCharName(), msg_ack.m_byRevengeStatNum, msg_ack.m_byRevengePoint);
    //_KR_0_20091021_FIRENDINVITATION_EXTEND //에이전트 서버에 친구추천 목록을 전달한다. (W->A->G)
    if (ns_world::FriendInvitationInWorld* event = ns_world::FriendInvitationInWorld::Instance())
    {
        event->NotifyTotalFriendInvitationList(user, event->eMsgForward_ToAgent);
    };
}

// 친구 목록 수신 실패
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_GETLIST_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_GETLIST_NAK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_GETLIST_NAK*>(msg_base);

    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_CharGuid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               recv_msg->m_CharGuid);
        return;
    };

    MSG_CW_FRIEND_NEWLIST_NAK msg_nak;
    msg_nak.dwResult = recv_msg->m_byResult;
    user->SendPacket(&msg_nak, sizeof(msg_nak));
    //
    SUNLOG(eFULL_LOG,
        _T("|["__FUNCTION__"]|Msg=char_name = '%s', user_guid=%d, char_guid=%d, fail_result=%d|"),
        user->GetCharName(), user->GetUserGuid(), user->GetCharGuid(), recv_msg->m_byResult);
}

// 친구 추가 성공
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_ADD_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_ADD_ACK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_ADD_ACK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    STRUCT_FRIEND_INFO friend_info = recv_msg->m_ToFriendInfo;
    STRUCT_FRIEND_PACKET_INFO* const sub_data = &friend_info.sPacketInfo;
    sub_data->ptszFriendName[_countof(sub_data->ptszFriendName) - 1] = _T('\0');

    ns_world::WorldUser* const friend_user = \
        user_manager->FindUserByCharGuid(friend_info.dwFriendGuid);
    // NOTE: 친구가 나를 차단 했을때 경우 추가 2009-11-09 youngmoon 버그번호:5315
    const bool online = (friend_user != NULL) &&
        (ns_world::FriendMap::FindBlockChar(friend_user, requester_char_guid) == NULL);
    sub_data->bOnline = online;
    sub_data->byChannel = 0;
    if (online)
    {
        sub_data->byChannel = static_cast<uint8_t>(\
            friend_user->GetUserAccountInfo().channel->GetChannelID());
#if WORLD_CONTENTS_BLOCK
        // 실제 친구 데이터와 다를 수 있으므로 업데이트
        friend_user->CheckOnlineLevel(recv_msg->m_ToFriendInfo.sPacketInfo.Level);
        friend_user->AddOnlineFriend(user, false); // 아직 실제 접속 상황이 아니므로 2번째 매개변수 false 세팅
#endif
    };

    ns_world::FriendMap* const friend_map = ns_world::FriendMap::Instance();
    if (friend_map->InsertNewFriend(user, friend_info) == false)
    {
        SUNLOG(eCRITICAL_LOG,
            _T("|["__FUNCTION__"]|Msg=can't add to friend list, char_name='%s',%d, friend='%s',%d|"),
            user->GetCharName(), requester_char_guid,
            sub_data->ptszFriendName, friend_info.dwFriendGuid);
        return;
    };

    // 친구 추가됐음을 알려준다
    MSG_CW_FRIEND_ADD_ACK msg_ack;
    msg_ack.sFriendStat = friend_info;
    // NOTE: 대상 유저가 Gm 일경우 자신이 Gm이 아니면 정보를 주지 않는다.
    if ((friend_user != NULL) &&
        (friend_user->IsGMUser()) &&
        (user->IsGMUser() == false))
    {
        STRUCT_FRIEND_PACKET_INFO* const sub_data = &msg_ack.sFriendStat.sPacketInfo;
        sub_data->bOnline = false;
        sub_data->byChannel = 0;
    };
    user->SendPacket(&msg_ack, sizeof(msg_ack));
    //
#if WORLD_CONTENTS_BLOCK
    ns_world::WorldUserManager::Instance()->AddNewFriend(sFriendInfo.dwFriendGuid, user);
    SUNLOG(eFULL_LOG, "[%s->%s] Friend ADD", user->GetCharName(), sFriendInfo.sPacketInfo.ptszFriendName);
#endif
}

// 친구 요청 실패
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_ADD_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_ADD_NAK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_ADD_NAK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    MSG_CW_FRIEND_ADD_NAK msg_nak;
    msg_nak.m_byResult = recv_msg->m_byResult;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "[%s] FRIEND_ADD_NAK", user->GetCharName());
}

// 친구 삭제 성공
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DEL_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_DEL_ACK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_DEL_ACK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));
    //
    MSG_CW_FRIEND_DEL_ACK msg_ack;
    ZeroMemory(msg_ack.ptszDeletedName, sizeof(msg_ack.ptszDeletedName));
    //
    //ns_world::FriendMap* const friend_map = ns_world::FriendMap::Instance();
    const STRUCT_FRIEND_INFO* found_info = \
        ns_world::FriendMap::FindFriend(user, recv_msg->m_CharGuidTo);
    if (found_info == NULL)
    {
        user->SendPacket(&msg_ack, sizeof(msg_ack));
        //
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found friend. ")
            _T("owner=('%s', %d), target=(%d)|"),
            user->GetCharName(), requester_char_guid, recv_msg->m_CharGuidTo);
        return;
    };
    //_KR_0_20091021_FIRENDINVITATION_EXTEND //친구 삭제 성공
    //서로 친구 추천관계일때 친구를 한쪽에서 삭제하면 친구 추천관계가 종료된다.
    ns_world::FriendInvitationInWorld::Instance()->OnFriendRemove(\
        server_session, recv_msg, msg_size);
    ns_world::FriendMap::RemoveFriend(user, found_info->dwFriendGuid);
    //
    _tcsncpy(msg_ack.ptszDeletedName, found_info->sPacketInfo.ptszFriendName,
             _countof(msg_ack.ptszDeletedName));
    msg_ack.ptszDeletedName[_countof(msg_ack.ptszDeletedName) - 1] = _T('\0');
    user->SendPacket(&msg_ack, sizeof(msg_ack));

    SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=owner=('%s', %d), target=('%s', %d)||"),
        user->GetCharName(), requester_char_guid, msg_ack.ptszDeletedName, recv_msg->m_CharGuidTo);
}

// 친구 삭제 실패
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DEL_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_DEL_NAK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_DEL_NAK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    // 알려준다
    MSG_CW_FRIEND_DEL_NAK msg_nak;
    msg_nak.m_byResult = recv_msg->m_byResult;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "[%s][CharGuid:%u] FRIEND_DEL_NAK",
        user->GetCharName(), recv_msg->m_CharGuidFrom);
}

// 차단 성공
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_BLOCK_ACK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_BLOCK_ACK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|")
               _T("addin=blocking char_name='%s'|"),
               requester_char_guid, recv_msg->m_BlockInfo.ptszBlockName);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    STRUCT_BLOCK_INFO block_info = recv_msg->m_BlockInfo;
    block_info.ptszBlockName[_countof(block_info.ptszBlockName) - 1] = _T('\0');

    ns_world::FriendMap* const friend_map = ns_world::FriendMap::Instance();
    if (friend_map->InsertNewBlockChar(user, block_info) == false)
    {
        MSG_CW_FRIEND_BLOCK_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.ERR_DEFAULT;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=('%s') can't add blocking char ('%s')|"),
               user->GetCharName(), block_info.ptszBlockName);
        return;
    };

    // NOTE: 블록된 놈이 혹시나 온라인에 있으면 내 정보를 숨긴다!
#if WORLD_CONTENTS_BLOCK
    ns_world::WorldUser* const blocked_user = \
        user_manager->FindUserByCharGuid(block_info.dwBlockGuid);
    if (blocked_user != NULL)
    {
        user->RemoveOnlineFriend(blocked_user);
    };
#endif

    MSG_CW_FRIEND_BLOCK_ACK msg_ack;
    _tcsncpy(msg_ack.ptszBlockedName, block_info.ptszBlockName, _countof(msg_ack.ptszBlockedName));
    msg_ack.ptszBlockedName[_countof(msg_ack.ptszBlockedName) - 1] = _T('\0');
    user->SendPacket(&msg_ack, sizeof(msg_ack));
    //_KR_0_20091021_FIRENDINVITATION_EXTEND //친구 삭제 성공
    //서로 친구 추천관계일때 친구를 한쪽에서 차단하면 친구 추천관계가 종료된다.
    ns_world::FriendInvitationInWorld::Instance()->OnFriendBlock(\
        server_session, recv_msg, msg_size);

    SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=Msg=('%s') added the blocking char ('%s')"),
           user->GetCharName(), block_info.ptszBlockName);
}

// 차단 실패
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_BLOCK_NAK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_BLOCK_NAK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    MSG_CW_FRIEND_BLOCK_NAK msg_nak;
    msg_nak.m_byResult = recv_msg->m_byResult;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "[%s] FRIEND_BLOCK_NAK", user->GetCharName());
}

// 차단 해제 성공
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_FREE_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_BLOCK_FREE_ACK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_BLOCK_FREE_ACK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    const STRUCT_BLOCK_INFO* block_info = \
        ns_world::FriendMap::FindBlockChar(user, recv_msg->m_CharGuidTo);
    // 차단 정보가 없으면
    if (block_info == NULL)
    {
        MSG_CW_FRIEND_BLOCK_FREE_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.NOT_BLOCKED;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };

    MSG_CW_FRIEND_BLOCK_FREE_ACK msg_ack;
    ;{
        _tcsncpy(msg_ack.ptszBlockedName, block_info->ptszBlockName,
                 _countof(msg_ack.ptszBlockedName));
        msg_ack.ptszBlockedName[_countof(msg_ack.ptszBlockedName) - 1] = _T('\0');
    };

    // 혹시 이놈이 온라인이고 나를 친구로 추가했었다면 로그인 상태로 바꿔준다
    ns_world::WorldUser* blocked_user = user_manager->FindUserByCharGuid(recv_msg->m_CharGuidTo);
    if (bool changed_online_state = \
            (blocked_user && ns_world::FriendMap::FindFriend(blocked_user, requester_char_guid)))
    {
#if WORLD_CONTENTS_BLOCK
        // 로그인 상태로 변신!!
        user->AddOnlineFriend(blocked_user);
#endif
    };

    user->SendPacket(&msg_ack, sizeof(msg_ack));
    //
    ns_world::FriendMap::RemoveBlockChar(user, block_info->dwBlockGuid);
    // state: corrupted = { block_info }
    SUNLOG(eFULL_LOG, _T("[%s->%s] FRIEND_BLOCK_FREE_ACK"),
           user->GetCharName(), msg_ack.ptszBlockedName);
}

// 차단 해제 실패
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_BLOCK_FREE_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_BLOCK_FREE_NAK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_BLOCK_FREE_NAK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    MSG_CW_FRIEND_BLOCK_FREE_NAK msg_nak;
    msg_nak.m_byResult = recv_msg->m_byResult;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "[%s] MSG_DW_FRIEND_BLOCK_FREE_NAK", user->GetCharName());
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DETAIL_INFO_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_DETAIL_INFO_ACK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_DETAIL_INFO_ACK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD receiver_char_guid = recv_msg->m_dwKey;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(receiver_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               receiver_char_guid);
        return;
    };

    MSG_CW_FRIEND_STATUS_ANS msg_ack;
    ns_world::WorldUser* to_user = user_manager->FindUserByCharGuid(recv_msg->m_dwToCharGuid);
    if (to_user != NULL)
    {
        msg_ack.m_byChannel = static_cast<uint8_t>(\
            user->GetUserAccountInfo().channel->GetChannelID());
        const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
        msg_ack.m_byZoneType = char_info.zone_type;
        msg_ack.m_dwZoneKey = char_info.zone_key;
        msg_ack.m_MapCode = char_info.map_code;
    }
    else
    {
        msg_ack.m_byChannel = 0;
        msg_ack.m_byZoneType = eZONETYPE_MAX;
        msg_ack.m_dwZoneKey = 0;
        msg_ack.m_MapCode = 0;
    };

    _tcsncpy(msg_ack.m_ptszCharName, recv_msg->m_ptszToCharName,
             _countof(msg_ack.m_ptszCharName));
    msg_ack.m_ptszCharName[_countof(msg_ack.m_ptszCharName) - 1] = _T('\0');
    _tcsncpy(msg_ack.m_tszGuildName, recv_msg->m_ptszToCharGuildName,
             _countof(msg_ack.m_tszGuildName));
    msg_ack.m_tszGuildName[_countof(msg_ack.m_tszGuildName) - 1] = _T('\0');
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    _tcsncpy(msg_ack.m_ptszMemo, recv_msg->m_ptszMemo, _countof(msg_ack.m_ptszMemo));
    msg_ack.m_ptszMemo[_countof(msg_ack.m_ptszMemo) - 1] = _T('\0');
#endif
    msg_ack.m_byDuty = recv_msg->m_byToCharGuildDuty;

    user->SendPacket(&msg_ack, sizeof(msg_ack));
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_DETAIL_INFO_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_DETAIL_INFO_NAK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_DETAIL_INFO_NAK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_dwKey;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
#if SUN_CODE_BACKUP // WARNING: f110818.1L, sending to user need 'CW' protocol not 'DW' protocol
    MSG_DW_FRIEND_DETAIL_INFO_NAK msg_nak;
    msg_nak.m_byErrorCode = RC::RC_FRIEND_NOTFRIEND;
    user->SendPacket((BYTE*)&msg_nak, sizeof(msg_nak));
#endif
    SUNLOG(eFULL_LOG, "[%s][%d] OnDW_FRIEND_DETAIL_INFO_NAK",
           user->GetCharName(), recv_msg->m_byErrorCode);
}

HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_MEMO_UPDATE_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_MEMO_UPDATE_ACK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_MEMO_UPDATE_ACK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_dwKey;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };

    MSG_CW_FRIEND_MEMO_UPDATE_ACK msg_ack;
    msg_ack.m_dwToCharGuid = recv_msg->m_dwToCharGuid;
    user->SendPacket(&msg_ack, sizeof(msg_ack));
}

HANDLER_NODE_INFO(DW_FRIEND, DW_FRIEND_MEMO_UPDATE_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_FRIEND_MEMO_UPDATE_NAK* recv_msg = \
        static_cast<const MSG_DW_FRIEND_MEMO_UPDATE_NAK*>(msg_base);
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_dwKey;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };

    MSG_CW_FRIEND_MEMO_UPDATE_NAK msg_nak;
    msg_nak.m_byErrorCode = RC::RC_FRIEND_NOTFRIEND;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "[%s][%d] OnDW_FRIEND_MEMO_UPDATE_NAK",
           user->GetCharName(), recv_msg->m_byErrorCode);
}

// 채팅금지
HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCK_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GM_CHATBLOCK_ACK* recv_msg = \
        static_cast<const MSG_DW_GM_CHATBLOCK_ACK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_dwKey;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    const DWORD blocked_char_guid = recv_msg->m_CharGuid;
    const DWORD blocking_timeout = recv_msg->m_dwBlockTime;
    //
    MSG_CW_GM_USERCHAT_BLOCK_ACK msg_ack;
    msg_ack.m_dwKey = requester_char_guid;
    msg_ack.m_CharGuid = blocked_char_guid;
    msg_ack.m_bBlock = true;
    msg_ack.m_dwBlockTime = blocking_timeout;
    user->SendPacket(&msg_ack, sizeof(msg_ack));

    ns_world::WorldUser* blocked_user = user_manager->FindUserByCharGuid(blocked_char_guid);
    if (blocked_user)
    {
        // 실제 금지 세팅
        ns_world::ChattingService::BlockChatting(blocked_user, blocking_timeout);
        //_WHATTHEHELL_BLOCKED_20060517    // 채팅금지 통보 기능 테스트중
        // 금지 사실을 알려준다
        MSG_CW_CHAT_BLOCKED_CMD msg_block;
        msg_block.m_dwRemainTime = blocking_timeout;
        blocked_user->SendPacket(&msg_block, sizeof(msg_block));

        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
               _T("Msg=chatting blocked, gm:{name, char_guid} = {'%s', %d} ")
               _T("blocked char: {name, char_guid} = {'%s', %d}, blocking_time(%d)|"),
               user->GetCharName(), requester_char_guid,
               blocked_user->GetCharName(), blocked_char_guid, blocking_timeout);
    };

    // 채팅 금지 성공
    SUNLOG(eFULL_LOG, "[%u] ChatBlock Success[%u]", recv_msg->m_CharGuid, recv_msg->m_dwBlockTime);
}

HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCK_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GM_CHATBLOCK_NAK* recv_msg = \
        static_cast<const MSG_DW_GM_CHATBLOCK_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_dwKey;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };

    MSG_CW_GM_USERCHAT_BLOCK_NAK msg_nak;
    // TODO: f110818.1L, check the auto alignment user guid in the user->SendPacket method
    msg_nak.m_dwKey = user->GetUserGuid();
    msg_nak.m_wErrorCode = msg_nak._ERR_DEFAULT;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    // 채팅 금지 실패
    SUNLOG(eCRITICAL_LOG, "[%u] ChatBlock Fail[%u]", recv_msg->m_dwKey, recv_msg->m_wErrorCode);
}

// 채팅허용
HANDLER_NODE_INFO(DW_GM, DW_GM_CHATALLOW_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GM_CHATALLOW_ACK* recv_msg = \
        static_cast<const MSG_DW_GM_CHATALLOW_ACK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD allowed_char_guid = recv_msg->m_CharGuid;
    ns_world::WorldUser* allowed_user = user_manager->FindUserByCharGuid(allowed_char_guid);
    if (allowed_user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               allowed_char_guid);
        return;
    };

    ns_world::ChattingService::BlockChatting(allowed_user, 0);
    //
    MSG_CW_GM_USERCHAT_BLOCK_ACK msg_ack;
    //msg_ack.m_dwKey = allowed_user->GetUserGuid();//recv_msg->m_dwKey;
    msg_ack.m_CharGuid = allowed_char_guid; // chat allowed character
    msg_ack.m_bBlock = false;
    msg_ack.m_dwBlockTime = 0;

    allowed_user->SendPacket(&msg_ack, sizeof(msg_ack));

    SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=chatting allowed, ")
           _T("blocked char: {name, char_guid} = {'%s', %d}|"),
           allowed_user->GetCharName(), allowed_char_guid);
}

HANDLER_NODE_INFO(DW_GM, DW_GM_CHATALLOW_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GM_CHATALLOW_NAK* recv_msg = \
        static_cast<const MSG_DW_GM_CHATALLOW_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_dwKey;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };

    MSG_CW_GM_USERCHAT_BLOCK_NAK msg_nak;
    msg_nak.m_dwKey = user->GetUserGuid();
    msg_nak.m_wErrorCode = msg_nak._ERR_DEFAULT;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    // 채팅 허용 실패
    SUNLOG(eCRITICAL_LOG, "[%u] ChatAllow Fail[%u]", recv_msg->m_dwKey, recv_msg->m_wErrorCode);
}

// 차감시간 업데이트
HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCK_UPDATE_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GM_CHATBLOCK_UPDATE_NAK* recv_msg = \
        static_cast<const MSG_DW_GM_CHATBLOCK_UPDATE_NAK*>(msg_base);

    SUNLOG(eFULL_LOG, "[%u]ChatBlock_Update Fail[%u]", recv_msg->m_dwKey, recv_msg->m_wErrorCode);
}

// 채팅금지 시간 요청(처음 접속할 때)
HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCKTIME_ANS, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GM_CHATBLOCKTIME_ANS* recv_msg = \
        static_cast<const MSG_DW_GM_CHATBLOCKTIME_ANS*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD char_guid = recv_msg->m_dwKey;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               char_guid);
        return;
    };
    // if the m_dwRemainTime is 0, the user will enables chatting.
    ns_world::ChattingService::BlockChatting(user, recv_msg->m_dwRemainTime);

    SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|[CharGuid:%u] WorldUser ChatBlock Status Recv[%u]"),
           char_guid, recv_msg->m_dwRemainTime);
}

HANDLER_NODE_INFO(DW_GM, DW_GM_CHATBLOCKTIME_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GM_CHATBLOCKTIME_NAK* recv_msg = \
        static_cast<const MSG_DW_GM_CHATBLOCKTIME_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD char_guid = recv_msg->m_dwKey;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               char_guid);
        return;
    };
    // if the m_dwRemainTime is 0, the user will enables chatting.
    ns_world::ChattingService::BlockChatting(user, 0);
    //
    SUNLOG(eCRITICAL_LOG, "[CharGuid:%u] User ChatBlock Status Recv Failed", char_guid);
}

////////////////////////////
// 리벤지 관련 핸들러
HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_ACK, PHASE_SERVICE)
{   // 리벤지 추가 완료
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_CHAO_ADD_REVENGE_CHAR_ACK* recv_msg = \
        static_cast<const MSG_DW_CHAO_ADD_REVENGE_CHAR_ACK*>(msg_base);
    // m_dwKey : channel id
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    STRUCT_REVENGE_INFO revenge_info = recv_msg->m_sRevengeUserInfo;
    revenge_info.sPacketInfo.bOnline = false;
    //
    const DWORD revenge_char_guid = revenge_info.dwRevengeCharGuid;
    ns_world::WorldUser* revenge_user = user_manager->FindUserByCharGuid(revenge_char_guid);
    if (revenge_user)
    {
        revenge_info.sPacketInfo.bOnline = true;
#if WORLD_CONTENTS_BLOCK
        revenge_user->CheckOnlineLevel(revenge_info.sPacketInfo.Level);
        revenge_user->AddOnlineRevenge(user, false);
#endif
    };
    // 리벤지 포인트 감소
    ns_world::FriendMap::DecreaseRevengePoint(user);
    ns_world::FriendMap* const friend_map = ns_world::FriendMap::Instance();
    if (friend_map->InsertNewRevengeChar(user, revenge_info) == false)
    {
        MSG_AW_CHAO_ADD_REVENGE_CHAR_NAK msg_nak;
        msg_nak.m_dwCharGuid = requester_char_guid;
        msg_nak.m_byErrorCode = RC::RC_REVENGE_FAILED;

        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't add revenge info.|")
               _T("requester:target={'%s',%d:%d}"),
               user->GetCharName(), requester_char_guid, revenge_char_guid);
        return;
    };

    // 복수 대상 추가됐음을 알려준다
    MSG_CW_REVENGE_ADD_CMD msg_cmd;
    msg_cmd.m_byRevengePoint = ns_world::FriendMap::GetRevengePoint(user);

    msg_cmd.m_sRevengeStat = revenge_info;
    STRUCT_REVENGE_PACKET_INFO* const sub_info = &msg_cmd.m_sRevengeStat.sPacketInfo;

    // 대상 유저가 Gm 일경우 자신이 Gm이 아니면 정보를 주지 않는다./
    if ((revenge_user != NULL) &&
        (revenge_user->IsGMUser() != false) &&
        (user->IsGMUser() == false))
    {
        sub_info->bOnline = false;
        sub_info->byChannelID = 0;
    };
    user->SendPacket(&msg_cmd, sizeof(msg_cmd));
#if WORLD_CONTENTS_BLOCK
    ns_world::WorldUserManager::Instance()->AddNewRevenge(revenge_info.dwRevengeCharGuid, user);
#endif
    ;{  // Agent에 결과 전송
        MSG_AW_CHAO_ADD_REVENGE_CHAR_ACK msg_ack;
        msg_ack.m_dwCharGuid = requester_char_guid;
        msg_ack.m_dwRevengeCharGuid = revenge_char_guid;
        BOOST_STATIC_ASSERT(
            sizeof(msg_ack.m_ptszRevengeName) == sizeof(sub_info->ptszRevengeName) &&
            _countof(msg_ack.m_ptszRevengeName) == _countof(sub_info->ptszRevengeName));
        CopyMemory(msg_ack.m_ptszRevengeName, sub_info->ptszRevengeName,
            sizeof(msg_ack.m_ptszRevengeName));

        user->SendPacket(&msg_ack, sizeof(msg_ack));
    };

    SUNLOG(eFULL_LOG, _T("[%s(%u)->%s(%u)] Revenge ADD"),
           user->GetCharName(), requester_char_guid,
           sub_info->ptszRevengeName, revenge_char_guid);
}

HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_ADD_REVENGE_CHAR_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_CHAO_ADD_REVENGE_CHAR_NAK* recv_msg = \
        static_cast<const MSG_DW_CHAO_ADD_REVENGE_CHAR_NAK*>(msg_base);
    //m_dwKey: Agent Server Session...
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    MSG_AW_CHAO_ADD_REVENGE_CHAR_NAK msg_nak;
    msg_nak.m_dwCharGuid = recv_msg->m_CharGuidFrom;
    msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;

    user->SendPacket(&msg_nak, sizeof(msg_nak));
    SUNLOG(eFULL_LOG, "[%u] Revenge ADD NAK[Error:%u]",
           recv_msg->m_CharGuidFrom, recv_msg->m_byErrorCode);
}

// 유저가 리벤지 리스트에서 삭제
HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_CHAO_REMOVE_REVENGE_CHAR_ACK* recv_msg = \
        static_cast<const MSG_DW_CHAO_REMOVE_REVENGE_CHAR_ACK*>(msg_base);
    // m_dwKey : channel id
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
#if SUN_CODE_BACKUP // agent and worldserver will have been the closly coupled.
    ns_world::WorldChannelManager* const channel_manager = \
        ns_world::WorldChannelManager::Instance();

    ns_world::community::WorldChannel* const channel = \
        channel_manager->GetChannel(recv_msg->m_dwKey);
    if (channel == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|"__FUNCTION__"|Msg=can't found targeted channel(%d)|"),
               recv_msg->m_dwKey);
        return;
    };
#endif
    ;{  // Agent에 먼저 전송 - 리스트에서 삭제해야 하기 때문
        MSG_AW_CHAO_REMOVE_REVENGE_CHAR_ACK msg_ack;
        msg_ack.m_dwCharGuid = requester_char_guid;
        msg_ack.m_dwRevengeCharGuid = recv_msg->m_RevengeGuidTo;
        user->SendPacket(&msg_ack, sizeof(msg_ack));
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    MSG_CW_REVENGE_DEL_ACK msg_ack;
    ZeroMemory(msg_ack.m_ptszCharName, sizeof(msg_ack.m_ptszCharName));
    // 온라인 리스트에서 지우고
    const STRUCT_REVENGE_INFO* revenge_info = \
        ns_world::FriendMap::FindRevengeChar(user, recv_msg->m_RevengeGuidTo);
    if (revenge_info == NULL)
    {
        // 이런 경우도 있을까? 아마도 중복 쿼리가 날아가는 정도의 경우에만 발생할 것이다
        // 이런 상황이라고 해도 DB에서는 확실히 지워진 것이므로 ACK 줘야 한다
        user->SendPacket(&msg_ack, sizeof(msg_ack));

        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found targeted revenge chaacter, ")
               _T("requester:target={'%s',%d:%d}|"),
               user->GetCharName(), requester_char_guid, recv_msg->m_RevengeGuidTo);
        return;
    };
#if WORLD_CONTENTS_BLOCK
    ns_world::WorldUserManager::Instance()->RemoveNewRevenge(pRevengeInfo->dwRevengeCharGuid, user);
#endif

    _tcsncpy(msg_ack.m_ptszCharName, revenge_info->sPacketInfo.ptszRevengeName,
             _countof(msg_ack.m_ptszCharName));
    msg_ack.m_ptszCharName[_countof(msg_ack.m_ptszCharName) - 1] = _T('\0');
    user->SendPacket(&msg_ack, sizeof(msg_ack));
    //
    ns_world::FriendMap::RemoveRevengeChar(user, recv_msg->m_RevengeGuidTo);
    //
    SUNLOG(eFULL_LOG, "[%s/%u->%s] REVENGE_DEL_ACK", user->GetCharName(), recv_msg->m_CharGuidFrom, 
           revenge_info->sPacketInfo.ptszRevengeName);
}

HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_REMOVE_REVENGE_CHAR_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_CHAO_REMOVE_REVENGE_CHAR_NAK* recv_msg = \
        static_cast<const MSG_DW_CHAO_REMOVE_REVENGE_CHAR_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    MSG_CW_REVENGE_DEL_NAK msg_nak;
    msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "[%s][CharGuid:%u] REVENGE_DEL_NAK",
           user->GetCharName(), recv_msg->m_CharGuidFrom);
}

// PK를 통해 Agent에서 리스트 삭제 요청
HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_CHAO_PK_REVENGE_CHAR_ACK* recv_msg = \
        static_cast<const MSG_DW_CHAO_PK_REVENGE_CHAR_ACK*>(msg_base);
    // m_dwKey == agent channel index

    ns_world::WorldChannelManager* const channel_manager = \
        ns_world::WorldChannelManager::Instance();

    ns_world::community::WorldChannel* const channel = \
        channel_manager->FindChannel(recv_msg->m_dwKey);
    if (channel == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|"__FUNCTION__"|Msg=can't found targeted channel(%d)|"),
               recv_msg->m_dwKey);
        return;
    };

    MSG_AW_CHAO_REMOVE_REVENGE_CHAR_NAK msg_nak;

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        msg_nak.m_dwCharGuid = requester_char_guid;
        msg_nak.m_byErrorCode = RC::RC_REVENGE_NO_CHAR;
        channel->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u), #%d|"),
               requester_char_guid, 1);
        return;
    };

    if (!FlowControl::FCAssert(user->EnabledSendMessage())) {
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));

    const STRUCT_REVENGE_INFO* revenge_info = \
        ns_world::FriendMap::FindRevengeChar(user, recv_msg->m_RevengeGuidTo);
    if (revenge_info == NULL)
    {
        msg_nak.m_dwCharGuid = requester_char_guid;
        msg_nak.m_byErrorCode = RC::RC_REVENGE_INVALID_CHAR;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eCRITICAL_LOG,
            _T("|["__FUNCTION__"]|Msg=can't found target character(CharGuid=%u), #%d|"),
            recv_msg->m_RevengeGuidTo, 2);
        return;
    };

    ;{
        MSG_AW_CHAO_REMOVE_REVENGE_CHAR_ACK msg_ack;
        msg_ack.m_dwCharGuid = requester_char_guid;
        msg_ack.m_dwRevengeCharGuid = recv_msg->m_RevengeGuidTo;
        user->SendPacket(&msg_ack, sizeof(msg_ack));
    };
    ;{  // 유저에게도 알려준다(리스트에서 지워야 하기 때문)
        MSG_CW_REVENGE_DEL_ACK msg_ack;
        _tcsncpy(msg_ack.m_ptszCharName, revenge_info->sPacketInfo.ptszRevengeName,
                 _countof(msg_ack.m_ptszCharName));
        msg_ack.m_ptszCharName[_countof(msg_ack.m_ptszCharName) - 1] = _T('\0');
        user->SendPacket(&msg_ack, sizeof(msg_ack));
    };

    SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=WorldChannel:%u, CharGuid:%u, Target:%u]|"),
           recv_msg->m_dwKey, requester_char_guid, recv_msg->m_RevengeGuidTo);
}

// NOTE: f110818.1L, same 'MSG_DW_CHAO_PK_REVENGE_CHAR_NAK' and 'MSG_DW_CHAO_REMOVE_REVENGE_CHAR_NAK'
HANDLER_NODE_INFO(DW_CHAO, DW_CHAO_PK_REVENGE_CHAR_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_CHAO_PK_REVENGE_CHAR_NAK* recv_msg = \
        static_cast<const MSG_DW_CHAO_PK_REVENGE_CHAR_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuidFrom;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::FriendMap::EndFriendTransaction(user));
    // NOTE: f110818.1L, think about the not found user and reply to agent...

    MSG_AW_CHAO_REMOVE_REVENGE_CHAR_NAK msg_nak;
    msg_nak.m_dwCharGuid = recv_msg->m_CharGuidFrom;
    msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;
    user->SendPacket(&msg_nak, sizeof(msg_nak));
    //
    SUNLOG(eFULL_LOG, "|["__FUNCTION__"]|[channel:%u][CharGuid:%u][Error:%u]",
           recv_msg->m_dwKey, recv_msg->m_CharGuidFrom, 
           recv_msg->m_byErrorCode);
}

// 쪽지 관련
HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_SEND_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_MEMO_SEND_ACK* recv_msg = \
        static_cast<const MSG_DW_MEMO_SEND_ACK*>(msg_base);
    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };

    const TCHAR* sender_name = _T("System"); // 시스템 쪽지의 디폴트 송신자 이름
    TCHAR receiver_name[_countof(recv_msg->m_ptszRecvName)];
    _tcsncpy(receiver_name, recv_msg->m_ptszRecvName, _countof(receiver_name));
    receiver_name[_countof(receiver_name) - 1] = _T('\0');
    //
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    //
    if (recv_msg->m_SendGuid != 0)
    {   // 시스템 쪽지가 아니면
        ns_world::WorldUser* sender = user_manager->FindUserByCharGuid(recv_msg->m_SendGuid);
        if (sender == NULL)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u), #%d|"),
                   recv_msg->m_SendGuid, 1);
            return;
        };
        sender_name = sender->GetCharName();
        FlowControl::FCAssert(ns_world::WorldSystemMemo::EndMemoTransaction(sender));

        MSG_CW_MEMO_SEND_ACK msg_ack;
        _tcsncpy(msg_ack.m_ptszRecvName, receiver_name, _countof(msg_ack.m_ptszRecvName));
        msg_ack.m_ptszRecvName[_countof(msg_ack.m_ptszRecvName) - 1] = _T('\0');
        sender->SendPacket(&msg_ack, sizeof(msg_ack));
    }
    else
    {   // 시스템 쪽지이면 타입에 따라 송신자 이름을 다르게 설정할 수 있다
        std::string s;
        s.assign(recv_msg->m_Subject.m_ptszSubject);
        std::string::size_type pos = s.find('|');
        if (pos != std::string::npos)
        {
            std::string t = s.substr(0, pos);
            eSYSTEM_MEMO_TYPE type = static_cast<eSYSTEM_MEMO_TYPE>(atoi(t.c_str()));
            sender_name = ns_world::WorldSystemMemo::GetSystemMemoStringByType(type);
            // changed logic to support _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
            // 시스템 쪽지 발송 로그
            WorldUser* recipient = user_manager->FindUserByCharGuid(recv_msg->m_RecvGuid);

            TCHAR additional_info[MAX_ADDINFO_SIZE + 1];
            _sntprintf(additional_info, _countof(additional_info), _T("%I64u %s"),
                recv_msg->m_Subject.m_MemoGuid, recv_msg->m_Subject.m_ptszSubject);
            additional_info[_countof(additional_info) - 1] = _T('\0');
            GAMELOG->LogAction(ACT_SYSTEM_MEMO_RECEIVE, recipient, receiver_name, additional_info);

            SUNLOG(eMIDDLE_LOG, "[Memo] Sent system memo to character '%s' : %s", 
                   receiver_name, additional_info);
        }
    }

    WorldUser* receiver = user_manager->FindUserByCharGuid(recv_msg->m_RecvGuid);

    if (receiver)
    {
        MSG_CW_MEMO_NEWMEMO_BRD msg_brd;
        // CHANGES: f110819.4L, fixed the received message modification not sending message.
        msg_brd.m_Subject = recv_msg->m_Subject;
        _tcsncpy(msg_brd.m_Subject.m_SendName, sender_name,
                 _countof(msg_brd.m_Subject.m_SendName));
        msg_brd.m_Subject.m_SendName[_countof(msg_brd.m_Subject.m_SendName) - 1] = _T('\0');

        receiver->SendPacket(&msg_brd, sizeof(msg_brd));
    }

    SUNLOG(eFULL_LOG, _T("DW_MEMO_SEND_ACK[%s][%u]->[%s]"),
           sender_name, recv_msg->m_SendGuid, receiver_name);
}

HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_SEND_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_MEMO_SEND_NAK* recv_msg = \
        static_cast<const MSG_DW_MEMO_SEND_NAK*>(msg_base);
    // if the 'm_SendGuid' is 0, the requested message is the system memo
    if (recv_msg->m_SendGuid == 0) {
        return;
    };

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD sender_char_guid = recv_msg->m_SendGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(sender_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               sender_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::WorldSystemMemo::EndMemoTransaction(user));

    MSG_CW_MEMO_SEND_NAK msg_nak;
    // 20061124_YARU_쪽지 에러코드 추가에 따른 수정
    if (recv_msg->m_byErrorCode == 2) {
        msg_nak.m_byErrorCode = RC::RC_MEMO_NOUSER;
    }
    else {
        msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;
    };
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "DW_MEMO_SEND_NAK[%s][%u][Error:%u]",
           user->GetCharName(), user->GetCharGuid(), msg_nak.m_byErrorCode);
}

HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_LIST_ANS, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_MEMO_LIST_ANS* recv_msg = \
        static_cast<const MSG_DW_MEMO_LIST_ANS*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD receiver_char_guid = recv_msg->m_RecvGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(receiver_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               receiver_char_guid);
        return;
    };

    MSG_CW_MEMO_LIST_ANS msg_ack;
    msg_ack.m_byMemoCnt = recv_msg->m_byMemoCnt;
    msg_ack.m_byMemoPage = recv_msg->m_byMemoPage;
    msg_ack.m_byMemoPageCnt = recv_msg->m_byMemoPageCnt;
    if (FlowControl::FCAssert(_countof(msg_ack.m_Subject) >= msg_ack.m_byMemoCnt) == false) {
        msg_ack.m_byMemoCnt = _countof(msg_ack.m_Subject);
    };
    // TODO: f110818.1L, type check
    BOOST_STATIC_ASSERT(sizeof(msg_ack.m_Subject) == sizeof(recv_msg->m_Memo) &&
                        _countof(msg_ack.m_Subject) == _countof(recv_msg->m_Memo) &&
                        sizeof(msg_ack.m_Subject[0]) == sizeof(recv_msg->m_Memo[0]));
    CopyMemory(msg_ack.m_Subject, recv_msg->m_Memo,
        sizeof(msg_ack.m_Subject[0]) * msg_ack.m_byMemoCnt);

    user->SendPacket(&msg_ack, msg_ack.GetSize());
    if (msg_ack.m_byMemoPage >= msg_ack.m_byMemoPageCnt) {
        FlowControl::FCAssert(ns_world::WorldSystemMemo::EndMemoTransaction(user));
    }
    SUNLOG(eFULL_LOG, "DW_MEMO_LIST_ANS[%s][%u][Memo:%u][Page:%u/%u]",
        user->GetCharName(),
        user->GetCharGuid(),
        msg_ack.m_byMemoCnt,
        msg_ack.m_byMemoPage,
        msg_ack.m_byMemoPageCnt);
}

HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_LIST_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_MEMO_LIST_NAK* recv_msg = \
        static_cast<const MSG_DW_MEMO_LIST_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD receiver_char_guid = recv_msg->m_RecvGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(receiver_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               receiver_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::WorldSystemMemo::EndMemoTransaction(user));

    MSG_CW_MEMO_LIST_NAK msg_nak;
    msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "DW_MEMO_LIST_NAK[%s][%u]", user->GetCharName(), user->GetCharGuid());
}

HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_VIEW_ANS, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_MEMO_VIEW_ANS* recv_msg = \
        static_cast<const MSG_DW_MEMO_VIEW_ANS*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD receiver_char_guid = recv_msg->m_RecvGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(receiver_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               receiver_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::WorldSystemMemo::EndMemoTransaction(user));

    MSG_CW_MEMO_VIEW_ANS msg_ack;
    msg_ack.m_MemoGuid = recv_msg->m_MemoGuid;
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    msg_ack.m_typeSystemMemo = recv_msg->m_typeSystemMemo;
    msg_ack.m_recvItemCode = recv_msg->m_recvItemCode;
    msg_ack.m_amountCost = recv_msg->m_amountCost;
    //}
    _tcsncpy(msg_ack.m_ptszMemo, recv_msg->m_ptszMemo, _countof(msg_ack.m_ptszMemo));
    // CHANGES: f110818.1L, changed to null terminated string
    msg_ack.m_ptszMemo[_countof(msg_ack.m_ptszMemo) - 1] = _T('\0');
    user->SendPacket(&msg_ack, sizeof(msg_ack)); // TODO: f110818.1L, think the 'GetSize' usage

    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    if (recv_msg->m_typeSystemMemo != NORMAL_MEMO)
    {
        SUNLOG(eMIDDLE_LOG, _T("[Memo] Read system memo %I64u of character '%s' : ")
               _T("Code=%d Amount=%I64u"), 
               recv_msg->m_MemoGuid,
               user->GetCharName(), msg_ack.m_recvItemCode, msg_ack.m_amountCost);
    };
    //}
    SUNLOG(eFULL_LOG, "DW_MEMO_VIEW_ANS[%s][%u][Memo:%u]",
           user->GetCharName(), user->GetCharGuid(), msg_ack.m_MemoGuid);
}

HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_VIEW_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_MEMO_VIEW_NAK* recv_msg = \
        static_cast<const MSG_DW_MEMO_VIEW_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD receiver_char_guid = recv_msg->m_RecvGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(receiver_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               receiver_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::WorldSystemMemo::EndMemoTransaction(user));

    MSG_DW_MEMO_VIEW_NAK msg_nak;
    msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "DW_MEMO_VIEW_NAK[%s][%u]", user->GetCharName(), user->GetCharGuid());
}

HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_DELETE_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_MEMO_DELETE_ACK* recv_msg = \
        static_cast<const MSG_DW_MEMO_DELETE_ACK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD receiver_char_guid = recv_msg->m_RecvGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(receiver_char_guid);

    // added by _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    int log_type = 0;
    if (recv_msg->m_MemoGuid == 0)
    {
        log_type = ACT_ALL_MEMO_DELETE; // 전체 쪽지 일괄 삭제
        SUNLOG(eMIDDLE_LOG, _T("[Memo] Deleted all memo of character %d"),
               recv_msg->m_RecvGuid);
    }
    else if (recv_msg->m_MemoType ==  STRUCT_SUBJECT_PACKETINFO::SYSTEM_MEMO)
    {
        log_type = ACT_SYSTEM_MEMO_DELETE; // 시스템 쪽지 삭제
        SUNLOG(eMIDDLE_LOG, _T("[Memo] Deleted system memo %I64u of character %d"),
               recv_msg->m_MemoGuid, recv_msg->m_RecvGuid);
    }
    if (log_type != 0 && user)
    {
        // 쪽지 삭제 로그
        TCHAR additional_info[MAX_ADDINFO_SIZE + 1];
        _sntprintf(additional_info, _countof(additional_info), _T("%I64u CharID:%d"),
                   recv_msg->m_MemoGuid, recv_msg->m_RecvGuid);
        additional_info[_countof(additional_info) - 1] = _T('\0');
        GAMELOG->LogAction(log_type, user, NULL, additional_info);
    };

    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               receiver_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::WorldSystemMemo::EndMemoTransaction(user));

    MSG_CW_MEMO_DELETE_ACK msg_ack;
    msg_ack.m_MemoGuid = recv_msg->m_MemoGuid;
    user->SendPacket(&msg_ack, sizeof(msg_ack));

    SUNLOG(eFULL_LOG, _T("DW_MEMO_DELETE_ACK[%s][%u][Memo:%u]"),
           user->GetCharName(), user->GetCharGuid(), msg_ack.m_MemoGuid);
}

HANDLER_NODE_INFO(DW_MEMO, DW_MEMO_DELETE_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_MEMO_DELETE_NAK* recv_msg = \
        static_cast<const MSG_DW_MEMO_DELETE_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD receiver_char_guid = recv_msg->m_RecvGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(receiver_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               receiver_char_guid);
        return;
    };
    FlowControl::FCAssert(ns_world::WorldSystemMemo::EndMemoTransaction(user));

    MSG_CW_MEMO_DELETE_NAK msg_nak;
    msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eFULL_LOG, "DW_MEMO_DELETE_NAK[%s][%u][Error:%u]",
           user->GetCharName(), user->GetCharGuid(), msg_nak.m_byErrorCode);
}

HANDLER_NODE_INFO(DW_STATUS, DW_STATUS_INTERCEPTION_ANS, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_STATUS_INTERCEPTION_ANS* recv_msg = \
        static_cast<const MSG_DW_STATUS_INTERCEPTION_ANS*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };

    // NOTE: gameserver와 연결 후 이곳에서 gameserver의 정보를 요청하자
    ns_world::ChattingService::EnableWhisperingIntercepted(user, (recv_msg->m_bWhisperFlag != 0));
}

HANDLER_NODE_INFO(DW_STATUS, DW_STATUS_INTERCEPTION_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_STATUS_INTERCEPTION_NAK* recv_msg = \
        static_cast<const MSG_DW_STATUS_INTERCEPTION_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    SUNLOG(eFULL_LOG, "DW_STATUS_INTERCEPTION_NAK_[CHARGUID:%u]", recv_msg->m_CharGuid);
}

//{__NA001254_20090109_RTTG_FUNC_ADD
// 멀티 쪽지
HANDLER_NODE_INFO(DW_MEMO, DW_MULTIMEMO_SEND_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_MULTIMEMO_SEND_ACK* recv_msg = \
        static_cast<const MSG_DW_MULTIMEMO_SEND_ACK*>(msg_base);

    ns_world::ServerSessionInWorld* master_server = \
        ns_world::WorldServer::Instance()->GetMasterServer();

    const int upperbound = min(_countof(recv_msg->m_arrResultInfoList), recv_msg->m_dwCount);
    ASSERT(upperbound == recv_msg->m_dwCount);
    if (master_server != NULL)
    {
        MSG_MW_MULTIMEMO_SEND_ACK msg_ack;
        msg_ack.m_dwKey = recv_msg->m_dwKey;
        msg_ack.m_dwCount = recv_msg->m_dwCount;
        BOOST_STATIC_ASSERT(_countof(msg_ack.m_arrResultInfoList) ==
                            _countof(recv_msg->m_arrResultInfoList));
        for (int index = 0; index < upperbound; ++index)
        {
            msg_ack.m_arrResultInfoList[index] = \
                recv_msg->m_arrResultInfoList[index].m_ErrResult;
        };

        master_server->SendPacket(&msg_ack, sizeof(msg_ack));
    };

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    for (int index = 0; index < upperbound; ++index)
    {
        const MMEMO_RESULT& multi_memo_result = recv_msg->m_arrResultInfoList[index];
        ns_world::WorldUser* target_user = \
            user_manager->FindUserByCharGuid(multi_memo_result.m_RecvGuid);
        if (target_user == NULL) {
            continue;
        };

        MSG_CW_MEMO_NEWMEMO_BRD msg_brd;
        STRUCT_SUBJECT_PACKETINFO* const sub_info = &msg_brd.m_Subject;
        sub_info->m_byRead = 0;
        sub_info->m_MemoGuid = multi_memo_result.m_MemoGuid;
        sub_info->m_MemoType = sub_info->NORMAL_MEMO;

        _tcsncpy(sub_info->m_SendName, recv_msg->m_SendName, _countof(sub_info->m_SendName));
        sub_info->m_SendName[_countof(sub_info->m_SendName) - 1] = _T('\0');

        _tcsncpy(sub_info->m_tszCreateDate, multi_memo_result.m_tszCreateDate,
                 _countof(sub_info->m_tszCreateDate));
        sub_info->m_tszCreateDate[_countof(sub_info->m_tszCreateDate) - 1] = _T('\0');

        _tcsncpy(sub_info->m_ptszSubject, multi_memo_result.m_ptszSubject,
                 _countof(sub_info->m_ptszSubject));
        sub_info->m_ptszSubject[_countof(sub_info->m_ptszSubject) - 1] = _T('\0');

        target_user->SendPacket(&msg_brd, sizeof(msg_brd));
    };
}
//}__NA001254_20090109_RTTG_FUNC_ADD

//////////////////////////////////////////////////////////////////////////
//그룹 맺기 시스템
//////////////////////////////////////////////////////////////////////////
//그룹 맴버 추가 성공 패킷 처리 : 상위 멤버의 그룹 정보에 하위 맴버를 추가하고 상하위 멤버에게 정보를 알린다.
HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_ADD_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GROUP_ADD_ACK* recv_msg = \
        static_cast<const MSG_DW_GROUP_ADD_ACK*>(msg_base);

    // NOTE: old comments, (상위 멤버에게 하위 멤버의 정보를), (하위 멤버에게 상위 멤버의 정보를) 알려준다.
    // (맴버 추가 요청시 저장했던 m_tempMemberInfo 사용)
    // [2010.02.26]gameluluWorking : m_tempMemberInfo내용에 대한 신뢰도에 대한 문제 해결 강구
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD from_char_guid = recv_msg->m_fromCharGUID;
    ns_world::WorldUser* from_user = user_manager->FindUserByCharGuid(from_char_guid);
    const DWORD to_char_guid = recv_msg->m_toCharGUID;
    ns_world::WorldUser* to_user = user_manager->FindUserByCharGuid(to_char_guid);

    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    if (from_user != NULL) {
        FlowControl::FCAssert(group_manager->EndGroupMemberAdditionTransaction(\
            from_user, true, (to_user != NULL), GROUP_LEVEL_LOW));
    };
    if (to_user != NULL) {
        FlowControl::FCAssert(group_manager->EndGroupMemberAdditionTransaction(\
            to_user, true, (from_user != NULL), GROUP_LEVEL_HIGH));
    };
}

//그룹 맴버 추가 실패 패킷 처리
HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_ADD_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GROUP_ADD_NAK* recv_msg = \
        static_cast<const MSG_DW_GROUP_ADD_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD from_char_guid = recv_msg->m_fromCharGUID;
    ns_world::WorldUser* from_user = user_manager->FindUserByCharGuid(from_char_guid);
    const DWORD to_char_guid = recv_msg->m_toCharGUID;
    ns_world::WorldUser* to_user = user_manager->FindUserByCharGuid(to_char_guid);

    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    if (from_user != NULL) {
        FlowControl::FCAssert(group_manager->EndGroupMemberAdditionTransaction(\
            from_user, false, (to_user != NULL), GROUP_LEVEL_LOW));
    };
    if (to_user != NULL) {
        FlowControl::FCAssert(group_manager->EndGroupMemberAdditionTransaction(\
            to_user, false, (from_user != NULL), GROUP_LEVEL_HIGH));
    };

    MSG_CW_GROUP_ADD_NAK msg_nak;
    msg_nak.m_byResult = msg_nak.ERR_DEFAULT;
    from_user->SendPacket(&msg_nak, sizeof(msg_nak));

    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][%d]DB InsertFail", from_char_guid);
}

//그룹 맴버 삭제 성공 패킷 처리
HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_DEL_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GROUP_DEL_ACK* recv_msg = \
        static_cast<const MSG_DW_GROUP_DEL_ACK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD from_char_guid = recv_msg->m_fromCharGUID;
    const DWORD to_char_guid = recv_msg->m_toCharGUID;
    ns_world::WorldUser* from_user = user_manager->FindUserByCharGuid(from_char_guid);
    ns_world::WorldUser* to_user = user_manager->FindUserByCharGuid(to_char_guid);
    //
    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    //
    if (from_user)
    {
        // NOTE: 요청 유저에게 삭제 결과를 알림,
        // 삭제하기 전에 맴버 정보를 이용하여 요청 유저에게 삭제결과를 알림
        const STRUCT_GROUP_MEMBER_INFO* member_info = \
            group_manager->FindGroupMember(from_user, to_char_guid);
        if (member_info != NULL)
        {
            MSG_CW_GROUP_DEL_ACK msg_ack;
            _tcsncpy(msg_ack.ptszMemeberName, member_info->GroupMemberInfo.ptszMemberName,
                     _countof(msg_ack.ptszMemeberName));
            msg_ack.ptszMemeberName[_countof(msg_ack.ptszMemeberName) - 1] = _T('\0');
            from_user->SendPacket(&msg_ack, sizeof(msg_ack));
            group_manager->RemoveMemberInfo(from_user, to_char_guid);
        }
        else
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|Msg=can't found 'to' member (from=%d, to=%d)"),
                   from_char_guid, to_char_guid);
        };
        group_manager->UpdateGroupInfo(from_user);
    };
    //
    if (to_user)
    {
        // NOTE: 삭제된 유저에게 삭제 사실을 알림
        const STRUCT_GROUP_MEMBER_INFO* member_info = \
            group_manager->FindGroupMember(to_user, from_char_guid);
        if (member_info != NULL)
        {
            MSG_CW_GROUP_DEL_BRD msg_brd;
            _tcsncpy(msg_brd.ptszMemeberName, member_info->GroupMemberInfo.ptszMemberName,
                     _countof(msg_brd.ptszMemeberName));
            msg_brd.ptszMemeberName[_countof(msg_brd.ptszMemeberName) - 1] = _T('\0');

            to_user->SendPacket(&msg_brd, sizeof(msg_brd));
            group_manager->RemoveMemberInfo(to_user, from_char_guid);
        }
        else
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|Msg=can't found 'from member (from=%d, to=%d)"),
                   from_char_guid, to_char_guid);
        };
        group_manager->UpdateGroupInfo(to_user);
    };
}

//그룹 맴버 삭제 실패 패킷 처리
HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_DEL_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GROUP_DEL_NAK* recv_msg = \
        static_cast<const MSG_DW_GROUP_DEL_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD from_char_guid = recv_msg->m_fromCharGUID;
    const DWORD to_char_guid = recv_msg->m_toCharGUID;
    ns_world::WorldUser* from_user = user_manager->FindUserByCharGuid(from_char_guid);
    ns_world::WorldUser* to_user = user_manager->FindUserByCharGuid(to_char_guid);

    if (from_user) {
        FlowControl::FCAssert(ns_world::GroupManager::EndGroupTransaction(from_user));
    };
    if (to_user) { // TODO: check whether this control is valid.
        FlowControl::FCAssert(ns_world::GroupManager::EndGroupTransaction(to_user));
    };

    if (from_user == NULL) {
        return;
    };

    MSG_CW_GROUP_DEL_NAK msg_nak;
    ;{
        msg_nak.m_byResult = recv_msg->m_byResult;
        //                               요청한 케릭터 이름 (...why?)
        _tcsncpy(msg_nak.ptszMemeberName, from_user->GetCharName(),
                 _countof(msg_nak.ptszMemeberName));
        msg_nak.ptszMemeberName[_countof(msg_nak.ptszMemeberName) - 1] = _T('\0');
    };
    from_user->SendPacket(&msg_nak, sizeof(msg_nak));
}

//그룹 맴버 리스트 요청 성공 패킷 처리
HANDLER_NODE_INFO(DW_GROUP, DW_GROUP_LIST_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GROUP_LIST_ACK* recv_msg = \
        static_cast<const MSG_DW_GROUP_LIST_ACK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_CharGUID;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };
    if (recv_msg->GetSize() != msg_size)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=invalid message specification, ")
               _T("char_guid=%d, number_of_nodes=%d, size comparision(%d:%d)|"),
               requester_char_guid, recv_msg->m_DataNum, recv_msg->GetSize(), msg_size);
        return;
    };

    // DONE: f110923.1L, CHANGES: f100425.3L, waverix, prevent buffer overrun process
    BOOST_STATIC_ASSERT(_countof(recv_msg->m_GroupInfo) == MAX_GROUP_MEMBER);
    const bool failed__number_of_node_has_index_out_of_range = \
        (recv_msg->m_DataNum > MAX_GROUP_MEMBER);
    assert(failed__number_of_node_has_index_out_of_range == false);

    const int number_of_node = min(_countof(recv_msg->m_GroupInfo), recv_msg->m_DataNum);
    if (failed__number_of_node_has_index_out_of_range)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=index out of range, char_guid=%d, number_of_nodes=%d|"),
               requester_char_guid, recv_msg->m_DataNum);
    };

    //-- 맴버 정보 셋팅
    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    //group_manager->InitializeGroupMemberTable(user);
    //
    const STRUCT_GROUP_MEMBER_INFO* member_it = recv_msg->m_GroupInfo,
        * const member_end = &recv_msg->m_GroupInfo[number_of_node];
    for (; member_it != member_end; ++member_it)
    {
        STRUCT_GROUP_MEMBER_INFO member_info = *member_it;
        ns_world::WorldUser* group_member = user_manager->FindUserByCharGuid(member_it->charGUID);

        STRUCT_GROUP_MEMBER_PACKET_INFO* const sub_data = &member_info.GroupMemberInfo;
        if (group_member != NULL)
        {
            sub_data->bOnline = true;
            sub_data->byChannel = static_cast<uint8_t>(\
                group_member->GetUserAccountInfo().channel->GetChannelID());
        }
        else
        {
            sub_data->bOnline = false;
            sub_data->byChannel = 0;
        };

        if (group_manager->IsOverBonusDate(member_info.lastDate, MAX_DATE_GROUPBONUS))
        {
            group_manager->AddAccumulatedExp(user, member_info.accumEXP);
            SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not apply accumexp because date is over");
        };

        group_manager->AddMemberInfo(user, member_info);
    };

    group_manager->UpdateGroupInfo(user);
    if (user->EnabledSendMessage() == NULL)
    {
        SUNLOG(eFULL_LOG, _T("|["__FUNCTION__"]|Msg=Can not found agentserver"));
        return;
    };
    group_manager->SendAroundMember(user, GroupManager::LOG_IN);
    //
    if (const EXPTYPE accumulated_exp = group_manager->GetAccumulatedExp(user))
    {
        // 축적 경험치 획득을 알림
        MSG_AW_STATUS_GROUP_ADDEXP_CMD msg_cmd;
        ;{
            msg_cmd.m_CharGuid = requester_char_guid;
            msg_cmd.m_Amount = accumulated_exp;
        };
        user->SendPacket(&msg_cmd, sizeof(msg_cmd));

        group_manager->SendAccumEXPMemo(user);
    };
    ;{
        // 축적 경험치를 초기화 시킨다.
        MSG_DW_GROUP_INIT_ACCUMEXP_CMD msg_cmd;
        msg_cmd.m_CharGUID = requester_char_guid;
        server_session->SendPacket(&msg_cmd, sizeof(msg_cmd));
    };
}

HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_LOAD_ENTRY_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_LOTTO_LOAD_ENTRY_ACK* recv_msg = \
        static_cast<const MSG_DW_LOTTO_LOAD_ENTRY_ACK*>(msg_base);

    if (FlowControl::FCAssert(recv_msg->GetSize() == msg_size) == false) {
        return;
    };

    Lotto::GetInstance().OnEntryLoad(recv_msg);
}

HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_LOAD_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_LOTTO_LOAD_ACK* recv_msg = \
        static_cast<const MSG_DW_LOTTO_LOAD_ACK*>(msg_base);

    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };

    Lotto::GetInstance().OnLoad(recv_msg);
}

HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_WINNER_LIST_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_LOTTO_WINNER_LIST_ACK* recv_msg = \
        static_cast<const MSG_DW_LOTTO_WINNER_LIST_ACK*>(msg_base);

    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };

    Lotto::GetInstance().OnWinnerList(recv_msg);
}

HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_SAVE_ENTRY_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_LOTTO_SAVE_ENTRY_ACK* recv_msg = \
        static_cast<const MSG_DW_LOTTO_SAVE_ENTRY_ACK*>(msg_base);

    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };

    Lotto::GetInstance().OnEntrySave(recv_msg);
}

HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_ROLLOVER_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_LOTTO_ROLLOVER_ACK* recv_msg = \
        static_cast<const MSG_DW_LOTTO_ROLLOVER_ACK*>(msg_base);

    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };

    Lotto::GetInstance().OnRollOver(recv_msg);
}

HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_ADD_TICKET_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_LOTTO_ADD_TICKET_ACK* recv_msg = \
        static_cast<const MSG_DW_LOTTO_ADD_TICKET_ACK*>(msg_base);

    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };

    Lotto::GetInstance().OnTicketAcquisition(recv_msg);
}

HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_REWARD_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_LOTTO_REWARD_ACK* recv_msg = \
        static_cast<const MSG_DW_LOTTO_REWARD_ACK*>(msg_base);

    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };

    Lotto::GetInstance().OnReward(recv_msg);
}

HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_NOTICE_LIST_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_LOTTO_NOTICE_LIST_ACK* recv_msg = \
        static_cast<const MSG_DW_LOTTO_NOTICE_LIST_ACK*>(msg_base);

    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };

    Lotto::GetInstance().OnNoticeList(recv_msg);
}

HANDLER_NODE_INFO(DW_LOTTO, DW_LOTTO_NOTICE_DEL_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_LOTTO_NOTICE_DEL_ACK* recv_msg = \
        static_cast<const MSG_DW_LOTTO_NOTICE_DEL_ACK*>(msg_base);

    if (FlowControl::FCAssert(sizeof(*recv_msg) == msg_size) == false) {
        return;
    };

    Lotto::GetInstance().OnNoticeDelete(recv_msg);
}
