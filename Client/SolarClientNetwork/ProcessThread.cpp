#include "StdAfx.h"
#include ".\processthread.h"
#include ".\SolarClientNetwork.h"
#include ".\Connection.h"
#include ".\ConnectionAllocator.h"

ProcessThread::ProcessThread( SolarClientNetwork & net, ConnectionAllocator * pool, HANDLE * phSend, HANDLE * phRecv )
	:	m_net ( net )
{
	m_hEndThreadEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
	m_pConnectionPool = pool;
	m_phSendEvent = phSend;
	m_phRecvEvent = phRecv;
}

ProcessThread::~ProcessThread()
{
	CloseHandle( m_hEndThreadEvent );
}

VOID ProcessThread::EndThread()
{
	SetEvent( m_hEndThreadEvent );
	WaitForSingleObject( GetHandle(), INFINITE );
}

DWORD ProcessThread::run()
{
	DWORD halfnum				= ( m_net.GetMaxConnectionNum() );
	DWORD num					= ( m_net.GetMaxConnectionNum() << 1 );
	HANDLE * phEvent			= new HANDLE[num+1];
	phEvent[num]				= m_hEndThreadEvent;
	for( DWORD i = 0 ; i < halfnum ; ++i )
	{
		phEvent[i]				= m_phRecvEvent[i];
		phEvent[i+halfnum]		= m_phSendEvent[i];
	}

	DWORD dwFlag				= 0;
	DWORD dwSignalIndex			= 0;
	DWORD dwTransBytes			= 0;
	OVERLAPPEDEX * pOvlapEx		= NULL;
	Connection * pConnection	= NULL;
	SOCKET socket				= INVALID_SOCKET;
	
	while(TRUE)
	{
		if( ( dwSignalIndex = WaitForMultipleObjects( num+1, phEvent, FALSE, INFINITE ) ) == WAIT_OBJECT_0 + num )
			break;	//< 스레드 종료 이벤트

		if( dwSignalIndex < halfnum )
		{
			// recv event
			pConnection = m_pConnectionPool->GetConnection( dwSignalIndex );
			//pConnection = m_net.m_pConnectionHash->GetData( dwSignalIndex );
			pOvlapEx	= &pConnection->m_RecvOvlp;
			socket		= pConnection->GetSocket();
			ASSERT( pConnection->GetArrayIndex() == dwSignalIndex );
		}
		else 
		{
			// send event
			pConnection = m_pConnectionPool->GetConnection( dwSignalIndex - halfnum );
			//pConnection = m_net.m_pConnectionHash->GetData( dwSignalIndex - halfnum );
			//?해야 하나?
			pOvlapEx	= &pConnection->m_SendOvlp;
			socket		= pConnection->GetSocket();
			ASSERT( pConnection->GetArrayIndex() == (dwSignalIndex - halfnum) );
		}
		

		
		BOOL rt = WSAGetOverlappedResult( socket, (LPOVERLAPPED)pOvlapEx, &dwTransBytes, TRUE, &dwFlag );

		// for test
		if( pOvlapEx )
		{
			ASSERT( pOvlapEx->pConnection == pConnection );
			if( NULL == m_net.FindConnection( pConnection->GetIndex() ) )
			{
				m_net._setLastError( "[%s]Ovlap = NULL ErrorCode[%d]\n", __DATE__ __TIME__, WSAGetLastError() );
			}	
		}

		if( FALSE == rt )
		{
			if( WSAENOTSOCK == WSAGetLastError() )
			{
				//m_net._setLastError( "[%s]Error Occur WSAENOTSOCK [SOCKET:%d]\n", __DATE__ __TIME__, socket );
				//continue;
			}
			if( WSAECONNRESET == WSAGetLastError() )
			{
			}
			m_net._setLastError( "[%s]return FALSE : Index(%d) Disconnect ErrorCode[%d]\n", __DATE__ __TIME__, pConnection->GetIndex(), WSAGetLastError() );
			pConnection->Disconnect();
			continue;
		}

		if( 0 == dwTransBytes )
		{
			m_net._setLastError( "[%s]dwTransBytes == 0 : Index(%d) Disconnect ErrorCode[%d]\n", __DATE__ __TIME__, WSAGetLastError(), pConnection->GetIndex() );
			pConnection->Disconnect();
			continue;
		}

		switch( pOvlapEx->IOType )
		{
		case eIO_RECV:
			{
		
				ASSERT( pOvlapEx == &pConnection->m_RecvOvlp );
				if( !pConnection->PostRecv( dwTransBytes ) )
				{
					m_net._setLastError( "[%s]PostRecv is FALSE : Index(%d) Disconnect ErrorCode[%d]\n", __DATE__ __TIME__, pConnection->GetIndex(), WSAGetLastError() );
					pConnection->Disconnect();
				}
				
			}
			break;
		case eIO_SEND:
			{
				ASSERT( pOvlapEx == &pConnection->m_SendOvlp );
				if( !pConnection->PostSend( dwTransBytes ) )
				{
					m_net._setLastError( "[%s]PostSend is FALSE : Index(%d) Disconnect ErrorCode[%d]\n", __DATE__ __TIME__, pConnection->GetIndex(), WSAGetLastError() );
					pConnection->Disconnect();
				}
			}
			break;
		}
	}

	delete [] phEvent;
	phEvent = NULL;

	return 0;
}

