#include "SunClientPrecompiledHeader.h"
#include "ChatDialog.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "CommandManager.h"
#include "GlobalFunc.h"
#include "ChatMenu.h"
#include "ChatPersonalOption.h"
#include "HeroActionInput.h"
#include "uiChatMan/uiChatMan_def.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

WzID ChatDialog::m_wzId[CHAT_MAX] = 
{ 
	StrToWzID("L001"),	//CHAT_LIST = 0,
    StrToWzID("LS01"),
	StrToWzID("E001"),	//CHAT_EDIT,	
	StrToWzID("V000"),	//CHAT_VSCR,
	StrToWzID("TI00"),  //CHAT_BUTTON_SELECT_MENU
	StrToWzID("B002"),	//CHAT_MENU_BUTTON
	StrToWzID("B003"),
	StrToWzID("B004"),
	StrToWzID("C100"),	//CHAT_VOICE_CHAT_BUTTON
	StrToWzID("C102"),
	StrToWzID("C103"),
	StrToWzID("C104"),
	StrToWzID("C107"),
	StrToWzID("C105"),
	StrToWzID("C108"),
	StrToWzID("CT00"),
#ifdef	_K2P_ADD_WISPER_WINDOW
	StrToWzID("CT01"),
#endif
    StrToWzID("C000"),  //CHAT_SYSTEM_CHAT_BUTTON
        
};


//----------------------------------------------------------------------------
/**
*/
CControlWZ * 
ChatDialog::getControl( POSTYPE AtPos )
{
	if( AtPos < CHAT_MAX )
		return GetControlWZ( m_wzId[AtPos] );
	else
		return NULL;
}



//----------------------------------------------------------------------------
/**
*/
VOID 
ChatDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
			if( getCtrlIDToPos( pMessage->dwCtrlID ) ==  CHAT_EDIT )
			{
				ChatEdit_MsgProc(pMessage);
			}
		}
		break;

	case RT_MSG_EDIT_TABKEYDOWN:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if( (this->m_eCurChatType == CHAT_TYPE_WHISPER || 
				this->m_eCurChatType == CHAT_TYPE_GM_WHISPER ) && 
				(pos == CHAT_EDIT) )
			{
				m_CurChatEditPos = CHAT_EDIT;

				this->m_pCurEditCtrl = static_cast<CCtrlDxIMEEditBox *>(getControl(m_CurChatEditPos ));
				SetFocusChatEdit();
			}
		}
		break;


		// 포커스 문제로 Click이 제대로 동작하지 않아서 Down 메시지를 받아서 처리
	case RT_MSG_LBUTTONDOWN:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				return;
			}

			switch(pos)
			{
			case CHAT_MENU_BUTTON:
				{
					//TODO: 채팅 속성창 Show

					assert(m_pChatMenuDlg);

					if(m_pChatMenuDlg->IsVisibleWindow())
					{
						m_pChatMenuDlg->ShowWindow(FALSE);
					}
					else
					{
						RECT rcControl = this->getControl(CHAT_MENU_BUTTON)->GetSizeRect();
						RECT rcMenuDlg;
						m_pChatMenuDlg->GetDialogWZ()->GetDlgSize(&rcMenuDlg);

						m_pChatMenuDlg->ShowWindow(TRUE);

						m_pChatMenuDlg->MoveWindowWZ(
							(float)(rcControl.left - 5), 
							(float)(rcControl.top - rcMenuDlg.bottom - 2) );	
					}

				}
				break;

			case CHAT_LIST_SIZE_DOWN:
				{
					if( m_eCurChatControlType > LIST_CONTROL_1X)
					{
						m_eCurChatControlType -=  1;
                        m_iMaxChatMsgLine = SHOW_DEFAULT_CHAT_MSG_LINE;
                        m_pButton[eCHAT_BTN_UP]->ShowWindowWZ(WZ_SHOW);
                        m_pButton[eCHAT_BTN_DOWN]->ShowWindowWZ(WZ_HIDE);
						ChangeChatListControl((eLIST_CONTROL_TYPE) m_eCurChatControlType);
					}
				}
				break;

			case CHAT_LIST_SIZE_UP:
				{
                    if( m_eCurChatControlType < LIST_CONTROL_2X)
					{
						m_eCurChatControlType +=  1;
                        m_iMaxChatMsgLine = SHOW_EXTEND_CHAT_MSG_LINE;
                        m_pButton[eCHAT_BTN_UP]->ShowWindowWZ(WZ_HIDE);
                        m_pButton[eCHAT_BTN_DOWN]->ShowWindowWZ(WZ_SHOW);
						ChangeChatListControl((eLIST_CONTROL_TYPE) m_eCurChatControlType);
					}
				}
				break;
			}

		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			CControlWZ * pControl = getControl(pos);
			if (!pControl)
			{
				this->SetFocusChatEdit();
				return;
			}

			switch (pos)
			{
			case CHAT_LIST:
			case CHAT_LIST_2X:
				{
					this->SetFocusChatEdit();
					AutoFillWhisperNameWhenChatListClick();
				}
				break;

			case CHAT_CHECK_ALL:
				{
					OnNoramlChat();
				}
				break;

			case CHAT_CHECK_PARTY:
				{
					OnPartyChat();
				}
				break;

			case CHAT_CHECK_GUILD:
				{
					OnGuildChat();
				}
				break;

			case CHAT_CHECK_CHANNEL:
				{
					OnChannelChat();
				}
				break;
			case CHAT_CHECK_PROPERTY:
				{
					if( m_pPropertyDlg)
					{
						if( m_pPropertyDlg->IsVisibleWindow() )
						{
							m_pPropertyDlg->ShowWindow(FALSE);
							m_pCheckFilter[eCHECK_PROPERTY]->SetCheckState(FALSE);
						}
						else
						{
                            m_pPropertyDlg->MoveDefaultPosition();
							m_pPropertyDlg->ShowWindow(TRUE);
							m_pCheckFilter[eCHECK_PROPERTY]->SetCheckState(TRUE);
						}
					}
				}
				break;
			case CHAT_CHECK_LOCK_SCROLL:
				{
					m_bLockScroll = !m_bLockScroll;
					m_pCheckFilter[eCHECK_LOCK_SCROLL]->SetCheckState((m_bLockScroll!=FALSE));
					// 이곳에서 m_bLockScroll이 false이면 강제로 refreshfilterlist 를 호출 해준다.
					if( ! m_bLockScroll)
					{
						if ( m_pChatListCtrl )
						{
							m_pChatListCtrl->DeleteItemAll();
						}		
						refreshChatFilterList();
					}
				}
				break;

			case CHAT_CHECK_SYSTEM:
				{
                    OnSystemChat();
				}
				break;
#ifdef _K2P_ADD_WISPER_WINDOW
			case CHAT_CHECK_WISPER:
				{
					OnWisperChar();
				}
				break;
#endif
            case CHAT_SYSTEM_CHAT_BUTTON:
                {
                    BOOL is_show_system_chat = GetShowSystemChat();
                    ShowSystemChatDialog(!is_show_system_chat);
                }
                break;
			}	
		}
		break;
	case RT_MSG_RBUTTONCLICK:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			switch(pos)
			{
			case CHAT_CHECK_SYSTEM:
				{
					ShowSystemChatDialog( TRUE);
				}
				break;

			case CHAT_LIST:
			case CHAT_LIST_2X:
				{
					this->SetFocusChatEdit();
					const int c_iSize = 50;
					TCHAR szResult[c_iSize];
					memset(szResult, 0 , sizeof(szResult));
					int  index = m_pChatListCtrl->GetCurSel();
					TCHAR * pszMessage = (TCHAR * )m_pChatListCtrl->GetItemData(index);
					if(pszMessage)
					{
						ChatPersonalOption* pDlg = GET_CAST_DIALOG(ChatPersonalOption, IM_CHAT_MANAGER::CHAT_PERSONAL_OPTION);
						if(pDlg)
						{
							pDlg->SetClickedCharName(pszMessage);
							pDlg->ShowDialog(TRUE);
						}
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_SB_LINETOP:
		{
			//.윗쪽 화살표를 눌렀다.
			//
			//.아랫쪽 화살표를 눌렀다
			CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
			int pos = pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
			int mmin = 0 , mmax = 0;
			pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

			if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
			{
				pos--;//한줄 감소 시키고 
				pScroll->SetScrollPosWZ(pos);
				//visible line 세팅 
				m_pChatListCtrl->SetBeginDrawIndex( pos  );
			}

			this->SetFocusChatEdit();
		}
		break;
	case RT_MSG_SB_LINEBOTTOM:
		{
			//.아랫쪽 화살표를 눌렀다
			CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
			int pos = pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
			int mmin = 0 , mmax = 0;
			pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

			if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
			{
				pos++;//한줄 증가 시키고 
				pScroll->SetScrollPosWZ(pos);
				//visible line 세팅 
				m_pChatListCtrl->SetBeginDrawIndex( pos  );

			}

			this->SetFocusChatEdit();
		}
		break;

	case RT_MSG_SB_PAGETOP:
		{
			//.윗쪽 몸통을 눌렀다.

			this->SetFocusChatEdit();
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:
		{
			//.아래쪽 몸통을 눌렀다.
			this->SetFocusChatEdit();
		}
		break;
	case RT_MSG_SB_THUMBTRACK:
		{
		}
		break;
	case RT_MSG_SB_THUMBPOSITION:
		{
			//.썸을 드래그한후 마우스를 놓았다.
		}
		break;
	case RT_MSG_SB_ENDSCROLL:
		{
			//.스크롤이 종료 되었다.
		}
		break;

	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );
			m_iTooltipStringCode = getTooltipString(pos);
		}
		break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

void ChatDialog::OnProcessChatMessage( LPTSTR Message )
{
    DWORD dwVal = g_CommandManager.ParseCommand( Message );
    if( 1 != dwVal )//1 == TRUE : 0,1이외 == FALSE
    {
        //	디버그용 명령어 검사( 함수 처음에 스페셜 모드 체크해서 바로 리턴한다 )
        if( g_DebugCommandManager.ParseCommand( Message ) == FALSE )
        {
            int iStrLen = _tcslen(Message);

            BOOL bGMCommand = FALSE;

            if ( iStrLen > 3 && iStrLen < MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH)
            {
                if (Message[0] == '/' && Message[1] == '/')
                {
                    if(GENERALPARAM->GetSpecialMode())
                    {
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                        GuildSystem::Function::CloseGuildWareHouse();
#else
                        // 일부 GM 명령어 사용시, 길드창고가 열려있으면 안되는 경우가 있어서 예외처리함.(by영주)
                        uiGuildMan* pGuildMan = (uiGuildMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
                        if(pGuildMan)
                        {
                            if(pGuildMan->IsGuildWareHouseWindow())
                            {
                                pGuildMan->SEND_CG_GUILD_WAREHOUSE_CLOSE_SYN();
                            }
                        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                    }

                    if(GENERALPARAM->IsNet())
                    {
                        // 금지단어 check
                        if(GlobalFunc::IsSlang(Message, true) == TRUE)
                        {
                            return;
                        }
#ifdef _DEV_VER
                        // <W>로 시작되는건 월드서버 전용 테스트 치트
                        if(Message[2] == '<' && Message[3] == 'W' && Message[4] == '>')
                        {
                            MSG_CW_GM_STRING_CMD_SYN msg_cw;
                            ZeroMemory(msg_cw.m_szStringCmd,MSG_CW_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
                            StrnCopy(msg_cw.m_szStringCmd,&Message[5],MSG_CW_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
                            GlobalFunc::SendPacket(CI_CHATSERVERIDX,&msg_cw,sizeof(msg_cw));
                        }
                        else
#endif//_DEV_VER
                        {
                            MSG_CG_GM_STRING_CMD_SYN msg;
                            ZeroMemory(msg.m_szStringCmd,MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
                            StrnCopy(msg.m_szStringCmd,&Message[2],MSG_CG_GM_STRING_CMD_SYN::MAX_STRING_CMD_LENGTH);
                            GlobalFunc::SendPacket(CI_GAMESERVERIDX,&msg,sizeof(msg));
                        }
                        bGMCommand = TRUE;
                    }

                }
            }

            if (!bGMCommand)
            {
                switch (m_eCurChatType)
                {
                case CHAT_TYPE_GM:
                case CHAT_TYPE_GM_WHISPER:
                case CHAT_TYPE_GENERAL:
                case CHAT_TYPE_TRADE:		// 거래 체팅에 대해서 협의 해보자.
                    SendChatMsg( Message );
                    break;

                case CHAT_TYPE_SHOUT:
                    {
                        BOOL bSend = SendShoutMsg( Message );
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
                            SendGuildMsg( Message, guild_info.m_GuildGuid );
                        }
                        else
                        {
                            // 길드가 존재하지 않다면
                            TCHAR	szMessage[MAX_MESSAGE_LENGTH] = {0,};
                            g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_GUILD, szMessage, MAX_MESSAGE_LENGTH );
                            AddString( szMessage );
                        }
                    }
                    break;

                case CHAT_TYPE_PARTY:
                    {
                        uiPartyMan* partyMan =
                            static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

                        if (partyMan && partyMan->IsParty())
                        {
                            SendChatPartyMsg(Message);
                        }
                        else
                        {
                            // 파티 존재하지 않음
                            TCHAR	szMessage[MAX_MESSAGE_LENGTH] = {0,};
                            g_InterfaceManager.GetInterfaceString( eST_PARTY_NOT_EXIST, szMessage, MAX_MESSAGE_LENGTH );
                            AddString( szMessage );
                        }

                    }
                    break;

                case CHAT_TYPE_CHANNEL:
                    {
                        if( m_bExistChannel )
                        {
                            SendChatChannelMsg( Message );
                        }
                        else
                        {
                            TCHAR	szMessage[MAX_MESSAGE_LENGTH] = {0,};
                            g_InterfaceManager.GetInterfaceString(eST_CHANNEL_NOT_CREATE , szMessage ,MAX_MESSAGE_LENGTH);
                            AddString(szMessage);
                        }
                    }
                    break;

                default:
                    break;

                }	//	switch (m_eCurChatType)

                if((0 != dwVal) && (1 != dwVal))
                {
                    //------------------------------------------------------------------------------
                    // 수정 : define _YMS_CANNOT_EMOTION
                    // 내용 : if 문을 ProcessUserEmoticon() 함수 내로 이동
                    // 사유 : 중복된 코드 삭제
                    //       : ProcessUserEmoticon() 함수 안에서 false 가 될수 있는데 상관없이 
                    //         ProcessUserEmoticon() 내용과 상관없이 stop 패킷을 보내게됨
                    //if(g_pHero->IsMove())
                    //	g_pHero->SendStopMovePacket();
                    //------------------------------------------------------------------------------
                    g_HeroInput.ProcessUserEmoticon( (SLOTCODE)dwVal );
                }
            }
        }
    }

}


//----------------------------------------------------------------------------
/**
*/
VOID			
ChatDialog::ChatEdit_MsgProc( SI_MESSAGE * pMessage )
{

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
			m_bFirstEnter = TRUE;
			CCtrlDxIMEEditBox * pE = (CCtrlDxIMEEditBox *)getControl( CHAT_EDIT );

			if( !pE )
				return;
			if( getCtrlIDToPos( pMessage->dwCtrlID ) ==  CHAT_EDIT )
			{
				pE->GetTextWZ( m_szbuf, MAX_CHATMSG_SIZE );
				memset(m_szResult, 0, sizeof(m_szResult));
				strncpy(m_szResult, m_szbuf, _tcslen(m_szbuf));
				int msgLen = (int)lstrlen( m_szbuf )+1;

				if( 1 == msgLen )		
				{
					if ( !this->m_bAlwaysOnTop )
					{
						g_InterfaceManager.ShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG, FALSE);
					}
					pE->ClearText();
					m_preShow=TRUE;
					return;
				}

				if (GENERALPARAM->GetSpecialMode()) 
				{
					Command(m_szbuf);
				}

				// 채팅 히스토리 저장
				TCHAR* szChatMsg;
				szChatMsg = new TCHAR[MAX_CHATMSG_SIZE];
				memset(szChatMsg, 0, MAX_CHATMSG_SIZE);

				int iVecSize = m_vecChatHistory.size();
				StrnCopy(szChatMsg, m_szResult, MAX_CHATMSG_SIZE);
				if(iVecSize < c_Max_Chat_Save_cnt)
				{
					m_vecChatHistory.push_back(szChatMsg);
				}
				else
				{
                    // assert not empty 
                    TCHAR* temp = m_vecChatHistory.front();
                    SAFE_DELETE_ARRAY(temp);
                    m_vecChatHistory.pop_front();
                    m_vecChatHistory.push_back(szChatMsg);
				}
				m_icurrntStrInHistory = -1;
				// 채팅 히스토리

				//	일반 명령어 인지 검사
				if(checkChatToken(m_szResult))
				{
					if ( !this->m_bAlwaysOnTop )
					{
						m_preShow=TRUE;
						g_InterfaceManager.ShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG, FALSE);
					}
					return;
				}

                OnProcessChatMessage( m_szResult );

				if( GENERALPARAM->IsNet() == FALSE )
				{
					AddString( m_szbuf );
					if ( g_pHero )
					{
						g_pHero->AddChatMessage(m_szbuf);
					}
				}

				//ck: SetText 대신 ClearText로
				pE->ClearText();

				if ( !this->m_bAlwaysOnTop )
				{
					m_preShow=TRUE;
					g_InterfaceManager.ShowDialog(IM_CHAT_MANAGER::CHAT_DIALOG, FALSE);
				}

			}
		}
		break;
	}
}