#include "SunClientPrecompiledHeader.h"

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "GuildMissionTreeParser.h"

BOOL GuildMissionTreeParser::_Load( BOOL bReload )
{
    if (bReload == TRUE) {
        GetGuildMissionInfoHash().clear();
    }

    int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        GuildMissionInfo* mission_info = new GuildMissionInfo();


        mission_info->large_group           = GetDataBYTE("LGroup", row);
        mission_info->sub_group             = GetDataBYTE("SGroup", row);
        mission_info->large_group_string    = GetDataDWORD("LGroupStringCode", row);
        mission_info->sub_group_string      = GetDataDWORD("SGroupStringCode", row);
        mission_info->mission_string_code   = GetDataDWORD("MissionStringCode", row);
        mission_info->map_code              = GetDataWORD("MapCode", row);

        sscanf(GetDataString("EnableColor", row), "%x", &(mission_info->font_color[0]));
        sscanf(GetDataString("DisableColor", row), "%x", &(mission_info->font_color[1]));
        sscanf(GetDataString("OverColor", row), "%x", &(mission_info->font_color[2]));
        sscanf(GetDataString("SelectColor", row), "%x", &(mission_info->font_color[3]));

        GetGuildMissionInfoHash().insert(GuildMissionInfoHash::value_type(mission_info->map_code, mission_info));
    }

    return TRUE;
}

VOID GuildMissionTreeParser::Init( char* file_name )
{
    SetPackFileName(file_name);
}

VOID GuildMissionTreeParser::Release()
{
    _Unload();
}

BOOL GuildMissionTreeParser::LoadScript( eSCRIPT_CODE script_code, BOOL is_reload )
{
    switch(script_code)
    {
    case SCRIPT_GUILDMISSIONTREE:
        return _Load(is_reload);
    default:
        break;
    }
    return FALSE;
}

const GuildMissionInfo* GuildMissionTreeParser::FindData( const MAPCODE& map_code ) const
{
    GuildMissionInfoHash::const_iterator itr = info_hash_.find(map_code);

    if (itr != info_hash_.end()) {
        return itr->second;
    }

    return NULL;
}

VOID GuildMissionTreeParser::_Unload()
{
    GuildMissionInfoHash::iterator itr = info_hash_.begin();
    for (itr; itr != info_hash_.end(); ++itr)
    {
        GuildMissionInfo* info = itr->second;
        SAFE_DELETE(info);
    }
    info_hash_.clear();
}



// 그룹번호로 찾기
const GuildMissionInfo* GuildMissionTreeParser::FindData(const BYTE LGroup, const BYTE SGroup) const
{
    GuildMissionInfoHash::const_iterator itr = info_hash_.begin();

    for(itr; itr != info_hash_.end(); ++itr)
    {
        GuildMissionInfo* info = itr->second;
        if (info->large_group == LGroup && info->sub_group == SGroup)
        {
            return info;
        }
    }
    return NULL;
}

#endif //_NA_008334_20150608_SONNENSCHEIN
