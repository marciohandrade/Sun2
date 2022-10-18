#include "stdafx.h"
#include "Entry.h"
#include "RTTServer.h"
#include "ManagerSkel.h"
#include "StringTokenizer.h"
#include "DropStatePoll.h"
#include "ProcessMon.h"
#include "TestCounter.h"
#include "WebRequestMonitor.h"


#define  LIMITE_CONNECTTICK		3000

//  Connection state
#define  CS_CLOSED				0x0001
#define  CS_TRYCONNECT			0x0002
#define  CS_CONNECTED			0x0003


RTTServer*	RTTServer::instance	= 0;
char		RTTServer::m_patchLauncherPath[MAX_PATH];
char		RTTServer::m_patchLauncherBinary[60];


RTTServer::RTTServer()
	: eXtreme::ServerSession( 20480, 20480 )
{
	instance		= this;

	m_disconnTick	= 0;
	m_connectState	= CS_CLOSED;
	m_serverKey		= 0;

	m_dwTicHeartBeat = 0;
}

RTTServer::~RTTServer()
{
}

void RTTServer::OnConnect( bool success, DWORD /*error*/ )
{
	if( success == true )
	{
		GetHandle()->SetBufferSize( 65535, 65535 );
		m_connectState = CS_CONNECTED;

		SendRTTWCertifyReq();

		//  Patch Launcher 정보
		::GetPrivateProfileString( "Patch Launcher", "Path", "none", RTTServer::m_patchLauncherPath, MAX_PATH, SERVERINFO_FILENAME );
		::GetPrivateProfileString( "Patch Launcher", "Binary", "none", RTTServer::m_patchLauncherBinary, 60, SERVERINFO_FILENAME );

		m_dwTicHeartBeat = ::GetTickCount();

		OutputDebug( "[RTTServer::OnConnect] success (%s)\n", Entry::Instance()->m_serverIPAddr );
	}
	else
	{
		m_connectState	= CS_CLOSED;
		m_disconnTick	= ::GetTickCount();

		OutputDebug( "[RTTServer::OnConnect] failed (%s)\n", Entry::Instance()->m_serverIPAddr );
	}
}

void RTTServer::OnClose( DWORD error )
{
	m_connectState	= CS_CLOSED;
	m_disconnTick	= ::GetTickCount();

	strcpy( m_patchLauncherPath, "none" );
	strcpy( m_patchLauncherBinary, "none" );
	
	OutputDebug( "Connection closed with RTTServer(%d)\n", error );
	Entry::Instance()->ServiceTerminate();
}

void RTTServer::CheckDisconnected()
{
	if( m_connectState == CS_CLOSED )
	{
		//  연결을 재시도 한다.
		if( ::GetTickCount() - m_disconnTick > LIMITE_CONNECTTICK )
		{
			Connect( Entry::Instance()->m_serverIPAddr, ( u_short )Entry::Instance()->m_serverPortNo );
			m_connectState	= CS_TRYCONNECT;
			m_disconnTick	= ::GetTickCount();
			OutputDebug( "[RTTServer::CheckDisconnected](%s)\n", Entry::Instance()->m_serverIPAddr );
		}
	}
}

void RTTServer::Update()
{	
	CheckDisconnected();	

	if( m_connectState == CS_CONNECTED )
	{
		if( ::GetTickCount() - m_dwTicHeartBeat > TIME_HEARTBEAT )
		{
			SendHeartBeat();
			m_dwTicHeartBeat = ::GetTickCount();
		}

		eXtreme::ServerSession::Update();
	}
}

void RTTServer::SendRTTWCertifyReq()
{
	eXtreme::Stream			stream;
	MSG_RTTW_CERTIFY_REQ	sendPacket;

	sendPacket.dwType		= SERVERTYPE_AGENT;
	sendPacket.dwVersion	= Entry::Instance()->GetVersion();

	stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_CERTIFY_REQ << ( DWORD )GetUID();
	stream.Write( &sendPacket, sizeof( sendPacket ) );

	SendPacket( stream.GetBuffer(), stream.GetDataSize() );

	OutputDebug( "[RTTServer::SendRTTWCertifyReq] version of RTTAgent(0x00%x) \n", Entry::Instance()->GetVersion());
}

void RTTServer::SendRTTRProcessListReq()
{
	MSG_RTTR_PROCESSLIST_REQ	sendPacket;
	SendMessage( sendPacket );
}

void RTTServer::SendRTTRWebUrlReq()
{
	MSG_RTTR_WEB_URL_REQ SendPacket;
	SendMessage( SendPacket );
}

void RTTServer::SendHeartBeat()
{
	MSG_RTTR_HEARTBEAT_NTF sendPacket;
	SendMessage( sendPacket );
}

void RTTServer::SendMessage( MSG_HEADER8& packet, int size )
{
	if( m_connectState != CS_CONNECTED )
		return;

	if( size == 0 )
		ServerSession::SendPacket( &packet, packet.dwSize + sizeof( MSG_HEADER8 ) );
	else
		ServerSession::SendPacket( &packet, size );

	Flush();
}

void RTTServer::OnSend( int /*size*/ )
{
#ifdef  TESTCOUNTER
	TestCounter::Instance()->Inc( COUNTER_RTTS_SENDPACKET, 1 );
	TestCounter::Instance()->Inc( COUNTER_RTTS_SENDTRANSBYTES, size );
#endif
}

void RTTServer::OnReceive( char* buffer, int size )
{
	MSG_HEADER8*		header = ( MSG_HEADER8* )buffer;

	switch( header->dwID )
	{
	case  RTTW_CERTIFY_ANS:				OnRTTWCertifyAns( buffer, size );				break;
	case  RTTW_GAMENOTICE_REQ:			OnRTTWGameNoticeReq( buffer, size );			break;
	case  RTTW_DROPUSER_REQ:			OnRTTWDropUserReq( buffer, size );				break;
	case  RTTW_GAMEADMINLIST_ANS:		OnRTTWGameAdminListAns( buffer, size );			break;
	case  RTTW_GAMEADMINADD_NTF:		OnRTTWGameAdminAddNTF( buffer, size );			break;
	case  RTTW_GAMEADMINREMOVE_NTF:		OnRTTWGameAdminRemoveNTF( buffer, size );		break;
	case  RTTW_PROCESSCONTROL_NTF:		OnRTTWProecssControlNTF( buffer, size );		break;
	case  RTTW_BATTLEZONEINFO_REQ:		OnRTTWBattleZoneInfoReq( buffer, size );		break;
	case  RTTW_USERMONEY_REQ:			OnRTTWUserMoneyReq( buffer, size );				break;
	case  RTTW_SYSTEMREBOOT_NTF:		OnRTTWSystemRebootNTF( buffer, size );			break;
	case  RTTW_SERVERSHUTDOWN_REQ:		OnRTTWServerShutdownReq( buffer, size );		break;
	case  RTTW_LAUNCHPATCHER_REQ:		OnRTTWLaunchPatcherReq( buffer, size );			break;
	case  RTTW_LAUNCHPROCESS_REQ:		OnRTTWLaunchProcessReq( buffer, size );			break;

	case  RTTR_PROCESSLIST_ANS:			OnRTTRProcessListAns( buffer, size );			break;
	case  RTTR_PATCHLAUNCHERINFO_NTF:	OnRTTRPatchLauncherInfoNTF( buffer, size );		break;
	case  RTTR_WEB_URL_ANS:				OnRTTRWebUrlAns( buffer, size );				break;
	
	case  RTTW_FUNCRESTRICT_RELOAD_NTF:	OnRTTWFuncrestrictReloadNTF( buffer, size );	break;
	case  RTTW_AUTHIPLIST_RELOAD_REQ:	OnRTTAuthIPListReloadReq( buffer, size );		break;
	case  RTTW_GAMEVERSION_UPDATE_REQ:	OnRTTWGameVersionUpdateReq( buffer, size );		break;
	case  RTTW_GAMESTATISTICS_REQ:		OnRTTWGameStatisticsReq( buffer, size );		break;		
//	case  RTTW_PATTERN_RELOAD_REQ:		OnRTTPatternReloadReq( buffer, size );			break;

	case  RTTW_ARMORKIT_DISPATCH_NTF:	OnRTTWArmorkitDispatchNTF( buffer, size );		break;
	case  RTTW_CHANGE_GUILDNAME_REQ:	OnRTTWChangeGuildNameReq( buffer, size);		break;
	case  RTTW_SEND_MESSAGE_REQ:		OnRTTWSendMessageReq( buffer, size);			break;
	case  RTTH_DROPUSER_REQ:			OnRTTHDropUserReq( buffer, size );				break;
	}

#ifdef  TESTCOUNTER
	TestCounter::Instance()->Inc( COUNTER_RTTS_RECVPACKET, 1 );
	TestCounter::Instance()->Inc( COUNTER_RTTS_RECVTRANSBYTES, size );
#endif
}

void RTTServer::OnRTTWCertifyAns( char* buffer, int size )
{
	eXtreme::Stream			stream;
	MSG_RTTW_CERTIFY_ANS*	recvPacket = ( MSG_RTTW_CERTIFY_ANS* )( buffer + 12 );
	DWORD					dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A 사이의 통신을 위해 임의로 삽입된 헤더를 읽어들인다.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;

	if( recvPacket->dwType == SERVERTYPE_AGENT )
	{
		if( recvPacket->dwResult == 0 )		//  인증 성공
		{
			OutputDebug( "[OnRTTCertifyAns] Authentication Completed\n" );

			m_serverKey	= recvPacket->dwUID;
			Entry::Instance()->ServiceStartUp();
			SendRTTRProcessListReq();
			SendRTTRWebUrlReq();		// 웹 감시 목록을 요청한다.
			//ProcessMon::Instance()->StartMonitor();
		}
		else if( recvPacket->dwResult == 1 )	//  버전 틀림
		{
			OutputDebug( "[OnRTTCertifyAns] Invalid RTTA version(=0x00%x)\n", Entry::Instance()->GetVersion() );
		}
		else if( recvPacket->dwResult == 2 )	//  등록되지 않은 머신
		{
			OutputDebug( "[OnRTTCertifyAns] Not registered ip.\n" );
		}
		else
		{
			OutputDebug( "[OnRTTCertifyAns] Not Define Result =NEW LOG=\n" );
		}
	}
	else
	{
		MSG_RTTG_CERTIFY_ANS	sendPacket;

		Skel* skel = ( Skel* )ManagerSkel::Instance()->Find( key );
		if( skel == 0 )
		{
			OutputDebug( "[OnRTTCertifyAns] Not Found Skel Key<Not SERVERTYPE_AGENT>.(Type=%d) =NEW LOG=\n", recvPacket->dwType );
			return;
		}

		sendPacket.dwResult	= recvPacket->dwResult;
		sendPacket.dwType	= recvPacket->dwType;
		sendPacket.dwUID	= recvPacket->dwUID;

		skel->SetType( sendPacket.dwType );
		skel->SendMessage( sendPacket );

		OutputDebug( "[OnRTTCertifyAns] Server(Type = %d) authenticated\n", recvPacket->dwType );
	}
}

void RTTServer::OnRTTWGameNoticeReq( char* buffer, int size )
{
	eXtreme::Stream				stream( 4096 );
	MSG_RTTW_GAMENOTICE_REQ*	recvPacket = ( MSG_RTTW_GAMENOTICE_REQ* )( buffer + 12 );
	DWORD						dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A 사이의 통신을 위해 임의로 삽입된 헤더를 읽어들인다.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;

	stream.Clear();

	Skel* skel = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if( skel == 0 )
	{
		MSG_RTTW_GAMENOTICE_ANS		sendPacket;

		//  해당 MasterServer가 아직 등록되지 않았다.
		sendPacket.dwError = 2;		//  MasterServer와의 통신에러

		stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_GAMENOTICE_ANS << key;
		stream.Write( &sendPacket, sizeof( sendPacket ) );

		SendPacket( stream );

		char tmp[NOTICE_LENGTH*2] = {0, };
		int nLen = ::WideCharToMultiByte( CP_ACP, 0, recvPacket->szNotice, -1, tmp, (NOTICE_LENGTH - 1) * 2, 0, 0 );
		tmp[nLen] = '\0';

		OutputDebug( "[RTT_GAMENOTICE_REQ] RTTAgent Search Failed(%d)(%s), RGB( %d, %d, %d )\n", recvPacket->dwServerUID, tmp,
															recvPacket->btRed, recvPacket->btGreen, recvPacket->btBlue );

		return;
	}
	else
	{
		MSG_RTTG_GAMENOTICE_REQ	sendPacket;

		sendPacket.dwServerUID		= recvPacket->dwServerUID;
		sendPacket.dwTickPerLine	= recvPacket->dwTickPerLine;
		sendPacket.dwNoticeLength	= recvPacket->dwNoticeLength;

		int nLen = ::WideCharToMultiByte(CP_ACP, 0, recvPacket->szNotice, -1, NULL, 0, NULL, NULL );

		if( nLen > NOTICE_LENGTH-1 )		// 변환후 문자열이 버퍼보다 크면 그 이후 크기는 자른다
		{
			WideCharToMultiByte( CP_ACP, 0, recvPacket->szNotice, -1, sendPacket.szNotice, NOTICE_LENGTH-1, 0, 0 );
			sendPacket.szNotice[NOTICE_LENGTH-1] = '\0';
		}
		else
		{
			WideCharToMultiByte( CP_ACP, 0, recvPacket->szNotice, -1, sendPacket.szNotice, nLen, 0, 0 );
		    sendPacket.szNotice[nLen] = '\0';
		}

		sendPacket.btRed = recvPacket->btRed;
		sendPacket.btGreen = recvPacket->btGreen;
		sendPacket.btBlue = recvPacket->btBlue;

		skel->SendMessage( sendPacket );
	}

	MSG_RTTW_GAMENOTICE_ANS		sendPacket;

	//  성공
	sendPacket.dwError = 0;

	stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_GAMENOTICE_ANS << key;
	stream.Write( &sendPacket, sizeof( sendPacket ) );

	SendPacket( stream );

	{
		char		tmp[NOTICE_LENGTH * 2] = {0, };
		int nLen = ::WideCharToMultiByte( CP_ACP, 0, recvPacket->szNotice, -1, tmp, (NOTICE_LENGTH - 1) * 2, 0, 0 );
		tmp[nLen] = '\0';

		OutputDebug( "[RTT_GAMENOTICE_REQ] %d(%s), RGB( %d, %d, %d )\n", recvPacket->dwServerUID, tmp,
											recvPacket->btRed, recvPacket->btGreen, recvPacket->btBlue );
	}
}

void RTTServer::OnRTTWDropUserReq( char* buffer, int size )
{
	eXtreme::Stream			stream( 4096 );
	MSG_RTTW_DROPUSER_REQ*	recvPacket = ( MSG_RTTW_DROPUSER_REQ* )( buffer + 12 );
	DWORD					dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A 사이의 통신을 위해 임의로 삽입된 헤더를 읽어들인다.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;


	stream.Clear();


	Skel* skel = ManagerSkel::Instance()->FindClient( SERVERTYPE_AUTH );
	if( skel == 0 )
	{
		//  접속된 인증서버가 없을 경우...
		MSG_RTTW_DROPUSER_ANS	sendPacket;

		sendPacket.dwError = 2;

		stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_DROPUSER_ANS << key;
		stream.Write( &sendPacket, sizeof( sendPacket ) );

		SendPacket( stream );

		OutputDebug( "[RTTServer::OnRTTWDropUserReq] AuthServer is not accepted.\n" );
	}
	else if( DropStatePoll::Instance()->IsProcessing( recvPacket->szUserID ) == true )
	{
		//  이미 다른 관리자에 의해 Drop 요청이 진행중인 상황
		MSG_RTTW_DROPUSER_ANS	sendPacket;

		sendPacket.dwError = 3;

		stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_DROPUSER_ANS << key;
		stream.Write( &sendPacket, sizeof( sendPacket ) );

		SendPacket( stream );

		OutputDebug( "[RTTServer::OnRTTWDropUserReq] Already processing of drop from another manager.\n" );
	}
	else
	{
		MSG_RTTG_DROPUSER_REQ	sendPacket;

		//  Drop 상태에 정보를 추가한다.
		DropStatePoll::Instance()->Add( recvPacket->szUserID, key );
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szUserID, -1, sendPacket.szUserID, ID_MAX_LEN, 0, 0 );

		sendPacket.szUserID[ID_MAX_LEN] = NULL;
		
		// WOPS에서 전송되는 DROP은 'eDRC_WOPS_GM'으로 강제 설정한다.
		sendPacket.dwDropCode = eDRC_WOPS_GM;

		skel->SendMessage( sendPacket );

		OutputDebug( "[RTTServer::OnRTTWDropUserReq] Session UID : %d, %s\n", key, sendPacket.szUserID );
	}
}

void RTTServer::OnRTTWGameAdminListAns( char* buffer, int size )
{
	eXtreme::Stream				stream( buffer, size ), dataStream( 1024 );
	MSG_RTTW_GAMEADMINLIST_ANS*	recvPacket = ( MSG_RTTW_GAMEADMINLIST_ANS* )( buffer + 12 );
	MSG_RTTG_GAMEADMINLIST_ANS	sendPacket;
	DWORD						dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A 사이의 통신을 위해 임의로 삽입된 헤더를 읽어들인다.
	stream >> dummySize >> dummyID >> key;

	Skel* skel = ( Skel* )ManagerSkel::Instance()->Find( key );
	if( skel == 0 )
		return;

	//  Stream buffer를 초기화시킨다.
	stream.Clear( 1024 );
	dataStream.Clear( 1024 );

	sendPacket.dwCount = 0;

	//  String parsing
	{
		StringTokenizer		tokInfo, tokData;
		char				tmp[1024] = {0, };
		int					index = 0;

		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szList, -1, tmp, 1024, 0, 0 );
		tokInfo.setBreakupString( tmp, ";" );

		//  ; 를 분리하여 ID, IP, Attr 까지 하나의 정보세트를 얻는다.
		while( tokInfo.hasMoreTokens() == true )
		{
			index = 0;
			tokData.setBreakupString( tokInfo.nextToken(), "," );

			//  , 를 분리하여 ID, IP, Attr 세 가지를 각각 나눈다.
			while( tokData.hasMoreTokens() == true )
			{
				if( index == 0 )		//  Account
				{
					String	tok = tokData.nextToken();
					char	account[ID_MAX_LEN] = {0, };

					strcpy( account, tok.getBuffer() );
					dataStream.Write( account, sizeof( account ) );

					sendPacket.wSize += sizeof( account );
				}
				if( index == 1 )		//  IP
				{
					String	tok = tokData.nextToken();
					char	ip[IP_MAX_LEN] = {0, };

					strcpy( ip, tok.getBuffer() );
					dataStream.Write( ip, sizeof( ip ) );

					sendPacket.wSize += sizeof( ip );
				}
				if( index == 2 )		//  Attr
				{
					String	tok = tokData.nextToken();

					DWORD attr = ( DWORD )atoi( tok.getBuffer() );
					dataStream.Write( &attr, sizeof( attr ) );

					sendPacket.wSize += sizeof( attr );
				}

				index++;
			}

			sendPacket.dwCount++;
		}
	}

	stream.Write( &sendPacket, sizeof( sendPacket ) );
	stream << dataStream;

	skel->SendPacket( stream );
}

void RTTServer::OnRTTWGameAdminAddNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_GAMEADMINADD_NTF*	recvPacket = ( MSG_RTTW_GAMEADMINADD_NTF* )buffer;
	MSG_RTTG_GAMEADMINADD_NTF	sendPacket;

	Skel* skel = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if( skel != 0 )
	{
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szAccount, -1, sendPacket.szAccount, ID_MAX_LEN, 0, 0 );
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szIP, -1, sendPacket.szIP, IP_MAX_LEN, 0, 0 );

		sendPacket.szAccount[ID_MAX_LEN] = NULL;

		sendPacket.dwAttr = recvPacket->dwAttr;

		skel->SendMessage( sendPacket );
	}
}

void RTTServer::OnRTTWGameAdminRemoveNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_GAMEADMINREMOVE_NTF*	recvPacket = ( MSG_RTTW_GAMEADMINREMOVE_NTF* )buffer;
	MSG_RTTG_GAMEADMINREMOVE_NTF	sendPacket;

	Skel* skel = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if( skel != 0 )
	{
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szAccount, -1, sendPacket.szAccount, ID_MAX_LEN, 0, 0 );

		sendPacket.szAccount[ID_MAX_LEN] = NULL;

		skel->SendMessage( sendPacket );
	}
}

void RTTServer::OnRTTWProecssControlNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_PROCESSCONTROL_NTF*	recvPacket = ( MSG_RTTW_PROCESSCONTROL_NTF* )buffer;
	char		path[MAX_PATH] = {0, };
	char		binary[MAX_PATH] = {0, };

	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szAbsolutePath, -1, path, MAX_PATH, 0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szBinaryName, -1, binary, MAX_PATH, 0, 0 );
	OutputDebug("[RTTServer::OnRTTWProecssControlNTF Path : %s, Binary : %s", path, binary);
	
	switch( recvPacket->dwCommand )
	{
	case  0:
		//  Process 종료
		ProcessMon::Instance()->KillMonitorProcess( path, binary );
		break;

	case  1:
		//  Process 시작
		ProcessMon::Instance()->CreateMonitorProcess( path, binary );
		break;
	}
}

void RTTServer::OnRTTWBattleZoneInfoReq( char* /*buffer*/, int /*size*/ )
{
	MSG_RTTG_BATTLEZONEINFO_REQ	sendPacket;

	Skel* instance = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if( instance == 0 )
		return;

	instance->SendMessage( sendPacket );
}

void RTTServer::OnRTTWUserMoneyReq( char* /*buffer*/, int /*size*/ )
{
	//MSG_RTTW_USERMONEY_REQ*	recvPacket = ( MSG_RTTW_USERMONEY_REQ* )buffer;
	MSG_RTTG_USERMONEY_REQ	sendPacket;

	Skel* instance = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if( instance == 0 )
		return;

	instance->SendMessage( sendPacket );
}

void RTTServer::OnRTTWSystemRebootNTF( char* /*buffer*/, int /*size*/ )
{
	//MSG_RTTW_SYSTEMREBOOT_NTF*	recvPacket = ( MSG_RTTW_SYSTEMREBOOT_NTF* )buffer;

	eXtreme::WinService::Instance()->LocalSystemShutdown(
				"This system will shutdown and reboot by RTT-System.\r\n"
				"If you doing some works, maybe the job will lost data.", 10, TRUE); // recvPacket->bReboot );
}

void RTTServer::OnRTTWServerShutdownReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_SERVERSHUTDOWN_REQ*	recvPacket = ( MSG_RTTW_SERVERSHUTDOWN_REQ* )buffer;
	MSG_RTTG_SERVERSHUTDOWN_REQ		sendPacket;

	sendPacket.dwKey		= recvPacket->dwKey;
	sendPacket.dwWorldID	= (BYTE)recvPacket->dwWorldID;
	sendPacket.dwChannelID	= (BYTE)recvPacket->dwChannelID;
	sendPacket.dwServerType = (BYTE)recvPacket->dwServerType;
	sendPacket.dwServerID   = (BYTE)recvPacket->dwServerID;
	sendPacket.i64WopsKey   = recvPacket->i64WopsKey;

	Skel* master = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if( master != 0 ){
		master->SendMessage( sendPacket );
	}
}

void RTTServer::OnRTTRProcessListAns( char* buffer, int /*size*/ )
{
	MSG_RTTR_PROCESSLIST_ANS*	recvPacket = ( MSG_RTTR_PROCESSLIST_ANS* )buffer;
	char						binary[100] = {0, };
	char						path[100] = {0, };

	if( recvPacket->dwFlag == 0 )
		ProcessMon::Instance()->Clear();

	for(char i = 0 ; i < recvPacket->dwCount ; i++ )
	{
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szBinaryName[i], -1, binary, 100, 0, 0 );
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szAbsolutePath[i], -1, path, 100, 0, 0 );

		ProcessMon::Instance()->AddMonitorProcess( path, binary, "", false );

		OutputDebug( "[RTTServer::OnRTTRProcessListAns] %s\\%s\n", path, binary );
		//OutputDebug( "[RTTServer::OnRTTRProcessListAns] Path Len : %d\n", wcslen(recvPacket->szAbsolutePath[i]) );
		//OutputDebug( "[RTTServer::OnRTTRProcessListAns] %s%s\n", binary, path );
	}
}

void RTTServer::OnRTTRPatchLauncherInfoNTF( char* buffer, int /*size*/ )
{
	MSG_RTTR_PATCHLAUNCHERINFO_NTF*		recvPacket = ( MSG_RTTR_PATCHLAUNCHERINFO_NTF* )buffer;

	//  RTTA의 서버 정보파일에 런쳐 정보가 없는 경우에만 처리한다.
	if( strcmp( m_patchLauncherPath, "none" ) == 0 )
	{
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szAbsolutePath, -1, m_patchLauncherPath, MAX_PATH, 0, 0 );
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szBinaryName, -1, m_patchLauncherBinary, 60, 0, 0 );
	}
}

void RTTServer::OnRTTRWebUrlAns( char* buffer, int size )
{
	MSG_RTTR_WEB_URL_ANS* pRecvPacket = (MSG_RTTR_WEB_URL_ANS*)buffer;

	if( !pRecvPacket->bExist )
	{
		return;
	}

	char szTemp[MAX_URL] = {0, };
	int nSize = ::WideCharToMultiByte(CP_ACP, 0, &pRecvPacket->szURL[0], -1, NULL, 0, NULL, NULL );

	if( nSize > (MAX_URL - 1) )
	{
		::WideCharToMultiByte(CP_ACP, 0, &pRecvPacket->szURL[0], -1, &szTemp[0], (MAX_URL - 1), NULL, NULL );
		szTemp[MAX_URL - 1] = '\0';
		OutputDebug("[RTTServer::OnRTTRWebUrlAns] Warning, URL too long!!! Buffer Overflow\n");
	}
	else
	{
		::WideCharToMultiByte(CP_ACP, 0, &pRecvPacket->szURL[0], -1, &szTemp[0], nSize, NULL, NULL );
		OutputDebug("[RTTServer::OnRTTRWebUrlAns] URL Inserted\n");
	}

	if( CWebRequestMonitor::Instance()->AddURL(pRecvPacket->dwURLIndex, szTemp) )
	{
		if( !CWebRequestMonitor::Instance()->IsStartWebRequest() )
		{
			CWebRequestMonitor::Instance()->StartWebRequestMonitor();
            OutputDebug("[RTTServer::OnRTTRWebUrlAns] Web Request Started\n");
		}
	}
}

void RTTServer::OnRTTWLaunchPatcherReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_LAUNCHPATCHER_REQ*		recvPacket = ( MSG_RTTW_LAUNCHPATCHER_REQ* )buffer;
	STARTUPINFO						startUpInfo = { 0 };
	PROCESS_INFORMATION				processInfo;
	char							tmp[MAX_PATH] = {0, };

	//  PatchKey를 저장한다.
	ProcessMon::Instance()->SetPatchKey( recvPacket->dwKey );

	//  응용 프로그램을 Foreground에서 실행하도록 지정한다.
	startUpInfo.lpDesktop = "WinSta0\\default";

	sprintf( tmp, "%s\\%s <RTTA> <%d> <%d> <%d>", m_patchLauncherPath, m_patchLauncherBinary, recvPacket->dwWorldID, recvPacket->dwChannelID, recvPacket->dwServerTypeForProcess );

	if( ::CreateProcess( 0, tmp, 0, 0, FALSE, CREATE_NEW_CONSOLE, 0, m_patchLauncherPath, &startUpInfo, &processInfo ) == FALSE )
	{
		{
			//  어찌되었든, 패치 결과는 보고되어야만 한다.
			MSG_RTTW_PATCHRESULT_ANS		sendPacket;

			sendPacket.dwKey	= recvPacket->dwKey;
			sendPacket.dwError	= MSG_RTTG_PATCHRESULT_ANS::ERROR_NOTEXISTLAUNCHER;
			//sendPacket.dwError	= MSG_RTTG_PATCHRESULT_ANS::ERROR_PATCHSUCCESS;			
			sendPacket.byIndex  = 0;
			sendPacket.byVer1	= 0;
			sendPacket.byVer2	= 0;
			sendPacket.byVer3	= 0;

			RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );
		}

		OutputDebug( "CreateProcess failed(=%d). '%s'\n", ::GetLastError(), tmp );
		return;
	}
	
	OutputDebug( "CreateProcess Success. '%s'\n", tmp );
	::CloseHandle( processInfo.hThread );
	::CloseHandle( processInfo.hProcess );
}

void RTTServer::OnRTTWLaunchProcessReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_LAUNCHPROCESS_REQ*		recvPacket = ( MSG_RTTW_LAUNCHPROCESS_REQ* )buffer;
	MSG_RTTW_LAUNCHPROCESS_ANS		sendPacket;

	sendPacket.dwServerKey	= ( recvPacket->dwWorldID << 24 | recvPacket->dwChannelID << 16 | recvPacket->dwServerType << 8 | recvPacket->dwServerID);
	sendPacket.dwWopsUID	= recvPacket->dwWopsUID;
	sendPacket.i64WopsKey   = recvPacket->i64WopsKey;

	if( ProcessMon::Instance()->CreateAllMonitorProcess() == true )
		sendPacket.dwError	= true;
	else
		sendPacket.dwError	= false;

	//  ★  이 곳을 처리하다 RTTS와 끊기는 버그가 있음
	SendMessage( sendPacket );
}

void RTTServer::OnRTTWFuncrestrictReloadNTF(char* buffer, int /*size*/ )
{
	MSG_RTTW_FUNCRESTRICT_RELOAD_NTF*	recvPacket = ( MSG_RTTW_FUNCRESTRICT_RELOAD_NTF* )buffer;
	MSG_RTTG_FUNCRESTRICT_RELOAD_NTF	sendPacket;
	
	MSG_RTTW_FUNCRESTRICT_RELOAD_ANS	resultPacket;
	resultPacket.dwResult = 0;	// 초기 실패로 설정
	resultPacket.dwWopsKey = recvPacket->dwWopsKey;

	Skel* pMaster = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if( pMaster != 0 ){
		pMaster->SendMessage( sendPacket );
		resultPacket.dwResult = 1;	// 성공 설정
	}

	SendMessage(resultPacket);
}

void RTTServer::OnRTTAuthIPListReloadReq(char* buffer, int /*size*/ )
{
	MSG_RTTW_AUTHIPLIST_RELOAD_REQ*	recvPacket = ( MSG_RTTW_AUTHIPLIST_RELOAD_REQ* )buffer;
	MSG_RTTG_AUTHIPLIST_RELOAD_REQ	sendPacket;
	
	sendPacket.dwWopsKey = recvPacket->dwWopsKey;

	Skel* pAuth = ManagerSkel::Instance()->FindClient( SERVERTYPE_AUTH );
	if( pAuth != NULL ){	// 인증서버에게 패킷을 전송한다.
		pAuth->SendMessage( sendPacket );
		OutputDebug( "RTTServer::OnRTTAuthIPListReloadReq(wopskey:%d)\n", sendPacket.dwWopsKey );
	}
	else{	// 인증서버가 없으므로 실패 패킷을 RTTServer로 전송한다.
		MSG_RTTW_AUTHIPLIST_RELOAD_ANS		errorPacket;

		errorPacket.dwResult	= 0;
		errorPacket.dwWopsKey	= recvPacket->dwWopsKey;
		RTTServer::instance->SendMessage( errorPacket, errorPacket.dwSize + sizeof( MSG_HEADER8 ) );

		OutputDebug("[RTTServer::OnRTTAuthIPListReloadReq] Auth Server Search Fail  \n");
	}
}

void RTTServer::OnRTTWGameVersionUpdateReq(char* buffer, int /*size*/ )
{
	MSG_RTTW_GAMEVERSION_UPDATE_REQ*	recvPacket = ( MSG_RTTW_GAMEVERSION_UPDATE_REQ* )buffer;
	MSG_RTTG_GAMEVERSION_UPDATE_REQ		sendPacket;
	
	sendPacket.dwWopsKey   = recvPacket->dwWopsKey;


	Skel* pAuth = ManagerSkel::Instance()->FindClient( SERVERTYPE_AUTH );
	if( pAuth != NULL ){	// 인증서버에게 패킷을 전송한다.
		pAuth->SendMessage( sendPacket );
		OutputDebug( "RTTServer::OnRTTAuthVersionReloadReq(wopskey:%d)\n", sendPacket.dwWopsKey );
	}
	else{	// 인증서버가 없으므로 실패 패킷을 RTTServer로 전송한다.
		MSG_RTTW_GAMEVERSION_UPDATE_ANS		errorPacket;

		errorPacket.dwResult	= 0;
		errorPacket.dwWopsKey	= recvPacket->dwWopsKey;
		RTTServer::instance->SendMessage( errorPacket, errorPacket.dwSize + sizeof( MSG_HEADER8 ) );

		OutputDebug("[RTTServer::OnRTTGameVersionUpdateReq] Auth Server Search Fail  \n");
	}
}

void RTTServer::OnRTTWGameStatisticsReq(char* /*buffer*/, int /*size*/ )
{
	MSG_RTTG_GAMESTATISTICS_REQ sendPacket;
	Skel* instance = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if(instance==NULL){
		OutputDebug("[RTTServer::OnRTTGameStatisticsReq] Master Server Search Fail  \n");
		return;
	}

	instance->SendMessage( sendPacket );
}

//void RTTServer::OnRTTPatternReloadReq(char* buffer, int /*size*/ )
//{
//	MSG_RTTW_PATTERN_RELOAD_REQ*	recvPacket = ( MSG_RTTW_PATTERN_RELOAD_REQ* )buffer;
//	MSG_RTTG_PATTERN_RELOAD_REQ		sendPacket;
//	
//	sendPacket.dwWopsKey = recvPacket->dwWopsKey;
//
//	Skel* pAuth = ManagerSkel::Instance()->FindClient( SERVERTYPE_AUTH );
//	if( pAuth != NULL ){	// 인증서버에게 패킷을 전송한다.
//		pAuth->SendMessage( sendPacket );
//		OutputDebug( "RTTServer::OnRTTPatternReloadReq(wopskey:%d)\n", sendPacket.dwWopsKey );
//	}
//	else{	// 인증서버가 없으므로 실패 패킷을 RTTServer로 전송한다.
//		MSG_RTTW_PATTERN_RELOAD_ANS		errorPacket;
//
//		errorPacket.dwResult	= 0;
//		errorPacket.dwWopsKey	= recvPacket->dwWopsKey;
//		RTTServer::instance->SendMessage( errorPacket, errorPacket.dwSize + sizeof( MSG_HEADER8 ) );
//
//		OutputDebug("[RTTServer::OnRTTPatternReloadReq] Auth Server Search Fail  \n");
//	}
//}

void RTTServer::OnRTTWArmorkitDispatchNTF(char* buffer, int /*size*/ )
{
	MSG_RTTW_ARMORKIT_DISPATCH_NTF*	recvPacket = ( MSG_RTTW_ARMORKIT_DISPATCH_NTF* )buffer;
	MSG_RTTG_ARMORKIT_DISPATCH_NTF	sendPacket;
	
	Skel* pAuth = ManagerSkel::Instance()->FindClient( SERVERTYPE_AUTH );
	if( pAuth != 0 )
	{
		sendPacket.dwWopsKey = recvPacket->dwWopsKey;
		pAuth->SendMessage( sendPacket );

		OutputDebug( "RTTServer::OnRTTWArmorkitDispatchNTF(wopskey:%d)\n", sendPacket.dwWopsKey );
	}
	else
	{
		MSG_RTTW_ARMORKIT_DISPATCH_ANS	resultPacket;
		
		resultPacket.dwResult = 0;	// 초기 실패로 설정
		resultPacket.dwWopsKey = recvPacket->dwWopsKey;
		SendMessage(resultPacket);

		OutputDebug("[RTTServer::OnRTTWArmorkitDispatchNTF] Auth Server Search Fail  \n");
	}
}

void RTTServer::OnRTTWChangeGuildNameReq(char* buffer, int size)
{
	eXtreme::Stream					stream( 1024 );
	MSG_RTTW_CHANGE_GUILDNAME_REQ*	recvPacket = ( MSG_RTTW_CHANGE_GUILDNAME_REQ* )(buffer + 12);
	DWORD						dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A 사이의 통신을 위해 임의로 삽입된 헤더를 읽어들인다.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;

//*////////////////////////////////////////////////////////// 로그

	char szGuildName[ MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_GUILD_NAME_LENGTH + 1 ] = {0, };
	char szDescription[ MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH + 1 ] = {0, };

	::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszGuildName,		MSG_RTTW_CHANGE_GUILDNAME_REQ::MAX_GUILD_NAME_LENGTH,	szGuildName,	MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_GUILD_NAME_LENGTH,	0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszDescription,	MSG_RTTW_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH,	szDescription,	MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH,	0, 0 );

    szGuildName[ MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_GUILD_NAME_LENGTH ] = NULL;
	szDescription[ MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH ] = NULL;

	OutputDebug( "[RTTServer::OnRTTGChangeGuildNameReq]		WopsKey		: %d\n", key );
	OutputDebug( "[RTTServer::OnRTTGChangeGuildNameReq]		World		: %d\n", recvPacket->dwServerGuid.GetWorldID() );
	OutputDebug( "[RTTServer::OnRTTGChangeGuildNameReq]		GuildGuid	: %d\n", recvPacket->dwGuildGuid);
	OutputDebug( "[RTTServer::OnRTTGChangeGuildNameReq]		GuildName	: %s\n", szGuildName);
	OutputDebug( "[RTTServer::OnRTTGChangeGuildNameReq]		Description	: %s\n", szDescription);

////////////////////////////////////////////////////////////*/

	Skel* pMaster = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if( pMaster != 0 )
	{
		MSG_RTTG_CHANGE_GUILDNAME_REQ	sendPacket;

	    sendPacket.dwWopsKey			= key;		// 명령을 보낸 WOPS Session Key
		sendPacket.dwServerGuid.dwKey	= recvPacket->dwServerGuid;
		sendPacket.dwGuildGuid			= recvPacket->dwGuildGuid;
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszGuildName,		MSG_RTTW_CHANGE_GUILDNAME_REQ::MAX_GUILD_NAME_LENGTH,	sendPacket.szGuildName,		MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_GUILD_NAME_LENGTH,	0, 0 );
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszDescription,	MSG_RTTW_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH,	sendPacket.szDescription,	MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH,	0, 0 );

		sendPacket.szGuildName[	MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_GUILD_NAME_LENGTH ] = NULL;
		sendPacket.szDescription[ MSG_RTTG_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH] = NULL;

		pMaster->SendMessage( sendPacket );
		OutputDebug( "[RTTServer::OnRTTGChangeGuildNameReq] Send To Master Server\n");
	}
	else
	{
		MSG_RTTW_CHANGE_GUILDNAME_ANS	resultPacket;
        resultPacket.dwResult = 2;		// 마스터 서버와 통신 실패

		wcscpy(resultPacket.wszResultDescription, L"Master Server를 찾을 수 없습니다");

		stream.Clear();

		stream << ( DWORD )( 4 + sizeof( resultPacket ) ) << ( DWORD )RTTW_CHANGE_GUILDNAME_ANS << ( DWORD )key;
		stream.Write( &resultPacket, sizeof( resultPacket ) );

		SendPacket( stream );

		OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq] Master Server Search Fail\n");
	}
}

void RTTServer::OnRTTWSendMessageReq(char* buffer, int size)
{
	eXtreme::Stream				stream( 4096 );
	MSG_RTTW_SEND_MESSAGE_REQ*	recvPacket = ( MSG_RTTW_SEND_MESSAGE_REQ* )(buffer + 12);
	DWORD						dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A 사이의 통신을 위해 임의로 삽입된 헤더를 읽어들인다.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;

//*////////////////////////////////////////////////////////// 로그

	char szRecvCharName[ MSG_RTTG_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH + 1 ]	= {0, };
	char szSendCharName[ MSG_RTTG_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH + 1 ]	= {0, };
	char szMessage[ MSG_RTTG_SEND_MESSAGE_REQ::MAX_MESSAGE_LENGTH + 1 ]			= {0, };

	int nMaxCharNameLen	= MSG_RTTG_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH;
	int nMaxMessageLen	= MSG_RTTG_SEND_MESSAGE_REQ::MAX_MESSAGE_LENGTH;

	OutputDebug( "[RTTServer::OnRTTWSendMessageReq]		WopsKey		: %d\n", key );
	OutputDebug( "[RTTServer::OnRTTWSendMessageReq]		World		: %d\n", recvPacket->dwServerGuid.GetWorldID() );

	for(DWORD dwLogIndex = 0 ; (dwLogIndex < recvPacket->dwCount) && (dwLogIndex < MSG_RTTG_SEND_MESSAGE_REQ::MAX_USER_INFO) ; ++dwLogIndex)
	{
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->arrUserInfoList[dwLogIndex].wszRecvCharName,	nMaxCharNameLen,	szRecvCharName,	nMaxCharNameLen,	0, 0 );

		szRecvCharName[ nMaxCharNameLen ] = NULL;

		OutputDebug( "[RTTServer::OnRTTGSendMessageReq]		RecvChar	: %s\n", szRecvCharName);

		memset(szRecvCharName,	0, sizeof(szRecvCharName));
	}

	::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszSendCharName,	nMaxCharNameLen,	szSendCharName,	nMaxCharNameLen,	0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszMessage,		nMaxMessageLen / 2,		szMessage,		nMaxMessageLen,		0, 0 );

	szSendCharName[ nMaxCharNameLen ] = NULL;
	szMessage[ nMaxMessageLen ] = NULL;

	OutputDebug( "[RTTServer::OnRTTGSendMessageReq]		SendChar	: %s\n", szSendCharName);
	OutputDebug( "[RTTServer::OnRTTGSendMessageReq]		Message		: %s\n", szMessage);

////////////////////////////////////////////////////////////*/

	Skel* pMaster = ManagerSkel::Instance()->FindClient( SERVERTYPE_MASTER );
	if( pMaster != 0 )
	{
        MSG_RTTG_SEND_MESSAGE_REQ	sendPacket;

		sendPacket.dwWopsKey			= key;		// 명령을 보낸 WOPS Session Key
		sendPacket.dwServerGuid.dwKey	= recvPacket->dwServerGuid;
		sendPacket.dwCount				= recvPacket->dwCount;

		for(DWORD dwIndex = 0 ; (dwIndex < sendPacket.dwCount) && (dwIndex < MSG_RTTG_SEND_MESSAGE_REQ::MAX_USER_INFO) ; ++dwIndex)
		{
			::WideCharToMultiByte( CP_ACP, 0, recvPacket->arrUserInfoList[dwIndex].wszRecvCharName,	MSG_RTTW_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH,	sendPacket.arrUserInfoList[dwIndex].szRecvCharName,	MSG_RTTG_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH,	0, 0 );
			sendPacket.arrUserInfoList[dwIndex].szRecvCharName[ MSG_RTTG_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH ] = NULL;
		}

		::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszSendCharName,	MSG_RTTW_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH,	sendPacket.szSendCharName,	MSG_RTTG_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH,	0, 0 );
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszMessage,		MSG_RTTW_SEND_MESSAGE_REQ::MAX_MESSAGE_LENGTH,		sendPacket.szMessage,		MSG_RTTG_SEND_MESSAGE_REQ::MAX_MESSAGE_LENGTH,		0, 0 );

		sendPacket.szSendCharName[ MSG_RTTG_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH ] = NULL;
		sendPacket.szMessage[ MSG_RTTG_SEND_MESSAGE_REQ::MAX_MESSAGE_LENGTH ] = NULL;

		pMaster->SendMessage( sendPacket );
		OutputDebug( "[RTTServer::OnRTTGSendMessageReq] Send To Master Server(Count : %d)\n", sendPacket.dwCount);
	}
	else
	{
		MSG_RTTW_SEND_MESSAGE_ANS		resultPacket;

		resultPacket.dwCount = recvPacket->dwCount;

		for(DWORD dwIndex = 0 ; (dwIndex < resultPacket.dwCount) && (dwIndex < MSG_RTTW_SEND_MESSAGE_ANS::MAX_RESULT_INFO) ; ++dwIndex)
		{
			resultPacket.arrResultInfoList[dwIndex].dwResult = 2;		// Master 서버와 통신 실패
			::wcsncpy( resultPacket.arrResultInfoList[dwIndex].wszRecvCharName, recvPacket->arrUserInfoList[dwIndex].wszRecvCharName, MSG_RTTW_SEND_MESSAGE_ANS::MAX_CHAR_NAME_LENGTH );
			wcscpy(resultPacket.arrResultInfoList[dwIndex].wszResultDescription, L"Master Server를 찾을 수 없습니다");
		}

		stream.Clear();

		stream << ( DWORD )( 4 + sizeof( resultPacket ) ) << ( DWORD )RTTW_SEND_MESSAGE_ANS << ( DWORD )key;
		stream.Write( &resultPacket, sizeof( resultPacket ) );

		SendPacket( stream );

		OutputDebug("[RTTServer::OnRTTGSendMessageReq] Master Server Search Fail(Count : %d)\n", recvPacket->dwCount);
	}
}

void RTTServer::OnRTTHDropUserReq( char* buffer, int size )
{
	eXtreme::Stream			stream( 4096 );
	MSG_RTTH_DROPUSER_REQ*	recvPacket = ( MSG_RTTH_DROPUSER_REQ* )( buffer + 12 );
	DWORD					dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A 사이의 통신을 위해 임의로 삽입된 헤더를 읽어들인다.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;

	stream.Clear();

	Skel* skel = ManagerSkel::Instance()->FindClient( SERVERTYPE_AUTH );
	if( skel == 0 )
	{
		//  접속된 인증서버가 없을 경우...
		MSG_RTTH_DROPUSER_ANS	sendPacket;

		sendPacket.dwError = 2;

		stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTH_DROPUSER_ANS << key;
		stream.Write( &sendPacket, sizeof( sendPacket ) );

		SendPacket( stream );

		OutputDebug( "[RTTServer::OnRTTHDropUserReq] AuthServer is not accepted.\n" );
	}
	else if( DropStatePoll::Instance()->IsProcessing( recvPacket->szUserID ) == true )
	{
		//  이미 다른 관리자에 의해 Drop 요청이 진행중인 상황
		MSG_RTTH_DROPUSER_ANS	sendPacket;

		sendPacket.dwError = 3;

		stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTH_DROPUSER_ANS << key;
		stream.Write( &sendPacket, sizeof( sendPacket ) );

		SendPacket( stream );

		OutputDebug( "[RTTServer::OnRTTHDropUserReq] Already processing of drop from another manager.\n" );
	}
	else
	{
		MSG_RTTG_DROPUSER_REQ	sendPacket;

		//  Drop 상태에 정보를 추가한다.
		DropStatePoll::Instance()->Add( recvPacket->szUserID, key );
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szUserID, -1, sendPacket.szUserID, ID_MAX_LEN, 0, 0 );

		sendPacket.szUserID[ID_MAX_LEN] = NULL;
		
		// WEB에서 전송되는 DROP은 'eDRC_WEB'으로 설정한다.
		sendPacket.dwDropCode = eDRC_WEB;

		skel->SendMessage( sendPacket );

		OutputDebug( "[RTTServer::OnRTTHDropUserReq] Session UID : %d, %s\n", key, sendPacket.szUserID );
	}
}