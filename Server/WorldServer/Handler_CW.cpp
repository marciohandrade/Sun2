#include "stdafx.h"
#include "Handler_CW.h"
#include "User.h"
#include "WorldServer.h"
#include "UserManager.h"

//GM판정을 위해
//#include "GMList.h"
#include <PacketStruct_CW.h>
#include <Protocol_CW.h>

// 채팅 메시지 전송을 위해
#include "ChannelManager.h"
#include "Channel.h"
#include "BaseZone.h"

// ArmorKit 프로토콜 릴레이 위해
#include "ServerSession.h"
#include "ServerSessionManager.h"
#include <PacketStruct_MW.h>

// 친구 관리 위해
#include <PacketStruct_DW.h>
#include <Protocol_DW.h>

// 길드 관리 위해
#include "WorldGuildManager.h"
#include "WorldGuild.h"
#include "WorldGuildMember.h"
#include <PacketStruct_WZ.h>

// GM 스트링 명령 파싱 위해
#include "GMCmdParser.h"
#include "GMCmdString.h"
#include "GMCmdManager.h"

// 채널 채팅 위해
#include "ChatRoomManager.h"

// 암호화 위해
#include "WorldServerInfoParser.h"
#include <ProhibitionWordParser.h>
#include <GameOptionParser.h>

#include <PacketStruct_AW.h>
#include <Protocol_AW.h>
#include "BaseZone.h"

#include "Lotto/Lotto.h"
// 그룹시스템
#include "GroupManager.h"

//친구 초대 이벤트
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

	// 모든 채팅에서 크기가 0이거나 실제 내용이 없으면 무시
//	if( !pRecvMsg->byMsgLen || !_tcslen(pRecvMsg->szWhisperMsg) )
//	{
//		MessageOut( eFULL_LOG, "채팅 내용 없음" );
//		return;
//	}

	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

    //__NA001094_080928_TASK_SURVIVAL_SOLOING_QUEST_ETC__
    if(CheckChatBlockZone(pUser) == 0)
        return;
	
	//채팅 금지라면
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();
		return;
	}

	// 귓말 사이즈가 너무 크면 접속을 끊는다. 라기 보단, 패킷 위조?
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

	// 안전한 버퍼 처리 1
	util::cSTRING_REF strNameRD( pRecvMsg->szCharNameTo, sizeof(pRecvMsg->szCharNameTo) );
	util::cSTRING_REF_RW_WITH_BUFFER<sizeof(pRecvMsg->szCharNameTo)> strNameRW;
	strNameRD.CopyTo( strNameRW );
	strNameRW.MakeSafeString();

	// 안전한 버퍼 처리 2
	util::cSTRING_REF_RW_WITH_BUFFER<sizeof(pRecvMsg->szWhisperMsg)> strMsgRW;
	strMsgRD.CopyTo( strMsgRW );
	strMsgRW.MakeSafeString();

	//if( strMsgRW.GetSTRING_LENGTH() != byMsgLen )
	//{
	//	MessageOut( eCRITICAL_LOG, _T("Whisper Chat Size Mismatch(%d:%d)"), strMsgRW.GetSTRING_LENGTH(), byMsgLen );
	//	return;
	//}

	// find User std::string이다... 복사다... 좋지 못하다.
	User* pTargetUser = UserManager::Instance()->FindUser( strNameRW.GetBUFFER() );

	// 귓말 대상이 없으면 실패 처리
	if( !pTargetUser || eZONETYPE_MAX == pTargetUser->GetStatus() )
	{
		MessageOut( eCRITICAL_LOG, _T("No Whisper Target (%s) to (%s)"),
			pUser->GetCharName().c_str(), strNameRW.GetBUFFER() );

		MSG_CW_WHISPER_NAK msgNAK;
		msgNAK.byReason = MSG_CW_WHISPER_NAK::USER_NOT_FOUND;
		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(MSG_CW_WHISPER_NAK) );
		return;
	}

	// 대상이 나 자신인 경우 무시
	if( pUser == pTargetUser )
	{
		MessageOut( eFULL_LOG, _T("Whisper to MySelf (%s)"), pUser->GetCharName().c_str() );
		return;
	}

	//친구 차단이면 실패
	if( pTargetUser->FindBlock( pUser->GetCharGuid()) )
	{
		MSG_CW_WHISPER_NAK msgNAK;
		msgNAK.byReason = MSG_CW_WHISPER_NAK::USER_WHISPER_OFF;
		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(MSG_CW_WHISPER_NAK) );
		return;
	}

	if(pTargetUser->CanWhisper())
	{
		//GM 유저 맞으면 귓말은 정상적으로 보내지만, 유저에게는 귓말 보내지 못한다고 알려준다
		if( pTargetUser->GetGMGrade() >= eGM_GRADE_FIRST )
		{
			MessageOut(eCRITICAL_LOG, _T("Whisper to GM but Whisper Block (%s) to (%s)"),
				pUser->GetCharName().c_str(), strNameRW.GetBUFFER() );

			// 귓말 차단된 것처럼 알려 준다
			MSG_CW_WHISPER_NAK msgNAK;
			msgNAK.byReason = MSG_CW_WHISPER_NAK::USER_WHISPER_OFF;
			pUser->SendPacket( (BYTE*)&msgNAK, sizeof(MSG_CW_WHISPER_NAK) );

			// 대상에게 귓말 전송
			MSG_CW_WHISPER_ACK msgACK;

			util::cSTRING_REF_RW tmpWriter1( msgACK.szCharNameFrom, sizeof(msgACK.szCharNameFrom) );
			tmpWriter1.CopyFromString( pUser->GetCharName().c_str() );
			tmpWriter1.MakeSafeString();	// 쓰기용 버퍼이므로...

			util::cSTRING_REF_RW tmpWriter2( msgACK.szWhisperMsg, sizeof(msgACK.szWhisperMsg) );
			strMsgRW.CopyTo( tmpWriter2 );
			strMsgRW.MakeSafeString();		// 쓰기용 버퍼이므로...

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

	if( 1 )	// 대상에게 귓말 전송
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

	// 길이가 최대값을 넘기면 접속을 끊는다.
/*	if( pRecvMsg->byLen > MAX_NOTICE_LEN )
	{
		pUser->Disconnect();
		return;
	}

	// TODO: 공지 권한이 없으면 리턴한다.

	char szNotice[MAX_NOTICE_LEN + 1];
	ZeroMemory( szNotice, sizeof(szNotice) );
	strncpy( szNotice, pRecvMsg->szMsg, pRecvMsg->byLen );

	WorldServer::Instance()->SendNoticeMsg( szNotice );*/
}

// 귓말 켜고 끔(차단) 처리
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

	// 이 유저의 귓말 켬/끔 상태 세팅
	pUser->SetWhisper(pRecvMsg->bWhisper);

	// 성공했음을 알린다
	MSG_CW_WHISPER_SET_ACK	resMsg;

	resMsg.bWhisper = pRecvMsg->bWhisper;

	pUser->SendPacket( (BYTE*)&resMsg, sizeof(MSG_CW_WHISPER_SET_ACK) );

	MessageOut(eCRITICAL_LOG,  "(%s) Whisper Set(%d)", pUser->GetCharName().c_str(), pRecvMsg->bWhisper);
}

// 채팅 요청
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

	//채팅 금지라면
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

#ifndef _DEBUG
	// 부하 방지를 위해 1/10초 이하의 시간에 연달아 보낸 채팅은 무시
	if( !pUser->CheckChatTime() )
	{
		return;
	}
#endif

	// 너 어느 동네 소속이냐?
	Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
	if(!pChannel)
	{
		// 소속된 채널 없어도 오류
		MessageOut(eCRITICAL_LOG, "%s CW_CHAT_NORMAL_SYN Failed(Channel %u)", pUser->GetCharName().c_str(), pUser->GetChannelID());
		return;
	}

/*
	BaseZone *pZone = pChannel->FindZone(pUser->GetZoneKey());
	if(!pZone)
	{
		MessageOut( eCRITICAL_LOG, "%s CW_CHAT_NORMAL_SYN 실패[ZT:%u][ZK:%u]", pUser->GetCharName().c_str(), 
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

	//채팅 도배인지 체크
	//1. 같은 메세지면..
	if( strcmp( pRecvMsg->m_pszChatMsg, pUser->GetLastChatMsg() ) == 0 )	
	{
		//2. 시간이 5초를 지났는지..
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
		//메세지가 다르면.. 타이머와 마지막 메세지를 새로 세팅한다.
		pUser->SetLastChatMsg( pRecvMsg->m_pszChatMsg );
		pUser->SetChatTimer( 5 * 1000 );		//5초
	}


	// 채팅 메시지 패킷 조립
	MSG_CW_CHAT_NORMAL_BRD msgBRD;

	memset(msgBRD.m_ptszCharName, 0, sizeof(TCHAR) * MAX_CHARNAME_LENGTH);
	_tcsncpy(msgBRD.m_ptszCharName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );	// 같은 메시지이므로 그대로 복사
	msgBRD.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';

	if( 1 )
	{
		util::cSTRING_REF_RW strSendMsgRW( msgBRD.m_ptszChatMsg, sizeof(msgBRD.m_ptszChatMsg) );
		strSendMsgRW.CopyFrom( strMsgRD );
		strSendMsgRW.MakeSafeString();
		// 원 소스에 의하면 여기에 +1을 해준다...NULL포함?
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

	//채팅 금지라면
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

#ifndef _DEBUG
	// 부하 방지를 위해 1/10초 이하의 시간에 연달아 보낸 채팅은 무시
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

	// 너 어느 동네 소속이냐?
	Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
	if( !pChannel )
	{
		// 소속된 마을 없어도 오류
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

	// 채팅 메시지 패킷 조립
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

			if( stGameOption.m_byNormalShoutLevel == 0 )			//0이면 블럭상태이다.
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
			if( stGameOption.m_byWorldShoutLevel == 0 )			//0이면 블럭상태이다.
				return;

			if( pUser->GetCharLevel() < stGameOption.m_byWorldShoutLevel )
			{
				msgNAK.m_wErrorCode = RC::RC_WORLD_SHOUT_LIMIT_LEVEL;
				pUser->SendPacket((BYTE*)&msgNAK, sizeof(msgNAK));
				return;
			}
		#endif
			// 소모성 외치기 아이템이 있거나(기간제가 아니다.), PC방인데 PC방에서 외치기가 가능한 옵션이면.
			BOOL bNow = FALSE;

            // 기간제 외치기 아이템이 있거나, PC방인데 PC방에서 외치기가 가능한 옵션이면.
			if( bShoutItem || ( pUser->CanApplyPCBangBenefit() && stGameOption.m_byWorldShoutPCBANG ) ) 	
			{
				bNow = TRUE;
			}	
			else if( bShoutConsumeItem )	// 소모성 외치기 아이템이 있을시.
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

// GM 공지
Handler_CW_IMPL( CW_GM_NOTICE_SYN )
{
	MSG_CW_GM_NOTICE_SYN *pRecvMsg = (MSG_CW_GM_NOTICE_SYN*)pMsg;

	if(!pUser)
	{
		MessageOut(eCRITICAL_LOG,  "OnCW_GM_NOTICE_SYN Failed(pUser == NULL)");
		return;
	}

	// 모든 채팅에서 크기가 0이거나 실제 내용이 없으면 무시
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

	// GM인지 판정
	// 대상 유저가 GM인지 검사한다
	if( pUser->GetGMGrade() <= eGM_GRADE_BASE || pUser->GetGMGrade() >= eGM_GRADE_MAX )
	{
		// 이 경우는 크래킹이나 프로토콜 타입 버그일 가능성이 있다
		MessageOut(eCRITICAL_LOG, "Invalid User Request Notice(Not GM)!!!(%s)", pUser->GetCharName().c_str());
		return;
	}

	MSG_CW_NOTICE_BRD noticeMsg;
    // (WARNING) buffer overrun 위험성 존재하고 있다. 우선 위험 방지 처리 수행한다.
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

	// GM인지 확인되면 공지 종류 구별하여 날려준다
	switch(pRecvMsg->byNoticeType)
	{
	case MSG_CW_GM_NOTICE_SYN::NOTICE_WORLD:
		{
			// 월드 전체 공지-갖고 있는 모든 유저들에게 날린다
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
			// 해당 유저가 접속한 채널에만 날려준다
			Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
			if(!pChannel)
			{
				// 사용자의 채널 ID가 틀렸다. 치명적인 버그
				MessageOut( eCRITICAL_LOG, "Invalid Channel ID for User %s[%u]!! Channel Notice Failed!!", pUser->GetCharName().c_str(), pUser->GetChannelID() );
				return;
			}
			// 마을, 배틀존 모두 공지
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
			// 내가 접속한 지역에만 공지
			Channel *pChannel = ChannelManager::Instance()->GetChannel( pUser->GetChannelID() );
			if(!pChannel)
			{
				// 사용자의 채널 ID가 틀렸다. 치명적인 버그
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
				// 사용자의 Zone ID가 틀렸다. 치명적인 버그
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

// 스트링 파싱을 통한 GM 명령
Handler_CW_IMPL( CW_GM_STRING_CMD_SYN )
{
	MSG_CW_GM_STRING_CMD_SYN* pRecvMsg = (MSG_CW_GM_STRING_CMD_SYN*)pMsg;
	m_GMCmdManager.ParseCommand( pUser, pRecvMsg->m_szStringCmd );
}

Handler_CW_IMPL( CW_VIEWPORT_CHARSTATE )
{
	MSG_CW_VIEWPORT_CHARSTATE	*pRecvMsg = (MSG_CW_VIEWPORT_CHARSTATE*)pMsg;

	// 일단 해킹 방지용으로 현재 User State 확인
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

	// 새로 넣는 것인지 이동인지 확인
	if( pUser->GetSectorIndex() == -1 )
	{
		// 새로 넣는 것
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
		// 이동의 경우 기존 ChatZone에서 뺐다가 새로운 ChatZone으로 세팅하여 다시 넣어준다
		// 이놈은 플레이어가 있던 Sector의 ViewPortZone
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
		// 있던 ViewPortZone에서 유저 빼 주고, 새로운 ViewPortZone에 넣어준다(이 역할은 ChatZone이 한다)
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

// Armorkit에서 해킹 시도 발견
Handler_CW_IMPL( CW_ARMORKIT_FINDHACK_SYN )
{
}

// 친구 목록 요청
Handler_CW_IMPL( CW_FRIEND_NEWLIST_SYN )
{
	if( !pUser->CheckFriendList() )	// 이미 친구 목록을 받은 적이 있다면 해킹 시도 또는 잘못된 패킷
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

    //  MAX 치 까지 친구 추가 가능하나 추가시 친구 정보를 유저에게 주지 않는 버그 [9/11/2009 lst1024]
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

    //(lst1024)(090611)로그 잘못 남기던 버그
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

	// 친구 상태 전부 확인 끝났으면 보낸다
	pUser->SendPacket( (BYTE*)&sendFriend, sendFriend.GetSize() );
    //에이전트 서버에 친구추천 목록을 전달한다. (W->A->G), _KR_0_20091021_FIRENDINVITATION_EXTEND
    WFriendInvitationController::Instance()->SendInvitateFriendList(pUser, SEND_CLIENT);
	pUser->SendPacket( (BYTE*)&sendBlock, sendBlock.GetSize() );
	pUser->SendPacket( (BYTE*)&sendRevenge, sendRevenge.GetSize() );
	MessageOut( eFULL_LOG, "[%s][Guid:%u] Friend/Block/Revenge List Send", pUser->GetCharName().c_str(), pUser->GetGUID() );



/*
    // 친구 목록 요청
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

// 친구 요청
Handler_CW_IMPL( CW_FRIEND_ADD_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}

	MSG_CW_FRIEND_ADD_SYN	*pRecvMsg = (MSG_CW_FRIEND_ADD_SYN*)pMsg;

	// 기본적으로 이미 있는 친구인지, 아니면 나 자신인지 정도는 검사해 주는 쎈쓰!!!
	std::string strName = pUser->GetCharName();

	if( !_tcsnicmp( strName.c_str(), pRecvMsg->ptszCharNameTo, MAX_CHARNAME_LENGTH ) )
	{  
		// 나 자신이면 에러
		MSG_CW_FRIEND_ADD_NAK	sendMsg;
		sendMsg.m_byResult = MSG_CW_FRIEND_ADD_NAK::FRIEND_IS_ME;	// 내가 친구냐?!
		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_ADD_NAK) );
		MessageOut( eCRITICAL_LOG, "[Handler_CW::OnCW_FRIEND_ADD_SYN] [%s] Friend ADD Error - Friend Request to Myself", pRecvMsg->ptszCharNameTo );
		return;
	}

	// 이미 있는 친구라도 오류
#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING
	if( pUser->GetFriendMap()->FindFriend( pRecvMsg->ptszCharNameTo ) )
#else
	if( pUser->FindFriend( pRecvMsg->ptszCharNameTo ) )
#endif	
	{
		MSG_CW_FRIEND_ADD_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_ADD_NAK::ALREADY_FRIEND;	// 이미 친구다!

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_ADD_NAK) );
		MessageOut( eFULL_LOG, "[%s] Friend ADD Error - Already Friends", pRecvMsg->ptszCharNameTo );
		return;
	}

	// 친구 최대 개수는 90개
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

	// 일단 바~로 DB로 전송~
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

// 친구 삭제
Handler_CW_IMPL( CW_FRIEND_DEL_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}


	MSG_CW_FRIEND_DEL_SYN		*pRecvMsg = (MSG_CW_FRIEND_DEL_SYN*)pMsg;
	

	// 친구 맞아?
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
    // added data field to support _KR_0_20091021_FIRENDINVITATION_EXTEND //친구 삭제 
	sendMsg.m_Sts			= WFriendInvitationController::Instance()->GetFriendSts(pUser, pFriendInfo->dwFriendGuid);

	// DB로 요청~
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

// 차단 요청
Handler_CW_IMPL( CW_FRIEND_BLOCK_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}

	// 그냥 차단하면 끝~
	MSG_CW_FRIEND_BLOCK_SYN*	pRecvMsg = (MSG_CW_FRIEND_BLOCK_SYN*)pMsg;

	// 기본적인 오류 여부 검사
	if( _tcslen(pRecvMsg->ptszBlockName) > MAX_CHARNAME_LENGTH )
	{
		MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::ERR_DEFAULT;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Block Error - BlockName Overflow", pUser->GetCharName().c_str() );
		return;
	}

	// 혹시 너 자신을 차단하는 것은 아니겠지?
	if( !_tcsnicmp( pRecvMsg->ptszBlockName, pUser->GetCharName().c_str(), strlen(pUser->GetCharName().c_str()) ) )
	{
		//그러면 실패거든!!
		MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::BLOCKED_ME;	// 너 자신을 차단했다고!!!

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Block Error - Block Myself", pUser->GetCharName().c_str() );
		return;
	}

	// 이미 블록된 놈이면 블록 불가
	if( pUser->FindBlock( pRecvMsg->ptszBlockName ) )
	{
		MSG_CW_FRIEND_BLOCK_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_NAK::ALREADY_BLOCKED;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Block Error - Already Blocked[%s]", pUser->GetCharName().c_str(), pRecvMsg->ptszBlockName );
		return;
	}

	// 블록 최대 개수는 90개
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

	// DB로 요청~
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

// 차단 삭제 요청
Handler_CW_IMPL( CW_FRIEND_BLOCK_FREE_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}

	MSG_CW_FRIEND_BLOCK_FREE_SYN*	pRecvMsg = (MSG_CW_FRIEND_BLOCK_FREE_SYN*)pMsg;

	// 차단 중이긴 하냐?
	STRUCT_BLOCK_INFO* pBlockInfo = pUser->FindBlock( pRecvMsg->ptszBlockName );

	// 차단 정보가 없으면
	if( !pBlockInfo)
	{
		// 에러다
		MSG_CW_FRIEND_BLOCK_FREE_NAK	sendMsg;

		sendMsg.m_byResult = MSG_CW_FRIEND_BLOCK_FREE_NAK::NOT_BLOCKED;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_FRIEND_BLOCK_FREE_NAK) );
		return;
	}

	// 정확한 차단 정보가 있으면 차단 해제 요청을 DB에 보낸다
	MSG_DW_FRIEND_BLOCK_FREE_SYN	sendMsg;

	sendMsg.m_CharGuidFrom = pUser->GetCharGuid();
	sendMsg.m_CharGuidTo = pBlockInfo->dwBlockGuid;

	// DB로 요청~
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

// 친구 채팅
Handler_CW_IMPL( CW_FRIEND_CHAT_SYN )
{
	MSG_CW_FRIEND_CHAT_SYN*	pRecvMsg = (MSG_CW_FRIEND_CHAT_SYN*)pMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	util::cSTRING_REF strFriendNameRD( pRecvMsg->ptszFriendNameTo, sizeof(pRecvMsg->ptszFriendNameTo) );
	util::cSTRING_REF_RW_WITH_BUFFER<sizeof(CHAR)*(MAX_CHARNAME_LENGTH+1)> strFriendNameRW;
	strFriendNameRD.CopyTo( strFriendNameRW );
	strFriendNameRW.MakeSafeString();

	// 모든 채팅에서 크기가 0이거나 실제 내용이 없으면 무시
//	if( !pRecvMsg->byMsgLength || !_tcslen(pRecvMsg->ptszChatMsg) )
//	{
//		MessageOut( eFULL_LOG, "채팅 내용 없음" );
//		return;
//	}

	// 온라인인지 확인
	User *pFriend = UserManager::Instance()->FindUser( strFriendNameRW.GetBUFFER() );
    //__KR_001355_20090608_GROUPINGSYSTEM_CW
    STRUCT_GROUP_MEMBER_INFO* pMember = NULL;
    if (pFriend != NULL) {
        pMember = GroupManager::Instance()->FindGroupMember( pUser, pFriend->GetCharGuid() );
    }
    if( ( !pFriend || eZONETYPE_MAX == pFriend->GetStatus() ) && !pMember) //그룹 맴버간의 대화창을 위해
	{
		// 온라인 아이다!!
		MSG_CW_FRIEND_CHAT_NAK msgNAK;

		util::cSTRING_REF_RW strFriendNameToRW( msgNAK.ptszFriendNameTo, sizeof(msgNAK.ptszFriendNameTo) );
		strFriendNameRW.CopyTo( strFriendNameToRW );

		msgNAK.m_byResult = MSG_CW_FRIEND_CHAT_NAK::OFFLINE_FRIEND;

		pUser->SendPacket( (BYTE*)&msgNAK, sizeof(msgNAK) );
		MessageOut( eCRITICAL_LOG, _T("Friend Chat Error[%s][CharGuid:%u] - Friend Offline[%s]"),
			pUser->GetCharName().c_str(), pUser->GetCharGuid(), strFriendNameRW.GetBUFFER() );

		return;
	}

	// 온라인이면 친구냐?
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
			// 상대에게도 친구 아이다!!
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

	// 자 이제 보내자
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

	// 친구 채팅은 상대방에게만 보내준다. 보낸 놈은 그냥 지가 알고 있으니 알아서 출력해줘!
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

// 1:1 대화 거부 체크
Handler_CW_IMPL( CW_FRIEND_CHATBLOCK_SYN )
{
	// 해당 유저의 1:1대화를 거부한다
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

	// 모든  크기가 0이거나 실제 내용이 없으면 무시
//	if( !pRecvMsg->m_byMsgLen || !_tcslen(pRecvMsg->m_ptszCharMsg) )
//	{
//		MessageOut( eFULL_LOG, "채팅 내용 없음" );
//		return;
//	}

	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

	//채팅 금지라면
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

	if( pUser->GetGuildGuid() == 0 )	// 길드 소속이 아니라고 나온다면?
	{
		// 길드 정보를 요청해야 한다
		resMsg.byResult = MSG_CW_GUILD_CHAT_NAK::ERR_WAIT_INFO;	// 잠시 후에 다시 시도하십시오

		pUser->SendPacket( (BYTE*)&resMsg, sizeof(MSG_CW_GUILD_CHAT_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Guild Chat Error - No Guild %u", pUser->GetCharName().c_str(), pUser->GetGuildGuid() );

		// 길드 멤버 복구 코드는 전부 접속 부분으로 옮겨갔다
/*
		// 길드 멤버 복구 요청
		MSG_WZ_GUILD_SELECT_SYN ToGuildMsg;
		ToGuildMsg.m_dwKey = pUser->GetGUID();
		ToGuildMsg.m_GuildGuid = pRecvMsg->m_GuildGuid;
		ToGuildMsg.m_CharGuid = pUser->GetCharGuid();
		ServerSession *pGuildServer = ServerSessionManager::Instance()->GetGuildServer();

		if( !pGuildServer )
		{
			MessageOut( eCRITICAL_LOG, "GUILD_MEMBER_SYN 길드서버 없어 복구 요청 불가!" );
			return;
		}
		pGuildServer->Send( (BYTE*)&ToGuildMsg, sizeof(MSG_WZ_GUILD_SELECT_SYN) );

		MessageOut( eFULL_LOG, "GUILD_CHAT_SYN 길드정보[%u] 없어서 요청[%s]", pRecvMsg->m_GuildGuid, pUser->GetCharName().c_str() );
*/
	}
	// 일단 받은 정보가 있다면 100% 이 정보를 신뢰한다. 따라서 RecvMsg와 비교할 필요 없다
/*
	else if( pUser->GetGuildGuid() != pRecvMsg->m_GuildGuid )
	{
		// 이건 그냥 오류다
		resMsg.byResult = MSG_CW_GUILD_CHAT_NAK::ERR_NOT_GUILD;

		pUser->SendPacket( (BYTE*)&resMsg, sizeof(MSG_CW_GUILD_CHAT_NAK) );
		MessageOut( eCRITICAL_LOG, "[%s] Guild Chat Error - No Guild %u", pUser->GetCharName().c_str(), pUser->GetGuildGuid() );
	}
*/
	else
	{
		// 소속 길드 찾는다
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

		// 길드원 전체에게 메시지 전송 - 이거 UserManager에서 처리해야 되지 않을라나 싶네?
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

// 파티 채팅 처리
Handler_CW_IMPL( CW_CHAT_PARTY_SYN )
{
	MSG_CW_CHAT_PARTY_SYN* pRecvMsg = (MSG_CW_CHAT_PARTY_SYN*)pMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	// 모든 채팅에서 크기가 0이거나 실제 내용이 없으면 무시
//	if( !pRecvMsg->m_byMsgLength || !_tcslen(pRecvMsg->m_ptszChatMsg) )
//	{
//		MessageOut( eFULL_LOG, "채팅 내용 없음" );
//		return;
//	}

	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

	//채팅 금지라면
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
	// 전송할 파티 있으면 메시지 전송
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

// 클라이언트 암호화 세팅 완료
Handler_CW_IMPL( CW_CRYPTOGRAPHY_ACK )
{
	if( WorldServerInfoParser::Instance()->IsCryptable() )
	{
		// 클라이언트한테 이제부터 그 키 사용하라고 알려주고 서버도 세팅한다. 이 세팅이 끝난 다음 보내고 받는 것은 무조건 암호화된 내용들이다
		MSG_CW_CRYPTOGRAPHY_CMD	sendMsg;

		pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_CRYPTOGRAPHY_CMD) );

		pUser->SetCrypt();	// 이제부터 암호화 쓴다!

		MessageOut( eFULL_LOG, "[GUID:%u] Crypt Set", pUser->GetGUID() );
		return;
	}

	MessageOut( eCRITICAL_LOG, "[GUID:%u] Request Crypt Set but Not Crypt Server", pUser->GetGUID() );
}

// 클라이언트 암호화 세팅 실패
Handler_CW_IMPL( CW_CRYPTOGRAPHY_NAK )
{
	// 잘라버려야 하나?
	MessageOut( eCRITICAL_LOG, "[GUID:%u] Crypt Set Failed!!!", pUser->GetGUID() );

	pUser->Logout();
}

// 음성 채팅 요청
Handler_CW_IMPL( CW_CHAT_VOICE_REQUEST_SYN )
{
	// 요청한 상대 검색하여 음성 채팅 요청
	MSG_CW_CHAT_VOICE_REQUEST_SYN* pRecvMsg = (MSG_CW_CHAT_VOICE_REQUEST_SYN*)pMsg;

	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	//채팅 금지라면
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

	// 상대가 음성채팅 불가 상태이거나 이미 다른 사람에게 요청을 먼저 받았을 경우도 있으므로 여기에 대처한다
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

// 음성 채팅 수락
Handler_CW_IMPL( CW_CHAT_VOICE_ACCEPT_SYN )
{
	MSG_CW_CHAT_VOICE_ACCEPT_SYN* pRecvMsg = (MSG_CW_CHAT_VOICE_ACCEPT_SYN*)pMsg;

	// 요청한 아이디가 있는지 확인하여 수락 프로토콜 전송
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

// 음성 채팅 거절
Handler_CW_IMPL( CW_CHAT_VOICE_REJECT_SYN )
{
	// 요청했던 아이디 있는지 확인하여 거절 프로토콜 전송
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

	// 채팅 중인지 아닌지는 판단 불가. 별도 변수를 둘지 말지는 월드서버가 음성 채팅에서 어느 정도까지 역할을 하는가에 달려 있다.

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

// 음성 채팅 on/off 요청
Handler_CW_IMPL( CW_CHAT_VOICE_MIC_STATUS_SYN )
{
	// 음성 채팅 on/off 여부를 클라에서 받아서 해당 유저의 파티원들에게 전달한다
	MSG_CW_CHAT_VOICE_MIC_STATUS_SYN*	pRecvMsg = (MSG_CW_CHAT_VOICE_MIC_STATUS_SYN*)pMsg;
	MSG_CW_CHAT_VOICE_MIC_STATUS_BRD	sendMsg;

	_tcsncpy( sendMsg.m_ptszCharName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );

	// 전달만 하면 된다...에서 나아가서 저장도 해야 된다.(2006/05/16)
	pUser->SetVoiceChat( pRecvMsg->bOn );
	sendMsg.bOn = pRecvMsg->bOn;

	pUser->SendParty( (BYTE*)&sendMsg, sizeof(sendMsg) );

	MessageOut( eFULL_LOG, "[GUID:%u][C:%u] Request Voice Chat On/Off [bOn:%u]", pUser->GetGUID(), pUser->GetChannelID(), pRecvMsg->bOn );
}

// 파티원들의 음성 채팅 상태 요청
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

// 플레이어들의 음성채팅 상태 요청(CW_CHAT_PARTY_MIC_STATUS_REQ가 실패할 경우 클라이언트가 전송
Handler_CW_IMPL( CW_CHAT_PLAYER_MIC_STATUS_REQ )
{
	MSG_CW_CHAT_PLAYER_MIC_STATUS_REQ* pRecvMsg = (MSG_CW_CHAT_PLAYER_MIC_STATUS_REQ*)pMsg;
	User *pTarget = NULL;
	MSG_CW_CHAT_PLAYER_MIC_STATUS_ANS	sendMsg;

	sendMsg.m_byMemberNum = 0;

	// 제한 인원 이상으로 오면 오류 - 치명적 버그이거나 크래킹 시도로 간주해야 한다
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

// 다른 사람의 계정ID 요청
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

// 채널채팅방 입장 요청
Handler_CW_IMPL( CW_CHAT_CHANNEL_JOIN_SYN )
{
	MSG_CW_CHAT_CHANNEL_JOIN_SYN* pRecvMsg = (MSG_CW_CHAT_CHANNEL_JOIN_SYN*)pMsg;
	MSG_CW_CHAT_CHANNEL_JOIN_NAK nakMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	// 패킷 오류 체크
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

// 채널채팅 보냄
Handler_CW_IMPL( CW_CHAT_CHANNEL_SEND_SYN )
{
	MSG_CW_CHAT_CHANNEL_SEND_SYN*	pRecvMsg = (MSG_CW_CHAT_CHANNEL_SEND_SYN*)pMsg;
	MSG_CW_CHAT_CHANNEL_SEND_NAK	nakMsg;
	MSG_CW_CHAT_CHANNEL_SEND_BRD	brdMsg;


	if(!pUser || pUser->GetStatus() == eZONETYPE_MAX)	return;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	//채팅 금지라면
	if(pUser->IsChatBlocked())
	{
		pUser->SendChatBlock();	
		return;
	}

	// 패킷 오류 검사
	if( pRecvMsg->m_byChatLength > MAX_CHAT_LEN )
	{
		MessageOut( eCRITICAL_LOG, "CHAT_CHANNEL_SEND Error - ChatLength Overflow![CharGuid:%u]", pUser->GetCharGuid() );

		nakMsg.m_byErrorCode = RC::RC_CHAT_UNKNOWN;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );
		return;
	}

	// 현재 사용중인 채팅방 있는지 검사
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

// 채널채팅방 초대
Handler_CW_IMPL( CW_CHAT_CHANNEL_INVITE_SYN )
{
	MSG_CW_CHAT_CHANNEL_INVITE_SYN* pRecvMsg = (MSG_CW_CHAT_CHANNEL_INVITE_SYN*)pMsg;
	MSG_CW_CHAT_CHANNEL_INVITE_NAK	nakMsg;
	MSG_CW_CHAT_CHANNEL_INVITE_ACK	ackMsg;
	MSG_CW_CHAT_CHANNEL_INVITE_CMD	cmdMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	// 패킷 오류 방지
	TCHAR	ptszTargetName[MAX_CHARNAME_LENGTH+1] = {0,};

	_tcsncpy( ptszTargetName, pRecvMsg->m_ptszInviteName, MAX_CHARNAME_LENGTH );

	// 해당 유저 있는지 검사
	User *pTarget = UserManager::Instance()->FindUser( ptszTargetName );

	if( !pTarget || pTarget == pUser )
	{
		// 유저 없다~
		nakMsg.m_byErrorCode = RC::RC_CHAT_NOCHAR;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "CHAT_CHANNEL_INVITE Error - No Target User[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), ptszTargetName );
		return;
	}

    //_NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL
    // 초대한 대상이 이미 채널 채팅 중인지 검사한다.
    if (pTarget->GetChannelChatRoom())
    {
        nakMsg.m_byErrorCode = RC::RC_CHAT_ALREADYINROOM;
        // 초대 대상자를 설정한다.
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
               _T("|%s|초대한 대상이 이미 채널 채팅 중입니다.|신청자 = %s, 대상자 = %s|"), 
               __FUNCTION__, 
               (pUser->GetCharName()).c_str(),
               (pTarget->GetCharName()).c_str());
        return;
    }

	// 어떤 방에 초대하는가?
	CommChatZone *pChatRoom = pUser->GetChannelChatRoom();

	if( !pChatRoom )
	{
		nakMsg.m_byErrorCode = RC::RC_CHAT_NOROOM;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "CHAT_CHANNEL_INVITE Error - No ChatRoom[CharGuid:%u]", pUser->GetCharGuid() );
		return;
	}

#if WAVERIX_CODE_RENEWAL
    { // 초대하는 놈 이름
        BOOST_STATIC_ASSERT(sizeof(cmdMsg.m_ptszInviteName) ==
                            (_countof(cmdMsg.m_ptszInviteName) * sizeof(char)));
        strncpy(cmdMsg.m_ptszInviteName, pUser->GetCharName().c_str(), _countof(cmdMsg.m_ptszInviteName));
        cmdMsg.m_ptszInviteName[_countof(cmdMsg.m_ptszInviteName) - 1] = '\0';
    };
    { // 방제
        BOOST_STATIC_ASSERT(sizeof(cmdMsg.m_ptszRoomName) ==
                            (_countof(cmdMsg.m_ptszRoomName) * sizeof(char)) &&
                            _countof(cmdMsg.m_ptszRoomName) <= UCHAR_MAX);
        strncpy(cmdMsg.m_ptszRoomName, pChatRoom->GetChatRoomName(), _countof(cmdMsg.m_ptszRoomName));
        cmdMsg.m_ptszRoomName[_countof(cmdMsg.m_ptszRoomName) - 1] = '\0';
        cmdMsg.m_byRoomNameLength = (BYTE)strlen(cmdMsg.m_ptszRoomName); //assert
    };
#else
	// 초대하는놈 이름
	_tcsncpy( cmdMsg.m_ptszInviteName, pUser->GetCharName().c_str(), MAX_CHARNAME_LENGTH );
	cmdMsg.m_ptszInviteName[MAX_CHARNAME_LENGTH] = '\0';

	// 방제
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
// 초대 대상자가 채널 초대를 거부하거나 차단했을 때 호출된다.
Handler_CW_IMPL(CW_CHAT_CHANNEL_INVITE_NAK)
{
    __UNUSED(wSize);

    if (!::CheckChatBlockZone(pUser))
    {
        return;
    }
    
    // 초대 거부 요청이 왔는지 검사한다.
    MSG_CW_CHAT_CHANNEL_INVITE_NAK* const recv_invite_nak = 
        static_cast<MSG_CW_CHAT_CHANNEL_INVITE_NAK*>(pMsg);
    if (recv_invite_nak->m_byErrorCode != RC::RC_CHAT_DENY_INVITATION && 
        recv_invite_nak->m_byErrorCode != RC::RC_CHANNEL_CHAT_BLOCKED)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|채널 초대 거부나 차단 상태가 아닙니다.|요청자 = %s, 요청코드 = %d|"), 
               __FUNCTION__, 
               (pUser->GetCharName()).c_str(),
               recv_invite_nak->m_byErrorCode);
        return;
    }
    
    // 초대 신청자를 검사한다.
    User* const invite_request_user = 
        UserManager::Instance()->FindUser(recv_invite_nak->invite_target_name_);
    if (invite_request_user == NULL || invite_request_user == pUser)
    {
        // 초대 신청자를 찾을 수 없다면 응답 대상자가 존재하지 않으므로 여기서 끝낸다.
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|초대 신청자를 찾을 수 없습니다.|요청자 = %s, 신청자 = %s|"), 
               __FUNCTION__, 
               (pUser->GetCharName()).c_str(),
               recv_invite_nak->invite_target_name_);
        return;
    }

    // 초대 대상자를 설정한다.
    {
        BOOST_STATIC_ASSERT(sizeof(recv_invite_nak->invite_target_name_) == 
            (_countof(recv_invite_nak->invite_target_name_) * sizeof(TCHAR)));
        ::_tcsncpy(
            recv_invite_nak->invite_target_name_, 
            (pUser->GetCharName()).c_str(), 
            _countof(recv_invite_nak->invite_target_name_));
        recv_invite_nak->invite_target_name_[_countof(recv_invite_nak->invite_target_name_)-1] = _T('\0');
    }
    
    // 초대 신청자에게 응답 메시지를 중계한다.
    invite_request_user->SendPacket((BYTE*)recv_invite_nak, sizeof(*recv_invite_nak));

    SUNLOG(eFULL_LOG, 
           _T("|%s|초대 신청자에게 초대 거부나 차단 상태 알림 메시지를 보냈습니다.|신청자 = %s, 대상자 = %s|"), 
           __FUNCTION__, 
           (invite_request_user->GetCharName()).c_str(), 
           (pUser->GetCharName()).c_str());
}

// 채널채팅방 퇴장 요청
Handler_CW_IMPL( CW_CHAT_CHANNEL_OUT_SYN )
{
	MSG_CW_CHAT_CHANNEL_OUT_SYN* pRecvMsg = (MSG_CW_CHAT_CHANNEL_OUT_SYN*)pMsg;

    if(CheckChatBlockZone(pUser) == 0)
        return;

	ChatRoomManager::Instance()->RemoveUser( pUser );

	MessageOut( eFULL_LOG, "CHAT_CHANNEL_OUT[CharGuid:%u]", pUser->GetCharGuid() );
}

// 채널채팅방 정보 요청
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

// 친구/리벤지/길드원/그룹원 상세정보 요청
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
	// 혹시 있을 수 있는 문자열 오류 보정
    // NOTE: fixed, prevent changes about cg network buffer stream
    char char_name[MAX_CHARNAME_LENGTH + 1];
    strncpy(char_name, pRecvMsg->m_ptszCharName, _countof(char_name));
    char_name[_countof(char_name) - 1] = '\0';

	User* pTarget = UserManager::Instance()->FindUser(char_name);

	if( !pTarget )
	{
		nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTCONNECTED;	// 해당 유저 미접속
		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_REQ Error - Not Connected Target[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name);
		return;
	}

#ifdef _NA_0_20091104_FIRENDMAP_REFACTORYING		
	STRUCT_FRIEND_INFO *pFriendInfo = pUser->GetFriendMap()->FindFriend( pTarget->GetCharGuid() );
#else
	STRUCT_FRIEND_INFO *pFriendInfo = pUser->FindFriend( pTarget->GetCharGuid() );
#endif
	
	if( (!pFriendInfo || pFriendInfo->dwFriendGuid != pTarget->GetCharGuid()) &&        // 친구 관계가 아니고
        (!pUser->GetGuildGuid() || pUser->GetGuildGuid() != pTarget->GetGuildGuid()) )  // 같은 길드원도 아니면
	{
		// 혹시나 리벤지 대상인지 한번 더 확인
		STRUCT_REVENGE_INFO *pRevengeInfo = pUser->FindRevenge( pTarget->GetCharGuid() );

		if( !pRevengeInfo || pRevengeInfo->dwRevengeCharGuid != pTarget->GetCharGuid() )
		{
			nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTFRIEND;	// 친구가 아니거나 잘못된 정보임
			pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

			MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_REQ Error - Not Freind or Incorrect Name[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name);
			return;
		}
	}

	// 친구 정보 수집
	MSG_CW_FRIEND_STATUS_ANS	ansMsg;

	ansMsg.m_byChannel = (BYTE)pTarget->GetChannelID();
	ansMsg.m_byZoneType = pTarget->GetStatus();
	ansMsg.m_dwZoneKey = pTarget->GetZoneKey();
	ansMsg.m_MapCode = pTarget->GetMapCode();
    strncpy(ansMsg.m_ptszCharName, char_name, _countof(ansMsg.m_ptszCharName));
    ansMsg.m_ptszCharName[_countof(ansMsg.m_ptszCharName) - 1] = '\0';
    ZeroMemory(ansMsg.m_tszGuildName, sizeof(ansMsg.m_tszGuildName));

	// 길드 가입돼 있으면 길드 관련 정보도 전송
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
    case RC::RC_FRIEND_NOTGUILDMEMBER:          // 한번 이상 접속 하지 않은 오프라인 길드원
        {                                       // 여기까지 왔다면, 상세 정보 창을 열었다는 뜻이며, 길드서버에 선 체크를 한 상황이므로 통과 한다.
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

// 친구 따라가기
Handler_CW_IMPL( CW_FRIEND_STATUS_JOIN_SYN ) 
{
	MSG_CW_FRIEND_STATUS_JOIN_SYN*	pRecvMsg = (MSG_CW_FRIEND_STATUS_JOIN_SYN*)pMsg;
	MSG_CW_FRIEND_STATUS_JOIN_NAK	nakMsg;

	// 혹시 있을 수 있는 문자열 오류 보정
    // NOTE: fixed, prevent changes about cg network buffer stream
    char char_name[MAX_CHARNAME_LENGTH + 1];
    strncpy(char_name, pRecvMsg->m_ptszCharName, _countof(char_name));
    char_name[_countof(char_name) - 1] = '\0';

	User* pTarget = UserManager::Instance()->FindUser(char_name);

    if( !pTarget)
    {
        nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTCONNECTED;	// 해당 유저 미접속
        pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

        MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_JOIN_SYN Error - Not Connected Target[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name );
        return;
    }
    // __KR_001355_20090608_GROUPINGSYSTEM_CW
    STRUCT_GROUP_MEMBER_INFO* pMember = GroupManager::Instance()->FindGroupMember( pUser, pTarget->GetCharGuid() );

    //BUG:5045 문상현
    //밑에서 그룹 비교하는 부분이 있으므로 주석처리 한다.
    //if( !pMember)
    //{
    //    nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTCONNECTED;	// 해당 유저 미접속
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
		// 혹시나 리벤지 대상인지 한번 더 확인
		STRUCT_REVENGE_INFO *pRevengeInfo = pUser->FindRevenge( pTarget->GetCharGuid() );

		if( !pRevengeInfo || pRevengeInfo->dwRevengeCharGuid != pTarget->GetCharGuid() )
		{
			memcpy( nakMsg.m_ptszCharName, char_name, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
			nakMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
			nakMsg.m_byErrorCode = RC::RC_FRIEND_NOTFRIEND;	// 친구가 아니거나 잘못된 정보임
			pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

			MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_JOIN_SYN Error - Not Freind or Incorrect Name[CharGuid:%u][Target:%s]", pUser->GetCharGuid(), char_name );
			return;
		}
	}

	// 채널이 다르면
	if( pTarget->GetChannelID() != pUser->GetChannelID() )
	{
		memcpy( nakMsg.m_ptszCharName, char_name, sizeof(TCHAR)*MAX_CHARNAME_LENGTH );
		nakMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';
		nakMsg.m_byErrorCode = RC::RC_FRIEND_DIFFCHANNEL;	// 채널 달라요~
		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eFULL_LOG, "CW_FRIEND_STATUS_JOIN_SYN Error - Diff Channel[CharGuid:%u][%u][Target:%s][%u]", pUser->GetCharGuid(), 
																											pUser->GetChannelID(), 
																											char_name, 
																											pTarget->GetChannelID() );
		return;
	}

	// 방 타입이 잘못되었으면
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

	// 현재 배틀존 정보 넣는다
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

// 리벤지 리스트에서 삭제
Handler_CW_IMPL( CW_REVENGE_DEL_SYN )
{

    if(CheckChatBlockZone(pUser) == 0)
        return;

	if( pUser->IsFriendTransaction() )
	{
		return;
	}

	MSG_CW_REVENGE_DEL_SYN		*pRecvMsg = (MSG_CW_REVENGE_DEL_SYN*)pMsg;

	// 혹시 있을지 모르는 문자열 길이 오류 보정
    // NOTE: fixed, prevent changes about cg network buffer stream
    char char_name[MAX_CHARNAME_LENGTH + 1];
    strncpy(char_name, pRecvMsg->m_ptszCharName, _countof(char_name));
    char_name[_countof(char_name) - 1] = '\0';

	// 나쁜친구 맞아?
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

	// DB로 요청~
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


// 쪽지 관련 프로토콜 처리
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

	//20070131_메모 특수문자 파싱하고 다음문자 붙여주기
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
	// DB로 요청~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_MEMO_SEND_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// 그냥 실패

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

	// 시간 체크하고 바로 전송
	if( !pUser->CheckMemoList() )	// 이미 길드 멤버 받았으면 응답 없다
	{
		MSG_CW_MEMO_LIST_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_WAIT_INFO;

		pUser->SendPacket( (BYTE*)&nakMsg, sizeof(nakMsg) );

		MessageOut( eMIDDLE_LOG, "[%s][CharGuid:%u]Memo List Req Error - MemoList Time Checked", pUser->GetCharName().c_str(), pUser->GetCharGuid() );
		return;
	}

	// DB로 요청~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_MEMO_LIST_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// 그냥 실패

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

	// DB로 요청~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_MEMO_VIEW_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// 그냥 실패

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

	// DB로 요청~
	ServerSession *pGameDBProxy = ServerSessionManager::Instance()->GetGameDBProxy();
	if( !pGameDBProxy )
	{
		MSG_CW_MEMO_DELETE_NAK	nakMsg;

		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// 그냥 실패

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

	// 권한 체크로 인해 길드서버를 통해야 함
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
	
	// 요청 실패
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
		nakMsg.m_byErrorCode = RC::RC_MEMO_FAILED;	// 그냥 실패
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
//그룹맺기 시스템 핸들러 함수
//////////////////////////////////////////////////////////////////////////
//클라이언트로 부터 그룹 맴버 추가 요청
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
        //-- 상대 유저가 게임에 접속해 있지 않다.
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

    //-- 서로의 정보를 저장한다.
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

//그룹 초대 받은 대상이 수락을 하였다.
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
        send_msg.m_toCharGUID = pUser->GetCharGuid();    //중요한 개념 : 
        send_msg.m_GroupSTATFirst = GROUP_LEVEL_LOW;     //DB상 destChar는 Char의 하위 레벨이다.
        send_msg.m_GroupSTATSecond = GROUP_LEVEL_HIGH;   //DB상 destChar는 Char의 상위 레벨이다.
    }
    ServerSession *game_db_proxy = ServerSessionManager::Instance()->GetGameDBProxy();
    if (game_db_proxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG,"["__FUNCTION__"]Not found gamedbproxy");
        return;
    }
    game_db_proxy->Send((BYTE*)&send_msg, sizeof(send_msg));
}

//그룹 초대 받은 대상이 여러 가지 이유로 거절하였다.
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


//클라이언트로 부터 그룹 맴버 리스트 요청받음
Handler_CW_IMPL( CW_GROUP_GROUPLIST_REQ )
{
    //MSG_CW_GROUP_GROUPLIST_REQ 패킷 내용이 없다.
    if (CheckChatBlockZone(pUser) == 0)
        return;

    GroupManager::Instance()->SendMemberList(pUser);
}


//그룹 맴버 상세 정보 요청
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

    //찾는 유저가 존재하지않을 경우
    if( !pTarget )
    {
        MSG_CW_GROUP_STATUS_NAK sendMsg;

        sendMsg.dwResult = MSG_CW_GROUP_STATUS_NAK::NO_MEMBER_DATA;//유저 미접속
        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_GROUP_STATUS_NAK) );

        SUNLOG( eCRITICAL_LOG, "["__FUNCTION__"]Not Connected Target[Guid:%u][Target:%s]", 
            pUser->GetCharGuid(), pRecvMsg->m_ptszCharName );
        return;
    }

    STRUCT_GROUP_MEMBER_INFO* pMemberInfo = 
        GroupManager::Instance()->FindGroupMember( pUser, pTarget->GetCharGuid() );

    //그룹 맴버가 존재 하지 않을때
    if( !pMemberInfo ) 
    {
        MSG_CW_GROUP_STATUS_NAK sendMsg;

        sendMsg.dwResult = MSG_CW_GROUP_STATUS_NAK::NOT_MEMBER;//그룹 맴버가 아니다.
        pUser->SendPacket( (BYTE*)&sendMsg, sizeof(MSG_CW_GROUP_STATUS_NAK) );

        SUNLOG( eFULL_LOG, "["__FUNCTION__"]Can't Found Member [Guid:%u][Target:%s]", 
            pUser->GetCharGuid(), pRecvMsg->m_ptszCharName );
        return;
    }
    
    //찾기 결과 값이 정확하지 않을 경우
    if( pMemberInfo->charGUID != pTarget->GetCharGuid() )
    {
        MSG_CW_GROUP_STATUS_NAK sendMsg;

        sendMsg.dwResult = MSG_CW_GROUP_STATUS_NAK::NOT_MEMBER;//그룹 맴버가 아니다.
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
    memset( ansMsg.m_tszGuildName, 0, sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1) );//요청한 케릭터 이름
    _tcsncpy( ansMsg.m_ptszCharName,  pRecvMsg->m_ptszCharName, MAX_CHARNAME_LENGTH );
    ansMsg.m_ptszCharName[MAX_CHARNAME_LENGTH] = '\0';

    // 길드 가입돼 있으면 길드 관련 정보도 전송
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


    //그룹원이 맞는지 확인
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

//유저가 지역 점령전 쪽지를 확인하였다. DB에 해당 내용이 있는지 확인해본다.
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

// 구현 전용 지역 함수
namespace {
    
// 결과에 따라 로그를 기록한다.
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

// 유저를 검사한다.
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

// 파티 모집 등록/갱신이 요청되면 호출된다.
Handler_CW_IMPL(CW_PARTY_FINDING_REGISTER_SYN)
{
    __UNUSED(wSize);

    // 요청자를 검사한다.
    if (!scoped_check_request_user(pUser))
    {
        return;
    }
        
    const MSG_CW_PARTY_FINDING_REGISTER_SYN* const register_syn = 
        static_cast<const MSG_CW_PARTY_FINDING_REGISTER_SYN* const>(pMsg);

    User* const party_master = pUser;

    // 응답 메시지를 생성한다.
    MSG_CW_PARTY_FINDING_RESULT_ACK result_ack;
    result_ack.response_type_ = PartyFindingResponseTypeConst::GetDefaultValue();
    
    // 요청 받은 정보로 파티 모집 정보를 만들어서
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

    // 등록하거나 이미 등록되어 있으면 갱신한다.
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

    // 결과를 요청자에게 보낸다.
    party_master->SendPacket(reinterpret_cast<BYTE*>(&result_ack), result_ack.GetSize());

    // 로그를 기록한다.
    scoped_write_log(party_master, result_ack.result_, __FUNCTION__);
}

// 파티 모집 목록이 요청되면 호출된다.
Handler_CW_IMPL(CW_PARTY_FINDING_GET_LIST_SYN)
{
    __UNUSED(pMsg);
    __UNUSED(wSize);

    // 요청자를 검사한다.
    if (!scoped_check_request_user(pUser))
    {
        return;
    }

    User* const request_user = pUser;
    
    // 파티 모집 목록을 얻는다.
    MSG_CW_PARTY_FINDING_GET_LIST_ACK get_list_ack;
    PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
    get_list_ack.get_list_result_ = party_finding_manager.GetList(get_list_ack.party_finding_list_, 
                                                                  get_list_ack.party_finding_list_size_);

    // 결과를 요청자에게 보낸다.
    request_user->SendPacket(reinterpret_cast<BYTE*>(&get_list_ack), get_list_ack.GetSize());

    // 로그를 기록한다.
    scoped_write_log(request_user, get_list_ack.get_list_result_, __FUNCTION__);
}

// 파티 모집 참가 신청이 요청되면 호출된다.
Handler_CW_IMPL(CW_PARTY_FINDING_JOIN_REQUEST_SYN)
{
    __UNUSED(wSize);

    // 요청자를 검사한다.
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
        // 참가하려는 파티 모집 정보를 찾을 수 있는지 검사한다.
        const PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
        const PartyFindingInfo* const join_wanted_party_finding = 
            party_finding_manager.FindPartyFinding(join_request_syn->join_wanted_party_finding_key_);
        if (join_wanted_party_finding == NULL)
        {
            // == throw
            join_request_result = RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY_FINDING;
            break;
        }
        
        // 연결된 파티 정보가 있다면
        if (join_wanted_party_finding->linked_party_key_ != PartyFindingInfo::NULL_PARTY)
        {
            // 파티 정보를 찾을 수 있는지 검사한다.
            const PartyInfo* const join_wanted_party = 
                party_finding_manager.FindParty(join_wanted_party_finding->linked_party_key_);
            if (join_wanted_party != NULL)
            {
                // == throw
                join_request_result = RC::PARTY_FINDING_FAIL_CANT_FIND_PARTY;
                break;
            }

            // 파티 모집 인원을 검사한다.
            if (join_wanted_party->num_of_joined_member_ >= join_wanted_party_finding->num_of_max_joinable_member_)
            {
                // == throw
                join_request_result = RC::PARTY_FINDING_FAIL_PARTY_IS_FULL;
                break;
            }
        }

        // 파티장을 찾을 수 있는지 검사한다.
        User* const party_master = 
            UserManager::Instance()->FindUser(join_wanted_party_finding->party_master_name_);
        if (party_master == NULL)
        {
            // == throw
            join_request_result = RC::PARTY_FINDING_FAIL_CANT_FIND_MASTER;
            break;
        }

        // 요청자와 파티장의 채널이 같은지 검사한다.
        if (join_wanted_user->GetChannelID() != party_master->GetChannelID())
        {
            // == throw
            join_request_result = RC::PARTY_FINDING_FAIL_CHANNEL_IS_DIFFERENT;
            break;
        }

        // 참가 신청에 대한 응답 요청 메시지를 만들어서 파티장에게 보낸다.
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
        
        // 로그를 기록한다.
        scoped_write_log(join_wanted_user, join_request_result, __FUNCTION__);

        return;

    } while (false); // == catch
    {
        // 참가 신청 실패 메시지를 만들어서 신청자에게 보낸다.
        MSG_CW_PARTY_FINDING_RESULT_ACK result_ack;
        result_ack.response_type_ = PartyFindingResponseTypeConst::kJoin;
        result_ack.result_ = join_request_result;
        result_ack.party_finding_key_ = join_request_syn->join_wanted_party_finding_key_;
        join_wanted_user->SendPacket(reinterpret_cast<BYTE*>(&result_ack), result_ack.GetSize());

        // 로그를 기록한다.
        scoped_write_log(join_wanted_user, result_ack.result_, __FUNCTION__);
    }
}

// 파티장으로부터 파티 모집 참가 신청에 대한 응답이 도착하면 호출된다.
Handler_CW_IMPL(CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK)
{
    __UNUSED(wSize);

    // 요청자를 검사한다.
    if (!scoped_check_request_user(pUser))
    {
        return;
    }
    
    const MSG_CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK* const join_response_result_ack = 
        static_cast<const MSG_CW_PARTY_FINDING_JOIN_RESPONSE_RESULT_ACK* const>(pMsg);

    User* const party_master = pUser;

    // 참가 신청자를 검사한다.
    const CHARGUID join_wanted_user_id = join_response_result_ack->join_wanted_user_id_;
    User* const join_wanted_user = UserManager::Instance()->FindUserByCharGuid(join_wanted_user_id);
    if (join_wanted_user == NULL)
    {
        return;
    }

    PartyFindingResultValueType join_response_result = join_response_result_ack->join_response_result_;
    
    // 파티장이 파티 참가를 승락했다면
    typedef RC::PartyFindingResultUtil ResultUtil;
    if (join_response_result == RC::PARTY_FINDING_SUCCESS)
    {
        // 파티장이 속한 채널을 검사한다.
        ServerSession* const agent_server = 
            ServerSessionManager::Instance()->FindAgentServer(party_master->GetChannelID());
        if (agent_server != NULL)
        {
            // 파티장이 속한 파티 모집을 검사한다.
            const PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
            const PartyFindingInfo* const join_wanted_party_finding = 
                party_finding_manager.FindPartyFinding(
                    join_response_result_ack->join_wanted_party_finding_key_);
            if (join_wanted_party_finding != NULL)
            {
                // 파티 참가 요청 메시지를 만들어서 에이전트 서버에 보낸다.
                MSG_AW_PARTY_FINDING_JOIN_SYN aw_join_syn;
                aw_join_syn.join_wanted_party_finding_key_ = join_response_result_ack->join_wanted_party_finding_key_;
                aw_join_syn.join_wanted_party_master_id_ = party_master->GetCharGuid();
                aw_join_syn.join_wanted_user_id_ = join_response_result_ack->join_wanted_user_id_;
                aw_join_syn.num_of_max_joinable_member_ = join_wanted_party_finding->num_of_max_joinable_member_;
                agent_server->Send(reinterpret_cast<BYTE*>(&aw_join_syn), aw_join_syn.GetSize());

                // 로그를 기록한다.
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
    
    // 참가 거절 메시지를 만들어서 신청자에게 보낸다.
    MSG_CW_PARTY_FINDING_RESULT_ACK result_ack;
    result_ack.response_type_ = PartyFindingResponseTypeConst::kJoin;
    result_ack.result_ = join_response_result;
    result_ack.party_finding_key_ = join_response_result_ack->join_wanted_party_finding_key_;
    join_wanted_user->SendPacket(reinterpret_cast<BYTE*>(&result_ack), result_ack.GetSize());

    // 로그를 기록한다.
    scoped_write_log(party_master, result_ack.result_, __FUNCTION__);    
}

// 파티 모집 취소가 요청되면 호출된다.
Handler_CW_IMPL(CW_PARTY_FINDING_CANCEL_SYN)
{
    __UNUSED(wSize);

    // 요청자를 검사한다.
    if (!scoped_check_request_user(pUser))
    {
        return;
    }

    const MSG_CW_PARTY_FINDING_CANCEL_SYN* const cancel_syn = 
        static_cast<const MSG_CW_PARTY_FINDING_CANCEL_SYN* const>(pMsg);

    User* const party_master = pUser;

    // 응답 메시지를 생성한다.
    MSG_CW_PARTY_FINDING_RESULT_ACK result_ack;
    result_ack.response_type_ = PartyFindingResponseTypeConst::kCancel;
    result_ack.party_finding_key_ = cancel_syn->cancel_wanted_party_finding_key_;

    // 파티 모집을 취소한다.
    PartyFindingManager& party_finding_manager = PartyFindingManager::Instance();
    result_ack.result_ = 
        party_finding_manager.Cancel(party_master, cancel_syn->cancel_wanted_party_finding_key_);

    // 취소 결과를 요청자에게 보낸다.
    party_master->SendPacket(reinterpret_cast<BYTE*>(&result_ack), result_ack.GetSize());

    // 로그를 기록한다.
    scoped_write_log(party_master, result_ack.result_, __FUNCTION__);
}

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
