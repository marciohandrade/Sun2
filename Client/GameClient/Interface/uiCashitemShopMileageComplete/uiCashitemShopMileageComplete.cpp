#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#ifdef __NA_00387_20070605_SHOP_MILEAGE2

#include "uiCashItemShopMileageComplete.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"

#include "interfacemanager.h"
#include "ItemInfoParser.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopMileageComplete::m_wzId[eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_MAX] = 
{
	StrToWzID("BT00"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_BTN_EXIT = 0,

	StrToWzID("B002"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_BTN_OK,

	StrToWzID("L001"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_LST,
	StrToWzID("V001"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_VSCR,

	StrToWzID("T004"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_TXT_COST,	// 비용
	StrToWzID("T005"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_TXT_REMAIN, // 잔액
};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopMileageComplete::uiCashItemShopMileageComplete(InterfaceManager *pUIMan) :
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
	ZeroMemory(&m_DisplayItemInfo, sizeof(m_DisplayItemInfo));
	m_moneyType = MONEY_CASH_TYPE;

}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopMileageComplete::~uiCashItemShopMileageComplete()
{
	this->Release();
	ZeroMemory(&m_DisplayItemInfo, sizeof(m_DisplayItemInfo));
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageComplete::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageComplete::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMileageComplete::InitControls()
{
	m_pListItem = static_cast<CCtrlListWZ *>(this->getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_LST));
	assert (m_pListItem);

	m_pScroll = static_cast<CCtrlVScrollWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_VSCR));
	assert(m_pScroll);

	this->m_pTxtCtrlCost = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_TXT_COST));
	assert(m_pTxtCtrlCost);

	this->m_pTxtCtrlRemain = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_TXT_REMAIN));
	assert(m_pTxtCtrlRemain);


	this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiCashItemShopMileageComplete::OnShowWindow( BOOL val )
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
		msg.dwType = eDoType_dialog_cash_item_shop_mileage_complete;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_mileage_complete);
		m_bShow = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopMileageComplete::_isEnable()
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
void uiCashItemShopMileageComplete::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	//TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_BTN_OK:
			case eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_BTN_EXIT:
				{
					m_bShow = false;
				}
				break;
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
void uiCashItemShopMileageComplete::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

	if (!_isEnable())
		return;
}


//------------------------------------------------------------------------------
void uiCashItemShopMileageComplete::ShowBuyComplete(stCashItemInfo* pInfo)
{
	if (!_isEnable())
		return;

	if(!pInfo)
		return;

	CopyMemory(&m_DisplayItemInfo, pInfo, sizeof(m_DisplayItemInfo));

	ShowInterface(TRUE);

	_refresh();
}

//------------------------------------------------------------------------------
void uiCashItemShopMileageComplete::_refresh()
{
	if (!_isEnable())
		return;

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( m_DisplayItemInfo.dwItemCode );
	if ( !pItemInfo ) 
		return;

	if(!m_pListItem)
		return;

	m_pListItem->SetUseReport(false);
	m_pListItem->DeleteItemAll();
	m_pListItem->SetColorSelBar(c_CashItemShopListSelBarColor);

	RECT rc = m_pListItem->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
	int iListWidth = rc.right - rc.left - 20;
#else
	int iListWidth = rc.right - rc.left - 10;
#endif	

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szName[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTime[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szDesc[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};

	std::vector<util::_tstring> vstrMsg;
	vstrMsg.clear();

	g_InterfaceManager.GetItemName(m_DisplayItemInfo.dwItemCode, szName, INTERFACE_STRING_LENGTH);	// 아이템 이름

	if(m_DisplayItemInfo.byEnchant)
	{
		Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), m_DisplayItemInfo.byEnchant);
		_tcscat(szName, szTemp);
	}

	if( pItemInfo->CanRankUpItem())
	{
		int iRankIndex = eST_RANK_E_STRING_INDEX  + (m_DisplayItemInfo.byRank);
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

	DWORD dwCashMoney = 0;
	DWORD dwHeimMoney = 0;

	int day = 0;
	int hour = 0;
	int min = 0;

	for(int i=0; i<_MAX_PRICE_INFO; ++i)
	{
		if(m_DisplayItemInfo.stPrice[i].priceGuid <= 0)
			continue;

		if(m_DisplayItemInfo.stPrice[i].dwUseTime)	// %d시간 %d분
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			ZeroMemory(szTime, sizeof(szTime));
			GlobalFunc::CalcTime(m_DisplayItemInfo.stPrice[i].dwUseTime, day, hour, min, szTime);

			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s"), szName, szTime);
			GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );


		}    
		else if(m_DisplayItemInfo.stPrice[i].dwAmount)	// %d 개
		{
			ZeroMemory(szDesc, sizeof(szDesc));
			TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
			// 155376	%d개
			g_InterfaceManager.GetInterfaceString( 155376 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(szDesc, INTERFACE_STRING_LENGTH-1, szTemp, m_DisplayItemInfo.stPrice[i].dwAmount);
			Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, _T("%s %s"), szName, szDesc);

			GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );
		}
		else
		{
			GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szName, c_wiCashItemShopFont, _tcslen(szName), iListWidth );
		}

		if(m_DisplayItemInfo.stPrice[i].dwPrice)
		{
			ZeroMemory(szMessage, sizeof(szMessage));
			m_moneyType = m_DisplayItemInfo.moneyType;
			if(LOGINPARAM->IsLiveTestServer() == FALSE) 
			{
				dwCashMoney += m_DisplayItemInfo.stPrice[i].dwPrice;
				// %d Cash

			}
			else	
			{
				dwHeimMoney += m_DisplayItemInfo.stPrice[i].dwPrice;
				// %d Heim
			}
		}
	}

	//리스트에  add
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

	// 사용액
	GlobalFunc::SetMoneyText(this->m_pTxtCtrlCost, dwCashMoney, m_DisplayItemInfo.moneyType);
	// 잔액
	GlobalFunc::SetMoneyText(this->m_pTxtCtrlRemain, GetManager()->GetMyMileage(), m_DisplayItemInfo.moneyType);
}
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
