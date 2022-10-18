#include "WorldServerPch.hxx"
#include "./ChatRoomCommunity.h"

#include "Objects/WorldUser.h"
#include "Channel/WorldChannelManager.h"

//==================================================================================================

ns_world::community::ChatRoomCommunity::ChatRoomCommunity()
    : chat_room_guid_(0)
{
    ZeroMemory(chat_room_name_, sizeof(chat_room_name_));
    util::LList::Init(&user_list_);
}

ns_world::community::ChatRoomCommunity::~ChatRoomCommunity()
{
    ChatRoomUserNode* it = user_list_.next;
    while (it != &user_list_)
    {
        if (ns_world::ConnectedCharInfo* char_info = &it->base->connected_char_info_)
        {
            __TOUCHED(char_info);
        };
        it->chat_room = NULL;
        it = util::LList::Delete(it);
    };
}

// ICommunity interface
ns_world::community::eCommunityType ns_world::community::ChatRoomCommunity::GetCommunityType() const
{
    return eCommunity_None;
}

bool ns_world::community::ChatRoomCommunity::EnabledChat() const
{
    if (chat_room_name_[0] == _T('\0')) {
        return false; // does not prepared
    };
    return true;
}

void ns_world::community::ChatRoomCommunity::ConfigureChatEnabled(bool enabled)
{
    __UNUSED(enabled);
    // nothing to do
}

// ICommunity interface
bool ns_world::community::ChatRoomCommunity::Enter(
    WorldUser* user, const ns_world::ConnectedCharInfo& char_info)
{
    return false;
}

// ICommunity interface
bool ns_world::community::ChatRoomCommunity::Leave(
    WorldUser* user, ns_world::ConnectedCharInfo* const backup_info)
{
    return false;
}

// ICommunity interface
bool ns_world::community::ChatRoomCommunity::SendToAll(
    MSG_BASE* msg_base, uint16_t msg_size) const
{
    // TODO: use composed message broadcaster
    const ChatRoomUserNode* it = user_list_.next;
    for (; it != &user_list_; it = it->next)
    {
        it->base->SendPacket(msg_base, msg_size);
    };
    return true;
};

bool ns_world::community::ChatRoomCommunity::RegisterRoomName(
    const util::StrArrayForRoomTitle& chat_room_name)
{
    _tcsncpy(chat_room_name_, chat_room_name, _countof(chat_room_name_));
    chat_room_name_[_countof(chat_room_name_) - 1] = _T('\0');
    chat_room_guid_ = ns_world::WorldChannelManager::GetChatRoomGuid(chat_room_name_);
    return true;
}

size_t ns_world::community::ChatRoomCommunity::GetNumberOfJoinedUsers() const
{
    size_t counter = 0;
    const ChatRoomUserNodeList* it = user_list_.next;
    for (; it != &user_list_; it = it->next) {
        ++counter;
    };
    return counter;
}