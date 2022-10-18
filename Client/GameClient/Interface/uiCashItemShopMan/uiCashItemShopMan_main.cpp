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
#include "uiCashItemShopPresentDlg/uiCashItemShopPresentDlg.h"
#include "uiCashItemShopPresentCheck/uiCashItemShopPresentCheck.h"
#include "uiCashItemShopPresentComplete/uiCashItemShopPresentComplete.h"
#include "uiCashItemShopPackageDlg/uiCashItemShopPackageDlg.h"
#include "uiCashItemShopPackageCheck/uiCashItemShopPackageCheck.h"
#include "uiCashItemShopPackageComplete/uiCashItemShopPackageComplete.h"
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
#include "uiCashItemShopMileageDlg/uiCashItemShopMileageDlg.h"
#include "uiCashItemShopMileageBuy/uiCashItemShopMileageBuy.h"
#include "uiCashItemShopMileageCheck/uiCashItemShopMileageCheck.h"
#include "uiCashItemShopMileageComplete/uiCashItemShopMileageComplete.h"
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
#include "uiCashItemShopExtensionBuy/uiCashItemShopExtensionBuy.h"
#include "uiCashItemShopExtensionComplete/uiCashItemShopExtensionComplete.h"
#include "./uiCashPurchaseDlg\uiCashItemPurchaseDlg.h"
#include "uiCashItemShopNpcPopup/uiCashItemShopNpcPopup.h"
#include "GlobalFunc.h"
#include "ItemManager.h"
#include "OptionListParser.h"
#include "ResultCode.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "SceneBase.h"
#include "uiSystemMan/uiSystemMan.h"
#include "ItemInfoParser.h"
#include "SceneBase.h"
#include "SCItemSlot.h"
#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
#include "Application.h"
#include "uiLoginMan/uiLoginMan.h"
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

using namespace RC;

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopMan::uiCashItemShopMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
    m_pUICashItemShopDlg				= NULL;
    m_pUICashItemShopBuy				= NULL;
	m_pUICashItemShopBuyCheck			= NULL;
	m_pUICashItemShopBuyComplete		= NULL;
	m_pUICashItemShopCart				= NULL;
	m_pUICashItemShopNpcPopup			= NULL;
	m_pUiCashItemShopPresentDlg			= NULL;
	m_pUICashItemShopPresentCheck		= NULL;
	m_pUICashItemShopPresentComplete	= NULL;
	m_pUiCashItemShopPackageDlg			= NULL;
	m_pUiCashItemShopPackageCheck		= NULL;
	m_pUiCashItemShopPackageComplete	= NULL;

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	m_pUICashItemShopMileageDlg			= NULL;
	m_pUICashItemShopMileageBuy			= NULL;
	m_pUICashItemShopMileageCheck		= NULL;
	m_pUICashItemShopMileageComplete	= NULL;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	m_pUICashItemShopExtensionBuy		= NULL;
	m_pUICashItemShopExtensionComplete	= NULL;

	m_pUICashItemPurchaseDlg            =NULL;
    pp_cash_                            = 0;
    
    if (LOGINPARAM->IsLiveTestServer())	//테섭실행모드
	{
	    use_cash_type_ = CASH_ITEM_SHOP_HAIM;
	}
	else
	{
        use_cash_type_ = CASH_ITEM_SHOP_DEFAULT;
	}
    selected_index_                     = 0;
    pp_category_                        = false;
    cc_category_                        = false;
	m_iPresentState						= ePresentState_None;
	m_iCashShopDlgState					= eCashItemShopDlgState_None;
	m_Cash								= 0;
	ClearAllDlg();
	m_bShowSystemDialog					= false;

	// 풀 사이즈 설정
	m_ShopManager.Initialize_DefaultUserPool(2);

    active_tab_ = NULL;

    ZeroMemory(&current_show_item_list_, sizeof(current_show_item_list_));
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopMan::~uiCashItemShopMan()
{
	ClearColorStr();

    //this->Release();
}


void uiCashItemShopMan::OnInitialize()
{
    _LoadUI();

    m_ShopManager.Init();

    ClearItemInfo();
}

//------------------------------------------------------------------------------
/**
*/

void uiCashItemShopMan::ClearItemInfo( void )
{
	memset( &m_stCashItemInfo, 0, sizeof( m_stCashItemInfo ) );
	memset( &m_stPackageItemInfo, 0, sizeof( m_stPackageItemInfo ) );
	memset( &m_stPresentItemInfo, 0, sizeof( m_stPresentItemInfo ) );
	memset( &m_stExtensionItemInfo, 0, sizeof( m_stExtensionItemInfo ) );
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::Release()
{
	m_ShopManager.Release();
    active_tab_ = NULL;
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::Update()
{
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::Process(DWORD dwTick)
{
	if( m_CurSceneType != GameFramework::GetCurrentScene()->GetType() )
		m_pUICashItemShopDlg->ShowInterface(FALSE);
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ClearAllDlg()
{
	m_bWaitPacket			= false;

	m_vecCashShopRcvInfo.clear();
	m_vecPackageRcvInfo.clear();
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	m_vecMileageShopRcvInfo.clear();
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	m_vecExtensionShopRcvInfo.clear();

	if(m_pUiCashItemShopPresentDlg)
		m_pUiCashItemShopPresentDlg->SetPresentCount(0);

	m_byPendingSeperate		= 0;
	m_iCashShopDlgState		= eCashItemShopDlgState_None;
	m_iPresentState			= ePresentState_None;

	ClearNewPresentRcvCount();

	ClearCashItemInfo();
	ClearPackageItemInfo();
	ClearPresentItemInfo();

	ClearPendingBuyList();

	m_ShopManager.Clear();

	static const int HERO_TEMP_KEY = 3;
	ShopUser * pShopUser = m_ShopManager.CreateShopUser();
	if(pShopUser)
	{
		pShopUser->SetUserGuid( HERO_TEMP_KEY );
		m_ShopManager.AddShopUser( pShopUser );
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ClearDlg()
{
	m_bWaitPacket					= false;

	m_byPage						= 0;

	m_byPendingPage					= 0;
    m_wPendingFilter = 0;

	m_iPendingItemGuid				= 0;
	m_iPendingItemIndex				= 0;

	m_dwPendingExtensionItemCode	= 0;

	m_byCategory					= 0;
	m_byPendingCategory				= 0;

	SetRqstState(eRqstState_None);
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ClearCart()
{
	m_TotalPageNumByCart = 0;
	m_CurPageNumByCart = 0;

	ZeroMemory(&m_stCartInfo, sizeof(m_stCartInfo));
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ToggleCashItemShopNpcPopup()	// npc 팝업창 Toggle
{
	if(!m_pUICashItemShopNpcPopup)
		return;

	if(m_pUICashItemShopNpcPopup->IsVisibleWindow())
	{
		m_pUICashItemShopNpcPopup->ShowInterface(FALSE);
	}
	else
	{
		m_pUICashItemShopNpcPopup->ShowInterface(TRUE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemPurchaseDlg(bool bShow)	// PP결제 확인창
{
	if (!m_pUICashItemPurchaseDlg)
    {
		return;
    }

	if (bShow && !m_pUICashItemPurchaseDlg->IsVisibleWindow())
	{
    	m_pUICashItemPurchaseDlg->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemPurchaseDlg->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::SelectBuyItem(int selected_index)
{
    SetBuyIndex(selected_index);

    switch (GetCashShopDlgState())
    {
    case eCashItemShopDlgState_None:
    case eCashItemShopDlgState_CashShop:
    case eCashItemShopDlgState_Package:
#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
        {   // 결제 방법이 C 캐시, PP 캐시 두가지 방법이 있다.
            ShowCashItemPurchaseDlg(false);// 일단 이전창이 있을 수도 있으므로 닫는다.
            if (pp_category())
            {
                SetUseCashType(CASH_ITEM_SHOP_PCASH);
                OpenBuyWindowFromCashType();
            }
            else if (cc_category())
            {
                SetUseCashType(CASH_ITEM_SHOP_CCASH);
                OpenBuyWindowFromCashType();
            }
            else
            {
                ShowCashItemPurchaseDlg(true);
            }
        }
        break;
#endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    case eCashItemShopDlgState_Mileage:
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
    case eCashItemShopDlgState_ShopBasket:
    case eCashItemShopDlgState_Present:
        {
            SetBuyIndex((int)selected_index);
            OpenBuyWindowFromCashType();
        }
        break;
    default:
        {
        }
        break;
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::OpenBuyWindowFromCashType()
{
    switch (GetCashShopDlgState())
    {
    case eCashItemShopDlgState_None:
    case eCashItemShopDlgState_CashShop:   // 일반캐쉬
        {
            if(m_pUICashItemShopDlg)
            {
                m_pUICashItemShopDlg->SetSelectedIndex(GetBuyIndex());
            }
        }
        break;
    case eCashItemShopDlgState_Package:  // 패키지
        {
            if(m_pUiCashItemShopPackageDlg)
            {
                ShowCashItemShopPackageCheck(GetBuyIndex());
            }
        }
        break;
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    case eCashItemShopDlgState_Mileage:  // 마일리지
        {
            if(m_pUICashItemShopMileageDlg)
            {
                m_pUICashItemShopMileageDlg->SetSelectedIndex(GetBuyIndex());
            }
        }
        break;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
    default:
        {
        }
        break;
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopNpcPopup(bool bShow)	// npc 팝업창 Show
{
	if(!m_pUICashItemShopNpcPopup)
		return;

	if(bShow && !m_pUICashItemShopNpcPopup->IsVisibleWindow())
	{
		//m_pUICashItemShopNpcPopup->ShowInterface(TRUE);
        ChangeTab(m_pUICashItemShopNpcPopup);
	}
	else
	{
		m_pUICashItemShopNpcPopup->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ToggleCashItemShopDlg()		// 유료 상점창 Toggle
{
	if(!m_pUICashItemShopDlg)
		return;

	m_CurSceneType = GameFramework::GetCurrentScene()->GetType();

	if(m_pUICashItemShopDlg->IsVisibleWindow())
	{
		m_pUICashItemShopDlg->ShowInterface(FALSE);
		
	}
	else
	{
        // 죽은 상태에서는 캐시샵을 열지 않는다.(서버에서 죽은상태에서는 구매가 되지 않도록 되어 있음)
        if ((g_pHero != NULL) && 
            (g_pHero->IsDead() == FALSE))
        {
            // 창을 열기 전에 cash 요청을 하자
            ClearAllDlg();
            this->Send_CG_SHOP_CASH_REQ_SYN();
        }
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopDlg(bool bShow)		// 유료 상점창 Show
{
	if (!m_pUICashItemShopDlg)
    {
		return;
    }
    ShowCashItemPurchaseDlg(false);
	if (bShow && !m_pUICashItemShopDlg->IsVisibleWindow())
	{
		// 창을 열기 전에 cash 요청을 하자
		//m_pUICashItemShopDlg->ShowInterface(true);
        ChangeTab(m_pUICashItemShopDlg);
	}
	else
	{
		m_pUICashItemShopDlg->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/

void uiCashItemShopMan::ShowCashItemShopPresentDlg(bool bShow)	// 받은 선물함 Show
{
#ifdef	__MY_PAGE_BLOCK
	return;
#endif
	if (!m_pUiCashItemShopPresentDlg)
    {
		return;
    }
    ShowCashItemPurchaseDlg(false);
	if (bShow && !m_pUiCashItemShopPresentDlg->IsVisibleWindow())
	{
		//m_pUiCashItemShopPresentDlg->ShowInterface(TRUE);
        ChangeTab(m_pUiCashItemShopPresentDlg);
	}
	else
	{
		m_pUiCashItemShopPresentDlg->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPresentComplete(bool bShow)	// 받은 선물함 팝업창
{
	if(!m_pUICashItemShopPresentComplete)
		return;

	if(bShow && !m_pUICashItemShopPresentComplete->IsVisibleWindow())
	{
		m_pUICashItemShopPresentComplete->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemShopPresentComplete->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPresentComplete(stPresentItemInfo* pInfo)	// 받은 선물함 팝업창
{
	if(!m_pUICashItemShopPresentComplete)
	return;

	m_pUICashItemShopPresentComplete->ShowPresentItemComplete(pInfo);

}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPresentCheck(bool bShow)	// 받은 선물함 체크팝업창
{
	if(!m_pUICashItemShopPresentCheck)
		return;

	if(bShow && !m_pUICashItemShopPresentCheck->IsVisibleWindow())
	{
		m_pUICashItemShopPresentCheck->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemShopPresentCheck->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPresentCheck(int iIndex)
{
	if(!m_pUICashItemShopPresentCheck)
		return;

	m_pUICashItemShopPresentCheck->ShowPresentItemCheck(&m_stPresentItemInfo[iIndex]);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPresentCheck(stPresentItemInfo* pInfo)
{
	if(!m_pUICashItemShopPresentCheck)
		return;

	m_pUICashItemShopPresentCheck->ShowPresentItemCheck(pInfo);
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPackageDlg(bool bShow)	// 패키지샾 Show
{
	if (!m_pUiCashItemShopPackageDlg)
    {
		return;
    }
    ShowCashItemPurchaseDlg(false);
	if (bShow && !m_pUiCashItemShopPackageDlg->IsVisibleWindow())
	{
		//m_pUiCashItemShopPackageDlg->ShowInterface(TRUE);
        ChangeTab(m_pUiCashItemShopPackageDlg);
	}
	else
	{
		m_pUiCashItemShopPackageDlg->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPackageComplete(bool bShow)	// 패키지샾 Show
{
	if(!m_pUiCashItemShopPackageComplete)
		return;

	if(bShow && !m_pUiCashItemShopPackageComplete->IsVisibleWindow())
	{
		m_pUiCashItemShopPackageComplete->ShowInterface(TRUE);
	}
	else
	{
		m_pUiCashItemShopPackageComplete->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPackageComplete(stPackageItemInfo* pInfo)
{
	if(!m_pUiCashItemShopPackageComplete)
		return;

	m_pUiCashItemShopPackageComplete->ShowPackageComplete(pInfo);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPackageCheck(bool bShow)	// 패키지샾 Show
{
	if(!m_pUiCashItemShopPackageCheck)
		return;

	if(bShow && !m_pUiCashItemShopPackageCheck->IsVisibleWindow())
	{
		m_pUiCashItemShopPackageCheck->ShowInterface(TRUE);
	}
	else
	{
		m_pUiCashItemShopPackageCheck->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopPackageCheck(int iIndex)
{
	if(!m_pUiCashItemShopPackageCheck)
		return;

	m_pUiCashItemShopPackageCheck->ShowPackageCheck(&m_stPackageItemInfo[iIndex]);
}



#ifdef __NA_00387_20070605_SHOP_MILEAGE2
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopMileageDlg(bool bShow)
{
	if (!m_pUICashItemShopMileageDlg)
    {
		return;
    }
    ShowCashItemPurchaseDlg(false);
	if (bShow && !m_pUICashItemShopMileageDlg->IsVisibleWindow())
	{
		//m_pUICashItemShopMileageDlg->ShowInterface(TRUE);
        ChangeTab(m_pUICashItemShopMileageDlg);
	}
	else
	{
		m_pUICashItemShopMileageDlg->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopMileageBuy(int iIndex)
{
	if(!m_pUICashItemShopMileageBuy)
		return;

	m_pUICashItemShopMileageBuy->ShowBuyItem(&m_stCashItemInfo[iIndex]);
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopMileageBuy(bool bShow)
{
	if(!m_pUICashItemShopMileageBuy)
		return;

	if(bShow && !m_pUICashItemShopMileageBuy->IsVisibleWindow())
	{
		m_pUICashItemShopMileageBuy->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemShopMileageBuy->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopMileageCheck(bool bShow)
{
	if(!m_pUICashItemShopMileageCheck)
		return;

	if(bShow && !m_pUICashItemShopMileageCheck->IsVisibleWindow())
	{
		m_pUICashItemShopMileageCheck->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemShopMileageCheck->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopMileageCheck(stCashItemInfo* pInfo)
{
	if(!m_pUICashItemShopMileageCheck)
		return;

	m_pUICashItemShopMileageCheck->ShowBuyItemCheck(pInfo);
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopMileageComplete(bool bShow)
{
	if(!m_pUICashItemShopMileageComplete)
		return;

	if(bShow && !m_pUICashItemShopMileageComplete->IsVisibleWindow())
	{
		m_pUICashItemShopMileageComplete->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemShopMileageComplete->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopMileageComplete(stCashItemInfo* pInfo)
{
	if(!m_pUICashItemShopMileageComplete)
		return;

	m_pUICashItemShopMileageComplete->ShowBuyComplete(pInfo);
}
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopExtensionBuy(int iIndex)
{
	if(!m_pUICashItemShopExtensionBuy)
		return;

	m_pUICashItemShopExtensionBuy->ShowBuyItem(&m_stExtensionItemInfo[iIndex]);
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopExtensionBuy(bool bShow)
{
	if(!m_pUICashItemShopExtensionBuy)
		return;

	if(bShow && !m_pUICashItemShopExtensionBuy->IsVisibleWindow())
	{
		m_pUICashItemShopExtensionBuy->ShowInterface(TRUE);
		m_pUICashItemShopExtensionBuy->ClearItem();
	}
	else
	{
		m_pUICashItemShopExtensionBuy->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopExtensionComplete(bool bShow)
{
	if(!m_pUICashItemShopExtensionComplete)
		return;

	if(bShow && !m_pUICashItemShopExtensionComplete->IsVisibleWindow())
	{
		m_pUICashItemShopExtensionComplete->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemShopExtensionComplete->ShowInterface(FALSE);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopExtensionComplete(stCashItemInfo* pInfo)
{
	if(!m_pUICashItemShopExtensionComplete)
		return;

	m_pUICashItemShopExtensionComplete->ShowBuyComplete(pInfo);
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopBuyCart(bool bShow)	// 장바구니 Show
{
	
	if(!m_pUICashItemShopCart)
		return;

	if(bShow && !m_pUICashItemShopCart->IsVisibleWindow())
	{
		//m_pUICashItemShopCart->ShowInterface(TRUE);
        ChangeTab(m_pUICashItemShopNpcPopup);
	}
	else
	{
		m_pUICashItemShopCart->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopBuy(bool bShow)
{
	if(!m_pUICashItemShopBuy)
		return;

	if(bShow && !m_pUICashItemShopBuy->IsVisibleWindow())
	{
		m_pUICashItemShopBuy->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemShopBuy->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopBuy(int iIndex)
{
	if(!m_pUICashItemShopBuy)
		return;

	m_pUICashItemShopBuy->ShowBuyItem(&m_stCashItemInfo[iIndex]);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopBuyCheck(bool bShow)
{
	if(!m_pUICashItemShopBuyCheck)
		return;

	if(bShow && !m_pUICashItemShopBuyCheck->IsVisibleWindow())
	{
		m_pUICashItemShopBuyCheck->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemShopBuyCheck->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopBuyCheck(stCashItemInfo* pInfo)
{
	if(!m_pUICashItemShopBuyCheck)
		return;

	m_pUICashItemShopBuyCheck->ShowBuyItemCheck(pInfo);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopBuyComplete(bool bShow)
{
	if(!m_pUICashItemShopBuyComplete)
		return;

	if(bShow && !m_pUICashItemShopBuyComplete->IsVisibleWindow())
	{
		m_pUICashItemShopBuyComplete->ShowInterface(TRUE);
	}
	else
	{
		m_pUICashItemShopBuyComplete->ShowInterface(FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::ShowCashItemShopBuyComplete(stCashItemInfo* pInfo)
{
	if(!m_pUICashItemShopBuyComplete)
		return;

	m_pUICashItemShopBuyComplete->ShowBuyComplete(pInfo);
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::Send_CG_SHOP_CASH_REQ_SYN()	// 샵 오픈시 제일먼저 보냄
{
	if(!this->m_bWaitPacket)
	{
		MSG_CG_SHOP_CASH_REQ_SYN sync;

		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
		m_bWaitPacket = true;
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::Send_CG_SHOP_ACTION_RECV_GIFT_SYN( int actionType )
{
	if(!this->m_bWaitPacket)
	{
		MSG_CG_SHOP_ACTION_RECV_GIFT_SYN sync;

		enum { OBTAIN=1, REJECT=2, DISCARD=3, };
		enum { MAX_GIFT_MESSAGE_LEN = 96, };
		//BYTE	m_ActionType;	//< enum { OBTAIN=1, REJECT=2, DISCARD=3, };
		//DWORD	m_GiftGuid;		//< 선물Guid

		m_bWaitPacket = true;
		sync.m_ActionType	= actionType;
		sync.m_GiftGuid		= m_pendingBuyPresentItemInfo.giftGuid;

		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
	}
}

void uiCashItemShopMan::Send_CG_SHOP_SEND_GIFT_SYN( void )
{
	if(!this->m_bWaitPacket)
	{
		MSG_CG_SHOP_SEND_GIFT_SYN sync;

		sync.m_ItemType		= m_pendingBuyPresentItemInfo.byItemType;
		sync.m_PriceGuid	= m_pendingBuyPresentItemInfo.priceGuid;

        strncpy(sync.m_tszCharName, m_pendingBuyPresentItemInfo.strPlayerName, _countof(sync.m_tszCharName));
        sync.m_tszCharName[_countof(sync.m_tszCharName) - 1] = '\0';        
        strncpy(sync.m_tszGiftMessage, m_pendingBuyPresentItemInfo.strSendMessage, _countof(sync.m_tszGiftMessage));
        sync.m_tszGiftMessage[_countof(sync.m_tszGiftMessage) - 1] = '\0';

		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));

		m_bWaitPacket = true;
		SetRqstState(eRqstState_Present_Send);
	}
}

void uiCashItemShopMan::Send_CG_SHOP_IS_RECV_GIFT_SYN( void )
{
	if(!this->m_bWaitPacket)
	{
		MSG_CG_SHOP_IS_RECV_GIFT_SYN sync;

		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::Send_CG_SHOP_PRESENT_LIST_REQ_SYN( void )
{
	if(!this->m_bWaitPacket)
	{
		MSG_CG_SHOP_RECV_GIFT_LIST_SYN sync;

		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
		m_bWaitPacket = true;
	}
}

       
//------------------------------------------------------------------------------ 
//! 예 캐쉬아템 -> 아이템-> 행운류 -> 신상, 발킬 1페이지
//! partition = SHOP_PARTITION_SINGLEITEM
//! category = SHOP_CATEGORY_EQUIP_HELMET
//! m_Filter = eSHOP_FILTER_CHAR_VALKYRIE | eITEMSHOP_FILTER_DAY_DESCENDING;
//! page = 1;
//------------------------------------------------------------------------------ 
void uiCashItemShopMan::Send_CG_SHOP_ITEM_LIST_REQ_SYN(BYTE partition, BYTE category, WORD filter, BYTE page)
{
    if(m_bWaitPacket == false)
    {
        MSG_CG_SHOP_ITEM_LIST_REQ_SYN sync;

        if (category <= CT_MAX)
        {
            ASSERT(0 && "(category <= CT_MAX)");
            return;
        }

        sync.partition_index = partition;
        sync.category_index = category;
        sync.item_filter = filter;
        sync.request_page = page;

        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
        m_bWaitPacket = true;

        m_byPendingCategory = category;
        m_byPendingPage = page;
        m_wPendingFilter = filter;
        m_byPendingSeperate = GetCashShopDlgState();
    }
}

//------------------------------------------------------------------------------ 
void uiCashItemShopMan::OnClickExtensionButton(int page)
{
#ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
    if (pp_category())
    {
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "Lucky Items only can be extended by W-coin(p)");
    }
    else if (cc_category())
    {
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "Premium Items only can be extended by W-coin(c)");
    }
    else
#endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
    {
        Send_CG_SHOP_ITEM_LIST_REQ_SYN(SHOP_PARTITION_SINGLEITEM, 
                                        SHOP_CATEGORY_PERIOD_EXTEND, 
                                        SHOP_FILTER_CHAR_ALL, 
                                        page);
    }
}


//------------------------------------------------------------------------------
/**
*/

void uiCashItemShopMan::Send_CG_SHOP_ITEM_REQ_SYN(int iItemIndex, BOOL bExtension)
{
	if(!this->m_bWaitPacket)
	{
		MSG_CG_SHOP_ITEM_REQ_SYN sync;

		if(bExtension)
			sync.m_ItemGuid = m_stExtensionItemInfo[iItemIndex].itemguid;
		else
			sync.m_ItemGuid = m_stCashItemInfo[iItemIndex].itemguid;

		m_iPendingItemGuid = sync.m_ItemGuid;
		m_iPendingItemIndex = iItemIndex;		

		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
		m_bWaitPacket = true;

	}
}

//------------------------------------------------------------------------------
/**
*/
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
void uiCashItemShopMan::RefreshItemPrice()
{
	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	int i, j;

	int iStartIndex = _ITEM_NUM_PER_PAGE * m_pUICashItemShopMileageDlg->GetCurPage();
	int iIndex = 0;

	ShopCategory* pShopCategory = NULL;

	uiCashItemShopBase * pCashItemShopBase = NULL;

	if( GetCashShopDlgState() == eCashItemShopDlgState_CashShop )
		pCashItemShopBase = static_cast<uiCashItemShopBase *>(m_pUICashItemShopDlg);
	else if( GetCashShopDlgState() == eCashItemShopDlgState_Mileage )
		pCashItemShopBase = static_cast<uiCashItemShopBase *>(m_pUICashItemShopMileageDlg);

	if(pCashItemShopBase == NULL)	return;

	eSHOP_CATEGORY byCategory;

	if ((m_byPendingCategory == CT_PERIOD_EXTEND) || (m_byPendingCategory == SHOP_CATEGORY_PERIOD_EXTEND))
		byCategory = (eSHOP_CATEGORY)m_byPendingCategory;
	else
		byCategory = (eSHOP_CATEGORY)pCashItemShopBase->GetCurCategoryType();

	pCashItemShopBase->RefreshItemPrice();

    iStartIndex = 0;

    SHOPITEM_LIST shopItemList;
    for (int kk = 0; kk < current_show_item_list()->guid_count; ++kk)
    {
        ShopItem* shop_item = m_ShopManager.FindShopItem(current_show_item_list()->shopitem_guid[kk]);
        if (shop_item != NULL)
        {
            shopItemList.push_back(shop_item);
        }
    }

	SHOPITEM_LIST::iterator it;
	SingleItem* pItem = NULL;
	ShopItemPrice* pPrice = NULL;

#ifdef _DEBUG
	ZeroMemory(szDebug, sizeof(szDebug));
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("shopItemList SizeNum : %d \n"), shopItemList.size());
	OutputDebugString(szDebug);
#endif

	for(it = shopItemList.begin(); it != shopItemList.end(); ++it)
	{
		if((*it)->GetGuid() == m_iPendingItemGuid)
		{
			pItem = (SingleItem *)*it;

			if(pItem)
			{
				if(iStartIndex <= iIndex)
				{
					for(j = 0; j < _ITEM_NUM_PER_PAGE; j++)
					{
						stCashItemInfo* stItemInfo;

						if ((byCategory == CT_PERIOD_EXTEND) || (byCategory == SHOP_CATEGORY_PERIOD_EXTEND))
							stItemInfo = &m_stExtensionItemInfo[j];
						else
							stItemInfo = &m_stCashItemInfo[j];


						if(stItemInfo->itemguid == m_iPendingItemGuid)
						{
							stItemInfo->moneyType	= pItem->GetMoneyType();
							stItemInfo->dwCoolTime	= pItem->GetCoolTime();
							stItemInfo->SellEndDate	= pItem->GetSellEndDate();

							if(pItem->GetShopItemPriceCount())
							{
								SHOPITEMPRICE_LIST priceList = pItem->GetShopItemPriceList();
								SHOPITEMPRICE_LIST::iterator itPrice;

								for(i = 0, itPrice = priceList.begin(); itPrice != priceList.end(); ++itPrice)
								{
									pPrice = *itPrice;
									if(pPrice)
									{
										if(i >= _MAX_PRICE_INFO)
											break;

										stItemInfo->stPrice[i].priceGuid = pPrice->GetPriceGuid();
										stItemInfo->stPrice[i].dwAmount = pPrice->GetAmount();
										stItemInfo->stPrice[i].dwUseTime = pPrice->GetUseTime();
										stItemInfo->stPrice[i].dwPrice = pPrice->GetPrice();
										stItemInfo->stPrice[i].bySaleRatio = pPrice->GetSaleRatio();
										++i;

										__asm nop;
									}
								}
							}
							if( GetCashShopDlgState() == eCashItemShopDlgState_CashShop )
							{
								if ((byCategory == CT_PERIOD_EXTEND) || (byCategory == SHOP_CATEGORY_PERIOD_EXTEND))
									ShowCashItemShopExtensionBuy(m_iPendingItemIndex);
								else
									ShowCashItemShopBuy(m_iPendingItemIndex);
							}
							else if( GetCashShopDlgState() == eCashItemShopDlgState_Mileage )
								ShowCashItemShopMileageBuy(m_iPendingItemIndex);
							break;
						}
					}
				}

			}
			break;
		}
		++iIndex;
	}
}
#else	//	__NA_00387_20070605_SHOP_MILEAGE2
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::RefreshItemPrice()
{
	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	int i, j;

	int iStartIndex = _ITEM_NUM_PER_PAGE * m_pUICashItemShopDlg->GetCurPage();
	int iIndex = 0;

		
	ShopCategory* pShopCategory = NULL;

	eSHOP_CATEGORY byCategory;

	if ((m_byPendingCategory == CT_PERIOD_EXTEND) || (m_byPendingCategory == SHOP_CATEGORY_PERIOD_EXTEND))
		byCategory = (eSHOP_CATEGORY)m_byPendingCategory;
	else
		byCategory = (eSHOP_CATEGORY)m_pUICashItemShopDlg->GetCurCategoryType();

	m_pUICashItemShopDlg->RefreshItemPrice();

    iStartIndex = 0;

    SHOPITEM_LIST shopItemList;
    for (int kk = 0; kk < current_show_item_list()->guid_count; ++kk)
    {
        ShopItem* shop_item = m_ShopManager.FindShopItem(current_show_item_list()->shopitem_guid[kk]);
        if (shop_item != NULL)
        {
            shopItemList.push_back(shop_item);
        }
    }

	SHOPITEM_LIST::iterator it;

	SingleItem* pItem = NULL;
	ShopItemPrice* pPrice = NULL;

#ifdef _DEBUG
	ZeroMemory(szDebug, sizeof(szDebug));
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("shopItemList SizeNum : %d \n"), shopItemList.size());
	OutputDebugString(szDebug);
#endif

	for(it = shopItemList.begin(); it != shopItemList.end(); ++it)
	{
		if((*it)->GetGuid() == m_iPendingItemGuid)
		{
			pItem = (SingleItem *)*it;

			if(pItem)
			{
				if(iStartIndex <= iIndex)
				{
					for(j = 0; j < _ITEM_NUM_PER_PAGE; j++)
					{

						stCashItemInfo* stItemInfo;
						if ((byCategory == CT_PERIOD_EXTEND) || (byCategory == SHOP_CATEGORY_PERIOD_EXTEND))
							stItemInfo = &m_stExtensionItemInfo[j];
						else
							stItemInfo = &m_stCashItemInfo[j];

						if(stItemInfo->itemguid == m_iPendingItemGuid)
						{
							stItemInfo->moneyType = pItem->GetMoneyType();
							stItemInfo->dwCoolTime = pItem->GetCoolTime();
							stItemInfo->SellEndDate = pItem->GetSellEndDate();

							if(pItem->GetShopItemPriceCount())
							{
								SHOPITEMPRICE_LIST priceList = pItem->GetShopItemPriceList();
								SHOPITEMPRICE_LIST::iterator itPrice;

								for(i = 0, itPrice = priceList.begin(); itPrice != priceList.end(); ++itPrice)
								{
									pPrice = *itPrice;
									if(pPrice)
									{
										if(i >= _MAX_PRICE_INFO)
										{
											break;
										}

										stItemInfo->stPrice[i].priceGuid = pPrice->GetPriceGuid();
										stItemInfo->stPrice[i].dwAmount = pPrice->GetAmount();
										stItemInfo->stPrice[i].dwUseTime = pPrice->GetUseTime();
										stItemInfo->stPrice[i].dwPrice = pPrice->GetPrice();
										stItemInfo->stPrice[i].bySaleRatio = pPrice->GetSaleRatio();
										++i;

										__asm nop;
									}
								}
							}
							if ((byCategory == CT_PERIOD_EXTEND) || (byCategory == SHOP_CATEGORY_PERIOD_EXTEND))
							{
								ShowCashItemShopExtensionBuy(m_iPendingItemIndex);
							}
							else
							{
								ShowCashItemShopBuy(m_iPendingItemIndex);
							}
							break;
						}

					}
				}
			}
			break;
		}
		++iIndex;
	}
}
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

//------------------------------------------------------------------------------

void uiCashItemShopMan::RefreshItemList(MSG_CG_SHOP_ITEM_LIST_REQ_ACK* packet)
{
    m_ShopManager.Serialize((eSHOP_PARTITION)packet->partition_index, 
                            (eSHOP_CATEGORY)packet->category_index, 
                            packet->buffer, 
                            packet->buffer_size);

    ZeroMemory(&current_show_item_list_, sizeof(current_show_item_list_));
    memcpy(&current_show_item_list_, packet, packet->GetSize());

    if ((packet->category_index == CT_PERIOD_EXTEND) || 
        (packet->category_index == SHOP_CATEGORY_PERIOD_EXTEND))
    {
        RefreshExtensionItemList();
    }
    else if (packet->partition_index == SHOP_PARTITION_SINGLEITEM)
    {
        RefreshSingleItemList();
        m_pUICashItemShopDlg->m_iCurPage = packet->answer_page;
        m_pUICashItemShopDlg->SetCurPage();
    }
    else if (packet->partition_index == SHOP_PARTITION_PACKAGEITEM)
    {
        RefreshPackageItemList();
        m_pUiCashItemShopPackageDlg->m_iCurPage = packet->answer_page;
        m_pUiCashItemShopPackageDlg->SetCurPage();
    }
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    else if (packet->partition_index == SHOP_PARTITION_MILEAGEITEM)
    {
        RefreshSingleItemList();
        m_pUICashItemShopMileageDlg->m_iCurPage = packet->answer_page;
        m_pUICashItemShopMileageDlg->SetCurPage();
    }
#endif //__NA_00387_20070605_SHOP_MILEAGE2
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::RefreshPackageItemList()
{
	ShopCategory* pShopCategory = NULL;

	m_pUiCashItemShopPackageDlg->RefreshPackageItemList();
	ClearPackageItemInfo();

	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	int iStartIndex = _PACKAGE_ITEM_PER_PAGE * m_pUiCashItemShopPackageDlg->GetCurPage();
	int iCount = 0;
	int iIndex = 0;
	int i=0;

    iStartIndex = 0;
    SHOPITEM_LIST packageItemList;
    for (int kk = 0; kk < current_show_item_list()->guid_count; ++kk)
    {
        ShopItem* shop_item = m_ShopManager.FindShopItem(current_show_item_list()->shopitem_guid[kk]);
        if (shop_item != NULL)
        {
            packageItemList.push_back(shop_item);
        }
    }

	SHOPITEM_LIST::iterator it;

	//ShopItem* pItem
	PackageItem * pItem = NULL;
	ShopItemPrice* pPrice = NULL;

#ifdef _DEBUG
	ZeroMemory(szDebug, sizeof(szDebug));
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("packageItemList SizeNum : %d \n"), packageItemList.size());
	OutputDebugString(szDebug);
#endif

	for(it = packageItemList.begin(); it != packageItemList.end(); ++it)
	{
		pItem = (PackageItem *)*it;

		if(pItem)
		{
			if(_PACKAGE_ITEM_PER_PAGE <= iCount)
				return;
			
			if(iStartIndex <= iIndex)
			{
				m_stPackageItemInfo[iCount].itemguid			= pItem->GetGuid();
				m_stPackageItemInfo[iCount].dwImageCode			= pItem->GetImageCode();
				m_stPackageItemInfo[iCount].saleRatio			= pItem->GetShopItemPrice()->GetSaleRatio();
				m_stPackageItemInfo[iCount].moneyType			= pItem->GetMoneyType();
				m_stPackageItemInfo[iCount].dwCoolTime			= pItem->GetCoolTime();
				m_stPackageItemInfo[iCount].SellEndDate			= pItem->GetSellEndDate();
				m_stPackageItemInfo[iCount].dwPrice				= pItem->GetShopItemPrice()->GetPrice();
				m_stPackageItemInfo[iCount].priceGuid			= pItem->GetShopItemPrice()->GetPriceGuid();
				m_stPackageItemInfo[iCount].byDetailItemCount	= pItem->GetPieceCount();

				Snprintf(m_stPackageItemInfo[iCount].szName, MAX_PACKAGE_NAME_LENGTH, pItem->GetName());

				for(i= 0; i < pItem->GetPieceCount() && i < MAX_PACKAGE_ITEM_SIZE; ++i)
				{
					m_stPackageItemInfo[iCount].stDetailItem[i].dwItemCode	= pItem->GetItemCode(i);
					m_stPackageItemInfo[iCount].stDetailItem[i].Aoumt		= pItem->GetAmount(i);
					m_stPackageItemInfo[iCount].stDetailItem[i].dwUseTime	= pItem->GetUseTime(i);
					m_stPackageItemInfo[iCount].stDetailItem[i].byEnchant	= pItem->GetEnchant(i);
					m_stPackageItemInfo[iCount].stDetailItem[i].byRank		= pItem->GetRank(i);
				}	

				m_pUiCashItemShopPackageDlg->SetItem(iCount, m_stPackageItemInfo[iCount].dwImageCode,
					m_stPackageItemInfo[iCount].szName);
				++iCount;
			}
			++iIndex;
		}
	}

}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::RefreshExtensionItemList()
{
	ShopCategory* pShopCategory = NULL;

	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	int iStartIndex = 0;
	int iCount = 0;
	int iIndex = 0;

    iStartIndex = 0;
    SHOPITEM_LIST shopItemList;
    for (int kk = 0; kk < current_show_item_list()->guid_count; ++kk)
    {
        ShopItem* shop_item = m_ShopManager.FindShopItem(current_show_item_list()->shopitem_guid[kk]);
        if (shop_item != NULL)
        {
            shopItemList.push_back(shop_item);
        }
    }

	SHOPITEM_LIST::iterator it;

	SingleItem * pItem = NULL;
	ShopItemPrice* pPrice = NULL;

#ifdef _DEBUG
	ZeroMemory(szDebug, sizeof(szDebug));
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("shopItemList SizeNum : %d \n"), shopItemList.size());
	OutputDebugString(szDebug);
#endif

	for(it = shopItemList.begin(); it != shopItemList.end(); ++it)
	{
		pItem = (SingleItem *)*it;
		if(pItem)
		{
			if(_ITEM_NUM_PER_PAGE <= iCount)
				return;

			if(iStartIndex <= iIndex)
			{
				m_stExtensionItemInfo[iCount].itemguid			=  pItem->GetGuid();
				m_stExtensionItemInfo[iCount].dwItemCode		= pItem->GetItemCode();
				m_stExtensionItemInfo[iCount].byEnchant			= pItem->GetEnchant();
				m_stExtensionItemInfo[iCount].byRank			= pItem->GetRank();

				if(pItem->GetShopItemPriceCount())
				{
					SHOPITEMPRICE_LIST priceList = pItem->GetShopItemPriceList();
					SHOPITEMPRICE_LIST::iterator itPrice;

					m_stExtensionItemInfo[iCount].moneyType		= pItem->GetMoneyType();
					m_stExtensionItemInfo[iCount].dwCoolTime	= pItem->GetCoolTime();
					m_stExtensionItemInfo[iCount].SellEndDate	= pItem->GetSellEndDate();

					int i=0;
					for(itPrice = priceList.begin(); itPrice != priceList.end(); ++itPrice)
					{
						pPrice = *itPrice;
						if(pPrice)
						{
							if(i>=_MAX_PRICE_INFO)
								break;

							m_stExtensionItemInfo[iCount].stPrice[i].priceGuid = pPrice->GetPriceGuid();
							m_stExtensionItemInfo[iCount].stPrice[i].dwAmount = pPrice->GetAmount();
							m_stExtensionItemInfo[iCount].stPrice[i].dwUseTime = pPrice->GetUseTime();
							m_stExtensionItemInfo[iCount].stPrice[i].dwPrice = pPrice->GetPrice();
							m_stExtensionItemInfo[iCount].stPrice[i].bySaleRatio = pPrice->GetSaleRatio();
							++i;

							__asm nop;

						}
					}	
				}
				++iCount;
			}
			++iIndex;
		}
	}

	ShowCashItemShopExtensionBuy(true);
}

//------------------------------------------------------------------------------
/**
*/
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
void uiCashItemShopMan::RefreshSingleItemList()
{
	ShopCategory* pShopCategory = NULL;

	uiCashItemShopBase * pCashItemShopBase = NULL;

	if( GetCashShopDlgState() == eCashItemShopDlgState_CashShop )
		pCashItemShopBase = static_cast<uiCashItemShopBase *>(m_pUICashItemShopDlg);
	else if( GetCashShopDlgState() == eCashItemShopDlgState_Mileage )
		pCashItemShopBase = static_cast<uiCashItemShopBase *>(m_pUICashItemShopMileageDlg);

	if(pCashItemShopBase == NULL)	return;

	pCashItemShopBase->RefreshSingleItemList();
	ClearCashItemInfo();

	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	int iStartIndex = _ITEM_NUM_PER_PAGE * pCashItemShopBase->GetCurPage();
	int iCount = 0;
	int iIndex = 0;

    iStartIndex = 0;
    SHOPITEM_LIST shopItemList;
    for (int kk = 0; kk < current_show_item_list()->guid_count; ++kk)
    {
        ShopItem* shop_item = m_ShopManager.FindShopItem(current_show_item_list()->shopitem_guid[kk]);
        if (shop_item != NULL)
        {
            shopItemList.push_back(shop_item);
        }
    }

	SHOPITEM_LIST::iterator it;

	SingleItem * pItem = NULL;
	ShopItemPrice* pPrice = NULL;

#ifdef _DEBUG
	ZeroMemory(szDebug, sizeof(szDebug));
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("shopItemList SizeNum : %d \n"), shopItemList.size());
	OutputDebugString(szDebug);
#endif

	for(it = shopItemList.begin(); it != shopItemList.end(); ++it)
	{
		pItem = (SingleItem *)*it;
		if(pItem)
		{
			if(_ITEM_NUM_PER_PAGE <= iCount)
				return;

			if(iStartIndex <= iIndex)
			{
				m_stCashItemInfo[iCount].itemguid	=  pItem->GetGuid();
				m_stCashItemInfo[iCount].dwItemCode = pItem->GetItemCode();
				m_stCashItemInfo[iCount].byEnchant	= pItem->GetEnchant();
				m_stCashItemInfo[iCount].byRank		= pItem->GetRank();


				if(pItem->GetShopItemPriceCount())
				{
					SHOPITEMPRICE_LIST priceList = pItem->GetShopItemPriceList();
					SHOPITEMPRICE_LIST::iterator itPrice;

					m_stCashItemInfo[iCount].moneyType = pItem->GetMoneyType();
					m_stCashItemInfo[iCount].dwCoolTime = pItem->GetCoolTime();
					m_stCashItemInfo[iCount].SellEndDate = pItem->GetSellEndDate();

					int i=0;
					for(itPrice = priceList.begin(); itPrice != priceList.end(); ++itPrice)
					{
						pPrice = *itPrice;
						if(pPrice)
						{
							if(i>=_MAX_PRICE_INFO)
								break;

							m_stCashItemInfo[iCount].stPrice[i].priceGuid = pPrice->GetPriceGuid();
							m_stCashItemInfo[iCount].stPrice[i].dwAmount = pPrice->GetAmount();
							m_stCashItemInfo[iCount].stPrice[i].dwUseTime = pPrice->GetUseTime();
							m_stCashItemInfo[iCount].stPrice[i].dwPrice = pPrice->GetPrice();
							m_stCashItemInfo[iCount].stPrice[i].bySaleRatio = pPrice->GetSaleRatio();
							++i;

							__asm nop;

						}
					}	
				}
				pCashItemShopBase->SetItem(iCount, m_stCashItemInfo[iCount].dwItemCode);
				++iCount;
			}
			++iIndex;
		}
	}
}
#else	//	__NA_00387_20070605_SHOP_MILEAGE2

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::RefreshSingleItemList()
{
	m_pUICashItemShopDlg->RefreshSingleItemList();
	ClearCashItemInfo();

	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	int iStartIndex = _ITEM_NUM_PER_PAGE * m_pUICashItemShopDlg->GetCurPage();
	int iCount = 0;
	int iIndex = 0;

    iStartIndex = 0;
    SHOPITEM_LIST shopItemList;
    for (int kk = 0; kk < current_show_item_list()->guid_count; ++kk)
    {
        ShopItem* shop_item = m_ShopManager.FindShopItem(current_show_item_list()->shopitem_guid[kk]);
        if (shop_item != NULL)
        {
            shopItemList.push_back(shop_item);
        }
    }

	SHOPITEM_LIST::iterator it;
	SingleItem * pItem = NULL;
	ShopItemPrice* pPrice = NULL;

#ifdef _DEBUG
	ZeroMemory(szDebug, sizeof(szDebug));
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("shopItemList SizeNum : %d \n"), shopItemList.size());
	OutputDebugString(szDebug);
#endif

	for(it = shopItemList.begin(); it != shopItemList.end(); ++it)
	{
		pItem = (SingleItem *)*it;
		if(pItem)
		{
			if(_ITEM_NUM_PER_PAGE <= iCount)
				return;
			
			if(iStartIndex <= iIndex)
			{
				m_stCashItemInfo[iCount].itemguid =  pItem->GetGuid();
				m_stCashItemInfo[iCount].dwItemCode = pItem->GetItemCode();
				m_stCashItemInfo[iCount].byEnchant	= pItem->GetEnchant();
				m_stCashItemInfo[iCount].byRank		= pItem->GetRank();

				if(pItem->GetShopItemPriceCount())
				{
					SHOPITEMPRICE_LIST priceList = pItem->GetShopItemPriceList();
					SHOPITEMPRICE_LIST::iterator itPrice;

					m_stCashItemInfo[iCount].moneyType = pItem->GetMoneyType();
					m_stCashItemInfo[iCount].dwCoolTime = pItem->GetCoolTime();
					m_stCashItemInfo[iCount].SellEndDate = pItem->GetSellEndDate();

					int i=0;
					for(itPrice = priceList.begin(); itPrice != priceList.end(); ++itPrice)
					{
						pPrice = *itPrice;
						if(pPrice)
						{
							if(i>=_MAX_PRICE_INFO)
								break;

							m_stCashItemInfo[iCount].stPrice[i].priceGuid = pPrice->GetPriceGuid();
							m_stCashItemInfo[iCount].stPrice[i].dwAmount = pPrice->GetAmount();
							m_stCashItemInfo[iCount].stPrice[i].dwUseTime = pPrice->GetUseTime();
							m_stCashItemInfo[iCount].stPrice[i].dwPrice = pPrice->GetPrice();
							m_stCashItemInfo[iCount].stPrice[i].bySaleRatio = pPrice->GetSaleRatio();
							++i;

							__asm nop;

						}
					}	
				}

				m_pUICashItemShopDlg->SetItem(iCount, m_stCashItemInfo[iCount].dwItemCode);
				++iCount;
			}
			++iIndex;
		}
	}
}
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::RefreshPresentItemList()
{
	int i;
	int iCount = 0;
	int iIndex = 0;
	int iCurIndex = m_pUiCashItemShopPresentDlg->GetCurPage() * _PRESENT_ITEM_PER_PAGE;
	PresentRecvBox::PresentInfo* pPresent = NULL;

	PresentRecvBox* pPresentRecvBox = GetPresentRcvBox();
							if(!pPresentRecvBox)
								return;

	m_pUiCashItemShopPresentDlg->RefreshPresentItemList();
	ClearPresentItemInfo();

	PresentRecvBox::GIFT_HASH & rHash = pPresentRecvBox->GetHash();
	PresentRecvBox::GIFT_HASH::iterator it = rHash.begin(), end = rHash.end();
	for( ; it != end ; ++it )
	{
		if( iIndex >= iCurIndex)
		{
			pPresent = (PresentRecvBox::PresentInfo*)(*it);
			if(pPresent)
			{
				m_stPresentItemInfo[iCount].giftGuid		= pPresent->GetGiftGuid();
				m_stPresentItemInfo[iCount].SendDateTime	= pPresent->GetSendDateTime();
				m_stPresentItemInfo[iCount].byPresentState	= pPresent->GetState();
				m_stPresentItemInfo[iCount].priceGuid		= pPresent->GetPriceGuid();

				Snprintf(m_stPresentItemInfo[iCount].strPlayerName, MAX_CHARNAME_LENGTH, _T("%s"), pPresent->GetSendCharName());
				Snprintf(m_stPresentItemInfo[iCount].strSendMessage, MAX_GIFT_MESSAGE_LENGTH, _T("%s"), pPresent->GetSendMessage());
				
				m_stPresentItemInfo[iCount].iPieceCount		= pPresent->GetPieceCount();
				for(i = 0; i < m_stPresentItemInfo[iCount].iPieceCount; ++i)
				{
					m_stPresentItemInfo[iCount].stPiece[i].dwUseTime	= pPresent->m_ItemPieces[i].UseTime;
					m_stPresentItemInfo[iCount].stPiece[i].dwAmount		= pPresent->m_ItemPieces[i].Amount;
					m_stPresentItemInfo[iCount].stPiece[i].dwItemCode	= pPresent->m_ItemPieces[i].ItemCode;
					m_stPresentItemInfo[iCount].stPiece[i].byEnchant	= pPresent->GetEnchant(i);
					m_stPresentItemInfo[iCount].stPiece[i].byRank		= pPresent->GetRank(i);
				}

				if(_PRESENT_ITEM_PER_PAGE <= iCount)
				{
					return;
				}
				else
				{
					m_pUiCashItemShopPresentDlg->SetItem(iCount, m_stPresentItemInfo[iCount].stPiece[0].dwItemCode);
					++iCount;
				}
			}
		}
		iIndex++;
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::PrepareShow(int iCashShopDlgState)
{
	SetCashShopDlgState(iCashShopDlgState);

	ClearDlg();

	switch(m_iCashShopDlgState)
	{
	case eCashItemShopDlgState_CashShop:    //!<uiCashItemShopDlg
		{
			ShowCashItemShopNpcPopup(false);
			ShowCashItemShopBuyCart(false);
			ShowCashItemShopPresentDlg(false);
			ShowCashItemShopPackageDlg(false);
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
			ShowCashItemShopMileageDlg(false);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
		}
		break;

	case eCashItemShopDlgState_ShopBasket:
		break;
	case eCashItemShopDlgState_Present: //!<uiCashItemShopPresentDlg
		{
			ShowCashItemShopNpcPopup(false);
			ShowCashItemShopBuyCart(false);
			ShowCashItemShopDlg(false);
			ShowCashItemShopPackageDlg(false);
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
			ShowCashItemShopMileageDlg(false);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
		}
		break;

	case eCashItemShopDlgState_Package: //!<uiCashItemShopPackageDlg
		{
			ShowCashItemShopNpcPopup(false);
			ShowCashItemShopBuyCart(false);
			ShowCashItemShopDlg(false);
			ShowCashItemShopPresentDlg(false);
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
			ShowCashItemShopMileageDlg(false);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
		}
		break;
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	case eCashItemShopDlgState_Mileage: //!<uiCashItemShopMileageDlg
		{
			ShowCashItemShopDlg(false);
			ShowCashItemShopNpcPopup(false);
			ShowCashItemShopBuyCart(false);
			ShowCashItemShopPresentDlg(false);
			ShowCashItemShopPackageDlg(false);
		}
		break;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	}
}


//------------------------------------------------------------------------------
/**
*/
bool uiCashItemShopMan::IsRcvCashShopInfo(BYTE byCategory, BYTE byPage)
{
	for(unsigned int i=0; i<this->m_vecCashShopRcvInfo.size(); ++i)
	{
		if( (m_vecCashShopRcvInfo[i].byCategory == byCategory) && 
            (m_vecCashShopRcvInfo[i].byPage == byPage))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//------------------------------------------------------------------------------
/**
*/
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
BOOL uiCashItemShopMan::IsRcvMileageShopInfo(BYTE byCategory, BYTE byPage)
{
	for(size_t i=0; i<this->m_vecMileageShopRcvInfo.size(); ++i)
	{
		if ((m_vecMileageShopRcvInfo[i].byCategory == byCategory) && 
            (m_vecMileageShopRcvInfo[i].byPage == byPage))
		{
			return TRUE;
		}
	}

	return FALSE;
}
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

//------------------------------------------------------------------------------
/**
*/

BOOL uiCashItemShopMan::IsRcvExtensionShopInfo(BYTE byCategory, BYTE byPage)
{
	for(unsigned int i=0; i<this->m_vecExtensionShopRcvInfo.size(); ++i)
	{
		if ((m_vecExtensionShopRcvInfo[i].byCategory == byCategory) && 
            (m_vecExtensionShopRcvInfo[i].byPage == byPage))
		{
			return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
bool uiCashItemShopMan::IsRcvPackageInfo(BYTE byCategory, BYTE byPage)
{
	for(unsigned int i=0; i<this->m_vecPackageRcvInfo.size(); ++i)
	{
        if((m_vecPackageRcvInfo[i].byCategory == byCategory) && 
            (m_vecPackageRcvInfo[i].byPage == byPage))
        {
            return true;
        }
	}

	return false;
}


//------------------------------------------------------------------------------
/**
*/
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
int uiCashItemShopMan::GetMileageTotalPage(BYTE byCategory)
{
	for(size_t i=0; i<this->m_vecMileageShopRcvInfo.size(); ++i)
	{
		if((m_vecMileageShopRcvInfo[i].byCategory == byCategory))
		{
			return m_vecMileageShopRcvInfo[i].iTotalPage;
		}
	}
	return -1;
}
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

//------------------------------------------------------------------------------
/**
*/
int uiCashItemShopMan::GetExtensionTotalPage(BYTE byCategory)
{
	for(unsigned int i=0; i<this->m_vecExtensionShopRcvInfo.size(); ++i)
	{
		if((m_vecExtensionShopRcvInfo[i].byCategory == byCategory))
		{
			return m_vecExtensionShopRcvInfo[i].iTotalPage;
		}
	}
	return -1;
}

//------------------------------------------------------------------------------
/**
*/
int uiCashItemShopMan::GetCashShopTotalPage(BYTE byCategory)
{
	for(unsigned int i=0; i<this->m_vecCashShopRcvInfo.size(); ++i)
	{
		if((m_vecCashShopRcvInfo[i].byCategory == byCategory))
		{
			return m_vecCashShopRcvInfo[i].iTotalPage;
		}
	}

	return -1;
}
//------------------------------------------------------------------------------
/**
*/
int uiCashItemShopMan::GetPackageTotalPage(BYTE byCategory)
{
	for(unsigned int i=0; i<this->m_vecPackageRcvInfo.size(); ++i)
	{
		if((m_vecPackageRcvInfo[i].byCategory == byCategory))
		{
			return m_vecPackageRcvInfo[i].iTotalPage;
		}
	}

	return -1;
}

//------------------------------------------------------------------------------
/**
*/
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
void uiCashItemShopMan::SetMileageTotalPage(BYTE byCategory, int iTotalPage)
{
	--iTotalPage;
	if(iTotalPage <= 0) 
		iTotalPage =0;

	for(size_t i=0; i<this->m_vecMileageShopRcvInfo.size(); ++i)
	{
		if((m_vecMileageShopRcvInfo[i].byCategory == byCategory))
		{
			m_vecMileageShopRcvInfo[i].iTotalPage = iTotalPage;
		}
	}
}
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::SetExtensionTotalPage(BYTE byCategory, int iTotalPage)
{
	--iTotalPage;
	if(iTotalPage <= 0) 
		iTotalPage =0;

	for(unsigned int i=0; i<this->m_vecExtensionShopRcvInfo.size(); ++i)
	{
		if((m_vecExtensionShopRcvInfo[i].byCategory == byCategory))
		{
			m_vecExtensionShopRcvInfo[i].iTotalPage = iTotalPage;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::SetCashShopTotalPage(BYTE byCategory, int iTotalPage)
{
	--iTotalPage;
	if(iTotalPage <= 0) 
		iTotalPage =0;

	for(unsigned int i=0; i<this->m_vecCashShopRcvInfo.size(); ++i)
	{
		if((m_vecCashShopRcvInfo[i].byCategory == byCategory))
		{
			m_vecCashShopRcvInfo[i].iTotalPage = iTotalPage;
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::SetPackageTotalPage(BYTE byCategory, int iTotalPage)
{
	--iTotalPage;
	if(iTotalPage <= 0) 
		iTotalPage =0;

	for(unsigned int i=0; i<this->m_vecPackageRcvInfo.size(); ++i)
	{
		if((m_vecPackageRcvInfo[i].byCategory == byCategory))
		{
			m_vecPackageRcvInfo[i].iTotalPage = iTotalPage;
		}
	}
}



int uiCashItemShopMan::GetRqstState()
{
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	int		stringIndex = 0;
	switch(m_iRqstState)
	{
	case eRqstState_Buy:
	case eRqstState_Package:
		{
			//85197	구매 요청중입니다.
			stringIndex = 85197;
		}
		break;
	case eRqstState_Present_Send:
		{
			//5971	선물보내기 요청중입니다.
			stringIndex = 5971;
		}
		break;
	case eRqstState_Present_Rcv:
		{
			//5972 돌려보내기 요청중입니다.
			stringIndex = 5972;
		}
		break;
	case eRqstState_Present_Discard:
		{
			//5973 목록 삭제 요청중입니다.
			stringIndex = 5973;
		}
		break;
	}
	if(stringIndex)
	{
		g_InterfaceManager.GetInterfaceString(stringIndex, szMessage, INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_SysMsg(szMessage);
	}
	return m_iRqstState;
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::SetItemTipListPCBangDesc(BASE_ITEMINFO * pItemInfo)
{

	int		i;
	int		iStringIndex = 0;
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp1[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szTemp3[INTERFACE_STRING_LENGTH] = {0,};

	if( pItemInfo->IsAccessory() || pItemInfo->IsSpecialAccessory())
	{
		for(i = 0; pItemInfo->m_wPCOptionIndex[i] > 0 && i < BASE_ITEMINFO::_MAX_OPTION_NUM; ++i)
		{
			stColorStr	colorStr;
			BASE_OptionList* pOption = OptionListParser::Instance()->GetOptionList(pItemInfo->m_wPCOptionIndex[i]);
			if(pOption)
				iStringIndex = pOption->mdwNameCode;

			if(iStringIndex)
			{
				g_InterfaceManager.GetInterfaceString( pOption->mdwNameCode, szTemp1, INTERFACE_STRING_LENGTH );
				if( g_pHero && g_pHero->GetPCBangMode() )
				{
					colorStr.txtColor = c_CashItemShopListPcBangColor;
				}
				else
				{
					colorStr.txtColor = c_CashItemShopListNonPcBangColor;
				}

				g_InterfaceManager.GetInterfaceString(1099, szMessage ,INTERFACE_STRING_LENGTH);
				Snprintf( colorStr.szStr ,INTERFACE_STRING_LENGTH-1, szMessage, szTemp1, pItemInfo->m_iPCOptionValue[i]);

                //옵션명 접미사
                GlobalFunc::AddOptionSuffix(pItemInfo->m_byPCOptionType[i], pOption, colorStr.szStr);

				m_vstrColorStr.push_back(colorStr);
			}
		}
	}
	else
	{
		for(i = 0; pItemInfo->m_wPCOptionIndex[i] > 0 && i < BASE_ITEMINFO::_MAX_OPTION_NUM; ++i)
		{
			stColorStr	colorStr;
			BASE_OptionList* pOption = OptionListParser::Instance()->GetOptionList(pItemInfo->m_wPCOptionIndex[i]);
			if(pOption)
				iStringIndex = pOption->mdwNameCode;

			if(iStringIndex)
			{
				g_InterfaceManager.GetInterfaceString( pOption->mdwNameCode, szTemp2, INTERFACE_STRING_LENGTH );

				Snprintf( szTemp3,INTERFACE_STRING_LENGTH-1,szTemp2, pItemInfo->m_iPCOptionValue[i]);

				if( g_pHero && g_pHero->GetPCBangMode() )
				{
					colorStr.txtColor = c_CashItemShopListPcBangColor;
				}
				else
				{
					colorStr.txtColor = c_CashItemShopListNonPcBangColor;
				}
				g_InterfaceManager.GetInterfaceString(1098 , szMessage,INTERFACE_STRING_LENGTH);
				Snprintf( colorStr.szStr, INTERFACE_STRING_LENGTH-1, szMessage, "", szTemp3);

                //옵션명 접미사
                GlobalFunc::AddOptionSuffix(pItemInfo->m_byPCOptionType[i], pOption, colorStr.szStr);

				m_vstrColorStr.push_back(colorStr);
			}
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::SetItemTipListEqLimitDesc(BASE_ITEMINFO * pItemInfo)
{
	TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};

	// 착용 제한
	if (pItemInfo->m_wEqClass1 && 
		pItemInfo->m_wEqClass2 && 
		pItemInfo->m_wEqClass3 && 
		pItemInfo->m_wEqClass4 && 
        pItemInfo->m_wEqClass5 && 
        pItemInfo->m_wEqClass6 && 
        pItemInfo->m_wEqClass7 && 
		pItemInfo->m_wEqClass8)
	{
		// 모두 허용이면 하지마
	}
	else
	{
		int		iClassNameCode[eCHAR_TYPE_MAX] = {
			eST_LIMITCLASS_BER_STRING_INDEX,
				eST_LIMITCLASS_DRA_STRING_INDEX,
				eST_LIMITCLASS_SHA_STRING_INDEX,
				eST_LIMITCLASS_VAL_STRING_INDEX,
				eST_LIMITCLASS_ELM_STRING_INDEX,
                eST_LIMITCLASS_MYS_STRING_INDEX, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                eST_LIMITCLASS_HEL_STRING_INDEX, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                eST_LIMITCLASS_WIT_STRING_INDEX, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		};

		WORD	wCharEqClass[eCHAR_TYPE_MAX] = {
				pItemInfo->m_wEqClass1,
				pItemInfo->m_wEqClass2,
				pItemInfo->m_wEqClass3,
				pItemInfo->m_wEqClass4,
				pItemInfo->m_wEqClass5,
                pItemInfo->m_wEqClass6, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                pItemInfo->m_wEqClass7, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                pItemInfo->m_wEqClass8, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		};

		stColorStr	colorStr;

		//155372	착용(사용) 제한
		g_InterfaceManager.GetInterfaceString( 155372 , colorStr.szStr ,INTERFACE_STRING_LENGTH);

		for( int i = 0 ; i < eCHAR_TYPE_MAX ; ++i )
		{
			//	클래스 제한 있는 캐릭터 이름들 나열
			if( wCharEqClass[i] )
			{
				g_InterfaceManager.GetInterfaceString( iClassNameCode[i], szMessage, INTERFACE_STRING_LENGTH );

				Snprintf(szTemp,INTERFACE_STRING_LENGTH-1,_T("%s "), szMessage );
				_tcscat(colorStr.szStr, szTemp);

			}
		}
		colorStr.txtColor = c_CashItemShopListNormalColor;
		m_vstrColorStr.push_back(colorStr);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::SetItemTipListBuyCoolTimeDesc(DWORD	dwCoolTime)
{
	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
	if(dwCoolTime)
	{
		stColorStr	colorStr;
		int iHour = dwCoolTime / 60;
		int iMin = dwCoolTime % 60;

		if(iHour && iMin)
		{
			// 155373	구매 쿨타임: %d시간 %d분
			g_InterfaceManager.GetInterfaceString( 155373 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(colorStr.szStr, INTERFACE_STRING_LENGTH-1, szTemp , iHour, iMin );
		}
		else if(iHour)
		{
			// 155374	구매 쿨타임: %d시간
			g_InterfaceManager.GetInterfaceString( 155374 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(colorStr.szStr, INTERFACE_STRING_LENGTH-1, szTemp , iHour);
		}
		else if(iMin)
		{
			// 155375	구매 쿨타임: %d분
			g_InterfaceManager.GetInterfaceString( 155375 , szTemp ,INTERFACE_STRING_LENGTH);
			Snprintf(colorStr.szStr, INTERFACE_STRING_LENGTH-1, szTemp , iMin );
		}
		colorStr.txtColor = c_CashItemShopListTextColor;
		m_vstrColorStr.push_back(colorStr);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::SetItemtipListTradeInDesc(BASE_ITEMINFO * pItemInfo)
{
    if (SCItemSlot::CanTradeSell(pItemInfo, eITEM_TRADESELL_DONTALL) == false)
	{
		bool		bStart = false;
		stColorStr	colorStr;
		TCHAR	szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR	szTemp[INTERFACE_STRING_LENGTH] = {0,};

		ZeroMemory(colorStr.szStr, sizeof(colorStr.szStr));
		colorStr.txtColor = c_CashItemShopListTradeInErrorColor;

        if (SCItemSlot::CanTradeSell(pItemInfo, eITEM_TRADESELL_DONTALL))
		{

			//거래, 판매, 바닥에 버리기, 계정내 캐릭터 공유(슬롯이동), 삭제 불가능
			g_InterfaceManager.GetInterfaceString(1349, colorStr.szStr, INTERFACE_STRING_LENGTH);
			bStart = true;
		}
		else
		{
            if (SCItemSlot::CanTradeSell(pItemInfo, eITEM_TRADESELL_TRADE) == false)
			{
				//거래		21005 
				//거래 불가능
				ZeroMemory(szTemp, sizeof(szTemp));
				ZeroMemory(szMessage, sizeof(szMessage));

				g_InterfaceManager.GetInterfaceString(21005, szMessage, INTERFACE_STRING_LENGTH);
				Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), szMessage);

				_tcscat( colorStr.szStr, szTemp );
				bStart = true;
			}

            if (SCItemSlot::CanTradeSell(pItemInfo, eITEM_TRADESELL_SELL) == false)
			{
				//판매 불가능

				//판매		124 
				ZeroMemory(szTemp, sizeof(szTemp));
				ZeroMemory(szMessage, sizeof(szMessage));

				g_InterfaceManager.GetInterfaceString(124, szMessage, INTERFACE_STRING_LENGTH);

				if(bStart)
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T(",%s"), szMessage);
				else
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), szMessage);

				_tcscat( colorStr.szStr, szTemp );
				bStart = true;
			}

            if (SCItemSlot::CanTradeSell(pItemInfo, eITEM_TRADESELL_DROP) == false)
			{
				//바닥에 버리기 불가능
				//바닥에 버리기	1345
				ZeroMemory(szTemp, sizeof(szTemp));
				ZeroMemory(szMessage, sizeof(szMessage));

				g_InterfaceManager.GetInterfaceString(1345, szMessage, INTERFACE_STRING_LENGTH);
				if(bStart)
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T(",%s"), szMessage);
				else
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), szMessage);

				_tcscat( colorStr.szStr, szTemp );
				bStart = true;
			}

            if (SCItemSlot::CanTradeSell(pItemInfo, eITEM_TRADESELL_DELETE) == false)
			{
				//삭제		1346
				//삭제 불가능
				ZeroMemory(szTemp, sizeof(szTemp));
				ZeroMemory(szMessage, sizeof(szMessage));

				g_InterfaceManager.GetInterfaceString(1346, szMessage, INTERFACE_STRING_LENGTH);

				if(bStart)
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T(",%s"), szMessage);
				else
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), szMessage);

				_tcscat( colorStr.szStr, szTemp );
				bStart = true;
			}

            if (SCItemSlot::CanTradeSell(pItemInfo, eITEM_TRADESELL_DELETE) == false)
			{
				//슬롯이동 불가능 11026
				ZeroMemory(szTemp, sizeof(szTemp));
				ZeroMemory(szMessage, sizeof(szMessage));

				g_InterfaceManager.GetInterfaceString(11026, szMessage, INTERFACE_STRING_LENGTH);

				if(bStart)
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T(",%s"), szMessage);
				else
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), szMessage);

				_tcscat( colorStr.szStr, szTemp );
				bStart = true;
			}

            if (SCItemSlot::CanTradeSell(pItemInfo, eITEM_TRADESELL_WAREHOUSE) == false)
			{
				//창고 이동	1347
				ZeroMemory(szTemp, sizeof(szTemp));
				ZeroMemory(szMessage, sizeof(szMessage));

				g_InterfaceManager.GetInterfaceString(1347, szMessage, INTERFACE_STRING_LENGTH);

				if(bStart)
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T(",%s"), szMessage);
				else
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), szMessage);

				_tcscat( colorStr.szStr, szTemp );
				bStart = true;
			}

            if (SCItemSlot::CanTradeSell(pItemInfo, eITEM_TRADESELL_GUILDWAREHOUSE) == false)
			{
				//길드 창고 이동 1378
				ZeroMemory(szTemp, sizeof(szTemp));
				ZeroMemory(szMessage, sizeof(szMessage));

				g_InterfaceManager.GetInterfaceString(1378, szMessage, INTERFACE_STRING_LENGTH);

				if(bStart)
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T(",%s"), szMessage);
				else
					Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("%s"), szMessage);

				_tcscat( colorStr.szStr, szTemp );
				bStart = true;
			}

			if(bStart)
			{
				//불가능		1348
				ZeroMemory(szTemp, sizeof(szTemp));
				ZeroMemory(szMessage, sizeof(szMessage));

				g_InterfaceManager.GetInterfaceString(1348, szMessage, INTERFACE_STRING_LENGTH);
				Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T(" %s"), szMessage);

				_tcscat( colorStr.szStr, szTemp );
			}
		}
		m_vstrColorStr.push_back(colorStr);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::SetItemTipListDesc( BASE_ITEMINFO * pItemInfo, CCtrlListWZ* pTipList, int iTipListCurLine, DWORD dwCoolTime)
{
	if(!pTipList)
		return;

	ClearColorStr();

	//	PC방 옵션
	SetItemTipListPCBangDesc(pItemInfo);
	// 착용 제한
	if(pItemInfo->IsWeapon() || pItemInfo->IsArmor() || pItemInfo->IsSpecialAccessory())
		SetItemTipListEqLimitDesc(pItemInfo);
	//구매 쿨타임 
	SetItemTipListBuyCoolTimeDesc(dwCoolTime);
	// 계정내 공유, 거래, 판매, 바닥에 버리기, 삭제, 슬롯이동
	SetItemtipListTradeInDesc(pItemInfo);

	int iListIndex = -1;
	int iCurLine = iTipListCurLine;
	for (size_t i=0; i<m_vstrColorStr.size() && iCurLine <= 0; ++i, --iCurLine)
	{
		iListIndex = pTipList->InsertItemBack();

		GlobalFunc::SetItem(
			pTipList, 
			iListIndex, 0, m_vstrColorStr[i].szStr, 
			c_wiCashItemShopFont,
			DT_VCENTER | DT_LEFT, 
			c_CashItemShopListBKColor, 
			m_vstrColorStr[i].txtColor, 
			m_vstrColorStr[i].txtColor);
	}

}

void uiCashItemShopMan::ClearColorStr()
{
	for(unsigned int i = 0; m_vstrColorStr.size() < i; ++i)
	{
		ZeroMemory(m_vstrColorStr[i].szStr, sizeof(m_vstrColorStr[i].szStr));
	}
	m_vstrColorStr.clear();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::Send_CG_SHOP_ITEM_BUY_SYN(SLOTIDX slotIndex, POSTYPE slotPos)
{
	if(!this->m_bWaitPacket)
	{
		int iCount = 0;
		MSG_CG_SHOP_ITEM_BUY_SYN sync;
		if(GetCashShopDlgState() == eCashItemShopDlgState_Package)
		{
			sync.m_PriceGuid[0] = m_pendingBuyPackageIndex;
			iCount = 1;
		}
		else
		{
			for(int i=0; i<MSG_CG_SHOP_ITEM_BUY_SYN::_MAX_BUY_LIST_NUM; ++i)
			{
				if(this->m_pendingBuyList[i] != 0)
				{
					++iCount;
					sync.m_PriceGuid[i] = m_pendingBuyList[i];
				}
			}
		}

        if (CASH_ITEM_SHOP_PCASH == GetUseCashType())
        {
            sync.m_PurchaseType = MSG_CG_SHOP_ITEM_BUY_SYN::PURCHASE_PPCASH;
        }
        else
        { // 현재는 PPCash를 빼고는 다 Cash로 보낸다.
            sync.m_PurchaseType = MSG_CG_SHOP_ITEM_BUY_SYN::PURCHASE_DEFAULT;
        }
		sync.m_Count		= iCount;
		sync.m_SlotIndex	= slotIndex;
		sync.m_SlotPos		= slotPos;
		SetRqstState(eRqstState_Buy);
		GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sync.GetSize());
		m_bWaitPacket = true;
	}
}


	
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::_processErrorCode(BYTE byErrorCode, eOUTPUT_MSG_TYPE type, DWORD dwPriceGuid/* = 0*/)
{
	TCHAR szInterfaceString[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR szName[INTERFACE_STRING_LENGTH] ={0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};

	if(dwPriceGuid > 0)
	{
		ShopItemPrice * pPriceInfo = m_ShopManager.FindShopItemPrice(dwPriceGuid);
		if(pPriceInfo)
		{
			SingleItem* pShopItem = (SingleItem*)pPriceInfo->GetShopItem();
			if(pShopItem)
			{
				g_InterfaceManager.GetItemName(pShopItem->GetItemCode(), szName, INTERFACE_STRING_LENGTH);	// 아이템 이름
				//85204	[%s 구매 실패]
				g_InterfaceManager.GetInterfaceString(85204, szTemp, INTERFACE_STRING_LENGTH);
				Snprintf(szInterfaceString, INTERFACE_STRING_LENGTH-1, szTemp, szName);
			}
		}
	}

	switch(byErrorCode)
	{
	case RC_SHOP_SUCCESS:
		return;

	case RC_SHOP_FAILED:
		{
			//85205	아이템 샵: 실패했습니다.
			g_InterfaceManager.GetInterfaceString(85205, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_CANNOT_USE_SHOP:
		{
			if (GameFramework::GetCurScene() == SCENE_TYPE_FIELD )
			{
				//85206	필드에서는 아이템 샵을 이용할 수 없습니다.
				g_InterfaceManager.GetInterfaceString(85206, szMessage, INTERFACE_STRING_LENGTH);
			}
			else if((GameFramework::GetCurScene() >= SCENE_TYPE_LOBBY) && (GameFramework::GetCurScene() <= SCENE_TYPE_CHUNTING))
			{
				//85207	배틀존에서는 아이템 샵을 이용할 수 없습니다.
				g_InterfaceManager.GetInterfaceString(85207, szMessage, INTERFACE_STRING_LENGTH);
			}
			else 
			{
				//85208	지금은 아이템 샵을 이용할 수 없습니다.
				g_InterfaceManager.GetInterfaceString(85208, szMessage, INTERFACE_STRING_LENGTH);
			}
			
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_CANNOTBUYITEM:				//< 판매하지 않는 아이템입니다.
		{
			//85209	아이템 샵: 판매하지 않는 아이템입니다.
			g_InterfaceManager.GetInterfaceString(85209, szMessage, INTERFACE_STRING_LENGTH);
			
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_CANNOTBUYITEMPRICE:			//< 판매하지 않는 아이템 가격입니다.
		{
			//85210	아이템 샵: 판매하지 않는 아이템 가격입니다.
			g_InterfaceManager.GetInterfaceString(85210, szMessage, INTERFACE_STRING_LENGTH);
			
			_tcscat(szInterfaceString, szMessage);
		}	
		break;

	case RC_SHOP_USERISNOTFOUND:				//< 유저를 찾을 수 없습니다.
		{
			//155227	캐릭터명이 정확하지 않습니다.
			g_InterfaceManager.GetInterfaceString(155227, szMessage, INTERFACE_STRING_LENGTH);

			_tcscat(szInterfaceString, szMessage);

			if(eOUTPUT_MSG_CENTER_SHOW == type)
			{

				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_Verify(szInterfaceString);

			}
			return;
		}
		break;

	case RC_SHOP_SHOPSERVERDISCONNECTED:		//< 샵서버가 연결되어있지않습니다.(잠시뒤 이용해주세요)
		{
			//85212	아이템 샵: 서비스 준비중입니다.(잠시 후에 이용해주세요)
			g_InterfaceManager.GetInterfaceString(85212, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_NOTENOUGHCASH:				//< 캐쉬가 부족합니다.
		{
			//85213	아이템 샵: 캐시가 부족합니다.
			g_InterfaceManager.GetInterfaceString(85213, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_NOTENOUGHMONEY:				//< 하임이 부족합니다.
		{
			//85214	아이템 샵: 하임이 부족합니다.
			g_InterfaceManager.GetInterfaceString(85214, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_CANNOT_REJECT_GIFT_MYSELF:		//< 자기자신에게 선물을 반송할 수없습니다.
		{
			//5983	자기 자신에게는 선물을 보낼 수 없습니다.
			g_InterfaceManager.GetInterfaceString(5983, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	case RC_SHOP_NOTENOUGHMILEAGE:				//< 마일리지가 부족합니다.
		{
			// 155396	썬 포인트 샵 : 해방군의 훈장 개수가 부족합니다.
			g_InterfaceManager.GetInterfaceString(155396, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	case RC_SHOP_NOTENOUGHINVENTORYSPACE:	//< 인벤토리 공간이 부족합니다.
		{
			//85215	인벤토리가 가득 찼습니다
			g_InterfaceManager.GetInterfaceString(85215, szMessage, INTERFACE_STRING_LENGTH);

			_tcscat(szInterfaceString, szMessage);

			if(eOUTPUT_MSG_CENTER_SHOW == type)
			{
				uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSystemMan)
					pSystemMan->Sys_Verify(szInterfaceString);

				return;
			}
		}	
		break;

	case RC_SHOP_CRITICAL_ERROR:				//< 치명적인 오류입니다. 운영팀에 문의하세요!
		{
			//85216	아이템 샵: 치명적인 오류입니다. 운영팀에 문의하세요!
			g_InterfaceManager.GetInterfaceString(85216, szMessage, INTERFACE_STRING_LENGTH);

			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_INVALID_CHAR_NAME_LEN:		//< 캐릭터 이름 길이를 지원하지 않습니다.
		{
			//85217	아이템 샵: 캐릭터 이름이 잘못되었습니다.
			g_InterfaceManager.GetInterfaceString(85217, szMessage, INTERFACE_STRING_LENGTH);
			
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_STANDBY_SERVICE:			//< 샵 서비스 준비중입니다. 잠시뒤에 이용하세요.(정보쿼리중)
		{
			//85218	아이템 샵: 서비스 준비중입니다. 잠시 후에 이용하세요.
			g_InterfaceManager.GetInterfaceString(85218, szMessage, INTERFACE_STRING_LENGTH);

			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_ALREADY_BUYING_TRANSACTION:	//< 이미 구매 트랜잭션이 진행중입니다. 잠시뒤 이용하세요.
		{
			//85219	아이템 샵: 처리중입니다. 잠시 후에 요청해주세요.
			g_InterfaceManager.GetInterfaceString(85219, szMessage, INTERFACE_STRING_LENGTH);
			
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_SERVICE_STOP:
		{
			//85220	아이템 샵: 서비스 중지중입니다.(잠시 후에 이용해주세요)
			g_InterfaceManager.GetInterfaceString(85220, szMessage, INTERFACE_STRING_LENGTH);
			
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_CANNOTBUYBYLIMITCOUNT:		//< 구매 개수 제한을 넘어서서 구입할 수 없습니다.
		{
			//85221	아이템 샵: 구매 개수 제한을 넘어서서 구입할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(85221, szMessage, INTERFACE_STRING_LENGTH);
			
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_NOTEXPIREDCOOLTIME:			//< 구매 쿨타임이 지나지 않아서 구입할 수 없습니다.
		{
			//85224	아이템 샵: 구매 쿨타임이 지나지 않아서 구입할 수 없습니다.
			g_InterfaceManager.GetInterfaceString(85224, szMessage, INTERFACE_STRING_LENGTH);
			
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_OVER_BUY_COUNT:				//< 한번에 구매할 수 있는 개수를 넘어섰습니다.
		{
			g_InterfaceManager.GetInterfaceString(5974, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_INVALID_GIFT_GUID:				//< 유효하지 않는 선물 일련번호입니다.
		{
			g_InterfaceManager.GetInterfaceString(5979, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_CANNOT_OBTAIN_GIFT:			//< 선물을 받을 수 없습니다.
		{
			g_InterfaceManager.GetInterfaceString(5975, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_CANNOT_REJECT_GIFT:			//< 선물을 거절할 수 없습니다
		{
			g_InterfaceManager.GetInterfaceString(5976, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_CANNOT_DISCARD_GIFT:			//< 선물기록을 삭제할 수 없습니다.
		{
			g_InterfaceManager.GetInterfaceString(5977, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_CANNOT_FIND_CHARNAME:
		{
			//< 캐릭터 이름에 해당하는 캐릭터를 찾을 수 없습니다.
			//155227	캐릭터명이 정확하지 않습니다
			g_InterfaceManager.GetInterfaceString(155227, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
		
	case RC_SHOP_CANNOT_SEND_GIFT_MYSELF:		//< 자기자신에게 선물을 보낼 수 없습니다.
		{
			g_InterfaceManager.GetInterfaceString(5983, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	case RC_SHOP_CANNOT_PRESENT_ITEM:			//< 해당 아이템은 선물할 수 없다
		{
			g_InterfaceManager.GetInterfaceString(5984, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	
	//####### 북미 샵서버 관련된 메시지처리 추가 시작 ####### 
	case RC_SHOP_USERLEVEL_LOW:                          ///>  유저 레벨 이 낮아 선물하기 불가능
		{
#ifdef _RUSSIA
            g_InterfaceManager.GetInterfaceString(155547, szMessage, INTERFACE_STRING_LENGTH);
#else
            g_InterfaceManager.GetInterfaceString(7146, szMessage, INTERFACE_STRING_LENGTH);
#endif //_RUSSIA
			
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_INVALID_BILLING_PARAM:                  ///>  빌링 파라메터 에러
		{
			g_InterfaceManager.GetInterfaceString(7147, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_BILLING_NETWORK_ERROR:                  ///>  빌링 서버 네트워크 장애
		{
			g_InterfaceManager.GetInterfaceString(7148, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_BILLING_SERVER_LOGIC_ERROR:             ///>  빌링 서버 로직 장애
		{
			g_InterfaceManager.GetInterfaceString(7149, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_NOT_EXIST_BILLING_ITEM_CODE:            ///>  빌링 서버에 등록되지 않은 상품코드
		{
			g_InterfaceManager.GetInterfaceString(7150, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_NOT_EXIST_BILLING_TRADE_CODE:           ///>  빌링 서버에 등록되지 않은 거래구분코드
		{
			g_InterfaceManager.GetInterfaceString(7151, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_EXCEED_LIMIT_CHARGE:                    ///>  충전한도초과
		{
			g_InterfaceManager.GetInterfaceString(7152, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_ABNORMALITY_CHARGE_TRANSACTION:         ///>  비정상적인 충전 트랜잭션 
		{
			g_InterfaceManager.GetInterfaceString(7153, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_GIFT_ITEM_BLACK_LIST:                   ///>  선물 자격 미달 - black listed
		{
			g_InterfaceManager.GetInterfaceString(7154, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_GIFT_ITEM_BANNED_USER:                  ///>  선물 자격 미달 - banned user
		{
			g_InterfaceManager.GetInterfaceString(7155, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_GIFT_ITEM_NOT_EXIST_HISTORY:            ///>  선물 자격 미달 - 두달내 두번이상 충전이력이 없을때
		{
			g_InterfaceManager.GetInterfaceString(7156, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_EXCEED_LIMIT_GIFT_ITEM:                 ///>  선물 자격 미달 - 선물하기 한도초과
		{
			g_InterfaceManager.GetInterfaceString(7157, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_BILLING_SYSTEM_ERROR:                   ///>  빌링 시스템 에러
		{
			g_InterfaceManager.GetInterfaceString(7158, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;
	case RC_SHOP_BILLING_ETC_ERROR:                      ///>  빌링 기타에러
		{
			g_InterfaceManager.GetInterfaceString(7159, szMessage, INTERFACE_STRING_LENGTH);
			_tcscat(szInterfaceString, szMessage);
		}
		break;

    case RC_SHOP_CANNOT_SEND_GIFT_MYACCOUNT:            ///>   자기 자신의 계정의로 선물할수없다.
        {
            g_InterfaceManager.GetInterfaceString(7145, szMessage, INTERFACE_STRING_LENGTH);
            _tcscat(szInterfaceString, szMessage);
        }
        break;
	////####### 북미 샵서버 관련된 메시지처리 추가 끝 ####### 
    case RC_SHOP_NOT_FRIEND_ME:
    case RC_SHOP_NOT_FRINED_YOUR:
        {
            //- 러시아 선물하기 요건(상호간 친구목록 리스트에 있어야한다.)
            g_InterfaceManager.GetInterfaceString(155548, szMessage, INTERFACE_STRING_LENGTH);
            _tcscat(szInterfaceString, szMessage);
        }
        break;
	default:
		{
			//85222	아이템 샵: 오류입니다.
			g_InterfaceManager.GetInterfaceString(85222, szMessage, INTERFACE_STRING_LENGTH);
			
			_tcscat(szInterfaceString, szMessage);
		}
		break;

	}

	GlobalFunc::OutputMsg_Notice(type, szInterfaceString);

}

//------------------------------------------------------------------------------
/**
*/

PresentRecvBox* uiCashItemShopMan::GetPresentRcvBox()
{
	ShopUser* pShopUser = m_ShopManager.FindShopUser(3/*HERO_TEMP_KEY*/);
	if(!pShopUser)
		return NULL;

	PresentRecvBox* pPresentRecvBox = NULL;
	pPresentRecvBox = pShopUser->GetPresentRecvBox();
	if(!pPresentRecvBox)
		return NULL;
	return pPresentRecvBox;
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::renderCoverImage(CASH_ITEM_IMG& info, RECT& rect)
{
    if (info.type == KindOfItemType::Shell)
    {
        info.pImage->RenderCoverImage(&rect, WzColor_RGBA(0, 0, 0, 175));    
        return;
    }

    if (info.type == KindOfItemType::Etheria)
    {
        BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(info.itemCode);
        if (item_info == NULL)
        {
            return;
        }
        
        if (item_info->m_wType == eITEMTYPE_PET)
        {
            g_InterfaceManager.pet_etheria_icon()->Render(&rect);
        }
        else
        {
            g_InterfaceManager.etheria_icon()->Render(&rect);
        }
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopMan::NetworkProc( MSG_BASE * pMsg )
{
	TCHAR   szMessage[INTERFACE_STRING_LENGTH] = {0,};

    switch (pMsg->m_byCategory)
    {
    case CG_SHOP:
        {
            switch (pMsg->m_byProtocol)
            {
			case CG_SHOP_CASH_REQ_ACK:
				{
					MSG_CG_SHOP_CASH_REQ_ACK *pPacket = (MSG_CG_SHOP_CASH_REQ_ACK *)pMsg;
                    SetCash(pPacket->cash_info_.cash_);
                    SetPPCash(pPacket->cash_info_.pp_cash_);
					SetMyMileage(pPacket->cash_info_.mileage_);
					m_bWaitPacket = false;
					if (GetRqstState() == eRqstState_Present_List)
					{
						if (m_pUiCashItemShopPresentDlg && !m_pUiCashItemShopPresentDlg->IsVisibleWindow())
						{
							ClearAllDlg();
							//this->m_pUiCashItemShopPresentDlg->ShowInterface(TRUE);
                            ChangeTab(m_pUiCashItemShopPresentDlg);
						}
					}
					else
					{
						if (m_pUICashItemShopDlg && !m_pUICashItemShopDlg->IsVisibleWindow())
                        {
							//this->m_pUICashItemShopDlg->ShowInterface(TRUE);
                            ChangeTab(m_pUICashItemShopDlg);
                        }
					}
					SetRqstState(eRqstState_None);
				}
				break;

			case CG_SHOP_CASH_REQ_NAK:
				{
					MSG_CG_SHOP_CASH_REQ_NAK *pPacket = (MSG_CG_SHOP_CASH_REQ_NAK *)pMsg;
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CENTER_SHOW);
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CHAT);

					m_bWaitPacket = false;
				}
				break;

			case CG_SHOP_ACTION_RECV_GIFT_ACK:
				{
					MSG_CG_SHOP_ACTION_RECV_GIFT_ACK *pPacket = (MSG_CG_SHOP_ACTION_RECV_GIFT_ACK *)pMsg;

					switch(pPacket->m_ActionType)
					{
					case MSG_CG_SHOP_ACTION_RECV_GIFT_ACK::OBTAIN:
						{
							BOOL bRet = ItemManager::Instance()->InsertJustInventory(PRESENT, pPacket->m_TotalInfo);
							assert(bRet);

							ShowCashItemShopPresentCheck(false);

							PresentRecvBox* pPresentRecvBox = GetPresentRcvBox();
							if(!pPresentRecvBox)
								return;

							PresentRecvBox::PresentInfo* pPresentInfo = pPresentRecvBox->FindPresent(m_pendingBuyPresentItemInfo.giftGuid);

							pPresentInfo->SetState(PresentRecvBox::PresentInfo::ePS_ACCEPT);

							RefreshPresentItemList();
							this->ShowCashItemShopPresentComplete(&m_pendingBuyPresentItemInfo);

						}
						break;
					case MSG_CG_SHOP_ACTION_RECV_GIFT_ACK::REJECT:
						{
							ShowCashItemShopPresentCheck(false);

							PresentRecvBox* pPresentRecvBox = GetPresentRcvBox();
							if(!pPresentRecvBox)
								return;

							pPresentRecvBox->ErasePresent(m_pendingBuyPresentItemInfo.giftGuid);

							m_pUiCashItemShopPresentDlg->SetPresentCount(pPresentRecvBox->GetHash().size());
							RefreshPresentItemList();

							this->ShowCashItemShopPresentComplete(&m_pendingBuyPresentItemInfo);


						}
						break;
					case MSG_CG_SHOP_ACTION_RECV_GIFT_ACK::DISCARD:
						{
							PresentRecvBox* pPresentRecvBox = GetPresentRcvBox();
							if(!pPresentRecvBox)
								return;

							pPresentRecvBox->ErasePresent(m_pendingBuyPresentItemInfo.giftGuid);

							m_pUiCashItemShopPresentDlg->SetPresentCount(pPresentRecvBox->GetHash().size());
							RefreshPresentItemList();
						}
						break;
					default:
						break;;
					}

					m_bWaitPacket = false;
					SetRqstState(eRqstState_None);
					ClearPendingBuyPresentItemInfo();

				}
				break;

			case CG_SHOP_IS_RECV_GIFT_ACK:
				{
					MSG_CG_SHOP_IS_RECV_GIFT_ACK *pPacket = (MSG_CG_SHOP_IS_RECV_GIFT_ACK *)pMsg;
					if(pPacket->m_GiftCount)
					{
						PresentRecvBox* pPresentRecvBox = GetPresentRcvBox();
						if(!pPresentRecvBox)
						{
							m_iNewPresentRcvCount = pPacket->m_GiftCount;
						}
						else
						{
							m_iNewPresentRcvCount = pPacket->m_GiftCount - pPresentRecvBox->GetHash().size();
							if(m_iNewPresentRcvCount < 0)	m_iNewPresentRcvCount = 0;
						}
					}
				}
				break;

			case CG_SHOP_IS_RECV_GIFT_NAK:
				{
					MSG_CG_SHOP_IS_RECV_GIFT_NAK *pPacket = (MSG_CG_SHOP_IS_RECV_GIFT_NAK *)pMsg;
				}
				break;

			case CG_SHOP_ACTION_RECV_GIFT_NAK:
				{
					MSG_CG_SHOP_ACTION_RECV_GIFT_NAK *pPacket = (MSG_CG_SHOP_ACTION_RECV_GIFT_NAK *)pMsg;
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CENTER_SHOW);
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CHAT);

					ShowCashItemShopPresentCheck(false);

					m_bWaitPacket = false;
					SetRqstState(eRqstState_None);
					ClearPendingBuyPresentItemInfo();
				}
				break;

			case CG_SHOP_SEND_GIFT_ACK:
				{
					MSG_CG_SHOP_SEND_GIFT_ACK *pPacket = (MSG_CG_SHOP_SEND_GIFT_ACK *)pMsg;
                    SetCash(pPacket->m_CashData.cash_);
                    SetPPCash(pPacket->m_CashData.pp_cash_);
					SetMyMileage(pPacket->m_CashData.mileage_);

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
                    if ((uiLoginMan::IsUseParameterAuthInfo() == true) &&
                        (g_pApplication->IsInitWIC() == TRUE))
                    {
                        g_pApplication->InvokeCash();
                    }
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

					if(g_pHero)
                    {
						g_pHero->SetMoney(pPacket->m_UserMoney);
                    }
					m_bWaitPacket = false;
					ShowCashItemShopBuy(false);
					ShowCashItemShopPresentCheck(false);
					ShowCashItemShopPresentComplete(&m_pendingBuyPresentItemInfo);

                    if(m_pUICashItemShopDlg)
                    {
						m_pUICashItemShopDlg->SetCurMoney();
                    }
					SetRqstState(eRqstState_None);
					ClearPendingBuyPresentItemInfo();
				}
				break;

			case CG_SHOP_SEND_GIFT_NTF:
				{
					MSG_CG_SHOP_SEND_GIFT_NTF *pPacket = (MSG_CG_SHOP_SEND_GIFT_NTF *)pMsg;
					if(this->m_pUiCashItemShopPresentDlg && m_pUiCashItemShopPresentDlg->IsVisibleWindow()
						|| this->m_pUICashItemShopDlg && m_pUICashItemShopDlg->IsVisibleWindow()
						|| this->m_pUiCashItemShopPackageDlg && m_pUiCashItemShopPackageDlg->IsVisibleWindow())
					{
						Send_CG_SHOP_PRESENT_LIST_REQ_SYN();
					}
					else
					{
						m_iNewPresentRcvCount++;
					}
				}
				break;
			case CG_SHOP_SEND_GIFT_NAK:
				{
					MSG_CG_SHOP_SEND_GIFT_NAK *pPacket = (MSG_CG_SHOP_SEND_GIFT_NAK *)pMsg;
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CENTER_SHOW);
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CHAT);

					ShowCashItemShopBuy(false);
					ShowCashItemShopPresentCheck(false);

					m_bWaitPacket = false;
					SetRqstState(eRqstState_None);
					ClearPendingBuyPresentItemInfo();
				}
				break;

			case CG_SHOP_RECV_GIFT_LIST_ACK:
				{
					MSG_CG_SHOP_RECV_GIFT_LIST_ACK *pPacket = (MSG_CG_SHOP_RECV_GIFT_LIST_ACK *)pMsg;

					ShopUser* pShopUser = m_ShopManager.FindShopUser(3/*HERO_TEMP_KEY*/);
					if(!pShopUser)
						return;

					ArchiveOfStream lArchive( pPacket->m_pBUFFER, pPacket->m_Size );
					PresentRecvBox* pPresentRecvBox = pShopUser->GetPresentRecvBox();
					if(!pPresentRecvBox)
						return;

					pPresentRecvBox->Serialize(lArchive, eSHOP_SERIALIZE_SAVE);
					m_pUiCashItemShopPresentDlg->SetPresentCount(pPresentRecvBox->GetHash().size());
					//
					m_bWaitPacket = false;

					if(this->m_pUiCashItemShopPresentDlg && m_pUiCashItemShopPresentDlg->IsVisibleWindow())
					{
						RefreshPresentItemList();
					}
				}
				break;

			case CG_SHOP_RECV_GIFT_LIST_NAK:
				{
					MSG_CG_SHOP_RECV_GIFT_LIST_NAK *pPacket = (MSG_CG_SHOP_RECV_GIFT_LIST_NAK *)pMsg;
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CENTER_SHOW);
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CHAT);

					m_bWaitPacket = false;
				}
				break;

			case CG_SHOP_ITEM_LIST_REQ_ACK:
				{
					MSG_CG_SHOP_ITEM_LIST_REQ_ACK *pPacket = (MSG_CG_SHOP_ITEM_LIST_REQ_ACK *)pMsg;

					RefreshItemList(pPacket);

					m_bWaitPacket = false;

				}
				break;

			case CG_SHOP_ITEM_LIST_REQ_NAK:
				{
					MSG_CG_SHOP_ITEM_LIST_REQ_NAK *pPacket = (MSG_CG_SHOP_ITEM_LIST_REQ_NAK *)pMsg;
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CENTER_SHOW);
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CHAT);

					m_bWaitPacket = false;
				}
				break;

			case CG_SHOP_ITEM_REQ_ACK:
				{
					MSG_CG_SHOP_ITEM_REQ_ACK *pPacket = (MSG_CG_SHOP_ITEM_REQ_ACK *)pMsg;

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
					m_ShopManager.SerializeItemDetail(m_iPendingItemGuid, pPacket->m_pBUFFER, pPacket->m_Size, eSHOP_SERIALIZE_ITEM_SAVE);
    #ifdef _DEBUG
					TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
					Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("size[%d], category[%d], page[%d], itemGuid[%d]\n"),
						pPacket->m_Size, m_byPendingCategory, m_byPendingPage, m_iPendingItemGuid);
					OutputDebugString(szDebug);
    #endif
					if( (this->m_pUICashItemShopDlg && m_pUICashItemShopDlg->IsVisibleWindow() && GetCashShopDlgState() == eCashItemShopDlgState_CashShop) 
						|| ( this->m_pUICashItemShopMileageDlg && m_pUICashItemShopMileageDlg->IsVisibleWindow() && GetCashShopDlgState() == eCashItemShopDlgState_Mileage) )
						RefreshItemPrice();
#else	//	__NA_00387_20070605_SHOP_MILEAGE2
					if(this->m_pUICashItemShopDlg && m_pUICashItemShopDlg->IsVisibleWindow())
					{
						// 창이 열린 상태에서만 필요한...
						m_ShopManager.SerializeItemDetail(m_iPendingItemGuid, pPacket->m_pBUFFER, pPacket->m_Size, eSHOP_SERIALIZE_ITEM_SAVE);

    #ifdef _DEBUG
						TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
						Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("size[%d], category[%d], page[%d], itemGuid[%d]\n"),
							pPacket->m_Size, m_byPendingCategory, m_byPendingPage, m_iPendingItemGuid);
						OutputDebugString(szDebug);
    #endif

						RefreshItemPrice();
					}
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
					m_bWaitPacket = false;
				}
				break;
			case CG_SHOP_ITEM_REQ_NAK:
				{
					MSG_CG_SHOP_ITEM_REQ_NAK *pPacket = (MSG_CG_SHOP_ITEM_REQ_NAK *)pMsg;
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CENTER_SHOW);
					_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CHAT);

					m_bWaitPacket = false;
				}
				break;

			case CG_SHOP_ITEM_BUY_ACK:
				{
					MSG_CG_SHOP_ITEM_BUY_ACK *pPacket = (MSG_CG_SHOP_ITEM_BUY_ACK *)pMsg;
					BOOL bRet = ItemManager::Instance()->InsertJustInventory(BUY, pPacket->m_TotalInfo);
					assert(bRet);
                    SetCash(pPacket->m_CashData.cash_);
                    SetPPCash(pPacket->m_CashData.pp_cash_);
					SetMyMileage(pPacket->m_CashData.mileage_);

#ifdef _APPLY_JAPAN_GAMECHU_CHANNELING
                    if ((uiLoginMan::IsUseParameterAuthInfo() == true) &&
                        (g_pApplication->IsInitWIC() == TRUE))
                    {
                        g_pApplication->InvokeCash();
                    }
#endif //_APPLY_JAPAN_GAMECHU_CHANNELING

					if (g_pHero)
                    {
						g_pHero->SetMoney(pPacket->m_UserMoney);
                    }
					m_bWaitPacket = false;
					//구매 완료창 open
					if (GetCashShopDlgState() == eCashItemShopDlgState_CashShop)
					{
						if ((m_byPendingCategory == CT_PERIOD_EXTEND) || (m_byPendingCategory == SHOP_CATEGORY_PERIOD_EXTEND))
						{
							ShowCashItemShopExtensionComplete(&m_pendingBuyItemInfo);
						}
						else
						{
							this->ShowCashItemShopBuyComplete(&m_pendingBuyItemInfo);
						}
						ShowCashItemShopExtensionBuy(false);

						ShowCashItemShopBuy(false);
						ShowCashItemShopBuyCheck(false);
						if (m_pUICashItemShopDlg)
                        {
							m_pUICashItemShopDlg->SetCurMoney();
                        }
						ClearPendingBuyList();
					}
					else if(GetCashShopDlgState() == eCashItemShopDlgState_Package)
					{
						this->ShowCashItemShopPackageComplete(&m_pendingBuyPackageItemInfo);
						this->ShowCashItemShopPackageCheck(false);
						if (m_pUiCashItemShopPackageDlg)
                        {
							m_pUiCashItemShopPackageDlg->SetCurMoney();
                        }
						ClearPendingBuyPackageItemInfo();
					}
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
					else if(GetCashShopDlgState() == eCashItemShopDlgState_Mileage)
					{
						this->ShowCashItemShopMileageComplete(&m_pendingBuyItemInfo);
						this->ShowCashItemShopMileageCheck(false);
						if(m_pUICashItemShopMileageDlg)
                        {
							m_pUICashItemShopMileageDlg->SetCurMoney();
                        }
						ClearPendingBuyList();
					}
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
					SetRqstState(eRqstState_None);

				}
				break;

			case CG_SHOP_ITEM_BUY_NAK:
				{
					MSG_CG_SHOP_ITEM_BUY_NAK *pPacket = (MSG_CG_SHOP_ITEM_BUY_NAK *)pMsg;
					if(pPacket->m_PriceGuid == 0)
					{
						//< m_PriceGuid = 0 이면 모든 상품 실패
						// 아이템 구입이 실패했습니다.
						//85223	유료아이템샵: 아이템 구입이 실패했습니다.
						g_InterfaceManager.GetInterfaceString(85223, szMessage, INTERFACE_STRING_LENGTH);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CENTER_SHOW, szMessage);
						GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
					}
					else
					{
						_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CENTER_SHOW, pPacket->m_PriceGuid);
						_processErrorCode(pPacket->m_byErrorCode, eOUTPUT_MSG_CHAT, pPacket->m_PriceGuid);
					}
					
					m_bWaitPacket = false;

					if(GetCashShopDlgState() == eCashItemShopDlgState_CashShop)
						ClearPendingBuyList();
					else if(eCashItemShopDlgState_Package)
						ClearPendingBuyPackageItemInfo();

					SetRqstState(eRqstState_None);

				}
				break;
            }
        }
        break;
    }
}

//------------------------------------------------------------------------------ 
void uiCashItemShopMan::ExecBuyWCoin()
{
    TCHAR szREGISTER_URL[512];
    // 마이폴더 홈페이지에 접속하렴
    SHELLEXECUTEINFO shell_execute_info;
    shell_execute_info.cbSize = sizeof(SHELLEXECUTEINFO);
    shell_execute_info.fMask = NULL;
    shell_execute_info.hwnd = g_hWndMain;
    shell_execute_info.lpVerb = NULL;
    // 155451: http://peyment.webzen.net/Payment/PaymentMethodSelectFrm.asp
    g_InterfaceManager.GetInterfaceString(155451, szREGISTER_URL, 512);
    shell_execute_info.lpFile = szREGISTER_URL;
    shell_execute_info.lpParameters = NULL;
    shell_execute_info.lpDirectory = NULL;
    shell_execute_info.nShow = SW_MAXIMIZE;
    shell_execute_info.hInstApp = NULL;
    ShellExecuteEx(&shell_execute_info);
}

//------------------------------------------------------------------------------ 
void uiCashItemShopMan::FillSubMenu(CCtrlListWZ* list_control, int start_sub_menu, int end_sub_menu)
{
    //! 하위 메뉴
    static DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
    static DWORD c_dwSelTextColor = RGBA(0,220,0,255);
    static DWORD c_dwBackColor = RGBA(0,0,0,0);
    static TCHAR item_text[INTERFACE_STRING_LENGTH] = {0,};

    if (list_control == NULL)
    {
        return;
    }

    list_control->DeleteItemAll();

    for (int i = start_sub_menu; i <= end_sub_menu; ++i)
    {
        ZeroMemory(item_text, sizeof(item_text));

        g_InterfaceManager.GetInterfaceString(i, item_text, INTERFACE_STRING_LENGTH);

        int index = list_control->InsertItemBack();

        list_control->SetStartLineGapTop(3);
        list_control->SetSellHeight(16);
        list_control->SetFontID(StrToWzID("st11"));
        list_control->SetItemWZA(index, 
                                item_text, 
                                StrToWzID("st11"), 
                                DT_VCENTER | DT_CENTER, 
                                c_dwBackColor, 
                                c_dwTextColor, 
                                c_dwSelTextColor);
    }
}


//------------------------------------------------------------------------------ 
void uiCashItemShopMan::FillSortCombo(CCtrlListWZ* list_control)
{

    static DWORD c_dwTextColor = RGBA(255, 255, 255, 255);
    static DWORD c_dwSelTextColor = RGBA(0,220,0,255);
    static DWORD c_dwBackColor = RGBA(0,0,0,0);
    static TCHAR item_text[INTERFACE_STRING_LENGTH] = {0,};

    if (list_control == NULL)
    {
        return;
    }

    for (int i = uiCashItemShopMan::kShopStringCode_Filter_Cash; i < uiCashItemShopMan::kShopStringCode_Filter_Package; ++i)
    {
        ZeroMemory(item_text, sizeof(item_text));

        //! 2544 부터 소팅에 사용할 항목
        g_InterfaceManager.GetInterfaceString(i, item_text, INTERFACE_STRING_LENGTH);

        int index = list_control->InsertItemBack();

        list_control->SetStartLineGapTop(3);
        list_control->SetSellHeight(14);
        list_control->SetFontID(StrToWzID("st10"));
        list_control->SetItemWZA(index, 
                                item_text, 
                                StrToWzID("st10"), 
                                DT_VCENTER | DT_CENTER, 
                                c_dwBackColor, 
                                c_dwTextColor, 
                                c_dwSelTextColor);
    }
}


//------------------------------------------------------------------------------ 
WORD uiCashItemShopMan::GetFilter(int main_Menu, int sub_menu, CCtrlListWZ* list_control)
{

    WORD filter = SHOP_FILTER_NONE;
    if (main_Menu == 0)
    {
        if (sub_menu == 1)
        {
            filter |= SHOP_FILTER_DISCOUNT;
        }
        else if (sub_menu == 2)
        {
            filter |= SHOP_FILTER_EVENT;
        }
    }

    if (list_control != NULL)
    {
        //2544	전체 클레스	캐시상점 필터
        //2545	버서커	캐시상점 필터
        //2546	드래곤	캐시상점 필터
        //2547	발키리 	캐시상점 필터
        //2548	엘리멘탈	캐시상점 필터
        //2549	새도우	캐시상점 필터
        switch (list_control->GetCurSel())
        {
        case 0: filter |= SHOP_FILTER_CHAR_ALL; break;
        case 1: filter |= SHOP_FILTER_CHAR_BERSERKER; break;
        case 2: filter |= SHOP_FILTER_CHAR_PALADIN; break;
        case 3: filter |= SHOP_FILTER_CHAR_VALKYRIE; break;
        case 4: filter |= SHOP_FILTER_CHAR_ELEMENTALIST; break;
        case 5: filter |= SHOP_FILTER_CHAR_SHADOW; break;
        }
    }

    return filter;

}

//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 
