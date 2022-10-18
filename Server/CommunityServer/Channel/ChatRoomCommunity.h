#pragma once
#ifndef WORLDSERVER_FREE_COMMUNITY_H
#define WORLDSERVER_FREE_COMMUNITY_H

#include "./ICommunity.h"

namespace ns_world { namespace community {
;

class ChatRoomCommunity : public ns_world::community::ICommunity
{
public:
    static const int kMaxNumberOfJoinedMembers = MAX_CHATZONE_USER;
    //
    ChatRoomCommunity();
    virtual ~ChatRoomCommunity();
    //
    // ICommunity interface
    virtual eCommunityType GetCommunityType() const;
    virtual bool EnabledChat() const;
    virtual void ConfigureChatEnabled(bool enabled);
    // ICommunity interface
    virtual bool Enter(WorldUser* user, const ns_world::ConnectedCharInfo& char_info);
    // ICommunity interface
    virtual bool Leave(WorldUser* user, ns_world::ConnectedCharInfo* const backup_info);
    // ICommunity interface
    virtual bool SendToAll(MSG_BASE* msg_base, uint16_t msg_size) const;
    //
    bool RegisterRoomName(const util::StrArrayForRoomTitle& chat_room_name);
    const util::StrArrayForRoomTitle& GetChatRoomName() const;
    size_t GetNumberOfJoinedUsers() const;
private:
    typedef util::__LList<ns_world::community::ChatRoomUserNode> ChatRoomUserNodeList;
    //
    ulong chat_room_guid_;
    util::StrArrayForRoomTitle chat_room_name_;
    ChatRoomUserNodeList user_list_;
    //
    friend class ns_world::WorldChannelManager;
    __DISABLE_COPY(ChatRoomCommunity);
};

}}; //end of namespace

inline const util::StrArrayForRoomTitle&
ns_world::community::ChatRoomCommunity::GetChatRoomName() const
{
    return chat_room_name_;
}

#endif //WORLDSERVER_FREE_COMMUNITY_H