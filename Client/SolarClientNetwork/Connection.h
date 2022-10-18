#pragma once

#include <winsock2.h>
//#include <RingBuffer.h>
#include <CriticalSection.h>
#include "SolarClientNetwork.h"		//< for function typedef
#include "./CircuitQueue.h"

enum eIOTYPE
{
	eIO_NONE			= 0x00000000,
	eIO_RECV			= 0x00000001,
	eIO_SEND			= 0x00000002,
};


class Connection;
class SolarClientNetwork;
template<typename T>
class CircuitQueue;

class OVERLAPPEDEX : public OVERLAPPED
{
public:
	eIOTYPE					IOType;
	Connection *			pConnection;
};

class Connection
{
	friend class				ProcessThread;
public:
	Connection();
	~Connection();

	VOID						Initialize( SolarClientNetwork * net );
	VOID						Release();

	VOID						Create( DWORD dwConnectionBufferSize, HANDLE hSendEvent, HANDLE hRecvEvent );
	BOOL						Connect( char * pszIP, WORD wPort, BYTE HeaderSize, BOOL bExcludeHeaderSize );

	BOOL						Update( fn_Recv fnRecv );

	BOOL						Send( VOID * pMsg, DWORD dwMsgSize );

	inline VOID					SetIndex( DWORD dwIndex ) { m_dwIndex = dwIndex;	}
	inline DWORD				GetIndex() { return m_dwIndex;	}
	
	
	VOID						Disconnect();
	BOOL						CloseSocket();

	inline SOCKET				GetSocket() { return m_socket;	}
	BOOL						PostRecv( DWORD dwRecvSize );
	BOOL						PostSend( DWORD dwCompletedSendSize );

	const char *				GetIP() { return m_IP; }
	// for test
	BOOL						IsDisconnected() { return m_bDisconnected;	}
	inline DWORD				GetArrayIndex() { return m_dwArrayIndex;	}
	inline VOID					SetArrayIndex( DWORD dwIndex ) { m_dwArrayIndex = dwIndex;	}
	
private:
#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    BYTE*						_getFirstPacketPtr(CircuitQueueByte* pBuffer);
#else
	BYTE *						_getFirstPacketPtr();
#endif
	BOOL						_preRecv();
	BOOL						_preSend();

	OVERLAPPEDEX				m_RecvOvlp;
	OVERLAPPEDEX				m_SendOvlp;
	util::CCriticalSection		m_recvCS;
	util::CCriticalSection		m_sendCS;
	CircuitQueueByte*		    m_pRecvBuffer;
	CircuitQueueByte*		    m_pSendBuffer;
	DWORD						m_dwIndex;
	SOCKET						m_socket;
	BOOL						m_bWaitSend;
	HANDLE						m_hSendOvlEvent;
	HANDLE						m_hRecvOvlEvent;
	BOOL						m_bDisconnected;
	BOOL						m_bDisconnecting;
	SolarClientNetwork *		m_pNet;
	static const int			_MAX_IPADDRESS_SIZE = 15;
	char						m_IP[_MAX_IPADDRESS_SIZE+1];
	BYTE						m_byHeaderSize;
	BOOL						m_bExcludeHeaderSize;
	// for test
	DWORD						m_dwArrayIndex;
	
};
