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
	//   ServerFrame�� ���� ������ �̿��� ����� ������?
	//   SERVER_KEY�� ã�� �� �ְ�, Ư�� ó���� ������ �� �ִ�...

	//
	// <SERVER MANAGEMENT>
	// <WARNING>
	//   TempServer�� �߰�/�������� �ʴ´�
	static BOOL							AddServer( ServerSessionEx* pServer );
	static VOID							RemoveServer( SERVER_KEY skServerKey );
	static ServerSessionEx*				FindServer( SERVER_KEY skServerKey );

	// Ư�� ����Ÿ�Ե鿡 ���� ��� ó���� ������� �ϴ� ����� �־�� �Ѵ�.
	// �ƴϸ� Ư�� �׷�

	static VOID							GetWorldServerList( sPTR_SERVER_NODE_ARRAY* OUT pServerList );
	static VOID							GetAgentServerList( sPTR_SERVER_NODE_ARRAY* OUT pServerList );
	static VOID							GetGameServerList( sPTR_SERVER_NODE_ARRAY* OUT pServerList );
	static VOID							GetGameDBProxyList( sPTR_SERVER_NODE_ARRAY* OUT pServerList );

	static ServerTreeNODE*				GetServerTree();

	//VOID						SendAllServerOnOffInfo( ServerSession *pOPServer );

	//int								AddExitServerList( SERVER_KEY serverKey );	// ������ ���� ����(���ϰ��� Add�� ���� ����)

	//inline ServerSession*			GetOpServer() { return m_pOpServer; }

	// ���Ӽ����鿡 ��ε�ĳ����
	static VOID							SendToGameServers( MSG_BASE* pMsg, WORD wSize );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// �̰� ���� ä�� ��.. ����� ó���� �� ������, ����� ������ ����.
	static VOID		SEND_PHASE_SHIFT_CMD_BROADCAST( ePHASE_SHIFT shiftCMD, ePHASE_SHIFT exceptedStatus );
	static VOID		SEND_PHASE_SHIFT_CMD( ServerSessionEx* pServerSession, ePHASE_SHIFT shiftCMD, ePHASE_SHIFT exceptedStatus );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

private:
	ServerTreeNODE						m_ServerTree;
};


#endif //__SERVER_SESSION_MANAGER_H__

