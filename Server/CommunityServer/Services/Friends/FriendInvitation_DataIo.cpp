#include "WorldServerPch.hxx"
#include "./FriendInvitation.h"

// NOTE: implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND

#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"

//==================================================================================================
// NOTE: IsSetFriendsNormalSts, 내가 추천받은 친구이면 나를 추천한 모든 캐릭터의 친구 관계 코드를
// 일반 친구로 변경한다.
bool ns_world::FriendInvitationInWorld::IsSetFriendsNormalSts(ns_world::WorldUser* user)
{
    FRIEND_INFO_MAP* friends = NULL;
    const DWORD to_char_guid = user->GetCharGuid();
    ;{
        FRIEND_TABLE::const_iterator found = observer_friend_table_.find(to_char_guid);
        if (found != observer_friend_table_.end()) {
            friends = found->second;
        };
    };
    if (friends == NULL) {
        return false;
    };

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    FRIEND_INFO_MAP::const_iterator friend_it = friends->begin();
    const FRIEND_INFO_MAP::const_iterator friend_end = friends->end();
    for (; friend_it != friend_end; ++friend_it)
    {
        const STRUCT_FRIEND_INFO& friend_info = *friend_it->second;
        //나를 추천한 친구 객체에서 목록 제거
        ns_world::WorldUser* friend_user = \
            user_manager->FindUserByCharGuid(friend_info.dwFriendGuid);
        if (friend_user == NULL) {
            continue;
        };
        ProcessFriendInvitationRelationDeletionEvent(\
            friend_user, STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED, to_char_guid);
    };

    //나를 추천한 친구 목록 제거를 서버에 알린다.
    NotifyPartialFriendInvitationList(\
        STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED,
        user,
        INVITATEFRIENDLIST_INFO::FRIENDLIST_DEL,
        static_cast<eMsgForward>(this->eMsgForward_ToAgent |
                                 this->eMsgForward_ToClient));

    friends->clear(); // 내 객체에서 나를 추천한 친구 목록 제거
    return true;
}

void ns_world::FriendInvitationInWorld::ProcessFriendInvitationRelationDeletionEvent(
    ns_world::WorldUser* user,
    STRUCT_FRIEND_DB_INFO::eNodeType selected_invitation_type,
    DWORD related_char_guid)
    //STRUCT_FRIEND_DB_INFO::eNodeType type, ns_world::WorldUser* user, DWORD FriendGUID)
{
    FRIEND_INFO_MAP* friends = NULL;
    ;{
        const FRIEND_TABLE* table = NULL;
        if (selected_invitation_type == STRUCT_FRIEND_DB_INFO::INFO_INVIAT) {
            table = &inviting_friend_table_;
        };
        if (selected_invitation_type == STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED) {
            table = &observer_friend_table_;
        };
        if (FlowControl::FCAssert(table != NULL) == false) {
            return;
        };
        FRIEND_TABLE::const_iterator found = table->find(user->GetUserGuid());
        if (found != table->end()) {
            friends = found->second;
        };
    };
    if (friends == NULL) {
        return;
    };

    FRIEND_INFO_MAP::size_type number_of_prev_deletion = friends->size();
    friends->erase(related_char_guid);
    FRIEND_INFO_MAP::size_type number_of_post_deletion = friends->size();
    if (number_of_prev_deletion == number_of_post_deletion) {
        return; // no deletion
    };
#if WORLD_CONTENTS_BLOCK
    //개발시 필요한 로그 삭제해야한다.
    SUNLOG(eDEV_LOG, "FRIEND_NORMAL Count[%u]", user->GetFriendMap(ns_world::WorldUser::FRIEND_NORMAL)->GetFriendNum());
    SUNLOG(eDEV_LOG, "FRIEND_INVIATE Count[%u]", user->GetFriendMap(ns_world::WorldUser::FRIEND_INVIATE)->GetFriendNum());
    SUNLOG(eDEV_LOG, "FRIEND_BEINVIATED Count[%u]", user->GetFriendMap(ns_world::WorldUser::FRIEND_BEINVIATED)->GetFriendNum());
#endif

    INVITATEFRIENDLIST_INFO friend_invitation_list;
    this->MakeInitializedFriendInvitationNode(\
        selected_invitation_type, user, &friend_invitation_list, related_char_guid);

    this->NotifyPreparedFriendInvitationList(user, friend_invitation_list,
        INVITATEFRIENDLIST_INFO::FRIENDLIST_DEL,
        static_cast<eMsgForward>(this->eMsgForward_ToAgent |
                                 this->eMsgForward_ToClient));
}

// NOTE: migrated from GetInvitateFriendList
// HISTORY: f110914.1L, f100624.2L
bool ns_world::FriendInvitationInWorld::LoadFriendInvitationRelationList(
    ns_world::WorldUser* user,
    STRUCT_FRIEND_DB_INFO::eNodeType selected_invitation_type,
    INVITATEFRIENDLIST_INFO* const result_list) const
{
    ZeroMemory(result_list, sizeof(*result_list));
    result_list->m_dwCharGuid = user->GetCharGuid();
    result_list->m_byListCount = 0;
    result_list->m_FriendSts = selected_invitation_type;

    const FRIEND_INFO_MAP* friends = NULL;
    ;{
        const FRIEND_TABLE* table = NULL;
        if (selected_invitation_type == STRUCT_FRIEND_DB_INFO::INFO_INVIAT) {
            table = &inviting_friend_table_;
        };
        if (selected_invitation_type == STRUCT_FRIEND_DB_INFO::INFO_BEINVIATED) {
            table = &observer_friend_table_;
        };
        if (FlowControl::FCAssert(table != NULL) == false) {
            return false;
        };
        FRIEND_TABLE::const_iterator found = table->find(user->GetUserGuid());
        if (found != table->end()) {
            friends = found->second;
        };
    };
    if (friends == NULL) {
        return false;
    };
    FRIEND_INFO_MAP::const_iterator friend_it = friends->begin();
    const FRIEND_INFO_MAP::const_iterator friend_end = friends->end();
    // NOTE: protocol specifiaction rule
    BOOST_STATIC_ASSERT(_countof(result_list->m_GuidList) == MAX_FRIEND_STAT_NUM);
    DWORD* guid_it = result_list->m_GuidList;
    int count = 0;
    for (; count < _countof(result_list->m_GuidList) && friend_it != friend_end;
         ++count, ++guid_it, ++friend_it)
    {
        const STRUCT_FRIEND_INFO& friend_info = *friend_it->second;
        *guid_it = friend_info.dwFriendGuid;
    };
    result_list->m_byListCount = static_cast<BYTE>(count);

    return true;
};

void ns_world::FriendInvitationInWorld::MakeInitializedFriendInvitationNode(
    STRUCT_FRIEND_DB_INFO::eNodeType invitation_type,
    ns_world::WorldUser* user,
    INVITATEFRIENDLIST_INFO* const result_node,
    DWORD invited_char_guid) const
{
    ZeroMemory(result_node, sizeof(*result_node));
    //
    result_node->m_dwCharGuid = user->GetCharGuid();
    result_node->m_byListCount = 1;
    result_node->m_FriendSts = invitation_type;
    result_node->m_GuidList[0] = invited_char_guid;
}

