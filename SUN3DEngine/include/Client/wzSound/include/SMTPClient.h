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
///  SMTP�� ����Ͽ� ���ϼ����� ���� ������ ������ �����Ѵ�.
/**
	@author
			Server Engine part, Research & Development, Webzen
	@since
			2005. 2. 7
	@remarks
			������ ���� ������ �����Ͽ� ������ �����Ѵ�.
			���ÿ� n���� ���� �� ������, ÷�������� ����� �� �ִ�.

			- SMTPClient.h�� include�ϱ� ���� �ռ� Windows.h�� include�ؾ� �Ѵ�.
	@note
			�� Ŭ������ ��� �޼���� ����� ��Ʈ��ũ ����� �Ѵ�. �׷��� ������ ��Ʈ��ũ��
			��Ȳ, ��û�� �۾��� ũ�� � ���� ȣ��� �޼����� ��ȯ�� ������ �� �ִ�.
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