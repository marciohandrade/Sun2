#include "genericftpclient.h"
#include ".\genericftpclient.h"

CGenericFTPClient::CGenericFTPClient(void)
: m_hInetOpen(INVALID_HANDLE_VALUE),
  m_hInetConnection(INVALID_HANDLE_VALUE)
{
	Initialize();
}

CGenericFTPClient::~CGenericFTPClient(void)
{
	UnInitialize();	
}

void CGenericFTPClient::Initialize(void)
{
	m_hInetOpen = ::InternetOpen(__GENERIC_FTP_AGENT,			// agent
							     INTERNET_OPEN_TYPE_PRECONFIG,	// access type
							     NULL,							// proxy
							     NULL,							// proxy by pass
							     0);							// flags
}

void CGenericFTPClient::UnInitialize(void)
{
	if( m_hInetConnection != INVALID_HANDLE_VALUE )	// Connection Handle 정리
	{
		::InternetCloseHandle(m_hInetConnection);
		m_hInetConnection= INVALID_HANDLE_VALUE;
	}
	
	if( m_hInetOpen != INVALID_HANDLE_VALUE )	// Open Handle 정리
	{
		::InternetCloseHandle(m_hInetOpen);
		m_hInetOpen= INVALID_HANDLE_VALUE;
	}
}

BOOL CGenericFTPClient::Open(LPCSTR lpFTPAddress, LPCSTR lpUserName, LPCSTR lpPassword)
{
	return Open(lpFTPAddress, INTERNET_DEFAULT_FTP_PORT, lpUserName, lpPassword);
}

BOOL CGenericFTPClient::Open(LPCSTR lpFTPAddress, DWORD dwPort, LPCSTR lpUserName, LPCSTR lpPassword)
{
	BOOL bRet = FALSE;
	if( m_hInetOpen != INVALID_HANDLE_VALUE )	// Handle 유효성 검사
	{
		m_hInetConnection = ::InternetConnect(m_hInetOpen,
											  lpFTPAddress, 
											  (INTERNET_PORT)dwPort,
											  lpUserName, 
											  lpPassword,
											  INTERNET_SERVICE_FTP,
											  INTERNET_FLAG_PASSIVE,
											  NULL);
		bRet = TRUE;
	}
	return bRet;
}

void CGenericFTPClient::GetInternetFile(LPCSTR lpRemoteFileName, LPCSTR lpLocalFileName)
{
	if( m_hInetOpen != INVALID_HANDLE_VALUE && m_hInetConnection != INVALID_HANDLE_VALUE )	// Handle 유효성 검사
	{
		HINTERNET hInetRequest;

		hInetRequest = ::FtpOpenFile(m_hInetConnection,
									 lpRemoteFileName,
									 GENERIC_READ,
									 INTERNET_FLAG_TRANSFER_BINARY,
									 NULL);

		if(hInetRequest != INVALID_HANDLE_VALUE) // 파일 open 성공
		{
			HANDLE hFile=::CreateFile(lpLocalFileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			BYTE byReceiveBuffer[__DEFAULT_BUFFER_SIZE]="\0";
			DWORD dwReadSize = 0;

			if(hFile){
				while(::InternetReadFile(hInetRequest, byReceiveBuffer, __DEFAULT_BUFFER_SIZE, &dwReadSize) && dwReadSize>0)
				{
					::WriteFile(hFile, byReceiveBuffer, dwReadSize, &dwReadSize, NULL);
				}
				::CloseHandle(hFile);
			}			
		}

		if(hInetRequest != INVALID_HANDLE_VALUE)	// 
		{
			::InternetCloseHandle(hInetRequest);
			hInetRequest= INVALID_HANDLE_VALUE;
		}		
	}
}
void CGenericFTPClient::Close(void)
{
	UnInitialize();
}
