#pragma once


//=================================================================================================
// DB���� �ε��� ���� ������ �����ϴ� Ŭ����
//	- tagSERVER_INFO ����ü�� DB���� �ε��� �������� ��� ������ �����Ѵ�.
//	- ���� Ű�ν� ����, ä��, ����Ÿ��, ����ID 4���� ������� ���� ����Ѵ�.
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

