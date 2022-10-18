#include "stdAfx.h"
#include "GuildFacilities.h"

GuildFacilities::GuildFacilities( void )
{
    loaded_ = false;
}

const void GuildFacilities::SetGuildFacility( GUILD_FACILITY_INFO info )
{
    GUILD_FACILITY_MAP_ITERATOR find_iter = guild_facility_map_.find(info.code);
    if (find_iter != guild_facility_map_.end())
    {
        GUILD_FACILITY_INFO& cur_info = find_iter->second;
        cur_info = info;
    }
    else
    {
        guild_facility_map_.insert(std::make_pair(info.code, info));
    }
}

const GUILD_FACILITY_INFO* GuildFacilities::GetGuildFacility( FCODE code )
{
    GUILD_FACILITY_MAP_ITERATOR find_iter = guild_facility_map_.find(code);
    if (find_iter == guild_facility_map_.end())
    {
        return NULL;
    }

    return &(find_iter->second);
}

const void GuildFacilities::GetGuildFacilityTotalInfo( OUT GUILD_FACILITY_TOTAL_INFO& guild_facility_total_info )
{
    GUILD_FACILITY_MAP_ITERATOR cur_iter = guild_facility_map_.begin();
    GUILD_FACILITY_MAP_ITERATOR end_iter = guild_facility_map_.end();

    int total_info_count = 0;
    for (; cur_iter != end_iter; ++cur_iter)
    {
        if (total_info_count >= MAX_GUILD_FACILITY)
        {
            break;
        }

        guild_facility_total_info.m_GuildFacilityInfo[total_info_count++] = cur_iter->second;
    }

    guild_facility_total_info.m_count = total_info_count;
}

const GUILD_FACILITY_MAP* GuildFacilities::GetGuildFacilityMap()
{
    return &guild_facility_map_;
}

const void GuildFacilities::SetLoaded( bool is_load )
{
    loaded_ = is_load;
}

const bool GuildFacilities::GetLoaded()
{
    return loaded_;
}
