#ifndef _AutomaticInfoParser_h_
#define _AutomaticInfoParser_h_

#if _MSC_VER > 1000
#pragma once
#endif 

#include <Singleton.h>
#include <ScriptCode.IParser.h>

//------------------------------------------------------------------------------ 
struct BASE_AutomaticInfo 
{
    static const int kComboCount = 2;
    static const int kAutomaticinfoNameLength = 64;
    static const int kItemCount= 10;

    eAUTO_HUNTING_TYPE code;
    char name[kAutomaticinfoNameLength];

    int combo_index[kComboCount];

    SLOTCODE item_code[kItemCount];
   
    BASE_AutomaticInfo()
    {
        code = eAUTO_TYPE_INVALID;
        ZeroMemory(name, sizeof(name));
        ZeroMemory(combo_index, sizeof(combo_index));
        ZeroMemory(item_code, sizeof(item_code));
    }
};

//------------------------------------------------------------------------------ 
class AutomaticInfoParser : public util::Singleton<AutomaticInfoParser>, public IParser
{
    typedef std::map<eAUTO_HUNTING_TYPE, BASE_AutomaticInfo> AutomaticInfo_map;

public:
    AutomaticInfoParser(void);
	~AutomaticInfoParser(void);

	virtual void Release();
	virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    BASE_AutomaticInfo* GetAutomaticInfo(eAUTO_HUNTING_TYPE code);
    bool CheckItemCodeInAutomaticInfo(eAUTO_HUNTING_TYPE hunting_type, SLOTCODE item_code);

    AutomaticInfo_map& automaticinfo_map() { return automaticInfo_map_; }

private:
    AutomaticInfo_map automaticInfo_map_;
};

#endif //_AutomaticInfoParser_h_

