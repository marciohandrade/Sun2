#include "stdafx.h"
#include "Handler_CW.h"
#include "User.h"
#include "WorldServer.h"
#include "UserManager.h"

//GM������ ����
//#include "GMList.h"
#include <PacketStruct_CW.h>
#include <Protocol_CW.h>

// ä�� �޽��� ������ ����
#include "ChannelManager.h"
#include "Channel.h"
#include "BaseZone.h"

// ArmorKit �������� ������ ����
#include "ServerSession.h"
#include "ServerSessionManager.h"
#include <PacketStruct_MW.h>

// ģ�� ���� ����
#include <PacketStruct_DW.h>
#include <Protocol_DW.h>

// ��� ���� ����
#include "WorldGuildManager.h"
#include "WorldGuild.h"
#include "WorldGuildMember.h"
#include <PacketStruct_WZ.h>

// GM ��Ʈ�� ��� �Ľ� ����
#include "GMCmdParser.h"
#include "GMCmdString.h"
#include "GMCmdManager.h"

// ä�� ä�� ����
#include "ChatRoomManager.h"

// ��ȣȭ ����
#include "WorldServerInfoParser.h"
#include <ProhibitionWordParser.h>
#include <GameOptionParser.h>

#include <PacketStruct_AW.h>
#include <Protocol_AW.h>
#include "BaseZone.h"

#include "Lotto/Lotto.h"
// �׷�ý���
#include "GroupManager.h"

//ģ�� �ʴ� �̺�Ʈ
#include "FriendInvitation/WFriendInvitationController.h"

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
#include "PartyFinding/PartyFindingManager.h"
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationManager.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

GMCmdManager	Handler_CW::m_GMCmdManager;

//added by __NA001094_080928_TASK_SURVIVAL_SOLOING_QUEST_ETC__

//true : Passed, false : Blocked
static BOOLEAN 
CheckChatBlockZone(User* user)
{
    DWORD channel_id = user->GetChannelID();
    
    Channel* channel = ChannelManager::Instance()->GetChannel(channel_id);
    if (channel == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Channel value is null");
        return FALSE;
    }

    ChatZone* chat_zone = channel->FindZone(user->GetZoneKey());
    if (chat_zone == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]ChatZone value is null");
        return FALSE;
    }

    if (chat_zone->IsBlockZone()) 
    {
        user->SendChatBlock(RC::RC_CHAT_BELONG_ZONE_BLOCK);
        return FALSE;
    }

    return TRUE;
}


Handler_CW_IMPL( CW_HEARTBEAT )
{
//	MessageOut( eCRITICAL_LOG, "[%s/%u] HeartBeat Recv", pUser->GetCharName().c_str(), pUser->GetGUID() );
}

Handler_CW_IMPL( CW_WHISPER_SYN )
{
	MSG_CW_WHISPER_SYN *pRecvMsg = (MSG_CW_WHISPER_SYN*)pMsg;

	// ��� ä�ÿ��� ũ�Ⱑ 0�̰ų� ���� ������ ������ ����
//	if( !pRecvMsg->byMsgLen || !_tcslen(pRecvMsg->szWhisperMsg) )
//	{
//		MessageOut( eFULL_LOG, "ä�� ���� ����" );
//		return;
//	}

	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

    //__NA001094_080928_TASK_SURVIVAL_SOLOING_QUEST_ETC__
    if(CheckChatBlockZone(pUser) == 0)
        return;
	
	//ä�� �������
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();
		return;
	}

	// �Ӹ� ����� �ʹ� ũ�� ������ ���´�. ��� ����, ��Ŷ ����?
	util::cSTRING_REF strMsgRD( pRecvMsg->szWhisperMsg, sizeof(pRecvMsg->szWhisperMsg) );
	BYTE byMsgLen = pRecvMsg->byMsgLen;
	if( (MAX_CHAT_LEN < byMsgLen) || (pRecvMsg->GetSize() != wSize) || (strMsgRD.GetSTRING_LENGTH() != byMsgLen) )
	{
		MessageOut( eCRITICAL_LOG,
			_T("Invalid Whisper Chat Size {%u %u %u %u}"),
			byMsgLen, pRecvMsg->GetSize(), wSize, strMsgRD.GetSTRING_LENGTH() );
		//pUser->Disconnect();
		return;
	}

	// ������ ���� ó�� 1
	util::cSTRING_REF strNameRD( pRecvMsg->szCharNameTo, sizeof(pRecvMsg->szCharNameTo) );
	util::cSTRING_REF_RW_WITH_BUFFER<sizeof(pRecvMsg->szCharNameTo)> strNameRW;
	strNameRD.CopyTo( strNameRW );
	strNameRW.MakeSafeString();

	// ������ ���� ó�� 2
	util::cSTRING_REF_RW_WITH_BUFFER<sizeof(pRecvMsg->szWhisperMsg)> strMsgRW;
	strMsgRD.CopyTo( strMsgRW );
	strMsgRW.MakeSafeString();

	//if( strMsgRW.GetSTRING_LENGTH() != byMsgLen )
	//{
	//	MessageOut( eCRITICAL_LOG, _T("Whisper Chat Size Mismatch(%d:%d)"), strMsgRW.GetSTRING_LENGTH(), byMsgLen );
	//	return;
	//}

	// find User std::string�̴�... �����... ���� ���ϴ�.
	User* pTargetUser = UserManager::Instance()->FindUser( strNameRW.GetBUFFER() );

	// �Ӹ� ����� ������ ���� ó��
	if( !pTargetUser || eZONETYPE_MAX == pTargetUser->GetStatus() )
	{
		MessageOut( eCRITICAL_LOG, _T("No Whisper Target (%s) to (%s)"),
			pUser->GetCharName().c_str(), strNameRW.GetBUFFER() );

		MSG_CW_WHISPER_NAK msgNAK;
		msgNAK.byReason = MSG_CW_WHISPER_NAK::USER_NOT_FOUND;
		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(MSG_CW_WHISPER_NAK) );
		return;
	}

	// ����� �� �ڽ��� ��� ����
	if( pUser == pTargetUser )
	{
		MessageOut( eFULL_LOG, _T("Whisper to MySelf (%s)"), pUser->GetCharName().c_str() );
		return;
	}

	//ģ�� �����̸� ����
	if( pTargetUser->FindBlock( pUser->GetCharGuid()) )
	{
		MSG_CW_WHISPER_NAK msgNAK;
		msgNAK.byReason = MSG_CW_WHISPER_NAK::USER_WHISPER_OFF;
		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(MSG_CW_WHISPER_NAK) );
		return;
	}

	if(pTargetUser->CanWhisper())
	{
		//GM ���� ������ �Ӹ��� ���������� ��������, �������Դ� �Ӹ� ������ ���Ѵٰ� �˷��ش�
		if( pTargetUser->GetGMGrade() >= eGM_GRADE_FIRST )
		{
			MessageOut(eCRITICAL_LOG, _T("Whisper to GM but Whisper Block (%s) to (%s)"),
				pUser->GetCharName().c_str(), strNameRW.GetBUFFER() );

			// �Ӹ� ���ܵ� ��ó�� �˷� �ش�
			MSG_CW_WHISPER_NAK msgNAK;
			msgNAK.byReason = MSG_CW_WHISPER_NAK::USER_WHISPER_OFF;
			pUser->SendPacket( (BYTE*)&msgNAK, sizeof(MSG_CW_WHISPER_NAK) );

			// ��󿡰� �Ӹ� ����
			MSG_CW_WHISPER_ACK msgACK;

			util::cSTRING_REF_RW tmpWriter1( msgACK.szCharNameFrom, sizeof(msgACK.szCharNameFrom) );
			tmpWriter1.CopyFromString( pUser->GetCharName().c_str() );
			tmpWriter1.MakeSafeString();	// ����� �����̹Ƿ�...

			util::cSTRING_REF_RW tmpWriter2( msgACK.szWhisperMsg, sizeof(msgACK.szWhisperMsg) );
			strMsgRW.CopyTo( tmpWriter2 );
			strMsgRW.MakeSafeString();		// ����� �����̹Ƿ�...

			msgACK.byMsgLen = strMsgRW.GetSTRING_LENGTH();

			pTargetUser->SendPacket( (BYTE*)&msgACK, msgACK.GetSize() );
		}
		else
		{
			MessageOut(eCRITICAL_LOG, _T("Target User Block Whisper (%s) to (%s)"),
				pUser->GetCharName().c_str(), strNameRW.GetBUFFER() );

			MSG_CW_WHISPER_NAK msgNAK;
			msgNAK.byReason = MSG_CW_WHISPER_NAK::USER_WHISPER_OFF;
			pUser->SendPacket( (BYTE*)&msgNAK, sizeof(msgNAK) );
		}
		return;
	}

	if(!ProhibitionWordParser::Instance()->CheckFilter( strMsgRW ) )
	{
		MSG_CW_CHAT_NORMAL_NAK msgNAK;
		msgNAK.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;
		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(msgNAK) );

		MessageOut( eCRITICAL_LOG, _T("Whisper Chat Error - USE BAN WORD[%s][CharGuid:%u]"),
			pUser->GetCharName().c_str(), pUser->GetCharGuid());

		return;
	}

	if( 1 )	// ��󿡰� �Ӹ� ����
	{
		MSG_CW_WHISPER_ACK msgACK;

		util::cSTRING_REF_RW tmpWriter1( msgACK.szCharNameFrom, sizeof(msgACK.szCharNameFrom) );
		tmpWriter1.CopyFromString( pUser->GetCharName().c_str() );
		tmpWriter1.MakeSafeString();
		//strNameRW.CopyTo( tmpWriter1 );

		util::cSTRING_REF_RW tmpWriter2( msgACK.szWhisperMsg, sizeof(msgACK.szWhisperMsg) );
		strMsgRW.CopyTo( tmpWriter2 );

		msgACK.byMsgLen = strMsgRW.GetSTRING_LENGTH();

		pUser->SendPacket( (BYTE*)&msgACK, msgACK.GetSize() );
		pTargetUser->SendPacket( (BYTE*)&msgACK, msgACK.GetSize() );
	}

	// GAMELOG
	GAMELOG->WriteWhisperChat( pUser, pUser->GetChannelID(), pTargetUser, strMsgRW.GetBUFFER(), MAX_CHATMSG_SIZE );

	MessageOut( eFULL_LOG, _T("Whisper (%s) to (%s): %s"),
		pUser->GetCharName().c_str(), strNameRW.GetBUFFER(), strMsgRW.GetBUFFER() );
}

Handler_CW_IMPL( CW_NOTICE_SYN )
{
	MSG_CW_NOTICE_SYN *pRecvMsg = (MSG_CW_NOTICE_SYN*)pMsg;

	// ���̰� �ִ밪�� �ѱ�� ������ ���´�.
/*	if( pRecvMsg->byLen > MAX_NOTICE_LEN )
	{
		pUser->Disconnect();
		return;
	}

	// TODO: ���� ������ ������ �����Ѵ�.

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, pRecvMsg->szMsg, pRecvMsg->byLen );

	WorldServer::Instance()->SendNoticeMsg( szNotice );*/
}

// �Ӹ� �Ѱ� ��(����) ó��
Handler_CW_IMPL( CW_WHISPER_SET_SYN )
{
	MSG_CW_WHISPER_SET_SYN *pRecvMsg = (MSG_CW_WHISPER_SET_SYN*)pMsg;

	if(!pUser)
	{
		MessageOut(eCRITICAL_LOG,  "Whisper Set Failed(pUser == NULL)");
		return;
	}

    if(CheckChatBlockZone(pUser) == 0)
        return;

	// �� ������ �Ӹ� ��/�� ���� ����
	pUser->SetWhisper(pRecvMsg->bWhisper);

	// ���������� �˸���
	MSG_CW_WHISPER_SET_ACK	resMsg;

	resMsg.bWhisper = pRecvMsg->bWhisper;

	pUser->SendPacket( (BYTE*)&resMsg, sizeof(MSG_CW_WHISPER_SET_ACK) );

	MessageOut(eCRITICAL_LOG,  "(%s) Whisper Set(%d)", pUser->GetCharName().c_str(), pRecvMsg->bWhisper);
}

// ä�� ��û
Handler_CW_IMPL( CW_CHAT_NORMAL_SYN ) 
{
	MSG_CW_CHAT_NORMAL_SYN	*pRecvMsg = (MSG_CW_CHAT_NORMAL_SYN*)pMsg;

	if(!pUser || eZONETYPE_MAX == pUser->GetStatus())
	{
		MessageOut(eCRITICAL_LOG,  "CW_CHAT_VILLAGE_SYN Failed(pUser == NULL)");
		return;
	}

    if(CheckChatBlockZone(pUser) == 0)
        return;

	//ä�� �������
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

#ifndef _DEBUG
	// ���� ������ ���� 1/10�� ������ �ð��� ���޾� ���� ä���� ����
	if( !pUser->CheckChatTime() )
	{
		return;
	}
#endif

	// �� ��� ���� �Ҽ��̳�?
	Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
	if(!pChannel)
	{
		// �Ҽӵ� ä�� ��� ����
		MessageOut(eCRITICAL_LOG, "%s CW_CHAT_NORMAL_SYN Failed(Channel %u)", pUser->GetCharName().c_str(), pUser->GetChannelID());
		return;
	}

/*
	BaseZone *pZone = pChannel->FindZone(pUser->GetZoneKey());
	if(!pZone)
	{
		MessageOut( eCRITICAL_LOG, "%s CW_CHAT_NORMAL_SYN ����[ZT:%u][ZK:%u]", pUser->GetCharName().c_str(), 
																				pUser->GetStatus(), 
																				pUser->GetZoneKey() );
		return;
	}
*/

	util::cSTRING_REF strMsgRD( pRecvMsg->m_pszChatMsg, sizeof(pRecvMsg->m_pszChatMsg) );

	BYTE byChatMsgLen = pRecvMsg->m_byMsgLength;
	if( !byChatMsgLen || (pRecvMsg->GetSize() != wSize) || (strMsgRD.GetSTRING_LENGTH() != byChatMsgLen) )
	{
		MessageOut( eFULL_LOG, _T("Normal Chat message size is different %u"), byChatMsgLen );
		return;
	}

	if(!ProhibitionWordParser::Instance()->CheckFilter( strMsgRD ))
	{
		MSG_CW_CHAT_NORMAL_NAK	sendMsg;
		sendMsg.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CHAT_NORMAL_NAK) );
		MessageOut( eCRITICAL_LOG, "Normal Chat Error - USE BAN WORD[%s][CharGuid:%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid());
		return;
	}

	//ä�� �������� üũ
	//1. ���� �޼�����..
	if( strcmp( pRecvMsg->m_pszChatMsg, pUser->GetLastChatMsg() ) == 0 )	
	{
		//2. �ð��� 5�ʸ� ��������..
		if( !pUser->IsExpiredChatTimer() )
		{
			MSG_CW_CHAT_NORMAL_NAK	sendMsg;
			sendMsg.m_wErrorCode = RC::RC_CHAT_ANTI_DOBAE;
			pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CHAT_NORMAL_NAK) );
			return;			
		}
	}
	else
	{
		//�޼����� �ٸ���.. Ÿ�̸ӿ� ������ �޼����� ���� �����Ѵ�.
		pUser->SetLastChatMsg( pRecvMsg->m_pszChatMsg );
		pUser->SetChatTimer( 5 * 1000 );		//5��
	}


	// ä�� �޽��� ��Ŷ ����
	MSG_CW_CHAT_NORMAL_BRD msgBRD;

	memset(msgBRD.m_ptszCharName, 0, sizeof(TCHAR) * MAX_CHARNAME_LENGTH);
	_tcsncpy(msgBRD.m_ptszCharName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );	// ���� �޽����̹Ƿ� �״�� ����
	msgBRD.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';

	if( 1 )
	{
		util::cSTRING_REF_RW strSendMsgRW( msgBRD.m_ptszChatMsg, sizeof(msgBRD.m_ptszChatMsg) );
		strSendMsgRW.CopyFrom( strMsgRD );
		strSendMsgRW.MakeSafeString();
		// �� �ҽ��� ���ϸ� ���⿡ +1�� ���ش�...NULL����?
		msgBRD.m_byMsgLength = strSendMsgRW.GetSTRING_LENGTH()+1;
	}

	pUser->SendNormalChat( (BYTE*)&msgBRD, msgBRD.GetSize() );

//	SERVER_KEY ServerKey, tempKey = WorldServer::Instance()->GetServerKey();
//	ServerKey.Set(tempKey.GetWorldID(), pUser->GetChannelID(), tempKey.GetServerType(), tempKey.GetServerID() );

	GAMELOG->WriteNormalChat( pUser, pUser->GetChannelID(), msgBRD.m_ptszChatMsg , MSG_CW_CHAT_NORMAL_BRD::_MAX_CHATMSG_SIZE);
//	GAMELOG->WriteNormalChat( pUser->GetGUID(), pUser->GetStatus(), resMsg.m_ptszCharName, 0, ServerKey, pUser->GetFieldCode(), resMsg.m_ptszChatMsg);

	MessageOut( eFULL_LOG, "CW_CHAT_NORMAL_BRD[%s][C:%u][ZT:%u][ZK:%u]:%s", pUser->GetCharName().c_str(), 
																		pUser->GetChannelID(), 
																		pUser->GetStatus(), 
																		pUser->GetZoneKey(), 
																		msgBRD.m_ptszChatMsg );
}

Handler_CW_IMPL( CW_CHAT_SHOUT_SYN )
{
	MSG_CW_CHAT_SHOUT_SYN* pRecvMsg = (MSG_CW_CHAT_SHOUT_SYN*)pMsg;

	if(!pUser || eZONETYPE_MAX == pUser->GetStatus())
	{
		MessageOut(eCRITICAL_LOG,  "CW_CHAT_SHOUT_SYN Failed(pUser == NULL)");
		return;
	}

    if(CheckChatBlockZone(pUser) == 0)
        return;

	//ä�� �������
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

#ifndef _DEBUG
	// ���� ������ ���� 1/10�� ������ �ð��� ���޾� ���� ä���� ����
	if( !pUser->CheckChatTime() )
	{
		return;
	}
#endif

	BYTE byMsgLength = pRecvMsg->m_byMsgLength;
	util::cSTRING_REF strMsgRD( pRecvMsg->m_pszChatMsg, sizeof(pRecvMsg->m_pszChatMsg) );

	if( !byMsgLength || (pRecvMsg->GetSize() != wSize) || (strMsgRD.GetSTRING_LENGTH() != byMsgLength) )
	{
		MessageOut( eFULL_LOG, _T("Shout Chat message size is different %u"), byMsgLength );
		return;
	}

	// �� ��� ���� �Ҽ��̳�?
	Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
	if( !pChannel )
	{
		// �Ҽӵ� ���� ��� ����
		MessageOut(eCRITICAL_LOG, _T("%s CW_CHAT_SHOUT_SYN Failed[Channel %u][ZT:%u][ZK:%u]"),
			pUser->GetCharName().c_str(), pUser->GetChannelID(), pUser->GetStatus(), pUser->GetZoneKey() );
		return;
	}

	//if( !(byMsgLength < MSG_CW_CHAT_SHOUT_SYN::_MAX_CHATMSG_SIZE) )
	//{
	//	MessageOut(eCRITICAL_LOG, _T("%s CW_CHAT_SHOUT_SYN Length Over(%u)"),
	//		pUser->GetCharName().c_str(), byMsgLength );
	//	return;
	//}

	if( !ProhibitionWordParser::Instance()->CheckFilter( strMsgRD ) )
	{
		MSG_CW_CHAT_NORMAL_NAK msgNAK;

		msgNAK.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;

		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(msgNAK) );
		MessageOut( eCRITICAL_LOG, _T("Shout Chat Error - USE BAN WORD[%s][CharGuid:%u]"),
			pUser->GetCharName().c_str(), pUser->GetCharGuid());
		return;
	}

	// ä�� �޽��� ��Ŷ ����
	MSG_CW_CHAT_SHOUT_BRD msgBRD;

	do
	{
		util::cSTRING_REF_RW strNameRW4BRD( msgBRD.m_szCharName, sizeof(msgBRD.m_szCharName) );
		strNameRW4BRD.CopyFromString( pUser->GetCharName().c_str() );
		strNameRW4BRD.MakeSafeString();

		util::cSTRING_REF_RW strMsgRW4BRD( msgBRD.m_pszChatMsg, sizeof(msgBRD.m_pszChatMsg) );
		strMsgRW4BRD.CopyFrom( strMsgRD );
		strMsgRW4BRD.MakeSafeString();

		msgBRD.m_byMsgLength = strMsgRW4BRD.GetSTRING_LENGTH()+1;
	} while(0);

	MSG_CW_CHAT_SHOUT_NAK msgNAK;

	BOOL bShoutItem = FALSE;
	BOOL bSuperMasterItem = FALSE;
	DWORD dwPCRoom = pUser->GetPCRoomID();

	BOOL bShoutConsumeItem = FALSE;
	if(pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_CHAT_SHOUT_CONSUME) != NULL)	bShoutConsumeItem = TRUE;
	if(pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_CHAT_SHOUT) != NULL)	bShoutItem = TRUE;
	if(pUser->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER) != NULL) bSuperMasterItem = TRUE;

	switch(pRecvMsg->m_byMessageType)
	{
	case pRecvMsg->GENERAL_SHOUT: //@
		{
			GAME_OPTION& stGameOption = GameOptionParser::Instance()->GetGameOption();

			if( stGameOption.m_byNormalShoutLevel == 0 )			//0�̸� �������̴�.
				return;

			if( pUser->GetCharLevel() < stGameOption.m_byNormalShoutLevel )
			{
				msgNAK.m_wErrorCode = RC::RC_GENERAL_SHOUT_LIMIT_LEVEL;
				pUser->SendPacket((BYTE*)&msgNAK, sizeof(msgNAK));
				return;
			}

			if( bSuperMasterItem || pUser->CanApplyPCBangBenefit() )
			{
				switch( pUser->GetStatus())
				{
				case eZONETYPE_LOBBY:
				case eZONETYPE_HUNTING:
				case eZONETYPE_MISSION:
				case eZONETYPE_CHUNTING:
					msgBRD.m_byMessageType = msgBRD.SUPER_MASTER_SHOUT;
					break;
				default:
					msgBRD.m_byMessageType = msgBRD.GENERAL_SHOUT;
				}
			}
			else
			{
				msgBRD.m_byMessageType = msgBRD.GENERAL_SHOUT;
			}	
			pUser->SendNomalChatToAll((BYTE*)&msgBRD, msgBRD.GetSize());

		}break;
	case pRecvMsg->WORLD_SHOUT: //@@
		{
			GAME_OPTION& stGameOption = GameOptionParser::Instance()->GetGameOption();

		#ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
			if( stGameOption.m_byWorldShoutLevel == 0 )			//0�̸� �������̴�.
				return;

			if( pUser->GetCharLevel() < stGameOption.m_byWorldShoutLevel )
			{
				msgNAK.m_wErrorCode = RC::RC_WORLD_SHOUT_LIMIT_LEVEL;
				pUser->SendPacket((BYTE*)&msgNAK, sizeof(msgNAK));
				return;
			}
		#endif
			// �Ҹ� ��ġ�� �������� �ְų�(�Ⱓ���� �ƴϴ�.), PC���ε� PC�濡�� ��ġ�Ⱑ ������ �ɼ��̸�.
			BOOL bNow = FALSE;

            // �Ⱓ�� ��ġ�� �������� �ְų�, PC���ε� PC�濡�� ��ġ�Ⱑ ������ �ɼ��̸�.
			if( bShoutItem || ( pUser->CanApplyPCBangBenefit() && stGameOption.m_byWorldShoutPCBANG ) ) 	
			{
				bNow = TRUE;
			}	
			else if( bShoutConsumeItem )	// �Ҹ� ��ġ�� �������� ������.
			{
				pUser->SetTmpDecreeMsg(msgBRD.m_pszChatMsg);
			}
			else
			{
				msgNAK.m_wErrorCode = RC::RC_CHAT_NOT_DECREE_ITEM;
				pUser->SendPacket((BYTE*)&msgNAK, sizeof(msgNAK) );
				return;
			}

			msgBRD.m_byMessageType = msgBRD.DECREE_SHOUT;
			pUser->SendWorldChatToAll((BYTE*)&msgBRD, msgBRD.GetSize(), bNow);
		}break;
	}	

	GAMELOG->WriteShoutChat( pUser, pUser->GetChannelID(), msgBRD.m_pszChatMsg, MSG_CW_CHAT_SHOUT_BRD::_MAX_CHATMSG_SIZE );
	MessageOut( eFULL_LOG, _T("CW_CHAT_SHOUT_SYN[%s][C:%u][ZK:%u]:%s"), pUser->GetCharName().c_str(), 
																		pUser->GetChannelID(), 
																		pUser->GetZoneKey(), 
																		msgBRD.m_pszChatMsg );
}

// GM ����
Handler_CW_IMPL( CW_GM_NOTICE_SYN )
{
	MSG_CW_GM_NOTICE_SYN *pRecvMsg = (MSG_CW_GM_NOTICE_SYN*)pMsg;

	if(!pUser)
	{
		MessageOut(eCRITICAL_LOG,  "OnCW_GM_NOTICE_SYN Failed(pUser == NULL)");
		return;
	}

	// ��� ä�ÿ��� ũ�Ⱑ 0�̰ų� ���� ������ ������ ����
	if( !pRecvMsg->dwNoticeLength || _tcslen(pRecvMsg->szNotice) )
	{
		MessageOut( eFULL_LOG, "No Notice" );
		return;
	}

	if( pRecvMsg->dwNoticeLength >= MAX_NOTICE_LEN )
	{
		MessageOut(eCRITICAL_LOG,  "Notice Length Over(%d)", pRecvMsg->dwNoticeLength );
		MSG_CW_GM_NOTICE_NAK	nakMsg;
		nakMsg.dwResultCode = MSG_CW_GM_NOTICE_NAK::ERR_NOTICE_DEFAULT;
		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(MSG_CW_GM_NOTICE_NAK) );
		return;
	}

	// GM���� ����
	// ��� ������ GM���� �˻��Ѵ�
	if( pUser->GetGMGrade() <= eGM_GRADE_BASE || pUser->GetGMGrade() >= eGM_GRADE_MAX )
	{
		// �� ���� ũ��ŷ�̳� �������� Ÿ�� ������ ���ɼ��� �ִ�
		MessageOut(eCRITICAL_LOG, "Invalid User Request Notice(Not GM)!!!(%s)", pUser->GetCharName().c_str());
		return;
	}

	MSG_CW_NOTICE_BRD noticeMsg;
    // (WARNING) buffer overrun ���輺 �����ϰ� �ִ�. �켱 ���� ���� ó�� �����Ѵ�.
    // (NOTE) MAX_NOTICE_LEN = 1024, _MAX_CHATMSG_SIZE = 255
    const size_t MessageLength = min(_countof(noticeMsg.szMsg), _countof(pRecvMsg->szNotice));
    BOOST_STATIC_ASSERT(MessageLength < USHRT_MAX);
    strncpy(noticeMsg.szMsg, pRecvMsg->szNotice, MessageLength);
    noticeMsg.szMsg[MessageLength - 1] = '\0';
    // CHANGES: f101210.2L, consider length with null-terminated position
    if (size_t str_len = strlen(noticeMsg.szMsg) + 1) {
        // NOTE: always true, because wLen need consider with null-terminated position
        noticeMsg.wLen = static_cast<uint16_t>(str_len);
    }

	// GM���� Ȯ�εǸ� ���� ���� �����Ͽ� �����ش�
	switch(pRecvMsg->byNoticeType)
	{
	case MSG_CW_GM_NOTICE_SYN::NOTICE_WORLD:
		{
			// ���� ��ü ����-���� �ִ� ��� �����鿡�� ������
			UserManager::Instance()->SendToAll( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

			char szNotice[MAX_NOTICE_LEN + 1];
			ZeroMemory( szNotice, sizeof(szNotice) );
			strncpy( szNotice, noticeMsg.szMsg, pRecvMsg->dwNoticeLength );
			MessageOut(eCRITICAL_LOG,  "-----------------------------<< WorldNotice >>-----------------------------" );
			MessageOut(eCRITICAL_LOG,  "%s(%u)", noticeMsg.szMsg, noticeMsg.wLen );
			MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );
			break;
		}
		case MSG_CW_GM_NOTICE_SYN::NOTICE_CHANNEL:
		{
			// �ش� ������ ������ ä�ο��� �����ش�
			Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
			if(!pChannel)
			{
				// ������� ä�� ID�� Ʋ�ȴ�. ġ������ ����
				MessageOut( eCRITICAL_LOG, "Invalid Channel ID for User %s[%u]!! Channel Notice Failed!!", pUser->GetCharName().c_str(), pUser->GetChannelID() );
				return;
			}
			// ����, ��Ʋ�� ��� ����
			pChannel->SendToAll( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

			char szNotice[MAX_NOTICE_LEN + 1];
			ZeroMemory( szNotice, sizeof(szNotice) );
			strncpy( szNotice, noticeMsg.szMsg, pRecvMsg->dwNoticeLength );
			MessageOut(eCRITICAL_LOG,  "----------------------------<< %d Channel Notice >>----------------------------", pUser->GetChannelID() );
			MessageOut(eCRITICAL_LOG,  "%s(%u)", noticeMsg.szMsg, noticeMsg.wLen );
			MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );
			break;
		}
		case MSG_CW_GM_NOTICE_SYN::NOTICE_ZONE:
		{
			// ���� ������ �������� ����
			Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
			if(!pChannel)
			{
				// ������� ä�� ID�� Ʋ�ȴ�. ġ������ ����
				MessageOut( eCRITICAL_LOG, "Invalid Channel ID for User %s[%u]!! Zone Notice Failed!!", pUser->GetCharName().c_str(), pUser->GetChannelID() );
				return;
			}
			BaseZone *pZone = NULL;
#ifdef _DEBUG
			assert( pUser->GetStatus() < eZONETYPE_MAX && "invalid user status" );
#else
			if( pUser->GetStatus() >= eZONETYPE_MAX )
			{
				MessageOut( eCRITICAL_LOG, "Notive Error - Invalid User Status[C:%u][ZT:%u][ZK:%u]", pUser->GetChannelID(), pUser->GetStatus(), pUser->GetZoneKey() );
				break;
			}
#endif
			pZone = pChannel->FindZone( pUser->GetZoneKey() );
			if(!pZone)
			{
				// ������� Zone ID�� Ʋ�ȴ�. ġ������ ����
				MessageOut( eCRITICAL_LOG, "Invalid Zone ID for User %s[%u]!! Zone Notice Failed!!", pUser->GetCharName().c_str(), 
																				pUser->GetZoneKey() );
				return;
			}
			pZone->SendToAll( (BYTE*)&noticeMsg, noticeMsg.GetSize() );

			char szNotice[MAX_NOTICE_LEN + 1];
			ZeroMemory( szNotice, sizeof(szNotice) );
			strncpy( szNotice, noticeMsg.szMsg, pRecvMsg->dwNoticeLength );
			MessageOut(eCRITICAL_LOG,  "---------------------------<< %u Channel [ZT:%u][ZK:%u] Notice >>-------------------------", 
											pUser->GetChannelID(), 
											pUser->GetStatus(), 
											pUser->GetZoneKey() );
			MessageOut(eCRITICAL_LOG,  "%s(%u)", noticeMsg.szMsg, noticeMsg.wLen );
			MessageOut(eCRITICAL_LOG,  "------------------------------------------------------------------------" );

			break;
		}
		default:
		{
			MessageOut(eCRITICAL_LOG, "Invalid Type of GM Notice!!!(%s:%u)", pUser->GetCharName().c_str(), pRecvMsg->byNoticeType);
			return;
			break;
		}
	}
}

// ��Ʈ�� �Ľ��� ���� GM ���
Handler_CW_IMPL( CW_GM_STRING_CMD_SYN )
{
	MSG_CW_GM_STRING_CMD_SYN* pRecvMsg = (MSG_CW_GM_STRING_CMD_SYN*)pMsg;
	m_GMCmdManager.ParseCommand( pUser, pRecvMsg->m_szStringCmd );
}

Handler_CW_IMPL( CW_VIEWPORT_CHARSTATE )
{
	MSG_CW_VIEWPORT_CHARSTATE	*pRecvMsg = (MSG_CW_VIEWPORT_CHARSTATE*)pMsg;

	// �ϴ� ��ŷ ���������� ���� User State Ȯ��
	if( pUser->GetStatus() != eZONETYPE_VILLAGE &&
        pUser->GetStatus() != eZONETYPE_DOMINATION_FIELD &&
        pUser->GetStatus() != eZONETYPE_FIELD &&
        pUser->GetStatus() != eZONETYPE_SPA_FIELD)
	{
		MessageOut( eFULL_LOG, "VIEWPORT_CHARSTATE Sync Error - Status[Guid:%u][CharGuid:%u][Status:%u][F:%u]", 
																						pUser->GetGUID(), 
																						pUser->GetCharGuid(), 
																						pUser->GetStatus(), 
																						pRecvMsg->dwFieldCode );
		return;
	}

	Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
	if( !pChannel )
	{
		MessageOut(eCRITICAL_LOG, "CHARSTATE Error - No Channel[Guid:%u][CharGuid:%u][C:%u]", 
																			pUser->GetGUID(), 
																			pUser->GetCharGuid(), 
																			pUser->GetChannelID() );
		return;
	}

	ChatZone* pChatZone = pChannel->FindZone( pUser->GetZoneKey() );
	if( !pChatZone )
	{
		if( 0 == pUser->GetZoneKey() ||  7000000 < pRecvMsg->dwSectorIndex)
		{
			ServerSession* pAgentServer = ServerSessionManager::Instance()->FindAgentServer(WorldServer::Instance()->GetChannelID());
			if(pAgentServer)
			{
				MSG_AW_ABUSING_USER_VERIFY_QUERY_CMD msg;
				msg.m_dwKey = pUser->GetGUID();
				msg.m_CharGuid = pUser->GetCharGuid();
				msg.m_eAbuseSubType = eABUSE_SUB_LOGIC_VIOLATION_WRONG_SECTOR_WORLDSVR;
				pAgentServer->Send((BYTE*)&msg, sizeof(msg));
			}
			else
			{
				MessageOut( eCRITICAL_LOG, "SectorIndex  Client Error - Status[Guid:%u][CharGuid:%u][ZK:%u][FieldIdx:%u][SectorIdx:%u]", 
					pUser->GetGUID(), pUser->GetCharGuid(), pUser->GetZoneKey(),
					pRecvMsg->dwFieldCode,
					pRecvMsg->dwSectorIndex);
			}
		}
		else
		{
			MessageOut(eCRITICAL_LOG, "CHARSTATE Error - No ChatZone[Guid:%u][CharGuid:%u][C:%u][ZK:%u][SectorKey:%u]", 
				pUser->GetGUID(), 
				pUser->GetCharGuid(), 
				pUser->GetChannelID(), 
				pUser->GetZoneKey(),
				pUser->GetSectorIndex()
				);
		}
		return;
	}

	// ���� �ִ� ������ �̵����� Ȯ��
	if( pUser->GetSectorIndex() == -1 )
	{
		// ���� �ִ� ��
		pUser->SetFieldCode( pRecvMsg->dwFieldCode );
		pUser->SetSectorIndex( pRecvMsg->dwSectorIndex );

		pChatZone->AddUser( pUser->GetGUID(), pUser );
		MessageOut(eFULL_LOG, "CHARSTATE AddUser [Guid:%u][CharGuid:%u][F:%u][S:%u]", 
																			pUser->GetGUID(), 
																			pUser->GetCharGuid(), 
																			pRecvMsg->dwFieldCode, 
																			pRecvMsg->dwSectorIndex );
	}
	else
	{
		// �̵��� ��� ���� ChatZone���� ���ٰ� ���ο� ChatZone���� �����Ͽ� �ٽ� �־��ش�
		// �̳��� �÷��̾ �ִ� Sector�� ViewPortZone
		BaseZone *pZone = pUser->GetNormalChatZone();
#ifdef _DEBUG
		assert( pZone && "MoveUser Error - No NormalChatZone" );
#else
		if( !pZone )
		{
			MessageOut( eCRITICAL_LOG, "CHARSTATE MoveUser Error - No NormalChatZone[Guid:%u][CharGuid:%u][C:%u][ZK:%u]", 
																				pUser->GetGUID(), 
																				pUser->GetCharGuid(), 
																				pUser->GetChannelID(), 
																				pUser->GetZoneKey() );
			return;
		}
#endif
		// �ִ� ViewPortZone���� ���� �� �ְ�, ���ο� ViewPortZone�� �־��ش�(�� ������ ChatZone�� �Ѵ�)
		pZone->RemoveUser( pUser->GetGUID() );
		pUser->SetFieldCode( pRecvMsg->dwFieldCode );
		pUser->SetSectorIndex( pRecvMsg->dwSectorIndex );
		pChatZone->AddUser( pUser->GetGUID(), pUser );
		MessageOut(eFULL_LOG, "CHARSTATE MoveUser [Guid:%u][CharGuid:%u][F:%u][S:%u]", pUser->GetGUID(), 
																			pUser->GetCharGuid(), 
																			pRecvMsg->dwFieldCode, 
																			pRecvMsg->dwSectorIndex );
	}

}

#ifdef _DEBUG
#include <ArmorKit.h>
#endif

// Armorkit���� ��ŷ �õ� �߰�
Handler_CW_IMPL( CW_ARMORKIT_FINDHACK_SYN )
{
}

// ģ�� ��� ��û
Handler_CW_IMPL( CW_FRIEND_NEWLIST_SYN )
{
	if( !pUser->CheckFriendList() )	// �̹� ģ�� ����� ���� ���� �ִٸ� ��ŷ �õ� �Ǵ� �߸��� ��Ŷ
	{
		MessageOut( eCRITICAL_LOG, "Friend_NewList Error - Already Sent List... U Hacker?(%s)", pUser->GetCharName().c_str() );
		return;
	}

	MSG_CW_FRIEND_FRIENDLIST_ACK	sendFriend;
	MSG_CW_FRIEND_BLOCKLIST_ACK		sendBlock;
	MSG_CW_REVENGE_LIST_CMD			sendRevenge;
	User*	pFriend = NULL;

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	sendFriend.byFriendStatNum = pUser->GetFriendMap()->GetFriendNum();
#else
	sendFriend.byFriendStatNum = pUser->GetFriendNum();
#endif
	
	sendBlock.byBlockStatNum = pUser->GetBlockNum();
	sendRevenge.byRevengeStatNum = pUser->GetRevengeNum();
	sendRevenge.byRevengePoint = pUser->GetRevengePoint();

    //  MAX ġ ���� ģ�� �߰� �����ϳ� �߰��� ģ�� ������ �������� ���� �ʴ� ���� [9/11/2009 lst1024]
	if( sendFriend.byFriendStatNum > MAX_FRIENDBLOCK_STAT_NUM || 
		sendBlock.byBlockStatNum > MAX_BLOCK_STAT_NUM || 
		sendRevenge.byRevengeStatNum > MAX_REVENGE_STAT_NUM )
	{
		SUNLOG( eCRITICAL_LOG, "[Handler_CW::OnCW_FRIEND_NEWLIST_SYN] Overflow, byFriendStatNum = %u, byBlockStatNum = %u, \
			byRevengeStatNum = %u", sendFriend.byFriendStatNum, sendBlock.byBlockStatNum, sendRevenge.byRevengeStatNum );
		MSG_CW_FRIEND_NEWLIST_NAK	nak;
		nak.dwResult = 1;
		pUser->SendPacket( (BYTE*)&nak, sizeof(nak) );
		return;
	}

    //{__CN_000001_20090513_ADD_FRIEND_CHECK_GM
    INT nCopyBlockInfo   = pUser->CopyBlockInfo( sendBlock.sBlockStat, MAX_BLOCK_STAT_NUM );
    INT nCopyFriendInfo  = pUser->CopyFriendInfo( sendFriend.sFriendStat, MAX_FRIENDBLOCK_STAT_NUM );
    INT nCopyRevengeInfo = pUser->CopyRevengeInfo( sendRevenge.sRevengeStat, MAX_REVENGE_STAT_NUM );

    //(lst1024)(090611)�α� �߸� ����� ����
    if( nCopyFriendInfo != sendFriend.byFriendStatNum )
        SUNLOG( eCRITICAL_LOG, "[%s][Guid:%u] Friend List Count Err Dst[%d] != Src[%d]",
                pUser->GetCharName().c_str(), pUser->GetGUID(), nCopyFriendInfo, sendFriend.byFriendStatNum );

    if( nCopyBlockInfo != sendBlock.byBlockStatNum )
        SUNLOG( eCRITICAL_LOG, "[%s][Guid:%u] Block List Count Err Dst[%d] != Src[%d]",
                pUser->GetCharName().c_str(), pUser->GetGUID(), nCopyBlockInfo, sendBlock.byBlockStatNum );

    if( nCopyRevengeInfo != sendRevenge.byRevengeStatNum )
        SUNLOG( eCRITICAL_LOG, "[%s][Guid:%u] Revenge List Count Err Dst[%d] != Src[%d]",
                pUser->GetCharName().c_str(), pUser->GetGUID(), nCopyRevengeInfo, sendRevenge.byRevengeStatNum );
    //}

	// ģ�� ���� ���� Ȯ�� �������� ������
	pUser->SendPacket( (BYTE*)&sendFriend, sendFriend.GetSize() );
    //������Ʈ ������ ģ����õ ����� �����Ѵ�. (W->A->G), _KR_0_20091021_FIRENDINVITATION_EXTEND
    WFriendInvitationController::Instance()->SendInvitateFriendList(pUser, SEND_CLIENT);
	pUser->SendPacket( (BYTE*)&sendBlock, sendBlock.GetSize() );
	pUser->SendPacket( (BYTE*)&sendRevenge, sendRevenge.GetSize() );
	MessageOut( eFULL_LOG, "[%s][Guid:%u] Friend/Block/Revenge List Send", pUser->GetCharName().c_str(), pUser->GetGUID() );



/*
    // ģ�� ��� ��û
	MSG_DW_FRIEND_GETLIST_SYN	sendMsg;

	sendMsg.m_CharGuid = pUser->GetCharGuid();

	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MessageOut( eCRITICAL_LOG, "Friend_NewList Error - No GameDBProxy");
		return;
	}
	pGameDBProxy->Send( (BYTE*)&sendMsg, sendMsg.GetSize() );

	MessageOut(eFULL_LOG, "Friend_NewList[%s][CharGuid:%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
*/
}

// ģ�� ��û
Handler_CW_IMPL( CW_FRIEND_ADD_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}

	MSG_CW_FRIEND_ADD_SYN	*pRecvMsg = (MSG_CW_FRIEND_ADD_SYN*)pMsg;

	// �⺻������ �̹� �ִ� ģ������, �ƴϸ� �� �ڽ����� ������ �˻��� �ִ� �뾲!!!
	std::string strName = pUser->GetCharName();

	if( !_tcsnicmp( strName.c_str(), pRecvMsg->ptszCharNameTo, MAX_CHARNAME_LENGTH ) )
	{  
		// �� �ڽ��̸� ����
		MSG_CW_FRIEND_ADD_NAK	sendMsg;
		sendMsg.m_byResult = MSG_CW_FRIEND_ADD_NAK::FRIEND_IS_ME;	// ���� ģ����?!
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_ADD_NAK) );
		MessageOut( eCRITICAL_LOG, "[Handler_CW::OnCW_FRIEND_ADD_SYN] [%s] Friend ADD Error - Friend Request to Myself", pRecvMsg->ptszCharNameTo );
		return;
	}

	// �̹� �ִ� ģ���� ����
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	if( pUser->GetFriendMap()->FindFriend( pRecvMsg->ptszCharNameTo ) )
#else
	if( pUser->FindFriend( pRecvMsg->ptszCharNameTo ) )
#endif	
	{
		MSG_CW_FRIEND_ADD_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_ADD_NAK::ALREADY_FRIEND;	// �̹� ģ����!

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_ADD_NAK) );
		MessageOut( eFULL_LOG, "[%s] Friend ADD Error - Already Friends", pRecvMsg->ptszCharNameTo );
		return;
	}

	// ģ�� �ִ� ������ 90��
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	if( pUser->GetFriendMap()->GetFriendNum() >= MAX_FRIEND_STAT_NUM )
#else
	if( pUser->GetFriendNum() >= MAX_FRIEND_STAT_NUM )
#endif	
	{
		MSG_CW_FRIEND_ADD_NAK	sendMsg;

#ifdef _FRIEND_OVERFLOW
		sendMsg.m_byResult = MSG_CW_FRIEND_ADD_NAK::FRIEND_OVERFLOW;
#else
		sendMsg.m_byResult = MSG_CW_FRIEND_ADD_NAK::ERR_DEFAULT;
#endif

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_ADD_NAK) );
		MessageOut( eFULL_LOG, "[%s] Friend ADD Error - Friend Number Overflow", pRecvMsg->ptszCharNameTo );
		return;

	}

	// �ϴ� ��~�� DB�� ����~
	MSG_DW_FRIEND_ADD_SYN	sendMsg;

	sendMsg.m_CharGuidFrom = pUser->GetCharGuid();
	_tcsncpy( sendMsg.m_ptszCharNameTo, pRecvMsg->ptszCharNameTo, MAX_CHARNAME_LENGTH );
	sendMsg.m_ptszCharNameTo[MAX_CHARNAME_LENGTH] = '\0';

	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MessageOut( eCRITICAL_LOG,  "Friend ADD Error - No GameDBProxy(User %s)", pUser->GetCharName().c_str() );
		return;
	}
	pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(MSG_DW_FRIEND_ADD_SYN) );

	pUser->SetFriendTransaction( TRUE );

	MessageOut(eFULL_LOG,  "Friend ADD to GameDBProxy(%s->%s)", pUser->GetCharName().c_str(), sendMsg.m_ptszCharNameTo );
}

// ģ�� ����
Handler_CW_IMPL( CW_FRIEND_DEL_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}


	MSG_CW_FRIEND_DEL_SYN		*pRecvMsg = (MSG_CW_FRIEND_DEL_SYN*)pMsg;
	

	// ģ�� �¾�?
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	STRUCT_FRIEND_INFO* pFriendInfo = pUser->GetFriendMap()->FindFriend( pRecvMsg->ptszFriendName );
#else
	STRUCT_FRIEND_INFO* pFriendInfo = pUser->FindFriend( pRecvMsg->ptszFriendName );
#endif
	
	if( !pFriendInfo )
	{
		MSG_CW_FRIEND_DEL_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_DEL_NAK::NOT_FRIEND_USER;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
		MessageOut( eCRITICAL_LOG, "Friend Del Error - No Friend[%s][CharGuid:%u]->[%s]", pUser->GetCharName().c_str(), 
																				pUser->GetCharGuid(),
																				pRecvMsg->ptszFriendName );
		return;
	}

	MSG_DW_FRIEND_DEL_SYN	sendMsg;

	sendMsg.m_CharGuidFrom	= pUser->GetCharGuid();
	sendMsg.m_CharGuidTo	= pFriendInfo->dwFriendGuid;
    // added data field to support _KR_0_20091021_FIRENDINVITATION_EXTEND //ģ�� ���� 
	sendMsg.m_Sts			= WFriendInvitationController::Instance()->GetFriendSts(pUser, pFriendInfo->dwFriendGuid);

	// DB�� ��û~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_FRIEND_DEL_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_DEL_NAK::ERR_DEFAULT;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_DEL_NAK) );
		MessageOut( eCRITICAL_LOG,  "Friend Del Error - No GameDBProxy(User %s)", pUser->GetCharName().c_str() );
		return;
	}
	pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(MSG_DW_FRIEND_DEL_SYN) );

	pUser->SetFriendTransaction( TRUE );

	MessageOut(eFULL_LOG,  "Friend NO to GameDBProxy(%s->%s)", pUser->GetCharName().c_str(), pFriendInfo->sPacketInfo.ptszFriendName );

}

// ���� ��û
Handler_CW_IMPL( CW_FRIEND_BLOCK_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}

	// �׳� �����ϸ� ��~
	MSG_CW_FRIEND_BLOCK_SYN*	pRecvMsg = (MSG_CW_FRIEND_BLOCK_SYN*)pMsg;

	// �⺻���� ���� ���� �˻�
	if( _tcslen(pRecvMsg->ptszBlockName) > MAX_CHARNAME_LENGTH )
	{
		MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::ERR_DEFAULT;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Block Error - BlockName Overflow", pUser->GetCharName().c_str() );
		return;
	}

	// Ȥ�� �� �ڽ��� �����ϴ� ���� �ƴϰ���?
	if( !_tcsnicmp( pRecvMsg->ptszBlockName, pUser->GetCharName().c_str(), strlen(pUser->GetCharName().c_str()) ) )
	{
		//�׷��� ���аŵ�!!
		MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::BLOCKED_ME;	// �� �ڽ��� �����ߴٰ�!!!

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Block Error - Block Myself", pUser->GetCharName().c_str() );
		return;
	}

	// �̹� ��ϵ� ���̸� ��� �Ұ�
	if( pUser->FindBlock( pRecvMsg->ptszBlockName ) )
	{
		MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::ALREADY_BLOCKED;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Block Error - Already Blocked[%s]", pUser->GetCharName().c_str(), pRecvMsg->ptszBlockName );
		return;
	}

	// ��� �ִ� ������ 90��
	if( pUser->GetBlockNum() >= MAX_BLOCK_STAT_NUM )
	{
		MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

#ifdef _FRIEND_OVERFLOW
		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::BLOCK_OVERFLOW;
#else
		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::ERR_DEFAULT;
#endif

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );
		MessageOut( eFULL_LOG, "[%s] Block Error - Block Number Overflow", pRecvMsg->ptszBlockName );
		return;

	}

	MSG_DW_FRIEND_BLOCK_SYN		sendMsg;

	sendMsg.m_CharGuidFrom	= pUser->GetCharGuid();
	_tcsncpy( sendMsg.m_ptszCharNameTo, pRecvMsg->ptszBlockName, MAX_CHARNAME_LENGTH );
	sendMsg.m_ptszCharNameTo[MAX_CHARNAME_LENGTH] = '\0';

	// DB�� ��û~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::ERR_DEFAULT;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );
		MessageOut( eCRITICAL_LOG,  "Friend Block Error - No GameDBProxy(User %s)", pUser->GetCharName().c_str() );
		return;
	}
	pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(MSG_DW_FRIEND_BLOCK_SYN) );

	pUser->SetFriendTransaction( TRUE );

	MessageOut(eFULL_LOG,  "Friend Block to GameDBProxy(%s->%s)", pUser->GetCharName().c_str(), sendMsg.m_ptszCharNameTo );

}

// ���� ���� ��û
Handler_CW_IMPL( CW_FRIEND_BLOCK_FREE_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}

	MSG_CW_FRIEND_BLOCK_FREE_SYN*	pRecvMsg = (MSG_CW_FRIEND_BLOCK_FREE_SYN*)pMsg;

	// ���� ���̱� �ϳ�?
	STRUCT_BLOCK_INFO* pBlockInfo = pUser->FindBlock( pRecvMsg->ptszBlockName );

	// ���� ������ ������
	if( !pBlockInfo)
	{
		// ������
		MSG_CW_FRIEND_BLOCK_FREE_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_FREE_NAK::NOT_BLOCKED;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_FREE_NAK) );
		return;
	}

	// ��Ȯ�� ���� ������ ������ ���� ���� ��û�� DB�� ������
	MSG_DW_FRIEND_BLOCK_FREE_SYN	sendMsg;

	sendMsg.m_CharGuidFrom = pUser->GetCharGuid();
	sendMsg.m_CharGuidTo = pBlockInfo->dwBlockGuid;

	// DB�� ��û~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_FRIEND_BLOCK_FREE_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::ERR_DEFAULT;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_FREE_NAK) );
		MessageOut( eCRITICAL_LOG,  "Friend Block Free Error - No GameDBProxy(User %s)", pUser->GetCharName().c_str() );
		return;
	}
	pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(MSG_DW_FRIEND_BLOCK_FREE_SYN) );

	pUser->SetFriendTransaction( TRUE );

	MessageOut(eFULL_LOG,  "Friend Block Free to GameDBProxy(%s->%s)", pUser->GetCharName().c_str(), pBlockInfo->ptszBlockName );

}

// ģ�� ä��
Handler_CW_IMPL( CW_FRIEND_CHAT_SYN )
{
	MSG_CW_FRIEND_CHAT_SYN*	pRecvMsg = (MSG_CW_FRIEND_CHAT_SYN*)pMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	util::cSTRING_REF strFriendNameRD( pRecvMsg->ptszFriendNameTo, sizeof(pRecvMsg->ptszFriendNameTo) );
	util::cSTRING_REF_RW_WITH_BUFFER<sizeof(CHAR)*(MAX_CHARNAME_LENGTH+1)> strFriendNameRW;
	strFriendNameRD.CopyTo( strFriendNameRW );
	strFriendNameRW.MakeSafeString();

	// ��� ä�ÿ��� ũ�Ⱑ 0�̰ų� ���� ������ ������ ����
//	if( !pRecvMsg->byMsgLength || !_tcslen(pRecvMsg->ptszChatMsg) )
//	{
//		MessageOut( eFULL_LOG, "ä�� ���� ����" );
//		return;
//	}

	// �¶������� Ȯ��
	User *pFriend = UserManager::Instance()->FindUser( strFriendNameRW.GetBUFFER() );
    //__KR_001355_20090608_GROUPINGSYSTEM_CW
    STRUCT_GROUP_MEMBER_INFO* pMember = NULL;
    if (pFriend != NULL) {
        pMember = GroupManager::Instance()->FindGroupMember( pUser, pFriend->GetCharGuid() );
    }
    if( ( !pFriend || eZONETYPE_MAX == pFriend->GetStatus() ) && !pMember) //�׷� �ɹ����� ��ȭâ�� ����
	{
		// �¶��� ���̴�!!
		MSG_CW_FRIEND_CHAT_NAK msgNAK;

		util::cSTRING_REF_RW strFriendNameToRW( msgNAK.ptszFriendNameTo, sizeof(msgNAK.ptszFriendNameTo) );
		strFriendNameRW.CopyTo( strFriendNameToRW );

		msgNAK.m_byResult = MSG_CW_FRIEND_CHAT_NAK::OFFLINE_FRIEND;

		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(msgNAK) );
		MessageOut( eCRITICAL_LOG, _T("Friend Chat Error[%s][CharGuid:%u] - Friend Offline[%s]"),
			pUser->GetCharName().c_str(), pUser->GetCharGuid(), strFriendNameRW.GetBUFFER() );

		return;
	}

	// �¶����̸� ģ����?
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING	
	STRUCT_FRIEND_INFO* pFriendInfo = pUser->GetFriendMap()->FindFriend( strFriendNameRW.GetBUFFER() );
#else
	STRUCT_FRIEND_INFO* pFriendInfo = pUser->FindFriend( strFriendNameRW.GetBUFFER() );
#endif
	

    if( !pFriendInfo && !pMember )
	{

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING			
		if( !pFriend->GetFriendMap()->FindFriend( pUser->GetCharGuid() ) )
#else
		if( !pFriend->FindFriend( pUser->GetCharGuid() ) )
#endif		
		{
			// ��뿡�Ե� ģ�� ���̴�!!
			MSG_CW_FRIEND_CHAT_NAK msgNAK;

			util::cSTRING_REF_RW strFriendNameToRW( msgNAK.ptszFriendNameTo, sizeof(msgNAK.ptszFriendNameTo) );
			strFriendNameRW.CopyTo( strFriendNameToRW );

			msgNAK.m_byResult = MSG_CW_FRIEND_CHAT_NAK::NOT_FRIEND;

			pUser->SendPacket( (BYTE*)&msgNAK, sizeof(msgNAK) );
			MessageOut( eCRITICAL_LOG, _T("Friend Chat Error[%s][CharGuid:%u] - Not Friend[%s]"),
				pUser->GetCharName().c_str(), pUser->GetCharGuid(), strFriendNameRW.GetBUFFER() );

			return;
		}
	}

    if( ( pFriend->GetFriendChatBlock() || pFriend->FindBlock( pUser->GetCharGuid() ) ) && !pMember )
	{
		MSG_CW_FRIEND_CHAT_NAK msgNAK;

		util::cSTRING_REF_RW strFriendNameToRW( msgNAK.ptszFriendNameTo, sizeof(msgNAK.ptszFriendNameTo) );
		strFriendNameRW.CopyTo( strFriendNameToRW );

		msgNAK.m_byResult = MSG_CW_FRIEND_CHAT_NAK::BLOCK_CHAT;

		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(msgNAK) );
		MessageOut( eCRITICAL_LOG, _T("Friend Chat Error[%s][CharGuid:%u] - Chat Block[%s]"),
			pUser->GetCharName().c_str(), pUser->GetCharGuid(), strFriendNameRW.GetBUFFER() );

		return;
	}

	util::cSTRING_REF strMsgRD( pRecvMsg->ptszChatMsg, sizeof(pRecvMsg->ptszChatMsg) );

	if(!ProhibitionWordParser::Instance()->CheckFilter( strMsgRD ))
	{
		MSG_CW_CHAT_NORMAL_NAK msgNAK;
		msgNAK.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;

		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(msgNAK) );
		MessageOut( eCRITICAL_LOG, _T("Friend Chat Error - USE BAN WORD[%s][CharGuid:%u]"),
			pUser->GetCharName().c_str(), pUser->GetCharGuid() );

		return;
	}

	// �� ���� ������
	MSG_CW_FRIEND_CHAT_BRD msgBRD;
	BYTE byMsgLength = pRecvMsg->byMsgLength;

	if( !(MSG_CW_FRIEND_CHAT_SYN::_MAX_CHATMSG_SIZE > byMsgLength) )
	{
		MessageOut( eCRITICAL_LOG, _T("%s CW_FRIEND_CHAT_SYN Length Over(%u)"),
			pUser->GetCharName().c_str(), byMsgLength );
		return;
	}

	util::cSTRING_REF_RW strMsgRW( msgBRD.ptszChatMsg, sizeof(msgBRD.ptszChatMsg) );
	strMsgRD.CopyTo( strMsgRW );
	strMsgRW.MakeSafeString();

	msgBRD.byMsgLength = strMsgRW.GetSTRING_LENGTH()+1;

	util::cSTRING_REF_RW strFriendNameFrom( msgBRD.ptszFriendNameFrom, sizeof(msgBRD.ptszFriendNameFrom) );
	strFriendNameFrom.CopyFromString( pUser->GetCharName().c_str() );
	strFriendNameFrom.MakeSafeString();

	// ģ�� ä���� ���濡�Ը� �����ش�. ���� ���� �׳� ���� �˰� ������ �˾Ƽ� �������!
	//pUser->SendPacket( (BYTE*)&sendMsg, sendMsg.GetSize() );
	pFriend->SendPacket( (BYTE*)&msgBRD, msgBRD.GetSize() );

//	SERVER_KEY ServerKey = WorldServer::Instance()->GetServerKey();
//	ServerSession *pAgentServer = ServerSessionManager::Instance()->FindAgentServer( pUser->GetChannelID() );
//	if( pAgentServer )
//	{
//		ServerKey = pAgentServer->GetServerKey();
//	}

	GAMELOG->WriteFriendChat( pUser, pUser->GetChannelID(), pFriend, strMsgRW.GetBUFFER(), MSG_CW_FRIEND_CHAT_BRD::_MAX_CHATMSG_SIZE );
	MessageOut( eFULL_LOG, _T("Friend Chat[%s -> %s]: %s"),
		pUser->GetCharName().c_str(), pFriend->GetCharName().c_str(), strMsgRW.GetBUFFER() );
}

// 1:1 ��ȭ �ź� üũ
Handler_CW_IMPL( CW_FRIEND_CHATBLOCK_SYN )
{
	// �ش� ������ 1:1��ȭ�� �ź��Ѵ�
	MSG_CW_FRIEND_CHATBLOCK_SYN* pRecvMsg = (MSG_CW_FRIEND_CHATBLOCK_SYN*)pMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	pUser->SetFriendChatBlock( pRecvMsg->m_bBlock );

	MSG_CW_FRIEND_CHATBLOCK_ACK sendMsg;

	sendMsg.m_bNowBlock = pRecvMsg->m_bBlock;

	pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_CHATBLOCK_ACK) );

	MessageOut( eFULL_LOG, "[%s] Friend Chat Block(%u)", pUser->GetCharName().c_str(), sendMsg.m_bNowBlock );
}



Handler_CW_IMPL( CW_GUILD_CHAT_SYN )
{
	MSG_CW_GUILD_CHAT_SYN* pRecvMsg = (MSG_CW_GUILD_CHAT_SYN*)pMsg;
	MSG_CW_GUILD_CHAT_NAK	resMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	// ���  ũ�Ⱑ 0�̰ų� ���� ������ ������ ����
//	if( !pRecvMsg->m_byMsgLen || !_tcslen(pRecvMsg->m_ptszCharMsg) )
//	{
//		MessageOut( eFULL_LOG, "ä�� ���� ����" );
//		return;
//	}

	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

	//ä�� �������
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

	if( pUser->GetGuildGuid() == 0 )	// ��� �Ҽ��� �ƴ϶�� ���´ٸ�?
	{
		// ��� ������ ��û�ؾ� �Ѵ�
		resMsg.byResult = MSG_CW_GUILD_CHAT_NAK::ERR_WAIT_INFO;	// ��� �Ŀ� �ٽ� �õ��Ͻʽÿ�

		pUser->SendPacket( (BYTE*)&resMsg, sizeof(MSG_CW_GUILD_CHAT_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Guild Chat Error - No Guild %u", pUser->GetCharName().c_str(), pUser->GetGuildGuid() );

		// ��� ��� ���� �ڵ�� ���� ���� �κ����� �Űܰ���
/*
		// ��� ��� ���� ��û
		MSG_WZ_GUILD_SELECT_SYN ToGuildMsg;
		ToGuildMsg.m_dwKey = pUser->GetGUID();
		ToGuildMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
		ToGuildMsg.m_CharGuid = pUser->GetCharGuid();
		ServerSession *pGuildServer = ServerSessionManager::Instance()->GetGuildServer();

		if( !pGuildServer )
		{
			MessageOut( eCRITICAL_LOG, "GUILD_MEMBER_SYN ��弭�� ���� ���� ��û �Ұ�!" );
			return;
		}
		pGuildServer->Send( (BYTE*)&ToGuildMsg, sizeof(MSG_WZ_GUILD_SELECT_SYN) );

		MessageOut( eFULL_LOG, "GUILD_CHAT_SYN �������[%u] ��� ��û[%s]", pRecvMsg->m_GuildGuid, pUser->GetCharName().c_str() );
*/
	}
	// �ϴ� ���� ������ �ִٸ� 100% �� ������ �ŷ��Ѵ�. ���� RecvMsg�� ���� �ʿ� ����
/*
	else if( pUser->GetGuildGuid() != pRecvMsg->m_GuildGuid )
	{
		// �̰� �׳� ������
		resMsg.byResult = MSG_CW_GUILD_CHAT_NAK::ERR_NOT_GUILD;

		pUser->SendPacket( (BYTE*)&resMsg, sizeof(MSG_CW_GUILD_CHAT_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Guild Chat Error - No Guild %u", pUser->GetCharName().c_str(), pUser->GetGuildGuid() );
	}
*/
	else
	{
		// �Ҽ� ��� ã�´�
		WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild( pUser->GetGuildGuid() );

		if( !pGuild )
		{
			MSG_CW_GUILD_CHAT_NAK	resMsg;

			resMsg.byResult = MSG_CW_GUILD_CHAT_NAK::ERR_NOT_GUILD;

			pUser->SendPacket( (BYTE*)&resMsg, sizeof(MSG_CW_GUILD_CHAT_NAK) );
			MessageOut( eCRITICAL_LOG, "[%s] Guild Chat Error - No Guild %u", pUser->GetCharName().c_str(), pUser->GetGuildGuid() );
			return;
		}

		BYTE byMsgLen = pRecvMsg->m_byMsgLen;
		util::cSTRING_REF strMsgRD( pRecvMsg->m_ptszCharMsg, sizeof(pRecvMsg->m_ptszCharMsg) );

		if( !byMsgLen || ( pRecvMsg->GetSize() != wSize ) || (strMsgRD.GetSTRING_LENGTH() != byMsgLen) )
		{
			MessageOut(eCRITICAL_LOG, "Invalid GuildChatMsg %s %u", pUser->GetCharName().c_str(), pRecvMsg->m_byMsgLen);
			return;
		}
		
		if(!ProhibitionWordParser::Instance()->CheckFilter( strMsgRD ))
		{
			MSG_CW_CHAT_NORMAL_NAK	sendMsg;
			sendMsg.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;
			pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_CHAT_NAK) );
			MessageOut( eCRITICAL_LOG, _T("Guild Chat Error - USE BAN WORD[%s][CharGuid:%u]"),
				pUser->GetCharName().c_str(), pUser->GetCharGuid());

			return;
		}

		// ���� ��ü���� �޽��� ���� - �̰� UserManager���� ó���ؾ� ���� ������ �ͳ�?
		MSG_CW_GUILD_CHAT_BRD msgBRD;

		util::cSTRING_REF_RW strCharNameRW( msgBRD.ptszCharName, sizeof(msgBRD.ptszCharName) );
		strCharNameRW.CopyFromString( pUser->GetCharName().c_str() );
		strCharNameRW.MakeSafeString();

		util::cSTRING_REF_RW strMsgRW4BRD( msgBRD.ptszChatMsg, sizeof(msgBRD.ptszChatMsg) );
		strMsgRW4BRD.CopyFrom( strMsgRD );

		msgBRD.byMsgLen = strMsgRW4BRD.GetSTRING_LENGTH()+1;

		pGuild->SendAllMember( (BYTE*)&msgBRD, msgBRD.GetSize() ); 

//		SERVER_KEY ServerKey = WorldServer::Instance()->GetServerKey();
//		ServerSession *pAgentServer = ServerSessionManager::Instance()->FindAgentServer( pUser->GetChannelID() );
//		if( pAgentServer )
//		{
//			ServerKey = pAgentServer->GetServerKey();
//		}

		GAMELOG->WriteGuildChat( pUser, pUser->GetChannelID(), pGuild, strMsgRW4BRD.GetBUFFER(), MSG_CW_GUILD_CHAT_BRD::_MAX_CHATMSG_SIZE);

		MessageOut( eFULL_LOG, _T("[%s] Send Guild Chat[%u]"),
			pUser->GetCharName().c_str(), pRecvMsg->m_GuildGuid );
	}
}

// ��Ƽ ä�� ó��
Handler_CW_IMPL( CW_CHAT_PARTY_SYN )
{
	MSG_CW_CHAT_PARTY_SYN* pRecvMsg = (MSG_CW_CHAT_PARTY_SYN*)pMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	// ��� ä�ÿ��� ũ�Ⱑ 0�̰ų� ���� ������ ������ ����
//	if( !pRecvMsg->m_byMsgLength || !_tcslen(pRecvMsg->m_ptszChatMsg) )
//	{
//		MessageOut( eFULL_LOG, "ä�� ���� ����" );
//		return;
//	}

	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

	//ä�� �������
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

	BYTE byMsgLen = pRecvMsg->m_byMsgLength;
	util::cSTRING_REF strMsgRD( pRecvMsg->m_ptszChatMsg, sizeof(pRecvMsg->m_ptszChatMsg) );

	if( !byMsgLen || ( pRecvMsg->GetSize() != wSize ) || (strMsgRD.GetSTRING_LENGTH() != byMsgLen) )
	{
		MessageOut(eCRITICAL_LOG, "Invalid PartyChatMsg %s %u", pUser->GetCharName().c_str(), byMsgLen);
		return;
	}

	if(!ProhibitionWordParser::Instance()->CheckFilter( strMsgRD ))
	{
		MSG_CW_CHAT_NORMAL_NAK	sendMsg;

		sendMsg.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_CHAT_NAK) );
		MessageOut( eCRITICAL_LOG, "Party Chat Error - USE BAN WORD[%s][CharGuid:%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid());
		return;
	}

	MSG_CW_CHAT_PARTY_BRD msgBRD;
	// ������ ��Ƽ ������ �޽��� ����
	util::cSTRING_REF_RW strNameRW4BRD( msgBRD.m_ptszCharName, sizeof(msgBRD.m_ptszCharName) );
	strNameRW4BRD.CopyFromString( pUser->GetCharName().c_str() );
	strNameRW4BRD.MakeSafeString();

	util::cSTRING_REF_RW strMsgRW4BRD( msgBRD.m_ptszChatMsg, sizeof(msgBRD.m_ptszChatMsg) );
	strMsgRW4BRD.CopyFrom( strMsgRD );
	strMsgRW4BRD.MakeSafeString();

	msgBRD.m_byMsgLength = strMsgRW4BRD.GetSTRING_LENGTH()+1;

	pUser->SendParty( (BYTE*)&msgBRD, msgBRD.GetSize() );

//	SERVER_KEY ServerKey = WorldServer::Instance()->GetServerKey();
//	ServerSession *pAgentServer = ServerSessionManager::Instance()->FindAgentServer( pUser->GetChannelID() );
//	if( pAgentServer )
//	{
//		ServerKey = pAgentServer->GetServerKey();
//	}

	GAMELOG->WritePartyChat( pUser, pUser->GetChannelID(), strMsgRW4BRD.GetBUFFER(), MSG_CW_CHAT_PARTY_BRD::_MAX_CHATMSG_SIZE );
	MessageOut( eFULL_LOG, _T("[%s] Send Party Chat"), pUser->GetCharName().c_str() );
}

// Ŭ���̾�Ʈ ��ȣȭ ���� �Ϸ�
Handler_CW_IMPL( CW_CRYPTOGRAPHY_ACK )
{
	if( WorldServerInfoParser::Instance()->IsCryptable() )
	{
		// Ŭ���̾�Ʈ���� �������� �� Ű ����϶�� �˷��ְ� ������ �����Ѵ�. �� ������ ���� ���� ������ �޴� ���� ������ ��ȣȭ�� ������̴�
		MSG_CW_CRYPTOGRAPHY_CMD	sendMsg;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CRYPTOGRAPHY_CMD) );

		pUser->SetCrypt();	// �������� ��ȣȭ ����!

		MessageOut( eFULL_LOG, "[GUID:%u] Crypt Set", pUser->GetGUID() );
		return;
	}

	MessageOut( eCRITICAL_LOG, "[GUID:%u] Request Crypt Set but Not Crypt Server", pUser->GetGUID() );
}

// Ŭ���̾�Ʈ ��ȣȭ ���� ����
Handler_CW_IMPL( CW_CRYPTOGRAPHY_NAK )
{
	// �߶������ �ϳ�?
	MessageOut( eCRITICAL_LOG, "[GUID:%u] Crypt Set Failed!!!", pUser->GetGUID() );

	pUser->Logout();
}

// ���� ä�� ��û
Handler_CW_IMPL( CW_CHAT_VOICE_REQUEST_SYN )
{
	// ��û�� ��� �˻��Ͽ� ���� ä�� ��û
	MSG_CW_CHAT_VOICE_REQUEST_SYN* pRecvMsg = (MSG_CW_CHAT_VOICE_REQUEST_SYN*)pMsg;

	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	//ä�� �������
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

	User *pTargetUser = UserManager::Instance()->FindUser(pRecvMsg->m_ptszCharName);

	if( !pTargetUser || eZONETYPE_MAX == pTargetUser->GetStatus() )
	{
		MessageOut( eCRITICAL_LOG, "[GUID:%u][C:%u] Request to Not Connected(or Loading) User[%s] - Error", pUser->GetGUID(), 
																								pUser->GetChannelID(), 
																								pRecvMsg->m_ptszCharName );

		MSG_CW_CHAT_VOICE_REQUEST_NAK sendMsg;

		sendMsg.wErrorCode = MSG_CW_CHAT_VOICE_REQUEST_NAK::_ERR_NO_CHARNAME;
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
		return;
	}

	// ��밡 ����ä�� �Ұ� �����̰ų� �̹� �ٸ� ������� ��û�� ���� �޾��� ��쵵 �����Ƿ� ���⿡ ��ó�Ѵ�
	if( !pTargetUser->IsVoiceChatOn() )
	{
		MessageOut( eMIDDLE_LOG, "[GUID:%u][C:%u] Request to User but Can't use Voice Chat[%s]", pUser->GetGUID(), pUser->GetChannelID(), pRecvMsg->m_ptszCharName );
		MSG_CW_CHAT_VOICE_REQUEST_NAK sendMsg;

		sendMsg.wErrorCode = MSG_CW_CHAT_VOICE_REQUEST_NAK::_ERR_BLOCKVOICE;
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
		return;
	}

	if( pTargetUser->GetVoiceChatRequestGUID() != 0 )
	{
		MessageOut( eMIDDLE_LOG, "[GUID:%u][C:%u] Already Requested Voice Chat User[%s]", pUser->GetGUID(), pUser->GetChannelID(), pRecvMsg->m_ptszCharName );
		MSG_CW_CHAT_VOICE_REQUEST_NAK sendMsg;

		sendMsg.wErrorCode = MSG_CW_CHAT_VOICE_REQUEST_NAK::_ERR_DEFAULT;
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
		return;
	}

	if( _tcslen(pRecvMsg->m_ptszAccountID) >= MAX_CHARNAME_LENGTH )
	{
		MessageOut( eCRITICAL_LOG, "[GUID:%u][C:%u] Voice Chat AccountID Length Over", pUser->GetGUID(), pUser->GetChannelID() );
		MSG_CW_CHAT_VOICE_REQUEST_NAK sendMsg;

		sendMsg.wErrorCode = MSG_CW_CHAT_VOICE_REQUEST_NAK::_ERR_DEFAULT;
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
		return;
	}

	MSG_CW_CHAT_VOICE_REQUEST_BRD	sendMsg;

	memset( sendMsg.m_ptszCharName, 0, sizeof(sendMsg.m_ptszCharName) );
	_tcsncpy( sendMsg.m_ptszCharName, pUser->GetCharName().c_str(), pUser->GetCharName().length() );
	memset( sendMsg.m_ptszAccountID, 0, sizeof(sendMsg.m_ptszAccountID) );
	_tcsncpy( sendMsg.m_ptszAccountID, pRecvMsg->m_ptszAccountID, _tcslen(pRecvMsg->m_ptszAccountID) );

	pTargetUser->RequestVoiceChat( pUser->GetGUID() );
	pTargetUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CHAT_VOICE_REQUEST_BRD) );

	MessageOut( eFULL_LOG, "[GUID:%u][C:%u] Voice Chat Request to [%s][GUID:%u]", pUser->GetGUID(), 
																					pUser->GetChannelID(), 
																					pRecvMsg->m_ptszCharName, 
																					pTargetUser->GetGUID() );
}

// ���� ä�� ����
Handler_CW_IMPL( CW_CHAT_VOICE_ACCEPT_SYN )
{
	MSG_CW_CHAT_VOICE_ACCEPT_SYN* pRecvMsg = (MSG_CW_CHAT_VOICE_ACCEPT_SYN*)pMsg;

	// ��û�� ���̵� �ִ��� Ȯ���Ͽ� ���� �������� ����
	User *pTargetUser = UserManager::Instance()->FindUser( pUser->GetVoiceChatRequestGUID() );

	if( !pTargetUser )
	{
		MessageOut( eMIDDLE_LOG, "[GUID:%u][C:%u] Voice Chat Accept but No User Requested", pUser->GetGUID(), pUser->GetChannelID() );

		MSG_CW_CHAT_VOICE_ACCEPT_NAK sendMsg;
		sendMsg.wErrorCode = MSG_CW_CHAT_VOICE_ACCEPT_NAK::_ERR_NO_CHARNAME;
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
		return;
	}

	if( _tcslen(pRecvMsg->m_ptszAccountID) >= MAX_CHARNAME_LENGTH )
	{
		MessageOut( eCRITICAL_LOG, "[GUID:%u][C:%u] Voice Chat Accept AccountID Length Over", pUser->GetGUID(), pUser->GetChannelID() );
		MSG_CW_CHAT_VOICE_ACCEPT_NAK sendMsg;

		sendMsg.wErrorCode = MSG_CW_CHAT_VOICE_ACCEPT_NAK::_ERR_DEFAULT;
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
		return;
	}

	MSG_CW_CHAT_VOICE_ACCEPT_BRD	sendMsg;

	memset( sendMsg.m_ptszCharName, 0, sizeof(sendMsg.m_ptszCharName) );
	_tcsncpy( sendMsg.m_ptszCharName, pUser->GetCharName().c_str(), pUser->GetCharName().length() );
	memset( sendMsg.m_ptszAccountID, 0, sizeof(sendMsg.m_ptszAccountID) );
	_tcsncpy( sendMsg.m_ptszAccountID, pRecvMsg->m_ptszAccountID, _tcslen(pRecvMsg->m_ptszAccountID) );
	pTargetUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CHAT_VOICE_ACCEPT_BRD) );

	pUser->ClearVoiceChatRequest();

	MessageOut( eFULL_LOG, "[GUID:%u][C:%u] Accepted Voice Chat Request from User [%s][GUID:%u]", pUser->GetGUID(), 
																				pUser->GetChannelID(), 
																				sendMsg.m_ptszCharName, 
																				pTargetUser->GetGUID() );
}

// ���� ä�� ����
Handler_CW_IMPL( CW_CHAT_VOICE_REJECT_SYN )
{
	// ��û�ߴ� ���̵� �ִ��� Ȯ���Ͽ� ���� �������� ����
	User *pTargetUser = UserManager::Instance()->FindUser( pUser->GetVoiceChatRequestGUID() );

	if( !pTargetUser )
	{
		MessageOut( eMIDDLE_LOG, "[GUID:%u][C:%u] Reject Voic Chat but No User Requested", pUser->GetGUID(), pUser->GetChannelID() );

		return;
	}

	MSG_CW_CHAT_VOICE_REJECT_SYN*	pRecvMsg = (MSG_CW_CHAT_VOICE_REJECT_SYN*)pMsg;
	MSG_CW_CHAT_VOICE_REJECT_BRD	sendMsg;

	memset( sendMsg.m_ptszCharName, 0, sizeof(sendMsg.m_ptszCharName) );
	_tcsncpy( sendMsg.m_ptszCharName, pUser->GetCharName().c_str(), pUser->GetCharName().length() );
	sendMsg.wReasonCode = pRecvMsg->wReasonCode;

	pTargetUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CHAT_VOICE_REJECT_BRD) );

	pUser->ClearVoiceChatRequest();

	MessageOut( eFULL_LOG, "[GUID:%u][C:%u] Reject Voice Chat Request from [%s][GUID:%u]", pUser->GetGUID(), 
																					pUser->GetChannelID(), 
																					sendMsg.m_ptszCharName, 
																					pTargetUser->GetGUID() );
}

Handler_CW_IMPL( CW_CHAT_VOICE_DISCONNECT_SYN )
{
	MSG_CW_CHAT_VOICE_DISCONNECT_SYN* pRecvMsg = (MSG_CW_CHAT_VOICE_DISCONNECT_SYN*)pMsg;

	if( _tcslen( pRecvMsg->m_ptszCharName ) >= MAX_CHARNAME_LENGTH )
	{
		MessageOut( eCRITICAL_LOG, "[GUID:%u][C:%u] VoiceChat Disconn Target Name Length Over!![%u]", pUser->GetGUID(), pUser->GetChannelID(), _tcslen( pRecvMsg->m_ptszCharName ) );

		return;
	}

	User *pTargetUser = UserManager::Instance()->FindUser( pRecvMsg->m_ptszCharName );

	if( !pTargetUser )
	{
		MessageOut( eMIDDLE_LOG, "[GUID:%u][C:%u] No Target User for Disconnect Voice Chat", pUser->GetGUID(), pUser->GetChannelID() );

		MSG_CW_CHAT_VOICE_DISCONNECT_NAK	sendMsg;

		sendMsg.wErrorCode = MSG_CW_CHAT_VOICE_DISCONNECT_NAK::_ERR_NO_CHARNAME;
		return;
	}

	// ä�� ������ �ƴ����� �Ǵ� �Ұ�. ���� ������ ���� ������ ���弭���� ���� ä�ÿ��� ��� �������� ������ �ϴ°��� �޷� �ִ�.

	MSG_CW_CHAT_VOICE_DISCONNECT_BRD	sendMsg;

	memset( sendMsg.m_ptszCharName, 0, sizeof(sendMsg.m_ptszCharName) );
	_tcsncpy( sendMsg.m_ptszCharName, pUser->GetCharName().c_str(), pUser->GetCharName().length() );

	sendMsg.wReasonCode = pRecvMsg->wReasonCode;

	pTargetUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CHAT_VOICE_DISCONNECT_BRD) );

	memset( sendMsg.m_ptszCharName, 0, sizeof(sendMsg.m_ptszCharName) );
	_tcsncpy( sendMsg.m_ptszCharName, pRecvMsg->m_ptszCharName, _tcslen(pRecvMsg->m_ptszCharName) );

	pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CHAT_VOICE_DISCONNECT_BRD) );

	MessageOut( eFULL_LOG, "[GUID:%u][C:%u] Disconnect Voice chat with [%s][%u]", pUser->GetGUID(), pUser->GetChannelID(), 
																		pTargetUser->GetCharName().c_str(), pTargetUser->GetGUID() );
}

// ���� ä�� on/off ��û
Handler_CW_IMPL( CW_CHAT_VOICE_MIC_STATUS_SYN )
{
	// ���� ä�� on/off ���θ� Ŭ�󿡼� �޾Ƽ� �ش� ������ ��Ƽ���鿡�� �����Ѵ�
	MSG_CW_CHAT_VOICE_MIC_STATUS_SYN*	pRecvMsg = (MSG_CW_CHAT_VOICE_MIC_STATUS_SYN*)pMsg;
	MSG_CW_CHAT_VOICE_MIC_STATUS_BRD	sendMsg;

	_tcsncpy( sendMsg.m_ptszCharName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );

	// ���޸� �ϸ� �ȴ�...���� ���ư��� ���嵵 �ؾ� �ȴ�.(2006/05/16)
	pUser->SetVoiceChat( pRecvMsg->bOn );
	sendMsg.bOn = pRecvMsg->bOn;

	pUser->SendParty( (BYTE*)&sendMsg, sizeof(sendMsg) );

	MessageOut( eFULL_LOG, "[GUID:%u][C:%u] Request Voice Chat On/Off [bOn:%u]", pUser->GetGUID(), pUser->GetChannelID(), pRecvMsg->bOn );
}

// ��Ƽ������ ���� ä�� ���� ��û
Handler_CW_IMPL( CW_CHAT_PARTY_MIC_STATUS_REQ )
{
	CommChatZone* pParty = pUser->GetParty();
	if( !pParty )
	{
		MSG_CW_CHAT_PARTY_MIC_STATUS_NAK	nakMsg;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
		MessageOut( eMIDDLE_LOG, "[GUID:%u][C:%u] Request Party Member Mic Status Error - No Party", pUser->GetGUID(), pUser->GetChannelID() );
		return;
	}

	MSG_CW_CHAT_PARTY_MIC_STATUS_ANS sendMsg;

	sendMsg.m_byMemberNum = pParty->GetPartyVoiceChatInfo( (BYTE*)sendMsg.m_sMemberStatus, pUser );

	pUser->SendPacket( (BYTE*)&sendMsg, sendMsg.GetSize() );

	MessageOut( eFULL_LOG, "[GUID:%u][C:%u] Request Party Member Mic Status[PartyKey:%u]", pUser->GetGUID(), pUser->GetChannelID(), pParty->GetKey() );
}

// �÷��̾���� ����ä�� ���� ��û(CW_CHAT_PARTY_MIC_STATUS_REQ�� ������ ��� Ŭ���̾�Ʈ�� ����
Handler_CW_IMPL( CW_CHAT_PLAYER_MIC_STATUS_REQ )
{
	MSG_CW_CHAT_PLAYER_MIC_STATUS_REQ* pRecvMsg = (MSG_CW_CHAT_PLAYER_MIC_STATUS_REQ*)pMsg;
	User *pTarget = NULL;
	MSG_CW_CHAT_PLAYER_MIC_STATUS_ANS	sendMsg;

	sendMsg.m_byMemberNum = 0;

	// ���� �ο� �̻����� ���� ���� - ġ���� �����̰ų� ũ��ŷ �õ��� �����ؾ� �Ѵ�
	if( pRecvMsg->m_byMemberNum > MSG_CW_CHAT_PLAYER_MIC_STATUS_REQ::_MAX_PLAYER_NUM )
	{
		MessageOut( eCRITICAL_LOG, "[GUID:%u][C:%u]Request User Mic Status Error - User Count overflow[%u]", pUser->GetGUID(), 
            pUser->GetChannelID(),
			pRecvMsg->m_byMemberNum );

		pUser->SendPacket( (BYTE*)&sendMsg, sendMsg.GetSize() );
		return;
	}

	for(BYTE i = 0; i < pRecvMsg->m_byMemberNum; i++ )
	{
		pTarget = UserManager::Instance()->FindUser( pRecvMsg->m_ptszMemberStatus[i] );

		if( !pTarget )
			continue;

		_tcsncpy( sendMsg.m_sMemberStatus[sendMsg.m_byMemberNum].m_ptszCharName, pTarget->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
		sendMsg.m_sMemberStatus[sendMsg.m_byMemberNum].m_bVoiceOn = pTarget->IsVoiceChatOn();
		sendMsg.m_byMemberNum++;
	}

	pUser->SendPacket( (BYTE*)&sendMsg, sendMsg.GetSize() );

	MessageOut( eFULL_LOG, "[GUID:%u][C:%u] Request User Mic Status[Num:%u]", pUser->GetGUID(), pUser->GetChannelID(), sendMsg.m_byMemberNum );
}

// �ٸ� ����� ����ID ��û
Handler_CW_IMPL( CW_COMM_USERID_REQ )
{
	MSG_CW_COMM_USERID_REQ* pRecvMsg = (MSG_CW_COMM_USERID_REQ*)pMsg;

	if( _tcslen( pRecvMsg->m_ptszCharName ) > MAX_CHARNAME_LENGTH )
	{
		MessageOut( eCRITICAL_LOG, "[GUID:%u][C:%u] UserID Req Error - CharName Length Overflow", pUser->GetGUID(), pUser->GetChannelID() );
		return;
	}
	User *pCommUser = UserManager::Instance()->FindUser( pRecvMsg->m_ptszCharName );

	if( !pCommUser )
	{
		MSG_CW_COMM_USERID_NAK	nakMsg;

		nakMsg.m_wErrorCode = MSG_CW_COMM_USERID_NAK::_ERR_NOTCONNUSER;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
		MessageOut( eMIDDLE_LOG, "[%u][%CharGuid:%u] UserID Req Error - Not Connected User[%s]", pUser->GetGUID(), pUser->GetCharGuid(), pRecvMsg->m_ptszCharName );
		return;
	}

	MSG_CW_COMM_USERID_ACK	sendMsg;

	memset( sendMsg.m_ptszAccountID, 0, sizeof(TCHAR) * (ID_MAX_LEN+1) );
	_tcsncpy( sendMsg.m_ptszAccountID, pCommUser->GetAccountID(), ID_MAX_LEN );
	memset( sendMsg.m_ptszCharName, 0, sizeof(TCHAR) * (MAX_CHARNAME_LENGTH+1) );
	_tcsncpy( sendMsg.m_ptszCharName, pCommUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );

	pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
	MessageOut( eFULL_LOG, "[GUID:%u][C:%u] UserID Req[GUID:%u][UserID:%s]", pUser->GetGUID(), pUser->GetChannelID(), 
																			pCommUser->GetGUID(), sendMsg.m_ptszAccountID );
}

// ä��ä�ù� ���� ��û
Handler_CW_IMPL( CW_CHAT_CHANNEL_JOIN_SYN )
{
	MSG_CW_CHAT_CHANNEL_JOIN_SYN* pRecvMsg = (MSG_CW_CHAT_CHANNEL_JOIN_SYN*)pMsg;
	MSG_CW_CHAT_CHANNEL_JOIN_NAK nakMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	// ��Ŷ ���� üũ
	BYTE byRoomNameLength = pRecvMsg->m_byRoomNameLength;
	if( (MAX_ROOMTITLE_LENGTH < byRoomNameLength) || (pRecvMsg->GetSize() != wSize) )
	{
		MessageOut( eCRITICAL_LOG, "CHAT_CHANNEL_JOIN_SYN Error - RoomName Overflow[CharGuid:%u]", pUser->GetCharGuid() );

		nakMsg.m_byErrorCode = RC::RC_CHAT_UNKNOWN;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
		return;
	}

    util::StrArrayForRoomTitle room_title;
    BOOST_STATIC_ASSERT(_countof(pRecvMsg->m_ptszRoomName) == _countof(room_title));
    _tcsncpy(room_title, pRecvMsg->m_ptszRoomName, _countof(room_title));
    room_title[_countof(room_title) - 1] = _T('\0');

    if (!ProhibitionWordParser::Instance()->CheckFilterName(room_title))
	{	
		nakMsg.m_byErrorCode = RC::RC_CHANNEL_CHAT_TITLE_BAN_WORD;
		pUser->SendPacket((BYTE*)&nakMsg, sizeof(nakMsg));
		return;
	}

	ChatRoomManager::Instance()->AddUser( pUser, byRoomNameLength, room_title);
	MessageOut( eFULL_LOG, "CHAT_CHANNEL_JOIN_SYN[CharGuid:%u]", pUser->GetCharGuid() );
}

// ä��ä�� ����
Handler_CW_IMPL( CW_CHAT_CHANNEL_SEND_SYN )
{
	MSG_CW_CHAT_CHANNEL_SEND_SYN*	pRecvMsg = (MSG_CW_CHAT_CHANNEL_SEND_SYN*)pMsg;
	MSG_CW_CHAT_CHANNEL_SEND_NAK	nakMsg;
	MSG_CW_CHAT_CHANNEL_SEND_BRD	brdMsg;


	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	//ä�� �������
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

	// ��Ŷ ���� �˻�
	if( pRecvMsg->m_byChatLength > MAX_CHAT_LEN )
	{
		MessageOut( eCRITICAL_LOG, "CHAT_CHANNEL_SEND Error - ChatLength Overflow![CharGuid:%u]", pUser->GetCharGuid() );

		nakMsg.m_byErrorCode = RC::RC_CHAT_UNKNOWN;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
		return;
	}

	// ���� ������� ä�ù� �ִ��� �˻�
	CommChatZone *pChatRoom = pUser->GetChannelChatRoom();
	if(!pChatRoom)	return;

	brdMsg.m_byChannelID = (BYTE)pUser->GetChannelID();
	_tcsncpy( brdMsg.m_ptszCharName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	brdMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
	brdMsg.m_byChatLength = pRecvMsg->m_byChatLength;
	memset( brdMsg.m_ptszChat, 0, sizeof(TCHAR) * MAX_CHAT_LEN+1 );
	_tcsncpy( brdMsg.m_ptszChat, pRecvMsg->m_ptszChat, pRecvMsg->m_byChatLength );

	if( !pUser->SendChannelChat( (BYTE*)&brdMsg, brdMsg.GetSize() ) )
	{
		MessageOut( eCRITICAL_LOG, "CHAT_CHANNEL_SEND Error - No ChatRoom[CharGuid:%u]", pUser->GetCharGuid() );

		nakMsg.m_byErrorCode = RC::RC_CHAT_NOROOM;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
		return;
	}

    GAMELOG->WriteChannelChat(pUser, pUser->GetChannelID(),
        brdMsg.m_ptszChat, _countof(brdMsg.m_ptszChat) - 1);
	MessageOut( eFULL_LOG, "CHAT_CHANNEL_SEND BRD[CharGuid:%u]", pUser->GetCharGuid() );
}

// ä��ä�ù� �ʴ�
Handler_CW_IMPL( CW_CHAT_CHANNEL_INVITE_SYN )
{
	MSG_CW_CHAT_CHANNEL_INVITE_SYN* pRecvMsg = (MSG_CW_CHAT_CHANNEL_INVITE_SYN*)pMsg;
	MSG_CW_CHAT_CHANNEL_INVITE_NAK	nakMsg;
	MSG_CW_CHAT_CHANNEL_INVITE_ACK	ackMsg;
	MSG_CW_CHAT_CHANNEL_INVITE_CMD	cmdMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	// ��Ŷ ���� ����
	TCHAR	ptszTargetName[MAX_CHARNAME_LENGTH+1] = {0,};

	_tcsncpy( ptszTargetName, pRecvMsg->m_ptszInviteName, MAX_CHARNAME_LENGTH );

	// �ش� ���� �ִ��� �˻�
	User *pTarget = UserManager::Instance()->FindUser( ptszTargetName );

	if( !pTarget || pTarget == pUser )
	{
		// ���� ����~
		nakMsg.m_byErrorCode = RC::RC_CHAT_NOCHAR;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "CHAT_CHANNEL_INVITE Error - No Target User[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), ptszTargetName );
		return;
	}

    //_NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL
    // �ʴ��� ����� �̹� ä�� ä�� ������ �˻��Ѵ�.
    if (pTarget->GetChannelChatRoom())
    {
        nakMsg.m_byErrorCode = RC::RC_CHAT_ALREADYINROOM;
        // �ʴ� ����ڸ� �����Ѵ�.
        {
            BOOST_STATIC_ASSERT(sizeof(nakMsg.invite_target_name_) == 
                (_countof(nakMsg.invite_target_name_) * sizeof(TCHAR)));
            ::_tcsncpy(
                nakMsg.invite_target_name_, 
                (pTarget->GetCharName()).c_str(), 
                _countof(nakMsg.invite_target_name_));
            nakMsg.invite_target_name_[_countof(nakMsg.invite_target_name_)-1] = _T('\0');
        }
        pUser->SendPacket((BYTE*)&nakMsg, sizeof(nakMsg));

        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|�ʴ��� ����� �̹� ä�� ä�� ���Դϴ�.|��û�� = %s, ����� = %s|"), 
               __FUNCTION__, 
               (pUser->GetCharName()).c_str(),
               (pTarget->GetCharName()).c_str());
        return;
    }

	// � �濡 �ʴ��ϴ°�?
	CommChatZone *pChatRoom = pUser->GetChannelChatRoom();

	if( !pChatRoom )
	{
		nakMsg.m_byErrorCode = RC::RC_CHAT_NOROOM;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "CHAT_CHANNEL_INVITE Error - No ChatRoom[CharGuid:%u]", pUser->GetCharGuid() );
		return;
	}

#if WAVERIX_CODE_RENEWAL
    { // �ʴ��ϴ� �� �̸�
        BOOST_STATIC_ASSERT(sizeof(cmdMsg.m_ptszInviteName) ==
                            (_countof(cmdMsg.m_ptszInviteName) * sizeof(char)));
        strncpy(cmdMsg.m_ptszInviteName, pUser->GetCharName().c_str(), _countof(cmdMsg.m_ptszInviteName));
        cmdMsg.m_ptszInviteName[_countof(cmdMsg.m_ptszInviteName) - 1] = '\0';
    };
    { // ����
        BOOST_STATIC_ASSERT(sizeof(cmdMsg.m_ptszRoomName) ==
                            (_countof(cmdMsg.m_ptszRoomName) * sizeof(char)) &&
                            _countof(cmdMsg.m_ptszRoomName) <= UCHAR_MAX);
        strncpy(cmdMsg.m_ptszRoomName, pChatRoom->GetChatRoomName(), _countof(cmdMsg.m_ptszRoomName));
        cmdMsg.m_ptszRoomName[_countof(cmdMsg.m_ptszRoomName) - 1] = '\0';
        cmdMsg.m_byRoomNameLength = (BYTE)strlen(cmdMsg.m_ptszRoomName); //assert
    };
#else
	// �ʴ��ϴ³� �̸�
	_tcsncpy( cmdMsg.m_ptszInviteName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	cmdMsg.m_ptszInviteName[MAX_CHARNAME_LENGTH] = '\0';

	// ����
	TCHAR ptszRoomName[MAX_ROOMTITLE_LENGTH+1]={0,};
	memcpy( ptszRoomName, pChatRoom->GetChatRoomName(), MAX_ROOMTITLE_LENGTH );
	cmdMsg.m_byRoomNameLength = _tcslen( ptszRoomName );
	memset( cmdMsg.m_ptszRoomName, 0, sizeof(TCHAR) * MAX_ROOMTITLE_LENGTH );
    _tcsncpy( cmdMsg.m_ptszRoomName, ptszRoomName, MAX_ROOMTITLE_LENGTH );
#endif

    pUser->SendPacket( (BYTE*)&ackMsg, sizeof(ackMsg) );
	pTarget->SendPacket( (BYTE*)&cmdMsg, cmdMsg.GetSize() );

	MessageOut( eFULL_LOG, "CHAT_CHANNEL_INVITE SEND[CharGuid:%u][TargetCharGuid:%u]", pUser->GetCharGuid(), pTarget->GetCharGuid() );
}

//_NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL
// �ʴ� ����ڰ� ä�� �ʴ븦 �ź��ϰų� �������� �� ȣ��ȴ�.
Handler_CW_IMPL(CW_CHAT_CHANNEL_INVITE_NAK)
{
    __UNUSED(wSize);

    if (!::CheckChatBlockZone(pUser))
    {
        return;
    }
    
    // �ʴ� �ź� ��û�� �Դ��� �˻��Ѵ�.
    MSG_CW_CHAT_CHANNEL_INVITE_NAK* const recv_invite_nak = 
        static_cast<MSG_CW_CHAT_CHANNEL_INVITE_NAK*>(pMsg);
    if (recv_invite_nak->m_byErrorCode != RC::RC_CHAT_DENY_INVITATION && 
        recv_invite_nak->m_byErrorCode != RC::RC_CHANNEL_CHAT_BLOCKED)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|ä�� �ʴ� �źγ� ���� ���°� �ƴմϴ�.|��û�� = %s, ��û�ڵ� = %d|"), 
               __FUNCTION__, 
               (pUser->GetCharName()).c_str(),
               recv_invite_nak->m_byErrorCode);
        return;
    }
    
    // �ʴ� ��û�ڸ� �˻��Ѵ�.
    User* const invite_request_user = 
        UserManager::Instance()->FindUser(recv_invite_nak->invite_target_name_);
    if (invite_request_user == NULL || invite_request_user == pUser)
    {
        // �ʴ� ��û�ڸ� ã�� �� ���ٸ� ���� ����ڰ� �������� �����Ƿ� ���⼭ ������.
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|�ʴ� ��û�ڸ� ã�� �� �����ϴ�.|��û�� = %s, ��û�� = %s|"), 
               __FUNCTION__, 
               (pUser->GetCharName()).c_str(),
               recv_invite_nak->invite_target_name_);
        return;
    }

    // �ʴ� ����ڸ� �����Ѵ�.
    {
        BOOST_STATIC_ASSERT(sizeof(recv_invite_nak->invite_target_name_) == 
            (_countof(recv_invite_nak->invite_target_name_) * sizeof(TCHAR)));
        ::_tcsncpy(
            recv_invite_nak->invite_target_name_, 
            (pUser->GetCharName()).c_str(), 
            _countof(recv_invite_nak->invite_target_name_));
        recv_invite_nak->invite_target_name_[_countof(recv_invite_nak->invite_target_name_)-1] = _T('\0');
    }
    
    // �ʴ� ��û�ڿ��� ���� �޽����� �߰��Ѵ�.
    invite_request_user->SendPacket((BYTE*)recv_invite_nak, sizeof(*recv_invite_nak));

    SUNLOG(eFULL_LOG, 
           _T("|%s|�ʴ� ��û�ڿ��� �ʴ� �źγ� ���� ���� �˸� �޽����� ���½��ϴ�.|��û�� = %s, ����� = %s|"), 
           __FUNCTION__, 
           (invite_request_user->GetCharName()).c_str(), 
           (pUser->GetCharName()).c_str());
}

// ä��ä�ù� ���� ��û
Handler_CW_IMPL( CW_CHAT_CHANNEL_OUT_SYN )
{
	MSG_CW_CHAT_CHANNEL_OUT_SYN* pRecvMsg = (MSG_CW_CHAT_CHANNEL_OUT_SYN*)pMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	ChatRoomManager::Instance()->RemoveUser( pUser );

	MessageOut( eFULL_LOG, "CHAT_CHANNEL_OUT[CharGuid:%u]", pUser->GetCharGuid() );
}

// ä��ä�ù� ���� ��û
Handler_CW_IMPL( CW_CHAT_CHANNEL_INFO_REQ )
{
	MSG_CW_CHAT_CHANNEL_INFO_REQ* pRecvMsg = (MSG_CW_CHAT_CHANNEL_INFO_REQ*)pMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	CommChatZone* pChatZone = pUser->GetChannelChatRoom();
	if( !pChatZone )
	{
		MSG_CW_CHAT_CHANNEL_INFO_NAK nakMsg;
		nakMsg.m_byErrorCode = RC::RC_CHAT_NOROOM;
		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		return;
	}
    // NOTE: f110310.5L, this message processing seems to dangerous...
	MSG_CW_CHAT_CHANNEL_INFO_ANS ansMsg;
    {
        DWORD number_of_users = pChatZone->GetNumberOfUsers();
        ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, number_of_users, BYTE));
	    ansMsg.m_byUserNum = (BYTE)number_of_users;
    };
	if( pChatZone->GetFirstUserName( ansMsg.m_ptszCharName[0] ) )
	{
		int i;
		for( i = 1; i < MAX_CHATZONE_USER; i++ )
		{
			if( !pChatZone->GetNextUserName( ansMsg.m_ptszCharName[i] ) )
				break;
		}
	}

	pUser->SendPacket( (BYTE*)&ansMsg, ansMsg.GetSize() );

	MessageOut( eFULL_LOG, "ChannelChatZone Info Req[%u][Num:%u]", pUser->GetCharGuid(), ansMsg.m_byUserNum );
}

// ģ��/������/����/�׷�� ������ ��û
Handler_CW_IMPL( CW_FRIEND_STATUS_REQ )
{
	MSG_CW_FRIEND_STATUS_REQ*	pRecvMsg = (MSG_CW_FRIEND_STATUS_REQ*)pMsg;
	MSG_CW_FRIEND_STATUS_NAK	nakMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

#ifdef _NA_0_20100527_ADD_CHAR_RELATION_MEMO
    RC::eFRIEND_RESULT friend_result = 
        pUser->CheckConditionAccessFriendInfo(pRecvMsg->m_dwToCharGuid);

    BYTE error_code = RC::RC_FRIEND_SUCCESS;

    switch (friend_result)
    {
    case RC::RC_FRIEND_SUCCESS:
        {
            MSG_DW_FRIEND_DETAIL_INFO_SYN msg;
            msg.m_dwKey = pUser->GetCharGuid();
            msg.m_dwToCharGuid = pRecvMsg->m_dwToCharGuid;

            ServerSession* pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
            if (pGameDBProxy != NULL)
                pGameDBProxy->Send((BYTE*)&msg, sizeof(msg));
            else
                error_code = RC::RC_FRIEND_FAIL;
        }
        break;

    case RC::RC_FRIEND_NOTGUILDMEMBER:
        {
            MSG_WZ_GUILD_CHECK_CONDITION_ACCESS_DETAIL_INFO_SYN msg;
            msg.m_dwKey = pUser->GetCharGuid();
            msg.m_guildGuid = pUser->GetGuildGuid();
            msg.m_dwToCharGuid = pRecvMsg->m_dwToCharGuid;

            ServerSession* pGuildServer = ServerSessionManager::Instance()->GetGuildServer();
            if (pGuildServer != NULL)
                pGuildServer->Send((BYTE*)&msg, sizeof(msg));
            else
                error_code = RC::RC_FRIEND_FAIL;
        }
        break;

    default:
        error_code = static_cast<BYTE>(friend_result);
        break;
    }

    if (error_code != RC::RC_FRIEND_SUCCESS)
    {
        MSG_CW_FRIEND_STATUS_NAK nak_msg;
        nak_msg.m_byErrorCode = error_code;
        pUser->SendPacket((BYTE*)&nak_msg, sizeof(nak_msg));
    }
#else
	// Ȥ�� ���� �� �ִ� ���ڿ� ���� ����
    // NOTE: fixed, prevent changes about cg network buffer stream
    char char_name[MAX_CHARNAME_LENGTH + 1];
    strncpy(char_name, pRecvMsg->m_ptszCharName, _countof(char_name));
    char_name[_countof(char_name) - 1] = '\0';

	User* pTarget = UserManager::Instance()->FindUser(char_name);

	if( !pTarget )
	{
		nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTCONNECTED;	// �ش� ���� ������
		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_REQ Error - Not Connected Target[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name);
		return;
	}

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING		
	STRUCT_FRIEND_INFO *pFriendInfo = pUser->GetFriendMap()->FindFriend( pTarget->GetCharGuid() );
#else
	STRUCT_FRIEND_INFO *pFriendInfo = pUser->FindFriend( pTarget->GetCharGuid() );
#endif
	
	if( (!pFriendInfo || pFriendInfo->dwFriendGuid != pTarget->GetCharGuid()) &&        // ģ�� ���谡 �ƴϰ�
        (!pUser->GetGuildGuid() || pUser->GetGuildGuid() != pTarget->GetGuildGuid()) )  // ���� ������ �ƴϸ�
	{
		// Ȥ�ó� ������ ������� �ѹ� �� Ȯ��
		STRUCT_REVENGE_INFO *pRevengeInfo = pUser->FindRevenge( pTarget->GetCharGuid() );

		if( !pRevengeInfo || pRevengeInfo->dwRevengeCharGuid != pTarget->GetCharGuid() )
		{
			nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTFRIEND;	// ģ���� �ƴϰų� �߸��� ������
			pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

			MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_REQ Error - Not Freind or Incorrect Name[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name);
			return;
		}
	}

	// ģ�� ���� ����
	MSG_CW_FRIEND_STATUS_ANS	ansMsg;

	ansMsg.m_byChannel = (BYTE)pTarget->GetChannelID();
	ansMsg.m_byZoneType = pTarget->GetStatus();
	ansMsg.m_dwZoneKey = pTarget->GetZoneKey();
	ansMsg.m_MapCode = pTarget->GetMapCode();
    strncpy(ansMsg.m_ptszCharName, char_name, _countof(ansMsg.m_ptszCharName));
    ansMsg.m_ptszCharName[_countof(ansMsg.m_ptszCharName) - 1] = '\0';
    ZeroMemory(ansMsg.m_tszGuildName, sizeof(ansMsg.m_tszGuildName));

	// ��� ���Ե� ������ ��� ���� ������ ����
	if( pTarget->GetGuildGuid() )
	{
		WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild( pTarget->GetGuildGuid() );

		if( pGuild )
		{
			WorldGuildMember *pMember = pGuild->FindMember( pTarget->GetCharGuid() );

			if( pMember )
			{
                strncpy(ansMsg.m_tszGuildName, pGuild->GetGuildName(),
                        _countof(ansMsg.m_tszGuildName));
                ansMsg.m_tszGuildName[_countof(ansMsg.m_tszGuildName) - 1] = '\0';
				ansMsg.m_byDuty = pMember->GetDuty();
			}
		}
	}

	pUser->SendPacket( (BYTE*)&ansMsg, sizeof(ansMsg) );
    MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_REQ Answer[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name);
#endif
}

//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
Handler_CW_IMPL( CW_FRIEND_MEMO_UPDATE_SYN )
{
    MSG_CW_FRIEND_MEMO_UPDATE_SYN* pRecvMsg = (MSG_CW_FRIEND_MEMO_UPDATE_SYN*)pMsg;

    if (CheckChatBlockZone(pUser) == 0)
        return;

    RC::eFRIEND_RESULT friend_result = 
        pUser->CheckConditionAccessFriendInfo(pRecvMsg->m_dwToCharGuid);

    BYTE error_code = RC::RC_FRIEND_SUCCESS;

    switch (friend_result)
    {
    case RC::RC_FRIEND_SUCCESS:
    case RC::RC_FRIEND_NOTGUILDMEMBER:          // �ѹ� �̻� ���� ���� ���� �������� ����
        {                                       // ������� �Դٸ�, �� ���� â�� �����ٴ� ���̸�, ��弭���� �� üũ�� �� ��Ȳ�̹Ƿ� ��� �Ѵ�.
            MSG_DW_FRIEND_MEMO_UPDATE_SYN msg;
            msg.m_dwKey = pUser->GetCharGuid();
            msg.m_dwToCharGuid = pRecvMsg->m_dwToCharGuid;

            memcpy(msg.m_ptszMemo, pRecvMsg->m_ptszMemo, sizeof(TCHAR)*MAX_FRIEND_MEMO_LENGHT);
            msg.m_ptszMemo[MAX_FRIEND_MEMO_LENGHT] = '\0';

            ServerSession* pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
            if (pGameDBProxy != NULL)
                pGameDBProxy->Send((BYTE*)&msg, sizeof(msg));
            else
                error_code = RC::RC_FRIEND_FAIL;
        }
        break;

    default:
        error_code = static_cast<BYTE>(friend_result);
        break;
    }

    if (error_code != RC::RC_FRIEND_SUCCESS)
    {
        MSG_CW_FRIEND_MEMO_UPDATE_NAK nak_msg;
        nak_msg.m_byErrorCode = error_code;
        pUser->SendPacket((BYTE*)&nak_msg, sizeof(nak_msg));
    }
}

// ģ�� ���󰡱�
Handler_CW_IMPL( CW_FRIEND_STATUS_JOIN_SYN ) 
{
	MSG_CW_FRIEND_STATUS_JOIN_SYN*	pRecvMsg = (MSG_CW_FRIEND_STATUS_JOIN_SYN*)pMsg;
	MSG_CW_FRIEND_STATUS_JOIN_NAK	nakMsg;

	// Ȥ�� ���� �� �ִ� ���ڿ� ���� ����
    // NOTE: fixed, prevent changes about cg network buffer stream
    char char_name[MAX_CHARNAME_LENGTH + 1];
    strncpy(char_name, pRecvMsg->m_ptszCharName, _countof(char_name));
    char_name[_countof(char_name) - 1] = '\0';

	User* pTarget = UserManager::Instance()->FindUser(char_name);

    if( !pTarget)
    {
        nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTCONNECTED;	// �ش� ���� ������
        pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

        MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_JOIN_SYN Error - Not Connected Target[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name );
        return;
    }
    // __KR_001355_20090608_GROUPINGSYSTEM_CW
    STRUCT_GROUP_MEMBER_INFO* pMember = GroupManager::Instance()->FindGroupMember( pUser, pTarget->GetCharGuid() );

    //BUG:5045 ������
    //�ؿ��� �׷� ���ϴ� �κ��� �����Ƿ� �ּ�ó�� �Ѵ�.
    //if( !pMember)
    //{
    //    nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTCONNECTED;	// �ش� ���� ������
    //    pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

    //    MessageOut( eFULL_LOG, "GROUP_MEMBER_JOIN Error - Not Connected Member[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name );
    //    return;
    //}
    //BUG:5045

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING			
	STRUCT_FRIEND_INFO *pFriendInfo = pUser->GetFriendMap()->FindFriend( pTarget->GetCharGuid() );
#else
	STRUCT_FRIEND_INFO *pFriendInfo = pUser->FindFriend( pTarget->GetCharGuid() );
#endif
	
    if( (!pFriendInfo || pFriendInfo->dwFriendGuid != pTarget->GetCharGuid()) && !pMember )    
	{
		// Ȥ�ó� ������ ������� �ѹ� �� Ȯ��
		STRUCT_REVENGE_INFO *pRevengeInfo = pUser->FindRevenge( pTarget->GetCharGuid() );

		if( !pRevengeInfo || pRevengeInfo->dwRevengeCharGuid != pTarget->GetCharGuid() )
		{
			memcpy( nakMsg.m_ptszCharName, char_name, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
			nakMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
			nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTFRIEND;	// ģ���� �ƴϰų� �߸��� ������
			pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

			MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_JOIN_SYN Error - Not Freind or Incorrect Name[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name );
			return;
		}
	}

	// ä���� �ٸ���
	if( pTarget->GetChannelID() != pUser->GetChannelID() )
	{
		memcpy( nakMsg.m_ptszCharName, char_name, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
		nakMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
		nakMsg.m_byErrorCode = RC::RC_FRIEND_DIFFCHANNEL;	// ä�� �޶��~
		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_JOIN_SYN Error - Diff Channel[CharGuid:%u][%u][Target:%s][%u]", pUser->GetCharGuid(), 
																											pUser->GetChannelID(), 
																											char_name, 
																											pTarget->GetChannelID() );
		return;
	}

	// �� Ÿ���� �߸��Ǿ�����
	if( pTarget->GetStatus() != eZONETYPE_LOBBY && pTarget->GetStatus() != eZONETYPE_CHUNTING )
	{
		memcpy( nakMsg.m_ptszCharName, char_name, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
		nakMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
		nakMsg.m_byErrorCode = RC::RC_FRIEND_INVALIDZONE;
		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_JOIN_SYN Error - Invalid ZoneType[CharGuid:%u][Target:%s][ZT:%u]", pUser->GetCharGuid(), 
																												char_name, 
																												pTarget->GetStatus() );
		return;
	}

	// ���� ��Ʋ�� ���� �ִ´�
	MSG_CW_FRIEND_STATUS_JOIN_ACK sendMsg;

	sendMsg.m_byZoneType = pTarget->GetStatus();
	sendMsg.m_dwZoneKey = pTarget->GetZoneKey();
	memcpy( sendMsg.m_ptszCharName, char_name, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
	sendMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';

	pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );

	MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_JOIN_ACK send[CharGuid:%u][Target:%s][ZT:%u][ZK:%u]", pUser->GetCharGuid(),
																								char_name, 
																								pTarget->GetStatus(),
																								pTarget->GetZoneKey() );
}

// ������ ����Ʈ���� ����
Handler_CW_IMPL( CW_REVENGE_DEL_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}

	MSG_CW_REVENGE_DEL_SYN		*pRecvMsg = (MSG_CW_REVENGE_DEL_SYN*)pMsg;

	// Ȥ�� ������ �𸣴� ���ڿ� ���� ���� ����
    // NOTE: fixed, prevent changes about cg network buffer stream
    char char_name[MAX_CHARNAME_LENGTH + 1];
    strncpy(char_name, pRecvMsg->m_ptszCharName, _countof(char_name));
    char_name[_countof(char_name) - 1] = '\0';

	// ����ģ�� �¾�?
	STRUCT_REVENGE_INFO* pRevengeInfo = pUser->FindRevenge( char_name );
	if( !pRevengeInfo )
	{
		MSG_CW_REVENGE_DEL_NAK	sendMsg;

		memset( sendMsg.m_ptszCharName, 0, sizeof(TCHAR) * (MAX_CHARNAME_LENGTH+1) );
		_tcsncpy( sendMsg.m_ptszCharName, char_name, MAX_CHARNAME_LENGTH );
		sendMsg.m_byErrorCode = RC::RC_REVENGE_INVALID_CHAR;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_REVENGE_DEL_NAK) );
		MessageOut( eCRITICAL_LOG, "Revenge Del Error - Not Revenge[%s][CharGuid:%u]->[%s]", pUser->GetCharName().c_str(), 
																				pUser->GetCharGuid(),
																				char_name );
		return;
	}

	MSG_DW_CHAO_REMOVE_REVENGE_CHAR_SYN	sendMsg;

	sendMsg.m_dwKey			= pUser->GetChannelID();
	sendMsg.m_CharGuidFrom	= pUser->GetCharGuid();
	sendMsg.m_RevengeGuidTo	= pRevengeInfo->dwRevengeCharGuid;

	// DB�� ��û~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_REVENGE_DEL_NAK	sendMsg;

		memset( sendMsg.m_ptszCharName, 0, sizeof(TCHAR) * (MAX_CHARNAME_LENGTH+1) );
		_tcsncpy( sendMsg.m_ptszCharName, char_name, MAX_CHARNAME_LENGTH );
		sendMsg.m_byProtocol = RC::RC_REVENGE_FAILED;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_REVENGE_DEL_NAK) );
		MessageOut( eCRITICAL_LOG,  "Revenge Del Error - No GameDBProxy(User %s)", pUser->GetCharName().c_str() );
		return;
	}
	pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(MSG_DW_CHAO_REMOVE_REVENGE_CHAR_SYN) );

	pUser->SetFriendTransaction( TRUE );

	MessageOut(eFULL_LOG,  "Revenge Del to GameDBProxy(%s->%s)", pUser->GetCharName().c_str(), pRevengeInfo->sPacketInfo.ptszRevengeName );
}


// ���� ���� �������� ó��
Handler_CW_IMPL( CW_MEMO_SEND_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsMemoTransaction() )
	{
		MSG_CW_MEMO_SEND_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_DOING_TRANSACTION;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eMIDDLE_LOG,  "Memo Send Error - Already Doing Transaction(User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}

	MSG_CW_MEMO_SEND_SYN*	pRecvMsg = (MSG_CW_MEMO_SEND_SYN*)pMsg;
	MSG_DW_MEMO_SEND_SYN	sendMsg;

	util::cSTRING_REF strMemoRD( pRecvMsg->m_ptszMemo, sizeof(pRecvMsg->m_ptszMemo) );
	util::cSTRING_REF_RW_WITH_BUFFER<MAX_MEMO_LENGTH + 1> strMemoRW;
	strMemoRD.CopyTo( strMemoRW );
	strMemoRW.MakeSafeString();

	User* pTargetUser = UserManager::Instance()->FindUser(pRecvMsg->m_ptszRecvName);
	if(pTargetUser && pTargetUser->FindBlock(pUser->GetCharGuid()))
	{
		MSG_CW_MEMO_SEND_NAK	nakMsg;
		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;
		pUser->SendPacket((BYTE*)&nakMsg, sizeof(nakMsg));
		MessageOut( eMIDDLE_LOG,  "Memo Send Fail By BlockFriend - (User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}

	if( !ProhibitionWordParser::Instance()->CheckFilter( strMemoRW ) )
	{
		MSG_CW_CHAT_NORMAL_NAK	sendMsg;
		sendMsg.m_wErrorCode = RC::RC_CHAT_USE_BAN_WORD;
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_CHAT_NAK) );
		MessageOut( eCRITICAL_LOG, "Memo Send Error - USE BAN WORD[%s][CharGuid:%u]", pUser->GetCharName().c_str(), pUser->GetCharGuid());
		return;
	}

	//20070131_�޸� Ư������ �Ľ��ϰ� �������� �ٿ��ֱ�
#ifdef _KOREA
	ProhibitionWordParser::Instance()->CheckIncludeSymbol(pRecvMsg->m_ptszMemo);
#endif

	sendMsg.m_dwKey = pUser->GetGUID();
	sendMsg.m_SendGuid = pUser->GetCharGuid();
	sendMsg.m_ptszRecvName[MAX_CHARNAME_LENGTH] = '\0';
	_tcsncpy( sendMsg.m_ptszRecvName, pRecvMsg->m_ptszRecvName, MAX_CHARNAME_LENGTH );
	sendMsg.m_ptszMemo[MAX_MEMO_LENGTH] = '\0';
	_tcsncpy( sendMsg.m_ptszMemo, pRecvMsg->m_ptszMemo, MAX_MEMO_LENGTH );
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    sendMsg.m_typeSystemMemo = NORMAL_MEMO;
    sendMsg.m_recvItemCode = 0;
    sendMsg.m_amountCost = 0;
    sendMsg.m_recvGuid = 0;
    //}
	// DB�� ��û~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_MEMO_SEND_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// �׳� ����

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eCRITICAL_LOG,  "Memo Send Error - No GameDBProxy(User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}
	pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(MSG_DW_MEMO_SEND_SYN) );

	pUser->SetMemoTransaction( TRUE );

	MessageOut( eFULL_LOG,  "Memo Send (%s[%u]->%s)", pUser->GetCharName().c_str(), pUser->GetCharGuid(), sendMsg.m_ptszRecvName );
}

Handler_CW_IMPL( CW_MEMO_LIST_REQ )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsMemoTransaction() )
	{
		MSG_CW_MEMO_LIST_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_DOING_TRANSACTION;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eMIDDLE_LOG,  "Memo List Req Error - Already Doing Transaction(User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}

	MSG_CW_MEMO_LIST_REQ*	pRecvMsg = (MSG_CW_MEMO_LIST_REQ*)pMsg;

	// �ð� üũ�ϰ� �ٷ� ����
	if( !pUser->CheckMemoList() )	// �̹� ��� ��� �޾����� ���� ����
	{
		MSG_CW_MEMO_LIST_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_WAIT_INFO;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eMIDDLE_LOG, "[%s][CharGuid:%u]Memo List Req Error - MemoList Time Checked", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}

	// DB�� ��û~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_MEMO_LIST_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// �׳� ����

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eCRITICAL_LOG,  "Memo List Req Error - No GameDBProxy(User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}

	MSG_DW_MEMO_LIST_REQ	sendMsg;

	sendMsg.m_RecvGuid = pUser->GetCharGuid();

	pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(MSG_DW_MEMO_LIST_REQ) );

	pUser->SetMemoTransaction( TRUE );

	MessageOut( eFULL_LOG,  "Memo List Req To DB(%s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
}

Handler_CW_IMPL( CW_MEMO_VIEW_REQ )
{
    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsMemoTransaction() )
	{
		MSG_CW_MEMO_VIEW_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_DOING_TRANSACTION;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eMIDDLE_LOG,  "Memo View Req Error - Already Doing Transaction(User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}

	MSG_CW_MEMO_VIEW_REQ*	pRecvMsg = (MSG_CW_MEMO_VIEW_REQ*)pMsg;

	// DB�� ��û~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_MEMO_VIEW_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// �׳� ����

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eCRITICAL_LOG,  "Memo View Req Error - No GameDBProxy(User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}
	MSG_DW_MEMO_VIEW_REQ	sendMsg;

	sendMsg.m_MemoGuid = pRecvMsg->m_MemoGuid;
	sendMsg.m_RecvGuid = pUser->GetCharGuid();

	pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(MSG_DW_MEMO_VIEW_REQ) );

	pUser->SetMemoTransaction( TRUE );

	MessageOut( eFULL_LOG,  "Memo View Req To DB(%s[%u][Msg:%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid(), sendMsg.m_MemoGuid );
}

Handler_CW_IMPL( CW_MEMO_DELETE_SYN )
{
    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsMemoTransaction() )
	{
		MSG_CW_MEMO_DELETE_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_DOING_TRANSACTION;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eMIDDLE_LOG,  "Memo Delete Error - Already Doing Transaction(User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}

	MSG_CW_MEMO_DELETE_SYN*	pRecvMsg = (MSG_CW_MEMO_DELETE_SYN*)pMsg;

	// DB�� ��û~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_MEMO_DELETE_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// �׳� ����

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eCRITICAL_LOG,  "Memo Delete Error - No GameDBProxy(User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}

	MSG_DW_MEMO_DELETE_SYN	sendMsg;

	sendMsg.m_MemoGuid = pRecvMsg->m_MemoGuid;
	sendMsg.m_RecvGuid = pUser->GetCharGuid();
    sendMsg.m_MemoType = pRecvMsg->m_MemoType;
	pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );

	MessageOut( eFULL_LOG,  "Memo Delete Req To DB(%s[%u][Msg:%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid(), sendMsg.m_MemoGuid );
}


Handler_CW_IMPL(CW_GUILD_CHANGE_NOTICE_SYN)
{
    if(CheckChatBlockZone(pUser) == 0)
        return;

	MSG_CW_GUILD_CHANGE_NOTICE_SYN* pRecvMsg = (MSG_CW_GUILD_CHANGE_NOTICE_SYN*)pMsg;
	if(!pUser)	return;

	int nLen = 0;
	RC::eGUILD_RESULT rc = WorldGuildManager::Instance()->CanChangeNotice( pUser->GetGuildGuid(), pUser->GetCharGuid(), pRecvMsg->m_tszGuildNotice, nLen );
	if( rc != RC::RC_GUILD_SUCCESS )
	{
		MSG_CW_GUILD_CHANGE_NOTICE_NAK	nakMsg;
		nakMsg.m_byErrorCode = rc;
		pUser->SendPacket((BYTE*)&nakMsg, sizeof(nakMsg));
		return;
	}

	ServerSession *pGuildServer = ServerSessionManager::Instance()->GetGuildServer();
	if( !pGuildServer )		return;

	// ���� üũ�� ���� ��弭���� ���ؾ� ��
	MSG_WZ_GUILD_CHANGE_NOTICE_SYN	synMsg;
	synMsg.m_GuildGuid = pUser->GetGuildGuid();
	synMsg.m_OperCharGuid = pUser->GetCharGuid();
	synMsg.m_wNoticeLen = nLen;
	strncpy( synMsg.m_szGuildNotice, pRecvMsg->m_tszGuildNotice, MAX_GUILDNOTICE_LENGTH );
	synMsg.m_szGuildNotice[MAX_GUILDNOTICE_LENGTH] = '\0';
	if( !pGuildServer->Send( (BYTE*)&synMsg, synMsg.GetSize() ) )
	{
		MSG_CW_GUILD_CHANGE_NOTICE_NAK	nakMsg;
		nakMsg.m_byErrorCode = RC::RC_GUILD_INVALID_STATE;
		pUser->SendPacket((BYTE*)&nakMsg, sizeof(nakMsg));
		return;
	}
}

Handler_CW_IMPL(CW_GUILD_SELECT_NOTICE_SYN)
{
	PACKET_PTR_CONVERT(MSG_CW_GUILD_SELECT_NOTICE_SYN, pRecvMsg, pMsg);
	if(!pUser) return;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild(pRecvMsg->m_GuildGuid);
	if(pGuild)
	{
		MSG_CW_GUILD_SELECT_NOTICE_ACK msg;
		msg.m_wNoticeLen = strlen(pGuild->GetGuildNotice());
		strncpy( msg.m_ptszNotice, pGuild->GetGuildNotice(), MAX_GUILDNOTICE_LENGTH);
		msg.m_ptszNotice[MAX_GUILDNOTICE_LENGTH] = '\0';
		pUser->SendPacket((BYTE*)&msg, sizeof(msg));
		return;

	}

	MSG_DW_GUILD_SELECT_NOTICE_SYN msg;
	msg.m_GuildGuid = pUser->GetGuildGuid();
	msg.m_OperCharGuid = pUser->GetCharGuid();
	msg.m_NoticeType = 0;

	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if(pGameDBProxy && pGameDBProxy->Send( (BYTE*)&msg, sizeof(msg))) return;
	
	// ��û ����
	MSG_CW_GUILD_SELECT_NOTICE_NAK nak;
	nak.m_dwErrorCode = RC::RC_GUILD_INVALID_STATE;
	pUser->SendPacket((BYTE*)&nak, sizeof(nak));
}

Handler_CW_IMPL(CW_GUILD_WAREHOUSE_LOGLIST_REQ)
{
	MSG_CW_GUILD_WAREHOUSE_LOGLIST_REQ* pRecvMsg = (MSG_CW_GUILD_WAREHOUSE_LOGLIST_REQ*)pMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if(!pGameDBProxy)
	{
		MSG_CW_GUILD_WAREHOUSE_LOGLIST_NAK	nakMsg;
		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// �׳� ����
		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
#ifdef _DEBUG
		MessageOut( eCRITICAL_LOG,  "Memo List Req Error - No GameDBProxy(User %s[%u])", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
#endif
		return;
	}

	MSG_DW_GUILD_WAREHOUSE_LOGLIST_REQ msg;
	msg.m_GuildGuid = pUser->GetGuildGuid();
	msg.m_ReqCharGuid = pUser->GetCharGuid();
	pGameDBProxy->Send((BYTE*)&msg, sizeof(msg));
}

Handler_CW_IMPL(CW_LOTTO_INFO_SYN)
{
    MSG_CW_LOTTO_INFO_SYN* pRecvMsg = static_cast<MSG_CW_LOTTO_INFO_SYN*>(pMsg);
    ASSERT(pUser != NULL);
    if (pUser == NULL) { return; }

    Lotto::GetInstance().OnInfoRequest(pUser->GetCharGuid(), pMsg);
}

Handler_CW_IMPL(CW_LOTTO_ENTRY_SYN)
{
    MSG_CW_LOTTO_ENTRY_SYN* pRecvMsg = static_cast<MSG_CW_LOTTO_ENTRY_SYN*>(pMsg);
    ASSERT(pUser != NULL);
    if (pUser == NULL) { return; }

    Lotto::GetInstance().OnEntryRequest(pUser->GetCharGuid(), pMsg);
}

Handler_CW_IMPL(CW_LOTTO_NOTICE_LIST_SYN)
{
    MSG_CW_LOTTO_NOTICE_LIST_SYN* pRecvMsg = static_cast<MSG_CW_LOTTO_NOTICE_LIST_SYN*>(pMsg);
    ASSERT(pUser != NULL);
    if (pUser == NULL) { return; }

    Lotto::GetInstance().OnNoticeRequest(pUser->GetCharGuid(), pMsg);
}

Handler_CW_IMPL(CW_LOTTO_NOTICE_CONFIRM)
{
    MSG_CW_LOTTO_NOTICE_CONFIRM* pRecvMsg = static_cast<MSG_CW_LOTTO_NOTICE_CONFIRM*>(pMsg);
    ASSERT(pUser != NULL);
    if (pUser == NULL) { return; }

    Lotto::GetInstance().OnNoticeConfirm(pUser->GetCharGuid(), pMsg);
}

//////////////////////////////////////////////////////////////////////////
//�׷�α� �ý��� �ڵ鷯 �Լ�
//////////////////////////////////////////////////////////////////////////
//Ŭ���̾�Ʈ�� ���� �׷� �ɹ� �߰� ��û
Handler_CW_IMPL(CW_GROUP_ADD_SYN)
{
    MSG_CW_GROUP_ADD_SYN *recv_msg = (MSG_CW_GROUP_ADD_SYN*)pMsg;

    if (CheckChatBlockZone(pUser) == FALSE)
    {  
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Current zone is chat block zone");
        return;
    }

    GroupManager* group_manager = GroupManager::Instance();

    User* dest_user = UserManager::Instance()->FindUser(recv_msg->ptszMemeberName);
    if (dest_user == NULL)
    {
        //-- ��� ������ ���ӿ� ������ ���� �ʴ�.
        MSG_CW_GROUP_ADD_NAK nak_msg;
        nak_msg.m_byResult = MSG_CW_GROUP_ADD_NAK::NO_USER;
        pUser->SendPacket((BYTE*)&nak_msg, sizeof(nak_msg));

        SUNLOG(eFULL_LOG, "["__FUNCTION__"]%s Request to logout user",recv_msg->ptszMemeberName);
        return;
    }

    if (group_manager->CanAddGroupMember(pUser, dest_user, pMsg) == false)
    {
        SUNLOG(eFULL_LOG, "["__FUNCTION__"]Group Member Add Fail");
        return;
    }

    //-- ������ ������ �����Ѵ�.
    pUser->SetStructMemberInfo(dest_user, GROUP_LEVEL_LOW);
    dest_user->SetStructMemberInfo(pUser, GROUP_LEVEL_HIGH);

    dest_user->SetGroupTransaction(TRUE);
    pUser->SetGroupTransaction(TRUE);

    MSG_CW_GROUP_ADD_RESPONSE_SYN resp_msg;
    {
        _tcsncpy(resp_msg.ptszMemeberName,  pUser->GetCharName().c_str(),
                 _countof(resp_msg.ptszMemeberName));
        resp_msg.ptszMemeberName[_countof(resp_msg.ptszMemeberName) - 1] = '\0';
    }
    dest_user->SendPacket((BYTE*)&resp_msg, sizeof(resp_msg));
}

//�׷� �ʴ� ���� ����� ������ �Ͽ���.
Handler_CW_IMPL(CW_GROUP_ADD_RESPONSE_ACK)
{
    MSG_CW_GROUP_ADD_RESPONSE_ACK* recv_msg = static_cast<MSG_CW_GROUP_ADD_RESPONSE_ACK*>(pMsg);

    if (CheckChatBlockZone(pUser) == FALSE)
    {
        return;
    }

    User* request_user = UserManager::Instance()->FindUser(recv_msg->ptszMemeberName);
    if (request_user == NULL)
    {
        SUNLOG(eFULL_LOG,"["__FUNCTION__"]Not found user");
        return;
    }

    GroupManager::Instance()->SetGroupHighMember(pUser, GROUP_HAVE_ONLINE_HIGH);

    MSG_DW_GROUP_ADD_SYN send_msg;
    {
        send_msg.m_fromCharGUID = request_user->GetCharGuid();
        send_msg.m_toCharGUID = pUser->GetCharGuid();    //�߿��� ���� : 
        send_msg.m_GroupSTATFirst = GROUP_LEVEL_LOW;     //DB�� destChar�� Char�� ���� �����̴�.
        send_msg.m_GroupSTATSecond = GROUP_LEVEL_HIGH;   //DB�� destChar�� Char�� ���� �����̴�.
    }
    ServerSession *game_db_proxy = ServerSessionManager::Instance()->GetGameDBProxy();
    if (game_db_proxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Not found gamedbproxy");
        return;
    }
    game_db_proxy->Send((BYTE*)&send_msg, sizeof(send_msg));
}

//�׷� �ʴ� ���� ����� ���� ���� ������ �����Ͽ���.
Handler_CW_IMPL( CW_GROUP_ADD_RESPONSE_NAK )
{
    const MSG_CW_GROUP_ADD_RESPONSE_NAK* recv_msg = 
        static_cast<MSG_CW_GROUP_ADD_RESPONSE_NAK*>(pMsg);

    if (CheckChatBlockZone(pUser) == FALSE)
    {
        return;
    }

    User* request_user = UserManager::Instance()->FindUser(recv_msg->ptszMemeberName);
    if (request_user == NULL) 
    {
        SUNLOG( eCRITICAL_LOG,"["__FUNCTION__"]Not found user" );
        return;
    }

    request_user->SetGroupTransaction(FALSE);
    pUser->SetGroupTransaction(FALSE);

    MSG_CW_GROUP_ADD_NAK nak_msg;
    {
        nak_msg.m_byResult = recv_msg->m_byResult;

        _tcsncpy(nak_msg.ptszMemeberName,  pUser->GetCharName().c_str(), 
                 _countof(nak_msg.ptszMemeberName));
        nak_msg.ptszMemeberName[_countof(nak_msg.ptszMemeberName) - 1] = '\0';
    }
    request_user->SendPacket((BYTE*)&nak_msg, sizeof(nak_msg));
}


//Ŭ���̾�Ʈ�� ���� �׷� �ɹ� ����Ʈ ��û����
Handler_CW_IMPL( CW_GROUP_GROUPLIST_REQ )
{
    //MSG_CW_GROUP_GROUPLIST_REQ ��Ŷ ������ ����.
    if (CheckChatBlockZone(pUser) == 0)
        return;

    GroupManager::Instance()->SendMemberList(pUser);
}


//�׷� �ɹ� �� ���� ��û
Handler_CW_IMPL( CW_GROUP_STATUS_REQ )
{
    if(CheckChatBlockZone(pUser) == 0)
        return;

    MSG_CW_GROUP_STATUS_REQ* pRecvMsg = (MSG_CW_GROUP_STATUS_REQ*)pMsg;

    if(Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID()))
    {
        if(ChatZone *pZone = pChannel->FindZone(pUser->GetZoneKey()))
        {
            if(pZone->IsBlockZone())
            {
                pUser->SendChatBlock(RC::RC_CHAT_BELONG_ZONE_BLOCK);
                return;
            }
        }
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]channel not exist(%s)(%d)", 
        pUser->GetCharName(), pUser->GetChannelID());
    }


    User* pTarget = UserManager::Instance()->FindUser( pRecvMsg->m_ptszCharName );

    //ã�� ������ ������������ ���
    if( !pTarget )
    {
        MSG_CW_GROUP_STATUS_NAK sendMsg;

        sendMsg.dwResult = MSG_CW_GROUP_STATUS_NAK::NO_MEMBER_DATA;//���� ������
        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_GROUP_STATUS_NAK) );

        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not Connected Target[Guid:%u][Target:%s]", 
            pUser->GetCharGuid(), pRecvMsg->m_ptszCharName );
        return;
    }

    STRUCT_GROUP_MEMBER_INFO* pMemberInfo = 
        GroupManager::Instance()->FindGroupMember( pUser, pTarget->GetCharGuid() );

    //�׷� �ɹ��� ���� ���� ������
    if( !pMemberInfo ) 
    {
        MSG_CW_GROUP_STATUS_NAK sendMsg;

        sendMsg.dwResult = MSG_CW_GROUP_STATUS_NAK::NOT_MEMBER;//�׷� �ɹ��� �ƴϴ�.
        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_GROUP_STATUS_NAK) );

        SUNLOG( eFULL_LOG, "["__FUNCTION__"]Can't Found Member [Guid:%u][Target:%s]", 
            pUser->GetCharGuid(), pRecvMsg->m_ptszCharName );
        return;
    }
    
    //ã�� ��� ���� ��Ȯ���� ���� ���
    if( pMemberInfo->charGUID != pTarget->GetCharGuid() )
    {
        MSG_CW_GROUP_STATUS_NAK sendMsg;

        sendMsg.dwResult = MSG_CW_GROUP_STATUS_NAK::NOT_MEMBER;//�׷� �ɹ��� �ƴϴ�.
        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_GROUP_STATUS_NAK) );

        SUNLOG( eFULL_LOG, "["__FUNCTION__"]Incorrect Info of Member [Guid:%u][Target:%s]", 
            pUser->GetCharGuid(), pRecvMsg->m_ptszCharName );
        return;
    }

    MSG_CW_GROUP_STATUS_ANS ansMsg;

    ansMsg.m_byChannel = (BYTE)pTarget->GetChannelID();
    ansMsg.m_byZoneType = pTarget->GetStatus();
    ansMsg.m_dwZoneKey = pTarget->GetZoneKey();
    ansMsg.m_MapCode = pTarget->GetMapCode();
    memset( ansMsg.m_tszGuildName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );//��û�� �ɸ��� �̸�
    _tcsncpy( ansMsg.m_ptszCharName,  pRecvMsg->m_ptszCharName, MAX_CHARNAME_LENGTH );
    ansMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';

    // ��� ���Ե� ������ ��� ���� ������ ����
    if( pTarget->GetGuildGuid() )
    {
        WorldGuild *pGuild = WorldGuildManager::Instance()->FindGuild( pTarget->GetGuildGuid() );

        if( pGuild )
        {
            WorldGuildMember *pMember = pGuild->FindMember( pTarget->GetCharGuid() );

            if( pMember )
            {
                memcpy( ansMsg.m_tszGuildName, pGuild->GetGuildName(), sizeof(TCHAR)*MAX_GUILDNAME_LENGTH );
                ansMsg.m_byDuty = pMember->GetDuty();
            }
        }
    }

    pUser->SendPacket( (BYTE*)&ansMsg, sizeof(MSG_CW_GROUP_STATUS_ANS) );
}

Handler_CW_IMPL( CW_GROUP_DEL_SYN )
{
    if(CheckChatBlockZone(pUser) == 0)
        return;

    MSG_CW_GROUP_DEL_SYN *pRecvMsg = (MSG_CW_GROUP_DEL_SYN*)pMsg;

    if( pUser->IsGroupTransaction() )
    {
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Transaction Error", 
            pRecvMsg->ptszMemeberName );
        return; 
    }

    STRUCT_GROUP_MEMBER_INFO* pMemberInfo = 
        GroupManager::Instance()->FindGroupMember( pUser, pRecvMsg->ptszMemeberName );


    //�׷���� �´��� Ȯ��
    if( !pMemberInfo )
    {
        MSG_CW_GROUP_DEL_NAK sendMsg;

        sendMsg.m_byResult = MSG_CW_GROUP_DEL_NAK::NOT_MEMBER_USER;

        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_GROUP_DEL_NAK) );
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]No Member[%s][CharGuid:%u]->[%s]", 
            pUser->GetCharName().c_str(), pUser->GetCharGuid(), pRecvMsg->ptszMemeberName );
        return;
    }

    MSG_DW_GROUP_DEL_SYN sendMsg;

    sendMsg.m_fromCharGUID = pUser->GetCharGuid();
    sendMsg.m_toCharGUID = pMemberInfo->charGUID;

    ServerSession* pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();

    if( !pGameDBProxy )
    {
        MSG_CW_GROUP_DEL_NAK sendMsg;
        sendMsg.m_byResult = MSG_CW_GROUP_DEL_NAK::ERR_DEFAULT;

        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_GROUP_DEL_NAK) );
        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]No GameDBProxy(User %s)", 
            pUser->GetCharName().c_str() );

        return;
    }

    pGameDBProxy->Send( (BYTE*)&sendMsg, sizeof(MSG_DW_GROUP_DEL_SYN));
}

Handler_CW_IMPL( CW_GROUP_ADDBLOCK_SYN )
{
    MSG_CW_GROUP_ADDBLOCK_SYN *pRecvMsg = (MSG_CW_GROUP_ADDBLOCK_SYN*)pMsg;
    if( pUser )
    {
        MSG_CW_GROUP_ADDBLOCK_ACK sendMsg;
        pUser->SetAddBlock( pRecvMsg->m_bBlock );

        sendMsg.m_bNowBlock = pUser->GetAddBlockInfo();
        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
    }
    else
    {
        MSG_CW_GROUP_ADDBLOCK_NAK sendMsg;
        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(sendMsg) );
    }

}

Handler_CW_IMPL( CW_GROUP_RESTOR_CMD )
{
    MSG_CW_GROUP_RESTOR_CMD *pRecvMsg = (MSG_CW_GROUP_RESTOR_CMD*)pMsg;

    GroupManager::Instance()->UpdateGroupInfo( pUser );
}

//������ ���� ������ ������ Ȯ���Ͽ���. DB�� �ش� ������ �ִ��� Ȯ���غ���.
Handler_CW_IMPL( CW_DOMINATION_RETURN_HEIM_CMD )
{
    MSG_CW_DOMINATION_RETURN_HEIM_CMD* pRecvMsg = 
        static_cast<MSG_CW_DOMINATION_RETURN_HEIM_CMD*>(pMsg);

    ASSERT(pUser != NULL);
    if (pUser == NULL) { return; }

    eSYSTEM_MEMO_TYPE curType;
    MAPCODE mapCode;

    if (WorldGuildManager::Instance()->GetValueInMemo(pRecvMsg->m_ptszMemo, curType, mapCode))
    {
        if (curType == SYSMEMO_DOMINATION_RETURN_MONEY || 
            curType == SYSMEMO_DOMINATION_REWARD_MONEY)
        {
            ServerSession* pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();

            if(pGameDBProxy == NULL)
            {
                SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]No GameDBProxy");
                return;
            }

            MSG_DW_DOMINATION_FINDMONEY_SYN sendMsg;
            sendMsg.m_MapCode = mapCode;
            sendMsg.m_RecvCharGuid = pUser->GetCharGuid();
            sendMsg.m_CostType = curType;

            pGameDBProxy->Send((BYTE*)&sendMsg, sizeof(sendMsg));
        }
    }
}

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
Handler_CW_IMPL( CW_NOTIFICATION_PREV_SYN )
{
    MSG_CW_NOTIFICATION_PREV_SYN *pRecvMsg = (MSG_CW_NOTIFICATION_PREV_SYN*)pMsg;

    if( pUser )
    {
        MSG_CW_NOTIFICATION_PREV_ACK ackMsg;
        NotificationManager::Instance()->GetPrevNotifications(ackMsg);
        pUser->SendPacket( (BYTE*)&ackMsg, sizeof(ackMsg) );
    }
}
#endif _NA_008283_20150511_NOTIFICATIONS_SYSTEM
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// ���� ���� ���� �Լ�
namespace {
    
// ����� ���� �α׸� ����Ѵ�.
inline void scoped_write_log(User* const request_user, 
                             const PartyFindingResultValueType result, 
                             const TCHAR* log_function_string)
{
    const BYTE log_level = (result == RC::PARTY_FINDING_SUCCESS) ? eFULL_LOG : eCRITICAL_LOG;
    
    MessageOut(log_level, 
               _T("|PartyFinding|%s|Result:%d|RequestUserCharGuid:%u|RequestUserName:%s|"), 
               log_function_string, 
               result, 
               request_user->GetCharGuid(), 
               (request_user->GetCharName()).c_str());
}

// ������ �˻��Ѵ�.
inline bool scoped_check_request_user(User* const request_user)
{
    if (request_user == NULL)
    {
        return false;
    }

    const Channel* const channel = 
        ChannelManager::Instance()->GetChannel(request_user->GetChannelID());
    if (channel == NULL)
    {
        return false;
    }

    return true;
}

} // namespace

// ��Ƽ ���� ���/������ ��û�Ǹ� ȣ��ȴ�.
Handler_CW_IMPL(CW_PARTY_FINDING_REGISTER_SYN)
{
    __UNUSED(wSize);

    // ��û�ڸ� �˻��Ѵ�.
    if (!scoped_check_request_user(pUser))
    {
        return;
    }
        
    const MSG_CW_PARTY_FINDING_REGISTER_SYN* const register_syn = 
        static_cast<const MSG_CW_PARTY_FINDING_REGISTER_SYN* const>(pMsg);

    User* const party_master = pUser;

    // ���� �޽����� �����Ѵ�.
    MSG_CW_PARTY_FINDING_RESULT_ACK result_ack;
    result_ack.response_type_ = PartyFindingResponseTypeConst::GetDefaultValue();
    
    // ��û ���� ������ ��Ƽ ���� ������ ����
    PartyFindingInfo party_finding;
    party_finding.party_finding_key_ = register_syn->update_wanted_party_finding_key_;
    {
        const size_t dest_string_size = _countof(party_finding.party_finding_title_);
        ::_tcsncpy(party_finding.party_finding_title_, register_syn->party_finding_title_, dest_string_size);
        party_finding.party_finding_title_[dest_string_size-1] = _T('\0');
    }
    party_finding.party_goal_ = register_syn->party_goal_;
    party_finding.num_of_max_joinable_member_ = register_syn->num_of_max_joinable_people_;
    {
        const size_t dest_string_size = _countof(party_finding.party_master_name_);
        ::_tcsncpy(party_finding.party_master_name_, (party_master->GetCharName()).c_str(), dest_string_size);
        party_finding.party_master_name_[dest_string_size-1] = _T('\0');
    }
    party_finding.party_master_level_ = party_master->GetCharLevel();
    party_finding.party_master_location_ = party_master->GetMapCode();
    party_finding.party_master_channel_ = party_master->GetChannelID();

    // ����ϰų� �̹� ��ϵǾ� ������ �����Ѵ�.
    PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
    if (party_finding.party_finding_key_ == PartyFindingInfo::NULL_PARTY_FINDING)
    {
        result_ack.response_type_ = PartyFindingResponseTypeConst::kRegister;
        result_ack.result_ = party_finding_manager.Register(party_finding);
    }
    else
    {
        result_ack.response_type_ = PartyFindingResponseTypeConst::kUpdate;
        result_ack.result_ = party_finding_manager.Update(party_finding);
    }
    result_ack.party_finding_key_ = party_finding.party_finding_key_;

    // ����� ��û�ڿ��� ������.
    party_master->SendPacket(reinterpret_cast<BYTE*>(&result_ack), result_ack.GetSize());

    // �α׸� ����Ѵ�.
    scoped_write_log(party_master, result_ack.result_, __FUNCTION__);
}

// ��Ƽ ���� ����� ��û�Ǹ� ȣ��ȴ�.
Handler_CW_IMPL(CW_PARTY_FINDING_GET_LIST_SYN)
{
    __UNUSED(pMsg);
    __UNUSED(wSize);

    // ��û�ڸ� �˻��Ѵ�.
    if (!scoped_check_request_user(pUser))
    {
        return;
    }

    User* const request_user = pUser;
    
    // ��Ƽ ���� ����� ��´�.
    MSG_CW_PARTY_FINDING_GET_LIST_ACK get_list_ack;
    PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
    get_list_ack.get_list_result_ = party_finding_manager.GetList(get_list_ack.party_finding_list_, 
                                                                  get_list_ack.party_finding_list_size_);

    // ����� ��û�ڿ��� ������.
    request_user->SendPacket(reinterpret_cast<BYTE*>(&get_list_ack), get_list_ack.GetSize());

    // �α׸� ����Ѵ�.
    scoped_write_log(request_user, get_list_ack.get_list_result_, __FUNCTION__);
}

// ��Ƽ ���� ���� ��û�� ��û�Ǹ� ȣ��ȴ�.
Handler_CW_IMPL(CW_PARTY_FINDING_JOIN_REQUEST_SYN)
{
    __UNUSED(wSize);

    // ��û�ڸ� �˻��Ѵ�.
    if (!scoped_check_request_user(pUser))
    {
        return;
    }
    
    const MSG_CW_PARTY_FINDING_JOIN_REQUEST_SYN* const join_request_syn = 
        static_cast<const MSG_CW_PARTY_FINDING_JOIN_REQUEST_SYN* const>(pMsg);
    
    User* const join_wanted_user = pUser;

    PartyFindingResultValueType join_request_result = RC::PartyFindingResultUtil::GetDefaultValue();

    do // == try
    {
        // �����Ϸ��� ��Ƽ ���� ������ ã�� �� �ִ��� �˻��Ѵ�.
        const PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
        const PartyFindingInfo* const join_wanted_party_finding = 
            party_finding_manager.FindPartyFinding(join_request_syn->join_wanted_party_finding_key_);
        if (join_wanted_party_finding == NULL)
        {
            // == throw
            join_request_result = RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY_FINDING;
            break;
        }
        
        // ����� ��Ƽ ������ �ִٸ�
        if (join_wanted_party_finding->linked_party_key_ != PartyFindingInfo::NULL_PARTY)
        {
            // ��Ƽ ������ ã�� �� �ִ��� �˻��Ѵ�.
            const PartyInfo* const join_wanted_party = 
                party_finding_manager.FindParty(join_wanted_party_finding->linked_party_key_);
            if (join_wanted_party != NULL)
            {
                // == throw
                join_request_result = RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY;
                break;
            }

            // ��Ƽ ���� �ο��� �˻��Ѵ�.
            if (join_wanted_party->num_of_joined_member_ >= join_wanted_party_finding->num_of_max_joinable_member_)
            {
                // == throw
                join_request_result = RC::PARTY_FINDING_FAIL_PARTY_IS_FULL;
                break;
            }
        }

        // ��Ƽ���� ã�� �� �ִ��� �˻��Ѵ�.
        User* const party_master = 
            UserManager::Instance()->FindUser(join_wanted_party_finding->party_master_name_);
        if (party_master == NULL)
        {
            // == throw
            join_request_result = RC::PARTY_FINDING_FAIL_CANT_FIND_MASTER;
            break;
        }

        // ��û�ڿ� ��Ƽ���� ä���� ������ �˻��Ѵ�.
        if (join_wanted_user->GetChannelID() != party_master->GetChannelID())
        {
            // == throw
            join_request_result = RC::PARTY_FINDING_FAIL_CHANNEL_IS_DIFFERENT;
            break;
        }

        // ���� ��û�� ���� ���� ��û �޽����� ���� ��Ƽ�忡�� ������.
        MSG_CW_PARTY_FINDING_JOIN_RESPONSE_CMD join_response_cmd;
        join_response_cmd.join_wanted_party_finding_key_ = join_request_syn->join_wanted_party_finding_key_;
        join_response_cmd.join_wanted_user_id_ = join_wanted_user->GetCharGuid();
        {
            const size_t dest_string_size = _countof(join_response_cmd.join_wanted_user_name_);
            ::_tcsncpy(join_response_cmd.join_wanted_user_name_, (join_wanted_user->GetCharName()).c_str(), dest_string_size);
            join_response_cmd.join_wanted_user_name_[dest_string_size-1] = _T('\0');
        }
        join_response_cmd.join_wanted_user_class_ = join_wanted_user->GetCharClass();
        join_response_cmd.join_wanted_user_level_ = join_wanted_user->GetCharLevel();
        party_master->SendPacket(reinterpret_cast<BYTE*>(&join_response_cmd), join_response_cmd.GetSize());
        
        // �α׸� ����Ѵ�.
        scoped_write_log(join_wanted_user, join_request_result, __FUNCTION__);

        return;

    } while (false); // == catch
    {
        // ���� ��û ���� �޽����� ���� ��û�ڿ��� ������.
        MSG_CW_PARTY_FINDING_RESULT_ACK result_ack;
        result_ack.response_type_ = PartyFindingResponseTypeConst::kJoin;
        result_ack.result_ = join_request_result;
        result_ack.party_finding_key_ = join_request_syn->join_wanted_party_finding_key_;
        join_wanted_user->SendPacket(reinterpret_cast<BYTE*>(&result_ack), result_ack.GetSize());

        // �α׸� ����Ѵ�.
        scoped_write_log(join_wanted_user, result_ack.result_, __FUNCTION__);
    }
}

// ��Ƽ�����κ��� ��Ƽ ���� ���� ��û�� ���� ������ �����ϸ� ȣ��ȴ�.
Handler_CW_IMPL(CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK)
{
    __UNUSED(wSize);

    // ��û�ڸ� �˻��Ѵ�.
    if (!scoped_check_request_user(pUser))
    {
        return;
    }
    
    const MSG_CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK* const join_response_result_ack = 
        static_cast<const MSG_CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK* const>(pMsg);

    User* const party_master = pUser;

    // ���� ��û�ڸ� �˻��Ѵ�.
    const CHARGUID join_wanted_user_id = join_response_result_ack->join_wanted_user_id_;
    User* const join_wanted_user = UserManager::Instance()->FindUserByCharGuid(join_wanted_user_id);
    if (join_wanted_user == NULL)
    {
        return;
    }

    PartyFindingResultValueType join_response_result = join_response_result_ack->join_response_result_;
    
    // ��Ƽ���� ��Ƽ ������ �¶��ߴٸ�
    typedef RC::PartyFindingResultUtil ResultUtil;
    if (join_response_result == RC::PARTY_FINDING_SUCCESS)
    {
        // ��Ƽ���� ���� ä���� �˻��Ѵ�.
        ServerSession* const agent_server = 
            ServerSessionManager::Instance()->FindAgentServer(party_master->GetChannelID());
        if (agent_server != NULL)
        {
            // ��Ƽ���� ���� ��Ƽ ������ �˻��Ѵ�.
            const PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
            const PartyFindingInfo* const join_wanted_party_finding = 
                party_finding_manager.FindPartyFinding(
                    join_response_result_ack->join_wanted_party_finding_key_);
            if (join_wanted_party_finding != NULL)
            {
                // ��Ƽ ���� ��û �޽����� ���� ������Ʈ ������ ������.
                MSG_AW_PARTY_FINDING_JOIN_SYN aw_join_syn;
                aw_join_syn.join_wanted_party_finding_key_ = join_response_result_ack->join_wanted_party_finding_key_;
                aw_join_syn.join_wanted_party_master_id_ = party_master->GetCharGuid();
                aw_join_syn.join_wanted_user_id_ = join_response_result_ack->join_wanted_user_id_;
                aw_join_syn.num_of_max_joinable_member_ = join_wanted_party_finding->num_of_max_joinable_member_;
                agent_server->Send(reinterpret_cast<BYTE*>(&aw_join_syn), aw_join_syn.GetSize());

                // �α׸� ����Ѵ�.
                scoped_write_log(party_master, RC::PARTY_FINDING_SUCCESS, __FUNCTION__);

                return;
            }
            else
            {
                join_response_result = RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY_FINDING;
            }
        }
        else
        {
            join_response_result = RC::PARTY_FINDING_FAIL_CANT_FIND_CHANNEL;
        }
    }
    
    // ���� ���� �޽����� ���� ��û�ڿ��� ������.
    MSG_CW_PARTY_FINDING_RESULT_ACK result_ack;
    result_ack.response_type_ = PartyFindingResponseTypeConst::kJoin;
    result_ack.result_ = join_response_result;
    result_ack.party_finding_key_ = join_response_result_ack->join_wanted_party_finding_key_;
    join_wanted_user->SendPacket(reinterpret_cast<BYTE*>(&result_ack), result_ack.GetSize());

    // �α׸� ����Ѵ�.
    scoped_write_log(party_master, result_ack.result_, __FUNCTION__);    
}

// ��Ƽ ���� ��Ұ� ��û�Ǹ� ȣ��ȴ�.
Handler_CW_IMPL(CW_PARTY_FINDING_CANCEL_SYN)
{
    __UNUSED(wSize);

    // ��û�ڸ� �˻��Ѵ�.
    if (!scoped_check_request_user(pUser))
    {
        return;
    }

    const MSG_CW_PARTY_FINDING_CANCEL_SYN* const cancel_syn = 
        static_cast<const MSG_CW_PARTY_FINDING_CANCEL_SYN* const>(pMsg);

    User* const party_master = pUser;

    // ���� �޽����� �����Ѵ�.
    MSG_CW_PARTY_FINDING_RESULT_ACK result_ack;
    result_ack.response_type_ = PartyFindingResponseTypeConst::kCancel;
    result_ack.party_finding_key_ = cancel_syn->cancel_wanted_party_finding_key_;

    // ��Ƽ ������ ����Ѵ�.
    PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
    result_ack.result_ = 
        party_finding_manager.Cancel(party_master, cancel_syn->cancel_wanted_party_finding_key_);

    // ��� ����� ��û�ڿ��� ������.
    party_master->SendPacket(reinterpret_cast<BYTE*>(&result_ack), result_ack.GetSize());

    // �α׸� ����Ѵ�.
    scoped_write_log(party_master, result_ack.result_, __FUNCTION__);
}

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
