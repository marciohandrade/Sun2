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
//		// winsock.dll ���� ����
//		WSACleanup( );
//		_SetLastError(CSM_ERROR_LOWVERSION_WINSOCK_DLL);
//		return FALSE;
//	}
//	SAFE_LOGPRINT0("WSAStartup ȣ�� ����\n");
//
//	s_bStartUpDone = TRUE;
//	return TRUE;
//}
//
//BOOL CClientSocket::_GracefulClose()
//{
//
//	//���� ����
//    if (shutdown(m_socket, SD_SEND) == SOCKET_ERROR) {
//        return FALSE;
//    }
//
//	//���Ͽ��ִ� ���� �����
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
//			//Graceful ���� - ���߿� �Ұ�.
//			SAFE_LOGPRINT0("Graceful ����!! - Endurance �ʰ�\n");
//            break;
//        }
//        else if (nNewBytes == 0) {
//			break;// Zero indicates that the connection has been gracefully closed
//        }
//     }
//	//���ϴݱ�
//	if (!_CloseSocket()) return FALSE;
//	SAFE_LOGPRINT0("Disconnect ��\n");
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
//	{//���� ������ ���� ���
//
//		SAFE_LOGPRINT0("CreateSocket ����!! - Connect ���¿��� Disconnect�����ʰ� Connect �ٽ� ȣ��()\n");
//		SAFE_LOGPRINT0("                    - Disconnect() ������\n");
//		Disconnect();
//	}
//	m_socket = socket( PF_INET, SOCK_STREAM, 0);
//
//	if( INVALID_SOCKET==m_socket   ) 
//	{
//		_SetLastError(CSM_ERROR_CREATE_SOKCET);
//		return FALSE;
//	}
//	SAFE_LOGPRINT0("CreateSocket ����.\n");
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
//		SAFE_LOGPRINT0("Disconnect ����!! - Disconnect ȣ������ ������ �������� ����\n");
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
//		SAFE_LOGPRINT1("���� %d�� ������ üũ�Ǿ�����\n",m_dwLastError);
//	}
//	switch(ecse)
//	{
//		
//		case CSM_ERROR_INIT_WINSOCK_DLL:
//			SAFE_LOGPRINT0("Winsock Dll �ʱ�ȭ ����\n");
//			break;
//		case CSM_ERROR_LOWVERSION_WINSOCK_DLL:	
//			SAFE_LOGPRINT0("Winsock dll ������ ����\n");
//			break;
//		case CSM_ERROR_CREATE_SOKCET:					
//			SAFE_LOGPRINT1("Create socket ���� %d\n",WSAGetLastError());
//			break;
//		case CSM_ERROR_CLOSE_SOCKET:					
//			SAFE_LOGPRINT0("close socket  ����\n");
//			break;
//		case CSM_ERROR_CONNECT:						
//			SAFE_LOGPRINT1("Connet �Լ� ���� %d\n",WSAGetLastError());
//			break;
//		case CSM_ERROR_INVALID_ADDRESS:				
//			SAFE_LOGPRINT0("�߸� �� �ּ�\n");
//			break;
//		case CSM_ERROR_TRY_LOCAL_ADDRESS:				
//			SAFE_LOGPRINT0("error ��Į �ּҷ� ���� �õ�\n");
//			break;
//		case CSM_ERROR_WSA_EVENT_SELECT:		
//			SAFE_LOGPRINT0("WSAAsyncSelect ȣ�����\n" );
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
//			SAFE_LOGPRINT0("Error - ��Ŷ����� 0���� ����\n");
//			break;
//		case CSM_ERROR_RECEIVE_BUFF_OVERFLOW:
//			SAFE_LOGPRINT0("Error - Receive Buffer Overflow \n");
//			break;
//		default:
//			SAFE_LOGPRINT0("�߸��� ���� �ڵ�\n" );
//			assert(!"�߸��� ���� �ڵ�\n");		
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
//	// �̰� �� �տ� �״���? -_-+
//	if( WSAAsyncSelect( m_socket, hWnd, message, lSelectEvent) == SOCKET_ERROR) {
//		_SetLastError(CSM_ERROR_WSA_EVENT_SELECT);
//		_CloseSocket();
//		return FALSE;
//    }
//
//	m_dwInitialTick = GetTickCount();
//	m_lTotalSendSize = 0;
//
//	SAFE_LOGPRINT4("%d.%d.%d.%d �� ���ӵ�\n",addr.sin_addr.S_un.S_un_b.s_b1, addr.sin_addr.S_un.S_un_b.s_b2,
//		 addr.sin_addr.S_un.S_un_b.s_b3, addr.sin_addr.S_un.S_un_b.s_b4);
//
//	return TRUE;
//}
//
//int CClientSocket::Send( char *buf, int len)
//{	
//	SAFE_LOGPRINT0("================Send========================\n");
//
//	// ���� �� ����Ʈ�� ������ �ֱ�
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
//	// ���ۿ��� ������ �����͸� ��´�.
//	BYTE *pWritePtr		= m_pRecvBuffer->GetWritePtr();
//	int nWritableLen	= m_pRecvBuffer->GetWritableLen();
//
//	nResult = recv( m_socket, (char*)pWritePtr, nWritableLen, 0 );
//
//	// ������ ��������
//	if( nResult == 0 )
//	{
//#ifdef _DEBUG
//		//	������ ������ ������ϴ�
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
//	// ������ŭ ť�� ���̸� ������Ų��.
//	m_pRecvBuffer->Enqueue( NULL, nResult );
//
//	m_dwRecvByte += nResult;
//
//	return TRUE;
//}
//
////////////////////////////////////////////////////////////////////////////
//// �ϼ��� ��Ŷ�� �����͸� ����.
////////////////////////////////////////////////////////////////////////////
//BYTE* CClientSocket::GetFirstPacketPtr()
//{
//	WORD wSize;
//
//	// ���� ���ۿ� �ִ� ������ ũ�Ⱑ ����� ǥ���ϴ� 2����Ʈ��ŭ�� �ȵǴ� ���
//	if( m_pRecvBuffer->GetLength() < sizeof(wSize) )
//	{
//		return NULL;
//	}
//
//	// ����� ǥ���ϴ� �� �� 2����Ʈ�� �о ����.
//	m_pRecvBuffer->Peek( (BYTE*)&wSize, sizeof(wSize) );
//
//	// ��Ŷ ����� �̻��� ���
//	if( wSize > MAX_PACKET_SIZE )
//	{
//		_SetLastError(CSM_ERROR_RECEIVE_INVALID_SIZE);
//		return NULL;
//	}
//
//	// ������� �޾��� ���� �ϼ��� ��Ŷ ũ�Ⱑ �ƴ� ���
//	if( m_pRecvBuffer->GetLength() < (int)( wSize + sizeof(wSize) ) )
//	{
//		return NULL;		
//	}
//
//	// �ϳ��� ��Ŷ�� ���� ���ʿ��� �������� �������� �� ������ ���ۿ� �߸��� �޺κ��� �����Ѵ�.
//	if( m_pRecvBuffer->GetBackDataCount() < (int)( wSize + sizeof(wSize) ) )
//	{
//		m_pRecvBuffer->CopyHeadDataToExtraBuffer( wSize + sizeof(wSize) - m_pRecvBuffer->GetBackDataCount() );
//	}
//
//	return m_pRecvBuffer->GetReadPtr();
//}
//
//// ù��° ��Ŷ ����
//VOID CClientSocket::RemoveFirstPacket( WORD wSize )
//{
//	// ù��° ��Ŷ �����ŭ ť���� ����
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
//		//	��Ŷ ������ ����
//		m_dwSendBytePerSec = m_dwSendByte;
//		m_dwRecvBytePerSec = m_dwRecvByte;
//		m_dwSendByte =0;
//		m_dwRecvByte =0;
//	}
//
//}

