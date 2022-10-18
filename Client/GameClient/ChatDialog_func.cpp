//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "GmManager.h"

#include "Chatdialog.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "ResourceManager.h"
#include "LoadScene.h"
#include "MapInfoParser.h"
#include "ObjectManager.h"
#include "ChatMenu.h"
#include "GameFunc.h"
#include "GlobalFunc.h"
#include "DlgPosInfoParser.h"
#include "VillageScene.h"
#include "InterfaceManager.h"
#include "uiGuildMan/uiGuildMan.h"
#include "Application.h"
#include "ResultCode.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "uichatman/uiChatMan.h"
#include "QuickContainer.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "GuildSystem/GuildSystemData.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

using namespace RC;

#include "uiPartyMan/uiPartyMan.h"

#ifdef _KIKI_UI_WEB_DIALOG
	#include "uiWebDialogMan/uiWebDialogMan.h"
#endif

#ifdef MAKE_REALTIME_SEND_PACKET
	#include "MakeVirtualPacket.h"
#endif


sCHAT_TOKEN szChatToken[] =
{
	{CHAT_TYPE_GENERAL, _T("!")},
	{CHAT_TYPE_WHISPER, _T("\"")},
	{CHAT_TYPE_GUILD,	_T("$")},	
	{CHAT_TYPE_PARTY,	_T("#")},
	{CHAT_TYPE_WORLD_SHOUT , _T("@@")},
	{CHAT_TYPE_SHOUT,	_T("@")},	
	{CHAT_TYPE_CHANNEL, _T("&")},
};

#include "MouseHandler.h"
#include "Mouse.h"

#ifdef _YMS_AUTOBOT_GAME
    #include "Autobot/Autobot.h"
#include "HeroData.h"
#endif //_YMS_AUTOBOT_GAME

#pragma warning ( push )
#pragma warning(disable : 4800)

//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::ExhaustChatMsgALL()
{	
	if (m_pChatListCtrl )
	{
		m_pChatListCtrl->DeleteItemAll();
	}
	ExhaustChatMsgList(&m_ChatMsgList);
	m_ChatMsgList.clear();
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::SetFocusChatEdit()
{
	if ( m_pCurEditCtrl )
	{
		GlobalFunc::SetFocus( (SolarDialog *)this,  m_pCurEditCtrl );
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::SetAlwaysOnTop(BOOL bFlag)
{
	this->m_bAlwaysOnTop = bFlag;
}


//----------------------------------------------------------------------------
/**
*/
BOOL			
ChatDialog::IsAlwaysOnTop()
{
	return this->m_bAlwaysOnTop;
}


//----------------------------------------------------------------------------
/**
*/
BOOL			
ChatDialog::IsInputMessageAtChat()
{
	return this->m_bInputChatingMessage;
}



void ChatDialog::Command( TCHAR * pszString)
{
#ifdef MAKE_REALTIME_SEND_PACKET
	TCHAR szTemp[100];
	TCHAR szProtocol[INTERFACE_STRING_LENGTH] ={0,};
	if ( CSTR_EQUAL == CompareString(LOCALE_USER_DEFAULT,NULL, pszString,
				strlen("@send"), "@send",strlen("@send")) )
	{
		if (_stscanf(pszString,_T("%s %s"), szTemp ,szProtocol) == 2)
		{
			CMakeVirtualPacket::Instance()->SendPacket(szProtocol);
			
		}
		
	}
	else if( CSTR_EQUAL == CompareString(LOCALE_USER_DEFAULT,NULL, pszString,
		strlen("@recv"), "@recv",strlen("@recv")) )
	{
		if (_stscanf(pszString,_T("%s %s"), szTemp ,szProtocol) == 2)
		{
			CMakeVirtualPacket::Instance()->RecvPacket(szProtocol);

		}
	}
#endif
}


//----------------------------------------------------------------------------
/**
*/
TCHAR *			
ChatDialog::GetCurChatMessage()
{
	if (m_pChatEditCtrl)
	{
		m_pChatEditCtrl->GetTextWZ(m_szCurChatMessage, MAX_CHATMSG_SIZE);
		return m_szCurChatMessage;
	}
	else
	{
		return NULL;
	}
}


//----------------------------------------------------------------------------
/**
*/
BOOL			
ChatDialog::checkChatToken(TCHAR* pszMessage)
{
	if(_tcslen(pszMessage) <= 0) 
	{
		return FALSE;
	}

	TCHAR	szString[MAX_MESSAGE_LENGTH] = {0,};
	TCHAR	szString2[MAX_MESSAGE_LENGTH] = {0,};
	int iTotalLen = _tcslen(pszMessage);
	StrnCopy(szString, pszMessage+1, iTotalLen-1);

	if( _tcslen(szString) <= 0 )
	{
		return FALSE;
	}

	int iCount = sizeof(szChatToken) / sizeof(szChatToken[0]);
	for ( int i = 0; i < iCount; ++i)
	{
		int testLength = _tcslen(szChatToken[i].szToken );
		// check token
		if ( CSTR_EQUAL == CompareString(
			LOCALE_USER_DEFAULT,
			NULL, 
			pszMessage,
			testLength,
			szChatToken[i].szToken,
			testLength) )
		{	
			switch(szChatToken[i].eChatType)
			{
			case CHAT_TYPE_GENERAL:
				{
					SendChatMsg(szString);
					m_pChatEditCtrl->ClearText();
					return TRUE;
				}
				break;

			case CHAT_TYPE_WHISPER:
				{
					if(_stscanf(szString,_T("%s"),szString2))
					{
						int iCharNameLen = _tcslen(szString2);
						TCHAR szSendMsg[MAX_MESSAGE_LENGTH] ={0,};
						StrnCopy(szSendMsg, szString+iCharNameLen+1, _tcslen(szString)-iCharNameLen+1);
						SendWhisperMsg(szSendMsg, szString2);
					}
					m_pChatEditCtrl->ClearText();
					return TRUE;
				}
				break;

			case CHAT_TYPE_GUILD:
				{
                    bool join_guild = false;
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                    join_guild = GuildSystem::GetGuildSystemData().is_recv_guild_info();
#else
                    uiGuildMan* guildMan = static_cast<uiGuildMan*>(
                        g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));
                    if (guildMan != NULL)
                    {
                        join_guild = guildMan->IsGuildInfo();
                    }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

                    if (join_guild == true)
                    {
                        // 길드가 존재한다.
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                        GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();
#else
                        stGUILD_INFO& guild_info = static_cast<stGUILD_INFO>(guildMan->GetGuildInfo());
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                        SendGuildMsg( szString, guild_info.m_GuildGuid );
                    }
                    else
                    {
                        // 길드가 존재하지 않다면
                        g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_GUILD, szString, MAX_MESSAGE_LENGTH );
                        AddString( szString );
                    }

					m_pChatEditCtrl->ClearText();
					return TRUE;
				}
				break;

			case CHAT_TYPE_PARTY:
				{
					uiPartyMan* partyMan =
						static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

					if (partyMan && partyMan->IsParty())
					{
						SendChatPartyMsg(szString);
					}
					else
					{
						// 파티 존재하지 않음
						g_InterfaceManager.GetInterfaceString( eST_PARTY_NOT_EXIST, szString, MAX_MESSAGE_LENGTH );
						AddString( szString );
					}
					m_pChatEditCtrl->ClearText();
					return TRUE;
				}
				break;

			case CHAT_TYPE_SHOUT:
				{
					SendShoutMsg( szString );
					m_pChatEditCtrl->ClearText();
					return TRUE;
				}
				break;
			case CHAT_TYPE_WORLD_SHOUT:
				{
					SendShoutMsg( &szString[1] , MSG_CW_CHAT_SHOUT_SYN::WORLD_SHOUT);
					m_pChatEditCtrl->ClearText();
					return TRUE;
				}

			case CHAT_TYPE_CHANNEL:
				{
					if(m_bExistChannel)
					{
						SendChatChannelMsg(szString);
					}
					else
					{	
						g_InterfaceManager.GetInterfaceString(eST_CHANNEL_NOT_CREATE , szString ,MAX_MESSAGE_LENGTH);
						AddString(szString);
					}
					m_pChatEditCtrl->ClearText();
					return TRUE;
				}
				break;
			}
		}
	}

	return FALSE;
}

void ChatDialog::ExhaustChatMsgList(std::deque<ChatMsg *> *pChatList)
{
	if( pChatList->size() > 0)
	{
		deque<ChatMsg *>::iterator itr =  pChatList->begin();
		while(itr != pChatList->end())
		{
			ChatMsg * pChatMsg = (ChatMsg *)(*itr);

			if( pChatMsg->m_dwRefCount == 1)
			{
				if( pChatMsg->m_pszCharName )
				{
					delete pChatMsg->m_pszCharName;
				}

				delete pChatMsg;
				pChatMsg = NULL;
			}
			else
			{
				pChatMsg->m_dwRefCount -= 1;
			}
			++itr;
		}
	}
}


//-------------------------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::pushBackChatMsg( ChatMsg * pChatMsg )
{
#ifdef _YMS_AUTOBOT_GAME
    Autobot::Instance()->WriteChat(pChatMsg);
#endif //_YMS_AUTOBOT_GAME

	if ( m_ChatMsgList.size() < MAX_CHAT_REMAIN_MESSAGE  )	
	{
		m_ChatMsgList.push_back( pChatMsg );
		pChatMsg->m_dwRefCount += 1;

		if ( IsPassChatType( pChatMsg->m_eType ) )
		{
			BOOL bScroll = TRUE;
			if ( this->m_VScrollInfo.ScrollRange.iMax > m_iMaxChatMsgLine)
			{
				bScroll = BOOL(this->m_VScrollInfo.iCurPos >= (this->m_VScrollInfo.ScrollRange.iMax - (2)));
			}

			if( ! m_bLockScroll )
				RegisterChatMsgAtList(pChatMsg, bScroll);
		}
	}	
	else
	{
		std::deque<ChatMsg *>::iterator itr = m_ChatMsgList.begin();
		ChatMsg * pBeginMsg = *itr;


		CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
		int nCurPos = pScroll->GetScrollPosWZ();
		
		RECT rtSize = m_pChatListCtrl->GetSize();
		std::vector<util::_tstring> vstr;
		GetCompleteChatMsg(pBeginMsg);

		bool	bChat = true;
		eCHAT_TYPE eChatType = pBeginMsg->m_eType;
		if( eChatType == CHAT_TYPE_SYSTEM )
			bChat = false;

#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
		int nTextWidth = rtSize.right - 20;
#else
		int nTextWidth = rtSize.right - 10;
#endif//__LTJ_TEXT_OUTPUT_AT_LIST_MOD
		GlobalFunc::ProcessWordWrapByFontSize( &vstr, const_cast<TCHAR *>(m_szResult) ,StrToWzID("mn12") ,_tcslen (m_szResult) , nTextWidth, bChat );
		nCurPos -= (int)vstr.size();
		if ( nCurPos < 0)
			nCurPos = 0;

		if( pBeginMsg->m_dwRefCount == 1)
		{
			if( pBeginMsg->m_pszCharName )
			{
				delete pBeginMsg->m_pszCharName;
			}

			SAFE_DELETE( pBeginMsg );
		}
		else
		{
			if( pBeginMsg )
			{
				pBeginMsg->m_dwRefCount -= 1;
			}
		}

		m_ChatMsgList.pop_front();
		m_ChatMsgList.push_back( pChatMsg );
		pChatMsg->m_dwRefCount += 1;

		BOOL bScroll = BOOL(this->m_VScrollInfo.iCurPos >= (this->m_VScrollInfo.ScrollRange.iMax - (2)));

		if( ! m_bLockScroll)
		{
			// 삭제되는 채팅내용이 필터체크가 통과된다면 리스트컨트롤에서 출력되는 내용이므로 리스트 컨트롤에서 삭제
			if ( IsPassChatType( eChatType ) )
			{
				for (int nCount = vstr.size() ; nCount > 0 ; --nCount)
				{
					m_pChatListCtrl->DeleteItem(0);
				}
			}
			// 삽입되는 채팅내용이 필터체크가 통과되면 리스트컨트롤에 삽입하여 출력
			if ( IsPassChatType( pChatMsg->m_eType ))
			{
				RegisterChatMsgAtList(pChatMsg, bScroll);
			}			

			if ( FALSE == bScroll )
			{
				m_VScrollInfo.iStartDrawIndex = nCurPos;
				pScroll->SetScrollPosWZ(nCurPos);				
			}
		}
	}
}

VOID ChatDialog::refreshChatFilterList(BOOL bScroll /*= TRUE*/)
{
	std::deque<ChatMsg *>::iterator itr = m_ChatMsgList.begin();
	std::deque<ChatMsg *>::iterator itr_End = m_ChatMsgList.end();
	ChatMsg * pChatMsg = NULL;
	
	while(itr != itr_End)
	{
		pChatMsg = (*itr);

		if ( pChatMsg )
		{		
			if( IsPassChatType(pChatMsg->m_eType))
				this->RegisterChatMsgAtList( pChatMsg, bScroll ); 
		}
		itr++;
	}
}

//----------------------------------------------------------------------------
/**
*/
BOOL 
ChatDialog::IsPassChatType(eCHAT_TYPE eChatType)
{
	BOOL bResult = TRUE;

	int iFilterMode = GetFilterMode();
	switch (eChatType)
	{
	case CHAT_TYPE_GENERAL:
		{
			bResult = BOOL(this->m_eFilter[iFilterMode] & CHAT_FILTER_TYPE_GENERAL);
		}
		break;
	case CHAT_TYPE_PARTY:
		{
			bResult = BOOL(this->m_eFilter[iFilterMode] & CHAT_FILTER_TYPE_PARTY);
		}
		break;
	case CHAT_TYPE_GUILD:
		{
			bResult = BOOL(this->m_eFilter[iFilterMode] & CHAT_FILTER_TYPE_GUILD);
		}
		break;
	case CHAT_TYPE_SYSTEM:
		{
			bResult = BOOL(this->m_eFilter[iFilterMode] & CHAT_FILTER_TYPE_SYSTEM);
		}
		break;
	case CHAT_TYPE_WHISPER:
		{
			bResult = BOOL(this->m_eFilter[iFilterMode] & CHAT_FILTER_TYPE_WHISPER);
		}
		break;
	case CHAT_TYPE_WORLD_SHOUT:
	case CHAT_TYPE_SUPER_LOBBY_MASTER_SHOUT:
	case CHAT_TYPE_SHOUT:
		{
			bResult = BOOL(this->m_eFilter[iFilterMode] & CHAT_FILTER_TYPE_SHOUT);
		}
		break;
	case CHAT_TYPE_CHANNEL:
		{
			bResult = BOOL ( this->m_eFilter[iFilterMode] & CHAT_FILTER_TYPE_CHANNEL);
		}
		break;
	case CHAT_TYPE_TRADE:
	case CHAT_TYPE_GM:
	case CHAT_TYPE_GM_WHISPER:
    case CHAT_TYPE_GM_NOTICE:
	case CHAT_TYPE_HELP:
    // _100416_LTJ_MODIFY_CHAT_FILTERING
    case CHAT_TYPE_SLANG_FILTERING:
    case CHAT_TYPE_SLANG_FILTERING_NOTICE:
	default:
		{
			bResult = TRUE;
		}
		break;
	}

	return bResult;
}

//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::RefreshChatMsg()
{
	InitChatList();

	deque<ChatMsg *>::iterator itr;
	itr = m_ChatMsgList.begin();

	ChatMsg * pChatMsg = NULL;

	refreshChatFilterList();

	CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
	m_VScrollInfo.iStartDrawIndex = pScroll->GetScrollPosWZ();
	m_pChatListCtrl->SetBeginDrawIndex(m_VScrollInfo.iStartDrawIndex);
	pScroll->SetScrollRangeWZ(0,m_VScrollInfo.iStartDrawIndex);
	pScroll->SetScrollPosWZ(m_VScrollInfo.iStartDrawIndex);
}

//------------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::RegisterChatMsgAtList(ChatMsg * pChatMsg, BOOL bScroll /*= TRUE*/)
{
	memset(m_szResult, 0, sizeof(m_szResult));
	assert( pChatMsg );
	GetCompleteChatMsg(pChatMsg);

	if( !m_pChatListCtrl)
		return;
	RECT rtSize = m_pChatListCtrl->GetSize();
	std::vector<util::_tstring> m_vstr;

	bool	bChat = true;	
	if( pChatMsg->m_eType == CHAT_TYPE_SYSTEM )
		bChat = false;

#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
	int nTextWidth = rtSize.right - 20;
#else
	int nTextWidth = rtSize.right - 10;
#endif//__LTJ_TEXT_OUTPUT_AT_LIST_MOD
	GlobalFunc::ProcessWordWrapByFontSize( &m_vstr, const_cast<TCHAR *>(m_szResult) ,StrToWzID("mn12") ,_tcslen (m_szResult) , nTextWidth, bChat );
	for (size_t i=0; i<m_vstr.size(); ++i)
	{
		int index = m_pChatListCtrl->InsertItemBack();
		TCHAR * pszCurSelCharName = GetCurSelNameAtChatList(pChatMsg);

		if( pszCurSelCharName && g_pHero)
		{
			if( StrnCmp(g_pHero->GetName() , pszCurSelCharName , MAX_CHARNAME_LENGTH ) != 0)
				m_pChatListCtrl->SetItemData(index, pszCurSelCharName );
		}
		TCHAR   szItemText[INTERFACE_STRING_LENGTH];
		_tcsncpy(szItemText, const_cast<TCHAR *>(m_vstr[i].c_str()) , m_vstr[i].size() +1);
		szItemText[m_vstr[i].size()] = '\0';

		GlobalFunc::SetItemText( m_pChatListCtrl, index, szItemText );
		m_pChatListCtrl->SetItemTextColor(index, 0, 
			GlobalFunc::WzColorToRGBA(m_dwTextColor[CHAT_TEXT_COLOR][pChatMsg->m_eType]));

		m_pChatListCtrl->SetItemFontID(index, 0, StrToWzID("mn12"));
		m_pChatListCtrl->SetItemBKColor(index, 0, RGBA(0, 0, 0, 0));

		m_pChatListCtrl->SetItemSelTextColor(index, 0 , 
                                			GlobalFunc::WzColorToRGBA(m_dwTextColor[CHAT_TEXT_COLOR][pChatMsg->m_eType]) );

		// 스크롤 바 재 설정
		int dwVisibleRowCount = m_iMaxChatMsgLine;
		CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
		if ( bScroll )
		{
			// 스크롤 처리			
			if( index > dwVisibleRowCount-1 )
			{
				m_VScrollInfo.iStartDrawIndex = index - (dwVisibleRowCount-1);

				pScroll->SetScrollRangeWZ(0,index - (dwVisibleRowCount-1));
			}
			pScroll->SetScrollPosWZ(index - (dwVisibleRowCount-1));
		}
		else
		{
			pScroll->SetScrollRangeWZ(0,index - (dwVisibleRowCount-1));
		}

	}


}

//----------------------------------------------------------------------------
/**
*/
TCHAR *			
ChatDialog::GetCompleteChatMsg(ChatMsg * pChatMsg)
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH];

	memset(m_szResult, 0, sizeof(m_szResult));

	if (pChatMsg->m_dwSenderKey)
	{
		if (_tcslen(pChatMsg->m_pszCharNameRef) > 0)
		{
			Snprintf(m_szResult, MAX_CHATMSG_SIZE , _T("%s : %s"), pChatMsg->m_pszCharNameRef, pChatMsg->m_pszMessage );
		}
		else
		{
			Snprintf(m_szResult,MAX_CHATMSG_SIZE, _T("%s"), pChatMsg->m_pszMessage);
		}

	}
	else
	{
		switch ( pChatMsg->m_eType )
		{
		case CHAT_TYPE_WHISPER:
			{
				if (pChatMsg->m_pszCharName)
				{
					if (!Strcmp( xGetHeroData()->GetName(), pChatMsg->m_pszCharName) ) 
					{
						// 보낸 귓말 말
						//	%s 님에게 귓속말 : %s
						g_InterfaceManager.GetInterfaceString( eST_SEND_WHISPER, szMessage, INTERFACE_STRING_LENGTH );

						Snprintf(m_szResult, MAX_CHATMSG_SIZE ,szMessage, pChatMsg->m_pszRecvName, pChatMsg->m_pszMessage );
					}
					else
					{
						// 받는 귓말 말
						//	%s 님의 귓속말 : %s
						g_InterfaceManager.GetInterfaceString( eST_RECV_WHISPER, szMessage, INTERFACE_STRING_LENGTH );
						Snprintf(m_szResult,MAX_CHATMSG_SIZE, szMessage, pChatMsg->m_pszCharName, pChatMsg->m_pszMessage );
					}
				}
				else
				{
					assert(0);
					Snprintf(m_szResult, MAX_CHATMSG_SIZE , _T("%s"), pChatMsg->m_pszMessage);
				}				
			}
			break;
        case CHAT_TYPE_SLANG_FILTERING:
            {
                if (g_pHero)
                {
                    Snprintf(m_szResult, MAX_CHATMSG_SIZE , _T("%s : %s"), g_pHero->GetName(), pChatMsg->m_pszMessage);
                }                
            }
            break;
		default:
			{
				if (pChatMsg->m_pszCharName)
				{
					Snprintf(m_szResult, MAX_CHATMSG_SIZE , _T("%s : %s"), pChatMsg->m_pszCharName, pChatMsg->m_pszMessage );
				}
				else
				{
					Snprintf(m_szResult,MAX_CHATMSG_SIZE,  _T("%s"), pChatMsg->m_pszMessage);
				}
			}
			break;
		}
	}

	return m_szResult;
}


//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::AddString( 
					  TCHAR * pszString, 
					  eCHAT_TYPE eChatType /*= CHAT_TYPE_SYSTEM*/,
					  DWORD dwSender /*= 0*/)
{
	if (GlobalFunc::IsSSQScene() && (eChatType != CHAT_TYPE_SYSTEM))
    {
		return;
    }

	// list에 추가
	int iLen = _tcslen(pszString);

	if ((iLen <= 0) || (iLen > MAX_CHATMSG_SIZE))
	{
		return;
	}

	ChatMsg* pChatMsg  = new ChatMsg;
	assert(pChatMsg);

	if (!pChatMsg)
    {
		return;
    }

	pChatMsg->m_eType		= eChatType;

#ifdef _100331_LTJ_MOD_GM_CHAT_COLOR_JAPAN
    // no operation
#else
    Player *pPlayer = (Player *)g_ObjectManager.GetObject( dwSender );
    if (pPlayer)
    {
        if( pPlayer->IsGM() )
        {
            pChatMsg->m_eType = CHAT_TYPE_GM_NOTICE;
        }
    }
#endif //_100331_LTJ_MOD_GM_CHAT_COLOR_JAPAN

	pChatMsg->m_dwSenderKey = dwSender;
	pChatMsg->m_pszCharName = NULL;

	memset(pChatMsg->m_pszMessage, 0, sizeof(pChatMsg->m_pszMessage));
	strncpy(pChatMsg->m_pszMessage, pszString, iLen );
	pChatMsg->m_dwDelayAppearTick = CHAT_MESSAGE_DELAY;
	pChatMsg->m_dwAccTick = 0;

	Object * pObject = g_ObjectManager.GetObject(dwSender);
	if (pObject)
		strncpy(pChatMsg->m_pszCharNameRef, pObject->GetName(), MAX_CHARNAME_LENGTH);
	else
		pChatMsg->m_pszCharNameRef[0] = NULL;


	pushBackChatMsg( pChatMsg );
    bool show_system_chat = (eChatType == CHAT_TYPE_SYSTEM
                            || eChatType == CHAT_TYPE_SLANG_FILTERING_NOTICE);
	if(show_system_chat)
	{
		pushBackSystemChatMsg( pChatMsg );
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::AddString( 
					  TCHAR * pszString, 
					  eCHAT_TYPE eChatType,
					  TCHAR * szSender,
					  TCHAR * szRecver /* = NULL */)
{
	if (GlobalFunc::IsSSQScene())
    {
		return;
    }

	int iLen = _tcslen(pszString);

	if ((iLen <= 0) || (iLen > MAX_CHATMSG_SIZE))
	{
		return;
	}

	ChatMsg* pChatMsg  = new ChatMsg;
	assert(pChatMsg);

	if (!pChatMsg)
    {
		return;
    }

	pChatMsg->m_eType		= eChatType;

#ifdef _100331_LTJ_MOD_GM_CHAT_COLOR_JAPAN
    // no operation
#else
    if ( szSender )
    {
        Player *pPlayer = (Player *)g_ObjectManager.GetObject( szSender );
        if (pPlayer)
        {
            if( pPlayer->IsGM() )
            {
                pChatMsg->m_eType = CHAT_TYPE_GM_NOTICE;
            }
        }
    }    
#endif //_100331_LTJ_MOD_GM_CHAT_COLOR_JAPAN

	pChatMsg->m_pszCharName = new TCHAR[MAX_CLIENT_CHARNAME_LENGTH];

	memset(pChatMsg->m_pszMessage, 0, sizeof(pChatMsg->m_pszMessage));
	strncpy(pChatMsg->m_pszMessage, pszString, iLen );
	pChatMsg->m_dwDelayAppearTick = CHAT_MESSAGE_DELAY;
	pChatMsg->m_dwAccTick = 0;

	if (szSender)
	{
		memset(pChatMsg->m_pszCharName, 0, MAX_CLIENT_CHARNAME_LENGTH);
		strncpy(pChatMsg->m_pszCharName, szSender, MAX_CHARNAME_LENGTH);
		strncpy(pChatMsg->m_pszCharNameRef, szSender, MAX_CHARNAME_LENGTH);
	}

	if (szRecver)
	{
		memset(pChatMsg->m_pszRecvName, 0, MAX_CLIENT_CHARNAME_LENGTH);
		strncpy(pChatMsg->m_pszRecvName, szRecver, MAX_CHARNAME_LENGTH);
	}

	pushBackChatMsg( pChatMsg );

}


//----------------------------------------------------------------------------
/**
*/
POSTYPE 
ChatDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < CHAT_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return CHAT_MAX;
}

//----------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::setChatType( eCHAT_TYPE eChatType )
{
	assert(eChatType < CHAT_TYPE_MAX);
	if ( eChatType >= CHAT_TYPE_MAX ) 
	{
		return;
	}

	this->m_eCurChatType = eChatType;

	// 타입에 따른 일들을 하자
#ifdef _K2P_ADD_WISPER_WINDOW
    DWORD dwTextColor;
    if (eChatType != CHAT_TYPE_WHISPER)
    {
        dwTextColor = GlobalFunc::WzColorToRGBA(this->m_dwTextColor[CHAT_TEXT_COLOR][eChatType]);
    }
    else
    {
        dwTextColor =RGBA(71,160,197,255);
    }
#else
    DWORD dwTextColor = GlobalFunc::WzColorToRGBA(this->m_dwTextColor[CHAT_TEXT_COLOR][eChatType]);
#endif //_K2P_ADD_WISPER_WINDOW	

	if ( m_pChatEditCtrl )
	{
		m_pChatEditCtrl->SetColorText( dwTextColor );
		m_pChatEditCtrl->SetColorCaret( dwTextColor );
		this->m_pCtrlCurChatType->SetTextColor( dwTextColor );
	}
}



//----------------------------------------------------------------------------
/**
*/
VOID ChatDialog::InitChatInfo()
{
	int iStringID = 0;

	setChatType( this->m_eCurChatType );
}

//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::UpdateChatInfo(int iFilterMode)
{
	m_iCurFilterMode = iFilterMode;
}

//----------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::MoveDefaultPosition()
{
	m_eChatPosition = eNORMAL;

	POINT pt = g_pApplication->Get2DModeMapping();
	RECT rcClient;
	GetDialogWZ()->GetClientRect(&rcClient);

	m_MyInitialPoint.x = 3;
    m_MyInitialPoint.y =  pt.y - (rcClient.bottom - rcClient.top + m_iGapOfDialogYPosition);
	this->MoveWindowWZ( (float)m_MyInitialPoint.x, (float)m_MyInitialPoint.y );

    // 체크버튼 배틀존 밖에서는 표시함
    CCtrlButtonCheckWZ* system_button = 
        static_cast<CCtrlButtonCheckWZ*>(getControl(CHAT_SYSTEM_CHAT_BUTTON));
    if (system_button != NULL)
    {
        system_button->ShowWindowWZ(WZ_SHOW);
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::MoveBattleZonePosition( RECT & rcBattleZoneClient )
{
	SetAlwaysOnTop( TRUE );

    POINT pt = g_pApplication->Get2DModeMapping();
	//if (g_pApplication)
	//{
	//	pt = g_pApplication->Get2DModeMapping();
	//}
	//else
	//{
	//	pt.x = 1024;
	//	pt.x = 768;
	//}

	m_eChatPosition = eBATTLEZONE;

	RECT rcCurrent;
	GetDialogWZ()->GetClientRect( &rcCurrent );

	memcpy( &m_rcBattleZone, &rcBattleZoneClient, sizeof(rcBattleZoneClient));
	int iHeight = rcCurrent.bottom - rcCurrent.top;

	MoveWindowWZ( (float)(rcBattleZoneClient.left + 20), (float)(rcBattleZoneClient.bottom - (iHeight+26)) );

    // 체크버튼 배틀존에서는 표시안함
    CCtrlButtonCheckWZ* system_button = 
        static_cast<CCtrlButtonCheckWZ*>(getControl(CHAT_SYSTEM_CHAT_BUTTON));
    if (system_button != NULL)
    {
        system_button->ShowWindowWZ(WZ_HIDE);
    }
}

//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::RenderChatMsgWithoutDialog( DWORD dwTick )
{
	if( !m_pChatListCtrl)
		return;
	std::deque<ChatMsg *> * pCurDeque = &m_ChatMsgList;
	if( pCurDeque == NULL ) return;
	if( pCurDeque->size()  < 1 ) return;
	BOOL bShow = this->IsVisibleWindow();

	int iLineCnt = m_iMaxChatMsgLine ;

	ChatMsg * pChatMsg = NULL;

	RECT rcClient;
	rcClient = m_pChatListCtrl->GetSizeRect();

	char  * pszText = "W";
	SIZE    FontSize;
	SIZE    TextSize;

	g_pSunRenderer->x_pManagerTextOut->StartToUseFont("mn12");

	g_pSunRenderer->x_pManagerTextOut->GetTextExtentPointA(pszText, strlen(pszText), &FontSize);

	deque<ChatMsg *>::reverse_iterator itr = pCurDeque->rbegin();
	while(itr != pCurDeque->rend() )
	{
		BYTE    fadeValue = 255;

		if (iLineCnt <= 0 )
		{
			break;
		}

		pChatMsg = (ChatMsg *)(*itr);

		pChatMsg->m_dwAccTick += dwTick;


		//	화면에 랜더링할 필요없고 시간만 업데이트 시켜주면 된다
		if( bShow == TRUE )
		{
			--iLineCnt;
			++itr;
			continue;
		}

		m_WzTempFontColor = WzColor_RGBA(
			Red_WzColor(m_dwTextColor[CHAT_TEXT_COLOR][pChatMsg->m_eType]),
			Green_WzColor(m_dwTextColor[CHAT_TEXT_COLOR][pChatMsg->m_eType]),
			Blue_WzColor(m_dwTextColor[CHAT_TEXT_COLOR][pChatMsg->m_eType]),
			255);

		if (pChatMsg->m_dwAccTick >= pChatMsg->m_dwDelayAppearTick + CHAT_HIDE_DELAY )
		{
			--iLineCnt;
			++itr;

			continue;		// 안보여주고 스킵하기.
		}
		else if (pChatMsg->m_dwAccTick >= pChatMsg->m_dwDelayAppearTick )
		{
			// 알파값을 서서히 더해서 사라지게 만든다.
			float	fPercent = ((float)(pChatMsg->m_dwAccTick - pChatMsg->m_dwDelayAppearTick) / (float)(CHAT_HIDE_DELAY));
			m_fDelta = 1.0f - fPercent;
			if( m_fDelta < 0.0f )
			{
				m_fDelta = 0.0f;
			}

			fadeValue = (BYTE)(m_fDelta*255.0f);
			m_WzTempFontColor = WzColor_RGBA(
				Red_WzColor(m_dwTextColor[CHAT_TEXT_COLOR][pChatMsg->m_eType]),
				Green_WzColor(m_dwTextColor[CHAT_TEXT_COLOR][pChatMsg->m_eType]),
				Blue_WzColor(m_dwTextColor[CHAT_TEXT_COLOR][pChatMsg->m_eType]),
				fadeValue);

		}

		if ( IsPassChatType(pChatMsg->m_eType ) )
		{
			GetCompleteChatMsg(pChatMsg);

			std::vector<util::_tstring> m_vstr;

			bool bChat = true;
			if( pChatMsg->m_eType == CHAT_TYPE_SYSTEM )
				bChat = false;

			RECT rtSize = m_pChatListCtrl->GetSize();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
			int nTextWidth = rtSize.right - 20;
#else
			int nTextWidth = rtSize.right - 10;
#endif//__LTJ_TEXT_OUTPUT_AT_LIST_MOD
			GlobalFunc::ProcessWordWrapByFontSize( &m_vstr, const_cast<TCHAR *>(m_szResult), StrToWzID("mn12") ,_tcslen (m_szResult) , nTextWidth, bChat );

			for (size_t i=0; i<m_vstr.size(); ++i)
			{
				TCHAR   szItemText[INTERFACE_STRING_LENGTH] = {0,};
				StrnCopy(szItemText, const_cast<TCHAR *>(m_vstr[(m_vstr.size()-1) - i].c_str()) , m_vstr[(m_vstr.size()-1) - i].size());
				szItemText[m_vstr[(m_vstr.size()-1) - i].size()] = '\0';

				g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szItemText, _tcslen(szItemText), &TextSize);	

				const int	LINE_POS_X = 0;
				const int	LINE_POS_Y = 3;
                const int	LINE_GAP = 16;

				RECT rc;
				rc = rcClient;
				rc.left += LINE_POS_X;
				rc.top += LINE_POS_Y + ((iLineCnt-1) * LINE_GAP);
				RenderOutLinedText(szItemText,&rc,m_WzTempFontColor, fadeValue);
				--iLineCnt;
			}
		}

		++itr;
	} 

	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL SendWhisper( TCHAR * pszMessage, TCHAR * pszToChar)
{
	ChatDialog * pChatDlg =	GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

	if (pChatDlg)
	{
		pChatDlg->SendWhisperMsg( pszMessage, pszToChar );
	}
	return FALSE;
}


//-------------------------------------------------------------------------------------------
/**
*/
BOOL SendNotice( TCHAR * pszMessage )
{
	ChatDialog * pChatDlg =	GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

	if (pChatDlg)
	{
		return pChatDlg->SendNoticeMsg( pszMessage );
	}
	return FALSE;
}


BOOL SendGMNotice( TCHAR * pszMessage, BYTE byNoticeType )
{
	ChatDialog * pChatDlg =	GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);

	if (pChatDlg)
	{
		return pChatDlg->SendGMNoticeMsg( pszMessage, byNoticeType );
	}
	return FALSE;
}

//------------------------------------------------------------------------------ 
void	ChatDialog::MoveDefaultWindowPos()
{
    SolarDialog::MoveDefaultWindowPos();

    if (g_pHero != NULL)
    {
        uiChatMan* char_man = GET_DIALOG_MANAGER(uiChatMan, UIMAN_CHAT);
        QuickContainer* quick_container = g_pHero->GetQuickContainer();
        if (char_man != NULL && quick_container != NULL)
        {
            if (quick_container->double_size() == true)
            {
                char_man->MoveChatDialog(2);
            }
            else
            {
                char_man->MoveChatDialog(1);
            }
        }
    }

}

////----------------------------------------------------------------------------
///**
//*/
//void 
//ChatDialog::UpdatePosition()
//{
//	switch ( m_eChatPosition )
//	{
//	case eNORMAL:
//		this->MoveDefaultPosition();
//		break;
//	}
//}

void ChatDialog::SetChatMessage(TCHAR* pszString)
{
	if(m_pChatEditCtrl)
	{
		m_pChatEditCtrl->ClearText();
		m_pChatEditCtrl->SetTextWZ(pszString);
		SetFocusChatEdit();
	}
}

//------------------------'-------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::OnErrorCode( DWORD dwErrorCode )
{
    TCHAR buffer[INTERFACE_STRING_LENGTH] = { 0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = { 0,};
	switch (dwErrorCode )
	{
	case RC_CHAT_BLOCKED:					// 채팅 금지 상태
		{
			// 해당 캐릭터는 채팅 기능 이용이 제한 되었습니다. 자세한 사항은....어쩌고....
			g_InterfaceManager.GetInterfaceString( 5232, szMessage , INTERFACE_STRING_LENGTH );
			AddString(szMessage);

		}
		break;
	case RC_CHAT_ALLOWED:					// 채팅 허용 상태
		{

		}
		break;

	case RC_CHAT_SYNC:						// 동기화 오류(마을/필드에서 발생하며, VIEWPORT_CHARSTATE를 다시 보내줘야 함)
		{

		}
		break;

	case RC_CHAT_NOCHAR:					// 상대 캐릭터가 없거나 접속하지 않았음
		{

		}
		break;

	case RC_CHAT_ALREADYSHOUT:				// 외치기 시간 간격 이전에 다시 시도
		{
			// 5061	외치기는 연속으로 하실 수 없습니다.	
			// 5062	잠시 후에 다시 시도해 주시기 바랍니다.
			g_InterfaceManager.GetInterfaceString(5061, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
			g_InterfaceManager.GetInterfaceString(5062, szMessage, INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
		}
		break;

	case RC_CHAT_NOROOM:
		{
			// 채널이 존재하지 않습니다.
			g_InterfaceManager.GetInterfaceString(eST_CHANNEL_NOT_CREATE , szMessage , INTERFACE_STRING_LENGTH );
			AddString(szMessage);
		}
		break;
	case RC_CHAT_ALREADYINROOM:
		{
			//채널에 이미 참여 중입니다.
			g_InterfaceManager.GetInterfaceString(eST_CHANNEL_ALREADY_JOIN , szMessage , INTERFACE_STRING_LENGTH );
			AddString(szMessage);
		}
		break;
	case RC_CHAT_NOT_SUPER_MASTER:
		{
			//5867	슈퍼방장만 사용이 가능합니다.
			g_InterfaceManager.GetInterfaceString( 5867 , szMessage ,INTERFACE_STRING_LENGTH );
			Snprintf( szMessage , INTERFACE_STRING_LENGTH -1, szMessage);
			AddString(szMessage);
		}
		break;

	case RC_CHAT_NOT_DECREE_ITEM:
		{
			// 5066 해방군의 포고문이 없어 월드외치기 사용이 불가능 합니다.
			g_InterfaceManager.GetInterfaceString(5066 , szMessage , INTERFACE_STRING_LENGTH);
			AddString(szMessage);
		}
		break;
#ifdef __NA_0_20080515_BUG_FIX_WORLD_SHOUT_LEVEL
	case RC_WORLD_SHOUT_LIMIT_LEVEL:		// TW_월드 외치기 레벨 제한
#endif
	case RC_GENERAL_SHOUT_LIMIT_LEVEL:
		{
			// 5060 외치기 기능은 20레벨부터 사용 가능합니다.
			g_InterfaceManager.GetInterfaceString( 5060  , szMessage , INTERFACE_STRING_LENGTH);
			AddString(szMessage);
		}
		break;

	case RC_CHAT_USE_BAN_WORD:
		{
			// 금지단어라 사용할수 없습니다.
			g_InterfaceManager.GetInterfaceString( 70022  , szMessage , INTERFACE_STRING_LENGTH);
			AddString(szMessage);
		}
		break;

	case RC_CHANNEL_CHAT_TITLE_BAN_WORD:
		{
			// 금지단어라 사용할수 없습니다.
			g_InterfaceManager.GetInterfaceString( 70022  , szMessage , INTERFACE_STRING_LENGTH);
			AddString(szMessage);
		}
		break;


	case RC_CHAT_ANTI_DOBAE:
		{
			/// 1602	같은 내용의 글은 연속으로 입력할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(eST_BAN_SAME_CHAT_MSG , szMessage ,INTERFACE_STRING_LENGTH );
			AddString(szMessage);
		}
		break;
    case RC_CHANNEL_CHAT_BLOCKED:
        {
            /// 2702 채널초대를 차단하고 있어 초대할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(2702 , szMessage ,INTERFACE_STRING_LENGTH);
            AddString(szMessage);
        }
        break;
    case RC_CHAT_DENY_INVITATION:
        {
            /// 1711 상대방이 채널초대를 사양했습니다.
            g_InterfaceManager.GetInterfaceString(1711 , szMessage ,INTERFACE_STRING_LENGTH);
            AddString(szMessage);
        }
        break;
    default:
        {
            sprintf(szMessage, "unknow errorcode(%d)", dwErrorCode);
            AddString(szMessage);
        }
        break;
	}	
}


VOID RenderChatMsg( DWORD dwTick )
{
	ChatDialog * pDlg = GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
	if (pDlg)
	{
		pDlg->RenderChatMsgWithoutDialog(dwTick);
	}
}

void ChatDialog::OnNoramlChat()
{
	SetCheckButtonForFilter( eCHECK_ALL );
	
	UpdateChatInfo(eLFM_NORMAL);
	m_pPropertyDlg->SetPropertyDlgFilter( eLFM_NORMAL , m_eFilter[eLFM_NORMAL]);
	setChatType(CHAT_TYPE_GENERAL);
	m_pCtrlCurChatType->SetTextID(eST_NORMAL_CHAT);

	if( m_pChatListCtrl )
		m_pChatListCtrl->DeleteItemAll();
	
	refreshChatFilterList();
}

void ChatDialog::OnGuildChat()
{
	SetCheckButtonForFilter( eCHECK_GUILD );
	
	UpdateChatInfo(eLFM_GUILD);
	setChatType(CHAT_TYPE_GUILD);
	m_pPropertyDlg->SetPropertyDlgFilter(eLFM_GUILD , m_eFilter[eLFM_GUILD]);
	m_pCtrlCurChatType->SetTextID(eST_GUILD_CHAT);

	if( m_pChatListCtrl )
		m_pChatListCtrl->DeleteItemAll();
	
	refreshChatFilterList();
}

void ChatDialog::OnPartyChat()
{
	SetCheckButtonForFilter( eCHECK_PARTY );
	
	m_pPropertyDlg->SetPropertyDlgFilter(eLFM_PARTY , m_eFilter[eLFM_PARTY]);
	UpdateChatInfo(eLFM_PARTY);
	setChatType(CHAT_TYPE_PARTY);
	m_pCtrlCurChatType->SetTextID(eST_PARTY_CHAT);

	if( m_pChatListCtrl )
		m_pChatListCtrl->DeleteItemAll();
	
	refreshChatFilterList();

}

//------------------------------------------------------------------------------
int	ChatDialog::getTooltipString(POSTYPE pos)
{
	map<int,int>::iterator iter;

	iter = m_UseTooltipCtrls.find(pos);

	if (iter != m_UseTooltipCtrls.end())
	{
		if( pos == CHAT_CHECK_PROPERTY )
		{
			if( m_pCheckFilter[eCHECK_PROPERTY]->GetCheckState() ) 
			{
				return eST_CHAT_PROPERTY_ON;
			}
			else
			{
				return eST_CHAT_PROPERTY_OFF;
			}
		}

		if ( pos == CHAT_CHECK_LOCK_SCROLL )
		{
			if( m_pCheckFilter[eCHECK_LOCK_SCROLL]->GetCheckState())
			{
				return 1715;
			}
			else
			{
				return 1716;
			}	
		}


		return iter->second;

	}
	else
	{
		return 0;
	}
}

//------------------------------------------------------------------------------
void ChatDialog::updateTooltip()
{
	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	BOOL	bChannelMouseOver = FALSE;


	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;
	for (POSTYPE a = 0; a < CHAT_MAX; a++)
	{
		CControlWZ * pControl = getControl(a);

		if(pControl)
		{
			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = TRUE;
					if ( a == CHAT_CHECK_CHANNEL )
					{
						bChannelMouseOver = TRUE;
					}
				}
			}
		}
	}

	if ( !bMouseOver )
	{
		m_iTooltipStringCode = 0;
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		return;
	}

	if( bChannelMouseOver )
	{

		if( m_bExistChannel)
		{
			TCHAR szChannelName[11];
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
			{
				pUIMan->RegisterChannelInfoTooltip( m_ChannelName , m_ChannelJoinMember_Num );
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}
			StrnCopy(szChannelName,m_ChannelName,10);
			szChannelName[10] = 0;
			GlobalFunc::SetCaption(m_pCheckFilter[eCHECK_CHANNEL],szChannelName);
		}
		else
		{
			TCHAR szMessage[INTERFACE_STRING_LENGTH];
			g_InterfaceManager.GetInterfaceString(eST_CHANNEL_CHAT,szMessage , INTERFACE_STRING_LENGTH);
			m_pCheckFilter[eCHECK_CHANNEL]->SetTextID(eST_CHANNEL_CHAT);
		}

	}
	else
	{
		if(m_iTooltipStringCode)
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
			if(pUIMan)
			{
				pUIMan->RegisterTooltipWithStringCode(m_iTooltipStringCode);
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}
		}
	}
}

//-------------------------------------------------------------------------------------------
/**
*/
void            
ChatDialog::clearEdit()
{
	ZeroMemory( m_szCurChatMessage, sizeof(m_szCurChatMessage) );
	if ( m_pChatEditCtrl )
	{
		m_pChatEditCtrl->ClearText();
	}
}
void ChatDialog::pushBackSystemChatMsg( ChatMsg * pChatMsg )
{
	assert(m_pSystemChatDlg);
	if(m_pSystemChatDlg)
	{
		m_pSystemChatDlg->PushBackChatMsg(pChatMsg);
	}
}

void ChatDialog::OnChannelChat()
{
	SetCheckButtonForFilter( eCHECK_CHANNEL );

	m_pPropertyDlg->SetPropertyDlgFilter(eLFM_CHANNEL , m_eFilter[eLFM_CHANNEL]);
	UpdateChatInfo(eLFM_CHANNEL);
	setChatType(CHAT_TYPE_CHANNEL);
	m_pCheckFilter[eCHECK_SYSTEM]->SetCheckState(FALSE);
	m_pCtrlCurChatType->SetTextID(283);

	if( m_pChatListCtrl )
		m_pChatListCtrl->DeleteItemAll();
	
	refreshChatFilterList();

}

#ifdef _K2P_ADD_WISPER_WINDOW
void ChatDialog::OnWisperChar()
{
	SetCheckButtonForFilter( eCHECK_WISPER );

	m_pPropertyDlg->SetPropertyDlgFilter( eLFM_WISPER, m_eFilter[eLFM_WISPER] );
	UpdateChatInfo(eLFM_WISPER);
    setChatType(CHAT_TYPE_WHISPER);
    m_pCtrlCurChatType->SetTextID(9706);
	
	if( m_pChatListCtrl )
		m_pChatListCtrl->DeleteItemAll();

	refreshChatFilterList();
	
}
#endif

void ChatDialog::SetCheckButtonForFilter( int index )
{	
	for( int i = 0; i < eCHECK_MAX; ++i )
	{
		if( i == index )
		{
			m_pCheckFilter[i]->SetCheckState( TRUE );
		}
		else
		{
			m_pCheckFilter[i]->SetCheckState( FALSE );
		}
	}
}

void ChatDialog::SetWhisperSender( TCHAR * pszMessage)
{
	std::deque<SenderName *>::iterator itr = m_WhisperSenderList.begin();
	SenderName * pSenderName = NULL;

	// 동일 이름 검색
	while( itr != m_WhisperSenderList.end() )
	{
		pSenderName = *itr;
		// 동일 이름이 존재함.
		if( StrnCmp ( pSenderName->m_szName , pszMessage , MAX_CHARNAME_LENGTH ) == 0)
		{
			// 동일 이름 삭제
			SAFE_DELETE(pSenderName);
			m_WhisperSenderList.erase(itr);

			// 새로운 이름 추가
			SenderName * pNewData = new SenderName;
			StrnCopy(pNewData->m_szName , pszMessage , MAX_CHARNAME_LENGTH);
			m_WhisperSenderList.push_back(pNewData);
			m_iWhisperDequeIndex = m_WhisperSenderList.size();
            return;
		}
		itr++;
	}
	
	// 동일 이름 존재하지 않음
	if( m_WhisperSenderList.size() > MAX_PENDING_WHISPER_SENDER )
	{
		// 제일 처음 원소를 지워준다.
		itr = m_WhisperSenderList.begin();
		pSenderName = *itr;
		SAFE_DELETE(pSenderName);
		m_WhisperSenderList.pop_front();
	}
	// 새로운 이름 추가
	SenderName * pNewData = new SenderName;
	StrnCopy(pNewData->m_szName , pszMessage , MAX_CHARNAME_LENGTH);
	m_WhisperSenderList.push_back(pNewData);
	m_iWhisperDequeIndex = m_WhisperSenderList.size();
	return;
}

TCHAR * ChatDialog::GetWhisperSender()
{
	if( m_iWhisperDequeIndex > 0 )
	{
		int index = m_iWhisperDequeIndex -1;

		return m_WhisperSenderList[index]->m_szName;
	}
	return NULL;
}

void ChatDialog::SetWhisperReceiver( TCHAR * pszMessage )
{
	memset(m_szPendingWhisperSenderToMe,0 ,sizeof(m_szPendingWhisperSenderToMe));
	if ( pszMessage != NULL)
	{
		StrnCopy( m_szPendingWhisperSenderToMe, pszMessage , MAX_CHARNAME_LENGTH);
	}
}


void ChatDialog::autoFillWhisperSender()
{
	if(GetWhisperSender()  == 0)
		return;

	TCHAR szMessage[MAX_CHATMSG_SIZE+1] = {0,};
	CCtrlDxIMEEditBox * pE = (CCtrlDxIMEEditBox *)getControl( CHAT_EDIT );
	assert(pE);
	pE->GetTextWZ( szMessage, MAX_CHATMSG_SIZE );

	int iLength = _tcslen(szMessage);

	if( iLength == 1 && szMessage[0] == _T('"') && m_bFirstEnter )
	{
		int index = m_WhisperSenderList.size() - 1;
		TCHAR szWhisper[64] = {0,};
		szWhisper[0] = _T('"');
		StrnCopy(&szWhisper[1], m_WhisperSenderList[index]->m_szName , MAX_CHARNAME_LENGTH);
		_tcscat(szWhisper,_T(" "));
		SetChatMessage(szWhisper);
		m_bFirstEnter = FALSE;
		return;
	}

	if( m_bClickUPandDOWN )
	{
		m_bClickUPandDOWN = FALSE;
		TCHAR szToken[INTERFACE_STRING_LENGTH] = { 0,} , szToken2[INTERFACE_STRING_LENGTH] = {0,};
		_stscanf( szMessage , _T("%s %s") , szToken , szToken2 );
		if( szMessage[0] == _T('"') && _tcslen(szToken2) == 0 )
		{
			TCHAR szWhisper[64] = {0,};
			szWhisper[0] = _T('"');
			StrnCopy(&szWhisper[1], GetWhisperSender() , MAX_CHARNAME_LENGTH);
			_tcscat(szWhisper,_T(" "));
			SetChatMessage(szWhisper);
			return;
		}
	}
}

void ChatDialog::autoFillWhisperreceiver() 
{
	TCHAR szMessage[MAX_CHATMSG_SIZE+1] = {0,};
	CCtrlDxIMEEditBox * pE = (CCtrlDxIMEEditBox *)getControl( CHAT_EDIT );
	assert(pE);
	pE->GetTextWZ( szMessage, MAX_CHATMSG_SIZE );

	int iLength = _tcslen(szMessage);
	if( iLength == 2 && szMessage[0] == _T('/') && ( szMessage[1] == _T('r') || szMessage[1] == _T('R') ) )
	{
		int iLength = _tcslen(GetWhisperReceiver());
		if( iLength == 0)
			return;

		TCHAR szWhisper[64] = {0,};
		szWhisper[0] = _T('"');
		StrnCopy(&szWhisper[1], GetWhisperReceiver() , MAX_CHARNAME_LENGTH);
		_tcscat(szWhisper,_T(" ") );
		SetChatMessage(szWhisper);
		m_bFirstEnter = FALSE;
		return;
	}
}

void ChatDialog::AutoFill()
{
	if( HaveFocusInEdit() )
	{
		autoFillWhisperSender();
		autoFillWhisperreceiver();
	}
}

#ifdef 	_000541_BLOCK_CHAT_REPEAT
BOOL ChatDialog::isRepeatChatting(TCHAR * pszMessage)
{
	if( !PROGRAMPARAM->CheckBlockChatRepeat() )
		return FALSE;

	DWORD dwCurTime = clock_function::GetTickCount();
	if( ( dwCurTime - m_dwPendingSendChattingTime ) < PROGRAMPARAM->GetBlockChatRepeatTime() )
	{
		int iPending_Length = _tcslen( m_szPendingSendChatMsg );
		int iInput_Length = _tcslen( pszMessage );
		if( StrnCmp(m_szPendingSendChatMsg , pszMessage , max(iPending_Length , iInput_Length) ) == 0)
		{
			// 1602	같은 내용의 글은 연속으로 입력할 수 없습니다.
			TCHAR szMessage[INTERFACE_STRING_LENGTH];
			g_InterfaceManager.GetInterfaceString(eST_BAN_SAME_CHAT_MSG , szMessage ,INTERFACE_STRING_LENGTH );
			AddString(szMessage,CHAT_TYPE_SYSTEM);
			return TRUE;
		}
	}

	return FALSE;
}

void ChatDialog::pendingSendChatInfomation(TCHAR * pszMessage)
{
	StrnCopy( m_szPendingSendChatMsg , pszMessage , MAX_CHATMSG_SIZE );
	m_dwPendingSendChattingTime = clock_function::GetTickCount();
}
#endif


void ChatDialog::AutoFillWhisperNameWhenChatListClick()
{
	const int c_iSize = 50;
	TCHAR szResult[c_iSize];
	memset(szResult, 0 , sizeof(szResult));
	int  index = m_pChatListCtrl->GetCurSel();
	TCHAR * pszMessage = (TCHAR * )m_pChatListCtrl->GetItemData(index);
	if( pszMessage)
	{
		// 2010	/귓속말
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
		g_InterfaceManager.GetInterfaceString(2010, szTemp, INTERFACE_STRING_LENGTH);
		
		Snprintf(szResult, c_iSize-1, _T("%s %s "), szTemp, pszMessage);
		SetChatMessage(szResult);
	}
}

TCHAR *  ChatDialog::GetCurSelNameAtChatList(ChatMsg * pChatMsg)
{
	if (pChatMsg->m_dwSenderKey)
	{
		if (_tcslen(pChatMsg->m_pszCharNameRef) > 0)
			return pChatMsg->m_pszCharNameRef;
	}
	else
	{
		switch ( pChatMsg->m_eType )
		{
		case CHAT_TYPE_WHISPER:
			{
				if (pChatMsg->m_pszCharName)
				{
					if (!Strcmp( xGetHeroData()->GetName(), pChatMsg->m_pszCharName) ) 
						return pChatMsg->m_pszRecvName;
					else
						return pChatMsg->m_pszCharName;
				}		
			}
			break;

		default:
			{
				if (pChatMsg->m_pszCharName)
					return pChatMsg->m_pszCharName;
			}
			break;
		}
	}
	return NULL;
}
void ChatDialog::OnSystemChat()
{
	SetCheckButtonForFilter( eCHECK_SYSTEM );
	
	UpdateChatInfo(eLFM_SYSTEM);
	m_pPropertyDlg->SetPropertyDlgFilter(eLFM_SYSTEM , m_eFilter[eLFM_SYSTEM]);

	if( m_pChatListCtrl )
		m_pChatListCtrl->DeleteItemAll();
	
	refreshChatFilterList();
}

int ChatDialog::GetCurrentBitFilter()
{
	assert( MAX_CHAT_CHECK > m_iCurFilterMode);
	return m_eFilter[m_iCurFilterMode];
}

void ChatDialog::SetCurrentBitFilter(int filter)
{
	m_eFilter[m_iCurFilterMode] = filter;
	if( m_pChatListCtrl )
		m_pChatListCtrl->DeleteItemAll();
	
	refreshChatFilterList();
}

void ChatDialog::ChangeChatListControl(eLIST_CONTROL_TYPE eType)
{
	for ( int i = 0; i < LIST_CONTROL_MAX ; i ++)
	{
		if( i != eType )
			m_pListControlType[i]->ShowWindowWZ(WZ_HIDE);
	}
	if( m_pChatListCtrl )
		m_pChatListCtrl->DeleteItemAll();

	m_pChatListCtrl = m_pListControlType[eType];
	m_pChatListCtrl->ShowWindowWZ(WZ_SHOW);
	updateDialogSize();
	refreshChatFilterList();
}

void ChatDialog::updateDialogSize()
{
	RECT ControlRect = m_pChatListCtrl->GetSizeRect();

	RECT DialogRect;
	this->GetDialogWZ()->GetDlgPartSizeRect(&DialogRect );

	DialogRect.top = ControlRect.top;

	float fWidth = (float) ( DialogRect.right  - DialogRect.left);
	float fHeight = (float) ( DialogRect.bottom - DialogRect.top);
	this->GetDialogWZ()->SetDlgPartSizeRect(DialogRect );
}
void ChatDialog::ChatPropertyDisable()
{
    if (m_pCheckFilter[eCHECK_PROPERTY] != NULL)
    {
        m_pCheckFilter[eCHECK_PROPERTY]->SetCheckState(false);
    }
}
void ChatDialog::ShowSystemChatDialog( BOOL val)
{
	m_bShowSystemChat =val;
	m_pSystemChatDlg->ShowWindow(val);

    // 체크버튼 상태 변경
    CCtrlButtonCheckWZ* system_button = 
        static_cast<CCtrlButtonCheckWZ*>(getControl(CHAT_SYSTEM_CHAT_BUTTON));
    if (system_button != NULL)
    {
        bool check_state = (val == TRUE ? false : true);
        system_button->SetCheckState(check_state);
    }
}


void ChatDialog::updateWhisperSelectInput()
{
	if ( HaveFocusInEdit() )
	{
		if( GetAsyncKeyState(VK_UP) )
		{
			if( m_iWhisperDequeIndex > 0 )
			{
				m_iWhisperDequeIndex -=1;
				m_bClickUPandDOWN = TRUE;
			}
		}
		if( GetAsyncKeyState(VK_DOWN) )
		{
			if( m_iWhisperDequeIndex < (int)m_WhisperSenderList.size() )
			{
				m_iWhisperDequeIndex += 1;
				m_bClickUPandDOWN = TRUE;
			}
		}
	}
	
}

void ChatDialog::updateScrollPosition()
{
	int total = m_pChatListCtrl->GetListRowCnt();//입력된 총 라인수를 구한다.
	if( total > m_iMaxChatMsgLine )
	{
		CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
		if(pScroll)
		{
			m_VScrollInfo.iCurPos = pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.

			pScroll->GetScrollRangeWZ(&m_VScrollInfo.ScrollRange.iMin, 
                                      &m_VScrollInfo.ScrollRange.iMax); //스크롤 값의 최대 최소를 얻어 온다.
			

			int iPos = m_VScrollInfo.iCurPos;
	        
			m_VScrollInfo.iStartDrawIndex = (iPos < 0)? 0 : iPos;
			m_pChatListCtrl->SetBeginDrawIndex( m_VScrollInfo.iStartDrawIndex );
		}
	}
	else
	{
		m_pChatListCtrl->SetBeginDrawIndex( 0 );
	}
}

//------------------------------------------------------------------------------ 
void ChatDialog::ShowChatMsgInHistory()
{
#if defined(_CHINA)
    if (PROGRAMPARAM->nation_type() == kLanguageTaiwan)       //!< 2:중국, 3:대만
    {
        //대만은 history 사용하지 않는다.
        return;
    }
#endif

	if(NULL == m_pChatEditCtrl)
    {
		return;
    }

	if( m_vecChatHistory.empty() )
    {
		return;
    }

	TCHAR szMessage[MAX_CHATMSG_SIZE+1] = {0,};
	m_pChatEditCtrl->GetTextWZ(szMessage, MAX_CHATMSG_SIZE);
	if('"' == szMessage[0])
		return;

	if ( HaveFocusInEdit() )
	{
		TCHAR szMsg[MAX_CHATMSG_SIZE+1] = {0,};
#ifdef _DH_CHANGE_CHAT_HISTORY
	    if(g_Input.GetState(DIK_DOWN, KS_DOWN) || g_Input.GetState(DIK_DOWN, IKS_DOWN))	
#else
        if(g_Input.GetState(DIK_UP, KS_DOWN) || g_Input.GetState(DIK_UP, IKS_DOWN))
#endif//_DH_CHANGE_CHAT_HISTORY
		{
			m_icurrntStrInHistory++;
			if(m_icurrntStrInHistory > (int)(m_vecChatHistory.size() - 1))
				m_icurrntStrInHistory = 0;

			StrnCopy(szMsg, m_vecChatHistory[m_icurrntStrInHistory], MAX_CHATMSG_SIZE);

			m_pChatEditCtrl->SetTextWZ(szMsg);
		}

#ifdef _DH_CHANGE_CHAT_HISTORY
		if(g_Input.GetState(DIK_UP, KS_DOWN) || g_Input.GetState(DIK_UP, IKS_DOWN))
#else
        if(g_Input.GetState(DIK_DOWN, KS_DOWN) || g_Input.GetState(DIK_DOWN, IKS_DOWN))
#endif//
		{
			m_icurrntStrInHistory--;
			if(0 > m_icurrntStrInHistory)
				m_icurrntStrInHistory = m_vecChatHistory.size() - 1;

			StrnCopy(szMsg, m_vecChatHistory[m_icurrntStrInHistory], MAX_CHATMSG_SIZE);

			m_pChatEditCtrl->SetTextWZ(szMsg);
		}
	}
}

void ChatDialog::updateWheelScroll()
{
	if ( m_pChatListCtrl && m_pChatVScrollCtrl)
	{
		int nOffset = 0;
		RECT rc	= m_pChatListCtrl->GetSizeRect();

		switch (GetMouseWheelStatus(&rc))
		{
		case MOUSE_WHEEL_UP:
			nOffset = -(m_iMaxChatMsgLine/3 + 1);
			break; 
		case MOUSE_WHEEL_DOWN:
			nOffset = (m_iMaxChatMsgLine/3 + 1 );
			break;
		}

		rc	= m_pChatVScrollCtrl->GetSizeRect();

		switch (GetMouseWheelStatus(&rc))
		{
		case MOUSE_WHEEL_UP:
			nOffset = -(m_iMaxChatMsgLine/3 + 1);
			break; 
		case MOUSE_WHEEL_DOWN:
			nOffset = (m_iMaxChatMsgLine/3 + 1 );
			break;
		}

		if ( nOffset == 0 )
			return;
				
		nOffset += m_pChatVScrollCtrl->GetScrollPosWZ();
		if ( nOffset < 0 )
			nOffset = 0;
		else
		{
			if ( nOffset > m_VScrollInfo.ScrollRange.iMax )
				nOffset = m_VScrollInfo.ScrollRange.iMax;
		}
		m_pChatVScrollCtrl->SetScrollPosWZ(	nOffset );
	}
}

bool ChatDialog::CheckSlangChatMessage(TCHAR* chat_message)
{
#if defined(_GLOBAL)
    return true;
#endif//defined(_GLOBAL)

    if (g_pHero == NULL)
    {
        SUN_ASSERT(g_pHero);
        return false;
    }
    TCHAR replaced_string[MAX_MESSAGE_LENGTH] = {0, };
    if (GlobalFunc::ReplaceSlangWord(chat_message, replaced_string, MAX_MESSAGE_LENGTH) == true)
    {
    #ifdef _RU_0000_20120227_RUSSIA_APPLY_CHAT_FILTERING
        // 시스템 메시지 출력
        g_InterfaceManager.GetInterfaceString(eST_CANNOT_USE_WORD, chat_message, MAX_MESSAGE_LENGTH);
        AddString(chat_message, CHAT_TYPE_SLANG_FILTERING_NOTICE);

        // 대치된 문자열 복사
        Snprintf(chat_message, MAX_CHATMSG_SIZE, "%s", replaced_string);
    #else
        // 대치된 문자열 출력
        AddString(replaced_string, CHAT_TYPE_SLANG_FILTERING);
        g_InterfaceManager.GetInterfaceString(eST_CANNOT_USE_WORD, replaced_string, MAX_MESSAGE_LENGTH);
        // 시스템 메시지 출력
        AddString(replaced_string, CHAT_TYPE_SLANG_FILTERING_NOTICE);
        return false;
    #endif //_RU_0000_20120227_RUSSIA_APPLY_CHAT_FILTERING
    }
    return true;
}

void ChatDialog::BaseChannelCreate()
{
#if defined(_GLOBAL)
    const int khelpchannel_level = 130;
#else //defined(_RUSSIA)
    const int khelpchannel_level = 100;
#endif//
    if (is_first_enter_ && g_pHero && g_pHero->GetLevel() <= khelpchannel_level)
    {
        SendChatChannelCreate("Help");
        is_first_enter_ = false;
    }                    
}
#pragma warning ( pop )
