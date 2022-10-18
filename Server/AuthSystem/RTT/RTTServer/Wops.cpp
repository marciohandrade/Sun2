#include "stdafx.h"
#include "Wops.h"
#include "ManagerClient.h"
#include "ManagerService.h"
#include "ManagerWops.h"
#include "Database.h"
#include "TestCounter.h"


Wops::Wops()
	: eXtreme::ServerSession( 20480, 20480 )
{
}

Wops::~Wops()
{
}

void Wops::OnAccept()
{
	OutputDebug( "Accepted Wops component from %s\n", GetIPAddress() );
	OutputDebug("[Wops::OnAccept] WOPS Connect Count = %d\n", ManagerWops::Instance()->GetActiveSessionCnt());
}

void Wops::OnClose( DWORD error )
{
	Wops*	instance = ManagerWops::Instance()->GetSustenance();
	
	BOOL bSustenance = FALSE;
	if( instance == this ){
		ManagerWops::Instance()->SetSustenance( 0 );
		bSustenance = TRUE;
	}
	
	if(bSustenance){
		OutputDebug( "Closed connection with WOPS Sustenance component(%s, err=%d)\n", GetIPAddress(), error );
	}
	else{
		OutputDebug( "Closed connection with WOPS component(%s, err=%d)\n", GetIPAddress(), error );
	}

	OutputDebug("[Wops::OnClose] WOPS Connect Count = %d\n", ManagerWops::Instance()->GetActiveSessionCnt());
}

void Wops::Update()
{
	ServerSession::Update();
}

void Wops::SendMessage( MSG_HEADER8& packet, int size )
{
	if( size == 0 )
		ServerSession::SendPacket( &packet, packet.dwSize + sizeof( MSG_HEADER8 ) );
	else
		ServerSession::SendPacket( &packet, size );
}

void Wops::OnSend( int /*size*/ )
{
#ifdef  TESTCOUNTER
	TestCounter::Instance()->Inc( COUNTER_WOPS_SENDPACKET, 1 );
	TestCounter::Instance()->Inc( COUNTER_WOPS_SENDTRANSBYTES, size );
#endif
}

void Wops::OnReceive( char* buffer, int size )
{
	MSG_HEADER8*		header = ( MSG_HEADER8* )buffer;

	switch( header->dwID )
	{
	case  RTTW_SERVERLIST_REQ:		OnRTTWServerListReq( buffer, size );		break;
	case  RTTW_GAMENOTICE_REQ:		OnRTTWGameNoticeReq( buffer, size );		break;
	case  RTTW_GAMEADMINLIST_REQ:	OnRTTWGameAdminListReq( buffer, size );		break;
	case  RTTW_GAMEADMINADD_NTF:	OnRTTWGameAdminAddNTF( buffer, size );		break;
	case  RTTW_GAMEADMINREMOVE_NTF:	OnRTTWGameAdminRemoveNTF( buffer, size );	break;
	case  RTTW_DROPUSER_REQ:		OnRTTWDropUserReq( buffer, size );			break;
	case  RTTW_RTTASTART_REQ:		OnRTTWRTTAStartReq( buffer, size );			break;
	case  RTTW_RTTASTOP_REQ:		OnRTTWRTTAStopReq( buffer, size );			break;
	case  RTTW_RTTAPATCH_NTF:		OnRTTWRTTAPatchNTF( buffer, size );			break;
	case  RTTW_SYSTEMREBOOT_NTF:	OnRTTWSystemRebootReq( buffer, size );		break;
	case  RTTW_PROCESSCONTROL_NTF:	OnRTTWProcessControlNTF( buffer, size );	break;
	case  RTTW_CONNECTIONTYPE_NTF:	OnRTTWConnectionTypeNTF( buffer, size );	break;
	case  RTTW_SERVERSHUTDOWN_REQ:	OnRTTWServerShutdownReq( buffer, size );	break;
	case  RTTW_LAUNCHPATCHER_REQ:	OnRTTWLaunchPatcherReq( buffer, size );		break;
	case  RTTW_LAUNCHPROCESS_REQ:	OnRTTWLaunchProcessReq( buffer, size );		break;
	
	case  RTTW_FUNCRESTRICT_RELOAD_NTF:	OnRTTWFuncrestrictReloadNTF( buffer, size );	break;
	case  RTTW_AUTHIPLIST_RELOAD_REQ:	OnRTTAuthIPListReloadReq( buffer, size );		break;
	case  RTTW_GAMEVERSION_UPDATE_REQ:	OnRTTGameVersionUpdateReq( buffer, size );		break;

	// RTTW_ARMORKIT_DISPATCH_NTF 프로토콜로 변경
	//case  RTTW_PATTERN_RELOAD_REQ:		OnRTTPatternReloadReq(buffer, size);			break;
	//
	
	/*
		2006.09.08
		AromorKit Dispatch 
	*/
	case  RTTW_ARMORKIT_DISPATCH_NTF:	OnRTTWArmorkitDispatchNTF(buffer, size);		break;
	case  RTTW_CHANGE_GUILDNAME_REQ:	OnRTTWChangeGuildNameReq( buffer, size);		break;
	case  RTTW_SEND_MESSAGE_REQ:		OnRTTWSendMessageReq( buffer, size);			break;

	default:
		OutputDebug( "[Wops Session] Not defined MessageID(=0x%x)\n", header->dwID );
		break;
	}

#ifdef  TESTCOUNTER
	TestCounter::Instance()->Inc( COUNTER_WOPS_RECVPACKET, 1 );
	TestCounter::Instance()->Inc( COUNTER_WOPS_RECVTRANSBYTES, size );
#endif
}

void Wops::OnRTTWServerListReq( char* /*size*/, int /*size*/ )
{
	MSG_RTTW_SERVERLIST_ANS	sendPacket;
	ServerInfo*				list[256] = {0, };
	DWORD					count = 0, len = 0;
	wchar_t					data[32] = {0, };

	//  서버 목록정보를 Wide character 정보로 변환한다.
	{
		Database::Instance()->MakeServerList( list, 256, &count );

		sendPacket.dwLength = 0;
		memset( sendPacket.szList, 0, sizeof( sendPacket.szList ) );
		for(DWORD i = 0 ; i < count ; i++ )
		{
			_snwprintf( data, 32, L"%s,%d,%d,%d,%d;", list[i]->nv_ServerName,
				list[i]->ti_WorldID, list[i]->ti_ChannelID, list[i]->ti_ServerType, list[i]->ti_ServerID );

			//  문자열 버퍼를 넘을 수 없다.
			len = ( DWORD )wcslen( data );
			if( sendPacket.dwLength + len >= 5120 )
				break;

			wcscat( sendPacket.szList, data );
			sendPacket.dwLength = ( DWORD )wcslen( sendPacket.szList );
		}
	}

	SendMessage( sendPacket );
}

void Wops::OnRTTWGameNoticeReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_GAMENOTICE_REQ*	recvPacket = ( MSG_RTTW_GAMENOTICE_REQ* )buffer;
	eXtreme::Stream				stream( 4096 );

	stream << ( DWORD )( 4 + sizeof( *recvPacket ) ) << ( DWORD )RTTW_GAMENOTICE_REQ << ( DWORD )GetUID();
	stream.Write( recvPacket, sizeof( *recvPacket ) );

	SERVER_KEY serverKey(recvPacket->dwServerUID);

	if( false == ManagerClient::Instance()->SendToMasterClient( stream, serverKey.GetWorldID() ) )
	{
		MSG_RTTW_GAMENOTICE_ANS	sendPacket;

		//  해당 RTTAgent가 아직 등록되지 않았다.
		sendPacket.dwError = 1;		//  RTTAgent와의 통신에러
		SendMessage( sendPacket );

		OutputDebug( "[RTT_GAMENOTICE_REQ] RTTAgent Search Failed(%d)\n", recvPacket->dwServerUID);
	}

	char tmp[NOTICE_LENGTH*2] = {0, };
	int nLen = ::WideCharToMultiByte( CP_ACP, 0, recvPacket->szNotice, -1, tmp, (NOTICE_LENGTH - 1) * 2, 0, 0 );
	tmp[nLen] = '\0';

	OutputDebug( "[RTT_GAMENOTICE_REQ] %d(%s), RGB( %d, %d, %d )\n", recvPacket->dwServerUID, tmp,
																							recvPacket->btRed, recvPacket->btGreen, recvPacket->btBlue );



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// 기존 마스터 서버가 1개 있던 방식 //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//MSG_RTTW_GAMENOTICE_REQ*	recvPacket = ( MSG_RTTW_GAMENOTICE_REQ* )buffer;
	//eXtreme::Stream				stream( 4096 );

 //   RTTAgent* rttAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_MASTER );
	//if( rttAgent == 0 )
	//{
	//	MSG_RTTW_GAMENOTICE_ANS	sendPacket;

	//	//  해당 RTTAgent가 아직 등록되지 않았다.
	//	sendPacket.dwError = 1;		//  RTTAgent와의 통신에러
	//	SendMessage( sendPacket );
	//	
	//	char tmp[NOTICE_LENGTH*2] = {0, };
	//	int nLen = ::WideCharToMultiByte( CP_ACP, 0, recvPacket->szNotice, -1, tmp, (NOTICE_LENGTH - 1) * 2, 0, 0 );
	//	tmp[nLen] = '\0';

	//	OutputDebug( "[RTT_GAMENOTICE_REQ] RTTAgent Search Failed(%d)(%s), RGB( %d, %d, %d )\n", recvPacket->dwServerUID, tmp,
	//														recvPacket->btRed, recvPacket->btGreen, recvPacket->btBlue );

	//	return;
	//}

	//stream << ( DWORD )( 4 + sizeof( *recvPacket ) ) << ( DWORD )RTTW_GAMENOTICE_REQ << ( DWORD )GetUID();
	//stream.Write( recvPacket, sizeof( *recvPacket ) );

	//rttAgent->SendPacket( stream );

	//{
	//	char tmp[NOTICE_LENGTH*2] = {0, };
	//	int nLen = ::WideCharToMultiByte( CP_ACP, 0, recvPacket->szNotice, -1, tmp, (NOTICE_LENGTH - 1) * 2, 0, 0 );
	//	tmp[nLen] = '\0';

	//	OutputDebug( "[RTT_GAMENOTICE_REQ] %d(%s), RGB( %d,%d,%d )\n", recvPacket->dwServerUID, tmp,
	//										recvPacket->btRed, recvPacket->btGreen, recvPacket->btBlue );
	//}
}

void Wops::OnRTTWGameAdminListReq( char* /*size*/, int /*size*/ )
{
	MSG_RTTW_GAMEADMINLIST_ANS	sendPacket;
	AdminInfo*					list[256] = {0, };
	DWORD						count = 0;
	wchar_t						data[96] = {0, };

	//  관리자 목록정보를 Wide character 정보로 변환한다.
	{
		Database::Instance()->MakeAdminList( list, 256, &count );

		memset( sendPacket.szList, 0, sizeof( sendPacket.szList ) );
		for(DWORD i = 0 ; i < count ; i++ )
		{
			_snwprintf( data, 96, L"%s,%s,%d;", list[i]->UserID, list[i]->IpAddr, list[i]->LV );
			wcscat( sendPacket.szList, data );
			memset(data, 0, 96);
		}
		sendPacket.dwLength = ( DWORD )wcslen( sendPacket.szList );
	}

	SendMessage( sendPacket );
}

void Wops::OnRTTWGameAdminAddNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_GAMEADMINADD_NTF*	recvPacket = ( MSG_RTTW_GAMEADMINADD_NTF* )buffer;

	Database::Instance()->AddAdmin( recvPacket->szAccount, recvPacket->szIP, recvPacket->dwAttr );

	if( false == ManagerClient::Instance()->SendToMasterClient( *recvPacket ) )
	{
		OutputDebug( "[Wops::OnRTTWGameAdminAddNTF] Matched skel is not found.\n" );
	}

	//MSG_RTTW_GAMEADMINADD_NTF*	recvPacket = ( MSG_RTTW_GAMEADMINADD_NTF* )buffer;

	//Database::Instance()->AddAdmin( recvPacket->szAccount, recvPacket->szIP, recvPacket->dwAttr );

	//RTTAgent* rttAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_MASTER );
	//if( rttAgent != 0 )
	//	rttAgent->SendMessage( *recvPacket );
	//else
	//	OutputDebug( "[Wops::OnRTTWGameAdminAddNTF] Matched skel is not found.\n" );
}

void Wops::OnRTTWGameAdminRemoveNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_GAMEADMINREMOVE_NTF*	recvPacket = ( MSG_RTTW_GAMEADMINREMOVE_NTF* )buffer;

	Database::Instance()->RemoveAdmin( recvPacket->szAccount );

	if( false == ManagerClient::Instance()->SendToMasterClient( *recvPacket ) )
	{
		OutputDebug( "[Wops::OnRTTWGameAdminRemoveNTF] Matched skel is not found.\n" );
	}


	//MSG_RTTW_GAMEADMINREMOVE_NTF*	recvPacket = ( MSG_RTTW_GAMEADMINREMOVE_NTF* )buffer;
	//MSG_RTTG_GAMEADMINREMOVE_NTF	sendPacket;

	//Database::Instance()->RemoveAdmin( recvPacket->szAccount );

	//RTTAgent* rttAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_MASTER );
	//if( rttAgent != 0 )
	//	rttAgent->SendMessage( *recvPacket );
	//else
	//	OutputDebug( "[Wops::OnRTTWGameAdminRemoveNTF] Matched skel is not found.\n" );
}

void Wops::OnRTTWDropUserReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_DROPUSER_REQ*	recvPacket = ( MSG_RTTW_DROPUSER_REQ* )buffer;
	eXtreme::Stream			stream( 4096 );

	RTTAgent* rttAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_AUTH );
	if( rttAgent == 0 )		//  인증 서버가 접속되지 않았다.
	{
		MSG_RTTW_DROPUSER_ANS	sendPacket;
		sendPacket.dwError = 2;	//  서버간 통신 오류
		SendMessage( sendPacket );

		OutputDebug("[Wops::OnRTTWDropUserReq] RTTAgent not found\n");
		return;
	}
	

	stream << ( DWORD )( 4 + sizeof( *recvPacket ) ) << ( DWORD )RTTW_DROPUSER_REQ << ( DWORD )GetUID();
	stream.Write( recvPacket, sizeof( *recvPacket ) );

	rttAgent->SendPacket( stream );

	OutputDebug("[Wops::OnRTTWDropUserReq] Session UID : %d\n", ( DWORD )GetUID() );
}

void Wops::OnRTTWRTTAStartReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_RTTASTART_REQ*	recvPacket = ( MSG_RTTW_RTTASTART_REQ* )buffer;
	MSG_RTTW_RTTASTART_ANS	sendPacket;

	OutputDebug( "Message : RTTA Start request(flag = %d)(%S).\n", recvPacket->dwFlag, recvPacket->szIPAddress );

	if( recvPacket->dwFlag == 0 )
	{
		ManagerService::Instance()->BroadcastRTTAStart();
	}
	else if( recvPacket->dwFlag == 2) // World 단위로 RTTAgent를 실행한다.
	{
		OutputDebug( "[Wops::OnRTTWRTTAStartReq]Send RTTAStart Broadcast\n");

		DWORD dwWordlID = _wtoi(recvPacket->szIPAddress);
		MSG_RTTR_RTTASTART_NTF	packet;
		ManagerService::Instance()->Broadcast(dwWordlID, (char*)&packet, sizeof(MSG_RTTR_RTTASTART_NTF));

		sendPacket.dwResult	= 0;
		wcsncpy( sendPacket.szIPAddress, recvPacket->szIPAddress, IP_MAX_LEN );

		SendMessage( sendPacket );
	}
	else
	{
		char					ipAddress[IP_MAX_LEN] = {0, };
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szIPAddress, -1, ipAddress, IP_MAX_LEN, 0, 0 );

		RTTService* instance = ManagerService::Instance()->FindClient( ipAddress );

		if( instance == 0 )
		{
			//  이 상황은 발생되어서는 안된다. WOPS와 RTTS가 서로 갖고있는 RTTA의 정보가 다를 경우이기 때문이다.
			sendPacket.dwResult	= 2;
			wcsncpy( sendPacket.szIPAddress, recvPacket->szIPAddress, IP_MAX_LEN );

			SendMessage( sendPacket );

			OutputDebug( "[OnRTTWRTTAStartReq: RTTA(%s)를 검색하지 못하였습니다.\n", ipAddress );

			return;
		}

		//  성공
		instance->SendRTTAStart();

		sendPacket.dwResult	= 0;
		wcsncpy( sendPacket.szIPAddress, recvPacket->szIPAddress, IP_MAX_LEN );

		SendMessage( sendPacket );
	}
}

void Wops::OnRTTWRTTAStopReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_RTTASTOP_REQ*	recvPacket = ( MSG_RTTW_RTTASTOP_REQ* )buffer;
	MSG_RTTW_RTTASTOP_ANS	sendPacket;

	OutputDebug( "Message : RTTA Stop request(flag = %d).\n", recvPacket->dwFlag );

	if( recvPacket->dwFlag == 0 )
	{
		ManagerService::Instance()->BroadcastRTTAStop();
	}
	else if( recvPacket->dwFlag == 2) // World 단위로 RTTAgent를 실행한다.
	{
		OutputDebug( "[Wops::OnRTTWRTTAStartReq]Send RTTAStop Broadcast\n");

		DWORD dwWordlID = _wtoi(recvPacket->szIPAddress);
		MSG_RTTR_RTTASTOP_NTF	packet;
		ManagerService::Instance()->Broadcast(dwWordlID, (char*)&packet, sizeof(MSG_RTTR_RTTASTOP_NTF));

		sendPacket.dwResult	= 0;
		wcsncpy( sendPacket.szIPAddress, recvPacket->szIPAddress, IP_MAX_LEN );

		SendMessage( sendPacket );
	}
	else
	{
		char					ipAddress[IP_MAX_LEN] = {0, };
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->szIPAddress, -1, ipAddress, IP_MAX_LEN, 0, 0 );

		RTTService* instance = ManagerService::Instance()->FindClient( ipAddress );

		if( instance == 0 )
		{
			//  이 상황은 발생되어서는 안된다. WOPS와 RTTS가 서로 갖고있는 RTTA의 정보가 다를 경우이기 때문이다.
			sendPacket.dwResult	= 2;
			wcsncpy( sendPacket.szIPAddress, recvPacket->szIPAddress, IP_MAX_LEN );

			SendMessage( sendPacket );

			OutputDebug( "[OnRTTWRTTAStopReq: RTTA(%s)를 검색하지 못하였습니다.\n", ipAddress );

			return;
		}

		//  성공
		instance->SendRTTAStop();

		sendPacket.dwResult	= 0;
		wcsncpy( sendPacket.szIPAddress, recvPacket->szIPAddress, IP_MAX_LEN );

		SendMessage( sendPacket );
	}
}

void Wops::OnRTTWRTTAPatchNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_RTTAPATCH_NTF*	recvPacket = ( MSG_RTTW_RTTAPATCH_NTF* )buffer;
	MSG_RTTR_RTTAPATCH_NTF	sendPacket;
	
	sendPacket.dwFtpPort = recvPacket->dwFtpPort;
	wcsncpy( sendPacket.szFtpAddress	, recvPacket->szFtpAddress		,wcslen(recvPacket->szFtpAddress)	);
	wcsncpy( sendPacket.szFtpUser		, recvPacket->szFtpUser			,wcslen(recvPacket->szFtpUser)		);
	wcsncpy( sendPacket.szFtpPassword	, recvPacket->szFtpPassword		,wcslen(recvPacket->szFtpPassword)	);
	wcsncpy( sendPacket.szFtpGetFilePath, recvPacket->szFtpGetFilePath	,wcslen(recvPacket->szFtpGetFilePath));
	wcsncpy( sendPacket.szFtpGetFileName, recvPacket->szFtpGetFileName	,wcslen(recvPacket->szFtpGetFileName));

	/*
	wcsncpy( sendPacket.szIp, recvPacket->szIp, 20 );
	wcsncpy( sendPacket.szPath, recvPacket->szPath, 40 );
	wcsncpy( sendPacket.szAccount, recvPacket->szAccount, 20 );
	wcsncpy( sendPacket.szPassword, recvPacket->szPassword, 40 );
	*/
	ManagerService::Instance()->Broadcast( recvPacket->dwWorldID, recvPacket->dwChannelID, ( char* )&sendPacket, sizeof( sendPacket ) );
}

void Wops::OnRTTWSystemRebootReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_SYSTEMREBOOT_NTF*		recvPacket = ( MSG_RTTW_SYSTEMREBOOT_NTF* )buffer;
	char							ipAddress[IP_MAX_LEN] = {0, };

	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szIPAddress, -1, ipAddress, IP_MAX_LEN, 0, 0 );

	RTTAgent* instance = ManagerClient::Instance()->FindClient( ipAddress );

	if( instance == 0 )
		return;

	instance->SendMessage( *recvPacket );
}

void Wops::OnRTTWProcessControlNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_PROCESSCONTROL_NTF*	recvPacket = ( MSG_RTTW_PROCESSCONTROL_NTF* )buffer;
	char							ipAddress[IP_MAX_LEN] = {0, };
	char							szPath[200] = {0, };
	char							szBinaryName[200] = {0, };

	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szIPAddress, -1, ipAddress, IP_MAX_LEN, 0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szBinaryName, -1, szBinaryName, 200, 0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szAbsolutePath, -1, szPath, 200, 0, 0 );
	
	RTTAgent* instance = ManagerClient::Instance()->FindClient( ipAddress );

	if( instance == 0 )
		return;

	instance->SendMessage( *recvPacket );

	OutputDebug("[Wops::OnRTTWProcessControlNTF] command : %d, Path : %s, Binary : %s\n", recvPacket->dwCommand, szPath, szBinaryName);
}

void Wops::OnRTTWConnectionTypeNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_CONNECTIONTYPE_NTF*	recvPacket = ( MSG_RTTW_CONNECTIONTYPE_NTF* )buffer;

	//  이미 설정된 session이 있다면 이전 설정은 무시한다.
	if( recvPacket->dwType == 0 )
	{
		ManagerWops::Instance()->SetSustenance( this );
		OutputDebug( "Sustenance WOPS connection linked.\n" );
	}
	else if( ManagerWops::Instance()->GetSustenance() == this )
	{
		ManagerWops::Instance()->SetSustenance( 0 );
		OutputDebug( "Sustenance WOPS connection linked. SetSustenance is Zero\n" );
	}
	else
	{
		OutputDebug( "WOPS connection linked.\n" );
	}
}

void Wops::OnRTTWServerShutdownReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_SERVERSHUTDOWN_REQ*	recvPacket = ( MSG_RTTW_SERVERSHUTDOWN_REQ* )buffer;

	recvPacket->dwKey	= GetUID();

	if(recvPacket->dwChannelID==10000)
	{
		recvPacket->dwChannelID = 255;
	}


	if( false == ManagerClient::Instance()->SendToMasterClient( *recvPacket ) )
	{
		OutputDebug( "[Wops::OnRTTWGameAdminRemoveNTF] Matched skel is not found.\n" );
	}

	{
		MSG_RTTW_SERVERSHUTDOWN_ANS	sendPacket;
		sendPacket.dwKey	= 0;
		sendPacket.dwError	= 0;
		sendPacket.m_ServerKey.Set(0, 0, 0, 0);
		sendPacket.m_ServerKey.dwKey = 0;
		SendMessage(sendPacket);
	}

	//MSG_RTTW_SERVERSHUTDOWN_REQ*	recvPacket = ( MSG_RTTW_SERVERSHUTDOWN_REQ* )buffer;

	//recvPacket->dwKey	= GetUID();

	//if(recvPacket->dwChannelID==10000)
	//{
	//	recvPacket->dwChannelID = 255;
	//}

	//RTTAgent* rttAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_MASTER );
	//if( rttAgent != 0 )
	//{
	//	rttAgent->SendMessage( *recvPacket );
	//}

	//{
	//	MSG_RTTW_SERVERSHUTDOWN_ANS	sendPacket;
	//	sendPacket.dwKey	= 0;
	//	sendPacket.dwError	= 0;
	//	sendPacket.m_ServerKey.Set(0, 0, 0, 0);
	//	sendPacket.m_ServerKey.dwKey = 0;
	//	SendMessage(sendPacket);
	//}
}

/*
	[2006.04.20] modify by guiun
	서버 패치 정보에 ServerType, ServerID 추가
*/
void Wops::OnRTTWLaunchPatcherReq( char* buffer, int size )
{
	MSG_RTTW_LAUNCHPATCHER_REQ*		recvPacket = ( MSG_RTTW_LAUNCHPATCHER_REQ* )buffer;

	//  보관 되어 있는 패치키를 삭제 한다.
	Database::Instance()->ClearPatchKeyAll();
	//  이 함수에서 관련된 서버들의 패치 상태가 초기화된다.
	//  두 키를 저장하여 PATCHRESULT_ANS가 오면 dwKey값으로 Wops 객체를 찾아내야 한다.
	recvPacket->dwKey = Database::Instance()->MakePatchKey(recvPacket->i64WopsKey, GetUID(), 
		                                                   (short)recvPacket->dwWorldID, 
														   (short)recvPacket->dwChannelID,
														   (short)recvPacket->dwServerType,
														   (short)recvPacket->dwServerID,
														   (short)recvPacket->dwServerTypeForProcess);

	OutputDebug( "LaunchPatcher: key = %d, (%d, %d)\n", 
					recvPacket->dwKey, 
					recvPacket->dwWorldID, 
					recvPacket->dwChannelID,
					recvPacket->dwServerType,
					recvPacket->dwServerID);

	//  관련 서버에 대한 Multi casting이다.
	//DWORD dwSize = sizeof(MSG_RTTW_LAUNCHPATCHER_REQ);	
	Database::Instance()->BroadcastForPatch((char*)recvPacket, size);

	/*
	ManagerClient::Instance()->Broadcast(recvPacket->dwWorldID, 
		                                 recvPacket->dwChannelID,
										 recvPacket->dwServerType,
										 recvPacket->dwServerID,
										 (char*)recvPacket, 
										 size);
	*/
	/*
		[2006.05.05] modify by guiun
		Patch를 받았으면 바로 wops에 통지를 한다.
	*/
	{	
		MSG_RTTW_PATCHRESULT_ANS sendPacket;
		sendPacket.dwKey	= (DWORD)recvPacket->i64WopsKey;
		sendPacket.dwError	= 0xFF;
		sendPacket.byVer1	= 0;
		sendPacket.byVer2	= 0;
		sendPacket.byVer3	= 0;

		SendMessage( sendPacket );
	}
}

void Wops::OnRTTWLaunchProcessReq( char* buffer, int /*size*/ )
{
	MSG_RTTW_LAUNCHPROCESS_REQ*		recvPacket = ( MSG_RTTW_LAUNCHPROCESS_REQ* )buffer;

	//  전체 World는 지원하지않는다.
	//  서비스 실행에서 Boot Sequence상 인증 관련부분은 가장 마지막에
	//  실행되어야 하기 때문이다.
	if( recvPacket->dwWorldID == 0 )
		return;

	OutputDebug( "[OnRTTWLaunchProcessReq: TickID %d.\n", recvPacket->i64WopsKey);
	
	recvPacket->dwWopsUID = GetUID();
	Database::Instance()->ClearLaunchProcessStatus(( short )recvPacket->dwWorldID, 
		                                           ( short )recvPacket->dwChannelID,
												   ( short )recvPacket->dwServerType, 
												   ( short )recvPacket->dwServerID);

	//  관련 서버에 대한 Multi casting이다.
	ManagerClient::Instance()->Broadcast(recvPacket->dwWorldID, 
		                                 recvPacket->dwChannelID,
										 recvPacket->dwServerType,
										 recvPacket->dwServerID,
										 ( char* )recvPacket, sizeof( *recvPacket ) );
	{
		MSG_RTTW_LAUNCHPROCESS_ANS sendPacket;
		sendPacket.dwError = 0;
		SendMessage(sendPacket);
	}
}

void Wops::OnRTTWFuncrestrictReloadNTF(char* buffer, int /*size*/ )
{
	MSG_RTTW_FUNCRESTRICT_RELOAD_NTF*	recvPacket = ( MSG_RTTW_FUNCRESTRICT_RELOAD_NTF* )buffer;
	recvPacket->dwWopsKey = GetUID();

	if( false == ManagerClient::Instance()->SendToMasterClient( *recvPacket ) )
	{
		MSG_RTTW_FUNCRESTRICT_RELOAD_ANS	resultPacket;
		resultPacket.dwResult = 0;	// 초기 실패로 설정
		resultPacket.dwWopsKey = recvPacket->dwWopsKey;
		SendMessage(resultPacket);

		OutputDebug("[Wops::OnRTTWFuncrestrictReloadNTF] Failed RTTAgent(Master) Search \n");
	}
	else
	{
		OutputDebug("[Wops::OnRTTWFuncrestrictReloadNTF] Send To RTTAgent(Master) \n");
	}

	//MSG_RTTW_FUNCRESTRICT_RELOAD_NTF*	recvPacket = ( MSG_RTTW_FUNCRESTRICT_RELOAD_NTF* )buffer;
	//RTTAgent* pRTTAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_MASTER );
	//
	//recvPacket->dwWopsKey = GetUID();

	//if( pRTTAgent != 0 )
	//{
	//	pRTTAgent->SendMessage( *recvPacket );

	//	OutputDebug("[Wops::OnRTTWFuncrestrictReloadNTF] send to RTTAgent %s  \n", pRTTAgent->GetIPAddress());
	//}
	//else
	//{
	//	OutputDebug("[Wops::OnRTTWFuncrestrictReloadNTF] fail rttagent search \n");
	//}
}

void Wops::OnRTTAuthIPListReloadReq(char* buffer, int /*size*/ )
{
	MSG_RTTW_AUTHIPLIST_RELOAD_REQ*	recvPacket = ( MSG_RTTW_AUTHIPLIST_RELOAD_REQ* )buffer;
	RTTAgent* pRTTAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_AUTH );
	
	recvPacket->dwWopsKey = GetUID();

	if( pRTTAgent != 0 ){
		pRTTAgent->SendMessage( *recvPacket );
		OutputDebug("[Wops::OnRTTAuthIPListReloadReq] send to RTTAgent %s  \n", pRTTAgent->GetIPAddress());
	}
	else{ // 인증 서버가 설치 되어 있는 RTTAgent 검색 실패
		MSG_RTTW_AUTHIPLIST_RELOAD_ANS		sendPacket;

		sendPacket.dwResult		= 0;
		sendPacket.dwWopsKey	= recvPacket->dwWopsKey;
		SendMessage(sendPacket);

		OutputDebug("[Wops::OnRTTAuthIPListReloadReq] RTTAgent Search Fail  \n");
	}
}

void Wops::OnRTTGameVersionUpdateReq(char* buffer, int /*size*/ )
{
	MSG_RTTW_GAMEVERSION_UPDATE_REQ*	recvPacket = ( MSG_RTTW_GAMEVERSION_UPDATE_REQ* )buffer;
	RTTAgent* pRTTAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_AUTH );
	
	recvPacket->dwWopsKey = GetUID();

	if( pRTTAgent != 0 ){
		pRTTAgent->SendMessage( *recvPacket );
	}
	else{ // 인증 서버가 설치 되어 있는 RTTAgent 검색 실패
		MSG_RTTW_GAMEVERSION_UPDATE_ANS		sendPacket;

		sendPacket.dwResult		= 0;
		sendPacket.dwWopsKey	= recvPacket->dwWopsKey;
		SendMessage(sendPacket);

		OutputDebug("[Wops::OnRTTGameVersionUpdateReq] RTTAgent Search Fail  \n");
	}
}

//void Wops::OnRTTPatternReloadReq(char* buffer, int /*size*/ )
//{
//	//MSG_RTTW_PATTERN_RELOAD_ANS		sendPacket;
//
//	//sendPacket.dwResult		= 1;
//	//sendPacket.dwWopsKey	= GetUID();
//
//	//// 어차피 자기 자신한테 가는거라 Find할 필요는 없지만 그래도 한번...-_-;
//
//	////  저장된 key로 '정상종료'를 요청한 Wops Component를 찾는다.
//	//Wops* pWops = ( Wops* )ManagerWops::Instance()->Find( sendPacket.dwWopsKey );
//	//
//	//if( pWops != 0 )
//	//{
//	//	pWops->SendMessage( sendPacket );
//	//}
//	//else
//	//{
//	//	OutputDebug( "[RTTAgent::OnRTTPatternReloadReq] not found wops \n");
//	//}
//
//	//OutputDebug("[Skel::OnRTTPatternReloadReq] RESULT(%d) \n", sendPacket.dwResult);
//
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
//
//	MSG_RTTW_PATTERN_RELOAD_REQ*	recvPacket = ( MSG_RTTW_PATTERN_RELOAD_REQ* )buffer;
//	RTTAgent* pRTTAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_AUTH );
//	
//	recvPacket->dwWopsKey = GetUID();
//
//	if( pRTTAgent != 0 ){
//		pRTTAgent->SendMessage( *recvPacket );
//		OutputDebug("[Wops::OnRTTPatternReloadReq] send to RTTAgent %s  \n", pRTTAgent->GetIPAddress());
//	}
//	else{ // 인증 서버가 설치 되어 있는 RTTAgent 검색 실패
//		MSG_RTTW_PATTERN_RELOAD_ANS		sendPacket;
//
//		sendPacket.dwResult		= 0;
//		sendPacket.dwWopsKey	= recvPacket->dwWopsKey;
//		SendMessage(sendPacket);
//
//		OutputDebug("[Wops::OnRTTPatternReloadReq] RTTAgent Search Fail  \n");
//	}
//}

void Wops::OnRTTWArmorkitDispatchNTF(char* buffer, int /*size*/ )
{
	MSG_RTTW_ARMORKIT_DISPATCH_NTF*	recvPacket = ( MSG_RTTW_ARMORKIT_DISPATCH_NTF* )buffer;
	RTTAgent* pRTTAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_AUTH );
	
	recvPacket->dwWopsKey = GetUID();

	if( pRTTAgent != 0 )
	{
		pRTTAgent->SendMessage( *recvPacket );

		OutputDebug("[Wops::OnRTTWArmorkitDispatchNTF] send to RTTAgent %s  \n", pRTTAgent->GetIPAddress());
	}
	else
	{
		OutputDebug("[Wops::OnRTTWArmorkitDispatchNTF] fail rttagent search \n");
	}
}

void Wops::OnRTTWChangeGuildNameReq(char* buffer, int size)
{
	MSG_RTTW_CHANGE_GUILDNAME_REQ*	recvPacket = ( MSG_RTTW_CHANGE_GUILDNAME_REQ* )buffer;
	
//*////////////////////////////////////////////////////////// 로그

	char szGuildName[ MSG_RTTW_CHANGE_GUILDNAME_REQ::MAX_GUILD_NAME_LENGTH + 1 ]		= {0, };
	char szDescription[ MSG_RTTW_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH * 2 + 1 ]	= {0, };	// 유니코드 256자 이므로 멀티바이트는 최대 2배로 잡아야한다

	int nMaxGuildNameLen	= MSG_RTTW_CHANGE_GUILDNAME_REQ::MAX_GUILD_NAME_LENGTH;
	int nMaxDescriptionLen	= MSG_RTTW_CHANGE_GUILDNAME_REQ::MAX_DESCRIPTION_LENGTH;

	::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszGuildName,		nMaxGuildNameLen,		szGuildName,	nMaxGuildNameLen,		0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszDescription,	nMaxDescriptionLen,		szDescription,	nMaxDescriptionLen * 2,	0, 0 );

	szGuildName[nMaxGuildNameLen] = NULL;
	szDescription[nMaxDescriptionLen * 2] = NULL;

	OutputDebug( "[Wops::OnRTTWChangeGuildNameReq]		WopsKey		: %d\n", GetUID() );
	OutputDebug( "[Wops::OnRTTWChangeGuildNameReq]		World		: %d\n", recvPacket->dwServerGuid.GetWorldID() );
	OutputDebug( "[Wops::OnRTTWChangeGuildNameReq]		GuildGuid	: %d\n", recvPacket->dwGuildGuid);
	OutputDebug( "[Wops::OnRTTWChangeGuildNameReq]		GuildName	: %s\n", szGuildName);
	OutputDebug( "[Wops::OnRTTWChangeGuildNameReq]		Description	: %s\n", szDescription);

////////////////////////////////////////////////////////////*/


	eXtreme::Stream					stream( 1024 );

	stream << ( DWORD )( 4 + sizeof( *recvPacket ) ) << ( DWORD )RTTW_CHANGE_GUILDNAME_REQ << ( DWORD )GetUID();
	stream.Write( recvPacket, sizeof( *recvPacket ) );

	if( false == ManagerClient::Instance()->SendToMasterClient( stream, recvPacket->dwServerGuid.GetWorldID() ) )
	{
		MSG_RTTW_CHANGE_GUILDNAME_ANS	sendResultPacket;

		//  해당 RTTAgent가 아직 등록되지 않았다.
		sendResultPacket.dwResult = 2;		//  RTTAgent와의 통신에러

		wcscpy(sendResultPacket.wszResultDescription, L"전체 혹은 일부 Master RTTAgent를 찾을 수 없습니다");

		SendMessage( sendResultPacket );
		
		OutputDebug( "[Wops::OnRTTWChangeGuildNameReq] RTTAgent Search Failed\n");

		return;
	}

	OutputDebug( "[Wops::OnRTTWChangeGuildNameReq] Send To RttAgent(%d World)\n", recvPacket->dwServerGuid.GetWorldID() );


	//RTTAgent* rttAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_MASTER );
	//if( rttAgent == 0 )
	//{
	//	MSG_RTTW_CHANGE_GUILDNAME_ANS	sendResultPacket;

	//	//  해당 RTTAgent가 아직 등록되지 않았다.
	//	sendResultPacket.dwResult = 2;		//  RTTAgent와의 통신에러

	//	wcscpy(sendResultPacket.wszResultDescription, L"Master RTTAgent를 찾을 수 없습니다");

	//	SendMessage( sendResultPacket );
	//	
	//	OutputDebug( "[Wops::OnRTTWChangeGuildNameReq] RTTAgent Search Failed\n");

	//	return;
	//}

	//eXtreme::Stream					stream( 1024 );

	//stream << ( DWORD )( 4 + sizeof( *recvPacket ) ) << ( DWORD )RTTW_CHANGE_GUILDNAME_REQ << ( DWORD )GetUID();
	//stream.Write( recvPacket, sizeof( *recvPacket ) );

	//rttAgent->SendPacket( stream );

	//OutputDebug( "[Wops::OnRTTWChangeGuildNameReq] Send To RttAgent\n");
}

void Wops::OnRTTWSendMessageReq(char* buffer, int size)
{
	MSG_RTTW_SEND_MESSAGE_REQ*	recvPacket = ( MSG_RTTW_SEND_MESSAGE_REQ* )buffer;

//*////////////////////////////////////////////////////////// 로그 

	char szRecvCharName[ MSG_RTTW_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH + 1 ]	= {0, };
	char szSendCharName[ MSG_RTTW_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH + 1 ]	= {0, };
	char szMessage[ MSG_RTTW_SEND_MESSAGE_REQ::MAX_MESSAGE_LENGTH * 2 + 1 ]		= {0, };	// 유니코드 100 자이므로 멀티바이트는 최대 2배로로 잡아야한다

	int nMaxCharNameLen	= MSG_RTTW_SEND_MESSAGE_REQ::MAX_CHAR_NAME_LENGTH;
	int nMaxMessageLen	= MSG_RTTW_SEND_MESSAGE_REQ::MAX_MESSAGE_LENGTH;

	OutputDebug( "[Wops::OnRTTWSendMessageReq] Send To RttAgent(Count : %d)\n", recvPacket->dwCount);
	OutputDebug( "[Wops::OnRTTWSendMessageReq]		WopsKey		: %d\n", GetUID() );
	OutputDebug( "[Wops::OnRTTWSendMessageReq]		World		: %d\n", recvPacket->dwServerGuid.GetWorldID() );

	for(DWORD dwLogIndex = 0 ; (dwLogIndex < recvPacket->dwCount) && (dwLogIndex < MSG_RTTW_SEND_MESSAGE_REQ::MAX_USER_INFO) ; ++dwLogIndex)
	{
		::WideCharToMultiByte( CP_ACP, 0, recvPacket->arrUserInfoList[dwLogIndex].wszRecvCharName,	nMaxCharNameLen,	szRecvCharName,	nMaxCharNameLen,	0, 0 );

		szRecvCharName[nMaxCharNameLen] = NULL;

		OutputDebug( "[Wops::OnRTTWSendMessageReq]		RecvChar	: %s\n", szRecvCharName);

		memset(szRecvCharName,	0, sizeof(szRecvCharName));
	}

	::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszSendCharName,	nMaxCharNameLen,	szSendCharName,	nMaxCharNameLen,	0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->wszMessage,		nMaxMessageLen,		szMessage,		nMaxMessageLen * 2,	0, 0 );

	szSendCharName[nMaxCharNameLen] = NULL;
	szMessage[nMaxMessageLen * 2] = NULL;

	OutputDebug( "[Wops::OnRTTWSendMessageReq]		SendChar	: %s\n", szSendCharName);
	OutputDebug( "[Wops::OnRTTWSendMessageReq]		Message		: %s\n", szMessage);

////////////////////////////////////////////////////////////*/

	eXtreme::Stream					stream( 4096 );

	stream << ( DWORD )( 4 + sizeof( *recvPacket ) ) << ( DWORD )RTTW_SEND_MESSAGE_REQ << ( DWORD )GetUID();
	stream.Write( recvPacket, sizeof( *recvPacket ) );

	if( false == ManagerClient::Instance()->SendToMasterClient( stream, recvPacket->dwServerGuid.GetWorldID() ) )
	{
		MSG_RTTW_SEND_MESSAGE_ANS	sendResultPacket;

		//  해당 RTTAgent가 아직 등록되지 않았다.
		sendResultPacket.dwCount = recvPacket->dwCount;

		for(DWORD dwIndex = 0 ; (dwIndex < sendResultPacket.dwCount) && (dwIndex < MSG_RTTW_SEND_MESSAGE_ANS::MAX_RESULT_INFO) ; ++dwIndex)
		{
			sendResultPacket.arrResultInfoList[dwIndex].dwResult = 2; //  RTTAgent와의 통신에러
			::wcsncpy( sendResultPacket.arrResultInfoList[dwIndex].wszRecvCharName, recvPacket->arrUserInfoList[dwIndex].wszRecvCharName, MSG_RTTW_SEND_MESSAGE_ANS::MAX_CHAR_NAME_LENGTH );
			wcscpy(sendResultPacket.arrResultInfoList[dwIndex].wszResultDescription, L"Master RTTAgent를 찾을 수 없습니다");
		}

		SendMessage( sendResultPacket );
		
		OutputDebug( "[Wops::OnRTTWSendMessageReq] RTTAgent Search Failed( %d World, Count : %d)\n", recvPacket->dwServerGuid.GetWorldID(), recvPacket->dwCount);

		return;
	}

	OutputDebug( "[Wops::OnRTTWSendMessageReq] Send To RttAgent(%d World)\n", recvPacket->dwServerGuid.GetWorldID() );

	//RTTAgent* rttAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_MASTER );
	//if( rttAgent == 0 )
	//{
	//	MSG_RTTW_SEND_MESSAGE_ANS	sendResultPacket;

	//	//  해당 RTTAgent가 아직 등록되지 않았다.
	//	sendResultPacket.dwCount = recvPacket->dwCount;

	//	for(DWORD dwIndex = 0 ; (dwIndex < sendResultPacket.dwCount) && (dwIndex < MSG_RTTW_SEND_MESSAGE_ANS::MAX_RESULT_INFO) ; ++dwIndex)
	//	{
	//		sendResultPacket.arrResultInfoList[dwIndex].dwResult = 2; //  RTTAgent와의 통신에러
	//		::wcsncpy( sendResultPacket.arrResultInfoList[dwIndex].wszRecvCharName, recvPacket->arrUserInfoList[dwIndex].wszRecvCharName, MSG_RTTW_SEND_MESSAGE_ANS::MAX_CHAR_NAME_LENGTH );
	//		wcscpy(sendResultPacket.arrResultInfoList[dwIndex].wszResultDescription, L"Master RTTAgent를 찾을 수 없습니다");
	//	}

	//	SendMessage( sendResultPacket );
	//	
	//	OutputDebug( "[Wops::OnRTTWSendMessageReq] RTTAgent Search Failed(Count : %d)\n", recvPacket->dwCount);

	//	return;
	//}

	//eXtreme::Stream					stream( 4096 );

	//stream << ( DWORD )( 4 + sizeof( *recvPacket ) ) << ( DWORD )RTTW_SEND_MESSAGE_REQ << ( DWORD )GetUID();
	//stream.Write( recvPacket, sizeof( *recvPacket ) );

	//rttAgent->SendPacket( stream );
}