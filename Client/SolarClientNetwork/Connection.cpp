#include "StdAfx.h"
#include ".\connection.h"
#include ".\SolarClientNetwork.h"
#include ".\CircuitQueue.h"
#include "./SunNetworkApi.h"

#include <CriticalSection.h>
using util::CSyncCriticalSection;

#define		INIT_INDEX_VALUE		99

Connection::Connection()
:	m_byHeaderSize ( SolarClientNetwork::DEFAULT_HEADER_SIZE )
,	m_bExcludeHeaderSize ( TRUE )
{
	SetIndex( INIT_INDEX_VALUE );
}

Connection::~Connection()
{	
	if( m_pRecvBuffer )
	{
		delete m_pRecvBuffer;
		m_pRecvBuffer = NULL;
	}

	if( m_pSendBuffer )
	{
		delete m_pSendBuffer;
		m_pSendBuffer = NULL;
	}

}

VOID Connection::Create( DWORD dwConnectionBufferSize, HANDLE hSendEvent, HANDLE hRecvEvent )
{
//	_setArrayIndex( dwArrayIndex );
	//----------------------------------------------------------------------------------
	// Send/Recv ���� ����
	//----------------------------------------------------------------------------------
	m_hSendOvlEvent				= hSendEvent;
	m_hRecvOvlEvent				= hRecvEvent;
	
	ZeroMemory( &m_SendOvlp, sizeof(m_SendOvlp) );
	ZeroMemory( &m_RecvOvlp, sizeof(m_RecvOvlp) );
	m_SendOvlp.pConnection		= this;
	m_RecvOvlp.pConnection		= this;
	m_SendOvlp.IOType			= eIO_SEND;
	m_RecvOvlp.IOType			= eIO_RECV;
	m_SendOvlp.hEvent			= m_hSendOvlEvent;
	m_RecvOvlp.hEvent			= m_hRecvOvlEvent;

	m_pRecvBuffer				= new CircuitQueueByte;
	m_pSendBuffer				= new CircuitQueueByte;
	m_pRecvBuffer->Create( dwConnectionBufferSize, MAX_PACKET_SIZE );
#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    m_pSendBuffer->Create(dwConnectionBufferSize >> 1, MAX_PACKET_SIZE);
#else
	m_pSendBuffer->Create( dwConnectionBufferSize, MAX_PACKET_SIZE );
#endif
}

VOID Connection::Initialize( SolarClientNetwork * pNet )
{
	m_bDisconnected				= FALSE;
	m_bDisconnecting			= FALSE;
	m_bWaitSend					= TRUE;
	m_pRecvBuffer->Clear();
	m_pSendBuffer->Clear();
	m_pNet						= pNet;
}

VOID Connection::Release()
{
	Disconnect();
	SetIndex( INIT_INDEX_VALUE );
}

VOID Connection::Disconnect()
{
	CloseSocket();
	m_bDisconnected	= TRUE;
	m_bDisconnecting = FALSE;
}

BOOL Connection::CloseSocket()
{
	if( m_socket == INVALID_SOCKET || m_bDisconnecting )
		 return FALSE;
	if( closesocket( m_socket ) == SOCKET_ERROR ) 
	{
		m_pNet->_setLastError( "closesocket error(%d)\n", WSAGetLastError() );
		return FALSE;
    }
	m_socket		= INVALID_SOCKET;
	m_bDisconnecting = TRUE;

	return TRUE;
}

BOOL Connection::Connect(  char * pszIP, WORD wPort, BYTE HeaderSize, BOOL bExcludeHeaderSize )
{
	m_socket = WSASocket( AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED );
	if( INVALID_SOCKET == m_socket )	
	{
		m_pNet->_setLastError( "���� ���� ���� (%d)\n", WSAGetLastError() );
		return FALSE;
	}

	sockaddr_in		addr;
	int nResult;
	struct hostent    *host = NULL;

    addr.sin_family			= PF_INET;
    addr.sin_port			= htons( wPort );
    addr.sin_addr.s_addr	= inet_addr( pszIP ); 

	if( addr.sin_addr.s_addr == INADDR_NONE )
	{
		host = gethostbyname( pszIP );
		if( host == NULL )
		{
			m_pNet->_setLastError( "�߸��� �ּ�(%s)�̴�. (%d)\n", pszIP, WSAGetLastError() );
			Disconnect();
			return FALSE;
		}
		CopyMemory( &addr.sin_addr, host->h_addr_list[0], host->h_length );
	}
  
	/*
	if ( addr.sin_addr.S_un.S_un_b.s_b1 == 127 && addr.sin_addr.S_un.S_un_b.s_b2 == 0 &&
		addr.sin_addr.S_un.S_un_b.s_b3 == 0 && addr.sin_addr.S_un.S_un_b.s_b4 == 1)
	{	
		m_pNet->_setLastError( "�����ּ��̴� �ּ�(%s)�̴�. %d", pszIP );
		Disconnect();
		return FALSE;
	}
	*/

	nResult = sun_network_api::sun_connect( m_socket, (LPSOCKADDR)&addr, sizeof(addr) );
	if( nResult == SOCKET_ERROR ) 
	{
		DWORD dwError = WSAGetLastError();
		if( dwError != WSAEWOULDBLOCK ) 
		{
			m_pNet->_setLastError( "���ּ�(%s)�� ���� �� �� �����ϴ�[%d].\n", pszIP, dwError );
			Disconnect();
			return FALSE;
		}
	}

	if( !_preRecv() ) return FALSE;

	strncpy( m_IP, pszIP, _MAX_IPADDRESS_SIZE );
	m_IP[_MAX_IPADDRESS_SIZE] = '\0';
	m_byHeaderSize = HeaderSize;
	m_bExcludeHeaderSize = bExcludeHeaderSize;
	return TRUE;
}

#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
BYTE*
Connection::_getFirstPacketPtr(CircuitQueueByte* pBuffer)
{
    DWORD header = 0;
    //WORD  header;

    // ������ ������ ���̰� ��� ���̺��� ������ NULL ����
    if(!pBuffer->Peek((BYTE*)&header, m_byHeaderSize))
        return NULL;

    int packetTokenSize = m_byHeaderSize + header;
    if(false == m_bExcludeHeaderSize)
        packetTokenSize -= m_byHeaderSize;

    // ������ ������ ���̰� ��Ŷ ���̺��� ������ NULL ����(��Ŷ �ϼ� �ȵ�)
    if(pBuffer->GetLength() < packetTokenSize)
        return NULL;

    // ��Ŷ�� ���� ���ʿ��� ������ �������� �� �߸� �޺κ��� ������ ������ ī���Ѵ�.
    if(pBuffer->GetBackDataCount() < packetTokenSize)
    {
        pBuffer->CopyHeadDataToExtraBuffer(packetTokenSize - pBuffer->GetBackDataCount()); 
    }

    return pBuffer->GetReadPtr();
}

#else //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__

BYTE*
Connection::_getFirstPacketPtr()
{
	DWORD header = 0;
	//WORD  header;

	// ������ ������ ���̰� ��� ���̺��� ������ NULL ����
	if( !m_pRecvBuffer->Peek( (BYTE*)&header, m_byHeaderSize ) )				return NULL;

	int PacketTotalSize = m_byHeaderSize + header;
	if( false == m_bExcludeHeaderSize ) PacketTotalSize -= m_byHeaderSize;

	// ������ ������ ���̰� ��Ŷ ���̺��� ������ NULL ����(��Ŷ �ϼ� �ȵ�)
	if( m_pRecvBuffer->GetLength() < PacketTotalSize )	return NULL;

	// ��Ŷ�� ���� ���ʿ��� ������ �������� �� �߸� �޺κ��� ������ ������ ī���Ѵ�.
	if( m_pRecvBuffer->GetBackDataCount() < PacketTotalSize )
	{
		m_pRecvBuffer->CopyHeadDataToExtraBuffer( PacketTotalSize - m_pRecvBuffer->GetBackDataCount() ); 
	}

	return  m_pRecvBuffer->GetReadPtr();

}
#endif //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__


BOOL Connection::Update( fn_Recv fnRecv )
{
#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    {   // sending operation
        CSyncCriticalSection cs(m_sendCS);
        int sendingSize = m_pSendBuffer->GetSpace();
        if(sendingSize)
            _preSend();
    };

    CircuitQueueByte* pDoubleBuffer = 0;
    {
        void* ptr = m_pNet->GetDoubleBuffer();
        if(ptr == 0)
            return FALSE;
        pDoubleBuffer = reinterpret_cast<CircuitQueueByte*>(ptr);
    };

    int remainedRatio = 0;
    // CopyLocalToSecondBuffer - ���� ������ ��Ŷ ó���� �����ϴ� ���� Recv Thread�� ������ ����
    //�ϰ� �־���. ���캸��... CircuitQueue�� ����ȭ ó���� ���̰� ���� ���¿���. �װ� �����ϸ�
    //���� �κ��� ������ �����ؾ� �ϴ����� �������� �ʴ� �ѵ����� �����Ѵ�.
    // received event (offset change)�� ������ ���ŵ� ��Ŷ�� ó���� �� ��Ȯ�� ���� �����Ƿ�, 
    //Post Event�� ó���Ѵ�.
    {
        CSyncCriticalSection cs(m_recvCS);
        m_pRecvBuffer->CopyLocalToSecondBuffer(pDoubleBuffer, &remainedRatio);
    };

    const int Percent = 1;
    const DWORD parsingIndex = GetIndex();
    BOOLEAN verified = TRUE, globalLocked = FALSE;
    DWORD header = 0, packetTotalSize = 0;

    // globalLock - ���� �ؼ� ���� ������� ������ Recv Thread���� ��� �޾ƹ��� ���,
    //�� �̻� �޴� �̺�Ʈ ó���� ������ �� ���� ���濡 �̸��� ���� �����ϱ� ����.
    if(remainedRatio < 20 * Percent)
    {
        m_recvCS.Lock();
        globalLocked = TRUE;
    }

    while(BYTE* pPacket = _getFirstPacketPtr(pDoubleBuffer))
    {
        CopyMemory(&header, pPacket, m_byHeaderSize);

        int packetTokenSize = m_byHeaderSize + header;
        if(false == m_bExcludeHeaderSize)
            packetTokenSize -= m_byHeaderSize;

        if(packetTokenSize > MAX_PACKET_SIZE)
        {
            verified = FALSE;
            break;
        }

        //sprintf(buf, "[%010d] size=%d,   bufferSize=%d\n", GetTickCount(), packetSize, m_pRecvBuffer->GetLength());
        //OutputDebugString(buf);
        fnRecv(parsingIndex, pPacket + m_byHeaderSize, packetTokenSize - m_byHeaderSize);
        pDoubleBuffer->Dequeue(NULL, packetTokenSize);
        packetTotalSize += packetTokenSize;
    }

    if(packetTotalSize)
    {   // (WAVERIX) (090415) (NOTE) ������ �������� Ring Buffer �� Multi-threaded programming�̶�
        // ���� ���� �κе��� �����Ѵ�. �ϴ� ���� ���� ���ư��� ���·θ� ó���Ѵ�.
        if(globalLocked == FALSE)
        {
            m_recvCS.Lock();
            globalLocked = TRUE;
        }
        m_pRecvBuffer->Dequeue(NULL, packetTotalSize);
    };

    if(globalLocked)
        m_recvCS.Unlock();

    //verified �� ó���� ������ ���ΰ��� ����غ���. �߸��� ó���� �߻��������� �� ��, ������
    //���� ���� ����...
    if(verified == FALSE && m_bDisconnected == FALSE)
        Disconnect();

    // ��������?
    if(m_bDisconnected)
        return FALSE;

    return TRUE;


#else //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
	// ������Ŷ �ݹ�
	CSyncCriticalSection cs( m_recvCS );

	//char buf[256];

	BYTE * pPacket = NULL;
	while( pPacket = _getFirstPacketPtr() )
	{

		DWORD header = 0;
		memcpy( &header, pPacket, m_byHeaderSize );
		//WORD packetSize = *(WORD*)pPacket;

		int PacketTotalSize = m_byHeaderSize + header;
		if( false == m_bExcludeHeaderSize ) PacketTotalSize -= m_byHeaderSize;

		if( PacketTotalSize > MAX_PACKET_SIZE )
		{
			return FALSE;
		}

		//sprintf(buf, "[%010d]               size=%d,   bufferSize=%d\n", GetTickCount(), packetSize, m_pRecvBuffer->GetLength() );
		//OutputDebugString(buf);

		fnRecv( GetIndex(), pPacket + m_byHeaderSize, PacketTotalSize-m_byHeaderSize );

		m_pRecvBuffer->Dequeue( NULL, PacketTotalSize );
	}

	// ��������?
	if( m_bDisconnected ) return FALSE;

	return TRUE;
#endif //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
}

BOOL Connection::PostRecv( DWORD dwRecvSize )
{
	CSyncCriticalSection cs( m_recvCS );

	if( !m_pRecvBuffer->Enqueue( NULL, dwRecvSize ) )
	{
		m_pNet->_setLastError( "�߸��� Put()�Լ� ȣ��\n" );
		return FALSE;
	}

	if( !_preRecv() )	return FALSE;

	return TRUE;
}


BOOL Connection::_preRecv()
{
	if( 0 == m_pRecvBuffer->GetWritableLen() )
		return FALSE;

	DWORD dwTransBytes	= 0;
	DWORD dwFlags		= 0;
	WSABUF wsabuf;
	wsabuf.buf			= (char *)m_pRecvBuffer->GetWritePtr();
	wsabuf.len			= m_pRecvBuffer->GetWritableLen();
	SASSERT( wsabuf.len > 0 , "���� ����� 0�̴�." );

	//char buf[256];
	//sprintf(buf, "[%010d]_preRecv(%d)\n", GetTickCount(), wsabuf.len);
	//OutputDebugString(buf);

    //sun_network_api::NetworkDebugFile debug_file("WSARecv.txt", wsabuf.len);

	int rt = sun_network_api::sun_WSARecv( m_socket, &wsabuf, 1, &dwTransBytes, &dwFlags, &m_RecvOvlp, NULL );
	if( rt == SOCKET_ERROR && GetLastError() != ERROR_IO_PENDING )
	{
		// _setLastErrror();
		return FALSE;
	}

	return TRUE;
}

BOOL Connection::PostSend( DWORD dwCompletedSendSize )
{
	CSyncCriticalSection cs( m_sendCS );

	if( !m_pSendBuffer->Dequeue( NULL, dwCompletedSendSize ) )
	{
		m_pNet->_setLastError( "�߸��� Get()�Լ� ȣ��\n" );
		return FALSE;
	}

	m_bWaitSend = TRUE;

#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    // empty
#else //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
	if( 0 == m_pSendBuffer->GetLength() ) return TRUE;

	if( !_preSend() )	return FALSE;
#endif

	return TRUE;
}

BOOL Connection::_preSend()
{
	if( FALSE == m_bWaitSend || m_pSendBuffer->GetLength() == 0 ) return TRUE;
	m_bWaitSend = FALSE;

	DWORD dwTransBytes	= 9999999;
	WSABUF wsabuf;
	wsabuf.buf			= (char *)m_pSendBuffer->GetReadPtr();
	wsabuf.len			= m_pSendBuffer->GetReadableLen();

	//char buf[256];
	//sprintf(buf, "[%010d]_preSend(%d)\n", GetTickCount(), wsabuf.len);
	//OutputDebugString(buf);	

    //sun_network_api::NetworkDebugFile debug_file("WSASend.txt", wsabuf.len);

	int rt = sun_network_api::sun_WSASend( m_socket, &wsabuf, 1, &dwTransBytes, NULL, &m_SendOvlp, NULL );
	if( rt == SOCKET_ERROR && GetLastError() != ERROR_IO_PENDING )
	{
		// _setLastErrror();
		return FALSE;
	}

	return TRUE;
}

BOOL Connection::Send( VOID * pMsg, DWORD dwMsgSize )
{
	ASSERT( GetIndex() != INIT_INDEX_VALUE );
	if( m_bDisconnecting ) return FALSE;
	if( m_bDisconnected ) return FALSE;

#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    // ���
    DWORD realPacketSize = dwMsgSize;
    if(false == m_bExcludeHeaderSize)
        realPacketSize += m_byHeaderSize;

    CSyncCriticalSection cs( m_sendCS );
    if(!m_pSendBuffer->Enqueue((BYTE*)&realPacketSize, m_byHeaderSize))
    {
        m_pNet->_setLastError("���۰� ���ƽ��ϴ�.\n");
        return FALSE;
    }

    // �ٵ�
    if(!m_pSendBuffer->Enqueue((BYTE*)pMsg, dwMsgSize))
    {
        m_pNet->_setLastError("���۰� ���ƽ��ϴ�.\n");
        return FALSE;
    }

    // no _preSend...
    return TRUE;

#else //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__

	CSyncCriticalSection cs( m_sendCS );

	// ���
	DWORD RealPacketSize = dwMsgSize;
	if( false == m_bExcludeHeaderSize ) RealPacketSize += m_byHeaderSize;

	if( !m_pSendBuffer->Enqueue( (BYTE *)(DWORD*)&RealPacketSize, m_byHeaderSize ) )
	{
		m_pNet->_setLastError( "���۰� ���ƽ��ϴ�.\n" );
		return FALSE;
	}

	// �ٵ�
	if( !m_pSendBuffer->Enqueue( (BYTE *)pMsg, dwMsgSize ) )
	{
		m_pNet->_setLastError( "���۰� ���ƽ��ϴ�.\n" );
		return FALSE;
	}
	
	if( !_preSend() )	return FALSE;
#endif //!__NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__


	return TRUE;
}
