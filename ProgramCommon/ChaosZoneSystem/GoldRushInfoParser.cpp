#include <stdafx.h>

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "ChaosZoneSystem/GoldRushInfoParser.h"

void GoldRushInfoParser::Init( char* pszPackFileName )
{
    SetPackFileName(pszPackFileName);
}

void GoldRushInfoParser::Release()
{
    Unload();
}

BOOL GoldRushInfoParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
    switch (scriptCode)
    {
    case SCRIPT_GOLDRUSH_INFO:
        return _Load(bReload);
    default:
        break;
    }

    return FALSE;
}

const GoldRushInfo* GoldRushInfoParser::FindData( const MAPCODE map_code ) const
{
    GoldRushInfo_Hash::const_iterator begin_iterator = goldrushinfo_hash_.begin(),
        end_iterator = goldrushinfo_hash_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const GoldRushInfo* find_info = begin_iterator->second;
        if (find_info->map_code == map_code)
        {
            return find_info;
        }
    }
    return NULL;
}

BOOL GoldRushInfoParser::_Load( BOOL is_reload )
{
    if (is_reload == TRUE) {
        goldrushinfo_hash().clear();
    }

    int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        GoldRushInfo* info = new GoldRushInfo();

        info->index =           GetDataBYTE("Index", row);
        info->map_code =        static_cast<MAPCODE>(GetDataDWORD("MapCode", row));
        
        info->play_time =       GetDataDWORD("Playtime", row);
        info->extra_time =      GetDataDWORD("Extratime", row);

        info->start_point =     GetDataWORD("StartPoint", row);
        info->base_kill_point = GetDataWORD("BaseKillPoint", row);
        info->steal_point_ratio = GetDataBYTE("StealPoint", row);
        info->lose_point_ratio =  GetDataBYTE("LosePoint", row);
        info->last_boss_code = static_cast<MONSTERCODE>(GetDataWORD("LastBossCode", row));

        //MonsterGradePoint
        TCHAR szTemp[MAX_FIELD_NAME_SIZE];
        for(int r=0; r < GoldRushInfo::eMONSTERGRADEPOINT_SIZE; ++r )
        {
            _snprintf(szTemp, _countof(szTemp), "MonsterGradePoint%d\0", r+1);
            info->monster_grade_point[r] = GetDataWORD(szTemp, row);
        }
        //GoldLevel
        for(int ro=0; ro < GoldRushInfo::eGOLDLEVEL_SIZE; ++ro )
        {
            _snprintf(szTemp, _countof(szTemp), "GoldLevel%d\0", ro+1);
            info->gold_grade[ro] = GetDataWORD(szTemp, row);
        }

        GoldRushInfo_Hash::_Pairib result = goldrushinfo_hash().insert(
            GoldRushInfo_Hash::value_type(info->index, info));
        ASSERT(result.second == true);
    }
    return TRUE;
}

void GoldRushInfoParser::Unload()
{
    GoldRushInfo_Hash::const_iterator it, end( goldrushinfo_hash_.end() );
    for(it = goldrushinfo_hash_.begin() ; it != end ; ++it)
    {
        GoldRushInfo* info = (it->second);
        SAFE_DELETE(info);
    }

    goldrushinfo_hash_.clear();
}
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH