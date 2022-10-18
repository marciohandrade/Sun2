#pragma once
#ifndef WORLDSERVER_ICOMMUNITY_H
#define WORLDSERVER_ICOMMUNITY_H

#include <LinkedListCS.h>

#include "Channel/SectorGrid.h"

//==================================================================================================

namespace ns_world { namespace community {
;

enum eCommunityType
{
    eCommunity_None = 0,
    eCommunity_WorldChannel,
    eCommunity_Zone,
    eCommunity_Party,
    eCommunity_Free,
    //
    eCommunity_Counts
};

struct ICommunity
{
    virtual eCommunityType GetCommunityType() const = 0;
    virtual bool EnabledChat() const = 0;
    virtual void ConfigureChatEnabled(bool enabled) = 0;
    virtual bool Enter(WorldUser* user, const ns_world::ConnectedCharInfo& char_info) = 0;
    virtual bool Leave(WorldUser* user, ns_world::ConnectedCharInfo* const backup_info) = 0;
    //
    virtual bool SendToAll(MSG_BASE* msg_base, uint16_t msg_size) const = 0;
};

//==================================================================================================

class WorldChannel;
class ZoneCommunity;
class PartyCommunity;
class ChatRoomCommunity;

struct ChannelUserNode : public util::__LList<ChannelUserNode>
{
    ns_world::WorldUser* base;
    ns_world::community::WorldChannel* link_channel;
};

struct ZoneUserNode : public util::__LList<ZoneUserNode>
{
    ns_world::WorldUser* base;
    ns_world::community::ZoneCommunity* link_zone;
    ns_world::grid::SectorGrid::Node sector_node;
};

struct PartyUserNode : public util::__LList<PartyUserNode>
{
    ns_world::WorldUser* base;
    ns_world::community::PartyCommunity* link_party;
};

struct ChatRoomUserNode : public util::__LList<ChatRoomUserNode>
{
    ns_world::WorldUser* base;
    ns_world::community::ChatRoomCommunity* chat_room;
};

// 월드서버 외치기용 연결 존 정보 (references: 'shoutrange.txt')
struct ShoutRange
{
    MAPCODE map_code;
    uint8_t zone_type; // < 'MKind', eZONETYPE
    MAPCODE linked_map_codes[7]; // LField1~7
    ns_world::community::ZoneCommunity* linked_zone[7];
    char map_name[MAX_MAPNAME_LENGTH + 1]; // for debug 'MName'
};

typedef STLX_HASH_MAP<DWORD, ns_world::community::ShoutRange> ShoutRangeTable;

}}; //end of namespace
//==================================================================================================

#endif //WORLDSERVER_ICOMMUNITY_H