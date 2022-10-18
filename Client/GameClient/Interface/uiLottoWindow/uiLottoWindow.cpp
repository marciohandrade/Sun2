#include "SunClientPrecompiledHeader.h"
#include "uiLottoMan/uiLottoMan.h"
#include "uiLottoNumInput/uiLottoNumInput.h"
#include "uilottowindow.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "ItemManager.h"
#include "SCItemSlot.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include <SCItemSlot.h>
#include "MouseHandler.h"
#include "Mouse.h"
#include "ItemUnitRenderer.h"
#include "ItemManager.h"
#include "SlotKeyGenerator.h"
#include "../uiCashItemShopMan/uiCashItemShopMan.h"

const WzColor		cBKcolor  = WzColor_RGBA( 0,0,0,0 );
const WzColor		cNumcolor = WzColor_RGBA( 213,201,171,255 );
const WzColor		cTXTcolor = WzColor_RGBA( 255,255,255,255 );
const WzColor		cSELcolor = WzColor_RGBA( 255,255,255,255);
const WzID			cLottoFont= StrToWzID("st10");

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
const int kcolumn_number = 3;
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
//------------------------------------------------------------------------------
WzID uiLottoWindow::m_wzId[LOTTO_MAX] = 
{
	StrToWzID("BT01"),//LOTTO_BTN_SLOT(���Թ�ư)
	StrToWzID("B001"),//LOTTO_BTN_EXIT(������)
	StrToWzID("B002"),//LOTTO_BTN_APPLY(�����ϱ�)
	StrToWzID("ST00"),//LOTTO_TXT_INFO(ȸ��÷��ȣ)
	StrToWzID("ST01"),//LOTTO_TXT_DRAWNUM(ȸ��÷��ȣ) 
	StrToWzID("ST06"),//LOTTO_TXT_WINNUM(��÷��ȣ)
	
	StrToWzID("ST02"),//LOTTO_TXT_HEIM_TURN(��ȸ���Ǳ��)
	StrToWzID("ST03"),//LOTTO_TXT_WINNER_TURN(��ȸ��÷�ڼ�)
	StrToWzID("ST04"),//LOTTO_TXT_WINHEIM_TURN(��ȸ��1���÷��)

	StrToWzID("ST07"),//LOTTO_TXT_HEIM(���Ǳ��)
	StrToWzID("ST08"),//LOTTO_TXT_WINNER(��÷�ڼ�)
	StrToWzID("ST09"),//LOTTO_TXT_WINHEIM(1���÷��)
	StrToWzID("LS01"),//LOTTO_LIST_TEXT(����Ʈ����)
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    StrToWzID("V001"),//LOTTO_SCROLL,(��ũ�ѹ�)
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
};

uiLottoWindow::uiLottoWindow(InterfaceManager *pUIMan)
:uiBase(pUIMan),
m_pItemUnitRenderer(NULL),
m_pList(NULL)
{
	memset(&m_pLottoBtn,0,sizeof(m_pLottoBtn));
	memset(&m_pTxtCtrl,0,sizeof(m_pTxtCtrl));
	
	SCItemSlotContainer::Init( MAX_LOTTO_SLOT, SI_LOTTO );
	m_listApplyContainer.clear();
}
//------------------------------------------------------------------------------
/**
*/
uiLottoWindow::~uiLottoWindow(void)
{
	Release();
}
//------------------------------------------------------------------------------
/**
*/
VOID uiLottoWindow::Init( CDrawBase * pDrawBase )
{
	uiBase::Init(pDrawBase);
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    offset_list_ = 0;
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
	InitControls();
}
//------------------------------------------------------------------------------
/**
*/
void uiLottoWindow::InitControls()
{
	int i= 0;
	for(; i<LOTTO_BTN_MAX; ++i)
	{
		m_pLottoBtn[i]	= static_cast<CCtrlButtonWZ *>(getControl(LOTTO_BTN_SLOT+i));
		assert(m_pLottoBtn[i]);
	}

	for(i=0; i<LOTTO_TEXT_MAX; ++i)
	{
		m_pTxtCtrl[i]	= static_cast<CCtrlStaticWZ *>(getControl(LOTTO_TXT_INFO+i));
		assert(m_pTxtCtrl[i]);
	}

	m_pList = static_cast<CCtrlListWZ *>(getControl(LOTTO_LIST_TEXT));
	assert(m_pList);

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    m_pScroll = static_cast<CCtrlVScrollWZ*>(getControl(LOTTO_SCROLL));
    assert(m_pScroll);
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET

	InitUnitRenderer();

	
}
//------------------------------------------------------------------------------
/** 
*/
void uiLottoWindow::InitUnitRenderer()
{
	if (m_pItemUnitRenderer)
	{
		SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	}

	m_pItemUnitRenderer = new ItemUnitRenderer;
	m_pItemUnitRenderer->Init( g_pSunRenderer, MAX_LOTTO_SLOT );

#if defined(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE) || defined(_NA_20101011_HEIM_LOTTO_CASH_TICKET)
    m_pItemUnitRenderer->SetOverLap(FALSE);
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
}
//------------------------------------------------------------------------------
/**	���������� �ڿ��� ���� ����
*/
void uiLottoWindow::InsertApplyInfo(LOTTO_APPLY_DATA& Data,BOOL bTail)
{
	if(bTail)
		m_listApplyContainer.push_back(Data);
	else
		m_listApplyContainer.push_front(Data);
}
//------------------------------------------------------------------------------
/**	���������� �տ��ٰ� �����Ѵ�(�ֽ�������)
*/
void uiLottoWindow::InsertNewApply(WORD wDrawNum,BYTE *arApplyNum)
{
	LOTTO_APPLY_DATA ApplyData;
	memset(&ApplyData,0,sizeof(ApplyData));
	memcpy(&ApplyData.arNumInfo[0],arApplyNum,sizeof(ApplyData.arNumInfo[0]));
	ApplyData.byEntryCnt= 1;
	ApplyData.wDrawNum=wDrawNum;
	InsertApplyInfo(ApplyData);
}
//------------------------------------------------------------------------------
/**	���������� â�� �����ִµ� ������ �ٲ���⶧���� �����ȣ�� ����������Ѵ�.
*/
void uiLottoWindow::InsertLastApply(WORD wDrawNum,BYTE *arApplyNum)
{
		//���λ���
	if(m_listApplyContainer.empty())
	{
		InsertNewApply(wDrawNum,arApplyNum);
	}
	else
	{
       LOTTO_APPLY_DATA  ApplyData = m_listApplyContainer.front();
		
		//true:�������� �����ٿ��� �ٽ��������
		//false:�����ϳ�����
		if(ApplyData.wDrawNum==wDrawNum)
		{
			memcpy(&ApplyData.arNumInfo[ApplyData.byEntryCnt][0],arApplyNum,sizeof(ApplyData.arNumInfo[0]));
			m_listApplyContainer.pop_front();
			++ApplyData.byEntryCnt;
			m_listApplyContainer.push_front(ApplyData);
		}
		else
		{
			InsertNewApply(wDrawNum,arApplyNum);
		}
	}

	//�������ش�.
	SetApplyInfo();
}
//------------------------------------------------------------------------------
/**	�������� ����
*/
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
void uiLottoWindow::SetApplyInfo()
{
	if(!m_pList) return;

	m_pList->DeleteItemAll();
	m_pList->SetSellHeight(18);

	static int TurnNumWidth = 40;
	static int ApplyInfoWidth = 500;
	
	m_pList->SetUseReport(true);
	m_pList->SetReportUseCnt(2); 
	m_pList->SetHideReportRender(true);
	//2���� Row������(Ƚ��:��ȣ��)
	GlobalFunc::SetReportButtonInfo( m_pList, 0, TurnNumWidth, _T("Trun"));
	GlobalFunc::SetReportButtonInfo( m_pList, 1, ApplyInfoWidth, _T("Apply"));

	if(m_listApplyContainer.empty()) return;

	TCHAR szTurnNum[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szInfo[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szApplyNum[INTERFACE_STRING_LENGTH] = {0,};
		
	//��ȸ��:
	g_InterfaceManager.GetInterfaceString(1893, szTurnNum);

	//�����ȣ(<0-0-0-0>)
	g_InterfaceManager.GetInterfaceString(1898, szApplyNum);

	std::list<LOTTO_APPLY_DATA>::iterator itr =	m_listApplyContainer.begin();

    int count = 0;
    int index = 0;
	//���������� �����Ѵ�.
	for(;itr!=m_listApplyContainer.end();++itr)
	{
		LOTTO_APPLY_DATA Info = (*itr);
        if (count == 0)
        {
            index = m_pList->InsertItemBack();
        }

        if (count >= offset_list_)
        {
		    //��ȸ��
		    Snprintf(szInfo, INTERFACE_STRING_LENGTH-1, szTurnNum, Info.wDrawNum);
		    GlobalFunc::SetItem(m_pList, index, 0, szInfo, cLottoFont,DT_VCENTER | DT_LEFT,cBKcolor,cNumcolor,cSELcolor);
        }
		
		//ȸ���� �����ȣ �κ�
		BYTE byEntryCnt  = Info.byEntryCnt;
		ZeroMemory(szInfo,sizeof(szInfo));

		for(BYTE a=0;a<byEntryCnt;++a)//for(BYTE a=0;a<byEntryCnt;++a)
		{
			//���𰹼��� 30���� ����
			if(a>=LOTTO_MAX_TICKETS) 
            {
                break;
            }
			_snprintf(szTemp, INTERFACE_STRING_LENGTH-1, szApplyNum, 
					Info.arNumInfo[a][0], //1�ڸ�
					Info.arNumInfo[a][1], //2�ڸ�
					Info.arNumInfo[a][2], //3�ڸ�
					Info.arNumInfo[a][3]);//4�ڸ�
			strcat(szInfo, szTemp);

            if(a%kcolumn_number==(kcolumn_number-1))
            {
                if (count > offset_list_)
                {
                    GlobalFunc::SetItem(m_pList, index, 1, szInfo, cLottoFont,DT_VCENTER | DT_LEFT,cBKcolor,cTXTcolor,cSELcolor);
                    index = m_pList->InsertItemBack();
                    ZeroMemory(szInfo,sizeof(szInfo));
                }
                else
                {
                    ZeroMemory(szInfo,sizeof(szInfo));
                }
            }
            ++count;
		}
        if (count > offset_list_)
        {
		    GlobalFunc::SetItem(m_pList, index, 1, szInfo, cLottoFont,DT_VCENTER | DT_LEFT,cBKcolor,cTXTcolor,cSELcolor);
            ZeroMemory(szInfo,sizeof(szInfo));
        }
        if (count%kcolumn_number != 0) // ���� ī��Ʈ�� �����ش�
        {
            count = count + kcolumn_number - (count%kcolumn_number);
            if (count > offset_list_)
            {
                index = m_pList->InsertItemBack();
            }
        }
	}
    if (m_pScroll)
    {
        m_pScroll->SetScrollRangeWZ(0, (count/kcolumn_number)-4);//4 = ����Ʈ�� ���μ�
    }
}
#else
void uiLottoWindow::SetApplyInfo()
{
    if(!m_pList) return;

    m_pList->DeleteItemAll();
    m_pList->SetSellHeight(18);

    static int TurnNumWidth = 40;
    static int ApplyInfoWidth = 500;

    m_pList->SetUseReport(true);
    m_pList->SetReportUseCnt(2); 
    m_pList->SetHideReportRender(true);
    //2���� Row������(Ƚ��:��ȣ��)
    GlobalFunc::SetReportButtonInfo( m_pList, 0, TurnNumWidth, _T("Trun"));
    GlobalFunc::SetReportButtonInfo( m_pList, 1, ApplyInfoWidth, _T("Apply"));

    if(m_listApplyContainer.empty()) return;

    TCHAR szTurnNum[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szInfo[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szApplyNum[INTERFACE_STRING_LENGTH] = {0,};

    //��ȸ��:
    g_InterfaceManager.GetInterfaceString(1893, szTurnNum);

    //�����ȣ(<0-0-0-0>)
    g_InterfaceManager.GetInterfaceString(1898, szApplyNum);

    std::list<LOTTO_APPLY_DATA>::iterator itr =	m_listApplyContainer.begin();

    //���������� �����Ѵ�.
    for(;itr!=m_listApplyContainer.end();++itr)
    {
        LOTTO_APPLY_DATA Info = (*itr);
        int index = m_pList->InsertItemBack();

        //��ȸ��
        Snprintf(szInfo, INTERFACE_STRING_LENGTH-1, szTurnNum, Info.wDrawNum);
        GlobalFunc::SetItem(m_pList, index, 0, szInfo, cLottoFont,DT_VCENTER | DT_LEFT,cBKcolor,cNumcolor,cSELcolor);


        //ȸ���� �����ȣ �κ�
        BYTE byEntryCnt  = Info.byEntryCnt;
        ZeroMemory(szInfo,sizeof(szInfo));

        for(BYTE a=0;a<byEntryCnt;++a)
        {
            //���𰹼��� 5���� ����
            if(a>=LOTTO_MAX_TICKETS) continue;

            //����Ʈ ������(3)
            if(a==3)
            {
                GlobalFunc::SetItem(m_pList, index, 1, szInfo, cLottoFont,DT_VCENTER | DT_LEFT,cBKcolor,cTXTcolor,cSELcolor);
                index = m_pList->InsertItemBack();
                ZeroMemory(szInfo,sizeof(szInfo));
            }

            _snprintf(szTemp, INTERFACE_STRING_LENGTH-1, szApplyNum, 
                Info.arNumInfo[a][0], //1�ڸ�
                Info.arNumInfo[a][1], //2�ڸ�
                Info.arNumInfo[a][2], //3�ڸ�
                Info.arNumInfo[a][3]);//4�ڸ�

            strcat(szInfo, szTemp);

        }
        GlobalFunc::SetItem(m_pList, index, 1, szInfo, cLottoFont,DT_VCENTER | DT_LEFT,cBKcolor,cTXTcolor,cSELcolor);
    }
}
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
//------------------------------------------------------------------------------
/** 
*/
void uiLottoWindow::ClearAllInLottoInfo()
{
	m_listApplyContainer.clear();
	LOTTO_BASE_INFO BaseInfo;
	memset(&BaseInfo,0,sizeof(BaseInfo));
	SetLottoBaseInfo(BaseInfo);
	SetItemNum(0);
	
}	
//------------------------------------------------------------------------------
/** �ζ�������ºҰ�(��ư�׸޽����޸�����)
*/
void uiLottoWindow::SetEnableWindow(bool bFlag)
{
	if(m_pLottoBtn[LOTTO_BTN_APPLY])
	{
		m_pLottoBtn[LOTTO_BTN_APPLY]->EnableWindowWZ(bFlag);
		TCHAR szInfo[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
					
		//�ζ� ��÷0ȸ��
		if(bFlag)
		{
			g_InterfaceManager.GetInterfaceString(1886, szInfo);
		}
		else
		{
			g_InterfaceManager.GetInterfaceString(5286, szTemp);
			
			uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
			
			if(pLottoMan)
			{
				Snprintf(szInfo, INTERFACE_STRING_LENGTH-1, szTemp,pLottoMan->GetLottoInfo()->m_wLottoDrawNum);
			}
		}
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_INFO], szInfo);
	}
}	
//------------------------------------------------------------------------------
/** �ζǱ⺻������ ���� UI�� �����Ѵ�.
*/
void uiLottoWindow::SetLottoBaseInfo(LOTTO_BASE_INFO &rInfo)
{
	TCHAR szInfo[INTERFACE_STRING_LENGTH] = {0,};	
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};	
	TCHAR szHeim[INTERFACE_STRING_LENGTH] = {0,};

	//����
	g_InterfaceManager.GetInterfaceString(999, szHeim);

	//=====================================
	// 1 ȸ�� ��÷��ȣ: 0-0-0-0
	//=====================================
	//1 ȸ�� ��÷��ȣ
	g_InterfaceManager.GetInterfaceString(1888, szTemp);
	Snprintf(szInfo, INTERFACE_STRING_LENGTH-1, szTemp,rInfo.wDrawNum);
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_DRAWNUM], szInfo);
	
	//0-0-0-0
	Snprintf(szInfo, INTERFACE_STRING_LENGTH-1, _T("%02d-%02d-%02d-%02d"),rInfo.arLuckyNum[0],rInfo.arLuckyNum[1],rInfo.arLuckyNum[2],rInfo.arLuckyNum[3]);
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNUM], szInfo);
	
	//=====================================
	// 1 ȸ�� ���Ǳ��: 1000����
	//=====================================
	uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
	if(pLottoMan)
	{
		//1 ȸ�� ���Ǳ��
		g_InterfaceManager.GetInterfaceString(1887, szTemp);
		Snprintf(szInfo,INTERFACE_STRING_LENGTH-1,szTemp,pLottoMan->GetLottoInfo()->m_wLottoDrawNum);
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_HEIM_TURN], szInfo);

		//1000����
        GlobalFunc::SetMoneyText(m_pTxtCtrl[IDX_LOTTO_HEIM], rInfo.LottoHeim, CASH_ITEM_SHOP_HAIM, false);
	}

	//=====================================
	// 1 ȸ�� ��÷�ڼ�: 1000��
	//=====================================
	//1ȸ�� ��÷�ڼ�
	g_InterfaceManager.GetInterfaceString(1897, szTemp);
	Snprintf(szInfo,INTERFACE_STRING_LENGTH-1,szTemp,rInfo.wDrawNum);
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNER_TURN], szInfo);
	
	//1000��
	Snprintf(szInfo,INTERFACE_STRING_LENGTH-1,_T("%d"),rInfo.dwWinnerNum);
	g_InterfaceManager.GetInterfaceString(611, szTemp);
	_tcscat( szInfo, szTemp );
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNER], szInfo);

	
	//=====================================
	// 1 ȸ�� 1���÷��: 1000����
	//=====================================
	//1 ȸ�� 1���÷��
	g_InterfaceManager.GetInterfaceString(1934, szTemp);
	Snprintf(szInfo,INTERFACE_STRING_LENGTH-1,szTemp,rInfo.wDrawNum);
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINHEIM_TURN], szInfo);

	//1000����
    GlobalFunc::SetMoneyText(m_pTxtCtrl[IDX_LOTTO_WINHEIM], rInfo.WinnerHeim, CASH_ITEM_SHOP_HAIM, false);

	//=====================================
	// 0ȸ���϶��� ����ó���Ұ͵�
	// (��÷��ȣ.��÷�ڼ�.��÷�ݾ�)
	//=====================================
	if(rInfo.wDrawNum<1)
	{
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_DRAWNUM], _T(" "));			//0ȸ��
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNUM], _T(" "));			//0��÷��ȣ 0-0-0-0

		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNER_TURN], _T(" "));		//1�� ��÷�ڼ�(txt)
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNER], _T(" "));			//1�� ��÷�ڼ�

		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINHEIM_TURN], _T(" "));	//1�� ��÷�ݾ�(txt)
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINHEIM], _T(" "));			//1�� ��÷�ݾ�
	}

}
//------------------------------------------------------------------------------
/** 
*/
void uiLottoWindow::RefreshLottoSlot()
{
	ItemUnitRenderer::RenderUnit *	pRender = NULL;
	RECT				rcSlot;
	CControlWZ *		pCtrl = NULL;

	for(int i=0;i<GetMaxSlotNum();++i)
	{
		SCSlot & rSlot = SCSlotContainer::GetSlot(i);

		if(!IsEmpty(i))
		{
			if(!m_pItemUnitRenderer) continue;
			pRender = m_pItemUnitRenderer->GetItemUnit( rSlot.GetSerial() );
			if (pRender == NULL)
			{
				pCtrl = getControl( i );
				rcSlot = pCtrl->GetSizeRect();
				m_pItemUnitRenderer->AddItemUnit( rSlot.GetSerial(), rSlot.GetCode(), &rcSlot, &rSlot ,pCtrl);
			}
            
            if (pRender)
            {
			    pRender->SetShow(TRUE);
            }
		}
	}
}
//------------------------------------------------------------------------------
/** 
*/
void uiLottoWindow::OnShowWindow( BOOL val )
{
	if(val)
	{
		//uiBase::MoveCenter();
		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_Dialog_Lotto_Window_Dlg;
		msg.wParam = IM_LOTTO_MANAGER::LOTTO_WINDOW_DLG;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
		ClearAllInLottoInfo();
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_Dialog_Lotto_Window_Dlg);
		
	}
}
//-----------------------------------------------------------------------------
/** 
*/
void uiLottoWindow::renderToolTip()
{
	if ( this->m_MouseOverObject.eMouseOverType != eMOUSE_OVER_MAX)
	{
		if ( eMOUSE_OVER_SLOT == m_MouseOverObject.eMouseOverType )
		{
			uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);

			if(pUIMan && m_MouseOverObject.pSlot)
			{
				pUIMan->InitTooltip();
				pUIMan->RegisterSlotInfoTooltip(static_cast<SCItemSlot*>( m_MouseOverObject.pSlot ));
				pUIMan->SetTooltipDialogID(GetDialogKey());
			}
		}
	}

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	BOOL bMouseOver = FALSE;

	CControlWZ * pControl = getControl(LOTTO_BTN_SLOT);

	if(pControl && bMouseOver==FALSE)
	{
		RECT rc = pControl->GetSizeRect();

		if (!IsEmpty(0))
		{
			g_InterfaceManager.RenderActivateMark( rc.left - 13, rc.top - 13);
		}

		if (rc.left <= iMouseX && rc.right >= iMouseX)
		{
			if (rc.top <= iMouseY && rc.bottom >= iMouseY)
			{
				bMouseOver = TRUE;
			}
		}
	}

	if ( !bMouseOver )
	{
		m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
		return;
	}
}
//-----------------------------------------------------------------------------
/** �޽��� PROC
*/
VOID uiLottoWindow::MessageProc( SI_MESSAGE * pMessage )
{
	//��ưŬ��ó������
	switch ( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch(getCtrlIDToPos( pMessage->dwCtrlID ))
			{
			case LOTTO_BTN_EXIT:
				{
					this->ShowInterface(FALSE);
				}
				break;
			case LOTTO_BTN_APPLY:
				{
					//���� �ζ����������� ��û�Ѵ�
					uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);

					if(!pLottoMan) return;
#if defined(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE) || defined(_NA_20101011_HEIM_LOTTO_CASH_TICKET)
                    //�߱���û����:���ǿ���üũ�� �ʿ䰡���� / �۷ι��� ����üũ ����
                    uiLottoNumInput* pLottoNumInput= GET_CAST_DIALOG( uiLottoNumInput, IM_LOTTO_MANAGER::LOTTO_NUMINPUT_DLG );

                    if(pLottoNumInput)
                    {
                        pLottoNumInput->ShowInterface(TRUE);
                    }
#else
					if(pLottoMan->GetLottoInfo()->m_bNumTickets>0)
					{
						uiLottoNumInput* pLottoNumInput= GET_CAST_DIALOG( uiLottoNumInput, IM_LOTTO_MANAGER::LOTTO_NUMINPUT_DLG );

						if(pLottoNumInput)
						{
							pLottoNumInput->ShowInterface(TRUE);
						}
					}
					else
					{
						//5289	�ζǺ����� �����ϴ�.
						TCHAR	szMessage[INTERFACE_STRING_LENGTH];
						g_InterfaceManager.GetInterfaceString(5289,szMessage);
						pLottoMan->LottoMessage(szMessage);		
					}
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
				}
				break;
			}
		}
		break;
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    case RT_MSG_SB_LINETOP:
        {
            UpScroll(1);
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            DownScroll(1);
        }
        break;
    case RT_MSG_SB_THUMBTRACK:				//. ���� �巡�� �ϰ� �ִ�.
    case RT_MSG_SB_THUMBPOSITION:			//. ���� �巡������ ���콺�� ���Ҵ�.
    case RT_MSG_SB_ENDSCROLL:				//. ��ũ���� ���� �Ǿ���.
        {
            if(m_pScroll)
            {
                int pos = m_pScroll->GetScrollPosWZ();//���� ��ũ�� ���� �޾� �´�.
                offset_list_ = pos * kcolumn_number;
                SetApplyInfo();
            }
        }
        break;
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
	}

}
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
//----------------------------------------------------------------------------
/**
*/
void uiLottoWindow::DownScroll(int nRange)
{
    if (m_pScroll == NULL)
    {
        return;
    }
    int pos = this->m_pScroll->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
    int mmin = 0 , mmax = 0;
    this->m_pScroll->GetScrollRangeWZ(&mmin, &mmax);  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

    if(pos<mmax)    // ���� �Էµ� ���μ� ���� ������ 
    {
        ((pos+nRange)>mmax ? (pos=mmax):(pos+=nRange));
        this->m_pScroll->SetScrollPosWZ(pos);
        offset_list_ = pos * kcolumn_number;
        SetApplyInfo();
    }
}
//----------------------------------------------------------------------------
/**
*/
void uiLottoWindow::UpScroll(int nRange)
{
    if (m_pScroll == NULL)
    {
        return;
    }
    int pos = this->m_pScroll->GetScrollPosWZ();        // ���� ��ũ�� ���� �޾� �´�.
    int mmin = 0 , mmax = 0;
    this->m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // ��ũ�� ���� �ִ� �ּҸ� ��� �´�.

    if(pos>mmin)    // ���� �Էµ� ���μ� ���� ������ 
    {
        ((pos-nRange)<mmin ? (pos=mmin):(pos-=nRange));
        this->m_pScroll->SetScrollPosWZ(pos);
        offset_list_ = pos * kcolumn_number;
        SetApplyInfo();
    }
}
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
//----------------------------------------------------------------------------
/** �޽���ó��
*/
VOID			
uiLottoWindow::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos < this->GetMaxSlotNum() && pos >= 0 && pos < MAX_LOTTO_SLOT)
			{
				if (!IsEmpty( pos ))
				{
					SCSlot & rSlot = GetSlot( pos );
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_SLOT;
					m_MouseOverObject.pSlot = &rSlot;
				}
				else
				{
					m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
				}
			}
			else
			{
				m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
			}			

		}
		break;
	}
}
//----------------------------------------------------------------------------
/** �������� �׻�������̴�.
*/
void uiLottoWindow::SetItemNum(int iNum)
{
	//��罽���� ����� �����Ѵ�.
	for (int i = 0; i <GetMaxSlotNum(); ++i)
	{
		if( !IsEmpty(i) )
		{
			DeleteSlot(i, NULL);
		}
	}

#if defined(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE) || defined(_NA_20101011_HEIM_LOTTO_CASH_TICKET)
    //no operation
#else
    //������ �ȵǾ��ٸ� ����
    if(iNum==0) return;   
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

    //�����ֱ����� �ڵ�
    SCItemSlot ItemSlot;
    ItemSlot.SetSlotType(ST_ITEM);
    ItemSlot.SetCode(7292);
    ItemSlot.SetNum(iNum);
    this->InsertSlot( 0, ItemSlot);
    RefreshLottoSlot();
}
//----------------------------------------------------------------------------
/** 
*/
RC::eSLOT_INSERT_RESULT 
uiLottoWindow::InsertSlot(POSTYPE AtPos, SCSlot& rSlot)
{
    //slot_safe_code:������ ����üũ ����(����)
    if (ValidPos(AtPos) == false)
    {
        return RC_INSERT_SLOT_FAILED;
    }

	// ���Կ� ����
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	//��ü �ø���
	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	CControlWZ * pCtrl = getControl(AtPos);
    //��Ʈ���� ���� ��쿡 ���� ��ȣ�ڵ�
    //���Կ� ������ ����.
    if(!pCtrl)
    {
        return RC_INSERT_SLOT_FAILED;
    }

	RECT rect = pCtrl->GetSizeRect();

	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->AddItemUnit( rSetSlot.GetSerial(), rSetSlot.GetCode(), &rect, &rSetSlot, pCtrl);
	}
	return eInsertSlotResult;
}
//----------------------------------------------------------------------------
/** ��������
*/
BOOL 
uiLottoWindow::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
    //slot_safe_code: ������ ����üũ ����(����)
    if ((ValidPos(AtPos) == false) ||
        IsEmpty(AtPos))
    {
        return FALSE;
    }


	SCItemSlot & rItemSlot = (SCItemSlot & )GetSlot(AtPos);

	if (m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->RemoveItemUnit( rItemSlot.GetSerial() );
	}

	g_SlotKeyGenerator.Restore((DWORD)rItemSlot.GetSerial());

	SCSlotContainer::DeleteSlot(AtPos, pSlotOut );

	return TRUE;

}
//------------------------------------------------------------------------------
/**
*/
void uiLottoWindow::OnUpdateSolarDialog()
{
	if(m_pItemUnitRenderer)
	{
		m_pItemUnitRenderer->Update();
	}
	
	uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);

	if(pLottoMan && pLottoMan->CheckDistanceNPC())
	{
		this->ShowInterface(FALSE);
	}

}
//------------------------------------------------------------------------------
/**
*/
void uiLottoWindow::OnRenderSolarDialog()
{
	if(m_pItemUnitRenderer)
		m_pItemUnitRenderer->Render(); 

#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    // ��������
#else
	renderToolTip();
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
}
//------------------------------------------------------------------------------
/**
*/
VOID uiLottoWindow::Release()
{
	if (!m_pItemUnitRenderer)
	{
		return;
	}

	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if( !IsEmpty(i) )
		{
			DeleteSlot(i, NULL);
		}
	}

	SAFE_RELEASENDELETE( m_pItemUnitRenderer );
	SCItemSlotContainer::Release();
}
