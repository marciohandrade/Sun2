#ifndef __SOLOAR_CLIENT_NETWORK_H__
#define __SOLOAR_CLIENT_NETWORK_H__

//=======================================================================================================================
/// 클라이언트 소켓 네트워크 모듈 : SolarClientNetwork class
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2005. 2.18
            2009.04.15 - waverix (double buffering for safely connection)
	@remarks
			- Overlapped I/O방식의 client 소켓 모듈, Windows 95 ( winsocket2 부터 지원 )
			- Send / Recvg 버퍼링 처리
	@caution
			- SOLAR_CLIENT_DESC의 nMaxConnectionNum의 개수는 MAXIMUM_WAIT_OBJECTS/2를 넘을 수 없다. ( 즉, 최대치 32 : WaitForMultipleObjects()의 한계 )
	@example

	//=======================================================================================================================		
	VOID _fn_Recv( DWORD dwIndex, BYTE * pData, DWORD dwSize )
	{
		// 수신 패킷 처리
	}

	VOID _fn_Disconnect( DWORD dwIndex )
	{
		// 접속 종료 처리
	}

	VOID _fn_Error( char * pszMsg )
	{
		// 에러 메시지 처리
	}

	void main()
	{
		SolarClientNetwork network;

		struct AA
		{
			int index;
		} packet;

		SOLAR_CLIENT_DESC desc;
		desc.nMaxBufferSizePerConn	= 10000;
		desc.nMaxConnectionNum		= 1;
		desc.fnCallBackRecv			= _fn_Recv;
		desc.fnCallBackError		= _fn_Error;
		desc.fnCallBackDisconnect	= _fn_Disconnect;
		network.InitializeNetwork( desc );
		DWORD index = 3;
		if( index != network.Connect(index, "10.1.28.xxx", 5000 ) )
		{
			// 연결 에러 처리
		}

		while(1)
		{
			//... game logic

			if( !network.UpdateNetwork() )
				break;							//< network.CloseNetwork()를 호출 했을 때 종료

			//... game logic
			network.SendPacket( index, &packet, sizeof(packet) );
		}

		network.ReleaseNework();

	}
	//=======================================================================================================================

*/
//=======================================================================================================================

#pragma once

//#ifdef SOLARCLIENTNETWORK_EXPORTS
//#define SOLARCLIENTNETWORK_API __declspec(dllexport)
//#else
//#define SOLARCLIENTNETWORK_API __declspec(dllimport)
//#endif

// 09.04.15 / 이은파 / 패킷 수신과 패킷 처리상의 병목 해결
// - side-effect (연결 불안정한 문제 해결)
#define __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__


#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    #define MAX_PACKET_SIZE					(31 * 1024)
#else
    #define MAX_PACKET_SIZE					614400
    #include <SolarHashTable.h>
#endif
#define MAX_COMPULSIVE_DICONNECT_TIME		15000


typedef VOID (*fn_Recv)( DWORD dwIndex, BYTE * pData, DWORD dwSize );
typedef VOID (*fn_Disconnect)( DWORD dwIndex );
typedef VOID (*fn_Error)( char * pszMsg );


struct SOLAR_CLIENT_DESC
{
	DWORD			nMaxConnectionNum;
	DWORD			nMaxBufferSizePerConn;
	fn_Recv			fnCallBackRecv;
	fn_Disconnect	fnCallBackDisconnect;
	fn_Error		fnCallBackError;
};

class Connection;
class ProcessThread;
class ConnectionAllocator;

class /*SOLARCLIENTNETWORK_API*/ SolarClientNetwork
{
	static const int	DEFAULT_HEADER_SIZE = 2;
	static const int	MAX_HEADER_SIZE		= 4;
	enum { MAX_CONNECTION_OBJECTS = MAXIMUM_WAIT_OBJECTS/2, };
	friend class		ProcessThread;
	friend class		Connection;
public:
	SolarClientNetwork();
	~SolarClientNetwork();

	VOID				InitializeNetwork( SOLAR_CLIENT_DESC & desc );
	BOOL				UpdateNetwork();
	VOID				ReleaseNetwork();
	VOID				CloseNetwork();

	BOOL				IsDisconnected( DWORD dwIndex );
	BOOL				Disconnect( DWORD dwIndex );
	BOOL				Connect( DWORD dwIndex, char * pszIP, WORD wPort, BYTE HeaderSize = DEFAULT_HEADER_SIZE, BOOL bExcludeHeaderSize = TRUE);
	BOOL				SendPacket( DWORD dwIndex, VOID * pMsg, DWORD dwMsgSize );

	const char *		GetConnectionIP( DWORD dwIndex );
	DWORD				GetMaxConnectionNum();
	Connection *		FindConnection( DWORD dwIndex );
#ifdef __NA000000_090415_DOUBLE_BUFFERING_FOR_SAFELY_CONNECTION__
    void*               GetDoubleBuffer() const;
#endif
private:
	VOID				_setLastError( const char * pszErrorMsg, ... );
	struct INTERAL_INFO;
	INTERAL_INFO *		in;
};


#endif // __SOLOAR_CLIENT_NETWORK_H__