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
	//m_ServerTree 는 알아서 제거될 것이다.
    
    // (CHANGES)(20100312)(hukim) m_pPoolRouter를 생성자에서 생성하고 있으므로 소멸 시점은 소멸자가
    // 된다. m_ServerSessionManager는 CreateInstance()에서 생성하고 DestroyInstance()에서 소멸된다.

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
			_T("Warning: 서버키 중복(%u/%u/%u/%u)이므로 연결을 끊습니다. DB ServerInfo 테이블 정보를 확인하세요."),
			rServerKey.GetWorldID(), rServerKey.GetChannelID(), rServerKey.GetServerType(), rServerKey.GetServerID() );

		// <WARNING!>
		//   이 루틴이 여기 있어도 문제가 없는가?
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
	// 다음 루틴은 OP_SERVER에 추가 연결된 서버에 대한 정보를 보내는 루틴이다.
	// 1) 추가되는 서버가 OP_SERVER인 경우를 제외하고,
	// 2) OP_SERVER가 연결된 상태에서
	// 3) OP_SERVER에 현재 추가 연결된 서버에 대한 정보를 보낸다.

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
		//	_T("잘못된 서버키이거나 서버 관리 동기화가 깨졌을 가능성 존재 서버키 [%u:%u:%u:%u]"),
		//	skServerKey.GetWorldID(), skServerKey.GetChannelID(), skServerKey.GetServerType(), skServerKey.GetServerID()
		//	);

		return;
	}

	ServerSessionEx* pSession = pNODE->m_pServerSession;	//백업용
	rTREE.RemoveServer( skServerKey );

	//------------------------------------------------------------------------------------------------
	// TempServerSession은 매니져에 추가되지 않는다.(실제 사용될 세션으로 Redirect된 후에 추가됨)
	// 그러니까 TempServerSesssion::OnDisconnect에서 이 함수를 호출해도 무시하자.
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------
	// 다음 루틴은 OP_SERVER에 서버 연결이 끊어진 경우에 대한 정보를 보내는 루틴이다.
	// 1) 추가되는 서버가 OP_SERVER인 경우를 제외하고,
	// 2) GAME_SERVER의 경우는 Money관련 정보를 제거하며,
	// 3) OP_SERVER가 연결된 상태에서
	// 4) OP_SERVER에 현재 연결이 끊어진 서버에 대한 정보를 보낸다.
	eSERVER_TYPE stType = pSession->GetServerType();

	if( OP_SERVER == stType )
		return;

	//const DWORD bitMask = (DWORD)( (1<<GAME_SERVER) | (1<<BATTLE_SERVER) | (1<<FIELD_SERVER) );
	//if( bitMask & (DWORD)(1<<stType) )
	//{
	//	//통화량 관련 정보 제거...
	//	// 기존 로직에서 주석처리된 부분이므로 확인하고 설정하기로 한다.
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

// 특정 서버타입들에 대해 어떠한 처리를 수행토록 하는 기능이 있어야 한다.
// 아니면 특정 그룹

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

// 현재 연결된 게임서버들에 브로드캐스팅
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

//int								AddExitServerList( SERVER_KEY serverKey );	// 종료할 서버 세팅(리턴값은 Add된 서버 개수)

//inline ServerSession*			GetOpServer() { return m_pOpServer; }

//VOID							SendGameServer( BYTE* pMsg, WORD wSize );	// 게임서버들에 브로드캐스팅


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

	// 이 함수는 TempServerSession에 적용되지 않는다.

	Services4Session& rSERVICE = pServerSession->Services();
	if( rSERVICE.GetPhaseStatus() & exceptedStatus )
		return;

	PacketHandlerShared::SEND_SERVERCOMMON_PHASESHIFT_CMD( pServerSession, shiftCMD );
}

VOID	ServerSessionManager::SEND_PHASE_SHIFT_CMD_BROADCAST(
													ePHASE_SHIFT shiftCMD,
													ePHASE_SHIFT exceptedStatus
													)
{	// PHASE_SHIFT_CMD는 공통 CATEGORY&PROTOCOL로 정하자

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

