#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include ".\BattleGroundInfoParser.h"

//------------------------------------------------------------------------------ 
BattleGroundInfoParser::BattleGroundInfoParser(void)
{
}
//------------------------------------------------------------------------------ 
BattleGroundInfoParser::~BattleGroundInfoParser(void)
{
}
VOID BattleGroundInfoParser::Init()
{
}

VOID BattleGroundInfoParser::Init(char * pszPackFileName)
{
    SetPackFileName(pszPackFileName);
}
//------------------------------------------------------------------------------ 
void BattleGroundInfoParser::Release()
{
    Unload();
}

void BattleGroundInfoParser::Unload()
{
    BattleGroundInfo_Hash::const_iterator it, end( battlegroundinfo_hash_.end() );
    for(it = battlegroundinfo_hash_.begin() ; it != end ; ++it)
    {
        BattleGroundInfo* info = (it->second);
        SAFE_DELETE(info);
    }

    battlegroundinfo_hash_.clear();	
}

BOOL BattleGroundInfoParser::_Load(BOOL bReload)
{
    if (bReload == TRUE)
    {
        battlegroundinfo_hash().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        BattleGroundInfo* info = new BattleGroundInfo();

        info->index = GetDataBYTE("Index", nRow);
        info->map_code = GetDataDWORD("MapCode", nRow);
        info->limit_level = GetDataDWORD("LimitLv", nRow);
        info->equipment_rating = GetDataDWORD("EquipmentRating", nRow);
        info->team_max = GetDataBYTE("TeamMAX", nRow);
        info->max_party = GetDataBYTE("MAXParty", nRow);
        info->max_match_time = GetDataDWORD("MaxMatchTime", nRow);
        info->start_time = GetDataDWORD("StartTime", nRow);
        info->entry_time = GetDataDWORD("EntryTime", nRow);
        info->prepare_condition_code = GetDataDWORD("PrepareConCode", nRow);
        info->prepare_time = GetDataDWORD("PrepareTime", nRow);
        info->gate_trigger_id = GetDataDWORD("GateTriggerID", nRow);
        info->goal_score = GetDataDWORD("GoalScore", nRow);
        info->sealing_time = GetDataDWORD("SealingTime", nRow);
        info->conquer_wait_time = GetDataDWORD("ConquerWaitTime", nRow);
        info->gather_point = GetDataDWORD("GatherPoint", nRow);
        info->gather_period = GetDataDWORD("GatherPeriod", nRow);
#ifdef _NA_006599_20130401_BATTLEGROUND_PKPOINT
        info->pk_point = GetDataBYTE("PkPoint", nRow);
#endif
        info->dead_time = GetDataDWORD("DeadTime", nRow);
        info->resurrection_period = GetDataDWORD("ResPeriod", nRow);
        info->resurrection_condition_code = GetDataDWORD("ResConCode", nRow);
        info->iron_mode = GetDataDWORD("IronMode", nRow);
        info->abusing_point = GetDataDWORD("AbusingPoint", nRow);
        info->win_item_code = GetDataDWORD("WinItemCode", nRow);
        info->base_item_code = GetDataDWORD("BaseItemCode", nRow);
        info->penalty_item_code = GetDataDWORD("PenaltyItemCode", nRow);

        TCHAR* item_limit_text[10] = {"LimitIT01", "LimitIT02", "LimitIT03", "LimitIT04",
        "LimitIT05", "LimitIT06", "LimitIT07", "LimitIT08", "LimitIT09", "LimitIT10"};

        for (int i = 0; i < BattleGroundInfo::MAX_ITEM_LIMIT_COUNT; ++i)
        {
            info->limit_item[i] = GetDataDWORD(item_limit_text[i], nRow);
        }

        info->draw_time = GetDataDWORD("DrawTime", nRow);
        info->re_enter_condiion = GetDataDWORD("ReEnterCon", nRow);
        info->re_enter_condition_period = GetDataDWORD("ReEnterConPeriod", nRow);
        info->exit_condition = GetDataDWORD("BGESCCon", nRow);
        info->exit_period = GetDataDWORD("BGESCPeriod", nRow);
        
        BattleGroundInfo_Hash::_Pairib result = battlegroundinfo_hash().insert(
            BattleGroundInfo_Hash::value_type(static_cast<DWORD>(info->map_code), info));
        ASSERT(result.second == true);
    }
    return TRUE;
}
//------------------------------------------------------------------------------ 
BOOL BattleGroundInfoParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
    switch (scriptCode)
    {
    case SCRIPT_BATTLE_GROUND_INFO:
        return _Load(bReload);
    }

    return FALSE;
}

const BattleGroundInfo* BattleGroundInfoParser::FindData(const MAPCODE map_code) const
{
    BattleGroundInfo_Hash::const_iterator begin_iterator = battlegroundinfo_hash_.begin(),
        end_iterator = battlegroundinfo_hash_.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const BattleGroundInfo* find_info = begin_iterator->second;
        if (find_info->map_code == map_code)
        {
            return find_info;
        }
    }
    return NULL;
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND