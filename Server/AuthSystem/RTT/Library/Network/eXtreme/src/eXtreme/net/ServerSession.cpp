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

//  �� �Լ��� ServerSession�� ��ӹ��� Ŭ������ �����ڿ��� ����� �� ����.
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
		ERROR ó���� Send �Լ����� �Ͽ����Ƿ� ���⼭�� ��� �� ���� ������ ������ Ȯ���Ѵ�.
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

	//  ������ ������ ������ �����Ѵ�.
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
			ERROR ó���� Send �Լ����� �Ͽ����Ƿ� ���⼭�� ��� �� ���� ������ ������ Ȯ���Ѵ�.
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

			//  ����, �Ϸ�� ũ�⸸ŭ Send buffer���� �����Ѵ�.
			//  �񵿱� I/O�� ����߱� ������ �Ϸᰡ �߻��� �� ���� ������ ���۴� �����Ǿ�� �Ѵ�.
			m_sendBuffer.Dequeue( 0, result->transBytes );

			//  ��û�� I/O �۾��� ũ�⸸ŭ �Ϸᰡ �߻��ؾ� ���� Send�۾��� ������ �� �ִ�.
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
					0byte receive�� WSAGetLastError�� ȣ�� ���� �ʴ´�.
					EXT_ERROR_ZERO_BYTE_RECEIVE ���Ǹ� �Ͽ� ���� �ڵ带 �����Ѵ�.
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

			// WSARecv ���� ���� �����͸�ŭ ����� ���� ������
			if( m_recvBuffer.Enqueue( result->szData, result->transBytes ) == false )
//			if( m_recvBuffer.Enqueue( 0, result->transBytes ) == false )
			{
				assert( false );
			}

			//  �Ʒ� WaitForRecv�� ȣ���ϴ� ���� result�� ������ ���� �� �����Ƿ�
			//  �ʿ��ϴٸ� �� ������ ����صξ�� �Ѵ�.

// 			if( m_recvBuffer.GetWritableSize() > 0 )
// 			{
// 				//  ���� ���ۿ� ���� �뷮�� ���ٸ� ���� ���۸� ������Ѵ�.
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
				DWORD	totalSize = 0;		//  ����� ������ ��Ŷ �ϳ��� ��ü ũ��

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
				//  ���� ���ۿ� ���� �뷮�� ���ٸ� ���� ���۸� ������Ѵ�.
				WaitForRecv();
				requestedRecv = true;	// i think it does not necessary...:p
			}
			else
			{
				CallbackErrorHandler( 0, "[ServerSession::HandleEvent] ���Ź��� ����" );
			}

			//  ���� ��û�� ���� �ʾҴٸ�, ���� ���۰� ����� �� ��û�Ѵ�.
// 			if( requestedRecv == false )
// 			{
// 				if( m_recvBuffer.GetWritableSize() == 0 )
// 				{
// 					//  �׷��� ���� ������ ���ٸ� ������ �����Ѵ�.
// 					CallbackErrorHandler( 0, "[eXtreme::ServerSession::HandleEvent] ���Ź��� ����" );
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
		sprintf(szMsg, "[eXtreme::ServerSession::SendPacket::buffer] SendBuffer�� �뷮����, Remain : %d, Packet : %d\n", m_sendBuffer.GetRemainBufSize(), size );
		CallbackErrorHandler( 0, szMsg );
		return false;
	}

//	Guard < Mutex >	guard( m_mutexSend );

	if( m_sendBuffer.Enqueue( ( char* )buffer, size ) == false )
	{
		char szMsg[256] = {0, };
		sprintf(szMsg, "[eXtreme::ServerSession::SendPacket::buffer] SendBuffer�� Enqueue����, Remain : %d, Packet : %d\n", m_sendBuffer.GetRemainBufSize(), size );
		CallbackErrorHandler( 0, szMsg );
		//CallbackErrorHandler( 0, "[eXtreme::ServerSession::SendPacket] SendBuffer�� Enqueue����" );
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
		sprintf(szMsg, "[eXtreme::ServerSession::SendPacket::Stream] SendBuffer�� �뷮����, Remain : %d, Packet : %d\n", m_sendBuffer.GetRemainBufSize(), stream.GetDataSize() );
		CallbackErrorHandler( 0, szMsg );
//		CallbackErrorHandler( 0, "[eXtreme::ServerSession::SendPacket::Stream] SendBuffer�� �뷮����" );
		return false;
	}

//	Guard < Mutex >	guard( m_mutexSend );

	if( m_sendBuffer.Enqueue( stream.GetBuffer(), stream.GetDataSize() ) == false )
	{
		char szMsg[256] = {0, };
		sprintf(szMsg, "[eXtreme::ServerSession::SendPacket::Stream] SendBuffer�� Enqueue����, Remain : %d, Packet : %d\n", m_sendBuffer.GetRemainBufSize(), stream.GetDataSize() );
		CallbackErrorHandler( 0, szMsg );
//		CallbackErrorHandler( 0, "[eXtreme::ServerSession::SendPacket::Stream] SendBuffer�� Enqueue����\n" );
		return false;
	}

	Flush();

	return true;
}