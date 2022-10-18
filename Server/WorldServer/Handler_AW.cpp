#include "StdAfx.h"
#include ".\handler_AW.h"
#include "WorldServer.h"
#include "AgentServerSession.h"
#include "UserFactory.h"
#include "UserManager.h"
#include "WaitingUserList.h"
#include "Channel.h"
#include "ChannelManager.h"
#include "ServerSessionManager.h"
#include <PacketStruct_AW.h>
#include <Protocol_AW.h>
#include <PacketStruct_MW.h>
#include <Protocol_ServerCommon.h>
#include <PacketStruct_ServerCommon.h>

//ģ�� ��õ �̺�Ʈ ������
#include "FriendInvitation/WFriendInvitationController.h"

//������ ���� ��� ����
#include "WorldServerInfoParser.h"

// ��Ƽ ���� ��ɵ� ����
//#include "Party.h"
#include "BaseZone.h"
#include "PartyManager.h"

// ģ�� ��� ���� ����
#include <Protocol_DW.h>
#include <PacketStruct_DW.h>

// GM��� ����
#include <Protocol_CW.h>
#include <PacketStruct_CW.h>

// ��� ���� ����
#include "WorldGuildManager.h"
#include "WorldGuild.h"
#include <PacketStruct_WZ.h>
// �׷�α� �ý��� 
#include "GroupManager.h"
//�ع決�� ����
#include <ProhibitionWordParser.h>

#include "GMCmdManager.h"
#include <SSQ/SSQPacketProcS2S.h>

#include "Lotto/Lotto.h"
// ��Ƽ ã�� �ý���
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
#include "PartyFinding/PartyFindingManager.h"
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
#include "WorldSystemMemo.h"

GMCmdManager	Handler_AW::m_GMCmdManager;


VOID Handler_AW::OnAW_AGENTSERVER_INFO_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_AGENTSERVER_INFO_SYN *pRecvMsg = (MSG_AW_AGENTSERVER_INFO_SYN*)pMsg;

/*
#ifdef _DEBUG
	assert( pRecvMsg->dwChannelID > 0 && pRecvMsg->dwChannelID < 10 );
#else
	if( pRecvMsg->dwChannelID <= 0 || pRecvMsg->dwChannelID >= 10 )
	{
		MessageOut( eCRITICAL_LOG, "AGENTSERVER_INFO_SYN Error - ä�� �ε��� ����![%u]", pRecvMsg->dwChannelID );
		pAgentServer->Disconnect();
		return;
	}
#endif
*/

	//if(pRecvMsg->dwChannelID == 0)
	//	pRecvMsg->dwChannelID = 1;

	if( ChannelManager::Instance()->GetChannel( pRecvMsg->dwChannelID ) != NULL )
	{
		MessageOut(eCRITICAL_LOG,  "Channel(%d) Already Exist...Disconnect", pRecvMsg->dwChannelID );
		pAgentServer->Disconnect();
		return;
	}

	pAgentServer->SetChannelID( pRecvMsg->dwChannelID );

	SERVER_KEY serverKey;

	serverKey.Set( (BYTE)WorldServer::Instance()->GetWorldID(), (BYTE)pRecvMsg->dwChannelID, AGENT_SERVER, 1 );
	pAgentServer->SetServerKey( (DWORD)serverKey );

	ChannelManager::Instance()->AddChannel( pRecvMsg->dwChannelID );
/*
	// Ȥ�� ���� �׾����� �ž�?
	MSG_AW_USER_RESTORE_SYN	sendMsg;

	sendMsg.m_byCategory = AW_USER_STATUS;
	sendMsg.m_byProtocol = AW_USER_RESTORE_SYN;
	sendMsg.m_dwKey = WorldServer::Instance()->GetServerKey();

	// �׷� ���� �� ����ٹ�...
	pAgentServer->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
*/
}

VOID Handler_AW::OnAW_PREPARE_NEW_USER_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_PREPARE_NEW_USER_SYN *pRecvMsg = (MSG_AW_PREPARE_NEW_USER_SYN*)pMsg;

	User *pUser;

	if( pAgentServer->GetChannelID() == 0 )
	{
		MessageOut(eCRITICAL_LOG,  "Not Setted Channel ID - Send to AgentServer AW_PREPARE_NEW_USER_NAK" );
		MessageOut(eCRITICAL_LOG,  "���߹������� ������ �޽����� ���������ϴ�.~ �Ѿ�ּ�");
		//MSG_AW_PREPARE_NEW_USER_NAK nakMsg;
		//nakMsg.m_dwKey = pRecvMsg->m_dwKey;
		//pAgentServer->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );
		//return;
	}

	// �ش� ������ �̹� �ִ� ��� ������ ���´�.
	if( pUser = UserManager::Instance()->FindUser( pRecvMsg->dwUserGuid ) )
	{
		MessageOut(eCRITICAL_LOG, "Duplicated guid! Disconnect Connected User(%s:%u)", pUser->GetCharName().c_str(), pUser->GetGUID() );
		pUser->Logout();
		return;
	}

	// ��Ƽ�� ���� �Ҵ�
	pUser = UserFactory::Instance()->Alloc( ACTIVE_USER );

//	pUser->Init();

	// ���� ���� ����
	pUser->SetGUID( pRecvMsg->dwUserGuid );
	pUser->SetChannelID( pAgentServer->GetChannelID() );
	pUser->SetAccountID( pRecvMsg->ptszAccountID );
//	pUser->SetStatus( (eZONETYPE)pRecvMsg->byUserStatus );
	pUser->SetPCRoomID( pRecvMsg->dwPCRoomID );

	// ������ IP ����
	pUser->SetAuthIP( pRecvMsg->szClientIP );
	pUser->SetAuthID( pRecvMsg->dwAuthUserID );

	MessageOut(eFULL_LOG, "PREPARE_NEW_USER_SYN[Channel:%u][Guid:%u][IP:%s]", pUser->GetChannelID(), 
																					pUser->GetGUID(), pUser->GetAuthIP());

	pUser->SetGMGrade( pRecvMsg->byGMGrade );
	MessageOut( eFULL_LOG, "GMUser PREPARE[Grade:%u][GUID:%u]", pRecvMsg->byGMGrade - eGM_GRADE_BASE, pUser->GetGUID() );

/*
	// ĳ���� �̸� ����.. ������ ���ڿ� ����
	if( strcmp( pRecvMsg->szCharName, "" ) != 0 )
	{
		pUser->SetCharName( pRecvMsg->szCharName );
	}
	else
	{
*/
		pUser->SetCharName( "" );
//	}

	// PC�� ����
	pUser->SetPCRoomID( pRecvMsg->dwPCRoomID );

	// ��� ����Ʈ�� �߰�
	WaitingUserList::Instance()->AddUser( pRecvMsg->dwAuthUserID, pUser );

	//MessageOut(eCRITICAL_LOG,  "���� ���� ���.. ä��(%d) Guid(%d) IP(%s)", 
	//	pUser->GetChannelID(), pUser->GetGUID(), pUser->GetAuthIP() );

	// AgentServer�� ����
	MSG_AW_PREPARE_NEW_USER_ACK ackMsg;
	ackMsg.m_byCategory		= AW_CONNECTION;
	ackMsg.m_byProtocol		= AW_PREPARE_NEW_USER_ACK;
	ackMsg.m_dwKey			= pRecvMsg->m_dwKey;
	pAgentServer->Send( (BYTE*)&ackMsg, sizeof(ackMsg) );

	MessageOut( eFULL_LOG, "PREPARE_NEW_USER_SYN[Channel:%u][Guid:%u]", pUser->GetChannelID(), pUser->GetGUID() );
}

VOID Handler_AW::OnAW_USER_SET_CHARNAME_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_USER_SET_CHARNAME_SYN *pRecvMsg = (MSG_AW_USER_SET_CHARNAME_SYN*)pMsg;

	User *pUser = UserManager::Instance()->FindUser( pRecvMsg->m_dwKey );//���� ������ ã�´�.(����ÿ� �����ϰ� �Ǹ�, �̶����� �ɸ��� ������ ����Ǿ� ���� �ʴ�.)

	if( !pUser )
	{
		//UserManager�� ������ ��� ����Ʈ���� ã�´�.
        pUser = WaitingUserList::Instance()->FindUserWithGuid( pRecvMsg->m_dwKey );
		if( pUser )
		{
			pUser->SetCharGuid( pRecvMsg->dwCharGuid );
			pUser->SetCharName( pRecvMsg->szCharName );
			pUser->SetGuildGuid(pRecvMsg->m_GuildGuid);
			pUser->SetCharLevel( pRecvMsg->m_CharLevel);
            pUser->SetCharClass( pRecvMsg->m_CharClass );
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
            pUser->SetCharGender( pRecvMsg->gender );
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
		}
		return;
	}
	else
	{
		// ������ ������ �ִ� ĳ���� ���� ��� �α׾ƿ��� ���������� ó���� ��� �Ѵ�(ģ�� ������ ��� ���� ��)
		pUser->ProcessCharLogout();

		// ProcessCharLogout���� ���ش�
//		// �� ģ�� ������ ���̻� �ʿ����
//		UserManager::Instance()->RemoveFriend( pUser );
//
//		// ä�� �������̾��� ��� ä�� ���� �ð� ������Ʈ ó���Ѵ�
//		if( pUser->IsChatBlocked() )
//		{
//			pUser->UpdateBlockTime();
//		}

		// UserManager���� ���� ĳ���� ���� ����
		if( !UserManager::Instance()->UnSetCharGuid( pUser->GetCharGuid() ) )
		{
			User *pUser = WaitingUserList::Instance()->FindUserWithGuid( pRecvMsg->m_dwKey );
			if( pUser )
			{
				pUser->SetCharGuid( 0 );
				MessageOut( eFULL_LOG, "WaitingUserList - CharName Unset[%u]", pUser->GetGUID() );
			}
		}
		else
		{
			MessageOut( eFULL_LOG, "CharGuid Unset[%u]", pUser->GetGUID() );
		}
		if( !UserManager::Instance()->UnsetUserCharName( pRecvMsg->m_dwKey ) )
		{
			User *pUser = WaitingUserList::Instance()->FindUserWithGuid( pRecvMsg->m_dwKey );
			if( pUser )
			{
				pUser->SetCharName( "" );
				MessageOut( eFULL_LOG, "WaitingUserList - CharName Unset[%u]", pUser->GetGUID() );
			}
		}
		else
		{
			MessageOut( eFULL_LOG, "CharName Unset[%u]", pUser->GetGUID() );
		}

		// ���� ���� �ʱ�ȭ
		UserManager::Instance()->SetUserCharGuid( pRecvMsg->m_dwKey, pRecvMsg->dwCharGuid );
		UserManager::Instance()->SetUserCharName( pRecvMsg->m_dwKey, pRecvMsg->szCharName );
		UserManager::Instance()->SetUserCharLevel( pRecvMsg->m_dwKey, pRecvMsg->m_CharLevel );
        UserManager::Instance()->SetUserCharClass( pRecvMsg->m_dwKey, pRecvMsg->m_CharClass );
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
        UserManager::Instance()->SetUserCharGender( pRecvMsg->m_dwKey, pRecvMsg->gender );
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

		// ��� ������ ���⼭ ����
		if( pRecvMsg->m_GuildGuid )
		{
			pUser->SetGuildGuid( pRecvMsg->m_GuildGuid );
		}

		// ģ�� ��� ��û - ĳ���� ������ �� ���� �������� ����
		MSG_DW_FRIEND_GETLIST_SYN	sendMsg;

		sendMsg.m_CharGuid = pUser->GetCharGuid();

		ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
		if( !pGameDBProxy )
		{
			MessageOut( eCRITICAL_LOG, "Friend_NewList Error - No GameDBProxy");
			return;
		}
		pGameDBProxy->Send( (BYTE*)&sendMsg, sendMsg.GetSize() );

		// Chat Block Ȯ�� ��û�� ����
		MSG_DW_GM_CHATBLOCKTIME_REQ	reqMsg;

		reqMsg.m_dwKey = pUser->GetCharGuid();

		pGameDBProxy->Send( (BYTE*)&reqMsg, sizeof(reqMsg) );

		MessageOut( eFULL_LOG, "[CharGuid:%u]ChatBlock Time Req", pUser->GetCharGuid() );

		MSG_DW_STATUS_INTERCEPTION_REQ msgREQ;
		msgREQ.m_dwKey		= pRecvMsg->m_dwKey;
		msgREQ.m_CharGuid	= pRecvMsg->dwCharGuid;
		pGameDBProxy->Send((BYTE*)&msgREQ, sizeof(msgREQ));   
	}
}

VOID Handler_AW::OnAW_USER_UNSET_CHARNAME_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_USER_UNSET_CHARNAME_SYN *pRecvMsg = (MSG_AW_USER_UNSET_CHARNAME_SYN*)pMsg;

	User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwKey );

	if( pUser )
	{
		// ������ ������ �ִ� ĳ���� ���� ��� �α׾ƿ��� ���������� ó���� ��� �Ѵ�(ģ�� ������ ��� ���� ��)
		pUser->ProcessCharLogout();
	}

	if( !UserManager::Instance()->UnSetCharGuid( pRecvMsg->m_dwKey ) )
	{
		User *pUser = WaitingUserList::Instance()->FindUserWithGuid( pRecvMsg->m_dwKey );
		if( pUser )
		{
			pUser->SetCharGuid( 0 );
			MessageOut( eFULL_LOG, "CharName Unset[%u]", pUser->GetGUID() );
		}
	}
	if( !UserManager::Instance()->UnsetUserCharName( pRecvMsg->m_dwKey ) )
	{
		User *pUser = WaitingUserList::Instance()->FindUserWithGuid( pRecvMsg->m_dwKey );
		if( pUser )
		{
			pUser->SetCharName( "" );
			MessageOut( eFULL_LOG, "WaitingUserList - CharName Unset[%u]", pUser->GetGUID() );
		}
	}

	MessageOut( eFULL_LOG, "CharName Unset Error - Unset Failed[%u]", pRecvMsg->m_dwKey );
}

VOID Handler_AW::OnAW_USER_ENTER_ZONE_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_USER_ENTER_ZONE_SYN *pRecvMsg = (MSG_AW_USER_ENTER_ZONE_SYN*)pMsg;

	// �ش� ���� ������ ã��
	User *pUser = UserManager::Instance()->FindUser( pRecvMsg->m_dwKey );
	if( pUser )
	{
		Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
		if( !pChannel )
		{
			MessageOut( eCRITICAL_LOG, "USER_ENTER_ZONE Error - [Guid:%u][ZT:%u][ZK:%u] Channel Not Exist!", pRecvMsg->m_dwKey, pRecvMsg->byZoneType, pRecvMsg->dwZoneKey );
			// �� �Ҿ���ȴ�! �ٽ� �پ��!
//			pUser->Logout();
			return;
		}

		if( !pChannel->UserEnterZone( pUser, static_cast<eZONETYPE>(pRecvMsg->byZoneType), pRecvMsg->dwZoneKey, pRecvMsg->MapCode ) )
		{
#ifdef _DEBUG
			assert( !"invalid user status" );
#else
			MessageOut( eCRITICAL_LOG, "USER_ENTER_ZONE Error - No User Status[Guid:%u][Stat:%u]", pRecvMsg->m_dwKey, pRecvMsg->byZoneType );
			// �� �Ҿ���ȴ�! �ٽ� �پ��!!!
//			pUser->Logout();
			return;
#endif
		}
	}
	else
	{
		// �� ���� ����ȭ ���� - ó�� ���� ���� �ݵ�� �߻��ϸ�, ó���� �� �ʿ䰡 ����
/*
		MessageOut( eFULL_LOG, "USER_ENTER_ZONE Error - User[%u] ���� �� ����(����ȭ ����)", pRecvMsg->m_dwKey );

		pUser = WaitingUserList::Instance()->FindUserWithGuid( pRecvMsg->m_dwKey );
		if( pUser )
		{
#ifdef _DEBUG
			ASSERT( !pUser->GetZoneKey() && "LeaveZone ���� EnterZone!!!");	// sectorIndex�� 0 �ƴϸ� LeaveZone�� ����� �� �ƴٴ� ���
#else
			if( pUser->GetZoneKey() )
			{
				MessageOut( eCRITICAL_LOG, "USER_ENTER_ZONE Error - LeaveZone ���� EnterZone[Guid:%u][UserZT:%u][ZT:%u][ZK:%u]", 
											pRecvMsg->m_dwKey, pUser->GetStatus(), pRecvMsg->byZoneType, pRecvMsg->dwZoneKey );
				return;
			}
#endif
//			pUser->SeFtStatus( (eZONETYPE)pRecvMsg->byZoneType );
//			pUser->SetZoneKey( pRecvMsg->dwZoneKey );

			Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
			if( !pChannel )
			{
				MessageOut( eCRITICAL_LOG, "USER_ENTER_ZONE_SYN Error - No User Channel %u(%u)", pUser->GetChannelID(), 
																								pUser->GetGUID() );
				return;
			}
		}
		else
		{
			MessageOut( eCRITICAL_LOG, "USER_ENTER_ZONE Error - No User[%u]", pRecvMsg->m_dwKey );
		}
*/
	}

    //������ �Ѱ���ġ���� �����ϸ� ���Ӽ����� �����ش�.
}

VOID Handler_AW::OnAW_USER_LEAVE_ZONE_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_USER_LEAVE_ZONE_SYN *pRecvMsg = (MSG_AW_USER_LEAVE_ZONE_SYN*)pMsg;
	
	// �ش� ���� ������ ã��
	User *pUser = UserManager::Instance()->FindUser( pRecvMsg->m_dwKey );
	if( pUser )
	{
		Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
		if( !pChannel )
		{
			MessageOut( eCRITICAL_LOG, "USER_LEAVE_ZONE Error - [Guid:%u][ZT:%u][ZK:%u] Channel Not Exist!", pRecvMsg->m_dwKey, pRecvMsg->byZoneType, pRecvMsg->dwZoneKey );
			// �� �Ҿ���ȴ�! �ٽ� �پ��!
//			pUser->Logout();
			return;
		}

		// ������ �α��� ����ȭ ���� ���� ��� _MAX ���¿��� �� ���������� �� �� �ִ�. ��ó �ʿ�.
		if( pUser->GetStatus() >= eZONETYPE_MAX )
		{
			MessageOut( eMIDDLE_LOG, "USER_LEAVE_ZONE Error - No User Status[Guid:%u][Stat:%u]", pRecvMsg->m_dwKey, pRecvMsg->byZoneType );
			// �� �Ҿ���ȴ�! �ٽ� �پ��!!!
			pUser->Logout();
			return;
		}
		
		pChannel->UserLeavePrevZone( pUser );
	}
	else
	{
		pUser = WaitingUserList::Instance()->FindUserWithGuid( pRecvMsg->m_dwKey );
		if( pUser )
		{
			Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
			if( !pChannel )
			{
				MessageOut( eCRITICAL_LOG, "[Guid:%u] Agent Leave[Channel:%u] Error - No Channel", pUser->GetGUID(), pUser->GetChannelID() );
				return;
			}

			MessageOut( eFULL_LOG, "[Guid:%u] Agent Leave Zone[Channel:%u]", pUser->GetGUID(), pUser->GetChannelID() );

			// ��Ƽ key�� ���� �����(���� ���� ���̾��� ���) ��Ƽ���� �� �ش�
			if( pUser->GetParty() )
			{
				pChannel->WithdrawParty( pUser );
			}
//			pChannel->UserLeavePrevZone( pUser );
//			pUser->SetStatus( (eZONETYPE)pRecvMsg->byStatus );
//			pUser->SetZoneKey( pRecvMsg->dwZoneKey );
		}
	}

}

VOID Handler_AW::OnAW_USER_LOGOUT( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_USER_LOGOUT *pRecvMsg = (MSG_AW_USER_LOGOUT*)pMsg;

	User *pUser = UserManager::Instance()->FindUser( pRecvMsg->m_dwKey );

	if( !pUser ) return;

	// ���� ���� ����
	MessageOut(eCRITICAL_LOG, "AgentServer Disconnect User[%s][Guid:%u]", pUser->GetCharName().c_str(), pUser->GetGUID() );
	pUser->Logout();
}

//���� ���� <---> Agent �������� ���Ṯ���� �����ÿ� Ŭ���̾�Ʈ ȣ���Ͽ� ���� ����
VOID Handler_AW::OnAW_USER_RESTORE_CMD( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_USER_RESTORE_CMD* pRecvMsg = (MSG_AW_USER_RESTORE_CMD*)pMsg;

	User* pUser;
	DWORD dwChannelID = pAgentServer->GetChannelID();
	Channel* pChannel = ChannelManager::Instance()->GetChannel( dwChannelID );

	if( !pChannel )
	{
		MessageOut( eCRITICAL_LOG, "[C:%u] Agent Restore User[Guid:%u] Error - Invalid Channel!!", dwChannelID, pRecvMsg->dwUserGUID );
		return;
	}

	// �ش� ������ �̹� �ִ� ��� ������ ���´�.
	pUser = UserManager::Instance()->FindUser( pRecvMsg->dwUserGUID );

	if( pUser )
	{
		MessageOut(eCRITICAL_LOG, "Duplicated guid! Disconnect Connected User[%s][Guid:%u][ZT:%u]", pUser->GetCharName().c_str(), 
																							pUser->GetGUID(), 
																							pRecvMsg->byZoneType );
		pUser->Logout();
//		return;
	}
	else
	{
		pUser = WaitingUserList::Instance()->FindUserWithAuthID( pRecvMsg->dwAuthUserID );

		if( pUser )
		{
			MessageOut(eCRITICAL_LOG, "WaitingUserList duplicated guid! Disconnect Connected User[%s][Guid:%u][ZT:%u]", pUser->GetCharName().c_str(), 
																								pUser->GetGUID(), 
																								pRecvMsg->byZoneType );
			pUser->Logout();
//			return;
		}
	}

	// ��Ƽ�� ���� �Ҵ�
	pUser = UserFactory::Instance()->Alloc( ACTIVE_USER );

	// ���� ���� ����
	pUser->SetGUID( pRecvMsg->dwUserGUID );
	pUser->SetChannelID( dwChannelID );

	pUser->SetGMGrade( pRecvMsg->byGMGrade );
	MessageOut( eFULL_LOG, "GMUser PREPARE[Grade:%u][GUID:%u]", pRecvMsg->byGMGrade - eGM_GRADE_BASE, pUser->GetGUID() );

	// ������ IP ����
	pUser->SetAuthIP( pRecvMsg->pszClientIP );
	pUser->SetAuthID( pRecvMsg->dwAuthUserID );

	MessageOut(eFULL_LOG, "AW_USER_RESTORE_CMD[Channel:%u][Guid:%u][IP:%s]", pUser->GetChannelID(), 
																					pUser->GetGUID(), pUser->GetAuthIP());

	if( pRecvMsg->byZoneType != eZONETYPE_CHARSELECT )
	{
		// ĳ���� ���� ����
		pUser->SetCharGuid( pRecvMsg->CharGuid );
		pRecvMsg->tszCharName[MAX_CHARNAME_LENGTH] = '\0';
		pUser->SetCharName( pRecvMsg->tszCharName );
		pUser->SetStatus( (eZONETYPE)pRecvMsg->byZoneType );
		pUser->SetZoneKey( pRecvMsg->dwZoneKey );
		pUser->SetCharLevel(pRecvMsg->m_CharLevel);
        pUser->SetCharClass(pRecvMsg->m_CharClass);

		MessageOut( eCRITICAL_LOG, "[C:%u]AgentServer Restore User[Guid:%u][CharGuid:%u][ZT:%u][ZK:%u]", dwChannelID, 
																									pRecvMsg->dwUserGUID, 
																									pRecvMsg->CharGuid, 
																									pRecvMsg->byZoneType, 
																									pRecvMsg->dwZoneKey );

		// ��� ������ ���⼭ ���� - ĳ���� ����â�� �ִٸ� ������ CHARNAME_SELECT�� �� ó���ȴ�
		if( pRecvMsg->m_GuildGuid )
		{
			pUser->SetGuildGuid( pRecvMsg->m_GuildGuid );

			WorldGuildManager::Instance()->RequestGuildRestore( pRecvMsg->m_GuildGuid, pUser->GetCharGuid() );
		}
		else
		{
			if( pUser->GetGuildGuid() )
			{
				MessageOut(eCRITICAL_LOG,
                           "USER_RESTORE Error - No Guild but have GuildGuid "
                           "[Guid:%u][CharGuid:%u][GuildGuid:%u]",
                           pUser->GetGUID(), pUser->GetCharGuid(), pUser->GetGuildGuid());
				pUser->SetGuildGuid( 0 );
			}
		}

        if( pUser->GetCharGuid() )
		{
			// ģ�� ��� ��û - ĳ���� ���� â�� �ƴ� ��� ���ӵ� ������ ����
			pUser->InitFriendListInit();	// ��û�ϱ� ���� �ʱ�ȭ�� �ִ� �뾲!
            pUser->InitGroupInfo();

            ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
            if( !pGameDBProxy )
            {
                MessageOut( eCRITICAL_LOG, "Error Restore Friend_NewList & GroupList - No GameDBProxy");
                return;
            }

			MSG_DW_FRIEND_GETLIST_SYN	sendMsg;

			sendMsg.m_CharGuid = pUser->GetCharGuid();
			pGameDBProxy->Send( (BYTE*)&sendMsg, sendMsg.GetSize() );

            MSG_DW_GROUP_LIST_REQ msgList;//�׷� �ɹ� ����Ʈ ��û �޼���
            msgList.m_CharGUID = pUser->GetCharGuid();
            pGameDBProxy->Send( (BYTE*)&msgList, sizeof(msgList) );

			// Chat Block Ȯ�� ��û�� ����
			MSG_DW_GM_CHATBLOCKTIME_REQ	reqMsg;

			reqMsg.m_dwKey = pUser->GetCharGuid();

			pGameDBProxy->Send( (BYTE*)&reqMsg, sizeof(reqMsg) );

			MessageOut( eFULL_LOG, "[CharGuid:%u]ChatBlock Time Req", pUser->GetCharGuid() );

			MSG_DW_STATUS_INTERCEPTION_REQ reqMSG;
			reqMSG.m_dwKey = pUser->GetGUID();
			reqMSG.m_CharGuid = pUser->GetCharGuid();
			pGameDBProxy->Send((BYTE*)&reqMSG, sizeof(reqMSG));
		}
	}
	else
	{
		MessageOut( eFULL_LOG, "[C:%u]AgentServer Restore User[Guid:%u]", dwChannelID, pRecvMsg->dwUserGUID );
	}

	// ��Ƽ �Ἲ�Ǿ� ������ ����
	if( pRecvMsg->wPartyKey )
	{
		ChannelManager::Instance()->GetChannel( dwChannelID )->RestoreParty( pRecvMsg->wPartyKey, pUser );
	}

	// ID
	pUser->SetAccountID( pRecvMsg->ptszAccountID );

	// PC�� ����
	pUser->SetPCRoomID( pRecvMsg->dwPCRoomID );

	// ��� ����Ʈ�� �߰�
	WaitingUserList::Instance()->AddUser( pRecvMsg->dwAuthUserID, pUser );

	// AgentServer�� ����
	MSG_AW_PREPARE_NEW_USER_ACK ackMsg;
	ackMsg.m_byCategory		= AW_CONNECTION;
	ackMsg.m_byProtocol		= AW_PREPARE_NEW_USER_ACK;
	ackMsg.m_dwKey			= pRecvMsg->dwUserGUID;
	pAgentServer->Send( (BYTE*)&ackMsg, sizeof(ackMsg) );

	MessageOut( eFULL_LOG, "PREPARE_NEW_USER_SYN_CMD[Channel:%u][Guid:%u]", pUser->GetChannelID(), pUser->GetGUID() );
}

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// ���� ���� ���� �Լ�
namespace {

// ����� ���� �α׸� ����Ѵ�.
inline void scoped_write_log(const PartyInfo* const party, 
                             const PartyFindingResultValueType result, 
                             const TCHAR* log_function_string)
{
    const BYTE log_level = (result == RC::PARTY_FINDING_SUCCESS) ? eFULL_LOG : eCRITICAL_LOG;
    if (party == NULL)
    {
        MessageOut(log_level, 
                   _T("|PartyFinding|Result:%d|%s|PartyInfo is null|"), 
                   result, 
                   log_function_string);
    }
    else
    {
        MessageOut(log_level, 
                   _T("|PartyFinding|Result:%d|%s|PartyKey:%u|PartyMasterID:%u|NumOfJoinedMember:%u|"), 
                   result, 
                   log_function_string, 
                   party->party_key_, 
                   party->party_master_id_, 
                   party->num_of_joined_member_);
    }
} // scoped_write_log()

} // namespace

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

// ��Ƽ �Ἲ
VOID Handler_AW::OnAW_PARTY_MAKE_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_PARTY_MAKE_SYN* pRecvMsg = (MSG_AW_PARTY_MAKE_SYN*)pMsg;
	Channel *pChannel = ChannelManager::Instance()->GetChannel( pAgentServer->GetChannelID() );

	if( !pChannel )
	{
		MessageOut( eCRITICAL_LOG, "PARTY_MAKE_SYN Error - [C:%u] Channel Not Exist!", pRecvMsg->m_dwKey );
		return;
	}

	pChannel->CreateParty( pRecvMsg->wPartyKey );
	
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    // ��Ƽ ���� �������� ��Ƽ ������ ������ �ش�.
    {
        PartyInfo new_party;
        new_party.party_key_ = pRecvMsg->wPartyKey;
        new_party.party_master_id_ = pRecvMsg->party_master_id_;
        new_party.num_of_joined_member_ = pRecvMsg->num_of_joined_member_;

        PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
        const PartyFindingResultValueType create_party_result = 
            party_finding_manager.CreateParty(new_party);
        
        scoped_write_log(&new_party, create_party_result, __FUNCTION__);
        
        if (create_party_result != RC::PARTY_FINDING_SUCCESS)
        {
            return;
        }
    }
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
    
    MessageOut( eFULL_LOG, "Party Make Complete[C:%u][P:%u]", pChannel->GetChannelID(), pRecvMsg->wPartyKey );
}

// ��Ƽ�� �߰�
VOID Handler_AW::OnAW_PARTY_JOIN_MEMBER_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_PARTY_JOIN_MEMBER_SYN* pRecvMsg = (MSG_AW_PARTY_JOIN_MEMBER_SYN*)pMsg;

	Channel *pChannel = ChannelManager::Instance()->GetChannel( pAgentServer->GetChannelID() );

	if( !pChannel )
	{
		MessageOut( eCRITICAL_LOG, "PARTY_JOIN_MEMBER_SYN Error - [C:%u] Channel Not Exist!", pRecvMsg->m_dwKey );
		return;
	}

	User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGUID );

	if( !pUser )
	{
		MessageOut( eCRITICAL_LOG, "PARTY_JOIN_MEMBER_SYN Error - [C:%u] User[CharGuid:%u] Not Exist!", pRecvMsg->m_dwKey, pRecvMsg->m_CharGUID );
		return;
	}

	if( !pChannel->JoinParty( pRecvMsg->wPartyKey, pUser ) )
	{
		MessageOut( eCRITICAL_LOG, "PARTY_JOIN_MEMBER_SYN Error - [C:%u][P:%u][CharGuid:%u] Join Error!", pRecvMsg->m_dwKey, 
																										pRecvMsg->wPartyKey, 
																										pRecvMsg->m_CharGUID );
		return;
	}

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    // ��Ƽ ���� �������� ��Ƽ ������ ������ �ش�.
    {
        PartyInfo join_party;
        join_party.party_key_ = pRecvMsg->wPartyKey;
        join_party.party_master_id_ = pRecvMsg->party_master_id_;
        join_party.num_of_joined_member_ = pRecvMsg->num_of_joined_member_;

        PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
        const PartyFindingResultValueType join_party_result = 
            party_finding_manager.UpdateParty(join_party);
        
        scoped_write_log(&join_party, join_party_result, __FUNCTION__);

        if (join_party_result != RC::PARTY_FINDING_SUCCESS)
        {
            return;
        }
    }
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

    MessageOut( eFULL_LOG, "Party Join Complete[C:%u][P:%u][CharGuid:%u]", pRecvMsg->m_dwKey, 
        pRecvMsg->wPartyKey, 
        pRecvMsg->m_CharGUID );
}

// ��Ƽ�� Ż��
VOID Handler_AW::OnAW_PARTY_WITHDRAW_MEMBER_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_PARTY_WITHDRAW_MEMBER_SYN* pRecvMsg = (MSG_AW_PARTY_WITHDRAW_MEMBER_SYN*)pMsg;

	Channel *pChannel = ChannelManager::Instance()->GetChannel( pAgentServer->GetChannelID() );

	if( !pChannel )
	{
		MessageOut( eCRITICAL_LOG, "PARTY_WITHDRAW_MEMBER_SYN Error - [C:%u] Channel Not Exist!", pRecvMsg->m_dwKey );
		return;
	}

	User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGUID );

	if( !pUser )
	{
		// �� ���� Withdraw ������ �̹� ��Ƽ���� ���弭������ ������ ��� - ���� ������ �� ���� ������ MIDDLE_LOG ������ �����Ͽ� üũ�� ����
		MessageOut( eMIDDLE_LOG, "PARTY_WITHDRAW_MEMBER_SYN Error - [C:%u] User[CharGuid:%u] Not Exist!", pRecvMsg->m_dwKey, pRecvMsg->m_CharGUID );
		return;
	}

	if( !pChannel->WithdrawParty( pUser ) )
	{
		MessageOut( eCRITICAL_LOG, "PARTY_WITHDRAW_MEMBER_SYN Error - [C:%u][P:%u][CharGuid:%u] Withdraw Error!", pRecvMsg->m_dwKey, 
																										pRecvMsg->wPartyKey, 
																										pRecvMsg->m_CharGUID );
		return;
	}

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    // ��Ƽ ���� �������� ��Ƽ ������ ������ �ش�.
    {
        PartyInfo withdraw_party;
        withdraw_party.party_key_ = pRecvMsg->wPartyKey;
        withdraw_party.party_master_id_ = pRecvMsg->party_master_id_;
        withdraw_party.num_of_joined_member_ = pRecvMsg->num_of_joined_member_;

        PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
        const PartyFindingResultValueType withdraw_party_result = 
            party_finding_manager.UpdateParty(withdraw_party);
        
        scoped_write_log(&withdraw_party, withdraw_party_result, __FUNCTION__);

        if (withdraw_party_result != RC::PARTY_FINDING_SUCCESS)
        {
            return;
        }
    }
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

    MessageOut( eFULL_LOG, "Party Withdraw Complete[C:%u][P:%u][CharGuid:%u]", pRecvMsg->m_dwKey, 
        pRecvMsg->wPartyKey, 
        pRecvMsg->m_CharGUID );
}

// ��Ƽ ��ü
VOID Handler_AW::OnAW_PARTY_DESTROY_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_PARTY_DESTROY_SYN* pRecvMsg = (MSG_AW_PARTY_DESTROY_SYN*)pMsg;
	Channel *pChannel = ChannelManager::Instance()->GetChannel( pAgentServer->GetChannelID() );

	if( !pChannel )
	{
		MessageOut( eCRITICAL_LOG, "PARTY_DESTROY_SYN Error - [C:%u] Channel Not Exist!", pRecvMsg->m_dwKey );
		return;
	}

	pChannel->DestroyParty( pRecvMsg->wPartyKey );
	
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    // ��Ƽ ���� �������� ��Ƽ ������ ������ �ش�.
    {
        PartyInfo destroy_party;
        destroy_party.party_key_ = pRecvMsg->wPartyKey;
        destroy_party.party_master_id_ = pRecvMsg->party_master_id_;
        destroy_party.num_of_joined_member_ = pRecvMsg->num_of_joined_member_;

        PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
        const PartyFindingResultValueType destroy_party_result = 
            party_finding_manager.DestroyParty(destroy_party);
        
        scoped_write_log(&destroy_party, destroy_party_result, __FUNCTION__);

        if (destroy_party_result != RC::PARTY_FINDING_SUCCESS)
        {
            return;
        }
    }
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

    MessageOut( eFULL_LOG, "Party Destroy Complete[C:%u][P:%u]", pChannel->GetChannelID(), pRecvMsg->wPartyKey );
}

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// ��Ƽ���� ����Ǹ� ȣ��ȴ�.
void Handler_AW::OnAW_PARTY_CHANGE_MASTER_SYN(AgentServerSession* /*agent_server*/, 
                                              MSG_BASE* receive_msg, 
                                              WORD /*receive_msg_size*/)
{
    const MSG_AW_PARTY_CHANGE_MASTER_SYN* const change_master_syn = 
        static_cast<const MSG_AW_PARTY_CHANGE_MASTER_SYN* const>(receive_msg);

    PartyInfo change_master_party;
    change_master_party.party_key_ = change_master_syn->party_key_;
    change_master_party.party_master_id_ = change_master_syn->new_party_master_id_;
    change_master_party.num_of_joined_member_ = change_master_syn->num_of_joined_member_;

    PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
    const PartyFindingResultValueType change_master_result = 
        party_finding_manager.UpdateParty(change_master_party);

    // �α׸� ����Ѵ�.
    scoped_write_log(&change_master_party, change_master_result, __FUNCTION__);
}

// ��Ƽ �������� ��Ƽ ���� ��û�� ���� ������ ���� ȣ��ȴ�.
void Handler_AW::OnAW_PARTY_FINDING_JOIN_RESULT_ACK(AgentServerSession* /*agent_server*/, 
                                                    MSG_BASE* receive_msg, 
                                                    WORD /*receive_msg_size*/)
{
    const MSG_AW_PARTY_FINDING_JOIN_RESULT_ACK* const join_result_ack = 
        static_cast<const MSG_AW_PARTY_FINDING_JOIN_RESULT_ACK* const>(receive_msg);

    // ���� ��û�� ������ �˻��Ѵ�.
    User* const join_wanted_user = 
        UserManager::Instance()->FindUserByCharGuid(join_result_ack->join_wanted_user_id_);
    if (join_wanted_user == NULL)
    {
        return;
    }
    
    // ���� ä���� �˻��Ѵ�.
    const Channel* const channel = 
        ChannelManager::Instance()->GetChannel(join_wanted_user->GetChannelID());
    if (channel == NULL)
    {
        return;
    }

    // ���� �޽����� �����ؼ� ��û�ڿ��� ������.
    MSG_CW_PARTY_FINDING_RESULT_ACK result_ack;
    ::ZeroMemory(&result_ack, sizeof(result_ack));
    result_ack.response_type_ = PartyFindingResponseTypeConst::kJoin;
    result_ack.result_ = join_result_ack->join_result_;
    result_ack.party_finding_key_ = join_result_ack->join_wanted_party_finding_key_;
    join_wanted_user->SendPacket(reinterpret_cast<BYTE*>(&result_ack), result_ack.GetSize());

    // ������ ��Ƽ�� �˻��Ѵ�.
    PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
    const PartyInfo* const joined_party = 
        party_finding_manager.FindParty(join_result_ack->join_wanted_party_key_);

    // �α׸� ����Ѵ�.
    scoped_write_log(joined_party, result_ack.result_, __FUNCTION__);
}

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

// GM �Ӹ� ��/��
VOID Handler_AW::OnAW_GM_WHISPER_SET_CMD(  AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_GM_WHISPER_SET_CMD *pRecvMsg = (MSG_AW_GM_WHISPER_SET_CMD*)pMsg;

	User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwKey );

	if(!pUser)
	{
		MessageOut(eCRITICAL_LOG,  "GM Whisper Set Failed(pUser == NULL)");
		return;
	}

	// �� ������ �Ӹ� ��/�� ���� ����
	pUser->SetWhisper(pRecvMsg->m_bOn);

	// ���������� �˸���
	MSG_CW_GM_WHISPER_SET_ACK	resMsg;

	resMsg.m_bOn = pRecvMsg->m_bOn;

	pUser->SendPacket( (BYTE*)&resMsg, sizeof(MSG_CW_GM_WHISPER_SET_ACK) );

	MessageOut(eCRITICAL_LOG,  "(%s) GM Whisper Set(%d)", pUser->GetCharName().c_str(), pRecvMsg->m_bOn);
}

// ä�ñ���/���
VOID Handler_AW::OnAW_GM_USERCHAT_BLOCK_CMD(  AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	// ����� ��Ȯ�� �䱸������ ���� �ϴ� ����
	MSG_AW_GM_USERCHAT_BLOCK_CMD* pRecvMsg = (MSG_AW_GM_USERCHAT_BLOCK_CMD*)pMsg;

	Channel *pChannel = ChannelManager::Instance()->GetChannel(pAgentServer->GetChannelID());
	if(!pChannel)
	{
		MessageOut(eCRITICAL_LOG, "AW_GM_USERCHAT_BLOCK_CMD Error - [C:%u] Channel Not Exist!", pRecvMsg->m_dwKey);
		return;
	}

	// ��û ���� ã�Ƽ� GM���� �˻��ϰ�, ��� ������ ���¿� ���� Block�Ѵ�
	User *pGM = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_dwKey);
	if(!pGM)
	{
		MessageOut(eCRITICAL_LOG, "UserChat Block Error - GMUser Not Connected[GM:%u]", pRecvMsg->m_dwKey);
		return;
	}

	User *pBlockUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_CharGuid);
	// ���� ������ ���� ������ ��� ���� �� ����!!
	// ���� �� ����� �����Ϸ��� �� ��찡 ���� ������ DB ������ ���� �ش� ������ �����ϴ��� �˻��ؾ� �ϹǷ�
	// �� ����� ���� WOPS������ �����ؾ� �� ���̴�
	if(!pBlockUser)
	{
		MSG_CW_GM_USERCHAT_BLOCK_NAK nakMsg;
		nakMsg.m_dwKey = pRecvMsg->m_dwKey;
		nakMsg.m_wErrorCode = RC::RC_CHAT_NOCHAR;
		MessageOut( eCRITICAL_LOG, "UserChat Block Error - BlockUser Not Connected[%u]", pRecvMsg->m_CharGuid );
		return;
	}

	// ������ �� ó��
	if(TRUE == pRecvMsg->m_bBlock)
	{
		// �̹� ���� �����ε� �� ���ƿ��� ����
		if(pBlockUser->IsChatBlocked())
		{
			MSG_CW_GM_USERCHAT_BLOCK_NAK nakMsg;

			nakMsg.m_dwKey = pRecvMsg->m_dwKey;
			nakMsg.m_wErrorCode = RC::RC_CHAT_BLOCKED;
			MessageOut(eCRITICAL_LOG, "UserChat Block Error - Already Blocked User[%u]", pRecvMsg->m_CharGuid);
			return;
		}

		// ���� �߰�
		MSG_DW_GM_CHATBLOCK_SYN	sendMsg;

        //__NA_000313_20070223_REALTIME_CHAT_BLOCK
		sendMsg.m_dwKey = pGM->GetCharGuid();
		sendMsg.m_CharGuid = pBlockUser->GetCharGuid();
		sendMsg.m_dwBlockTIme = pRecvMsg->m_dwBlockTime;
		_tcsncpy( sendMsg.m_ptszGMAccountID, pGM->GetCharName().c_str(), MAX_CHARNAME_LENGTH );

		ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
		if(!pGameDBProxy)
		{
			MessageOut(eCRITICAL_LOG, "DW_CHATBLOCK Error - No GameDBProxy");
			return;
		}
		pGameDBProxy->Send((BYTE*)&sendMsg, sizeof(sendMsg));

		// ���� ���� ����
//		pBlockUser->SetChatBlock( pRecvMsg->m_dwBlockTime );

		MessageOut(eFULL_LOG, "ChatBlock Set - Block[%u][CharGuid:%u][Time:%u]", pBlockUser->GetGUID(), pBlockUser->GetCharGuid(), 
																					pRecvMsg->m_dwBlockTime);
	}
	else	// ����� �� ó��
	{
		// �̹� ��� ���¸� ����
		if(!pBlockUser->IsChatBlocked())
		{
			MSG_CW_GM_USERCHAT_BLOCK_NAK nakMsg;

			nakMsg.m_dwKey = pRecvMsg->m_dwKey;
			nakMsg.m_wErrorCode = RC::RC_CHAT_ALLOWED;
			MessageOut(eCRITICAL_LOG, "UserChat Block Error - Not Blocked User[%u]", pRecvMsg->m_CharGuid);
			return;
		}

		// ����ߴٰ� �˷��ش�
		MSG_DW_GM_CHATALLOW_SYN	sendMsg;

		sendMsg.m_dwKey = pGM->GetCharGuid();
		sendMsg.m_CharGuid = pBlockUser->GetCharGuid();
		_tcsncpy( sendMsg.m_ptszGMAccountID, pGM->GetCharName().c_str(), MAX_CHARNAME_LENGTH );

		ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
		if(!pGameDBProxy)
		{
			MessageOut(eCRITICAL_LOG, "DW_CHATALLOW Error - No GameDBProxy");
			return;
		}
		pGameDBProxy->Send((BYTE*)&sendMsg, sizeof(sendMsg));

		MessageOut(eFULL_LOG, "ChatBlock Set - Allow[%u][CharGuid:%u]", pBlockUser->GetGUID(), pBlockUser->GetCharGuid());
	}
}

// ���� ������ ó��
VOID Handler_AW::OnAW_USER_LEVELUP_CMD( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_USER_LEVELUP_CMD* pRecvMsg = (MSG_AW_USER_LEVELUP_CMD*)pMsg;

	User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_CharGuid );

	if( !pUser )
	{
		MessageOut( eCRITICAL_LOG, "AW_USER_LEVELUP_CMD Error - No Levelup User[CharGuid:%u]", pRecvMsg->m_CharGuid );
		return;
	}
    //���� ��, added by _KR_0_20091021_FIRENDINVITATION_EXTEND
    WFriendInvitationController::Instance()->OnCharacterLevelUp(pAgentServer, pMsg, wSize);	
    // ģ�� �˸�
	MSG_CW_FRIEND_LEVELUP_BRD	friendMsg;
	friendMsg.m_Level = pRecvMsg->m_Level;
	memset( friendMsg.m_ptszCharName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
	_tcsncpy( friendMsg.m_ptszCharName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	pUser->SendOnlineFriendAll( (BYTE*)&friendMsg, sizeof(friendMsg) );

	// ������ ��Ŷ���� ī�װ�/�������ݸ� �ٲ㼭 �ѹ� �� ������
	friendMsg.m_byCategory = CW_REVENGE;
	friendMsg.m_byProtocol = CW_REVENGE_LEVELUP_BRD;
	pUser->SendOnlineRevengeAll( (BYTE*)&friendMsg, sizeof(friendMsg) );

	MessageOut( eFULL_LOG, "FriendLevelup Msg Send[CharGuid:%u]", pRecvMsg->m_CharGuid );
	pUser->SetOnlineLevel( pRecvMsg->m_Level );
	pUser->SetCharLevel(pRecvMsg->m_Level);

    //�׷� ����鿡�Ե� �������� �˸���.
    GroupManager::Instance()->SendAroundMember( pUser, GroupManager::MEMBER_LEVELUP );
}

//////////////////////////////////////
// ������(����ģ��) ���� ��Ŷ �ڵ鷯

VOID Handler_AW::OnAW_CHAO_REQUEST_REVENGEINFO_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_CHAO_REQUEST_REVENGEINFO_SYN* pRecvMsg = (MSG_AW_CHAO_REQUEST_REVENGEINFO_SYN*)pMsg;

	User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwCharGuid );
	if( !pUser )
	{
		ServerSession* pAgentServer = ServerSessionManager::Instance()->FindAgentServer(WorldServer::Instance()->GetChannelID());
		if(pAgentServer)
		{
            // TODO(WAVERIX) (090623) (NOTE) ����� ��������.
			MSG_AW_ABUSING_USER_VERIFY_QUERY_CMD msg;
			msg.m_dwKey = pRecvMsg->m_dwKey;
			msg.m_CharGuid = pRecvMsg->m_dwCharGuid;
			msg.m_eAbuseSubType = eABUSE_SUB_LOGIC_VIOLATION_USER_NONE_WORLDSVR;
			pAgentServer->Send((BYTE*)&msg, sizeof(msg));
		}
		return;
	}

	// ������ ���� �����Ѵ�
	MSG_AW_CHAO_REQUEST_REVENGEINFO_ACK ackMsg;
	ackMsg.m_dwKey = pRecvMsg->m_dwKey;
	ackMsg.m_dwCharGuid = pRecvMsg->m_dwCharGuid;
	ackMsg.m_byRevengePoint = pUser->GetRevengePoint();
	ackMsg.m_byRevengeStatNum = pUser->GetRevengeNum();

	if( ackMsg.m_byRevengeStatNum > 0 )
	{
		STRUCT_REVENGE_INFO* pRevengeInfo = pUser->GetFirstRevenge();

		memcpy( ackMsg.m_sRevengeUserInfo, pRevengeInfo, sizeof(STRUCT_REVENGE_INFO) );

		int i=1;
		while( pRevengeInfo = pUser->GetNextRevenge() )
		{
			memcpy( &ackMsg.m_sRevengeUserInfo[i], pRevengeInfo, sizeof(STRUCT_REVENGE_INFO) );
			i++;
		}
	}

	pAgentServer->Send( (BYTE*)&ackMsg, ackMsg.GetSize() );

	MessageOut( eFULL_LOG, "REQUEST_REVENGEINFO_ACK Send[CharGuid:%u][RevengeNum:%u]", pRecvMsg->m_dwCharGuid, ackMsg.m_byRevengeStatNum );
}

VOID Handler_AW::OnAW_CHAO_REMOVE_REVENGE_CHAR_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	// PK�� ������ ���
	MSG_AW_CHAO_REMOVE_REVENGE_CHAR_SYN* pRecvMsg = (MSG_AW_CHAO_REMOVE_REVENGE_CHAR_SYN*)pMsg;
	MSG_AW_CHAO_REMOVE_REVENGE_CHAR_NAK nakMsg;

	// ������ ������ ������� �˻�
	User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwCharGuid );

	if( !pUser )
	{
		nakMsg.m_dwCharGuid = pRecvMsg->m_dwCharGuid;
		nakMsg.m_byErrorCode = RC::RC_REVENGE_NO_CHAR;

		pAgentServer->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eCRITICAL_LOG, "AW_CHAO_REMOVE_REVENGE Error - Not Connected User[CharGuid:%u]", pRecvMsg->m_dwCharGuid );
		return;
	}

	STRUCT_REVENGE_INFO *pRevengeInfo = pUser->FindRevenge( pRecvMsg->m_dwRevengeCharGuid );

	if( !pRevengeInfo )
	{
		nakMsg.m_dwCharGuid = pRecvMsg->m_dwCharGuid;
		nakMsg.m_byErrorCode = RC::RC_REVENGE_INVALID_CHAR;

		MessageOut( eCRITICAL_LOG, "AW_CHAO_REMOVE_REVENGE Error - Invalid Revenge Target[CharGuid:%u][Target:%u]", 
																					pRecvMsg->m_dwCharGuid, pRecvMsg->m_dwRevengeCharGuid );
		return;
	}

	// Ȯ���� ������ ��� DB�� ���� ��û
	ServerSession *pDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();

	if( !pDBProxy )
	{
		nakMsg.m_dwCharGuid = pRecvMsg->m_dwCharGuid;
		nakMsg.m_byErrorCode = RC::RC_REVENGE_FAILED;

		MessageOut( eCRITICAL_LOG, "AW_CHAO_REMOVE_REVENGE Error - No DBProxy!!![CharGuid:%u]", pRecvMsg->m_dwCharGuid );
		return;
	}

	MSG_DW_CHAO_PK_REVENGE_CHAR_SYN	sendMsg;

	sendMsg.m_dwKey = pAgentServer->GetChannelID();
	sendMsg.m_CharGuidFrom = pRecvMsg->m_dwCharGuid;
	sendMsg.m_RevengeGuidTo = pRecvMsg->m_dwRevengeCharGuid;

	pDBProxy->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
	pUser->SetFriendTransaction( TRUE );

	MessageOut( eFULL_LOG, "CHAO_REMOVE_REVENGE Send[CharGuid:%u][Target:%u]", sendMsg.m_CharGuidFrom, sendMsg.m_RevengeGuidTo );
}

VOID Handler_AW::OnAW_CHAO_ADD_REVENGE_CHAR_SYN( AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize )
{
	MSG_AW_CHAO_ADD_REVENGE_CHAR_SYN* pRecvMsg = (MSG_AW_CHAO_ADD_REVENGE_CHAR_SYN*)pMsg;
	MSG_AW_CHAO_ADD_REVENGE_CHAR_NAK nakMsg;

	User *pUser = UserManager::Instance()->FindUserByCharGuid( pRecvMsg->m_dwCharGuid );

	if( !pUser )
	{
		nakMsg.m_dwCharGuid = pRecvMsg->m_dwCharGuid;
		nakMsg.m_byErrorCode = RC::RC_REVENGE_NO_CHAR;

		pAgentServer->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );
		MessageOut( eCRITICAL_LOG, "AW_CHAO_ADD_REVENGE_CHAR_SYN Error - Not Connected User[%u]", pRecvMsg->m_dwCharGuid );
		return;
	}

	// ������ ����Ʈ�� ���� �ִ��� Ȯ��
	if( !pUser->GetRevengePoint() )
	{
		nakMsg.m_dwCharGuid = pRecvMsg->m_dwCharGuid;
		nakMsg.m_byErrorCode = RC::RC_REVENGE_NO_POINT;

		pAgentServer->Send( (BYTE*)&nakMsg, sizeof(nakMsg) );
		MessageOut( eCRITICAL_LOG, "AW_CHAO_ADD_REVENGE_CHAR_SYN Error - No Point[%u]", pRecvMsg->m_dwCharGuid );
		return;
	}

	// �ٷ� DB ����~
	MSG_DW_CHAO_ADD_REVENGE_CHAR_SYN sendMsg;

	sendMsg.m_dwKey = pAgentServer->GetChannelID();
	sendMsg.m_CharGuidFrom = pRecvMsg->m_dwCharGuid;
	memset( sendMsg.m_ptszCharNameTo, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );
	_tcsncpy( sendMsg.m_ptszCharNameTo, pRecvMsg->m_ptszRevengeName, MAX_CHARNAME_LENGTH );

	ServerSession *pDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();

	if( !pDBProxy )
	{
		MessageOut( eCRITICAL_LOG, "AW_CHAO_ADD_REVENGE Error - No DBProxy!!!" );

		MSG_AW_CHAO_ADD_REVENGE_CHAR_NAK nakMsg;

		nakMsg.m_dwCharGuid = pRecvMsg->m_dwCharGuid;
		nakMsg.m_byErrorCode = RC::RC_REVENGE_FAILED;
		return;
	}

	pDBProxy->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
	pUser->SetFriendTransaction( TRUE );

	MessageOut( eFULL_LOG, "AW_CHAO_ADD_REVENGE_CHAR_SYN send to DBProxy[CharGuid:%u][Name:%s]", sendMsg.m_CharGuidFrom, sendMsg.m_ptszCharNameTo );
}


// ����� ������ ���� ����
VOID Handler_AW::OnAW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD(AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize)
{
	MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD *pRecvMsg = (MSG_AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD*)pMsg;

	User *pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_dwCharGuid);
	if(!pUser) return;

    for(int cnt = 0; cnt < pRecvMsg->m_Count; cnt++)
	{			
		if(pRecvMsg->m_ItemInfo[cnt].m_Apply)
		{
			pUser->GetItemOwnFlag().IncItemInfo( (eITEMTYPE)pRecvMsg->m_ItemInfo[cnt].m_ItemType, pRecvMsg->m_ItemInfo[cnt].m_Count );
			MessageOut( eFULL_LOG, "SPECIAL ITEM INC_[CHARGUID:%u][ITEMTYPE:%d]",
                pUser->GetCharGuid(), (eITEMTYPE)pRecvMsg->m_ItemInfo[cnt].m_ItemType);
		}
		else
		{
			pUser->GetItemOwnFlag().DecItemInfo( (eITEMTYPE)pRecvMsg->m_ItemInfo[cnt].m_ItemType, pRecvMsg->m_ItemInfo[cnt].m_Count );
			MessageOut( eFULL_LOG, "SPECIAL ITEM DEC_[CHARGUID:%u][ITEMTYPE:%d]",
				pUser->GetCharGuid(), (eITEMTYPE)pRecvMsg->m_ItemInfo[cnt].m_ItemType);
		}
	}
}

VOID Handler_AW::OnAW_ITEM_SPECIAL_ITEM_CLEAR_CMD(AgentServerSession *pAgentServer, MSG_BASE *pMsg, WORD wSize)
{	
	MSG_AW_ITEM_SPECIAL_ITEM_CLEAR_CMD *pRecvMsg = (MSG_AW_ITEM_SPECIAL_ITEM_CLEAR_CMD*)pMsg;

	User *pUser = UserManager::Instance()->FindUserByCharGuid(pRecvMsg->m_dwCharGuid);
	if(!pUser) return;

	pUser->GetItemOwnFlag().Init();
}

VOID Handler_AW::OnAW_ITEM_USE_DECREE_ITEM_ACK(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
	//���� ��ġ��
    MSG_AW_ITEM_USE_DECREE_ITEM_ACK* pRecvMsg = (MSG_AW_ITEM_USE_DECREE_ITEM_ACK*)pMsg;
	User* pUser = UserManager::Instance()->FindUser(pRecvMsg->m_dwKey);
	if(!pUser)	return;

	MSG_CW_CHAT_SHOUT_BRD msgBRD;
	do
	{
		// TODO(WAVERIX) (NOTE) m_tszTmpDecreeMsg�� ����ũ��� 256�Դϴٸ�...? -.-;
		BYTE byMsgLength = _tcslen(pUser->GetTmpDecreeMsg());
		util::cSTRING_REF strMsgRD(pUser->GetTmpDecreeMsg(), sizeof(byMsgLength));

		util::cSTRING_REF_RW strNameRW4BRD( msgBRD.m_szCharName, sizeof(msgBRD.m_szCharName) );
		strNameRW4BRD.CopyFromString( pUser->GetCharName().c_str() );
		strNameRW4BRD.MakeSafeString();

		util::cSTRING_REF_RW strMsgRW4BRD( msgBRD.m_pszChatMsg, sizeof(msgBRD.m_pszChatMsg) );
		strMsgRW4BRD.CopyFromString(pUser->GetTmpDecreeMsg());
		strMsgRW4BRD.MakeSafeString();

		msgBRD.m_byMsgLength = strMsgRW4BRD.GetSTRING_LENGTH()+1;
	} while(0);

	msgBRD.m_byMessageType = msgBRD.DECREE_SHOUT;

	UserManager::Instance()->SendToAll((BYTE*)&msgBRD, msgBRD.GetSize());
}

VOID Handler_AW::OnAW_ITEM_USE_DECREE_ITEM_NAK(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
	MSG_AW_ITEM_USE_DECREE_ITEM_NAK* pRecvMsg = (MSG_AW_ITEM_USE_DECREE_ITEM_NAK*)pMsg;

	User* pUser = UserManager::Instance()->FindUser(pRecvMsg->m_dwKey);
	if(!pUser)	return;

	MSG_CW_CHAT_SHOUT_NAK msgNAK;
	msgNAK.m_wErrorCode = RC::RC_CHAT_NOT_DECREE_ITEM;
	pUser->SendPacket((BYTE*)&msgNAK, sizeof(msgNAK) );
}

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
// ������� ĳ�� ����� ��� ��û
VOID Handler_AW::OnAW_ITEM_USE_HEIM_LOTTO_TICKET_SYN(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    const MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN* msg = \
        static_cast<const MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN*>(pMsg);

    // �ش� ĳ���Ͱ� ���� ���� ���� �ִ��� �˻��Ѵ�.
    const TicketActivationXactions& xactions = Lotto::GetInstance().GetTicketActivationXactions();
    if (xactions.Find(msg->char_guid) != NULL)
    {
        // �̹� ���� ���� ���� �ִٸ� ���� ����
        MSG_AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK ack;
        ack.result_code = static_cast<WORD>(RC::RC_ITEM_INVALIDSTATE);
        ack.char_guid   = msg->char_guid;
        ack.index       = msg->index;
        ack.pos         = msg->pos;
        pAgentServer->Send((BYTE*)&ack, (WORD)sizeof(ack));
        return;
    }

    Lotto::GetInstance().OnTicketActivation(pMsg);
}
#endif // _NA_20101011_HEIM_LOTTO_CASH_TICKET

VOID Handler_AW::OnAW_USER_STATUS_INTERCEPTION_SYN(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
	MSG_AW_USER_STATUS_INTERCEPTION_SYN* pRecvMsg = (MSG_AW_USER_STATUS_INTERCEPTION_SYN*)pMsg;

	User* pUser = UserManager::Instance()->FindUser(pRecvMsg->m_dwKey);
	if(!pUser)	return;

	pUser->SetInterceptionOption(pRecvMsg->m_byInterceptionCode, pRecvMsg->m_bFlag);
}

VOID Handler_AW::OnAW_GM_NOTICE_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{	
	MSG_AW_GM_NOTICE_CMD* pRecvMsg = (MSG_AW_GM_NOTICE_CMD *)pMsg;

	User *pUser = UserManager::Instance()->FindUser(pRecvMsg->m_dwKey);
	if(!pUser)	return;

	m_GMCmdManager.ProcessNoticeType(pUser, pRecvMsg->m_eType, pRecvMsg->m_tszNoticeMsg);
}

VOID Handler_AW::OnAW_GM_LOTTO_NUM_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    MSG_AW_GM_LOTTO_NUM_CMD* pRecvMsg = (MSG_AW_GM_LOTTO_NUM_CMD*)pMsg;

    User* pUser = UserManager::Instance()->FindUser(pRecvMsg->dwUserKey);
    if (pUser == NULL) { return; }

    MSG_CW_LOTTO_FINISH_BRD resp;

    resp.wDrawNum = (WORD)Lotto::GetInstance().GetCurrentDrawNum();
    Lotto::GetInstance().GetCurrentWinningNum(resp.bWinningNum);

    pUser->SendPacket((BYTE*)&resp, (WORD)sizeof(resp));
}

VOID Handler_AW::OnAW_GM_LOTTO_SETTLE_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    MSG_AW_GM_LOTTO_SETTLE_CMD* pRecvMsg = (MSG_AW_GM_LOTTO_SETTLE_CMD*)pMsg;

    User* pUser = UserManager::Instance()->FindUser(pRecvMsg->dwUserKey);
    if (pUser == NULL) { return; }

    Lotto::GetInstance().ForceRollOver();
}

VOID Handler_AW::OnAW_GM_LOTTO_FUND_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    MSG_AW_GM_LOTTO_FUND_CMD* pRecvMsg = (MSG_AW_GM_LOTTO_FUND_CMD*)pMsg;

    User* pUser = UserManager::Instance()->FindUser(pRecvMsg->user_key);
    if (pUser == NULL) { return; }

    Lotto::GetInstance().OnTotalAccumChange(pRecvMsg->amount_delta);
}

VOID Handler_AW::OnAW_STATUS_GROUP_START_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    const MSG_AW_STATUS_GROUP_START_CMD* pRecvMsg = 
        static_cast<MSG_AW_STATUS_GROUP_START_CMD*>(pMsg);

    MSG_DW_GROUP_LIST_REQ msgList;//�׷� �ɹ� ����Ʈ ��û �޼���
    msgList.m_CharGUID = pRecvMsg->m_CharGuid;

    ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
    
    if (pGameDBProxy == NULL)
        return;

    pGameDBProxy->Send((BYTE*)&msgList, sizeof(msgList));
}

VOID Handler_AW::OnAW_OPERATION_ROOM_CTRL_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    using namespace nsSSQ;
	const MSG_AW_OPERATION_ROOM_CTRL_CMD* pRecvMsg = static_cast<const MSG_AW_OPERATION_ROOM_CTRL_CMD*>(pMsg);

	const WAR_MSG_HDR* IN pStream = pRecvMsg->m_Header;
	const WAR_MSG_HDR IN header = *pStream;

	WAR_TAG_P pIt	= (WAR_TAG_P)(pStream+1);
	WAR_TAG_P pEnd	= pIt + header.size;
	for( WORD size=0 ; pIt<pEnd ; pIt+=size )
	{
		size	= 10000;	// �� ������ ���� ��ŵ ����

		switch( pIt->_TAG )
		{
		case SSQ_S2S_SET_CHAT_BLOCK_ZONE:
			{
				RECV_PATTERNCODE_FOR_SSQ( SSQ_S2S_SET_CHAT_BLOCK_ZONE );

				ChannelManager* const pChannelManager = ChannelManager::Instance();
				const DWORD channel_ID = pAgentServer->GetChannelID();
				Channel* const pChannel = pChannelManager->GetChannel( channel_ID );
				if( !pChannel )
					break;

				ChatZone* const pChatZone = pChannel->FindZone( rMsg.ZoneKey );
				if( !pChatZone )
					break;

                // (WAVERIX) (090220) (BUG-FIX) Ÿ���� ��� ���� ����, ���弭���� ���� ����� ��
                //�۵��� �ʵ��� �ϴ� ����� ��� ��ö���� ���ǽ� ������ ���� �Ųٷ� ������
                //���弭�� �����ؼ� �����ϴ� ����� ���Ǵ� ���� ����.
				pChatZone->SetBlockZone(rMsg.Enabled == FALSE);
			}
			break;
		default:
			{
				SUNLOG( eCRITICAL_LOG, "Can't Readable SSQ Msg Format %2u", pIt->_TAG );
			}
			break;
		}
	}
}

// __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
VOID Handler_AW::OnAW_STATUS_GROUP_EXP_ACQUISITION_CMD(AgentServerSession* agent_server, MSG_BASE* msg, WORD size)
{
    const MSG_AW_STATUS_GROUP_EXP_ACQUISITION_CMD* recv_msg = 
        static_cast<MSG_AW_STATUS_GROUP_EXP_ACQUISITION_CMD*>(msg);

    User* user = UserManager::Instance()->FindUserByCharGuid(recv_msg->m_CharGuid);
    if (user == NULL)
    {
        return;
    }

    user->SetCurBonusEXP(recv_msg->m_Amount);

#ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY 
    if (GroupManager::Instance()->GetGroupLowMember(user) == GROUP_HAVE_ONLINE_LOW)
    {
        GroupManager::Instance()->SendAroundMember(user, GroupManager::GET_EXP);
    }
#else
    if (GroupManager::Instance()->GetGroupHighMember(user) == GROUP_HAVE_OFFLINE_HIGH ||
        GroupManager::Instance()->GetGroupHighMember(user) == GROUP_HAVE_ONLINE_HIGH)  
    {
        GroupManager::Instance()->SendAroundMember(user, GroupManager::GET_EXP);
    }
#endif

    user->SetCurBonusEXP(0);
}

// implemented by __NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
VOID Handler_AW::OnAW_USER_STATUS_HEIM_ACQUISITION_NTF(AgentServerSession* agent_server, MSG_BASE* msg, WORD size)
{
    const MSG_AW_USER_STATUS_HEIM_ACQUISITION_NTF* recv_msg = 
        static_cast<MSG_AW_USER_STATUS_HEIM_ACQUISITION_NTF*>(msg);

    User* user = UserManager::Instance()->FindUserByCharGuid(recv_msg->m_CharGuid);
    if (user == NULL) 
    {
        return;
    }
    user->SetCurBonusMoney(recv_msg->m_Amount);

    #ifdef _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY
        if (GroupManager::Instance()->GetGroupHighMember(user) == GROUP_HAVE_ONLINE_HIGH ||
            GroupManager::Instance()->GetGroupHighMember(user) == GROUP_HAVE_OFFLINE_HIGH)
        {
            GroupManager::Instance()->SendAroundMember(user, GroupManager::GET_MONEY);
        }
    #else
        if (GroupManager::Instance()->GetGroupLowMember(user) == GROUP_HAVE_ONLINE_LOW)
        {
            GroupManager::Instance()->SendAroundMember(user, GroupManager::GET_MONEY);
        }
    #endif

    user->SetCurBonusMoney(0);
}

// implemented by __NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION
VOID Handler_AW::OnAW_USER_STATUS_HEIM_CONSUMPTION_NTF(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    const MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF* pRecvMsg = 
        static_cast<MSG_AW_USER_STATUS_HEIM_CONSUMPTION_NTF*>(pMsg);

#ifndef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    // ���ǿ��� ���� ���� ����� ������� �ʴ� ��쿡�� ������ ���� ���� ������ �̷������
    Lotto::GetInstance().OnHeimConsumption(pMsg);
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
}

VOID Handler_AW::OnAW_USER_LOTTO_REWARD_ACK(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    Lotto::GetInstance().OnActualReward(pMsg);
}
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
VOID Handler_AW::OnAW_USER_LOTTO_TICKET_PURCHASE_ACK(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    Lotto::GetInstance().OnTicketPurchase(pMsg);
}
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE


#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
void
Handler_AW::OnAW_USER_STATUS_PCBANG_BILLING_NTF(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    MSG_AW_USER_STATUS_PCBANG_BILLING_NTF* pRecvMsg = static_cast<MSG_AW_USER_STATUS_PCBANG_BILLING_NTF*>(pMsg);
    DWORD user_key = pRecvMsg->m_dwKey;
    User* user = UserManager::Instance()->FindUser(user_key);
    if(user == 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]:[UserGuid:%u]:Can't find user.", user_key);
        return;
    }
    assert(sizeof(*pRecvMsg) == wSize);
    user->SetPCRoomID(pRecvMsg->m_PcRoomId);
#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
    user->SetPremiumService(pRecvMsg->premiumservice_);
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM
}
#endif

//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
void
Handler_AW::OnAW_USER_SYSTEMMEMO_SEND_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    const MSG_AW_USER_SYSTEMMEMO_SEND_CMD* recv_msg = 
        static_cast<MSG_AW_USER_SYSTEMMEMO_SEND_CMD*>(pMsg);

    WorldSystemMemo system_memo;
    system_memo.SendSystemMemo(recv_msg->system_memo_info_, 
                               recv_msg->system_memo_list_num_,
                               recv_msg->system_memo_info_->system_memo_type_);
}

void
Handler_AW::OnAW_FRIEND_INVITATIONR_INSERT_CMD(AgentServerSession* pAgentServer, MSG_BASE* pMsg, WORD wSize)
{
    WFriendInvitationController::Instance()->OnAddInvitatedFriendCMD(pAgentServer, pMsg, wSize);
}

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
void 
Handler_AW::OnAW_PARTY_ROOM_KEY_SYN(AgentServerSession* agentServer_session, MSG_BASE* message, WORD message_size)
{
    MSG_AW_PARTY_ROOM_KEY_SYN* receive_message = static_cast<MSG_AW_PARTY_ROOM_KEY_SYN*>(message);
    Channel* channel = ChannelManager::Instance()->GetChannel( agentServer_session->GetChannelID() );

    if (channel == NULL)
    {
        MessageOut( 
            eCRITICAL_LOG, 
            "PARTY_ROOM_KEY_SYN Error - [C:%u] Channel Not Exist!", 
            receive_message->m_dwKey 
            );
        return;
    }

    channel->SetRoomKey(receive_message->party_key_, receive_message->room_key_);
}
#endif  //_NA_004694_20120412_1202_PARTY_ROOM_KEY
