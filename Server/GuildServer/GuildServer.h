#pragma once

//
#include <ServerFrame.h>
#include <TempServerSession.h>

#define GUILD_SERVER_INFO_FILE_NAME	".\\GuildServer.ini"

// -------------------------------------
// <Connect Sessions><MasterToServer>
class ServerSession;
class MasterServerSession;
class DBProxyServerSession;
class WorldServerSession;
// -------------------------------------

class ScriptParserManager;

class GuildServer : public ServerFrame
{
public:
	// <CONSTRUCTOR><DESTRUCTOR>
	GuildServer();
	~GuildServer();

	static GuildServer*		GetMainFrame();

public:
	// Callback Handlers
	static NetworkObject*	CreateServerSideAcceptedObject();
	static VOID				DestroyServerSideAcceptedObject( NetworkObject* pNetworkObject );
	static VOID				DestroyServerSideConnectedObject( NetworkObject* pNetworkObject );

	// =========================================================================================================
	// <IMPLEMENTATION><SERVERFRAME><INTERFACEs>
public:
	inline eSERVER_TYPE		GetServerType()	{ return GUILD_SERVER;	}

protected:
	
	virtual BOOL			Init();
	virtual BOOL			Update( DWORD tick );
	virtual VOID			Release();
	virtual BOOL			KeyEvent( CHAR ch );
			VOID			DisplayServerInfo();
public:
	
	virtual ServerSession*	AllocServerSession( eSERVER_TYPE serverType );
	virtual VOID			FreeServerSession( ServerSession* pServerSession );
	// <IMPLEMENTATION><SERVERFRAME><INTERFACE>
	// =========================================================================================================
public:
	//
	//BOOL ServerFrame::SendToSession( DWORD idx, MSG_BASE * pMsg, WORD wSize )
	//void ServerFrame::SendToSessionType( eSERVER_TYPE type, MSG_BASE * pMsg, WORD wSize, DWORD dwExceptIndex )
	VOID					SendToAllGameServers( MSG_BASE* pMsg, WORD wSize );
	BOOL					ConnectTo( eSERVER_TYPE serverType );
	// =========================================================================================================
	// <IMPLEMENTATION><for SOLAR_AUTH><INTERFACEs>
public:
	inline ePHASE_SHIFT		GetPhaseStatus() { return m_PHASE_SHIFT; }
	inline VOID				SetPhaseStatus( ePHASE_SHIFT phase ) { m_PHASE_SHIFT = phase; }

	static VOID				StartServer();
private:
	BOOL					Init_ScriptData( BOOL bReadFromFile );
	BOOL					InitializeLogger();
	//
	// =========================================================================================================

private:
	typedef util::PoolRouter<MAX_SERVER>	SERVER_SESSION_ROUTER;
	SERVER_SESSION_ROUTER&	GetPoolRouter() { return *m_pPoolRouter; }

private:
	ePHASE_SHIFT			m_PHASE_SHIFT;
	BOOL					m_bServiceMode;

public:
	class sConnectSession
	{
	private:
		MasterServerSession*	m_pMasterServerSession;
		DBProxyServerSession*	m_pDBProxyServerSession;
		WorldServerSession*		m_pWorldServerSession;

	public:
		BOOL				SendToMaster( MSG_BASE* pMsg, WORD wSize );
		BOOL				SendToDBProxy( MSG_BASE* pMsg, WORD wSize );
		BOOL				SendToWorld( MSG_BASE* pMsg, WORD wSize );

		BOOL				IsMasterConnected();
		BOOL				IsDBProxyConnected();
		BOOL				IsWorldConnected();

		MasterServerSession*&	SessionMaster() { return m_pMasterServerSession; }
		DBProxyServerSession*&	SessionDBProxy() { return m_pDBProxyServerSession; }
		WorldServerSession*&	SessionWorld() { return m_pWorldServerSession; }

		VOID				Init()
		{
			m_pMasterServerSession = NULL;
			m_pDBProxyServerSession = NULL;
			m_pWorldServerSession = NULL;
		}
	} ConnectSessions;

	//Timer
	void GuildDestoryTimeStart() { m_GuildDestroyTimer.SetTimer(MAX_GUILDDESTROY_TIME); }
	void GuildDestoryTimeStop() { m_GuildDestroyTimer.DisableCheckTime();}

private:
	SERVER_SESSION_ROUTER*								m_pPoolRouter;
	util::CMemoryPoolFactory<TempServerSession>			m_TempServerSessionPool;
	ScriptParserManager*								m_pGuildScriptParserManager;

#ifdef _DEBUG
	enum { MAX_GUILDDESTROY_TIME = 10*1000 }; /* 1Ка */
#else
	enum { MAX_GUILDDESTROY_TIME = 5*60*1000 }; /* 5Ка */
#endif
	util::Timer	 m_GuildDestroyTimer;
};


