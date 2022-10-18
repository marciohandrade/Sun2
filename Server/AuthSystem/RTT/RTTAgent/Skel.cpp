#include "stdafx.h"
#include "Skel.h"
#include "ManagerSkel.h"
#include "DropStatePoll.h"
#include "TestCounter.h"
#include "ProcessMon.h"
#include "RTTServer.h"


Skel::Skel()
: eXtreme::ServerSession( 40960, 40960 ), m_tickPacket(0)
{
}

Skel::~Skel()
{
}

void Skel::OnAccept()
{
	ManagerSkel::Instance()->AddClient( this );

	m_type				= 0;
	m_tickPacket		= 0;
	m_tickCheckStatus	= 0;

	//  접속과 동시에 RTTG_PATCHRESULT_REQ를 보내야한다.
	SendRTTGPatchResultReq();

	OutputDebug( "Accept connection from %s\n", GetIPAddress());
}

void Skel::OnClose( DWORD error )
{
	MSG_RTTW_DISCERTIFY_NTF	sendPacket;

	sendPacket.dwType	= m_type;
	sendPacket.dwUID	= 0;
	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );

	ManagerSkel::Instance()->RemoveClient( this );

	OutputDebug( "Closed connection from %s %d\n", GetIPAddress() , error);
}

void Skel::SendMessage( MSG_HEADER4& packet, int size )
{
	if( size == 0 )
		ServerSession::SendPacket( &packet, packet.wSize + SIZEOFHDR );
	else
		ServerSession::SendPacket( &packet, size );

	Flush();
}

void Skel::OnSend( int /*size*/ )
{
#ifdef  TESTCOUNTER
	TestCounter::Instance()->Inc( COUNTER_SKEL_SENDPACKET, 1 );
	TestCounter::Instance()->Inc( COUNTER_SKEL_SENDTRANSBYTES, size );
#endif
}

void Skel::SendRTTGPatchResultReq()
{
	MSG_RTTG_PATCHRESULT_REQ	sendPacket;

	sendPacket.dwKey = ProcessMon::Instance()->GetPatchKey();
	SendMessage( sendPacket );
}

void Skel::OnReceive( char* buffer, int size )
{
	MSG_HEADER4*		header = ( MSG_HEADER4* )buffer;

	m_tickPacket = ::GetTickCount();

	switch( header->wID )
	{
	case  RTTG_CERTIFY_REQ :			OnRTTGCertifyReq( buffer, size );			break;
	case  RTTG_USERCOUNT_NTF:			OnRTTGUserCountNTF( buffer, size );			break;
	case  RTTG_DROPUSER_ANS:			OnRTTGDropUserAns( buffer, size );			break;
	case  RTTG_GAMEADMINLIST_REQ:		OnRTTGGameAdminListReq( buffer, size );		break;
	case  RTTG_BATTLEZONEINFO_ANS:		OnRTTGBattleZoneInfoAns( buffer, size );	break;
	case  RTTG_ARMORKIT_FINDHACK_NTF:	OnRTTGArmorKitFindHackNTF( buffer, size );	break;
	case  RTTG_ABUSING_NTF:				OnRTTGAbusingNTF( buffer, size );			break;
	case  RTTG_USERMONEY_ANS:			OnRTTGUserMoneyAns( buffer, size );			break;
	case  RTTG_SERVERSHUTDOWN_ANS:		OnRTTGServerShutdownAns( buffer, size );	break;
	case  RTTG_PATCHRESULT_ANS:			OnRTTGPatchResultAns( buffer, size );		break;
	
	case  RTTG_AUTHIPLIST_RELOAD_ANS:	OnRTTAuthIPListReloadAns( buffer, size );	break;
	case  RTTG_GAMEVERSION_UPDATE_ANS:	OnRTTGameVersionUpdateAns( buffer, size );	break;
	case  RTTG_GAMESTATISTICS_ANS:		OnRTTGameStatisticsAns( buffer, size );		break;
//	case  RTTG_PATTERN_RELOAD_ANS:		OnRTTPatternReloadAns( buffer, size );		break;

	case  RTTG_ARMORKIT_DISPATCH_ANS:	OnRTTGArmorkitDispatchAns( buffer, size );	break;
	case  RTTG_CHANGE_GUILDNAME_ANS:	OnRTTGChangeGuildNameAns( buffer, size);	break;
	case  RTTG_SEND_MESSAGE_ANS:		OnRTTGSendMessageAns( buffer, size);		break;
		
	default:
		{
			OutputDebug( "Invalid Message.(wID=0x%x, wSize=%d)\n", header->wID, header->wSize );
			break;
		}
	}

#ifdef  TESTCOUNTER
	TestCounter::Instance()->Inc( COUNTER_SKEL_RECVPACKET, 1 );
	TestCounter::Instance()->Inc( COUNTER_SKEL_RECVTRANSBYTES, size );
#endif
}

void Skel::Update()
{
	if( ::GetTickCount() - m_tickCheckStatus > 30000 )
	{
		DWORD	sendBufMaxSize = 0, sendBufRemainSize = 0;
		DWORD	recvBufMaxSize = 0, recvBufRemainSize = 0;
		char	type[16] = {0, };

		GetSendBufferSize( &sendBufRemainSize, &sendBufMaxSize );
		GetRecvBufferSize( &recvBufRemainSize, &recvBufMaxSize );

		if( m_type == SERVERTYPE_AUTH )			strcpy( type, "AuthAgent" );
		else if( m_type == SERVERTYPE_MASTER )	strcpy( type, "Master Server" );
		else strcpy( type, "none" );

		m_tickCheckStatus = ::GetTickCount();
	}

	if( (m_type == SERVERTYPE_MASTER) && (0 != m_tickPacket) && (::GetTickCount() - m_tickPacket > 60000) )
	{
		OutputDebug( "Master Server로부터 60초 동안 응답이 없습니다. 연결을 종료합니다\n" );
		SetKill();
	}

	eXtreme::ServerSession::Update();
}

void Skel::OnRTTGCertifyReq( char* buffer, int /*size*/ )
{
	eXtreme::Stream			stream;
	MSG_RTTG_CERTIFY_REQ*	recvPacket = ( MSG_RTTG_CERTIFY_REQ* )buffer;
	MSG_RTTW_CERTIFY_REQ	sendPacket;

	sendPacket.dwType		= recvPacket->dwType;
	sendPacket.dwVersion	= 0;		//  No means...

	stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_CERTIFY_REQ << ( DWORD )GetUID();
	stream.Write( &sendPacket, sizeof( sendPacket ) );

	RTTServer::instance->SendPacket( stream.GetBuffer(), stream.GetDataSize() );

	OutputDebug( "[OnRTTGCertifyReq] %d, from %s\n", recvPacket->dwType, GetIPAddress() );
}

void Skel::OnRTTGUserCountNTF( char* buffer, int /*size*/ )
{
	MSG_RTTG_USERCOUNT_NTF*	recvPacket = (MSG_RTTG_USERCOUNT_NTF*)buffer;
	MSG_RTTW_USERCOUNT_NTF	sendPacket;

	sendPacket.dwWorldID			= recvPacket->dwWorldID;
	sendPacket.dwChannelID			= recvPacket->dwChannelID;

	sendPacket.dwFieldUserCount		= recvPacket->dwFieldUserCount;
	sendPacket.dwFieldPCRoomCount   = recvPacket->dwFieldPCRoomCount;
	
	sendPacket.dwBattleUserCount		= recvPacket->dwBattleUserCount;
	sendPacket.dwBattleZonePCRoomCount  = recvPacket->dwBattleZonePCRoomCount;

#ifdef	_090901_CHANNELLING_CONNECT_USER_
	sendPacket.dwHangameFieldUserCount			= recvPacket->dwHangameFieldUserCount;
	sendPacket.dwHangameFieldPCRoomCount		= recvPacket->dwHangameFieldPCRoomCount;
	sendPacket.dwHangameBattleUserCount			= recvPacket->dwHangameBattleUserCount;
	sendPacket.dwHangameBattleZonePCRoomCount	= recvPacket->dwHangameBattleZonePCRoomCount;
	sendPacket.dwPMangFieldUserCount			= recvPacket->dwPMangFieldUserCount;
	sendPacket.dwPMangFieldPCRoomCount			= recvPacket->dwPMangFieldPCRoomCount;
	sendPacket.dwPMangBattleUserCount			= recvPacket->dwPMangBattleUserCount;
	sendPacket.dwPMangBattleZonePCRoomCount		= recvPacket->dwPMangBattleZonePCRoomCount;
#endif
	
	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );


#ifdef	_090901_CHANNELLING_CONNECT_USER_
	OutputDebug( "[OnRTTGUserCountNTF] %d World %d Channel = Field(%04d/%04d, %04d/%04d, %04d/%04d), Battle(%04d/%04d, %04d/%04d, %04d/%04d)\n",
		recvPacket->dwWorldID,
		recvPacket->dwChannelID, 
		recvPacket->dwFieldUserCount, 
		recvPacket->dwFieldPCRoomCount,
		recvPacket->dwHangameFieldUserCount,
		recvPacket->dwHangameFieldPCRoomCount,
		recvPacket->dwPMangFieldUserCount,
		recvPacket->dwPMangFieldPCRoomCount,
		recvPacket->dwBattleUserCount,
		recvPacket->dwBattleZonePCRoomCount,
		recvPacket->dwHangameBattleUserCount,
		recvPacket->dwHangameBattleZonePCRoomCount,
		recvPacket->dwPMangBattleUserCount,
		recvPacket->dwPMangBattleZonePCRoomCount
		);
#else
	//*
	OutputDebug( "[OnRTTGUserCountNTF] %d World %d Channel = Field(%04d/%04d), Battle(%04d/%04d)\n",recvPacket->dwWorldID, recvPacket->dwChannelID, 
		recvPacket->dwFieldUserCount, 
		recvPacket->dwFieldPCRoomCount,
		recvPacket->dwBattleUserCount,
		recvPacket->dwBattleZonePCRoomCount );
	//*/

#endif

}

void Skel::OnRTTGDropUserAns( char* buffer, int /*size*/ )
{
	eXtreme::Stream			stream;
	MSG_RTTG_DROPUSER_ANS*	recvPacket = ( MSG_RTTG_DROPUSER_ANS* )buffer;

#ifdef	_090427_HTTP_WEB_DROP_USER		// 중국 웹 상에서 유저 강제 종료 요청 기능 사용

	// 패킷 선언하는 부분빼고는 나머지 소스는 똑같다...그래서..깔끔하게 공통된 부분을 어떻게 빼야할지 고민해봤는데 뾰족한 수가 없어서 걍 드럽게 놔둠..ㅡ,.ㅡ;
	if( eDRC_WEB == recvPacket->dwDropCode )		// 웹 요청이었을 경우
	{
		MSG_RTTH_DROPUSER_ANS	sendPacket;

		sendPacket.dwError = recvPacket->dwError;
		::MultiByteToWideChar( CP_ACP, 0, recvPacket->szUserID, -1, sendPacket.szUserID, ID_MAX_LEN );
		sendPacket.szUserID[ID_MAX_LEN] = NULL;

		DWORD key = DropStatePoll::Instance()->Remove( sendPacket.szUserID );

		OutputDebug("[RTTServer::OnRTTGDropUserAns] Web Session UID : %d\n", key );

		stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTH_DROPUSER_ANS << key;
		stream.Write( &sendPacket, sizeof( sendPacket ) );
	}
	else		// WOPS 요청이었을 경우
	{
		MSG_RTTW_DROPUSER_ANS	sendPacket;

		sendPacket.dwError = recvPacket->dwError;
		::MultiByteToWideChar( CP_ACP, 0, recvPacket->szUserID, -1, sendPacket.szUserID, ID_MAX_LEN );
		sendPacket.szUserID[ID_MAX_LEN] = NULL;

		DWORD key = DropStatePoll::Instance()->Remove( sendPacket.szUserID );

		OutputDebug("[RTTServer::OnRTTGDropUserAns] Wops Session UID : %d\n", key );

		stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_DROPUSER_ANS << key;
		stream.Write( &sendPacket, sizeof( sendPacket ) );
	}

#else

    MSG_RTTW_DROPUSER_ANS	sendPacket;

	sendPacket.dwError = recvPacket->dwError;
	::MultiByteToWideChar( CP_ACP, 0, recvPacket->szUserID, -1, sendPacket.szUserID, ID_MAX_LEN );
	sendPacket.szUserID[ID_MAX_LEN] = NULL;

	DWORD key = DropStatePoll::Instance()->Remove( sendPacket.szUserID );

	OutputDebug("[RTTServer::OnRTTGDropUserAns] Wops Session UID : %d\n", key );

	stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_DROPUSER_ANS << key;
	stream.Write( &sendPacket, sizeof( sendPacket ) );

#endif
	
	RTTServer::instance->SendPacket( stream.GetBuffer(), stream.GetDataSize() );

	OutputDebug( "[OnRTTGDropUserAns] %s(%d)\n", recvPacket->szUserID, recvPacket->dwError );
}

void Skel::OnRTTGGameAdminListReq( char* /*buffer*/, int /*size*/ )
{
	eXtreme::Stream				stream;
	//MSG_RTTG_GAMEADMINLIST_REQ*	recvPacket = ( MSG_RTTG_GAMEADMINLIST_REQ* )buffer;
	MSG_RTTW_GAMEADMINLIST_REQ	sendPacket;

	stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_GAMEADMINLIST_REQ << ( DWORD )GetUID();
	stream.Write( &sendPacket, sizeof( sendPacket ) );

	RTTServer::instance->SendPacket( stream.GetBuffer(), stream.GetDataSize() );
}

void Skel::OnRTTGBattleZoneInfoAns( char* buffer, int /*size*/ )
{
	MSG_RTTG_BATTLEZONEINFO_ANS*	recvPacket = ( MSG_RTTG_BATTLEZONEINFO_ANS* )buffer;
	MSG_RTTW_BATTLEZONEINFO_ANS		sendPacket;
	BATTLEZONE_INFO*				info = NULL;

	int offset = sizeof( MSG_HEADER4 ) + sizeof( BYTE );
	for(int i = 0 ; i < recvPacket->byCount ; i++ )
	{
		info	= ( BATTLEZONE_INFO* )( buffer + offset );
		
		//OutputDebug( "[SendRTTGBattleZoneInfoAns] %d, %d, %d\n", info->dwZoneCount[eZONETYPE_HUNTING], info->dwZoneCount[eZONETYPE_MISSION], info->dwZoneCount[eZONETYPE_PVP]);
		memcpy( &sendPacket.info, info, sizeof( BATTLEZONE_INFO ) );
		RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );
		

		offset	+= sizeof( BATTLEZONE_INFO );
	}
}

void Skel::OnRTTGArmorKitFindHackNTF( char* buffer, int /*size*/ )
{
	MSG_RTTG_ARMORKIT_FINDHACK_NTF*	recvPacket = ( MSG_RTTG_ARMORKIT_FINDHACK_NTF* )buffer;
	MSG_RTTW_ARMORKIT_FINDHACK_NTF	sendPacket;

	sendPacket.dwGuid		= recvPacket->dwGuid;
	sendPacket.dwHackType	= recvPacket->dwHackType;
	sendPacket.dwBufSize	= recvPacket->dwBufSize;
	::MultiByteToWideChar( CP_ACP, 0, recvPacket->szIP, -1, sendPacket.szIP, 16 );
	::memcpy( sendPacket.szBuf, recvPacket->szBuf, 1024 );

	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );
}

void Skel::OnRTTGAbusingNTF( char* buffer, int /*size*/ )
{
	MSG_RTTG_ABUSING_NTF*	recvPacket = ( MSG_RTTG_ABUSING_NTF* )buffer;
	MSG_RTTW_ABUSING_NTF	sendPacket;

	if( NULL != recvPacket->m_ptszUserID[0] )
	{		
#ifndef _110503_CUSTOM_USERID_LENGTH
		strncpy( sendPacket.m_ptszUserID , recvPacket->m_ptszUserID, MSG_RTTG_ABUSING_NTF::MAX_ID_LEN );
		sendPacket.m_ptszUserID[MSG_RTTW_ABUSING_NTF::MAX_ID_LEN] = '\0';
#else
		strncpy( sendPacket.m_ptszUserID , recvPacket->m_ptszUserID, ID_MAX_LEN );
		sendPacket.m_ptszUserID[ID_MAX_LEN] = '\0';
#endif
	}

	if( NULL != recvPacket->m_ptszCharName[0] )
	{
		strncpy( sendPacket.m_ptszCharName , recvPacket->m_ptszCharName, MSG_RTTG_ABUSING_NTF::MAX_CHARNAME_LENGTH );
		sendPacket.m_ptszCharName[MSG_RTTW_ABUSING_NTF::MAX_CHARNAME_LENGTH] = '\0';
	}

	if( NULL != recvPacket->m_ptszIP[0] )
	{
		strncpy( sendPacket.m_ptszIP, recvPacket->m_ptszIP, MSG_RTTG_ABUSING_NTF::MAX_IP_LENGTH );
		sendPacket.m_ptszIP[MSG_RTTW_ABUSING_NTF::MAX_IP_LENGTH] = '\0';
	}

	if( NULL != recvPacket->m_ptszSubTypeName[0] )
	{
		// 글고보니, 여기도 헤더는 TCHAR로 써놓고 코딩은 CHAR로 되어있네 -_-   <- 님 좀 짱인듯...ㅡ,.ㅡㅋ

		strncpy( sendPacket.m_ptszSubTypeName, recvPacket->m_ptszSubTypeName, MSG_RTTG_ABUSING_NTF::MAX_SUBTYPE_NAME_LEN);
		sendPacket.m_ptszSubTypeName[MSG_RTTW_ABUSING_NTF::MAX_SUBTYPE_NAME_LEN] = '\0';
	}

	if( recvPacket->m_wLength > MSG_RTTW_ABUSING_NTF::MAX_DESC_LENGTH )
	{
		strncpy( sendPacket.m_ptszDesc, recvPacket->m_ptszDesc, MSG_RTTW_ABUSING_NTF::MAX_DESC_LENGTH );
		sendPacket.m_ptszDesc[MSG_RTTW_ABUSING_NTF::MAX_DESC_LENGTH] = '\0';
//		OutputDebug( "[Skel::OnRTTGAbusingNTF] %s\n", sendPacket.m_ptszDesc);
	}
	else
	{
		strncpy( sendPacket.m_ptszDesc, recvPacket->m_ptszDesc, recvPacket->m_wLength );
		sendPacket.m_ptszDesc[recvPacket->m_wLength] = '\0';
//		OutputDebug( "[Skel::OnRTTGAbusingNTF] %s\n", sendPacket.m_ptszDesc);
	}

	memcpy( &sendPacket.m_ServerKey, &recvPacket->m_ServerKey, sizeof( sendPacket.m_ServerKey ) );


	sendPacket.m_dwTime			= recvPacket->m_dwTime		;
	sendPacket.m_dwUserGuid		= recvPacket->m_dwUserGuid	;
	sendPacket.m_dwCharGuid		= recvPacket->m_dwCharGuid	;
	sendPacket.m_AbuseType		= recvPacket->m_AbuseType ;
	sendPacket.m_Level			= recvPacket->m_Level ;

	sendPacket.m_wLength		= recvPacket->m_wLength		;
	
	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );


	//OutputDebug( "[Skel::OnRTTGAbusingNTF] !!! RecvPacket !!! ServerKey(%d,%d,%d,%d), UserGuid : %d, CharGuid : %d, UserID : %s, CharName : %s, IP : %s, AbuseType : %d, \
	//			 Level : %d, SubTypeName : %s, Desc : %s\n",
	//			recvPacket->m_ServerKey.GetWorldID(),		
	//			recvPacket->m_ServerKey.GetChannelID(),
	//			recvPacket->m_ServerKey.GetServerType(),	
	//			recvPacket->m_ServerKey.GetServerID(),
	//			recvPacket->m_dwUserGuid,
	//			recvPacket->m_dwCharGuid,
	//			recvPacket->m_ptszUserID,
	//			recvPacket->m_ptszCharName,
	//			recvPacket->m_ptszIP,
	//			recvPacket->m_AbuseType,
	//			recvPacket->m_Level,
	//			recvPacket->m_ptszSubTypeName,
	//			recvPacket->m_ptszDesc);

	//OutputDebug( "[Skel::OnRTTGAbusingNTF] !!! SendPacket !!!  ServerKey(%d,%d,%d,%d), UserGuid : %d, CharGuid : %d, UserID : %s, CharName : %s, IP : %s, AbuseType : %d, \
	//			 Level : %d, SubTypeName : %s, Desc : %s\n",
	//			sendPacket.m_ServerKey.GetWorldID(),		
	//			sendPacket.m_ServerKey.GetChannelID(),
	//			sendPacket.m_ServerKey.GetServerType(),	
	//			sendPacket.m_ServerKey.GetServerID(),
	//			sendPacket.m_dwUserGuid,
	//			sendPacket.m_dwCharGuid,
	//			sendPacket.m_ptszUserID,
	//			sendPacket.m_ptszCharName,
	//			sendPacket.m_ptszIP,
	//			sendPacket.m_AbuseType,
	//			sendPacket.m_Level,
	//			sendPacket.m_ptszSubTypeName,
	//			sendPacket.m_ptszDesc);
}


void Skel::OnRTTGUserMoneyAns( char* buffer, int /*size*/ )
{
	MSG_RTTG_USERMONEY_ANS*	recvPacket = ( MSG_RTTG_USERMONEY_ANS* ) buffer;
	MSG_RTTW_USERMONEY_ANS	sendPacket;

	//sendPacket.byServerNum	= recvPacket->byServerNum;
	//memcpy( sendPacket.sServerMoney, recvPacket->sServerMoney, sizeof( recvPacket->sServerMoney ) );
	sendPacket.byWorldID	= recvPacket->byWorldID;
	sendPacket.byChannelID	= recvPacket->byChannelID;
	sendPacket.byServerType = recvPacket->byServerType;
	sendPacket.byServerID   = recvPacket->byServerID;

	// 개인 보유액
	sendPacket.ui64TotalGetMoney	= recvPacket->ui64TotalGetMoney;
	sendPacket.ui64TotalLoseMoney	= recvPacket->ui64TotalLoseMoney;
	sendPacket.ui64TotalUserMoney	= recvPacket->ui64TotalUserMoney;
	
	// 창고 보유액 관련 추가
	sendPacket.ui64AveMoney			= recvPacket->ui64AveMoney;
	sendPacket.ui64TotalPlusMoney	= recvPacket->ui64TotalPlusMoney;
	sendPacket.ui64TotalMinusMoney	= recvPacket->ui64TotalMinusMoney;

	// jackpot 금액
	sendPacket.usJackpotCnt         = recvPacket->usJackpotCnt;
	sendPacket.ui64JackpotMoney     = recvPacket->ui64JackpotMoney;

	// 상거래 금액 합계
	sendPacket.ui64CirculateMoney   = recvPacket->ui64CirculateMoney;

	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );
/*
	OutputDebug( "[Skel::OnRTTGUserMoneyAns](%d, %d, %d, %d) TotalGetMoney=%I64d TotalLoseMoney=%I64d TotalUserMoney=%I64d AveMoney= %I64d TotalPlusMoney=%I64d TotalMinusMoney=%I64d JackpotCnt=%d JackpotMoney=%I64d\n", 
		recvPacket->byWorldID,
		recvPacket->byChannelID,
		recvPacket->byServerType,
		recvPacket->byServerID,		
		recvPacket->ui64TotalGetMoney,
		recvPacket->ui64TotalLoseMoney,
		recvPacket->ui64TotalUserMoney,
		recvPacket->ui64AveMoney,
		recvPacket->ui64TotalPlusMoney,
		recvPacket->ui64TotalMinusMoney,
		recvPacket->usJackpotCnt,
		recvPacket->ui64JackpotMoney);
//*/
}

void Skel::OnRTTGServerShutdownAns( char* buffer, int /*size*/ )
{
	MSG_RTTG_SERVERSHUTDOWN_ANS*	recvPacket = ( MSG_RTTG_SERVERSHUTDOWN_ANS* )buffer;
	MSG_RTTW_SERVERSHUTDOWN_ANS		sendPacket;

	sendPacket.dwKey				= recvPacket->dwKey;
	sendPacket.dwError				= recvPacket->dwError;
	sendPacket.m_ServerKey.dwKey	= recvPacket->m_ServerKey.dwKey;
	sendPacket.i64WopsKey           = recvPacket->i64WopsKey;

	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );
}

void Skel::OnRTTGPatchResultAns( char* buffer, int /*size*/ )
{
	MSG_RTTG_PATCHRESULT_ANS*		recvPacket = ( MSG_RTTG_PATCHRESULT_ANS* )buffer;
	MSG_RTTW_PATCHRESULT_ANS		sendPacket;

	sendPacket.dwKey		= ProcessMon::Instance()->GetPatchKey();
	sendPacket.dwError		= recvPacket->dwError;
	sendPacket.byIndex		= recvPacket->byIndex;
	sendPacket.byServerType = recvPacket->byServerType;
	sendPacket.byVer1		= recvPacket->byVer1;
	sendPacket.byVer2		= recvPacket->byVer2;
	sendPacket.byVer3		= recvPacket->byVer3;

	OutputDebug( "[OnRTTGPatchResultAns] index=%d, key=%d, err=%d, servertype=%d, ver=%d.%d.%d\n", recvPacket->byIndex, recvPacket->dwKey, recvPacket->dwError, recvPacket->byServerType, recvPacket->byVer1, recvPacket->byVer2, recvPacket->byVer3 );
	
	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );
}

void Skel::OnRTTAuthIPListReloadAns(char* buffer, int /*size*/)
{
	MSG_RTTG_AUTHIPLIST_RELOAD_ANS*		recvPacket = ( MSG_RTTG_AUTHIPLIST_RELOAD_ANS* )buffer;
	MSG_RTTW_AUTHIPLIST_RELOAD_ANS		sendPacket;

	sendPacket.dwResult		= recvPacket->dwResult;
	sendPacket.dwWopsKey	= recvPacket->dwWopsKey;
	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );

	OutputDebug("[Skel::OnRTTAuthIPListReloadAns] RESULT(%d) \n", sendPacket.dwResult);
}

void Skel::OnRTTGameVersionUpdateAns(char* buffer, int /*size*/)
{
	MSG_RTTG_GAMEVERSION_UPDATE_ANS*	recvPacket = ( MSG_RTTG_GAMEVERSION_UPDATE_ANS* )buffer;
	MSG_RTTW_GAMEVERSION_UPDATE_ANS		sendPacket;

	sendPacket.dwResult		= recvPacket->dwResult;
	sendPacket.dwWopsKey	= recvPacket->dwWopsKey;
	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );

	OutputDebug("[Skel::OnRTTGameVersionUpdateAns] RESULT(%d) \n", sendPacket.dwResult);
}

void Skel::OnRTTGameStatisticsAns(char* buffer, int /*size*/)
{
	MSG_RTTG_GAMESTATISTICS_ANS*	recvPacket = ( MSG_RTTG_GAMESTATISTICS_ANS* )buffer;
	MSG_RTTW_GAMESTATISTICS_ANS		sendPacket;
	
	//memcpy(sendPacket.ui64Statitics, recvPacket->ui64Statitics, recvPacket->wSize);
	sendPacket.dwWorldID	= recvPacket->byWorldID;
	sendPacket.dwChannelID	= recvPacket->byChannelID;
	sendPacket.dwServerType = recvPacket->byServerType;
	sendPacket.dwServerID   = recvPacket->byServerID;

	for(int i=0;i<eSTATISTIC_MAX + 1;i++){
		sendPacket.ui64Statistics[i] = recvPacket->ui64Statistics[i];
	}

	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );
}

//void Skel::OnRTTPatternReloadAns(char* buffer, int /*size*/)
//{
//	MSG_RTTG_PATTERN_RELOAD_ANS*	recvPacket = ( MSG_RTTG_PATTERN_RELOAD_ANS* )buffer;
//	MSG_RTTW_PATTERN_RELOAD_ANS		sendPacket;
//
//	sendPacket.dwResult		= recvPacket->dwResult;
//	sendPacket.dwWopsKey	= recvPacket->dwWopsKey;
//	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );
//
//	OutputDebug("[Skel::OnRTTPatternReloadAns] RESULT(%d) \n", sendPacket.dwResult);
//}

void Skel::OnRTTGArmorkitDispatchAns(char* buffer, int /*size*/)
{
	MSG_RTTG_ARMORKIT_DISPATCH_ANS*	recvPacket = ( MSG_RTTG_ARMORKIT_DISPATCH_ANS* )buffer;
	MSG_RTTW_ARMORKIT_DISPATCH_ANS	sendPacket;

	sendPacket.dwResult		= recvPacket->dwResult;
	sendPacket.dwWopsKey	= recvPacket->dwWopsKey;
	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );

	OutputDebug("[Skel::OnRTTGArmorkitDispatchAns] RESULT(%d) \n", sendPacket.dwResult);
}

void Skel::OnRTTGChangeGuildNameAns(char* buffer, int size)
{
    MSG_RTTG_CHANGE_GUILDNAME_ANS*	recvPacket = (MSG_RTTG_CHANGE_GUILDNAME_ANS*)buffer;
	MSG_RTTW_CHANGE_GUILDNAME_ANS	sendPacket;

	eXtreme::Stream					stream( 1024 );

	sendPacket.dwResult = recvPacket->dwResult;

	::MultiByteToWideChar( CP_ACP, 0, recvPacket->szResultDescription, MSG_RTTG_CHANGE_GUILDNAME_ANS::MAX_RESULT_DESCRIPTION_LENGTH, sendPacket.wszResultDescription, MSG_RTTW_CHANGE_GUILDNAME_ANS::MAX_RESULT_DESCRIPTION_LENGTH );

	sendPacket.wszResultDescription[ MSG_RTTW_CHANGE_GUILDNAME_ANS::MAX_RESULT_DESCRIPTION_LENGTH ] = NULL;

	stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_CHANGE_GUILDNAME_ANS << ( DWORD )recvPacket->dwWopsKey;
	stream.Write( &sendPacket, sizeof( sendPacket ) );

	RTTServer::instance->SendPacket( stream.GetBuffer(), stream.GetDataSize() );

	OutputDebug("[Skel::OnRTTGChangeGuildNameAns] RESULT(%d) : %s\n", sendPacket.dwResult, recvPacket->szResultDescription);
}

void Skel::OnRTTGSendMessageAns(char* buffer, int size)
{
	MSG_RTTG_SEND_MESSAGE_ANS*	recvPacket = (MSG_RTTG_SEND_MESSAGE_ANS*)buffer;
	MSG_RTTW_SEND_MESSAGE_ANS	sendPacket;

	eXtreme::Stream					stream( 5120 );

	sendPacket.dwCount = recvPacket->dwCount;

	for(DWORD dwIndex = 0 ; (dwIndex < sendPacket.dwCount) && (dwIndex < MSG_RTTW_SEND_MESSAGE_ANS::MAX_RESULT_INFO) ; ++dwIndex)
	{
		sendPacket.arrResultInfoList[dwIndex].dwResult = recvPacket->arrResultInfoList[dwIndex].dwResult;
		::MultiByteToWideChar( CP_ACP, 0, recvPacket->arrResultInfoList[dwIndex].szRecvCharName, MSG_RTTG_SEND_MESSAGE_ANS::MAX_CHAR_NAME_LENGTH, sendPacket.arrResultInfoList[dwIndex].wszRecvCharName, MSG_RTTW_SEND_MESSAGE_ANS::MAX_CHAR_NAME_LENGTH );
		::MultiByteToWideChar( CP_ACP, 0, recvPacket->arrResultInfoList[dwIndex].szResultDescription, MSG_RTTG_SEND_MESSAGE_ANS::MAX_RESULT_DESCRIPTION_LENGTH, sendPacket.arrResultInfoList[dwIndex].wszResultDescription, MSG_RTTW_SEND_MESSAGE_ANS::MAX_RESULT_DESCRIPTION_LENGTH );

		sendPacket.arrResultInfoList[dwIndex].wszRecvCharName[ MSG_RTTW_SEND_MESSAGE_ANS::MAX_CHAR_NAME_LENGTH ] = NULL;
		sendPacket.arrResultInfoList[dwIndex].wszResultDescription[ MSG_RTTW_SEND_MESSAGE_ANS::MAX_RESULT_DESCRIPTION_LENGTH ] = NULL;
	}

	stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_SEND_MESSAGE_ANS << ( DWORD )recvPacket->dwWopsKey;
	stream.Write( &sendPacket, sizeof( sendPacket ) );

	RTTServer::instance->SendPacket( stream.GetBuffer(), stream.GetDataSize() );

//*/////////////// 로그 
	OutputDebug("[Skel::OnRTTGSendMessageAns] Send RESULT To RTTServer(Count : %d)\n", recvPacket->dwCount);

	for(DWORD dwIndex = 0 ; (dwIndex < recvPacket->dwCount) && (dwIndex < MSG_RTTG_SEND_MESSAGE_ANS::MAX_RESULT_INFO) ; ++dwIndex)
	{
		OutputDebug( "[Skel::OnRTTGSendMessageAns]		RecvChar	: %s(Result:%d)\n", recvPacket->arrResultInfoList[dwIndex].szRecvCharName, recvPacket->arrResultInfoList[dwIndex].dwResult);
        OutputDebug( "[Skel::OnRTTGSendMessageAns]		Description	: %s\n", recvPacket->arrResultInfoList[dwIndex].szResultDescription);
	}
/////////////////*/
}
