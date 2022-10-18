#include "ClientSocket.h"	//  Windows.h, WinSock2.h ������ �����ϱ� ���� ���� include�Ѵ�.
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
	//  ������ ClientSocket.h�� ���ܽ�Ű�� ���ؼ��� cpp���� ClientSocket��
	//  ����ؾ� �Ѵ�. ������ member�δ� LPVOID�� �����ϰ� ClientSocket��
	//  ��ü�� ������ �� �����͸� ����Ű�� �ϴ� ���̴�.
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
			���ϼ����� ������ �õ��Ѵ�.
	@param	ipAddress
			������ ���� ������ �ּҸ� �Է��Ѵ�.
			�ּ��� ������ ���ڷ� ������ IP �ּҿ� ���ڷ� ������ dns �ּ� �� ���� ��� ���ȴ�.
			( "10.0.0.1" or "mail.webzen.co.kr" )
	@param	userName
			���� ������ ���� ������ �ʿ�� �ϴ� ��찡 �ִ�. �� ���� userName�� password ��
			�Ķ���͸� ��� ä���־�� �Ѵ�. �⺻������ NULL�� �Էµȴ�.
			�� ���� ���� �����κ��� �߱޹��� �����̴�.
	@param	password
			�⺻������ NULL�� �Էµȴ�.
			������ �Ҵ�� �н����带 �Է��Ѵ�.
	@param	portNo
			������ ���� ������ ��Ʈ ��ȣ.
			��κ��� ��� 23�� ��Ʈ�� ����ϸ� �� ���� �⺻������ �Էµȴ�.
	@ret
			���ῡ �����ϸ� TRUE�� ��ȯ�Ѵ�.
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
			�������� ������ �����Ѵ�.
	@ret
			������ ���������� ����Ǹ� TRUE�� ��ȯ�Ѵ�.
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
			���� ������ �߽����� ������ ������.
	@param  fromNick
			�߽����� E-Mail �ּҿ� ���� �̸� Ȥ�� ��Ī�� �Է¹޴´�.
			�ִ� ���̴� 63 Bytes�̴�.
	@param  fromEMail
			�߽����� E-Mail �ּҸ� �Է¹޴´�.
			�ִ� ���̴� 63 Bytes�̴�.
	@ret
			�۾��� ���������� �̷������ TRUE�� ��ȯ�Ѵ�.
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
			���� ������ �������� ����( email ���� )�� ������.
	@param  to
			�������� E-Mail �ּҸ� �Է¹޴´�.
			�����ڸ� ������ �����ϰ��� �� ������, ������ E-Mail �ּҸ� �ٲٸ鼭
			�� �Լ��� ��� ȣ�����ָ� �ȴ�.
	@ret
			�۾��� ���������� �̷������ TRUE�� ��ȯ�Ѵ�.
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
			������ ������ ������ ������.
	@param  subject
			������ ������ �Է¹޴´�.
	@param  body
			text�� ������ ������ ������ �Է¹޴´�. body�� ũ��� ������ ������, �����ϸ�
			�۾��� ȿ������ ����Ͽ� unsigned short������ ������ ���� �����Ѵ�.
	@ret
			�۾��� ���������� �̷������ TRUE�� ��ȯ�Ѵ�.
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

	//  �߽��� ����
	{
		::sprintf( buffer, "From: %s<%s>\r\n", m_fromNick, m_fromEMail );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );

		::sprintf( buffer, "X-Mailer:SSUL SendMail v1.0 [Korea]\r\n" );
		clientSocket->Send( buffer, ( int )::strlen( buffer ) );
	}


	//  ������ ����
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


	//  ���� ���� ����
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

	//  ���� ���� ����
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
			���Ͽ� ÷�������� �����Ѵ�.
			�������� ������ ÷���ϱ� ���ؼ��� �� �Լ��� ���ϴ� ��ŭ ȣ������ �� �ִ�.
	@param  fileName
			÷���� ���ϸ��� �Է��Ѵ�.
			���ϸ��� ������ Ȥ�� ����� ��θ� ����ϸ�, ��ΰ� ���� ���ϸ� �Է��� �� �ִ�.
	@ret
			�۾��� ���������� �̷������ TRUE�� ��ȯ�Ѵ�.
	@note
			���� ÷�θ� ��û�ϱ� ���ؼ��� �ռ� ������ ���� �ۼ��� ������ �Ѵ�.
			�� �Լ����� �ռ� MailContents�Լ��� ���������� �Ϸ�Ǿ������� Ȯ���� �� �ֵ��� �Ѵ�.
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

		//  Ȯ���ڿ��� dot�� �����Ѵ�.
		for( i = 1 ; ext[i] != 0 ; i++ )
		{
			ext[i - 1]	= ext[i];
			ext[i]		= 0;
		}

		contentType = mime->GetContentType( ext );


		//  ������ ��ο� ���ϸ��� �и��Ͽ� �����Ѵ�.
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
			���� �ۼ��� ������ �߽��϶�� ����� ���ϼ����� ��û�Ѵ�.
			�߽��� ���������� �̷�������� ���� �������� ������ ���� �ʴ´�. ���� ���Ḧ
			���Ѵٸ� Disconnect �Լ��� ���� ȣ���ؾ� �Ѵ�.
	@ret
			�۾��� ���������� �̷������ TRUE�� ��ȯ�Ѵ�.
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