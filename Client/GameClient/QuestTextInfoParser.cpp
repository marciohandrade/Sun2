//------------------------------------------------------------------------------				    
//                   	
//	(C) 2005 y2jinc					
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "QuestTextInfoParser.h"
#include <SolarFileReader.h>
#include "GlobalFunc.h"

//----------------------------------------------------------------------------                   	
/**                    
*/
QuestTextInfoParser::QuestTextInfoParser():	m_pDataTable ( NULL )
{
}

//----------------------------------------------------------------------------                   	
/**                    
*/
QuestTextInfoParser::~QuestTextInfoParser()
{
	assert( m_pDataTable == NULL);
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID QuestTextInfoParser::Release()
{
	Unload();
	delete m_pDataTable;
	m_pDataTable = NULL;
}

//----------------------------------------------------------------------------                   	
/**                    
*/
VOID QuestTextInfoParser::Init(DWORD dwPoolSize , char *pszPackFileName)
{
	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<QuestTextInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	SetPackFileName( pszPackFileName );
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID QuestTextInfoParser::Init(DWORD dwPoolSize )
{	assert( m_pDataTable == NULL );
	m_pDataTable = new util::SolarHashTable<QuestTextInfo*>;
	m_pDataTable->Initialize( dwPoolSize );
	
}
//----------------------------------------------------------------------------                   	
/**                    
*/
VOID QuestTextInfoParser::Unload()
{
	QuestTextInfo * pInfo = NULL;
	if( m_pDataTable == NULL )
	{
		return;
	}
	m_pDataTable->SetFirst();
	while( pInfo = m_pDataTable->GetNext() )
	{
		delete [] pInfo->m_pszText;
		delete pInfo;
	}
	m_pDataTable->RemoveAll();
}

//----------------------------------------------------------------------------                   	
/**                    
*/
QuestTextInfo * 
QuestTextInfoParser::GetQuestTextInfo(DWORD dwKey)
{

	return m_pDataTable->GetData( dwKey );
}

#pragma warning ( push )
#pragma warning ( disable : 4244)
//----------------------------------------------------------------------------                   	
/**                    
*/
BOOL QuestTextInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	TCHAR szBuf[10240];
	int nRowSize = GetRowSize();
    DWORD string_code = 0;
	for (int nRow = 0; nRow < nRowSize; ++nRow)
	{
        string_code = GetDataNumber("StrCode", nRow);

		if (NULL != m_pDataTable->GetData(string_code))
		{
#ifdef _DH_OS_LANGUAGETYPE
#else
			WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE,_T("[QuestText.txt] StrCode[%d] ม฿บน!\n"),
                string_code);
			assert(NULL == m_pDataTable->GetData(string_code));
#endif//_DH_OS_LANGUAGETYPE
		}
        else
        {
            QuestTextInfo * pInfo = new QuestTextInfo;
            pInfo->m_dwCode = string_code;
            StrCopy( szBuf, GetDataString("Str", nRow));
            pInfo->m_pszText = new TCHAR[_tcslen(szBuf)+1];
            StrCopy(pInfo->m_pszText, szBuf);

		    m_pDataTable->Add(pInfo, pInfo->m_dwCode);
        }
	}

	return TRUE;
}



#pragma warning ( pop )
