#include "SunClientPrecompiledHeader.h"

#include "SystemChatDialog.h"
#include "GlobalFunc.h"
#include "ChatDialog.h"
#include "GameConst_Color.h"
#include "Application.h"
#include "QuickContainer.h"
#include "hero.h"
#include "uichatman/uiChatMan.h"

const int   c_iLIMITED_EDIT_SYSTEMTEXT        = 52;

WzID SystemChatDialog::m_wzId[SYSTEMCHAT_MAX] =
{
	StrToWzID("L001"),
	StrToWzID("V000"),
	StrToWzID("B001"),
};



//----------------------------------------------------------------------------
/**
*/
SystemChatDialog::SystemChatDialog(InterfaceManager* pUIMan)
:uiBase(pUIMan)
{
	m_iCurScrollValue = 0;
	m_iMaxScrollValue = 0;
}

//----------------------------------------------------------------------------
/**
*/
SystemChatDialog::~SystemChatDialog()
{

}

//----------------------------------------------------------------------------
/**
*/
VOID SystemChatDialog::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
	RegisterControlPosition(StrToWzID("L001"), SYSTEMCHAT_LIST);
	RegisterControlPosition(StrToWzID("V000"), SYSTEMCHAT_VSCROLL);
	RegisterControlPosition(StrToWzID("B001"), SYSTEMCHAT_BTN_EXIT);
	InitControll();
	//UpdatePosition();
	DisableInterSectRect(m_pChatListCtrl->GetSizeRect());
    m_pChatListCtrl->SetSellHeight(16);
    m_pChatListCtrl->SetStartLineGapTop(5);
    m_pChatListCtrl->SetStartLineGapLeft(8);
    m_iGapOfDialogYPosition = c_iOneLineYPostionToQuickSlot + c_iAddYSizeofSystemChat;
    MoveDefaultPosition();

    m_Rect= m_pChatListCtrl->GetSizeRect();
    m_pChatListCtrl->SetTextKind(ETK_SHADOW);

	VScrollDialog::ResizeingVScrollControl(0, 0);

//    MoveWindowWZ(m_Rect.left,m_Rect.top);		
}

//----------------------------------------------------------------------------
/**
*/
VOID SystemChatDialog::Release()
{
	std::deque<ChatMsg *>::iterator itr;

	itr = m_ChatSystemMsg.begin();
	while ( itr != m_ChatSystemMsg.end())
	{
		ChatMsg * pChatMsg = (ChatMsg *) (*itr);
		if( pChatMsg->m_dwRefCount == 1)
		{
			if( pChatMsg->m_pszCharName )
			{
				delete pChatMsg->m_pszCharName;
			}

			SAFE_DELETE(pChatMsg);
			pChatMsg =NULL;
		}
		else
		{
			pChatMsg->m_dwRefCount -= 1;
		}

		++itr;
	}
	m_ChatSystemMsg.clear();
}
//----------------------------------------------------------------------------
/** 시스템채팅창 영역재설정한다
*/
/*
BOOL SystemChatDialog::InterSectRect( POINT point )
{
	//1.전체영역에 들어왔는지 체크
	if(SolarDialog::InterSectRect(point))
	{
		//2.리스트창은 피킹먹게하자
		RECT rc = m_pChatListCtrl->GetSizeRect();

		if(DEF_IntersectRect(rc,point)==FALSE)
		{
			return TRUE;
		}
	}
	
	return FALSE;
	
}
*/

//----------------------------------------------------------------------------
/**
*/
VOID SystemChatDialog::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_SB_LINETOP:
		{
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case SYSTEMCHAT_VSCROLL:
				{	
					if(m_iCurScrollValue > 0)
					{
						m_iCurScrollValue -= 1;
						m_pChatVScrollCtrl->SetScrollPosWZ(m_iCurScrollValue);
						m_pChatListCtrl->SetBeginDrawIndex(m_iCurScrollValue);
					}
				}
				break;
			}
		}
		break;
	case RT_MSG_SB_LINEBOTTOM:
		{
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case SYSTEMCHAT_VSCROLL:
				{
					if(m_iCurScrollValue < m_iMaxScrollValue )
					{
						m_iCurScrollValue += 1;
						m_pChatVScrollCtrl->SetScrollPosWZ(m_iCurScrollValue);
						m_pChatListCtrl->SetBeginDrawIndex(m_iCurScrollValue);
					}
				}
				break;
			}
		}
		break;
	
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos(pMessage->dwCtrlID) )
			{
			case SYSTEMCHAT_BTN_EXIT:
				{
                    ChatDialog* chat_dialog = 
                        GET_CAST_DIALOG(ChatDialog, IM_CHAT_MANAGER::CHAT_DIALOG);
                    if (chat_dialog != NULL)
                    {
                        chat_dialog->ShowSystemChatDialog(FALSE);
                    }
                    else
                    {
                        this->ShowWindow(FALSE);
                    }                    
				}
				break;
			}
		}
		break;
	default:
		{
			SolarDialog::MessageProc( pMessage );
			VScrollDialog::MessageProc( pMessage );
		}
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID SystemChatDialog::NetworkProc( MSG_BASE * pMsg )
{

}

//----------------------------------------------------------------------------
/**
*/
VOID SystemChatDialog::Process(DWORD dwTick)
{

}

//----------------------------------------------------------------------------
/**
*/
VOID SystemChatDialog::OnUpdateSolarDialog()
{
	VScrollDialog::Update();

	UpdateScrollPosition();
}


//----------------------------------------------------------------------------
/**
*/
VOID SystemChatDialog::OnShowWindow(BOOL val)
{
    //MoveDefaultPosition()
	if( val )
	{
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
		SetGapOfDialogYPosition(c_iTwoLineYPostionToQuickSlot + c_iAddYSizeofSystemChat);
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        MoveDefaultPosition();
		//this->MoveWindowWZ(m_Rect.left,m_Rect.top);		
	}
//	else
//	{
//         m_Rect= m_pChatListCtrl->GetSizeRect();
//	}

}

//----------------------------------------------------------------------------
/**
*/
void SystemChatDialog::InitControll()
{
	m_bInitControll = TRUE;

	m_pChatListCtrl = static_cast<CCtrlListWZ * >(getControl(SYSTEMCHAT_LIST));
	if(!m_pChatListCtrl)
	{
		m_bInitControll = FALSE;
		return;
	}
	m_pChatListCtrl->SetSellHeight(21);

	m_pChatVScrollCtrl = static_cast<CCtrlVScrollWZ * >(getControl(SYSTEMCHAT_VSCROLL));
	if(!m_pChatVScrollCtrl)
	{
		m_bInitControll = FALSE;
		return;
	}
	else
	{
		 m_Rect = m_pChatVScrollCtrl->GetSizeRect();
	}

	

}

//----------------------------------------------------------------------------
/**
*/
BOOL SystemChatDialog::CheckControll()
{
	if(!m_bInitControll)
	{
		 assert ("Unknown controls!!");
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

//----------------------------------------------------------------------------
/**
*/
void SystemChatDialog::AddString(TCHAR * pszMessage)
{
	int size = m_pChatListCtrl->GetListRowCnt();

	std::vector<util::_tstring> m_vstr;

	RECT rtSize = m_pChatListCtrl->GetSize();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
	int Interval = 20;
#else
	int Interval = 10;
#endif	
	GlobalFunc::ProcessWordWrapByFontSize( &m_vstr, const_cast<TCHAR *>(pszMessage), StrToWzID("mn12"), _tcslen (pszMessage) , rtSize.right - Interval );
	
	if ( size >= MAX_LIST_LINE )
	{
		//라인 수만큼 지워준다.
		for (int nCount = m_vstr.size() ; nCount > 0 ; --nCount)
		{
			m_pChatListCtrl->DeleteItem(0);
			--m_iCurScrollValue;
		}		
		if (m_iCurScrollValue < 0)
			m_iCurScrollValue = 0;
	}

	for( unsigned int i =0; i < m_vstr.size(); ++i)
	{
		int index = m_pChatListCtrl->InsertItemBack();
		
		GlobalFunc::SetItem(m_pChatListCtrl,index,0,const_cast<TCHAR *>(m_vstr[i].c_str()),StrToWzID("mn12"), 
			DT_LEFT | DT_VCENTER,
			RGBA(0,0,0,0), 
			GlobalFunc::WzColorToRGBA(c_Chat_SystemColor), 
			GlobalFunc::WzColorToRGBA(c_Chat_SystemColor));
	}
	UpdateScrollBar();
}

void SystemChatDialog::UpdateScrollBar()
{
	m_pChatListCtrl->SetUnuseSelBar(FALSE);
	m_iMaxScrollValue = m_pChatListCtrl->GetListRowCnt() - MAX_SHOW_LINE;

	if(m_iMaxScrollValue <= 0)
	{
		m_iMaxScrollValue = 0;
		m_iCurScrollValue = 0;
		m_pChatListCtrl->SetBeginDrawIndex(m_iCurScrollValue);
	}

	m_pChatVScrollCtrl->SetScrollRangeWZ(0,m_iMaxScrollValue);

	if ( m_iMaxScrollValue - m_iCurScrollValue <= 3 || m_iCurScrollValue > m_iMaxScrollValue)
	{
		m_iCurScrollValue = m_iMaxScrollValue;
	}
	
	m_pChatVScrollCtrl->SetScrollPosWZ(m_iCurScrollValue);		
	m_pChatListCtrl->SetBeginDrawIndex(m_iCurScrollValue);
}

void SystemChatDialog::UpdateScrollPosition()
{
	int total = m_pChatListCtrl->GetListRowCnt();//입력된 총 라인수를 구한다.
	if( total > MAX_SHOW_LINE )
	{
		m_iCurScrollValue = m_pChatVScrollCtrl->GetScrollPosWZ();
	}
	else
	{
		m_iCurScrollValue = 0;
	}
	m_pChatListCtrl->SetBeginDrawIndex( m_iCurScrollValue );
}

void SystemChatDialog::PushBackChatMsg( ChatMsg * pChatMsg )
{
	if (!pChatMsg)
		return;

	int size = m_pChatListCtrl->GetListRowCnt();
	if ( size < MAX_LIST_LINE )
	{
		m_ChatSystemMsg.push_back( pChatMsg );
		pChatMsg->m_dwRefCount += 1;
		AddString( pChatMsg->m_pszMessage );

	}
	else
	{
		std::deque<ChatMsg *>::iterator itr = m_ChatSystemMsg.begin();
		ChatMsg * pBeginMsg = *itr;

		if(pBeginMsg)
		{
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
				pBeginMsg->m_dwRefCount -= 1;
			}
		}

		m_ChatSystemMsg.pop_front();
		m_ChatSystemMsg.push_back( pChatMsg );
		pChatMsg->m_dwRefCount += 1;
		AddString( pChatMsg->m_pszMessage );
	}

}

//----------------------------------------------------------------------------
/**
*/
void SystemChatDialog::MoveDefaultPosition()
{
	POINT pt = g_pApplication->Get2DModeMapping();
	RECT rcClient;
	GetDialogWZ()->GetClientRect(&rcClient);

	POINT MyInitialPoint;
    MyInitialPoint.x = pt.x - (rcClient.right- rcClient.left);//-386;
    MyInitialPoint.y =  pt.y - (rcClient.bottom - rcClient.top + m_iGapOfDialogYPosition) - 4;
	this->MoveWindowWZ((float)MyInitialPoint.x, (float)MyInitialPoint.y);
}

CCtrlVScrollWZ* SystemChatDialog::vscroll_control()
{
	return GetVScrollControlWZ(GetControlID_byPosition(SYSTEMCHAT_VSCROLL));
}

bool SystemChatDialog::GetScrollArea( RECT& area )
{
	if (IsOverControl(GetControlWZ_byPosition(SYSTEMCHAT_LIST)) == true && 
		HaveFocus() == TRUE)
	{
		area = GetControlWZ_byPosition(SYSTEMCHAT_LIST)->GetSizeRect();
		return true;
	}
	return false;
}

bool SystemChatDialog::OnVScrollThumb( SI_MESSAGE* message )
{
	if (VScrollDialog::OnVScrollThumb(message) == true)
	{
		m_pChatListCtrl->SetBeginDrawIndex(m_pChatVScrollCtrl->GetScrollPosWZ());
		return true;
	}
	return false;
}

bool SystemChatDialog::OnVScrollUp( SI_MESSAGE* message )
{
	if (m_iCurScrollValue > 0)
	{
		m_iCurScrollValue -= 1;
		m_pChatVScrollCtrl->SetScrollPosWZ(m_iCurScrollValue);
		m_pChatListCtrl->SetBeginDrawIndex(m_iCurScrollValue);
		return true;
	}

	return false;
}

bool SystemChatDialog::OnVScrollDown( SI_MESSAGE* message )
{
	if (m_iCurScrollValue < m_iMaxScrollValue)
	{
		m_iCurScrollValue += 1;
		m_pChatVScrollCtrl->SetScrollPosWZ(m_iCurScrollValue);
		m_pChatListCtrl->SetBeginDrawIndex(m_iCurScrollValue);
		return true;
	}

	return false;
}

//void SystemChatDialog::UpdatePosition()
//{
////	// 장착 창 뜰때 포지션 지정해준다.
////	// 스킵~
////    if (g_pHero == NULL)
////    {
////        return;
////    }
////
////    QuickContainer* pQuickContainer = g_pHero->GetQuickContainer();
////	if (pQuickContainer)
////    {
////        uiChatMan* puiChatMap = GET_DIALOG_MANAGER(uiChatMan, UIMAN_CHAT);
////        if (puiChatMap == NULL)
////        {
////            return;
////        }
////        if (pQuickContainer->double_size() == true)
////        {
////            puiChatMap->MoveChatDialog(2);
////        }
////        else
////        {
////            puiChatMap->MoveChatDialog(1);
////        }
////    }
//}
