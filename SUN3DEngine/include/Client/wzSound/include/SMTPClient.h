//////////////////////////////////////////////////////////////////////////////
//  SMTPClient / Send Mail Transfer Protocol implementation
//  Version 1.0
//
//  NOTE : Each API is synchronized with reaction of connected mail server
//         You must linking 'ws2_32.lib'
//----------------------------------------------------------------------------
//  Built in Server Engine part, Research & Development, Webzen

#ifndef  __SMTPClient_H
#define  __SMTPClient_H

#define  SMTP_DEFAULT_PORT		25
#define  SMTP_MAXRECEIVERCOUNT	64

#include <Windows.h>
#include <TChar.h>
//=================================================================================================
///  SMTP를 사용하여 메일서버로 직접 접속후 메일을 전송한다.
/**
	@author
			Server Engine part, Research & Development, Webzen
	@since
			2005. 2. 7
	@remarks
			지정된 메일 서버로 접속하여 메일을 전송한다.
			동시에 n명에게 보낼 수 있으며, 첨부파일을 등록할 수 있다.

			- SMTPClient.h를 include하기 전에 앞서 Windows.h를 include해야 한다.
	@note
			이 클래스의 모든 메서드는 동기식 네트워크 통신을 한다. 그러기 때문에 네트워크의
			상황, 요청한 작업의 크기 등에 따라 호출된 메서드의 반환이 느려질 수 있다.
*/
//=================================================================================================
class SMTPClient
{
public:
			SMTPClient();
			~SMTPClient();

			BOOL		Connect( LPCTSTR ipAddress, LPCTSTR userName = 0,
									LPCTSTR password = 0, unsigned short portNo = SMTP_DEFAULT_PORT );
			BOOL		Disconnect();

			BOOL		MailFrom( LPCTSTR fromNick, LPCTSTR fromEMail );
			BOOL		MailTo( LPCTSTR to );
			BOOL		MailContents( LPCTSTR subject, LPCTSTR body );
			BOOL		MailAttachFile( LPTSTR fileName );
			BOOL		MailSubmit();

private:
			BOOL		MailLogin( LPCTSTR userName, LPCTSTR password );
			BOOL		CheckResponse( BYTE type );


			LPVOID		m_instanceSocket;
			LPVOID		m_mime;
			TCHAR		m_fromEMail[64], m_fromNick[64];


			TCHAR		m_listReceiver[SMTP_MAXRECEIVERCOUNT][64];
			int			m_receiverCount;
};

#endif