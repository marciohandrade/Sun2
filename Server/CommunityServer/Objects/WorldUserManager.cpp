#include "WorldServerPch.hxx"
#include "./WorldUserManager.h"

#include "WorldServer.h"
#include "Objects/WorldUser.h"
#include "Channel/ZoneCommunity.h"

#include "Services/ChattingService.h"
#include "Services/Friends/FriendMap.h"
#include "Services/Friends/FriendInvitation.h"
#include "Services/Group/GroupManager.h"

//==================================================================================================

ns_world::WorldUserManager* ns_world::WorldUserManager::instance_static_ = NULL;

//==================================================================================================

ns_world::WorldUserManager* ns_world::WorldUserManager::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(ns_world::WorldUserManager** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        ns_world::WorldUserManager** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    ns_world::WorldUserManager* provider = new ns_world::WorldUserManager;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

ns_world::WorldUserManager::WorldUserManager()
{
    world_user_pool_ = new WorldUserPool;
    world_user_pool_->Initialize(10, 10);

    update_service_execute_timer_.SetTimer(5 * 1000);
    chatting_block_update_timer_.SetTimer(60 * 1000);

    ns_world::FriendMap::Instance()->RegisterCharStatusDataNodeAccessor(this);
    ns_world::FriendInvitationInWorld::Instance()->RegisterCharStatusDataNodeAccessor(this);
}

ns_world::WorldUserManager::~WorldUserManager()
{
    assert(user_key_table_.empty());
    SAFE_DELETE(world_user_pool_);
}

void ns_world::WorldUserManager::Shutdown()
{
    ;{
        UserTable::const_iterator user_it = user_key_table_.begin();
        const UserTable::const_iterator user_end = user_key_table_.end();
        while (user_it != user_end)
        {
            ns_world::WorldUser* user = user_it->second;
            ++user_it;
            LogoutInternal(user);
        };
    };
}

ns_world::WorldUser* ns_world::WorldUserManager::FindUser(DWORD user_guid) const
{
    UserTable::const_iterator found = user_key_table_.find(user_guid);
    if (found == user_key_table_.end()) {
        return NULL;
    };
    ns_world::WorldUser* user = found->second;
    return user;
}

ns_world::WorldUser* ns_world::WorldUserManager::FindUserByCharName(
    const util::StrArrayForCharName& finding_char_name) const
{
    if (finding_char_name[0] == _T('\0')) {
        return NULL;
    };
    util::StrArrayForCharName char_name;
    _tcsncpy(char_name, finding_char_name, _countof(char_name));
    char_name[_countof(char_name) - 1] = _T('\0');
    _tcslwr(char_name);
    //
    const DWORD hash_value = util::StreamHashKey::GetStringHashKey(char_name);
    UserTable::const_iterator found_pair = char_name_hash_table_.find(hash_value);
    if (found_pair == char_name_hash_table_.end()) {
        return NULL;
    };
    ns_world::WorldUser* user = found_pair->second;
    return user;
}

ns_world::WorldUser* ns_world::WorldUserManager::FindUserByCharGuid(DWORD char_guid) const
{
    UserTable::const_iterator found = char_key_table_.find(char_guid);
    if (found == char_key_table_.end()) {
        return NULL;
    };
    ns_world::WorldUser* user = found->second;
    return user;
}

ns_world::NAMEHASH ns_world::WorldUserManager::MakeNameHash(
    const util::StrArrayForCharName& input_char_name)
{
    if (input_char_name[0] == _T('\0')) {
        return 0;
    };

    util::StrArrayForCharName char_name;
    _tcsncpy(char_name, input_char_name, _countof(char_name));
    char_name[_countof(char_name) - 1] = _T('\0');
    _tcslwr(char_name);

    ns_world::NAMEHASH name_hash = util::StreamHashKey::GetStringHashKey(char_name);
    return name_hash;
}

bool ns_world::WorldUserManager::FindNameHashByCharGuid(
    const CHARGUID char_guid, ns_world::NAMEHASH* const result_name_hash)
{
    *result_name_hash = 0;
    if (char_guid == 0) {
        return false;
    };
    ns_world::WorldUserManager* const this_ = Instance();
    NameHashTable::const_iterator found = this_->cache_charguid_to_namehash_table_.find(char_guid);
    if (found == this_->cache_charguid_to_namehash_table_.end()) {
        return false;
    };
    *result_name_hash = found->second;
    return true;
}

bool ns_world::WorldUserManager::FindCharGuidByNameHash(
    const NAMEHASH name_hash, CHARGUID* const result_char_guid)
{
    *result_char_guid = 0;
    if (name_hash == 0) {
        return false;
    };
    ns_world::WorldUserManager* const this_ = Instance();
    NameHashTable::const_iterator found = this_->cache_namehash_to_charguid_table_.find(name_hash);
    if (found == this_->cache_namehash_to_charguid_table_.end()) {
        return false;
    };
    *result_char_guid = found->second;
    return true;
}

bool ns_world::WorldUserManager::FindCharGuidByCharName(
    const util::StrArrayForCharName& char_name, CHARGUID* const result_char_guid)
{
    *result_char_guid = 0;
    const NAMEHASH name_hash = MakeNameHash(char_name);
    if (name_hash == 0) {
        return false;
    };
    ns_world::WorldUserManager* const this_ = Instance();
    NameHashTable::const_iterator found = this_->cache_namehash_to_charguid_table_.find(name_hash);
    if (found == this_->cache_namehash_to_charguid_table_.end()) {
        return false;
    };
    *result_char_guid = found->second;
    return true;
}

bool ns_world::WorldUserManager::UpdateNameHash(const CHARGUID char_guid,
    const util::StrArrayForCharName& char_name, NAMEHASH* const result_name_hash)
{
    *result_name_hash = 0;
    if (char_guid == 0) {
        return false;
    };
    const NAMEHASH name_hash = MakeNameHash(char_name);
    if (name_hash == 0) {
        return false;
    };
    ns_world::WorldUserManager* const this_ = Instance();
    this_->cache_namehash_to_charguid_table_[name_hash] = char_guid;
    this_->cache_charguid_to_namehash_table_[char_guid] = name_hash;

    *result_name_hash = name_hash;
    return true;
}


void ns_world::WorldUserManager::UpdateCharStatusData(
    ns_world::WorldUser* user, UpdateCharStatusDataEnum update_type)
{
    const CHARGUID char_guid = user->GetCharGuid();
    if (char_guid == 0) {
        return;
    };
    ns_world::WorldUserManager* const this_ = ns_world::WorldUserManager::Instance();
    CharStatusDataTable::iterator found = this_->char_status_data_table_.find(char_guid);
    if (found == this_->char_status_data_table_.end()) {
        return;
    };

    const ns_world::UserAccountInfo& user_info = user->GetUserAccountInfo();
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    ns_world::CharStatusDataNode* const data_node = &found->second;
    // NOTE:
    //  (online == false) must be not cleared about connected character information
    //  (online != false) must be set about connected character information
    const bool online = (update_type != eUpdateCharStatus_Offline);
    if (online)
    {
        data_node->online_user = user;
        data_node->friend_configured = true;
        if (STRUCT_FRIEND_INFO* const dest_info = &data_node->friend_info)
        {
            dest_info->dwFriendGuid = char_guid;
            if (STRUCT_FRIEND_PACKET_INFO* const sub_info = &dest_info->sPacketInfo)
            {
                util::StrArrayForCharName& dest_name = sub_info->ptszFriendName;
                CopyMemory(dest_name, char_info.char_name, sizeof(dest_name));
	            sub_info->bOnline = online;
	            sub_info->byClass = char_info.char_class;
	            sub_info->Level = char_info.char_level;
	            sub_info->byChannel = user_info.channel->GetChannelID();
            };
        };
        data_node->block_configured = true;
        if (STRUCT_BLOCK_INFO* const dest_info = &data_node->block_info)
        {
            dest_info->dwBlockGuid = char_guid;
            util::StrArrayForCharName& dest_name = dest_info->ptszBlockName;
            CopyMemory(dest_name, char_info.char_name, sizeof(dest_name));
        };
        data_node->revenge_configured = true;
        if (STRUCT_REVENGE_INFO* const dest_info = &data_node->revenge_info)
        {
            dest_info->dwRevengeCharGuid = char_guid;
            if (STRUCT_REVENGE_PACKET_INFO* const sub_info = &dest_info->sPacketInfo)
            {
                util::StrArrayForCharName& dest_name = sub_info->ptszRevengeName;
                CopyMemory(dest_name, char_info.char_name, sizeof(dest_name));
	            sub_info->bOnline = online;
	            sub_info->byClass = char_info.char_class;
	            sub_info->Level = char_info.char_level;
	            sub_info->byChannelID = user_info.channel->GetChannelID();
            };
        };
        //if (STRUCT_GROUP_MEMBER_INFO* const dest_info = &data_node->group_info)
        //{
        //};
    }
    else
    {   // offline
        data_node->online_user = NULL;
        if (STRUCT_FRIEND_INFO* const dest_info = &data_node->friend_info)
        {
            if (STRUCT_FRIEND_PACKET_INFO* const sub_info = &dest_info->sPacketInfo)
            {
	            sub_info->bOnline = online;
            };
        };
        if (STRUCT_BLOCK_INFO* const dest_info = &data_node->block_info)
        {   __TOUCHED(dest_info);
        };
        if (STRUCT_REVENGE_INFO* const dest_info = &data_node->revenge_info)
        {
            if (STRUCT_REVENGE_PACKET_INFO* const sub_info = &dest_info->sPacketInfo)
            {
	            sub_info->bOnline = online;
            };
        };
    };
}

// NOTE: ns_world::ICharStatusDataNodeAccessor interface
// if the character is not found, new data structure is inserted.
ns_world::CharStatusDataNode*
ns_world::WorldUserManager::GetCharStatusData(const CHARGUID char_guid)
{
    static ns_world::CharStatusDataNode empty_char_data_node;
    if (char_guid == 0) {
        ZeroMemory(&empty_char_data_node, sizeof(empty_char_data_node));
        return &empty_char_data_node;
    };
    //
    CharStatusDataTable::iterator found = char_status_data_table_.find(char_guid);
    ns_world::CharStatusDataNode* data_node = NULL;
    if (found != char_status_data_table_.end()) {
        data_node = &found->second;
    }
    else {
        data_node = &char_status_data_table_[char_guid];
        data_node->Initialize();
    };
    return data_node;
}

//void ns_world::WorldUserManager::Init()
//{
//}

//void ns_world::WorldUserManager::Release()
//{
//}

ns_world::WorldUser* ns_world::WorldUserManager::AllocNewUser()
{
    ns_world::WorldUser* user = reinterpret_cast<ns_world::WorldUser*>(world_user_pool_->Alloc());
    user->Init();
    return user;
}

bool ns_world::WorldUserManager::AddUser(ns_world::WorldUser* user)
{
    DWORD user_guid = user->GetUserAccountInfo().user_guid;
    if (FlowControl::FCAssert(user_guid != 0) == false) {
        return false;
    };
    UserTable::_Pairib insert_result = \
        user_key_table_.insert(UserTable::value_type(user_guid, user));
    ASSERT(insert_result.second);
    return insert_result.second;
};

//bool ns_world::WorldUserManager::RemoveUser(ns_world::WorldUser* user)
//{
//    DWORD user_guid = user->GetUserAccountInfo().user_guid;
//    if (FlowControl::FCAssert(user_guid != 0) == false) {
//        return false;
//    };
//    user_key_table_.erase(user_guid);
//    if (const ns_world::ConnectedCharInfo* char_info = &user->GetConnectedCharInfo())
//    {
//        if (char_info->char_guid) {
//            char_key_table_.erase(char_info->char_guid);
//        };
//        if (char_info->char_name_hash) {
//            char_name_hash_table_.erase(char_info->char_name_hash);
//        };
//    };
//    return true;
//};

void ns_world::WorldUserManager::OnCloseChannel(
    ns_world::community::WorldChannel* closing_channel)
{
    UserTable::const_iterator user_it = user_key_table_.begin();
    const UserTable::const_iterator user_end = user_key_table_.end();
    while (user_it != user_end)
    {
        ns_world::WorldUser* user = user_it->second;
        ++user_it; // pre-increment
        if (user->user_account_info_.channel != closing_channel) {
            continue;
        };

        LogoutInternal(user);
    };
};

template<>
bool ns_world::WorldUserManager::Logout<ns_functor::LogoutOption::Reuse>(
    ns_world::WorldUser* user, const ns_functor::LogoutOption::Reuse& option)
{
    ns_world::WorldUserManager* const this_ = ns_world::WorldUserManager::Instance();
    if (option.notification_)
    {
        // NOTE: notify to related users
        ns_world::FriendMap::Instance()->NotifyOnlineStatusChangedFriendData(user, false);
    };
    //
    bool result = LogoutInternal(user);
    return result;
}

template<>
bool ns_world::WorldUserManager::Logout<ns_functor::LogoutOption::Free>(
    ns_world::WorldUser* user, const ns_functor::LogoutOption::Free& option)
{
    ns_world::WorldUserManager* const this_ = ns_world::WorldUserManager::Instance();
    if (option.notification_)
    {
        // NOTE: notify to related users
        ns_world::FriendMap::Instance()->NotifyOnlineStatusChangedFriendData(user, false);
    };
    //
    bool result = this_->LogoutInternal(user);
    this_->world_user_pool_->Free(user);
    return result;
}

bool ns_world::WorldUserManager::LogoutInternal(
    ns_world::WorldUser* user)
{
    ns_world::WorldUserManager* const this_ = ns_world::WorldUserManager::Instance();

    if (ns_world::GroupManager* group_manager = ns_world::GroupManager::Instance())
    {
        group_manager->ReleaseMember(user);
        user->group_link_.Init(user);
    };
    if (ns_world::FriendMap* friend_map = ns_world::FriendMap::Instance())
    {
        this_->UpdateCharStatusData(user, this_->eUpdateCharStatus_Offline);
        friend_map->ReleaseFriendData(user);
        user->friend_link_.Init(user);
    };
    ;{
        user->memo_link_.Init(user);
    };

    if (user->chat_room_node_.chat_room)
    {
        user->chat_room_node_.chat_room = NULL;
        util::LList::Delete(&user->chat_room_node_);
    };
    if (user->party_node_.link_party)
    {
        user->party_node_.link_party = NULL;
        util::LList::Delete(&user->party_node_);
    };
    if (user->zone_node_.link_zone)
    {
        user->zone_node_.link_zone = NULL;
        util::LList::Delete(&user->zone_node_);
        util::LList::Delete(&user->zone_node_.sector_node);
    };
    if (user->channel_node_.link_channel)
    {
        user->channel_node_.link_channel = NULL;
        util::LList::Delete(&user->channel_node_);
    };

    if (DWORD char_guid = user->connected_char_info_.char_guid) {
        this_->char_key_table_.erase(char_guid);
    };
    if (DWORD char_name_hash = user->connected_char_info_.char_name_hash) {
        this_->char_name_hash_table_.erase(char_name_hash);
    };
    if (DWORD user_guid = user->user_account_info_.user_guid) {
        this_->user_key_table_.erase(user_guid);
    };

    ZeroMemory(&user->connected_char_info_, sizeof(user->connected_char_info_));
    ZeroMemory(&user->user_account_info_, sizeof(user->user_account_info_));

    return true;
}

bool ns_world::WorldUserManager::ChangeUserAccountInfo(
    ns_world::WorldUser* user, const ns_world::UserAccountInfo& changed_user_info)
{
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        return false; // NOTE: world server will have been closed...
    };

    const ns_world::UserAccountInfo origin_info = user->user_account_info_; // copied
    ns_world::UserAccountInfo filtered_info = origin_info;

    //DWORD user_guid;
    //uint8_t channel_id;
    //uint8_t gm_grade;
    //DWORD pc_room_id;
    //TCHAR account_id[ID_MAX_LEN + 1];
    //char ip_address[MAX_IP_STRING_LEN + 1];
    if (origin_info.user_guid != changed_user_info.user_guid)
    {
        ASSERT(origin_info.user_guid == 0 && changed_user_info.user_guid != 0);
        filtered_info.user_guid = changed_user_info.user_guid;
    };
    if (origin_info.channel != changed_user_info.channel)
    {
        //ASSERT(origin_info.channel == 0 && changed_user_info.channel != 0);
        filtered_info.channel = changed_user_info.channel;
    };
    if (origin_info.gm_grade != changed_user_info.gm_grade)
    {
        filtered_info.gm_grade = changed_user_info.gm_grade;
    };
    if (origin_info.pc_room_id != changed_user_info.pc_room_id)
    {
        filtered_info.pc_room_id = changed_user_info.pc_room_id;
    };
    if (::memcmp(origin_info.account_id, changed_user_info.account_id,
                 sizeof(origin_info.account_id)) != 0)
    {
        CopyMemory(filtered_info.account_id, changed_user_info.account_id,
            sizeof(filtered_info.account_id));
    };
    if (::memcmp(origin_info.ip_address, changed_user_info.ip_address,
                 sizeof(origin_info.ip_address)) != 0)
    {
        CopyMemory(filtered_info.ip_address, changed_user_info.ip_address,
            sizeof(filtered_info.ip_address));
    };

    user->user_account_info_ = filtered_info;
    return true;
}

// NOTE: f110922.2L, side-effects = {
//  MSG_DW_FRIEND_GETLIST_SYN,
//  MSG_DW_GM_CHATBLOCKTIME_REQ,
//  MSG_DW_STATUS_INTERCEPTION_REQ
// }
bool ns_world::WorldUserManager::ChangeConnectedCharInfo(
    ns_world::WorldUser* user, const ns_world::ConnectedCharInfo& changed_char_info)
{
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL) {
        return false; // NOTE: world server will have been closed...
    };

    const ns_world::ConnectedCharInfo origin_info = user->connected_char_info_; // copied
    ns_world::ConnectedCharInfo filtered_info = origin_info;
    //
    bool event_char_changes = false;
    bool event_char_attr_changes = false;
    bool event_guild_changes = false;
    bool event_zone_changes = false;
    bool event_sector_changes = false;
    bool event_party_changes = false;
    //bool event_guild_changes = false;
    // relations = { user_guid }
    if (origin_info.user_guid != changed_char_info.user_guid)
    {
        ASSERT(origin_info.user_guid == 0 && changed_char_info.user_guid != 0);
        filtered_info.user_guid = changed_char_info.user_guid;
    };
    // relations = { }
    if (origin_info.char_guid != changed_char_info.char_guid)
    {
        ASSERT(changed_char_info.char_guid != 0);
        filtered_info.char_guid = changed_char_info.char_guid;
        event_char_changes = true;
        if (origin_info.char_guid)
        {
            ns_world::WorldUserManager::Logout(user, ns_functor::LogoutOption::Reuse(false));
            assert(char_key_table_.find(origin_info.char_guid) == char_key_table_.end());
        };
    };
    // relations = { }
    if (origin_info.guild_guid != changed_char_info.guild_guid)
    {
        filtered_info.guild_guid = changed_char_info.guild_guid;
        event_guild_changes = true;
    };
    // relations = { }
    if (origin_info.char_level != changed_char_info.char_level)
    {
        filtered_info.char_level = changed_char_info.char_level;
        event_char_attr_changes = true;
    };
    // relations = { }
    if (origin_info.char_class != changed_char_info.char_class)
    {
        filtered_info.char_class = changed_char_info.char_class;
        event_char_attr_changes = true;
    };
    // relations = { }
    if (origin_info.zone_type != changed_char_info.zone_type)
    {
        filtered_info.zone_type = changed_char_info.zone_type;
        // TODO: ZoneCommunity
        event_zone_changes = true;
    };
    // relations = { }
    if (origin_info.zone_key != changed_char_info.zone_key)
    {
        filtered_info.zone_key = changed_char_info.zone_key;
        // TODO: ZoneCommunity
        event_zone_changes = true;
    };
    // relations = { }
    if (origin_info.map_code != changed_char_info.map_code)
    {
        filtered_info.map_code = changed_char_info.map_code;
        // TODO: ZoneCommunity
        event_zone_changes = true;
    };
    // relations = { }
    if (origin_info.field_code != changed_char_info.field_code)
    {
        filtered_info.field_code = changed_char_info.field_code;
        // TODO: ZoneCommunity
        event_zone_changes = true;
    };
    // relations = { }
    if (origin_info.party_key != changed_char_info.party_key)
    {
        filtered_info.party_key = changed_char_info.party_key;
        // TODO: PartyCommunity
        event_party_changes = true;
    };
    // relations = { }
    if (origin_info.position != changed_char_info.position)
    {
        filtered_info.position = changed_char_info.position;
        // TODO: sector replace event
        event_sector_changes = true;
    };
    // relations = { }
    if (_tcsncmp(origin_info.char_name, changed_char_info.char_name,
                 _countof(origin_info.char_name)) != 0)
    {
        CopyMemory(filtered_info.char_name, changed_char_info.char_name,
            sizeof(filtered_info.char_name));
        // TODO: create char_name_hash
        event_char_attr_changes = true;

        if (origin_info.char_name_hash != 0) {
            char_name_hash_table_.erase(origin_info.char_name_hash);
        };
        bool updated = this->UpdateNameHash(\
            filtered_info.char_guid, filtered_info.char_name, &filtered_info.char_name_hash);
        ASSERT(updated);
        char_name_hash_table_[filtered_info.char_name_hash] = user;
    };

    if (event_zone_changes)
    {
        if (user->zone_node_.link_zone) {
            user->GetUserAccountInfo().channel->LeaveZone(user);
        };
    };
    //
    user->connected_char_info_ = filtered_info;

    if (event_char_changes)
    {
        char_key_table_[filtered_info.char_guid] = user;
        //
        if (ns_world::FriendMap* const friend_map = ns_world::FriendMap::Instance()) {
            friend_map->PrepareFriendServices(user);
        };
        if (ns_world::GroupManager* const group_manager = ns_world::GroupManager::Instance()) {
            group_manager->PrepareGroupServices(user);
        };
        this->GetCharStatusData(filtered_info.char_guid);
        this->UpdateCharStatusData(user, this->eUpdateCharStatus_Online);
    };
    if (event_zone_changes)
    {
        if (filtered_info.zone_key)
        {
            if (FlowControl::FCAssert(user->GetUserAccountInfo().channel->EnterZone(\
                    user, filtered_info)) == false)
            {
                // ...
            };
        };
    };
    if (event_sector_changes && user->zone_node_.link_zone)
    {
        user->zone_node_.link_zone->UpdatePosition(user);
    };
    if (event_party_changes)
    {
        if (filtered_info.party_key)
        {
            user->GetUserAccountInfo().channel->JoinParty(filtered_info.party_key, user);
        };
    };
    //----------------------------------------------------------------------------------------------
    if (event_char_changes)
    {
        ;{
            MSG_DW_FRIEND_GETLIST_SYN msg_syn;
            msg_syn.m_CharGuid = filtered_info.char_guid;
            dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
        };
        ;{
            MSG_DW_GM_CHATBLOCKTIME_REQ msg_syn;
            msg_syn.m_dwKey = filtered_info.char_guid;
            dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
        };
        ;{
            MSG_DW_STATUS_INTERCEPTION_REQ msg_syn;
            msg_syn.m_dwKey    = filtered_info.user_guid;
            msg_syn.m_CharGuid = filtered_info.char_guid;
            dbproxy->SendPacket(&msg_syn, sizeof(msg_syn));
        };
    };
    return true;
}

bool ns_world::WorldUserManager::ChangeChattingMode(
    ns_world::WorldUser* user, const ns_world::ChattingMode& chatting_mode)
{
    // TODO: event handling
    user->chatting_mode_ = chatting_mode;
    return true;
}

void ns_world::WorldUserManager::SendToAll(MSG_BASE* msg_base, uint16_t msg_size)
{
    // NOTE: f110915.1L, use aw_braodcaster
    UserTable::const_iterator user_it = user_key_table_.begin();
    const UserTable::const_iterator user_end = user_key_table_.end();
    for (; user_it != user_end; ++user_it)
    {
        ns_world::WorldUser* user = user_it->second;
        user->SendPacket(msg_base, msg_size);
    };
};

void ns_world::WorldUserManager::UpdateServices(
    const util::ServerTimeInfoPerFrame& time_info,
    ns_world::ServerSessionInWorld* const dbproxy)
{
    if (update_service_execute_timer_.IsExpired() == false) {
        return;
    };
    const ulong aging_counter = time_info.aging_counter_;
    __TOUCHED(aging_counter);
    // updating enabled flags
    const bool chatting_block_update_timer_expired = \
        (dbproxy && chatting_block_update_timer_.IsExpired());
    //
    UserTable::const_iterator user_it = user_key_table_.begin();
    const UserTable::const_iterator user_end = user_key_table_.end();
    for (; user_it != user_end; ++user_it)
    {
        ns_world::WorldUser* const user = user_it->second;
        if (chatting_block_update_timer_expired)
        {
            const ns_world::ChattingMode& chatting_mode = user->GetChattingMode();
            if (chatting_mode.chatting_blocked != false)
            {
                ns_world::ChattingService::UpdateDiscountingBlockedState(user, dbproxy);
            };
        }; //chatting_block_update_timer_expired
    };
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

#if WORLD_CONTENTS_BLOCK

// FriendCharGuid를 친구로 지정한 pUser 정보 추가
BOOL ns_world::WorldUserManager::AddNewFriend(DWORD FriendCharGuid, ns_world::WorldUser* pUser)
{
    // 이 사용자의 친구들 목록 추가
    FRIEND_USER_MAP_ITER    iter;
    FRIEND_ATOM_MAP mapFriendAtom;

    // 친구가 들어오면 바로 데이터 얻을 수 있도록 내 친구 정보를 등록한다
    iter = m_mapUserFriend.find(FriendCharGuid);
    if(iter != m_mapUserFriend.end())
    {
        // 데이터 있으면 그 뒤에 추가
        (*iter).second.insert(FRIEND_ATOM_MAP_PAIR(pUser->GetCharGuid(), pUser));
    }
    else
    {
        // 데이터 없으면 새로 만들어 넣는다
        mapFriendAtom.insert(FRIEND_ATOM_MAP_PAIR(pUser->GetCharGuid(), pUser));
        m_mapUserFriend.insert(FRIEND_USER_MAP_PAIR(FriendCharGuid, mapFriendAtom));
    }

    // 이 친구가 온라인이라면 친구 인스턴스에 '내가 너를 친구 먹었다!'고 저장시킨다
    // 이후 pFriend가 로그아웃하면 pUser가 알 수 있다
    ns_world::WorldUser* pFriend = FindUserByCharGuid(FriendCharGuid);

    if(pFriend)
    {
        // 이 친구가 나를 차단했다면 알려주지 않는다
        if(!pFriend->FindBlock(pUser->GetCharGuid()))
        {
            pFriend->AddOnlineFriend(pUser);
        }
    }

//  MessageOut(eFULL_LOG, "[%s] AddNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid);
    return true;
}

BOOL ns_world::WorldUserManager::AddFriend(ns_world::WorldUser* pUser)
{
    // 이 사용자의 친구들 목록 추가
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
    STRUCT_FRIEND_INFO*     pFriend = pUser->GetFriendMap()->GetFirstFriend();
#else
    STRUCT_FRIEND_INFO*     pFriend = pUser->GetFirstFriend();  
#endif
    FRIEND_USER_MAP_ITER    iter;
    FRIEND_ATOM_MAP mapFriendAtom;

    // 친구가 들어오면 바로 데이터 얻을 수 있도록 내 친구 정보를 등록한다
    if(pFriend)
    {
        do
        {
            iter = m_mapUserFriend.find(pFriend->dwFriendGuid);
            if(iter != m_mapUserFriend.end())
            {
                // 데이터 있으면 그 뒤에 추가
                (*iter).second.insert(FRIEND_ATOM_MAP_PAIR(pUser->GetCharGuid(), pUser));
            }
            else
            {
                // 데이터 없으면 새로 만들어 넣는다
                mapFriendAtom.insert(FRIEND_ATOM_MAP_PAIR(pUser->GetCharGuid(), pUser));
                m_mapUserFriend.insert(FRIEND_USER_MAP_PAIR(pFriend->dwFriendGuid, mapFriendAtom));
            }
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
        }while(pFriend = pUser->GetFriendMap()->GetNextFriend());
#else
        }while(pFriend = pUser->GetNextFriend());
#endif
        
    }

    // 이미 등록된 정보들 중에 나를 등록한 정보가 있으면 넣어준다
    iter = m_mapUserFriend.find(pUser->GetCharGuid());

    if(iter != m_mapUserFriend.end())
    {
        // 있으면 이걸 유저에게 등록
        FRIEND_ATOM_MAP_ITER    iterAtom;

        if(!(*iter).second.empty())
        {
            for(iterAtom = (*iter).second.begin(); iterAtom != (*iter).second.end(); iterAtom++)
            {

                // 내가 이 유저를 차단했다면 알려주지 않는다
                if(!pUser->FindBlock((*iterAtom).second->GetCharGuid()))
                {
                    pUser->AddOnlineFriend((*iterAtom).second);
                }
            }
        }
    }

//  MessageOut(eFULL_LOG, "[%s] AddFriend Complete", pUser->GetCharName().c_str());
    return true;
}

void ns_world::WorldUserManager::RemoveNewFriend(DWORD FriendCharGuid, ns_world::WorldUser* world_user)
{
    FRIEND_USER_MAP_ITER    iter;
    FRIEND_ATOM_MAP_ITER    iterAtom;

    // 친구 목록 데이터에서 이 사용자의 친구들을 찾는다
    iter = m_mapUserFriend.find(FriendCharGuid);
    if(iter != m_mapUserFriend.end())
    {
        // 데이터 있으면
        iterAtom = (*iter).second.find(pUser->GetCharGuid());

        if(iterAtom != (*iter).second.end())
        {
            // 이놈을 친구 삼은 나의 데이터도 있으면 지워준다!
            (*iter).second.erase(iterAtom);
            // 지우고 났더니 맵이 비었으면 맵도 지워준다!
            if((*iter).second.empty())
            {
                m_mapUserFriend.erase(iter);
            }
        }
        else
        {
            // 데이터 없으면 버그
            MessageOut(eCRITICAL_LOG, "[%s] RemoveNewFriend Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), FriendCharGuid);
            return;
        }
    }
    else
    {
        // 데이터 없으면 버그
        MessageOut(eCRITICAL_LOG, "[%s] RemoveNewFriend Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), FriendCharGuid);
        return;
    }

    // 이 친구가 온라인이라면 이쪽에서도 데이터 지워 줘야 한다
    ns_world::WorldUser* pFriend = FindUserByCharGuid(FriendCharGuid);

    if(pFriend)
    {
        pFriend->RemoveOnlineFriend(pUser);
    }

//  MessageOut(eFULL_LOG, "[%s] RemoveNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid);
}

// 친구 목록 업데이트 - 친구 제거!!
void ns_world::WorldUserManager::RemoveFriend(ns_world::WorldUser* world_user)
{
    // 이 사용자의 친구들 정보를 제거한다
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
    STRUCT_FRIEND_INFO*     pFriend = pUser->GetFriendMap()->GetFirstFriend();
#else
    STRUCT_FRIEND_INFO*     pFriend = pUser->GetFirstFriend();  
#endif  
    FRIEND_USER_MAP_ITER    iter;
    FRIEND_ATOM_MAP_ITER    iterAtom;
    ns_world::WorldUser                    *pOnlineUser = NULL;

    // 친구 목록 데이터에서 이 사용자의 친구들을 찾는다
    if(pFriend)
    {
        do
        {
            // 친구 정보 있냐?
            iter = m_mapUserFriend.find(pFriend->dwFriendGuid);
            if(iter != m_mapUserFriend.end())
            {
                // 데이터 있으면
                iterAtom = (*iter).second.find(pUser->GetCharGuid());

                if(iterAtom != (*iter).second.end())
                {
                    // 이놈을 친구 삼은 나의 데이터도 있으면 지워준다!
                    (*iter).second.erase(iterAtom);
                    // 지우고 났더니 맵이 비었으면 맵도 지워준다!
                    if((*iter).second.empty())
                    {
                        m_mapUserFriend.erase(iter);
                    }
                    // 이 친구놈이 온라인이면
                    if(pFriend->sPacketInfo.bOnline)
                    {
                        // 실제 접속중인지 찾아서 온라인 체크 풀어준다
                        pOnlineUser = FindUserByCharGuid(pFriend->dwFriendGuid);
                        if(pOnlineUser)
                        {
                            pOnlineUser->RemoveOnlineFriend(pUser);
                        }
                    }
                }
                else
                {
                    // 데이터 없으면 버그
                    MessageOut(eCRITICAL_LOG, "[%s] RemoveFriend Error - No %s FriendAtomTable", pUser->GetCharName().c_str(), pFriend->sPacketInfo.ptszFriendName);
                }
            }
            else
            {
                // 데이터 없으면 버그
                MessageOut(eCRITICAL_LOG, "[%s] RemoveFriend Error - No %s FriendTable", pUser->GetCharName().c_str(), pFriend->sPacketInfo.ptszFriendName);
                continue;
            }
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
        }while(pFriend = pUser->GetFriendMap()->GetNextFriend());
#else
        }while(pFriend = pUser->GetNextFriend());
#endif      
    }

    // ns_world::WorldUser가 삭제되면서 OnlineFriend 정보도 같이 삭제되므로 굳이 이 ns_world::WorldUser 클래스에 있는 m_mapOnlineFriend를 삭제할 필요는 없다
//  MessageOut(eFULL_LOG, "[%s] RemoveFriend Complete", pUser->GetCharName().c_str());
}

////////////////////////////////////////
// 리벤지 관련
// RevengeCharGuid를 복수 대상으로 지정한 pUser 정보 추가
BOOL ns_world::WorldUserManager::AddNewRevenge(DWORD RevengeCharGuid, ns_world::WorldUser* pUser)
{
    // 이 사용자의 복수 대상들 목록 추가
    FRIEND_USER_MAP_ITER    iter;
    FRIEND_ATOM_MAP mapRevengeAtom;

    // 그놈이 들어오면 바로 데이터 얻을 수 있도록 내 리벤지 정보를 등록한다
    iter = m_mapUserRevenge.find(RevengeCharGuid);
    if(iter != m_mapUserRevenge.end())
    {
        // 데이터 있으면 그 뒤에 추가
        (*iter).second.insert(FRIEND_ATOM_MAP_PAIR(pUser->GetCharGuid(), pUser));
    }
    else
    {
        // 데이터 없으면 새로 만들어 넣는다
        mapRevengeAtom.insert(FRIEND_ATOM_MAP_PAIR(pUser->GetCharGuid(), pUser));
        m_mapUserRevenge.insert(FRIEND_USER_MAP_PAIR(RevengeCharGuid, mapRevengeAtom));
    }

    // 이놈이 온라인이라면 리벤지 인스턴스에 '내가 너를 복수 대상으로 찜했다!'고 저장시킨다
    // 이후 pRevenge가 로그아웃하면 pUser가 알 수 있다
    ns_world::WorldUser* pRevenge = FindUserByCharGuid(RevengeCharGuid);

    if(pRevenge)
    {
        pRevenge->AddOnlineRevenge(pUser);
    }
//  MessageOut(eFULL_LOG, "[%s] AddNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid);
    return true;
}

BOOL ns_world::WorldUserManager::AddRevenge(ns_world::WorldUser* pUser)
{
    // 이 사용자의 복수 대상들 목록 추가
    STRUCT_REVENGE_INFO*    pRevenge = pUser->GetFirstRevenge();
    FRIEND_USER_MAP_ITER    iter;
    FRIEND_ATOM_MAP mapRevengeAtom;

    // 친구가 들어오면 바로 데이터 얻을 수 있도록 내 친구 정보를 등록한다
    if(pRevenge)
    {
        do
        {
            iter = m_mapUserRevenge.find(pRevenge->dwRevengeCharGuid);
            if(iter != m_mapUserRevenge.end())
            {
                // 데이터 있으면 그 뒤에 추가
                (*iter).second.insert(FRIEND_ATOM_MAP_PAIR(pUser->GetCharGuid(), pUser));
            }
            else
            {
                // 데이터 없으면 새로 만들어 넣는다
                mapRevengeAtom.insert(FRIEND_ATOM_MAP_PAIR(pUser->GetCharGuid(), pUser));
                m_mapUserRevenge.insert(FRIEND_USER_MAP_PAIR(pRevenge->dwRevengeCharGuid, mapRevengeAtom));
            }
        }while(pRevenge = pUser->GetNextRevenge());
    }

    // 이미 등록된 정보들 중에 나를 등록한 정보가 있으면 넣어준다
    iter = m_mapUserRevenge.find(pUser->GetCharGuid());

    if(iter != m_mapUserRevenge.end())
    {
        // 있으면 이걸 유저에게 등록
        FRIEND_ATOM_MAP_ITER    iterAtom;

        if(!(*iter).second.empty())
        {
            for(iterAtom = (*iter).second.begin(); iterAtom != (*iter).second.end(); iterAtom++)
            {
                pUser->AddOnlineRevenge((*iterAtom).second);
            }
        }
    }

//  MessageOut(eFULL_LOG, "[%s] AddFriend Complete", pUser->GetCharName().c_str());
    return true;
}

void ns_world::WorldUserManager::RemoveNewRevenge(DWORD RevengeCharGuid, ns_world::WorldUser* world_user)
{
    FRIEND_USER_MAP_ITER    iter;
    FRIEND_ATOM_MAP_ITER    iterAtom;

    // 리벤지 목록 데이터에서 이 사용자의 복수 대상을 찾는다
    iter = m_mapUserRevenge.find(RevengeCharGuid);
    if(iter != m_mapUserRevenge.end())
    {
        // 데이터 있으면
        iterAtom = (*iter).second.find(pUser->GetCharGuid());

        if(iterAtom != (*iter).second.end())
        {
            // 이놈을 복수 대상 삼은 나의 데이터도 있으면 지워준다!
            (*iter).second.erase(iterAtom);
            // 지우고 났더니 맵이 비었으면 맵도 지워준다!
            if((*iter).second.empty())
            {
                m_mapUserRevenge.erase(iter);
            }
        }
        else
        {
            // 데이터 없으면 버그
            MessageOut(eCRITICAL_LOG, "[%s] RemoveNewRevenge Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), RevengeCharGuid);
            return;
        }
    }
    else
    {
        // 데이터 없으면 버그
        MessageOut(eCRITICAL_LOG, "[%s] RemoveNewRevenge Error - No Data[CharGuid:%u]", pUser->GetCharName().c_str(), RevengeCharGuid);
        return;
    }

    // 이놈이 온라인이라면 이쪽에서도 데이터 지워 줘야 한다
    ns_world::WorldUser* pRevenge = FindUserByCharGuid(RevengeCharGuid);

    if(pRevenge)
    {
        pRevenge->RemoveOnlineRevenge(pUser);
    }

//  MessageOut(eFULL_LOG, "[%s] RemoveNewFriend(%u)", pUser->GetCharName().c_str(), FriendCharGuid);
}

// 리벤지 목록 업데이트 - 복수 대상 제거!!
void ns_world::WorldUserManager::RemoveRevenge(ns_world::WorldUser* world_user)
{
    // 이 사용자의 복수 대상들 정보를 제거한다
    STRUCT_REVENGE_INFO*    pRevenge = pUser->GetFirstRevenge();
    FRIEND_USER_MAP_ITER    iter;
    FRIEND_ATOM_MAP_ITER    iterAtom;
    ns_world::WorldUser                    *pOnlineUser = NULL;

    // 리벤지 목록 데이터에서 이 사용자의 친구들을 찾는다
    if(pRevenge)
    {
        do
        {
            // 리벤지 정보 있냐?
            iter = m_mapUserRevenge.find(pRevenge->dwRevengeCharGuid);
            if(iter != m_mapUserRevenge.end())
            {
                // 데이터 있으면
                iterAtom = (*iter).second.find(pUser->GetCharGuid());

                if(iterAtom != (*iter).second.end())
                {
                    // 이놈을 복수 대상 삼은 나의 데이터도 있으면 지워준다!
                    (*iter).second.erase(iterAtom);
                    // 지우고 났더니 맵이 비었으면 맵도 지워준다!
                    if((*iter).second.empty())
                    {
                        m_mapUserRevenge.erase(iter);
                    }
                    // 이놈이 온라인이면
                    if(pRevenge->sPacketInfo.bOnline)
                    {
                        // 실제 접속중인지 찾아서 온라인 체크 풀어준다
                        pOnlineUser = FindUserByCharGuid(pRevenge->dwRevengeCharGuid);
                        if(pOnlineUser)
                        {
                            pOnlineUser->RemoveOnlineRevenge(pUser);
                        }
                    }
                }
                else
                {
                    // 데이터 없으면 버그
                    MessageOut(eCRITICAL_LOG, "[%s] RemoveRevenge Error - No %s FriendAtomTable", pUser->GetCharName().c_str(), pRevenge->sPacketInfo.ptszRevengeName);
                }
            }
            else
            {
                // 데이터 없으면 버그
                MessageOut(eCRITICAL_LOG, "[%s] RemoveRevenge Error - No %s RevengeTable", pUser->GetCharName().c_str(), pRevenge->sPacketInfo.ptszRevengeName);
                continue;
            }
        }while(pRevenge = pUser->GetNextRevenge());
    }

    // ns_world::WorldUser가 삭제되면서 OnlineFriend 정보도 같이 삭제되므로 굳이 이 ns_world::WorldUser 클래스에 있는 m_mapOnlineFriend를 삭제할 필요는 없다
//  MessageOut(eFULL_LOG, "[%s] RemoveFriend Complete", pUser->GetCharName().c_str());
}

// 채팅금지자 상태 업데이트
void ns_world::WorldUserManager::UpdateChatBlockUser()
{
    CHATBLOCK_USER_MAP_ITER iter;
    ns_world::WorldUser                    *pUser;
    STLX_VECTOR<ns_world::WorldUser*>      vecRemoveUser;
    STLX_VECTOR<ns_world::WorldUser*>::iterator    iter_vec;

    vecRemoveUser.clear();

    for(iter = m_mapChatBlocked.begin(); iter != m_mapChatBlocked.end(); ++iter)
    {
        pUser = (*iter).second;

        if(!pUser)
        {
            MessageOut(eCRITICAL_LOG, "ns_world::WorldUserManager ChatBlock Update Error - ns_world::WorldUser NULL");
            break;
        }

        if(!pUser->UpdateBlockTime())
        {
            vecRemoveUser.push_back(pUser);
        }
    }

    for(iter_vec = vecRemoveUser.begin(); iter_vec != vecRemoveUser.end(); ++iter_vec)
    {
        pUser = (*iter_vec);
        if(!pUser)
        {
            continue;
        }
        RemoveChatBlockUser(pUser);
    }

    MessageOut(eFULL_LOG, "UpdateChatBlockTime[%u]", m_mapChatBlocked.size());
}

#endif //WORLD_CONTENTS_BLOCK