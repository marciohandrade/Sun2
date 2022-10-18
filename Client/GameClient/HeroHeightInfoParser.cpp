//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "HeroHeightInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
HeroHeightInfoParser::HeroHeightInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
HeroHeightInfoParser::~HeroHeightInfoParser()
{
	assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroHeightInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroHeightInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_HeroHeightInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroHeightInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_HeroHeightInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID HeroHeightInfoParser::Unload()
{
	BASE_HeroHeightInfo * pInfo = NULL;
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
BASE_HeroHeightInfo* HeroHeightInfoParser::GetHeroHeightInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL HeroHeightInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
    assert(ScriptCode == SCRIPT_HERO_HEIGHT_INFO);

    int rowSize = GetRowSize();

    for (int row = 0; row < rowSize; ++row)
    {
        BASE_HeroHeightInfo* info = new BASE_HeroHeightInfo;
        assert(info);

        info->m_bClass = GetDataNumber("bClass", row);
        info->m_fBase = GetDataNumber("fBase", row);

        for (int i = 0; i < MAX_HEIGHT_VARIATION; i++)
        {
            char colName[24];
            sprintf(colName, "fHeight_%d", i + 1);

            info->m_fHeight[i] = GetDataNumber(colName, row);
        }

        assert(!m_pDataTable->GetData(info->m_bClass));
        m_pDataTable->Add(info, info->m_bClass);
    }

	return TRUE;
}


#pragma warning ( pop )
