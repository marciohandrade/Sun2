#include "stdafx.h"

#include "ServerSessionManager.h"
#include <MemoryPoolFactoryV.h>
#include <MasterServerEx.h>
#include "SolarAuthServerSession.h"

typedef util::PoolRouter<MAX_SERVER>	SERVER_SESSION_ROUTER;

static struct sServerSessionManagerInternal
{
	ServerSessionManager*	m_ServerSessionManager;
	SERVER_SESSION_ROUTER*	m_pPoolRouter;
} s_ServerSessionManagerInternal =
{
	NULL,
};


ServerSessionManager::ServerSessionManager()
{
	s_ServerSessionManagerInternal.m_pPoolRouter = new SERVER_SESSION_ROUTER;

	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	SERVER_SESSION_ROUTER& rRouter = *rINTERNAL.m_pPoolRouter;

	//rRouter.Register<UNKNOWN_SERVER>
	rRouter.Register<AgentServerSession>(AGENT_SERVER);
	rRouter.Register<FieldServerSession>(FIELD_SERVER);
	rRouter.Register<BattleServerSession>(BATTLE_SERVER);
	rRouter.Register<GameDBProxySession>(GAME_DBPROXY);
	rRouter.Register<WorldServerSession>(WORLD_SERVER);
	//rRouter.Register<MasterServerSession>(MASTER_SERVER);
	rRouter.Register<GuildServerSession>(GUILD_SERVER);
	rRouter.Register<AccountDBProxySession>(ACCOUNT_DBPROXY);
	//rRouter.Register<TOURNAMENT_SERVER);
	//rRouter.Register<AUTH_AGENT_SERVER);
	rRouter.Register<OpServerSession>(OP_SERVER);
	rRouter.Register<TempServerSessionEx>(TEMP_SERVER);
	//rRouter.Register<GameServerSession>(GAME_SERVER);
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
	rRouter.Register<PortalServerSession>(PORTAL_SERVER);
#endif
	rRouter.Register<SolarAuthServerSession>(SOLARAUTH_SERVER);

	rRouter.Initialize( AGENT_SERVER, 50, 10 );
	rRouter.Initialize( FIELD_SERVER, 100, 10 );
	rRouter.Initialize( BATTLE_SERVER, 200, 20 );
	rRouter.Initialize( GAME_DBPROXY, 10, 2 );
	rRouter.Initialize( WORLD_SERVER, 10, 2 );
	rRouter.Initialize( GUILD_SERVER, 10, 2 );
	rRouter.Initialize( ACCOUNT_DBPROXY, 1, 1 );
	rRouter.Initialize( OP_SERVER, 1, 1 );
	rRouter.Initialize( TEMP_SERVER, 200, 10 );
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
	rRouter.Initialize( PORTAL_SERVER, 10, 2 );
#endif
	rRouter.Initialize( SOLARAUTH_SERVER, 1, 1 );

	SolarAuthServerSessionCrypted::CreateCode();
}

ServerSessionManager::~ServerSessionManager()
{
	//m_ServerTree �� �˾Ƽ� ���ŵ� ���̴�.
    
    // (CHANGES)(20100312)(hukim) m_pPoolRouter�� �����ڿ��� �����ϰ� �����Ƿ� �Ҹ� ������ �Ҹ��ڰ�
    // �ȴ�. m_ServerSessionManager�� CreateInstance()���� �����ϰ� DestroyInstance()���� �Ҹ�ȴ�.

    SERVER_SESSION_ROUTER*& session_router = s_ServerSessionManagerInternal.m_pPoolRouter;
    SAFE_DELETE(session_router);
}

BOOL	ServerSessionManager::CreateInstance()
{
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;

	if( rManager == NULL )
		rManager = new ServerSessionManager();

	return TRUE;
}

BOOL	ServerSessionManager::DestroyInstance()
{
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;

	SAFE_DELETE( rManager );

	return TRUE;
}

//====================================================================================
// <ServerSession><ALLOC><DEALLOC>
//
ServerSession*
ServerSessionManager::AllocNewSession( eSERVER_TYPE serverType )
{
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	SERVER_SESSION_ROUTER& rRouter = *rINTERNAL.m_pPoolRouter;

	ServerSession* pSession = (ServerSession*)rRouter.Alloc( serverType );

	return pSession;
}

BOOL
ServerSessionManager::FreeSession( ServerSession* pSession )
{
	eSERVER_TYPE serverType = pSession->GetServerType();

	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	SERVER_SESSION_ROUTER& rRouter = *rINTERNAL.m_pPoolRouter;

	return rRouter.Free( (DWORD)serverType, pSession );
}
//
//====================================================================================

BOOL	ServerSessionManager::AddServer( ServerSessionEx* pServer )
{
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;
	ServerTreeNODE& rTREE = rManager->m_ServerTree;

	SERVER_KEY skServerKey = pServer->GetServerKey();

	ServerSessionEx* pFindSession = rManager->FindServer( skServerKey );

	if( pFindSession != NULL )
	{
		SERVER_KEY& rServerKey = pServer->GetServerKey();

		MessageOut( eCRITICAL_LOG,
			_T("Warning: ����Ű �ߺ�(%u/%u/%u/%u)�̹Ƿ� ������ �����ϴ�. DB ServerInfo ���̺� ������ Ȯ���ϼ���."),
			rServerKey.GetWorldID(), rServerKey.GetChannelID(), rServerKey.GetServerType(), rServerKey.GetServerID() );

		// <WARNING!>
		//   �� ��ƾ�� ���� �־ ������ ���°�?
		pServer->Disconnect();

		return FALSE;
	}

	eSERVER_TYPE stType = pServer->GetServerType();

	sSERVER_NODE node;
	node.m_ServerType = stType;
	node.m_ServerKey = skServerKey;
	node.m_pServerSession = pServer;
	rTREE.AddServer( skServerKey, &node );

	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "[AddServer] ServerType[%d] \n", pServer->GetServerType() ); );

	//------------------------------------------------------------------------------------
	// ���� ��ƾ�� OP_SERVER�� �߰� ����� ������ ���� ������ ������ ��ƾ�̴�.
	// 1) �߰��Ǵ� ������ OP_SERVER�� ��츦 �����ϰ�,
	// 2) OP_SERVER�� ����� ���¿���
	// 3) OP_SERVER�� ���� �߰� ����� ������ ���� ������ ������.

	if( OP_SERVER == stType )
		return TRUE;

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	OpServerSession* pOpServer = pMasterServer->ConnectSessions.SessionOpServer();

	if( pOpServer && pOpServer->IsConnected() )
	{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_MO_RTTG_ALL_SERVER_STATUS_NTF* pMsgNTF =
			new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MO_RTTG_ALL_SERVER_STATUS_NTF;
#else
		MSG_MO_RTTG_ALL_SERVER_STATUS_NTF pMsgNTF[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

		pMsgNTF->m_Count = 1;

		MSG_MO_RTTG_ALL_SERVER_STATUS_NTF::ServerConnectInfo& rConnectInfo = pMsgNTF->m_ServerConnectInfo[0];

		SERVER_KEY skOldServerKey = SWAP_32BIT_DATATYPE((DWORD)skServerKey);
		rConnectInfo.m_ServerKey = skOldServerKey;
		rConnectInfo.m_bConnected = (BYTE)TRUE;

		pOpServer->Send( (BYTE*)pMsgNTF, (WORD)pMsgNTF->GetSize() );
	}

	return TRUE;
}

VOID	ServerSessionManager::RemoveServer( SERVER_KEY skServerKey )
{
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;
	ServerTreeNODE& rTREE = rManager->m_ServerTree;

	sSERVER_NODE* pNODE = rTREE.FindServer( skServerKey );

	if( !(pNODE && pNODE->m_pServerSession) )
	{
		//MessageOut( eCRITICAL_LOG,
		//	_T("�߸��� ����Ű�̰ų� ���� ���� ����ȭ�� ������ ���ɼ� ���� ����Ű [%u:%u:%u:%u]"),
		//	skServerKey.GetWorldID(), skServerKey.GetChannelID(), skServerKey.GetServerType(), skServerKey.GetServerID()
		//	);

		return;
	}

	ServerSessionEx* pSession = pNODE->m_pServerSession;	//�����
	rTREE.RemoveServer( skServerKey );

	//------------------------------------------------------------------------------------------------
	// TempServerSession�� �Ŵ����� �߰����� �ʴ´�.(���� ���� �������� Redirect�� �Ŀ� �߰���)
	// �׷��ϱ� TempServerSesssion::OnDisconnect���� �� �Լ��� ȣ���ص� ��������.
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------
	// ���� ��ƾ�� OP_SERVER�� ���� ������ ������ ��쿡 ���� ������ ������ ��ƾ�̴�.
	// 1) �߰��Ǵ� ������ OP_SERVER�� ��츦 �����ϰ�,
	// 2) GAME_SERVER�� ���� Money���� ������ �����ϸ�,
	// 3) OP_SERVER�� ����� ���¿���
	// 4) OP_SERVER�� ���� ������ ������ ������ ���� ������ ������.
	eSERVER_TYPE stType = pSession->GetServerType();

	if( OP_SERVER == stType )
		return;

	//const DWORD bitMask = (DWORD)( (1<<GAME_SERVER) | (1<<BATTLE_SERVER) | (1<<FIELD_SERVER) );
	//if( bitMask & (DWORD)(1<<stType) )
	//{
	//	//��ȭ�� ���� ���� ����...
	//	// ���� �������� �ּ�ó���� �κ��̹Ƿ� Ȯ���ϰ� �����ϱ�� �Ѵ�.
	//}

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	OpServerSession* pOpServer = pMasterServer->ConnectSessions.SessionOpServer();

	if( pOpServer && pOpServer->IsConnected() )
	{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		MSG_MO_RTTG_ALL_SERVER_STATUS_NTF* pMsgNTF =
			new (PACKET_CONTROL_SM::GetBasePacketPtr4Send()) MSG_MO_RTTG_ALL_SERVER_STATUS_NTF;
#else
		MSG_MO_RTTG_ALL_SERVER_STATUS_NTF pMsgNTF[1];
#endif __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

		pMsgNTF->m_Count = 1;

		MSG_MO_RTTG_ALL_SERVER_STATUS_NTF::ServerConnectInfo& rConnectInfo = pMsgNTF->m_ServerConnectInfo[0];

		SERVER_KEY skOldServerKey = SWAP_32BIT_DATATYPE((DWORD)skServerKey);
		rConnectInfo.m_ServerKey = skOldServerKey;
		rConnectInfo.m_bConnected = (BYTE)FALSE;

		pOpServer->Send( (BYTE*)pMsgNTF, (WORD)pMsgNTF->GetSize() );
	}
}

ServerSessionEx*	ServerSessionManager::FindServer( SERVER_KEY skServerKey )
{
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;
	ServerTreeNODE& rTREE = rManager->m_ServerTree;

	sSERVER_NODE* pNode = rTREE.FindServer( skServerKey );

	if( pNode )
		return pNode->m_pServerSession;

	return NULL;
}

// Ư�� ����Ÿ�Ե鿡 ���� ��� ó���� ������� �ϴ� ����� �־�� �Ѵ�.
// �ƴϸ� Ư�� �׷�

VOID	ServerSessionManager::GetWorldServerList( sPTR_SERVER_NODE_ARRAY* OUT pServerList )
{
	const DWORD bitMask = (DWORD)(1<<WORLD_SERVER);
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;
	ServerTreeNODE& rTREE = rManager->m_ServerTree;

	rTREE.GetPtrList( bitMask, pServerList );
}

VOID	ServerSessionManager::GetAgentServerList( sPTR_SERVER_NODE_ARRAY* OUT pServerList )
{
	const DWORD bitMask = (DWORD)(1<<AGENT_SERVER);
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;
	ServerTreeNODE& rTREE = rManager->m_ServerTree;

	rTREE.GetPtrList( bitMask, pServerList );
}

VOID	ServerSessionManager::GetGameServerList( sPTR_SERVER_NODE_ARRAY* OUT pServerList )
{
	const DWORD bitMask = (DWORD)( (1<<GAME_SERVER) | (1<<FIELD_SERVER) | (1<<BATTLE_SERVER) );
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;
	ServerTreeNODE& rTREE = rManager->m_ServerTree;

	rTREE.GetPtrList( bitMask, pServerList );
}

VOID	ServerSessionManager::GetGameDBProxyList( sPTR_SERVER_NODE_ARRAY* OUT pServerList )
{
	const DWORD bitMask = (DWORD)(1<<GAME_DBPROXY);
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;
	ServerTreeNODE& rTREE = rManager->m_ServerTree;

	rTREE.GetPtrList( bitMask, pServerList );
}

ServerTreeNODE*		ServerSessionManager::GetServerTree()
{
	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;
	return &(rManager->m_ServerTree);
}

// ���� ����� ���Ӽ����鿡 ��ε�ĳ����
VOID	ServerSessionManager::SendToGameServers( MSG_BASE* pMsg, WORD wSize )
{
	const DWORD bitMask = (DWORD)( (1<<GAME_SERVER) | (1<<FIELD_SERVER) | (1<<BATTLE_SERVER) );

	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;

	ServerTreeNODE* pTREE = rManager->GetServerTree();

	DWORD dwUpperBound;
	sSERVER_NODE* pNODEs = pTREE->GetServerArrayList( dwUpperBound );
	sSERVER_NODE* pNODE = pNODEs;

	for( DWORD i=0 ; i<dwUpperBound ; ++i, ++pNODE )
	{
		if( ((DWORD)(1<<pNODE->m_ServerType)) & bitMask )
		{
			ServerSessionEx* pSession = pNODE->m_pServerSession;
			if( pSession )
			{
				pSession->SendPacket( pMsg, wSize );
			}
		}
	}
}

//VOID						SendAllServerOnOffInfo( ServerSession *pOPServer );

//int								AddExitServerList( SERVER_KEY serverKey );	// ������ ���� ����(���ϰ��� Add�� ���� ����)

//inline ServerSession*			GetOpServer() { return m_pOpServer; }

//VOID							SendGameServer( BYTE* pMsg, WORD wSize );	// ���Ӽ����鿡 ��ε�ĳ����


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

#include <Sessions/ServerSessionEx.h>
#include <PacketHandler/PacketHandler.Shared.h>

VOID	ServerSessionManager::SEND_PHASE_SHIFT_CMD(
											ServerSessionEx* pServerSession,
											ePHASE_SHIFT shiftCMD,
											ePHASE_SHIFT exceptedStatus
											)
{
	if( !pServerSession )
		return;

	// �� �Լ��� TempServerSession�� ������� �ʴ´�.

	Services4Session& rSERVICE = pServerSession->Services();
	if( rSERVICE.GetPhaseStatus() & exceptedStatus )
		return;

	PacketHandlerShared::SEND_SERVERCOMMON_PHASESHIFT_CMD( pServerSession, shiftCMD );
}

VOID	ServerSessionManager::SEND_PHASE_SHIFT_CMD_BROADCAST(
													ePHASE_SHIFT shiftCMD,
													ePHASE_SHIFT exceptedStatus
													)
{	// PHASE_SHIFT_CMD�� ���� CATEGORY&PROTOCOL�� ������

	sServerSessionManagerInternal& rINTERNAL = s_ServerSessionManagerInternal;
	ServerSessionManager*& rManager = rINTERNAL.m_ServerSessionManager;
	ServerTreeNODE& rTREE = rManager->m_ServerTree;

	DWORD dwUpperBound;
	sSERVER_NODE* pNODEs = rTREE.GetServerArrayList( dwUpperBound );
	sSERVER_NODE* pNODE = pNODEs;

	for( DWORD i=0 ; i<dwUpperBound ; ++i, ++pNODE )
	{
		if( UNKNOWN_SERVER == pNODE->m_ServerType ) continue;
		ServerSessionEx* pServerSession = pNODE->m_pServerSession;

		SEND_PHASE_SHIFT_CMD( pServerSession, shiftCMD, exceptedStatus );
	}
}


#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

