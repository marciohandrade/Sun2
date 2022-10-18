#ifndef WEB_REQUEST_MONITOR_H
#define WEB_REQUEST_MONITOR_H

// 06³â 12¿ù ±èÁ¾È£

#include <vector>
#include <eXtreme/eXtreme.h>
#include <eXtreme/threading/Mutex.h>

#include "WebRequest.h"

class CWebRequestMonitor
{
	Singleton(CWebRequestMonitor)

	typedef std::vector<CWebRequest*> OBSERVERLIST;

    OBSERVERLIST m_WebRequestList;
	HANDLE m_eventKill;
	HANDLE m_hThread;
	eXtreme::Mutex m_mutexRequest;
	bool m_bStartRequest;

public:

	bool IsStartWebRequest() { return m_bStartRequest; }
    void StartWebRequestMonitor();
	bool AddURL(DWORD dwIndex, char* pURL);
	static unsigned int __stdcall Runner(void* lParam);
};

#endif