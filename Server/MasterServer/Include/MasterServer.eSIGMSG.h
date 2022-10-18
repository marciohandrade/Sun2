#pragma once

#include <Timer.h>

class ServerSessionEx;
class TempServerSessionEx;
struct MSG_SERVERCOMMON_SECURE_LOG_NTF;

struct eSIGMSG
{
	static const eSIG_TYPE		EXITPROCESS = (eSIG_TYPE)(SIGMSG_NORMAL+100);
	static const eSIG_TYPE		TIMERPROCESS = (eSIG_TYPE)(SIGMSG_NORMAL+101);
	static const eSIG_TYPE		SERVERAUTHENTICATION = (eSIG_TYPE)(SIGMSG_NORMAL+102);
	static const eSIG_TYPE		ABUSINGUSERNOTIFY = (eSIG_TYPE)(SIGMSG_NORMAL+103);
};

struct sSIG_SECURE_LOG_NOTIFY
{
	MSG_SERVERCOMMON_SECURE_LOG_NTF*	m_pRecvNTF;
	SERVER_KEY							m_ServerKey;
	TCHAR*								m_pMessage;
};

struct sSIG_EXITED_SESSION : sSIG_EXTEND
{
	sSIG_EXITED_SESSION()
	{
		_TYPE = eSIGMSG::EXITPROCESS;
		m_pServerSession = NULL;
	}

	inline ServerSessionEx*&	Session() { return m_pServerSession; }

	// <FIELDs>
	ServerSessionEx*  m_pServerSession;
};

struct sSIG_SERVER_AUTHENTICATION : sSIG_EXTEND
{
	enum eSUBTYPE { UNKWNOWN = 0, REGISTERATION = 1, UNREGISTRATION = 2, AUTHED_RELEASE = 3, };
	sSIG_SERVER_AUTHENTICATION()
	{
		_TYPE = eSIGMSG::SERVERAUTHENTICATION;
		m_pServerSession = NULL;
		m_dwServerKey = 0;
		m_dwInnerIP = 0;
	}

	// <FIELDs>
	DWORD							m_dwSubType;
	TempServerSessionEx*			m_pServerSession;
	// Addin
	DWORD							m_dwServerKey;
	DWORD							m_dwInnerIP;
};


