#include "WorldServerPch.hxx"
#include "./Handler_CW.h"

#include "./PacketHandler.h"
#include "WorldServer.h"

//#include "Sessions/ServerSessionInWorld.h"
//#include "Channel/WorldChannelManager.h"
//#include "Channel/WorldChannel.h"
#include "Objects/WorldUserManager.h"
//#include "Objects/WorldUser.h"
//#include "Services/ChattingService.h"
//#include "Services/Lotto/Lotto.h"
#include "Services/Group/GroupManager.h"
#include "Services/Guild/WorldGuildManager.h"

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

//==================================================================================================
// 그룹맺기 시스템 핸들러 함수
//==================================================================================================
//클라이언트로 부터 그룹 맴버 추가 요청
HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADD_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GROUP_ADD_SYN* recv_msg = static_cast<const MSG_CW_GROUP_ADD_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    //
    WorldUser* dest_user = user_manager->FindUserByCharName(recv_msg->ptszMemeberName);
    if (dest_user == NULL)
    {
        //-- 상대 유저가 게임에 접속해 있지 않다.
        MSG_CW_GROUP_ADD_NAK nak_msg;
        nak_msg.m_byResult = nak_msg.NO_USER;
        user->SendPacket(&nak_msg, sizeof(nak_msg));

        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Request to logout user", recv_msg->ptszMemeberName);
        return;
    };
    if (group_manager->CanAddGroupMember(user, dest_user, recv_msg) == false)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Group Member Add Fail");
        return;
    }

    // NOTE: f110818.1L, aligned the opposite user info, start transaction.
    // WARNING: side-effect of CanAddGroupMember support success the below codes.
    //  requester GROUP_LEVEL_HIGH
    //      |           |
    //   target   GROUP_LEVEL_LOW
    if (group_manager->StartGroupMemberAdditionTransaction(user, dest_user) == false) {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Group Member Add Fail");
        return;
    };
    // NOTE: side-effects of StartGroupMemberAdditionTransaction
    //  = { contains opposite user info for transaction,
    //      transaction_state }
#if SUN_CODE_BACKUP
    //-- 서로의 정보를 저장한다.
    user->SetStructMemberInfo(dest_user, GROUP_LEVEL_LOW);
    dest_user->SetStructMemberInfo(user, GROUP_LEVEL_HIGH);

    dest_user->SetGroupTransaction(true);
    user->SetGroupTransaction(true);
#endif

    MSG_CW_GROUP_ADD_RESPONSE_SYN relay_msg;
    ;{
        _tcsncpy(relay_msg.ptszMemeberName,  user->GetCharName(),
                 _countof(relay_msg.ptszMemeberName));
        relay_msg.ptszMemeberName[_countof(relay_msg.ptszMemeberName) - 1] = _T('\0');
    };
    dest_user->SendPacket(&relay_msg, sizeof(relay_msg));
}

//그룹 초대 받은 대상이 수락을 하였다.
HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADD_RESPONSE_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GROUP_ADD_RESPONSE_ACK* recv_msg = \
        static_cast<const MSG_CW_GROUP_ADD_RESPONSE_ACK*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    //
    WorldUser* request_user = user_manager->FindUserByCharName(recv_msg->ptszMemeberName);
    if (request_user == NULL)
    {
        SUNLOG(eFULL_LOG,"["__FUNCTION__"]Not found user");
        return;
    };
    group_manager->SetGroupHighMember(user, GROUP_HAVE_ONLINE_HIGH);
    //
    MSG_DW_GROUP_ADD_SYN msg_syn;
    ;{
        msg_syn.m_fromCharGUID = request_user->GetCharGuid();
        msg_syn.m_toCharGUID = user->GetCharGuid();    //중요한 개념 : 
        msg_syn.m_GroupSTATFirst = GROUP_LEVEL_LOW;     //DB상 destChar는 Char의 하위 레벨이다.
        msg_syn.m_GroupSTATSecond = GROUP_LEVEL_HIGH;   //DB상 destChar는 Char의 상위 레벨이다.
    };
    //
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Not found gamedbproxy");
        return;
    };
    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
}

//그룹 초대 받은 대상이 여러 가지 이유로 거절하였다.
HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADD_RESPONSE_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GROUP_ADD_RESPONSE_NAK* recv_msg = \
        static_cast<const MSG_CW_GROUP_ADD_RESPONSE_NAK*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    __TOUCHED(group_manager);
    //
    WorldUser* request_user = user_manager->FindUserByCharName(recv_msg->ptszMemeberName);
    if (request_user == NULL) 
    {
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Not found user");
        return;
    };
    // WARNING: f110823.1L, check 'user' transaction problem that is not released
    group_manager->EndGroupTransaction(request_user);
    group_manager->EndGroupTransaction(user);

    MSG_CW_GROUP_ADD_NAK msg_nak;
    {
        msg_nak.m_byResult = recv_msg->m_byResult;
        _tcsncpy(msg_nak.ptszMemeberName,  user->GetCharName(),
                 _countof(msg_nak.ptszMemeberName));
        msg_nak.ptszMemeberName[_countof(msg_nak.ptszMemeberName) - 1] = _T('\0');
    }
    request_user->SendPacket(&msg_nak, sizeof(msg_nak));
}

//클라이언트로 부터 그룹 맴버 리스트 요청받음
HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_GROUPLIST_REQ, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GROUP_GROUPLIST_REQ* recv_msg = \
        static_cast<const MSG_CW_GROUP_GROUPLIST_REQ*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    if (ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance()) {
        group_manager->SendMemberList(user);
    };
}

//그룹 맴버 상세 정보 요청
HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_STATUS_REQ, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GROUP_STATUS_REQ* recv_msg = \
        static_cast<const MSG_CW_GROUP_STATUS_REQ*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    //------------------------------------------------------------------------------
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    //
    ns_world::WorldUser* target_user = user_manager->FindUserByCharName(recv_msg->m_ptszCharName);
    if (target_user == NULL)
    {
        MSG_CW_GROUP_STATUS_NAK msg_nak;
        msg_nak.dwResult = msg_nak.NO_MEMBER_DATA;//유저 미접속
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Not Connected Target[Guid:%u][Target:%s]", 
               user->GetCharGuid(), recv_msg->m_ptszCharName);
        return;
    };

    const STRUCT_GROUP_MEMBER_INFO* member_info = \
        group_manager->FindGroupMember(user, target_user->GetCharGuid());

    if (member_info == NULL) 
    {
        MSG_CW_GROUP_STATUS_NAK msg_nak;
        msg_nak.dwResult = msg_nak.NOT_MEMBER;//그룹 맴버가 아니다.
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Can't Found Member [Guid:%u][Target:%s]", 
               user->GetCharGuid(), recv_msg->m_ptszCharName);
        return;
    };
    //찾기 결과 값이 정확하지 않을 경우
    if (member_info->charGUID != target_user->GetCharGuid())
    {
        MSG_CW_GROUP_STATUS_NAK msg_nak;
        msg_nak.dwResult = msg_nak.NOT_MEMBER;//그룹 맴버가 아니다.
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Incorrect Info of Member [Guid:%u][Target:%s]", 
               user->GetCharGuid(), recv_msg->m_ptszCharName);
        return;
    };

    MSG_CW_GROUP_STATUS_ANS msg_ack;
    msg_ack.m_byChannel = target_user->GetUserAccountInfo().channel->GetChannelID();
    const ns_world::ConnectedCharInfo& target_char_info = target_user->GetConnectedCharInfo();
    msg_ack.m_byZoneType = target_char_info.zone_type;
    msg_ack.m_dwZoneKey = target_char_info.zone_key;
    msg_ack.m_MapCode = target_char_info.map_code;
    _tcsncpy(msg_ack.m_ptszCharName, recv_msg->m_ptszCharName, _countof(msg_ack.m_ptszCharName));
    msg_ack.m_ptszCharName[_countof(msg_ack.m_ptszCharName) - 1] = _T('\0');
    msg_ack.m_byDuty = 0; // CHANGES: f110823.1L, set the default value;

    // 길드 가입돼 있으면 길드 관련 정보도 전송
    if (target_char_info.guild_guid != 0)
    {
        ns_world::WorldGuild* const guild = \
            ns_world::WorldGuildManager::Instance()->FindGuild(\
                target_char_info.guild_guid);

        if (guild)
        {
            ns_world::WorldGuildMember* guild_member = \
                guild->FindMember(target_char_info.char_guid);
            if (guild_member)
            {
                _tcsncpy(msg_ack.m_tszGuildName, guild->GetGuildName(),
                         _countof(msg_ack.m_tszGuildName));
                msg_ack.m_tszGuildName[_countof(msg_ack.m_tszGuildName) - 1] = _T('\0');
                msg_ack.m_byDuty = guild_member->GetDuty();
            }
        }
    }
    user->SendPacket(&msg_ack, sizeof(msg_ack));
}

HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_DEL_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GROUP_DEL_SYN* recv_msg = \
        static_cast<const MSG_CW_GROUP_DEL_SYN*>(msg_base);
    //
    if (CheckEnabledChattingState(user, server_session) == false) {
        return;
    };
    //------------------------------------------------------------------------------
    ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance();
    //
    if (group_manager->IsGroupTransaction(user))
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Transaction Error", 
               recv_msg->ptszMemeberName);
        return;
    };

    const STRUCT_GROUP_MEMBER_INFO* member_info = \
        group_manager->FindGroupMember(user, recv_msg->ptszMemeberName);
    if (member_info == NULL)
    {
        MSG_CW_GROUP_DEL_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.NOT_MEMBER_USER;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]No Member[%s][CharGuid:%u]->[%s]", 
               user->GetCharName(), user->GetCharGuid(), recv_msg->ptszMemeberName);
        return;
    };

    MSG_DW_GROUP_DEL_SYN msg_syn;
    msg_syn.m_fromCharGUID = user->GetCharGuid();
    msg_syn.m_toCharGUID = member_info->charGUID;

    ns_world::ServerSessionInWorld* dbproxy = ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        MSG_CW_GROUP_DEL_NAK msg_nak;
        msg_nak.m_byResult = msg_nak.ERR_DEFAULT;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]No GameDBProxy(WorldUser %s)", 
               user->GetCharName());
        return;
    }
    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
}

HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_ADDBLOCK_SYN, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GROUP_ADDBLOCK_SYN* recv_msg = \
        static_cast<const MSG_CW_GROUP_ADDBLOCK_SYN*>(msg_base);
    //
    MSG_CW_GROUP_ADDBLOCK_ACK msg_ack;
    ns_world::GroupManager::ChangeAddingPreventedFlag(user, (recv_msg->m_bBlock != 0));
    msg_ack.m_bNowBlock = ns_world::GroupManager::GetAddingPreventedFlag(user);
    user->SendPacket(&msg_ack, sizeof(msg_ack));

#if SUN_CODE_BACKUP
    else
    {
        MSG_CW_GROUP_ADDBLOCK_NAK sendMsg;
        user->SendPacket(&sendMsg, sizeof(sendMsg));
    }
#endif

}

HANDLER_NODE_INFO(CW_GROUP, CW_GROUP_RESTOR_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    ns_world::WorldUser* const user = FIND_USER_FROM_AGENT_MSG(msg_base);
    if (user == NULL) {
        return;
    };
    const MSG_CW_GROUP_RESTOR_CMD* recv_msg = \
        static_cast<const MSG_CW_GROUP_RESTOR_CMD*>(msg_base);
    //
    if (ns_world::GroupManager* group_manager = ns_world::GroupManager::Instance()) {
        group_manager->UpdateGroupInfo(user);
    };
}
