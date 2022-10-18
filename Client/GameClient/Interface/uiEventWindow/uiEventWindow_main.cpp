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
        StrToWzID("B999"),	//DLGPOS_BTN_EXIT			//�����ư
		StrToWzID("B001"),	//DLGPOS_BTN_GAME_ITEM		//���� ������
		StrToWzID("B002"),	//DLGPOS_BTN_OFFLINE_EVENT	//�������� �̺�Ʈ
		StrToWzID("B003"),	//DLGPOS_BTN_BLANK_EVENT	//����.. �� ������

		StrToWzID("T999"),	//DLGPOS_TEXT_TITLE			// ���� �ؽ�Ʈ
		StrToWzID("LS01"),
		StrToWzID("VS00"),	//DLGPOS_SCR_VERTICAL		//��ũ�ѹ��߰�				
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
			pSystemMan->Sys_VerifyLock(_T("m_pEventMan�� �����ϴ�."));

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

	TCHAR szMessage[INTERFACE_STRING_LENGTH];// = "�ȳ��Ͻÿ�.\n�߼��� �¾� �ɽ��� ��ģ �ݶ��� �������� ���� ���� ���ɵ�� �Բ� �߼� ������ �غ��ߴٿ�. �׷��� �׸� ���� �������� �� ������ ���͵鿡�� ���Ѱܹ����� ���ڼ�.\n�� Ž�彺���� ���� ���� ���!\n���� �ð��� �ٰ����� ������ ��� �����̶��. ����� ���� �������� �ʰڼ�?\n������ ���۵Ǳ� ���� ���͵鿡�� ������ ��ã�� �ֽÿ�. ���� 1������ ���� ���ɵ��� �غ��� ������ �����۵��� ������ �帮����.";
    
#ifdef _NA_006222_20121031_EVENT_WINDOW_DISTRIBUTE_ENABLE_TEXT
    // �̺�Ʈ ��Ȱ��ȭ �� 4072,  Ȱ��ȭ �� 4071
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


	//�ؽ�Ʈ������ 4071�ڵ忡 �׻� �������̸� �׷��⿡ init�κп� �ѹ��� ���������� ���ָ�˴ϴ�.
	int iRangeSize = nRowSize-c_iMaxEventListLine;

	if(m_pScroll && iRangeSize>0)
		m_pScroll->SetScrollRangeWZ(0, iRangeSize);

}
////------------------------------------------------------------------------------
///**	�ػ󵵺�������� ���Ͽ� ȭ����ġ������ â�� �ݴ´�.
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
/** ������Ʈ ��ũ��
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
/** ��ũ�Ѵٿ�
*/
void
uiEventWindow::DownScroll(int nRange)
{
	if(!m_pScroll || !m_pList) return;

	int pos = this->m_pScroll->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
	int mmin = 0 , mmax = 0;
	this->m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

	if(pos<mmax)    // ���� �Էµ� ���μ� ���� ������ 
	{
		( (pos+nRange)>mmax ? (pos=mmax):(pos+=nRange));

		this->m_pScroll->SetScrollPosWZ(pos);

		m_pList->SetBeginDrawIndex(pos);

	}
}
//----------------------------------------------------------------------------
/**	��ũ�Ѿ�
*/
void
uiEventWindow::UpScroll(int nRange)
{
	if(!m_pScroll || !m_pList) return;

	int pos = this->m_pScroll->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
	int mmin = 0 , mmax = 0;
	this->m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

	if(pos>mmin)    // ���� �Էµ� ���μ� ���� ������ 
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
	case RT_MSG_SB_THUMBTRACK:				//. ���� �巡�� �ϰ� �ִ�.
	case RT_MSG_SB_THUMBPOSITION:			//. ���� �巡������ ���콺�� ���Ҵ�.
	case RT_MSG_SB_ENDSCROLL:				//. ��ũ���� ���� �Ǿ���.
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

