#include <eXtreme/Common.h>

#include <eXtreme/net/Connector.h>
#include <eXtreme/net/ServerSession.h>
#include <eXtreme/threading/Guard.h>

using namespace eXtreme;

Connector::Connector()
	: m_eventQueue( NULL ), m_eventKill( INVALID_HANDLE_VALUE )
{
}

Connector::~Connector()
{
	Destroy();
}

void Connector::Create( EventQueue* eq )
{
	if( m_eventKill != INVALID_HANDLE_VALUE )
	{
		return;
	}

	m_eventQueue = eq;
	m_pool.Create( 10 );

	m_eventKill	= ::CreateEvent( 0, 0, 0, 0 );

	if( NULL == m_eventKill )
	{
		
	}
	Start();
}

void Connector::Destroy()
{
	if( m_eventKill == INVALID_HANDLE_VALUE )
	{
		return;
	}

	//  Thread 종료
	{
		::SetEvent( m_eventKill );
		if( WaitForTerminate( 1000 ) == FALSE )
			Terminate( 0 );

		::CloseHandle( m_eventKill );
		m_eventKill		= INVALID_HANDLE_VALUE;
		m_eventQueue	= 0;
	}

	//  Pool 제거
	{
		while( m_queue.size() > 0 )
		{
			m_pool.Push( m_queue.front() );
			m_queue.pop();
		}

		m_pool.Destroy();
	}
}

void Connector::Connect( ServerSession* s, char* ipAddress, unsigned short portNo )
{
	ConnectInfo*	info = NULL;

	{
		Guard	< Mutex >	guard( m_mutexPool );

		info = m_pool.Pop();
		if( info == 0 )
		{
			m_pool.IncSize( 10 );
			info = m_pool.Pop();
		}

		s->Create();

		::strcpy( info->ipAddress, ipAddress );
		info->session	= s;
		info->portNo	= portNo;
	}

	{
		Guard	< Mutex >	guard( m_mutexQueue );

		m_queue.push( info );
	}
}

void Connector::Run()
{
	ConnectInfo*	info = NULL;

	while( 1 )
	{
		{
			Guard	< Mutex >	guard( m_mutexQueue );
			if( m_queue.size() == 0 ) {
				info = NULL;
			}
			else {
                info = m_queue.front();
				m_queue.pop();
			}
		}

		if( NULL == info ) 
		{
			if( ::WaitForSingleObject( m_eventKill, 100 ) == WAIT_OBJECT_0 ) 
			{
				break;
			}
		}
		else
		{
			::memset( &info->session->m_resultConnect, 0, sizeof( info->session->m_resultConnect ) );
			info->session->m_resultConnect.eventType	= Event_Connect;
			info->session->m_resultConnect.handler		= info->session;

			Socket* s	= info->session->GetHandle();

			bool ret	= s->Connect( info->ipAddress, info->portNo );

			if( ret == true )
			{	
				info->session->m_resultConnect.transBytes = 1;
			}
			else
			{
				info->session->m_resultConnect.transBytes = 0;
				info->session->m_resultConnect.error = ::GetLastError();
			}

			m_eventQueue->PostCompletion( info->session, &info->session->m_resultConnect );

			{
				Guard	< Mutex >	guard( m_mutexPool );

				::memset( info, 0, sizeof( *info ) );
				m_pool.Push( info );
			}
		}
	}
}