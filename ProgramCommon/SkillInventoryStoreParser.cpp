////////////////////////////////////////////////////////////////////////////////////////////////////
// SkillInventoryStoreParser.cpp
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SkillInventoryStoreParser.h"


BOOL SkillInventoryStoreParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    if (script_code == SCRIPT_SKILL_INVEN_STORE)
    {
        return _Load(is_reload);
    }
    return FALSE; 
}

BOOL SkillInventoryStoreParser::_Load(const BOOL is_reload)
{
    if (is_reload)
    {
        skill_invens_.clear();
    }
    
    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        SkillInven_s new_info;
        new_info.index = row;
        new_info.bClassID = GetDataBYTE("bClassID", row);
        new_info.bSkillTab1 = GetDataBYTE("bSkillTab", row);
        new_info.bSkillTab2 = GetDataBYTE("bSkillInTab", row);
        new_info.wSkillNum = GetDataWORD("wSkillNum", row);
        
        if (new_info.wSkillNum > MAX_SKILL_INVENTORY_STORE_NUM)
        {
            new_info.wSkillNum = MAX_SKILL_INVENTORY_STORE_NUM;
        }
        
        for (int i = 0; i < MAX_SKILL_INVENTORY_STORE_NUM; ++i)
        {
            TCHAR skill_id[32] = { 0 };
            _sntprintf(skill_id, _countof(skill_id), _T("wSkillID%d"), i + 1);
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
            new_info.iSkillID[i] = GetDataInt(skill_id, row);
#else
            new_info.dwSkillID[i] = GetDataDWORD(skill_id, row);
#endif
        }

        skill_invens_[new_info.index] = new_info;
    }
    return TRUE;
}