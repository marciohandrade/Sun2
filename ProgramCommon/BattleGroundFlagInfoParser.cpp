#include "stdafx.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "BattleGroundFlagInfoParser.h"

BattleGroundFlagInfo::BattleGroundFlagInfo() :
collection_id(0),
strong_point_key(0),
strong_point_name(0)
{
    vector_flag_key.clear();
}
BattleGroundFlagInfo::~BattleGroundFlagInfo()
{
    vector_flag_key.clear();
}


BattleGroundFlagInfoParser::BattleGroundFlagInfoParser()
{
}

BattleGroundFlagInfoParser::~BattleGroundFlagInfoParser()
{
    Unload();
}

void BattleGroundFlagInfoParser::Unload()
{
    BattleGroundFlagInfo* flag_info = NULL;

    BATTLE_GROUND_FLAG_INFO_MAP_ITER itr = data_table_.begin();
    BATTLE_GROUND_FLAG_INFO_MAP_ITER itr_end = data_table_.end();
    for (; itr != itr_end; ++itr)
    {
        BattleGroundFlagInfo* flag_info = itr->second;
        delete flag_info;
    }
    data_table_.clear();
}

BattleGroundFlagInfo* BattleGroundFlagInfoParser::GetBattleGroundFlagInfo(const MAPCODE map_code, const DWORD collection_id)
{
    BATTLE_GROUND_FLAG_INFO_MAP_ITER itr;
    std::pair<BATTLE_GROUND_FLAG_INFO_MAP_ITER, BATTLE_GROUND_FLAG_INFO_MAP_ITER> range;

    range = data_table_.equal_range(map_code);
    for (itr = range.first; itr != range.second; ++itr)
    {
        BattleGroundFlagInfo* info_map = itr->second;
        if (info_map->collection_id == collection_id)
        {
            return info_map;
        }
    }

    return NULL;
}
DWORD BattleGroundFlagInfoParser::GetStrongPointName(const MAPCODE map_code, const DWORD object_id)
{
    BATTLE_GROUND_FLAG_INFO_MAP_ITER itr;
    std::pair<BATTLE_GROUND_FLAG_INFO_MAP_ITER, BATTLE_GROUND_FLAG_INFO_MAP_ITER> range;

    range = data_table_.equal_range(map_code);
    for (itr = range.first; itr != range.second; ++itr)
    {
        BattleGroundFlagInfo* info_map = itr->second;
        if (info_map->strong_point_key == object_id)
        {
            return info_map->strong_point_name;
        }
    }

    return NULL;
}
#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL BattleGroundFlagInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    if (script_code != SCRIPT_BATTLE_GROUND_FLAG_INFO)
    {
        return FALSE;
    }

    int row_size = GetRowSize();

    for (int row = 0; row < row_size; ++row)
    {
        MAPCODE map_code = GetDataWORD("MapCode", row);
        DWORD collection_id = StrToWzID(GetDataString("CollectionID", row));
        DWORD collection_object = MAP_OBJECT_KEY + GetDataDWORD("StrongPointObject", row);
        DWORD name_code = GetDataDWORD("StrongPointName", row);
        DWORD flag_object = MAP_OBJECT_KEY + GetDataDWORD("FlagObject", row);

        BattleGroundFlagInfo* flag_info = GetBattleGroundFlagInfo(map_code, collection_id);

        if (flag_info == NULL)
        {
            flag_info = new BattleGroundFlagInfo;
            flag_info->collection_id = collection_id;
            flag_info->strong_point_key = collection_object;
            flag_info->strong_point_name = name_code;
            flag_info->vector_flag_key.push_back(flag_object);

            data_table_.insert(BATTLE_GROUND_FLAG_INFO_MAP::value_type(map_code, flag_info));
        }
        else
        {
            flag_info->vector_flag_key.push_back(flag_object);
        }
    }

    return TRUE;
}


#pragma warning ( pop )

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND