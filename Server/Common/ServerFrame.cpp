#include "stdafx.h"
#include <conio.h>
#include ".\ServerFrame.h"

#include <time.h>

#include <algorithm>
#include <functional_custom.h>

#include <Version.h>
#include <ServerBuildVersion.h>
#include <ServerGlobal.h>
#include <PacketStruct.h>
#include <PacketStruct_ServerCommon.h>
#include <PacketControl/PacketControlS2S.h>

#include <SafeMemoryPoolFactory.h>

#include "ServerSession.h"

// custom
#include <TestResultStdErr.h>
#include <TestRegistry.h>


//==================================================================================================
//	<PROTOTYPEs>
static VOID ReadIOCPDescFromFile(const CHAR szSectionName[], IOHANDLER_DESC& desc, const CHAR* ini );
static void RedirectAssertionHandler(TCHAR* msg, TCHAR* file_name, DWORD line_number);
static void RedirectProblemMemoryReporter(const TCHAR* type_name, void* mem_ptr, size_t mem_size);

//==================================================================================================
void util::ServerTimeInfoPerFrame::Init()
{
    // (CHANGES) (f100415.1L) (WAVERIX) fix up delta tick error related to degree of accuracy
    QueryPerformanceCounter(&time_counter_for_start_);
    time_counter_for_latest_ = time_counter_for_start_;
    QueryPerformanceFrequency(&frequency_);
    // (f100611.3L) change to use sync timer on server
    util::TimeSync::Command_Standalone();
    util::TimeSync::UpdateTimeInfoPerFrame(this);
}

void util::ServerTimeInfoPerFrame::Update()
{
    // CHANGES: f110805.1L, globally server time information accessability per frame.
    util::TimeSync::UpdateTimeInfoPerFrame(this);
    // (CHANGES) (f100415.1L) (WAVERIX) fix up delta tick error related to degree of accuracy
    LARGE_INTEGER current_counter;
    QueryPerformanceCounter(&current_counter);
    QueryPerformanceFrequency(&frequency_); // correct multi-processor integrity
    // prevent irregular errors
    if (time_counter_for_latest_.QuadPart > current_counter.QuadPart) {
        current_counter = time_counter_for_latest_;
    };
    ULONGLONG lifetime_offset = current_counter.QuadPart - time_counter_for_start_.QuadPart;
    ;         lifetime_offset *= 1000; //convert to milliseconds
    ULONGLONG pre_offset = time_counter_for_latest_.QuadPart - time_counter_for_start_.QuadPart;
    ;         pre_offset *= 1000; //convert to milliseconds
    ULONGLONG delta_offset = lifetime_offset - pre_offset;
    delta_tick_ = static_cast<DWORD>(delta_offset / frequency_.QuadPart);
    ;         delta_offset %= frequency_.QuadPart;
    ;         delta_offset /= 1000; // the value for a correct loss time
    time_counter_for_latest_.QuadPart = current_counter.QuadPart - delta_offset;
    // NOTE: f110822.2L, increasing 1 per second
    aging_counter_ = static_cast<ulong>((lifetime_offset / frequency_.QuadPart) / 1000);

}
//==================================================================================================
//	<SERVER FRAME IMPLEMENTATION>
ServerFrame::ServerFrame(VOID)
	:	m_Guid(0)
	,	m_Key(0)
	,	m_bShutdown(FALSE)
	,	m_bNetworkBlock4Closing(FALSE)
	//,	m_ReconnectTimer
	,	m_dwFPS(0)
	,	m_wListenPort_s(0)
	,	m_wListenPort_c(0)
	,	m_pIOCPServer(NULL)
	,	m_bGetServerInfoFromFile(FALSE)
    , m_freq(time_info_per_frame_.frequency_)
{
    srand((unsigned)time(NULL)); // C Runtime Library Random Seed 초기화

	m_pszListenIP_s[0]='\0';
	m_pszListenIP_c[0]='\0';
    time_info_per_frame_.Init();
}

ServerFrame::~ServerFrame(VOID) 
{
}

BOOL		ServerFrame::DoInit(
				   const CHAR* name,
				   fnCallBackCreateAcceptedObject		FnAllocAcceptServer,
				   fnCallBackDestroyAcceptedObject		FnFreeAcceptServer,
				   fnCallBackDestroyConnectedObject		FnFreeConnectServer,
				   fnCallBackCreateAcceptedObject		FnAllocAcceptClient,
				   fnCallBackDestroyAcceptedObject		FnFreeAcceptClient,
				   fnCallBackDestroyConnectedObject		FnFreeConnectClient
				   )
{
	strncpy( m_pszDescFileName, name, sizeof(m_pszDescFileName) );

#ifdef __NA000000_071105_ASSERT_LOGGING_REDIRECTION__
	FlowControl::RegisterAssertionHandler( RedirectAssertionHandler );
    util::SafeMemoryPoolFactoryShared::RegisterLoggingHandler(&RedirectProblemMemoryReporter);
#endif
	nsPacket::Server::Environment::Init();

	// [COMMON] -----------------------------------------------------------------------------------------------
	INT ReconnectDelayTime = 0;
	__READ_CONFIG_NUMBER( m_Mode, "ATTRIBUTES", "SERVER_MODE", 0, name );
	__READ_CONFIG_NUMBER( m_bGetServerInfoFromFile, "ATTRIBUTES", "GET_SERVERINFO_FROM_FILE", 0, name );
	__READ_CONFIG_NUMBER( m_HandlerMask, "ATTRIBUTES", "IO_HANDLER_MASK", 0, name );
	__READ_CONFIG_NUMBER( ReconnectDelayTime, "ATTRIBUTES", "RECONNECT_DELAY_TIME", 0, name );


	DWORD dwErrConfig = 0;
	if( !m_Mode || !m_HandlerMask )
		dwErrConfig |= 1;
		
	if(!ReconnectDelayTime || ReconnectDelayTime < 1000)
		dwErrConfig |= 2;

	if( dwErrConfig )
	{
		DISPMSG( "Please Check xxxServer.ini\n" );
		if( dwErrConfig& 3 )
			DISPMSG( "\t[ATTRIBUTES] Is exact tab name? \n" );
		if( dwErrConfig& 1 )
			DISPMSG( "\t[ATTRIBUTES] Check value : SERVER_MODE -or- IO_HANDLER_MASK\n" );
		if( dwErrConfig& 2 )
			DISPMSG( "\t[ATTRIBUTES] RECONNECT_DELAY_TIME value (0 -or- less than 1000)\n" );
		system( "pause" );

		return FALSE;
	}

    // NOTE: the interval value of automatic reconnect is enough
    // to support stable starting considering a server initialization preparations
    m_ReconnectTimer.SetTimer(5 * 1000);
	UpdateAppTitle();

    // CHANGES: f110805.1L
    time_info_per_frame_.Init();

	// [CLIENT_IOHANDLER] or [SERVER_IOHANDLER] ---------------------------------------------------------------
	DISPMSG( "IOCP Initialize......" );
	m_pIOCPServer = new IOCPServer;

	INT nHandler = 0;
	IOHANDLER_DESC desc[2];
	if( m_HandlerMask & SERVER_IOHANDLER )
	{
		desc[nHandler].dwIoHandlerKey				= SERVER_IOHANDLER;
		ReadIOCPDescFromFile("SERVER_IOHANDLER", desc[nHandler], name);
		desc[nHandler].fnCreateAcceptedObject		= FnAllocAcceptServer;
		desc[nHandler].fnDestroyAcceptedObject		= FnFreeAcceptServer;
		desc[nHandler].fnDestroyConnectedObject		= FnFreeConnectServer;
		ASSERT( desc[nHandler].dwMaxPacketSize >= 8192 );
		++nHandler;
	}
	if( m_HandlerMask & CLIENT_IOHANDLER )
	{
		desc[nHandler].dwIoHandlerKey				= CLIENT_IOHANDLER;
		ReadIOCPDescFromFile("CLIENT_IOHANDLER", desc[nHandler], name);
		desc[nHandler].fnCreateAcceptedObject		= FnAllocAcceptClient;
		desc[nHandler].fnDestroyAcceptedObject		= FnFreeAcceptClient;
		desc[nHandler].fnDestroyConnectedObject		= FnFreeConnectClient;
		++nHandler;
	}


	if( !m_pIOCPServer->Init( desc, nHandler ) )
	{
		P_FAILED;
		return FALSE;
	}
	P_SUCCESS;

	if( IsServerInfoFromFile() )
	{
		if( m_HandlerMask & SERVER_IOHANDLER )
		{
			__READ_CONFIG_STRING_IP( m_pszListenIP_s, "SERVER_IOHANDLER", "IP", NULL, name );
			__READ_CONFIG_NUMBER( m_wListenPort_s, "SERVER_IOHANDLER", "PORT", 0, name );
			BOOL bListen = FALSE;
			__READ_CONFIG_NUMBER( bListen, "SERVER_IOHANDLER", "AUTO_LISTEN", FALSE, name );
			if(bListen)
				Listen( SERVER_IOHANDLER );
		}

		if( m_HandlerMask & CLIENT_IOHANDLER )
		{
			__READ_CONFIG_STRING_IP( m_pszListenIP_c, "CLIENT_IOHANDLER", "IP", NULL, name );
			__READ_CONFIG_NUMBER( m_wListenPort_c, "CLIENT_IOHANDLER", "PORT", 0, name );
			BOOL bListen = FALSE;
			__READ_CONFIG_NUMBER( bListen, "CLIENT_IOHANDLER", "AUTO_LISTEN", FALSE, name );
			if(bListen)
				Listen( CLIENT_IOHANDLER );
		}
	}

	if( !Init() )
	{
		return FALSE;
	}

	// [CONNECTION_XXX] --------------------------------------------------------------------------------------
	INT nConnectionNum = 0;
	__READ_CONFIG_NUMBER( nConnectionNum, "SERVER_IOHANDLER", "CONEECTION_NUM", 0, name );
	Connect( nConnectionNum );

	//--------------------------------------------------------------------------------------------------------
    // CHANGES: f110805.1L
    time_info_per_frame_.Init();
    //
    return true;
}


VOID		ServerFrame::DoProcess()
{
	while(TRUE)
	{
		if( m_bShutdown || !ProcessConsole() )
			break;

		if( _Update() == FALSE )
			break;

		Sleep( 1 );
	}
}

BOOL		ServerFrame::DoUpdate()
{
	Sleep( 1 );

	if( m_bShutdown || !ProcessConsole() ) return FALSE;

	return _Update();
}



VOID		ServerFrame::DoRelease()
{
	SERVER_MAP::iterator it = m_ConnectionlessMap.begin(), end = m_ConnectionlessMap.end();
	for( ; it != end ; ++it )
	{
		if( !it->second->IsConnected() )
		{
			FreeServerSession( it->second );
		}
	}
	m_ConnectionlessMap.clear();

	Release();

	m_pIOCPServer->Shutdown();
	SAFE_DELETE(m_pIOCPServer);
}

BOOL		ServerFrame::IsListening( DWORD dwKey )
{
	return m_pIOCPServer->IsListening( dwKey );
}

VOID		ServerFrame::Listen( DWORD dwKey )
{
	switch( dwKey )
	{
	case SERVER_IOHANDLER: Listen( dwKey, GetListenIP_s(), GetListenPort_s() ); break;
	case CLIENT_IOHANDLER: Listen( dwKey, GetListenIP_c(), GetListenPort_c() ); break;
	}
}

VOID ServerFrame::SuspendListenThread( DWORD dwKey )
{
	m_pIOCPServer->SuspendListenThread( dwKey );
}

VOID ServerFrame::ResumeListenThread( DWORD dwKey )
{
	m_pIOCPServer->ResumeListenThread( dwKey );
}

VOID		ServerFrame::Listen( DWORD dwKey, const CHAR* ip, WORD port )
{
	ResumeListenThread( dwKey );

	if( !m_pIOCPServer->IsListening( dwKey ) )
	{
		//SUNLOG( eCRITICAL_LOG, _T("[Listen] Starting listen (%s:%d)..."), ip, port );
		DISPMSG( "[Listen] Starting listen(%s:%d)...", ip, port );
		if( !m_pIOCPServer->StartListen( dwKey, const_cast<CHAR*>(ip), port ) )
		{
			P_FAILED;
			return ;
		}
		P_SUCCESS;
	}
}

VOID		ServerFrame::Connect( INT nNum )
{
	INT		ServerType				= UNKNOWN_SERVER;
	CHAR	pszSectionName[16+1]	= {'\0',};
	CHAR	pszConnectIP[32+1]		= {'\0',};
	WORD	wConnectPort			= 0;
	INT		nMax					= nNum;
	BOOL	bManualConnect			= FALSE;

	while( 0 < nMax-- )
	{
		_snprintf( pszSectionName, 16, "CONNECTION_%d", nMax );

		__READ_CONFIG_STRING_IP( pszConnectIP, pszSectionName, "IP", NULL, m_pszDescFileName );
		__READ_CONFIG_NUMBER( wConnectPort, pszSectionName, "PORT", 0, m_pszDescFileName );
		__READ_CONFIG_NUMBER( ServerType, pszSectionName, "TYPE", 0, m_pszDescFileName );
		__READ_CONFIG_NUMBER( bManualConnect, pszSectionName, "MANUAL_CONNECT", 0, m_pszDescFileName );
    #ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
        // NOTE: f110511.1L, it has changed role of the portal server. the database proxy server lead
        // the ranking system functionality instead of the portal server
        if (ServerType == PORTAL_SERVER) {
            continue;
        };
    #endif
    #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
        // NOTE: f110523.7L, it has changed role of the guild server. the database proxy server lead
        // the guild system functionality instead of the guild server
        if (ServerType == GUILD_SERVER) {
            continue;
        };
    #endif
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        if (ServerType == SHOP_SERVER) {
            continue;
        };
    #endif
        if (IsServerInfoFromFile())
        {
            if (ServerType == MASTER_SERVER) {
                continue;
            }
        }
        else
        {
            if (ServerType != MASTER_SERVER) {
                continue;
            }
        }

        // Caution
        // pSession이 연결되지 않은 상태에 종료하면 Dangling상태로 종료하게 된다.
        // =>m_ConnectionlessMap로 처리
        ServerSession* pSession = InitConnectSession( (eSERVER_TYPE)ServerType, pszConnectIP, wConnectPort );
        if( FALSE == bManualConnect )
            Connect( pSession );

	}
}

ServerSession*		ServerFrame::InitConnectSession( eSERVER_TYPE type, CHAR* pszIP, WORD wPort )
{
	ServerSession* pSession = AllocServerSession( type );
	ASSERT( pSession && "서버타입이 잘못되었습니다.Server.ini 오류!" );
	ASSERT( wPort != 0 && " 포트가 0입니다.Server.ini 오류!" );
	if( !wPort )	SUNLOG( eCRITICAL_LOG, "ServerType[%d] wPort is 0!", type );

	pSession->SetAddr( pszIP, wPort );
	AddConnectionlessConnector( pSession );
	return pSession;
}

VOID		ServerFrame::AddConnectionlessConnector( ServerSession* pSession )	
{ 
	static DWORD TempIndex = 0;
	pSession->SetSessionIndex( ++TempIndex );
	std::pair<SERVER_MAP::iterator,bool> rt = m_ConnectionlessMap.insert( SERVER_MAP::value_type(TempIndex, pSession) ); 
	ASSERT(rt.second);
}

VOID		ServerFrame::Connect( ServerSession* pNetObject )
{
	ASSERT( pNetObject );
	ASSERT( m_pIOCPServer );
#ifdef __NA_20100201_INTERGRATEDBILLING_
    if(BILLING_SERVER == pNetObject->GetServerType())
    {        
        pNetObject->Connect();
        return;
    }
#endif//__NA_20100201_INTERGRATEDBILLING_
	if( m_pIOCPServer )
	{
		ASSERT( !pNetObject->IsConnecting() );
		pNetObject->SetConnecting();
		m_pIOCPServer->Connect( SERVER_IOHANDLER, (NetworkObject*)pNetObject, 
			const_cast<CHAR*>(pNetObject->GetConnectIP().c_str()), pNetObject->GetConnectPort() );
	}
}

VOID		ServerFrame::MaintainConnection()
{
	if( !m_ReconnectTimer.IsExpired() )
		return;

	SERVER_MAP::iterator it = m_ConnectionlessMap.begin(), end = m_ConnectionlessMap.end();
	for( ; it != end ; ++it )
	{
		ServerSession* pSession = (it->second);
		if( pSession->IsManualConnect() || pSession->IsConnecting() )
			continue;

		ASSERT( !pSession->IsConnected() );
		Connect( pSession );
	}
}

BOOL		ServerFrame::_Update()
{
	FrameCheck();
	if( m_pIOCPServer && !m_bNetworkBlock4Closing )
		m_pIOCPServer->Update();

	MaintainConnection();

    FOREACH_CONTAINER(SERVER_MAP::value_type& node, m_SessionMap, SERVER_MAP)
    {
        ServerSession* session = node.second;
        session->Update();
    }

    //std::for_each( m_SessionMap.begin(), m_SessionMap.end(), 
    //    compose1( mem_fun(ServerSession::Update), select2nd<SERVER_MAP::value_type>() ) );

	READY_STATE ChangedState = GetReadyState().Update();

	// 모든 준비가 되었으면 Listen을 해서 다른 접속을 받아 들인다.
	if( ChangedState == READY_STATE_READY )
	{
		if( m_wListenPort_c )
			Listen( CLIENT_IOHANDLER );

		if( m_wListenPort_s )
			Listen( SERVER_IOHANDLER );
	}
	// 준비된 상태에서 변화가 생기면 접속을 받지 않는다.
	else if( ChangedState == READY_STATE_NOT_READY )
	{
		if( m_wListenPort_c )
			SuspendListenThread( CLIENT_IOHANDLER );

		if( m_wListenPort_s )
			SuspendListenThread( SERVER_IOHANDLER );
	}

    // CHANGES: f110805.1L, f100415.1L, globally server time information accessability per frame.
    time_info_per_frame_.Update();
#ifdef _JP_0_20101123_PMS
    pms_manager_.Run(); 
#endif

    return Update(time_info_per_frame_.delta_tick_);
}


BOOL		ServerFrame::ProcessConsole()
{
	if (_kbhit())
	{
		char ch = static_cast<char>(_getch());
		ch = (CHAR)toupper(ch);

		switch(ch)
		{
		case 0x1b:
			{
				SUNLOG( eCRITICAL_LOG, _T("Press 'ESC' Key, Start Server Terminating...") );
				BOOL bIsInstanceExit = KeyEvent( ch );
				return ( bIsInstanceExit ) ? FALSE : TRUE;
			}
		case ' ':
			DisplayAppInfo();
			break;
		case 'L':
			DisplayServerList();
			break;
#ifdef _JP_0_20101123_PMS
        case 'J':
            DisplayPMSInfo();
            break;
#endif
		}

		return KeyEvent( ch );
	}

	return TRUE;
}

//---------------------------------------------------------------------------------------------------------
// 커스텀한 구현 부분
//---------------------------------------------------------------------------------------------------------
VOID		ServerFrame::DisplayAppInfo( BOOL bLog )
{
	if( bLog )
	{
		SUNLOG( eCRITICAL_LOG, "============== %s ==============", GetServerType2String(GetServerType()) );
		SUNLOG( eCRITICAL_LOG, "Frame Per Second		: %d", m_dwFPS );

		if( m_HandlerMask& SERVER_IOHANDLER )
			SUNLOG( eCRITICAL_LOG, "Server Connections		: %d", m_pIOCPServer->GetNumberOfConnections( SERVER_IOHANDLER ) );
		if( m_HandlerMask& CLIENT_IOHANDLER )
			SUNLOG( eCRITICAL_LOG, "Client Connections		: %d", m_pIOCPServer->GetNumberOfConnections( CLIENT_IOHANDLER ) );
	}
	else
	{
		DISPMSG( "============== %s ==============\n", GetServerType2String(GetServerType()) );
		DISPMSG( "Frame Per Second		: %d\n", m_dwFPS );

		if( m_HandlerMask& SERVER_IOHANDLER )
			DISPMSG( "Server Connections		: %d\n", m_pIOCPServer->GetNumberOfConnections( SERVER_IOHANDLER ) );
		if( m_HandlerMask& CLIENT_IOHANDLER )
			DISPMSG( "Client Connections		: %d\n", m_pIOCPServer->GetNumberOfConnections( CLIENT_IOHANDLER ) );

	}

	SERVER_MAP::iterator it = m_ConnectionlessMap.begin(), end = m_ConnectionlessMap.end();
	for( ; it != end ; ++it )
	{
		if( !it->second->IsConnected() )
		{
			if( bLog )
				SUNLOG( eCRITICAL_LOG, ">> Warning: Not connected to %s\n", GetServerType2String(it->second->GetServerType()) );
			else
				DISPMSG( ">> Warning: Not connected to %s\n", GetServerType2String(it->second->GetServerType()) );
		}
	}
}

//
VOID		ServerFrame::DisplayServerList()
{
	const INT iMaxStr = 16383;
	const TCHAR* strfmtServerInfo = "[%02u] %-24s:  %-16s:%05u\n";
	const TCHAR* strfmtServerIPInfo = "[%02u] %-24s:  %u.%u.%u.%u:%05u\n";

	INT iWritten = 0;

	TCHAR strServerInfo[iMaxStr+((INT)sizeof(TCHAR))];	// 16384

	iWritten += _sntprintf( strServerInfo, iMaxStr, "===================  Connected Server List ====================\n" );

	SERVER_MAP::iterator it = m_SessionMap.begin(), end = m_SessionMap.end();

	for( ; it != end ; ++it )
	{
		ServerSession* pSession = it->second;

		TCHAR* pStrServerName = GetServerType2String( pSession->GetServerType() );
		WORD wConnPort = pSession->GetConnectPort();
		BOOL bConnecter = pSession->IsConnector();

		INT iWrittenTmp = 0;

		if( wConnPort )
		{
			iWritten += _sntprintf( strServerInfo+iWritten, iMaxStr-iWritten, strfmtServerInfo,
				pSession->GetSessionIndex(), pStrServerName, pSession->GetConnectIP().c_str(), wConnPort );
		}
		else
		{
			union
			{
				DWORD dwAddr;
				BYTE byAddrT[4];
			} uAddr;

			uAddr.dwAddr = pSession->GetIPAddress();
			WORD wPort = pSession->GetPort();

			iWritten += _sntprintf( strServerInfo+iWritten, iMaxStr-iWritten, strfmtServerIPInfo,
				pSession->GetSessionIndex(), pStrServerName,
				uAddr.byAddrT[0], uAddr.byAddrT[1], uAddr.byAddrT[2], uAddr.byAddrT[3], wPort );
		}

		if( iWrittenTmp < 0 )
			break;

		iWritten += iWrittenTmp;
	}

	iWritten += _sntprintf( strServerInfo+iWritten, iMaxStr-iWritten, " Connected Server Counts : %3d\n", m_SessionMap.size() );
	iWritten += _sntprintf( strServerInfo+iWritten, iMaxStr-iWritten, "===============================================================\n\n" );

	_putts( strServerInfo );
}

//

VOID		ServerFrame::UpdateAppTitle()
{
#ifdef _DEBUG
	DisplayDebugInfo( GetServerType(), GetMode(), TRUE, 
		C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, SERVER_RELEASE_BUILD_NO, 
		GetKey().GetWorldID(), GetKey().GetChannelID(), GetKey().GetServerID() );
#else
	DisplayDebugInfo( GetServerType(), GetMode(), FALSE, 
		C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO, SERVER_RELEASE_BUILD_NO, 
		GetKey().GetWorldID(), GetKey().GetChannelID(), GetKey().GetServerID() );
#endif
}

VOID		ServerFrame::FrameCheck()
{
	static INT cnt = 0;
	static DWORD prevTick = GetTickCount();

	++cnt;

	DWORD curTick = GetTickCount();

	if( prevTick + 1000 < curTick )
	{
		m_dwFPS		= cnt;
		prevTick	= curTick;
		cnt			= 0;

		if( m_dwFPS < 10 )
		{
			MessageOut( eCRITICAL_LOG, "============================================================\n" );
			MessageOut( eCRITICAL_LOG, "[Warning!!! Frame Rate Low[%d]\n", m_dwFPS );
			MessageOut( eCRITICAL_LOG, "============================================================\n" );
		}
	}
}

BOOL		ServerFrame::SendToSession( DWORD idx, MSG_BASE_INTERNAL* pMsg, WORD wSize )
{
	return SendToSession( idx, (MSG_BASE*)pMsg, wSize );
}

VOID		ServerFrame::SendToSessionType( eSERVER_TYPE type, MSG_BASE_INTERNAL* pMsg, WORD wSize, DWORD dwExceptIndex )
{
	SendToSessionType( type, (MSG_BASE*)pMsg, wSize, dwExceptIndex );
}

BOOL		ServerFrame::SendToSession( DWORD idx, MSG_BASE* pMsg, WORD wSize )
{
	SERVER_MAP::iterator it = m_SessionMap.find( idx );
	if( it == m_SessionMap.end() ) return FALSE;

	return (it->second)->SendPacket( pMsg, wSize );
}

VOID		ServerFrame::SendToSessionType( eSERVER_TYPE type, MSG_BASE* pMsg, WORD wSize, DWORD dwExceptIndex )
{
	SERVER_MAP::iterator it = m_SessionMap.begin(), end = m_SessionMap.end();
	for( ; it != end ; ++it )
	{
		ServerSession* pSession = (it->second);
		if( type == pSession->GetServerType() 
			&& dwExceptIndex != pSession->GetSessionIndex() )
		{
			pSession->SendPacket( pMsg, wSize );
		}
	}
}

VOID		ServerFrame::SendServerType( ServerSession* pSession )
{
	MSG_SERVER_TYPE msg;
	msg.m_byCategory = SERVERCOMMON;
	msg.m_byProtocol = SERVERCOMMON_SERVERTYPE;
	msg.m_byServerType = GetServerType();
	pSession->SendPacket( &msg, sizeof(msg) );
}


VOID		ServerFrame::RunUnitTest()
{
/*#ifdef _DEBUG
	TestResultStdErr result;
	TestRegistry::runAllTests(result);
	//result.getFailureCount();
#endif*/
}

#ifdef _JP_0_20101123_PMS
VOID        ServerFrame::SetTotalNumberOfConnections()
{ 
    DWORD session_count = 0;

    if( m_HandlerMask& SERVER_IOHANDLER )
        session_count += m_pIOCPServer->GetNumberOfConnections( SERVER_IOHANDLER );
    if( m_HandlerMask& CLIENT_IOHANDLER )
        session_count += m_pIOCPServer->GetNumberOfConnections( CLIENT_IOHANDLER );

    pms_manager_.SetSessioCount(session_count);
}

VOID        ServerFrame::DisplayPMSInfo()
{
    pms_manager_.DisplayPMSInfo();
}
#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<OTHER INTERFACEs>

static VOID ReadIOCPDescFromFile(const CHAR szSectionName[], IOHANDLER_DESC& desc, const CHAR* ini )
{
	__READ_CONFIG_NUMBER( desc.dwMaxAcceptSession,			szSectionName,	"MAX_ACCEPT_SESSION", 0, ini );
	__READ_CONFIG_NUMBER( desc.dwMaxConnectSession,			szSectionName,	"MAX_CONNECT_SESSION", 0, ini );
	__READ_CONFIG_NUMBER( desc.dwSendBufferSize,			szSectionName,	"SEND_BUFFER_SIZE", 0, ini );
	__READ_CONFIG_NUMBER( desc.dwRecvBufferSize,			szSectionName,	"RECV_BUFFER_SIZE", 0, ini );
	__READ_CONFIG_NUMBER( desc.dwMaxPacketSize,				szSectionName,	"MAX_PACKET_SIZE", 0, ini );
	__READ_CONFIG_NUMBER( desc.dwTimeOut,					szSectionName,	"TIME_OUT", 0, ini );
	__READ_CONFIG_NUMBER( desc.dwNumberOfAcceptThreads,		szSectionName,	"NUMBER_OF_ACCEPT_THREADS", 0, ini );
	__READ_CONFIG_NUMBER( desc.dwNumberOfConnectThreads,	szSectionName,	"NUMBER_OF_CONNECT_THREADS", 0, ini );
	__READ_CONFIG_NUMBER( desc.dwNumberOfIoThreads,			szSectionName,	"NUMBER_OF_IO_THREADS", 0, ini );
    //----------------------------------------------------------------------------------------------
    // CHANGES: f110621.2L, the quantity of the buffer usage on the io completion don't need
    // a large number of buffer size since dual buffer network application
    // NOTE: (desc.dwIoHandlerKey == SERVER_IOHANDLER) small sessions + large size buffers
    //       (dest.dwIoHandlerKey == CLIENT_IOHANDLER) a large number of sessions + small size buffers
    const DWORD max_buffer_size = (desc.dwIoHandlerKey == SERVER_IOHANDLER) ? (2 * 1024 * 1024)
                                                                            : (1 *  128 * 1024);
    const DWORD min_buffer_size = (desc.dwIoHandlerKey == SERVER_IOHANDLER) ? (1 *  128 * 1024)
                                                                            : (1 *   16 * 1024);
    desc.dwSendBufferSize = max(desc.dwSendBufferSize, min_buffer_size);
    desc.dwSendBufferSize = min(desc.dwSendBufferSize, max_buffer_size);
    desc.dwRecvBufferSize = max(desc.dwRecvBufferSize, min_buffer_size);
    desc.dwRecvBufferSize = min(desc.dwRecvBufferSize, max_buffer_size);
    //----------------------------------------------------------------------------------------------
}

//__NA000000_071105_ASSERT_LOGGING_REDIRECTION__
static void
RedirectAssertionHandler(TCHAR* msg, TCHAR* file_name, DWORD line_number)
{
	SUNLOG(eCRITICAL_LOG, "AssertionMessage: %s [file: %s : line: %u]", msg, file_name, line_number);
}

static void
RedirectProblemMemoryReporter(const TCHAR* type_name, void* mem_ptr, size_t mem_size)
{
    SUNLOG(eCRITICAL_LOG,
           "[Memory Report]|Release resource problem, alloc/free mismatch - Pool"
           "|Name=%s|Pointer=0x%08X|Size=%d|",
           type_name, static_cast<int>(reinterpret_cast<__int64>(mem_ptr)), mem_size);
}


