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
	StrToWzID("BT01"),//LOTTO_BTN_SLOT(슬롯버튼)
	StrToWzID("B001"),//LOTTO_BTN_EXIT(나가기)
	StrToWzID("B002"),//LOTTO_BTN_APPLY(응모하기)
	StrToWzID("ST00"),//LOTTO_TXT_INFO(회당첨번호)
	StrToWzID("ST01"),//LOTTO_TXT_DRAWNUM(회당첨번호) 
	StrToWzID("ST06"),//LOTTO_TXT_WINNUM(당첨번호)
	
	StrToWzID("ST02"),//LOTTO_TXT_HEIM_TURN(몇회복권기금)
	StrToWzID("ST03"),//LOTTO_TXT_WINNER_TURN(몇회당첨자수)
	StrToWzID("ST04"),//LOTTO_TXT_WINHEIM_TURN(몇회당1등당첨금)

	StrToWzID("ST07"),//LOTTO_TXT_HEIM(복권기금)
	StrToWzID("ST08"),//LOTTO_TXT_WINNER(당첨자수)
	StrToWzID("ST09"),//LOTTO_TXT_WINHEIM(1등당첨금)
	StrToWzID("LS01"),//LOTTO_LIST_TEXT(리스트내용)
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    StrToWzID("V001"),//LOTTO_SCROLL,(스크롤바)
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
/**	새응모정보 뒤에다 새로 삽입
*/
void uiLottoWindow::InsertApplyInfo(LOTTO_APPLY_DATA& Data,BOOL bTail)
{
	if(bTail)
		m_listApplyContainer.push_back(Data);
	else
		m_listApplyContainer.push_front(Data);
}
//------------------------------------------------------------------------------
/**	새응모정보 앞에다가 삽입한다(최신정보앞)
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
/**	응모했을때 창이 열렬있는데 정보가 바뀌었기때문에 응모번호도 갱신해줘야한다.
*/
void uiLottoWindow::InsertLastApply(WORD wDrawNum,BYTE *arApplyNum)
{
		//새로삽입
	if(m_listApplyContainer.empty())
	{
		InsertNewApply(wDrawNum,arApplyNum);
	}
	else
	{
       LOTTO_APPLY_DATA  ApplyData = m_listApplyContainer.front();
		
		//true:기존꺼에 꺼내붙여서 다시집어넣음
		//false:새로하나삽입
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

	//갱신해준다.
	SetApplyInfo();
}
//------------------------------------------------------------------------------
/**	응모정보 셋팅
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
	//2개의 Row를가진(횟차:번호순)
	GlobalFunc::SetReportButtonInfo( m_pList, 0, TurnNumWidth, _T("Trun"));
	GlobalFunc::SetReportButtonInfo( m_pList, 1, ApplyInfoWidth, _T("Apply"));

	if(m_listApplyContainer.empty()) return;

	TCHAR szTurnNum[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szInfo[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szApplyNum[INTERFACE_STRING_LENGTH] = {0,};
		
	//몇회차:
	g_InterfaceManager.GetInterfaceString(1893, szTurnNum);

	//응모번호(<0-0-0-0>)
	g_InterfaceManager.GetInterfaceString(1898, szApplyNum);

	std::list<LOTTO_APPLY_DATA>::iterator itr =	m_listApplyContainer.begin();

    int count = 0;
    int index = 0;
	//응모정보를 셋팅한다.
	for(;itr!=m_listApplyContainer.end();++itr)
	{
		LOTTO_APPLY_DATA Info = (*itr);
        if (count == 0)
        {
            index = m_pList->InsertItemBack();
        }

        if (count >= offset_list_)
        {
		    //몇회차
		    Snprintf(szInfo, INTERFACE_STRING_LENGTH-1, szTurnNum, Info.wDrawNum);
		    GlobalFunc::SetItem(m_pList, index, 0, szInfo, cLottoFont,DT_VCENTER | DT_LEFT,cBKcolor,cNumcolor,cSELcolor);
        }
		
		//회차별 응모번호 부분
		BYTE byEntryCnt  = Info.byEntryCnt;
		ZeroMemory(szInfo,sizeof(szInfo));

		for(BYTE a=0;a<byEntryCnt;++a)//for(BYTE a=0;a<byEntryCnt;++a)
		{
			//응모갯수가 30개가 제한
			if(a>=LOTTO_MAX_TICKETS) 
            {
                break;
            }
			_snprintf(szTemp, INTERFACE_STRING_LENGTH-1, szApplyNum, 
					Info.arNumInfo[a][0], //1자리
					Info.arNumInfo[a][1], //2자리
					Info.arNumInfo[a][2], //3자리
					Info.arNumInfo[a][3]);//4자리
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
        if (count%kcolumn_number != 0) // 남은 카운트를 더해준다
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
        m_pScroll->SetScrollRangeWZ(0, (count/kcolumn_number)-4);//4 = 리스트의 라인수
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
    //2개의 Row를가진(횟차:번호순)
    GlobalFunc::SetReportButtonInfo( m_pList, 0, TurnNumWidth, _T("Trun"));
    GlobalFunc::SetReportButtonInfo( m_pList, 1, ApplyInfoWidth, _T("Apply"));

    if(m_listApplyContainer.empty()) return;

    TCHAR szTurnNum[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szInfo[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR szApplyNum[INTERFACE_STRING_LENGTH] = {0,};

    //몇회차:
    g_InterfaceManager.GetInterfaceString(1893, szTurnNum);

    //응모번호(<0-0-0-0>)
    g_InterfaceManager.GetInterfaceString(1898, szApplyNum);

    std::list<LOTTO_APPLY_DATA>::iterator itr =	m_listApplyContainer.begin();

    //응모정보를 셋팅한다.
    for(;itr!=m_listApplyContainer.end();++itr)
    {
        LOTTO_APPLY_DATA Info = (*itr);
        int index = m_pList->InsertItemBack();

        //몇회차
        Snprintf(szInfo, INTERFACE_STRING_LENGTH-1, szTurnNum, Info.wDrawNum);
        GlobalFunc::SetItem(m_pList, index, 0, szInfo, cLottoFont,DT_VCENTER | DT_LEFT,cBKcolor,cNumcolor,cSELcolor);


        //회차별 응모번호 부분
        BYTE byEntryCnt  = Info.byEntryCnt;
        ZeroMemory(szInfo,sizeof(szInfo));

        for(BYTE a=0;a<byEntryCnt;++a)
        {
            //응모갯수가 5개가 제한
            if(a>=LOTTO_MAX_TICKETS) continue;

            //리스트 줄제한(3)
            if(a==3)
            {
                GlobalFunc::SetItem(m_pList, index, 1, szInfo, cLottoFont,DT_VCENTER | DT_LEFT,cBKcolor,cTXTcolor,cSELcolor);
                index = m_pList->InsertItemBack();
                ZeroMemory(szInfo,sizeof(szInfo));
            }

            _snprintf(szTemp, INTERFACE_STRING_LENGTH-1, szApplyNum, 
                Info.arNumInfo[a][0], //1자리
                Info.arNumInfo[a][1], //2자리
                Info.arNumInfo[a][2], //3자리
                Info.arNumInfo[a][3]);//4자리

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
/** 로또응모상태불가(버튼및메시지달리찍음)
*/
void uiLottoWindow::SetEnableWindow(bool bFlag)
{
	if(m_pLottoBtn[LOTTO_BTN_APPLY])
	{
		m_pLottoBtn[LOTTO_BTN_APPLY]->EnableWindowWZ(bFlag);
		TCHAR szInfo[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
					
		//로또 당첨0회차
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
/** 로또기본정보를 토대로 UI를 셋팅한다.
*/
void uiLottoWindow::SetLottoBaseInfo(LOTTO_BASE_INFO &rInfo)
{
	TCHAR szInfo[INTERFACE_STRING_LENGTH] = {0,};	
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};	
	TCHAR szHeim[INTERFACE_STRING_LENGTH] = {0,};

	//하임
	g_InterfaceManager.GetInterfaceString(999, szHeim);

	//=====================================
	// 1 회차 당첨번호: 0-0-0-0
	//=====================================
	//1 회차 당첨번호
	g_InterfaceManager.GetInterfaceString(1888, szTemp);
	Snprintf(szInfo, INTERFACE_STRING_LENGTH-1, szTemp,rInfo.wDrawNum);
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_DRAWNUM], szInfo);
	
	//0-0-0-0
	Snprintf(szInfo, INTERFACE_STRING_LENGTH-1, _T("%02d-%02d-%02d-%02d"),rInfo.arLuckyNum[0],rInfo.arLuckyNum[1],rInfo.arLuckyNum[2],rInfo.arLuckyNum[3]);
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNUM], szInfo);
	
	//=====================================
	// 1 회차 복권기금: 1000하임
	//=====================================
	uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
	if(pLottoMan)
	{
		//1 회차 복권기금
		g_InterfaceManager.GetInterfaceString(1887, szTemp);
		Snprintf(szInfo,INTERFACE_STRING_LENGTH-1,szTemp,pLottoMan->GetLottoInfo()->m_wLottoDrawNum);
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_HEIM_TURN], szInfo);

		//1000하임
        GlobalFunc::SetMoneyText(m_pTxtCtrl[IDX_LOTTO_HEIM], rInfo.LottoHeim, CASH_ITEM_SHOP_HAIM, false);
	}

	//=====================================
	// 1 회차 당첨자수: 1000명
	//=====================================
	//1회차 당첨자수
	g_InterfaceManager.GetInterfaceString(1897, szTemp);
	Snprintf(szInfo,INTERFACE_STRING_LENGTH-1,szTemp,rInfo.wDrawNum);
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNER_TURN], szInfo);
	
	//1000명
	Snprintf(szInfo,INTERFACE_STRING_LENGTH-1,_T("%d"),rInfo.dwWinnerNum);
	g_InterfaceManager.GetInterfaceString(611, szTemp);
	_tcscat( szInfo, szTemp );
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNER], szInfo);

	
	//=====================================
	// 1 회차 1등당첨금: 1000하임
	//=====================================
	//1 회차 1등당첨금
	g_InterfaceManager.GetInterfaceString(1934, szTemp);
	Snprintf(szInfo,INTERFACE_STRING_LENGTH-1,szTemp,rInfo.wDrawNum);
	GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINHEIM_TURN], szInfo);

	//1000하임
    GlobalFunc::SetMoneyText(m_pTxtCtrl[IDX_LOTTO_WINHEIM], rInfo.WinnerHeim, CASH_ITEM_SHOP_HAIM, false);

	//=====================================
	// 0회차일때는 공란처리할것들
	// (당첨번호.당첨자수.당첨금액)
	//=====================================
	if(rInfo.wDrawNum<1)
	{
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_DRAWNUM], _T(" "));			//0회차
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNUM], _T(" "));			//0당첨번호 0-0-0-0

		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNER_TURN], _T(" "));		//1등 당첨자수(txt)
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINNER], _T(" "));			//1등 당첨자수

		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINHEIM_TURN], _T(" "));	//1등 당첨금액(txt)
		GlobalFunc::SetCaption(m_pTxtCtrl[IDX_LOTTO_WINHEIM], _T(" "));			//1등 당첨금액
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
/** 메시지 PROC
*/
VOID uiLottoWindow::MessageProc( SI_MESSAGE * pMessage )
{
	//버튼클릭처리만함
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
					//나의 로또응모정보를 요청한다
					uiLottoMan* pLottoMan= GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);

					if(!pLottoMan) return;
#if defined(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE) || defined(_NA_20101011_HEIM_LOTTO_CASH_TICKET)
                    //중국요청사항:복권용지체크가 필요가없다 / 글로벌도 용지체크 없다
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
						//5289	로또복권이 없습니다.
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
    case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
        {
            if(m_pScroll)
            {
                int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
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
    int pos = this->m_pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
    int mmin = 0 , mmax = 0;
    this->m_pScroll->GetScrollRangeWZ(&mmin, &mmax);  // 스크롤 값의 최대 최소를 얻어 온다.

    if(pos<mmax)    // 현재 입력된 라인수 보다 작으면 
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
    int pos = this->m_pScroll->GetScrollPosWZ();        // 현재 스크롤 값을 받아 온다.
    int mmin = 0 , mmax = 0;
    this->m_pScroll->GetScrollRangeWZ( &mmin, &mmax );  // 스크롤 값의 최대 최소를 얻어 온다.

    if(pos>mmin)    // 현재 입력된 라인수 보다 작으면 
    {
        ((pos-nRange)<mmin ? (pos=mmin):(pos-=nRange));
        this->m_pScroll->SetScrollPosWZ(pos);
        offset_list_ = pos * kcolumn_number;
        SetApplyInfo();
    }
}
#endif//_NA_20101011_HEIM_LOTTO_CASH_TICKET
//----------------------------------------------------------------------------
/** 메시지처리
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
/** 아이템은 항상고정적이다.
*/
void uiLottoWindow::SetItemNum(int iNum)
{
	//모든슬롯은 지우고 셋팅한다.
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
    //셋팅이 안되었다면 리턴
    if(iNum==0) return;   
#endif//_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE

    //보여주기위한 코드
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
    //slot_safe_code:안전한 슬롯체크 구문(삽입)
    if (ValidPos(AtPos) == false)
    {
        return RC_INSERT_SLOT_FAILED;
    }

	// 슬롯에 넣음
	RC::eSLOT_INSERT_RESULT eInsertSlotResult = SCSlotContainer::InsertSlot( AtPos, rSlot );

	//자체 시리얼
	DBSERIAL dwSerial = g_SlotKeyGenerator.GetKey();
	SCSlot & rSetSlot = GetSlot(AtPos);
	rSetSlot.SetSerial(dwSerial);

	CControlWZ * pCtrl = getControl(AtPos);
    //컨트롤이 없을 경우에 대한 보호코드
    //슬롯에 아이템 막기.
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
/** 슬롯제거
*/
BOOL 
uiLottoWindow::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
    //slot_safe_code: 안전한 슬롯체크 구문(삭제)
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
    // 툴팁없음
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
