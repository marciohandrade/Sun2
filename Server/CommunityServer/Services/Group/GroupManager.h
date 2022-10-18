// implemented by __KR_001355_20090608_GROUPINGSYSTEM_DATA
#pragma once
#ifndef WORLDSERVER_GROUP_MANAGER_H
#define WORLDSERVER_GROUP_MANAGER_H

//==================================================================================================

#include "Objects/WorldUser.h"

namespace ns_world {
;

class GroupManager
{
public:
    enum eSTAT_ALAM
    {
        LOG_OUT     = 1,
        LOG_IN,
        GET_MONEY,
        GET_EXP,
        MEMBER_LEVELUP,
    };
    typedef ns_world::GroupLink::GROUP_MEMBER_MAP GROUP_MEMBER_MAP;
    typedef STLX_MAP<USERGUID, GROUP_MEMBER_MAP*> GROUP_TABLE;
    //
    static GroupManager* Instance();
    //
    const STRUCT_GROUP_MEMBER_INFO* FindGroupMember(
        ns_world::WorldUser* user, DWORD char_guid) const;
    const STRUCT_GROUP_MEMBER_INFO* FindGroupMember(
        ns_world::WorldUser* user, const TCHAR* member_name) const;
    // NOTE: f110818.1L, changes 'InsertMemberInfo' to 'StartGroupMemberAdditionTransaction'.
    bool StartGroupMemberAdditionTransaction(
        ns_world::WorldUser* requester, ns_world::WorldUser* target_user);
    bool EndGroupMemberAdditionTransaction(ns_world::WorldUser* transaction_user,
        bool succeed, bool opposite_user_online_state, eGROUP_STAT opposite_user_group_stat);
    //
    static bool IsGroupTransaction(ns_world::WorldUser* user);
    static bool StartGroupTransaction(ns_world::WorldUser* user);
    static bool EndGroupTransaction(ns_world::WorldUser* user);
    //
    static bool GetAddingPreventedFlag(ns_world::WorldUser* user);
    static void ChangeAddingPreventedFlag(ns_world::WorldUser* user, bool prevent_flag);
    //
    static int GetGroupMemberCount(ns_world::WorldUser* user);
    static eGROUP_STAT GetGroupHighMember(ns_world::WorldUser* user);
    static eGROUP_STAT GetGroupLowMember(ns_world::WorldUser* user);
    static const EXPTYPE& GetBonusExp(ns_world::WorldUser* user);
    static const MONEY& GetBonusMoney(ns_world::WorldUser* user);
    static const EXPTYPE& GetAccumulatedExp(ns_world::WorldUser* user);
    static void ChangeBonusExp(ns_world::WorldUser* user, EXPTYPE changed_exp);
    static void ChangeBonusMoney(ns_world::WorldUser* user, MONEY changed_heim);
    static void AddAccumulatedExp(ns_world::WorldUser* user, EXPTYPE added_exp);

    //
    void PrepareGroupServices(ns_world::WorldUser* user);
    //- 현재 그룹 상하위 멤버 상태를 알린다.
    void UpdateGroupInfo(ns_world::WorldUser* user);
    //- 상위 멤버 상태를 Update.
    void SetGroupHighMember(ns_world::WorldUser* user, eGROUP_STAT eHighMember);
    //- 멤버를 삭제 한다.
    void ReleaseMember(ns_world::WorldUser *user);
    ////- 최초 멤버 정보를 셋팅한다.
    //bool SetMemberInfo(ns_world::WorldUser* user, const STRUCT_GROUP_MEMBER_INFO& Member);
    //- 그룹 멤버에게 정보를 알린다.
    void SendAroundMember(ns_world::WorldUser* user, eSTAT_ALAM memberStat);
    //- 그룹 멤버를 추가 한다.
    void AddMemberInfo(ns_world::WorldUser* user, const STRUCT_GROUP_MEMBER_INFO& member_info);
    //- 그룹 멤버를 삭제 한다.
    void RemoveMemberInfo(ns_world::WorldUser* user, DWORD charGuid);
    //- 접속할때 하위 멤버가 쌓아놓은 경험치를 반영하기 위해 쪽지를 만든다.
    void SendAccumEXPMemo(ns_world::WorldUser* user);
    //- 그룹내 하위 멤버의 인원 수를 구한다.
    int GetLowMemberCount(ns_world::WorldUser* user) const;
    //- 현제 시간과 인자 값간의 날짜 차이와 결과를 알아낸다.
    bool IsOverBonusDate(DWORD64 lastDate, INT chDate);
    //- 그룹 리스트를 보낸다(gameluluWorking)
    void SendGroupList(ns_world::WorldUser* user);
    //- 멤버 추가시에 추가 멤버에 대한 정보를 유저에게 알려준다.
    void SendGroupMemberInfo(ns_world::WorldUser* user,
        const STRUCT_GROUP_MEMBER_INFO& stGroupMemberInfo, eGROUP_STAT stat);
    //- 맴버 리스트를 보낸다.
    void SendMemberList(ns_world::WorldUser* user);
    //- 맴버 추가가 가능한가?
    bool CanAddGroupMember(
        ns_world::WorldUser* request_user, ns_world::WorldUser* dest_user,
        const MSG_CW_GROUP_ADD_SYN* recv_msg);

private:
    GroupManager();
    ~GroupManager();
    static GroupManager* CreateInstance();

    STRUCT_GROUP_MEMBER_INFO* AllocMemberInfo();
    void SetGroupLowMember(ns_world::WorldUser* user, eGROUP_STAT eLowMember);
    void SendGroupInfoSet(ns_world::WorldUser* user, eGROUP_STAT eHighMember, eGROUP_STAT eLowMember);
    bool SetMemeberConnectStat(ns_world::WorldUser* user, DWORD char_guid, bool connected);
    bool SendAccumEXP(DWORD userGuid, CHARGUID highGuid, EXPTYPE AccumEXP);
    void NotifyLoginState(ns_world::WorldUser* user);
    void NotifyLogoutState(ns_world::WorldUser* user);
    void NotifyEXPState(ns_world::WorldUser* user);
    void NotifyMoneyState(ns_world::WorldUser* user);
    void NotifyLevelUpState(ns_world::WorldUser* user);

private:
    GROUP_TABLE group_table_;

    // last order...
    static GroupManager* instance_static_;
    //
    __DISABLE_COPY(GroupManager);
};

}; //end of namespace

//==================================================================================================

inline ns_world::GroupManager* ns_world::GroupManager::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}

//==================================================================================================

inline int ns_world::GroupManager::GetGroupMemberCount(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    if (group_link.link_ == NULL) {
        return 0;
    };
    const GROUP_MEMBER_MAP& member_table = *group_link.link_;
    return static_cast<int>(member_table.size());
};

inline eGROUP_STAT ns_world::GroupManager::GetGroupHighMember(ns_world::WorldUser* user) 
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    return group_link.group_state_high_member_;
}

inline eGROUP_STAT ns_world::GroupManager::GetGroupLowMember(ns_world::WorldUser* user) 
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    return group_link.group_state_low_member_;
}

inline const EXPTYPE& ns_world::GroupManager::GetBonusExp(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    return group_link.stacked_bonus_exp_;
}

inline const MONEY& ns_world::GroupManager::GetBonusMoney(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    return group_link.stacked_bonus_money_;
}

inline const EXPTYPE& ns_world::GroupManager::GetAccumulatedExp(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    return group_link.accumulated_exp_;
}

inline void ns_world::GroupManager::ChangeBonusExp(ns_world::WorldUser* user, EXPTYPE changed_exp)
{
    ns_world::GroupLink* editable_group_link = \
        const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    editable_group_link->stacked_bonus_exp_ = changed_exp;
}

inline void ns_world::GroupManager::ChangeBonusMoney(ns_world::WorldUser* user, MONEY changed_heim)
{
    ns_world::GroupLink* editable_group_link = \
        const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    editable_group_link->stacked_bonus_money_ = changed_heim;
}

inline void ns_world::GroupManager::AddAccumulatedExp(ns_world::WorldUser* user, EXPTYPE added_exp)
{
    ns_world::GroupLink* editable_group_link = \
        const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    editable_group_link->accumulated_exp_ += added_exp;
}

//==================================================================================================

#endif //WORLDSERVER_GROUP_MANAGER_H