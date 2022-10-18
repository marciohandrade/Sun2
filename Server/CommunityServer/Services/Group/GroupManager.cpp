#include "WorldServerPch.hxx"
#include "./GroupManager.h"

#include <time.h>
// utility
#include <SolarDateTime.h>
// program common
#include <Struct.h>
#include <PacketStruct_DW.h>
#include <PacketStruct_AW.h>

#include "WorldServer.h"

#include "Sessions/ServerSessionInWorld.h"

#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"

//==================================================================================================

ns_world::GroupManager* ns_world::GroupManager::instance_static_ = NULL;

ns_world::GroupManager* ns_world::GroupManager::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(ns_world::GroupManager** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            if (*static_table_) {
                TAllocDelete(ns_world::GroupManager, *static_table_);
            }
            *static_table_ = NULL;
        }
        ns_world::GroupManager** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    ns_world::GroupManager* provider = TAllocNew(ns_world::GroupManager);
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

//==================================================================================================

ns_world::GroupManager::GroupManager()
{
}

ns_world::GroupManager::~GroupManager()
{
    ;{
        GROUP_TABLE::const_iterator it = group_table_.begin();
        const GROUP_TABLE::const_iterator end = group_table_.end();
        for (; it != end; ++it)
        {
            GROUP_MEMBER_MAP* member_table = it->second;
            TAllocDelete(GROUP_MEMBER_MAP, member_table);
        };
    };
}

//==================================================================================================

bool ns_world::GroupManager::IsGroupTransaction(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    return group_link.transaction_;
}

bool ns_world::GroupManager::StartGroupTransaction(ns_world::WorldUser* user)
{
    ns_world::GroupLink* const editable_group_link = \
        const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    if (editable_group_link->transaction_ != false) {
        return false;
    };
    editable_group_link->transaction_ = true;
    return true;
}

bool ns_world::GroupManager::EndGroupTransaction(ns_world::WorldUser* user)
{
    ns_world::GroupLink* const editable_group_link = \
        const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    if (editable_group_link->transaction_ == false) {
        return false;
    };
    editable_group_link->transaction_ = false;
    if (editable_group_link->transaction_data_.member_addition_data)
    {
        TAllocDelete(STRUCT_GROUP_MEMBER_INFO, \
            editable_group_link->transaction_data_.member_addition_data);
        editable_group_link->transaction_data_.member_addition_data = NULL;
    };
    return true;
}

bool ns_world::GroupManager::GetAddingPreventedFlag(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    return group_link.adding_prevented_;
}

void ns_world::GroupManager::ChangeAddingPreventedFlag(ns_world::WorldUser* user, bool prevent_flag)
{
    ns_world::GroupLink* const editable_group_link = \
        const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    editable_group_link->adding_prevented_ = prevent_flag;
}

//==================================================================================================

void ns_world::GroupManager::PrepareGroupServices(ns_world::WorldUser* user)
{
    ns_world::GroupLink* const editable_group_link = \
        const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    GROUP_MEMBER_MAP* member_table = editable_group_link->link_;
    if (member_table != NULL)
    {
        member_table->clear();
        // TODO: f110818.1L, check other data fields...
        return;
    };

    const USERGUID user_guid = user->GetUserAccountInfo().user_guid;
    GROUP_TABLE::const_iterator found = group_table_.find(user_guid);
    if (found != group_table_.end())
    {
        member_table = found->second;
        member_table->clear();
        editable_group_link->link_ = member_table;
    }
    else
    {
        editable_group_link->link_ = TAllocNew(GROUP_MEMBER_MAP);
        group_table_.insert(GROUP_TABLE::value_type(user_guid, editable_group_link->link_));
    };
}

//==================================================================================================

bool ns_world::GroupManager::CanAddGroupMember(
    ns_world::WorldUser* request_user, ns_world::WorldUser* dest_user,
    const MSG_CW_GROUP_ADD_SYN* recv_msg)
{
    MSG_CW_GROUP_ADD_NAK msg_nak;
    msg_nak.m_byResult = msg_nak.NONE;
    //
    struct FailMessageSender {
        FailMessageSender(ns_world::WorldUser* requester, MSG_CW_GROUP_ADD_NAK* msg_nak)
            : requester_(requester), msg_nak_(msg_nak) {}
        ~FailMessageSender()
        {
            if (msg_nak_->m_byResult == msg_nak_->NONE) {
                return;
            };
            assert(msg_nak_->m_byResult > msg_nak_->ERR_DEFAULT &&
                   msg_nak_->m_byResult < msg_nak_->MAX_COUNTS);
            requester_->SendPacket(msg_nak_, sizeof(*msg_nak_));
        };
        ns_world::WorldUser* const requester_;
        MSG_CW_GROUP_ADD_NAK* const msg_nak_;
    private:
        FailMessageSender(const FailMessageSender&) : requester_(0), msg_nak_(0) {}
        void operator=(const FailMessageSender&) {}
    } fail_msg_sender(request_user, &msg_nak);
    //
    if (request_user == NULL || dest_user == NULL)
    {
        msg_nak.m_byResult = msg_nak.ERR_DEFAULT;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Not found user");
        return false;
    };
    if (request_user->GetGroupInfo().IsTransaction() != false)
    {
        //-- �ش� ������ �ٸ� ������ �׷� ��û�� �õ��� ����
        msg_nak.m_byResult = msg_nak.ALREADY_GROUP_REQ;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Transaction error", recv_msg->ptszMemeberName);
        return false;
    };
    if (dest_user->GetGroupInfo().IsAddingPrevented() != false)
    {
        //-- �׷� �ɹ��� �׷� ��û �ź� ����� Ȱ��ȭ ���״�.
        msg_nak.m_byResult = msg_nak.ON_TOGGLE_ADDBLOCK;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Ad request block user", recv_msg->ptszMemeberName);
        return false;
    };
    if (dest_user->GetGroupInfo().IsTransaction() != false)
    {
        //-- ���濡�� ������ �̹� ��û ���̴�.
        msg_nak.m_byResult = msg_nak.ALREADY_GROUP_REQ;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Already group request", recv_msg->ptszMemeberName);
        return false;
    };
    if (!_tcsnicmp(request_user->GetCharName(), recv_msg->ptszMemeberName, MAX_CHARNAME_LENGTH))
    {
        //-- �߰��Ϸ��� �׷� �ɹ��� �̸��� �ڽ��� �̸��� ��
        msg_nak.m_byResult = msg_nak.NOT_ADD_ME;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Not request to myself", recv_msg->ptszMemeberName);
        return false;
    };
    if (dest_user->GetConnectedCharInfo().char_level >=
        request_user->GetConnectedCharInfo().char_level)
    {
        //-- �߰� �Ϸ��� �׷� �ɹ��� ������ ���ų� ���� ��� ����
        msg_nak.m_byResult = msg_nak.NOT_MATCHING_LEVEL;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Not matching level", recv_msg->ptszMemeberName);
        return false;
    };
#if defined(_CHINA)
    if (request_user->GetConnectedCharInfo().char_level < HIGH_MEMBER_LEVEL || 
        dest_user->GetConnectedCharInfo().char_level < LOW_MEMBER_LEVEL)
    {
        //-- �߱� ��û���� ������ 80�̻� ���� �ɹ�, 10�̻� ���� �ɹ� ����
        msg_nak.m_byResult = msg_nak.NOT_MATCHING_LEVEL;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Not matching level", recv_msg->ptszMemeberName);
        return false;
    };
#endif
    if (GetGroupHighMember(dest_user) == GROUP_HAVE_ONLINE_HIGH ||
        GetGroupHighMember(dest_user) == GROUP_HAVE_OFFLINE_HIGH)
    {
        //-- �߰� �Ϸ��� �ɸ��Ͱ� ���� ����� ������ �ִ°�?
        msg_nak.m_byResult = msg_nak.ALREADY_GROUP;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Already had high member", recv_msg->ptszMemeberName);
        return false;
    };
    if (FindGroupMember(request_user, recv_msg->ptszMemeberName))
    {
        //-- �ɹ��� �׷쿡 ���� �Ҷ�
        msg_nak.m_byResult = msg_nak.ALREADY_GROUP;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Already Member", recv_msg->ptszMemeberName);
        return false;
    };
    if (GetLowMemberCount(request_user) >= MAX_GROUP_MEMBER_LOW_MAX)
    {
        //-- �� �׷�� 6�� ���� �ɹ��� 5����� �����ϴ�.
        msg_nak.m_byResult = msg_nak.GROUP_OVERFLOW;
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s OverFlow member count", recv_msg->ptszMemeberName);
        return false;
    };

    return true;
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO ���� ����
void ns_world::GroupManager::SendGroupMemberInfo(
    ns_world::WorldUser* user, 
    const STRUCT_GROUP_MEMBER_INFO& group_member_info, 
    eGROUP_STAT group_stat)
{
    MSG_CW_GROUP_ADD_ACK msg_ack;
    if (STRUCT_GROUP_MEMBER_PACKET_INFO* const dest_info = &msg_ack.m_MemberInfo) {
        *dest_info = group_member_info.GroupMemberInfo;
    };
#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    msg_ack.charGUID = group_member_info.charGUID;
#endif
    msg_ack.m_GroupInfo = group_stat;

    user->SendPacket(&msg_ack, sizeof(msg_ack));
}

//�׷� �ɹ� �ڷ� �������� Guid�� ������� ������ �����´�.
const STRUCT_GROUP_MEMBER_INFO*
ns_world::GroupManager::FindGroupMember(ns_world::WorldUser* user, DWORD char_guid) const
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    if (group_link.link_ == NULL) {
        return NULL;
    };

    const GROUP_MEMBER_MAP& member_table = *group_link.link_;
    GROUP_MEMBER_MAP::const_iterator found = member_table.find(char_guid);
    if (found == member_table.end()) {
        return NULL;
    };

    const STRUCT_GROUP_MEMBER_INFO* member_info = &found->second;
    return member_info;
}

//�׷� �ɹ� �ڷ� �������� �̸��� ������� �ش� ������ ã�´�.
const STRUCT_GROUP_MEMBER_INFO*
ns_world::GroupManager::FindGroupMember(ns_world::WorldUser* user, const TCHAR* member_name) const
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    if (group_link.link_ == NULL) {
        return NULL;
    };
    // TODO: change search method.
    const GROUP_MEMBER_MAP& member_table = *group_link.link_;
    FOREACH_CONTAINER(const GROUP_MEMBER_MAP::value_type& node, member_table, GROUP_MEMBER_MAP)
    {
        const STRUCT_GROUP_MEMBER_INFO& member_info = node.second;
        if (_tcsnicmp(member_info.GroupMemberInfo.ptszMemberName,
                      member_name, MAX_CHARNAME_LENGTH + 1) == 0)
        {
            return &member_info;
        }
    };
    return NULL;
}

//������ ������ �ִ� ��� �� ����
int ns_world::GroupManager::GetLowMemberCount(ns_world::WorldUser* user) const
{ 
    //MAX���� ���� ��������� ��û�� �����ϰ� �ȴ�.
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    if (group_link.link_ == NULL) {
        return -1;
    };
    int number_of_low_group_members = 0;
    const GROUP_MEMBER_MAP& member_table = *group_link.link_;
    FOREACH_CONTAINER(const GROUP_MEMBER_MAP::value_type& node, member_table, GROUP_MEMBER_MAP)
    {
        const STRUCT_GROUP_MEMBER_INFO& member_info = node.second;
        if (member_info.groupSTAT == GROUP_LEVEL_LOW) {
            ++number_of_low_group_members;
        };
    };
    return number_of_low_group_members;
}

void ns_world::GroupManager::AddMemberInfo(
    ns_world::WorldUser* user, const STRUCT_GROUP_MEMBER_INFO& group_member)
{
    if (user == NULL) {
        return;
    };
    if (this->GetGroupMemberCount(user) >= MAX_GROUP_MEMBER)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]OverFlow, WorldUser = %s, MemberGuid = %u" ,
               user->GetCharName(), group_member.charGUID);
        return;
    }

    if (this->FindGroupMember(user, group_member.charGUID) != NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               "["__FUNCTION__"]can't insert member. already exists. "
               "WorldUser = %s, MemberGuid = %u",
               user->GetCharName(), group_member.charGUID);
        return;
    };

    GROUP_MEMBER_MAP* const member_table = user->GetGroupInfo().link_;
    member_table->operator[](group_member.charGUID) = group_member;
    return;
}

void ns_world::GroupManager::SendMemberList(ns_world::WorldUser* user)
{
    const int member_count = GetGroupMemberCount(user);
    int low_member_count = GetLowMemberCount(user);

    //�ִ� �ɹ������� �������� ������ ���ٰ� �ؼ� ���񽺸� ������� ����.
    //�ش� �κ��ľ��� ���� �α׸� ����� ���� �׷���� ������ ���� ���
    //�׷� ������ ���� ���ϰ� �ϰ� �޼����� ����� ������� �ذ��Ѵ�.
    assert(member_count <= MAX_GROUP_MEMBER && member_count >= 0);
    if (member_count > MAX_GROUP_MEMBER || member_count < 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]WorldUser[%s]Groupmember count is incorrect = %d",
               user->GetCharName(), member_count);
    };
    assert(low_member_count <= MAX_GROUP_MEMBER_LOW_MAX && low_member_count >= 0);
    if (low_member_count > MAX_GROUP_MEMBER_LOW_MAX || low_member_count < 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]WorldUser[%s]Group Low Member Num Over = %d",
               user->GetCharName(), low_member_count);
    };
    // CHANGES: f110811.5L, fixed buffer overrun occurrance possiblity.
    // NOTE: �ɹ����� 0���̶�� �ص� ���� ��Ŷ�� Ŭ���̾�Ʈ���� ������.
    MSG_CW_GROUP_GROUPLIST_ANS msg_ack;
    msg_ack.byGroupMemberNum = 0;

    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    const GROUP_MEMBER_MAP* member_table = group_link.link_;
    if (member_table == NULL) {
        user->SendPacket(&msg_ack, msg_ack.GetSize());
        return;
    };
    int counter = 0;
    GROUP_MEMBER_MAP::const_iterator it = member_table->begin();
    const GROUP_MEMBER_MAP::const_iterator end = member_table->end();
    for (; counter < _countof(msg_ack.sGroupMemberInfo) && it != end; ++counter, ++it)
    {
        const STRUCT_GROUP_MEMBER_INFO& group_info = it->second;
        msg_ack.sGroupMemberInfo[counter] = group_info;
    };
    msg_ack.byGroupMemberNum = static_cast<uint8_t>(counter);
    user->SendPacket(&msg_ack, msg_ack.GetSize());

    SUNLOG(eFULL_LOG, "["__FUNCTION__"][%s][Guid:%u] Group List Send[Count:%d]", 
           user->GetCharName(), user->GetUserGuid(), counter);
}

// NOTE: f110818.1L, changes 'InsertMemberInfo' to 'StartGroupMemberAdditionTransaction'.
bool ns_world::GroupManager::StartGroupMemberAdditionTransaction(
    ns_world::WorldUser* requester, ns_world::WorldUser* target_user)
{
    ns_world::GroupLink* const editable_group_link = \
        const_cast<ns_world::GroupLink*>(&requester->GetGroupInfo());
    ns_world::GroupLink* const target_editable_group_link = \
        const_cast<ns_world::GroupLink*>(&target_user->GetGroupInfo());
    if (editable_group_link->transaction_ || target_editable_group_link->transaction_) {
        return false;
    };
    if (FlowControl::FCAssert(editable_group_link->link_ != NULL) == false) {
        return false;
    };
    if (FlowControl::FCAssert(target_editable_group_link->link_ != NULL) == false) {
        return false;
    };
    if (editable_group_link->transaction_data_.member_addition_data == NULL) {
        editable_group_link->transaction_data_.member_addition_data = \
            TAllocNew(STRUCT_GROUP_MEMBER_INFO);
    }
    else {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=unexpected transaction states,%d"), 1);
    };
    if (target_editable_group_link->transaction_data_.member_addition_data == NULL) {
        target_editable_group_link->transaction_data_.member_addition_data = \
            TAllocNew(STRUCT_GROUP_MEMBER_INFO);
    }
    else {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=unexpected transaction states,%d"), 2);
    };
    STRUCT_GROUP_MEMBER_INFO* const member_info = \
        editable_group_link->transaction_data_.member_addition_data;
    ZeroMemory(member_info, sizeof(*member_info));
    STRUCT_GROUP_MEMBER_INFO* const target_member_info = \
        target_editable_group_link->transaction_data_.member_addition_data;
    ZeroMemory(target_member_info, sizeof(*target_member_info));
    // cross alignment
    //  (target_member_info <- char_info)
    //  (member_info <- target_char_info)
    const ns_world::ConnectedCharInfo& char_info = requester->GetConnectedCharInfo();
    ;{
        target_member_info->accumEXP = 0;
        target_member_info->charGUID = char_info.char_guid;
        target_member_info->groupSTAT = GROUP_LEVEL_LOW;
        if (STRUCT_GROUP_MEMBER_PACKET_INFO* sub_data = &target_member_info->GroupMemberInfo)
        {
            sub_data->bOnline = true;
            sub_data->byChannel = static_cast<uint8_t>(\
                requester->GetUserAccountInfo().channel->GetChannelID());
            sub_data->byClass = char_info.char_class;
            sub_data->Level = char_info.char_level;
            _tcsncpy(sub_data->ptszMemberName, char_info.char_name,
                     _countof(sub_data->ptszMemberName));
            sub_data->ptszMemberName[_countof(sub_data->ptszMemberName) - 1] = _T('\0');
        };
        target_editable_group_link->transaction_ = true;
    };
    const ns_world::ConnectedCharInfo& target_char_info = target_user->GetConnectedCharInfo();
    ;{
        member_info->accumEXP = 0;
        member_info->charGUID = target_char_info.char_guid;
        member_info->groupSTAT = GROUP_LEVEL_HIGH;
        if (STRUCT_GROUP_MEMBER_PACKET_INFO* sub_data = &member_info->GroupMemberInfo)
        {
            sub_data->bOnline = true;
            sub_data->byChannel = static_cast<uint8_t>(\
                target_user->GetUserAccountInfo().channel->GetChannelID());
            sub_data->byClass = target_char_info.char_class;
            sub_data->Level = target_char_info.char_level;
            _tcsncpy(sub_data->ptszMemberName, target_char_info.char_name,
                     _countof(sub_data->ptszMemberName));
            sub_data->ptszMemberName[_countof(sub_data->ptszMemberName) - 1] = _T('\0');
        };
        editable_group_link->transaction_ = true;
    };
    return true;
};

bool ns_world::GroupManager::EndGroupMemberAdditionTransaction(
    ns_world::WorldUser* transaction_user, bool succeed,
    bool opposite_user_online_state, eGROUP_STAT opposite_user_group_stat)
{
    ns_world::GroupLink* const editable_group_link = \
        const_cast<ns_world::GroupLink*>(&transaction_user->GetGroupInfo());
    if (editable_group_link->link_ == NULL)
    {   // clear & realigned state?
        assert(!"unexpected state");
        return false;
    };
    if (editable_group_link->transaction_ == false ||
        editable_group_link->transaction_data_.member_addition_data == NULL)
    {
        assert(!"unexpected state");
        return false;
    };
    editable_group_link->transaction_ = false;

    STRUCT_GROUP_MEMBER_INFO member_info = \
        *editable_group_link->transaction_data_.member_addition_data;
    ;{
        TAllocDelete(STRUCT_GROUP_MEMBER_INFO,
            editable_group_link->transaction_data_.member_addition_data);
        editable_group_link->transaction_data_.member_addition_data = NULL;
    };
    if (succeed == false) {
        return true;
    };

    member_info.GroupMemberInfo.bOnline = opposite_user_online_state;
    this->AddMemberInfo(transaction_user, member_info);
    //
    // Synchronization
    // Send group member information...
    //
    this->UpdateGroupInfo(transaction_user);
    //_NA_0_20100527_ADD_CHAR_RELATION_MEMO ���� ����
    this->SendGroupMemberInfo(transaction_user, member_info, opposite_user_group_stat);
    //
    return true;
}

void ns_world::GroupManager::RemoveMemberInfo(ns_world::WorldUser* user, DWORD target_char_guid)
{
    ns_world::GroupLink* editable_group_link = \
        const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    if (editable_group_link->link_ == NULL) {
        return;
    };

    GROUP_MEMBER_MAP* const member_table = editable_group_link->link_;
    GROUP_MEMBER_MAP::iterator found = member_table->find(target_char_guid);
    if (found == member_table->end())
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found target character, ")
               _T("actor:target={'%s',%d:%d}|"),
               user->GetCharName(), user->GetConnectedCharInfo().char_guid, target_char_guid);
        return;
    };

    member_table->erase(found);
}

//�׷� �ɹ��鿡�� �α����� ����� �˸���. �ɹ����� ��/�������� �����͵� �ٲ��ش�.
void ns_world::GroupManager::NotifyLoginState(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    const GROUP_MEMBER_MAP* member_table = group_link.link_;
    if (member_table == NULL) {
        return;
    };

    MSG_CW_GROUP_LOGIN_BRD msg_brd;
    strncpy(msg_brd.ptszMemeberName, user->GetCharName(), _countof(msg_brd.ptszMemeberName));
    msg_brd.ptszMemeberName[_countof(msg_brd.ptszMemeberName) - 1] = '\0';
    msg_brd.byChannel = static_cast<uint8_t>(user->GetUserAccountInfo().channel->GetChannelID());
    //
    const DWORD actor_char_guid = user->GetCharGuid();
    //int counter = 0;
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    GROUP_MEMBER_MAP::const_iterator it = member_table->begin();
    const GROUP_MEMBER_MAP::const_iterator end = member_table->end();
    for (; /*counter < MAX_GROUP_MEMBER &&*/ it != end; /*++counter,*/ ++it)
    {
        DWORD char_guid = it->first;
        ns_world::WorldUser* member = user_manager->FindUserByCharGuid(char_guid);
        if (member == NULL) {
            continue;
        };

        SetMemeberConnectStat(member, actor_char_guid, true);
        UpdateGroupInfo(member);
        member->SendPacket(&msg_brd, sizeof(msg_brd));
    };
}

//�׷� �ɹ��鿡�� �α׾ƿ� �� ����� �˸���. �ɹ����� ��/�������� �����͵� �ٲ��ش�.
void ns_world::GroupManager::NotifyLogoutState(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    const GROUP_MEMBER_MAP* member_table = group_link.link_;
    if (member_table == NULL) {
        return;
    };

    MSG_CW_GROUP_LOGOUT_BRD msg_brd;
    strncpy(msg_brd.ptszMemeberName, user->GetCharName(), _countof(msg_brd.ptszMemeberName));
    msg_brd.ptszMemeberName[_countof(msg_brd.ptszMemeberName) - 1] = '\0';
    //
    const DWORD actor_char_guid = user->GetCharGuid();
    //int counter = 0;
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    GROUP_MEMBER_MAP::const_iterator it = member_table->begin();
    const GROUP_MEMBER_MAP::const_iterator end = member_table->end();
    for (; /*counter < MAX_GROUP_MEMBER &&*/ it != end; /*++counter,*/ ++it)
    {
        DWORD char_guid = it->first;
        ns_world::WorldUser* member = user_manager->FindUserByCharGuid(char_guid);
        if (member == NULL) {
            continue;
        };

        SetMemeberConnectStat(member, actor_char_guid, false);
        UpdateGroupInfo(member);
        member->SendPacket(&msg_brd, sizeof(msg_brd));
    };
}

//���� �ɹ����� ������ ����ġ�� ���ʽ��� �ش�. 
//���� �ɹ��� �α׾ƿ� ����(�ɸ��� ����â ����)��� ����ġ�� �����ϰ� �ȴ�.
void ns_world::GroupManager::NotifyEXPState(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    const GROUP_MEMBER_MAP* member_table = group_link.link_;
    if (member_table == NULL || member_table->empty()) {
        return;
    };
    // WARNING: f110811.5L, ... must be checked whether 'user' is already connected by character
    const ns_world::ConnectedCharInfo& actor_char_info = user->GetConnectedCharInfo();

    MSG_AW_STATUS_GROUP_ADDEXP_CMD msg_cmd;
    ;{
        //msg_cmd.m_CharGuid = high_group_member_info.charGUID;
        msg_cmd.m_Amount = group_link.stacked_bonus_exp_;
        _tcsncpy(msg_cmd.m_szCharName, user->GetCharName(), 
                 _countof(msg_cmd.m_szCharName));
        msg_cmd.m_szCharName[_countof(msg_cmd.m_szCharName) - 1] = '\0';
    };

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    GROUP_MEMBER_MAP::const_iterator it = member_table->begin();
    const GROUP_MEMBER_MAP::const_iterator end = member_table->end();
    for (; it != end; ++it)
    {
        const STRUCT_GROUP_MEMBER_INFO& high_group_member_info = it->second;
        ns_world::WorldUser* high_memeber_user = \
            user_manager->FindUserByCharGuid(high_group_member_info.charGUID);
    #ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY 
        if ((high_group_member_info.groupSTAT != GROUP_LEVEL_LOW) || 
            (actor_char_info.char_level <= high_group_member_info.GroupMemberInfo.Level))
        {
            //-- ���� ����̰ų� ������ ������
            continue;
        };
    #else
        if ((high_group_member_info.groupSTAT != GROUP_LEVEL_HIGH) || 
            (actor_char_info.char_level >= high_group_member_info.GroupMemberInfo.Level))
        {
            //-- ���� ����̰ų� ������ ������
            continue;
        };
    #endif

        if ((high_memeber_user == NULL) ||
            (high_memeber_user->GetConnectedCharInfo().zone_type == eZONETYPE_CHARSELECT))
        {
            //-- ����� ���ų� �ɸ��� ����â �̸�
            SendAccumEXP(actor_char_info.user_guid, high_group_member_info.charGUID,
                         group_link.stacked_bonus_exp_);
            continue;
        };
        // CHANGES: f110811.5L, ... changes flow control 'return' to 'continue'
        if (high_memeber_user->EnabledSendMessage() == false) {
            continue;
        };
        //
        ;{  // set up 'high_group_member_info' information
            // NOTE: waverix said, "I don't understand"
            msg_cmd.m_CharGuid = high_group_member_info.charGUID;
        };
        high_memeber_user->SendPacket(&msg_cmd, sizeof(msg_cmd));
        //
    };
}

//���� �ɹ����� ���ʽ� �ݾ��� �ش�.���� �ɹ��� ���������̸� �����Ѵ�.
void ns_world::GroupManager::NotifyMoneyState(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    const GROUP_MEMBER_MAP* member_table = group_link.link_;
    if (member_table == NULL || member_table->empty()) {
        return;
    };

    // WARNING: f110811.5L, ... must be checked whether 'user' is already connected by character
    const ns_world::ConnectedCharInfo& actor_char_info = user->GetConnectedCharInfo();

    MSG_AW_STATUS_GROUP_ADDMONEY_CMD msg_cmd;
    ;{
        //send_msg.m_CharGuid = low_group_member_Info.charGUID;
        msg_cmd.m_Amount = group_link.stacked_bonus_money_;
        _tcsncpy(msg_cmd.m_szCharName, user->GetCharName(),
                 _countof(msg_cmd.m_szCharName));
        msg_cmd.m_szCharName[_countof(msg_cmd.m_szCharName) - 1] = '\0';
    };

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    GROUP_MEMBER_MAP::const_iterator it = member_table->begin();
    const GROUP_MEMBER_MAP::const_iterator end = member_table->end();
    for (; it != end; ++it)
    {
        const STRUCT_GROUP_MEMBER_INFO& low_group_member_info = it->second;
        ns_world::WorldUser* low_member_user = \
            user_manager->FindUserByCharGuid(low_group_member_info.charGUID);
        if (low_member_user == NULL) {
            continue;
        };
    #ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY 
        if ((low_group_member_info.groupSTAT != GROUP_LEVEL_HIGH) || 
            (actor_char_info.char_level >= low_member_user->GetConnectedCharInfo().char_level))    
        {
            continue;
        }
    #else
        if ((low_group_member_info.groupSTAT != GROUP_LEVEL_LOW) || 
            (actor_char_info.char_level <= low_member_user->GetConnectedCharInfo().char_level))    
        {
            continue;
        }
    #endif
        if (low_member_user->EnabledSendMessage() == false) {
            continue;
        };

        ;{  // set up 'low_group_member_info' information
            msg_cmd.m_CharGuid = low_group_member_info.charGUID;
        };
        low_member_user->SendPacket(&msg_cmd, sizeof(msg_cmd));
    };
}

//�ɹ��鿡�� ������ ����� �˸���. ���� �ɹ��� ������ �������� ���̻� �׷� ȿ���� ���ӵ� �� 
//������ Ŭ���̾�Ʈ ������ �˷��ش�.
void ns_world::GroupManager::NotifyLevelUpState(ns_world::WorldUser* user)
{
    const ns_world::GroupLink& group_link = user->GetGroupInfo();
    const GROUP_MEMBER_MAP* member_table = group_link.link_;
    if (member_table == NULL || member_table->empty()) {
        return;
    };

    // WARNING: f110811.5L, ... must be checked whether 'user' is already connected by character
    const ns_world::ConnectedCharInfo& actor_char_info = user->GetConnectedCharInfo();

    MSG_CW_GROUP_LEVELUP_BRD msg_brd;
    ;{
        _tcsncpy(msg_brd.ptszMemeberName, user->GetCharName(),
                 _countof(msg_brd.ptszMemeberName));
        msg_brd.ptszMemeberName[_countof(msg_brd.ptszMemeberName) - 1] = '\0';
        // WARNING: f110811.5L, sizeof(MSG_CW_GROUP_LEVELUP_BRD::charLevel) == 1
        msg_brd.charLevel = actor_char_info.char_level;
    };

    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    GROUP_MEMBER_MAP::const_iterator it = member_table->begin();
    const GROUP_MEMBER_MAP::const_iterator end = member_table->end();
    for (; it != end; ++it)
    {
        const STRUCT_GROUP_MEMBER_INFO& group_member_info = it->second;
        ns_world::WorldUser* member_user = \
            user_manager->FindUserByCharGuid(group_member_info.charGUID);
        if (member_user == NULL) {
            continue;
        };

        if (member_user->EnabledSendMessage() == false) {
            continue;
        };
        member_user->SendPacket(&msg_brd, sizeof(msg_brd));
    };
}

void ns_world::GroupManager::SendAroundMember(ns_world::WorldUser* user, eSTAT_ALAM member_stat)
{
    int group_num = GetGroupMemberCount(user);
    if (group_num <= 0) {
        return;
    };    
    assert(group_num <= MAX_GROUP_MEMBER);
    if (group_num > MAX_GROUP_MEMBER)
    {
        //�ſ� Ư���� ��Ȳ�̳� �ش� ������ ������� ����. Ȯ���� ��ó��
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]%s Member count is Over[count:%d]",
               user->GetCharName(), group_num);
    };

    switch (member_stat)
    {
    case LOG_OUT:
        {
            NotifyLogoutState(user);
            break;
        }
    case LOG_IN:
        {
            NotifyLoginState(user);
            break;
        }
    case GET_MONEY:
        {
            NotifyMoneyState(user);
            break;
        }
    case GET_EXP:
        {
            NotifyEXPState(user);
            break;
        }
    case MEMBER_LEVELUP:
        {
            NotifyLevelUpState(user);
            break;
        }
    }; //end 'switch'
}

//������ ������ �ִ� GroupMemberMap���� �ش� ������ �¶��� �������� ������ �����Ѵ�.
bool ns_world::GroupManager::SetMemeberConnectStat(
    ns_world::WorldUser* user, DWORD char_guid, bool connected)
{
    STRUCT_GROUP_MEMBER_INFO* const editable_member_info =
        const_cast<STRUCT_GROUP_MEMBER_INFO*>(FindGroupMember(user, char_guid));
    if (editable_member_info == NULL) {
        return false;
    };

    editable_member_info->GroupMemberInfo.bOnline = connected;

    return true;
}
/*
//�ʵ� ����� �ɸ����� �׷� ������ �����Ѵ�.
bool ns_world::GroupManager::SetMemberInfo(
    ns_world::WorldUser* user, const STRUCT_GROUP_MEMBER_INFO& group_member_info)
{
    ns_world::GroupLink* const editable_group_link = \
        const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    GROUP_MEMBER_MAP* member_table = editable_group_link->link_;
    if (member_table == NULL)
    {
        InitializeGroupMemberTable(user);
        member_table = editable_group_link->link_;
    };

    if (GetGroupMemberCount(user) > MAX_GROUP_MEMBER)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]OverFlow, WorldUser = %s, MemberGuid = %u",
               user->GetCharName(), group_member_info.charGUID); 
    }

    if (FindGroupMember(user, group_member_info.charGUID) == NULL)
    {
        AddMemberInfo(user, group_member_info);
        return true;
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Already Group, WorldUser[%s], MemberGuid = %u" , 
            user->GetCharName().c_str(), group_member_info.charGUID); 

        return false;
    }
}
*/

//������ �׷� �ɹ��� �ʱ�ȭ �Ҷ�(���� ������ ���� ���)
void ns_world::GroupManager::ReleaseMember(ns_world::WorldUser* user)
{
    ns_world::GroupLink* group_link = const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    if (group_link->link_ == NULL) {
        return;
    };

    group_table_.erase(user->GetUserAccountInfo().user_guid);
    TAllocDelete(GROUP_MEMBER_MAP, group_link->link_);
    group_link->link_ = NULL;
    if (group_link->transaction_data_.member_addition_data) {
        TAllocDelete(STRUCT_GROUP_MEMBER_INFO, group_link->transaction_data_.member_addition_data);
        group_link->transaction_data_.member_addition_data = NULL;
    };

    group_link->Clear();
    group_link->user_ = user;
}

//������ ���� �׷� ������ ���� ������ ����ȭ ��Ų��
void ns_world::GroupManager::UpdateGroupInfo(ns_world::WorldUser* user)
{
    ns_world::GroupLink* group_link = const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    ;{
        group_link->group_state_high_member_ = GROUP_HAVE_NONE;
        group_link->group_state_low_member_ = GROUP_HAVE_NONE;
        if (group_link->link_ == NULL) {
            return;
        };
    };

    const GROUP_MEMBER_MAP& member_table = *group_link->link_;
    GROUP_MEMBER_MAP::const_iterator it = member_table.begin();
    const GROUP_MEMBER_MAP::const_iterator end = member_table.end();
    for (; it != end; ++it)
    {
        const STRUCT_GROUP_MEMBER_INFO& group_member_info = it->second;
        const STRUCT_GROUP_MEMBER_PACKET_INFO& group_message_info = \
            group_member_info.GroupMemberInfo;

        ;;;; if (group_member_info.groupSTAT == GROUP_LEVEL_HIGH)
        {
            group_link->group_state_high_member_ = (group_message_info.bOnline != false)
                ? GROUP_HAVE_ONLINE_HIGH : GROUP_HAVE_OFFLINE_HIGH;
        }
        else if (group_member_info.groupSTAT == GROUP_LEVEL_LOW &&
                 group_link->group_state_low_member_ != GROUP_HAVE_ONLINE_LOW)
        {
            group_link->group_state_high_member_ = (group_message_info.bOnline != false)
                ? GROUP_HAVE_ONLINE_LOW : GROUP_HAVE_OFFLINE_LOW;
        };

        //������ ����� ���� �¶������μ��� �Ǿ������� ���̻� ���캼 �ʿ� ����.
        if (group_link->group_state_high_member_ == GROUP_HAVE_ONLINE_HIGH &&
            group_link->group_state_low_member_ == GROUP_HAVE_ONLINE_LOW)
        {
            break;
        };
    }; //end 'for'

    SendGroupInfoSet(user, group_link->group_state_high_member_,
                           group_link->group_state_low_member_);
}

//���� ���� ��� ������ �����Ѵ�(���� ���� ����� ����ȭ)
void ns_world::GroupManager::SetGroupHighMember(ns_world::WorldUser* user, eGROUP_STAT eHighMember)
{
    ns_world::GroupLink* group_link = const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    group_link->group_state_high_member_ = eHighMember;
}

//���� ���� ��� ������ �����Ѵ�(���� ���� ����� ����ȭ)
void ns_world::GroupManager::SetGroupLowMember(ns_world::WorldUser* user, eGROUP_STAT eLowMember)
{
    ns_world::GroupLink* group_link = const_cast<ns_world::GroupLink*>(&user->GetGroupInfo());
    group_link->group_state_low_member_ = eLowMember;
}

//�׷� �ɹ� ���� ����ȭ�� ���� ������ ���� ������ ������.
void ns_world::GroupManager::SendGroupInfoSet(
    ns_world::WorldUser* user, eGROUP_STAT eHighMember, eGROUP_STAT eLowMember)
{
    if (user->EnabledSendMessage() == NULL) {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't found linked server, UserGuid=%u"),
               user->GetUserAccountInfo().user_guid);
        return;
    };

    ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
    ns_world::ServerSessionInWorld* const dbproxy = world_server->GetGameDBProxy();
    if (dbproxy == NULL) {
        return; // NOTE: f110811.5L, this case means all services closed.
    };

    const ns_world::GroupLink& group_link = user->GetGroupInfo();

    ;{
        MSG_AW_STATUS_GROUP_SET_MEMBERINFO_CMD msg_to_agent;
        msg_to_agent.m_CharGuid = user->GetConnectedCharInfo().char_guid;
        msg_to_agent.m_HighGroupinfo = group_link.group_state_high_member_;
        msg_to_agent.m_LowGroupInfo = group_link.group_state_low_member_;
        user->SendPacket(&msg_to_agent, sizeof(msg_to_agent));
    };
    //
    ;{
        MSG_DW_GROUP_SET_MEMBERINFO_CMD msg_to_dbp;
        msg_to_dbp.m_UserKey = user->GetUserAccountInfo().user_guid;
        msg_to_dbp.m_HighGroupinfo = group_link.group_state_high_member_;
        msg_to_dbp.m_LowGroupInfo = group_link.group_state_low_member_;
        dbproxy->SendPacket(&msg_to_dbp, sizeof(msg_to_dbp));
    };
}

bool ns_world::GroupManager::SendAccumEXP(
    DWORD user_guid, CHARGUID high_member_guid, EXPTYPE accumulated_exp)
{
    ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
    ns_world::ServerSessionInWorld* const dbproxy = world_server->GetGameDBProxy();
    if (dbproxy == NULL) {
        return false; // NOTE: f110811.5L, this case means all services closed.
    };

    MSG_DW_GROUP_ACCUMEXP_CMD msg_cmd;
    ;{
        msg_cmd.m_UserKey = user_guid;
        msg_cmd.m_HighCharGUID = high_member_guid;
        msg_cmd.m_AccumEXP = DWORD(accumulated_exp); // changes 'EXPTYPE' to 'DWORD'
    };
    dbproxy->SendPacket(&msg_cmd, sizeof(msg_cmd));

    return true;
}

//lastDate�� ���� �ð����̰� chDate���� ũ�� false ����
bool ns_world::GroupManager::IsOverBonusDate(DWORD64 last_date, INT check_date)
{
    ::tm stLastDate;
    ::SYSTEMTIME LASTTIME;
    time_t t_LastTime, t_NowTime, t_AccumTime;
    int date;

    last_date = last_date * 100;//�ʽð��� ���߱� ���� 100�� �����ش�.
    util::YYYYMMDDHHMMSSToSYSTEMTIME(last_date, LASTTIME);//DWORD64�� LASTTIME���� ����

    stLastDate.tm_year  = LASTTIME.wYear - 1900;
    stLastDate.tm_mon   = LASTTIME.wMonth - 1;
    stLastDate.tm_mday  = LASTTIME.wDay;
    stLastDate.tm_hour  = LASTTIME.wHour;
    stLastDate.tm_min   = LASTTIME.wMinute;
    stLastDate.tm_sec   = LASTTIME.wSecond;

    //util::GetDateTime_YYMMDDHHMMSS() : time()���� ���� �ð��� DWORD64�� ����
    //util::GetDateTime_YYMMDDHHMM()

    t_LastTime = ::mktime(&stLastDate);

    util::TimeSync::time(&t_NowTime);

    t_AccumTime = t_NowTime - t_LastTime;

    date = (INT)t_AccumTime / (3600*24);

    if (date > check_date) {
        return false;
    };
    return true;
}

void ns_world::GroupManager::SendAccumEXPMemo(ns_world::WorldUser* user)
{
    ns_world::WorldServer* const world_server = ns_world::WorldServer::Instance();
    ns_world::ServerSessionInWorld* const dbproxy = world_server->GetGameDBProxy();
    if (dbproxy == NULL) {
        return; // NOTE: f110811.5L, this case means all services closed.
    };

    MSG_DW_MEMO_SEND_SYN sendMsg;
    sendMsg.m_dwKey = 0;    // �ý������κ����� ����
    sendMsg.m_SendGuid = 0; // �ý������κ����� ����
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    sendMsg.m_typeSystemMemo = NORMAL_MEMO;
    sendMsg.m_recvItemCode = 0;
    sendMsg.m_amountCost = 0;
    sendMsg.m_recvGuid = 0;
    //}
    _tcsncpy(sendMsg.m_ptszRecvName, user->GetCharName(), _countof(sendMsg.m_ptszRecvName));
    sendMsg.m_ptszRecvName[_countof(sendMsg.m_ptszRecvName) - 1] = '\0';
    
    _sntprintf(sendMsg.m_ptszMemo, _countof(sendMsg.m_ptszMemo), "%d|%d", 
               SYSMEMO_GROUP_MGR, user->GetGroupInfo().accumulated_exp_);
    sendMsg.m_ptszMemo[_countof(sendMsg.m_ptszMemo) - 1] = '\0';

    dbproxy->SendPacket(&sendMsg, sizeof(sendMsg));
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
