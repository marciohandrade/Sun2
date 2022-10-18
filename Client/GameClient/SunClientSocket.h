
#pragma once


//#include <winsock2.h>
//#include <windows.h>
//#include <list>
//#include <assert.h>
//#include <CircuitQueue.h>
//
//
//
//#define MAX_SOCK_SENDBUF		( 10240)
//#define MAX_SOCK_RECVBUF		( 120000)
//#define MAX_PACKET_SIZE			( 102400 )
//
//
//#ifdef _DEBUG
////#define CLIENT_SOCKET_LOG
//#endif
//
//#ifdef CLIENT_SOCKET_LOG
//#define SAFE_LOGPRINT0(sz)						_LogPrint(sz)
//#define SAFE_LOGPRINT1(sz,p1)					_LogPrint(sz,p1)
//#define SAFE_LOGPRINT2(sz,p1,p2)				_LogPrint(sz,p1,p2)
//#define SAFE_LOGPRINT3(sz,p1,p2,p3)				_LogPrint(sz,p1,p2,p3)
//#define SAFE_LOGPRINT4(sz,p1,p2,p3,p4)			_LogPrint(sz,p1,p2,p3,p4)
//#else
//#define SAFE_LOGPRINT0(sz)						
//#define SAFE_LOGPRINT1(sz,p1)					
//#define SAFE_LOGPRINT2(sz,p1,p2)				
//#define SAFE_LOGPRINT3(sz,p1,p2,p3)				
//#define SAFE_LOGPRINT4(sz,p1,p2,p3,p4)			
//#endif
//
//
//using namespace std;
//typedef list<BYTE*> STL_BTLIST;
//typedef void (*CSFUNC_NOTIFY)(DWORD,void* param);
//
//enum EN_CLIENT_SOCKET_MESSAGE
//{
//	//에러 관련
//	CSM_NOERROR,							//에러 없음
//	CSM_ERROR_INIT_WINSOCK_DLL,				//WINSOCK.dll 초기화 에러
//	CSM_ERROR_LOWVERSION_WINSOCK_DLL,		//낮은 버전 WINSOCK.dll
//	CSM_ERROR_CREATE_SOKCET,				//소켓생성 실패
//	CSM_ERROR_CLOSE_SOCKET,					//소켓 닫기 실패
//	CSM_ERROR_CONNECT,						//connet() 함수 호출 실패
//	CSM_ERROR_SEND,							//send()호출 실패
//	CSM_ERROR_SEND_BUFF_OVERFLOW,			//send 버퍼초과
//	CSM_ERROR_RECEIVE,						//receive()호출 실패
//	CSM_ERROR_RECEIVE_BUFF_OVERFLOW,		//receive 버퍼 초과
//	CSM_ERROR_RECEIVE_INVALID_SIZE,			//사이즈 크기가 0 보다 작다
//	CSM_ERROR_INVALID_ADDRESS,				//잘못된 주소 커넥트 시도.
//	CSM_ERROR_TRY_LOCAL_ADDRESS,			//로칼 주소로 접속 시도
//	CSM_ERROR_WSA_EVENT_SELECT,				//WSAAsyncSelect 함수 호출 실패.
//	CSM_ERROR_LOG_PRINT_NOT_DEBUGMODE,		//릴리즈모드 로그가 출력됨.
//
//	//기타
//	CSM_PRINT_LOG,							//로그 출력을 알린다.
//};
//
//
//class CClientSocket
//{
//private:
//	DWORD		m_dwTimer;
//	DWORD		m_dwSendByte;
//	DWORD		m_dwRecvByte;
//	DWORD		m_dwSendBytePerSec;
//	DWORD		m_dwRecvBytePerSec;
//
//public : 
//	void		UpdateTimer( void );
//
//
//	DWORD		GetSendBytePerSec( void ) { return m_dwSendBytePerSec; }
//	DWORD		GetRecvBytePerSec( void ) { return m_dwRecvBytePerSec; }
//
//
//
//
//public:
//	CClientSocket();
//	~CClientSocket();
//	virtual BOOL Connect(char *ip_addr, unsigned short port,HWND hWnd,UINT message,long lSelectEvent=FD_READ|FD_WRITE|FD_CLOSE|FD_CONNECT);
//	virtual BOOL Create(CSFUNC_NOTIFY fnRecieve=NULL);
//	virtual BOOL Disconnect();
//	virtual BOOL Send( char *buf, int len);
//	virtual BOOL Receive();
//	virtual SOCKET GetSocket(){return m_socket;}
//	virtual BYTE* GetFirstPacketPtr();
//	virtual VOID RemoveFirstPacket( WORD wSize );
//	virtual BOOL FDWriteSend();
//	virtual BOOL IsConneted(){return m_socket!=INVALID_SOCKET;}
//	virtual DWORD GetLastError(){return m_dwLastError;}
//
//	int		GetBackDataCount() {  return m_pRecvBuffer->GetLength(); }
//protected:
//	//멤버 변수
//	SOCKET					m_socket;
//
//	CircuitQueue<BYTE>		*m_pRecvBuffer;
//
//	BYTE					m_SendBuf[MAX_SOCK_SENDBUF];	
//	int						m_nCurrSend;			
//
//	DWORD					m_dwLastError;
//	CSFUNC_NOTIFY			m_fn_SendParent;
//
//	static BOOL s_bStartUpDone;
//
//	//멤버 함수 (protected)
//	BOOL					_StartUp();
//	BOOL					_GracefulClose();
//	BOOL					_CreateSocket();
//	BOOL					_CloseSocket();
//	void					_SetLastError(EN_CLIENT_SOCKET_MESSAGE ecse);
//	void					_SafeNotify(DWORD dw,void* param){if(m_fn_SendParent)m_fn_SendParent(dw,param);}
//
//
//	// 패킷량 계산
//protected:
//	DWORD m_dwInitialTick;
//	long m_lTotalSendSize;
//public:
//	//로그 관련
//
//#ifdef CLIENT_SOCKET_LOG
//
//
//protected:
//	FILE*				m_fpLog;
//	void _InitLog();
//	void _LogPrint( char *szlog, ...);
//	void _LogEnd(){if(m_fpLog)fclose(m_fpLog);}
//
//#else
//protected:
//	void _InitLog(){}
//	// CLIENT_SOCKET_LOG 정의 안되어 있는 경우 직접 호출 금지
//	void _LogPrint( char *szlog, ...){ _SafeNotify(CSM_ERROR_LOG_PRINT_NOT_DEBUGMODE,NULL); throw CSM_ERROR_LOG_PRINT_NOT_DEBUGMODE;}
//	void _LogEnd(){}
//#endif
//
//};
