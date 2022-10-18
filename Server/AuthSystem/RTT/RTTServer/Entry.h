#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <eXtreme/net/ServerNetwork.h>

#pragma warning(pop)

#define  ALIVETICK_LIMITE		1000 * 10 * 3		// RTTAgent / RTTService HeartBeat 체크

#include "ManagerWops.h"
#include "ManagerClient.h"
#include "ManagerService.h"
#include "ManagerWeb.h"

class Entry : public eXtreme::Thread
{
	Singleton( Entry );

public:
	static	bool						OnServiceStart();
	static	void						OnServiceStop();


			bool						Initialize();
			void						Destroy();

			void						ServiceStartUp();
			void						ServiceTerminate();

			DWORD	GetRTTAgentVersion(){ return m_dwRTTAgentVersion; };

protected:
	virtual	void						Run();

private:
	static	void						ProcCrashHandling( LPCTSTR stackWalkLog, LPCTSTR dumpFileName );
	static	LPCTSTR						GenerateDmpName();


			eXtreme::ServerNetwork		m_networkClient;
			eXtreme::ServerNetwork		m_networkWops;
			eXtreme::ServerNetwork		m_networkService;

#ifdef	_090427_HTTP_WEB_DROP_USER		// 중국 웹 상에서 유저 강제 종료 요청 기능 사용
			eXtreme::ServerNetwork		m_networkWeb;
			char						m_ipAddressWeb[20];
			DWORD						m_portNoWeb;
#endif

			char						m_ipAddress[20], m_ipAddressWops[20], m_ipAddressService[20];
			DWORD						m_portNo, m_portNoWops, m_portNoService;
			bool						m_serviceRun;
			HANDLE						m_eventKill;

			DWORD						m_dwRTTAgentVersion;	// RTTAgent Version 을 ini 파일에서 읽어 와서 보관을 한다.
};