#include "WorldServerPch.hxx"
#include "./FriendMap.h"

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING

#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
#include "Services/Friends/FriendInvitation.h"
#include "Services/Guild/WorldGuildManager.h"
#include "Services/Group/GroupManager.h"

//==================================================================================================

ns_world::FriendMap* ns_world::FriendMap::instance_static_ = NULL;

ns_world::FriendMap* ns_world::FriendMap::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(FriendMap** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            if (*static_table_) {
                TAllocDelete(ns_world::FriendMap, *static_table_);
            }
            *static_table_ = NULL;
        }
        FriendMap** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    FriendMap* provider = TAllocNew(ns_world::FriendMap);
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

ns_world::FriendMap::FriendMap()
    : char_status_data_accessor_(NULL)
{
}

ns_world::FriendMap::~FriendMap()
{
    ;{
        FRIEND_TABLE::const_iterator it = friend_table_.begin();
        const FRIEND_TABLE::const_iterator end = friend_table_.end();
        for (; it != end; ++it)
        {
            FRIEND_INFO_MAP* info_map = it->second;
            TAllocDelete(FRIEND_INFO_MAP, info_map);
        };
        friend_table_.clear();
    };
    ;{
        BLOCK_CHAR_TABLE::const_iterator it = block_char_table_.begin();
        const BLOCK_CHAR_TABLE::const_iterator end = block_char_table_.end();
        for (; it != end; ++it)
        {
            BLOCK_INFO_MAP* info_map = it->second;
            TAllocDelete(BLOCK_INFO_MAP, info_map);
        };
        block_char_table_.clear();
    };
    ;{
        REVENGE_TABLE::const_iterator it = revenge_table_.begin();
        const REVENGE_TABLE::const_iterator end = revenge_table_.end();
        for (; it != end; ++it)
        {
            REVENGE_INFO_MAP* info_map = it->second;
            TAllocDelete(REVENGE_INFO_MAP, info_map);
        };
        revenge_table_.clear();
    };
}

//==================================================================================================

void ns_world::FriendMap::RegisterCharStatusDataNodeAccessor(
    ICharStatusDataNodeAccessor* char_status_data_accessor)
{
    char_status_data_accessor_ = char_status_data_accessor;
}

bool ns_world::FriendMap::PreparedFriendService(ns_world::WorldUser* user)
{
    const ns_world::FriendLink& friend_link = user->GetFriendInfo();
    return friend_link.link_ != NULL;
}

// NOTE: f110905.1L, this initialize the user linked storage on the first enterance
bool ns_world::FriendMap::PrepareFriendServices(ns_world::WorldUser* owner)
{
    const ns_world::UserAccountInfo& user_info = owner->GetUserAccountInfo();

    ns_world::FriendLink* const editable_friend_link = \
        const_cast<ns_world::FriendLink*>(&owner->GetFriendInfo());
    if (editable_friend_link->link_ == NULL)
    {
        FRIEND_TABLE::const_iterator found = friend_table_.find(user_info.user_guid);
        if (found != friend_table_.end())
        {
            editable_friend_link->link_ = found->second;
            editable_friend_link->link_->clear();
        }
        else
        {
            editable_friend_link->link_ = TAllocNew(FRIEND_INFO_MAP);
            friend_table_.insert(FRIEND_TABLE::value_type(\
                user_info.user_guid, editable_friend_link->link_));
        };
    };
    //----------------------------------------------------------------------------------------------
    ns_world::BlockCharLink* const editable_block_link = \
        const_cast<ns_world::BlockCharLink*>(&owner->GetBlockCharInfo());
    if (editable_block_link->link_ == NULL)
    {
        BLOCK_CHAR_TABLE::const_iterator found = block_char_table_.find(user_info.user_guid);
        if (found != block_char_table_.end())
        {
            editable_block_link->link_ = found->second;
            editable_block_link->link_->clear();
        }
        else
        {
            editable_block_link->link_ = TAllocNew(BLOCK_INFO_MAP);
            block_char_table_.insert(BLOCK_CHAR_TABLE::value_type(\
                user_info.user_guid, editable_block_link->link_));
        };
    };
    //----------------------------------------------------------------------------------------------
    ns_world::RevengeLink* const editable_revenge_link = \
        const_cast<ns_world::RevengeLink*>(&owner->GetRevengeInfo());
    if (editable_revenge_link->link_ == NULL)
    {
        REVENGE_TABLE::const_iterator found = revenge_table_.find(user_info.user_guid);
        if (found != revenge_table_.end())
        {
            editable_revenge_link->link_ = found->second;
            editable_revenge_link->link_->clear();
        }
        else
        {
            editable_revenge_link->link_ = TAllocNew(REVENGE_INFO_MAP);
            revenge_table_.insert(REVENGE_TABLE::value_type(\
                user_info.user_guid, editable_revenge_link->link_));
        };
    };
    //----------------------------------------------------------------------------------------------
    if (ns_world::FriendInvitationInWorld* invitation = \
            ns_world::FriendInvitationInWorld::Instance())
    {
        invitation->PrepareFriendInvitationTable(owner);
    }
    //
    return true;
}

bool ns_world::FriendMap::NotifyOnlineStatusChangedFriendData(
    ns_world::WorldUser* owner, bool changed_online_status)
{
    const CHARGUID owner_char_guid = owner->GetConnectedCharInfo().char_guid;
    if (owner_char_guid == 0) {
        return false;
    };
    //
    const ns_world::CharStatusDataNode& owner_data_node = \
        *char_status_data_accessor_->GetCharStatusData(owner_char_guid);
    const bool owner_gm_user_check = (owner->IsGMUser());
    if (const bool changed_to_online = changed_online_status)
    {
        MSG_CW_FRIEND_LOGIN_BRD msg_brd;
        CopyMemory(msg_brd.ptszFriendName, owner_data_node.friend_info.sPacketInfo.ptszFriendName,
            sizeof(msg_brd.ptszFriendName));
        msg_brd.byChannelID = owner_data_node.friend_info.sPacketInfo.byChannel;

        const ns_world::FriendLink& friend_link = owner->GetFriendInfo();
        const FRIEND_INFO_MAP* const relations = friend_link.link_;
        FRIEND_INFO_MAP::const_iterator it = relations->begin();
        const FRIEND_INFO_MAP::const_iterator end = relations->end();
        for (; it != end; ++it)
        {
            const STRUCT_FRIEND_INFO* relation = it->second;
            const ns_world::CharStatusDataNode* data_node = \
                ns_world::CharStatusDataNode::GetContainerObject(relation);
            if (bool offline = (data_node == NULL) || (data_node->online_user == NULL)) {
                continue;
            };
            if (owner_gm_user_check && data_node->online_user->IsGMUser() == false) {
                continue;
            };
            if (const STRUCT_BLOCK_INFO* found_block = \
                    FindBlockChar(data_node->online_user, owner_char_guid))
            {
                continue;
            }
            data_node->online_user->SendPacket(&msg_brd, sizeof(msg_brd));
        };
    };
    if (const bool changed_to_offline = (changed_online_status == false))
    {
        MSG_CW_FRIEND_LOGOUT_BRD msg_brd;
        CopyMemory(msg_brd.ptszFriendName, owner_data_node.friend_info.sPacketInfo.ptszFriendName,
            sizeof(msg_brd.ptszFriendName));

        const ns_world::FriendLink& friend_link = owner->GetFriendInfo();
        const FRIEND_INFO_MAP* const relations = friend_link.link_;
        FRIEND_INFO_MAP::const_iterator it = relations->begin();
        const FRIEND_INFO_MAP::const_iterator end = relations->end();
        for (; it != end; ++it)
        {
            const STRUCT_FRIEND_INFO* relation = it->second;
            const ns_world::CharStatusDataNode* data_node = \
                ns_world::CharStatusDataNode::GetContainerObject(relation);
            if (bool offline = (data_node == NULL) || (data_node->online_user == NULL)) {
                continue;
            };
            if (owner_gm_user_check && data_node->online_user->IsGMUser() == false) {
                continue;
            };
            if (const STRUCT_BLOCK_INFO* found_block = \
                    FindBlockChar(data_node->online_user, owner_char_guid))
            {
                continue;
            }
            data_node->online_user->SendPacket(&msg_brd, sizeof(msg_brd));
        };
    };
    if (const bool changed_to_online = changed_online_status)
    {
        MSG_CW_REVENGE_LOGIN_BRD msg_brd;
        CopyMemory(msg_brd.m_ptszCharName, owner_data_node.revenge_info.sPacketInfo.ptszRevengeName,
            sizeof(msg_brd.m_ptszCharName));
        msg_brd.byChannelID = owner_data_node.revenge_info.sPacketInfo.byChannelID;

        const ns_world::RevengeLink& revenge_link = owner->GetRevengeInfo();
        const REVENGE_INFO_MAP* const relations = revenge_link.link_;
        REVENGE_INFO_MAP::const_iterator it = relations->begin();
        const REVENGE_INFO_MAP::const_iterator end = relations->end();
        for (; it != end; ++it)
        {
            const STRUCT_REVENGE_INFO* relation = it->second;
            const ns_world::CharStatusDataNode* data_node = \
                ns_world::CharStatusDataNode::GetContainerObject(relation);
            if (bool offline = (data_node == NULL) || (data_node->online_user == NULL)) {
                continue;
            };
            if (owner_gm_user_check && data_node->online_user->IsGMUser() == false) {
                continue;
            };
            data_node->online_user->SendPacket(&msg_brd, sizeof(msg_brd));
        };
    };
    if (const bool changed_to_offline = (changed_online_status == false))
    {
        MSG_CW_REVENGE_LOGOUT_BRD msg_brd;
        CopyMemory(msg_brd.ptszCharName, owner_data_node.revenge_info.sPacketInfo.ptszRevengeName,
            sizeof(msg_brd.ptszCharName));

        const ns_world::RevengeLink& revenge_link = owner->GetRevengeInfo();
        const REVENGE_INFO_MAP* const relations = revenge_link.link_;
        REVENGE_INFO_MAP::const_iterator it = relations->begin();
        const REVENGE_INFO_MAP::const_iterator end = relations->end();
        for (; it != end; ++it)
        {
            const STRUCT_REVENGE_INFO* relation = it->second;
            const ns_world::CharStatusDataNode* data_node = \
                ns_world::CharStatusDataNode::GetContainerObject(relation);
            if (bool offline = (data_node == NULL) || (data_node->online_user == NULL)) {
                continue;
            };
            if (owner_gm_user_check && data_node->online_user->IsGMUser() == false) {
                continue;
            };
            data_node->online_user->SendPacket(&msg_brd, sizeof(msg_brd));
        };
    };
    return true;
}

bool ns_world::FriendMap::ReleaseFriendData(ns_world::WorldUser* owner)
{
    const ns_world::UserAccountInfo& user_info = owner->GetUserAccountInfo();

    if (ns_world::FriendLink* const editable_friend_link = \
            const_cast<ns_world::FriendLink*>(&owner->GetFriendInfo()))
    {
        editable_friend_link->link_ = NULL;
        editable_friend_link->Init(owner);
        FRIEND_TABLE::const_iterator found = friend_table_.find(user_info.user_guid);
        if (found != friend_table_.end()) {
            found->second->clear();
        };
    };
    //----------------------------------------------------------------------------------------------
    if (ns_world::BlockCharLink* const editable_block_link = \
            const_cast<ns_world::BlockCharLink*>(&owner->GetBlockCharInfo()))
    {
        editable_block_link->link_ = NULL;
        editable_block_link->Init(owner);
        BLOCK_CHAR_TABLE::const_iterator found = block_char_table_.find(user_info.user_guid);
        if (found != block_char_table_.end()) {
            found->second->clear();
        };
    };
    //----------------------------------------------------------------------------------------------
    if (ns_world::RevengeLink* const editable_revenge_link = \
            const_cast<ns_world::RevengeLink*>(&owner->GetRevengeInfo()))
    {
        editable_revenge_link->link_ = NULL;
        editable_revenge_link->Init(owner);
        REVENGE_TABLE::const_iterator found = revenge_table_.find(user_info.user_guid);
        if (found != revenge_table_.end()) {
            found->second->clear();
        };
    };
    //----------------------------------------------------------------------------------------------
    if (ns_world::FriendInvitationInWorld* invitation = \
            ns_world::FriendInvitationInWorld::Instance())
    {
        invitation->ReleaseFriendInvitationTable(owner);
    };
    //----------------------------------------------------------------------------------------------
    return true;
}

//==================================================================================================

bool ns_world::FriendMap::IsFriendTransaction(ns_world::WorldUser* user)
{
    const ns_world::FriendLink& friend_link = user->GetFriendInfo();
    return friend_link.transaction_;
}

bool ns_world::FriendMap::StartFriendTransaction(ns_world::WorldUser* user)
{
    ns_world::FriendLink* const editable_friend_link = \
        const_cast<ns_world::FriendLink*>(&user->GetFriendInfo());
    if (editable_friend_link->transaction_ != false) {
        return false;
    };
    editable_friend_link->transaction_ = true;
    return true;
}

bool ns_world::FriendMap::EndFriendTransaction(ns_world::WorldUser* user)
{
    ns_world::FriendLink* const editable_friend_link = \
        const_cast<ns_world::FriendLink*>(&user->GetFriendInfo());
    if (editable_friend_link->transaction_ == false) {
        return false;
    };
    editable_friend_link->transaction_ = false;
    return true;
}

RC::eFRIEND_RESULT ns_world::FriendMap::CheckAccessibleFriendInfo(
    ns_world::WorldUser* user, CHARGUID related_char_guid)
{
    ;{
        const ns_world::FriendLink& link = user->GetFriendInfo();
        const FRIEND_INFO_MAP* const table = link.link_;
        if (table != NULL)
        {
            const FRIEND_INFO_MAP::const_iterator found = table->find(related_char_guid);
            if (found != table->end()) {
                return RC::RC_FRIEND_SUCCESS; // 친구
            };
        };
    };
    ;{
        const ns_world::RevengeLink& link = user->GetRevengeInfo();
        const REVENGE_INFO_MAP* const table = link.link_;
        if (table != NULL)
        {
            const REVENGE_INFO_MAP::const_iterator found = table->find(related_char_guid);
            if (found != table->end()) {
                return RC::RC_FRIEND_SUCCESS; // 리벤지 대상
            };
        };
    };
    if (const STRUCT_GROUP_MEMBER_INFO* group_member = \
            ns_world::GroupManager::Instance()->FindGroupMember(user, related_char_guid))
    {
        return RC::RC_FRIEND_SUCCESS; // 그룹원
    };
    BOOST_STATIC_ASSERT(INVALID_GUILDGUID == 0);
    if (const GUILDGUID guild_guid = user->GetConnectedCharInfo().guild_guid)
    {
        ns_world::WorldGuild* guild = ns_world::WorldGuildManager::Instance()->FindGuild(guild_guid);
        if (guild != NULL)
        {
            const ns_world::WorldGuildMember* member = guild->FindMember(related_char_guid);
            if (member != NULL) {
                return RC::RC_FRIEND_SUCCESS;   // 길드원 (한번이상 접속한 유저)  
            }
            else {
                return RC::RC_FRIEND_NOTGUILDMEMBER;
            };
        };
    };
    return RC::RC_FRIEND_NOTFRIEND;
}

//==================================================================================================

int ns_world::FriendMap::GetNumberOfFriends(ns_world::WorldUser* user)
{
    const ns_world::FriendLink& friend_link = user->GetFriendInfo();
    const FRIEND_INFO_MAP* const friends = friend_link.link_;
    if (friends == NULL) {
        return 0;
    };

    return static_cast<int>(friends->size());
}

void ns_world::FriendMap::ClearFriends(ns_world::WorldUser* user)
{
    ns_world::FriendLink* const editable_friend_link = \
        const_cast<ns_world::FriendLink*>(&user->GetFriendInfo());

    FRIEND_INFO_MAP* backup = editable_friend_link->link_;
    if (backup != NULL) {
        backup->clear();
        editable_friend_link->link_ = NULL;
    };
    editable_friend_link->Clear();
    editable_friend_link->link_ = backup;
}

const STRUCT_FRIEND_INFO* ns_world::FriendMap::FindFriend(
    ns_world::WorldUser* user, USERGUID friend_char_guid)
{
    const ns_world::FriendLink& friend_link = user->GetFriendInfo();
    const FRIEND_INFO_MAP* const friends = friend_link.link_;
    if (friends == NULL) {
        return NULL;
    };

    const FRIEND_INFO_MAP::const_iterator found = friends->find(friend_char_guid);
    if (found == friends->end()) {
        return NULL;
    };

    const STRUCT_FRIEND_INFO* friend_info = found->second; assert(friend_info != NULL);
    return friend_info;
}

const STRUCT_FRIEND_INFO* ns_world::FriendMap::FindFriendByCharName(
    ns_world::WorldUser* user, const util::StrArrayForCharName& friend_char_name)
{
    const ns_world::FriendLink& friend_link = user->GetFriendInfo();
    const FRIEND_INFO_MAP* const friends = friend_link.link_;
    if (friends == NULL) {
        return NULL;
    };
    const NAMEHASH name_hash = ns_world::WorldUserManager::MakeNameHash(friend_char_name);
    if (name_hash == 0) { // empty string
        return NULL;
    };
    //
    //FriendMap* const friend_map = FriendMap::Instance();
    //FRIEND_KEY_PAIR_TABLE::const_iterator key_found = \
    //    friend_map->key_pair_table_.find(find_char_seed);
    //if (key_found == friend_map->key_pair_table_.end()) {
    //    return NULL;
    //};
    CHARGUID found_char_guid;
    if (ns_world::WorldUserManager::FindCharGuidByNameHash(name_hash, &found_char_guid) == false) {
        return NULL;
    };
    FRIEND_INFO_MAP::const_iterator found = friends->find(found_char_guid);
    if (found == friends->end()) {
        return NULL;
    };

    const STRUCT_FRIEND_INFO* found_friend_info = found->second;
    assert(found_friend_info != NULL);
    return found_friend_info;
}

bool ns_world::FriendMap::RemoveFriend(ns_world::WorldUser* user, CHARGUID friend_char_guid)
{
    ns_world::FriendLink* const editable_friend_link = \
        const_cast<ns_world::FriendLink*>(&user->GetFriendInfo());
    FRIEND_INFO_MAP* const friends = editable_friend_link->link_;
    if (friends == NULL) {
        return false;
    };

    FRIEND_INFO_MAP::iterator found = friends->find(friend_char_guid);
    if (found == friends->end()) {
        return false;
    };
    //
    friends->erase(found);
    return true;
};

bool ns_world::FriendMap::RemoveFriend(
    ns_world::WorldUser* user, const util::StrArrayForCharName& friend_char_name)
{
    ns_world::FriendLink* const editable_friend_link = \
        const_cast<ns_world::FriendLink*>(&user->GetFriendInfo());
    FRIEND_INFO_MAP* const friends = editable_friend_link->link_;
    if (friends == NULL) {
        return false;
    };
    const NAMEHASH name_hash = ns_world::WorldUserManager::MakeNameHash(friend_char_name);
    if (name_hash == 0) { // empty string
        return NULL;
    };
    //
    //FriendMap* const friend_map = FriendMap::Instance();
    //FRIEND_KEY_PAIR_TABLE::const_iterator key_found = \
    //    friend_map->key_pair_table_.find(find_char_seed);
    //if (key_found == friend_map->key_pair_table_.end()) {
    //    return false;
    //};
    CHARGUID found_char_guid;
    if (ns_world::WorldUserManager::FindCharGuidByNameHash(name_hash, &found_char_guid) == false) {
        return false;
    };
    FRIEND_INFO_MAP::iterator found = friends->find(found_char_guid);
    if (found == friends->end()) {
        return false;
    };

    friends->erase(found);
    return true;
}

int ns_world::FriendMap::GetNumberOfBlocks(ns_world::WorldUser* user)
{
    const ns_world::BlockCharLink& block_link = user->GetBlockCharInfo();
    const BLOCK_INFO_MAP* const blocks = block_link.link_;
    if (blocks == NULL) {
        return 0;
    };

    return static_cast<int>(blocks->size());
}

void ns_world::FriendMap::ClearBlocks(ns_world::WorldUser* user)
{
    ns_world::BlockCharLink* const editable_block_link = \
        const_cast<ns_world::BlockCharLink*>(&user->GetBlockCharInfo());

    BLOCK_INFO_MAP* backup = editable_block_link->link_;
    if (backup != NULL) {
        backup->clear();
        editable_block_link->link_ = NULL;
    };
    editable_block_link->Clear();
    editable_block_link->link_ = backup;
}

const STRUCT_BLOCK_INFO* ns_world::FriendMap::FindBlockChar(
    ns_world::WorldUser* user, USERGUID block_char_guid)
{
    const ns_world::BlockCharLink& block_link = user->GetBlockCharInfo();
    const BLOCK_INFO_MAP* const blocks = block_link.link_;
    if (blocks == NULL) {
        return NULL;
    };

    const BLOCK_INFO_MAP::const_iterator found = blocks->find(block_char_guid);
    if (found == blocks->end()) {
        return NULL;
    };

    const STRUCT_BLOCK_INFO* block_info = found->second; assert(block_info != NULL);
    return block_info;
}

const STRUCT_BLOCK_INFO* ns_world::FriendMap::FindBlockCharByCharName(
    ns_world::WorldUser* user, const util::StrArrayForCharName& block_char_name)
{
    const ns_world::BlockCharLink& block_link = user->GetBlockCharInfo();
    const BLOCK_INFO_MAP* const blocks = block_link.link_;
    if (blocks == NULL) {
        return NULL;
    };
    const NAMEHASH name_hash = ns_world::WorldUserManager::MakeNameHash(block_char_name);
    if (name_hash == 0) { // empty string
        return NULL;
    };
    //
    //FriendMap* const block_map = FriendMap::Instance();
    //FRIEND_KEY_PAIR_TABLE::const_iterator key_found = \
    //    block_map->key_pair_table_.find(find_char_seed);
    //if (key_found == block_map->key_pair_table_.end()) {
    //    return NULL;
    //};
    CHARGUID found_char_guid;
    if (ns_world::WorldUserManager::FindCharGuidByNameHash(name_hash, &found_char_guid) == false) {
        return NULL;
    };
    BLOCK_INFO_MAP::const_iterator found = blocks->find(found_char_guid);
    if (found == blocks->end()) {
        return NULL;
    };

    const STRUCT_BLOCK_INFO* found_block_info = found->second; assert(found_block_info != NULL);
    return found_block_info;
}


bool ns_world::FriendMap::RemoveBlockChar(ns_world::WorldUser* user, CHARGUID blocked_char_guid)
{
    ns_world::BlockCharLink* const editable_block_link = \
        const_cast<ns_world::BlockCharLink*>(&user->GetBlockCharInfo());
    BLOCK_INFO_MAP* const blocks = editable_block_link->link_;
    if (blocks == NULL) {
        return false;
    };

    BLOCK_INFO_MAP::iterator found = blocks->find(blocked_char_guid);
    if (found == blocks->end()) {
        return false;
    };
    //
    blocks->erase(found);
    return true;
}

bool ns_world::FriendMap::RemoveBlockChar(ns_world::WorldUser* user,
    const util::StrArrayForCharName& block_char_name)
{
    ns_world::BlockCharLink* const editable_block_link = \
        const_cast<ns_world::BlockCharLink*>(&user->GetBlockCharInfo());
    BLOCK_INFO_MAP* const blocks = editable_block_link->link_;
    if (blocks == NULL) {
        return false;
    };
    const NAMEHASH name_hash = ns_world::WorldUserManager::MakeNameHash(block_char_name);
    if (name_hash == 0) { // empty string
        return NULL;
    };
    //
    //FriendMap* const block_map = FriendMap::Instance();
    //FRIEND_KEY_PAIR_TABLE::const_iterator key_found = \
    //    block_map->key_pair_table_.find(find_char_seed);
    //if (key_found == block_map->key_pair_table_.end()) {
    //    return false;
    //};
    CHARGUID found_char_guid;
    if (ns_world::WorldUserManager::FindCharGuidByNameHash(name_hash, &found_char_guid) == false) {
        return NULL;
    };
    BLOCK_INFO_MAP::iterator found = blocks->find(found_char_guid);
    if (found == blocks->end()) {
        return false;
    };
    //
    blocks->erase(found);
    return true;
}


int ns_world::FriendMap::GetNumberOfRevenges(ns_world::WorldUser* user)
{
    const ns_world::RevengeLink& revenge_link = user->GetRevengeInfo();
    const REVENGE_INFO_MAP* const revenges = revenge_link.link_;
    if (revenges == NULL) {
        return 0;
    };

    return static_cast<int>(revenges->size());
}

void ns_world::FriendMap::ClearRevenges(ns_world::WorldUser* user)
{
    ns_world::RevengeLink* const editable_revenge_link = \
        const_cast<ns_world::RevengeLink*>(&user->GetRevengeInfo());

    REVENGE_INFO_MAP* backup = editable_revenge_link->link_;
    if (backup != NULL) {
        backup->clear();
        editable_revenge_link->link_ = NULL;
    };
    editable_revenge_link->Clear();
    editable_revenge_link->link_ = backup;
}

const STRUCT_REVENGE_INFO* ns_world::FriendMap::FindRevengeChar(
    ns_world::WorldUser* user, USERGUID revenged_char_guid)
{
    const ns_world::RevengeLink& revenge_link = user->GetRevengeInfo();
    const REVENGE_INFO_MAP* const revenges = revenge_link.link_;
    if (revenges == NULL) {
        return NULL;
    };

    const REVENGE_INFO_MAP::const_iterator found = revenges->find(revenged_char_guid);
    if (found == revenges->end()) {
        return NULL;
    };

    const STRUCT_REVENGE_INFO* revenge_info = found->second; assert(revenge_info != NULL);
    return revenge_info;
}

const STRUCT_REVENGE_INFO* ns_world::FriendMap::FindRevengeCharByCharName(
    ns_world::WorldUser* user, const util::StrArrayForCharName& revenge_char_name)
{
    const ns_world::RevengeLink& revenge_link = user->GetRevengeInfo();
    const REVENGE_INFO_MAP* const revenges = revenge_link.link_;
    if (revenges == NULL) {
        return NULL;
    };
    const NAMEHASH name_hash = ns_world::WorldUserManager::MakeNameHash(revenge_char_name);
    if (name_hash == 0) { // empty string
        return NULL;
    };
    //
    //FriendMap* const revenge_map = FriendMap::Instance();
    //FRIEND_KEY_PAIR_TABLE::const_iterator key_found = \
    //    revenge_map->key_pair_table_.find(find_char_seed);
    //if (key_found == revenge_map->key_pair_table_.end()) {
    //    return NULL;
    //};
    CHARGUID found_char_guid;
    if (ns_world::WorldUserManager::FindCharGuidByNameHash(name_hash, &found_char_guid) == false) {
        return NULL;
    };
    REVENGE_INFO_MAP::const_iterator found = revenges->find(found_char_guid);
    if (found == revenges->end()) {
        return NULL;
    };

    const STRUCT_REVENGE_INFO* found_revenge_info = found->second;
    assert(found_revenge_info != NULL);
    return found_revenge_info;
}

bool ns_world::FriendMap::RemoveRevengeChar(
    ns_world::WorldUser* user, CHARGUID revenged_char_guid)
{
    ns_world::RevengeLink* const editable_revenge_link = \
        const_cast<ns_world::RevengeLink*>(&user->GetRevengeInfo());
    REVENGE_INFO_MAP* const revenges = editable_revenge_link->link_;
    if (revenges == NULL) {
        return false;
    };

    REVENGE_INFO_MAP::iterator found = revenges->find(revenged_char_guid);
    if (found == revenges->end()) {
        return false;
    };
    //
    revenges->erase(found);
    return true;
}

bool ns_world::FriendMap::RemoveRevengeChar(
    ns_world::WorldUser* user, const util::StrArrayForCharName& revenge_char_name)
{
    ns_world::RevengeLink* const editable_revenge_link = \
        const_cast<ns_world::RevengeLink*>(&user->GetRevengeInfo());
    REVENGE_INFO_MAP* const revenges = editable_revenge_link->link_;
    if (revenges == NULL) {
        return false;
    };
    const NAMEHASH name_hash = ns_world::WorldUserManager::MakeNameHash(revenge_char_name);
    if (name_hash == 0) { // empty string
        return NULL;
    };
    //
    //FriendMap* const revenge_map = FriendMap::Instance();
    //FRIEND_KEY_PAIR_TABLE::const_iterator key_found = \
    //    revenge_map->key_pair_table_.find(find_char_seed);
    //if (key_found == revenge_map->key_pair_table_.end()) {
    //    return false;
    //};
    CHARGUID found_char_guid;
    if (ns_world::WorldUserManager::FindCharGuidByNameHash(name_hash, &found_char_guid) == false) {
        return NULL;
    };
    REVENGE_INFO_MAP::iterator found = revenges->find(found_char_guid);
    if (found == revenges->end()) {
        return false;
    };
    //
    revenges->erase(found);
    return true;
}

uint8_t ns_world::FriendMap::GetRevengePoint(ns_world::WorldUser* user)
{
    const ns_world::RevengeLink& revenge_link = user->GetRevengeInfo();
    return revenge_link.revenge_point_;
}

void ns_world::FriendMap::DecreaseRevengePoint(ns_world::WorldUser* user)
{
    ns_world::RevengeLink* const editable_revenge_link = \
        const_cast<ns_world::RevengeLink*>(&user->GetRevengeInfo());
    // TODO: check relations
    //REVENGE_INFO_MAP* const revenges = editable_revenge_link->link_;
    //if (revenges == NULL) {
    //    return false;
    //};
    //
    if (editable_revenge_link->revenge_point_) {
        --editable_revenge_link->revenge_point_;
    };
}

//==================================================================================================

bool ns_world::FriendMap::InsertNewFriend(ns_world::WorldUser* const owner,
    const STRUCT_FRIEND_INFO& filtered_friend_info)
{
    ns_world::FriendLink* const editable_friend_link = \
        const_cast<ns_world::FriendLink*>(&owner->GetFriendInfo());
    const ns_world::ConnectedCharInfo& char_info = owner->GetConnectedCharInfo();
    __TOUCHED(char_info);
    assert(editable_friend_link->link_ != NULL);

    ns_world::NAMEHASH name_hash;
    if (FlowControl::FCAssert(ns_world::WorldUserManager::UpdateNameHash(\
            filtered_friend_info.dwFriendGuid,
            filtered_friend_info.sPacketInfo.ptszFriendName,
            &name_hash)) == false)
    {
        return false;
    };

    const CHARGUID char_guid = filtered_friend_info.dwFriendGuid;
    FRIEND_INFO_MAP* const friend_table = editable_friend_link->link_;
    ns_world::CharStatusDataNode* const status_data_node = \
        char_status_data_accessor_->GetCharStatusData(char_guid);
    //
    if (status_data_node->friend_configured == false) {
        status_data_node->friend_info = filtered_friend_info;
        status_data_node->friend_configured = true;
    };
    friend_table->operator[](char_guid) = &status_data_node->friend_info;
    return true;
};

bool ns_world::FriendMap::InsertNewBlockChar(ns_world::WorldUser* const owner,
    const STRUCT_BLOCK_INFO& filtered_block_info)
{
    ns_world::BlockCharLink* const editable_block_link = \
        const_cast<ns_world::BlockCharLink*>(&owner->GetBlockCharInfo());
    const ns_world::ConnectedCharInfo& char_info = owner->GetConnectedCharInfo();
    __TOUCHED(char_info);
    assert(editable_block_link->link_ != NULL);

    ns_world::NAMEHASH name_hash;
    if (FlowControl::FCAssert(ns_world::WorldUserManager::UpdateNameHash(\
            filtered_block_info.dwBlockGuid,
            filtered_block_info.ptszBlockName,
            &name_hash)) == false)
    {
        return false;
    };

    const CHARGUID char_guid = filtered_block_info.dwBlockGuid;
    BLOCK_INFO_MAP* const block_table = editable_block_link->link_;
    ns_world::CharStatusDataNode* const status_data_node = \
        char_status_data_accessor_->GetCharStatusData(char_guid);
    //
    if (status_data_node->block_configured == false) {
        status_data_node->block_info = filtered_block_info;
        status_data_node->block_configured = true;
    };
    block_table->operator[](char_guid) = &status_data_node->block_info;
    return true;
}

bool ns_world::FriendMap::InsertNewRevengeChar(ns_world::WorldUser* const owner,
    const STRUCT_REVENGE_INFO& filtered_revenge_info)
{
    ns_world::RevengeLink* const editable_revenge_link = \
        const_cast<ns_world::RevengeLink*>(&owner->GetRevengeInfo());
    const ns_world::ConnectedCharInfo& char_info = owner->GetConnectedCharInfo();
    __TOUCHED(char_info);
    assert(editable_revenge_link->link_ != NULL);

    ns_world::NAMEHASH name_hash;
    if (FlowControl::FCAssert(ns_world::WorldUserManager::UpdateNameHash(\
            filtered_revenge_info.dwRevengeCharGuid,
            filtered_revenge_info.sPacketInfo.ptszRevengeName,
            &name_hash)) == false)
    {
        return false;
    };

    const CHARGUID char_guid = filtered_revenge_info.dwRevengeCharGuid;
    REVENGE_INFO_MAP* const revenge_table = editable_revenge_link->link_;
    ns_world::CharStatusDataNode* const status_data_node = \
        char_status_data_accessor_->GetCharStatusData(char_guid);
    //
    if (status_data_node->revenge_configured == false) {
        status_data_node->revenge_info = filtered_revenge_info;
        status_data_node->revenge_configured = true;
    };
    revenge_table->operator[](char_guid) = &status_data_node->revenge_info;
    return true;
}

//==================================================================================================

// NOTE: send to all online users
//  event broadcasters  = { friend, block, revenge, friend_invitation }
bool ns_world::FriendMap::SendToAllFriends(
    ns_world::WorldUser* owner, MSG_BASE* msg_base, uint16_t msg_size) const
{
    const ns_world::FriendLink& friend_link = owner->GetFriendInfo();
    const FRIEND_INFO_MAP* const relations = friend_link.link_;
    if (relations == NULL) {
        return false;
    };

    FRIEND_INFO_MAP::const_iterator it = relations->begin();
    const FRIEND_INFO_MAP::const_iterator end = relations->end();
    for (; it != end; ++it)
    {
        const STRUCT_FRIEND_INFO* relation = it->second;
        const ns_world::CharStatusDataNode* data_node = \
            ns_world::CharStatusDataNode::GetContainerObject(relation);
        if (bool offline = (data_node == NULL) || (data_node->online_user == NULL)) {
            continue;
        };
        data_node->online_user->SendPacket(msg_base, msg_size);
    };
    return true;
}

// NOTE: send to all online users
//  event broadcasters  = { friend, block, revenge, friend_invitation }
bool ns_world::FriendMap::SendToAllBlockChars(
    ns_world::WorldUser* owner, MSG_BASE* msg_base, uint16_t msg_size) const
{
    const ns_world::BlockCharLink& blockchar_link = owner->GetBlockCharInfo();
    const BLOCK_INFO_MAP* const relations = blockchar_link.link_;
    if (relations == NULL) {
        return false;
    };

    BLOCK_INFO_MAP::const_iterator it = relations->begin();
    const BLOCK_INFO_MAP::const_iterator end = relations->end();
    for (; it != end; ++it)
    {
        const STRUCT_BLOCK_INFO* relation = it->second;
        const ns_world::CharStatusDataNode* data_node = \
            ns_world::CharStatusDataNode::GetContainerObject(relation);
        if (bool offline = (data_node == NULL) || (data_node->online_user == NULL)) {
            continue;
        };
        data_node->online_user->SendPacket(msg_base, msg_size);
    };
    return true;
}

// NOTE: send to all online users
//  event broadcasters  = { friend, block, revenge, friend_invitation }
bool ns_world::FriendMap::SendToAllRevengeChars(
    ns_world::WorldUser* owner, MSG_BASE* msg_base, uint16_t msg_size) const
{
    const ns_world::RevengeLink& revenge_link = owner->GetRevengeInfo();
    const REVENGE_INFO_MAP* const relations = revenge_link.link_;
    if (relations == NULL) {
        return false;
    };

    REVENGE_INFO_MAP::const_iterator it = relations->begin();
    const REVENGE_INFO_MAP::const_iterator end = relations->end();
    for (; it != end; ++it)
    {
        const STRUCT_REVENGE_INFO* relation = it->second;
        const ns_world::CharStatusDataNode* data_node = \
            ns_world::CharStatusDataNode::GetContainerObject(relation);
        if (bool offline = (data_node == NULL) || (data_node->online_user == NULL)) {
            continue;
        };
        data_node->online_user->SendPacket(msg_base, msg_size);
    };
    return true;
}

//==================================================================================================
#endif //_NA_0_20091104_FIRENDMAP_REFACTORYING
