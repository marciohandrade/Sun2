#pragma once

#ifndef DBPROXY_GUILD_FACILITY_H
#define DBPROXY_GUILD_FACILITY_H

#include "Struct.h"
#include "GuildFacilityInfoParser.h"

typedef STLX_MAP<FCODE, GUILD_FACILITY_INFO> GUILD_FACILITY_MAP;
typedef GUILD_FACILITY_MAP::iterator GUILD_FACILITY_MAP_ITERATOR;

class GuildFacilities
{
public:
    GuildFacilities(void);
    ~GuildFacilities(void){};

    // 길드 시설 인터페이스 
    const void                 SetGuildFacility(GUILD_FACILITY_INFO info);
    const GUILD_FACILITY_INFO* GetGuildFacility(FCODE code);
    const void                 GetGuildFacilityTotalInfo(OUT GUILD_FACILITY_TOTAL_INFO& guild_facility_total_info);
    const GUILD_FACILITY_MAP*  GetGuildFacilityMap();
    const void                 SetLoaded(bool is_load);
    const bool                 GetLoaded();

private:
    bool loaded_;
    GUILD_FACILITY_MAP guild_facility_map_;
};

#endif // DBPROXY_GUILD_FACILITY_H
