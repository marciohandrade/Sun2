#include "StdAfx.h"
#include ".\tempuser.h"
#include "UserFactory.h"
#include "WaitingUserList.h"
#include "Channel.h"
#include "ChannelManager.h"
#include <Protocol_CW.h>
#include <PacketStruct_CW.h>

// 암호화 때문에 include
#include <Version.h>
#include "WorldServerInfoParser.h"
#include "UserManager.h"

#include "PacketHandler.h"

#include <PacketStruct_DW.h>
#include "ServerSessionManager.h"
#include "ServerSession.h"

#include <PacketStruct_AW.h>

#include "WorldGuildManager.h"

TempUser::TempUser(void)
{
}

TempUser::~TempUser(void)
{
}

VOID TempUser::Init()
{
	User::Init();
	m_bFirstTime = TRUE;
	m_dwAuthID = 0;
	m_bUseCryptography = FALSE;
	SetVoiceChat( FALSE );
}

VOID TempUser::OnAccept( DWORD dwNetworkIndex )
{
	User::OnAccept( dwNetworkIndex );
	MSG_CW_ENTER_READY_BRD	sendMsg;

	SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
}

BOOL TempUser::SendPacket( BYTE* pMsg, WORD wSize )
{
	union
	{
		MSG_BASE*			pBASE;
		MSG_BASE_INTERNAL*	pINTERNAL;
		BYTE*				pBYTE;
	};

	pBASE = PACKET_CONTROL::GetPacketPtr_BASE_A2C();
	if( pBYTE != pMsg )
		CopyMemory( pBYTE, pMsg, (SIZE_T)wSize );

	pBYTE = PACKET_CONTROL::GetPacketPtr_A2C();

	wSize = ((WORD)wSize-(WORD)sizeof(DWORD));
	ASSERT( wSize && wSize < 16000 );
	if( wSize && wSize < 16000 )
	{
		return Send( pBYTE, wSize );
	}
	// --error
	return FALSE;
}

VOID TempUser::OnRecv( BYTE *pMsg, WORD wSize )
{
	union
	{
		MSG_BASE*			pBASE;
		MSG_BASE_INTERNAL*	pINTERNAL;
		BYTE*				pBYTE;
	};

	pBYTE = PACKET_CONTROL::GetPacketPtr_C2A();
	if( pBYTE != pMsg )
		CopyMemory( pBYTE, pMsg, (SIZE_T)wSize );
	pBASE = PACKET_CONTROL::GetPacketPtr_BASE_C2A();
	pMsg = pBYTE;
	wSize += sizeof(DWORD);

	if( WorldServerInfoParser::Instance()->IsCryptable() )
	{
		const WORD	wConnType = MAKEWORD(CW_CONNECTION, CW_ENTER_SERVER_SYN);
		const WORD	wCryptType = MAKEWORD(CW_CRYPTOGRAPHY, CW_CRYPTOGRAPHY_ACK);

		MSG_BASE	*pBaseMsg = (MSG_BASE*)pMsg;
		WORD		wMsgType = MAKEWORD(pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol);

		if( wMsgType == wConnType )
		{
#ifdef _DEBUG
			assert( m_bFirstTime );
#else
			if( !m_bFirstTime )
			{
				MessageOut(eCRITICAL_LOG, "First Protocol Error(%d)(%d) - 1", pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol );
				Logout();;
				return;
			}
#endif
/*
			if( pBaseMsg->m_byProtocol != CW_ENTER_SERVER_SYN )
			{
				MessageOut(eCRITICAL_LOG, "최초 프로토콜 오류(%d)(%d)", pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol );
				Logout();;
				return;
			}
*/
			m_bFirstTime = FALSE;

			MSG_CW_ENTER_SERVER_SYN *pRecvMsg = (MSG_CW_ENTER_SERVER_SYN*)pMsg;

			m_dwAuthID = pRecvMsg->dwAuthUserID;

			// CRYPT_KEY 처리
			User *pActiveUser = WaitingUserList::Instance()->FindUserWithAuthID( pRecvMsg->dwAuthUserID );

			// WaitingUserList에 없는 경우 접속을 끊고 리턴
			if( !pActiveUser )
			{
				MessageOut(eCRITICAL_LOG, "Not in WaitingUserList User Disconnect(AuthID:%u)", pRecvMsg->dwAuthUserID );
				Disconnect();	// 이 경우에 Logout을 하면 유저 집계에 문제가 생길 수 있다
				return;
			}

			// IP 확인 후 틀리면 접속을 끊고 리턴	
			if( strncmp( GetIP(), pActiveUser->GetAuthIP(), IP_MAX_LEN ) != 0 )
			{
                MessageOut(eCRITICAL_LOG, "Warning: Connected User's IP is different from IP Recved from AgentServer[Guid:%u][IP:%s][A:%u][AIP:%s]",
					pActiveUser->GetGUID(), GetIP(), pActiveUser->GetAuthID(), pActiveUser->GetAuthIP());
//				// 대기열에서 삭제
//				WaitingUserList::Instance()->RemoveUser( pActiveUser->GetAuthID() );
//				Logout();;
//				return;
			}

			// <Cryptography>
			//   이곳에 SN설정과 암호화 프로토콜을 결정하기로 한다.
			if( WorldServerInfoParser::Instance()->IsCryptable() )
			{
				BYTE keySTREAM[280];
				MSG_CW_CRYPTOGRAPHY_KEY *pMsgKEY = new (keySTREAM) MSG_CW_CRYPTOGRAPHY_KEY();
				MSG_CW_CRYPTOGRAPHY_KEY& cryptMsg = *pMsgKEY;
				//MSG_CW_CRYPTOGRAPHY_KEY cryptMsg;

				BYTE key = InitCrypt( C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO );
			//	BYTE key = GetSendSNKey();
				cryptMsg.SetSNKey( key );

				key = GetCryptAlgorithmInfo( cryptMsg.GetCryptStream() );
				cryptMsg.SetCryptInfo( key );

				SendPacket( (BYTE*)&cryptMsg, sizeof(keySTREAM) /*sizeof(MSG_CW_CRYPTOGRAPHY_KEY)*/ );

				// 타임아웃 체크 - 어차피 ACK 오기 전에는 TempUser상태에서 더 이상 진행이 되지 않으므로 이 체크가 필요없다
//				UserManager::Instance()->AddCryptoUser( this );
			//	pActiveUser->SetCryptography( TRUE );
				NextRoundSNKey4Send();
			}
			// <END>
		}
		else if( wMsgType == wCryptType )
		{
//	#ifdef _DEBUG
//				assert( !m_bFirstTime );
//	#else
//				if( m_bFirstTime )
//				{
//					MessageOut(eCRITICAL_LOG,   "암호화 프로토콜 순서 오류(%d)(%d)", pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol );
//					Disconnect();
//					return;
//				}
//	#endif
/*
			if( pBaseMsg->m_byProtocol != CW_CRYPTOGRAPHY_ACK )
			{
				MessageOut(eCRITICAL_LOG, "최초 암호화 프로토콜 오류(%d)(%d)", pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol );
				Logout();;
				return;
			}
*/
			User *pActiveUser = WaitingUserList::Instance()->FindUserWithAuthID( m_dwAuthID );

			// WaitingUserList에 없는 경우 접속을 끊고 리턴
			if( !pActiveUser )
			{
				MessageOut(eCRITICAL_LOG, "Not in WaitingUserList User Disconnect(AuthID:%d)", m_dwAuthID );
				Disconnect();
				return;
			}

			pActiveUser->CopyCrypt( m_sCryptBack );
			
			// 액티브 유져를 WaitingUserList에서 제거
			WaitingUserList::Instance()->RemoveUser( m_dwAuthID );

			// 액티브 유져로 네트웍 오브젝트 교체
			Redirect( pActiveUser );
			pActiveUser->OnRedirect();

            // TempUser 반환
            UserFactory::Instance()->Free( this );

			eZONETYPE	eZoneType = pActiveUser->GetStatus();
			DWORD		dwZoneKey = 1;					// CharSelect Zonekey = 1

			if( pActiveUser->GetZoneKey() )
			{
				dwZoneKey = pActiveUser->GetZoneKey();	// 유저 접속 복구하는 경우에 한해 복구용 ZoneKey가 들어간다

				// 친구 목록 요청 - 캐릭터 선택이 된 것을 접속으로 본다
				pActiveUser->InitFriendListInit();	// 요청하기 전에 초기화해 주는 쎈쓰!!
                pActiveUser->InitGroupInfo();

				ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
				if( !pGameDBProxy )
				{
					MessageOut( eCRITICAL_LOG, "Restore User : Friend_NewList Error - No GameDBProxy");
					return;
				}

				MSG_DW_FRIEND_GETLIST_SYN	sendMsg;
				sendMsg.m_CharGuid = pActiveUser->GetCharGuid();
				pGameDBProxy->Send( (BYTE*)&sendMsg, sendMsg.GetSize() );

				// 복구인 경우 Chat Block 확인 요청도 전송
				MSG_DW_GM_CHATBLOCKTIME_REQ	reqMsg;
				reqMsg.m_dwKey = pActiveUser->GetCharGuid();
				pGameDBProxy->Send( (BYTE*)&reqMsg, sizeof(reqMsg) );
				MessageOut( eFULL_LOG, "[CharGuid:%u]ChatBlock Time Req", pActiveUser->GetCharGuid() );

				MSG_DW_STATUS_INTERCEPTION_REQ msgREQ;
				msgREQ.m_dwKey		= pActiveUser->GetGUID();
				msgREQ.m_CharGuid	= pActiveUser->GetCharGuid();
				pGameDBProxy->Send((BYTE*)&msgREQ, sizeof(msgREQ));
                //__KR_001355_20090608_GROUPINGSYSTEM_GW
                MSG_DW_GROUP_LIST_REQ msgList;//그룹 맴버 리스트 요청 메세지
                msgList.m_CharGUID = pActiveUser->GetCharGuid();
                pGameDBProxy->Send( (BYTE*)&msgList, sizeof(msgList) );
			}

			pActiveUser->SetStatus( eZONETYPE_MAX );
			pActiveUser->SetZoneKey( 0 );

			// 채널에 유져 추가
			Channel *pChannel = ChannelManager::Instance()->GetChannel( pActiveUser->GetChannelID() );
			if( pChannel )
			{
				//pChannel->AddUser( pActiveUser, US_IN_CHARSCENE, 0 );
				pChannel->AddUser( pActiveUser, eZoneType, dwZoneKey );
		//		pChannel->AddUser( pActiveUser, eZONETYPE_CHARSELECT, 0 );

				MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_SYN AWItemMsg;
				AWItemMsg.m_dwKey = pActiveUser->GetGUID();

				ServerSession *pAgentServer = ServerSessionManager::Instance()->FindAgentServer(pActiveUser->GetChannelID());
				if(pAgentServer)	
				{					
					pAgentServer->Send((BYTE*)&AWItemMsg, sizeof(MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_SYN));
				}

				MessageOut(eFULL_LOG, "User Connect Success! Channel(%d) Guid(%d) IP(%s) Status(%u)",
					pActiveUser->GetChannelID(), pActiveUser->GetGUID(), pActiveUser->GetAuthIP(), pActiveUser->GetStatus() );
			}
			else
			{
				pActiveUser->Disconnect();
				MessageOut(eCRITICAL_LOG, "Fatal error: User Connect Channel(%d) Failed!", pActiveUser->GetChannelID() );
				return;
			}

			if( WorldServerInfoParser::Instance()->IsCryptable() )
			{
				// 클라이언트한테 이제부터 그 키 사용하라고 알려주고 서버도 세팅한다. 이 세팅이 끝난 다음 보내고 받는 것은 무조건 암호화된 내용들이다
				MSG_CW_CRYPTOGRAPHY_CMD	sendMsg;
				SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CRYPTOGRAPHY_CMD) );
				pActiveUser->SetCrypt();	// 이제부터 암호화 쓴다!
				MessageOut( eFULL_LOG, "[GUID:%u] Crypt Set", pActiveUser->GetGUID() );
				return;
			}
		}
		else
		{
			MessageOut( eMIDDLE_LOG, "First Protocol Error(%d)(%d) - 2", pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol );
//				Disconnect();
			return;
		}
	}
	else	// 암호화 설정 안 돼있으면
	{
#ifdef _DEBUG
		assert( m_bFirstTime );
#else
		MSG_BASE	*pBaseMsg = (MSG_BASE*)pMsg;

		if( !m_bFirstTime )
		{
			MessageOut( eCRITICAL_LOG, "Not First Protocol!!(%u)(%u)", pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol );
			return;
		}
#endif
		MSG_CW_ENTER_SERVER_SYN *pRecvMsg = (MSG_CW_ENTER_SERVER_SYN*)pMsg;

		// 프로토콜이 잘못된 경우 리턴
		if( pRecvMsg->m_byCategory != CW_CONNECTION || pRecvMsg->m_byProtocol != CW_ENTER_SERVER_SYN )
		{
			MessageOut(eCRITICAL_LOG,   "First Protocol Error(%d)(%d)", pRecvMsg->m_byCategory, pRecvMsg->m_byProtocol );
//			Disconnect();
			return;
		}

		m_bFirstTime = FALSE;

		User *pActiveUser = WaitingUserList::Instance()->FindUserWithAuthID( pRecvMsg->dwAuthUserID );

		// WaitingUserList에 없는 경우 접속을 끊고 리턴
		if( !pActiveUser )
		{
			MessageOut(eCRITICAL_LOG,   "Not in WaitingUserList User Disconnect(AuthID:%d)", pRecvMsg->dwAuthUserID );
			Disconnect();
			return;
		}

		// IP 확인 후 틀리면 접속을 끊고 리턴	
		if( strncmp( GetIP(), pActiveUser->GetAuthIP(), IP_MAX_LEN ) != 0 )
		{
            MessageOut(eCRITICAL_LOG,   "Warning: Connected User's IP is different from IP Recved from AgentServer[Guid:%u][IP:%s][A:%u][AIP:%s]",
				pActiveUser->GetGUID(), GetIP(), pActiveUser->GetAuthID(), pActiveUser->GetAuthIP());
//			Disconnect();
			return;
		}
		
		// 액티브 유져를 WaitingUserList에서 제거
		WaitingUserList::Instance()->RemoveUser( pRecvMsg->dwAuthUserID );

		// 액티브 유져로 네트웍 오브젝트 교체
		Redirect( pActiveUser );
		pActiveUser->OnRedirect();

        // TempUser 반환
        UserFactory::Instance()->Free( this );

		eZONETYPE	eZoneType = pActiveUser->GetStatus();
		DWORD		dwZoneKey = 1;					// CharSelect Zonekey = 1

		if( pActiveUser->GetZoneKey() )
		{
			dwZoneKey = pActiveUser->GetZoneKey();	// 유저 접속 복구하는 경우에 한해 복구용 ZoneKey가 들어간다
		}

		pActiveUser->SetStatus( eZONETYPE_MAX );
		pActiveUser->SetZoneKey( 0 );

		// 채널에 유져 추가
		Channel *pChannel = ChannelManager::Instance()->GetChannel( pActiveUser->GetChannelID() );
		if( pChannel )
		{
			//pChannel->AddUser( pActiveUser, US_IN_CHARSCENE, 0 );
			pChannel->AddUser( pActiveUser, eZoneType, dwZoneKey );
	//		pChannel->AddUser( pActiveUser, eZONETYPE_CHARSELECT, 0 );

			MessageOut(eFULL_LOG,   "User Connect Success! 채널(%d) Guid(%d) IP(%s) Status(%u)",
				pActiveUser->GetChannelID(), pActiveUser->GetGUID(), pActiveUser->GetAuthIP(), pActiveUser->GetStatus() );
		}
		else
		{
			pActiveUser->Disconnect();
			MessageOut(eCRITICAL_LOG,   "Fatal error: User Connect Channel(%d)Failed!", pActiveUser->GetChannelID() );
			return;
		}
	}
}