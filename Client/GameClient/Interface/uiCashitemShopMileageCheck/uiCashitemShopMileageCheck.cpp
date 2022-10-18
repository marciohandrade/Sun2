#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#ifdef __NA_00387_20070605_SHOP_MILEAGE2

#include "uiCashitemShopMileageCheck.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"

#include "interfacemanager.h"
#include "ItemInfoParser.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopMileageCheck::m_wzId[eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_MAX] = 
{
	StrToWzID("BT00"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_BTN_EXIT = 0,

	StrToWzID("B001"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_BTN_OK,
	StrToWzID("B002"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_BTN_CANCEL,

	StrToWzID("L001"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_LST,
	StrToWzID("LS00"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_LST_ALERT,
	StrToWzID("V001"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR,
	StrToWzID("VS00"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR_ALERT,

	StrToWzID("T004"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_TXT_MONEY,
};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopMileageCheck::uiCashItemShopMileageCheck(InterfaceManager *pUIMan) :
	uiBase(pUIMan),
	m_bControlsInitialized(false)
{
	this->m_pCashItemShopMan =
		static_cast<uiCashItemShopMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_CASH_ITEM_SHOP));
	assert (m_pCashItemShopMan);

	if (!this->m_pCashItemShopMan)
	{
#ifdef _DEBUG
		uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
		if(pSystemMan)
			pSystemMan->Sys_VerifyLock(_T("m_pCashItemShopMan이 없습니다."));
#endif
		this->ShowInterface(FALSE);
		return;
	}

	m_bShow = false;
	m_iTotalPage = 0;
	m_iTotalPageAlert = 0;
	ZeroMemory(&m_BuyCashItemCheckInfo, sizeof(m_BuyCashItemCheckInfo));
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopMileageCheck::~uiCashItemShopMileageCheck()
{
	this->Release();
	ZeroMemory(&m_BuyCashItemCheckInfo, sizeof(m_BuyCashItemCheckInfo));
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageCheck::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageCheck::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageCheck::InitControls()
{
	m_pListItem = static_cast<CCtrlListWZ *>(this->getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_LST));
	assert (m_pListItem);

	m_pScroll = static_cast<CCtrlVScrollWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR));
	assert(m_pScroll);

	m_pListAlert = static_cast<CCtrlListWZ *>(this->getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_LST_ALERT));
	assert (m_pListAlert);

	m_pScrollAlert = static_cast<CCtrlVScrollWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR_ALERT));
	assert(m_pScrollAlert);

	m_pTxtCtrlMoney = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_TXT_MONEY));
	assert(m_pTxtCtrlMoney);

	this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiCashItemShopMileageCheck::OnShowWindow( BOOL val )
{
	if (val)
	{
		//uiBase::MoveCenter();

		if (!this->m_bControlsInitialized)
		{
			this->ShowInterface(FALSE);
			return;
		}

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_cash_item_shop_mileage_check;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_mileage_check);
		m_bShow = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopMileageCheck::_isEnable()
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
void uiCashItemShopMileageCheck::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	POSTYPE CtrlPos = getCtrlIDToPos( pMessage->dwCtrlID );

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch(CtrlPos)
			{
			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_BTN_CANCEL:
			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_BTN_EXIT:
				{
					m_bShow = false;
				}
				break;

			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_BTN_OK:	// 구매하겠다!
				{

					if(!GetManager()->GetRqstState())
					{
						GetManager()->ClearPendingBuyList();

						int iIndex = 0;
						for(int i=0; i<_MAX_PRICE_INFO; ++i)
						{
							if(this->m_BuyCashItemCheckInfo.stPrice[i].priceGuid > 0)
							{
								GetManager()->m_pendingBuyList[iIndex] = m_BuyCashItemCheckInfo.stPrice[i].priceGuid;
								++iIndex;
							}
						}

						if(iIndex > 0)
						{
							CopyMemory(&(GetManager()->m_pendingBuyItemInfo), &m_BuyCashItemCheckInfo, sizeof(this->m_BuyCashItemCheckInfo));
							GetManager()->Send_CG_SHOP_ITEM_BUY_SYN();
						}
						else
						{
							//85198	선택한 아이템이 없습니다.
							g_InterfaceManager.GetInterfaceString(85198, szMessage, INTERFACE_STRING_LENGTH);
							GlobalFunc::OutputMsg_SysMsg(szMessage);
						}
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_SB_PAGETOP:		//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINETOP:		//.윗쪽 화살표를 눌렀다.
		{
			if(CtrlPos == eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR)
			{
				int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
				{
					pos--;//한줄 감소 시키고 
					m_pScroll->SetScrollPosWZ(pos);

					m_pListItem->SetBeginDrawIndex(pos * 4);
				}
			}
			else if(CtrlPos == eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR_ALERT)
			{
				int pos = m_pScrollAlert->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pScrollAlert->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if(pos>mmin)// 현재 입력된 라인수 보다 작으면 
				{
					pos--;//한줄 감소 시키고 
					m_pScrollAlert->SetScrollPosWZ(pos);

					m_pListAlert->SetBeginDrawIndex(pos * 4);
				}
			}
		}
		break;

	case RT_MSG_SB_PAGEBOTTOM:	//. 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINEBOTTOM:	//.아랫쪽 화살표를 눌렀다
		{
			if(CtrlPos == eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR)
			{
				int pos = m_pScroll->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pScroll->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
				{
					pos++;//한줄 증가 시키고 
					m_pScroll->SetScrollPosWZ(pos);

					m_pListItem->SetBeginDrawIndex(pos * 4);
				}
			} 
			else if(CtrlPos == eDIALOG_CASH_ITEM_SHOP_MILEAGE_CHECK_VSCR_ALERT)
			{
				int pos = m_pScrollAlert->GetScrollPosWZ();//현재 스크롤 값을 받아 온다.
				int mmin = 0 , mmax = 0;
				m_pScrollAlert->GetScrollRangeWZ( &mmin, &mmax );//스크롤 값의 최대 최소를 얻어 온다.

				if(pos<mmax)// 현재 입력된 라인수 보다 작으면 
				{
					pos++;//한줄 증가 시키고 
					m_pScrollAlert->SetScrollPosWZ(pos);

					m_pListAlert->SetBeginDrawIndex(pos * 4);
				}
			}
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
void uiCashItemShopMileageCheck::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

	if (!_isEnable())
		return;
}

//------------------------------------------------------------------------------
void uiCashItemShopMileageCheck::ShowBuyItemCheck(stCashItemInfo* pInfo)
{
	if(!pInfo)
		return;

	if(!GetManager())
		return;

	CopyMemory(&m_BuyCashItemCheckInfo, pInfo, sizeof(m_BuyCashItemCheckInfo));

	ShowInterface(TRUE);

	_refresh();


	GetManager()->ShowCashItemShopMileageBuy(false);
}

//------------------------------------------------------------------------------
void uiCashItemShopMileageCheck::_refresh()
{
	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( m_BuyCashItemCheckInfo.dwItemCode );
	if ( !pItemInfo ) 
		return;

	if(!m_pListItem)
		return;

	if(!m_pListAlert)
		return;


	m_pListItem->SetUseReport(false);
	m_pListItem->DeleteItemAll();
	m_pListItem->SetColorSelBar(c_CashItemShopListSelBarColor);

	m_pListAlert->SetUseReport(false);
	m_pListAlert->DeleteItemAll();
	m_pListAlert->SetColorSelBar(c_CashItemShopListSelBarColor);

	RECT rc = m_pListItem->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
	int iListWidth = rc.right - rc.left - 20;
#else
	int iListWidth = rc.right - rc.left - 10;
#endif		

	DWORD dwCashMoney = 0;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTime[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szName[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szDesc[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};

	std::vector<util::_tstring> vstrMsg, vstrAlert;
	vstrMsg.clear();
	vstrAlert.clear();

	g_InterfaceManager.GetItemName(m_BuyCashItemCheckInfo.dwItemCode, szName, INTERFACE_STRING_LENGTH);	// 아이템 이름

	if(m_BuyCashItemCheckInfo.byEnchant)
	{
		Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), m_BuyCashItemCheckInfo.byEnchant);
		_tcscat(szName, szTemp);
	}

	if( pItemInfo->CanRankUpItem())
	{
		int iRankIndex = eST_RANK_E_STRING_INDEX  + (m_BuyCashItemCheckInfo.byRank);
		if (iRankIndex > eST_RANK_PS_STRING_INDEX)
			iRankIndex = eST_RANK_PS_STRING_INDEX;
		if (iRankIndex < eST_RANK_E_STRING_INDEX)
			iRankIndex = eST_RANK_E_STRING_INDEX;

		ZeroMemory(szTemp, sizeof(szTemp));
		ZeroMemory(szTemp2, sizeof(szTemp2));
		ZeroMemory(szTemp3, sizeof(szTemp3));
		g_InterfaceManager.GetInterfaceString(eST_RANK_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
		g_InterfaceManager.GetInterfaceString(iRankIndex,szTemp2,INTERFACE_STRING_LENGTH);	
		Snprintf(szTemp3,INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
		_tcscat(szName, _T("   "));
		_tcscat(szName, szTemp3);
	}

	int day = 0;
	int hour = 0;
	int min = 0;

	for(int i=0; i<_MAX_PRICE_INFO; ++i)
	{
		if(m_BuyCashItemCheckInfo.stPrice[i].priceGuid <= 0)
			continue;

		if(m_BuyCashItemCheckInfo.stPrice[i].dwUseTime)	// %d시간 %d분
		{
			ZeroMemory(szTime, sizeof(szTime));
			GlobalFunc::CalcTime(m_BuyCashItemCheckInfo.stPrice[i].dwUseTime, day, hour, min, szTime);

			ZeroMemory(szMessage, sizeof(szMessage));
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s"), szName, szTime);
			GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );

            BOOL bAmountForRider    = FALSE;
            BOOL bAmountExForRider  = FALSE;

            if(eCHARGE_SUB_RIDER_WITH_PART==pItemInfo->m_ChargeSubType)
            {
                if(pItemInfo->IsRiderPartsItem())
                {
                    bAmountExForRider=TRUE;
                }
                else
                {
                    bAmountForRider=TRUE;
                }
            }

            //기간 소모 경고
            if(eCHARGE_SUB_FIXED_AMOUNT == pItemInfo->m_ChargeSubType || bAmountForRider)
			{
				ZeroMemory(szMessage, sizeof(szMessage));
				//85199	선택하신 아이템은 사용기간이 %s 입니다. 해당 아이템은 구매하신 순간부터 바로 적용됩니다.
				g_InterfaceManager.GetInterfaceString(85199, szTemp, INTERFACE_STRING_LENGTH);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szTime);
				GlobalFunc::ProcessWordWrapByFontSize( &vstrAlert, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );
			}
			else if(eCHARGE_SUB_FIXED_AMOUNT_EX == pItemInfo->m_ChargeSubType  || bAmountExForRider)
			{
				ZeroMemory(szMessage, sizeof(szMessage));
				//85200	선택하신 아이템은 사용기간이 %s 입니다. 착용 시부터 적용됩니다.
				g_InterfaceManager.GetInterfaceString(85200, szTemp, INTERFACE_STRING_LENGTH);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szTime);
				GlobalFunc::ProcessWordWrapByFontSize( &vstrAlert, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );
			}
			else if(eCHARGE_SUB_FIXED_QUANTITY == pItemInfo->m_ChargeSubType)
			{
				ZeroMemory(szMessage, sizeof(szMessage));
				//85201	선택하신 아이템은 사용기간이 %s 입니다. 착용 시에만 적용됩니다.
				g_InterfaceManager.GetInterfaceString(85201, szTemp, INTERFACE_STRING_LENGTH);
				Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szTime);
				GlobalFunc::ProcessWordWrapByFontSize( &vstrAlert, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );
			}


		}    
		else if(m_BuyCashItemCheckInfo.stPrice[i].dwAmount)	// %d 개
		{
			ZeroMemory(szDesc, sizeof(szDesc));
			// 155376	%d개
			g_InterfaceManager.GetInterfaceString( 155376 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(szDesc, INTERFACE_STRING_LENGTH-1, szTemp , m_BuyCashItemCheckInfo.stPrice[i].dwAmount);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s"), szName, szDesc);

			GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );
		}
		else
		{
			GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szName, c_wiCashItemShopFont, _tcslen(szName), iListWidth );
		}

		if(m_BuyCashItemCheckInfo.stPrice[i].dwPrice)
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			dwCashMoney += m_BuyCashItemCheckInfo.stPrice[i].dwPrice;
		}
	}

	//85226	같은 효과는 중복되지 않으며, 로그 아웃시 효과가 사라집니다
	if(pItemInfo->m_wType == eITEMTYPE_WASTE)
	{
		g_InterfaceManager.GetInterfaceString(85226, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::ProcessWordWrapByFontSize( &vstrAlert, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );
	}

	//구매 목록 리스트에  add
	int iListIndex = -1;

	for (size_t i=0; i<vstrMsg.size(); ++i)
	{
		iListIndex = m_pListItem->InsertItemBack();

		TCHAR *pStr = const_cast<TCHAR *>(vstrMsg[i].c_str());
		GlobalFunc::SetItem(
			m_pListItem, 
			iListIndex, 0, pStr, 
			c_wiCashItemShopFont,
			DT_VCENTER | DT_LEFT, 
			c_CashItemShopListBKColor, 
			c_CashItemShopListTextColor, 
			c_CashItemShopListSelTextColor);
	}   

	m_iTotalPage = 0;
	if(iListIndex)
	{
		m_iTotalPage = iListIndex / 4;
	}

	m_pScroll->SetScrollRangeWZ(0,m_iTotalPage);	//스크롤 범위
	m_pScroll->SetScrollPosWZ(0);		// 현재 페이지
	m_pListItem->SetBeginDrawIndex(0);


	// 아이템 경고 문구
	vstrMsg.clear();
	if(pItemInfo->m_dCoolTime)	// 사용 쿨타임 경고
	{
		DWORD dwCoolTime = (DWORD)(pItemInfo->m_dCoolTime / 1000);	//(초)
		if(dwCoolTime)
		{
			int iMin = dwCoolTime / 60;
			int iSec = dwCoolTime % 60;

			if(iMin && iSec)
			{
				// 155366	%d분 %d초
				g_InterfaceManager.GetInterfaceString( 155366 , szTemp , INTERFACE_STRING_LENGTH);
				Snprintf(szTime,INTERFACE_STRING_LENGTH-1, szTemp, iMin, iSec );
			}
			else if(iMin)
			{
				//155367	%d분
				g_InterfaceManager.GetInterfaceString( 155367 , szTemp ,INTERFACE_STRING_LENGTH);
				Snprintf(szTime,INTERFACE_STRING_LENGTH-1, szTemp, iMin);
			}
			else if(iSec)
			{
				// 155368	%d초
				g_InterfaceManager.GetInterfaceString( 155368 , szTemp ,INTERFACE_STRING_LENGTH);
				Snprintf(szTime,INTERFACE_STRING_LENGTH-1,szTemp, iSec );
			}

			ZeroMemory(szMessage, sizeof(szMessage));
			//85202	해당 아이템은 %s 의 사용 쿨타임이 있습니다.
			g_InterfaceManager.GetInterfaceString(85202, szTemp, INTERFACE_STRING_LENGTH);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szTime);
			GlobalFunc::ProcessWordWrapByFontSize( &vstrAlert, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );
		}
	}

	//구매 경고 리스트에  add
	int AlertListIndex = -1;

	for (size_t i=0; i<vstrAlert.size(); ++i)
	{
		AlertListIndex = m_pListAlert->InsertItemBack();

		TCHAR *pStr = const_cast<TCHAR *>(vstrAlert[i].c_str());
		GlobalFunc::SetItem(
			m_pListAlert, 
			AlertListIndex, 0, pStr, 
			c_wiCashItemShopFont,
			DT_VCENTER | DT_LEFT, 
			c_CashItemShopListBKColor, 
			c_CashItemShopListTextColor, 
			c_CashItemShopListSelTextColor);
	}   

	m_iTotalPageAlert = 0;
	if(AlertListIndex)
	{
		m_iTotalPageAlert = AlertListIndex / 4;
	}

	GlobalFunc::SetMoneyText(this->m_pTxtCtrlMoney, dwCashMoney, m_BuyCashItemCheckInfo.moneyType);
}

#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
