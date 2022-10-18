#include "SunClientPrecompiledHeader.h"

#include "uiEventFriendWindow.h"
#include "uiEventMan/uiEventMan.h"
#include "SunEventInfoParser.h"
#include "ItemInfoParser.h"
#include "Hero.h"
#include "globalfunc.h"
#include "interfacemanager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"

static CODETYPE g_dwEventID = 0;

//------------------------------------------------------------------------------
WzID uiEventFriend::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("B999"),	//DLGPOS_BTN_EXIT		// 종료버튼
		StrToWzID("ST00"),	//DLGPOS_TXT_ITEM		// 재료
		StrToWzID("ST01"),	//DLGPOS_TXT_COUNT		// 참가횟수
		StrToWzID("ST02"),	//DLGPOS_TXT_ITEM2		
		StrToWzID("B003"),	//DLGPOS_BTN_OK			// 확인
		StrToWzID("B004"),	//DLGPOS_BTN_CANCEL		// 취소

		StrToWzID("T999"),	//DLGPOS_TXT_TITLE		 // 제목
		StrToWzID("T998"),	//DLGPOS_TXT_MISSION	 // 미션글자
		StrToWzID("L001"),	//DLGPOS_LIST			//리스트

		StrToWzID("T001"),	//DLGPOS_TXT			// 설명
        StrToWzID("VS00"),  //DLGPOS_VSCROLL        // 스크롤바
        StrToWzID("V000"),  //DLGPOS_VSCROLL_COMMENT        // 설명스크롤바

#ifdef _NA_000000_20130114_RENEWER_UI
		StrToWzID("P001"),
		StrToWzID("P004"),
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

};

//------------------------------------------------------------------------------
/**
*/
uiEventFriend::uiEventFriend(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
    m_bControlsInitialized(false),
    MAX_VISIBLE_LIST_NUMBER(5)
{
    this->m_pEventMan =
        static_cast<uiEventMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_EVENT));
    assert (m_pEventMan);

    if (!this->m_pEventMan)
    {
#ifdef _DEBUG

		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("uiEventFriend이 없습니다."));

#endif
		this->ShowInterface(FALSE);
        return;
    }

    search_index_ = 0;
    m_pListControl = NULL;
	m_vecEventList.clear();
    list_scroll_ = NULL;
	
	for(int a=0; a<POS_TXT_SIZE; ++a)
	{
		m_pTxtControl[a] = NULL;
	}
	
}

//------------------------------------------------------------------------------
/**
*/
uiEventFriend::~uiEventFriend()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiEventFriend::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

	this->InitControls();

    RECT rc = {0, };
    CControlWZ* controlwz = getControl(DLGPOS_TXT);
    if (controlwz)
    {
        controlwz->ShowWindowWZ(WZ_HIDE);

        rc = controlwz->GetSizeRect();
    }

    ftext_render().Init(g_pSunRenderer, rc);
    ftext_parser().SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);
    ftext_render().ClearSentence();

    TCHAR message_comment[INTERFACE_STRING_LENGTH] = {0,};
    GetUIMan()->GetInterfaceString(6084, message_comment, INTERFACE_STRING_LENGTH);

    if (ftext_parser().Parser(message_comment, ftext_sentence()) == TRUE)
    {
        ftext_param().eType = FORMAT_TEXT_PARAM_RETURN;
        ftext_param().dwValue = 1; // 1칸 띄워라.

        ftext_sentence().Pushback(ftext_param());

        ftext_render().Append(ftext_sentence());
    }
    ftext_render().convertLineInfo();

    CCtrlVScrollWZ* scrollbar = static_cast<CCtrlVScrollWZ*>(getControl(DLGPOS_VSCROLL_COMMENT));
    if (scrollbar)
    {
        scrollbar->SetScrollPosWZ(0);
        scrollbar->SetScrollRangeWZ(0, ftext_render().GetMaxLine() - ftext_render().GetDrawLineInRect());
    }

}

//------------------------------------------------------------------------------
/**
*/
void uiEventFriend::Release()
{
    ftext_sentence().Release();
    ftext_param().Clear();
    ftext_render().Release();

	m_vecEventList.clear();
}

//------------------------------------------------------------------------------
/**
*/
void uiEventFriend::InitControls()
{
	this->m_pListControl =	static_cast<CCtrlListWZ *>(this->getControl(DLGPOS_LIST));
	assert (this->m_pListControl);
	
	for(int i=0; i<POS_TXT_SIZE; ++i)
	{
		this->m_pTxtControl[i] = static_cast<CCtrlStaticWZ*>(this->getControl(DLGPOS_TXT_ITEM+i));
	}

    this->m_bControlsInitialized = true;

	CCtrlStaticWZ *pTitle = static_cast<CCtrlStaticWZ*>(this->getControl(DLGPOS_TEXT_TITLE));
	if(pTitle)
	{
		pTitle->SetTextID(4007);	// 4007: 친구 초대 이벤트 요원 
	}

    CCtrlStaticWZ* mission_text_ptr = static_cast<CCtrlStaticWZ*>(this->getControl(DLGPOS_TXT_MISSION));
    if (mission_text_ptr)
    {
        mission_text_ptr->ShowWindowWZ(WZ_HIDE);
    }

	CCtrlStaticWZ *pText = static_cast<CCtrlStaticWZ*>(this->getControl(DLGPOS_TXT));
	if(pText)
	{
		pText->SetTextID(6084);		// 6084: 웹사이트를 통해 브라키온 대륙에 새 친구를 초대하세요! 다양한 해택이 여러분들을 기다리고 있습니다.
	}
    
    list_scroll_ = static_cast<CCtrlVScrollWZ *> (this->getControl(DLGPOS_VSCROLL));
	SUN_ASSERT(list_scroll_);
//	_initList();
    if (list_scroll_)
    {
        list_scroll_->SetScrollRangeWZ(0, scroll_value_max_);
        list_scroll_->SetScrollPosWZ(scroll_current_value_);
        m_pListControl->SetBeginDrawIndex(scroll_current_value_);
    }
}
//------------------------------------------------------------------------------
void uiEventFriend::OnShowWindow( BOOL val )
{
    if (val)
    {
        //SolarDialog::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }

		for(int a=0; a<POS_TXT_SIZE; ++a)
		{
			if(m_pTxtControl[a])
				m_pTxtControl[a]->ShowWindowWZ(WZ_HIDE);
		}

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_Dialog_Event_Friend_Dlg;
        msg.wParam = IM_EVENT_MANAGER::EVENT_FRIEND;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    
		m_pListControl->SetCurSel(-1);
		UpdateList();
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_Dialog_Event_Friend_Dlg);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiEventFriend::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szName[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch (getControlIDToPos(pMessage->dwCtrlID))
            {
            case DLGPOS_BTN_EXIT:
                {
					this->ShowInterface(FALSE);
					_initList(); // 초기화
                }
                break;

			case DLGPOS_BTN_OK:
				{					
					int cursel = m_pListControl->GetCurSel();
					if (cursel < 0) 
					{
						// 6058: 보상내역이 없습니다.
						GetUIMan()->GetInterfaceString(6058, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
						return;
					}

					//if(GetManager()->GetEventMode() == eEVENT_FRIENDINVATION_EVENT)
					{
						if(!m_vecEventList.size()) break;

						FRIENDEVENT_INFO_LIST info = m_vecEventList[cursel]; 
						
						MSG_CG_EVENT_INVITATEFRIEND_REWARD_SYN sync;
						sync.FriendGUID = info.FriendGUID;
						sync.ReWardType = (BYTE)info.RewardType;

						if(info.IsReward == 0)	// 보상을 아직 받지 않은 경우
						{
							GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof(sync));
						}
						_initList();	// 보낸후 초기화

						/*uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);*/
						/*if(pSystemMan)
							pSystemMan->Sys_Confirm(szMessage, _Confirm_RequestRegItem);*/
					}
                }
                break;

			case DLGPOS_BTN_CANCEL:
				{
					this->ShowInterface(FALSE);
					_initList(); // 초기화
				}
				break;
        }
        break;
    
    case RT_MSG_SB_LINETOP:
		{
			switch (getControlIDToPos(pMessage->dwCtrlID) )
			{
			case DLGPOS_VSCROLL:
				{
					if (m_pListControl->GetListRowCnt() > MAX_VISIBLE_LIST_NUMBER
						&& scroll_current_value_ > 0)
					{
						scroll_current_value_ -= 1;
						list_scroll_->SetScrollPosWZ(scroll_current_value_);
						m_pListControl->SetBeginDrawIndex(scroll_current_value_);
					}					
				}
				break;
            case DLGPOS_VSCROLL_COMMENT:
                {
                    CCtrlVScrollWZ* scrollbar = static_cast<CCtrlVScrollWZ*>(getControl(DLGPOS_VSCROLL_COMMENT));
                    if (scrollbar)
                    {
                        int index = scrollbar->GetScrollPosWZ() - 1;

                        index = max(0, index);
                        ftext_render().SetBeginLineIndex(index);
                        scrollbar->SetScrollPosWZ(index);
                    }
                }
                break;
			}
		}
		break;

	case RT_MSG_SB_LINEBOTTOM:
		{
			switch ( getControlIDToPos(pMessage->dwCtrlID) )
			{
			case DLGPOS_VSCROLL:
				{
					if (m_pListControl->GetListRowCnt() > MAX_VISIBLE_LIST_NUMBER
						&& scroll_current_value_ < scroll_value_max_)
					{
						scroll_current_value_ += 1;
						list_scroll_->SetScrollPosWZ(scroll_current_value_);
						m_pListControl->SetBeginDrawIndex(scroll_current_value_);
					}
				}
				break;
            case DLGPOS_VSCROLL_COMMENT:
                {
                    CCtrlVScrollWZ* scrollbar = static_cast<CCtrlVScrollWZ*>(getControl(DLGPOS_VSCROLL_COMMENT));
                    if (scrollbar)
                    {
                        int index = scrollbar->GetScrollPosWZ() + 1;

                        index = min(int(ftext_render().GetMaxLine() - ftext_render().GetDrawLineInRect()), index);
                        ftext_render().SetBeginLineIndex(index);
                        scrollbar->SetScrollPosWZ(index);
                    }
                }
                break;
			}
		}
		break;
	case RT_MSG_SB_THUMBPOSITION:
		{
			switch ( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case DLGPOS_VSCROLL:
				{
					scroll_current_value_ = list_scroll_->GetScrollPosWZ();
					m_pListControl->SetBeginDrawIndex(scroll_current_value_);
				}
				break;
            case DLGPOS_VSCROLL_COMMENT:
                {
                    CCtrlVScrollWZ* scrollbar = static_cast<CCtrlVScrollWZ*>(getControl(DLGPOS_VSCROLL_COMMENT));
                    if (scrollbar)
                    {
                        int index = scrollbar->GetScrollPosWZ();

                        ftext_render().SetBeginLineIndex(index);
                    }
                }
                break;
			}
		}
		break;

    }
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiEventFriend::OnUpdateSolarDialog()
{
    if (!_isEnable())
        return;

    UpdateMouseWheel_Comment();
}

//------------------------------------------------------------------------------
/**
*/
void uiEventFriend::OnRenderSolarDialog()
{
    if (!_isEnable())
        return;

    CControlWZ* controlwz = getControl(DLGPOS_TXT);
    if (controlwz)
    {
        ftext_render().SetRect(controlwz->GetSizeRect());
    }

    ftext_render().Render();
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiEventFriend::_isEnable()
{
	if(GENERALPARAM->IsNet())
	{
		if (this->IsLock())
			return FALSE;

		if(!g_pHero)
			return FALSE;

		if( g_pHero && g_pHero->IsShowFullMap() )
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
void uiEventFriend::_initList()
{
	if (!GetManager())
        return;

	m_vecEventList.clear();
}

//------------------------------------------------------------------------------
void uiEventFriend::InsertList(InvitateFriendRewardInfo *pRewardInfo)
{
	if (!GetManager())
        return;
	
	FRIENDEVENT_INFO_LIST pInfo;
	pInfo.FriendGUID = pRewardInfo->InvitateFriendGuid;
	pInfo.IsReward = pRewardInfo->RewardSts;
	sprintf(pInfo.ptszFriendName,  pRewardInfo->CharName);
	pInfo.RewardType = pRewardInfo->Level;
	m_vecEventList.push_back(pInfo);
    search_index_ = m_vecEventList.size();
    scroll_current_value_ = 0;
    scroll_value_max_ = search_index_ - MAX_VISIBLE_LIST_NUMBER;
}
//------------------------------------------------------------------------------
void uiEventFriend::UpdateList()
{
    if (!GetManager())
        return;

	_refreshList();

	// 서버에 요청
	//GetManager()->SendEventFriend();
}

//------------------------------------------------------------------------------
void uiEventFriend::_refreshList()
{
    if (!GetManager())
        return;

	if(NULL == m_pListControl)
		return;

	DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
	DWORD c_dwSelTextColor = RGBA(0,220,0,255);
	DWORD c_dwBackColor = RGBA (0,0,0,0);

	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};
	
	m_pListControl->DeleteItemAll();
	m_pListControl->SetBeginDrawIndex(0);

	int idx = 0;

	for(unsigned int i=0; i<m_vecEventList.size(); ++i)
	{
		FRIENDEVENT_INFO_LIST info = m_vecEventList[i];
		idx = m_pListControl->InsertItemBack();
		
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%-21s Lv: %d") ,m_vecEventList[i].ptszFriendName, m_vecEventList[i].RewardType);
		GlobalFunc::SetItemText( m_pListControl, idx ,szMessage);
		
		m_pListControl->SetItemFontID( idx,  0, StrToWzID("m212"));

		m_pListControl->SetItemTextColor(
				idx, 
				0, 
				c_dwTextColor
				);

		m_pListControl->SetItemBKColor(
				idx, 
				0, 
				c_dwBackColor
				);

		m_pListControl->SetItemSelTextColor(
				idx, 
				0, 
				c_dwSelTextColor
				);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiEventFriend::NetworkProc( MSG_BASE * pMsg )
{
}

//------------------------------------------------------------------------------
void    uiEventFriend::UpdateMouseWheel_Comment()
{
    CCtrlVScrollWZ* scrollbar = static_cast<CCtrlVScrollWZ*>(getControl(DLGPOS_VSCROLL_COMMENT));
    if (scrollbar == NULL)
    {
        return;
    }

    CControlWZ* area = getControl(DLGPOS_TXT);
    if (area == NULL)
    {
        return;
    }

    switch (GetMouseWheelStatus(&(area->GetSizeRect())))
    {
    case MOUSE_WHEEL_UP:
        {
            int index = scrollbar->GetScrollPosWZ() - 1;

            index = max(0, index);
            ftext_render().SetBeginLineIndex(index);
            scrollbar->SetScrollPosWZ(index);
        }
        break;

    case MOUSE_WHEEL_DOWN:
        {
            int index = scrollbar->GetScrollPosWZ() + 1;

            index = min(int(ftext_render().GetMaxLine() - ftext_render().GetDrawLineInRect()), index);
            ftext_render().SetBeginLineIndex(index);
            scrollbar->SetScrollPosWZ(index);
        }
        break;
    }
}
