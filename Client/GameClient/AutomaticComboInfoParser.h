#ifndef _AutomaticComboInfoParser_h_
#define _AutomaticComboInfoParser_h_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <Singleton.h>
#include <ScriptCode.IParser.h>

//------------------------------------------------------------------------------ 
struct BASE_AutomaticComboInfo 
{
    enum AutomaticComboType
    {
        kOptionNone = 0, 
        kOptionInt,
        kOptionPercent,
        kOptionMax,
    };

    int                 combo_index;
    AutomaticComboType  option_type;
    int                 option_value;     //!< °ª
    int                 stringtable_code;
    SLOTCODE            item_code;

    BASE_AutomaticComboInfo()
    {
        combo_index = 0;
        option_type = kOptionNone;
        option_value = 0;
        stringtable_code = 0;
        item_code = 0;
    }
};

//------------------------------------------------------------------------------ 
class AutomaticComboInfoParser : public util::Singleton<AutomaticComboInfoParser>, public IParser
{
    typedef std::multimap<int, BASE_AutomaticComboInfo> AutomaticComboInfo_multimap;

public:
    AutomaticComboInfoParser(void);
	~AutomaticComboInfoParser(void);

	virtual void Release();
	virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    int  GetAutomaticComboInfo(OUT std::vector<BASE_AutomaticComboInfo*>& value_vector, int combo_index);

    AutomaticComboInfo_multimap& automaticcomboinfo_multimap() { return automaticcomboInfo_multimap_; }

private:
    AutomaticComboInfo_multimap automaticcomboInfo_multimap_;
};

#endif //_AutomaticComboInfoParser_h_

