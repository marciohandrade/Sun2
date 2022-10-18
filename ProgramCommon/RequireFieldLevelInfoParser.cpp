//------------------------------------------------------------------------------				    
//  RequireFieldLevelInfoParser                 	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------

#include "StdAfx.h"
#include "RequireFieldLevelInfoParser.h"
#include <Struct.h>
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
RequireFieldLevelInfoParser::RequireFieldLevelInfoParser()//:	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
RequireFieldLevelInfoParser::~RequireFieldLevelInfoParser()
{
   //FASSERT( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID RequireFieldLevelInfoParser::Release()
{
   //Unload();
   //delete m_pDataTable;
   //m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID 
RequireFieldLevelInfoParser::Init(DWORD dwPoolSize)
{
    __UNUSED(dwPoolSize);
   //FASSERT( m_pDataTable == NULL );
   //m_pDataTable = new util::SolarHashTable<BASE_RequireFieldLevelInfo*>;
   //m_pDataTable->Initialize( dwPoolSize );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
RequireFieldLevelInfoParser::Init(DWORD dwPoolSize, char * pszPackFileName)
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}


//----------------------------------------------------------------------------                   	
/**                    
*/
VOID 
RequireFieldLevelInfoParser::Unload()
{
	//BASE_RequireFieldLevelInfo * pInfo = NULL;
	//if( m_pDataTable == NULL )
	//	{
	//	return;
	//	}
	//m_pDataTable->SetFirst();
	//while( pInfo = m_pDataTable->GetNext() )
	//	{
	//			delete pInfo;
	//	}
 //	m_pDataTable->RemoveAll();
}


//----------------------------------------------------------------------------
/**
	MCode | Diff |
	16bit | 16bit |
*/
DWORD			
RequireFieldLevelInfoParser::MakeKey( WORD wMcode, BYTE byDiff )
{
	return DWORD( (DWORD(wMcode) << 16) | DWORD(byDiff) );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BASE_RequireFieldLevelInfo * 
RequireFieldLevelInfoParser::GetRequireFieldLevelInfo( WORD wMcode, BYTE byDiff, BOOL bGuildPVP )
{
	DWORD dwKey = this->MakeKey( wMcode, byDiff);
	BASE_RequireFieldLevelInfoPtr ptr;

	if( bGuildPVP == TRUE )
	{
		std::pair<RequireFieldLevelInfoItrType, RequireFieldLevelInfoItrType> pair = m_LevelInfoMap.equal_range( dwKey );

		if( pair.first == m_LevelInfoMap.end() )
			return NULL;
		
		for( RequireFieldLevelInfoItrType itr = pair.first; itr != pair.second; ++itr )
		{
			ptr = itr->second;

			if( ptr->m_dwIndex >= MIN_GUILD_PVP_INDEX && ptr->m_dwIndex < MAX_GUILD_PVP_INDEX )
			{
				return (BASE_RequireFieldLevelInfo * )ptr.get();
			}
		}

		return NULL;
	}
	else
	{
		RequireFieldLevelInfoItrType itr = m_LevelInfoMap.find( dwKey );

		if( itr == m_LevelInfoMap.end() )
			return NULL;

		ptr = itr->second;

		return (BASE_RequireFieldLevelInfo * )ptr.get();
	}

	//DWORD dwKey = this->MakeKey( wMcode, byDiff);
	//return m_pDataTable->GetData( dwKey );
}
VOID RequireFieldLevelInfoParser::Reload()
{
//	Load( m_pszFileName, TRUE );
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/

BOOL RequireFieldLevelInfoParser::_Load(BOOL bReload)
{
    __UNUSED(bReload);
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		CODETYPE Index		= GetDataDWORD( "bIndex", nRow);
		WORD wMcode			= GetDataWORD( "wMcode", nRow);
		BYTE byDiff			= GetDataBYTE( "bDiff", nRow);
		WORD wMinlv			= GetDataWORD( "wMinlv", nRow);
		WORD wMaxlv			= GetDataWORD( "wMaxlv", nRow);
		DWORD dwKey			= MakeKey( wMcode, byDiff);

		BASE_RequireFieldLevelInfoPtr pInfo( new BASE_RequireFieldLevelInfo);
		pInfo->m_wMcode		= wMcode;
		pInfo->m_byDiff		= byDiff;
		pInfo->m_dwIndex	= Index;
		pInfo->m_wMinlv		= wMinlv;
		pInfo->m_wMaxlv		= wMaxlv;

		m_LevelInfoMap.insert( RequireFieldLevelInfoType::value_type( dwKey, pInfo));
	}

	return TRUE;
}

//#pragma warning ( pop )

BOOL	RequireFieldLevelInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch( ScriptCode )
	{
	case SCRIPT_REQUIREFIELDLEVELINFO:
		return _Load( bReload);
	}

	return FALSE;
}

