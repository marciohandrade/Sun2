#include "StdAfx.h"
#include ".\tempuser.h"
#include "UserFactory.h"
#include "WaitingUserList.h"
#include "Channel.h"
#include "ChannelManager.h"
#include <Protocol_CW.h>
#include <PacketStruct_CW.h>

// ��ȣȭ ������ include
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
				MessageOut(eCRITICAL_LOG, "���� �������� ����(%d)(%d)", pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol );
				Logout();;
				return;
			}
*/
			m_bFirstTime = FALSE;

			MSG_CW_ENTER_SERVER_SYN *pRecvMsg = (MSG_CW_ENTER_SERVER_SYN*)pMsg;

			m_dwAuthID = pRecvMsg->dwAuthUserID;

			// CRYPT_KEY ó��
			User *pActiveUser = WaitingUserList::Instance()->FindUserWithAuthID( pRecvMsg->dwAuthUserID );

			// WaitingUserList�� ���� ��� ������ ���� ����
			if( !pActiveUser )
			{
				MessageOut(eCRITICAL_LOG, "Not in WaitingUserList User Disconnect(AuthID:%u)", pRecvMsg->dwAuthUserID );
				Disconnect();	// �� ��쿡 Logout�� �ϸ� ���� ���迡 ������ ���� �� �ִ�
				return;
			}

			// IP Ȯ�� �� Ʋ���� ������ ���� ����	
			if( strncmp( GetIP(), pActiveUser->GetAuthIP(), IP_MAX_LEN ) != 0 )
			{
                MessageOut(eCRITICAL_LOG, "Warning: Connected User's IP is different from IP Recved from AgentServer[Guid:%u][IP:%s][A:%u][AIP:%s]",
					pActiveUser->GetGUID(), GetIP(), pActiveUser->GetAuthID(), pActiveUser->GetAuthIP());
//				// ��⿭���� ����
//				WaitingUserList::Instance()->RemoveUser( pActiveUser->GetAuthID() );
//				Logout();;
//				return;
			}

			// <Cryptography>
			//   �̰��� SN������ ��ȣȭ ���������� �����ϱ�� �Ѵ�.
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

				// Ÿ�Ӿƿ� üũ - ������ ACK ���� ������ TempUser���¿��� �� �̻� ������ ���� �����Ƿ� �� üũ�� �ʿ����
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
//					MessageOut(eCRITICAL_LOG,   "��ȣȭ �������� ���� ����(%d)(%d)", pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol );
//					Disconnect();
//					return;
//				}
//	#endif
/*
			if( pBaseMsg->m_byProtocol != CW_CRYPTOGRAPHY_ACK )
			{
				MessageOut(eCRITICAL_LOG, "���� ��ȣȭ �������� ����(%d)(%d)", pBaseMsg->m_byCategory, pBaseMsg->m_byProtocol );
				Logout();;
				return;
			}
*/
			User *pActiveUser = WaitingUserList::Instance()->FindUserWithAuthID( m_dwAuthID );

			// WaitingUserList�� ���� ��� ������ ���� ����
			if( !pActiveUser )
			{
				MessageOut(eCRITICAL_LOG, "Not in WaitingUserList User Disconnect(AuthID:%d)", m_dwAuthID );
				Disconnect();
				return;
			}

			pActiveUser->CopyCrypt( m_sCryptBack );
			
			// ��Ƽ�� ������ WaitingUserList���� ����
			WaitingUserList::Instance()->RemoveUser( m_dwAuthID );

			// ��Ƽ�� ������ ��Ʈ�� ������Ʈ ��ü
			Redirect( pActiveUser );
			pActiveUser->OnRedirect();

            // TempUser ��ȯ
            UserFactory::Instance()->Free( this );

			eZONETYPE	eZoneType = pActiveUser->GetStatus();
			DWORD		dwZoneKey = 1;					// CharSelect Zonekey = 1

			if( pActiveUser->GetZoneKey() )
			{
				dwZoneKey = pActiveUser->GetZoneKey();	// ���� ���� �����ϴ� ��쿡 ���� ������ ZoneKey�� ����

				// ģ�� ��� ��û - ĳ���� ������ �� ���� �������� ����
				pActiveUser->InitFriendListInit();	// ��û�ϱ� ���� �ʱ�ȭ�� �ִ� �뾲!!
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

				// ������ ��� Chat Block Ȯ�� ��û�� ����
				MSG_DW_GM_CHATBLOCKTIME_REQ	reqMsg;
				reqMsg.m_dwKey = pActiveUser->GetCharGuid();
				pGameDBProxy->Send( (BYTE*)&reqMsg, sizeof(reqMsg) );
				MessageOut( eFULL_LOG, "[CharGuid:%u]ChatBlock Time Req", pActiveUser->GetCharGuid() );

				MSG_DW_STATUS_INTERCEPTION_REQ msgREQ;
				msgREQ.m_dwKey		= pActiveUser->GetGUID();
				msgREQ.m_CharGuid	= pActiveUser->GetCharGuid();
				pGameDBProxy->Send((BYTE*)&msgREQ, sizeof(msgREQ));
                //__KR_001355_20090608_GROUPINGSYSTEM_GW
                MSG_DW_GROUP_LIST_REQ msgList;//�׷� �ɹ� ����Ʈ ��û �޼���
                msgList.m_CharGUID = pActiveUser->GetCharGuid();
                pGameDBProxy->Send( (BYTE*)&msgList, sizeof(msgList) );
			}

			pActiveUser->SetStatus( eZONETYPE_MAX );
			pActiveUser->SetZoneKey( 0 );

			// ä�ο� ���� �߰�
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
				// Ŭ���̾�Ʈ���� �������� �� Ű ����϶�� �˷��ְ� ������ �����Ѵ�. �� ������ ���� ���� ������ �޴� ���� ������ ��ȣȭ�� ������̴�
				MSG_CW_CRYPTOGRAPHY_CMD	sendMsg;
				SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CRYPTOGRAPHY_CMD) );
				pActiveUser->SetCrypt();	// �������� ��ȣȭ ����!
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
	else	// ��ȣȭ ���� �� ��������
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

		// ���������� �߸��� ��� ����
		if( pRecvMsg->m_byCategory != CW_CONNECTION || pRecvMsg->m_byProtocol != CW_ENTER_SERVER_SYN )
		{
			MessageOut(eCRITICAL_LOG,   "First Protocol Error(%d)(%d)", pRecvMsg->m_byCategory, pRecvMsg->m_byProtocol );
//			Disconnect();
			return;
		}

		m_bFirstTime = FALSE;

		User *pActiveUser = WaitingUserList::Instance()->FindUserWithAuthID( pRecvMsg->dwAuthUserID );

		// WaitingUserList�� ���� ��� ������ ���� ����
		if( !pActiveUser )
		{
			MessageOut(eCRITICAL_LOG,   "Not in WaitingUserList User Disconnect(AuthID:%d)", pRecvMsg->dwAuthUserID );
			Disconnect();
			return;
		}

		// IP Ȯ�� �� Ʋ���� ������ ���� ����	
		if( strncmp( GetIP(), pActiveUser->GetAuthIP(), IP_MAX_LEN ) != 0 )
		{
            MessageOut(eCRITICAL_LOG,   "Warning: Connected User's IP is different from IP Recved from AgentServer[Guid:%u][IP:%s][A:%u][AIP:%s]",
				pActiveUser->GetGUID(), GetIP(), pActiveUser->GetAuthID(), pActiveUser->GetAuthIP());
//			Disconnect();
			return;
		}
		
		// ��Ƽ�� ������ WaitingUserList���� ����
		WaitingUserList::Instance()->RemoveUser( pRecvMsg->dwAuthUserID );

		// ��Ƽ�� ������ ��Ʈ�� ������Ʈ ��ü
		Redirect( pActiveUser );
		pActiveUser->OnRedirect();

        // TempUser ��ȯ
        UserFactory::Instance()->Free( this );

		eZONETYPE	eZoneType = pActiveUser->GetStatus();
		DWORD		dwZoneKey = 1;					// CharSelect Zonekey = 1

		if( pActiveUser->GetZoneKey() )
		{
			dwZoneKey = pActiveUser->GetZoneKey();	// ���� ���� �����ϴ� ��쿡 ���� ������ ZoneKey�� ����
		}

		pActiveUser->SetStatus( eZONETYPE_MAX );
		pActiveUser->SetZoneKey( 0 );

		// ä�ο� ���� �߰�
		Channel *pChannel = ChannelManager::Instance()->GetChannel( pActiveUser->GetChannelID() );
		if( pChannel )
		{
			//pChannel->AddUser( pActiveUser, US_IN_CHARSCENE, 0 );
			pChannel->AddUser( pActiveUser, eZoneType, dwZoneKey );
	//		pChannel->AddUser( pActiveUser, eZONETYPE_CHARSELECT, 0 );

			MessageOut(eFULL_LOG,   "User Connect Success! ä��(%d) Guid(%d) IP(%s) Status(%u)",
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