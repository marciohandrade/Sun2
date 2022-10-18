#include <stdafx.h>

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "ChaosZoneSystem/GoldRushMonsterPointParser.h"

void GoldRushMonsterPointParser::Init( char* pszPackFileName )
{
    SetPackFileName(pszPackFileName);
}

void GoldRushMonsterPointParser::Release()
{
    Unload();
}

BOOL GoldRushMonsterPointParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
    switch (scriptCode)
    {
    case SCRIPT_GOLDRUSH_MONSTERPOINT:
        return _Load(bReload);
    default:
        break;
    }

    return FALSE;
}

const GoldRushMonsterPoint* GoldRushMonsterPointParser::FindData( const MONSTERCODE monster_code) const
{
    GoldrushMonsterPoint_Hash::const_iterator itr = monsterpoint_hash_.find(monster_code);
    if (itr != monsterpoint_hash_.end())
        return itr->second;

    return NULL;
}

BOOL GoldRushMonsterPointParser::_Load( BOOL is_reload )
{
    if (is_reload == TRUE) {
        monsterpoint_hash().clear();
    }

    int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        GoldRushMonsterPoint* info = new GoldRushMonsterPoint();

        info->monster_code =      static_cast<MONSTERCODE>(GetDataWORD("MonsterCode", row));
        info->monster_goldpoint = GetDataWORD("MonsterPoint", row);

        GoldrushMonsterPoint_Hash::_Pairib result = monsterpoint_hash().insert(
            GoldrushMonsterPoint_Hash::value_type(info->monster_code, info));
        ASSERT(result.second == true);
    }
    return TRUE;
}

void GoldRushMonsterPointParser::Unload()
{
    GoldrushMonsterPoint_Hash::const_iterator it, end( monsterpoint_hash_.end() );
    for(it = monsterpoint_hash_.begin() ; it != end ; ++it)
    {
        GoldRushMonsterPoint* info = (it->second);
        SAFE_DELETE(info);
    }

    monsterpoint_hash_.clear();
}
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
