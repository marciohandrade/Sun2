#include "SunClientPrecompiledHeader.h"

//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 


#include "uiCashItemShopMan.h"
#include "uiCashItemShopBuy/uiCashItemShopBuy.h"
#include "uiCashItemShopBuyCheck/uiCashItemShopBuyCheck.h"
#include "uiCashItemShopBuyComplete/uiCashItemShopBuyComplete.h"
#include "uiCashItemShopCart/uiCashItemShopCart.h"
#include "uiCashItemShopDlg/uiCashItemShopDlg.h"
#include "uiCashItemShopNpcPopup/uiCashItemShopNpcPopup.h"
#include "uiCashItemShopPresentDlg/uiCashItemShopPresentDlg.h"
#include "uiCashItemShopPresentCheck/uiCashItemShopPresentCheck.h"
#include "uiCashItemShopPresentComplete/uiCashItemShopPresentComplete.h"
#include "uiCashItemShopPackageDlg/uiCashItemShopPackageDlg.h"
#include "uiCashItemShopPackageCheck/uicashItemShopPackageCheck.h"
#include "uiCashItemShopPacKageComplete/uiCashItemShopPackageComplete.h"
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
#include "uiCashItemShopMileageDlg/uiCashItemShopMileageDlg.h"
#include "uiCashItemShopMileageBuy/uiCashItemShopMileageBuy.h"
#include "uiCashItemShopMileageCheck/uiCashItemShopMileageCheck.h"
#include "uiCashItemShopMileageComplete/uiCashItemShopMileageComplete.h"
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
#include "uiCashItemShopExtensionBuy/uiCashItemShopExtensionBuy.h"
#include "uiCashItemShopExtensionComplete/uiCashItemShopExtensionComplete.h"
#include "../uiCashPurchaseDlg/uiCashItemPurchaseDlg.h"
#include "interfacemanager.h"

const DWORD c_dwCompareID_CashItemShop					= StrToWzID("0442");
const char  c_strDlgFname_CashItemShop[]				= ("Data\\Interface\\44_2_Cash_itemshop.iwz");

const DWORD c_dwCompareID_CashItemShopBuy				= StrToWzID("0444");
const char  c_strDlgFname_CashItemShopBuy[]				= ("Data\\Interface\\44_4_Cash_popup1.iwz");

const DWORD c_dwCompareID_CashItemShopBuyCheck			= StrToWzID("0447");
const char  c_strDlgFname_CashItemShopBuyCheck[]		= ("Data\\Interface\\44_7_Cash_popup2.iwz");

const DWORD c_dwCompareID_CashItemShopBuyComplete		= StrToWzID("0448");
const char  c_strDlgFname_CashItemShopBuyComplete[]		= ("Data\\Interface\\44_8_Cash_popup3.iwz");

const DWORD c_dwCompareID_CashItemShopCart				= StrToWzID("4431");
const char  c_strDlgFname_CashItemShopCart[]			= ("Data\\Interface\\44_3_1Cash_mypage_basket.iwz");

const DWORD c_dwCompareID_CashItemNpcPopup				= StrToWzID("0441");
const char  c_strDlgFname_CashItemNpcPopup[]			= ("Data\\Interface\\44_1_Cash_npcpopup.iwz");

const DWORD c_dwCompareID_CashItemShopPresentDlg		= StrToWzID("4432");
const char  c_strDlgFname_CashItemShopPresentDlg[]		= ("Data\\Interface\\44_3_2Cash_mypage_gift.iwz");

const DWORD c_dwCompareID_CashItemShopPresentCheck		= StrToWzID("0445");
const char  c_strDlgFname_CashItemShopPresentCheck[]	= ("Data\\Interface\\44_5_Cash_giftpopup.iwz");

const DWORD c_dwCompareID_CashItemShopPresentComplete	= StrToWzID("0446");
const char  c_strDlgFname_CashItemShopPresentComplete[]	= ("Data\\Interface\\44_6_Cash_giftpopup2.iwz");

const DWORD c_dwCompareID_CashItemShopPackageDlg		= StrToWzID("4412");
const char  c_strDlgFname_CashItemShopPackageDlg[]		= ("Data\\Interface\\44_12_Cash_packageshop.iwz");

const DWORD c_dwCompareID_CashItemShopPackageCheck		= StrToWzID("4413");
const char  c_strDlgFname_CashItemShopPackageCheck[]	= ("Data\\Interface\\44_13_Cash_Packagepopup.iwz");

const DWORD c_dwCompareID_CashItemShopPackageComplete	= StrToWzID("4414");
const char  c_strDlgFname_CashItemShopPackageComplete[]	= ("Data\\Interface\\44_14_Cash_Packagepopup2.iwz");

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
const DWORD c_dwCompareID_MileageItemShop				= StrToWzID("4415");
const char  c_strDlgFname_MileageItemShop[]				= ("Data\\Interface\\44_15_Cash_sunpointshop.iwz");

const DWORD c_dwCompareID_MileageItemShopBuy			= StrToWzID("4416");
const char  c_strDlgFname_MileageItemShopBuy[]			= ("Data\\Interface\\44_16_Cash_sunpointpopup1.iwz");

const DWORD c_dwCompareID_MileageItemShopCheck			= StrToWzID("4417");
const char  c_strDlgFname_MileageItemShopCheck[]		= ("Data\\Interface\\44_17_Cash_sunpointpopup2.iwz");

const DWORD c_dwCompareID_MileageItemShopComplete		= StrToWzID("4418");
const char  c_strDlgFname_MileageItemShopComplete[]		= ("Data\\Interface\\44_18_Cash_sunpointpopup3.iwz");
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

const DWORD c_dwCompareID_ExtensionItemShopBuy			= StrToWzID("4420");
const char  c_strDlgFname_ExtensionItemShopBuy[]		= ("Data\\Interface\\44_20_Cash_postponepopup.iwz");

const DWORD c_dwCompareID_ExtensionItemShopComplete		= StrToWzID("4421");
const char  c_strDlgFname_ExtensionItemShopComplete[]	= ("Data\\Interface\\44_21_Cash_postponepopup2.iwz");

const DWORD c_dwCompareID_CashPurchase		            = StrToWzID("4451");
const char  c_strDlgFname_CashPurchase[]	            = ("Data\\Interface\\44_51_Cash_purchase.iwz");

void uiCashItemShopMan::_LoadUI()
{
    m_pUICashItemShopDlg = CreateUIDialog<uiCashItemShopDlg>( c_dwCompareID_CashItemShop, c_strDlgFname_CashItemShop, this);
    m_pUICashItemShopBuy = CreateUIDialog<uiCashItemShopBuy>( c_dwCompareID_CashItemShopBuy, c_strDlgFname_CashItemShopBuy, this);
    m_pUICashItemShopBuyCheck = CreateUIDialog<uiCashItemShopBuyCheck>( c_dwCompareID_CashItemShopBuyCheck, c_strDlgFname_CashItemShopBuyCheck, this);
    m_pUICashItemShopBuyComplete = CreateUIDialog<uiCashItemShopBuyComplete>( c_dwCompareID_CashItemShopBuyComplete, c_strDlgFname_CashItemShopBuyComplete, this);
    m_pUICashItemShopCart = CreateUIDialog<uiCashItemShopCart>( c_dwCompareID_CashItemShopCart, c_strDlgFname_CashItemShopCart, this);
    m_pUICashItemShopNpcPopup = CreateUIDialog<uiCashItemShopNpcPopup>( c_dwCompareID_CashItemNpcPopup, c_strDlgFname_CashItemNpcPopup,this);
    m_pUiCashItemShopPresentDlg = CreateUIDialog<uiCashItemShopPresentDlg>( c_dwCompareID_CashItemShopPresentDlg, c_strDlgFname_CashItemShopPresentDlg, this);
    m_pUICashItemShopPresentCheck = CreateUIDialog<uiCashItemShopPresentCheck>( c_dwCompareID_CashItemShopPresentCheck, c_strDlgFname_CashItemShopPresentCheck, this);
    m_pUICashItemShopPresentComplete = CreateUIDialog<uiCashItemShopPresentComplete>( c_dwCompareID_CashItemShopPresentComplete, c_strDlgFname_CashItemShopPresentComplete, this);
    m_pUiCashItemShopPackageDlg = CreateUIDialog<uiCashItemShopPackageDlg>( c_dwCompareID_CashItemShopPackageDlg, c_strDlgFname_CashItemShopPackageDlg, this);
    m_pUiCashItemShopPackageCheck = CreateUIDialog<uiCashItemShopPackageCheck>( c_dwCompareID_CashItemShopPackageCheck, c_strDlgFname_CashItemShopPackageCheck, this);
    m_pUiCashItemShopPackageComplete = CreateUIDialog<uiCashItemShopPackageComplete>( c_dwCompareID_CashItemShopPackageComplete, c_strDlgFname_CashItemShopPackageComplete, this);

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    m_pUICashItemShopMileageDlg = CreateUIDialog<uiCashItemShopMileageDlg>( c_dwCompareID_MileageItemShop, c_strDlgFname_MileageItemShop, this);
    m_pUICashItemShopMileageBuy = CreateUIDialog<uiCashItemShopMileageBuy>( c_dwCompareID_MileageItemShopBuy, c_strDlgFname_MileageItemShopBuy, this);
    m_pUICashItemShopMileageCheck = CreateUIDialog<uiCashItemShopMileageCheck>( c_dwCompareID_MileageItemShopCheck, c_strDlgFname_MileageItemShopCheck, this);
    m_pUICashItemShopMileageComplete = CreateUIDialog<uiCashItemShopMileageComplete>( c_dwCompareID_MileageItemShopComplete, c_strDlgFname_MileageItemShopComplete, this);
#endif // __NA_00387_20070605_SHOP_MILEAGE2

    m_pUICashItemShopExtensionBuy = CreateUIDialog<uiCashItemShopExtensionBuy>( c_dwCompareID_ExtensionItemShopBuy,  c_strDlgFname_ExtensionItemShopBuy, this);
    m_pUICashItemShopExtensionComplete = CreateUIDialog<uiCashItemShopExtensionComplete>( c_dwCompareID_ExtensionItemShopComplete,  c_strDlgFname_ExtensionItemShopComplete, this);
    m_pUICashItemPurchaseDlg = CreateUIDialog<uiCashItemPurchaseDlg>( c_dwCompareID_CashPurchase, c_strDlgFname_CashPurchase,this);
}


//------------------------------------------------------------------------------ 
void uiCashItemShopMan::ChangeTab(uiBase* ui_base)
{
    float cx = -1.0f;
    float cy = -1.0f;

    if (active_tab() != NULL)
    {
        if ((active_tab() == ui_base) && (active_tab()->IsVisible() == TRUE))
        {
            return;
        }

        cx = active_tab()->GetDialogWZ()->GetDlgX();
        cy = active_tab()->GetDialogWZ()->GetDlgY();

        active_tab()->ShowInterface(FALSE);
    }

    if (ui_base == NULL)
    {
        return;
    }

    set_active_tab(ui_base);

    if (active_tab() != NULL)
    {
        active_tab()->ShowInterface(TRUE);

        if (cx >= 0.0f && cy >= 0.0f)
        {
            active_tab()->MoveWindowWZ(cx, cy);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiCashItemShopMan::ShowTab()
{
    if (active_tab() != NULL)
    {
        active_tab()->ShowInterface(TRUE);
    }
    else
    {
        //! ±âº».
        ChangeTab(m_pUICashItemShopDlg);
    }
}

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
