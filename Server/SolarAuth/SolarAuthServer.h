#pragma once

//=============================================================================================================================
/// SolarAuthServer class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 08. 11
	@remark
	- SolarAuthServer
	@note
	@history 
	-  
*/
//=============================================================================================================================

//#include <ServiceApp.h>
#include <ServerFrame.h>
#include "Sessions/ServerSessionManager.h"
#include "Sessions/TempServerSession.h"
#include "Sessions/MasterServerSession.h"
#include "Sessions/AccountDBProxySession.h"

class ServerSession;

class SolarAuthServer : public ServerFrame
{
public:
	enum eIOHANDLER { INSIDE_IOHANDLER = 0, OUTSIDE_IOHANDLER = 1, NUM_IOHANDER = 2, BOTH_IOHANDLER = 2, };
public:
	// <CONSTRUCTOR><DESTRUCTOR>
	SolarAuthServer();
	~SolarAuthServer();

	static SolarAuthServer*	GetMainFrame();
public:
	// ServerFrame으로 전환하면서 차례대로 정리
	static NetworkObject*	Handler_AllocServerSession_NoInit( eSERVER_TYPE serverType );
	static VOID				Handler_FreeServerSession_NoRelease( NetworkObject* pNetworkObject );

	static ServerSession*	Handler_AllocServerSession( eSERVER_TYPE serverType );
	static VOID				Handler_FreeServerSession( ServerSession* pServerSession );

	// Callback Handlers
	static NetworkObject*	CreateServerSideAcceptedObject();
	static VOID				DestroyServerSideAcceptedObject( NetworkObject* pNetworkObject );
	static VOID				DestroyServerSideConnectedObject( NetworkObject* pNetworkObject );

public:
	virtual eSERVER_TYPE	GetServerType() { return SOLARAUTH_SERVER; }

protected:
	virtual BOOL			Init();
	virtual BOOL			Update( DWORD tick );
	virtual VOID			Release();

	virtual BOOL			KeyEvent( CHAR ch );

	virtual ServerSession*	AllocServerSession( eSERVER_TYPE serverType );
	virtual VOID			FreeServerSession( ServerSession* pServerSession );

public:
	// <NETWORK><MAINTAIN>
	//VOID					Listen( eIOHANDLER where );
	VOID					Connect();
	VOID					Connect( const ServerSession* pNetObject, CHAR* IN pszIP, const WORD wPort );

	VOID					StartServer();

	// 이것은 ServerFrame을 상속하면서 생긴 Release 호출 이후에 발생하는 새로운 Instance를 제거하기 위함
	VOID					Finalize();
public:
	// <RUNTIME ENVIRONMENT>
	// <INTERFACEs><INFOs><Get/Set>
	inline BOOL				GetServiceMode() { return m_bServiceMode; }
	inline eSERVER_MODE		GetServerMode() { return m_ServerMode; }
	inline WORK_KEY			GetReportWorkKey() { return m_wkReportWorkKey; }
	BOOL					InitializeLogger();

	AccountDBProxySession*&	AccountDBP() { return m_pADBPSession; }

public:
	// CONSOLE INPUT/OUTPUT
	BOOL					ProcessConsole();
	VOID					DisplayServerInfo();
	VOID					DisplayServerListInfo();
	VOID					DisplayPoolInfo();

private:
	// Callback Handlers

private:
#pragma warning(push)
#pragma warning(disable : 4201)
	// <FIELDs>
	BOOL					m_bShutdown;
	GLOBAL_SERVER_KEY		m_GlobalServerKey;

	struct
	{	// Server Runtime Control Flags
		BOOL					m_bServiceMode;
		eSERVER_MODE			m_ServerMode;
	};
	struct
	{
		WORK_KEY				m_wkReportWorkKey;
	};
	AccountDBProxySession*		m_pADBPSession;

	struct
	{	// move from SessionFactory
		util::CMemoryPoolFactory<TempServerSessionEx>		m_TempServerPool;
		util::CMemoryPoolFactory<MasterServerSession>		m_MasterServerPool;
		util::CMemoryPoolFactory<AccountDBProxySession>	m_ADBPServerPool;
	};
#pragma warning(pop)
};


