#include "ClientSocket.h"	//  Windows.h, WinSock2.h 순서를 유지하기 위해 먼저 include한다.
#include "SmtpClient.h"
#include "Base64.h"
#include "Mime.h"

#include <stdio.h>

#define  DEF_CLIENTSOCKET( name )	ClientSocket*	name = ( ClientSocket* )m_instanceSocket
#define  DEF_MIME( name )			Mime*	name = ( Mime* )m_mime;

#define  CHECK_CONNECTION	0
#define  CHECK_HELLO		1

#define  CHECK_LOGIN		2
#define  CHECK_USERNAME		3
#define  CHECK_PASSWORD		4

#define  CHECK_MAIL			5
#define  CHECK_RCPT			6
#define  CHECK_DATA_START	7
#define  CHECK_DATA_END		8
#define  CHECK_QUIT			9
#define  CHECK_DATA			10

SMTPClient::SMTPClient()
{
	//  배포시 ClientSocket.h를 열외시키기 위해서는 cpp에서 ClientSocket을
	//  사용해야 한다. 때문에 member로는 LPVOID로 선언하고 ClientSocket의
	//  개체를 생성한 후 포인터를 가리키게 하는 것이다.
	m_instanceSocket	= ( LPVOID )new ClientSocket();
	m_mime				= ( LPVOID )new Mime();
}

SMTPClient::~SMTPClient()
{
	DEF_CLIENTSOCKET( clientSocket );

	clientSocket->Close();

	delete m_instanceSocket;
	delete m_mime;
}

BOOL SMTPClient::CheckResponse( BYTE type )
{
	DEF_CLIENTSOCKET( clientSocket );
	char	buffer[512], temp[3];
	int		typeNo;
	int		codeList[11] = { 220, 250, 334, 334, 235, 250, 250, 354, 250, 221, 354 };

	ZeroMemory( buffer, 512 );

	clientSocket->Recv( buffer, sizeof( buffer ) );
	
	::strncpy( temp, buffer, 3 );
	typeNo = atoi( temp );

	if( typeNo != codeList[type] )
		return FALSE;

	return TRUE;
}

//=================================================================================================
/**
	@remarks
			메일서버에 연결을 시도한다.
	@param	ipAddress
			접속할 메일 서버의 주소를 입력한다.
			주소의 형식은 숫자로 구성된 IP 주소와 문자로 구성된 dns 주소 두 가지 모두 허용된다.
			( "10.0.0.1" or "mail.webzen.co.kr" )
	@param	userName
			메일 서버에 따라 인증을 필요로 하는 경우가 있다. 이 때는 userName과 password 두
			파라미터를 모두 채워주어야 한다. 기본값으로 NULL이 입력된다.
			이 값은 메일 서버로부터 발급받은 계정이다.
	@param	password
			기본값으로 NULL이 입력된다.
			계정에 할당된 패스워드를 입력한다.
	@param	portNo
			접속할 메일 서버의 포트 번호.
			대부분의 경우 23번 포트를 사용하며 이 값이 기본값으로 입력된다.
	@ret
			연결에 성공하면 TRUE를 반환한다.
*/
//=================================================================================================
BOOL SMTPClient::Connect( LPCTSTR ipAddress, LPCTSTR userName, LPCTSTR password, u_short portNo )
{
	DEF_CLIENTSOCKET( clientSocket );
	TCHAR			hostName[127];
	TCHAR			buffer[512];

	::gethostname( hostName, 127 );
	if( clientSocket->Connect( ipAddress, portNo ) == false )
		return FALSE;

	if( CheckResponse( CHECK_CONNECTION ) == FALSE )
		return FALSE;

	m_receiverCount = 0;

	if( userName != 0 && password != 0 )
	{
		::sprintf( buffer, "EHLO %s\r\n", hostName );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );

		if( CheckResponse( CHECK_HELLO ) == FALSE )
			return FALSE;

		if( MailLogin( userName, password ) == FALSE )
			return FALSE;
	}
	else
	{
		::sprintf( buffer, "HELO %s\r\n", hostName );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );

		if( CheckResponse( CHECK_HELLO ) == FALSE )
			return FALSE;
	}

	return TRUE;
}

//=================================================================================================
/**
	@remarks
			서버와의 연결을 종료한다.
	@ret
			연결이 정상적으로 종료되면 TRUE를 반환한다.
*/
//=================================================================================================
BOOL SMTPClient::Disconnect()
{
	DEF_CLIENTSOCKET( clientSocket );
	TCHAR	buffer[127];

	::sprintf( buffer, "QUIT\r\n" );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );

	return ( CheckResponse( CHECK_QUIT ) == TRUE );
}

BOOL SMTPClient::MailLogin( LPCTSTR userName, LPCTSTR password )
{
	DEF_CLIENTSOCKET( clientSocket );
	TCHAR	encodeUserName[256];
	TCHAR	encodePassword[256];
	TCHAR	buffer[127];

	Base64Encode( userName, ( int )::strlen( userName ), encodeUserName, 256 );
	Base64Encode( password, ( int )::strlen( password ), encodePassword, 256 );

	::sprintf( buffer, "AUTH LOGIN\r\n" );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	if( CheckResponse( CHECK_LOGIN ) == FALSE )
		return FALSE;


	::sprintf( buffer, "%s\r\n", encodeUserName );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	if( CheckResponse( CHECK_USERNAME ) == FALSE )
		return FALSE;


	::sprintf( buffer, "%s\r\n", encodePassword );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	if( CheckResponse( CHECK_PASSWORD ) == FALSE )
		return FALSE;

	return TRUE;
}

//=================================================================================================
/**
	@remarks
			메일 서버로 발신자의 정보를 보낸다.
	@param  fromNick
			발신자의 E-Mail 주소에 대한 이름 혹은 별칭을 입력받는다.
			최대 길이는 63 Bytes이다.
	@param  fromEMail
			발신자의 E-Mail 주소를 입력받는다.
			최대 길이는 63 Bytes이다.
	@ret
			작업이 정상적으로 이루어지면 TRUE를 반환한다.
*/
//=================================================================================================
BOOL SMTPClient::MailFrom( LPCTSTR fromNick, LPCTSTR fromEMail )
{
	DEF_CLIENTSOCKET( clientSocket );
	TCHAR	buffer[127];

	::strcpy( m_fromNick, fromNick );
	::strcpy( m_fromEMail, fromEMail );

	::sprintf( buffer, "MAIL FROM:%s\r\n", m_fromEMail );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );

	return ( CheckResponse( CHECK_MAIL ) == TRUE );
}

//=================================================================================================
/**
	@remarks
			메일 서버로 수신자의 정보( email 계정 )을 보낸다.
	@param  to
			수신자의 E-Mail 주소를 입력받는다.
			수신자를 여러명 지정하고자 할 때에는, 수신자 E-Mail 주소를 바꾸면서
			이 함수를 계속 호출해주면 된다.
	@ret
			작업이 정상적으로 이루어지면 TRUE를 반환한다.
*/
//=================================================================================================
BOOL SMTPClient::MailTo( LPCTSTR to )
{
	DEF_CLIENTSOCKET( clientSocket );
	TCHAR	buffer[127];

	::sprintf( buffer, "RCPT TO:<%s>\r\n", to );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );

	if( CheckResponse( CHECK_RCPT ) == FALSE )
		return FALSE;

	::strcpy( m_listReceiver[m_receiverCount], to );
	m_receiverCount++;

	return TRUE;
}

//=================================================================================================
/**
	@remarks
			메일의 내용을 서버로 보낸다.
	@param  subject
			메일의 제목을 입력받는다.
	@param  body
			text로 구성된 메일의 본문을 입력받는다. body의 크기는 제한이 없지만, 가능하면
			작업의 효율성을 고려하여 unsigned short까지로 제한할 것을 권유한다.
	@ret
			작업이 정상적으로 이루어지면 TRUE를 반환한다.
*/
//=================================================================================================
BOOL SMTPClient::MailContents( LPCTSTR subject, LPCTSTR body )
{
	DEF_CLIENTSOCKET( clientSocket );
	DEF_MIME( mime );
	TCHAR	buffer[127];

	::sprintf( buffer, "DATA\r\n" );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	if( CheckResponse( CHECK_DATA ) == FALSE )
		return FALSE;

	//  발신자 정보
	{
		::sprintf( buffer, "From: %s<%s>\r\n", m_fromNick, m_fromEMail );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );

		::sprintf( buffer, "X-Mailer:SSUL SendMail v1.0 [Korea]\r\n" );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	}


	//  수신자 정보
	{
		int		i;

		::sprintf( buffer, "To: <" );
		for( i = 0 ; i < m_receiverCount ; i++ )
		{
			if( i > 0 )
				::strcat( buffer, ";" );

			::strcat( buffer, m_listReceiver[i] );
		}
		::strcat( buffer, ">\r\n" );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	}


	//  메일 제목 전송
	{
		::sprintf( buffer, "Subject: %s\r\n", subject );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	}

	::sprintf( buffer, "Mime-Version: 1.0\r\n" );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );


	::sprintf( buffer, "Content-Type: multipart/mixed; boundary=\"%s\"\r\n\r\n", mime->GetDefaultBoundary() );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );


	::sprintf( buffer, "--%s\r\n", mime->GetDefaultBoundary() );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );

	//  메일 본문 전송
	{
		LPTSTR		bodyText;
		int			bodySize, encodeSize;

		bodySize	= ( int )::strlen( body );
		encodeSize	= ( int )( ( float )bodySize * 1.5f );
		bodyText = new TCHAR [ encodeSize ];

		::sprintf( buffer, "Content-Type: text/plain; charset=euc_kr\r\n" );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );

		::sprintf( buffer, "Content-Transfer-Encoding: Base64\r\n\r\n" );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );


		Base64Encode( body, bodySize, bodyText, encodeSize );
		encodeSize = ( int )::strlen( bodyText );
		clientSocket->Send( bodyText, encodeSize );

		delete bodyText;
	}

	return TRUE;
}

//=================================================================================================
/**
	@remarks
			메일에 첨부파일을 삽입한다.
			여러개의 파일을 첨부하기 위해서는 이 함수를 원하는 만큼 호출해줄 수 있다.
	@param  fileName
			첨부할 파일명을 입력한다.
			파일명은 절대경로 혹은 상대경로 모두를 허용하며, 경로가 없는 파일명만 입력할 수 있다.
	@ret
			작업이 정상적으로 이루어지면 TRUE를 반환한다.
	@note
			파일 첨부를 요청하기 위해서는 앞서 메일의 본문 작성이 끝나야 한다.
			이 함수보다 앞서 MailContents함수가 정상적으로 완료되었는지를 확인할 수 있도록 한다.
*/
//=================================================================================================
BOOL SMTPClient::MailAttachFile( LPTSTR fileName )
{
	DEF_CLIENTSOCKET( clientSocket );
	DEF_MIME( mime );
	TCHAR		buffer[127], filePath[2048];
	LPTSTR		contentType;

	//  Split File extend
	{
		TCHAR	drive[127], dir[127];
		TCHAR	name[127], ext[24];
		int		i;

		::_splitpath( fileName, drive, dir, name, ext );

		//  확장자에서 dot을 제거한다.
		for( i = 1 ; ext[i] != 0 ; i++ )
		{
			ext[i - 1]	= ext[i];
			ext[i]		= 0;
		}

		contentType = mime->GetContentType( ext );


		//  파일의 경로와 파일명을 분리하여 저장한다.
		if( drive[0] == 0 && dir[0] == 0 )		::sprintf( filePath, "." );
		if( drive[0] == 0 && dir[0] != 0 )		::sprintf( filePath, ".\\%s", dir );
		if( drive[0] != 0 && dir[0] != 0 )		::sprintf( filePath, "%s\\%s", drive, dir );

		::sprintf( fileName, "%s.%s", name, ext );
	}

	//  Start boundary
	::sprintf( buffer, "\r\n--%s\r\n", mime->GetDefaultBoundary() );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );


	::sprintf( buffer, "Content-Type: %s; file=\"%s\"\r\n", contentType, fileName );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );


	::sprintf( buffer, "Content-Transfer-Encoding: Base64\r\n" );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );


	::sprintf( buffer, "Content-Disposition: attachment; filename=\"%s\"\r\n", fileName );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );


	::sprintf( buffer, "Content-Description: Created from SSUL mail engine\r\n\r\n" );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );


	//  Encoding File to Base64
	{
		HANDLE		handleFile;
		TCHAR		readBuffer[8192], encodeBuffer[8192];
		TCHAR		fileFullName[2048];
		DWORD		readBytes;
		int			size;

		::sprintf( fileFullName, "%s\\%s", filePath, fileName );

		handleFile = CreateFile( fileFullName, GENERIC_READ, FILE_SHARE_READ,
												0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );
		if( handleFile == INVALID_HANDLE_VALUE )
			return FALSE;

		do
		{
			if( ReadFile( handleFile, readBuffer, 3072, &readBytes, 0 ) == FALSE )
				break;

			Base64Encode( readBuffer, readBytes, encodeBuffer, 8192 );
			size = ( int )::strlen( encodeBuffer );

			if( readBytes > 0 )
				clientSocket->Send( encodeBuffer, size );
		} while( readBytes > 0 );

		CloseHandle( handleFile );
	}

	return TRUE;
}

//=================================================================================================
/**
	@remarks
			메일 작성을 끝내고 발신하라는 명령을 메일서버로 요청한다.
			발신이 정상적으로 이루어지더라도 메일 서버와의 연결은 끊지 않는다. 연결 종료를
			원한다면 Disconnect 함수를 직접 호출해야 한다.
	@ret
			작업이 정상적으로 이루어지면 TRUE를 반환한다.
*/
//=================================================================================================
BOOL SMTPClient::MailSubmit()
{
	DEF_CLIENTSOCKET( clientSocket );
	DEF_MIME( mime );
	TCHAR	buffer[127];


	::sprintf( buffer, "\r\n--%s--\r\n", mime->GetDefaultBoundary() );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );


	::sprintf( buffer, "\r\n.\r\n" );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	if( CheckResponse( CHECK_DATA_END ) == FALSE )
		return FALSE;


	::sprintf( buffer, "QUIT\r\n" );
	clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	if( CheckResponse( CHECK_QUIT ) == FALSE )
		return FALSE;

	return TRUE;
}