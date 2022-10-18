#pragma once
#ifndef WORLDSERVER_WORLD_USER_H
#define WORLDSERVER_WORLD_USER_H

#include "Channel/ICommunity.h"

//==================================================================================================

namespace ns_world {
;

class WorldUser
{
public:
    //
    WorldUser();
    virtual ~WorldUser();

    void Init();
    //void Release(); <- replacement. use 'ns_world::WorldUserManager::Logout'
    void Update();

    const ns_world::UserAccountInfo& GetUserAccountInfo() const;
    const ns_world::ConnectedCharInfo& GetConnectedCharInfo() const;
    const ns_world::ChattingMode& GetChattingMode() const;
    const ns_world::GroupLink& GetGroupInfo() const;
    const ns_world::FriendLink& GetFriendInfo() const;
    const ns_world::BlockCharLink& GetBlockCharInfo() const;
    const ns_world::RevengeLink& GetRevengeInfo() const;
    const ns_world::MemoLink& GetMemoInfo() const;

    bool IsGMUser() const;
    DWORD GetUserGuid() const;
    DWORD GetCharGuid() const;
    const util::StrArrayForCharName& GetCharName() const;
    //
    //void ConfigureAccountInfo(const ns_world::UserAccountInfo& user_account_info);
    //void ConfigureConnectedCharInfo(const ns_world::ConnectedCharInfo& connected_char_info);
    //void ConfigureChattingMode(const ns_world::ChattingMode& banned_list);
    //bool LogoutCharacter();
    // external interface, does not call in ICommunity object
    bool LeaveZone();
    bool EnabledChatting();
    void NotifyChattingDisabledState(RC::eCHAT_RESULT result) const;
    // message controllers
    bool EnabledSendMessage() const;
    // send the actor marked message
    bool SendPacket(MSG_BASE* const msg_base, uint16_t msg_size) const;
    // message broadcaster
    bool SendPacketBelongToChannel(MSG_BASE* const msg_base, uint16_t msg_size) const;
    // message broadcaster
    bool SendPacketBelongToZone(MSG_BASE* const msg_base, uint16_t msg_size) const;
    // message broadcaster
    bool SendPacketToParty(MSG_BASE* const msg_base, uint16_t msg_size) const;
    // message broadcaster
    bool SendPacketToGuild(MSG_BASE* const msg_base, uint16_t msg_size) const;
    // chatting message broadcaster
    bool SendNormalChat(MSG_BASE* const msg_base, uint16_t msg_size) const;
    // chatting message broadcaster
    bool SendPacketShouting(MSG_BASE* const msg_base, uint16_t msg_size,
        const ns_world::WorldShoutingArg* whole_shouting_arg);
    //bool SendWorldChat(MSG_BASE* const msg_base, uint16_t msg_size, bool now);
    // message broadcaster for user relations
    bool SendToAllOnlineFriends(MSG_BASE* const msg_base, uint16_t msg_size);
    // message broadcaster for user relations
    bool SendToAllRevengers(MSG_BASE* const msg_base, uint16_t msg_size);
private:
    //
    // data fields...
    uint8_t pod_field_start_[1];
    ns_world::community::ChannelUserNode channel_node_;
    ns_world::community::ZoneUserNode zone_node_;
    ns_world::community::PartyUserNode party_node_;
    ns_world::community::ChatRoomUserNode chat_room_node_;
    ns_world::UserAccountInfo user_account_info_;
    ns_world::ConnectedCharInfo connected_char_info_;
    ns_world::ChattingMode chatting_mode_;
    ns_world::GroupLink group_link_;
    ns_world::FriendLink friend_link_;
    ns_world::BlockCharLink block_char_link_;
    ns_world::RevengeLink revenge_link_;
    ns_world::MemoLink memo_link_;
    uint8_t pod_field_end_[1];
    //
    friend class ns_world::WorldUserManager;
    friend class ns_world::WorldChannelManager;
    friend class ns_world::community::WorldChannel;
    friend class ns_world::community::ZoneCommunity;
    friend class ns_world::community::PartyCommunity;
    friend class ns_world::community::ChatRoomCommunity;
    __DISABLE_COPY(WorldUser);
}; //end of class 'WorldUser'

}; //end of namespace

//==================================================================================================

inline const ns_world::UserAccountInfo& ns_world::WorldUser::GetUserAccountInfo() const {
    return user_account_info_;
};

inline const ns_world::ConnectedCharInfo& ns_world::WorldUser::GetConnectedCharInfo() const {
    return connected_char_info_;
};

inline const ns_world::ChattingMode& ns_world::WorldUser::GetChattingMode() const {
    return chatting_mode_;
};

inline const ns_world::GroupLink& ns_world::WorldUser::GetGroupInfo() const {
    return group_link_;
};

inline const ns_world::FriendLink& ns_world::WorldUser::GetFriendInfo() const {
    return friend_link_;
};

inline const ns_world::BlockCharLink& ns_world::WorldUser::GetBlockCharInfo() const {
    return block_char_link_;
};

inline const ns_world::RevengeLink& ns_world::WorldUser::GetRevengeInfo() const {
    return revenge_link_;
};

inline const ns_world::MemoLink& ns_world::WorldUser::GetMemoInfo() const {
    return memo_link_;
};

inline bool ns_world::WorldUser::IsGMUser() const {
    return (user_account_info_.gm_grade != 0); // TODO: gm-grade validation
};

inline DWORD ns_world::WorldUser::GetUserGuid() const {
    return user_account_info_.user_guid;
};

inline DWORD ns_world::WorldUser::GetCharGuid() const {
    return connected_char_info_.char_guid;
};

inline const util::StrArrayForCharName& ns_world::WorldUser::GetCharName() const {
    return connected_char_info_.char_name;
};

//==================================================================================================

#endif //WORLDSERVER_WORLD_USER_H