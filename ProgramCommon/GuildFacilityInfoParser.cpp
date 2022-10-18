#include "StdAfx.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#include "GuildFacilityInfoParser.h"

BOOL GuildFacilityInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    __UNUSED((script_code, is_reload));

    int row_size = GetRowSize();
    for (int script_row = 0; script_row < row_size; ++script_row)
    {
        FACILITY_INFO guild_facility_info;

        guild_facility_info.facility_name = std::string(GetDataString("facility_name", script_row));
        GetData(script_row, guild_facility_info.facility_index, "facility_index");
        GetData(script_row, guild_facility_info.facility_code, "facility_code");
        GetData(script_row, guild_facility_info.facility_level, "facility_level");
        GetData(script_row, guild_facility_info.upgrade_type, "upgrade_type");
        GetData(script_row, guild_facility_info.upgrade_guild_point, "upgrade_guild_point");
        GetData(script_row, guild_facility_info.upgrade_item_code, "upgrade_item_code");
        GetData(script_row, guild_facility_info.upgrade_item_num, "upgrade_item_num");

        //패시브 옵션
        GetData(script_row, guild_facility_info.passive_option.option_type, "passive_option_type");
        GetData(script_row, guild_facility_info.passive_option.option_value_type, "passive_option_value_type");
        GetData(script_row, guild_facility_info.passive_option.option_value, "passive_option_value");

        //액티브 옵션
        GetData(script_row, guild_facility_info.active_option.active_item_code, "active_item_code");
        GetData(script_row, guild_facility_info.active_option.active_item_count, "active_item_count");
        GetData(script_row, guild_facility_info.active_option.active_skill_code, "active_skill_code");

        if (IsValidFacilityInfo(&guild_facility_info) == false)
        {
            continue;
        }

        FACILITYTAG facility_tag = std::make_pair(guild_facility_info.facility_code, guild_facility_info.facility_level);
        facility_map.insert(std::make_pair(facility_tag, guild_facility_info));
    }

    return TRUE;
}

const FACILITY_INFO* GuildFacilityInfoParser::GetGuildFacilityInfo(FCODE  facility_code, FLEVEL facility_level)
{
    FACILITY_MAP::iterator find_iter = facility_map.find(std::make_pair(facility_code, facility_level));
    if (find_iter == facility_map.end())
    {
        return NULL;
    }

    return &(find_iter->second);
}

const bool GuildFacilityInfoParser::IsValidFacilityInfo(FACILITY_INFO* info)
{
    if (info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%s] 시설 정보가 올바르지 않습니다."), __FUNCTION__);
        ASSERT(false);
        return false;
    }

    if (info->facility_code <= 0)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%s] 시설 코드가 올바르지 않습니다.(%d)"), __FUNCTION__, info->facility_code);
        ASSERT(false);
        return false;
    }

    const FACILITY_INFO* prev_info = GetGuildFacilityInfo(info->facility_code, info->facility_level);
    if (prev_info)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%s] 같은 시설이 존재합니다. (%d, %d)"), __FUNCTION__, info->facility_code, info->facility_level);
        ASSERT(false);
        return false;
    }

    return true;
}

const FACILITY_OPTION_PASSIVE* GuildFacilityInfoParser::GetGuildFacilityPassive( FCODE facility_code, FLEVEL facility_level )
{
    const FACILITY_INFO* facility_info = GetGuildFacilityInfo(facility_code, facility_level);
    if (facility_info == NULL)
    {
        return NULL;
    }

    return &(facility_info->passive_option);
}

const FACILITY_OPTION_ACTIVE* GuildFacilityInfoParser::GetGuildFacilityActive( FCODE facility_code, FLEVEL facility_level )
{
    const FACILITY_INFO* facility_info = GetGuildFacilityInfo(facility_code, facility_level);
    if (facility_info == NULL)
    {
        return NULL;
    }

    return &(facility_info->active_option);
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
