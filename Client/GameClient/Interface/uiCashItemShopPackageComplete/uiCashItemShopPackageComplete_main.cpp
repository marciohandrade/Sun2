#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "uiCashItemShopPackageComplete.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "interfacemanager.h"
#include "GameFramework.h"
#include "ItemInfoParser.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopPackageComplete::m_wzId[eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_MAX] = 
{
	StrToWzID("BT00"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_EXIT = 0,			//	X 버튼

		StrToWzID("ST04"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_TXT_PRICE,			//	패키지 가격
		StrToWzID("ST05"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_TXT_MYMOMEY,		//	현재 보유 금액

		StrToWzID("BT01"),	//	eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_BTN_OK,			//	확인 버튼

};

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPackageComplete::uiCashItemShopPackageComplete(InterfaceManager *pUIMan) :
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
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopPackageComplete::~uiCashItemShopPackageComplete()
{
	ZeroMemory(&m_PackageItemInfo, sizeof(m_PackageItemInfo));
	this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageComplete::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);

	this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageComplete::Release()
{
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageComplete::InitControls()
{
	int i=0;

	m_pTxtCtrlPrice = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_TXT_PRICE));
	assert(m_pTxtCtrlPrice);

	m_pTxtCtrlMyMoney = static_cast<CCtrlStaticWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_TXT_MYMOMEY));
	assert(m_pTxtCtrlMyMoney);



	this->m_bControlsInitialized = true;
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopPackageComplete::_isEnable()
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
void uiCashItemShopPackageComplete::_refresh()
{
	_clearItem();
	_setItem();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageComplete::_clearItem()
{
	GlobalFunc::SetCaption(m_pTxtCtrlPrice, _T(""));
	GlobalFunc::SetCaption(m_pTxtCtrlMyMoney, _T(""));
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageComplete::_setItem()
{
    BYTE cash_type = GetManager()->GetUseCashType();
    if (LOGINPARAM->IsLiveTestServer())	//테섭실행모드
    {
        GlobalFunc::SetMoneyText(m_pTxtCtrlPrice, m_PackageItemInfo.dwPrice, MONEY_CASH_TYPE);
        GlobalFunc::SetMoneyText(m_pTxtCtrlMyMoney, g_pHero->GetMoney(), MONEY_HIME_TYPE);
    }
    else
    {
        MONEY remain_cash = 0;
        CASH normal_cash = GetManager()->GetMyCash();
        CASH pp_card_cash = GetManager()->GetPPCash();
        GlobalFunc::SetMoneyText(m_pTxtCtrlPrice, m_PackageItemInfo.dwPrice, cash_type);
        switch (GetManager()->GetUseCashType())
        {
        case CASH_ITEM_SHOP_DEFAULT:
            {
                remain_cash = normal_cash + pp_card_cash;
            }
            break;
        case CASH_ITEM_SHOP_HAIM:
            {
                remain_cash = (CASH)g_pHero->GetMoney();
            }
            break;
#ifdef __CN_0_20070507_SHOP_MILEAGE
        case CASH_ITEM_SHOP_MILEAGE:
            {
                remain_cash = GetManager()->GetMyMileage();
            }
            break;
#endif//__CN_0_20070507_SHOP_MILEAGE
        case CASH_ITEM_SHOP_CCASH:
            {
                remain_cash = normal_cash;
            }
            break;
        case CASH_ITEM_SHOP_PCASH:
            {
                remain_cash = pp_card_cash;
            }
            break;
        }        
        GlobalFunc::SetMoneyText(m_pTxtCtrlMyMoney, remain_cash, cash_type);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageComplete::ShowPackageComplete(stPackageItemInfo* pInfo)
{
	if(!pInfo)
		return;

	CopyMemory(&m_PackageItemInfo, pInfo, sizeof(m_PackageItemInfo));

	ShowInterface(TRUE);

	_refresh();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageComplete::OnShowWindow( BOOL val )
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
		msg.dwType = eDoType_dialog_cash_item_shop_package_complete;
		msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE;
		msg.DoSomething = GlobalFunc::DoShowWindow;
		g_KeyQueueManager.PushBack(msg);

		m_bShow = true;
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop_package_complete);
		m_bShow = false;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageComplete::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopPackageComplete::MessageProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
		return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			POSTYPE posType = getCtrlIDToPos( pMessage->dwCtrlID );
			switch( posType )
			{
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_EXIT:
			case eDIALOG_CASH_ITEM_SHOP_PACKAGE_COMPLETE_BTN_OK:
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
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
