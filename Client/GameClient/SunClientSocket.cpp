#include "SunClientPrecompiledHeader.h"

#include "SunClientSocket.h"


//#include "InterfaceManager.h"
//
//BOOL CClientSocket::s_bStartUpDone = FALSE;
//
//
//CClientSocket::CClientSocket()
//{
//	m_dwTimer = 0;
//	m_dwSendByte =0;
//	m_dwRecvByte =0;
//	
//	m_socket=INVALID_SOCKET;
//	m_dwLastError=CSM_NOERROR;
//
//	m_SendBuf[0]=NULL;	
//	m_nCurrSend=0;			
//
//	_InitLog();
//	m_fn_SendParent=NULL;
//
//	m_pRecvBuffer = new CircuitQueue<BYTE>;
//	m_pRecvBuffer->Create( MAX_SOCK_RECVBUF, MAX_PACKET_SIZE );
//}
//CClientSocket::~CClientSocket()
//{
//	delete m_pRecvBuffer;
//
//	if(INVALID_SOCKET!= m_socket)
//	{
//		_GracefulClose();
//	}
//	WSACleanup();
//	_LogEnd();
//
//}
//BOOL CClientSocket::Create(CSFUNC_NOTIFY fnRecieve)
//{
//	m_fn_SendParent=fnRecieve;
//	return _StartUp();
//}
//BOOL CClientSocket::_StartUp()
//{
//	if ( s_bStartUpDone)
//	{
//		return ( TRUE);
//	}
//
//	WORD wVersionRequested;
//	WSADATA wsaData;
//	int err;
//
//	wVersionRequested = MAKEWORD(2,2);
//
//	err = WSAStartup( wVersionRequested, &wsaData);
//	if( err != 0)
//	{
//		_SetLastError(CSM_ERROR_INIT_WINSOCK_DLL);
//		return FALSE;
//	}
//
//	if ( LOBYTE( wsaData.wVersion ) != 2 ||  HIBYTE( wsaData.wVersion ) != 2 ) {
//		// winsock.dll 버전 낮음
//		WSACleanup( );
//		_SetLastError(CSM_ERROR_LOWVERSION_WINSOCK_DLL);
//		return FALSE;
//	}
//	SAFE_LOGPRINT0("WSAStartup 호출 성공\n");
//
//	s_bStartUpDone = TRUE;
//	return TRUE;
//}
//
//BOOL CClientSocket::_GracefulClose()
//{
//
//	//센드 막고
//    if (shutdown(m_socket, SD_SEND) == SOCKET_ERROR) {
//        return FALSE;
//    }
//
//	//소켓에있는 버퍼 지우기
//	int iMaxEndurance=60;
//    char acReadBuffer[MAX_PACKET_SIZE];
//    while (1) {
//        int nNewBytes = recv(m_socket, acReadBuffer, MAX_PACKET_SIZE, 0);
//        if (nNewBytes == SOCKET_ERROR) {
//			if(WSAGetLastError() == WSAEWOULDBLOCK && iMaxEndurance > 0) {
//				Sleep(100);
//				iMaxEndurance--;
//				continue;
//			}
//			//Graceful 실패 - 나중에 할것.
//			SAFE_LOGPRINT0("Graceful 주의!! - Endurance 초과\n");
//            break;
//        }
//        else if (nNewBytes == 0) {
//			break;// Zero indicates that the connection has been gracefully closed
//        }
//     }
//	//소켓닫기
//	if (!_CloseSocket()) return FALSE;
//	SAFE_LOGPRINT0("Disconnect 됨\n");
//
//	return TRUE;
//}
//BOOL CClientSocket::_CloseSocket()
//{
//	assert(m_socket!=INVALID_SOCKET);
//	if (closesocket(m_socket) == SOCKET_ERROR) {
//		_SetLastError(CSM_ERROR_CLOSE_SOCKET);
//        return FALSE;
//    }
//	m_socket=INVALID_SOCKET;
//	m_SendBuf[0]=NULL;	
//	m_nCurrSend=0;			
//	return TRUE;
//
//}
//
//BOOL CClientSocket::_CreateSocket()
//{
//	if(INVALID_SOCKET!=m_socket)
//	{//이전 연결이 있을 경우
//
//		SAFE_LOGPRINT0("CreateSocket 주의!! - Connect 상태에서 Disconnect되지않고 Connect 다시 호출()\n");
//		SAFE_LOGPRINT0("                    - Disconnect() 수행함\n");
//		Disconnect();
//	}
//	m_socket = socket( PF_INET, SOCK_STREAM, 0);
//
//	if( INVALID_SOCKET==m_socket   ) 
//	{
//		_SetLastError(CSM_ERROR_CREATE_SOKCET);
//		return FALSE;
//	}
//	SAFE_LOGPRINT0("CreateSocket 생성.\n");
//   	return TRUE;
//	
//}
//
//
//BOOL CClientSocket::Disconnect()
//{
//	m_pRecvBuffer->Clear();
//
//	if(INVALID_SOCKET==m_socket)
//	{
//
//		SAFE_LOGPRINT0("Disconnect 주의!! - Disconnect 호출전에 소켓이 생성되지 않음\n");
//		return TRUE;
//	}
//	BOOL bRet=_GracefulClose();
//	
//	return bRet;
//}
//void CClientSocket::_SetLastError(EN_CLIENT_SOCKET_MESSAGE ecse)
//{
//	if(m_dwLastError != CSM_NOERROR)
//	{
//		SAFE_LOGPRINT1("에러 %d가 이전에 체크되어있음\n",m_dwLastError);
//	}
//	switch(ecse)
//	{
//		
//		case CSM_ERROR_INIT_WINSOCK_DLL:
//			SAFE_LOGPRINT0("Winsock Dll 초기화 에러\n");
//			break;
//		case CSM_ERROR_LOWVERSION_WINSOCK_DLL:	
//			SAFE_LOGPRINT0("Winsock dll 번전이 낮음\n");
//			break;
//		case CSM_ERROR_CREATE_SOKCET:					
//			SAFE_LOGPRINT1("Create socket 실패 %d\n",WSAGetLastError());
//			break;
//		case CSM_ERROR_CLOSE_SOCKET:					
//			SAFE_LOGPRINT0("close socket  실패\n");
//			break;
//		case CSM_ERROR_CONNECT:						
//			SAFE_LOGPRINT1("Connet 함수 실패 %d\n",WSAGetLastError());
//			break;
//		case CSM_ERROR_INVALID_ADDRESS:				
//			SAFE_LOGPRINT0("잘못 된 주소\n");
//			break;
//		case CSM_ERROR_TRY_LOCAL_ADDRESS:				
//			SAFE_LOGPRINT0("error 로칼 주소로 접속 시도\n");
//			break;
//		case CSM_ERROR_WSA_EVENT_SELECT:		
//			SAFE_LOGPRINT0("WSAAsyncSelect 호출실패\n" );
//			break;
//		case CSM_ERROR_SEND:
//			SAFE_LOGPRINT0("Send Error \n");
//			break;
//		case CSM_ERROR_RECEIVE:
//			SAFE_LOGPRINT0("Receive Error \n");
//			break;
//		case CSM_ERROR_SEND_BUFF_OVERFLOW:
//			SAFE_LOGPRINT0("Error -Send Buffer Overflow \n");
//			break;
//		case CSM_ERROR_RECEIVE_INVALID_SIZE:
//			SAFE_LOGPRINT0("Error - 패킷사이즈가 0보다 작음\n");
//			break;
//		case CSM_ERROR_RECEIVE_BUFF_OVERFLOW:
//			SAFE_LOGPRINT0("Error - Receive Buffer Overflow \n");
//			break;
//		default:
//			SAFE_LOGPRINT0("잘못된 에러 코드\n" );
//			assert(!"잘못된 에러 코드\n");		
//			break;
//	}
//	int iError=WSAGetLastError();
//	_SafeNotify(ecse,(void*)&iError);
//	m_dwLastError=ecse;
//
//}
//
//BOOL CClientSocket::Connect(char *ip_addr, unsigned short port,HWND hWnd,UINT message,long lSelectEvent)
//{
//	
//	SAFE_LOGPRINT0("================Conncet========================\n");
//	if(!_CreateSocket())
//	{
//		return FALSE;
//	}
//	sockaddr_in		addr;
//	int nResult;
//	struct hostent    *host = NULL;
//
//    addr.sin_family			= PF_INET;
//    addr.sin_port			= htons( port );
//    addr.sin_addr.s_addr	= inet_addr(ip_addr); 
//
//	if( addr.sin_addr.s_addr == INADDR_NONE )
//	{
//		host = gethostbyname(ip_addr);
//		if( host == NULL )
//		{
//			
//			_SetLastError(CSM_ERROR_INVALID_ADDRESS);
//			_CloseSocket();
//			return FALSE;
//		}
//		CopyMemory(&addr.sin_addr, host->h_addr_list[0], host->h_length);
//	}
//   
//	if ( addr.sin_addr.S_un.S_un_b.s_b1 == 127 && addr.sin_addr.S_un.S_un_b.s_b2 == 0 &&
//		addr.sin_addr.S_un.S_un_b.s_b3 == 0 && addr.sin_addr.S_un.S_un_b.s_b4 == 1)
//	{	
//		_SetLastError(CSM_ERROR_TRY_LOCAL_ADDRESS);
//		_CloseSocket();
//		return ( FALSE);
//	}
//
//	nResult = connect( m_socket, (LPSOCKADDR)&addr, sizeof(addr) );
//	if( nResult == SOCKET_ERROR) {
//        int errorVal = WSAGetLastError();
//		if(WSAGetLastError() != WSAEWOULDBLOCK) {
//			_SetLastError(CSM_ERROR_CONNECT);
//			_CloseSocket();
//			return FALSE;
//		}
//    }
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// 이걸 왜 앞에 뒀던거? -_-+
//	if( WSAAsyncSelect( m_socket, hWnd, message, lSelectEvent) == SOCKET_ERROR) {
//		_SetLastError(CSM_ERROR_WSA_EVENT_SELECT);
//		_CloseSocket();
//		return FALSE;
//    }
//
//	m_dwInitialTick = GetTickCount();
//	m_lTotalSendSize = 0;
//
//	SAFE_LOGPRINT4("%d.%d.%d.%d 에 접속됨\n",addr.sin_addr.S_un.S_un_b.s_b1, addr.sin_addr.S_un.S_un_b.s_b2,
//		 addr.sin_addr.S_un.S_un_b.s_b3, addr.sin_addr.S_un.S_un_b.s_b4);
//
//	return TRUE;
//}
//
//int CClientSocket::Send( char *buf, int len)
//{	
//	SAFE_LOGPRINT0("================Send========================\n");
//
//	// 앞의 두 바이트에 사이즈 넣기
//	BYTE byBuffer[MAX_PACKET_SIZE];
//	memcpy( byBuffer, &len, 2);
//	memcpy( &byBuffer[2], buf, len);
//
//	int iLength = len + 2;
//	int nLeft = iLength;
//	int nDx=0;
//
//	if( INVALID_SOCKET == m_socket )
//	{
//		return FALSE;
//	}
////	assert(INVALID_SOCKET!=m_socket);
//
//    int nResult = 0;
//	while( TRUE)
//	{
//		nResult = send(m_socket, (char*)byBuffer+nDx, iLength-nDx, 0);
//		if( nResult == SOCKET_ERROR )
//		{
//			if( WSAGetLastError() != WSAEWOULDBLOCK )
//			{
//				_SetLastError(CSM_ERROR_SEND);
//				return FALSE;
//			}
//			else 
//			{
//				if( (m_nCurrSend+iLength) > MAX_SOCK_SENDBUF )
//				{
//					_SetLastError(CSM_ERROR_SEND_BUFF_OVERFLOW);
//					return FALSE;
//				}
//				memcpy( m_SendBuf+m_nCurrSend, byBuffer, nLeft);
//				m_nCurrSend += nLeft;
//				return FALSE;
//			}
//		}
//		else {
//			if( nResult == 0 ) {
//				break;
//			}
//		}
//		nDx += nResult;
//		nLeft -= nResult;
//		m_lTotalSendSize += nResult;
//		if( nLeft <= 0 ) break;
//	}
//
//	m_dwSendByte += nResult;
//
//	return TRUE;
//}
//
//
//BOOL CClientSocket::FDWriteSend()
//{
//	SAFE_LOGPRINT0("================FD_WRITE========================\n");
//	int nResult;
//	int nDx	  = 0;
//	
//	
//	while( m_nCurrSend > 0 ) 
//	{
//		nResult = send( m_socket,(char*)m_SendBuf+nDx, m_nCurrSend-nDx, 0);
//		if( nResult == SOCKET_ERROR )
//		{
//			if( WSAGetLastError() != WSAEWOULDBLOCK )
//			{
//				_SetLastError(CSM_ERROR_SEND);
//				return FALSE;
//			}
//			else 
//			{
//				break;
//			}
//		}
//		else {
//			if( nResult <= 0 ) {
//				_SetLastError(CSM_ERROR_SEND);
//				return FALSE;
//			}
//		}
//		nDx += nResult;
//		m_nCurrSend -= nResult;		
//		m_lTotalSendSize += nResult;
//	}
//
//	return TRUE;
//}
//BOOL CClientSocket::Receive()
//{
//	int nResult;
//	SAFE_LOGPRINT0("================Receive========================\n");
//	
//	// 버퍼에서 쓰기할 포인터를 얻는다.
//	BYTE *pWritePtr		= m_pRecvBuffer->GetWritePtr();
//	int nWritableLen	= m_pRecvBuffer->GetWritableLen();
//
//	nResult = recv( m_socket, (char*)pWritePtr, nWritableLen, 0 );
//
//	// 접속이 끊어졌음
//	if( nResult == 0 )
//	{
//#ifdef _DEBUG
//		//	서버와 접속이 끊겼습니다
//		WzColor wzColor = WzColor_RGBA( 255, 0, 0, 255 );
//		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
//		g_InterfaceManager.GetInterfaceString( eST_SERVER_DISCONNECT, szMessage, INTERFACE_STRING_LENGTH );
//		g_InterfaceManager.ShowMessageBox( szMessage, -1, &wzColor );
//#endif
//		return FALSE;
//	}
//
//	if( nResult == SOCKET_ERROR )
//	{
//		if( WSAGetLastError() == WSAEWOULDBLOCK )
//		{
//			return TRUE;
//		}
//		else
//		{
//			_SetLastError(CSM_ERROR_RECEIVE);
//
//		}
//		return FALSE;
//	}
//
//	// 받은만큼 큐의 길이를 증가시킨다.
//	m_pRecvBuffer->Enqueue( NULL, nResult );
//
//	m_dwRecvByte += nResult;
//
//	return TRUE;
//}
//
////////////////////////////////////////////////////////////////////////////
//// 완성된 패킷의 포인터를 얻어낸다.
////////////////////////////////////////////////////////////////////////////
//BYTE* CClientSocket::GetFirstPacketPtr()
//{
//	WORD wSize;
//
//	// 현재 버퍼에 있는 데이터 크기가 사이즈를 표현하는 2바이트만큼도 안되는 경우
//	if( m_pRecvBuffer->GetLength() < sizeof(wSize) )
//	{
//		return NULL;
//	}
//
//	// 사이즈를 표현하는 맨 앞 2바이트를 읽어만 본다.
//	m_pRecvBuffer->Peek( (BYTE*)&wSize, sizeof(wSize) );
//
//	// 패킷 사이즈가 이상한 경우
//	if( wSize > MAX_PACKET_SIZE )
//	{
//		_SetLastError(CSM_ERROR_RECEIVE_INVALID_SIZE);
//		return NULL;
//	}
//
//	// 현재까지 받아진 양이 완성된 패킷 크기가 아닐 경우
//	if( m_pRecvBuffer->GetLength() < (int)( wSize + sizeof(wSize) ) )
//	{
//		return NULL;		
//	}
//
//	// 하나의 패킷이 버퍼 뒤쪽에서 앞쪽으로 나누어질 때 여분의 버퍼에 잘리는 뒷부분을 복사한다.
//	if( m_pRecvBuffer->GetBackDataCount() < (int)( wSize + sizeof(wSize) ) )
//	{
//		m_pRecvBuffer->CopyHeadDataToExtraBuffer( wSize + sizeof(wSize) - m_pRecvBuffer->GetBackDataCount() );
//	}
//
//	return m_pRecvBuffer->GetReadPtr();
//}
//
//// 첫번째 패킷 제거
//VOID CClientSocket::RemoveFirstPacket( WORD wSize )
//{
//	// 첫번째 패킷 사이즈만큼 큐에서 제거
//	m_pRecvBuffer->Dequeue( NULL, wSize );
//}
//
//#ifdef CLIENT_SOCKET_LOG
////Log*****************************************************************************************
//void CClientSocket::_InitLog()
//{
//	m_fpLog = fopen("net.log", "wt");
//	assert(m_fpLog!=NULL);
//
//}
//void CClientSocket::_LogPrint(char *szlog, ...)
//{
//	char szBuffer[256]="";
//	va_list		pArguments;
//	
//	va_start(pArguments, szlog);
//    vSprintf(szBuffer, szlog, pArguments);
//    va_end(pArguments);
//	_SafeNotify(CSM_PRINT_LOG,szBuffer);
//}
////Log*****************************************************************************************
//
//#endif
//
//void CClientSocket::UpdateTimer( void )
//{
//	DWORD	dwTimer = timeGetTime();
//
//	if( (dwTimer - m_dwTimer) >= 1000 )
//	{
//		m_dwTimer = dwTimer;
//		//	패킷 사이즈 갱신
//		m_dwSendBytePerSec = m_dwSendByte;
//		m_dwRecvBytePerSec = m_dwRecvByte;
//		m_dwSendByte =0;
//		m_dwRecvByte =0;
//	}
//
//}

