#pragma once

#include <Singleton.h>
#include "Struct.h"

struct sOptionItemInfo
{
    DWORD m_OptionID;
    DWORD m_PageIndex;
    DWORD m_ItemType;
    std::string m_DisplayName;
    std::string m_CategoryName;
    std::string m_Comment;
    std::vector<std::string> m_ItemDataArray;
    DWORD m_GaugeRange;
};

class OptionItemInfoParser	: public Singleton<OptionItemInfoParser>, public IParser
{
public:
    OptionItemInfoParser();
    ~OptionItemInfoParser();

    std::map<DWORD, sOptionItemInfo*> m_OptionItemMap;


public:
    virtual void Release();
    virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);


    sOptionItemInfo* GetOptionItemInfo( DWORD OptionID );

    void GetOptionItemInfoPage( DWORD PageIndex, OUT std::vector<sOptionItemInfo*>& Array );
};


