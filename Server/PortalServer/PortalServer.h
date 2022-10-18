//=============================================================================================================================
/// PortalServer class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 8. 22
	@remark
		- 포탈서버의 프레임 워크
	@note
		- 조건 : 싱글 인스턴스
	@history 
		- 
*/
//=============================================================================================================================

#pragma once

#include <ServerFrame.h>

#include <MemoryPoolFactory.h>

#include <TempServerSession.h>

#include "AgentServerSession.h"
#include "DBProxyServerSession.h"
#include "GameServerSession.h"
#include "MasterServerSession.h"
#include "PortalRankingManager.h"

class ScriptParserManager;

class PortalServer : public ServerFrame
{
public:
	static NetworkObject *	DoAllocServerSession();
	static VOID DoFreeServerSession( NetworkObject * pNetObject );
	static VOID DoFreeConnectSession( NetworkObject * pNetObject ){__UNUSED(pNetObject);}

	static NetworkObject *	DoAllocClient_NotUsed(){ return NULL; }
	static VOID DoFreeClient_NotUsed( NetworkObject * pNetObject ){__UNUSED(pNetObject);}
	static VOID DoFreeConnect_NotUsed( NetworkObject * pNetObject ){__UNUSED(pNetObject);}

public:
	PortalServer();
	virtual ~PortalServer(){}

	virtual eSERVER_TYPE	GetServerType() { return PORTAL_SERVER; }

	BOOL					SendDBProxy( MSG_BASE * pMsg, WORD wSize );
	BOOL					IsDBProxyConnected();
	BOOL					SendMaster( MSG_BASE * pMsg, WORD wSize );
	BOOL					IsMasterConnected();

	BOOL					ConnectTo( eSERVER_TYPE type );
	ServerSession *			GetSession( eSERVER_TYPE type );

protected:
	virtual BOOL			Init();
	virtual BOOL			Update( DWORD tick );
	virtual void			Release();

	virtual ServerSession * AllocServerSession( eSERVER_TYPE );
	virtual void			FreeServerSession( ServerSession * );

	virtual BOOL			KeyEvent( CHAR ch );;

	BOOL					Init_ScriptData( BOOL bReadFromFile );

	BOOL					InitializeLogger();

public:
	static VOID				StartServer();
	inline ePHASE_SHIFT		GetPhaseStatus() { return m_PHASE_SHIFT; }
	inline VOID				SetPhaseStatus( ePHASE_SHIFT phase ) { m_PHASE_SHIFT = phase; }

private:
	ePHASE_SHIFT			m_PHASE_SHIFT;


private:
	util::CMemoryPoolFactory<AgentServerSession> m_AgentServerSessionPool;
	util::CMemoryPoolFactory<DBProxyServerSession> m_DBProxyServerSessionPool;
	util::CMemoryPoolFactory<GameServerSession> m_GameServerSessionPool;
	util::CMemoryPoolFactory<TempServerSession> m_TempServerSessionPool;
	util::CMemoryPoolFactory<MasterServerSession> m_MasterServerSessionPool;


	DBProxyServerSession * m_pDBProxySession;
	MasterServerSession * m_pMasterSession;

	int m_WEEKDAY_NUMBER_OF_RANKING_PERIOD;
	ScriptParserManager*		m_pPortalScriptParserManager;
};


extern PortalServer g_PortalServer;