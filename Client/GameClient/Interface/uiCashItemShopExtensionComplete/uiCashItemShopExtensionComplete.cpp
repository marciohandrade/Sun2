#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 



#include "uiCashItemShopExtensionComplete.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"

#include "interfacemanager.h"
#include "ItemInfoParser.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopExtensionComplete::m_wzId[eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_MAX] = 
{
	StrToWzID("BT00"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_BTN_EXIT = 0,

	StrToWzID("BT01"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_BTN_OK,

	StrToWzID("ST01"), //eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_TXT_DESC,

	StrToWzID("T004"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_TXT_COST,	// 비용
	StrToWzID("T005"), //eDIALOG_CASH_ITEM_SHOP_MILEAGE_COMPLETE_TXT_REMAIN, // 잔액
};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopExtensionComplete::uiCashItemShopExtensionComplete(InterfaceManager *pUIMan) :
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
uiCashItemShopExtensionComplete::~uiCashItemShopExtensionComplete()
{
	this->Release();
	ZeroMemory(&m_DisplayItemInfo, sizeof(m_DisplayItemInfo));
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionComplete::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);
	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionComplete::Release()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopExtensionComplete::InitControls()
{
	this->m_pTxtCtrlDesc = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_TXT_DESC));
	assert(m_pTxtCtrlDesc);

	this->m_pTxtCtrlCost = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_TXT_COST));
	assert(m_pTxtCtrlCost);

	this->m_pTxtCtrlRemain = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_TXT_REMAIN));
	assert(m_pTxtCtrlRemain);


	this->m_bControlsInitialized = true;
}


//------------------------------------------------------------------------------
void uiCashItemShopExtensionComplete::OnShowWindow( BOOL val )
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
		msg.dwType = eDoType_dialog_cash_item_shop_extension_complete;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_extension_complete);
		m_bShow = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopExtensionComplete::_isEnable()
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
void uiCashItemShopExtensionComplete::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_BTN_OK:
			case eDIALOG_CASH_ITEM_SHOP_EXTENSION_COMPLETE_BTN_EXIT:
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
void uiCashItemShopExtensionComplete::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

	if (!_isEnable())
		return;
}


//------------------------------------------------------------------------------
void uiCashItemShopExtensionComplete::ShowBuyComplete(stCashItemInfo* pInfo)
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
void uiCashItemShopExtensionComplete::_refresh()
{
	if (!_isEnable())
		return;

	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( m_DisplayItemInfo.dwItemCode );
	if ( !pItemInfo ) 
		return;

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szName[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTime[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szDesc[INTERFACE_STRING_LENGTH] = {0,};

	std::vector<util::_tstring> vstrMsg;
	vstrMsg.clear();

	g_InterfaceManager.GetItemName(GetManager()->m_dwPendingExtensionItemCode, szName, INTERFACE_STRING_LENGTH);	// 아이템 이름

	//	5896	%s 아이템이 %s 연장되었습니다.
	g_InterfaceManager.GetInterfaceString(5896, szTemp, INTERFACE_STRING_LENGTH);

	int day = 0;
	int hour = 0;
	int min = 0;

	for(int i=0; i<_MAX_PRICE_INFO; ++i)
	{
		if(m_DisplayItemInfo.stPrice[i].priceGuid <= 0)
			continue;

		GlobalFunc::CalcTime(m_DisplayItemInfo.stPrice[i].dwUseTime, day, hour, min, szTime);
		Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, szName, szTime);
		// 기간
		GlobalFunc::SetCaption(m_pTxtCtrlDesc, szMessage);
#ifdef __CN_0_20070507_SHOP_MILEAGE
		// 구매액
		GlobalFunc::SetMoneyText(this->m_pTxtCtrlCost, m_DisplayItemInfo.stPrice[i].dwPrice, m_DisplayItemInfo.moneyType);
		// 잔액
		GlobalFunc::SetMoneyText(this->m_pTxtCtrlRemain, GetManager()->GetMyCash(), m_DisplayItemInfo.moneyType);
#else	//	__CN_0_20070507_SHOP_MILEAGE

#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
		GlobalFunc::SetMoneyText(m_pTxtCtrlCost, m_DisplayItemInfo.stPrice[i].dwPrice, CASH_ITEM_SHOP_CCASH);
		GlobalFunc::SetMoneyText(m_pTxtCtrlRemain, GetManager()->GetMyCash(), CASH_ITEM_SHOP_CCASH);
#else
	// 구매액
	if(LOGINPARAM->IsLiveTestServer())	//테섭실행모드
	{
		GlobalFunc::SetMoneyText(m_pTxtCtrlCost, m_DisplayItemInfo.stPrice[i].dwPrice, false);
		GlobalFunc::SetMoneyText(m_pTxtCtrlRemain, g_pHero->GetMoney(), false);
	}
	else
	{
		GlobalFunc::SetMoneyText(m_pTxtCtrlCost, m_DisplayItemInfo.stPrice[i].dwPrice, true);
		GlobalFunc::SetMoneyText(m_pTxtCtrlRemain, GetManager()->GetMyCash(), true);
	}
#endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_

#endif	//	__CN_0_20070507_SHOP_MILEAGE
	}
	
}


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

