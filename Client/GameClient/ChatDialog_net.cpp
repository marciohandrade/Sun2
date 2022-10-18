#include "SunClientPrecompiledHeader.h"
#include "ChatDialog.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "ItemManager.h"


//----------------------------------------------------------------------------
/**
*/
BOOL 
ChatDialog::SendChatMsg( TCHAR * pszMessage )
{
    if (CheckSlangChatMessage(pszMessage) == false)
    {
        return FALSE;
    }

#ifdef 	_000541_BLOCK_CHAT_REPEAT
	if( isRepeatChatting( pszMessage ) )
	{
		return FALSE;
	}
	pendingSendChatInfomation(pszMessage);
#endif

	BOOL bRet = this->sendChatMsgToWorld(pszMessage);
	if (!bRet)
	{
		// 채팅 메시지를 보낼 수 없는 상태입니다.
		g_InterfaceManager.GetInterfaceString( eST_CHAT_CANNOT_SEND_CHAT, this->m_szbuf, INTERFACE_STRING_LENGTH );
		AddString( this->m_szbuf );
	}
	return bRet;
}

BOOL ChatDialog::SendChatPartyMsg( TCHAR * pszMessage )
{
    if (CheckSlangChatMessage(pszMessage) == false)
    {
        return FALSE;
    }
	
	MSG_CW_CHAT_PARTY_SYN SendPacket;
	SendPacket.m_byCategory = CW_CHAT;
	SendPacket.m_byProtocol = CW_CHAT_PARTY_SYN;

	memset(SendPacket.m_ptszChatMsg , 0 , sizeof(SendPacket.m_ptszChatMsg) );
	StrnCopy(SendPacket.m_ptszChatMsg, pszMessage, MSG_CW_CHAT_PARTY_SYN::_MAX_CHATMSG_SIZE);
	SendPacket.m_byMsgLength = strlen(SendPacket.m_ptszChatMsg);

	BOOL bRet = GlobalFunc::SendPacket( CI_CHATSERVERIDX, &SendPacket, SendPacket.GetSize());
	if (!bRet)
	{
		// 채팅 메시지를 보낼 수 없는 상태입니다. 
		g_InterfaceManager.GetInterfaceString( eST_CHAT_CANNOT_SEND_CHAT, this->m_szbuf, INTERFACE_STRING_LENGTH );
		AddString( this->m_szbuf );
	}

	return bRet;
}

BOOL ChatDialog::SendChatChannelMsg ( TCHAR * pszMessage)
{
    if (CheckSlangChatMessage(pszMessage) == false)
    {
        return FALSE;
    }

	MSG_CW_CHAT_CHANNEL_SEND_SYN SendPacket;
	SendPacket.m_byCategory = CW_CHAT;
	SendPacket.m_byProtocol = CW_CHAT_CHANNEL_SEND_SYN;
	memset( SendPacket.m_ptszChat , 0 , sizeof(SendPacket.m_ptszChat) );
	StrnCopy(SendPacket.m_ptszChat, pszMessage, MAX_CHAT_LEN );
	SendPacket.m_byChatLength = strlen(SendPacket.m_ptszChat);

	BOOL bRet = GlobalFunc::SendPacket( CI_CHATSERVERIDX, &SendPacket, SendPacket.GetSize());
	if (!bRet)
	{
		// 채팅 메시지를 보낼 수 없는 상태입니다. 
		g_InterfaceManager.GetInterfaceString( eST_CHAT_CANNOT_SEND_CHAT, this->m_szbuf, INTERFACE_STRING_LENGTH );
		AddString( this->m_szbuf );
	}

	return bRet;
}

void ChatDialog::SendChannelNak()
{
    MSG_CW_CHAT_CHANNEL_INVITE_NAK channel_invite_nak_packet;

    if (CGameOption::Instance()->GetChannelAccept() != FALSE)
    {
        channel_invite_nak_packet.m_byErrorCode = RC_CHANNEL_CHAT_BLOCKED;
    }
    else
    {
        channel_invite_nak_packet.m_byErrorCode = RC_CHAT_DENY_INVITATION;
    }
    Snprintf(channel_invite_nak_packet.invite_target_name_, MAX_CHARNAME_LENGTH, channel_waitting_name());
    GlobalFunc::SendPacket(CI_CHATSERVERIDX, &channel_invite_nak_packet,
        sizeof(MSG_CW_CHAT_CHANNEL_INVITE_NAK));
}

VOID ChatDialog::SendChatChannelCreate( TCHAR * pszMessage)
{
	if( ! m_bExistChannel)
	{
		int nLength = _tcslen(pszMessage);
		if( nLength <= 0)
			return;
		if( nLength <= MAX_ROOMTITLE_LENGTH) 
		{
			MSG_CW_CHAT_CHANNEL_JOIN_SYN  Packet;
			
			memset( Packet.m_ptszRoomName , 0 , sizeof( Packet.m_ptszRoomName ) );
			memset( m_ChannelName , 0 , sizeof( m_ChannelName ) );
			StrnCopy(Packet.m_ptszRoomName,pszMessage,MAX_ROOMTITLE_LENGTH);
			Packet.m_byRoomNameLength = strlen( Packet.m_ptszRoomName);
			StrnCopy(m_ChannelName , pszMessage , MAX_ROOMTITLE_LENGTH);
			Packet.m_ptszRoomName[MAX_ROOMTITLE_LENGTH] = 0;
			m_ChannelName[MAX_ROOMTITLE_LENGTH] = 0;
			GlobalFunc::SendPacket(CI_CHATSERVERIDX,&Packet,Packet.GetSize());
		}
		else
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
			//1723	채널 이름이 너무 깁니다.
			g_InterfaceManager.GetInterfaceString(1723 , szMessage ,INTERFACE_STRING_LENGTH);
			AddString(szMessage);
		}
	}
	else
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH];
		// 채널에 이미 참여 중입니다.
		g_InterfaceManager.GetInterfaceString(eST_CHANNEL_ALREADY_JOIN , szMessage , INTERFACE_STRING_LENGTH );
		AddString( szMessage );
	}
}

VOID ChatDialog::SendChatChannelInvite( TCHAR * pszMessage)
{
	MSG_CW_CHAT_CHANNEL_INVITE_SYN Packet;
	int iLength = _tcslen(pszMessage);
	memset( Packet.m_ptszInviteName , 0 , sizeof( Packet.m_ptszInviteName ) );
	memset( m_PendingChannelInvitedName , 0 , sizeof ( Packet.m_ptszInviteName ) );
	StrnCopy( Packet.m_ptszInviteName , pszMessage , MAX_CHARNAME_LENGTH );
	StrnCopy( m_PendingChannelInvitedName , pszMessage , MAX_CHARNAME_LENGTH );
	Packet.m_ptszInviteName[MAX_CHARNAME_LENGTH] = 0;
	m_PendingChannelInvitedName[MAX_CHARNAME_LENGTH] = 0;
	GlobalFunc::SendPacket(CI_CHATSERVERIDX , &Packet , sizeof( Packet ) ); 
}

void ChatDialog::ExitChannelChat()
{
	TCHAR szString[INTERFACE_STRING_LENGTH] = {0,}, szMessage[INTERFACE_STRING_LENGTH] = { 0,} ;
	m_bExistChannel = FALSE;
	m_ChannelJoinMember_Num = 0;

	// 1714	[%s]채널에서 나갔습니다.
	g_InterfaceManager.GetInterfaceString(1714, szString , INTERFACE_STRING_LENGTH);
	Snprintf( szMessage , INTERFACE_STRING_LENGTH-1, szString , m_ChannelName );
	AddString( szMessage , CHAT_TYPE_SYSTEM );

	// 283	채널채팅
	g_InterfaceManager.GetInterfaceString(283, szMessage ,INTERFACE_STRING_LENGTH);
	StrnCopy(m_ChannelName , szMessage , MAX_ROOMTITLE_LENGTH );
	m_ChannelName[MAX_ROOMTITLE_LENGTH] = _T('\0');
	GlobalFunc::SetCaption( m_pCheckFilter[eCHECK_CHANNEL] , m_ChannelName);
}


//----------------------------------------------------------------------------
/**
*/
BOOL			
ChatDialog::SendGuildMsg( TCHAR * pszMessage, GUILDGUID guid )
{
    if (CheckSlangChatMessage(pszMessage) == false)
    {
        return FALSE;
    }

	MSG_CW_GUILD_CHAT_SYN SendPacket;
	SendPacket.m_byCategory = CW_GUILD;
	SendPacket.m_byProtocol = CW_GUILD_CHAT_SYN;
	SendPacket.m_GuildGuid	= guid;

	memset( SendPacket.m_ptszCharMsg , 0 , sizeof( SendPacket.m_ptszCharMsg) );
	StrnCopy(SendPacket.m_ptszCharMsg, pszMessage, MSG_CW_GUILD_CHAT_SYN::_MAX_CHATMSG_SIZE);
	SendPacket.m_byMsgLen	= strlen(SendPacket.m_ptszCharMsg);

	BOOL bRet = GlobalFunc::SendPacket( CI_CHATSERVERIDX, &SendPacket, SendPacket.GetSize());
	if (!bRet)
	{
		// 채팅 메시지를 보낼 수 없는 상태입니다. 
		g_InterfaceManager.GetInterfaceString( eST_CHAT_CANNOT_SEND_CHAT, this->m_szbuf, INTERFACE_STRING_LENGTH );
		AddString( this->m_szbuf );
	}


	return bRet;
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
ChatDialog::SendShoutMsg( TCHAR * pszMessage  , byte byShoutType)
{
    if (CheckSlangChatMessage(pszMessage) == false)
    {
        return FALSE;
    }

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	MSG_CW_CHAT_SHOUT_SYN SendPacket;
	memset(SendPacket.m_pszChatMsg , 0 , sizeof(SendPacket.m_pszChatMsg) );
	StrnCopy(SendPacket.m_pszChatMsg, pszMessage, MSG_CW_CHAT_SHOUT_SYN::_MAX_CHATMSG_SIZE);
	SendPacket.m_byMsgLength = strlen(SendPacket.m_pszChatMsg);

	SendPacket.m_byCategory = CW_CHAT;
	SendPacket.m_byProtocol = CW_CHAT_SHOUT_SYN;
	SendPacket.m_byMessageType = byShoutType;
	BOOL bRet = GlobalFunc::SendPacket(CI_CHATSERVERIDX, &SendPacket, SendPacket.GetSize());

#if defined( _ZONE_TEST_LOBBY_IN_SINGLE )
	MSG_CW_CHAT_SHOUT_BRD	SendPacketAck;

	SendPacketAck.m_byMsgLength = _tcslen(pszMessage);
	StrCopy( SendPacketAck.m_pszChatMsg, pszMessage );

	if (g_pHero) 
		StrCopy( SendPacketAck.m_szCharName, xGetHeroData()->GetName() );

	g_pApplication->AddVirtualPacket(CI_CHATSERVERIDX, (BYTE *)&SendPacketAck, sizeof(SendPacketAck));
#endif

	if (!bRet)
	{
		// 외치기를 보낼 수 없는 상태입니다.
		g_InterfaceManager.GetInterfaceString( eST_CHAT_CANNOT_SEND_SHOUT, this->m_szbuf, INTERFACE_STRING_LENGTH );
		AddString( this->m_szbuf );
	}

	return bRet;
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
ChatDialog::SendWhisperMsg( TCHAR * pszMessage, TCHAR * pszToChar)
{
	assert(pszToChar);
	assert(pszMessage);

    if (CheckSlangChatMessage(pszMessage) == false)
    {
        return FALSE;
    }

	if ( g_pHero && pszToChar )
	{
		if ( !Strcmp( g_pHero->GetName(), pszToChar ) )
		{
			//	자기 자신에게는 메시지를 보낼 수 없습니다.
			g_InterfaceManager.GetInterfaceString( eST_CHAT_CANNOT_SEND_WHISPER_YOURSELF, this->m_szbuf, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, this->m_szbuf);
			return TRUE;
		}
	}

	memset(m_WhisperMsg.szCharNameTo, 0, sizeof(m_WhisperMsg.szCharNameTo)); 
	memset(m_WhisperMsg.szWhisperMsg, 0, sizeof(m_WhisperMsg.szWhisperMsg)); 
	StrnCopy(m_WhisperMsg.szCharNameTo, pszToChar, MAX_CHARNAME_LENGTH);
	StrnCopy(m_WhisperMsg.szWhisperMsg, pszMessage, MAX_CHAT_LEN);
	m_WhisperMsg.byMsgLen = strlen(m_WhisperMsg.szWhisperMsg);

	BOOL bRet = GlobalFunc::SendPacket(CI_CHATSERVERIDX, &m_WhisperMsg, m_WhisperMsg.GetSize());

	if (bRet)
	{
		if ( g_pHero )
		{
			memset(m_szTempPendingWhisperSender, 0, sizeof(m_szTempPendingWhisperSender) );
			StrnCopy(m_szTempPendingWhisperSender, pszToChar,MAX_CHARNAME_LENGTH);
		}
	}
	else
	{
		// 귓말을 보낼 수 없는 상태입니다.
		g_InterfaceManager.GetInterfaceString( eST_CHAT_CANNOT_SEND_WHISPER, this->m_szbuf, INTERFACE_STRING_LENGTH );
		AddString(m_szbuf);
	}

	return bRet;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
ChatDialog::SendNoticeMsg( TCHAR * pszMessage )
{
	assert(pszMessage);

    if (CheckSlangChatMessage(pszMessage) == false)
    {
        return FALSE;
    }

	memset( m_NoticeMsg.szMsg, 0, sizeof(m_NoticeMsg.szMsg) );
	StrnCopy(m_NoticeMsg.szMsg, pszMessage, MAX_NOTICE_LEN);
	m_NoticeMsg.byLen	= strlen(m_NoticeMsg.szMsg);

	return GlobalFunc::SendPacket(CI_CHATSERVERIDX, &m_NoticeMsg, m_NoticeMsg.GetSize());
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
ChatDialog::SendGMNoticeMsg( TCHAR * pszMessage, BYTE byNoticeType )
{
	assert(pszMessage);

    if (CheckSlangChatMessage(pszMessage) == false)
    {
        return FALSE;
    }

	M_GM_NoticeMsg.byNoticeType = byNoticeType;
	memset( M_GM_NoticeMsg.szNotice, 0, sizeof(M_GM_NoticeMsg.szNotice) );
	StrnCopy(M_GM_NoticeMsg.szNotice, pszMessage, MSG_CW_GM_NOTICE_SYN::_MAX_CHATMSG_SIZE);
	M_GM_NoticeMsg.dwNoticeLength = strlen(M_GM_NoticeMsg.szNotice);
	return GlobalFunc::SendPacket(CI_CHATSERVERIDX, &M_GM_NoticeMsg, M_GM_NoticeMsg.GetSize());
}

BOOL			
ChatDialog::sendChatMsgToWorld(TCHAR * pszMessage)
{
	// 무조건 월드로
	m_C2W_FieldChatMsg.m_byCategory = CW_CHAT;
	m_C2W_FieldChatMsg.m_byProtocol = CW_CHAT_NORMAL_SYN;

	memset( m_C2W_FieldChatMsg.m_pszChatMsg , 0 , sizeof(m_C2W_FieldChatMsg.m_pszChatMsg) );
	StrnCopy( m_C2W_FieldChatMsg.m_pszChatMsg, pszMessage, MSG_CW_CHAT_NORMAL_SYN::_MAX_CHATMSG_SIZE );
	m_C2W_FieldChatMsg.m_byMsgLength = strlen(m_C2W_FieldChatMsg.m_pszChatMsg);
	BOOL bRet = GlobalFunc::SendPacket(CI_CHATSERVERIDX, &m_C2W_FieldChatMsg, m_C2W_FieldChatMsg.GetSize());

	return bRet;
}

BOOL			
ChatDialog::sendChatMsgToField(TCHAR * pszMessage)
{
	if(!g_pHero)
		return FALSE;

	m_FieldChatMsg.m_byCategory	 = CG_CONVERSATION;
	m_FieldChatMsg.m_byProtocol	 = CG_CONVERSATION_FIELDCHAT_SYN;
	m_FieldChatMsg.m_dwObjectKey = g_pHero->GetObjectKey();

	memset(m_FieldChatMsg.m_pszChatMsg, 0, sizeof(m_FieldChatMsg.m_pszChatMsg));
	StrnCopy( m_FieldChatMsg.m_pszChatMsg, pszMessage, MSG_CG_CONVERSATION_FIELDCHAT_SYN::_MAX_CHATMSG_SIZE );
	m_FieldChatMsg.m_byMsgLength = strlen(m_FieldChatMsg.m_pszChatMsg);
	g_pHero->AddChatMessage(pszMessage , m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_GENERAL]);
	AddString(pszMessage, CHAT_TYPE_GENERAL, g_pHero->GetObjectKey());

	BOOL bRet = GlobalFunc::SendPacket(CI_GAMESERVERIDX, &m_FieldChatMsg, m_FieldChatMsg.GetSize());
	return bRet;
}

void ChatDialog::SendChatChannelInfoREQ()
{
	if( m_bExistChannel && m_ChannelJoinMember_Num > 1)
	{
		MSG_CW_CHAT_CHANNEL_INFO_REQ SendPacket;
		GlobalFunc::SendPacket(CI_CHATSERVERIDX , &SendPacket , sizeof(SendPacket) );
	}
	else
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = { 0,};
		if( m_bExistChannel == FALSE )
		{
			g_InterfaceManager.GetInterfaceString(eST_CHANNEL_NOT_CREATE , szMessage ,INTERFACE_STRING_LENGTH);
			AddString(szMessage ,CHAT_TYPE_SYSTEM);
			return;
		}

		BOOL bFoundChannelJoinMember = m_bExistChannel && m_ChannelJoinMember_Num == 1;
		if( bFoundChannelJoinMember )
		{
			//1722	현재 채널에 참여한 사람이 없습니다.
			g_InterfaceManager.GetInterfaceString(1722, szMessage,INTERFACE_STRING_LENGTH);
			AddString(szMessage ,CHAT_TYPE_SYSTEM);
			return;
		}
	}
}