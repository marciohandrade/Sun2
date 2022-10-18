//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "HeroVoiceInfoParser.h"
#include "NPCVoiceInfoParser.h"
#include <SolarFileReader.h>


//----------------------------------------------------------------------------                   	
/**                    
*/
NPCVoiceInfoParser::NPCVoiceInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
NPCVoiceInfoParser::~NPCVoiceInfoParser()
{
	assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID NPCVoiceInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID NPCVoiceInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_NPCVoiceInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID NPCVoiceInfoParser::Init(DWORD dwPoolSize)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<BASE_NPCVoiceInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID NPCVoiceInfoParser::Unload()
{
	BASE_NPCVoiceInfo * pInfo = NULL;
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
BASE_NPCVoiceInfo* NPCVoiceInfoParser::GetNPCVoiceInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)

BOOL NPCVoiceInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	int nRowSize = GetRowSize();
	
	const int cMaxStringLen = 255;

	TCHAR 	szString[cMaxStringLen];

	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{

		BASE_NPCVoiceInfo* pInfo = new BASE_NPCVoiceInfo;
		
		//w_NPCCode
		pInfo->m_w_NPCCode =	GetDataDWORD("w_NPCCode", nRow);

		//s_Desc
		strncpy(pInfo->m_s_Desc, GetDataString("s_Desc", nRow), 64);
		
		int a;

		for (a = 0; a < MAX_NPC_VOICE_SAMPLE; a++)
		{
			//d_Wait
			Snprintf(szString, cMaxStringLen-1, _T("d_Wait%d"), a+1);
			pInfo->m_Wait[a].dwVoiceID	= GetDataDWORD(szString,nRow);
			//b_WaitVoiceType
			Snprintf(szString, cMaxStringLen-1, _T("b_WaitVoiceType%d"), a+1);
			pInfo->m_Wait[a].byVoiceType = GetDataBYTE(szString,nRow);
			//d_WaitText
			Snprintf(szString, cMaxStringLen-1, _T("d_WaitText%d"), a+1);
			pInfo->m_Wait[a].dwTextID	= GetDataDWORD(szString,nRow);
			//b_WaitTextType
			Snprintf(szString, cMaxStringLen-1, _T("b_WaitTextType%d"), a+1);
			pInfo->m_Wait[a].byTextType	= GetDataBYTE(szString,nRow);
		}
		
		for (a = 0; a < MAX_NPC_VOICE_SAMPLE; a++)
		{
			//d_Meet
			Snprintf(szString, cMaxStringLen-1, _T("d_Meet%d"), a+1);
			pInfo->m_Meet[a].dwVoiceID	= GetDataDWORD(szString,nRow);
			//b_MeetVoiceType
			Snprintf(szString, cMaxStringLen-1, _T("b_MeetVoiceType%d"), a+1);
			pInfo->m_Meet[a].byVoiceType = GetDataBYTE(szString,nRow);
			//d_MeetText
			Snprintf(szString, cMaxStringLen-1, _T("d_MeetText%d"), a+1);
			pInfo->m_Meet[a].dwTextID	= GetDataDWORD(szString,nRow);
			//b_MeetTextType
			Snprintf(szString, cMaxStringLen-1, _T("b_MeetTextType%d"), a+1);
			pInfo->m_Meet[a].byTextType	= GetDataBYTE(szString,nRow);
		}
        
		for (a = 0; a < MAX_NPC_VOICE_SAMPLE; a++)
		{
			//d_Leave
			Snprintf(szString, cMaxStringLen-1, _T("d_Leave%d"), a+1);
			pInfo->m_Leave[a].dwVoiceID	= GetDataDWORD(szString,nRow);
			//b_LeaveVoiceType
			Snprintf(szString, cMaxStringLen-1, _T("b_LeaveVoiceType%d"), a+1);
			pInfo->m_Leave[a].byVoiceType = GetDataBYTE(szString,nRow);
			//d_LeaveText
			Snprintf(szString, cMaxStringLen-1, _T("d_LeaveText%d"), a+1);
			pInfo->m_Leave[a].dwTextID	= GetDataDWORD(szString,nRow);
			//b_LeaveTextType
			Snprintf(szString, cMaxStringLen-1, _T("b_LeaveTextType%d"), a+1);
			pInfo->m_Leave[a].byTextType	= GetDataBYTE(szString,nRow);
		}

        if( m_pDataTable->GetData(pInfo->m_w_NPCCode) != NULL )
        {
            // 그냥넣으면 메모리릭이 발생하므로 지우고 패스
            delete pInfo;
            continue;
        }

		m_pDataTable->Add(pInfo, pInfo->m_w_NPCCode);
	}
	
	return TRUE;
}


#pragma warning ( pop )
