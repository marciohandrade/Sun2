#include "stdafx.h"

#include "WebRequestMonitor.h"
#include "RTTServer.h"

CWebRequestMonitor::CWebRequestMonitor() : m_eventKill(INVALID_HANDLE_VALUE), m_hThread(INVALID_HANDLE_VALUE), m_bStartRequest(false)
{
}

CWebRequestMonitor::~CWebRequestMonitor()
{
	if( m_bStartRequest )
	{
		::SetEvent(m_eventKill);
		::WaitForSingleObject(m_hThread, INFINITE);

		CloseHandle(m_eventKill);
		CloseHandle(m_hThread);
		m_eventKill = m_hThread = INVALID_HANDLE_VALUE;
        m_bStartRequest = false;

		if( !m_WebRequestList.empty() )
		{
			for( OBSERVERLIST::iterator iter = m_WebRequestList.begin() ; iter != m_WebRequestList.end() ; ++iter )
			{
				delete *iter;
			}
		}

		OutputDebug("[CWebRequestMonitor::~CWebRequestMonitor] Return WaitForSingleObject\n");
	}
}

void CWebRequestMonitor::StartWebRequestMonitor()
{
	if( !m_bStartRequest )
	{
		m_eventKill			= ::CreateEvent( NULL, false, false, NULL );
		unsigned int uiThreadID = 0;
		m_hThread = (HANDLE)_beginthreadex(0, 0, Runner, this, 0, &uiThreadID);
		m_bStartRequest = true;

        OutputDebug("[CWebRequestMonitor::StartWebRequestMonitor] Runner thread Create\n");
	}
}

bool CWebRequestMonitor::AddURL(DWORD dwIndex, char* pURL)
{
	if( NULL == pURL || !strlen(pURL) )
	{
		OutputDebug("[CWebRequestMonitor::AddURL] Error, URL Is NULL or Empty\n");
		return false;
	}

	CWebRequest* pWebRequest = new CWebRequest;	// 감시 클래스 생성

	if(	!pWebRequest->Init(dwIndex, pURL) )		// 감시 클래스 초기화
	{
        OutputDebug("[CWebRequestMonitor::AddURL] Error, Don't WebRequest Initialize\n");
		return false;
	}

    eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutexRequest );
	m_WebRequestList.push_back(pWebRequest);

	return true;
}

unsigned int __stdcall CWebRequestMonitor::Runner(void* lParam)
{
	CWebRequestMonitor* pWebRequestMonitor = (CWebRequestMonitor*) lParam;
	MSG_RTTR_WEB_REQUEST_NTF SendPacket;
	CWebRequest* pWebRequest = NULL;

	while(1)
	{
		if( WaitForSingleObject(pWebRequestMonitor->m_eventKill, 5000) == WAIT_OBJECT_0 )
		{
			OutputDebug("[CWebRequestMonitor::Runner] escape Ruuner Thread!!\n");
			break;
		}

		{
            eXtreme::Guard	< eXtreme::Mutex >	guard( pWebRequestMonitor->m_mutexRequest );

			for( CWebRequestMonitor::OBSERVERLIST::iterator iter = pWebRequestMonitor->m_WebRequestList.begin() ; iter != pWebRequestMonitor->m_WebRequestList.end() ; ++iter )
			{
				SendPacket.dwStatusCode = 0;

                pWebRequest = (*iter);   

				if( !pWebRequest->Open() )
				{
					OutputDebug("[CWebRequestMonitor::Runner] Error, Not InternetOpen\n");
					SendPacket.dwStatusCode = ERROR_INTERNET_TIMEOUT;
				}

				if( !pWebRequest->Connect() )
				{
					OutputDebug("[CWebRequestMonitor::Runner] Error, Not InternetConnect\n");
					SendPacket.dwStatusCode = ERROR_INTERNET_TIMEOUT;
				}

				if( !pWebRequest->RequestGet() )								// Request
				{
					OutputDebug("[CWebRequestMonitor::Runner] Error, GetLastError : %d\n", pWebRequest->GetStateCode() );
				}

				if( !SendPacket.dwStatusCode )		// 위에서 에러가 나지 않았다면 현재 State는 0 이다
				{
					SendPacket.dwStatusCode = pWebRequest->GetStateCode();	// Request 결과 상태 코드
				}
				SendPacket.dwURLIndex = pWebRequest->GetIndex();			// Request 완료한 URL의 고유 인덱스
				RTTServer::instance->SendMessage( SendPacket, SendPacket.dwSize + sizeof( MSG_HEADER8 ) );	// 서버로 전송

				OutputDebug("Send Request : %d, Send Time : %d\n", SendPacket.dwStatusCode, GetTickCount() / 1000);

				pWebRequest->Clear();
			}
		}
	}

	_endthreadex(0);
	return 1;
}
