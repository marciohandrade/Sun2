#pragma once
#ifndef WORLDSERVER_PARTY_COMMUNITY_H
#define WORLDSERVER_PARTY_COMMUNITY_H

#include "./ICommunity.h"

namespace ns_world { namespace community {
;

class PartyCommunity : public ns_world::community::ICommunity
{
public:
    explicit PartyCommunity(uint16_t party_key);
    virtual ~PartyCommunity();
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
    uint16_t GetPartyKey() const;
    bool Empty() const;
    void Disperse();

private:
    // data fields...
    uint16_t party_key_;
    util::__LList<ns_world::community::PartyUserNode> user_list_;
    //
    __DISABLE_COPY(PartyCommunity);
};

}}; //end of namespace


inline uint16_t ns_world::community::PartyCommunity::GetPartyKey() const {
    return party_key_;
};

inline bool ns_world::community::PartyCommunity::Empty() const {
    return util::LList::IsEmpty(&user_list_);
};

#endif //WORLDSERVER_PARTY_COMMUNITY_H