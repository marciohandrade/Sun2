#include "SunClientPrecompiledHeader.h"
#include "MatchInfoParser.h"
#include <Struct.h>
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
MatchInfoParser::MatchInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
MatchInfoParser::~MatchInfoParser()
{
	assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MatchInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MatchInfoParser::Init(DWORD dwPoolSize , TCHAR *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_MatchInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MatchInfoParser::Unload()
{
	BASE_MatchInfo * pInfo = NULL;
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
BASE_MatchInfo* MatchInfoParser::GetMatchInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL MatchInfoParser::Load( char * pszFileName )
{

	assert( m_pDataTable != NULL );
	Unload();


	util::SolarFileReader sr;
	if ( !OpenFile( sr , pszFileName, SEPERATOR_WHITESPACE ) )
	{

		assert(0);
		return (FALSE);
	}

	while( TRUE )
	{

		eTOKEN_TYPE ettType = sr.GetNextTokenType();
		if ( TOKEN_END == ettType )
		{

			break;
		}

		BASE_MatchInfo * pInfo = new BASE_MatchInfo;
		pInfo->m_dwID = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); pInfo->m_dwMatchLimitTime = sr.GetTokenNumber();
		sr.GetNextTokenType(TOKEN_NUMBER); pInfo->m_byMatchLimitCount = sr.GetTokenNumber();
		assert( NULL == m_pDataTable->GetData( pInfo->m_dwID ) );
		m_pDataTable->Add( pInfo, pInfo->m_dwID );

	}

	sr.CloseFile();

	return TRUE;
}



#pragma warning ( pop )
