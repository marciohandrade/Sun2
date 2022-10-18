#include "stdafx.h"

#include "WebRequest.h"

bool CWebRequest::Init(DWORD dwIndex, char* pURL, INTERNET_PORT wPort)
{
	if( NULL == pURL || !strlen(pURL) )
	{
		OutputDebug("[CWebRequest::Init] Error, URL Is NULL or Empty\n");
		return false;
	}

	OutputDebug("[CWebRequest::Init] Recv, URL \"%s\"\n", pURL);

	char* pStartAddress = pURL;

	if( !strncmp( pStartAddress, "http://", strlen("http://") ) )
	{
		// URL의 HTTP 부분을 떼어낸다
        pStartAddress += strlen("http://");
	}

	char* pEndAddress = strrchr( pStartAddress, '/' );	// URL과 파일이름의 구분자를 찾는다

	if( NULL == pEndAddress )
	{
		OutputDebug("[CWebRequest::Init] Error, Invaild URL(File Not found), URL : %s\n", pURL);
		return false;
	}

	char* pPort = strrchr( pStartAddress, ':' );	// 지정 포트를 찾는다

	if( NULL != pPort )
	{
		pPort[0] = NULL;
		pPort += 1;	// 구분자 다음이 포트이므로 한칸 이동
		m_URLInfo.wPort = static_cast<INTERNET_PORT>( atoi(pPort) );	// 포트 복사
	}
	else
	{
		pEndAddress[0] = NULL;
		m_URLInfo.wPort = wPort;										// 기본 포트 복사
	}

	char* pFileName = pEndAddress + 1;				// 파일 이름

	if( !strchr(pFileName, '.') )					// 파일이름.확장자 <- 파일 이름이 이 구조로 되어 있는지 검사
	{
        OutputDebug("[CWebRequest::Init] Error, Invaild URL(File Extend Not found), URL : %s\n", pURL);
		return false;
	}

	strcpy(&m_URLInfo.szURL[0], pStartAddress );	// 감시 URL
	strcpy(&m_URLInfo.szFileName[0], pFileName);	// 파일 이름
	m_dwURLIndex = dwIndex;							// URL 고유 Index

	return true;
}

bool CWebRequest::Open()
{
	m_hOpen = InternetOpen(DEFAULT_AGENT_NAME, INTERNET_OPEN_TYPE_PRECONFIG, "", "", 0);
	if( m_hOpen == NULL )
	{
		OutputDebug("[CWebRequest::Open()] Error, Not InternetOpen : %d\n", GetLastError() );
		Clear();
		return false;
	}

	return true;
}

bool CWebRequest::Connect()
{
	m_hConnect = InternetConnect(m_hOpen, m_URLInfo.szURL, m_URLInfo.wPort, "", "", INTERNET_SERVICE_HTTP, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if( m_hConnect == NULL )
	{
		OutputDebug("[CWebRequest::Connect()] Error, Not InternetConnect : %d\n", GetLastError() );
		Clear();
		return false;
	}

	return true;
}

bool CWebRequest::RequestGet()
{
	char szTemp[32] = {0, };
	sprintf(&szTemp[0], "?Tickcount=%d", ::GetTickCount() );	// Request 요청 시간이 같으면 Page Fault가 일어나더라도 항상
																// OK를 리턴하므로 요청할때마다 시간을 갱신한다

	char szTempURL[MAX_FILENAME+32] = {0, };

	strcpy(szTempURL, m_URLInfo.szFileName);
	strcat(szTempURL, szTemp);

	m_hRequest = HttpOpenRequest(m_hConnect, "GET", szTempURL, NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);

	if( ::HttpSendRequest(m_hRequest, NULL, 0, NULL, 0) )
	{
		char szTemp[4] = {0, };
		DWORD dwSize = sizeof(DWORD);

        HttpQueryInfo( m_hRequest, HTTP_QUERY_STATUS_CODE, &szTemp, &dwSize, NULL);

		m_dwStateCode = static_cast<DWORD>( atoi(szTemp) );

		return true;
	}
	else
	{
		m_dwStateCode = GetLastError();
		OutputDebug("[CWebRequest::RequestGet()] Error, HttpSendRequest TimeOut, GetLastError : %d\n", m_dwStateCode);

		return false;
	}
}

void CWebRequest::Clear()
{
	if( m_hRequest )
	{
		InternetCloseHandle(m_hRequest);
	}

	if( m_hConnect )
	{
		InternetCloseHandle(m_hConnect);
	}

	if( m_hOpen )
	{
		InternetCloseHandle(m_hOpen);
	}

	m_dwStateCode = 0;
	m_dwURLIndex = 0;
}