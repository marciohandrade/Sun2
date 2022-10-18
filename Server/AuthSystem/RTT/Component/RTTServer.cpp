#include "stdafx.h"
#include "Entry.h"
#include "RTTServer.h"
#include "../common/Protocol_RTT.h"


#define  LIMITE_CONNECTTICK		3000

//  Connection state
#define  CS_CLOSED				0x0001
#define  CS_TRYCONNECT			0x0002
#define  CS_CONNECTED			0x0003




RTTServer::RTTServer()
	: eXtreme::ServerSession( 20480, 20480 )
{
	m_disconnTick	= 0;
	m_connectState	= CS_CLOSED;
}

RTTServer::~RTTServer()
{
}

void RTTServer::OnConnect( bool success, DWORD error )
{
	if( success == true )
	{
		printf( "Press Any Number...\n" );

		//printf( "1 - Server shutdown\n" );
		//printf( "2 - Launch patch\n" );
		//printf( "3 - Launch all process\n" );
		//printf( "4 - Server list\n" );
		//printf( "5 - RTT Patch\n" );

		GetHandle()->SetBufferSize( 65535, 65535 );
		m_connectState = CS_CONNECTED;
	}
	else
	{
		m_connectState	= CS_CLOSED;
		m_disconnTick	= ::GetTickCount();
	}
}

void RTTServer::OnClose( DWORD error )
{
	m_connectState	= CS_CLOSED;
	m_disconnTick	= ::GetTickCount();
}

void RTTServer::CheckDisconnected()
{
	if( m_connectState != CS_CLOSED )
		return;

	//  연결을 재시도 한다.
	if( ::GetTickCount() - m_disconnTick > LIMITE_CONNECTTICK )
	{
		m_connectState	= CS_TRYCONNECT;
		m_disconnTick	= ::GetTickCount();

		char	ip[20];
		int		port;

		::GetPrivateProfileString( "RTTServer", "IpAddress", "err", ip, 20, "./Component.ini" );
		port = ::GetPrivateProfileInt( "RTTServer", "PortNo", 0, "./Component.ini" );


		Connect( ip, port );
	}
}

void RTTServer::Update()
{
	CheckDisconnected();
	if( m_connectState != CS_CONNECTED )
		return;

	if( kbhit() != 0 )
	{
		char ch = getch();

		if( ch == '1' )
			SendArmorkitDispatchNTF();//SendGameNoticeReq();
		if( ch == '2' )
			SendGameNoticeReq();//SendLaunchPatcherReq();		
		if( ch == '3' )
			SendChangeGuildNameReq();//	SendLaunchProcessReq();
		if( ch == '4' )
			SendSendMessageReq();//	SendServerListReq();
		if( ch == '5' )
			SendRTTAPatchNTF();
		if( ch == '6' )
			SendFuncrestrictReloadNTF();
		if( ch == '7')
			SendRTTG_DROPUSER_REQ();
		if( ch == '8')
			SendAuthIPListReloadReq();
		if( ch == '9')
			SendGameVersionUpdateReq();
		if( ch == 'a')
			SendDropUserFromWebReq();

	}

	eXtreme::ServerSession::Update();
	return;

	int r = rand() % 12;

	if( r == 0 )	SendServerListReq();
	if( r == 1 )	SendGameNoticeReq();
	if( r == 2 )	SendGameAdminListReq();
	if( r == 3 )	SendGameAdminAddNTF();
	if( r == 4 )	SendRTTAStartReq();
	if( r == 5 )	SendRTTAStopReq();
	if( r == 6 )	SendRTTAPatchNTF();
	//if( r == 7 )	SendSystemRebootNTF();
	if( r == 8 )	SendProcessControlNTF();
	if( r == 9 )	SendConnectionTypeNTF();
	if( r == 10 )	SendServerShutdownReq();
	if( r == 11 )	SendLaunchPatcherReq();

	eXtreme::ServerSession::Update();
}

void RTTServer::SendServerListReq()
{
	MSG_RTTW_SERVERLIST_REQ		sendPacket;

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendGameNoticeReq()
{
	
	MSG_RTTW_GAMENOTICE_REQ		sendPacket;
	
	//16777216,33554432,50331648,67108864,83886080
	sendPacket.dwServerUID		= 16909060;
	sendPacket.dwTickPerLine	= 0;
	sendPacket.dwNoticeLength	= 6;
	wcscpy( sendPacket.szNotice, L"공지 테스트" );
	SendPacket( &sendPacket, sizeof( sendPacket ) );
	
//	{
//		MSG_RTTW_GAMENOTICE_REQ		sendPacket;
//	
//		sendPacket.dwServerUID		= 16777216;
//		sendPacket.dwTickPerLine	= 0;
//		sendPacket.dwNoticeLength	= 6;
//		wcscpy( sendPacket.szNotice, L"공지 테스트" );
//		SendPacket( &sendPacket, sizeof( sendPacket ) );
//	}Sleep(1000);
//{
//		MSG_RTTW_GAMENOTICE_REQ		sendPacket;
//	
//		sendPacket.dwServerUID		= 33554432;
//		sendPacket.dwTickPerLine	= 0;
//		sendPacket.dwNoticeLength	= 6;
//		wcscpy( sendPacket.szNotice, L"공지 테스트" );
//		SendPacket( &sendPacket, sizeof( sendPacket ) );
//	}Sleep(1000);
//{
//		MSG_RTTW_GAMENOTICE_REQ		sendPacket;
//	
//		sendPacket.dwServerUID		= 50331648;
//		sendPacket.dwTickPerLine	= 0;
//		sendPacket.dwNoticeLength	= 6;
//		wcscpy( sendPacket.szNotice, L"공지 테스트" );
//		SendPacket( &sendPacket, sizeof( sendPacket ) );
//	}Sleep(1000);
//{
//		MSG_RTTW_GAMENOTICE_REQ		sendPacket;
//	
//		sendPacket.dwServerUID		= 67108864;
//		sendPacket.dwTickPerLine	= 0;
//		sendPacket.dwNoticeLength	= 6;
//		wcscpy( sendPacket.szNotice, L"공지 테스트" );
//		SendPacket( &sendPacket, sizeof( sendPacket ) );
//	}Sleep(1000);
//{
//		MSG_RTTW_GAMENOTICE_REQ		sendPacket;
//	
//		sendPacket.dwServerUID		= 83886080;
//		sendPacket.dwTickPerLine	= 0;
//		sendPacket.dwNoticeLength	= 6;
//		wcscpy( sendPacket.szNotice, L"공지 테스트" );
//		SendPacket( &sendPacket, sizeof( sendPacket ) );
//	}Sleep(1000);

}

void RTTServer::SendGameAdminListReq()
{
	MSG_RTTW_GAMEADMINLIST_REQ	sendPacket;

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendGameAdminAddNTF()
{
	MSG_RTTW_GAMEADMINADD_NTF	sendPacket;

	sendPacket.dwAttr	= 0;
	wcscpy( sendPacket.szAccount, L"Testor1" );
	wcscpy( sendPacket.szIP, L"127.0.0.1" );

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendRTTAStartReq()
{
	MSG_RTTW_RTTASTART_REQ		sendPacket;

	sendPacket.dwFlag	= 1;
	wcscpy( sendPacket.szIPAddress, L"127.0.0.1" );

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendRTTAStopReq()
{
	MSG_RTTW_RTTASTOP_REQ		sendPacket;

	sendPacket.dwFlag	= 1;
	wcscpy( sendPacket.szIPAddress, L"127.0.0.1" );

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendRTTAPatchNTF()
{
	MSG_RTTW_RTTAPATCH_NTF		sendPacket;

	sendPacket.dwWorldID	= 5;
	sendPacket.dwChannelID	= 1;

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendSystemRebootNTF()
{
	MSG_RTTW_SYSTEMREBOOT_NTF	sendPacket;

	wcscpy( sendPacket.szIPAddress, L"127.0.0.1" );

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendProcessControlNTF()
{
	MSG_RTTW_PROCESSCONTROL_NTF	sendPacket;

	sendPacket.dwCommand = 1;
	wcscpy( sendPacket.szIPAddress, L"10.1.25.132" );
	wcscpy( sendPacket.szBinaryName, L"TestApp_1.exe" );
	wcscpy( sendPacket.szAbsolutePath, L"D:\\ProcessTest\\Test1___" );

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendConnectionTypeNTF()
{
	MSG_RTTW_CONNECTIONTYPE_NTF	sendPacket;

	sendPacket.dwID = rand() % 2;

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendServerShutdownReq()
{
	MSG_RTTW_SERVERSHUTDOWN_REQ	sendPacket;

	sendPacket.dwWorldID	= ::GetPrivateProfileInt( "Test", "WorldID", 0, "./Component.ini" );
	sendPacket.dwChannelID	= ::GetPrivateProfileInt( "Test", "ChannelID", 0, "./Component.ini" );
	sendPacket.dwServerType	= ::GetPrivateProfileInt( "Test", "ServerType", 0, "./Component.ini" );
	sendPacket.dwServerID	= ::GetPrivateProfileInt( "Test", "ServerID", 0, "./Component.ini" );
	sendPacket.i64WopsKey   = 632824604280415608;
	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendLaunchPatcherReq()
{
	MSG_RTTW_LAUNCHPATCHER_REQ	sendPacket;

	sendPacket.dwWorldID	= ::GetPrivateProfileInt( "Test", "WorldID", 0, "./Component.ini" );
	sendPacket.dwChannelID	= ::GetPrivateProfileInt( "Test", "ChannelID", 0, "./Component.ini" );
	sendPacket.dwServerType	= ::GetPrivateProfileInt( "Test", "ServerType", 0, "./Component.ini" );
	sendPacket.dwServerID	= ::GetPrivateProfileInt( "Test", "ServerID", 0, "./Component.ini" );
	sendPacket.dwServerTypeForProcess = ::GetPrivateProfileInt( "Test", "ServerTypeForProcess", 0, "./Component.ini" );
	sendPacket.i64WopsKey	= ::GetTickCount();
	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendLaunchProcessReq()
{
	MSG_RTTW_LAUNCHPROCESS_REQ	sendPacket;

	sendPacket.dwWorldID	= ::GetPrivateProfileInt( "Test", "WorldID", 0, "./Component.ini" );
	sendPacket.dwChannelID	= ::GetPrivateProfileInt( "Test", "ChannelID", 0, "./Component.ini" );
	sendPacket.dwServerType	= ::GetPrivateProfileInt( "Test", "ServerType", 0, "./Component.ini" );
	sendPacket.dwServerID	= ::GetPrivateProfileInt( "Test", "ServerID", 0, "./Component.ini" );
	sendPacket.dwServerTypeForProcess = 0;
	sendPacket.i64WopsKey	= 632825529514851627;
	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendFuncrestrictReloadNTF()
{
	MSG_RTTW_FUNCRESTRICT_RELOAD_NTF sendPacket;
	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendAuthIPListReloadReq()
{
	MSG_RTTW_AUTHIPLIST_RELOAD_REQ sendPacket;
	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendGameVersionUpdateReq()
{
	MSG_RTTW_GAMEVERSION_UPDATE_REQ sendPacket;
	
	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendRTTG_DROPUSER_REQ()
{
	MSG_RTTW_DROPUSER_REQ sendPacket;
	wcscpy(sendPacket.szUserID, L"OPENTEST0439");

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendArmorkitDispatchNTF()
{
	MSG_RTTW_ARMORKIT_DISPATCH_NTF sendPacket;
	sendPacket.dwWopsKey = ::GetTickCount();
	SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendChangeGuildNameReq()
{
	MSG_RTTW_CHANGE_GUILDNAME_REQ	sendPacket;

	SERVER_KEY	serverKey;
	serverKey.Set(1, 0, 0, 0);

	sendPacket.dwServerGuid = serverKey;
	sendPacket.dwGuildGuid = 33177021;
	::wcscpy( sendPacket.wszGuildName, L"썬길드" );
	::wcscpy( sendPacket.wszDescription, L"이름바꿈" );

    SendPacket( &sendPacket, sizeof( sendPacket ) );
}

void RTTServer::SendSendMessageReq()
{
	MSG_RTTW_SEND_MESSAGE_REQ	sendPacket;

	SERVER_KEY	serverKey;
	serverKey.Set(1, 0, 0, 0);

	sendPacket.dwServerGuid = serverKey;
	sendPacket.dwCount = 5;

	for( DWORD dwIndex = 0 ; dwIndex < sendPacket.dwCount ; ++dwIndex )
	{
		::swprintf( sendPacket.arrUserInfoList[dwIndex].wszRecvCharName, L"%s%d", L"썬유저", dwIndex);
	}

	::wcscpy( sendPacket.wszSendCharName, L"GM대우" );
	::wcscpy( sendPacket.wszMessage, L"누구십니까?" );

	SendPacket( &sendPacket, sizeof( sendPacket ) );

}

void RTTServer::SendDropUserFromWebReq()
{
	MSG_RTTH_DROPUSER_REQ	sendPacket;

	wcscpy(	sendPacket.szUserID, L"Test" );

	SendPacket( &sendPacket, sizeof( sendPacket ) );
}


void RTTServer::OnReceive( char* buffer, int size )
{
	MSG_HEADER8*		header = ( MSG_HEADER8* )buffer;

	switch( header->dwID )
	{
	case  RTTW_SERVERLIST_ANS:			OnRTTWServerListAns( buffer, size );		break;
	case  RTTW_GAMENOTICE_ANS:			OnRTTWGameNoticeAns( buffer, size );		break;
	case  RTTW_GAMEADMINLIST_ANS:		OnRTTWGameAdminListAns( buffer, size );		break;
	case  RTTW_GAMEADMINREMOVE_NTF:		OnRTTWGameAdminRemoveNTF( buffer, size );	break;
	case  RTTW_RTTASTART_ANS:			OnRTTWRTTAStartAns( buffer, size );			break;
	case  RTTW_RTTASTOP_ANS:			OnRTTWRTTAStopAns( buffer, size );			break;
	case  RTTW_ERRORNOTIFY_NTF:			OnRTTWErrorNotifyNTF( buffer, size );		break;
	case  RTTW_PATCHRESULT_ANS:			OnRTTWPatchResultAns( buffer, size );		break;
	case  RTTW_LAUNCHPROCESS_ANS:		OnRTTWLaunchProcessAns( buffer, size );		break;
	case  RTTW_SERVERSHUTDOWN_ANS:		OnRTTWServerShutdownAns( buffer, size );	break;
	case  RTTW_AUTHIPLIST_RELOAD_ANS:	OnRTTWAuthIPListReloadAns( buffer, size );	break;
	case  RTTW_DROPUSER_ANS:			OnRTTW_DROPUSER_Ans( buffer, size );		break;
	case  RTTW_ARMORKIT_DISPATCH_ANS:	OnRTTWArmorkitDispatchAns( buffer, size );	break;
	case  RTTW_CHANGE_GUILDNAME_ANS:	OnRTTWChangeGuildNameAns( buffer, size );	break;
	case  RTTW_SEND_MESSAGE_ANS:		OnRTTWSendMessageAns( buffer, size );		break;
	case  RTTH_DROPUSER_ANS:			OnRTTH_DROPUSER_Ans( buffer, size );		break;
	}
}

void RTTServer::OnRTTWServerListAns( char* buffer, int size )
{
	MSG_RTTW_SERVERLIST_ANS*	recvPacket = ( MSG_RTTW_SERVERLIST_ANS* )buffer;
}

void RTTServer::OnRTTWGameNoticeAns( char* buffer, int size )
{
	MSG_RTTW_GAMENOTICE_ANS*	recvPacket = ( MSG_RTTW_GAMENOTICE_ANS* )buffer;
}

void RTTServer::OnRTTWGameAdminListAns( char* buffer, int size )
{
	MSG_RTTW_GAMEADMINLIST_ANS*	recvPacket = ( MSG_RTTW_GAMEADMINLIST_ANS* )buffer;
}

void RTTServer::OnRTTWGameAdminRemoveNTF( char* buffer, int size )
{
	MSG_RTTW_GAMEADMINREMOVE_NTF*	recvPacket = ( MSG_RTTW_GAMEADMINREMOVE_NTF* )buffer;
}

void RTTServer::OnRTTWRTTAStartAns( char* buffer, int size )
{
	MSG_RTTW_RTTASTART_ANS*	recvPacket = ( MSG_RTTW_RTTASTART_ANS* )buffer;
}

void RTTServer::OnRTTWRTTAStopAns( char* buffer, int size )
{
	MSG_RTTW_RTTASTOP_ANS*	recvPacket = ( MSG_RTTW_RTTASTOP_ANS* )buffer;
}

void RTTServer::OnRTTWErrorNotifyNTF( char* buffer, int size )
{
	MSG_RTTW_ERRORNOTIFY_NTF*	recvPacket = ( MSG_RTTW_ERRORNOTIFY_NTF* )buffer;
}

void RTTServer::OnRTTWPatchResultAns( char* buffer, int size )
{
	MSG_RTTW_PATCHRESULT_ANS*	recvPacket = ( MSG_RTTW_PATCHRESULT_ANS* )buffer;

	OutputDebug( "[OnRTTWPatchResultAns] err=%d\n", recvPacket->dwError );
}

void RTTServer::OnRTTWLaunchProcessAns( char* buffer, int size )
{
	MSG_RTTW_LAUNCHPROCESS_ANS*	recvPacket = ( MSG_RTTW_LAUNCHPROCESS_ANS* )buffer;

	OutputDebug( "[OnRTTWLaunchProcessAns] err=%d\n", recvPacket->dwError );
}

void RTTServer::OnRTTWServerShutdownAns( char* buffer, int size )
{
	MSG_RTTW_SERVERSHUTDOWN_ANS*	recvPacket = ( MSG_RTTW_SERVERSHUTDOWN_ANS* )buffer;

	OutputDebug( "[OnRTTWServerShutdownAns] err=%d\n", recvPacket->dwError );
}
void RTTServer::OnRTTWAuthIPListReloadAns( char* buffer, int size )
{
	MSG_RTTW_AUTHIPLIST_RELOAD_ANS*	recvPacket = ( MSG_RTTW_AUTHIPLIST_RELOAD_ANS* )buffer;

	OutputDebug( "[OnRTTWAuthIPListReloadAns] err=%d\n", recvPacket->dwResult );
}

void RTTServer::OnRTTW_DROPUSER_Ans( char* buffer, int size )
{
	MSG_RTTW_DROPUSER_ANS*	recvPacket = ( MSG_RTTW_DROPUSER_ANS* )buffer;

	OutputDebug( "[RTTServer::OnRTTW_DROPUSER_Ans] err=%d\n", recvPacket->dwError );
}

void RTTServer::OnRTTWArmorkitDispatchAns( char* buffer, int size )
{
	MSG_RTTW_ARMORKIT_DISPATCH_ANS*	recvPacket = ( MSG_RTTW_ARMORKIT_DISPATCH_ANS* )buffer;

	OutputDebug( "[RTTServer::OnRTTWArmorkitDispatchAns] err=%d\n", recvPacket->dwResult );
}

void RTTServer::OnRTTWChangeGuildNameAns( char* buffer, int size )
{
	MSG_RTTW_CHANGE_GUILDNAME_ANS*	recvPacket = ( MSG_RTTW_CHANGE_GUILDNAME_ANS* )buffer;
	OutputDebug( "[RTTServer::OnRTTWChangeGuildNameAns] Result = %d\n", recvPacket->dwResult );

	::_wsetlocale( LC_ALL, L"KOREAN");
	FILE* fp = _wfopen(L"GuildMessageLog.txt", L"a+");
	::fwprintf(fp, L"[Result] %d\n", recvPacket->dwResult);
	::fwprintf(fp, L"[ Desc ] %s\n", recvPacket->wszResultDescription);
	fclose(fp);
}

void RTTServer::OnRTTWSendMessageAns( char* buffer, int size )
{
	MSG_RTTW_SEND_MESSAGE_ANS*	recvPacket = ( MSG_RTTW_SEND_MESSAGE_ANS* )buffer;
	OutputDebug( "[RTTServer::OnRTTWSendMessageAns] Recv Count = %d\n", recvPacket->dwCount );

	::_wsetlocale( LC_ALL, L"KOREAN");
	FILE* fp = _wfopen(L"SendMessageLog.txt", L"a+");

	::fwprintf(fp, L"[Count ] %d\n", recvPacket->dwCount);

	for(DWORD dw = 0 ; dw < recvPacket->dwCount ; ++dw)
	{
		::fwprintf(fp, L"[Result] %d\n", recvPacket->arrResultInfoList[dw].dwResult);
		::fwprintf(fp, L"[ Char ] %s\n", recvPacket->arrResultInfoList[dw].wszRecvCharName);
		::fwprintf(fp, L"[ Desc ] %s\n", recvPacket->arrResultInfoList[dw].wszResultDescription);
	}
	fclose(fp);
}

void RTTServer::OnRTTH_DROPUSER_Ans( char* buffer, int size )
{
	MSG_RTTH_DROPUSER_ANS*	recvPacket = ( MSG_RTTH_DROPUSER_ANS* )buffer;

	OutputDebug( "[RTTServer::OnRTTH_DROPUSER_Ans] err=%d\n", recvPacket->dwError );
}