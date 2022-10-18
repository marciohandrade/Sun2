#pragma once

#include <Timer.h>
#include <ServerStructInPacket.h>

struct eSIGMSG
{
	static const eSIG_TYPE		REPORTFROMMASTERS		= (eSIG_TYPE)(SIGMSG_NORMAL+100);
	static const eSIG_TYPE		REPORTFROMMASTERS_CMD	= (eSIG_TYPE)(SIGMSG_NORMAL+101);
};

class ServerSessionEx;

struct sSIG_REPORTFROMMASTERS : sSIG_EXTEND
{
	sSIG_REPORTFROMMASTERS() { _TYPE = eSIGMSG::REPORTFROMMASTERS; }
	inline sCACHEITEM_REPORT_INFO_MINORVER&	INFO() { return m_INFO; }
	inline DWORD&					MasterIP() { return m_MasterIP; }
	inline ServerSessionEx*&		SessionRef() { return m_pSession; }

	DWORD						m_MasterIP;
	ServerSessionEx*			m_pSession;
	sCACHEITEM_REPORT_INFO_MINORVER m_INFO;
};

//struct sSIG_REPORTFROMMASTERS_CMD : sSIG_EXTEND
//{
//	enum eSUBTYPE { ADD, REMOVE, LOGGING };
//
//	sSIG_REPORTFROMMASTERS() { _TYPE = eSIGMSG::REPORTFROMMASTERS_CMD; }
//
//	inline eSUBTYPE&				CMD() { return m_CMD; }
//	inline DWORD&					MasterIP() { return m_MasterIP; }
//
//	DWORD						m_MasterIP;
//	eSUBTYPE					m_CMD;
//};


