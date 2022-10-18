//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "BGMSoundInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------
/**
*/
BGMSoundInfoParser::BGMSoundInfoParser() : m_pDataTable ( NULL )
{
}


//----------------------------------------------------------------------------
/**
*/
BGMSoundInfoParser::~BGMSoundInfoParser()
{
   assert( m_pDataTable == NULL);
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BGMSoundInfoParser::Release()
{
   Unload();
   delete m_pDataTable;
   m_pDataTable = NULL;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BGMSoundInfoParser::Init(DWORD dwPoolSize)
{
   assert( m_pDataTable == NULL );
   m_pDataTable = new util::SolarHashTable<BASE_BGMSoundInfo*>;
   m_pDataTable->Initialize( dwPoolSize );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BGMSoundInfoParser::Init( DWORD dwPoolSize, char * pszPackFileName )
{
	Init( dwPoolSize );
	SetPackFileName( pszPackFileName );
}


//----------------------------------------------------------------------------
/**
*/
VOID 
BGMSoundInfoParser::Unload()
{
	BASE_BGMSoundInfo * pInfo = NULL;

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
DWORD			
BGMSoundInfoParser::MakeKey(int iType, WORD FieldCode, eZONETYPE eRoomType)
{
	return (DWORD)( (DWORD(iType) << 24) | (DWORD(FieldCode) << 8) | (DWORD(eRoomType)) );
}


//----------------------------------------------------------------------------
/**
*/
BASE_BGMSoundInfo * 
BGMSoundInfoParser::GetBGMInfo(int iType, WORD FieldCode, eZONETYPE eRoomType)
{
	DWORD dwKey = MakeKey(iType, FieldCode, eRoomType);
	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

//----------------------------------------------------------------------------
/**
*/
BOOL BGMSoundInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch (ScriptCode)
	{
	case SCRIPT_BGM_SOUND_INFO:
		return _Load(bReload);
	}

	return FALSE;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL BGMSoundInfoParser::_Load(BOOL bReload)
{
	int nRowSize = GetRowSize();
	DWORD dwOffset = 0;
    DWORD dwKey = 0;
    
	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_BGMSoundInfo* pInfo = new BASE_BGMSoundInfo;

		pInfo->m_wNum = GetDataWORD("Num", nRow);
		strncpy(pInfo->m_szDesc, GetDataString("Desc",nRow), MAX_BGMDESC_LEN);
		pInfo->m_wType = GetDataWORD("Type", nRow);
		pInfo->m_wFieldCode = GetDataWORD("FieldCode", nRow);
		pInfo->m_RoomType = GetDataDWORD("MapType", nRow);
		pInfo->m_wBGMNum = GetDataWORD("BGMNum", nRow);
		dwKey = MakeKey(pInfo->m_wType, pInfo->m_wFieldCode, (eZONETYPE)pInfo->m_RoomType);
		assert(NULL == m_pDataTable->GetData(dwKey));
		m_pDataTable->Add(pInfo, dwKey);
	}

	return TRUE;
}


#pragma warning ( pop )
