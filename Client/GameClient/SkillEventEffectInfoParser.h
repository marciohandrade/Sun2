#pragma once

#include "SkillEventEffectInfo.h"


class cSkillEventEffectInfoParser	: public Singleton<cSkillEventEffectInfoParser>, public IParser
{
public:
    cSkillEventEffectInfoParser();
    ~cSkillEventEffectInfoParser();


    VOID                    Init(DWORD dwPoolSize);
    VOID			        Init(DWORD dwPoolSize, char * pszPackFileName);
    VOID                    Release();
    virtual	BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    cSkillEventEffectInfo*  GetInfo(DWORD SkillClassCode);

private:
    VOID		            Unload();
    util::SolarHashTable<cSkillEventEffectInfo*>*    m_pDataTable;
};

