//------------------------------------------------------------------------------				    
//  작업자: 이지연
//	2006/02/23
//------------------------------------------------------------------------------

#pragma once

#include <SolarHashTable.h>
#include <Singleton.h>

#pragma pack(push,1)

struct BASE_SkillTextParam
{
     DWORD    m_dw_AbilityCode;
     DWORD    m_dw_ParamType;
     DWORD    m_dw_ParamNum;
};

struct BASE_SkillTextInfo 
{
     DWORD    m_dw_DescIndex;
     char     m_sz_name[256];
     DWORD    m_dw_DescCode;
     BASE_SkillTextParam  m_arrayParam[8];
};

#pragma pack(pop)

class SkillTextInfoParser	: public Singleton<SkillTextInfoParser> ,public IParser
{
public:
    SkillTextInfoParser();
    ~SkillTextInfoParser();


    VOID			        Init(DWORD dwPoolSize , char *szPackfileName);
    VOID			        Init(DWORD dwPoolSize );

    VOID			        Release();
    virtual	BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    BASE_SkillTextInfo*		GetSkillTextInfo(DWORD dwDescIndex);

private:
    VOID		            Unload();
    util::SolarHashTable<BASE_SkillTextInfo*>*      m_pDataTable;
};
