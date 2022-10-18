#include "SunClientPrecompiledHeader.h"

#include "uiCommunityChat.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "GameConst_Color.h"
#include "interfacemanager.h"

const int c_iFriendChatMsgLine = 5;

//------------------------------------------------------------------------------
WzID uiCommunityChat::m_wzId[COMMUNITY_CHAT_MAX] = 
{
	StrToWzID("L001"),	//COMMUNITY_CHAT_LIST = 0,
	StrToWzID("ED00"),	//COMMUNITY_CHAT_EDIT,	
	StrToWzID("VS00"),	//COMMUNITY_CHAT_VSCR,
	StrToWzID("B999"),  //COMMUNITY_BUTTON_EXIT
	StrToWzID("B001"),  //COMMUNITY_BUTTON_MINIMIZE
	StrToWzID("T000"),  //COMMUNITY_TXT_FRIEND_NAME
};

//------------------------------------------------------------------------------
/**
*/
uiCommunityChat::uiCommunityChat(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false)
{
    this->m_pCommunityMan =
        static_cast<uiCommunityMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_COMMUNITY));
    assert (m_pCommunityMan);

    if (!this->m_pCommunityMan)
    {
#ifdef _DEBUG
		 uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
			if(pSystemMan)
				pSystemMan->Sys_VerifyLock(_T("m_pCommunityMan이 없습니다."));
#endif
        this->ShowInterface(FALSE);
        return;
    }

	ZeroMemory(m_ptszFriendNameTo, sizeof(m_ptszFriendNameTo));
	m_pCommTxtControl = NULL;
	m_pCommChatListCtrl = NULL;
	m_pCommChatVScrollCtrl	= NULL;
	m_pCommChatEditCtrl = NULL;
}

//------------------------------------------------------------------------------
/**
*/
uiCommunityChat::~uiCommunityChat()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityChat::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//----------------------------------------------------------------------------
/**
*/
void uiCommunityChat::InitChatList()
{
	if (!m_pCommChatListCtrl)
	{
		assert(!"왜 리스트가 없어!!");
	}

	m_pCommChatListCtrl->SetUnuseSelBar(FALSE);
	m_pCommChatListCtrl->DeleteItemAll();
	m_pCommChatListCtrl->SetFontID(c_wiCommunityFont);

	m_vstrMsg.clear();
}


//------------------------------------------------------------------------------
/**
*/
void uiCommunityChat::Release()
{
	if(m_pCommChatListCtrl)
	{
		m_pCommChatListCtrl->DeleteItemAll();
	}
	m_vstrMsg.clear();

	if(GetManager())
	{
		GetManager()->DeleteChatDlgId(this->GetDialogWZ()->GetDlgID());
	}	
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityChat::InitControls()
{
    m_bShow = false;

	// 사용안하는 컨트롤: 숨기자
	CControlWZ * pControl = getControl(COMMUNITY_BUTTON_MINIMIZE);
	if(pControl)
		pControl->ShowWindowWZ(WZ_HIDE);


	m_pCommChatListCtrl = static_cast<CCtrlListWZ *>(getControl(COMMUNITY_CHAT_LIST));
	if(m_pCommChatListCtrl)
	{
		m_pCommChatListCtrl->DeleteItemAll();
	}

	// EditBox 설정
	m_pCommChatEditCtrl = static_cast<CCtrlDxIMEEditBox *>(getControl(COMMUNITY_CHAT_EDIT));

	if(m_pCommChatEditCtrl)
	{
		m_pCommChatEditCtrl->SetLimitText(MSG_CW_FRIEND_CHAT_SYN::_MAX_CHATMSG_SIZE-2);
		m_pCommChatEditCtrl->SetColorText( RGBA( 255, 255, 255, 255));
		m_pCommChatEditCtrl->SetColorCaret(c_InterfaceCaretColor);
		m_pCommChatEditCtrl->SetColorSelText( RGBA( 255, 255, 255, 255) );

		m_pCommChatEditCtrl->SetFontID(c_wiCommunityFont);
	}

	m_pCommChatVScrollCtrl = static_cast<CCtrlVScrollWZ *>(getControl(COMMUNITY_CHAT_VSCR));
	assert(m_pCommChatVScrollCtrl);

	CCtrlStaticWZ *m_pCommTxtControl = static_cast<CCtrlStaticWZ *>(getControl(COMMUNITY_TXT_FRIEND_NAME));	
	assert(m_pCommTxtControl);

	if(m_pCommTxtControl)
		GlobalFunc::SetCaption(m_pCommTxtControl, _T(""));
	
	InitChatList();

    this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiCommunityChat::OnShowWindow( BOOL val )
{
    if (val)
    {
        this->MoveDefaultWindowPos();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
        keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType=this->GetDialogWZ()->GetDlgID();
		msg.wParam=this->GetDialogWZ()->GetDlgID();
		msg.DoSomething=GlobalFunc::DoDeleteWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;

		if(m_pCommChatEditCtrl)
		{
			m_pCommChatEditCtrl->ClearText();
		}
	}
    else
    {
		m_bShow = true;
        g_KeyQueueManager.DeleteMsg(this->GetDialogWZ()->GetDlgID());
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityChat::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if(g_pHero && g_pHero->IsShowFullMap())
			return FALSE;

		if(!GetManager())
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityChat::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
	case RT_MSG_EDIT_ENTERKEYDOWN:
		{
			if(!m_pCommChatEditCtrl)
				break;

			if( getCtrlIDToPos( pMessage->dwCtrlID ) ==  COMMUNITY_CHAT_EDIT )
			{
				ZeroMemory(m_szbuf, sizeof(m_szbuf));
				ZeroMemory(m_szResult, sizeof(m_szResult));
				
				m_pCommChatEditCtrl->GetTextWZ( m_szbuf, MAX_COMMUNITY_CHATMSG_SIZE );
				StrnCopy(m_szResult, m_szbuf, _tcslen(m_szbuf));
				int msgLen = (int)_tcslen( m_szbuf )+1;

				// 금지단어 check
				if(GlobalFunc::IsSlang(m_szResult, false) == TRUE)
				{
					//	사용 금지 단어입니다
					g_InterfaceManager.GetInterfaceString( eST_CANNOT_USE_WORD, szMessage, INTERFACE_STRING_LENGTH );
					AddString(szMessage, RGBA(255,0,0,255));
					m_pCommChatEditCtrl->ClearText();

					GlobalFunc::SetFocus( (SolarDialog *)this,  m_pCommChatEditCtrl );
					return;
				}

				//자신의 말은 서버에서 응답이 없음으로 직접 친다.
				if((GetManager()->IsRecvMessage() == FALSE) && _tcslen(m_ptszFriendNameTo))
				{
					if(msgLen>1)
					{
						MSG_CW_FRIEND_CHAT_SYN sync;
						sync.byMsgLength  = _tcslen(m_szbuf);

						ZeroMemory(sync.ptszChatMsg, sizeof(sync.ptszChatMsg));
						ZeroMemory(sync.ptszFriendNameTo, sizeof(sync.ptszFriendNameTo));
						StrnCopy(sync.ptszChatMsg, m_szbuf, sync.byMsgLength);
						StrnCopy(sync.ptszFriendNameTo, m_ptszFriendNameTo, MAX_CHARNAME_LENGTH);
						
						//GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sizeof (sync));	
                        GlobalFunc::SendPacket(CI_CHATSERVERIDX, &sync, sync.GetSize());	
					}
				}
				else
				{
					//	대화 거부 상태입니다.
					g_InterfaceManager.GetInterfaceString( eST_REJECT_CHAT_STATE, szMessage, INTERFACE_STRING_LENGTH );
					AddString(szMessage,RGBA(255,255,255,255));
					m_pCommChatEditCtrl->ClearText();

					GlobalFunc::SetFocus((SolarDialog *)this, m_pCommChatEditCtrl);

					break;
				}

				if(g_pHero && (msgLen>1))
				{
					AddString( g_pHero->GetName(), m_szbuf);				
					m_pCommChatEditCtrl->ClearText();
				}

				GlobalFunc::SetFocus((SolarDialog *)this,  m_pCommChatEditCtrl);
			}
		}
		break;

    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case COMMUNITY_BUTTON_EXIT:
                {
                    m_bShow = false;
                }
                break;

			case COMMUNITY_CHAT_LIST:
				{
					SetFocusChatEdit();
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
			if(m_pCommChatVScrollCtrl)
			{
				int pos = m_pCommChatVScrollCtrl->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pCommChatVScrollCtrl->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
				{
					pos--;//한줄 감소 시키고 
					m_pCommChatVScrollCtrl->SetScrollPosWZ(pos);
				}
			}


			this->SetFocusChatEdit();
		}
		break;

	case RT_MSG_SB_LINEBOTTOM:
		{
			//.아랫쪽 화살표를 눌렀다
			if(m_pCommChatVScrollCtrl)
			{
				int pos = m_pCommChatVScrollCtrl->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pCommChatVScrollCtrl->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.
				
				if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
				{
					pos++;//한줄 증가 시키고 
					m_pCommChatVScrollCtrl->SetScrollPosWZ(pos);
				}

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

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//----------------------------------------------------------------------------
/**
*/
void uiCommunityChat::AddString(LPCTSTR ptszFriendNameFrom, LPCTSTR ptszChatMsg, COLORREF TextColor) 
				
{
	if(GetManager() && GetManager()->IsRecvMessage()==FALSE)
	{
		TCHAR	m_szResult[MAX_COMMUNITY_CHATMSG_SIZE*2] = {0,};
		Snprintf (m_szResult, (MAX_COMMUNITY_CHATMSG_SIZE*2)-1, _T("%s:%s"), ptszFriendNameFrom, ptszChatMsg);

		AddString(m_szResult,TextColor);
	}
}

//----------------------------------------------------------------------------
/**
*/
void uiCommunityChat::AddString(LPCTSTR ptszChatMsg, COLORREF TextColor) 

{
	if(!m_pCommChatListCtrl || !m_pCommChatVScrollCtrl)
		return;

	const int c_iLen = MAX_COMMUNITY_CHATMSG_SIZE*2;
	TCHAR	szWideString[c_iLen] = {0,};
	StrnCopy(szWideString , ptszChatMsg, c_iLen);

	RECT rc = m_pCommChatListCtrl->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
	int iListWidth = rc.right - rc.left - 15;
#else
	int iListWidth = rc.right - rc.left - 5;
#endif	
	m_vstrMsg.clear();
#ifdef _INTERNATIONAL_UI
	GlobalFunc::ProcessWordWrapByFontSize( &m_vstrMsg, szWideString, c_wiCommunityFont, _tcslen(szWideString), iListWidth ,TRUE);
#else
	GlobalFunc::ProcessWordWrapByFontSize( &m_vstrMsg, szWideString, c_wiCommunityFont, _tcslen(szWideString), iListWidth);
#endif//_INTERNATIONAL_UI


	int index = 0;
	for (size_t i=0; i<this->m_vstrMsg.size(); ++i)
	{
		TCHAR *pszStr = const_cast<TCHAR *>(this->m_vstrMsg[i].c_str());

		if (pszStr && _tcslen(pszStr)) 
		{
			///////////////////////////////
			index = m_pCommChatListCtrl->InsertItemBack();
			GlobalFunc::SetItemText( m_pCommChatListCtrl, index, pszStr );
			m_pCommChatListCtrl->SetItemFontID( index,  0, c_wiCommunityFont);

			m_pCommChatListCtrl->SetItemTextColor(
				index, 
				0, 
				TextColor);

			m_pCommChatListCtrl->SetItemSelTextColor(
				index, 
				0, 
				TextColor);

			m_pCommChatListCtrl->SetItemBKColor(
				index, 
				0, 
				RGBA(255,255,255,0)
				);
		}
	}

	// 스크롤 처리
	int iRange = 0;
	if(index > (c_iFriendChatMsgLine-1))
	{
		iRange = index - c_iFriendChatMsgLine + 1;
	}

	m_pCommChatVScrollCtrl->SetScrollRangeWZ(0,iRange);	//스크롤 범위는 입력된 라인수로 한다.
	m_pCommChatVScrollCtrl->SetScrollPosWZ(iRange);
	
	
}

//----------------------------------------------------------------------------
/**
*/
TCHAR* uiCommunityChat::GetCurChatMessage()
{
	if (m_pCommChatEditCtrl)
	{
		m_pCommChatEditCtrl->GetTextWZ(m_szCurChatMessage, MAX_COMMUNITY_CHATMSG_SIZE);
		return m_szCurChatMessage;
	}
	else
	{
		return NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityChat::OnUpdateSolarDialog()
{
	if(!m_bShow)
	{
		ShowDialog(FALSE);
		SetDestroyWindow(TRUE);
		return;
	}

    if (!_isEnable())
        return;

	TCHAR * pszInputMessage = this->GetCurChatMessage();
	// 채팅 중인 지 Check
	BOOL rt = BOOL( _tcslen( this->GetCurChatMessage() ) );

	if(m_pCommChatVScrollCtrl && m_pCommChatListCtrl)
	{
		m_pCommChatListCtrl->SetBeginDrawIndex( m_pCommChatVScrollCtrl->GetScrollPosWZ() );
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityChat::OnRenderSolarDialog()
{
	if (!_isEnable())
        return;

	if(m_pCommTxtControl)
	{
		RECT rc;
		rc = m_pCommTxtControl->GetSizeRect();
		this->GetDrawBase()->x_pManagerTextOut->StartToUseFont( c_wiCommunityFont );
		
        this->GetDrawBase()->x_pManagerTextOut->DrawText(
            m_ptszFriendNameTo,
            &rc,
            WzColor_RGBA(255, 255, 255, 255),
            WzColor_RGBA(0,0,0,0),
            TP_HCENTER);

		this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
	}
	else
	{
		m_pCommTxtControl = static_cast<CCtrlStaticWZ *>(getControl(COMMUNITY_TXT_FRIEND_NAME));
	}

}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityChat::HaveFocus()
{
	if( m_pCommChatEditCtrl )
	{
		if( m_pCommChatEditCtrl->HaveFocus())
		{
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void uiCommunityChat::SetFriendName(TCHAR *pszName)
{
	ZeroMemory(m_ptszFriendNameTo, sizeof(m_ptszFriendNameTo));
	StrnCopy(m_ptszFriendNameTo, pszName, MAX_CHARNAME_LENGTH);
}


//------------------------------------------------------------------------------
/**
*/
BOOL uiCommunityChat::IsHaveFocusInEdit()
{
	return this->m_pCommChatEditCtrl->HaveFocus();
}

//----------------------------------------------------------------------------
/**
*/
void uiCommunityChat::SetFocusChatEdit()
{
	if ( m_pCommChatEditCtrl )
	{
		GlobalFunc::SetFocus( (SolarDialog *)this,  m_pCommChatEditCtrl );
	}
}

