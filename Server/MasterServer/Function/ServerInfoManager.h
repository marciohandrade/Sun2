#pragma once


//=================================================================================================
// DB에서 로드한 서버 정보를 관리하는 클래스
//	- tagSERVER_INFO 구조체에 DB에서 로드한 정보들을 담아 맵으로 관리한다.
//	- 맵의 키로써 월드, 채널, 서버타입, 서버ID 4개로 만들어진 값을 사용한다.
//=================================================================================================

class ServerSessionEx;

class ServerInfoManager
{
private:
	ServerInfoManager() {}
	~ServerInfoManager() {}

public:

	static VOID				CreateInstance();
	static VOID				DestroyInstance();

public:
	// <INTERFACEs>

	static VOID				Clear();
	static VOID				AddServerInfo( LPSERVER_INFO lpServerInfo, DWORD dwCount = 1, BOOL bRefresh = 0 );
	static LPSERVER_INFO	GetServerInfo( SERVER_KEY skKey );
	static SERVER_KEY		GetServerKey( eSERVER_TYPE serverType, CHAR* pszIP );

	static VOID				GetGameDBProxyInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pGameDBProxyInfo );
	static VOID				GetAgentServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pAgentServerInfo );
	static VOID				GetWorldServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pWorldServerInfo );
	static VOID				GetGuildServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pGuildServerInfo );
	static VOID				GetPortalServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pPortalServerInfo );
	static VOID				GetAuthAgentServerInfo( LPSERVER_INFO pAuthAgentServerInfo );
	static VOID				GetShopServerInfo( LPSERVER_INFO pShopServerInfo );

private:
	static VOID				_GetServerInfo( BYTE byWorldID, BYTE byChannelID, LPSERVER_INFO pTargetInfo, eSERVER_TYPE serverType );
};

