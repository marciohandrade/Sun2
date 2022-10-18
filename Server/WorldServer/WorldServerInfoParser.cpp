#include "stdafx.h"
#include "WorldServerInfoParser.h"

WorldServerInfoParser::WorldServerInfoParser(void)
{
	m_nServiceMode = 0;
	m_nCryptable = 0;
}

WorldServerInfoParser::~WorldServerInfoParser(void)
{
}

BOOL WorldServerInfoParser::ParseINI( char *szFileName )
{
	char szFullPath[512]={0,};

	DWORD dwError = GetCurrentDirectory( 512, szFullPath );
	if( dwError == 0 ) return FALSE;
	strcat(szFullPath, "\\");
	strcat(szFullPath, szFileName); // �־����� ���� �̸��� ���� Ǯ �н��� ������

	memcpy( m_szFullPath, szFullPath, sizeof(m_szFullPath) );

	// ���Ͽ��� ���� ���ΰ�?
	m_bServerInfoFromFile = GetPrivateProfileInt( "COMMON", "GET_SERVERINFO_FROM_FILE", NULL, szFullPath );

	//���� ���̵�
	m_dwWorldID = GetPrivateProfileInt( "COMMON", "WORLD_ID", NULL, szFullPath );

	m_nCryptable = GetPrivateProfileInt( "COMMON", "CRYPTOGRAPHY_ENABLED", 0, szFullPath );

	/// Client-side I/O ���� ���
	GetPrivateProfileString( "CLIENT_IO", "IP", NULL, m_ClientIoDesc.szIP, MAX_IPADDRESS_SIZE, szFullPath );
	m_ClientIoDesc.wPort					= GetPrivateProfileInt( "CLIENT_IO", "PORT", NULL, szFullPath );
	m_ClientIoDesc.dwMaxAcceptSession		= GetPrivateProfileInt( "CLIENT_IO", "MAX_ACCEPT_SESSION", NULL, szFullPath );
	m_ClientIoDesc.dwMaxConnectSession		= GetPrivateProfileInt( "CLIENT_IO", "MAX_CONNECT_SESSION", NULL, szFullPath );
	m_ClientIoDesc.dwSendBufferSize			= GetPrivateProfileInt( "CLIENT_IO", "SEND_BUFFER_SIZE", NULL, szFullPath );
	m_ClientIoDesc.dwRecvBufferSize			= GetPrivateProfileInt( "CLIENT_IO", "RECV_BUFFER_SIZE", NULL, szFullPath );
	m_ClientIoDesc.dwTimeOut				= GetPrivateProfileInt( "CLIENT_IO", "TIME_OUT", NULL, szFullPath );
	m_ClientIoDesc.dwNumberOfAcceptThreads	= GetPrivateProfileInt( "CLIENT_IO", "NUMBER_OF_ACCEPT_THREADS", NULL, szFullPath );
	m_ClientIoDesc.dwNumberOfIoThreads		= GetPrivateProfileInt( "CLIENT_IO", "NUMBER_OF_IO_THREADS", NULL, szFullPath );
	m_ClientIoDesc.dwNumberOfConnectThreads	= GetPrivateProfileInt( "CLIENT_IO", "NUMBER_OF_CONNECT_THREADS", NULL, szFullPath );
	m_ClientIoDesc.dwMaxPacketSize			= GetPrivateProfileInt( "CLIENT_IO", "MAX_PACKET_SIZE", NULL, szFullPath );

	/// Server-side I/O ���� ���
	GetPrivateProfileString( "SERVER_IO", "IP", NULL, m_ServerIoDesc.szIP, MAX_IPADDRESS_SIZE, szFullPath );
	m_ServerIoDesc.wPort					= GetPrivateProfileInt( "SERVER_IO", "PORT", NULL, szFullPath );
	m_ServerIoDesc.dwMaxAcceptSession		= GetPrivateProfileInt( "SERVER_IO", "MAX_ACCEPT_SESSION", NULL, szFullPath );
	m_ServerIoDesc.dwMaxConnectSession		= GetPrivateProfileInt( "SERVER_IO", "MAX_CONNECT_SESSION", NULL, szFullPath );
	m_ServerIoDesc.dwSendBufferSize			= GetPrivateProfileInt( "SERVER_IO", "SEND_BUFFER_SIZE", NULL, szFullPath );
	m_ServerIoDesc.dwRecvBufferSize			= GetPrivateProfileInt( "SERVER_IO", "RECV_BUFFER_SIZE", NULL, szFullPath );
	m_ServerIoDesc.dwTimeOut				= GetPrivateProfileInt( "SERVER_IO", "TIME_OUT", NULL, szFullPath );
	m_ServerIoDesc.dwNumberOfAcceptThreads	= GetPrivateProfileInt( "SERVER_IO", "NUMBER_OF_ACCEPT_THREADS", NULL, szFullPath );
	m_ServerIoDesc.dwNumberOfIoThreads		= GetPrivateProfileInt( "SERVER_IO", "NUMBER_OF_IO_THREADS", NULL, szFullPath );
	m_ServerIoDesc.dwNumberOfConnectThreads	= GetPrivateProfileInt( "SERVER_IO", "NUMBER_OF_CONNECT_THREADS", NULL, szFullPath );
	m_ServerIoDesc.dwMaxPacketSize			= GetPrivateProfileInt( "SERVER_IO", "MAX_PACKET_SIZE", NULL, szFullPath );

	/// Master Server ���� ���
	GetPrivateProfileString( "MASTERSERVER", "IP", NULL, m_szMasterServerIP, MAX_IPADDRESS_SIZE, szFullPath );
	m_wMasterServerPort = GetPrivateProfileInt( "MASTERSERVER", "PORT", NULL, szFullPath );

	// GameDBProxy ���� ���
	GetPrivateProfileString( "GAMEDBPROXY", "IP", NULL, m_szGameDBProxyIP, MAX_IPADDRESS_SIZE, szFullPath );
	m_wGameDBProxyPort = GetPrivateProfileInt( "GAMEDBPROXY", "PORT", NULL, szFullPath );

	// ���� ��� ���� ����
	m_nServiceMode = GetPrivateProfileInt("SERVICE", "SERVICE_MODE", 0, szFullPath);

	// SUN LOGFILE ���� ����
	GetPrivateProfileString("LOGFILE", "WOPS_LOGFILE_PATH", "..\\Log", m_szLogFilePath, MAX_LOGPATH_LEN, szFullPath);
	m_byLogOption = (BYTE)GetPrivateProfileInt("LOGFILE", "SUN_LOGFILE_OPTION", 0, szFullPath);
	m_SunLogInfo.bySunLogOption = (BYTE)GetPrivateProfileInt("LOGFILE", "SUN_LOGFILE_OPTION", 0, szFullPath);
	m_SunLogInfo.wSunLogfileLevel = GetPrivateProfileInt("LOGFILE", "SUN_LOGFILE_LEVEL", 1, szFullPath);
	GetPrivateProfileString("LOGFILE", "SUN_LOGFILE_PATH", ".\\Log", m_SunLogInfo.pszSunLogfilePath, MAX_LOGPATH_LEN, szFullPath);

	// �ݵ�� �־�� �ϴ� �� üũ!!
	if( strlen( m_ClientIoDesc.szIP ) == 0 )	return FALSE;
	if( m_ClientIoDesc.wPort == 0 )				return FALSE;
	if( strlen( m_szMasterServerIP ) == 0 )		return FALSE;
	if( m_wMasterServerPort == 0 )				return FALSE;

	// ��� ���� ���
	m_dwStatisticsInterval = (DWORD)GetPrivateProfileInt( "INTERVAL", "STATISTICS", 10000, szFullPath );

	// ��ġ�� ���� ���
	m_dwShoutInterval = (DWORD)GetPrivateProfileInt( "INTERVAL", "SHOUT", 30000, szFullPath );
//	MessageOut( eFULL_LOG, "Timer %u", m_dwStatisticsInterval );

	// ���� ���� ��� ���� üũ
	m_byFileNotice = (BOOL)GetPrivateProfileInt( "COMMON", "NOTICEFROMFILE", 0, szFullPath );

	// ���� ���� ��ȸ
	m_byServerStatus = (BYTE)GetPrivateProfileInt( "COMMON", "SERVER_STATUS", 0, szFullPath );

	return TRUE;
}

VOID WorldServerInfoParser::ReloadShoutInterval()
{
	// ��ġ�� ���� ���
	m_dwShoutInterval = (DWORD)GetPrivateProfileInt( "INTERVAL", "SHOUT", 30000, m_szFullPath );
}

VOID WorldServerInfoParser::ReloadFileNotice()
{
	m_byFileNotice = (BOOL)GetPrivateProfileInt( "COMMON", "NOTICEFROMFILE", 0, m_szFullPath );
}

