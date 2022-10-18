#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/io/EventQueue.h>

using namespace eXtreme;

//#include <iphlpapi.h>
//#pragma comment(lib, "Iphlpapi.lib")
//
//void EventHandler::PrintTCPStatus()
//{
//    struct in_addr inadLocal;
//	struct in_addr inadRemote;
//
//	DWORD dwStatus = NO_ERROR;
//	DWORD dwTCPActualSize = 0;
//
//	char szStatus[16] = {0, };
//
//	PMIB_TCPTABLE pTcpTable = NULL;
//
//	// query for buffer size needed
//	dwStatus = GetTcpTable(pTcpTable, &dwTCPActualSize, TRUE);
//
//	if(dwStatus == ERROR_INSUFFICIENT_BUFFER)
//	{
//		// need more space
//		pTcpTable = (PMIB_TCPTABLE) malloc(dwTCPActualSize);
//
//		if( NULL != pTcpTable )
//		{
//			// Get TCP IP List
//			dwStatus = GetTcpTable(pTcpTable, &dwTCPActualSize, TRUE);
//		}
//	}
//
//	if(dwStatus != NO_ERROR)
//	{
//		//	아래 구문을 Application Log 타입에 맞게 바꿔주삼~
//		//
//		//printf("Ipstat: Couldn't get tcp connection table.\n");
//		if(pTcpTable)
//		{
//			free(pTcpTable);
//		}
//	}
//	else
//	{
//		if (pTcpTable != NULL)
//		{
//			DWORD   dwRemotePort = 0;
//
//			char szTitle[128] = {0, };
//			sprintf(szTitle, "%10s %20s %20s %s", "Proto", "Local Addr", "Remote Addr", "State");
//
//			CallbackErrorHandler( 0, szTitle );
//
//			char szLocalFullAddr[24] = {0, };
//			char szRemoteFullAddr[24] = {0, };
//			char szNetStatus[128] = {0, };
//
//			for (DWORD i = 0 ; i < pTcpTable->dwNumEntries ; ++i)
//			{
//				WORD wLocalPort = static_cast<WORD>( ntohs((unsigned short)(0x0000FFFF & pTcpTable->table[i].dwLocalPort)));
//                if( (55821 != wLocalPort) && (55820 != wLocalPort) )
//				{
//					continue;
//				}
//
//				dwRemotePort = pTcpTable->table[i].dwRemotePort;
//
//				switch (pTcpTable->table[i].dwState)
//				{
//				case MIB_TCP_STATE_CLOSED:		strcpy(szStatus, "CLOSED");		break;
//				case MIB_TCP_STATE_LISTEN:		strcpy(szStatus, "LISTENING");
//												dwRemotePort = 0;				break;
//				case MIB_TCP_STATE_SYN_SENT:	strcpy(szStatus, "SYN_SENT");	break;
//				case MIB_TCP_STATE_SYN_RCVD:	strcpy(szStatus, "SYN_RCVD");	break;
//				case MIB_TCP_STATE_ESTAB:		strcpy(szStatus, "ESTABLISHED");break;
//				case MIB_TCP_STATE_FIN_WAIT1:	strcpy(szStatus, "FIN_WAIT1");	break;
//				case MIB_TCP_STATE_FIN_WAIT2:	strcpy(szStatus, "FIN_WAIT2");	break;
//				case MIB_TCP_STATE_CLOSE_WAIT:	strcpy(szStatus, "CLOSE_WAIT");	break;
//				case MIB_TCP_STATE_CLOSING:		strcpy(szStatus, "CLOSING");	break;
//				case MIB_TCP_STATE_LAST_ACK:	strcpy(szStatus, "LAST_ACK");	break;
//				case MIB_TCP_STATE_TIME_WAIT:	strcpy(szStatus, "TIME_WAIT");	break;
//				case MIB_TCP_STATE_DELETE_TCB:	strcpy(szStatus, "DELETE");		break;
//				default:
//					strcpy(szStatus, "UnKnown State!");							break;
//					break;
//				}
//
//				inadLocal.s_addr = pTcpTable->table[i].dwLocalAddr;
//				inadRemote.s_addr = pTcpTable->table[i].dwRemoteAddr;
//
//				sprintf(szLocalFullAddr, "%s:%u", inet_ntoa(inadLocal), static_cast<WORD>( ntohs((unsigned short)(0x0000FFFF & pTcpTable->table[i].dwLocalPort))));
//				sprintf(szRemoteFullAddr, "%s:%u", inet_ntoa(inadRemote), static_cast<WORD>( ntohs((unsigned short)(0x0000FFFF & dwRemotePort))));
//				sprintf(szNetStatus, "%10s %20s %20s %s", "TCP", szLocalFullAddr, szRemoteFullAddr, szStatus );
//
//				CallbackErrorHandler( 0, szNetStatus );
//
//				memset(szLocalFullAddr, 0, 24);
//				memset(szRemoteFullAddr, 0, 24);
//				memset(szNetStatus, 0, 128);
//			}
//
//		}
//
//		free(pTcpTable);
//	}
//}


EventQueue::EventQueue()
	: m_handleQueue( INVALID_HANDLE_VALUE )
{
}

EventQueue::~EventQueue()
{
	Close();
}

bool EventQueue::Create( int threadCount )
{
	if( m_handleQueue != INVALID_HANDLE_VALUE )
	{
		return false;
	}

	m_handleQueue = ::CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, threadCount );
	if( m_handleQueue == 0 )
	{
		DWORD dwErrorCode = ::GetLastError();
		CallbackErrorHandler( dwErrorCode, "[eXtreme::EventQueue::Create] CreateIoCompletionPort 생성 실패" );

		m_handleQueue = INVALID_HANDLE_VALUE;

		return false;
	}

	return true;
}

void EventQueue::Close()
{
	if( m_handleQueue == INVALID_HANDLE_VALUE )
	{
		return;
	}

	::CloseHandle( m_handleQueue );
	m_handleQueue = INVALID_HANDLE_VALUE;
}

bool EventQueue::AddEvent( EventHandler* handler )
{
	HANDLE ret = ::CreateIoCompletionPort( handler->GetHandle()->GetNativeHandle(), m_handleQueue, ( ULONG_PTR )handler, 0 );
	if( ret == 0 )
	{
		DWORD	err = ::GetLastError();

		if( err != ERROR_INVALID_PARAMETER ){
			CallbackErrorHandler( err, "[eXtreme::EventQueue::AddEvent] CreateIoCompletionPort 생성 실패" );
		}
	}

	return ( ret != 0 );
}

void EventQueue::RemoveEvent( EventHandler* handler )
{
	handler->GetHandle()->Close();
}

EventHandler* EventQueue::GetCompletionEvent( AsyncResult** result, int ms )
{
	if( result == 0 || m_handleQueue == INVALID_HANDLE_VALUE )
		return 0;
	//if( NULL == result )
	//{
 //       CallbackErrorHandler( 0, "[eXtreme::EventQueue::GetCompletionEvent] AsyncResult** result is NULL" );
	//	return NULL;
	//}

	//if( m_handleQueue == INVALID_HANDLE_VALUE )
	//{
 //       CallbackErrorHandler( 0, "[eXtreme::EventQueue::GetCompletionEvent] m_handleQueue is NULL" );
	//	return NULL;
	//}

	DWORD			transBytes = 0;
	//ULONG			key = 0;
	ULONG_PTR		key = 0;

	BOOL ret		= ::GetQueuedCompletionStatus( m_handleQueue, &transBytes, &key, ( LPOVERLAPPED* )result, ms );
	EventHandler*	handler	= ( EventHandler* )(u_int64)key;

	//if( 0 == transBytes && Event_Receive == (*result)->eventType )
	//{
	//	if( NULL != handler )
	//	{
	//		handler->PrintTCPStatus();
	//	}
	//}

	if( ret == TRUE && key != NULL && *result != NULL )
	{
		( *result )->transBytes	= transBytes;
		( *result )->error		= ::GetLastError();
	}
	else if( key != 0 && ::GetLastError() != ERROR_OPERATION_ABORTED )
	{
		DWORD error = ::GetLastError();

		if( error != ERROR_IO_PENDING && error != ERROR_SUCCESS && *result != 0 )
		{
			( *result )->error		= error;
			( *result )->transBytes	= transBytes;
			( *result )->handler	= handler;

			return handler;
	//	}
	//}
	//else
	//{
	//	if( NULL == handler )
	//	{
	//		CallbackErrorHandler( 0, "[eXtreme::EventQueue::GetCompletionEvent] AsyncResult** result is NULL" );
	//	}
	//	else
	//	{
	//		CallbackErrorHandler( 0, "[eXtreme::EventQueue::GetCompletionEvent] Unknown Error" );
		}
	}

	return handler;
}

void EventQueue::PostCompletion( EventHandler* handler, AsyncResult* result )
{
	BOOL bResult = 0;
	if( result != 0 )
	{
		bResult = ::PostQueuedCompletionStatus( m_handleQueue, result->transBytes, ( ULONG_PTR )handler, result );
	}
	else
	{
		bResult = ::PostQueuedCompletionStatus( m_handleQueue, 0, ( ULONG_PTR )handler, 0 );
	}

	if(bResult==FALSE)
	{	
	}
}