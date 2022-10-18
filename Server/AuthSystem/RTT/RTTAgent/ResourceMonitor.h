#pragma once

#include <eXtreme/eXtreme.h>


#define	WAIT_RESOURCE_TIME	30000

typedef struct tag_ResourceInfo
{
	DWORD			ramTotalSize;
	DWORD			ramUsingSize;

	DWORD			cpuUsing;

	DWORD			hddTotalSize;
	DWORD			hddUsingSize;
} ResourceInfo;


class ResourceMonitor
{
	Singleton( ResourceMonitor );

public:
			void				Initialize();
			void				Release();

			ResourceInfo		m_currentInfo;

private:
	static  u_int __stdcall		Runner( LPVOID parameter );

			void				ReadInfo();
			void				SendReport();


			HANDLE				m_handleThread;
			DWORD				m_threadID;
			HANDLE				m_eventKill;

	std::string FileRead(void);
	int FindIntegerValue(std::string strBuffer, char* lpHeader, char* lpValueName);
	__int64 FindInteger64Value(std::string strBuffer, char* lpHeader, char* lpValueName);
};