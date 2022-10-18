/** 
	*.ini 파일을 읽어 들이는 클래스를 정의함
*/

#include "stdafx.h"
#include "FileParser.h"

CFileParser::CFileParser(void)
{

}

CFileParser::~CFileParser(void)
{


}

void CFileParser::Init()
{

}

#pragma warning (push)
#pragma warning (disable: 4244)

BOOL CFileParser::ParseInitFile(char* szFileName)
{
	char	szFullPath[512];

	memset(&m_ServerEnv, NULL, sizeof(SERVER_ENV));

	DWORD dwError = GetCurrentDirectory( 512, szFullPath );
	if(0 == dwError)
	{
		return FALSE;
	}
	strcat(szFullPath, "\\");
	strcat(szFullPath, szFileName); // 주어지는 파일 이름에 대한 풀 패스를 설정함


	// COMMON SECTION 설정 시작
	m_ServerEnv.bGetServerInfoFromFile = GetPrivateProfileInt("COMMON", "GET_SERVERINFO_FROM_FILE", NULL, szFullPath);
	m_ServerEnv.bGMListApply = GetPrivateProfileInt("COMMON", "GMLIST_APPLY", NULL, szFullPath);
	m_ServerEnv.byFieldServerType = GetPrivateProfileInt("COMMON", "FIELD_SERVER_TYPE", NULL, szFullPath);
	m_ServerEnv.byBattleServerType = GetPrivateProfileInt("COMMON", "BATTLE_SERVER_TYPE", NULL, szFullPath);
	m_ServerEnv.bVillageRenderInfo = GetPrivateProfileInt("COMMON", "VILLAGE_RENDERINFO", NULL, szFullPath);
	//m_ServerEnv.bCryptography = GetPrivateProfileInt("COMMON", "CRYPTOGRAPHY_ENABLED", NULL, szFullPath);
	//m_ServerEnv.bCrypto_KeyChange = GetPrivateProfileInt("COMMON", "CRYPTO_KEY_CHANGED", NULL, szFullPath);
	//m_ServerEnv.dwCrypto_KeyChange_Interval = GetPrivateProfileInt("COMMON", "CRYPTO_KEY_CHANGE_INTERVAL", NULL, szFullPath);
	//if( m_ServerEnv.dwCrypto_KeyChange_Interval < SERVER_ENV::CRYPTO_KEYCHANGE_INTERVAL_MIN || m_ServerEnv.dwCrypto_KeyChange_Interval > SERVER_ENV::CRYPTO_KEYCHANGE_INTERVAL_MAX )
	//{
	//	//SUNLOG( eCRITICAL_LOG, "CRYPTO_KEY_CHANGE_INTERVAL 주기값이 범위를 벗어났습니다. 기본값을 %u초로 설정합니다.\n", (DWORD)(SERVER_ENV::CRYPTO_KEYCHANGE_INTERVAL_MIN/1000) );
	//	m_ServerEnv.dwCrypto_KeyChange_Interval = SERVER_ENV::CRYPTO_KEYCHANGE_INTERVAL_MIN;
	//}
	m_ServerEnv.byServiceMode = GetPrivateProfileInt("COMMON", "SERVICE_MODE", NULL, szFullPath);
	m_ServerEnv.byAgeGrade = GetPrivateProfileInt("COMMON", "AGE_GRADE", NULL, szFullPath);
	m_ServerEnv.byServerStatus = GetPrivateProfileInt("COMMON", "SERVER_STATUS", NULL, szFullPath);

	if( !m_ServerEnv.byServerStatus || !(m_ServerEnv.byServerStatus < 5/*eSERVER_UPPERBOUND*/) )
		return FALSE;
	
	// [CLIENT_IOHANDLER] 설정 시작
	INI_IOHANDLER_DESC &cio = m_ServerEnv.ClientIoHandler;
	GetPrivateProfileString( "CLIENT_IOHANDLER", "IP", NULL, m_ServerEnv.ClientIoHandler.szIP, MAX_IPADDRESS_SIZE, szFullPath);
	cio.wPort						= GetPrivateProfileInt( "CLIENT_IOHANDLER", "PORT", NULL, szFullPath );
	cio.dwMaxAcceptSession			= GetPrivateProfileInt( "CLIENT_IOHANDLER", "MAX_ACCEPT_SESSION", NULL, szFullPath );
	cio.dwMaxConnectSession			= GetPrivateProfileInt( "CLIENT_IOHANDLER", "MAX_CONNECT_SESSION", NULL, szFullPath );
	cio.dwSendBufferSize			= GetPrivateProfileInt( "CLIENT_IOHANDLER", "SEND_BUFFER_SIZE", NULL, szFullPath );
	cio.dwRecvBufferSize			= GetPrivateProfileInt( "CLIENT_IOHANDLER", "RECV_BUFFER_SIZE", NULL, szFullPath );
	cio.dwTimeOut					= GetPrivateProfileInt( "CLIENT_IOHANDLER", "TIME_OUT", NULL, szFullPath );
	cio.dwNumberOfAcceptThreads		= GetPrivateProfileInt( "CLIENT_IOHANDLER", "NUMBER_OF_ACCEPT_THREADS", NULL, szFullPath );
	cio.dwNumberOfIoThreads			= GetPrivateProfileInt( "CLIENT_IOHANDLER", "NUMBER_OF_IO_THREADS", NULL, szFullPath );
	cio.dwNumberOfConnectThreads	= GetPrivateProfileInt( "CLIENT_IOHANDLER", "NUMBER_OF_CONNECT_THREADS", NULL, szFullPath );
	cio.dwMaxPacketSize				= GetPrivateProfileInt( "CLIENT_IOHANDLER", "MAX_PACKET_SIZE", NULL, szFullPath );

	// [SERVER_IOHANDLER] 설정 시작
	INI_IOHANDLER_DESC &sio = m_ServerEnv.ServerIoHandler;
	GetPrivateProfileString( "SERVER_IOHANDLER", "IP", NULL, m_ServerEnv.ServerIoHandler.szIP, MAX_IPADDRESS_SIZE, szFullPath);
	sio.wPort						= GetPrivateProfileInt( "SERVER_IOHANDLER", "PORT", NULL, szFullPath );
	sio.dwMaxAcceptSession			= GetPrivateProfileInt( "SERVER_IOHANDLER", "MAX_ACCEPT_SESSION", NULL, szFullPath );
	sio.dwMaxConnectSession			= GetPrivateProfileInt( "SERVER_IOHANDLER", "MAX_CONNECT_SESSION", NULL, szFullPath );
	sio.dwSendBufferSize			= GetPrivateProfileInt( "SERVER_IOHANDLER", "SEND_BUFFER_SIZE", NULL, szFullPath );
	sio.dwRecvBufferSize			= GetPrivateProfileInt( "SERVER_IOHANDLER", "RECV_BUFFER_SIZE", NULL, szFullPath );
	sio.dwTimeOut					= GetPrivateProfileInt( "SERVER_IOHANDLER", "TIME_OUT", NULL, szFullPath );
	sio.dwNumberOfAcceptThreads		= GetPrivateProfileInt( "SERVER_IOHANDLER", "NUMBER_OF_ACCEPT_THREADS", NULL, szFullPath );
	sio.dwNumberOfIoThreads			= GetPrivateProfileInt( "SERVER_IOHANDLER", "NUMBER_OF_IO_THREADS", NULL, szFullPath );
	sio.dwNumberOfConnectThreads	= GetPrivateProfileInt( "SERVER_IOHANDLER", "NUMBER_OF_CONNECT_THREADS", NULL, szFullPath );
	sio.dwMaxPacketSize				= GetPrivateProfileInt( "SERVER_IOHANDLER", "MAX_PACKET_SIZE", NULL, szFullPath );

	/// Master Server 정보 얻기
	GetPrivateProfileString("MASTERSERVER", "MASTERSERVER_IP", NULL, m_ServerEnv.pszMasterServerIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wMasterServerPort = GetPrivateProfileInt("MASTERSERVER", "MASTERSERVER_PORT", NULL, szFullPath);

	// Account DB proxy server 정보 얻기
	GetPrivateProfileString("ACCOUNTDBPROXYSERVER", "ACCOUNTDBPROXYSERVER_IP", NULL, m_ServerEnv.pszAccountDBProxyServerIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wAccountDBProxyServerPort = GetPrivateProfileInt("ACCOUNTDBPROXYSERVER", "ACCOUNTDBPROXYSERVER_PORT", NULL, szFullPath);

	// Game DB proxy server 정보 얻기
	GetPrivateProfileString("GAMEDBPROXYSERVER", "GAMEDBPROXYSERVER_IP", NULL, m_ServerEnv.pszGameDBProxyServerIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wGameDBProxyServerPort = GetPrivateProfileInt("GAMEDBPROXYSERVER", "GAMEDBPROXYSERVER_PORT", NULL, szFullPath);

	// Agent server 정보 얻기
	GetPrivateProfileString("AGENTSERVER", "AGENTSERVER_OUTSIDE_IP", NULL, m_ServerEnv.pszAgentServerOutSideIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wAgentServerOutSidePort = GetPrivateProfileInt("AGENTSERVER", "AGENTSERVER_OUTSIDE_PORT", NULL, szFullPath);
	GetPrivateProfileString("AGENTSERVER", "AGENTSERVER_INSIDE_IP", NULL, m_ServerEnv.pszAgentServerInSideIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wAgentServerInSidePort = GetPrivateProfileInt("AGENTSERVER", "AGENTSERVER_INSIDE_PORT", NULL, szFullPath);

	// Portal server 정보 얻기
	GetPrivateProfileString("PORTALSERVER", "PORTALSERVER_IP", NULL, m_ServerEnv.pszPortalServerIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wPortalServerPort = GetPrivateProfileInt("PORTALSERVER", "PORTALSERVER_PORT", NULL, szFullPath);

	// AuthAgentServer
	m_ServerEnv.bEnableAuthAgentServer = GetPrivateProfileInt("AUTHAGENTSERVER", "ENABLE_AUTH_AGENT_SERVER", NULL, szFullPath);
	m_ServerEnv.wAgentServerID = GetPrivateProfileInt("AUTHAGENTSERVER", "AGENTSERVER_ID", NULL, szFullPath);
	GetPrivateProfileString("AUTHAGENTSERVER", "AUTHAGENTSERVER_IP", NULL, m_ServerEnv.pszAuthAgentServerIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wAuthAgentServerPort = GetPrivateProfileInt("AUTHAGENTSERVER", "AUTHAGENTSERVER_PORT", NULL, szFullPath);
	
	// WorldServer
	GetPrivateProfileString("WORLDSERVER", "WORLDSERVER_IP", NULL, m_ServerEnv.pszWorldServerIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wWorldServerPort = GetPrivateProfileInt("WORLDSERVER", "WORLDSERVER_PORT", NULL, szFullPath);
	GetPrivateProfileString("WORLDSERVER", "WORLDSERVER_CLIENT_IP", NULL, m_ServerEnv.pszWorldServerClientIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wWorldServerClientPort = GetPrivateProfileInt("WORLDSERVER", "WORLDSERVER_CLIENT_PORT", NULL, szFullPath);

	// GuildServer
	GetPrivateProfileString("GUILDSERVER", "GUILDSERVER_IP", NULL, m_ServerEnv.pszGuildServerIP, MAX_IPADDRESS_SIZE, szFullPath);
	m_ServerEnv.wGuildServerPort = GetPrivateProfileInt("GUILDSERVER", "GUILDSERVER_PORT", NULL, szFullPath);


	// DB(SQL Server)의 IP주소(2005.04.26 추가)
	GetPrivateProfileString("SQLSERVER", "SQLSERVER_IP", NULL, m_ServerEnv.pszSQLServerIP, MAX_IPADDRESS_SIZE, szFullPath);

	// Log File 저장 경로 
	GetPrivateProfileString("LOGFILE", "WOPS_LOGFILE_PATH", NULL, m_ServerEnv.pszLogPath, MAX_LOGPATH_LEN, szFullPath);
	m_ServerEnv.byLogOption = GetPrivateProfileInt("LOGFILE", "WOPS_LOGFILE_OPTION", NULL, szFullPath);

	// SUN LOGFILE 정보 저장
	m_ServerEnv.bySunLogOption = GetPrivateProfileInt("LOGFILE", "SUN_LOGFILE_OPTION", 0, szFullPath);
	m_ServerEnv.bySunLogfileLevel = GetPrivateProfileInt("LOGFILE", "SUN_LOGFILE_LEVEL", 0, szFullPath);
	GetPrivateProfileString("LOGFILE", "SUN_LOGFILE_PATH", NULL, m_ServerEnv.pszSunLogfilePath, MAX_LOGPATH_LEN, szFullPath);
	m_ServerEnv.dwServerInfoCycle = GetPrivateProfileInt("LOGFILE", "SERVERINFO_CYCLE", 0, szFullPath);

	// 반드시 있어야 하는 값 체크!!
	if( m_ServerEnv.ClientIoHandler.dwMaxAcceptSession == 0 )	return FALSE;
	if( m_ServerEnv.ServerIoHandler.wPort == 0 )				return FALSE;

	return TRUE;
}

#pragma warning (pop)



