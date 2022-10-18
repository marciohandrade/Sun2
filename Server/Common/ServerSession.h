#pragma once

#include <NetworkDefine.h>
#include <NetworkObject.h>
#include <string>
#include <Timer.h>
#include <MemoryPoolFactory.h>

class ServerFrame;
struct MSG_BASE;
struct MSG_BASE_INTERNAL;
struct MSG_SERVER_TYPE;

class ServerSession	: 
	public NetworkObject
{
public:
	ServerSession(void);
	virtual ~ServerSession(void);	
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg) = 0;

protected:
    // NetworkObject interfaces = {
    virtual void OnAccept(DWORD session_index);
    virtual void OnDisconnect();
    virtual void OnConnect(BOOL bSuccess, DWORD session_index);
    virtual void OnRecv(BYTE *pMsg, WORD wSize) { __UNUSED((pMsg, wSize)); }
    virtual void OnLogString(char* pszLog);
    // }
    virtual void Init();
    virtual void OnConnectSuccess(DWORD session_index);

public:
	inline virtual eSERVER_TYPE	GetServerType()		{ return UNKNOWN_SERVER; }
    bool IsConnected() const { return connected_; }
    bool IsConnector() const { return is_connector_; }
    bool IsManualConnect() const { return manual_connect_mode_; }
    bool IsConnecting() const { return connecting_; }
    // CHANGES: f110519.5L, changes to non-polyporphism interface because of no used cases
    void SetConnecting(bool connecting = true) { connecting_ = connecting; }

	void				SetAddr( char *pszIP, WORD wPort ) { m_strConnectIP = pszIP; m_wConnectPort = wPort; }

public:
	__inline void				Init( ServerFrame * pFrame ) { m_pFrame = pFrame; Init(); }
	virtual void				Release();
	virtual void				Update();							//< 매틱마다 호출이 되어야 하는 부분!!!!
	void						Clear();
    virtual void                Connect() {}

	virtual BOOL				SendPacket( MSG_BASE * pMsg, WORD wSize )			{ return Send( (BYTE*)pMsg, wSize ); }
	virtual BOOL				SendPacket( MSG_BASE_INTERNAL * pMsg, WORD wSize )	{ return Send( (BYTE*)pMsg, wSize ); }

	__inline DWORD				GetSessionIndex()	{ return m_dwSessionIndex; }
	__inline void				SetSessionIndex( DWORD dwIdx ) { m_dwSessionIndex = dwIdx; }

	__inline std::string&		GetConnectIP()		{ return m_strConnectIP; }
	__inline WORD				GetConnectPort()	{ return m_wConnectPort; }

protected:
	virtual void				SendHeartbeatPacket();
	__inline ServerFrame *		GetFrame() { return m_pFrame; }

    void SetIsManualConnect(bool manual_connect_mode) { manual_connect_mode_ = manual_connect_mode; }
private:
    void SetIsConnector(bool connector_flag) { is_connector_ = connector_flag; }
    void SetConnected(bool connected) { connected_ = connected; }
	

	// 연결 정보
	DWORD						m_dwSessionIndex;
	std::string					m_strConnectIP;				//< 연결할 IP
	WORD						m_wConnectPort;				//< 연결할 PORT

    // 연결 상태
    bool connected_; //< 연결되었는지 여부
    bool is_connector_; //< 세션의 특성은 연결형인가?
    bool manual_connect_mode_; //< 수동/자동 연결 여부
    bool connecting_; //< 연결중인가..(연결중이면 연결시도안함)

	ServerFrame *				m_pFrame;
	util::Timer					m_HeartBeatTimer;
	static const int KEEP_ALIVE_DEFAULT_TIME = 10*1000;
};

