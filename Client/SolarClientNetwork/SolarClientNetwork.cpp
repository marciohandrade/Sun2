#include "StdAfx.h"
#include ".\solarclientnetwork.h"
#include ".\Connection.h"
#include ".\processthread.h"
#include ".\ConnectionAllocator.h"
#include <winsock2.h>
#include <stdio.h> 
#include <hash_map> 

typedef stdext::hash_map<DWORD, Connection*> conn_hash;

struct SolarClientNetwork::INTERAL_INFO
{
	INTERAL_INFO()
	:	m_bEndNetowrk ( FALSE )
	,	m_pProcessThread ( NULL )
	,	m_pConnectionPool ( NULL )
	,	m_nMaxConnectionNum ( 0 )
#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    ,   m_pDoubleBuffer(0)
#endif
	{
	}
	~INTERAL_INFO()
	{

	}
	BOOL										m_bEndNetowrk;
	ProcessThread *								m_pProcessThread;
	stdext::hash_map<DWORD, Connection*>		m_ConnectionHash;
	//util::SolarHashTable<Connection *> *		m_pConnectionHash;
	ConnectionAllocator *						m_pConnectionPool;
	DWORD										m_nMaxConnectionNum;
	HANDLE										m_phSendEvent[MAX_CONNECTION_OBJECTS];
	HANDLE										m_phRecvEvent[MAX_CONNECTION_OBJECTS];
	fn_Recv										m_fnCallBackRecv;
	fn_Disconnect								m_fnCallBackDisconnect;
	fn_Error									m_fnCallBackError;
#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    CircuitQueueByte*                           m_pDoubleBuffer;
#endif
};

DWORD SolarClientNetwork::GetMaxConnectionNum()
{
	if( !in ) return 0;

	return in->m_nMaxConnectionNum;
}

Connection * SolarClientNetwork::FindConnection( DWORD dwIndex )
{
	Connection * pConn = NULL;
	conn_hash::iterator it = in->m_ConnectionHash.find( dwIndex );
	if( it == in->m_ConnectionHash.end() ) 	
	{
		return pConn;
	}
	pConn = it->second;
	return pConn;
}
SolarClientNetwork::SolarClientNetwork()
: in (NULL)
{
	WSADATA data;
	if( 0 != WSAStartup(MAKEWORD(2,2), &data) )
	{
		ASSERT( !"윈속 초기화 실패" );
	}
	if( LOBYTE( data.wVersion ) != 2 || HIBYTE( data.wVersion ) != 2 )
	{
		ASSERT( !"윈속 버전이 틀립니다." );
	}
}

SolarClientNetwork::~SolarClientNetwork()
{
	/*
	ASSERT( NULL == m_pProcessThread );
	ASSERT( NULL == m_pConnectionHash );
	ASSERT( NULL == m_pConnectionPool );
	*/

	ReleaseNetwork();

	WSACleanup();
}

#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
void*
SolarClientNetwork::GetDoubleBuffer() const
{
    return in ? in->m_pDoubleBuffer : 0;
}
#endif

VOID SolarClientNetwork::ReleaseNetwork()
{
	if(in)
	{
		if(in->m_pProcessThread )
		{
			in->m_pProcessThread->EndThread();
			delete in->m_pProcessThread;
			in->m_pProcessThread = NULL;
		}

		/*Connection * pConn = NULL;
		conn_hash::iterator it = in->m_ConnectionHash.begin(), end = in->m_ConnectionHash.end();
		for( ; it != end ; ++it )
		{
			pConn = it->second;
			pConn->Release();
			in->m_pConnectionPool->FreeConnection( pConn );
		}*/
		Connection * pConnection = NULL;
		conn_hash::iterator it = in->m_ConnectionHash.begin(), end = in->m_ConnectionHash.end();
		for( ; it != end ; )
		{
			pConnection = it->second;
			pConnection->Update( in->m_fnCallBackRecv );
			//in->m_fnCallBackDisconnect( pConnection->GetIndex() );
			pConnection->Release();
			in->m_ConnectionHash.erase( it++ );
			in->m_pConnectionPool->FreeConnection( pConnection );
		}
		in->m_ConnectionHash.clear();
		
		/*if( m_pConnectionHash )
		{
			Connection * pConn = NULL;
			m_pConnectionHash->SetFirst();
			while( pConn = m_pConnectionHash->GetNext() )
			{
				pConn->Release();
				m_pConnectionPool->FreeConnection( pConn );
			}
			m_pConnectionHash->RemoveAll();

			delete m_pConnectionHash;
			m_pConnectionHash = NULL;
		}*/
		
		if( in->m_pConnectionPool )
		{
			in->m_pConnectionPool->Release();
			delete in->m_pConnectionPool;
			in->m_pConnectionPool = NULL;
		}

		for( DWORD i = 0 ; i < in->m_nMaxConnectionNum ; ++i )
		{
			if( in->m_phSendEvent[i] )
			{
				CloseHandle( in->m_phSendEvent[i] );
				in->m_phSendEvent[i] = NULL;
			}
			if( in->m_phRecvEvent[i] )
			{
				CloseHandle( in->m_phRecvEvent[i] );
				in->m_phRecvEvent[i] = NULL;
			}
		}
    #ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
        if(in->m_pDoubleBuffer)
            delete in->m_pDoubleBuffer;
    #endif

		delete in;
		in = NULL;
	}

}
//=============================================================================================================================
/**
	@param	SOLAR_CLIENT_DESC.nMaxConnectionNum
			- 최대 접속 유지 개수
			-  MAXIMUM_WAIT_OBJECTS(64)/2를 넘을 수 없다.
*/
//=============================================================================================================================
VOID SolarClientNetwork::InitializeNetwork( SOLAR_CLIENT_DESC & desc )
{
	if( !in ) in = new INTERAL_INFO;
	in->m_bEndNetowrk				= FALSE;
	in->m_fnCallBackRecv			= desc.fnCallBackRecv;
	//m_fnCallBackConnect			= desc.fnCallBackConnect;
	in->m_fnCallBackDisconnect		= desc.fnCallBackDisconnect;
	in->m_fnCallBackError			= desc.fnCallBackError;
	in->m_nMaxConnectionNum			= desc.nMaxConnectionNum;


	//----------------------------------------------------------------------------------
	// 자료구조 초기화 ( 메모리 풀, 해쉬 테이블 ) 
	//----------------------------------------------------------------------------------
	// overlapped io는 MAXIMUM_WAIT_OBJECTS(64)/2개까지의 connection을 유지 할 수 있음.!!!!!!!
	if( desc.nMaxConnectionNum > MAX_CONNECTION_OBJECTS )
	{
		_setLastError( "최대 접속 허용 개수는 (%d)를 넘을 수 없습니다.\n", MAX_CONNECTION_OBJECTS );
		return;
	}

	//m_pConnectionHash			= new util::SolarHashTable<Connection *>;
	in->m_pConnectionPool		= new ConnectionAllocator;
	//m_pConnectionHash->Initialize( desc.nMaxConnectionNum );
	in->m_pConnectionPool->Initialize( desc.nMaxConnectionNum );
	for( DWORD i = 0 ; i < desc.nMaxConnectionNum ; ++i )
	{
		in->m_phSendEvent[i] = CreateEvent( NULL, FALSE, FALSE, NULL );
		in->m_phRecvEvent[i] = CreateEvent( NULL, FALSE, FALSE, NULL );
		in->m_pConnectionPool->GetConnection( i )->Create( desc.nMaxBufferSizePerConn, in->m_phSendEvent[i], in->m_phRecvEvent[i] );
	}

#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__

    // (Reference) (WxFramework 08121603)
    #define WxuRoundUp(_align, _size)   \
        ( ((size_t)(_size) + ((size_t)(_align) - 1)) & ~((size_t)_align - 1) )

    CircuitQueueByte* pDoubleBuffer = new CircuitQueueByte;
    in->m_pDoubleBuffer = pDoubleBuffer;
    DWORD chunkSize = desc.nMaxBufferSizePerConn + MAX_PACKET_SIZE;
    chunkSize = DWORD(WxuRoundUp(128, chunkSize));

    pDoubleBuffer->Create(chunkSize, 0, true);
#endif

	//----------------------------------------------------------------------------------
	// overlapped I/O를 담당하게 될 스레드 생성
	//----------------------------------------------------------------------------------
	in->m_pProcessThread = new ProcessThread( *this, in->m_pConnectionPool, in->m_phSendEvent, in->m_phRecvEvent );
	in->m_pProcessThread->StartThread();

#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    {
        HANDLE threadHandle = in->m_pProcessThread->GetHandle();
        SetThreadPriority(threadHandle, THREAD_PRIORITY_ABOVE_NORMAL);
    }
#endif

}
//=============================================================================================================================
/**
	@remarks
			- CallBackRecv
			- CallBackDisconnect
*/
//=============================================================================================================================
BOOL SolarClientNetwork::UpdateNetwork()
{
	if( !in || in->m_bEndNetowrk ) return FALSE;

	Connection * pConnection = NULL;
	conn_hash::iterator it = in->m_ConnectionHash.begin(), end = in->m_ConnectionHash.end();
	for( ; it != end ; )
	{
		pConnection = it->second;
		if( ! pConnection->Update( in->m_fnCallBackRecv ) )
		{
			in->m_fnCallBackDisconnect( pConnection->GetIndex() );
			pConnection->Release();
			in->m_ConnectionHash.erase( it++ );
			in->m_pConnectionPool->FreeConnection( pConnection );
		}
		else
		{
			++it;
		}
	}

	/*Connection * pConnection = NULL;
	m_pConnectionHash->SetFirst();
	while( pConnection = m_pConnectionHash->GetNext() )
	{
		if( ! pConnection->Update( m_fnCallBackRecv ) )
		{
			m_fnCallBackDisconnect( pConnection->GetIndex() );
			pConnection->Release();
			m_pConnectionHash->Remove( pConnection->GetIndex() );
			m_pConnectionPool->FreeConnection( pConnection );
		}
	}*/

	return TRUE;
}
//=============================================================================================================================
/**
	@remarks
			- SolarClientNetwork를 종료 처리 한다.
			- UpdateNetwork() return FALSE;
*/
//=============================================================================================================================
VOID SolarClientNetwork::CloseNetwork()
{
	in->m_bEndNetowrk = TRUE;
}

//=============================================================================================================================
/**
	@param	dwIndex
			- 연결하고자 하는 인덱스
	@retval	DWORD
			- 연결이 성공하면 return 1;
			  연결이 실패하면 return 0;
*/
//=============================================================================================================================
BOOL SolarClientNetwork::Connect( DWORD dwIndex, char * pszIP, WORD wPort, BYTE HeaderSize, BOOL bExcludeHeaderSize)
{
	if( HeaderSize > MAX_HEADER_SIZE )
	{
		_setLastError( "Header Size is Much Big(%d)\n", HeaderSize );
		return FALSE;
	}

	if( FindConnection( dwIndex ) ) 
	{
		_setLastError( "[%s]Already using conection index(%d)\n", __DATE__ __TIME__,  dwIndex );
		return FALSE;
	}

	Connection * pConnection = in->m_pConnectionPool->AllocConnection();
	if( !pConnection )
	{
		_setLastError( "[%s]Max(%d) Connection is full\n", __DATE__ __TIME__, in->m_nMaxConnectionNum );
		return FALSE;
	}

	pConnection->Initialize( this );
	pConnection->SetIndex( dwIndex );

	BOOL rt = pConnection->Connect( pszIP, wPort, HeaderSize, bExcludeHeaderSize );

	if( TRUE == rt )
	{
		in->m_ConnectionHash.insert( conn_hash::value_type(pConnection->GetIndex(),pConnection) );
	}
	else 
	{
		pConnection->Release();
		in->m_pConnectionPool->FreeConnection( pConnection );
	}
	return rt;
}

const char * SolarClientNetwork::GetConnectionIP( DWORD dwIndex )
{
	Connection * pConnection = FindConnection(dwIndex);
	if( pConnection )
		return pConnection->GetIP();
	return NULL;
}

BOOL SolarClientNetwork::IsDisconnected( DWORD dwIndex )
{
	conn_hash::iterator it = in->m_ConnectionHash.find( dwIndex );
	if( it == in->m_ConnectionHash.end() || (it->second)->IsDisconnected() ) 	
	{
		return TRUE;
	}

	return FALSE;
}

BOOL SolarClientNetwork::Disconnect( DWORD dwIndex )
{
	Connection * pConnection = FindConnection(dwIndex);
	if( !pConnection )
	{
		//for test
		Connection * pConnection = in->m_pConnectionPool->GetConnection( dwIndex );
		if( pConnection )
		{
			_setLastError( "SendPacket():dwIndex(%d)pooIDX(%d)dis(%d)\n", dwIndex,pConnection->GetArrayIndex(), pConnection->IsDisconnected() );
		}
		else
			_setLastError( "[%s] Disconnect() : dwIndex(%d)의 Connection을 찾을 수 없습니다.\n", __DATE__ __TIME__, dwIndex );
		return FALSE;
	}
	return pConnection->CloseSocket();
}

BOOL SolarClientNetwork::SendPacket( DWORD dwIndex, VOID * pMsg, DWORD dwMsgSize )
{
	Connection * pConnection = FindConnection(dwIndex);
	if( !pConnection )
	{
		//for test
		pConnection = in->m_pConnectionPool->GetConnection( dwIndex );
		if( pConnection )
		{
			_setLastError( "SendPacket():dwIndex(%d)pooIDX(%d)dis(%d)\n", dwIndex,pConnection->GetArrayIndex(), pConnection->IsDisconnected() );
		}
		else
			_setLastError( "SendPacket():dwIndex(%d)의InvalidConnectionidx[%s]\n", dwIndex, __DATE__ );

		return FALSE;
	}
	BOOL rt = pConnection->Send( pMsg, dwMsgSize );

	return rt;
}

VOID SolarClientNetwork::_setLastError( const char * pszErrorMsg, ... )
{
	char pszBuffer[512]; 

	va_list ArgPtr; 
	va_start( ArgPtr, pszErrorMsg ); 
	_vsnprintf( pszBuffer, 512, pszErrorMsg, ArgPtr ); 
	va_end( ArgPtr ); 

	in->m_fnCallBackError( pszBuffer );
}