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


		// ��Ŀ�� ������ Click�� ����� �������� �ʾƼ� Down �޽����� �޾Ƽ� ó��
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
					//TODO: ä�� �Ӽ�â Show

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
					// �̰����� m_bLockScroll�� false�̸� ������ refreshfilterlist �� ȣ�� ���ش�.
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
			//.���� ȭ��ǥ�� ������.
			//
			//.�Ʒ��� ȭ��ǥ�� ������
			CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
			int pos = pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
			int mmin = 0 , mmax = 0;
			pScroll->GetScrollRangeWZ( &mmin, &mmax );//��ũ�� ���� �ִ� �ּҸ� ��� �´�.

			if(pos>mmin)// ���� �Էµ� ���μ� ���� ������ 
			{
				pos--;//���� ���� ��Ű�� 
				pScroll->SetScrollPosWZ(pos);
				//visible line ���� 
				m_pChatListCtrl->SetBeginDrawIndex( pos  );
			}

			this->SetFocusChatEdit();
		}
		break;
	case RT_MSG_SB_LINEBOTTOM:
		{
			//.�Ʒ��� ȭ��ǥ�� ������
			CCtrlVScrollWZ * pScroll = (CCtrlVScrollWZ *)getControl( CHAT_VSCR );
			int pos = pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
			int mmin = 0 , mmax = 0;
			pScroll->GetScrollRangeWZ( &mmin, &mmax );//��ũ�� ���� �ִ� �ּҸ� ��� �´�.

			if(pos<mmax)// ���� �Էµ� ���μ� ���� ������ 
			{
				pos++;//���� ���� ��Ű�� 
				pScroll->SetScrollPosWZ(pos);
				//visible line ���� 
				m_pChatListCtrl->SetBeginDrawIndex( pos  );

			}

			this->SetFocusChatEdit();
		}
		break;

	case RT_MSG_SB_PAGETOP:
		{
			//.���� ������ ������.

			this->SetFocusChatEdit();
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:
		{
			//.�Ʒ��� ������ ������.
			this->SetFocusChatEdit();
		}
		break;
	case RT_MSG_SB_THUMBTRACK:
		{
		}
		break;
	case RT_MSG_SB_THUMBPOSITION:
		{
			//.���� �巡������ ���콺�� ���Ҵ�.
		}
		break;
	case RT_MSG_SB_ENDSCROLL:
		{
			//.��ũ���� ���� �Ǿ���.
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
    if( 1 != dwVal )//1 == TRUE : 0,1�̿� == FALSE
    {
        //	����׿� ��ɾ� �˻�( �Լ� ó���� ����� ��� üũ�ؼ� �ٷ� �����Ѵ� )
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
                        // �Ϻ� GM ��ɾ� ����, ���â�� ���������� �ȵǴ� ��찡 �־ ����ó����.(by����)
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
                        // �����ܾ� check
                        if(GlobalFunc::IsSlang(Message, true) == TRUE)
                        {
                            return;
                        }
#ifdef _DEV_VER
                        // <W>�� ���۵Ǵ°� ���弭�� ���� �׽�Ʈ ġƮ
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
                case CHAT_TYPE_TRADE:		// �ŷ� ü�ÿ� ���ؼ� ���� �غ���.
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
                            // ��尡 �����Ѵ�.
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                            GUILD_INFO& guild_info = GuildSystem::GetGuildSystemData().GetGuildInfo();
#else
                            stGUILD_INFO& guild_info = static_cast<stGUILD_INFO>(guildMan->GetGuildInfo());
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                            SendGuildMsg( Message, guild_info.m_GuildGuid );
                        }
                        else
                        {
                            // ��尡 �������� �ʴٸ�
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
                            // ��Ƽ �������� ����
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
                    // ���� : define _YMS_CANNOT_EMOTION
                    // ���� : if ���� ProcessUserEmoticon() �Լ� ���� �̵�
                    // ���� : �ߺ��� �ڵ� ����
                    //       : ProcessUserEmoticon() �Լ� �ȿ��� false �� �ɼ� �ִµ� ������� 
                    //         ProcessUserEmoticon() ����� ������� stop ��Ŷ�� �����Ե�
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

				// ä�� �����丮 ����
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
				// ä�� �����丮

				//	�Ϲ� ��ɾ� ���� �˻�
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

				//ck: SetText ��� ClearText��
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