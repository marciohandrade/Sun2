#include "SunClientPrecompiledHeader.h"
#include "AutomaticComboInfoParser.h"
#include <SolarFileReader.h>

//------------------------------------------------------------------------------ 
AutomaticComboInfoParser::AutomaticComboInfoParser(void)
{
    automaticcomboinfo_multimap().clear();
}
//------------------------------------------------------------------------------ 
AutomaticComboInfoParser::~AutomaticComboInfoParser(void)
{
}
//------------------------------------------------------------------------------ 
void AutomaticComboInfoParser::Release()
{
    automaticcomboinfo_multimap().clear();
}
//------------------------------------------------------------------------------ 
BOOL AutomaticComboInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    if (bReload == TRUE)
    {
        automaticcomboinfo_multimap().clear();
    }

    int nRowSize = GetRowSize();
    for (int nRow = 0; nRow < nRowSize; ++nRow)
    {
        BASE_AutomaticComboInfo info;

        info.combo_index = GetDataInt("ComboIndex", nRow);
        info.option_type = static_cast<BASE_AutomaticComboInfo::AutomaticComboType>(GetDataInt("OptionType", nRow));
        info.option_value = GetDataInt("OptionValue", nRow);
        info.stringtable_code = GetDataInt("String_Code", nRow);
        info.item_code = GetDataInt("ItemCode", nRow);
        
        automaticcomboinfo_multimap().insert(std::make_pair(info.combo_index, info));
    }

    return TRUE;
}
//------------------------------------------------------------------------------ 
int AutomaticComboInfoParser::GetAutomaticComboInfo(OUT std::vector<BASE_AutomaticComboInfo*>& value_vector, int combo_index)
{
    std::pair<AutomaticComboInfo_multimap::iterator, AutomaticComboInfo_multimap::iterator> range = automaticcomboinfo_multimap().equal_range(combo_index);

    AutomaticComboInfo_multimap::iterator begin = range.first;
    AutomaticComboInfo_multimap::iterator end = range.second;

    for (; begin != end; ++begin)
    {
        BASE_AutomaticComboInfo* info = &(begin->second);

        value_vector.push_back(info);
    }

    return static_cast<int>(value_vector.size());
}
