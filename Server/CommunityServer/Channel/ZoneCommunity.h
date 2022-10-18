#pragma once
#ifndef WORLDSERVER_ZONE_COMMUNITY_H
#define WORLDSERVER_ZONE_COMMUNITY_H

#include "./ICommunity.h"
#include "./SectorGrid.h"

namespace ns_world { namespace community {
;

class ZoneCommunity : public ns_world::community::ICommunity
{
public:
    ZoneCommunity(DWORD zone_key, const sMAPINFO* map_info,
        const ns_world::community::FieldInfo* field_info);
    virtual ~ZoneCommunity();
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
    bool SendPacketAround(MSG_BASE* msg_base, uint16_t msg_size,
        const ns_world::WorldUser* talker) const;
    //
    bool UpdatePosition(ns_world::WorldUser* user);
    //
    bool UsedSectorControl() const;
    DWORD GetZoneKey() const;
    bool Empty() const;
    //
private:
    //
    void RegisterShoutRange(const ns_world::community::ShoutRange* shout_range);
    //
    const DWORD zone_key_;
    const sMAPINFO* const map_info_;
    const FieldInfo* const field_info_;
    const ns_world::community::ShoutRange* shout_range_;
    util::__LList<ns_world::community::ZoneUserNode> user_list_;
    ns_world::grid::SectorGrid sectors_;
    //
    bool blocked_chat_;
    //
    friend class ns_world::WorldChannelManager;
    friend class ns_world::community::WorldChannel;
    __DISABLE_COPY(ZoneCommunity);
};

}}; //end of namespace


inline DWORD ns_world::community::ZoneCommunity::GetZoneKey() const
{
    return zone_key_;
};

inline bool ns_world::community::ZoneCommunity::UsedSectorControl() const {
    return (field_info_ != NULL);
};

#endif //WORLDSERVER_ZONE_COMMUNITY_H