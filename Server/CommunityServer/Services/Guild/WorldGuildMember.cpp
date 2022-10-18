#include "WorldServerPch.hxx"
#include "./WorldGuildMember.h"

__IMPL_GUILDPOOL(ns_world::WorldGuildMember);

ns_world::WorldGuildMember::WorldGuildMember()
{
}

ns_world::WorldGuildMember::~WorldGuildMember()
{
}

void ns_world::WorldGuildMember::Serialize(
    INOUT BASE_GUILD_MEMBER_INFO& base_guild_member_info, eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        base_guild_member_info = base_guild_member_info_;
    }
    else
    {
        base_guild_member_info_ = base_guild_member_info;
    }
}
