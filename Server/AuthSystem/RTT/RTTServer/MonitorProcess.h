#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <map>
#include <list>

#pragma warning(pop)

#include <eXtreme/threading/Mutex.h>

#include "Database.h"

class MonitorProcess
{
	Singleton( MonitorProcess );

public:
			void		Clear();

			void		AddProcess( short worldID, short channelID, short serverType, short serverID, wchar_t* serverIP, short serverTypeForPatch,
									wchar_t* processName, wchar_t* abPath );
			void		GetList( short worldID, short channelID, short serverType, short serverID, char* serverIP,
									ServerProcess processList[], int bufferSize, int *listSize, bool isSendRTTA = false );

			void		SetProcessStatus( char* serverIP, wchar_t* processName, wchar_t* abPath, bool isActive );
			void		SetProcessAlert( short worldID, short channelID, short serverType, short serverID, char* serverIP,
									wchar_t* processName, wchar_t* abPath, DWORD alertStatus, DWORD parentHistoryIDX = 0);

private:
	typedef	std::list	< ServerProcess* >		ListProcess;
	typedef	std::map	< DWORD, ListProcess >			MapList;

			eXtreme::Mutex		m_mutex;
			MapList				m_mapList;
};