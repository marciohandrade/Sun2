//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "HeroVariationInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
HeroVariationInfoParser::HeroVariationInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
HeroVariationInfoParser::~HeroVariationInfoParser()
{
	assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroVariationInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroVariationInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_HeroVariationInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );

    memset (m_iVariationCount, 0, sizeof(m_iVariationCount));
    memset (total_variation_count_, 0, sizeof(total_variation_count_));
}


//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroVariationInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_HeroVariationInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	
    memset (m_iVariationCount, 0, sizeof(m_iVariationCount));
    memset (total_variation_count_, 0, sizeof(total_variation_count_));

}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroVariationInfoParser::Unload()
{
	BASE_HeroVariationInfo * pInfo = NULL;
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
BASE_HeroVariationInfo* HeroVariationInfoParser::GetHeroVariationInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL HeroVariationInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    assert(ScriptCode == SCRIPT_HERO_VARIATION_INFO);

    int rowSize = GetRowSize();

    for (int row = 0; row < rowSize; ++row)
    {
        BASE_HeroVariationInfo* info = new BASE_HeroVariationInfo;
        assert(info);
        info->partID.byClass = GetDataNumber("bClassID", row);
        info->partID.byPart = GetDataNumber("bBodyParts", row);
        info->partID.wPartID = GetDataNumber("ID", row);
        info->name_code = GetDataNumber("NameCode", row);
        info->desc_code = GetDataNumber("DescriptionCode", row);
        info->resource_code = GetDataNumber("ResourceType", row);
        
        if ((info->partID.byClass >= eCHAR_TYPE_MAX) || 
            (info->partID.byPart >= PLAYER_VARIATION_MAX + 1))
        {
            continue;
        }

        // 0:기본외모
        if (info->resource_code == 0)
        {
            // 기본 갯수를 늘려준다.
            ++m_iVariationCount[info->partID.byClass][info->partID.byPart];
            GetList(info->partID.byClass, info->partID.byPart, false).push_back(info->partID.dwID);
        }// 1:캐쉬리스트
        else if (info->resource_code == 1)
        {
            GetList(info->partID.byClass, info->partID.byPart, true).push_back(info->partID.dwID);
        }

        for (int i = 0; i < MAX_VARIATION_RESOURCE; ++i)
        {
            char colName[16];
            sprintf(colName, "dResourceID%d", i + 1);

            info->m_dResourceID[i] = GetDataNumber(colName, row);
        }

        assert(NULL == m_pDataTable->GetData(info->partID.dwID));
        m_pDataTable->Add(info, info->partID.dwID);
    }

    return TRUE;
}


int  HeroVariationInfoParser::GetVariationCount(BYTE byClass,int iVariation)
{
	if (byClass >= eCHAR_TYPE_MAX)
	{
		return 0;
	}

	if (iVariation >= PLAYER_VARIATION_MAX + 1)
	{
		return 0;
	}

	return m_iVariationCount[byClass][iVariation];

}
#pragma warning ( pop )
