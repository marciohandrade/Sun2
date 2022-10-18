//------------------------------------------------------------------------------				    
//  작업자: 이도형
//	2009/11/02
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "InDunOptionParser.h"
#include <SolarFileReader.h>

//----------------------------------------------------------------------------                   	
/**                    
*/
InDunOptionParser::InDunOptionParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
InDunOptionParser::~InDunOptionParser()
{
   assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID InDunOptionParser::Release()
{
   Unload();
   delete m_pDataTable;
   m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID InDunOptionParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<INDUN_OPTION*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID InDunOptionParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<INDUN_OPTION*>;
	m_pDataTable->Initialize( dwPoolSize );
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID InDunOptionParser::Unload()
{
	INDUN_OPTION * pInfo = NULL;
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
INDUN_OPTION* InDunOptionParser::GetInDunOption(DWORD dwIndex)
{
	return m_pDataTable->GetData( dwIndex );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL InDunOptionParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_INDUN_OPTION:
		return _Load( bReload );
	}

	return FALSE;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL InDunOptionParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		// 스크립트에서 읽어온 정보
		INDUN_OPTION * pInfo = new INDUN_OPTION;

		pInfo->mdwIndex = GetDataDWORD("T_Num", nRow);
		_snprintf(pInfo->msDesc, sizeof(pInfo->msDesc),GetDataString("T_Desc", nRow));
		pInfo->mwMapCode = GetDataDWORD("FieldCode", nRow);
		pInfo->mdwLayer = GetDataDWORD("T_Floors", nRow);

	//	assert( NULL == m_pDataTable->GetData( pInfo->mwMapCode ) );
		m_pDataTable->Add( pInfo, pInfo->mwMapCode );

	}

	return TRUE;
}

#pragma warning ( pop )

