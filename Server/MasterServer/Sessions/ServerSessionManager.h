#ifndef __SERVER_SESSION_MANAGER_H__
#define __SERVER_SESSION_MANAGER_H__

#pragma once

#include <Sessions/ServerSessionEx.h>
//
typedef ServerSessionEx		TypeDefinedServerSession;
#include <Include/ServerTree.h>
//

//
#include <Sessions/AccountDBProxySession.h>
#include <Sessions/AgentServerSession.h>
#include <Sessions/BattleServerSession.h>
#include <Sessions/FieldServerSession.h>
#include <Sessions/GameDBProxySession.h>
#include <Sessions/GameServerSession.h>
#include <Sessions/GuildServerSession.h>
#include <Sessions/OpServerSession.h>
#include <Sessions/PortalServerSession.h>
#include <Sessions/SolarAuthServerSession.h>
#include <Sessions/WorldServerSession.h>

#include <Sessions/TempServerSessionEx.h>


typedef ServerTree<sSERVER_NODE>	ServerTreeNODE;

class ServerSessionManager
{
private:
	ServerSessionManager();
	~ServerSessionManager();
public:
	static BOOL							CreateInstance();
	static BOOL							DestroyInstance();

	//
	//
	static ServerSession*				AllocNewSession( eSERVER_TYPE serverType );
	static BOOL							FreeSession( ServerSession* pSession );

	// <THINKING!>
	//   ServerFrame의 연결 구조를 이용한 방법은 없을까?
	//   SERVER_KEY로 찾을 수 있고, 특정 처리를 수행할 수 있는...

	//
	// <SERVER MANAGEMENT>
	// <WARNING>
	//   TempServer는 추가/삭제하지 않는다
	static BOOL							AddServer( ServerSessionEx* pServer );
	static VOID							RemoveServer( SERVER_KEY skServerKey );
	static ServerSessionEx*				FindServer( SERVER_KEY skServerKey );

	// 특정 서버타입들에 대해 어떠한 처리를 수행토록 하는 기능이 있어야 한다.
	// 아니면 특정 그룹

	static VOID							GetWorldServerList( sPTR_SERVER_NODE_ARRAY* OUT pServerList );
	static VOID							GetAgentServerList( sPTR_SERVER_NODE_ARRAY* OUT pServerList );
	static VOID							GetGameServerList( sPTR_SERVER_NODE_ARRAY* OUT pServerList );
	static VOID							GetGameDBProxyList( sPTR_SERVER_NODE_ARRAY* OUT pServerList );

	static ServerTreeNODE*				GetServerTree();

	//VOID						SendAllServerOnOffInfo( ServerSession *pOPServer );

	//int								AddExitServerList( SERVER_KEY serverKey );	// 종료할 서버 세팅(리턴값은 Add된 서버 개수)

	//inline ServerSession*			GetOpServer() { return m_pOpServer; }

	// 게임서버들에 브로드캐스팅
	static VOID							SendToGameServers( MSG_BASE* pMsg, WORD wSize );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 이것 보단 채널 별.. 등등의 처리가 더 좋은데, 방법을 생각해 보자.
	static VOID		SEND_PHASE_SHIFT_CMD_BROADCAST( ePHASE_SHIFT shiftCMD, ePHASE_SHIFT exceptedStatus );
	static VOID		SEND_PHASE_SHIFT_CMD( ServerSessionEx* pServerSession, ePHASE_SHIFT shiftCMD, ePHASE_SHIFT exceptedStatus );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

private:
	ServerTreeNODE						m_ServerTree;
};


#endif //__SERVER_SESSION_MANAGER_H__

