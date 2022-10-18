#include "SunClientPrecompiledHeader.h"
#include "AutomaticInfoParser.h"
#include <SolarFileReader.h>

//------------------------------------------------------------------------------ 
AutomaticInfoParser::AutomaticInfoParser(void)
{
    automaticinfo_map().clear();
}
//------------------------------------------------------------------------------ 
AutomaticInfoParser::~AutomaticInfoParser(void)
{
}
//------------------------------------------------------------------------------ 
void AutomaticInfoParser::Release()
{
    automaticinfo_map().clear();
}
//------------------------------------------------------------------------------ 
BOOL AutomaticInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    if (bReload == TRUE)
    {
        automaticinfo_map().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        BASE_AutomaticInfo info;

        info.code = static_cast<eAUTO_HUNTING_TYPE>(GetDataInt("Code", nRow));
        strncpy(info.name, GetDataString("Name",nRow), BASE_AutomaticInfo::kAutomaticinfoNameLength);
        info.combo_index[0] = GetDataInt("ComboIndex1", nRow);
        info.combo_index[1] = GetDataInt("ComboIndex2", nRow);

        info.item_code[0] = GetDataInt("UseItemCode1", nRow);
        info.item_code[1] = GetDataInt("UseItemCode2", nRow);
        info.item_code[2] = GetDataInt("UseItemCode3", nRow);
        info.item_code[3] = GetDataInt("UseItemCode4", nRow);
        info.item_code[4] = GetDataInt("UseItemCode5", nRow);
        info.item_code[5] = GetDataInt("UseItemCode6", nRow);
        info.item_code[6] = GetDataInt("UseItemCode7", nRow);
        info.item_code[7] = GetDataInt("UseItemCode8", nRow);
        info.item_code[8] = GetDataInt("UseItemCode9", nRow);
        info.item_code[9] = GetDataInt("UseItemCode10", nRow);

        automaticinfo_map().insert(std::make_pair(info.code, info));
    }

    return TRUE;
}
//------------------------------------------------------------------------------ 
BASE_AutomaticInfo* AutomaticInfoParser::GetAutomaticInfo(eAUTO_HUNTING_TYPE code)
{
    AutomaticInfo_map::iterator iter = automaticinfo_map().find(code);
    if (iter != automaticinfo_map().end())
    {
        return &(iter->second);
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
bool AutomaticInfoParser::CheckItemCodeInAutomaticInfo(eAUTO_HUNTING_TYPE hunting_type, SLOTCODE item_code)
{
    if (item_code <= 0)
    {
        return false;
    }

    BASE_AutomaticInfo* info = AutomaticInfoParser::Instance()->GetAutomaticInfo(hunting_type);
    if (info == NULL)
    {
        return false;
    }

    for (int i = 0; i < BASE_AutomaticInfo::kItemCount; ++i)
    {
        if (info->item_code[i] == item_code)
        {
            return true;
        }
    }
    return false;
}