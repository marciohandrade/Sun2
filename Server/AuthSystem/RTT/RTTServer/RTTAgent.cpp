#include "stdafx.h"
#include "RTTAgent.h"
#include "Web.h"
#include "ManagerClient.h"
#include "ManagerService.h"
#include "ManagerWops.h"
#include "ManagerWeb.h"
#include "MonitorProcess.h"
#include "TestCounter.h"
#include "EVENT_TYPE_SPEC.h"
#include "HDHelper.h"

#include "Entry.h"

DWORD	RTTAgent::m_updateBattleZoneInfoTick	= 3000;
DWORD	RTTAgent::m_updateUserMoneyTick			= 3000;

RTTAgent::RTTAgent()
	: eXtreme::ServerSession( 40960, 40960 ),
		m_worldID( 0 ), m_channelID( 0 ), m_serverType( 0 ), m_serverID( 0 ),
		m_tickAlive(0), m_bConnect(false), m_ErrorIndexCount(0)
{
}

RTTAgent::~RTTAgent()
{
}

void RTTAgent::OnAccept()
{
	m_tickAlive			= ::GetTickCount();
	m_containType		= 0;
	m_ownServerCount	= 0;

	OutputDebug( "Accepted RTTAgent from %s\n", GetIPAddress() );

	m_bConnect = true;
}

void RTTAgent::OnClose( DWORD error )
{
	/*
	for(DWORD i = 0 ; i < m_ownServerCount ; i++ )
	{
		//m_ownServerList[i]->userCountField	= ( DWORD )-1;
		//m_ownServerList[i]->userCountBattle	= ( DWORD )-1;
	}
	*/
	
	{ // RTTAgent�� �׾��� ��� resource ������ �ʱ�ȭ �Ѵ�.
		ResourceInfo info;
		for(DWORD i = 0 ; i < m_ownServerCount ; i++ )
		{
			info.ti_WorldID				= m_ownServerList[i]->ti_WorldID;
			info.ti_ChannelID			= m_ownServerList[i]->ti_ChannelID;
			info.ti_ServerType			= m_ownServerList[i]->ti_ServerType;
			info.ti_ServerID			= m_ownServerList[i]->ti_ServerID;

			info.i_CpuMaxResource		= 0;
			info.i_CpuUseResource		= 0;
			info.i_MemoryMaxResource	= 0;
			info.i_MemoryUseResource	= 0;
			info.i_HDDMaxResource		= 0;
			info.i_HDDUseResource		= 0;
			
			Database::Instance()->UpdateSystemResource( info );
		}
	}

	ManagerClient::Instance()->RemoveClient( this );

    // �� ���� URL Index �ʱ�ȭ
	for( DWORD dw = 0 ; dw < m_ErrorIndexCount ; dw++ )
	{
		m_ErrorIndexList[dw].dwConnectParentHistoryIndex = m_ErrorIndexList[dw].dwPageFaultParentHistoryIndex =  0;
	}
    
	m_worldID		= 0;
	m_channelID		= 0;
	m_serverType	= 0;
	m_serverID		= 0;

	m_containType	= 0;
	m_ownServerCount= 0;
	m_ErrorIndexCount = 0;
    
	//  RTTAgent�� ������ �������� ����Ѵ�.
	{
		wchar_t		wcsIP[20] = {0, };

		::MultiByteToWideChar( CP_ACP, 0, GetIPAddress(), -1, wcsIP, 20 );
		Database::Instance()->CheckRTTAgentCertify( wcsIP, false );
	}

	OutputDebug( "Closed connection with RTTAgent(%s, err=%d)\n", GetIPAddress(), error );

	m_tickAlive = 0;
	m_bConnect = false;
}

void RTTAgent::Update()
{
	//  �ֱ����� ������ ���� RTTAgent�� ������ �����Ų��.
	if( (0 != m_tickAlive) && (::GetTickCount() - m_tickAlive > ALIVETICK_LIMITE) )
	{
		OutputDebug( "No more response from RTTAgent(%s)\n", GetIPAddress() );
		SetKill();
	}


	//  �̷��� ó���ϴ� �͵� �� �����ϴ�...
	if( ( m_containType & SERVERTYPE_MASTER ) == SERVERTYPE_MASTER )
	{
		static	DWORD	tickZone			= ::GetTickCount();
		static	DWORD	tickUserMoney		= ::GetTickCount();
		static  DWORD   tickGameStatistics	= ::GetTickCount();

		//  ��Ʋ���� ���������� ��û�Ѵ�.
		if( ::GetTickCount() - tickZone >= m_updateBattleZoneInfoTick )
		{
			SendBattleZoneInfoReq();

			tickZone = ::GetTickCount();
		}

		//  ��ȭ�� ������ ��û�Ѵ�.
		if( ::GetTickCount() - tickUserMoney >= m_updateUserMoneyTick )
		{
			SendUserMoneyReq();
			tickUserMoney = ::GetTickCount();
		}
		
		// ���� ��� ��û
		if( ::GetTickCount() - tickGameStatistics >= 30000){
			SendGameStatisticsReq();
			tickGameStatistics	= ::GetTickCount();
		}

        //OutputDebug( "[RTTAgent::Update] Send BattleZoneInfo, UserMoney, GameStatistics Req To RTTAgent(%s)\n", GetIPAddress() );
	}

	eXtreme::ServerSession::Update();
}

void RTTAgent::SendMessage( MSG_HEADER8& packet, int size )
{
	if( m_bConnect )
	{
		//  ������ ������ Overflow ���θ� �˻��Ѵ�.
		{
			DWORD	remainSize = 0, maxSize = 0;

			GetSendBufferSize( &remainSize, &maxSize );
			if( remainSize + size >= maxSize )
			{
				//  ������ ���ۿ� ���� �� ���ٸ�, ������ ������ ���� �۽��Ͽ�
				//  ���۸� ��쵵�� �Ѵ�.
				Flush();
			}
		}


		if( size == 0 )
		{
			ServerSession::SendPacket( &packet, packet.dwSize + sizeof( MSG_HEADER8 ) );
		}
		else
		{
			ServerSession::SendPacket( &packet, size );
		}
	}
	else	// ������ �ȵǾ� �ִ� ���
	{
		OutputDebug( "disconnected RTTAgent(%s)\n", GetIPAddress() );
	}
}

void RTTAgent::SendBattleZoneInfoReq()
{
	MSG_RTTW_BATTLEZONEINFO_REQ	sendPacket;
	SendMessage( sendPacket );
}

void RTTAgent::SendUserMoneyReq()
{
	MSG_RTTW_USERMONEY_REQ	sendPacket;
	SendMessage( sendPacket );
}

void RTTAgent::SendGameStatisticsReq()
{
	MSG_RTTW_GAMESTATISTICS_REQ	sendPacket;
	SendMessage( sendPacket );
}

void RTTAgent::SendPatchLauncherInfoNTF()
{
	MSG_RTTR_PATCHLAUNCHERINFO_NTF	sendPacket;
	wchar_t		fileName[MAX_PATH] = {0, };

	::MultiByteToWideChar( CP_ACP, 0, SERVERINFO_FILENAME, -1, fileName, MAX_PATH );

	::GetPrivateProfileStringW( L"Patch Launcher", L"Path", L"", sendPacket.szAbsolutePath, 100, fileName );
	::GetPrivateProfileStringW( L"Patch Launcher", L"Binary", L"", sendPacket.szBinaryName, 60, fileName );

	SendMessage( sendPacket );
}

void RTTAgent::OnSend( int /*size*/ )
{
#ifdef  TESTCOUNTER
	TestCounter::Instance()->Inc( COUNTER_RTTA_SENDPACKET, 1 );
	TestCounter::Instance()->Inc( COUNTER_RTTA_SENDTRANSBYTES, size );
#endif
}

void RTTAgent::OnReceive( char* buffer, int size )
{
	MSG_HEADER8*		header = ( MSG_HEADER8* )buffer;

	switch( header->dwID )
	{
	case  RTTW_CERTIFY_REQ:				OnRTTWCertifyReq( buffer, size );			break;
	case  RTTW_DISCERTIFY_NTF:			OnRTTWDisCertifyNTF( buffer, size );		break;
	case  RTTW_GAMENOTICE_ANS:			OnRTTWGameNoticeAns( buffer, size );		break;
	case  RTTW_DROPUSER_ANS:			OnRTTWDropUserAns( buffer, size );			break;
	case  RTTW_GAMEADMINLIST_REQ:		OnRTTWGameAdminListReq( buffer, size );		break;
	case  RTTW_USERCOUNT_NTF:			OnRTTWUserCountNTF( buffer, size );			break;
	case  RTTW_BATTLEZONEINFO_ANS:		OnRTTWBattleZoneInfoAns( buffer, size );	break;
	case  RTTW_ARMORKIT_FINDHACK_NTF:	OnRTTWArmorKitFindHackNTF( buffer, size );	break;
	case  RTTW_ABUSING_NTF:				OnRTTWAbusingNTF( buffer, size );			break;
	case  RTTW_USERMONEY_ANS:			OnRTTWUserMoneyAns( buffer, size );			break;
	case  RTTW_SERVERSHUTDOWN_ANS:		OnRTTWServerShutdownAns( buffer, size );	break;
	case  RTTW_PATCHRESULT_ANS:			OnRTTWPatchResultAns( buffer, size );		break;
	case  RTTW_LAUNCHPROCESS_ANS:		OnRTTWLaunchProcessAns( buffer, size );		break;
	case  RTTR_RESOURCE_NTF:			OnRTTRResourceNTF( buffer, size );			break;
	case  RTTR_PROCESSLIST_REQ:			OnRTTRProcessListReq( buffer, size );		break;
	case  RTTR_PROCESSMONITOR_NTF:		OnRTTRProcessMonitorNTF( buffer, size );	break;
	case  RTTR_WEB_URL_REQ:				OnRTTRWebUrlReq( buffer, size );			break;
	case  RTTR_WEB_REQUEST_NTF:			OnRTTRWebRequestNTF( buffer, size );		break;

	case  RTTW_FUNCRESTRICT_RELOAD_ANS:	OnRTTWFuncrestrictReloadAns( buffer, size );break;
	case  RTTW_AUTHIPLIST_RELOAD_ANS:	OnRTTAuthIPListReloadAns( buffer, size );	break;
	case  RTTW_GAMEVERSION_UPDATE_ANS:	OnRTTGameVersionUpdateAns( buffer, size );	break;
	case  RTTW_GAMESTATISTICS_ANS:		OnRTTGameStatisticsAns( buffer, size );		break;
//	case  RTTW_PATTERN_RELOAD_ANS:		OnRTTPatternReloadAns( buffer, size);		break;

	// RTTAagent �� HeartBeat �߰�
	case  RTTR_HEARTBEAT_NTF:			OnRTTRHeartBeatNTF( buffer, size );			break;	
	
	case  RTTW_ARMORKIT_DISPATCH_ANS:	OnRTTWArmorkitDispatchANS( buffer, size );	break;	
	case  RTTW_CHANGE_GUILDNAME_ANS:	OnRTTWChangeGuildNameAns( buffer, size);	break;
	case  RTTW_SEND_MESSAGE_ANS:		OnRTTWSendMessageAns( buffer, size);		break;
	case  RTTH_DROPUSER_ANS:			OnRTTHDropUserAns( buffer, size);			break;

	default:
		OutputDebug( "[RTTAgent Session] Not defined MessageID(=0x%x)(%s)\n", header->dwID, GetIPAddress() );
		break;
	}

#ifdef  TESTCOUNTER
	TestCounter::Instance()->Inc( COUNTER_RTTA_RECVPACKET, 1 );
	TestCounter::Instance()->Inc( COUNTER_RTTA_RECVTRANSBYTES, size );
#endif
	//static DWORD dwCurrentTick = ::GetTickCount();
	//OutputDebug( "[RTTAgent::OnReceive] %s : %.3f sec\n" , GetIPAddress(), ((double)(::GetTickCount() - dwCurrentTick)/1000)), dwCurrentTick = ::GetTickCount();
}

void RTTAgent::OnRTTWCertifyReq( char* buffer, int size )
{
	eXtreme::Stream			stream( buffer, size );
	MSG_RTTW_CERTIFY_REQ*	recvPacket = ( MSG_RTTW_CERTIFY_REQ* )( buffer + 12 );
	MSG_RTTW_CERTIFY_ANS	sendPacket;
	DWORD					dummySize = 0, dummyID = 0, key = 0;

	sendPacket.dwResult	= 1;		//  '���� ����'�� �ʱ�ȭ
	sendPacket.dwType	= recvPacket->dwType;
	sendPacket.dwUID	= 0;

//	m_containType = recvPacket->dwType;


	//  RTTS-A ������ ����� ���� ���Ƿ� ���Ե� ����� �о���δ�.
	stream >> dummySize >> dummyID >> key;

	switch( recvPacket->dwType )
	{
	case  SERVERTYPE_AGENT:
		{
			if( ManagerClient::Instance()->AddClient( this ) == false )
			{
				OutputDebug( "[OnRTTCertifyReq] Already connected IP(=%s).\n", GetIPAddress() );
				break;
			}
			else
			{
				OutputDebug( "[OnRTTCertifyReq] Add ManagerClient IP(=%s).\n", GetIPAddress() );
			}

			//  Agent�� ID���� �޾ƿ´�.
			wchar_t	wcs[20] = {0, };
			::MultiByteToWideChar( CP_ACP, 0, GetIPAddress(), -1, wcs, 20 );
			bool ret = Database::Instance()->FindServerID( wcs, &m_worldID, &m_channelID, &m_serverType, &m_serverID );
			if( ret == false )
			{
				//  DB�� ��ϵ��� ���� Agent�̴�.
				sendPacket.dwResult	= 2;
				sendPacket.dwType	= recvPacket->dwType;
				sendPacket.dwUID	= 0;

				OutputDebug( "[RTTAgent::OnRTTWCertifyReq] not regist database IP(%s).\n", GetIPAddress());
			}
			//  ������ Ȯ���Ѵ�.
			else if( recvPacket->dwVersion != Entry::Instance()->GetRTTAgentVersion() )
			{
				//  ������ Ʋ�� ���, RTTService�� ���� ��ġ�� ����Ѵ�.
				sendPacket.dwResult	= 1;
				sendPacket.dwType	= recvPacket->dwType;
				sendPacket.dwUID	= 0;

				OutputDebug( "[OnRTTWCertifyReq] Invalid version of RTTAgent(0x00%x) Server(%s)\n",recvPacket->dwVersion , GetIPAddress() );

				/**
					2006.08.10
					add by guiun
					RTTAgent�� RTTService�� �������� ���� �̴�.
					RTTService�� ���� RTTServer�� ������ �Ǿ� ���� �ʴٸ� RTTAgent �ڵ���ġ�� ���� �ʴ´�.
					RTTService�� �������� �ƴ϶�� RTTAgent�� ���� ���� �Ͽ� �翬���� ���� �Ͽ� ��ġ�� �ٽ� ���� �ǵ��� �Ѵ�.
				*/
			
				RTTService*	pService = ManagerService::Instance()->FindClient( GetIPAddress() );
				if( pService != NULL )
				{
					pService->SendRTTAPatch();
				}
				else
				{
					OutputDebug("[RTTAgent::OnRTTWCertifyReq] RTTService�� ���� ���� �ƴմϴ�. �ڵ���ġ ����!!\n");
					SetKill();
				}
			}
			else
			{
				SERVER_KEY	uid;

				uid.Set( ( BYTE )m_worldID, ( BYTE )m_channelID, ( BYTE )m_serverType, ( BYTE )m_serverID );

				sendPacket.dwResult	= 0;
				sendPacket.dwType	= recvPacket->dwType;
				sendPacket.dwUID	= uid.dwKey;

				//  ��ġ�� ��������� �����Ѵ�.
				{
					wchar_t		innerIP[20] = {0, };

					::MultiByteToWideChar( CP_ACP, 0, GetIPAddress(), -1, innerIP, 20 );
					Database::Instance()->MakeServerListIn( innerIP, m_ownServerList, 50, &m_ownServerCount );
				}

				//  RTTAgent�� �����Ǿ����� üũ�Ѵ�.
				{
					wchar_t		wcsIP[20] = {0, };

					::MultiByteToWideChar( CP_ACP, 0, GetIPAddress(), -1, wcsIP, 20 );
					Database::Instance()->CheckRTTAgentCertify( wcsIP, true );
				}

				OutputDebug( "[OnRTTWCertifyReq] RTTAgent Server(%s)\n", GetIPAddress() );
			}
		}
		break;

	case  SERVERTYPE_AUTH:
		{
			m_containType		|= recvPacket->dwType;
			sendPacket.dwResult	= 0;
			sendPacket.dwType	= recvPacket->dwType;
			sendPacket.dwUID	= 0;

			OutputDebug( "[OnRTTWCertifyReq] AuthAgent Server(%s)\n", GetIPAddress() );
		}
		break;

	case  SERVERTYPE_MASTER:
		{ 
			m_containType		|= recvPacket->dwType;
			sendPacket.dwResult	= 0;
			sendPacket.dwType	= recvPacket->dwType;
			sendPacket.dwUID	= 0;
			
			// ������ ������ IP�� ����Ѵ�
			ManagerClient::Instance()->AddMasterServerIP( m_worldID, GetIPAddress() );

			OutputDebug( "[OnRTTWCertifyReq] Master Server(%s)\n", GetIPAddress() );
			
		}
		break;
	}

	stream.Clear();
	stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_CERTIFY_ANS << ( DWORD )key;
	stream.Write( &sendPacket, sizeof( sendPacket ) );

	SendPacket( stream.GetBuffer(), stream.GetDataSize() );
}

void RTTAgent::OnRTTWDisCertifyNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_DISCERTIFY_NTF*	recvPacket = ( MSG_RTTW_DISCERTIFY_NTF* )buffer;
	/*
		if(recvPacket->dwType==SERVERTYPE_MASTER){	// Master Server�� �ش� Agent���� ���� ������ �Ȱ��
		}
	*/
	//  �ش� ������ ������Ͽ��� �����Ѵ�.
	m_containType ^= recvPacket->dwType;
}

void RTTAgent::OnRTTWGameNoticeAns( char* buffer, int size )
{
	eXtreme::Stream				stream( 4096 );
	MSG_RTTW_GAMENOTICE_ANS*	recvPacket = ( MSG_RTTW_GAMENOTICE_ANS* )( buffer + 12 );
	DWORD						dummySize = 0, dummyID = 0, key = 0;
	Wops*						wopsComponent = NULL;

	//  RTTS-A ������ ����� ���� ���Ƿ� ���Ե� ����� �о���δ�.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;

	wopsComponent = ( Wops* )ManagerWops::Instance()->Find( key );
	if( wopsComponent == 0 )
		return;

	wopsComponent->SendPacket( recvPacket, sizeof( *recvPacket ) );
}

void RTTAgent::OnRTTWDropUserAns( char* buffer, int size )
{
	eXtreme::Stream			stream( 4096 );
	MSG_RTTW_DROPUSER_ANS*	recvPacket = ( MSG_RTTW_DROPUSER_ANS* )( buffer + 12 );
	DWORD					dummySize = 0, dummyID = 0, key = 0;
	Wops*					wopsComponent = NULL;

	//  RTTS-A ������ ����� ���� ���Ƿ� ���Ե� ����� �о���δ�.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;

	OutputDebug("[RTTAgent::OnRTTWDropUserAns] Session UID : %d\n", key );

	wopsComponent = ( Wops* )ManagerWops::Instance()->Find( key );
	if( wopsComponent == 0 )
	{
		OutputDebug( "[RTTAgent::OnRTTWDropUserAns] Wops cannot found, Result:%d\n", recvPacket->dwError);
		return;
	}

	wopsComponent->SendPacket( recvPacket, sizeof( *recvPacket ) );

	OutputDebug( "[RTTAgent::OnRTTWDropUserAns] Result:%d\n", recvPacket->dwError);
}

void RTTAgent::OnRTTWGameAdminListReq( char* buffer, int size )
{
	eXtreme::Stream				stream( 4096 );
	//MSG_RTTW_GAMEADMINLIST_REQ*	recvPacket = ( MSG_RTTW_GAMEADMINLIST_REQ* )( buffer + 12 );
	MSG_RTTW_GAMEADMINLIST_ANS	sendPacket;
	DWORD						dummySize = 0, dummyID = 0, key = 0, count = 0;
	AdminInfo*					list[256] = {0, };
	wchar_t						data[96] = {0, };

	//  RTTS-A ������ ����� ���� ���Ƿ� ���Ե� ����� �о���δ�.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;

	//  ������ ��������� Wide character ������ ��ȯ�Ѵ�.
	{
		sendPacket.dwID = RTTW_GAMEADMINLIST_ANS;

		Database::Instance()->MakeAdminList( list, 256, &count );

		memset( sendPacket.szList, 0, sizeof( sendPacket.szList ) );
		for(DWORD i = 0 ; i < count ; i++ )
		{
			_snwprintf( data, 96, L"%s,%s,%d;", list[i]->UserID, list[i]->IpAddr, list[i]->LV );
			wcscat( sendPacket.szList, data );
		}
		sendPacket.dwLength = ( DWORD )wcslen( sendPacket.szList );
	}

	//  MSG_RTT_GAMEADMINLIST_ANS�� Capsulate�ؼ� �����Ѵ�.
	{
		stream.Clear();
		stream << ( DWORD )( 4 + sizeof( sendPacket ) ) << ( DWORD )RTTW_GAMEADMINLIST_ANS << ( DWORD )key;
		stream.Write( &sendPacket, sizeof( sendPacket ) );

		SendPacket( stream.GetBuffer(), stream.GetDataSize() );
	}
}

/*
	[2006.04.19] modify by guiun
	PC�� ���� ���� �߰�
*/
void RTTAgent::OnRTTWUserCountNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_USERCOUNT_NTF*	recvPacket = ( MSG_RTTW_USERCOUNT_NTF* )buffer;
	
	/*
	OutputDebug( "[OnRTTWUserCountNTF] %d, %d, %d, %d, %d, %d\n",
				recvPacket->dwWorldID, 
				recvPacket->dwChannelID, 
				recvPacket->dwFieldUserCount, 
				recvPacket->dwFieldPCRoomCount,
				recvPacket->dwBattleUserCount,
				recvPacket->dwBattleZonePCRoomCount);
	*/
	Database::Instance()->UpdateCurrentUserCount(recvPacket->dwWorldID, 
												 recvPacket->dwChannelID,
											     recvPacket->dwFieldUserCount, 
												 recvPacket->dwFieldPCRoomCount, 
												 recvPacket->dwBattleUserCount, 
												 recvPacket->dwBattleZonePCRoomCount
#ifdef	_090901_CHANNELLING_CONNECT_USER_
												 ,recvPacket->dwHangameFieldUserCount
												 ,recvPacket->dwHangameFieldPCRoomCount
												 ,recvPacket->dwHangameBattleUserCount
												 ,recvPacket->dwHangameBattleZonePCRoomCount
												 ,recvPacket->dwPMangFieldUserCount
												 ,recvPacket->dwPMangFieldPCRoomCount
												 ,recvPacket->dwPMangBattleUserCount
												 ,recvPacket->dwPMangBattleZonePCRoomCount
#endif
												 );
}

/*
	[2006.04.26] modify by guiun
	���� ���� �� �߰�
*/
void RTTAgent::OnRTTWBattleZoneInfoAns( char* buffer, int /*size*/ )
{
	MSG_RTTW_BATTLEZONEINFO_ANS*	recvPacket = ( MSG_RTTW_BATTLEZONEINFO_ANS* )buffer;

	//  ���ù�
	// eZONETYPE_HUNTING -> eZONETYPE_CHUNTING
	Database::Instance()->UpdateBattleZoneInfo(recvPacket->info.byWorldID, 
		                                       recvPacket->info.byChannelID, 
											   1, 
											   recvPacket->info.dwZoneCount[eZONETYPE_CHUNTING] );
	//  �̼ǹ�
	Database::Instance()->UpdateBattleZoneInfo(recvPacket->info.byWorldID, 
		                                       recvPacket->info.byChannelID, 
											   2, 
											   recvPacket->info.dwZoneCount[eZONETYPE_MISSION] );
	//  PvP��
	Database::Instance()->UpdateBattleZoneInfo(recvPacket->info.byWorldID, 
		                                       recvPacket->info.byChannelID, 
											   3, 
											   recvPacket->info.dwZoneCount[eZONETYPE_PVP] );
	//  ���� ���ù�
	/*
	Database::Instance()->UpdateBattleZoneInfo(recvPacket->info.byWorldID, 
		                                       recvPacket->info.byChannelID, 
											   4, 
											   recvPacket->info.dwZoneCount[eZONETYPE_CHUNTING] );
	*/
	//OutputDebug( "[OnRTTWBattleZoneInfoAns] %d, %d, %d\n", recvPacket->info.dwZoneCount[eZONETYPE_CHUNTING], recvPacket->info.dwZoneCount[eZONETYPE_MISSION], recvPacket->info.dwZoneCount[eZONETYPE_PVP]);
}

void RTTAgent::OnRTTWArmorKitFindHackNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_ARMORKIT_FINDHACK_NTF*	recvPacket = ( MSG_RTTW_ARMORKIT_FINDHACK_NTF* )buffer;
	char							ip[20] = {0, };

	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szIP, -1, ip, 20, 0, 0 );
	
//	try
//	{
		switch( recvPacket->dwHackType )
		{
		case  AKRUNTIME_ERROR:
			{
				//  Nothing contents...?
			}
			break;

		case  AKDETECT_SPEEDHACK:
			{
				PSPEED_HACK_CHECK_INFO	info = ( PSPEED_HACK_CHECK_INFO )&recvPacket->szBuf[0];

				Database::Instance()->QueryExternal2( "{call USP_AddHackLog_SPEEDHACK(%d, %d, '%s', %d, %d, %d, %d)}",
					recvPacket->dwHackType, recvPacket->dwGuid, ip,
					info->dwTGT, info->dwGTC, info->dwTSC, info->dwQPC );
			}
			break;

		case  AKDETECT_MALWARE:
			{
				PPROCESS_SCAN_INFO	info = ( PPROCESS_SCAN_INFO )&recvPacket->szBuf[0];

				Database::Instance()->QueryExternal2( "{call USP_AddHackLog_PROCESSSCAN(%d, %d, '%s', %d, '%s', %d, %d, '%s')}",
					recvPacket->dwHackType, recvPacket->dwGuid, ip,
					info->ProcessId, info->ProcessName, info->SignatureIndex, info->SianatureType, info->SignatureName );
			}
			break;

		case  AKDETECT_PEINTEGRITY:
			{
				PPE_CHECKSUM_INFO	info = ( PPE_CHECKSUM_INFO )&recvPacket->szBuf[0];

				Database::Instance()->QueryExternal2( "{call USP_AddHackLog_PECHECKSUM(%d, %d, '%s', %d, %d)}",
					recvPacket->dwHackType, recvPacket->dwGuid, ip,
					info->HeaderSum, info->CalcSum );
			}
			break;

		case  AKDETECT_APIHOOK:
			{
				PHOOKED_API_INFO	info = ( PHOOKED_API_INFO )&recvPacket->szBuf[0];
		
				if( info->API_TYPE == 0 )			//  Exported API	- HEA ����
				{
					Database::Instance()->QueryExternal2(
						"{call USP_AddHackLog_APIHOOK(%d, %d, '%s', %d, '%s', '%s', %d, %d, %d, '', '', '')}",
						recvPacket->dwHackType, 
						recvPacket->dwGuid, 
						ip,
						info->API_TYPE, 
						info->API_INFO.HEA.DLLName, 
						info->API_INFO.HEA.ApiName,
						info->API_INFO.HEA.HookdAddr, 
						info->API_INFO.HEA.HOOKED_CODE,
						info->API_INFO.HEA.OriginalAddr );
				}
				else if( info->API_TYPE == 1 )		//  Imported API	- HIA ����
				{
					Database::Instance()->QueryExternal2(
						"{call USP_AddHackLog_APIHOOK(%d, %d, '%s', %d, '%s', '%s', %d, %d, NULL, %d, %d, %d)}",
						recvPacket->dwHackType, 
						recvPacket->dwGuid, 
						ip,
						info->API_TYPE, 
						info->API_INFO.HIA.DLLName, 
						info->API_INFO.HIA.ApiName,
						info->API_INFO.HIA.HookdAddr, 
						info->API_INFO.HIA.HOOKED_CODE,
						info->API_INFO.HIA.DetourAddr,
						info->API_INFO.HIA.dwStart, 
						info->API_INFO.HIA.dwEnd );
				}
				/*
					[2006.05.08] by guiun
					���� �߰� ����
					IMP_MOD_INTEGRITY Type �߰� ó��
				*/
				else if( info->API_TYPE == 2)
				{
					Database::Instance()->QueryExternal2(
						"{call USP_AddHackLog_APIHOOK(%d, %d, '%s', %d, null, null, null, null, null, null, null, null, %d, %d)}",
						recvPacket->dwHackType, 
						recvPacket->dwGuid, 
						ip,
						info->API_TYPE, 
						info->API_INFO.IMI.OrgImpModCount,
						info->API_INFO.IMI.CurImpModCount );
				}
			}
			break;

		case  AKDETECT_DEBUG:
			{
				PANTI_DEBUG_CHECK_INFO	info = ( PANTI_DEBUG_CHECK_INFO )&recvPacket->szBuf[0];

				Database::Instance()->QueryExternal2( "{call USP_AddHackLog_DEBUGCHECK(%d, %d, '%s', %d, %d, %d, %d)}",
					recvPacket->dwHackType, recvPacket->dwGuid, ip,
					info->bSpecificAPI, info->bProcessInfo, info->bParentProcessInfo, info->bActiveDevices );
			}
			break;

		case  AKDETECT_AKINTEGRITY:
		case  AKDETECT_MEMEDIT:
		case  AKDETECT_DOUBT:
			{
				//  Not using
			}
			break;
		case  AKDETECT_RUNCODE:
			{
				//  No structure...
			}
			break;

/*-----------------------------------------------------------------------------------------*/
// 2006. 7. 19 ����ȣ HOOKDefender

		case  AKDETECT_HOOK_DETECT:
			{
				PAPI_TO_RESTORE restore = NULL;
				HOOKED_API_LIST hal;
				char* pBuf = recvPacket->szBuf;
				
				int iRet = CHDHelper::stream_to_hal(recvPacket->dwBufSize, pBuf, hal);

				_ASSERTE(-1 != iRet);
				if (-1 == iRet)
				{
					printf("Oops! stream_to_hal() failed	\n", "");
					break;
				}

                int iHackLogID  = 0;		// ���ν����� ���ε��� LogID����

				for (HOOKED_API_LIST::iterator iter = hal.begin(); iter != hal.end(); iter++)
				{
					restore = &(*iter);

					iHackLogID = Database::Instance()->QueryArmorKit(iHackLogID, AKDETECT_HOOK_DETECT, recvPacket->dwGuid,
						ip, restore->HookType, restore->DLLName, restore->Ordinal, restore->ApiName,
						restore->OriginalAddr, restore->CurrentAddr, restore->DetourAddr, restore->OriginalOpcodes,
						restore->CurrentOpcodes, restore->DetourAddrOpcodes, restore->ValidStartAddr, restore->ValidEndAddr);

				}

//				stream ���۴� �ݵ�� CHDHelper �� ���ؼ� �Ҹ��� �����
//				CHDHelper::free_converted_stream(pBuf);	
			}
			break;

		case  AKDETECT_HOOK_RESTORE_FAIL:
			{
				PRESTORE_FAULT_API restfail = NULL;
				HDFAULT_API_LIST hal;
				char* pBuf = &recvPacket->szBuf[0];

				int iRet = CHDHelper::stream_to_hdfal(recvPacket->dwBufSize, pBuf, hal);

				_ASSERTE(-1 != iRet);
				if (-1 == iRet)
				{
					printf("Oops! stream_to_hal() failed	\n", "");
					break;
				}

				int iHackLogID  = 0;	// ���ν����� ���ε��� LogID����

				for (HDFAULT_API_LIST::iterator iter = hal.begin(); iter != hal.end(); iter++)
				{
					restfail = &(*iter);

					iHackLogID = Database::Instance()->QueryArmorKit(iHackLogID, AKDETECT_HOOK_RESTORE_FAIL, recvPacket->dwGuid,
						ip, restfail->apiInfo.HookType, restfail->apiInfo.DLLName, restfail->apiInfo.Ordinal,
						restfail->apiInfo.ApiName, restfail->apiInfo.OriginalAddr, restfail->apiInfo.CurrentAddr,
						restfail->apiInfo.DetourAddr, restfail->apiInfo.OriginalOpcodes, restfail->apiInfo.CurrentOpcodes,
						restfail->apiInfo.DetourAddrOpcodes, restfail->apiInfo.ValidStartAddr, restfail->apiInfo.ValidEndAddr, restfail->ErrorCode);


					//OutputDebug("\n[ TEST DATA : %02d ]------------------------------\n", ord++);
					//OutputDebug("%d Protocol, Count : %d, Size : %d\n", restfail->apiInfo.HookType, hal.size(), recvPacket->dwBufSize);
					//OutputDebug("\tHookType: %d	\n", restfail->apiInfo.HookType);
					//OutputDebug("\tDLLName: %s	\n", restfail->apiInfo.DLLName);
					//OutputDebug("\tOrdinal: %d	\n", restfail->apiInfo.Ordinal);
					//OutputDebug("\tApiName: %s	\n", restfail->apiInfo.ApiName);
					//OutputDebug("\tErrorCode: %d	\n", restfail->ErrorCode);


					// �� ��ü�� �����͸� ���ν����� �����Ͽ� �����ϴ� �ڵ带 �߰��Ѵ�
//					Database::Instance()->QueryExternal2();
				}

//				stream ���۴� �ݵ�� CHDHelper �� ���ؼ� �Ҹ��� �����
//				CHDHelper::free_converted_stream(pBuf);	
			}
			break;
/*---------------------------------------------------------------------------------- END --*/
		}
//	}
//	catch( eXtreme::ExceptionDBC& )
//	{
//		return;
//	}
}

void RTTAgent::OnRTTWAbusingNTF( char* buffer, int /*size*/ )
{
	MSG_RTTW_ABUSING_NTF*	recvPacket = ( MSG_RTTW_ABUSING_NTF* )buffer;
	char					dateTime[32] = {0, };

	// recvPacket->m_dwTime �� DWORD64 �̴�.
	sprintf(dateTime, "%d-%02d-%02d %02d:%02d:%02d", 
		(DWORD)(recvPacket->m_dwTime/10000000000),													// ��
		(DWORD)(recvPacket->m_dwTime - (recvPacket->m_dwTime/10000000000) * 10000000000)/100000000,	// ��
		(DWORD)(recvPacket->m_dwTime - (recvPacket->m_dwTime/100000000) * 100000000)/1000000,		// ��
		(DWORD)(recvPacket->m_dwTime - (recvPacket->m_dwTime/1000000) * 1000000)/10000,				// ��
		(DWORD)(recvPacket->m_dwTime - (recvPacket->m_dwTime/10000) * 10000)/100,					// ��
		(DWORD)(recvPacket->m_dwTime - (recvPacket->m_dwTime/100) * 100));							// ��

	// �߰��� �κ�, ���ν����� �Ű����� �߰��� ���۾�(07-1-2)

	//  ���� Query�� ��û�Ѵ�.

	if( NULL != recvPacket->m_ptszUserID[0] )
	{
#ifndef _110503_CUSTOM_USERID_LENGTH
		recvPacket->m_ptszUserID[MSG_RTTW_ABUSING_NTF::MAX_ID_LEN] = '\0';
#else
		recvPacket->m_ptszUserID[ID_MAX_LEN] = '\0';
#endif
//		OutputDebug( "[RTTAgent::OnRTTWAbusingNTF] %s\n", recvPacket->m_ptszUserID);
	}

	if( NULL != recvPacket->m_ptszCharName[0] )
	{
		recvPacket->m_ptszCharName[MSG_RTTW_ABUSING_NTF::MAX_CHARNAME_LENGTH] = '\0';
//		OutputDebug( "[RTTAgent::OnRTTWAbusingNTF] %s\n", recvPacket->m_ptszCharName);
	}

	if( NULL != recvPacket->m_ptszIP[0] )
	{
		recvPacket->m_ptszIP[MSG_RTTW_ABUSING_NTF::MAX_IP_LENGTH] = '\0';
//		OutputDebug( "[RTTAgent::OnRTTWAbusingNTF] %s\n", recvPacket->m_ptszIP);
	}

	if( recvPacket->m_wLength > MSG_RTTW_ABUSING_NTF::MAX_DESC_LENGTH )
	{
        recvPacket->m_ptszDesc[MSG_RTTW_ABUSING_NTF::MAX_DESC_LENGTH] = '\0';
		OutputDebug( "[RTTAgent::OnRTTWAbusingNTF] over %d, %s\n", MSG_RTTW_ABUSING_NTF::MAX_DESC_LENGTH, recvPacket->m_ptszDesc);
	}
	else
	{
        recvPacket->m_ptszDesc[recvPacket->m_wLength] = '\0';
//		OutputDebug( "[RTTAgent::OnRTTWAbusingNTF] %s\n", recvPacket->m_ptszDesc);
	}

    // ĳ���� �̸��� ���ν����� �ϼ��Ǹ� ������ position�� �ִ´�

	Database::Instance()->QueryExternal2( "{call USP_AddAbuseLog(%d, '%s', %d, %d, %d, %d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s')}",
		recvPacket->m_AbuseType,
		recvPacket->m_ptszSubTypeName,
		recvPacket->m_Level,
		recvPacket->m_ServerKey.GetWorldID(),		
		recvPacket->m_ServerKey.GetChannelID(),
		recvPacket->m_ServerKey.GetServerType(),	
		recvPacket->m_ServerKey.GetServerID(),
		recvPacket->m_dwUserGuid,
		recvPacket->m_dwCharGuid,
		recvPacket->m_ptszUserID,
		recvPacket->m_ptszCharName,
		recvPacket->m_ptszIP,
		dateTime, 
		recvPacket->m_ptszDesc );

	//CHAR szMsg[2048] = {0,};
	//sprintf(szMsg,  "{call USP_AddAbuseLog(%d, '%s', %d, %d, %d, %d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s')}\n",
	//	recvPacket->m_AbuseType,
	//	recvPacket->m_ptszSubTypeName,
	//	recvPacket->m_Level,
	//	recvPacket->m_ServerKey.GetWorldID(),		
	//	recvPacket->m_ServerKey.GetChannelID(),
	//	recvPacket->m_ServerKey.GetServerType(),	
	//	recvPacket->m_ServerKey.GetServerID(),
	//	recvPacket->m_dwUserGuid,
	//	recvPacket->m_dwCharGuid,
	//	recvPacket->m_ptszUserID,
	//	recvPacket->m_ptszCharName,
	//	recvPacket->m_ptszIP,
	//	dateTime, 
	//	recvPacket->m_ptszDesc );
	//OutputDebug(szMsg);
}

void RTTAgent::OnRTTWUserMoneyAns( char* buffer, int /*size*/ )
{
	MSG_RTTW_USERMONEY_ANS*	recvPacket = ( MSG_RTTW_USERMONEY_ANS* )buffer;
	
	Database::Instance()->UpdateSupplyMoney(recvPacket->byWorldID,	
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
											recvPacket->ui64JackpotMoney,
											recvPacket->ui64CirculateMoney);
}

void RTTAgent::OnRTTWServerShutdownAns( char* buffer, int /*size*/ )
{
	MSG_RTTW_SERVERSHUTDOWN_ANS*	recvPacket = ( MSG_RTTW_SERVERSHUTDOWN_ANS* )buffer;

	OutputDebug( "[OnRTTWServerShutdownAns] err=%d, serverKey=%d, %d, %d, %d\n",
		recvPacket->dwError,
		recvPacket->m_ServerKey.GetWorldID(),
		recvPacket->m_ServerKey.GetChannelID(),
		recvPacket->m_ServerKey.GetServerType(),
		recvPacket->m_ServerKey.GetServerID() );
	
	DWORD dwError = (recvPacket->dwError==0) ? 1 : 2;
	
	Database::Instance()->QueryExternal2("{call USP_AddPatchCompleteCommand(%I64d, %d)}", recvPacket->i64WopsKey, dwError);
	OutputDebug( "ServerShutdownAns -> Completed.\n" );
	/*
	//  Wops�� �Ϸ� ����(�ϷḸ ������.)
	if( recvPacket->dwError == 0 )
	{
		Wops*	wops;

		//  ����� key�� '��������'�� ��û�� Wops Component�� ã�´�.
		wops = ( Wops* )ManagerWops::Instance()->Find( recvPacket->dwKey );
		if( wops != 0 )
		{
			MSG_RTTW_SERVERSHUTDOWN_ANS	sendPacket;

			sendPacket.dwKey	= 0;
			sendPacket.dwError	= 0;
			sendPacket.m_ServerKey.Set( 0, 0, 0, 0 );

			wops->SendMessage( sendPacket );
		}
	}
	*/
}

void RTTAgent::OnRTTWPatchResultAns( char* buffer, int /*size*/ )
{
	MSG_RTTW_PATCHRESULT_ANS*		recvPacket = ( MSG_RTTW_PATCHRESULT_ANS* )buffer;
	//Wops*							wops;

	OutputDebug( "[OnRTTWPatchResultAns] err=%d, index=%d, key=%d, ip=%s, ver=%d.%d.%d\n",
		recvPacket->dwError, 
		recvPacket->byIndex, 
		recvPacket->dwKey, 
		GetIPAddress(), 
		recvPacket->byVer1, 
		recvPacket->byVer2, 
		recvPacket->byVer3 );

	PatchKey* patchKey = Database::Instance()->GetPatchKey( recvPacket->dwKey );
	
	// ��ġ ���� ���
	// ���� ���� ����
	char lpVersion[256] = {0, };
	sprintf(lpVersion, "%d.%d.%d", recvPacket->byVer1, recvPacket->byVer2, recvPacket->byVer3);

	wchar_t agentIP[20] = {0, };
	MultiByteToWideChar( CP_ACP, 0, GetIPAddress(), -1, agentIP, 20 );
	DWORD dwError = Database::Instance()->SetPatchResult( agentIP, 
														  (patchKey) ? patchKey->bi_wopsKey : ::GetTickCount(), 
														  recvPacket->byServerType, 
														  recvPacket->dwError, 
														  recvPacket->byIndex,
														  lpVersion);
	
	// WOPS���� ������ ��ġ ����� ���Ŀ� ó���� ��� �Ѵ�.
	if(patchKey!=NULL){
		//  ��ġ�� �Ϸ�Ǿ������� �˻��Ѵ�.
		//  ����/���� ���ΰ� ��� RTTA�κ��� ���ŵǸ� �Ϸ�� ó���Ѵ�.
		bool ret = Database::Instance()->IsPatchCompleted(patchKey->ti_WorldID, patchKey->ti_ChannelID, patchKey->ti_ServerType, patchKey->ti_ServerID );
		/*
			[2006.05.05] modify by guiun
			����� wops�� �������� �ʰ� DB procedure call
		*/
		if( ret == true )
		{
			
			dwError = (dwError==0) ? 1 : 2;
			Database::Instance()->QueryExternal2("{call USP_AddPatchCompleteCommand(%I64d, %d)}", patchKey->bi_wopsKey, dwError);
			OutputDebug( "PatchResultAns -> Completed.\n" );

			// ��ġŰ ����

		}
	}
	else{
		//  recvPacket->dwKey == 0 �̶��, RTTA�� ���� ������ RTTG_PATCHRESULT_REQ��
		//  ������ ���� ��Ȳ�̴�.
		OutputDebug( "[OnRTTWPatchResultAns] PatchKey is ZERO!!. Invalid key value.\n" );
	}
}

void RTTAgent::OnRTTWLaunchProcessAns( char* buffer, int /*size*/ )
{
	MSG_RTTW_LAUNCHPROCESS_ANS*		recvPacket = ( MSG_RTTW_LAUNCHPROCESS_ANS* )buffer;
	MSG_RTTW_LAUNCHPROCESS_ANS		sendPacket;
	DWORD			error = 0;

	//	recvPacket->dwServerKey>>24  & 0x000000FF	0x00000005
	//	recvPacket->dwServerKey>>16  & 0x000000FF	0x00000001
	//	recvPacket->dwServerKey>>8   & 0x000000FF	0x00000007
	//	recvPacket->dwServerKey      & 0x000000FF	0x00000001

	bool ret		= Database::Instance()->CheckLaunchProcessStatus(
							(short)( recvPacket->dwServerKey>>24  & 0x000000FF ), 
							(short)( recvPacket->dwServerKey>>16  & 0x000000FF ),
							(short)( recvPacket->dwServerKey>>8   & 0x000000FF ),
							(short)( recvPacket->dwServerKey      & 0x000000FF ),
							recvPacket->dwError, 
							&error );

	if( ret == true )
	{
		DWORD dwError = (error==0) ? 1 : 2;
		
		Database::Instance()->QueryExternal2("{call USP_AddPatchCompleteCommand(%I64d, %d)}", recvPacket->i64WopsKey, dwError);
		OutputDebug( "PatchResultAns -> Completed.\n" );
		/*
		Wops*	wops;

		sendPacket.dwError = error;

		wops = ( Wops* )ManagerWops::Instance()->Find( recvPacket->dwWopsUID );
		if( wops != 0 )
			wops->SendMessage( sendPacket );
		*/
		OutputDebug( "[RTTAgent::OnRTTWLaunchProcessAns] key=%d, completed.\n", recvPacket->dwWopsUID );
	}
}

void RTTAgent::OnRTTRResourceNTF( char* buffer, int /*size*/ )
{
	MSG_RTTR_RESOURCE_NTF*	recvPacket = ( MSG_RTTR_RESOURCE_NTF* )buffer;
	ResourceInfo			info;

	for(DWORD i = 0 ; i < m_ownServerCount ; i++ )
	{
		//  Resource ��Ȳ ����
		info.ti_WorldID				= m_ownServerList[i]->ti_WorldID;
		info.ti_ChannelID			= m_ownServerList[i]->ti_ChannelID;
		info.ti_ServerType			= m_ownServerList[i]->ti_ServerType;
		info.ti_ServerID			= m_ownServerList[i]->ti_ServerID;

		info.i_CpuMaxResource		= 100;
		info.i_CpuUseResource		= recvPacket->dwCPU;
		info.i_MemoryMaxResource	= recvPacket->dwMemoryMax;
		info.i_MemoryUseResource	= recvPacket->dwMemoryUse;
		info.i_HDDMaxResource		= recvPacket->dwHDDMax;
		info.i_HDDUseResource		= recvPacket->dwHDDUse;
		
/*#ifdef _DEBUG
		OutputDebug( "[RTTAgent::OnRTTRResourceNTF](%d %d %d %d) CPU(%d/%d) MEM(%d/%d) HDD(%d/%d)\n", 
			info.ti_WorldID,
			info.ti_ChannelID,
			info.ti_ServerType,
			info.ti_ServerID,
			info.i_CpuUseResource,
			info.i_CpuMaxResource,
			info.i_MemoryUseResource,
			info.i_MemoryMaxResource,
			info.i_HDDUseResource,
			info.i_HDDMaxResource);
#endif*/
		Database::Instance()->UpdateSystemResource( info );
	}
}

void RTTAgent::OnRTTRProcessListReq( char* /*buffer*/, int /*size*/ )
{
	//MSG_RTTR_PROCESSLIST_REQ*	recvPacket = ( MSG_RTTR_PROCESSLIST_REQ* )buffer;
	MSG_RTTR_PROCESSLIST_ANS	sendPacket;
	ServerProcess				processList[20];
	int							count = 0;

	MonitorProcess::Instance()->GetList( m_worldID, m_channelID, m_serverType, m_serverID, this->GetIPAddress(), processList, 20, &count, true );
//	MonitorProcess::Instance()->GetList( m_worldID, m_channelID, m_serverType, m_serverID, processList, 20, &count, true );

	int offset = 0;
	while( offset < count )
	{
		//  �ѹ��� ���� �� �ִ� �ִ� ���� 10���̴�.
		sendPacket.dwCount	= ( count - offset > 10 ? 10 : count - offset );
		//  10�� ���Ŀ� �߻��Ǵ� �����ʹ� '����' flag�̴�.
		sendPacket.dwFlag	= ( offset == 0 ? 0 : 1 );

		for(int i = 0 ; i < sendPacket.dwCount ; i++ )
		{
			wcsncpy( sendPacket.szBinaryName[i], processList[offset].vc_ProcessName, 100 );
			wcsncpy( sendPacket.szAbsolutePath[i], processList[offset].vc_AbsolutePath, 100 );

			offset++;
		}

		SendMessage( sendPacket );
	}
}

void RTTAgent::OnRTTRProcessMonitorNTF( char* buffer, int /*size*/ )
{
	MSG_RTTR_PROCESSMONITOR_NTF*	recvPacket = ( MSG_RTTR_PROCESSMONITOR_NTF* )buffer;

	for(int i = 0 ; i < recvPacket->dwCount ; i++ )
	{
				//  �ش� ���μ����� Alive ���¸� ����Ѵ�.
		MonitorProcess::Instance()->SetProcessStatus( this->GetIPAddress(),
													  recvPacket->szBinaryName[i], 
													  recvPacket->szAbsolutePath[i], 
													  ( recvPacket->dwStatus[i] == 1 ) );

		////  �ش� ���μ����� Alive ���¸� ����Ѵ�.
		//MonitorProcess::Instance()->SetProcessStatus( m_worldID, 
		//	                                          m_channelID, 
		//											  m_serverType, 
		//											  m_serverID,
		//											  recvPacket->szBinaryName[i], 
		//											  recvPacket->szAbsolutePath[i], 
		//											  ( recvPacket->dwStatus[i] == 1 ) );
	}	
}

void RTTAgent::OnRTTRWebUrlReq( char* buffer, int size )
{
	MSG_RTTR_WEB_URL_ANS SendPacket;

	WebServerRequestTestURL wzTempUrl[10] = {0 ,};
	
	m_ErrorIndexCount = Database::Instance()->GetURLList( m_worldID, m_channelID, m_serverType, m_serverID, wzTempUrl );

	if( !m_ErrorIndexCount )
	{
		SendPacket.bExist = false;
		SendMessage( SendPacket );
//		OutputDebug("[RTTAgent::OnRTTRWebUrlReq] Send URL Not exist To %s\n", GetIPAddress() );
	}
	else
	{
		for( DWORD dw = 0 ; dw < m_ErrorIndexCount ; dw++ )
		{
			SendPacket.bExist = true;
			SendPacket.dwURLIndex = dw;
			wcscpy( SendPacket.szURL, wzTempUrl[dw].vc_URL );
			SendMessage( SendPacket );

			memset( SendPacket.szURL, 0, sizeof(wchar_t) * MAX_URL );	// SendPacket �����ʱ�ȭ

			OutputDebug("[RTTAgent::OnRTTRWebUrlReq] Send URL To %s\n", GetIPAddress() );
		}
	}
}

void RTTAgent::OnRTTRWebRequestNTF( char* buffer, int size )
{
	MSG_RTTR_WEB_REQUEST_NTF* pRecvPacket = (MSG_RTTR_WEB_REQUEST_NTF*)buffer;

	if( HTTP_STATUS_OK == pRecvPacket->dwStatusCode )
	{
		// �ش� URL�� ��ֵ�� ��ȣ�� ������ ������ �Ǿ����� �˸�
		if( m_ErrorIndexList[pRecvPacket->dwURLIndex].dwPageFaultParentHistoryIndex )
		{
			// ���ٸ� ����� �ϴ°� �ƴϰ� �μ��� �ѱ� �������� �ʹ� ��...-_-;;
			DWORD dwParentIndex = m_ErrorIndexList[pRecvPacket->dwURLIndex].dwPageFaultParentHistoryIndex;

			Database::Instance()->QueryWebPageRequestState( m_worldID, m_channelID, m_serverType, m_serverID, dwParentIndex, false );	// true == �������, false == �������
            m_ErrorIndexList[pRecvPacket->dwURLIndex].dwPageFaultParentHistoryIndex = 0;

			OutputDebug("[RTTAgent::OnRTTRWebRequestNTF] Page Fault Error Restore Inserted, IP : %s\n", GetIPAddress() );
		}

		if( m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex )
		{
			DWORD dwParentIndex = m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex;

   			Database::Instance()->QueryConnectRequestState( m_worldID, m_channelID, m_serverType, m_serverID, dwParentIndex, false, 0 );	// true == �������, false == �������
			m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex = 0;

			OutputDebug("[RTTAgent::OnRTTRWebRequestNTF] Connect Error Restore Inserted, IP : %s\n", GetIPAddress() );
		}
	}
	else
	{
		// Page Fault Error
		if( HTTP_STATUS_NOT_FOUND == pRecvPacket->dwStatusCode )
		{
			// Page Fault ������ ����Ѵ�(�����Ǳ� ������ ���� 1���� ����Ѵ�)
			if( !m_ErrorIndexList[pRecvPacket->dwURLIndex].dwPageFaultParentHistoryIndex )
			{
				m_ErrorIndexList[pRecvPacket->dwURLIndex].dwPageFaultParentHistoryIndex = Database::Instance()->QueryWebPageRequestState( m_worldID, m_channelID, m_serverType, m_serverID, 0, true );	// true == �������, false == �������
				OutputDebug("[RTTAgent::OnRTTRWebRequestNTF] Page Fault Error Inserted : %d, IP : %s\n", m_ErrorIndexList[pRecvPacket->dwURLIndex].dwPageFaultParentHistoryIndex, GetIPAddress() );
			}

			// Page Fault -> Ŀ�ؼ� ��� ������ 2������ ��ְ� ���������� ������� 
			// Ŀ�ؼ� ��ִ� ������ �ǰ� Page Fault�� ������ ���� �ʾ������ Ŀ�ؼ� ��ָ� ���� ��Ų��.
			if( m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex )
			{
				DWORD dwParentIndex = m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex;
																																		// Page Fault�� �������� �ʾ����Ƿ� �÷��׸� 0 �ƴѼ��� �Ѱ��ش�
				Database::Instance()->QueryConnectRequestState( m_worldID, m_channelID, m_serverType, m_serverID, dwParentIndex, false, 1 );	// true == �������, false == �������
				m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex = 0;

				OutputDebug("[RTTAgent::OnRTTRWebRequestNTF] ( HTTP_STATUS_NOT_FOUND ) Connect Error Restore Inserted, IP : %s\n", GetIPAddress() );
			}
		}

		// �� ���� ���� �Ұ� ����
		if( ERROR_INTERNET_TIMEOUT == pRecvPacket->dwStatusCode || ERROR_INTERNET_CANNOT_CONNECT == pRecvPacket->dwStatusCode )
		{
			if( !m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex )
			{
				m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex = Database::Instance()->QueryConnectRequestState( m_worldID, m_channelID, m_serverType, m_serverID, 0, true );	// true == �������, false == �������
				OutputDebug("[RTTAgent::OnRTTRWebRequestNTF] Connect Error Inserted : %d, IP : %s\n", m_ErrorIndexList[pRecvPacket->dwURLIndex].dwPageFaultParentHistoryIndex, GetIPAddress() );
			}
		}

		// �̵� ���� �ƴ� ������...Ŀ�ؼ� �̻����� ó���ϰ� �����ڵ带 �α׷� �����.
		if( (HTTP_STATUS_NOT_FOUND != pRecvPacket->dwStatusCode) && (ERROR_INTERNET_TIMEOUT != pRecvPacket->dwStatusCode) && (ERROR_INTERNET_CANNOT_CONNECT == pRecvPacket->dwStatusCode) )
		{
			if( !m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex )
			{
				m_ErrorIndexList[pRecvPacket->dwURLIndex].dwConnectParentHistoryIndex = Database::Instance()->QueryConnectRequestState( m_worldID, m_channelID, m_serverType, m_serverID, 0, true );	// true == �������, false == �������
				OutputDebug("[RTTAgent::OnRTTRWebRequestNTF] Unknown HTTP Error Inserted : %d, ErrorCode : %d, IP : %s\n", m_ErrorIndexList[pRecvPacket->dwURLIndex].dwPageFaultParentHistoryIndex, pRecvPacket->dwStatusCode, GetIPAddress() );
			}
		}
	}

//	OutputDebug("[RTTAgent::OnRTTRWebRequestNTF] Recv WebRequest_NTF StateCode : %d\n", pRecvPacket->dwStatusCode);
}

void RTTAgent::OnRTTWFuncrestrictReloadAns( char* buffer, int /*size*/ )
{
	MSG_RTTW_FUNCRESTRICT_RELOAD_ANS*	recvPacket = ( MSG_RTTW_FUNCRESTRICT_RELOAD_ANS* )buffer;
	
	//  ����� key�� '��������'�� ��û�� Wops Component�� ã�´�.
	Wops* pWops = ( Wops* )ManagerWops::Instance()->Find( recvPacket->dwWopsKey );
	if( pWops != 0 )
	{
		pWops->SendMessage( *recvPacket );
		OutputDebug( "[RTTAgent::OnRTTWFuncrestrictReloadAns] Send To Wops, Result : %d\n", recvPacket->dwResult);
	}
	else
	{
		OutputDebug( "[RTTAgent::OnRTTWFuncrestrictReloadAns] not found wops \n");
	}
}

void RTTAgent::OnRTTAuthIPListReloadAns( char* buffer, int /*size*/ )
{
	MSG_RTTW_AUTHIPLIST_RELOAD_ANS*	recvPacket = ( MSG_RTTW_AUTHIPLIST_RELOAD_ANS* )buffer;
	//  ����� key�� '��������'�� ��û�� Wops Component�� ã�´�.
	Wops* pWops = ( Wops* )ManagerWops::Instance()->Find( recvPacket->dwWopsKey );
	
	if( pWops != 0 )
	{
		pWops->SendMessage( *recvPacket );
		OutputDebug( "[RTTAgent::OnRTTAuthIPListReloadAns] Send To Wops, Result : %d\n", recvPacket->dwResult);
	}
	else
	{
		OutputDebug( "[RTTAgent::OnRTTAuthIPListReloadAns] not found wops \n");
	}
}

void RTTAgent::OnRTTGameVersionUpdateAns( char* buffer, int /*size*/ )
{
	MSG_RTTW_GAMEVERSION_UPDATE_ANS*	recvPacket = ( MSG_RTTW_GAMEVERSION_UPDATE_ANS* )buffer;
	//  ����� key�� '��������'�� ��û�� Wops Component�� ã�´�.
	Wops* pWops = ( Wops* )ManagerWops::Instance()->Find( recvPacket->dwWopsKey );
	
	if( pWops != 0 )
	{
		pWops->SendMessage( *recvPacket );
		OutputDebug( "[RTTAgent::OnRTTGameVersionUpdateAns] Send To Wops, Result : %d\n", recvPacket->dwResult);
	}
	else
	{
		OutputDebug( "[RTTAgent::OnRTTGameVersionUpdateAns] not found wops \n");
	}
}

void RTTAgent::OnRTTGameStatisticsAns( char* buffer, int /*size*/ )
{
	MSG_RTTW_GAMESTATISTICS_ANS*	recvPacket = ( MSG_RTTW_GAMESTATISTICS_ANS* )buffer;
	Database::Instance()->UpdateGameStatistics((short)recvPacket->dwWorldID, 
		                                       (short)recvPacket->dwChannelID, 
											   (short)recvPacket->dwServerID, 
											   (short)recvPacket->dwServerType, 
											   recvPacket->ui64Statistics);
//	OutputDebug( "[RTTAgent::OnRTTGameStatisticsAns] Recv Statistics\n");
	return;
}

//void RTTAgent::OnRTTPatternReloadAns( char* buffer, int /*size*/ )
//{
//	MSG_RTTW_PATTERN_RELOAD_ANS*	recvPacket = ( MSG_RTTW_PATTERN_RELOAD_ANS* )buffer;
//	//  ����� key�� '��������'�� ��û�� Wops Component�� ã�´�.
//	Wops* pWops = ( Wops* )ManagerWops::Instance()->Find( recvPacket->dwWopsKey );
//	
//	if( pWops != 0 )
//	{
//		pWops->SendMessage( *recvPacket );
//	}
//	else
//	{
//		OutputDebug( "[RTTAgent::OnRTTPatternReloadAns] not found wops \n");
//	}
//}

void RTTAgent::OnRTTRHeartBeatNTF( char* /*buffer*/, int /*size*/ )
{
	m_tickAlive = ::GetTickCount();
#ifdef _DEBUG
	//OutputDebug( "[RTTAgent::OnRTTRHeartBeatNTF] (%s)\n", GetIPAddress() );
#endif
}

void RTTAgent::OnRTTWArmorkitDispatchANS( char* buffer, int /*size*/ )
{
	MSG_RTTW_ARMORKIT_DISPATCH_ANS*	recvPacket = ( MSG_RTTW_ARMORKIT_DISPATCH_ANS* )buffer;
	
	//  ����� key�� '��������'�� ��û�� Wops Component�� ã�´�.
	Wops* pWops = ( Wops* )ManagerWops::Instance()->Find( recvPacket->dwWopsKey );
	if( pWops != 0 )
	{
		pWops->SendMessage( *recvPacket );

		OutputDebug( "[RTTAgent::OnRTTWArmorkitDispatchANS] send to wops(%d) \n", recvPacket->dwResult);
	}
	else
	{
		OutputDebug( "[RTTAgent::OnRTTWArmorkitDispatchANS] not found wops(%d) \n", recvPacket->dwWopsKey);
	}
}

void RTTAgent::OnRTTWChangeGuildNameAns(char* buffer, int size)
{
	eXtreme::Stream			stream( buffer, size );
	MSG_RTTW_CHANGE_GUILDNAME_ANS* recvPacket = ( MSG_RTTW_CHANGE_GUILDNAME_ANS* )( buffer + 12 );
	DWORD					dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A ������ ����� ���� ���Ƿ� ���Ե� ����� �о���δ�.
	stream >> dummySize >> dummyID >> key;

	//  ����� key�� '���� ����'�� ��û�� Wops Component�� ã�´�.
	Wops* pWops = ( Wops* )ManagerWops::Instance()->Find( key );
	if( pWops != 0 )
	{
		pWops->SendMessage( *recvPacket );
		OutputDebug( "[RTTAgent::OnRTTWChangeGuildNameAns] Send to Wops(%d) \n", recvPacket->dwResult);
	}
	else
	{
		OutputDebug( "[RTTAgent::OnRTTWChangeGuildNameAns] Not Found Wops(%d) \n", recvPacket->dwResult);
	}
}

void RTTAgent::OnRTTWSendMessageAns(char* buffer, int size)
{
	eXtreme::Stream				stream( buffer, size );
	MSG_RTTW_SEND_MESSAGE_ANS*	recvPacket = ( MSG_RTTW_SEND_MESSAGE_ANS* )( buffer + 12 );
	DWORD					dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A ������ ����� ���� ���Ƿ� ���Ե� ����� �о���δ�.
	stream >> dummySize >> dummyID >> key;

	//  ����� key�� '���� ����'�� ��û�� Wops Component�� ã�´�.
	Wops* pWops = ( Wops* )ManagerWops::Instance()->Find( key );
	if( pWops != 0 )
	{
		pWops->SendMessage( *recvPacket );
		OutputDebug( "[RTTAgent::OnRTTWSendMessageAns] Send to Wops(Count : %d) \n", recvPacket->dwCount);
	}
	else
	{
		OutputDebug( "[RTTAgent::OnRTTWSendMessageAns] Not Found Wops(Count : %d) \n", recvPacket->dwCount);
	}
}

// �߱� �� �󿡼� ���� ���� ���� ��û ���
void RTTAgent::OnRTTHDropUserAns( char* buffer, int size )
{
	eXtreme::Stream			stream( 4096 );
	MSG_RTTH_DROPUSER_ANS*	recvPacket = ( MSG_RTTH_DROPUSER_ANS* )( buffer + 12 );
	DWORD					dummySize = 0, dummyID = 0, key = 0;

	//  RTTS-A ������ ����� ���� ���Ƿ� ���Ե� ����� �о���δ�.
	stream.Write( buffer, size );
	stream >> dummySize >> dummyID >> key;

	OutputDebug("[RTTAgent::OnRTTHDropUserAns] Session UID : %d\n", key );

	Web* webComponent = ( Web* )ManagerWeb::Instance()->Find( key );
	if( webComponent == 0 )
	{
		OutputDebug( "[RTTAgent::OnRTTHDropUserAns] Web cannot found, Result:%d\n", recvPacket->dwError);
		return;
	}

	webComponent->SendPacket( recvPacket, sizeof( *recvPacket ) );

	OutputDebug( "[RTTAgent::OnRTTHDropUserAns] Result:%d\n", recvPacket->dwError);
}