#include "WorldServerPch.hxx"
#include "./Handler_DW.h"

#include <PacketStruct_WZ.h>
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
//#include "Services/Group/GroupManager.h"
#include "Services/Guild/WorldGuildManager.h"
//#include "Services/Friends/FriendMap.h"
//#include "Services/Friends/FriendInvitation.h"

//==================================================================================================

namespace ns_world { namespace handlers {
;

}}; //end of namespace

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
//==================================================================================================
//==================================================================================================

HANDLER_NODE_INFO(DW_GUILD, DW_DOMINATION_FINDMONEY_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_DOMINATION_FINDMONEY_ACK* recv_msg = \
        static_cast<const MSG_DW_DOMINATION_FINDMONEY_ACK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD receiver_char_guid = recv_msg->m_RecvCharGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(receiver_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               receiver_char_guid);
        return;
    };

    //보상 받지 못하는 이유에 대한 로그 강화
    if (recv_msg->m_Result != 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][WorldUser:%u]DB Query Result Fail", 
               recv_msg->m_RecvCharGuid);
        return;
    }
    else if (recv_msg->m_RecvCharGuid != recv_msg->m_DBCharGuid)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Guid Not Match[DB:%u]!=[REQ:%u]", 
            recv_msg->m_DBCharGuid, recv_msg->m_RecvCharGuid);
        return;
    }
    else if (recv_msg->m_CostType != recv_msg->m_DBCostType)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][WorldUser:%u]Money Type Not Same[DB:%d]!=[REQ:%d]", 
               recv_msg->m_RecvCharGuid, recv_msg->m_DBCostType, recv_msg->m_CostType);
        return;
    }
    else if (recv_msg->m_RequestMapCode != recv_msg->m_DBMapCode)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][WorldUser:%u]MapCode Not Match[DB:%u]!=[REQ:%u]", 
               recv_msg->m_RecvCharGuid, recv_msg->m_DBMapCode, recv_msg->m_RequestMapCode);
        return;
    }

    if (user->EnabledSendMessage() == false)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]No AgentServer");
        return;
    };

    MSG_AW_USER_DOMINATION_RETURNHIEM_CMD msg_cmd;
    msg_cmd.m_CharGuid = recv_msg->m_DBCharGuid;
    msg_cmd.m_Type = recv_msg->m_CostType;
    msg_cmd.m_TenderCost = recv_msg->m_TenderCost;
    user->SendPacket(&msg_cmd, sizeof(msg_cmd));

    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]%d Type(return:9,reward:10)%d Heim Give to WorldUser[%d]",
           msg_cmd.m_Type, msg_cmd.m_TenderCost, msg_cmd.m_CharGuid);
}

HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_ANS, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GUILD_WAREHOUSE_LOGLIST_ANS* recv_msg = \
        static_cast<const MSG_DW_GUILD_WAREHOUSE_LOGLIST_ANS*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_ReqCharGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };

    MSG_CW_GUILD_WAREHOUSE_LOGLIST_ANS msg_ack;
    msg_ack.m_byLogCount = recv_msg->m_byLogCount;
    msg_ack.m_byCurrentPage = recv_msg->m_byCurrentPage;
    msg_ack.m_byLogPageCount = recv_msg->m_byLogPageCount;
    BOOST_STATIC_ASSERT(sizeof(msg_ack.m_LogList) == sizeof(recv_msg->m_LogList) &&
                        _countof(msg_ack.m_LogList) == _countof(recv_msg->m_LogList) &&
                        sizeof(msg_ack.m_LogList[0]) == sizeof(recv_msg->m_LogList[0]));
    if (FlowControl::FCAssert(_countof(msg_ack.m_LogList) >= msg_ack.m_byLogCount) == false) {
        msg_ack.m_byLogCount = _countof(msg_ack.m_LogList);
    };
    CopyMemory(msg_ack.m_LogList, recv_msg->m_LogList, \
        sizeof(msg_ack.m_LogList[0]) * msg_ack.m_byLogCount);

    user->SendPacket(&msg_ack, msg_ack.GetSize());
    //if (msg_ack.m_byMemoPage >= msg_ack.m_byMemoPageCnt)
    //{
    //  user->SetMemoTransaction(false);
    //}

#ifdef _DEBUG
    SUNLOG(eDEV_LOG, "DW_GUILD_WAREHOUSE_LOGLIST_ANS[%s][%u][Memo:%u][Page:%u/%u]",
           user->GetCharName(), user->GetCharGuid(), 
           msg_ack.m_byLogCount, 
           msg_ack.m_byCurrentPage, 
           msg_ack.m_byLogPageCount);
#endif
}

HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_WAREHOUSE_LOGLIST_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GUILD_WAREHOUSE_LOGLIST_NAK* recv_msg = \
        static_cast<const MSG_DW_GUILD_WAREHOUSE_LOGLIST_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_ReqCharGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };

    //user->SetMemoTransaction(false);
    MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK msg_nak;
    msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;
    user->SendPacket(&msg_nak, sizeof(msg_nak));

#ifdef _DEBUG
    SUNLOG(eDEV_LOG, "DW_MEMO_LIST_NAK[%s][%u]", user->GetCharName(), user->GetCharGuid());
#endif
}


HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_SELECT_NOTICE_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GUILD_SELECT_NOTICE_ACK* recv_msg = \
        static_cast<const MSG_DW_GUILD_SELECT_NOTICE_ACK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_OperCharGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };

    ns_world::WorldGuild* const guild = \
        ns_world::WorldGuildManager::Instance()->FindGuild(recv_msg->m_GuildGuid);
    if (guild == NULL)
    {
        MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_GUILD_INVALID_STATE;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
    };
    // success
    guild->SetGuildNotice(recv_msg->m_szGuildNotice);

    MSG_CW_GUILD_SELECT_NOTICE_ACK msg_ack;
    msg_ack.m_NoticeType = recv_msg->m_GuildNoticeType;
    strncpy(msg_ack.m_ptszNotice, guild->GetGuildNotice(), _countof(msg_ack.m_ptszNotice));
    msg_ack.m_ptszNotice[_countof(msg_ack.m_ptszNotice) - 1] = '\0';
    msg_ack.m_wNoticeLen = static_cast<uint16_t>(strlen(msg_ack.m_ptszNotice));
    user->SendPacket(&msg_ack, sizeof(msg_ack)); // TODO: check message size

}

#if SUN_CODE_BACKUP
// NOTE: f110818.1L, not handled protocol
HANDLER_NODE_INFO(DW_GUILD, DW_GUILD_SELECT_NOTICE_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_DW_GUILD_SELECT_NOTICE_NAK* recv_msg = \
        static_cast<const MSG_DW_GUILD_SELECT_NOTICE_NAK*>(msg_base);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    const DWORD requester_char_guid = recv_msg->m_OperCharGuid;
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(requester_char_guid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found character(CharGuid=%u)|"),
               requester_char_guid);
        return;
    };

    // nothing to do...
    /*
    WorldUser *user = ns_world::WorldUserManager::Instance()->FindUserByCharGuid(recv_msg->m_OperCharGuid);
    if (user == NULL)
    {
    SUNLOG(eDEV_LOG, "OnDW_GUILD_SELECT_NOTICE_NAK Error - No WorldUser[%u]", recv_msg->m_OperCharGuid);
    return;
    }

    MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK nakMsg;
    nakMsg.m_byErrorCode = RC::RC_GUILD_INVALID_STATE;
    user->SendPacket((BYTE*)&nakMsg, sizeof(nakMsg));*/
}
#endif //SUN_CODE_BACKUP

//==================================================================================================
//==================================================================================================
//==================================================================================================

//HANDLER_NODE_INFO(WZ_GUILD, SERVERCOMMON_HEARTBEAT, PHASE_SERVICE)
//{
//}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_DESTROY_GUILD_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_DESTROY_GUILD_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_DESTROY_GUILD_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    guild_manager->DestroyGuild(recv_msg->m_GuildGuid);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_JOIN_GUILD_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_JOIN_GUILD_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_JOIN_GUILD_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    // 길드 정보 있는지 검사하여 없으면 복구 요청
    ns_world::WorldGuild* guild = guild_manager->FindGuild(recv_msg->m_GuildGuid);
    if (guild == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found guild(%d)|"),
               recv_msg->m_GuildGuid);
        return;
    }

    //1. ns_world::WorldUser 길드 세팅.
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(recv_msg->m_Info.m_CharGuid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found char_guid(%d) in guild(%d)|"),
               recv_msg->m_GuildGuid, recv_msg->m_Info.m_CharGuid);
        return;
    }
    ns_world::ConnectedCharInfo char_info = user->GetConnectedCharInfo();
    char_info.guild_guid = recv_msg->m_GuildGuid;
    user_manager->ChangeConnectedCharInfo(user, char_info);
    guild_manager->JoinGuild(recv_msg->m_GuildGuid, recv_msg->m_Info);

    // 새로 가입했으면 길드 공지를 날려준다.
    MSG_CW_GUILD_NOTICE_CMD msg_cmd;
    msg_cmd.m_wNoticeLen = strlen(guild->GetGuildNotice());
    //memset(msg_cmd.m_ptszNotice, 0, sizeof(TCHAR)*(MAX_GUILDNOTICE_LENGTH+1));
    strncpy(msg_cmd.m_ptszNotice, guild->GetGuildNotice(), MAX_GUILDNOTICE_LENGTH);
    //msg_cmd.m_ptszNotice[MAX_GUILDNOTICE_LENGTH+1] = '\0';

    //해당 길드원한테만 보내도록 하자
    guild->SendAllMember(&msg_cmd, msg_cmd.GetSize());
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_WITHDRAW_GUILD_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_WITHDRAW_GUILD_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_WITHDRAW_GUILD_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    // 이미 게임서버에서 탈퇴한것을 알려줬다.
    // 따라서 길드 멤버 구조만 유지한다.
    guild_manager->WithdrawGuild(recv_msg->m_GuildGuid, recv_msg->m_CharGuid);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(recv_msg->m_CharGuid);
    ns_world::ConnectedCharInfo char_info = user->GetConnectedCharInfo();
    char_info.guild_guid = 0;
    user_manager->ChangeConnectedCharInfo(user, char_info);
}

// 길드 공지
HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_NOTICE_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_NOTICE_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_NOTICE_CMD*>(msg_base);
    // nothing to do
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
// NOTE: this contents removed by TAIWAN_VERSION dropping
HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHUNTING_PVP_RESULT_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CHUNTING_PVP_RESULT_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CHUNTING_PVP_RESULT_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    guild_manager->SetChuntingPVPInfo(recv_msg->m_GuildGuid, recv_msg->m_PVPInfo);
}
#endif //

//길드 직책
HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_DUTY_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CHANGE_DUTY_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CHANGE_DUTY_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    bool changed_result = (guild_manager->ChangeMemberDuty(\
        recv_msg->m_GuildGuid, recv_msg->m_TargetCharGuid, recv_msg->m_eGuildDuty) != false);
    if (changed_result == false)
    {
        SUNLOG(eCRITICAL_LOG,
            _T("|["__FUNCTION__"]|Msg=DB에서는 성공인데, 월드서버에서 실패다.. 문제다. ")
            _T("GUILDGUID= %u, CHARGUID = %u, DUTY =%d|"),
            recv_msg->m_GuildGuid, recv_msg->m_TargetCharGuid, recv_msg->m_eGuildDuty);
    }
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_MASTER_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CHANGE_MASTER_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CHANGE_MASTER_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    //길드장이 변경되었으므로, 직책이 바뀐거하고 똑같이 처리한다.

    //1. 이전길드장을 정예병으로 바꾼다.
    guild_manager->ChangeMemberDuty(
        recv_msg->m_GuildGuid, 
        recv_msg->m_OldMasterGuid, 
        eGUILD_DUTY_REGULAR_SOLDIER
        );

    //2. 새로운 길드장을 길마로 바꾼다.
    guild_manager->ChangeMemberDuty(
        recv_msg->m_GuildGuid, 
        recv_msg->m_NewMasterGuid, 
        eGUILD_DUTY_MASTER
        );
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_COMMANDER_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CHANGE_COMMANDER_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CHANGE_COMMANDER_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    //군단장이 변경되었으면, 직책이 변경된거다.
    guild_manager->ChangeCommander(\
        recv_msg->m_GuildGuid, recv_msg->m_TargetCharGuid, recv_msg->m_byCorpsNum);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_CORPS_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CHANGE_CORPS_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CHANGE_CORPS_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    //군단변경.
    guild_manager->ChangeCorps(\
        recv_msg->m_GuildGuid, recv_msg->m_TargetCharGuid, recv_msg->m_byCorpsNum);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CREATE_REAR_CAMP_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CREATE_REAR_CAMP_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CREATE_REAR_CAMP_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    guild_manager->CreateRearCamp(recv_msg->m_GuildGuid, recv_msg->m_byCorpsNum);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_DESTROY_REAR_CAMP_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_DESTROY_REAR_CAMP_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_DESTROY_REAR_CAMP_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    guild_manager->DestroyRearCamp(recv_msg->m_GuildGuid, recv_msg->m_byCorpsNum);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CREATE_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CREATE_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CREATE_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();

    //길드생성.
    guild_manager->CreateGuild(recv_msg->m_WorldGuildInfo, recv_msg->m_BaseMemberInfo);

    // 이부분을 함수로 빼고 싶지만, ns_world::WorldUserManager와 ns_world::WorldGuildManager를 둘다 참조한다. 
    // 될 수 있으면 전역으로 쓰이는 것은 서로 연관시키지 말자. 

    // 길마가 접속해 있으면 길마에 길드정보 세팅하자.
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = \
        user_manager->FindUserByCharGuid(recv_msg->m_BaseMemberInfo.m_CharGuid);
    if (user == NULL)
    {
        SUNLOG(eMIDDLE_LOG, _T("|["__FUNCTION__"]|Msg=not online user[CharGuid:%u]|"),
               recv_msg->m_BaseMemberInfo.m_CharGuid);
        return;
    };

    ns_world::ConnectedCharInfo char_info = user->GetConnectedCharInfo();
    char_info.guild_guid = recv_msg->m_WorldGuildInfo.m_GuildGuid;
    user_manager->ChangeConnectedCharInfo(user, char_info);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_RESTORE_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_RESTORE_ACK* recv_msg = \
        static_cast<const MSG_WZ_GUILD_RESTORE_ACK*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    //내부적으로 월드서버에 세팅한다.
    guild_manager->GuildRestore(recv_msg->m_WorldGuildInfo, recv_msg->m_BaseMemberInfo);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_LOGIN_CHAR_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_LOGIN_CHAR_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_LOGIN_CHAR_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    const GUILDGUID guild_guid = recv_msg->m_WorldGuildInfo.m_GuildGuid;
    assert(guild_guid != 0);
    //내부적으로 월드서버에 세팅한다.
    guild_manager->LoginGuild(recv_msg->m_WorldGuildInfo, recv_msg->m_BaseMemberInfo);
    ns_world::WorldGuild* const guild = guild_manager->FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=cannot find guild(%d)|"), guild_guid);
        return;
    };

    // 길드 공지 보내줘야 한다.
    MSG_CW_GUILD_CHANGE_NOTICE_BRD msg_brd;
    ;{
        util::StrArrayForGuildNotice& dest = msg_brd.m_ptszNotice;
        _tcsncpy(dest, guild->GetGuildNotice(), _countof(dest));
        dest[_countof(dest) - 1] = _T('\0');
        msg_brd.m_wNoticeLen = static_cast<uint16_t>(::_tcslen(dest));
    };

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = \
        user_manager->FindUserByCharGuid(recv_msg->m_BaseMemberInfo.m_CharGuid);
    user->SendPacket(&msg_brd, msg_brd.GetSize());
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_LOGOUT_CHAR_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_LOGOUT_CHAR_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_LOGOUT_CHAR_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    // 내부적으로 월드서버에 세팅한다.
    guild_manager->LogoutGuild(recv_msg->m_GuildGuid, recv_msg->m_CharGuid);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_KICK_PLAYER_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_KICK_PLAYER_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_KICK_PLAYER_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    // 강퇴시킨다.
    guild_manager->KickMember(recv_msg->m_GuildGuid, recv_msg->m_KickCharGuid);

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(recv_msg->m_KickCharGuid);

    ns_world::ConnectedCharInfo char_info = user->GetConnectedCharInfo();
    char_info.guild_guid = 0;
    user_manager->ChangeConnectedCharInfo(user, char_info);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_MEMBER_INFO_REQ_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_MEMBER_INFO_REQ_ACK* recv_msg = \
        static_cast<const MSG_WZ_GUILD_MEMBER_INFO_REQ_ACK*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    guild_manager->StoreBaseMemberInfo(recv_msg->m_GuildGuid, recv_msg->m_BaseMemberInfo);
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_RECOVERY_GUILD_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_RECOVERY_GUILD_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_RECOVERY_GUILD_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    ns_world::WorldGuild* guild = guild_manager->FindGuild(recv_msg->m_GuildGuid);
    if (guild)
    {
        MSG_CW_GUILD_RECOVERY_GUILD_BRD msg_brd;
        msg_brd.m_GuildIndex = recv_msg->m_GuildGuid;
        guild->SendAllMember(&msg_brd, sizeof(msg_brd));
    };
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_DESTROY_WAIT_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_DESTROY_WAIT_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_DESTROY_WAIT_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    ns_world::WorldGuild* guild = guild_manager->FindGuild(recv_msg->m_GuildGuid);
    if (guild)
    {
        //MSG_CW_GUILD_DESTROY_WAIT_CMD msg_brd;
        MSG_CW_GUILD_RECOVERY_GUILD_BRD msg_brd; // NOTE: f110915.1L, ??
        msg_brd.m_GuildIndex = recv_msg->m_GuildGuid;
        guild->SendAllMember(&msg_brd, sizeof(msg_brd));
    }
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_GUILDMARK_REGISTER_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_GUILDMARK_REGISTER_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_GUILDMARK_REGISTER_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    ns_world::WorldGuild* guild = guild_manager->FindGuild(recv_msg->m_GuildGuid);
    if (guild)
    {   
        MSG_CW_GUILD_GUILDMARK_REGISTER_BRD msg_brd;
        msg_brd.m_GuildIndex= recv_msg->m_GuildGuid;
        msg_brd.m_PatternIndex = recv_msg->m_PatternIndex;
        msg_brd.m_BackGroundIndex = recv_msg->m_BackGroundIndex;
        guild->SendAllMember(&msg_brd, sizeof(msg_brd));
    }
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_GUILDMARK_DELETE_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_GUILDMARK_DELETE_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_GUILDMARK_DELETE_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    ns_world::WorldGuild* guild = guild_manager->FindGuild(recv_msg->m_GuildGuid);
    if (guild)
    {   
        MSG_CW_GUILD_GUILDMARK_DELETE_BRD msg_brd;
        msg_brd.m_GuildIndex= recv_msg->m_GuildGuid;
        msg_brd.m_PatternIndex = recv_msg->m_PatternIndex;
        msg_brd.m_BackGroundIndex = recv_msg->m_BackGroundIndex;
        guild->SendAllMember(&msg_brd, sizeof(msg_brd));
    }
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CHANGE_NOTICE_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CHANGE_NOTICE_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    ns_world::WorldGuild* guild = guild_manager->FindGuild(recv_msg->m_GuildGuid);
    if (guild == NULL) {
        return;
    };

    guild->SetGuildNotice(recv_msg->m_szGuildNotice);

    MSG_CW_GUILD_CHANGE_NOTICE_BRD msg_brd;
    ;{
        msg_brd.m_wNoticeLen = recv_msg->m_wNoticeLen;

        util::StrArrayForGuildNotice& dest = msg_brd.m_ptszNotice;
        strncpy(dest, guild->GetGuildNotice(), _countof(dest));
        dest[_countof(dest) - 1] = _T('\0');
    };
    guild->SendAllMember(&msg_brd, msg_brd.GetSize());
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHANGE_NOTICE_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CHANGE_NOTICE_NAK* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CHANGE_NOTICE_NAK*>(msg_base);

    //ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(recv_msg->m_OperCharGuid);
    if (user)
    {
        MSG_CW_GUILD_CHANGE_NOTICE_NAK msg_nak;
        msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
    }
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_NAME_CHANGE_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_NAME_CHANGE_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_NAME_CHANGE_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    ns_world::WorldGuild* guild = guild_manager->FindGuild(recv_msg->m_GuildGuid);

    if (guild) {
        guild->ChangeGuildName(recv_msg->m_tszGuildName);
    };
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_DOMINATION_RETURNHIME_CMD, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD* recv_msg = \
        static_cast<const MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD*>(msg_base);

    ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    guild_manager->SendDominationHeimReturnMemo(recv_msg);
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_ACK*>(msg_base);

    //ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(recv_msg->m_dwKey);
    if (user == NULL) {
        return;
    };
    MSG_DW_FRIEND_DETAIL_INFO_SYN msg_syn;
    msg_syn.m_dwKey = recv_msg->m_dwKey;
    msg_syn.m_dwToCharGuid = recv_msg->m_dwToCharGuid;

    ns_world::ServerSessionInWorld* dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {        
        MSG_CW_FRIEND_STATUS_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_FRIEND_FAIL;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    }
    dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
}

HANDLER_NODE_INFO(WZ_GUILD, WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK, PHASE_SERVICE)
{
    __TOUCHED((server_session, msg_base, msg_size));WAVERIX_WORLD_HANDLER_DEBUGGING();
    const MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK* recv_msg = \
        static_cast<const MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_NAK*>(msg_base);

    //ns_world::WorldGuildManager* const guild_manager = ns_world::WorldGuildManager::Instance();
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* user = user_manager->FindUserByCharGuid(recv_msg->m_dwKey);
    if (user == NULL) {
        return;
    };
    MSG_CW_FRIEND_STATUS_NAK msg_nak;
    msg_nak.m_byErrorCode = recv_msg->m_byErrorCode;
    user->SendPacket(&msg_nak, sizeof(msg_nak));
}

