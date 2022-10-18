#include "SunClientPrecompiledHeader.h"
#include "AmbientSoundInfoParser.h"
#include <SolarFileReader.h>

//----------------------------------------------------------------------------
/**
*/
AmbientSoundInfoParser::AmbientSoundInfoParser() : m_pDataTable ( NULL )
{
}


//----------------------------------------------------------------------------
/**
*/
AmbientSoundInfoParser::~AmbientSoundInfoParser()
{
   assert( m_pDataTable == NULL);
}


//----------------------------------------------------------------------------
/**
*/
VOID 
AmbientSoundInfoParser::Release()
{
   Unload();
   delete m_pDataTable;
   m_pDataTable = NULL;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
AmbientSoundInfoParser::Init(DWORD dwPoolSize)
{
   assert( m_pDataTable == NULL );
   m_pDataTable = new util::SolarHashTable<AmbientSoundInfo*>;
   m_pDataTable->Initialize( dwPoolSize );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
AmbientSoundInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
AmbientSoundInfoParser::Unload()
{
	AmbientSoundInfo * pInfo = NULL;

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
DWORD AmbientSoundInfoParser::GetAmbientSoundCode(DWORD dwWorldCode)
{
    AmbientSoundInfo* pAmbientInfo = m_pDataTable->GetData( dwWorldCode );
    if (NULL == pAmbientInfo)
    {
        return 0;
    }
    
    return pAmbientInfo->m_wAmbientCode;
}

AmbientSoundInfo* AmbientSoundInfoParser::GetAmbientSoundInfo(DWORD dwWorldCode)
{
    return m_pDataTable->GetData( dwWorldCode );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

//----------------------------------------------------------------------------
/**
*/
BOOL AmbientSoundInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_AMBIENT_SOUND_INFO:
		return _Load( bReload );
	}

	return FALSE;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL AmbientSoundInfoParser::_Load( BOOL bReload )
{
	int nRowSize = GetRowSize();
    DWORD dwKey = 0;
    
	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		AmbientSoundInfo * pInfo = new AmbientSoundInfo;

		pInfo->m_wNum = GetDataWORD("Num", nRow);
        strncpy( pInfo->m_szDesc, GetDataString("Desc",nRow), AmbientSoundInfo::MAX_DESC_LEN );
		pInfo->m_wWorldCode = GetDataWORD("WorldCode", nRow);
		pInfo->m_wAmbientCode = GetDataWORD("AmbientCode", nRow);
        
        dwKey = pInfo->m_wWorldCode;
		assert( NULL == m_pDataTable->GetData( dwKey ));
		m_pDataTable->Add( pInfo, dwKey );
	}

	return TRUE;
}

#pragma warning ( pop )
