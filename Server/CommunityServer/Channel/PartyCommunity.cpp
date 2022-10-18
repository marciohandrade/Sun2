#include "WorldServerPch.hxx"
#include "./PartyCommunity.h"

#include "Objects/WorldUser.h"

//==================================================================================================

ns_world::community::PartyCommunity::PartyCommunity(uint16_t party_key)
    : party_key_(party_key)
{
    util::LList::Init(&user_list_);
}

ns_world::community::PartyCommunity::~PartyCommunity()
{
    PartyUserNode* it = user_list_.next;
    while (it != &user_list_)
    {
        if (ns_world::ConnectedCharInfo* char_info = &it->base->connected_char_info_)
        {
            char_info->party_key = 0;
        };
        it->link_party = NULL;
        it = util::LList::Delete(it);
    };
}

// ICommunity interface
ns_world::community::eCommunityType ns_world::community::PartyCommunity::GetCommunityType() const
{
    return eCommunity_Party;
}

bool ns_world::community::PartyCommunity::EnabledChat() const
{
    return true;
}

void ns_world::community::PartyCommunity::ConfigureChatEnabled(bool enabled)
{
    __UNUSED(enabled);
    // nothing to do
}

//void ns_world::community::PartyCommunity::Init(uint16_t party_key)
//{
//    party_key_ = party_key;
//}

// ICommunity interface
bool ns_world::community::PartyCommunity::Enter(
    WorldUser* user, const ns_world::ConnectedCharInfo& char_info)
{
    util::LList::Delete(&user->party_node_);
    user->party_node_.link_party = this;
    util::LList::AddPrev(&user->party_node_, &user_list_);

    return true;
}

// ICommunity interface
bool ns_world::community::PartyCommunity::Leave(
    WorldUser* user, ns_world::ConnectedCharInfo* const backup_info)
{
    util::LList::Delete(&user->party_node_);
    user->party_node_.link_party = NULL;
    return true;
}

// ICommunity interface
bool ns_world::community::PartyCommunity::SendToAll(MSG_BASE* msg_base, uint16_t msg_size) const
{
    const PartyUserNode* it = user_list_.next;
    for (; it != &user_list_; it = it->next)
    {
        it->base->SendPacket(msg_base, msg_size);
    };
    return true;
};

void ns_world::community::PartyCommunity::Disperse()
{
    PartyUserNode* it = user_list_.next;
    while (it != &user_list_)
    {
        it->link_party = NULL;
        it = util::LList::Delete(it);
    };
}

