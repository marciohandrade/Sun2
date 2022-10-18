#pragma once
#ifndef WORLDSERVER_FRIEND_INVITIATION_H
#define WORLDSERVER_FRIEND_INVITIATION_H

/*

    �ۼ���: ������
    ������: 2009-10-28

    �����Ѽ���: ���� ģ�� �ʴ� �̺�Ʈ�� �����ϱ� ���� ��Ʈ�ѷ� Ŭ���� (���� ������)
    @history
    - 2011.09.08, waverix, migrated to the renewal world server
*/

#include "Services/Friends/FriendMap.h"

//#include <FriendInvitation/FriendInvitationController.h>
//#include "ns_world::WorldUser.h"
//#include "FriendInvitationListProc.h"
//#include "FriendInvitationLevelChangedProc.h"

class FriendInvitationController;
//class FriendInvitationListProc;
class FriendInvitationLevelChangedProc;

//==================================================================================================

namespace ns_functor {
;

}; //end of namespace

//==================================================================================================

namespace ns_world {
;

struct FriendInvitationNotificationEventArg
{
    bool need_to_send_to_agent;
    bool need_to_send_to_client;
    INVITATEFRIENDLIST_INFO::FRIEND_ACTION control_action;
};

class FriendInvitationInWorld// : public FriendInvitationController
{
public:
    enum eMsgForward {
        eMsgForward_Undefined   = 0,
        eMsgForward_ToAgent     = 0x01, // NOTE: migrated from SEND_AGENTSERVER
        eMsgForward_ToClient    = 0x02, // NOTE: migrated from SEND_CLIENT
        //
        eMsgForward_Counts
    };
    //
    static FriendInvitationInWorld* Instance();
    void StartService(); // call it on prepared service

    //ģ����õ������ ģ���� �����Ǿ�����
    void OnAddInvitatedFriendCMD(ns_world::ServerSessionInWorld* const server_session,
        const MSG_AW_FRIEND_INVITATIONR_INSERT_CMD* const recv_msg, const WORD msg_size);
    //������ ������ ������
    void OnCharacterLevelUp(ns_world::ServerSessionInWorld* const server_session,
        const MSG_AW_USER_LEVELUP_CMD* const recv_msg, const WORD msg_size);
    //ģ������� ���� ������ 
    void OnFriendRemove(ns_world::ServerSessionInWorld* const server_session,
        const MSG_DW_FRIEND_DEL_ACK* const recv_msg, const WORD msg_size);
    //ģ���� ���� ������
    void OnFriendBlock(ns_world::ServerSessionInWorld* const server_session,
        const MSG_DW_FRIEND_BLOCK_ACK* const recv_msg, const WORD msg_size);
    //
    //ģ�� ����� �����Ҷ� ģ���ʴ� ��ϵ� ���� �����Ѵ�.
    void OnAddFriend(STRUCT_FRIEND_INFO& friend_info,
        const STRUCT_FRIEND_DB_INFO& friend_info_from_db, ns_world::WorldUser* user);
    //������Ʈ ������ ģ����õ ����� �����Ѵ�.
    void NotifyTotalFriendInvitationList(ns_world::WorldUser* user, eMsgForward forwarding_flags);
    // ģ�� ���� �ڵ带 ��ȯ�Ѵ�.
    // returns: STRUCT_FRIEND_DB_INFO::enumeration
    BYTE GetFriendSts(ns_world::WorldUser* user, DWORD friend_char_guid);
protected:

private:
    struct ChangeEvent; // NOTE: migrated from FriendInvitationLevelChangedProc
    typedef ns_world::FriendMap::FRIEND_INFO_MAP FRIEND_INFO_MAP;
    typedef ns_world::FriendMap::FRIEND_TABLE FRIEND_TABLE;
    //
    FriendInvitationInWorld();
    virtual ~FriendInvitationInWorld();
    static FriendInvitationInWorld* CreateInstance();
    //
    void RegisterCharStatusDataNodeAccessor(ICharStatusDataNodeAccessor* accessor);
    void PrepareFriendInvitationTable(ns_world::WorldUser* user);
    bool UpdateFriendInvitationInfo(ns_world::WorldUser* user,
        FRIEND_TABLE* const link_table, const STRUCT_FRIEND_INFO& friend_info);
    void ReleaseFriendInvitationTable(ns_world::WorldUser* user);

    //ģ�� ���踦 ���� �Ѵ�.
    void ReleaseFriendInvitationRelation(CHARGUID from_char_guid, CHARGUID to_char_guid);

    // migrated from 'FriendInvitationListProc' = {
    //      //  custom interface     ->   renewal interface
    //      IsSetFriendsNormalSts,
    //      RemoveInvitateFriend, // -> ProcessFriendInvitationRelationDeletionEvent
    //      SendInvitateFriendList,  -> NotifyPartialFriendInvitationList
    //      SendInvitateFriendList (polymorphism), NotifyPreparedFriendInvitationList
    //      GetInvitateFriendList, -> LoadFriendInvitationRelationList
    //      GetInvitateFriendList (polymorphism) -> MakeInitializedFriendInvitationNode
    // }
    //������ ���� ��� ģ�� ��õ���踦 �Ϲ� ģ�� ����� �����Ѵ�.
    bool IsSetFriendsNormalSts(ns_world::WorldUser* user);
    //ģ����õ ����Ʈ���� �����ϰ� ����� ������ �����Ѵ�.
    void ProcessFriendInvitationRelationDeletionEvent(
        ns_world::WorldUser* user,
        STRUCT_FRIEND_DB_INFO::eNodeType selected_invitation_type,
        DWORD related_char_guid);
    //������Ʈ ������ ģ����õ ����� �����Ѵ�. (W->A->G)
    void NotifyPartialFriendInvitationList(
        STRUCT_FRIEND_DB_INFO::eNodeType invitation_type, // = { INFO_INVIAT(5), INFO_BEINVIATED(6) }
        ns_world::WorldUser* user,
        INVITATEFRIENDLIST_INFO::FRIEND_ACTION action,
        eMsgForward forwarding_flags) const;
    void NotifyPreparedFriendInvitationList(
        ns_world::WorldUser* user,
        const INVITATEFRIENDLIST_INFO& friend_list,
        INVITATEFRIENDLIST_INFO::FRIEND_ACTION action,
        eMsgForward forwarding_flags) const;

    // NOTE: migrated from GetInvitateFriendList
    bool LoadFriendInvitationRelationList(
        ns_world::WorldUser* user,
        STRUCT_FRIEND_DB_INFO::eNodeType selected_invitation_type,
        INVITATEFRIENDLIST_INFO* const result_list) const;

    //������ ��Ŷ�� �����.
    //void GetInvitateFriendList(
    //    STRUCT_FRIEND_DB_INFO::eNodeType SetType,
    //    ns_world::WorldUser* user,
    //    INVITATEFRIENDLIST_INFO& FriendList);
    // NOTE: the custom interface 'GetInvitateFriendList'... unused section
    void MakeInitializedFriendInvitationNode(
        STRUCT_FRIEND_DB_INFO::eNodeType invitation_type, // = { INFO_INVIAT(5), INFO_BEINVIATED(6) }
        ns_world::WorldUser* user,
        INVITATEFRIENDLIST_INFO* const result_node,
        DWORD invited_char_guid) const;
    //
    // fields...
    FriendInvitationInWorld* redirected_this_; // redirecting
    ns_world::ICharStatusDataNodeAccessor* char_status_data_accessor_;
    FriendInvitationController* inter_controller_; // dummy to compatible custom logic
    //FriendInvitationLevelChangedProc* LevelChangedProc;

    FRIEND_TABLE inviting_friend_table_;
    FRIEND_TABLE observer_friend_table_;
    //
    static FriendInvitationInWorld* instance_static_;
    //
    friend struct FriendInvitationInWorld::ChangeEvent;
    friend class ns_world::FriendMap;
    friend class ns_world::WorldUserManager;
    __DISABLE_COPY(FriendInvitationInWorld);
};

}; //end of namespace

//==================================================================================================

inline ns_world::FriendInvitationInWorld* ns_world::FriendInvitationInWorld::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
}


#endif //WORLDSERVER_FRIEND_INVITIATION_H