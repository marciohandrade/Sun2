#include "SunClientPrecompiledHeader.h"


#include "uiEventWindow.h"
#include "uiEventMan/uiEventMan.h"
#include "SunEventInfoParser.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"

const int c_iMaxEventListLine = 13;

//------------------------------------------------------------------------------
WzID uiEventWindow::m_wzId[DIALOG_MAX] = 
{
        StrToWzID("B999"),	//DLGPOS_BTN_EXIT			//종료버튼
		StrToWzID("B001"),	//DLGPOS_BTN_GAME_ITEM		//게임 아이템
		StrToWzID("B002"),	//DLGPOS_BTN_OFFLINE_EVENT	//오프라인 이벤트
		StrToWzID("B003"),	//DLGPOS_BTN_BLANK_EVENT	//아직.. 안 정해짐

		StrToWzID("T999"),	//DLGPOS_TEXT_TITLE			// 제목 텍스트
		StrToWzID("LS01"),
		StrToWzID("VS00"),	//DLGPOS_SCR_VERTICAL		//스크롤바추가				
};

//------------------------------------------------------------------------------
/**
*/
uiEventWindow::uiEventWindow(InterfaceManager *pUIMan) :
    uiBase(pUIMan),
	m_pList(NULL),
    m_bControlsInitialized(false)
{
    this->m_pEventMan =
        static_cast<uiEventMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_EVENT));
    assert (m_pEventMan);

    if (!this->m_pEventMan)
    {
#ifdef _DEBUG
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pEventMan이 없습니다."));

#endif
		this->ShowInterface(FALSE);
        return;
    }
}

//------------------------------------------------------------------------------
/**
*/
uiEventWindow::~uiEventWindow()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiEventWindow::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();

	m_pList = static_cast<CCtrlListWZ*>(getControl( DLGPOS_TEXT_LIST ) );
		
	if(!m_pList) return;

	refreshEventMessage(false);
 
}
//------------------------------------------------------------------------------
/**
*/
void uiEventWindow::refreshEventMessage(bool is_enable_event /* = false */)
{
	if(!m_pList) return;

	TCHAR szMessage[INTERFACE_STRING_LENGTH];// = "안녕하시오.\n추석을 맞아 심신이 지친 반란군 동지들을 위해 달의 정령들과 함께 추석 축제를 준비했다오. 그런데 그만 축제 음식으로 쓸 송편을 몬스터들에게 빼앗겨버렸지 뭐겠소.\n이 탐욕스러운 돼지 같은 놈들!\n축제 시간은 다가오고 음식은 없어서 걱정이라오. 당신이 나를 도와주지 않겠소?\n축제가 시작되기 전에 몬스터들에게 송편을 되찾아 주시오. 송편 1개마다 달의 정령들이 준비한 진기한 아이템들을 선물로 드리리다.";
    
#ifdef _NA_006222_20121031_EVENT_WINDOW_DISTRIBUTE_ENABLE_TEXT
    // 이벤트 비활성화 시 4072,  활성화 시 4071
    int string_id = 4072;
    if (is_enable_event == true)
    {
        string_id = 4071;
    }

	g_InterfaceManager.GetInterfaceString(string_id, szMessage , INTERFACE_STRING_LENGTH);
#else
    g_InterfaceManager.GetInterfaceString(4071, szMessage , INTERFACE_STRING_LENGTH);
#endif //_NA_006222_20121031_EVENT_WINDOW_ADD_DISABLE_TEXT

	BYTE nRowSize = GlobalFunc::AddDescribeString( m_pList, szMessage, StrToWzID("mn12"), DT_LEFT | DT_VCENTER,
		RGBA(255,255,255,0), 
		RGBA(255,255,255,255), 
		RGBA(255,255,255,255) );


	//텍스트내용은 4071코드에 항상 고정적이며 그러기에 init부분에 한번만 사이즈계산을 해주면됩니다.
	int iRangeSize = nRowSize-c_iMaxEventListLine;

	if(m_pScroll && iRangeSize>0)
		m_pScroll->SetScrollRangeWZ(0, iRangeSize);

}
////------------------------------------------------------------------------------
///**	해상도변경등으로 인하여 화면위치조정시 창을 닫는다.
//*/
//void uiEventWindow::UpdatePosition()
//{
//	//SolarDialog::UpdatePosition();
//	
//	if(this->IsVisibleWindow())
//	{
//		this->ShowInterface(FALSE);
//	}
//
//}
//------------------------------------------------------------------------------
/**
*/
void uiEventWindow::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiEventWindow::InitControls()
{
    this->m_bControlsInitialized = true;

	CCtrlButtonWZ* pBtnBlank = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_BLANK_EVENT));
	if(pBtnBlank)
		pBtnBlank->ShowWindowWZ(WZ_HIDE);

	m_pScroll	=	static_cast<CCtrlVScrollWZ *>(this->getControl(DLGPOS_SCR_VERTICAL));
}


//------------------------------------------------------------------------------
void uiEventWindow::OnShowWindow( BOOL val )
{
    if (val)
    {
        //SolarDialog::MoveCenter();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
		CCtrlButtonWZ* pBtnGameItem = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_GAME_ITEM));
		CCtrlButtonWZ* pBtnOffline = static_cast<CCtrlButtonWZ *>(this->getControl(DLGPOS_BTN_OFFLINE_EVENT));
		bool is_enable_event = false;

		if(GetManager())
		{
			if( pBtnGameItem )
			{
				if(GetManager()->IsEnableEvent(eEVENT_REGITEM_RANDREWARD) == true)
				{
					pBtnGameItem->ShowWindowWZ(WZ_SHOW);
                    is_enable_event = true;
				}
				else
				{
					pBtnGameItem->ShowWindowWZ(WZ_HIDE);
				}

			}

			if( pBtnOffline )
			{
				if(GetManager()->IsEnableEvent(eEVENT_REGITEM_GETITEM) == true)
				{
					pBtnOffline->ShowWindowWZ(WZ_SHOW);
                    is_enable_event = true;
				}
				else
				{
					pBtnOffline->ShowWindowWZ(WZ_HIDE);
				}
			}
		
		}

        refreshEventMessage(is_enable_event);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_event_window;
        msg.wParam = InterfaceManager::DIALOG_EVENT_WINDOW;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		if(m_pList)
			m_pList->SetBeginDrawIndex(0);
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_event_window);
    }
}
//----------------------------------------------------------------------------
/** 업데이트 스크롤
*/
void uiEventWindow::UpdateMouseWheel()
{
	if(!m_pList) return;

	RECT rc = m_pList->GetSizeRect();

	switch(GetMouseWheelStatus(&rc))
	{
	case MOUSE_WHEEL_UP: UpScroll(1);break;

	case MOUSE_WHEEL_DOWN:DownScroll(1);break;
	}
}
//----------------------------------------------------------------------------
/** 스크롤다운
*/
void
uiEventWindow::DownScroll(int nRange)
{
	if(!m_pScroll || !m_pList) return;

	int pos = this->m_pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	this->m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

	if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
	{
		( (pos+nRange)>mmax ? (pos=mmax):(pos+=nRange));

		this->m_pScroll->SetScrollPosWZ(pos);

		m_pList->SetBeginDrawIndex(pos);

	}
}
//----------------------------------------------------------------------------
/**	스크롤업
*/
void
uiEventWindow::UpScroll(int nRange)
{
	if(!m_pScroll || !m_pList) return;

	int pos = this->m_pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
	int mmin = 0 , mmax = 0;
	this->m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

	if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
	{
		( (pos-nRange)<mmin ? (pos=mmin):(pos-=nRange));

		this->m_pScroll->SetScrollPosWZ(pos);
		
		m_pList->SetBeginDrawIndex(pos);
	}

}
//------------------------------------------------------------------------------
/**
*/
void uiEventWindow::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_BTN_EXIT:
                {
                    this->ShowInterface(FALSE);
                }
                break;

			case DLGPOS_BTN_GAME_ITEM:
				{
					this->ShowInterface(FALSE);
					GetManager()->ShowEventItemSelectWindow(TRUE, eEVENT_REGITEM_RANDREWARD);
				}
				break;

			case DLGPOS_BTN_OFFLINE_EVENT:
				{
					this->ShowInterface(FALSE);
					GetManager()->ShowEventItemSelectWindow(TRUE, eEVENT_REGITEM_GETITEM);
				}
				break;
            }
        }
        break;
	case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
	case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
		{
			if(m_pList && m_pScroll)
				m_pList->SetBeginDrawIndex(m_pScroll->GetScrollPosWZ());
		}
		break;
	case RT_MSG_SB_PAGETOP:
	case RT_MSG_SB_LINETOP:
		{
			UpScroll(1);
		}
		break;
	case RT_MSG_SB_PAGEBOTTOM:
	case RT_MSG_SB_LINEBOTTOM:
		{
			DownScroll(1);
		}
		break;
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
void uiEventWindow::OnUpdateSolarDialog()
{
    if (!_isEnable())
        return;
	UpdateMouseWheel();
}


//------------------------------------------------------------------------------
/**
*/
BOOL uiEventWindow::_isEnable()
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
/**
*/
void uiEventWindow::NetworkProc( MSG_BASE * pMsg )
{
	
}

