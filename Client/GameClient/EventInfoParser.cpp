//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "EventInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
EventInfoParser::EventInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
EventInfoParser::~EventInfoParser()
{
	assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID EventInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID EventInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_EventInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );

	InitPlayedEvent();
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID EventInfoParser::Init(DWORD dwPoolSize )
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_EventInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	InitPlayedEvent();
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID EventInfoParser::Unload()
{
	BASE_EventInfo * pInfo = NULL;
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
BASE_EventInfo* EventInfoParser::GetEventInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

void _EVENTINFO::Load(util::SolarFileReader &sr)
{
	sr.GetNextTokenType(TOKEN_NUMBER); wText = sr.GetTokenNumber();
	sr.GetNextTokenType(TOKEN_NUMBER); wSound = sr.GetTokenNumber();
	sr.GetNextTokenType(TOKEN_NUMBER); wMovie = sr.GetTokenNumber();
	
}

BOOL EventInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();
	
	const int cMaxStringLen = 255;

	TCHAR 	szString[cMaxStringLen];

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
		BASE_EventInfo* pInfo = new BASE_EventInfo;

		//dCode
		pInfo->m_dwCode		= GetDataDWORD("dCode",nRow);
		
		for (int a = 0; a < MAX_EVENT_INFO; ++a)
		{
			//wText
			Snprintf(szString, cMaxStringLen-1, _T("wText%d"), a+1);
			pInfo->m_EventInfo[a].wText  =	GetDataWORD(szString,nRow);

			//wSound
			Snprintf(szString, cMaxStringLen-1, _T("wSound%d"), a+1);
			pInfo->m_EventInfo[a].wSound =	GetDataWORD(szString,nRow);

			//wMovie
			Snprintf(szString, cMaxStringLen-1, _T("wMovie%d"), a+1);
			pInfo->m_EventInfo[a].wMovie =	GetDataWORD(szString,nRow);
		}
		
		assert(NULL == m_pDataTable->GetData(pInfo->m_dwCode));
 		m_pDataTable->Add(pInfo, pInfo->m_dwCode);
	}
	
	return TRUE;
}


void EventInfoParser::InitPlayedEvent()
{
	m_vecPlayedList.clear();	
}

BOOL EventInfoParser::IsPlayed(DWORD dwEvent)
{
	
	vector<DWORD>::iterator iter = m_vecPlayedList.begin();

	while (iter != m_vecPlayedList.end())
	{
		if (*iter == dwEvent)
		{
			return TRUE;
		}

		iter++;
	}

	return FALSE;

}

void EventInfoParser::AddPlayedList(DWORD dwEvent)
{
	m_vecPlayedList.push_back(dwEvent);

}

#pragma warning ( pop )
