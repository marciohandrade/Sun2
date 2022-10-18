#include "SunClientPrecompiledHeader.h"
#include "message.h"
#include "GlobalFunc.h"

MessageInfoParser::MessageInfoParser(void)
{
    m_pMessagePool = NULL;
}

MessageInfoParser::~MessageInfoParser(void)
{
	assert( NULL == m_pMessagePool );
}

void MessageInfoParser::Init(DWORD dwMaxSize)
{
	m_dwMaxSize = dwMaxSize;
	m_pMessagePool = new util::CMemoryPoolFactory<MESSAGE_INFO>;
	m_pMessagePool->Initialize( m_dwMaxSize, m_dwMaxSize/2+1 );
}

//VOID MessageInfoParser::Init( DWORD dwMaxSize, char * pszPackFileName )
//{
//	Init( dwMaxSize );
//	SetPackFileName( pszPackFileName );
//}

MESSAGE_INFO * MessageInfoParser::GetMessage( DWORD dwID )
{
	MESSAGE_INFO_ITR itr;

	itr = m_MessageHashmap.find(dwID);
	return (itr != m_MessageHashmap.end())? itr->second : NULL;
}

bool MessageInfoParser::SetMessage(DWORD message_id, TCHAR* string)
{
    MESSAGE_INFO_ITR itr;
    itr = m_MessageHashmap.find(message_id);

    MESSAGE_INFO* message_info_ptr = NULL;
    if (itr != m_MessageHashmap.end())
    {
        message_info_ptr = itr->second;
        message_info_ptr->MessageLen = strlen(string);
        message_info_ptr->pszMessage = string;
        return true;
    }
    return false;
}

void MessageInfoParser::Release()
{
	m_MessageHashmap.clear();

	if (m_pMessagePool)
	{
		m_pMessagePool->Release();
		delete m_pMessagePool;
		m_pMessagePool = NULL;
	}
}

void MessageInfoParser::UnLoad()
{
	m_MessageHashmap.clear();
}

BOOL MessageInfoParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	UnLoad();

	DWORD dwID = 0;

	int nRowSize = GetRowSize();
	for(int nRow = 0; nRow < nRowSize; ++nRow)
	{
		dwID = GetDataDWORD("MessageCode", nRow);

		MESSAGE_INFO * pInfo = (MESSAGE_INFO *)m_pMessagePool->Alloc();
		assert(pInfo || !"메시지가 넘쳤다!");

		pInfo->iTime	  = GetDataInt("Duration", nRow);
		pInfo->byType     = GetDataBYTE("type", nRow);
		pInfo->dwImgCode  = GetDataDWORD("Imgcode", nRow);
		pInfo->dwNCode    = GetDataDWORD("Ncode", nRow);
		pInfo->dwDCode    = GetDataDWORD("Descode", nRow);

#ifdef _DEBUG
		MESSAGE_INFO * pPreInfo = pInfo;
#endif

#ifdef _DH_OS_LANGUAGETYPE
        if (GetMessage(dwID) == NULL)
#endif//_DH_OS_LANGUAGETYPE
        {
		    m_MessageHashmap.insert( MESSAGE_INFO_PAIR(dwID, pInfo) );
        }
	}

	return TRUE;
}




