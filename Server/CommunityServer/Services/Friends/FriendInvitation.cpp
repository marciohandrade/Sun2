#include "WorldServerPch.hxx"
#include "./FriendInvitation.h"

// NOTE: implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
#include <FriendInvitation/FriendInvitationController.h> // common

#include "WorldServer.h"
#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
#include "Channel/WorldChannelManager.h"
#include "Services/Friends/FriendMap.h"

#include "PacketHandler/Handler_CW.h"
#include "PacketHandler/Handler_DW.h"

//==================================================================================================

struct ns_world::FriendInvitationInWorld::ChangeEvent
{
    ChangeEvent(ns_world::FriendInvitationInWorld* root) : root_(root) {}
    //캐릭터의 레벨이 변경 되었을 때~
    void ChangedEventLevel(ns_world::WorldUser* user, LEVELTYPE level);
    //보상 정보를 DB Proxy 에 전송한다.
    void SendRewardInfo(CHARGUID char_guid, const util::StrArrayForCharName& char_name,
        ns_world::WorldUser* sender, LEVELTYPE level, eSYSTEM_MEMO_TYPE memo_type);
    //이벤트 종료 처리
    void OnEndEvent(ns_world::WorldUser* user);
    //
    ns_world::FriendInvitationInWorld* const root_;
    __DISABLE_COPY(ChangeEvent);
};

//==================================================================================================

ns_world::FriendInvitationInWorld*
ns_world::FriendInvitationInWorld::instance_static_ = NULL;

ns_world::FriendInvitationInWorld*
ns_world::FriendInvitationInWorld::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(ns_world::FriendInvitationInWorld** static_table)
            : static_table_(static_table) {
        }
        ~AutoClosure() {
            if (*static_table_) {
                TAllocDelete(ns_world::FriendInvitationInWorld, *static_table_);
            };
            *static_table_ = NULL;
        }
        ns_world::FriendInvitationInWorld** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    ns_world::FriendInvitationInWorld* provider = TAllocNew(ns_world::FriendInvitationInWorld);
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

//==================================================================================================

ns_world::FriendInvitationInWorld::FriendInvitationInWorld()
    : char_status_data_accessor_(NULL)
    , inter_controller_(NULL)
{
    redirected_this_ = this;
}

ns_world::FriendInvitationInWorld::~FriendInvitationInWorld()
{
    if (inter_controller_) {
        TAllocDelete(FriendInvitationController, inter_controller_);
    };
    ;{
        FRIEND_TABLE::const_iterator it = inviting_friend_table_.begin();
        const FRIEND_TABLE::const_iterator end = inviting_friend_table_.end();
        for (; it != end; ++it)
        {
            FRIEND_INFO_MAP* friends = it->second;
            TAllocDelete(FRIEND_INFO_MAP, friends);
        };
        inviting_friend_table_.clear();
    };
    ;{
        FRIEND_TABLE::const_iterator it = observer_friend_table_.begin();
        const FRIEND_TABLE::const_iterator end = observer_friend_table_.end();
        for (; it != end; ++it)
        {
            FRIEND_INFO_MAP* friends = it->second;
            TAllocDelete(FRIEND_INFO_MAP, friends);
        };
        observer_friend_table_.clear();
    };
}

// call it on prepared service
void ns_world::FriendInvitationInWorld::StartService()
{
    if (FlowControl::FCAssert(inter_controller_ == NULL) == false) {
        TAllocDelete(FriendInvitationController, inter_controller_);
    };
    inter_controller_ = TAllocNew(FriendInvitationController);
}

void ns_world::FriendInvitationInWorld::RegisterCharStatusDataNodeAccessor(
    ns_world::ICharStatusDataNodeAccessor* char_status_data_accessor)
{
    char_status_data_accessor_ = char_status_data_accessor;
}

void ns_world::FriendInvitationInWorld::PrepareFriendInvitationTable(ns_world::WorldUser* user)
{
    const DWORD user_guid = user->GetUserGuid();
    FRIEND_INFO_MAP*& inviting_friends = inviting_friend_table_[user_guid];
    if (inviting_friends == NULL) {
        inviting_friends = TAllocNew(FRIEND_INFO_MAP);
    } else {
        inviting_friends->clear();
    };
    FRIEND_INFO_MAP*& invited_friends = observer_friend_table_[user_guid];
    if (invited_friends == NULL) {
        invited_friends = TAllocNew(FRIEND_INFO_MAP);
    } else {
        invited_friends->clear();
    };
};

bool ns_world::FriendInvitationInWorld::UpdateFriendInvitationInfo(
    ns_world::WorldUser* user,
    FRIEND_TABLE* const link_table, const STRUCT_FRIEND_INFO& friend_info)
{
    FRIEND_TABLE::const_iterator found_table = link_table->find(user->GetUserGuid());
    if (found_table == link_table->end()) {
        return false;
    };
    FRIEND_INFO_MAP* const friends = found_table->second;
    assert(friends != NULL);
    // updates
    const CHARGUID char_guid = friend_info.dwFriendGuid;
    ns_world::CharStatusDataNode* const status_data_node = \
        char_status_data_accessor_->GetCharStatusData(char_guid);
    //
    if (status_data_node->friend_configured == false) {
        status_data_node->friend_info = friend_info;
        status_data_node->friend_configured = true;
    };
    friends->operator[](char_guid) = &status_data_node->friend_info;

    return true;
};

void ns_world::FriendInvitationInWorld::ReleaseFriendInvitationTable(ns_world::WorldUser* user)
{
    const USERGUID user_guid = user->GetUserAccountInfo().user_guid;
    ;{
        FRIEND_TABLE::const_iterator found = inviting_friend_table_.find(user_guid);
        if (found != inviting_friend_table_.end()) {
            FRIEND_INFO_MAP* const friends = found->second;
            friends->clear();
        };
    };
    ;{
        FRIEND_TABLE::const_iterator found = observer_friend_table_.find(user_guid);
        if (found != observer_friend_table_.end()) {
            FRIEND_INFO_MAP* const friends = found->second;
            friends->clear();
        };
    };
}

void ns_world::FriendInvitationInWorld::ReleaseFriendInvitationRelation(
    CHARGUID from_char_guid, CHARGUID to_char_guid)
{
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* const from_user = user_manager->FindUserByCharGuid(from_char_guid);
    if (from_user == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=can't found the from user(char_guid=%u)|"),
               from_char_guid);
        return;
    };

    const STRUCT_FRIEND_INFO* friend_info = \
        ns_world::FriendMap::FindFriend(from_user, to_char_guid);
    if (friend_info == NULL) {
        return;
    };

    // NOTE: f110909.2L, prevent the released object access point
    //const DWORD unlinked_char_guid = friend_info->dwFriendGuid;
    ;{
        redirected_this_->ProcessFriendInvitationRelationDeletionEvent(
            from_user, STRUCT_FRIEND_DB_INFO::INFO_INVIAT, to_char_guid);
        redirected_this_->ProcessFriendInvitationRelationDeletionEvent(
            from_user, STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED, to_char_guid);
    };

    if (ns_world::WorldUser* friend_user = user_manager->FindUserByCharGuid(to_char_guid))
    {
        redirected_this_->ProcessFriendInvitationRelationDeletionEvent(
            friend_user, STRUCT_FRIEND_DB_INFO::INFO_INVIAT, from_char_guid);
        redirected_this_->ProcessFriendInvitationRelationDeletionEvent(
            friend_user, STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED, from_char_guid);
    };
}



void ns_world::FriendInvitationInWorld::OnAddInvitatedFriendCMD(
    ns_world::ServerSessionInWorld* const server_session,
    const MSG_AW_FRIEND_INVITATIONR_INSERT_CMD* const recv_msg, const WORD msg_size)
{
    const DWORD inviting_char_guid = recv_msg->InvitateInfo.dwGuid;
    const DWORD invited_char_guid = recv_msg->InvitateInfo.dwFriendGuid;
    SUNLOG(eDEV_LOG, _T("|["__FUNCTION__"]|FromCharGuid=%u, ToCharGuid=%u|"),
           inviting_char_guid, invited_char_guid);

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        return; // can't service
    };

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* const user = user_manager->FindUserByCharGuid(inviting_char_guid);

    if (bool offline = (user == NULL)) { // 내가 온라인 상태라면
        return;
    };

    //나에게 친구가 추가됐다고 알려준다.
    MSG_DW_FRIEND_ADD_ACK msg_ack;
    msg_ack.m_CharGuidFrom = inviting_char_guid;
    if (STRUCT_FRIEND_INFO* const dest_info = &msg_ack.m_ToFriendInfo)
    {
        dest_info->dwFriendGuid = invited_char_guid;
        dest_info->sPacketInfo.bOnline = false;
        dest_info->sPacketInfo.byChannel = 0;
        dest_info->sPacketInfo.byClass = recv_msg->InvitateInfo.byClass;
        dest_info->sPacketInfo.Level = 1;
    };
    if (STRUCT_FRIEND_PACKET_INFO* const sub_info = &msg_ack.m_ToFriendInfo.sPacketInfo)
    {
        // type checking
        const util::StrArrayForCharName& src = recv_msg->InvitateInfo.ptszFriendName;
        util::StrArrayForCharName& dest = sub_info->ptszFriendName;
        assert(src[_countof(src) - 1] == '\0');
        strncpy(dest, src, _countof(dest));
        dest[_countof(dest) - 1] = '\0';
    };

    ns_world::handlers::Handler_DW::OnDW_FRIEND_ADD_ACK(dbproxy, &msg_ack, sizeof(msg_ack));

    this->UpdateFriendInvitationInfo(user, &inviting_friend_table_, msg_ack.m_ToFriendInfo);
    //친구 추가 상태인지 알려준다.
    INVITATEFRIENDLIST_INFO friend_invitation_node;
    redirected_this_->MakeInitializedFriendInvitationNode(\
        STRUCT_FRIEND_DB_INFO::INFO_INVIAT,
        user, &friend_invitation_node, invited_char_guid);
    redirected_this_->NotifyPreparedFriendInvitationList(\
        user,
        friend_invitation_node, INVITATEFRIENDLIST_INFO::FRIENDLIST_ADD,
        static_cast<eMsgForward>(redirected_this_->eMsgForward_ToAgent |
                                 redirected_this_->eMsgForward_ToClient));

    //쪽지가왔다고 알려준다. (다시 요청한다.)
    MSG_CW_MEMO_LIST_REQ memo_msg_syn;
    memo_msg_syn.m_dwKey = user->GetUserGuid();

    ns_world::community::WorldChannel* channel = user->GetUserAccountInfo().channel;
    assert(channel != NULL);
    ns_world::ServerSessionInWorld* linked_session = channel->GetBindedSession();
    assert(linked_session != NULL);
    ns_world::handlers::Handler_CW::OnCW_MEMO_LIST_REQ(\
        linked_session, &memo_msg_syn, sizeof(memo_msg_syn));
};

void ns_world::FriendInvitationInWorld::OnCharacterLevelUp(
    ns_world::ServerSessionInWorld* const server_session,
    const MSG_AW_USER_LEVELUP_CMD* const recv_msg, const WORD msg_size)
{
    if (inter_controller_ == NULL || inter_controller_->CheckFriendInvitationEvent() == false) {
        return;
    };

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    ns_world::WorldUser* const user = user_manager->FindUserByCharGuid(recv_msg->m_CharGuid);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found user(char_guid=%u)|"),
               recv_msg->m_CharGuid);
        return;
    };

    switch (LEVELTYPE event_level = recv_msg->m_Level)  //레벨 20, 40, 60, 80 달성시 선물 증정
    {
    case 20:
    case 40:
    case 60:
    case 80:
        ;{
            ChangeEvent change_event(this);
            change_event.ChangedEventLevel(user, event_level);
        };
        break;
    };
}

void ns_world::FriendInvitationInWorld::OnFriendRemove(
    ns_world::ServerSessionInWorld* const server_session,
    const MSG_DW_FRIEND_DEL_ACK* const recv_msg, const WORD msg_size)
{
    if (inter_controller_ == NULL || inter_controller_->CheckFriendInvitationEvent() == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=friend invitation event does not serviced"));
        return;
    };

    this->ReleaseFriendInvitationRelation(
        recv_msg->m_CharGuidFrom, recv_msg->m_CharGuidTo);
}

void ns_world::FriendInvitationInWorld::OnFriendBlock(
    ns_world::ServerSessionInWorld* const server_session,
    const MSG_DW_FRIEND_BLOCK_ACK* const recv_msg, const WORD msg_size)
{
    if (inter_controller_ == NULL || inter_controller_->CheckFriendInvitationEvent() == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=friend invitation event does not serviced"));
        return;
    };

    this->ReleaseFriendInvitationRelation(
        recv_msg->m_CharGuidFrom, recv_msg->m_BlockInfo.dwBlockGuid);
}

//디비에서 친구 목록 받기 성공
void ns_world::FriendInvitationInWorld::OnAddFriend(
    STRUCT_FRIEND_INFO& friend_info,
    const STRUCT_FRIEND_DB_INFO& friend_info_from_db,
    ns_world::WorldUser* user)
{
    // NOTE: replaced from 'CHECK_FRIENDINVITATION_EVENT_RETRUN'
    if (inter_controller_ == NULL || inter_controller_->CheckFriendInvitationEvent() == false) {
        return;
    };

    const LEVELTYPE event_max_level = inter_controller_->GetEventMaxLevel();

    if (friend_info_from_db.Sts == friend_info_from_db.INFO_INVIAT)
    {
        //내가 추천한 친구가 이벤트 종료 레벨 이상이었는데도 일반 친구가 아니면 디비 정보가 잘못된거야
        if (friend_info_from_db.Level >= event_max_level)
        {           
            SUNLOG(eFULL_LOG, 
                _T("|["__FUNCTION__"]|Msg=can't add to INVITING list, ")
                _T("violation of the friend invitation event level limitation|")
                _T("inviter=%d, invited char=%d|"),
                user->GetCharGuid(), friend_info_from_db.dwFriendGuid);
        }
        else
        {
            this->UpdateFriendInvitationInfo(user, &inviting_friend_table_, friend_info);
        };
    }
    else if (friend_info_from_db.Sts == friend_info_from_db.INFO_BEINVIATED)
    {
        //내가 친구 추천을 받았는데 내가 이벤트 종료 레벨 이상이었는데도 일반 친구가 아니면 디비 정보가 잘못된거야
        if (user->GetConnectedCharInfo().char_level >= event_max_level)
        {
            SUNLOG(eFULL_LOG, 
                _T("|["__FUNCTION__"]|Msg=can't add to INVITED list, ")
                _T("violation of the friend invitation event level limitation|")
                _T("invited char=%d, inviter=%d|"),
                user->GetCharGuid(), friend_info_from_db.dwFriendGuid);
        }
        else
        {
            this->UpdateFriendInvitationInfo(user, &observer_friend_table_, friend_info);
        };
    };
};

// 친구 상태 코드를 반환한다.
// returns: STRUCT_FRIEND_DB_INFO::enumeration
BYTE ns_world::FriendInvitationInWorld::GetFriendSts(
    ns_world::WorldUser* user, DWORD friend_char_guid)
{
    const DWORD user_guid = user->GetUserGuid();
    if (FRIEND_INFO_MAP* relation_table = \
            inviting_friend_table_[user_guid]) // assertion (true)
    {
        if (relation_table->find(friend_char_guid) != relation_table->end()) {
            return STRUCT_FRIEND_DB_INFO::INFO_INVIAT;
        };
    };
    if (FRIEND_INFO_MAP* relation_table = \
            observer_friend_table_[user_guid]) // assertion (true)
    {
        if (relation_table->find(friend_char_guid) != relation_table->end()) {
            return STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED;
        };
    };
    return STRUCT_FRIEND_DB_INFO::INFO_FRIEND;
}

void ns_world::FriendInvitationInWorld::NotifyTotalFriendInvitationList(
    ns_world::WorldUser* user, eMsgForward forwarding_flags)
{
    // NOTE: replaced from 'CHECK_FRIENDINVITATION_EVENT_RETRUN'
    if (inter_controller_ == NULL || inter_controller_->CheckFriendInvitationEvent() == false) {
        return;
    };

    redirected_this_->NotifyPartialFriendInvitationList(
        STRUCT_FRIEND_DB_INFO::INFO_INVIAT, user,
        INVITATEFRIENDLIST_INFO::FRIENDLIST_INIT, forwarding_flags);
    redirected_this_->NotifyPartialFriendInvitationList(
        STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED, user,
        INVITATEFRIENDLIST_INFO::FRIENDLIST_INIT, forwarding_flags);
}

void ns_world::FriendInvitationInWorld::NotifyPartialFriendInvitationList(
    STRUCT_FRIEND_DB_INFO::eNodeType invitation_type,
    ns_world::WorldUser* user,
    INVITATEFRIENDLIST_INFO::FRIEND_ACTION action,
    eMsgForward forwarding_flags) const
{
    INVITATEFRIENDLIST_INFO friend_invitation_list;
    bool loaded_list = this->LoadFriendInvitationRelationList(\
        user, invitation_type, &friend_invitation_list);
    if (FlowControl::FCAssert(loaded_list) == false) {
        return;
    };
    redirected_this_->NotifyPreparedFriendInvitationList(\
        user, friend_invitation_list, action, forwarding_flags);
}

void ns_world::FriendInvitationInWorld::NotifyPreparedFriendInvitationList(
    ns_world::WorldUser* user,
    const INVITATEFRIENDLIST_INFO& friend_list,
    INVITATEFRIENDLIST_INFO::FRIEND_ACTION action,
    eMsgForward forwarding_flags) const
{
    if (forwarding_flags == this->eMsgForward_Undefined) {
        return;
    };
    uint8_t msg_buffer[max(sizeof(MSG_AW_FRIENDINVITATE_INVITATELIST_CMD),
                           sizeof(MSG_CW_FRIENDINVITATE_INVITATELIST_CMD))];
    if (forwarding_flags & this->eMsgForward_ToAgent)
    {
        MSG_AW_FRIENDINVITATE_INVITATELIST_CMD* const msg_cmd = \
            new (msg_buffer) MSG_AW_FRIENDINVITATE_INVITATELIST_CMD;
        msg_cmd->m_InvitateFriendListInfo = friend_list;
        //
        msg_cmd->m_InvitateFriendListInfo.m_FreindAction = action;
        user->SendPacket(msg_cmd, msg_cmd->GetSize());
    };
    if (forwarding_flags & this->eMsgForward_ToClient)
    {
        MSG_CW_FRIENDINVITATE_INVITATELIST_CMD* const msg_cmd = \
            new (msg_buffer) MSG_CW_FRIENDINVITATE_INVITATELIST_CMD;
        msg_cmd->m_InvitateFriendInfo = friend_list;
        //
        msg_cmd->m_InvitateFriendInfo.m_FreindAction = action;
        if (action == INVITATEFRIENDLIST_INFO::FRIENDLIST_DEL) {
            msg_cmd->m_InvitateFriendInfo.m_FriendSts = STRUCT_FRIEND_DB_INFO::INFO_FRIEND;
        };
        user->SendPacket(msg_cmd, msg_cmd->GetSize());
    };

#if WORLD_CONTENTS_BLOCK
    //개발시 필요한 로그 삭제해야한다.
    SUNLOG(eDEV_LOG, "FRIEND_NORMAL Count[%u]", user->GetFriendMap(ns_world::WorldUser::FRIEND_NORMAL)->GetFriendNum());
    SUNLOG(eDEV_LOG, "FRIEND_INVIATE Count[%u]", user->GetFriendMap(ns_world::WorldUser::FRIEND_INVIATE)->GetFriendNum());
    SUNLOG(eDEV_LOG, "FRIEND_BEINVIATED Count[%u]", user->GetFriendMap(ns_world::WorldUser::FRIEND_BEINVIATED)->GetFriendNum());
#endif
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
// internal sub-parts, ns_world::FriendInvitationInWorld::ChangeEvent

//캐릭터의 레벨이 변경 되었을 때~
void ns_world::FriendInvitationInWorld::ChangeEvent::ChangedEventLevel(
    ns_world::WorldUser* user, LEVELTYPE level)
{
    assert(root_->inter_controller_ != NULL);
    FRIEND_INFO_MAP* friends = NULL;
    ;{
        FRIEND_TABLE::const_iterator found = \
            root_->observer_friend_table_.find(user->GetUserGuid());
        if (found != root_->observer_friend_table_.end()) {
            friends = found->second;
        };
    };
    if (friends == NULL) {
        return;
    };

    // NOTE: f111006.4L, friend invitation relations
    //  A(User)  -> B(User)     : friend invitation
    //  A'(char) -> B'(char)
    //  A"
    //  a"'
    //      OnEventLevelup(B)
    //          Reward targets = { A', A", A"', B' }
    //
    const LEVELTYPE event_max_level = root_->inter_controller_->GetEventMaxLevel();
    const eSYSTEM_MEMO_TYPE system_memo_type = (level == event_max_level)
        ? SYSMEMO_FRIENDINVITATION_INVATE_END : SYSMEMO_FRIENDINVITATION_INVATE_PRESENT;

    FRIEND_INFO_MAP::const_iterator friend_it = friends->begin();
    const FRIEND_INFO_MAP::const_iterator friend_end = friends->end();
    for (; friend_it != friend_end; ++friend_it)
    {
        const STRUCT_FRIEND_INFO& friend_info = *friend_it->second;

        SendRewardInfo(friend_info.dwFriendGuid,
            friend_info.sPacketInfo.ptszFriendName,
            user, level, system_memo_type);
    };
    // NOTE: send to me.
    SendRewardInfo(user->GetCharGuid(), user->GetCharName(), user, level, system_memo_type);
    //
    // 이벤트 종료레벨이 되었다.
    if (level == event_max_level)
    {   //버그수정:5864 친구초대 목표레벨에 도달했는데도 이벤트가 종료되지 않는버그 수정
        this->OnEndEvent(user);
    };
}

//보상 정보를 DB Proxy 에 전송한다.
void ns_world::FriendInvitationInWorld::ChangeEvent::SendRewardInfo(
    CHARGUID char_guid, const util::StrArrayForCharName& char_name,
    ns_world::WorldUser* sender, LEVELTYPE level, eSYSTEM_MEMO_TYPE memo_type)
{
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        return;
    };

    MSG_DW_FRIEND_INVITATIONRELATION_REWARDINSERT_CMD msg_cmd;
    msg_cmd.m_dwKey = 0; // 시스템으로부터의 쪽지
    msg_cmd.m_SendGuid = char_guid;
    msg_cmd.m_RevGuid = sender->GetCharGuid();
    msg_cmd.m_Level = level;
    // type checking
    util::StrArrayForCharName& dest_name = msg_cmd.m_ptszRecvName;
    _tcsncpy(dest_name, char_name, _countof(dest_name));
    dest_name[_countof(dest_name) - 1] = _T('\0');

    _sntprintf(msg_cmd.m_ptszMemo, _countof(msg_cmd.m_ptszMemo),
        _T("%d|%s|%d|"), memo_type, sender->GetCharName(), level);
    msg_cmd.m_ptszMemo[_countof(msg_cmd.m_ptszMemo) - 1] = _T('\0');

    dbproxy->SendPacket(&msg_cmd, sizeof(msg_cmd));
}

//이벤트 종료 처리
void ns_world::FriendInvitationInWorld::ChangeEvent::OnEndEvent(ns_world::WorldUser* user)
{
    // 도달 했으면 친구 추천 관계 코드를 일반친구로 초기화 시켜 버린당.
    if (root_->IsSetFriendsNormalSts(user) == false) { // 이 친구추천 받은 캐릭이야 ?
        return;
    };
    //원칙 적으로는 디비 작업이 성공을 해야 서버 데이타를 변경해야 하는데 
    //이 경우는 서버가 실패하던 성공하던 일단 무조건 바꿔야 하는 상태이다~
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        return;
    };
    MSG_DW_FRIEND_INVITATIONRELATION_FREE_CMD msg_cmd;
    msg_cmd.m_CharGuid = user->GetCharGuid();
    dbproxy->SendPacket(&msg_cmd, sizeof(msg_cmd));
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

