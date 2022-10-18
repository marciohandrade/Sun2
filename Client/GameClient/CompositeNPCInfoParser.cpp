//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "CompositeNPCInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
CompositeNPCInfoParser::CompositeNPCInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
CompositeNPCInfoParser::~CompositeNPCInfoParser()
{
	assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID CompositeNPCInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID CompositeNPCInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_CompositeNPCInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID CompositeNPCInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_CompositeNPCInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID CompositeNPCInfoParser::Unload()
{
	BASE_CompositeNPCInfo * pInfo = NULL;
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
BASE_CompositeNPCInfo* CompositeNPCInfoParser::GetCompositeNPCInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL CompositeNPCInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();
		
	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_CompositeNPCInfo* pInfo = new BASE_CompositeNPCInfo;
	
		//NPCCode
		pInfo->m_dwNPCCode = GetDataDWORD("NPCCode", nRow);

		//Param1
		pInfo->m_dwParams[0] = GetDataDWORD("Param1", nRow);

		//Param2
		pInfo->m_dwParams[1] = GetDataDWORD("Param2", nRow);

		//Param3
		pInfo->m_dwParams[2] = GetDataDWORD("Param3", nRow);

		//Param4
		pInfo->m_dwParams[3] = GetDataDWORD("Param4", nRow);

		//Param5
		pInfo->m_dwParams[4] = GetDataDWORD("Param5", nRow);
	
		assert(NULL == m_pDataTable->GetData(pInfo->m_dwNPCCode));
		m_pDataTable->Add(pInfo, pInfo->m_dwNPCCode);
	}
	return TRUE;
}



#pragma warning ( pop )
