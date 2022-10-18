#include "WorldServerPch.hxx"
#include "./ZoneCommunity.h"

#include "./FieldInfo.h"
#include "Objects/WorldUser.h"

//==================================================================================================

ns_world::community::ZoneCommunity::ZoneCommunity(
    DWORD zone_key, const sMAPINFO* map_info,
    const ns_world::community::FieldInfo* field_info)
    : zone_key_(zone_key)
    , map_info_(map_info)
    , field_info_(field_info)
    , shout_range_(NULL)
    , blocked_chat_(false)
{
    util::LList::Init(&user_list_);
    if (field_info_ != NULL) {
        sectors_.Build(field_info_->GetSectorGridInfo());
    };
}

ns_world::community::ZoneCommunity::~ZoneCommunity()
{
    ASSERT(util::LList::IsEmpty(&user_list_));
    ZoneUserNode* it = user_list_.next;
    while (it != &user_list_)
    {
        if (ns_world::ConnectedCharInfo* char_info = &it->base->connected_char_info_)
        {
            char_info->zone_type = 0;
            char_info->zone_key = 0;
            char_info->map_code = 0;
            char_info->field_code = 0;
            ZeroMemory(&char_info->position, sizeof(char_info->position));
        };
        it->link_zone = NULL;
        util::LList::Delete(&it->sector_node);
        //
        it = util::LList::Delete(it);
    };
}

// ICommunity interface
ns_world::community::eCommunityType ns_world::community::ZoneCommunity::GetCommunityType() const
{
    return eCommunity_Zone;
}

bool ns_world::community::ZoneCommunity::EnabledChat() const
{
    return (blocked_chat_ == false);
}

void ns_world::community::ZoneCommunity::ConfigureChatEnabled(bool enabled)
{
    blocked_chat_ = !enabled;
}

// ICommunity interface
bool ns_world::community::ZoneCommunity::Enter(
    WorldUser* user, const ns_world::ConnectedCharInfo& char_info)
{
    ns_world::community::ZoneUserNode* const user_node = &user->zone_node_;
    util::LList::Delete(user_node);
    util::LList::Delete(&user_node->sector_node);
    //
    user_node->link_zone = this;
    util::LList::AddPrev(user_node, &user_list_);
    sectors_.Link(&user_node->sector_node, char_info.position);

    return true;
}

// ICommunity interface
bool ns_world::community::ZoneCommunity::Leave(
    WorldUser* user, ns_world::ConnectedCharInfo* const backup_info)
{
    util::LList::Delete(&user->zone_node_);
    util::LList::Delete(&user->zone_node_.sector_node);
    user->zone_node_.link_zone = NULL;
    return true;
}

bool ns_world::community::ZoneCommunity::SendToAll(MSG_BASE* msg_base, uint16_t msg_size) const
{
    // TODO: use 1toN broadcaster
    ZoneUserNode* it = user_list_.next;
    for (; it != &user_list_; it = it->next)
    {
        ns_world::WorldUser* user = it->base;
        user->SendPacket(msg_base, msg_size);
    };
    return true;
}

bool ns_world::community::ZoneCommunity::SendPacketAround(
    MSG_BASE* msg_base, uint16_t msg_size, const ns_world::WorldUser* talker) const
{
    assert(this != NULL && this == talker->zone_node_.link_zone);
    if (field_info_ == NULL) {
        return this->SendToAll(msg_base, msg_size);
    };
    //
    const ns_world::ConnectedCharInfo& char_info = talker->GetConnectedCharInfo();

    if (sectors_.IsNullVector(&char_info.position)) {
        // unset position
        return false;
    };
    // pseudo code
    // #1. find position
    // #2. get sector array based on position
    // #3. foreach(sector in sectors)
    //          foreach(user in sector)
    //              user->SendPacket
    //
    const ns_world::grid::SECTOR_IDX center_index = sectors_.GetSectorIndex(&char_info.position);
    if (center_index == INVALID_SECTOR_INDEX) {
        return false;
    };
    //
    ns_world::grid::SectorGrid3x3 grid;
    if (sectors_.GetAroundSectors(center_index, &grid) == false) {
        return false;
    };
    FOREACH_ARRAY(const ns_world::grid::SECTOR_IDX sector_index, grid.Grid,
                  ns_world::grid::SectorGrid3x3::GridArray)
    {
        if (sector_index == INVALID_SECTOR_INDEX) {
            continue;
        };
        const ns_world::grid::SectorGrid::Node* const sector_root = \
            sectors_.GetSector(sector_index);
        if (sector_root == NULL)
        {
            assert(!"unexpected state");
            continue;
        };
        const ns_world::grid::SectorGrid::Node* member_it = sector_root->next;
        for (; member_it != sector_root; member_it = member_it->next)
        {
            member_it->user->SendPacket(msg_base, msg_size);
        };
    };
    return true;
}

void ns_world::community::ZoneCommunity::RegisterShoutRange(
    const ns_world::community::ShoutRange* shout_range)
{
    shout_range_ = shout_range;
};

bool ns_world::community::ZoneCommunity::UpdatePosition(ns_world::WorldUser* user)
{
    if (field_info_ == NULL) {
        // no sector control
        return true;
    };
    const ns_world::ConnectedCharInfo& char_info = user->GetConnectedCharInfo();
    return sectors_.Link(&user->zone_node_.sector_node, char_info.position);
}

bool ns_world::community::ZoneCommunity::Empty() const
{
    return util::LList::IsEmpty(&user_list_);
};