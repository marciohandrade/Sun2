#include "SunClientPrecompiledHeader.h"
#include "ChatDialog.h"
#include "InterfaceManager.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "ObjectManager.h"
#include "uiChatMan/uiChatMan_def.h"
#include "uiNoticeMan/uiNoticeMan.h"
#include "uiNoticeDialog/uiNoticeDialog.h"
#include "uiCommunityMan/uiCommunityMan.h"  
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
#include "uiTiredSystemMan/uiTiredSystemMan.h"
#include "TiredSystemDialog.h"
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
void _callback_ChannelChatInvite(bool bYes)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={ 0,} , szResult[INTERFACE_STRING_LENGTH] = { 0,};
	ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
	if ( pDlg)
	{
		if(bYes)
		{
			pDlg->SendChatChannelCreate(pDlg->GetInvitedRoomName());	//test �ּ�
		}
		else
		{
            pDlg->SendChannelNak();
		}
	}
    pDlg->SetChannelWaitting(false);
}

//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::NetworkProc( MSG_BASE * pMsg )
{

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0 ,};

	switch (pMsg->m_byCategory)
	{
	case CW_GUILD:
		{
			this->net_proc_CW_GUILD_CHAT( pMsg );
		}
		break;

	case CW_CHAT:
		{
			switch(pMsg->m_byProtocol)
			{
			case CW_CHAT_NORMAL_BRD:
			case CW_CHAT_NORMAL_NAK:
				net_proc_CW_CHAT_NORMAL(pMsg);
				break;

			case CW_CHAT_PARTY_BRD:
				net_proc_CW_PARTY_CHAT(pMsg);
				break;

			case CW_WHISPER_ACK:
			case CW_WHISPER_NAK:
				{
					net_proc_CW_WHISPER(pMsg);
				}
				break;

			case CW_NOTICE_BRD:
				{
					net_proc_CW_NOTICE(pMsg);
				}
				break;

			case CW_CHAT_SHOUT_BRD:
			case CW_CHAT_SHOUT_NAK:
				{
					net_proc_CW_CHAT_SHOUT(pMsg);
				}
				break;			

			case CW_WHISPER_SET_ACK:
				{
					net_proc_CW_WHISPER_SET_ACK(pMsg);
				}
				break;

			case CW_CHAT_CHANNEL_JOIN_ACK:
			case CW_CHAT_CHANNEL_JOIN_BRD:
			case CW_CHAT_CHANNEL_JOIN_NAK:
			case CW_CHAT_CHANNEL_OUT_ACK:
			case CW_CHAT_CHANNEL_OUT_BRD:
			case CW_CHAT_CHANNEL_OUT_NAK:
			case CW_CHAT_CHANNEL_INVITE_ACK:
			case CW_CHAT_CHANNEL_INVITE_NAK:
			case CW_CHAT_CHANNEL_INVITE_CMD:
			case CW_CHAT_CHANNEL_SEND_BRD:
			case CW_CHAT_CHANNEL_SEND_NAK:
			case CW_CHAT_CHANNEL_INFO_NAK:
			case CW_CHAT_CHANNEL_INFO_ANS:
				{
					net_proc_CW_CHAT_CHANNEL( pMsg );
				}		
				break;
			}
		}
		break;

	case CG_CONVERSATION:
		{
			switch( pMsg->m_byProtocol )
			{
			case CG_CONVERSATION_FIELDCHAT_BRD:
				{
					net_proc_CG_CONVERSATION_FIELDCHAT_BRD(pMsg);
				}
				break;

			default:
				assert(!"���ǵ��� ���� ä�� �޽���");
				break;
			}
		}
		break;
#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	case CG_EVENT:
		{
			switch ( pMsg->m_byProtocol )
			{
			case CG_EVENT_PEAKTIME_NOTIFY_BRD:
				{
					net_proc_CG_EVENT_PEAKTIME_BRD(pMsg);
				}
				break;
			}
		}
		break;
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
	}

}

VOID ChatDialog::net_proc_CW_CHAT_CHANNEL(MSG_BASE * pMsg)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = { 0,};
	switch (pMsg->m_byProtocol)
	{
	case CW_CHAT_CHANNEL_SEND_BRD:
		{
			MSG_CW_CHAT_CHANNEL_SEND_BRD * pRecvPacket = (MSG_CW_CHAT_CHANNEL_SEND_BRD *)pMsg;
			TCHAR	szCharName[INTERFACE_STRING_LENGTH] = {0,}, szChatMsg[INTERFACE_STRING_LENGTH] = {0,};            
			StrnCopy( szCharName , pRecvPacket->m_ptszCharName , MAX_CHARNAME_LENGTH);
			StrnCopy( szChatMsg , pRecvPacket->m_ptszChat , MAX_CHAT_LENGTH);
			AddString(szChatMsg, CHAT_TYPE_CHANNEL, szCharName );
		}
		break;


	case CW_CHAT_CHANNEL_SEND_NAK:
		{
			MSG_CW_CHAT_CHANNEL_SEND_NAK * pRecvPacket = (MSG_CW_CHAT_CHANNEL_SEND_NAK *)pMsg;
			OnErrorCode( pRecvPacket->m_byErrorCode );		
		}
		break;

	case CW_CHAT_CHANNEL_JOIN_ACK:
		{
			TCHAR szChannelCreate[INTERFACE_STRING_LENGTH] ={ 0,};
			m_bExistChannel = TRUE;
			MSG_CW_CHAT_CHANNEL_JOIN_ACK * pRecvPacket = (MSG_CW_CHAT_CHANNEL_JOIN_ACK *) pMsg;
			m_ChannelJoinMember_Num = pRecvPacket->m_byUserNum;
			if( pRecvPacket->m_byUserNum == 1)
			{
				// 1701	[%s] ä���� �����߽��ϴ�.
				g_InterfaceManager.GetInterfaceString(eST_CREATE_CHANNEL_CHAT , szChannelCreate , INTERFACE_STRING_LENGTH );
				Snprintf(szMessage ,INTERFACE_STRING_LENGTH -1, szChannelCreate , this->m_ChannelName );
			}
			else
			{
				//1702	[%s] ä�ο� �����߽��ϴ�.
				g_InterfaceManager.GetInterfaceString(eST_JOIN_CHANNEL_CHAT , szChannelCreate , INTERFACE_STRING_LENGTH);
				Snprintf(szMessage ,INTERFACE_STRING_LENGTH-1, szChannelCreate , this->m_ChannelName  );
			}
			AddString( szMessage, CHAT_TYPE_SYSTEM );
			TCHAR szChannelName[INTERFACE_STRING_LENGTH] = {0,};
			StrnCopy(szChannelName, m_ChannelName, 10);
			GlobalFunc::SetCaption(m_pCheckFilter[eCHECK_CHANNEL], szChannelName);
		}
		break;
	case CW_CHAT_CHANNEL_JOIN_BRD:
		{
			MSG_CW_CHAT_CHANNEL_JOIN_BRD * pRecvPacket = ( MSG_CW_CHAT_CHANNEL_JOIN_BRD * ) pMsg;

			TCHAR szSubMessage[INTERFACE_STRING_LENGTH] = {0,};
			//1720	[%s]���� [%s]ä�ο� �����ϼ̽��ϴ�.
			g_InterfaceManager.GetInterfaceString(1720, szSubMessage , INTERFACE_STRING_LENGTH);
			m_ChannelJoinMember_Num = pRecvPacket->m_byUserNum;
			Snprintf(szMessage,INTERFACE_STRING_LENGTH -1, szSubMessage, pRecvPacket->m_ptszJoinMember , m_ChannelName);
			AddString( szMessage, CHAT_TYPE_SYSTEM );
		}
		break;
	case CW_CHAT_CHANNEL_JOIN_NAK:
		{
			MSG_CW_CHAT_CHANNEL_JOIN_NAK * pRecvPacket = ( MSG_CW_CHAT_CHANNEL_JOIN_NAK * ) pMsg;
			OnErrorCode( pRecvPacket->m_byErrorCode );
		}
		break;


	case CW_CHAT_CHANNEL_OUT_ACK:
		{
			ExitChannelChat();
		}
		break;
	case CW_CHAT_CHANNEL_OUT_BRD:
		{
			MSG_CW_CHAT_CHANNEL_OUT_BRD * pRecvPacket = ( MSG_CW_CHAT_CHANNEL_OUT_BRD * ) pMsg;
			m_ChannelJoinMember_Num = pRecvPacket->m_byUserNum;	
			TCHAR szTemp[INTERFACE_STRING_LENGTH]= { 0,};
			// [%s]���� [%s]ä�ο��� �������ϴ�. 1717
			g_InterfaceManager.GetInterfaceString(eST_CHANNEL_LEAVE_MSG , szTemp ,INTERFACE_STRING_LENGTH );
			Snprintf(szMessage, INTERFACE_STRING_LENGTH -1 , szTemp  ,
				pRecvPacket->m_ptszOutName ,
				m_ChannelName );
			AddString(szMessage, CHAT_TYPE_SYSTEM );

		}
		break;
	case CW_CHAT_CHANNEL_OUT_NAK:
		{
			MSG_CW_CHAT_CHANNEL_OUT_NAK * pRecvPacket = ( MSG_CW_CHAT_CHANNEL_OUT_NAK * ) pMsg;
			OnErrorCode( pRecvPacket->m_byErrorCode );
		}
		break;

	case CW_CHAT_CHANNEL_INVITE_ACK:
		{
			TCHAR szTemp[INTERFACE_STRING_LENGTH ] = { 0,};
			// [%s]���� ä�ο� �ʴ��߽��ϴ�
			g_InterfaceManager.GetInterfaceString( eST_CHANNLE_INVITE_MSG , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf( szMessage , INTERFACE_STRING_LENGTH -1 , szTemp , m_PendingChannelInvitedName , MAX_CHARNAME_LENGTH +1 );
			AddString(szMessage);
		}
		break;
	case CW_CHAT_CHANNEL_INVITE_NAK:
		{
			MSG_CW_CHAT_CHANNEL_INVITE_NAK * pRecvPacket = ( MSG_CW_CHAT_CHANNEL_INVITE_NAK * ) pMsg;
			OnErrorCode( pRecvPacket->m_byErrorCode );
		}
		break;
	case CW_CHAT_CHANNEL_INVITE_CMD:
		{
			MSG_CW_CHAT_CHANNEL_INVITE_CMD * pRecvPacket =  ( MSG_CW_CHAT_CHANNEL_INVITE_CMD * ) pMsg;
			TCHAR szTemp[INTERFACE_STRING_LENGTH] = { 0,};
			// [%s]���� [%s]ä�ο� �ʴ��߽��ϴ�. 1719
			g_InterfaceManager.GetInterfaceString(eST_CHANNEL_INVITED_BY_ANOTHER , szTemp , INTERFACE_STRING_LENGTH );
			Snprintf(szMessage ,  INTERFACE_STRING_LENGTH -1,
				szTemp ,
				pRecvPacket->m_ptszInviteName,
				pRecvPacket->m_ptszRoomName);
			memset( m_ChannelInvitedRoomName , 0 , sizeof( m_ChannelInvitedRoomName ) );
			StrnCopy(m_ChannelInvitedRoomName , pRecvPacket->m_ptszRoomName , MAX_ROOMTITLE_LENGTH );

            if (CGameOption::Instance()->GetChannelAccept() != FALSE)
            {   // ä�� ������ �ź� �Ǿ� ������ ������ ����
                _callback_ChannelChatInvite(false);
            }
            else if (is_channel_join_waitting_ == false)
            {
                is_channel_join_waitting_ = true;
                // ä�� �ʴ��� �̸� ����
                strncpy(channel_waitting_name_, pRecvPacket->m_ptszInviteName, _countof(channel_waitting_name_));
                channel_waitting_name_[_countof(channel_waitting_name_) - 1] = '\0';
			    uiSystemMan *pSystemMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			    if (pSystemMan)
                {
				    pSystemMan->Sys_Confirm(szMessage, _callback_ChannelChatInvite);
                }
            }
            else
            {
                if (strcmp(channel_waitting_name_, pRecvPacket->m_ptszInviteName) != 0)  
                {   // ���� ä�� �ʴ��ڿ� �̸��� ���� ���� ��� ����
                    _callback_ChannelChatInvite(false);
                }// ������ �̹� �˸�â�� �� �����Ƿ� �� �ٿ�.
            }
		}
		break;

	case CW_CHAT_CHANNEL_INFO_ANS:
		{	
			MSG_CW_CHAT_CHANNEL_INFO_ANS * pRecvPacket = (MSG_CW_CHAT_CHANNEL_INFO_ANS * ) pMsg;
			
			for( int i =0 ; i < pRecvPacket->m_byUserNum ; ++i )
			{
                if ( !(g_pHero) || !(pRecvPacket->m_ptszCharName[i]) )
                    continue;
				if( StrnCmp( g_pHero->GetName() , pRecvPacket->m_ptszCharName[i] ,  MAX_CHARNAME_LENGTH) == 0 )
					continue;
				pRecvPacket->m_ptszCharName[i][MAX_CHARNAME_LENGTH] = _T('\0');
				TCHAR szCharName[MAX_CHARNAME_LENGTH] = {0,}; 
				StrnCopy( szCharName , pRecvPacket->m_ptszCharName[i] , MAX_CHARNAME_LENGTH);
				AddString(szCharName);
			}
			//1721	���� ���� ä�ο� ���� ���Դϴ�.
			g_InterfaceManager.GetInterfaceString(1721, szMessage , INTERFACE_STRING_LENGTH);
			AddString(szMessage,CHAT_TYPE_SYSTEM);
 		}
		break;


	case CW_CHAT_CHANNEL_INFO_NAK:
		{
			MSG_CW_CHAT_CHANNEL_INFO_NAK * pRecvPacket = ( MSG_CW_CHAT_CHANNEL_INFO_NAK *) pMsg;
			AddString(_T("ä�� ����Ʈ ��û�� ���� �߽��ϴ�.") , CHAT_TYPE_SYSTEM );
			OnErrorCode(pRecvPacket->m_byErrorCode);
		}
		break;

	default:
		break;
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::net_proc_CW_CHAT_NORMAL(MSG_BASE * pMsg)
{
	switch (pMsg->m_byProtocol)
	{
	case CW_CHAT_NORMAL_BRD:
		{
			MSG_CW_CHAT_NORMAL_BRD * pRecvPacket = (MSG_CW_CHAT_NORMAL_BRD *)pMsg;
            uiCommunityMan* community_manager = 
                static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
            if (community_manager != NULL && community_manager->FindBlockFriend(pRecvPacket->m_ptszCharName) != -1)// -1 = block
            {  
                return; // do not chat  
            }
			TCHAR	szCharName[INTERFACE_STRING_LENGTH] ={ 0 ,}, szChatMsg[INTERFACE_STRING_LENGTH]={0,};            
			StrnCopy( szCharName , pRecvPacket->m_ptszCharName , MAX_CHARNAME_LENGTH);
			StrnCopy( szChatMsg , pRecvPacket->m_ptszChatMsg ,  MSG_CW_CHAT_NORMAL_BRD::_MAX_CHATMSG_SIZE);
			Player * pPlayer = (Player *)g_ObjectManager.GetObject( szCharName );
			if (pPlayer)
			{
				pPlayer->AddChatMessage(szChatMsg , m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_GENERAL]);
			}

			AddString(szChatMsg, CHAT_TYPE_GENERAL, szCharName );
		}
		break;


	case CW_CHAT_NORMAL_NAK:
		{
			MSG_CW_CHAT_NORMAL_NAK * pRecvPacket = (MSG_CW_CHAT_NORMAL_NAK *)pMsg;
			OnErrorCode( pRecvPacket->m_wErrorCode );		
		}
		break;

	default:
		break;
	}

}

//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::net_proc_CW_PARTY_CHAT(MSG_BASE * pMsg)
{
	switch (pMsg->m_byProtocol)
	{
	case CW_CHAT_PARTY_BRD:
		{
			MSG_CW_CHAT_PARTY_BRD	*pRecvPacket = (MSG_CW_CHAT_PARTY_BRD*)pMsg;
            uiCommunityMan* community_manager = 
                static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
            if (community_manager != NULL && community_manager->FindBlockFriend(pRecvPacket->m_ptszCharName) != -1)// -1 = block
            {  
                return; // do not chat  
            }
			TCHAR	szCharName[INTERFACE_STRING_LENGTH] = {0,}, szChatMsg[INTERFACE_STRING_LENGTH] = {0,};
			StrnCopy( szCharName , pRecvPacket->m_ptszCharName , MAX_CHARNAME_LENGTH);
			StrnCopy( szChatMsg , pRecvPacket->m_ptszChatMsg , MSG_CW_CHAT_PARTY_BRD::_MAX_CHATMSG_SIZE);
			AddString(szChatMsg, CHAT_TYPE_PARTY, szCharName );
		}
		break;
	default:
		break;
	}

}

//----------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::net_proc_CW_WHISPER(MSG_BASE * pMsg)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = { 0,};

	switch (pMsg->m_byProtocol)
	{
	case CW_WHISPER_ACK:
		{
			MSG_CW_WHISPER_ACK * pRecvPacket = (MSG_CW_WHISPER_ACK *)pMsg;

			if(!g_pHero)
				return;
            
            // notice �������� ���� ���ֹǷ� ���� �߰� ���� (define _DH_NORMAL_CHAT_BLOCK)

			TCHAR	szWideString[INTERFACE_STRING_LENGTH] ={ 0,}, szWideMessage[INTERFACE_STRING_LENGTH] ={ 0,};

			StrnCopy( szWideString , pRecvPacket->szCharNameFrom , MAX_CHARNAME_LENGTH);
			StrnCopy( szWideMessage , pRecvPacket->szWhisperMsg , MAX_CHAT_LEN);

			// ���� �ӼӸ��� ���´�.
			if( Strcmp(g_pHero->GetName() , pRecvPacket->szCharNameFrom) == 0)
			{
				SetWhisperSender(m_szTempPendingWhisperSender);
				AddString(szWideMessage, CHAT_TYPE_WHISPER , szWideString ,m_szTempPendingWhisperSender );
			}
			// �ӼӸ��� �޾Ҵ�.
			else
			{
				TCHAR szCharFromName[MAX_CHARNAME_LENGTH +1 ] ={0,};
				StrnCopy( szCharFromName ,pRecvPacket->szCharNameFrom , MAX_CHARNAME_LENGTH );
				SetWhisperReceiver(szCharFromName);
				AddString(szWideMessage, CHAT_TYPE_WHISPER, szWideString );
			}

			// TODO : ACK�� NAK�� �޾ƾ� SEND�� �Ҽ� �ְ� �ؾ��Ұ��̴�.
		}
		break;

	case CW_WHISPER_NAK:
		{
			MSG_CW_WHISPER_NAK * pRecvPacket = (MSG_CW_WHISPER_NAK *)pMsg;

			switch (pRecvPacket->byReason)
			{
			case MSG_CW_WHISPER_NAK::USER_NOT_FOUND:
				{
					//	����ڸ� ã�� �� �����ϴ�.
					g_InterfaceManager.GetInterfaceString( eST_USER_NOT_FOUND, szMessage, INTERFACE_STRING_LENGTH );

					AddString( szMessage, CHAT_TYPE_SYSTEM );
					// TODO : ACK�� NAK�� �޾ƾ� SEND�� �Ҽ� �ְ� �ؾ��Ұ��̴�.
				}
				break;

			case MSG_CW_WHISPER_NAK::USER_WHISPER_OFF:
				{
					//	�ӼӸ��� ���� �� �����ϴ�. ������ �ӼӸ� ������ ������ �����Դϴ�.
					const int	c_iSetWhisperSetOFF = 70381;
					g_InterfaceManager.GetInterfaceString( c_iSetWhisperSetOFF, szMessage, INTERFACE_STRING_LENGTH );

					AddString( szMessage, CHAT_TYPE_SYSTEM );
					// TODO : ACK�� NAK�� �޾ƾ� SEND�� �Ҽ� �ְ� �ؾ��Ұ��̴�.
				}
				break;
			
			default:
				assert(!"���ǵ������� ������ NAK �� �Դ�!!");
				break;
			}
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::net_proc_CW_NOTICE(MSG_BASE * pMsg)
{
	switch (pMsg->m_byProtocol)
	{
	case CW_NOTICE_BRD:
		{
			MSG_CW_NOTICE_BRD * pRecvPacket = (MSG_CW_NOTICE_BRD *)pMsg;

			assert( pRecvPacket->szMsg );

			WzColor wzColor = WzColor_RGBA(pRecvPacket->m_byRed , pRecvPacket->m_byGreen , pRecvPacket->m_byBlue , 255);
			TCHAR	szWideMessage[INTERFACE_STRING_LENGTH] = {0,};
			StrnCopy( szWideMessage , pRecvPacket->szMsg , MAX_NOTICE_LEN);
			uiNoticeMan* pUIMan = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
			if (pUIMan)
			{
				pUIMan->ShowNoticeMessage(eGM_Notice, szWideMessage, 20000, true);
			}

			AddString(szWideMessage , CHAT_TYPE_GM_NOTICE);
		}
		break;
	}

}

//----------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::net_proc_CW_CHAT_SHOUT(MSG_BASE * pMsg)
{
	switch (pMsg->m_byProtocol)
	{
	case CW_CHAT_SHOUT_BRD:
		{
			MSG_CW_CHAT_SHOUT_BRD * pRecvPacket = (MSG_CW_CHAT_SHOUT_BRD *)pMsg;
            uiCommunityMan* community_manager = 
                static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
            if (community_manager != NULL && community_manager->FindBlockFriend(pRecvPacket->m_szCharName) != -1)// -1 = block
            {  
                return; // do not chat  
            }
			TCHAR	szCharName[INTERFACE_STRING_LENGTH] = { 0,}, szChatMessage[INTERFACE_STRING_LENGTH] = { 0,};
			StrnCopy( szCharName , pRecvPacket->m_szCharName , MAX_CHARNAME_LENGTH);
			StrnCopy( szChatMessage , pRecvPacket->m_pszChatMsg , MSG_CW_CHAT_SHOUT_BRD::_MAX_CHATMSG_SIZE);


			WzColor		ShoutColor = m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_SHOUT];
			eCHAT_TYPE	ShoutType   = CHAT_TYPE_SHOUT;
			if( pRecvPacket->m_byMessageType == MSG_CW_CHAT_SHOUT_BRD::DECREE_SHOUT )
			{
				ShoutColor = m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_WORLD_SHOUT];
				ShoutType = CHAT_TYPE_WORLD_SHOUT;
			}
			else if ( pRecvPacket->m_byMessageType == MSG_CW_CHAT_SHOUT_BRD::SUPER_MASTER_SHOUT)
			{
				ShoutColor = m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_SUPER_LOBBY_MASTER_SHOUT];
				ShoutType = CHAT_TYPE_SUPER_LOBBY_MASTER_SHOUT;
			}

			Player * pPlayer = (Player *)g_ObjectManager.GetObject( szCharName );
			if (pPlayer)
			{
				pPlayer->AddChatMessage(szChatMessage ,ShoutColor );
			}			

			AddString(szChatMessage, ShoutType , szCharName );	
		}
		break;

	case CW_CHAT_SHOUT_NAK:
		{
			MSG_CW_CHAT_SHOUT_NAK * pRecvPacket = (MSG_CW_CHAT_SHOUT_NAK *)pMsg;

			OnErrorCode( pRecvPacket->m_wErrorCode );
		}
		break;
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::net_proc_CW_GUILD_CHAT(MSG_BASE * pMsg)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = { 0,};
	switch (pMsg->m_byProtocol)
	{
	case CW_GUILD_CHAT_BRD:
		{
			MSG_CW_GUILD_CHAT_BRD * pRecvPacket = (MSG_CW_GUILD_CHAT_BRD *)pMsg;
            uiCommunityMan* community_manager = 
                static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
            if (community_manager != NULL && community_manager->FindBlockFriend(pRecvPacket->ptszCharName) != -1)// -1 = block
            {  
                return; // do not chat  
            }
			TCHAR	szCharName[INTERFACE_STRING_LENGTH]={0,};
			TCHAR	szChatMsg[INTERFACE_STRING_LENGTH] ={0,};
			StrnCopy( szCharName , pRecvPacket->ptszCharName ,MAX_CHARNAME_LENGTH);
			StrnCopy( szChatMsg , pRecvPacket->ptszChatMsg , MSG_CW_GUILD_CHAT_BRD::_MAX_CHATMSG_SIZE);
			AddString(szChatMsg, CHAT_TYPE_GUILD, szCharName );					
		}
		break;

	case CW_GUILD_CHAT_NAK:
		{
			MSG_CW_GUILD_CHAT_NAK * pRecvPacket = (MSG_CW_GUILD_CHAT_NAK *)pMsg;

			//	��� ä�� ����
			g_InterfaceManager.GetInterfaceString( eST_FAILED_GUILD_CHAT, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
		}
		break;
	}
}

VOID ChatDialog::net_proc_CW_WHISPER_SET_ACK(MSG_BASE * pMsg)
{
	MSG_CW_WHISPER_SET_ACK * pRecvPacket = (MSG_CW_WHISPER_SET_ACK *)pMsg;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	if( pRecvPacket->bWhisper )
	{
		//	�Ӹ� ���� ����
		const int	c_iSetWhisperON = 70379;
		g_InterfaceManager.GetInterfaceString( c_iSetWhisperON, szMessage, INTERFACE_STRING_LENGTH );
		AddString( szMessage, CHAT_TYPE_SYSTEM );
	}
	else
	{
		//	�Ӹ� ���� ����
		const int	c_iSetWhisperOFF = 70380;
		g_InterfaceManager.GetInterfaceString( c_iSetWhisperOFF, szMessage, INTERFACE_STRING_LENGTH );
		AddString( szMessage, CHAT_TYPE_SYSTEM );
	}
}

VOID ChatDialog::net_proc_CG_CONVERSATION_FIELDCHAT_BRD(MSG_BASE * pMsg)
{
	MSG_CG_CONVERSATION_FIELDCHAT_BRD *pRecvMsg = (MSG_CG_CONVERSATION_FIELDCHAT_BRD *)pMsg;

    TCHAR	ChatMsg[INTERFACE_STRING_LENGTH] = { 0,};

	StrnCopy(ChatMsg, pRecvMsg->m_pszChatMsg , MSG_CG_CONVERSATION_FIELDCHAT_SYN::_MAX_CHATMSG_SIZE);
	if (pRecvMsg->m_byMsgLength >= 1)			
	{
		if (ChatMsg[0] == '/')
		{
			return;
		}
	}

	Object *pObject = (Object *)g_ObjectManager.GetObject(pRecvMsg->m_dwObjectKey);

	if (pObject)
	{
		pObject->AddChatMessage(ChatMsg , m_dwTextColor[CHAT_TEXT_COLOR][CHAT_TYPE_GENERAL]);
	}

	AddString( ChatMsg, CHAT_TYPE_GENERAL, pRecvMsg->m_dwObjectKey);
}

#ifdef _NA_007935_20150109_GM_COMMAND_EXP_EVENT
VOID ChatDialog::net_proc_CG_EVENT_PEAKTIME_BRD(MSG_BASE* message)
{
	MSG_CG_EVENT_PEAKTIME_NOTIFY_BRD* recv_packet = static_cast<MSG_CG_EVENT_PEAKTIME_NOTIFY_BRD*>(message);

	TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR exp_ratio_string[INTERFACE_STRING_LENGTH] = {0, };
	Snprintf(exp_ratio_string, INTERFACE_STRING_LENGTH-1, _T("%d"), (int)(recv_packet->exp_ratio * 100));

	switch ( recv_packet->notify_type )
	{
	case ePEAKTIME_PRENOTIFY:
		{
			// ���۳�¥�� ���ᳯ¥�� ���� ���,
			// 70677 : ��� ��, %s�� %s�к��� %s�� %s�б��� ����ġ ȹ�淮 %s%% ���� �̺�Ʈ�� ���۵� �����Դϴ�.
			// ���۳�¥�� ���ᳯ¥�� �ٸ� ���
			// 70678 : ��� ��, %s�� %s�к��� %s�� %s�� %s�б��� ����ġ ȹ�淮 %s%% ���� �̺�Ʈ�� ���۵� �����Դϴ�.
			TCHAR start_time_string[INTERFACE_STRING_LENGTH] = {0, };
			TCHAR start_minute_string[INTERFACE_STRING_LENGTH] = {0, };
			TCHAR end_time_string[INTERFACE_STRING_LENGTH] = {0, };
			TCHAR end_minute_string[INTERFACE_STRING_LENGTH] = {0, };
			itoa(recv_packet->start_time / 100, start_time_string, 10);
			itoa(recv_packet->start_time % 100, start_minute_string, 10);
			itoa(recv_packet->end_time / 100, end_time_string, 10);
			itoa(recv_packet->end_time % 100, end_minute_string, 10);
			if ( recv_packet->start_day == recv_packet->end_day )
			{
				g_InterfaceManager.GetInterfaceString(70677, format_string);
				Snprintf(result_message, INTERFACE_STRING_LENGTH-1, 
					format_string, 
					start_time_string, 
					start_minute_string, 
					end_time_string, 
					end_minute_string, 
					exp_ratio_string);
			}
			else
			{
				TCHAR end_day_string[INTERFACE_STRING_LENGTH] = {0, };
				itoa(recv_packet->end_day, end_day_string, 10);
				g_InterfaceManager.GetInterfaceString(70678, format_string);
				Snprintf(result_message, INTERFACE_STRING_LENGTH-1, 
					format_string, 
					start_time_string, 
					start_minute_string, 
					end_day_string, 
					end_time_string, 
					end_minute_string, 
					exp_ratio_string);
			}
		}
		break;
	case ePEAKTIME_START:
		{
			// 70679 : ����ġ ȹ�淮 %s%% ���� �̺�Ʈ�� ���� �Ǿ����ϴ�.
			g_InterfaceManager.GetInterfaceString(70679, format_string);
			Snprintf(result_message, INTERFACE_STRING_LENGTH-1, format_string, exp_ratio_string);

			TiredSystemDialog* tired_system_dialog = static_cast<TiredSystemDialog*>(GET_DIALOG( IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM ));
			if ( tired_system_dialog )
			{
				tired_system_dialog->set_gm_event_exp_ratio_( recv_packet->exp_ratio );
			}
		}
		break;
	case ePEAKTIME_END:
		{
			// 70680 : ����ġ ȹ�淮 %s%% ���� �̺�Ʈ�� ���� �Ǿ����ϴ�.
			g_InterfaceManager.GetInterfaceString(70680, format_string);
			Snprintf(result_message, INTERFACE_STRING_LENGTH-1, format_string, exp_ratio_string);

			TiredSystemDialog* tired_system_dialog = static_cast<TiredSystemDialog*>(GET_DIALOG( IM_TIRED_SYSTEM_MANAGER::TIRED_SYSTEM ));
			if ( tired_system_dialog )
			{
				tired_system_dialog->set_gm_event_exp_ratio_( 0.0f );
			}
		}
		break;
	}

	WzColor wzColor = WzColor_RGBA(236, 96, 12, 255);
	TCHAR	result_wide_message[INTERFACE_STRING_LENGTH] = {0,};
	StrnCopy( result_wide_message , result_message , MAX_NOTICE_LEN);
	uiNoticeMan* notice_manager_ptr = GET_DIALOG_MANAGER(uiNoticeMan, UIMAN_NOTICE);
	if (notice_manager_ptr)
	{
		notice_manager_ptr->ShowNoticeMessage(eGM_Notice, result_wide_message, 20000, true);
	}

	AddString(result_wide_message , CHAT_TYPE_GM_NOTICE);
}
#endif // _NA_007935_20150109_GM_COMMAND_EXP_EVENT
