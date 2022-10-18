#ifndef _SERVERSESSION_H_
#define _SERVERSESSION_H_

#include <windows.h>
#include <const.h>
#include <NetworkObject.h>
#include <string>

//=================================================================================================
// 다른 서버들로부터의 접속을 받기 위한 세션의 최상위 클래스
//=================================================================================================
class ServerSession : public NetworkObject
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	, public ISession4StartServiceWorkNode
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
{
public:
	ServerSession();
	virtual ~ServerSession();

	virtual VOID				Init();
	virtual VOID				Release();
	virtual VOID				Update();
	virtual eSERVER_TYPE		GetServerType() { return UNKNOWN_SERVER; }

	inline VOID					SetSessionIndex( DWORD dwNetworkIndex ) { m_dwSessionIndex = dwNetworkIndex; }
	inline DWORD				GetSessionIndex() { return m_dwSessionIndex; }
	inline VOID					SetServerKey( DWORD dwServerKey ) { m_dwServerKey = dwServerKey; }
	inline DWORD				GetServerKey() { return m_dwServerKey; }

	VOID						SetAddr( char *pszIP, WORD wPort );

	virtual VOID				OnRedirect() {}

	inline std::string&			GetConnectIP() { return m_strConnectIP; }
	inline WORD					GetConnectPort() { return m_wConnectPort; }
	inline VOID					SetForConnect( BOOL bForConnect ) { m_bForConnect = bForConnect; }
	inline BOOL					IsForConnect() { return m_bForConnect; }
	inline BOOL					IsConnected() { return m_bConnection; }

protected:
	// Network
	virtual	VOID				OnAccept( DWORD dwNetworkIndex );
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual VOID				OnRecv( BYTE *pMsg, WORD wSize ) { __UNUSED((pMsg, wSize)); }
	virtual VOID				OnLogString( char *pszLog );

private:
	VOID						SendServerType();

	DWORD						m_dwSessionIndex;
	DWORD						m_dwServerKey;
	std::string					m_strConnectIP;
	WORD						m_wConnectPort;
	BOOL						m_bForConnect;
	DWORD						m_dwLastHeartbeatTick;
	BOOL						m_bConnection;
private:
	DWORD						m_dwSyncKey;
public:
	// 이것은 WorkLoad에서 해당 세션에 대한 유효성을 판별하기 위해서 사용한다.
	inline VOID					SetSyncKey( DWORD dwSyncValue ) { m_dwSyncKey = dwSyncValue; }
	inline DWORD				GetSyncKey() { return m_dwSyncKey; }

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
public:
	virtual BOOL				IsValidObject( DWORD dwCheckValue );
	virtual BOOL				IsValidState( DWORD dwStatusValue );
	virtual BOOL				SendPacket4Interface( MSG_BASE* pMsg, WORD wSize );

public:
	WORK_LOAD_CONTAINER_SESSION_WORK&	GetWorkLoadContainer() { return m_WorkLoad; }
	WorkLoadPacketStream&		GetWorkLoadPacketStream() { return m_WorkLoadPacketStream; }
private:
	WorkLoadPacketStream				m_WorkLoadPacketStream;
	WORK_LOAD_CONTAINER_SESSION_WORK	m_WorkLoad;
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
};

#endif // _SERVERSESSION_H_