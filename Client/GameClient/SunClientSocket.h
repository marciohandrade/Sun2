
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
//	//���� ����
//	CSM_NOERROR,							//���� ����
//	CSM_ERROR_INIT_WINSOCK_DLL,				//WINSOCK.dll �ʱ�ȭ ����
//	CSM_ERROR_LOWVERSION_WINSOCK_DLL,		//���� ���� WINSOCK.dll
//	CSM_ERROR_CREATE_SOKCET,				//���ϻ��� ����
//	CSM_ERROR_CLOSE_SOCKET,					//���� �ݱ� ����
//	CSM_ERROR_CONNECT,						//connet() �Լ� ȣ�� ����
//	CSM_ERROR_SEND,							//send()ȣ�� ����
//	CSM_ERROR_SEND_BUFF_OVERFLOW,			//send �����ʰ�
//	CSM_ERROR_RECEIVE,						//receive()ȣ�� ����
//	CSM_ERROR_RECEIVE_BUFF_OVERFLOW,		//receive ���� �ʰ�
//	CSM_ERROR_RECEIVE_INVALID_SIZE,			//������ ũ�Ⱑ 0 ���� �۴�
//	CSM_ERROR_INVALID_ADDRESS,				//�߸��� �ּ� Ŀ��Ʈ �õ�.
//	CSM_ERROR_TRY_LOCAL_ADDRESS,			//��Į �ּҷ� ���� �õ�
//	CSM_ERROR_WSA_EVENT_SELECT,				//WSAAsyncSelect �Լ� ȣ�� ����.
//	CSM_ERROR_LOG_PRINT_NOT_DEBUGMODE,		//�������� �αװ� ��µ�.
//
//	//��Ÿ
//	CSM_PRINT_LOG,							//�α� ����� �˸���.
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
//	//��� ����
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
//	//��� �Լ� (protected)
//	BOOL					_StartUp();
//	BOOL					_GracefulClose();
//	BOOL					_CreateSocket();
//	BOOL					_CloseSocket();
//	void					_SetLastError(EN_CLIENT_SOCKET_MESSAGE ecse);
//	void					_SafeNotify(DWORD dw,void* param){if(m_fn_SendParent)m_fn_SendParent(dw,param);}
//
//
//	// ��Ŷ�� ���
//protected:
//	DWORD m_dwInitialTick;
//	long m_lTotalSendSize;
//public:
//	//�α� ����
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
//	// CLIENT_SOCKET_LOG ���� �ȵǾ� �ִ� ��� ���� ȣ�� ����
//	void _LogPrint( char *szlog, ...){ _SafeNotify(CSM_ERROR_LOG_PRINT_NOT_DEBUGMODE,NULL); throw CSM_ERROR_LOG_PRINT_NOT_DEBUGMODE;}
//	void _LogEnd(){}
//#endif
//
//};
