#include "stdafx.h"
#include "MonitorProcess.h"

MonitorProcess::MonitorProcess()
{
}

MonitorProcess::~MonitorProcess()
{
}

void MonitorProcess::Clear()
{
	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );

	for(MapList::iterator iter = m_mapList.begin() ; iter != m_mapList.end() ; iter++ )
	{
		ListProcess&	listProcess = iter->second;

		while( listProcess.size() > 0 )
		{
			ServerProcess*	info;

			info = listProcess.front();
			listProcess.pop_front();

			delete info;
		}
	}

	m_mapList.clear();
}

void MonitorProcess::AddProcess( short worldID, short channelID, short serverType, short serverID, wchar_t* serverIP, short serverTypeForPatch,
								 wchar_t* processName, wchar_t* abPath )
{
	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );
	ServerProcess*	info	= new ServerProcess;

	info->i_ProcessIndex	= -1;
	info->ti_WorldID		= worldID;
	info->ti_ChannelID		= channelID;
	info->ti_ServerType		= serverType;
	info->ti_ServerID		= serverID;
	info->ti_ServerTypeForPatch = serverTypeForPatch;
	info->dwStatus			= 0;
	info->dw_alert			= 0;

	info->i_ParentHistoryIDX = 0;

	wcsncpy( info->vc_ProcessName, processName, 100 );
	wcsncpy( info->vc_AbsolutePath, abPath, 100 );

	char ip[IP_MAX_LEN] = {0, };
	::WideCharToMultiByte( CP_ACP, 0, serverIP, -1, ip, IP_MAX_LEN, 0, 0 );

	strncpy(info->vc_InnerIP, ip, IP_MAX_LEN);
	info->vc_InnerIP[IP_MAX_LEN-1] = '\0';

	ListProcess&	listProcess = m_mapList[::inet_addr(info->vc_InnerIP)];	//  key�� �ش��ϴ� List�� ���ٸ� �� ������ �ڵ����� �����ȴ�.

	listProcess.push_back( info );	//  key�� �ش��ϴ� List�� ���ٸ� �� ������ �ڵ����� �����ȴ�.

//*/ ���� ���μ��� �α�
	char						binary[100] = {0, };
	char						path[100] = {0, };

	::WideCharToMultiByte( CP_ACP, 0, info->vc_ProcessName, -1, binary, 100, 0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, info->vc_AbsolutePath, -1, path, 100, 0, 0 );

	OutputDebug( "[MonitorProcess::AddProcess] (%d/%d/%d/%d) %s, %s\\%s(%d)\n",
					info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID, info->vc_InnerIP, path, binary, listProcess.size() );

//	OutputDebug( "[MonitorProcess::AddProcess] Path Len : %d\n", wcslen(info->vc_AbsolutePath) );
//	OutputDebug( "[MonitorProcess::AddProcess] Name Len : %d\n", wcslen(info->vc_ProcessName) );
//*/
}

void MonitorProcess::GetList( short worldID, short channelID, short serverType, short serverID,
								char* serverIP,	ServerProcess processList[], int bufferSize, int *listSize, bool isSendRTTA )
{
	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );

	//  �ϴ� 0���� �ʱ�ȭ�Ѵ�.
	*listSize = 0;


	//  �����ϴ� Key������ �˻��Ѵ�.
	MapList::iterator iterMap = m_mapList.find( ::inet_addr(serverIP) );

	if( iterMap == m_mapList.end() )
		return;


	ListProcess&			listProcess = iterMap->second;

	for(ListProcess::iterator iter = listProcess.begin() ; iter != listProcess.end() ; iter++ )
	{
		ServerProcess*	info = *iter;

		if( *listSize >= bufferSize )
			break;

		// RTTAgent���� ��û�� ����̸� �� IP�� ����ִ� ��� ����� �ش�
		if( isSendRTTA )
		{
			::memcpy( &processList[*listSize], info, sizeof( ServerProcess ) );
				( *listSize )++;
		}
		else
		{
			if( info->ti_WorldID  == worldID &&
				info->ti_ChannelID == channelID &&
				info->ti_ServerType	== serverType &&
				info->ti_ServerID	== serverID )
			{
				::memcpy( &processList[*listSize], info, sizeof( ServerProcess ) );
				( *listSize )++;
			}
		}
	}
}

void MonitorProcess::SetProcessStatus( char* serverIP, wchar_t* processName, wchar_t* abPath, bool isActive )
{
	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );

	//  �����ϴ� Key������ �˻��Ѵ�.
	MapList::iterator iterMap = m_mapList.find( ::inet_addr(serverIP) );

	if( iterMap == m_mapList.end() )
	{
		/*
		OutputDebug( "[MonitorProcess::SetProcessStatus] not find (%d %d %d %d) %S\\%S(%d)\n", 
				worldID,
				channelID,
				serverType,
				serverID,
				abPath,
				processName,
				isActive);
		*/
		return;
	}

	ListProcess&			listProcess = iterMap->second;

	for(ListProcess::iterator iter = listProcess.begin() ; iter != listProcess.end() ; iter++ )
	{
		ServerProcess*	info = *iter;

		if( wcsncmp( processName, info->vc_ProcessName, 100 ) == 0 && wcsncmp( abPath, info->vc_AbsolutePath, 100 ) == 0 )
		{
			//  ����ִ� ���¶�� DB�� ������� ���� �ʴ´�.
			if( isActive == true )
			{
				info->dw_alert = (info->dw_alert==2 || info->dw_alert==3) ? 3 : 0 ;
			}
			//  �׾��ִ� �����̸�, ������ DB�� ������� �ʾҴٸ� ����� �����Ѵ�.
			else if( isActive == false && info->dw_alert == 0 )
			{
				info->dw_alert = 1;

                //*
				OutputDebug( "[MonitorProcess::SetProcessStatus] Not Running Process (%d %d %d %d) %S\\%S\n", info->ti_WorldID, info->ti_ChannelID, info->ti_ServerType, info->ti_ServerID, info->vc_AbsolutePath, info->vc_ProcessName );
				//*/
			}

			info->dwStatus = ( isActive == true ? 1 : 0 );

			/*OutputDebug( "[MonitorProcess::SetProcessStatus](%d %d %d %d)%S\\%S(%d)(%d)\n",
				worldID,
				channelID,
				serverType,
				serverID,
				abPath, 
				processName,
				info->dw_alert, 
				info->dwStatus);
			*/
		}
	}
}

void MonitorProcess::SetProcessAlert( short worldID, short channelID, short serverType, short serverID, char* serverIP,
								      wchar_t* processName, wchar_t* abPath, DWORD alertStatus, DWORD parentHistoryIDX )
{
	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );

	//  �����ϴ� Key������ �˻��Ѵ�.
	MapList::iterator iterMap = m_mapList.find( ::inet_addr(serverIP) );
	if( iterMap == m_mapList.end() )
		return;


	ListProcess&			listProcess = iterMap->second;

	for(ListProcess::iterator iter = listProcess.begin() ; iter != listProcess.end() ; iter++ )
	{
		ServerProcess*	info = *iter;

		if( info->ti_WorldID  == worldID &&
			info->ti_ChannelID == channelID &&
			info->ti_ServerType == serverType &&
			info->ti_ServerID	== serverID )
		{
			if( wcsncmp( processName, info->vc_ProcessName, 100 ) == 0 &&
				wcsncmp( abPath, info->vc_AbsolutePath, 100 ) == 0 )
			{
				info->dw_alert = alertStatus;
				info->i_ParentHistoryIDX = parentHistoryIDX;
				break;
			}
		}
	}
	/*
	OutputDebug("[MonitorProcess::SetProcessAlert](%d,%d,%d,%d) %S\\%S %d : %d \n", 
											worldID, 
											channelID, 
											serverType, 
											serverID,
											abPath, 
											processName, 
											alertStatus, 
											parentHistoryIDX);
											*/
}














//void MonitorProcess::Clear()
//{
//	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );
//
//	for(MapList::iterator iter = m_mapList.begin() ; iter != m_mapList.end() ; iter++ )
//	{
//		ListProcess&	listProcess = iter->second;
//
//		while( listProcess.size() > 0 )
//		{
//			ServerProcess*	info;
//
//			info = listProcess.front();
//			listProcess.pop_front();
//
//			delete info;
//		}
//	}
//
//	m_mapList.clear();
//}
//
//void MonitorProcess::AddProcess( short worldID, short channelID, short serverType, short serverID, short serverTypeForPatch,
//								 wchar_t* processName, wchar_t* abPath )
//{
//	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );
//	ServerProcess*	info	= new ServerProcess;
//
//	DWORD			key = UID( worldID, channelID, serverType, serverID );
//	ListProcess&	listProcess = m_mapList[key];	//  key�� �ش��ϴ� List�� ���ٸ� �� ������ �ڵ����� �����ȴ�.
//
//	info->i_ProcessIndex	= -1;
//	info->ti_WorldID		= worldID;
//	info->ti_ChannelID		= channelID;
//	info->ti_ServerType		= serverType;
//	info->ti_ServerID		= serverID;
//	info->ti_ServerTypeForPatch = serverTypeForPatch;
//	info->dwStatus			= 0;
//	info->dw_alert			= 0;
//
//	info->i_ParentHistoryIDX = 0;
//
//	wcsncpy( info->vc_ProcessName, processName, 100 );
//	wcsncpy( info->vc_AbsolutePath, abPath, 100 );
//
//	listProcess.push_back( info );
//
////*/ ���� ���μ��� �α�
//	char						binary[100] = {0, };
//	char						path[100] = {0, };
//
//	::WideCharToMultiByte( CP_ACP, 0, info->vc_ProcessName, -1, binary, 100, 0, 0 );
//	::WideCharToMultiByte( CP_ACP, 0, info->vc_AbsolutePath, -1, path, 100, 0, 0 );
//
//	OutputDebug( "[MonitorProcess::AddProcess] %s, %s\\%s\n", path, binary );
////	OutputDebug( "[MonitorProcess::AddProcess] Path Len : %d\n", wcslen(info->vc_AbsolutePath) );
////	OutputDebug( "[MonitorProcess::AddProcess] Name Len : %d\n", wcslen(info->vc_ProcessName) );
////*/
//}
//
//void MonitorProcess::GetList( short worldID, short channelID, short serverType, short serverID,
//									ServerProcess processList[], int bufferSize, int *listSize )
//{
//	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );
//
//	//  �ϴ� 0���� �ʱ�ȭ�Ѵ�.
//	*listSize = 0;
//
//
//	//  �����ϴ� Key������ �˻��Ѵ�.
//	MapList::iterator iterMap = m_mapList.find( UID( worldID, channelID, serverType, serverID ) );
//	if( iterMap == m_mapList.end() )
//		return;
//
//
//	ListProcess&			listProcess = iterMap->second;
//
//	for(ListProcess::iterator iter = listProcess.begin() ; iter != listProcess.end() ; iter++ )
//	{
//		ServerProcess*	info = *iter;
//
//		if( *listSize >= bufferSize )
//			break;
//
//		::memcpy( &processList[*listSize], info, sizeof( ServerProcess ) );
//		( *listSize )++;
//	}
//}
//
//void MonitorProcess::SetProcessStatus( short worldID, short channelID, short serverType, short serverID, wchar_t* processName, wchar_t* abPath, bool isActive )
//{
//	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );
//
//	//  �����ϴ� Key������ �˻��Ѵ�.
//	MapList::iterator iterMap = m_mapList.find( UID( worldID, channelID, serverType, serverID ) );
//	if( iterMap == m_mapList.end() )
//	{
//		/*
//		OutputDebug( "[MonitorProcess::SetProcessStatus] not find (%d %d %d %d) %S\\%S(%d)\n", 
//				worldID,
//				channelID,
//				serverType,
//				serverID,
//				abPath,
//				processName,
//				isActive);
//		*/
//		return;
//	}
//
//	ListProcess&			listProcess = iterMap->second;
//
//	for(ListProcess::iterator iter = listProcess.begin() ; iter != listProcess.end() ; iter++ )
//	{
//		ServerProcess*	info = *iter;
//
//		if( wcsncmp( processName, info->vc_ProcessName, 100 ) == 0 && wcsncmp( abPath, info->vc_AbsolutePath, 100 ) == 0 )
//		{
//			//  ����ִ� ���¶�� DB�� ������� ���� �ʴ´�.
//			if( isActive == true )
//			{
//				info->dw_alert = (info->dw_alert==2 || info->dw_alert==3) ? 3 : 0 ;
//			}
//			//  �׾��ִ� �����̸�, ������ DB�� ������� �ʾҴٸ� ����� �����Ѵ�.
//			else if( isActive == false && info->dw_alert == 0 )
//			{
//				info->dw_alert = 1;
//			}
//
//			info->dwStatus = ( isActive == true ? 1 : 0 );
//
//			/*OutputDebug( "[MonitorProcess::SetProcessStatus](%d %d %d %d)%S\\%S(%d)(%d)\n",
//				worldID,
//				channelID,
//				serverType,
//				serverID,
//				abPath, 
//				processName,
//				info->dw_alert, 
//				info->dwStatus);
//			*/
//		}
//	}
//}
//
//void MonitorProcess::SetProcessAlert( short worldID, short channelID, short serverType, short serverID,
//									wchar_t* processName, wchar_t* abPath, DWORD alertStatus, DWORD parentHistoryIDX )
//{
//	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutex );
//
//	//  �����ϴ� Key������ �˻��Ѵ�.
//	MapList::iterator iterMap = m_mapList.find( UID( worldID, channelID, serverType, serverID ) );
//	if( iterMap == m_mapList.end() )
//		return;
//
//
//	ListProcess&			listProcess = iterMap->second;
//
//	for(ListProcess::iterator iter = listProcess.begin() ; iter != listProcess.end() ; iter++ )
//	{
//		ServerProcess*	info = *iter;
//
//		if( wcsncmp( processName, info->vc_ProcessName, 100 ) == 0 &&
//			wcsncmp( abPath, info->vc_AbsolutePath, 100 ) == 0 )
//		{
//			info->dw_alert = alertStatus;
//			info->i_ParentHistoryIDX = parentHistoryIDX;
//			break;
//		}
//	}
//	/*
//	OutputDebug("[MonitorProcess::SetProcessAlert](%d,%d,%d,%d) %S\\%S %d : %d \n", 
//											worldID, 
//											channelID, 
//											serverType, 
//											serverID,
//											abPath, 
//											processName, 
//											alertStatus, 
//											parentHistoryIDX);
//											*/
//}