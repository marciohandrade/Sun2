#include "StdAfx.h"
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
	SAFE_DELETE( m_pDataTable );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID MatchInfoParser::Init( DWORD dwPoolSize )
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_MatchInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
void MatchInfoParser::Unload()
{
    if (m_pDataTable == NULL) {
        return;
    }
    m_pDataTable->SetFirst();
    while (BASE_MatchInfo* match_info = m_pDataTable->GetNext()) {
        SAFE_DELETE(match_info);
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

//#pragma warning ( push )
//#pragma warning ( disable : 4244)

VOID MatchInfoParser::Reload()
{
//	Load( m_pszFileName, TRUE );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL MatchInfoParser::_Load(BOOL reloaded)
{
    __UNUSED(reloaded);
	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		BASE_MatchInfo * pInfo = new BASE_MatchInfo;
		pInfo->m_dwID					= GetDataDWORD( "dwIndex", nRow );

		pInfo->m_dwMatchLimitTime		= GetDataDWORD( "dwMatchLimitTime", nRow );
		pInfo->m_byMatchLimitCount		= GetDataBYTE( "byMatchLimitCount", nRow );
		
		assert( NULL == m_pDataTable->GetData( pInfo->m_dwID ) );
		m_pDataTable->Add( pInfo, pInfo->m_dwID );
	}

	return TRUE;
}

BOOL	MatchInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_MATCHINFO:
		return _Load( bReload );
	};

	return FALSE;
}

//#pragma warning(pop) <- undeclared
