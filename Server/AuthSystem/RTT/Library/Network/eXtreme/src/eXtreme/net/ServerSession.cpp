#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/net/ServerNetwork.h>
#include <eXtreme/threading/Guard.h>


using namespace eXtreme;

ServerSession::ServerSession( int sendBufSize, int recvBufSize )
	: m_serverNetwork( NULL ), m_readyToSend( true ), m_sendBufSize( sendBufSize ),
	m_recvBufSize( recvBufSize ), m_maxSizePerUpdate( m_sendBufSize )
{
	m_uid			= (DWORD)(u_int64)this;
	m_dequeueBuffer	= _new_dbg_ char [m_recvBufSize];

	memset( &m_dequeueBuffer[0], 0, m_recvBufSize );

	m_sendBuffer.Create( m_sendBufSize );
	m_recvBuffer.Create( m_recvBufSize );
}

ServerSession::~ServerSession()
{
	m_sendBuffer.Destroy();
	m_recvBuffer.Destroy();

	delete [] m_dequeueBuffer;

	Close();
}

void ServerSession::Create()
{
	if( m_serverNetwork == 0 || m_socket.GetNativeHandle() != INVALID_HANDLE_VALUE )
	{
		assert( false );		
	}

	m_socket.Create( Socket::Protocol_TCP, true );
}

void ServerSession::Create( SOCKET s, Socket::SocketAddr& addr )
{
	if( m_serverNetwork == 0 || m_socket.GetNativeHandle() != INVALID_HANDLE_VALUE )
	{
		assert( false );
	}

	m_socket.Attach( Socket::Protocol_TCP, s, &addr );
}

void ServerSession::Close()
{
	m_socket.Close();
}

void ServerSession::Clear()
{
	m_sendBuffer.Clear();
	m_recvBuffer.Clear();

	m_sendCompletionSize	= 0;
	m_sendReqSize			= 0;
	m_readyToSend			= true;
}

void ServerSession::SetKill( DWORD err )
{
	if( m_socket.GetNativeHandle() == INVALID_HANDLE_VALUE )
	{
		return;
	}

	m_resultClose.eventType		= Event_Close;
	m_resultClose.error			= err;
	m_resultClose.handler		= this;
	m_resultClose.transBytes	= 0;
	m_serverNetwork->PostCompletion( this, &m_resultClose );

	//CallbackErrorHandler( err, "[eXtreme::ServerSession::SetKill] Post Event_Close" );
}

//  이 함수는 ServerSession을 상속받은 클래스의 생성자에서 사용할 수 없다.
void ServerSession::Connect( char* ipAddress, unsigned short portNo )
{
	m_serverNetwork->ConnectSession( this, ipAddress, portNo );
}

void ServerSession::WaitForRecv()
{
	Guard < Mutex >	guard( m_mutexRecv );

	::memset( &m_resultRecv, 0, sizeof( AsyncResult ) );
	m_resultRecv.handler	= this;
	m_resultRecv.eventType	= Event_Receive;
	
	/*
		ERROR 처리는 Send 함수에서 하였으므로 여기서는 결과 값 만을 가지고 에러를 확인한다.
	*/
	DWORD dwRecvSize = m_recvBuffer.GetRemainBufSize();
	if ( dwRecvSize > 10240 )
	{
		dwRecvSize = 10240;
	}

	int nResult = m_socket.Recv( m_resultRecv.szData, dwRecvSize, &m_resultRecv );
//	int nResult = m_socket.Recv( m_recvBuffer.GetWritePtr(), m_recvBuffer.GetWritableSize(), &m_resultRecv );
	if( nResult == 0 && m_resultRecv.error!=ERROR_IO_PENDING)
	{
		//PrintTCPStatus();
		CallbackErrorHandler( m_resultRecv.error, "[eXtreme::ServerSession::WaitForRecv] Failure Recv() -> SetKill()" );
		SetKill( m_resultRecv.error );
	}
}

void ServerSession::Update()
{
	Flush();
}

void ServerSession::Flush()
{
	Guard < Mutex >	guard( m_mutexSend );

	DWORD writeSize = m_sendBuffer.GetReadableSize();

	if( writeSize > 0 && writeSize != 52 )
		writeSize = m_sendBuffer.GetReadableSize();
	if( writeSize > m_maxSizePerUpdate )
		writeSize = m_maxSizePerUpdate;

	//  보내기 버퍼의 내용을 전송한다.
	{
		if( m_readyToSend == false || writeSize == 0 )
		{
			return;
		}

		m_readyToSend = false;

		::memset( &m_resultSend, 0, sizeof( AsyncResult ) );
		m_resultSend.handler	= this;
		m_resultSend.eventType	= Event_Send;

		m_sendCompletionSize	= 0;
		m_sendReqSize			= writeSize;

		/*
			ERROR 처리는 Send 함수에서 하였으므로 여기서는 결과 값 만을 가지고 에러를 확인한다.
		*/
		int nResult = m_socket.Send( m_sendBuffer.GetReadPtr(), writeSize, &m_resultSend );
		if( nResult == 0 && m_resultSend.error!=ERROR_IO_PENDING )
		{
			//PrintTCPStatus();
			CallbackErrorHandler( m_resultSend.error, "[eXtreme::ServerSession::Flush] Failure Send() -> SetKill()" );
			SetKill( m_resultSend.error );
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
//  Derived virtual function
void ServerSession::HandleEvent( AsyncResult* result )
{
	switch( result->eventType )
	{
	case  Event_Accept:
		Clear();
		OnAccept();
		WaitForRecv();
		break;

	case  Event_Connect:
		if( result->transBytes == 1 )
		{
			Clear();
			m_serverNetwork->AddSessionEvent( this );
			OnConnect( true, result->error );
			WaitForRecv();
		}
		else
		{
			Close();
			OnConnect( false, result->error );
		}
		break;

	case  Event_Send:
		{
			Guard < Mutex >	guard( m_mutexSend );

			//  먼저, 완료된 크기만큼 Send buffer에서 제거한다.
			//  비동기 I/O를 사용했기 때문에 완료가 발생할 때 까지 보내기 버퍼는 보존되어야 한다.
			m_sendBuffer.Dequeue( 0, result->transBytes );

			//  요청한 I/O 작업의 크기만큼 완료가 발생해야 다음 Send작업을 수행할 수 있다.
			m_sendCompletionSize += result->transBytes;
			if( m_sendReqSize <= m_sendCompletionSize )
				m_readyToSend = true;
		}

		OnSend( result->transBytes );
		break;

	case  Event_Receive:
		{
			if( result->transBytes == 0 )
			{
				/*
					2006.08.09
					modify guiun
					0byte receive는 WSAGetLastError를 호출 하지 않는다.
					EXT_ERROR_ZERO_BYTE_RECEIVE 정의를 하여 에러 코드를 리턴한다.
				*/
				CallbackErrorHandler( ::WSAGetLastError(), "[eXtreme::ServerSession::HandleEvent] Event_Receive transBytes == 0 -> SetKill()" );

				DWORD dwErrorCode = EXT_ERROR_ZERO_BYTE_RECEIVE;
				//DWORD dwErrorCode = ::WSAGetLastError();
				SetKill( dwErrorCode );
				break;
			}


			Guard < Mutex >		guard( m_mutexRecv );
			DWORD				offset = 0;
			bool				requestedRecv = false;

			// WSARecv 에서 받은 데이터만큼 사이즈를 증가 시켜줌
			if( m_recvBuffer.Enqueue( result->szData, result->transBytes ) == false )
//			if( m_recvBuffer.Enqueue( 0, result->transBytes ) == false )
			{
				assert( false );
			}

			//  아래 WaitForRecv를 호출하는 순간 result의 내용은 변할 수 있으므로
			//  필요하다면 이 곳에서 백업해두어야 한다.

// 			if( m_recvBuffer.GetWritableSize() > 0 )
// 			{
// 				//  수신 버퍼에 남은 용량이 없다면 수신 버퍼를 비워야한다.
// 				WaitForRecv();
// 				requestedRecv = true;
// 			}

			DWORD recvSize = m_recvBuffer.GetDataSize();

			if( m_recvBuffer.Peek( m_dequeueBuffer, recvSize ) == false )
			{
				assert( false );
			}

			while( recvSize > 0 )
			{
				DWORD	totalSize = 0;		//  헤더를 포함한 패킷 하나의 전체 크기

				if( IsValidPacket( &m_dequeueBuffer[offset], recvSize, &totalSize ) == true )
				{
					OnReceive( ( m_dequeueBuffer + offset ), totalSize );

					offset		+= totalSize;
					recvSize	-= totalSize;
				}
				else
				{
					break;
				}
			}

			memset(m_dequeueBuffer, 0, recvSize);

			if( m_recvBuffer.Dequeue( 0, offset ) == false )
			{
				assert( false );
			}

			if( m_recvBuffer.GetWritableSize() > 0 )
			{
				//  수신 버퍼에 남은 용량이 없다면 수신 버퍼를 비워야한다.
				WaitForRecv();
				requestedRecv = true;	// i think it does not necessary...:p
			}
			else
			{
				CallbackErrorHandler( 0, "[ServerSession::HandleEvent] 수신버퍼 부족" );
			}

			//  수신 요청을 하지 않았다면, 수신 버퍼가 비워질 때 요청한다.
// 			if( requestedRecv == false )
// 			{
// 				if( m_recvBuffer.GetWritableSize() == 0 )
// 				{
// 					//  그래도 남은 공간이 없다면 에러로 간주한다.
// 					CallbackErrorHandler( 0, "[eXtreme::ServerSession::HandleEvent] 수신버퍼 부족" );
// 				}
// 				else
// 					WaitForRecv();
// 			}
		}
		break;

	case  Event_Close:
		{
			Guard < Mutex >	guard( m_mutexSend );

			//CallbackErrorHandler( result->error, "[eXtreme::ServerSession::HandleEvent] Event_Close -> OnClose()" );
			OnClose( result->error );
			Close();
		}
		break;
	}
}

void ServerSession::GetSendBufferSize( DWORD* remain, DWORD* max )
{
	if( remain != 0 )
		*remain = m_sendBuffer.GetRemainBufSize();

	if( max != 0 )
		*max = m_sendBuffer.GetBufferSize();
}

void ServerSession::GetRecvBufferSize( DWORD* remain, DWORD* max )
{
	if( remain != 0 )
		*remain = m_recvBuffer.GetRemainBufSize();

	if( max != 0 )
		*max = m_recvBuffer.GetBufferSize();
}

bool ServerSession::SendPacket( void* buffer, int size )
{
	Guard < Mutex >	guard( m_mutexSend );

	if( m_sendBuffer.GetRemainBufSize() < ( DWORD )size )
	{
		char szMsg[256] = {0, };
		sprintf(szMsg, "[eXtreme::ServerSession::SendPacket::buffer] SendBuffer의 용량부족, Remain : %d, Packet : %d\n", m_sendBuffer.GetRemainBufSize(), size );
		CallbackErrorHandler( 0, szMsg );
		return false;
	}

//	Guard < Mutex >	guard( m_mutexSend );

	if( m_sendBuffer.Enqueue( ( char* )buffer, size ) == false )
	{
		char szMsg[256] = {0, };
		sprintf(szMsg, "[eXtreme::ServerSession::SendPacket::buffer] SendBuffer의 Enqueue실패, Remain : %d, Packet : %d\n", m_sendBuffer.GetRemainBufSize(), size );
		CallbackErrorHandler( 0, szMsg );
		//CallbackErrorHandler( 0, "[eXtreme::ServerSession::SendPacket] SendBuffer의 Enqueue실패" );
		return false;
	}

	Flush();
    
	return true;
}

bool ServerSession::SendPacket( Stream& stream )
{
	Guard < Mutex >	guard( m_mutexSend );

	if( m_sendBuffer.GetRemainBufSize() < stream.GetDataSize() )
	{
		char szMsg[256] = {0, };
		sprintf(szMsg, "[eXtreme::ServerSession::SendPacket::Stream] SendBuffer의 용량부족, Remain : %d, Packet : %d\n", m_sendBuffer.GetRemainBufSize(), stream.GetDataSize() );
		CallbackErrorHandler( 0, szMsg );
//		CallbackErrorHandler( 0, "[eXtreme::ServerSession::SendPacket::Stream] SendBuffer의 용량부족" );
		return false;
	}

//	Guard < Mutex >	guard( m_mutexSend );

	if( m_sendBuffer.Enqueue( stream.GetBuffer(), stream.GetDataSize() ) == false )
	{
		char szMsg[256] = {0, };
		sprintf(szMsg, "[eXtreme::ServerSession::SendPacket::Stream] SendBuffer의 Enqueue실패, Remain : %d, Packet : %d\n", m_sendBuffer.GetRemainBufSize(), stream.GetDataSize() );
		CallbackErrorHandler( 0, szMsg );
//		CallbackErrorHandler( 0, "[eXtreme::ServerSession::SendPacket::Stream] SendBuffer의 Enqueue실패\n" );
		return false;
	}

	Flush();

	return true;
}