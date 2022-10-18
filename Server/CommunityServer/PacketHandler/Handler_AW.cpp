#include "WorldServerPch.hxx"
#include "./Handler_AW.h"

#include <PacketStruct_DW.h>
#include <SSQ/SSQPacketProcS2S.h>

#include "Sessions/ServerSessionInWorld.h"
#include "./PacketHandler.h"

#include "WorldServer.h"
#include "Functions/GMCmdManager.h"
//
#include "Channel/WorldChannelManager.h"
#include "Channel/WorldChannel.h"
#include "Channel/ZoneCommunity.h"
#include "Channel/PartyCommunity.h"
//
#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"

#include "Services/Lotto/Lotto.h"
#include "Services/Group/GroupManager.h"
#include "Services/Friends/FriendMap.h"
#include "Services/Friends/FriendInvitation.h"
#include "Services/Memo/WorldSystemMemo.h"

namespace ns_world { namespace handlers {
;
}}; //end of namespace

const ns_world::PacketHandlerInfo ns_world::handlers::Handler_AW::GetPacketHandlerNodes()
{
#undef HANDLER_NODE_INFO
#undef HANDLER_NODE_INFO_EOR
#define HANDLER_NODE_INFO(_category_, _protocol_, _phase_) \
    { MAKEWORD(_category_, _protocol_), _phase_, &ns_world::handlers::Handler_AW::On##_protocol_ }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const ns_world::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(AW_CONNECTION, AW_AGENTSERVER_INFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_CONNECTION, AW_PREPARE_NEW_USER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_SET_CHARNAME_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_ENTER_ZONE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LEAVE_ZONE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LOGOUT, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_RESTORE_CMD, PHASE_SERVICE),
        //HANDLER_NODE_INFO(AW_USERCOUNT_SYN, PHASE_SERVICE),
        //HANDLER_NODE_INFO(AW_USERMONEY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_PARTY, AW_PARTY_MAKE_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_PARTY, AW_PARTY_JOIN_MEMBER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_PARTY, AW_PARTY_WITHDRAW_MEMBER_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_PARTY, AW_PARTY_DESTROY_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_GM, AW_GM_WHISPER_SET_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_GM, AW_GM_USERCHAT_BLOCK_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LEVELUP_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_SPECIAL_ITEM_CLEAR_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_USE_DECREE_ITEM_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_USE_DECREE_ITEM_NAK, PHASE_SERVICE),
    #ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
        HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN, PHASE_SERVICE),
    #endif
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STATUS_INTERCEPTION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_GM, AW_GM_NOTICE_CMD, PHASE_SERVICE),
        //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
        HANDLER_NODE_INFO(AW_GM, AW_GM_LOTTO_NUM_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_GM, AW_GM_LOTTO_SETTLE_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_GM, AW_GM_LOTTO_FUND_CMD, PHASE_SERVICE),
        //}
        HANDLER_NODE_INFO(AW_OPERATION, AW_OPERATION_ROOM_CTRL_CMD, PHASE_SERVICE),
        //__KR_001355_20090608_GROUPINGSYSTEM_GW
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_STATUS_GROUP_START_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_STATUS_GROUP_EXP_ACQUISITION_CMD, PHASE_SERVICE),
        //__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STATUS_HEIM_ACQUISITION_NTF, PHASE_SERVICE),
        //__NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STATUS_HEIM_CONSUMPTION_NTF, PHASE_SERVICE),
        //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LOTTO_REWARD_ACK, PHASE_SERVICE),
      #ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LOTTO_TICKET_PURCHASE_ACK, PHASE_SERVICE),
      #endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
        //}
    #ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STATUS_PCBANG_BILLING_NTF, PHASE_SERVICE),
    #endif
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_SYSTEMMEMO_SEND_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STADING_POSITION_CMD, PHASE_SERVICE),
        //_KR_0_20091021_FIRENDINVITATION_EXTEND
        HANDLER_NODE_INFO(AW_FRIENDINVITATE, AW_FRIEND_INVITATIONR_INSERT_CMD, PHASE_SERVICE),
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
    void ns_world::handlers::Handler_AW::On##_protocol_(\
        ns_world::ServerSessionInWorld* const server_session, \
        const MSG_BASE* const msg_base, const uint16_t msg_size)

#define WAVERIX_WORLD_HANDLER_DEBUGGING() \
    WAVERIX_WRAPPER((WAVERIX_LC_CALL4("waverix_log_trace_aw", \
        __FUNCTION__, server_session, msg_base, msg_size)))

#if SUN_CODE_SAMPLE // to support visual assistx intelli-sense
ns_world::ServerSessionInWorld* server_session;
MSG_BASE* msg_base; uint16_t msg_size;
#endif

//==================================================================================================

HANDLER_NODE_INFO(AW_CONNECTION, AW_AGENTSERVER_INFO_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_AGENTSERVER_INFO_SYN* recv_msg = \
        static_cast<const MSG_AW_AGENTSERVER_INFO_SYN*>(msg_base);

    const int channel_id = recv_msg->dwChannelID;
    if (ns_world::WorldChannelManager::Instance()->FindChannel(server_session) != NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("WorldChannel(%d) Already Exist...Disconnect"), channel_id);
        server_session->Disconnect();
        return;
    };

    ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
    const SERVER_KEY& world_key = world_server->GetServerKey();
    ;{
        // configure agent server
        SERVER_KEY server_key(world_key.GetWorldID(), BYTE(channel_id), AGENT_SERVER, 1);
        server_session->SetServerKey((DWORD)server_key);
    };
    //ns_world::community::WorldChannel* const world_channel = \
    //    ns_world::WorldChannelManager::Instance()->GetChannel(channel_id);
    if (ns_world::WorldChannelManager* channel_manager = \
            ns_world::WorldChannelManager::Instance())
    {
        channel_manager->AddChannel(channel_id, server_session);
    };
    /*
    // 혹시 내가 죽었었던 거야?
    MSG_AW_USER_RESTORE_SYN sendMsg;

    sendMsg.m_byCategory = AW_USER_STATUS;
    sendMsg.m_byProtocol = AW_USER_RESTORE_SYN;
    sendMsg.m_dwKey = ns_world::WorldServer::Instance()->GetServerKey();

    // 그럼 복구 좀 해줘바바...
    server_session->Send((BYTE*)&sendMsg, sizeof(sendMsg));
    */
}

HANDLER_NODE_INFO(AW_CONNECTION, AW_PREPARE_NEW_USER_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_PREPARE_NEW_USER_SYN* recv_msg = \
        static_cast<const MSG_AW_PREPARE_NEW_USER_SYN*>(msg_base);

    const int channel_id = server_session->GetChannelID();
    if (server_session->GetServerType() != AGENT_SERVER)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=critical control error, servertype=%d|"),
               server_session->GetServerType());
        return;
    };

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };

    ns_world::UserAccountInfo account_info = { 0, };
    ;{
        account_info.user_guid = recv_msg->dwUserGuid;
        account_info.channel = world_channel;
        BOOST_STATIC_ASSERT(_countof(account_info.account_id) == _countof(recv_msg->ptszAccountID));
        _tcsncpy(account_info.account_id, recv_msg->ptszAccountID,
                 _countof(account_info.account_id));
        account_info.account_id[_countof(account_info.account_id) - 1] = _T('\0');
        account_info.pc_room_id = recv_msg->dwPCRoomID;
        account_info.gm_grade = recv_msg->byGMGrade;
        //recv_msg->dwAuthUserID
        BOOST_STATIC_ASSERT(_countof(account_info.ip_address) == _countof(recv_msg->szClientIP));
        _tcsncpy(account_info.ip_address, recv_msg->szClientIP,
                 _countof(account_info.ip_address));
        account_info.ip_address[_countof(account_info.ip_address) - 1] = _T('\0');
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUser(account_info.user_guid);
    bool new_user = (user == NULL);
    if (new_user) {
        user = user_manager->AllocNewUser();
    }
    else {
        user_manager->Logout(user, ns_functor::LogoutOption::Reuse(\
            user->GetConnectedCharInfo().char_guid != 0));
        new_user = true;
    };
    user_manager->ChangeUserAccountInfo(user, account_info);
    world_channel->Enter(user, user->GetConnectedCharInfo());
    if (new_user) {
        user_manager->AddUser(user);
    };
    //
    MSG_AW_PREPARE_NEW_USER_ACK msg_ack;
    msg_ack.m_dwKey = recv_msg->m_dwKey;
    msg_ack.enabled = true;
    server_session->SendPacket(&msg_ack, sizeof(msg_ack));
    //
    SUNLOG(eFULL_LOG, _T("PREPARE_NEW_USER_SYN[WorldChannel:%u][Guid:%u]"),
           channel_id, account_info.user_guid);
}

HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_SET_CHARNAME_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_SET_CHARNAME_SYN* recv_msg = \
        static_cast<const MSG_AW_USER_SET_CHARNAME_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    ns_world::ConnectedCharInfo char_info = user->GetConnectedCharInfo();

    if (char_info.char_guid == 0)
    {
    }
    else if (char_info.char_guid == recv_msg->dwCharGuid)
    {
        // reuse
    }
    else
    {
        // TODO: f110726.4L, recheck, clear now or already cleared?
    };
    //
    ;{
        char_info.user_guid = recv_msg->m_dwKey;
        char_info.char_guid = recv_msg->dwCharGuid;
        char_info.guild_guid = recv_msg->m_GuildGuid;
        char_info.char_level = recv_msg->m_CharLevel;
        char_info.char_class = recv_msg->m_CharClass;
        const util::StrArrayForCharName& src = recv_msg->szCharName;
        util::StrArrayForCharName& dest = char_info.char_name;
        _tcsncpy(dest, src, _countof(dest));
        dest[_countof(dest) - 1] = _T('\0');
    };
    //----------------------------------------------------------------------------------------------
    // NOTE: f110922.2L, side-effects = {
    //  MSG_DW_FRIEND_GETLIST_SYN,
    //  MSG_DW_GM_CHATBLOCKTIME_REQ,
    //  MSG_DW_STATUS_INTERCEPTION_REQ
    // }
    user_manager->ChangeConnectedCharInfo(user, char_info);
    //
}

HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_ENTER_ZONE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_ENTER_ZONE_SYN* recv_msg = \
        static_cast<const MSG_AW_USER_ENTER_ZONE_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    const eZONETYPE zone_type = static_cast<eZONETYPE>(recv_msg->byZoneType);
    if (zone_type == eZONETYPE_CHARSELECT) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };
    //
    ns_world::ConnectedCharInfo char_info = user->GetConnectedCharInfo();
    char_info.zone_key = recv_msg->dwZoneKey;
    char_info.map_code = MAPCODE(recv_msg->MapCode);
    char_info.zone_type = static_cast<uint8_t>(zone_type);
    ZeroMemory(&char_info.position, sizeof(char_info.position));
    user_manager->ChangeConnectedCharInfo(user, char_info);
    //ns_world::community::ZoneCommunity* zone = world_channel->FindZone(char_info.zone_key);
    //if (FlowControl::FCAssert(zone != NULL) == false) {
    //    return;
    //};
    //zone->Enter(user, char_info);

    // NOTE: old comment, 축적된 총경험치양이 존재하면 게임서버로 보내준다.
}

HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LEAVE_ZONE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_LEAVE_ZONE_SYN* recv_msg = \
        static_cast<const MSG_AW_USER_LEAVE_ZONE_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };
    ns_world::ConnectedCharInfo char_info = user->GetConnectedCharInfo();
    const eZONETYPE zone_type = static_cast<eZONETYPE>(char_info.zone_type);
    if (char_info.zone_key != 0 && zone_type != eZONETYPE_CHARSELECT)
    {  // check integrity
        ASSERT(char_info.zone_key == recv_msg->dwZoneKey);
        ASSERT(char_info.zone_type == recv_msg->byZoneType);
    };
    ;{
        char_info.zone_type = 0;
        char_info.zone_key = 0;
        char_info.map_code = 0;
        char_info.field_code = 0;
        ZeroMemory(&char_info.position, sizeof(char_info.position));
    };
    user_manager->ChangeConnectedCharInfo(user, char_info);
    // NOTE: f110726.4L, the party control does not modified in this function.
    // it is to detach the zone and party
}

HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LOGOUT, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_LOGOUT* recv_msg = static_cast<const MSG_AW_USER_LOGOUT*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = ns_world::WorldUserManager::Instance()->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=logout user ")
           _T("|user_guid=%d, char_guid=%d, zone=%d, zone_type=%d, party=%d, char_name=%s|"),
           char_info.user_guid, char_info.char_guid, char_info.zone_key, char_info.zone_type,
           char_info.party_key, char_info.char_name);
    ns_world::WorldUserManager::Logout(user, ns_functor::LogoutOption::Free(true));
}

//월드 서버 <---> Agent 서버와의 연결문제가 있을시에 클라이언트 호출하여 정보 복구
HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_RESTORE_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_RESTORE_CMD* recv_msg = \
        static_cast<const MSG_AW_USER_RESTORE_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    //const int channel_id = world_channel->GetChannelID();
    //
    ns_world::UserAccountInfo account_info = { 0, };
    ;{
        account_info.user_guid = recv_msg->dwUserGUID;
        account_info.channel = world_channel;
        _tcsncpy(account_info.account_id, recv_msg->ptszAccountID,
            _countof(account_info.account_id));
        account_info.account_id[_countof(account_info.account_id) - 1] = _T('\0');
        account_info.pc_room_id = recv_msg->dwPCRoomID;
        account_info.gm_grade = recv_msg->byGMGrade;
        //recv_msg->dwAuthUserID
        _tcsncpy(account_info.ip_address, recv_msg->pszClientIP,
            _countof(account_info.ip_address));
        account_info.ip_address[_countof(account_info.ip_address) - 1] = _T('\0');
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUser(account_info.user_guid);
    bool new_user = (user == NULL);
    if (new_user) {
        user = user_manager->AllocNewUser();
    }
    else {
        user_manager->Logout(user, ns_functor::LogoutOption::Reuse(false));
        new_user = true;
    };
    user_manager->ChangeUserAccountInfo(user, account_info);
    world_channel->Enter(user, user->GetConnectedCharInfo());
    if (new_user) {
        user_manager->AddUser(user);
    };
    //
    ns_world::ConnectedCharInfo char_info = { 0, };
    if (recv_msg->byZoneType != eZONETYPE_CHARSELECT)
    {
        char_info.user_guid = recv_msg->m_dwKey;
        char_info.char_guid = recv_msg->CharGuid;
        char_info.guild_guid = recv_msg->m_GuildGuid;
        char_info.char_level = recv_msg->m_CharLevel;
        char_info.char_class = recv_msg->m_CharClass;
        _tcsncpy(char_info.char_name, recv_msg->tszCharName, _countof(char_info.char_name));
        char_info.char_name[_countof(char_info.char_name) - 1] = _T('\0');
        char_info.party_key = recv_msg->wPartyKey;
        //
        char_info.zone_key = recv_msg->dwZoneKey;
        char_info.map_code = recv_msg->map_code;
        char_info.zone_type = recv_msg->byZoneType;
        //
        user_manager->ChangeConnectedCharInfo(user, char_info);

        //ns_world::community::ZoneCommunity* zone = world_channel->FindZone(char_info.zone_key);
        //if (FlowControl::FCAssert(zone != NULL) == false) {
        //    return;
        //};
        //zone->Enter(user, char_info);
    };

    //
    MSG_AW_PREPARE_NEW_USER_ACK msg_ack;
    msg_ack.m_dwKey = recv_msg->m_dwKey;
    msg_ack.enabled = true;
    server_session->SendPacket(&msg_ack, sizeof(msg_ack));
    //
    // TODO: f110726.4L, recheck = { Guild(restore), Party, Friend }
}

// 파티 결성
HANDLER_NODE_INFO(AW_PARTY, AW_PARTY_MAKE_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_PARTY_MAKE_SYN* recv_msg = static_cast<const MSG_AW_PARTY_MAKE_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    world_channel->CreateParty(recv_msg->wPartyKey);
}

// 파티원 추가
HANDLER_NODE_INFO(AW_PARTY, AW_PARTY_JOIN_MEMBER_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_PARTY_JOIN_MEMBER_SYN* recv_msg = \
        static_cast<const MSG_AW_PARTY_JOIN_MEMBER_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_CharGUID);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    world_channel->JoinParty(recv_msg->wPartyKey, user);
}

// 파티원 탈퇴
HANDLER_NODE_INFO(AW_PARTY, AW_PARTY_WITHDRAW_MEMBER_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_PARTY_WITHDRAW_MEMBER_SYN* recv_msg = \
        static_cast<const MSG_AW_PARTY_WITHDRAW_MEMBER_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_CharGUID);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    world_channel->WithdrawParty(recv_msg->wPartyKey, user);
}

// 파티 해체
HANDLER_NODE_INFO(AW_PARTY, AW_PARTY_DESTROY_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_PARTY_DESTROY_SYN* recv_msg = \
        static_cast<const MSG_AW_PARTY_DESTROY_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };

    world_channel->DestroyParty(recv_msg->wPartyKey);
}

// GM 귓말 켬/끔
HANDLER_NODE_INFO(AW_GM, AW_GM_WHISPER_SET_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_GM_WHISPER_SET_CMD* recv_msg = \
        static_cast<const MSG_AW_GM_WHISPER_SET_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    ns_world::ChattingMode chatting_mode = user->GetChattingMode();
    ;{  // 이 유저의 귓말 켬/끔 상태 세팅
        chatting_mode.whispered_mode = (recv_msg->m_bOn != false);
        user_manager->ChangeChattingMode(user, chatting_mode);
    };
    MSG_CW_GM_WHISPER_SET_ACK msg_ack;
    msg_ack.m_bOn = chatting_mode.whispered_mode;
    user->SendPacket(&msg_ack, sizeof(msg_ack));
}

// 채팅금지/허용
HANDLER_NODE_INFO(AW_GM, AW_GM_USERCHAT_BLOCK_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    // NOTE: old-comment, 운영팀의 정확한 요구사항을 몰라서 일단 보류
    const MSG_AW_GM_USERCHAT_BLOCK_CMD* recv_msg = \
        static_cast<const MSG_AW_GM_USERCHAT_BLOCK_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    assert(!"warning, recv_msg->m_dwKey == char_guid");
    ns_world::WorldUser* const gm_user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(gm_user != NULL && gm_user->IsGMUser()) == false) {
        return;
    };

    ns_world::WorldUser* const block_user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_CharGuid);

    // NOTE: old-comment, 접속 중이지 않은 유저는 블락 먹일 수 없다!!
    // 만일 그 기능을 지원하려면 이 경우가 생길 때마다 DB 쿼리를 통해 해당 유저가 존재하는지 검사해야 하므로
    // 그 기능은 향후 WOPS에서나 지원해야 할 것이다

    MSG_CW_GM_USERCHAT_BLOCK_NAK msg_nak; // no-send...
    msg_nak.m_dwKey = recv_msg->m_dwKey; // gm char_guid
    msg_nak.m_wErrorCode = RC::RC_CHAT_UNKNOWN;
    if (block_user == NULL)
    {
        msg_nak.m_wErrorCode = RC::RC_CHAT_NOCHAR;
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't find block char(%d)|"),
               recv_msg->m_CharGuid);
        return;
    };

    const ns_world::ChattingMode& chatting_mode = block_user->GetChattingMode();
    if (recv_msg->m_bBlock != false)
    {
        if (chatting_mode.chatting_blocked) {
            msg_nak.m_wErrorCode = RC::RC_CHAT_BLOCKED;
            return;
        };

        ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
        if (ns_world::ServerSessionInWorld* dbproxy = world_server->GetGameDBProxy())
        {
            // request blocked mode
            MSG_DW_GM_CHATBLOCK_SYN msg_syn;
            msg_syn.m_dwKey = gm_user->GetCharGuid();
            msg_syn.m_CharGuid = block_user->GetCharGuid();
            msg_syn.m_dwBlockTIme = recv_msg->m_dwBlockTime;
            assert(!"change gm account id... char name?");
            _tcsncpy(msg_syn.m_ptszGMAccountID, gm_user->GetCharName(), MAX_CHARNAME_LENGTH);
            msg_syn.m_ptszGMAccountID[MAX_CHARNAME_LENGTH] = _T('\0');
            dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
        }
        else
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|Msg=can't apply to db about blocked, char_guid(%d)|"),
                   recv_msg->m_CharGuid);
        };
    }
    else
    {
        if (chatting_mode.chatting_blocked == false) {
            msg_nak.m_wErrorCode = RC::RC_CHAT_ALLOWED;
            return;
        };

        ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
        if (ns_world::ServerSessionInWorld* dbproxy = world_server->GetGameDBProxy())
        {
            // request unblocked mode
            MSG_DW_GM_CHATALLOW_SYN msg_syn;
            msg_syn.m_dwKey = gm_user->GetCharGuid();
            msg_syn.m_CharGuid = block_user->GetCharGuid();
            _tcsncpy(msg_syn.m_ptszGMAccountID, gm_user->GetCharName(), MAX_CHARNAME_LENGTH);
            msg_syn.m_ptszGMAccountID[MAX_CHARNAME_LENGTH] = _T('\0');
            dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
        }
        else
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|Msg=can't apply to db about unblocked, char_guid(%d)|"),
                   recv_msg->m_CharGuid);
        };
    };
}

// 유저 레벨업 처리
HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LEVELUP_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_LEVELUP_CMD* recv_msg = \
        static_cast<const MSG_AW_USER_LEVELUP_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(recv_msg->m_CharGuid);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };
    //
    ns_world::ConnectedCharInfo char_info = user->GetConnectedCharInfo();
    char_info.char_level = recv_msg->m_Level;
    user_manager->ChangeConnectedCharInfo(user, char_info);    

    BOOST_STATIC_ASSERT(sizeof(MSG_CW_FRIEND_LEVELUP_BRD) == sizeof(MSG_CW_REVENGE_LEVELUP_BRD));
    MSG_CW_REVENGE_LEVELUP_BRD msg_revenge_brd;
    msg_revenge_brd.Clear();
    // compile time check
    static_cast<MSG_CW_REVENGE_LEVELUP_BRD*>(\
        static_cast<MSG_CW_FRIEND_LEVELUP_BRD*>(&msg_revenge_brd));
    if (MSG_CW_REVENGE_LEVELUP_BRD* const msg_brd = &msg_revenge_brd)
    {
        msg_brd->m_Level = recv_msg->m_Level;
        _tcsncpy(msg_brd->m_ptszCharName, user->GetCharName(), _countof(msg_brd->m_ptszCharName));
        msg_brd->m_ptszCharName[_countof(msg_brd->m_ptszCharName) - 1] = _T('\0');
    };
    //
    if (MSG_CW_FRIEND_LEVELUP_BRD* const msg_brd =
            reinterpret_cast<MSG_CW_FRIEND_LEVELUP_BRD*>(&msg_revenge_brd))
    {  // broadcast to friends
        msg_brd->m_byCategory = CW_FRIEND;
        msg_brd->m_byProtocol = CW_FRIEND_LEVELUP_BRD;
        user->SendToAllOnlineFriends(msg_brd, sizeof(*msg_brd));
    };
    if (MSG_CW_REVENGE_LEVELUP_BRD* const msg_brd = &msg_revenge_brd)
    {
        msg_brd->m_byCategory = CW_REVENGE;
        msg_brd->m_byProtocol = CW_REVENGE_LEVELUP_BRD;
        user->SendToAllRevengers(msg_brd, sizeof(*msg_brd));
    };
    //_KR_0_20091021_FIRENDINVITATION_EXTEND //레벨 업
    if (ns_world::FriendInvitationInWorld* friend_invitation =
            ns_world::FriendInvitationInWorld::Instance())
    {
        friend_invitation->OnCharacterLevelUp(server_session, recv_msg, msg_size);
    };
    //그룹 멤버들에게도 레벨업을 알린다.
    if (ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance()) {
        group_manager->SendAroundMember(user, group_manager->MEMBER_LEVELUP);
    };
};

//==================================================================================================
// 리벤지(나쁜친구) 관련 패킷 핸들러

HANDLER_NODE_INFO(AW_CHAO, AW_CHAO_REQUEST_REVENGEINFO_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_CHAO_REQUEST_REVENGEINFO_SYN* recv_msg = \
        static_cast<const MSG_AW_CHAO_REQUEST_REVENGEINFO_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_dwCharGuid);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

#if SUN_CODE_BACKUP
    if (!user)
    {
        ServerSession* server_session = ServerSessionManager::Instance()->FindAgentServer(ns_world::WorldServer::Instance()->GetChannelID());
        if (server_session)
        {
            // TODO(WAVERIX) (090623) (NOTE) 재검증 수행하자.
            MSG_AW_ABUSING_USER_VERIFY_QUERY_CMD msg;
            msg.m_dwKey = recv_msg->m_dwKey;
            msg.m_CharGuid = recv_msg->m_dwCharGuid;
            msg.m_eAbuseSubType = eABUSE_SUB_LOGIC_VIOLATION_USER_NONE_WORLDSVR;
            server_session->Send((BYTE*)&msg, sizeof(msg));
        }
        return;
    }
#endif

    MSG_AW_CHAO_REQUEST_REVENGEINFO_ACK msg_ack;
    msg_ack.m_dwKey = recv_msg->m_dwKey;
    msg_ack.m_dwCharGuid = recv_msg->m_dwCharGuid;
    if (ns_world::FriendMap* const friend_map = ns_world::FriendMap::Instance())
    {
        friend_map->LoadRevengeList(user, msg_ack.m_sRevengeUserInfo,
            &msg_ack.m_byRevengeStatNum, &msg_ack.m_byRevengePoint);
    };
    server_session->SendPacket(&msg_ack, msg_ack.GetSize());

    SUNLOG(eFULL_LOG, _T("REQUEST_REVENGEINFO_ACK Send[CharGuid:%u][RevengeNum:%u]"),
           recv_msg->m_dwCharGuid, msg_ack.m_byRevengeStatNum);
}

HANDLER_NODE_INFO(AW_CHAO, AW_CHAO_REMOVE_REVENGE_CHAR_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    // PK에 성공한 경우
    const MSG_AW_CHAO_REMOVE_REVENGE_CHAR_SYN* recv_msg = \
        static_cast<const MSG_AW_CHAO_REMOVE_REVENGE_CHAR_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    MSG_AW_CHAO_REMOVE_REVENGE_CHAR_NAK msg_nak;
    msg_nak.m_dwCharGuid = recv_msg->m_dwCharGuid;
    //
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUser(recv_msg->m_dwCharGuid);
    if (FlowControl::FCAssert(user != NULL) == false)
    {
        msg_nak.m_byErrorCode = RC::RC_REVENGE_NO_CHAR;
        server_session->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };
    // NOTE: f110914.1L, added
    if (ns_world::FriendMap::IsFriendTransaction(user)) {
        return;
    };

    const STRUCT_REVENGE_INFO* revenge_info = \
        ns_world::FriendMap::FindRevengeChar(user, recv_msg->m_dwRevengeCharGuid);
    if (revenge_info == NULL)
    {
        msg_nak.m_byErrorCode = RC::RC_REVENGE_INVALID_CHAR;
        server_session->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG,
            _T("|["__FUNCTION__"]|Msg=can't found the revenging target(%u), requester(%u)|"),
            recv_msg->m_dwRevengeCharGuid, recv_msg->m_dwCharGuid);
        return;
    };

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        //RC_REVENGE_FAILED
        return;
    };

    MSG_DW_CHAO_PK_REVENGE_CHAR_SYN msg_syn;
    msg_syn.m_dwKey = world_channel->GetChannelID();
    msg_syn.m_CharGuidFrom = recv_msg->m_dwCharGuid;
    msg_syn.m_RevengeGuidTo = recv_msg->m_dwRevengeCharGuid;
    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
    //
    ns_world::FriendMap::StartFriendTransaction(user);

    SUNLOG(eFULL_LOG, _T("CHAO_REMOVE_REVENGE Send[CharGuid:%u][Target:%u]"),
           msg_syn.m_CharGuidFrom, msg_syn.m_RevengeGuidTo);
}

HANDLER_NODE_INFO(AW_CHAO, AW_CHAO_ADD_REVENGE_CHAR_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_CHAO_ADD_REVENGE_CHAR_SYN* recv_msg = \
        static_cast<const MSG_AW_CHAO_ADD_REVENGE_CHAR_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    MSG_AW_CHAO_ADD_REVENGE_CHAR_NAK msg_nak;
    msg_nak.m_dwCharGuid = recv_msg->m_dwCharGuid;
    //
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUser(recv_msg->m_dwCharGuid);
    if (FlowControl::FCAssert(user != NULL) == false)
    {
        msg_nak.m_byErrorCode = RC::RC_REVENGE_NO_CHAR;
        server_session->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };
    // NOTE: f110914.1L, added
    if (ns_world::FriendMap::IsFriendTransaction(user)) {
        return;
    };
    // 리벤지 포인트가 남아 있는지 확인
    if (ns_world::FriendMap::GetRevengePoint(user) == 0)
    {
        msg_nak.m_byErrorCode = RC::RC_REVENGE_NO_POINT;
        server_session->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };

    MSG_DW_CHAO_ADD_REVENGE_CHAR_SYN msg_syn;
    _tcsncpy(msg_syn.m_ptszCharNameTo, recv_msg->m_ptszRevengeName,
             _countof(msg_syn.m_ptszCharNameTo));
    msg_syn.m_ptszCharNameTo[_countof(msg_syn.m_ptszCharNameTo) - 1] = _T('\0');

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        //msg_nak.m_byErrorCode = RC::RC_REVENGE_FAILED;
        return;
    };

    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
    ns_world::FriendMap::StartFriendTransaction(user);

    SUNLOG(eFULL_LOG, _T("AW_CHAO_ADD_REVENGE_CHAR_SYN send to DBProxy[CharGuid:%u][Name:%s]"),
           msg_syn.m_CharGuidFrom, msg_syn.m_ptszCharNameTo);
}

// 스페셜 아이템 소유 변경
HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD* recv_msg = \
        static_cast<const MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_dwCharGuid);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    const int number_of_nodes = recv_msg->m_Count;
    for (int index = 0; index < number_of_nodes; ++index)
    {
#if WORLD_CONTENTS_BLOCK
        if (recv_msg->m_ItemInfo[index].m_Apply)
        {
            user->GetItemOwnFlag().IncItemInfo((eITEMTYPE)recv_msg->m_ItemInfo[index].m_ItemType, recv_msg->m_ItemInfo[index].m_Count);
            SUNLOG(eFULL_LOG, "SPECIAL ITEM INC_[CHARGUID:%u][ITEMTYPE:%d]",
                user->GetCharGuid(), (eITEMTYPE)recv_msg->m_ItemInfo[index].m_ItemType);
        }
        else
        {
            user->GetItemOwnFlag().DecItemInfo((eITEMTYPE)recv_msg->m_ItemInfo[index].m_ItemType, recv_msg->m_ItemInfo[index].m_Count);
            SUNLOG(eFULL_LOG, "SPECIAL ITEM DEC_[CHARGUID:%u][ITEMTYPE:%d]",
                user->GetCharGuid(), (eITEMTYPE)recv_msg->m_ItemInfo[index].m_ItemType);
        }
#endif //WORLD_CONTENTS_BLOCK
    };
}

HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_SPECIAL_ITEM_CLEAR_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_ITEM_SPECIAL_ITEM_CLEAR_CMD* recv_msg = \
        static_cast<const MSG_AW_ITEM_SPECIAL_ITEM_CLEAR_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_dwCharGuid);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };
#if WORLD_CONTENTS_BLOCK
    user->GetItemOwnFlag().Init();
#endif //WORLD_CONTENTS_BLOCK
}

HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_USE_DECREE_ITEM_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    //월드 외치기
    const MSG_AW_ITEM_USE_DECREE_ITEM_ACK* recv_msg = \
        static_cast<const MSG_AW_ITEM_USE_DECREE_ITEM_ACK*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = ns_world::WorldUserManager::Instance()->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };
#if WORLD_CONTENTS_BLOCK
    MSG_CW_CHAT_SHOUT_BRD msg_brd;
    do
    {
        // TODO(WAVERIX) (NOTE) m_tszTmpDecreeMsg의 버퍼크기는 256입니다만...? -.-;
        BYTE byMsgLength = _tcslen(user->GetTmpDecreeMsg());
        util::cSTRING_REF strMsgRD(user->GetTmpDecreeMsg(), sizeof(byMsgLength));

        util::cSTRING_REF_RW strNameRW4BRD(msg_brd.m_szCharName, sizeof(msg_brd.m_szCharName));
        strNameRW4BRD.CopyFromString(user->GetCharName().c_str());
        strNameRW4BRD.MakeSafeString();

        util::cSTRING_REF_RW strMsgRW4BRD(msg_brd.m_pszChatMsg, sizeof(msg_brd.m_pszChatMsg));
        strMsgRW4BRD.CopyFromString(user->GetTmpDecreeMsg());
        strMsgRW4BRD.MakeSafeString();

        msg_brd.m_byMsgLength = strMsgRW4BRD.GetSTRING_LENGTH()+1;
    } while(0);

    msg_brd.m_byMessageType = msg_brd.DECREE_SHOUT;

    UserManager::Instance()->SendToAll((BYTE*)&msg_brd, msg_brd.GetSize());
#endif //WORLD_CONTENTS_BLOCK
}

HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_USE_DECREE_ITEM_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_ITEM_USE_DECREE_ITEM_NAK* recv_msg = \
        static_cast<const MSG_AW_ITEM_USE_DECREE_ITEM_NAK*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = ns_world::WorldUserManager::Instance()->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    MSG_CW_CHAT_SHOUT_NAK msg_nak;
    msg_nak.m_wErrorCode = RC::RC_CHAT_NOT_DECREE_ITEM;
    user->SendPacket(&msg_nak, sizeof(msg_nak));
}

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
// 하임행운 캐시 응모권 사용 요청
HANDLER_NODE_INFO(AW_ITEM, AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN, PHASE_SERVICE)
{
    const MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN* msg = \
        static_cast<const MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN*>(msg_base);

    // 해당 캐릭터가 진행 중인 건이 있는지 검사한다.
    const TicketActivationXactions& xactions = Lotto::GetInstance().GetTicketActivationXactions();
    if (xactions.Find(msg->char_guid) != NULL)
    {
        // 이미 진행 중인 건이 있다면 오류 리턴
        MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK ack;
        ack.result_code = static_cast<WORD>(RC::RC_ITEM_INVALIDSTATE);
        ack.char_guid   = msg->char_guid;
        ack.index       = msg->index;
        ack.pos         = msg->pos;
        server_session->SendPacket(&ack, sizeof(ack));
        return;
    }
    Lotto::GetInstance().OnTicketActivation(msg_base);
}
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STATUS_INTERCEPTION_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_STATUS_INTERCEPTION_SYN* recv_msg = \
        static_cast<const MSG_AW_USER_STATUS_INTERCEPTION_SYN*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    ns_world::ChattingMode chatting_mode = user->GetChattingMode();
    const eINTERCEPTION_TYPE interception_type = \
        static_cast<eINTERCEPTION_TYPE>(recv_msg->m_byInterceptionCode);
    if (interception_type == eINTERCEPTION_WHISPER)
    {
        chatting_mode.whispering_intercepted = (recv_msg->m_bFlag != false);
        //
        MSG_AW_USER_STATUS_INTERCEPTION_ACK msg_ack;
        msg_ack.m_byInterceptionCode = eINTERCEPTION_WHISPER;
        msg_ack.m_bFlag = chatting_mode.whispering_intercepted;
        user->SendPacket(&msg_ack, sizeof(msg_ack));
    };
    user_manager->ChangeChattingMode(user, chatting_mode);
}

HANDLER_NODE_INFO(AW_GM, AW_GM_NOTICE_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_GM_NOTICE_CMD* recv_msg = \
        static_cast<const MSG_AW_GM_NOTICE_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = ns_world::WorldUserManager::Instance()->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL && user->IsGMUser()) == false) {
        return;
    };

    ns_world::GMCmdManager::Instance()->NoticeBroadcast(\
        user, recv_msg->m_eType, recv_msg->m_tszNoticeMsg);
}

HANDLER_NODE_INFO(AW_GM, AW_GM_LOTTO_NUM_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_GM_LOTTO_NUM_CMD* recv_msg = \
        static_cast<const MSG_AW_GM_LOTTO_NUM_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUser(recv_msg->dwUserKey);
    if (FlowControl::FCAssert(user != NULL && user->IsGMUser()) == false) {
        return;
    };

    MSG_CW_LOTTO_FINISH_BRD msg_brd;
    msg_brd.wDrawNum = (WORD)Lotto::GetInstance().GetCurrentDrawNum();
    Lotto::GetInstance().GetCurrentWinningNum(msg_brd.bWinningNum);
    user->SendPacket(&msg_brd, sizeof(msg_brd));
}

HANDLER_NODE_INFO(AW_GM, AW_GM_LOTTO_SETTLE_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_GM_LOTTO_SETTLE_CMD* recv_msg = \
        static_cast<const MSG_AW_GM_LOTTO_SETTLE_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUser(recv_msg->dwUserKey);
    if (FlowControl::FCAssert(user != NULL && user->IsGMUser()) == false) {
        return;
    };

    Lotto::GetInstance().ForceRollOver();
}

HANDLER_NODE_INFO(AW_GM, AW_GM_LOTTO_FUND_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_GM_LOTTO_FUND_CMD* recv_msg = \
        static_cast<const MSG_AW_GM_LOTTO_FUND_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUser(recv_msg->user_key);
    if (FlowControl::FCAssert(user != NULL && user->IsGMUser()) == false) {
        return;
    };

    Lotto::GetInstance().OnTotalAccumChange(recv_msg->amount_delta);
}

HANDLER_NODE_INFO(AW_OPERATION, AW_OPERATION_ROOM_CTRL_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    // NOTE: SSQ(survival soloing quest) 1toN composed command handler
    using namespace nsSSQ;
    const MSG_AW_OPERATION_ROOM_CTRL_CMD* recv_msg = \
        static_cast<const MSG_AW_OPERATION_ROOM_CTRL_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };

    const WAR_MSG_HDR* IN stream_root = recv_msg->m_Header;
    const WAR_MSG_HDR IN header = *stream_root;

    WAR_TAG_P pIt = (WAR_TAG_P)(stream_root + 1);
    const WAR_TAG_P end = pIt + header.size;
    for (uint16_t size = 0; pIt < end; pIt += size)
    {
        size = 10000;    // 미 설정에 대한 스킵 설정

        switch (pIt->_TAG)
        {
        case SSQ_S2S_SET_CHAT_BLOCK_ZONE:
            {
                RECV_PATTERNCODE_FOR_SSQ(SSQ_S2S_SET_CHAT_BLOCK_ZONE);

                ns_world::community::ZoneCommunity* zone = world_channel->FindZone(rMsg.ZoneKey);
                if (zone == NULL) {
                    break;
                };
                // (WAVERIX) (090220) (BUG-FIX) 타락한 사원 전투 직전, 월드서버의 각종 기능이 동
                //작되지 않도록 하는 기능이 당시 현철씨랑 협의시 착오로 인해 거꾸로 설정해
                //월드서버 관련해서 동작하는 기능이 허용되는 문제 수정.
                zone->ConfigureChatEnabled(rMsg.Enabled != false);
            }
            break;
        default:
            SUNLOG(eCRITICAL_LOG, "Can't Readable SSQ Msg Format %2u", pIt->_TAG);
            break;
        }; // end 'switch'
    }
}

HANDLER_NODE_INFO(AW_USER_STATUS, AW_STATUS_GROUP_START_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_STATUS_GROUP_START_CMD* recv_msg = \
        static_cast<const MSG_AW_STATUS_GROUP_START_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };

    MSG_DW_GROUP_LIST_REQ msg_req;//그룹 맴버 리스트 요청 메세지
    msg_req.m_CharGUID = recv_msg->m_CharGuid;
    // TODO: f110727.1L, check synchronization problem

    ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
    if (ns_world::ServerSessionInWorld* dbproxy = world_server->GetGameDBProxy()) {
        dbproxy->SendPacket(&msg_req, sizeof(msg_req));
    };
}

// __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
HANDLER_NODE_INFO(AW_USER_STATUS, AW_STATUS_GROUP_EXP_ACQUISITION_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_STATUS_GROUP_EXP_ACQUISITION_CMD* recv_msg = \
        static_cast<const MSG_AW_STATUS_GROUP_EXP_ACQUISITION_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_CharGuid);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    group_manager->ChangeBonusExp(user, recv_msg->m_Amount);
#ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY 
    if (group_manager->GetGroupLowMember(user) == GROUP_HAVE_ONLINE_LOW)
    {
        group_manager->SendAroundMember(user, ns_world::GroupManager::GET_EXP);
    }
#else
    if (group_manager->GetGroupHighMember(user) == GROUP_HAVE_OFFLINE_HIGH ||
        group_manager->GetGroupHighMember(user) == GROUP_HAVE_ONLINE_HIGH)  
    {
        group_manager->SendAroundMember(user, ns_world::GroupManager::GET_EXP);
    }
#endif
    group_manager->ChangeBonusExp(user, 0);
}

// implemented by __NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STATUS_HEIM_ACQUISITION_NTF, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_STATUS_HEIM_ACQUISITION_NTF* recv_msg =
        static_cast<const MSG_AW_USER_STATUS_HEIM_ACQUISITION_NTF*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUser* user = \
        ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_CharGuid);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };
    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    group_manager->ChangeBonusExp(user, recv_msg->m_Amount);
#ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY
    if (group_manager->GetGroupHighMember(user) == GROUP_HAVE_ONLINE_HIGH ||
        group_manager->GetGroupHighMember(user) == GROUP_HAVE_OFFLINE_HIGH)
    {
        group_manager->SendAroundMember(user, ns_world::GroupManager::GET_MONEY);
    }
#else
    if (group_manager->GetGroupLowMember(user) == GROUP_HAVE_ONLINE_LOW)
    {
        group_manager->SendAroundMember(user, ns_world::GroupManager::GET_MONEY);
    }
#endif
    group_manager->ChangeBonusExp(user, 0);
}

// implemented by __NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION
HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STATUS_HEIM_CONSUMPTION_NTF, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF* recv_msg = 
        static_cast<const MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };

  #if !defined(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE)
    // 복권용지 직접 구입 기능을 사용하지 않는 경우에만 적립과 복권 용지 지급이 이루어진다
    Lotto::GetInstance().OnHeimConsumption(recv_msg);
  #endif //
}

HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LOTTO_REWARD_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_LOTTO_REWARD_ACK* recv_msg = 
        static_cast<const MSG_AW_USER_LOTTO_REWARD_ACK*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    //
    Lotto::GetInstance().OnActualReward(recv_msg);
}

#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_LOTTO_TICKET_PURCHASE_ACK, PHASE_SERVICE)
{
    const MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK* recv_msg = 
        static_cast<const MSG_AW_USER_LOTTO_TICKET_PURCHASE_ACK*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    //
    Lotto::GetInstance().OnTicketPurchase(recv_msg);
}
#endif //

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STATUS_PCBANG_BILLING_NTF, PHASE_SERVICE)
{
    const MSG_AW_USER_STATUS_PCBANG_BILLING_NTF* recv_msg = \
        static_cast<const MSG_AW_USER_STATUS_PCBANG_BILLING_NTF*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUser(recv_msg->m_dwKey);
    if (FlowControl::FCAssert(user != NULL) == false) {
        return;
    };

    assert(sizeof(*recv_msg) == msg_size);
    ns_world::UserAccountInfo user_account_info = user->GetUserAccountInfo();
    user_account_info.pc_room_id = recv_msg->m_PcRoomId;
    user_manager->ChangeUserAccountInfo(user, user_account_info);
}
#endif

// implemented by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_SYSTEMMEMO_SEND_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_SYSTEMMEMO_SEND_CMD* recv_msg = \
        static_cast<const MSG_AW_USER_SYSTEMMEMO_SEND_CMD*>(msg_base);

    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    ns_world::WorldSystemMemo system_memo;
    system_memo.SendSystemMemo(recv_msg->system_memo_info_, 
                               recv_msg->system_memo_list_num_,
                               recv_msg->system_memo_info_->system_memo_type_);
}

HANDLER_NODE_INFO(AW_USER_STATUS, AW_USER_STADING_POSITION_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_AW_USER_STADING_POSITION_CMD* recv_msg = \
        static_cast<const MSG_AW_USER_STADING_POSITION_CMD*>(msg_base);
    //
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    for (uint32_t index = 0; index < recv_msg->number_of_nodes; ++index)
    {
        const MSG_AW_USER_STADING_POSITION_CMD::Node& node = recv_msg->nodes[index];
        ns_world::WorldUser* user = user_manager->FindUser(node.user_guid);
        if (user == NULL || user->GetCharGuid() == 0) {
            continue;
        };
        ns_world::ConnectedCharInfo char_info = user->GetConnectedCharInfo();
        char_info.position.x = node.pos_x;
        char_info.position.y = node.pos_y;
        char_info.position.z = 0;
        user_manager->ChangeConnectedCharInfo(user, char_info);
    };
}

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
HANDLER_NODE_INFO(AW_FRIENDINVITATE, AW_FRIEND_INVITATIONR_INSERT_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::community::WorldChannel* const world_channel = server_session->GetChannel();
    if (FlowControl::FCAssert(world_channel != NULL) == false) {
        return;
    };
    const MSG_AW_FRIEND_INVITATIONR_INSERT_CMD* recv_msg = \
        static_cast<const MSG_AW_FRIEND_INVITATIONR_INSERT_CMD*>(msg_base);
    ns_world::FriendInvitationInWorld::Instance()->OnAddInvitatedFriendCMD(\
        server_session, recv_msg, msg_size);
}
