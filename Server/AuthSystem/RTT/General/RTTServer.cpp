#include "stdafx.h"
#include "Entry.h"
#include "RTTServer.h"
#include "EVENT_TYPE_SPEC.h"
#include "HDHelper.h"


#define  LIMITE_CONNECTTICK		3000

//  Connection state
#define  CS_CLOSED				0x0001
#define  CS_TRYCONNECT			0x0002
#define  CS_CONNECTED			0x0003


RTTServer::RTTServer()
	: eXtreme::ServerSession( 20480, 20480 )
{
	m_AliveTick = 0;
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
		GetHandle()->SetBufferSize( 65535, 65535 );
		m_connectState = CS_CONNECTED;
		m_AliveTick = ::GetTickCount();
		SendRTTGCertifyReq();

		OutputDebug( "Connected to RTTAgent.\n" );
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

	OutputDebug( "Closed from RTTAgent.\n" );
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

		Connect( "127.0.0.1", 20510 );
	}
}

void RTTServer::SendHeartBeat()
{
	MSG_RTTR_HEARTBEAT_NTF	sendPacket;
	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::Update()
{
	CheckDisconnected();
	if( m_connectState != CS_CONNECTED )
		return;

	Sleep( 5000 );

	if( (::GetTickCount() - m_AliveTick) >  30000 )
	{
        SendHeartBeat();
		m_AliveTick = ::GetTickCount();
	}

//	SendRTTGGameAdminListReq();
//	SendRTTGUserCount();
//	SendRTTGBattleZoneInfoAns();
//	SendRTTGArmorKitFindHackNTF();
//	SendRTTGAbusingNTF();

//	SendTempRTTGArmorKitFindHackNTF();

	eXtreme::ServerSession::Update(  );
}

void RTTServer::SendRTTGCertifyReq()
{
	MSG_RTTG_CERTIFY_REQ	sendPacket;

	//sendPacket.dwType	= SERVERTYPE_MASTER;
	sendPacket.dwType	= SERVERTYPE_AUTH;
	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::SendRTTGGameAdminListReq()
{
	MSG_RTTG_GAMEADMINLIST_REQ	sendPacket;

	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::SendRTTGDropUserAns()
{
	MSG_RTTG_DROPUSER_ANS	sendPacket;

	sendPacket.dwError = rand() % 2;
	strcpy( sendPacket.szUserID, "아하하" );

	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::SendRTTGUserCount()
{
	MSG_RTTG_USERCOUNT_NTF sendPacket;
	for(int i=0;i<1;i++){
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		sendPacket.dwWorldID = 1;
		sendPacket.dwChannelID = 1;
		/*
		sendPacket.dwFieldUserCount			= 1000 + rand()%300;
		sendPacket.dwFieldPCRoomCount		= 1000 + rand()%300;		
		sendPacket.dwBattleUserCount		= 1000 + rand()%300;
		sendPacket.dwBattleZonePCRoomCount	= 1000 + rand()%300;
		*/
		sendPacket.dwFieldUserCount			= 100;
		sendPacket.dwFieldPCRoomCount		= 110;
		sendPacket.dwBattleUserCount		= 120;
		sendPacket.dwBattleZonePCRoomCount	= 130;

		SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
	}/*
	{
		sendPacket.dwWorldID = 1;
		sendPacket.dwChannelID = 2;
		
		sendPacket.dwFieldUserCount			= 1000 + rand()%300;
		sendPacket.dwFieldPCRoomCount		= 1000 + rand()%300;		
		sendPacket.dwBattleUserCount		= 1000 + rand()%300;
		sendPacket.dwBattleZonePCRoomCount	= 1000 + rand()%300;
		
		SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
	}
	{
		sendPacket.dwWorldID = 1;
		sendPacket.dwChannelID = 3;
		
		sendPacket.dwFieldUserCount			= 1000 + rand()%300;
		sendPacket.dwFieldPCRoomCount		= 1000 + rand()%300;		
		sendPacket.dwBattleUserCount		= 1000 + rand()%300;
		sendPacket.dwBattleZonePCRoomCount	= 1000 + rand()%300;
		
		SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
	}
	{
		sendPacket.dwWorldID = 1;
		sendPacket.dwChannelID = 4;
		
		sendPacket.dwFieldUserCount			= 1000 + rand()%300;
		sendPacket.dwFieldPCRoomCount		= 1000 + rand()%300;		
		sendPacket.dwBattleUserCount		= 1000 + rand()%300;
		sendPacket.dwBattleZonePCRoomCount	= 1000 + rand()%300;
		
		SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
	}
	{
		sendPacket.dwWorldID = 1;
		sendPacket.dwChannelID = 5;
		
		sendPacket.dwFieldUserCount			= 1000 + rand()%300;
		sendPacket.dwFieldPCRoomCount		= 1000 + rand()%300;		
		sendPacket.dwBattleUserCount		= 1000 + rand()%300;
		sendPacket.dwBattleZonePCRoomCount	= 1000 + rand()%300;

		SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
	}
	{
		sendPacket.dwWorldID = 1;
		sendPacket.dwChannelID = 6;
		
		sendPacket.dwFieldUserCount			= 1000 + rand()%300;
		sendPacket.dwFieldPCRoomCount		= 1000 + rand()%300;		
		sendPacket.dwBattleUserCount		= 1000 + rand()%300;
		sendPacket.dwBattleZonePCRoomCount	= 1000 + rand()%300;
		
		SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
	}
	{
		sendPacket.dwWorldID = 2;
		sendPacket.dwChannelID = 1;
		
		sendPacket.dwFieldUserCount			= 1000 + rand()%300;
		sendPacket.dwFieldPCRoomCount		= 1000 + rand()%300;		
		sendPacket.dwBattleUserCount		= 1000 + rand()%300;
		sendPacket.dwBattleZonePCRoomCount	= 1000 + rand()%300;
		
		SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
	}
	
	{
		sendPacket.dwWorldID = 2;
		sendPacket.dwChannelID = 2;
		
		sendPacket.dwFieldUserCount			= 1000 + rand()%300;
		sendPacket.dwFieldPCRoomCount		= 1000 + rand()%300;		
		sendPacket.dwBattleUserCount		= 1000 + rand()%300;
		sendPacket.dwBattleZonePCRoomCount	= 1000 + rand()%300;
		
		SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
	}*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}

void RTTServer::SendRTTGBattleZoneInfoAns()
{
	//MSG_RTTG_BATTLEZONEINFO_ANS		sendPacket;
	//
	//sendPacket.byCount	= 1;

	//sendPacket.BattlezonInfo[0].byWorldID = 1;
	//sendPacket.BattlezonInfo[0].byChannelID = 1;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_MISSION]		= 10;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_CHUNTING]		= 20;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_PVP]			= 30;

	//SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::SendRTTGArmorKitFindHackNTF()
{
	MSG_RTTG_ARMORKIT_FINDHACK_NTF	sendPacket;
	int a = rand() % 4;

	if( a == 0 )
	{
		SPEED_HACK_CHECK_INFO	info;

		sendPacket.dwHackType	= AKDETECT_SPEEDHACK;
		sendPacket.dwGuid		= 123;
		strcpy( sendPacket.szIP, "127.0.0.1" );

		info.dwGTC	= 0;
		info.dwQPC	= 1;
		info.dwTGT	= 2;
		info.dwTSC	= 3;

		memcpy( sendPacket.szBuf, &info, sizeof( info ) );

		a = 0;
	}
	if( a == 1 )
	{
		PROCESS_SCAN_INFO	info;

		sendPacket.dwHackType	= AKDETECT_MALWARE;
		sendPacket.dwGuid		= 123;
		strcpy( sendPacket.szIP, "127.0.0.1" );

		info.ProcessId	= 1;
		strcpy( info.ProcessName, "Haha.exe" );
		info.SianatureType	= 2;
		info.SignatureIndex = 3;
		strcpy( info.SignatureName, "Signiture" );

		memcpy( sendPacket.szBuf, &info, sizeof( info ) );

		a = 0;
	}
	if( a == 2 )
	{
		HOOKED_API_INFO	info;

		sendPacket.dwHackType	= AKDETECT_PEINTEGRITY;
		sendPacket.dwGuid		= 123;
		strcpy( sendPacket.szIP, "127.0.0.1" );

		info.API_TYPE = 1;
		strcpy( info.API_INFO.HEA.ApiName, "HEA.ApiName" );
		strcpy( info.API_INFO.HEA.DLLName, "HEA.DllName" );
		info.API_INFO.HEA.HookdAddr		= 1;
		info.API_INFO.HEA.HOOKED_CODE	= 2;
		info.API_INFO.HEA.OriginalAddr	= 3;


		strcpy( info.API_INFO.HIA.ApiName, "HIA.ApiName" );
		strcpy( info.API_INFO.HIA.DLLName, "HIA.DllName" );
		info.API_INFO.HIA.HookdAddr			= 0;
		info.API_INFO.HIA.DetourAddr		= 1;
		info.API_INFO.HIA.dwStart			= 2;
		info.API_INFO.HIA.dwEnd				= 3;
		info.API_INFO.HIA.HOOKED_CODE		= 4;

		memcpy( sendPacket.szBuf, &info, sizeof( info ) );

		a = 0;
	}
	if( a == 3 )
	{
		ANTI_DEBUG_CHECK_INFO	info;

		sendPacket.dwHackType	= AKDETECT_DEBUG;
		sendPacket.dwGuid		= 123;
		strcpy( sendPacket.szIP, "127.0.0.1" );

		info.bSpecificAPI			= 0;
		info.bProcessInfo			= 0;
		info.bParentProcessInfo		= 0;
		info.bActiveDevices			= 0;
		info.bSEH					= 0;

		info.bResSpecificAPI		= 1;
		info.bResProcessInfo		= 2;
		info.bResParentProcessInfo	= 3;
		info.bResActiveDevices		= 4;
		info.bResSEH				= 5;

		memcpy( sendPacket.szBuf, &info, sizeof( info ) );

		a = 0;
	}

	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::SendRTTGAbusingNTF()
{
//	MSG_RTTG_ABUSING_NTF	sendPacket;
//	int		r = rand() % 3;
//
//	sendPacket.m_byAbuseType = AKDETECT_SPEEDHACK;
//	sendPacket.m_dwCharGuid = 0x01;
//	sendPacket.m_dwUserGuid = 0x02;
//
//	sendPacket.m_ServerKey.Set( 1, 1, 1, 1 );
//	sendPacket.m_dwTime = 20060519181020;
//	sendPacket.m_wLength		= 8;
//	strcpy( sendPacket.m_ptszDesc, "뭘더바래" );
//
////*
//	if( r == 0 )
//	{
//		sendPacket.m_dwTime = 20051228151743;
//		strcpy( sendPacket.m_ptszUserID, "HOTDOG" );
//		strcpy( sendPacket.m_ptszCharName, "개농사실패" );
//		strcpy(&sendPacket.m_ptszIP[0], "127.0.0.1");
//		sendPacket.m_dwCharGuid = 0;
//		sendPacket.m_dwUserGuid = 0;
//	}
//	if( r == 1 )
//	{
//		sendPacket.m_dwTime = 20060119121412;
//		strcpy( sendPacket.m_ptszUserID, "ABCDEFG" );
//		strcpy( sendPacket.m_ptszCharName, "괴로우면농약" );
//		strcpy(&sendPacket.m_ptszIP[0], "127.127.127.127");
//		sendPacket.m_dwCharGuid = 0;
//		sendPacket.m_dwUserGuid = 0;
//	}
//	if( r == 2 )
//	{
//		sendPacket.m_dwTime = 20060228131614;
//		strcpy( sendPacket.m_ptszUserID, "sunsun" );
//		strcpy( sendPacket.m_ptszCharName, "필미스톤" );
//		strcpy(&sendPacket.m_ptszIP[0], "127.127.127.127");
//		sendPacket.m_dwCharGuid = 0;
//		sendPacket.m_dwUserGuid = 0;
//	}
////*/
//	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::SendTempRTTGArmorKitFindHackNTF()
{
    MSG_RTTG_ARMORKIT_FINDHACK_NTF	sendPacket;
	int stream_buf_size = -1;
	char *pstream_buf = NULL;		// 초기화 되지 않은 포인터 변수만 넘겨주삼~

	int a = rand() % 2;

	if(0 == a)
	{
		HOOKED_API_LIST hal;
		API_TO_RESTORE	atr;

		// 12 개의 벡터를 생성
		//
		for (int i = 0; i < 12; i++)
		{
			atr.HookType = HT_ENTRY_HOOK_INVALID_RANGE;
			_stprintf(atr.DLLName, "%s", "kernel32.dll");
			atr.Ordinal = i;
			_stprintf(atr.ApiName, "%s", "test_api_name");
			atr.OriginalAddr = 0x7c802234;
			atr.CurrentAddr = 0x7c802234;
			atr.DetourAddr = 0x10294234;
			_stprintf(atr.OriginalOpcodes, "%s", "test_api_original_opcode");
			_stprintf(atr.CurrentOpcodes, "%s", "test_api_current_opcodes");
			_stprintf(atr.DetourAddrOpcodes, "%s", "test_api_detour_opcodes");
			atr.ValidStartAddr = 0x7c808809;
			atr.ValidEndAddr = 0x7c908809;

			hal.push_back(atr);
		}

		int iRet = CHDHelper::hal_to_stream(hal, 1024, &stream_buf_size, pstream_buf);
		_ASSERTE(-1 != iRet);
		if (-1 == iRet)
		{
			OutputDebug("Oops! hal_to_stream() failed	\n", "");
			return;
		}

		sendPacket.dwHackType	= AKDETECT_HOOK_DETECT;

		memmove( &sendPacket.szBuf[0], pstream_buf, stream_buf_size );
	}

	if(1 == a)
	{
		HDFAULT_API_LIST hal;
		RESTORE_FAULT_API rfa;

		for (int i = 0; i < 12; i++)
		{
			rfa.apiInfo.HookType = HT_ENTRY_HOOK_USE_CAVE;
			_stprintf(rfa.apiInfo.DLLName, "%s", "kernel32.dll");
			rfa.apiInfo.Ordinal = i;
			_stprintf(rfa.apiInfo.ApiName, "%s", "test_api_name");
			rfa.apiInfo.OriginalAddr = 0x7c802234;
			rfa.apiInfo.CurrentAddr = 0x7c802234;
			rfa.apiInfo.DetourAddr = 0x10294234;
			_stprintf(rfa.apiInfo.OriginalOpcodes, "%s", "test_api_original_opcode");
			_stprintf(rfa.apiInfo.CurrentOpcodes, "%s", "test_api_current_opcodes");
			_stprintf(rfa.apiInfo.DetourAddrOpcodes, "%s", "test_api_detour_opcodes");
			rfa.apiInfo.ValidStartAddr = 0x7c808809;
			rfa.apiInfo.ValidEndAddr = 0x7c908809;
			rfa.ErrorCode = HDERR_GENERIC;

			hal.push_back(rfa);
		}

		int iRet = CHDHelper::hdfal_to_stream(hal, 1024, &stream_buf_size, pstream_buf);
		_ASSERTE(-1 != iRet);
		if (-1 == iRet)
		{
			OutputDebug("Oops! hdfal_to_stream() failed	\n", "");
			return;
		}

		sendPacket.dwHackType	= AKDETECT_HOOK_RESTORE_FAIL;

		memmove( &sendPacket.szBuf[0], pstream_buf, stream_buf_size );
	}

	sendPacket.dwGuid		= 123;
	strcpy( sendPacket.szIP, "127.0.0.1" );
	sendPacket.dwBufSize	= stream_buf_size;

	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );

	// stream 버퍼는 반드시 CHDHelper 를 통해서 소멸해 줘야함
	CHDHelper::free_converted_stream(pstream_buf);	
}

void RTTServer::OnReceive( char* buffer, int size )
{
	MSG_HEADER4*		header = ( MSG_HEADER4* )buffer;

	switch( header->wID )
	{
	case  RTTG_CERTIFY_ANS:				OnRTTGCertifyAns( buffer, size );			break;
	case  RTTG_GAMEADMINLIST_ANS:		OnRTTGGameAdminListAns( buffer, size );		break;
	case  RTTG_GAMENOTICE_REQ:			OnRTTGGameNoticeReq( buffer, size );		break;
	case  RTTG_GAMEADMINADD_NTF:		OnRTTGGameAdminAddNTF( buffer, size );		break;
	case  RTTG_GAMEADMINREMOVE_NTF:		OnRTTGGameAdminRemoveNTF( buffer, size );	break;
	case  RTTG_DROPUSER_REQ:			OnRTTGDropUserReq( buffer, size );			break;
	case  RTTG_BATTLEZONEINFO_REQ:		OnRTTGBattleZoneInfoReq( buffer, size );	break;
	case  RTTG_USERMONEY_REQ:			OnRTTGUserMoneyReq( buffer, size );			break;
	case  RTTG_SERVERSHUTDOWN_REQ:		OnRTTGServerShutdownReq( buffer, size );	break;
	case  RTTG_GAMESTATISTICS_REQ:		OnRTTGameStatisticsReq( buffer, size );		break;
	case  RTTG_ARMORKIT_DISPATCH_NTF:	OnRTTGArmorkitDispatchNTF( buffer, size );	break;
	case  RTTG_CHANGE_GUILDNAME_REQ:	OnRTTGChangeGuildNameReq( buffer, size );	break;
	case  RTTG_SEND_MESSAGE_REQ:		OnRTTGSendMessageReq( buffer, size );		break;

	default:
		OutputDebug( "[RTTServer::OnReceive] invalid id (%d) \n", header->wID);
	}
}

void RTTServer::OnRTTGCertifyAns( char* buffer, int size )
{
	MSG_RTTG_CERTIFY_ANS*	recvPacket = ( MSG_RTTG_CERTIFY_ANS* )buffer;

	OutputDebug( "인증되었습니다.\n" );
	SendRTTGGameAdminListReq();
}

void RTTServer::OnRTTGGameAdminListAns( char* buffer, int size )
{
	MSG_RTTG_GAMEADMINLIST_ANS*	recvPacket = ( MSG_RTTG_GAMEADMINLIST_ANS* )buffer;
	MSG_RTTG_GAMEADMINLIST_ANS	dummy;
	eXtreme::Stream				stream;
	char	id[ID_MAX_LEN], ip[IP_MAX_LEN];
	DWORD	attr;
	UINT	i;

	stream.Write( buffer, size );
	stream.Read( &dummy, sizeof( dummy ) );

	for( i = 0 ; i < recvPacket->dwCount ; i++ )
	{
		stream.Read( id, ID_MAX_LEN );
		stream.Read( ip, IP_MAX_LEN );
		stream.Read( &attr, sizeof( attr ) );
	}
}

void RTTServer::OnRTTGGameNoticeReq( char* buffer, int size )
{
	MSG_RTTG_GAMENOTICE_REQ*	recvPacket = ( MSG_RTTG_GAMENOTICE_REQ* )buffer;
	OutputDebug(recvPacket->szNotice);
}

void RTTServer::OnRTTGGameAdminAddNTF( char* buffer, int size )
{
	MSG_RTTG_GAMEADMINADD_NTF*	recvPacket = ( MSG_RTTG_GAMEADMINADD_NTF* )buffer;
}

void RTTServer::OnRTTGGameAdminRemoveNTF( char* buffer, int size )
{
	MSG_RTTG_GAMEADMINREMOVE_NTF*	recvPacket = ( MSG_RTTG_GAMEADMINREMOVE_NTF* )buffer;
}

void RTTServer::OnRTTGDropUserReq( char* buffer, int size )
{
	MSG_RTTG_DROPUSER_REQ*	recvPacket = ( MSG_RTTG_DROPUSER_REQ* )buffer;
	MSG_RTTG_DROPUSER_ANS	sendPacket;

	sendPacket.dwError = 0;
	strncpy(sendPacket.szUserID, recvPacket->szUserID, strlen(recvPacket->szUserID));

#ifdef  _090427_HTTP_WEB_DROP_USER				// 중국 웹 상에서 유저 강제 종료 기능
	sendPacket.dwDropCode = recvPacket->dwDropCode;
#endif
	
	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );

#ifdef  _090427_HTTP_WEB_DROP_USER				// 중국 웹 상에서 유저 강제 종료 기능
	OutputDebug( "[RTTServer::OnRTTGDropUserReq] %s, DropCode : %d \n", recvPacket->szUserID, recvPacket->dwDropCode );
#else
	OutputDebug( "[RTTServer::OnRTTGDropUserReq] %s \n", recvPacket->szUserID );
#endif

}

void RTTServer::OnRTTGBattleZoneInfoReq( char* buffer, int size )
{
	//MSG_RTTG_BATTLEZONEINFO_REQ*	recvPacket = ( MSG_RTTG_BATTLEZONEINFO_REQ* )buffer;
	//MSG_RTTG_BATTLEZONEINFO_ANS		sendPacket;

	//sendPacket.byCount	= 2;

	//sendPacket.BattlezonInfo[0].byWorldID = 1;
	//sendPacket.BattlezonInfo[0].byChannelID = 1;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_LOBBY]		= 10;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_VILLAGE]		= 20;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_CHARSELECT]	= 30;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_MISSION]		= 40;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_HUNTING]		= 50;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_QUEST]		= 60;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_PVP]			= 70;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_EVENT]		= 80;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_GUILD]		= 90;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_SIEGE]		= 100;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_FIELD]		= 110;
	//sendPacket.BattlezonInfo[0].dwZoneCount[eZONETYPE_CHUNTING] 	= 120;

	//sendPacket.BattlezonInfo[1].byWorldID = 2;
	//sendPacket.BattlezonInfo[1].byChannelID = 2;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_LOBBY]		= 10;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_VILLAGE]		= 20;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_CHARSELECT]	= 30;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_MISSION]		= 40;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_HUNTING]		= 50;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_QUEST]		= 60;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_PVP]			= 70;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_EVENT]		= 80;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_GUILD]		= 90;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_SIEGE]		= 100;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_FIELD]		= 110;
	//sendPacket.BattlezonInfo[1].dwZoneCount[eZONETYPE_CHUNTING] 	= 120;

	//
	//SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::OnRTTGUserMoneyReq( char* buffer, int size )
{
	MSG_RTTG_USERMONEY_REQ*	recvPacket = ( MSG_RTTG_USERMONEY_REQ* )buffer;
	MSG_RTTG_USERMONEY_ANS	sendPacket;

	sendPacket.byWorldID	= 1;
	sendPacket.byChannelID	= 1;
	sendPacket.byServerType	= 1;
	sendPacket.byServerID	= 1;

	if( rand() % 2 == 0 )	sendPacket.ui64TotalGetMoney	= 50000 + rand() % 10000;
	else					sendPacket.ui64TotalGetMoney	= 10000 - rand() % 10000;
	if( rand() % 2 == 0 )	sendPacket.ui64TotalLoseMoney	= 500 + rand() % 100000;
	else					sendPacket.ui64TotalLoseMoney	= 100000 - rand() % 100000;
	if( rand() % 2 == 0 )	sendPacket.ui64TotalUserMoney	= 10000 + rand() % 100000;
	else					sendPacket.ui64TotalUserMoney	= 100000 - rand() % 100000;
	
	sendPacket.usJackpotCnt = 1;
	sendPacket.ui64JackpotMoney = 500 + rand() % 100000;
	sendPacket.ui64AveMoney = 500 + rand() % 10000;
	sendPacket.ui64TotalMinusMoney = 500 + rand() % 10000;
	sendPacket.ui64TotalPlusMoney  = 500 + rand() % 10000;

	sendPacket.ui64CirculateMoney = 50000 + rand() % 10000;


	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::OnRTTGServerShutdownReq( char* buffer, int size )
{
	MSG_RTTG_SERVERSHUTDOWN_REQ*	recvPacket = ( MSG_RTTG_SERVERSHUTDOWN_REQ* )buffer;
	MSG_RTTG_SERVERSHUTDOWN_ANS		sendPacket;

	OutputDebug( "[OnRTTGServerShutdownReq] %d, %d, %d, %d, %d\n", 
					recvPacket->dwWorldID, 
					recvPacket->dwChannelID, 
					recvPacket->dwServerType, 
					recvPacket->dwServerID, 
					recvPacket->dwKey );

	sendPacket.dwKey	= recvPacket->dwKey;
	sendPacket.dwError	= 0;
	sendPacket.m_ServerKey.Set( (BYTE)recvPacket->dwWorldID, (BYTE)recvPacket->dwChannelID, (BYTE)recvPacket->dwServerType, (BYTE)recvPacket->dwServerID);
	sendPacket.i64WopsKey = recvPacket->i64WopsKey;

	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::OnRTTGameStatisticsReq( char* buffer, int size )
{
	//MSG_RTTG_GAMESTATISTICS_REQ*	recvPacket = ( MSG_RTTG_GAMESTATISTICS_REQ* )buffer;
	//MSG_RTTG_GAMESTATISTICS_ANS		sendPacket;
	//
	//sendPacket.byWorldID	= 1;
	//sendPacket.byChannelID	= 1;
	//sendPacket.byServerType	= 1;
	//sendPacket.byServerID	= 1;

	//
	//for(int i=0;i<eSTATISTIC_MAX + 1;i++){
	//	sendPacket.ui64Statitics[i] = 0;
	//}
	//

	//sendPacket.ui64Statitics[eSTATISTIC_FEELMISTONE_CREATE        ] =  1;
	//sendPacket.ui64Statitics[eSTATISTIC_FEELMISTONE_DESTROY       ] =  2;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_FEELMISTONE       ] =  3;
	//sendPacket.ui64Statitics[eSTATISTIC_OVERMISTONE_CREATE        ] =  4;
	//sendPacket.ui64Statitics[eSTATISTIC_OVERMISTONE_DESTROY       ] =  5;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_OVERMISTONE       ] =  6;
	//sendPacket.ui64Statitics[eSTATISTIC_STRENGTHMISTONE_CREATE    ] =  7;
	//sendPacket.ui64Statitics[eSTATISTIC_STRENGTHMISTONE_DESTROY   ] =  8;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_STRENGTHMISTONE   ] =  9;
	//sendPacket.ui64Statitics[eSTATISTIC_MEGAMISTONE_CREATE        ] = 10;
	//sendPacket.ui64Statitics[eSTATISTIC_MEGAMISTONE_DESTROY       ] = 11;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_MEGAMISTONE       ] = 12;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_EXTRASTONE        ] = 13;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_GIGAMISTONE       ] = 14;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_RING_LOW          ] = 15;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_RING_HIGH         ] = 16;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_NECKLACE_LOW      ] = 17;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_NECKLACE_HIGH     ] = 18;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_SOUL_SHEILD_LOW   ] = 19;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_SOUL_SHEILD_HIGH  ] = 20;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_SOUL_ARMLET_LOW   ] = 21;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_SOUL_ARMLET_HIGH  ] = 22;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_SOUL_WING_LOW     ] = 23;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_SOUL_WING_HIGH    ] = 24;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_SOUL_EYE_LOW      ] = 25;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_SOUL_EYE_HIGH     ] = 26;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_PROTECTION_CRYSTAL] = 27;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_SHINING_CRYSTAL   ] = 28;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_UNIQUE_CRYSTAL    ] = 29;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_GARNET_CRYSTAL    ] = 30;
	//sendPacket.ui64Statitics[eSTATISTIC_COMPOSE_NOBLESS_CRYSTAL   ] = 31;
	//sendPacket.ui64Statitics[eSTATISTIC_ENCHANT_COUNT             ] = 32;
	//sendPacket.ui64Statitics[eSTATISTIC_RANKUP_COUNT              ] = 33;
	//sendPacket.ui64Statitics[eSTATISTIC_CRYSTALIZE_COUNT          ] = 34;
	//
	//SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
}

void RTTServer::OnRTTGArmorkitDispatchNTF( char* buffer, int size )
{
	MSG_RTTG_ARMORKIT_DISPATCH_NTF*	recvPacket = ( MSG_RTTG_ARMORKIT_DISPATCH_NTF* )buffer;
	MSG_RTTG_ARMORKIT_DISPATCH_ANS	sendPacket;

	sendPacket.dwResult = 1;
	sendPacket.dwWopsKey = recvPacket->dwWopsKey;

	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );
	OutputDebug("OnRTTGArmorkitDispatchNTF");
}

void RTTServer::OnRTTGChangeGuildNameReq( char* buffer, int size )
{
	::srand(::GetTickCount());
	DWORD dwRet = (DWORD)(rand() % 3);

	MSG_RTTG_CHANGE_GUILDNAME_REQ*	recvPacket = ( MSG_RTTG_CHANGE_GUILDNAME_REQ* )buffer;
	MSG_RTTG_CHANGE_GUILDNAME_ANS	sendPacket;

	sendPacket.dwWopsKey = recvPacket->dwWopsKey;
	sendPacket.dwResult = dwRet;

	if( dwRet )
	{
		strcpy(sendPacket.szResultDescription, "길드이름을 변경하지 못하였습니다! 님하 죄송~ ^^;");
	}
	else
	{
		strcpy(sendPacket.szResultDescription, "길드이름을 성공적으로 변경하였습니다!");
	}
	
	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );

	OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq] Recv Change Gulid Name\n");
	OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq]		WopsKey		: %d\n", recvPacket->dwWopsKey );
	OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq]		World		: %d\n", recvPacket->dwServerGuid.GetWorldID() );
	OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq]		GuildGuid	: %d\n", recvPacket->dwGuildGuid);
	OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq]		GuildName	: %s\n", recvPacket->szGuildName);
	OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq]		Description	: %s\n", recvPacket->szDescription);

	OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq] Send Result To RTTAgent\n");
	OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq]		Result				: %d\n", sendPacket.dwResult );
	OutputDebug("[RTTServer::OnRTTGChangeGuildNameReq]		ResultDescriptin	: %s\n", sendPacket.szResultDescription );
}

void RTTServer::OnRTTGSendMessageReq( char* buffer, int size )
{
	::srand(::GetTickCount());
	DWORD	dwRet = 0;
	
	MSG_RTTG_SEND_MESSAGE_REQ*	recvPacket = ( MSG_RTTG_SEND_MESSAGE_REQ* )buffer;
	MSG_RTTG_SEND_MESSAGE_ANS	sendPacket;

	sendPacket.dwWopsKey = recvPacket->dwWopsKey;
	sendPacket.dwCount = recvPacket->dwCount;

	for( DWORD dwIndex = 0 ; (dwIndex < sendPacket.dwCount) && (dwIndex < MSG_RTTG_SEND_MESSAGE_ANS::MAX_RESULT_INFO) ; ++dwIndex )
	{
		dwRet = rand() % 2;
		sendPacket.arrResultInfoList[dwIndex].dwResult = dwRet;
		strncpy( sendPacket.arrResultInfoList[dwIndex].szRecvCharName, recvPacket->arrUserInfoList[dwIndex].szRecvCharName, MSG_RTTG_SEND_MESSAGE_ANS::MAX_CHAR_NAME_LENGTH );

		if( dwRet )
		{
			strcpy(sendPacket.arrResultInfoList[dwIndex].szResultDescription, "쪽지를 전송하지 못했습니다! 님하 죄송~ ^^;");
		}
		else
		{
			strcpy(sendPacket.arrResultInfoList[dwIndex].szResultDescription, "쪽지를 전송하였습니다!");
		}
	}

	SendPacket( &sendPacket, sendPacket.wSize + SIZEOFHDR );

	OutputDebug("[RTTServer::OnRTTGSendMessageReq] Send Message To User\n");
	OutputDebug("[RTTServer::OnRTTGSendMessageReq]		WopsKey		: %d\n", recvPacket->dwWopsKey);
	OutputDebug("[RTTServer::OnRTTGSendMessageReq]		ServerGuid	: %d\n", recvPacket->dwServerGuid);
	OutputDebug("[RTTServer::OnRTTGSendMessageReq]		Count		: %d\n", recvPacket->dwCount);

	for( DWORD dwLogIndex = 0 ; (dwLogIndex < recvPacket->dwCount) && ( dwLogIndex < MSG_RTTG_SEND_MESSAGE_REQ::MAX_USER_INFO ) ; ++dwLogIndex )
	{
		OutputDebug("[RTTServer::OnRTTGSendMessageReq]		RecvChar	: %s\n", recvPacket->arrUserInfoList[dwIndex].szRecvCharName);
	}

	OutputDebug("[RTTServer::OnRTTGSendMessageReq]		SendChar	: %s\n", recvPacket->szSendCharName);
	OutputDebug("[RTTServer::OnRTTGSendMessageReq]		Message		: %s\n\n", recvPacket->szMessage);


	OutputDebug("[RTTServer::OnRTTGSendMessageReq] Send Result To RTTAgent\n");
	for( dwLogIndex = 0 ; (dwLogIndex < sendPacket.dwCount) && (dwLogIndex < MSG_RTTG_SEND_MESSAGE_ANS::MAX_RESULT_INFO) ; ++dwLogIndex )
	{
		OutputDebug("[RTTServer::OnRTTGSendMessageReq]		Result				: %d\n", sendPacket.arrResultInfoList[dwLogIndex].dwResult);
		OutputDebug("[RTTServer::OnRTTGSendMessageReq]		RecvChar			: %s\n", sendPacket.arrResultInfoList[dwLogIndex].szRecvCharName);
		OutputDebug("[RTTServer::OnRTTGSendMessageReq]		ResultDescription	: %s\n\n", sendPacket.arrResultInfoList[dwLogIndex].szResultDescription);
	}
}