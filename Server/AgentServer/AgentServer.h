#pragma once

#include <ServerFrame.h>
#include "Function/ExitProcess.WorkLoad.h"
#include "UserSessions/UserInfo.SecureXignCode.h" // USING_XIGNCODE

// <MACROs>
#define AGENT_SERVER_INFO_FILE_NAME		".\\AgentServer.ini"
#define SHUTDOWN_NOACTION_DELAY_TIME	(10*1000)

// -------------------------------------
// <CONNECT SESSIONs>
class ServerSessionEx;

class MasterServerSession;
class AuthAgentServerSession;
class DBProxyServerSession;
class WorldServerSession;
// -------------------------------------
class ScriptParserManager;
// -------------------------------------

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
//
//	__NA000630_070817_MOLA_5th_UPDATE

#include <UserSessions/IGameResourceInServer.h>

struct sCLIENT_INIT_PRE_ENTRY_WRAP
{
	IWzGResTableInServer	m_GResTable;
#if USING_HACKSHIELD
	AHNHS_SERVER_HANDLE		m_hHANDLEofHS;
#elif USING_XIGNCODE
#elif USING_FROST
#endif
	sCLIENT_INIT_PRE_ENTRY_WRAP()
	{
		sCLIENT_INIT_PRE_ENTRY	Entry( &m_GResTable );
#if USING_HACKSHIELD
		m_hHANDLEofHS = ANTICPX_INVALID_HANDLE_VALUE;
#elif USING_XIGNCODE
#elif USING_FROST
#endif
	};

	inline VOID				SetActiveTransactionUser( User* pUser )
	{
		m_GResTable.SetActiveTransactionUser( pUser );
	}
	inline User*			GetActiveTransactionUser()
	{
		return m_GResTable.GetActiveTransactionUser();
	}
#if USING_HACKSHIELD
	inline AHNHS_SERVER_HANDLE	GetHANDLEofHS() { return m_hHANDLEofHS; }
	inline VOID					SetHANDLEofHS( AHNHS_SERVER_HANDLE hHS ) { m_hHANDLEofHS = hHS; }
#elif USING_XIGNCODE
#elif USING_FROST
#endif
};


//////////////////////////////////////////////////////////////////////////
// <MAIN SERVER FRAME>
class AgentServer : public ServerFrame
{
    USING_DUMMY_CLIENT_FRIEND_ACCESS;
public:		//-------------------------------------
			// <CONSTRUCTOR><DESTRUCTOR>
			AgentServer();
			~AgentServer();

			// <INSTANCE>
            static AgentServer* CreateInstance();
            static AgentServer* GetInstance() { return instance_; }
            static void DestroyInstance();

// =========================================================================================================
// <CALLBACK HANDLERs>
//   콘솔 메시지 콜백
public:
			friend void				CallBackCommand( char * szMessage );

public:
			// Callback Handlers
			static NetworkObject*	CreateServerSideAcceptedObject();
			static VOID				DestroyServerSideAcceptedObject( NetworkObject* pNetworkObject );
			static VOID				DestroyServerSideConnectedObject( NetworkObject* pNetworkObject );
			static NetworkObject*	CreateClientSideAcceptedObject();
			static VOID				DestroyClientSideAcceptedObject( NetworkObject* pNetworkObject );
			static VOID				DestroyClientSideConnectedObject( NetworkObject* pNetworkObject );

// =========================================================================================================
// <SERVERFRAME><INTERFACEs>
public:
			inline eSERVER_TYPE		GetServerType()	{ return AGENT_SERVER;	}

protected:
			virtual BOOL			Init();
			virtual BOOL			Update( DWORD dwTick );
			virtual VOID			Release();
			virtual BOOL			KeyEvent( CHAR ch );
public:

			virtual ServerSession*	AllocServerSession( eSERVER_TYPE serverType );
			virtual VOID			FreeServerSession( ServerSession* pServerSession );
// <SERVERFRAME><INTERFACEs>
// =========================================================================================================
public:
			BOOL					ConnectTo( eSERVER_TYPE serverType );
			BOOL					IsConnectedTo( eSERVER_TYPE eServerType );

			// =========================================================================================================
			// <IMPLEMENTATION><SESSION CONTROLING><CONNECTED SERVER>
			BOOL					SendToServerByIndex( eSERVER_TYPE stServerType, MSG_BASE* pMsg, WORD wSize );
			inline BOOL				SendToMasterServer( MSG_BASE* pMsg, WORD wSize ) { return SendToServerByIndex( MASTER_SERVER, pMsg, wSize ); }
			inline BOOL				SendToDBPServer( MSG_BASE* pMsg, WORD wSize ) { return SendToServerByIndex( GAME_DBPROXY, pMsg, wSize ); }
			BOOL					SendToAuthAgentServer( BYTE* pMsg, WORD wSize );
			inline BOOL				SendToWorldServer( MSG_BASE* pMsg, WORD wSize ) { return SendToServerByIndex( WORLD_SERVER, pMsg, wSize ); }
			// 연결되어있는 필드, 배틀서버에 패킷을 보낸다.
			VOID					SendToGameServer( MSG_BASE* pMsg, WORD wSize );

			MasterServerSession*	SessionMasterServer()		{ return (MasterServerSession*)m_pServerSessions[MASTER_SERVER]; }
			AuthAgentServerSession*	SessionAuthAgentServer()	{ return (AuthAgentServerSession*)m_pServerSessions[AUTH_AGENT_SERVER]; }
			DBProxyServerSession*	SessionDBProxyServer()		{ return (DBProxyServerSession*)m_pServerSessions[GAME_DBPROXY]; }
			WorldServerSession*		SessionWorldServer()		{ return (WorldServerSession*)m_pServerSessions[WORLD_SERVER]; }

			inline eSERVER_STATE	GetDBProxyServerState()		{ return m_eDBPState;	}
			inline VOID				SetDBProxyServerState( eSERVER_STATE eDBPState ){	m_eDBPState = eDBPState;	}
            int GetDBConnectorIndex() const { return db_connector_index_; };
            void ChangeDBConnectorIndex(int db_connector_index) { db_connector_index_ = db_connector_index; }
			CHAR*					GetWorldServerClientIP()	{ return m_pszWorldServerClientIP; }
			WORD					GetWorldServerClientPort()	{ return m_wWorldServerClientPort; }
			BOOL					SetWorldServerClientAddr( CHAR* pIP, WORD wPort )
			{
				if( pIP && wPort )
				{
					strncpy( m_pszWorldServerClientIP, pIP, sizeof(m_pszWorldServerClientIP) );
					m_pszWorldServerClientIP[MAX_IP_STRING_LEN] = '\0';
					m_wWorldServerClientPort = wPort;
					return TRUE;
				}
				else
				{
					memset( m_pszWorldServerClientIP, 0, sizeof(m_pszWorldServerClientIP) );
					m_wWorldServerClientPort = 0;
				}
				return FALSE;
			}

			VOID					StartServerSideListen();
			VOID					StartClientSideListen();
			//
			// =========================================================================================================

			// 인증쪽과 통신시에 필요한...
			WORK_KEY				GetPeriodicWorkKey()		{ return m_wkPeriodicWorkKey; }

public:		static sCLIENT_INIT_PRE_ENTRY_WRAP*		GetGResource();
#if USING_HACKSHIELD
            static AHNHS_SERVER_HANDLE				GetHANDLEofHS();
#elif USING_XIGNCODE
#elif USING_FROST
#endif
public:		BOOL					IsPVPMode(){ return m_bPVPMode; }

// =========================================================================================================
// <IMPLEMENTATION><for SOLAR_AUTH><INTERFACEs>
public:
	inline ePHASE_SHIFT		GetPhaseStatus() { return m_PHASE_SHIFT; }
			inline VOID				SetPhaseStatus( ePHASE_SHIFT phase ) { m_PHASE_SHIFT = phase; }

			static VOID				StartServer();
			VOID					DoFinalize();

private:
			VOID					Init_ClearFields();
			BOOL					Init_PoolInfo();
			BOOL					Init_ScriptData( BOOL bReadFromFile );
			BOOL					Init_Logger();
			BOOL					Init_SecureLogic();
//
// =========================================================================================================

private:
			typedef util::PoolRouter<MAX_SERVER>		SERVER_SESSION_ROUTER;
			SERVER_SESSION_ROUTER&	GetServerPoolRouter() { return *m_pServerPoolRouter; }

//////////////////////////////////////////////////////////////////////////
// <FIELDs>
private:
			SERVER_SESSION_ROUTER*		m_pServerPoolRouter;
			struct
			{	// SolarAuth 관련 FIELDs
				ePHASE_SHIFT			m_PHASE_SHIFT;
				BOOL					m_bServiceMode;
			};

			BOOL						m_bPVPMode;
			ScriptParserManager*		m_pAgentScriptParserManager;

			struct
			{	// Connection 관련 FIELDs
				eSERVER_STATE			m_eDBPState;
                int db_connector_index_; //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
				ServerSessionEx*		m_pServerSessions[MAX_SERVER];
			};

//////////////////////////////////////////////////////////////////////////
// <PROPERTY>
public:
			cEXITINFO_OF_AGENTSERVER	SERVER_EXIT;				// 서버 종료 처리 루틴

private:
			struct 
			{	// 추가 자료형
				BOOL					m_bShutDownProcess;
				DWORD					m_dwShutdownDelayTime;		// 서버 종료 대기 시간
				DWORD					m_dwStartTimeForShutdown;	// 서버 종료 시작 시간
				//////////////////////////////////////////////////////////////////////////
				util::Timer				m_TimerUserCountReport;
			};
			struct
			{
				CHAR					m_pszWorldServerClientIP[MAX_IP_STRING_LEN+1];
				WORD					m_wWorldServerClientPort;
			};

			struct {
				util::Timer				m_BattleJoinTimer;
				util::Timer				m_RegisterTimer;
			};

			struct
			{	// (KEYWORD:CCU) WorkLoad Key
				WORK_KEY				m_wkPeriodicWorkKey;
			};
			//
			//
			//
			//	<RESERVED><RESERVED><RESERVED><RESERVED><RESERVED><RESERVED><RESERVED><RESERVED>
			//
			//
			//
			//
			//
            static AgentServer* instance_;
private:
			// <RESERVED>
			VOID					DisplayPoolInfo();
            //_NA_000000_20150507_TIME_DISPLAY
            VOID                    DisplayCurrentServerTime();
public:
			//inline BOOL				IsShutDown() { return GetStartTimeForShutdown()!=0; }
			//inline VOID				StartShutdown() { SetStartTimeForShutdown(GetTickCount()); }
			//__PROPERTY( DWORD, ShutdownDelayTime );
			//__PROPERTY( DWORD, StartTimeForShutdown );


			//
			//
			//
			//
			//
			//	<RESERVED><RESERVED><RESERVED><RESERVED><RESERVED><RESERVED><RESERVED><RESERVED>
			//
			//
			//

};



