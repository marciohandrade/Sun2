//------------------------------------------------------------------------------				    
//  작업자: 이지연
//	2006/02/23
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "SkillTextInfoParser.h"
#include <SolarFileReader.h>

//----------------------------------------------------------------------------                   	
/**                    
*/
SkillTextInfoParser::SkillTextInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
SkillTextInfoParser::~SkillTextInfoParser()
{
   assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID SkillTextInfoParser::Release()
{
   Unload();
   delete m_pDataTable;
   m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID SkillTextInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_SkillTextInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}


//----------------------------------------------------------------------------                   	
/**                    
*/
VOID SkillTextInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_SkillTextInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID SkillTextInfoParser::Unload()
{
	BASE_SkillTextInfo * pInfo = NULL;
	if( m_pDataTable == NULL )
	{
		return;
	}
	m_pDataTable->SetFirst();
	while( pInfo = m_pDataTable->GetNext() )
	{
		delete pInfo;
	}
	m_pDataTable->RemoveAll();
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BASE_SkillTextInfo* SkillTextInfoParser::GetSkillTextInfo(DWORD dwDescIndex)
{
	return m_pDataTable->GetData( dwDescIndex );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

//----------------------------------------------------------------------------                      
/**                    
*/
BOOL SkillTextInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    assert(ScriptCode == SCRIPT_SKILL_TEXT_INFO);

    int rowSize = GetRowSize();

    for (int row = 0; row < rowSize; ++row)
    {
        BASE_SkillTextInfo* info = new BASE_SkillTextInfo;

        info->m_dw_DescIndex = GetDataNumber("Index", row);
        strncpy(info->m_sz_name, GetDataString("SkillName", row), 256);
        info->m_dw_DescCode = GetDataNumber("Scode", row);

        for (int i = 0; i < 8; ++i)
        {
            char codeName[24];
            char typeName[24];
            char paramName[32];

            sprintf(codeName, "AbilityIDCode%d", i + 1);
            sprintf(typeName, "Type%d", i + 1);
            sprintf(paramName, "AbilityParameterNo%d", i + 1);

            info->m_arrayParam[i].m_dw_AbilityCode = GetDataNumber(codeName, row);
            info->m_arrayParam[i].m_dw_ParamType = GetDataNumber(typeName, row);
            info->m_arrayParam[i].m_dw_ParamNum = GetDataNumber(paramName, row);
        }

        assert(!m_pDataTable->GetData(info->m_dw_DescIndex));
        m_pDataTable->Add(info, info->m_dw_DescIndex);
    }

    return TRUE;
}


#pragma warning ( pop )