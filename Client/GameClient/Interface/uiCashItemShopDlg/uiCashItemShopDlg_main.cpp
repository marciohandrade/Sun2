#include "SunClientPrecompiledHeader.h"


//------------------------------------------------------------------------------ 
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 



#include "uiCashItemShopDlg.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "GameFunc.h"

#include "interfacemanager.h"
#include "GameFramework.h"
#include "ItemInfoParser.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "SCItemSlot.h"

//------------------------------------------------------------------------------
WzID uiCashItemShopDlg::m_wzId[eDIALOG_CASH_ITEM_SHOP_DLG_MAX] = 
{
		StrToWzID("B999"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_EXIT = 0,	

		StrToWzID("C001"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_CASH_ITEM,	
		StrToWzID("C002"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_MY_PAGE,	
		StrToWzID("C003"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_PACKAGE,	
        StrToWzID("CT01"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_MILEAGE,
		StrToWzID("C010"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL,		

        StrToWzID("CT03"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_PC

		StrToWzID("C019"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_ARMOR,
		StrToWzID("C020"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION,		//	기간연장

        StrToWzID("C021"), // eDIALOG_CASH_ITEM_SHOP_DLG_CHK_PP_CARD_ONLY
        StrToWzID("CT02"), // eDIALOG_CASH_ITEM_SHOP_DLG_CHK_BUY_WCOIN

        StrToWzID("C016"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_SKIN,
        StrToWzID("C000"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_CUSTOMIZE,
        StrToWzID("C008"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_ITEM,
        StrToWzID("C009"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_PET,
        StrToWzID("C004"), //eDIALOG_CASH_ITEM_SHOP_DLG_CHK_RIDER,

		StrToWzID("B016"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY1,		
		StrToWzID("BT02"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY2,
		StrToWzID("B018"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY3,		
		StrToWzID("BT03"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY4,
		StrToWzID("B020"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY5,
		StrToWzID("BT04"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY6
		StrToWzID("BT00"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY7,
		StrToWzID("BT05"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY8,
		StrToWzID("BT01"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY9,
		StrToWzID("BT06"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY10,
		

		StrToWzID("B001"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM1,		
		StrToWzID("TI02"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM2,
		StrToWzID("B002"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM3,		
		StrToWzID("TI03"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM4,
		StrToWzID("B003"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM5,		
		StrToWzID("TI04"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM6,
		StrToWzID("TI00"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM7,
		StrToWzID("TI05"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM8,
		StrToWzID("TI01"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM9,
		StrToWzID("TI06"), //eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM10,

		StrToWzID("LS00"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM1,		
		StrToWzID("LS09"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM2,
		StrToWzID("LS01"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM3,
		StrToWzID("LS08"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM4,
		StrToWzID("LS02"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM5,
		StrToWzID("LS07"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM6,
		StrToWzID("LS03"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM7,
		StrToWzID("LS05"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM8,
		StrToWzID("LS04"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM9,
		StrToWzID("LS06"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM10,
		

		StrToWzID("LS10"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM1,	
		StrToWzID("LS24"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM2,
		StrToWzID("LS11"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM3,
		StrToWzID("LS23"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM4,
		StrToWzID("LS12"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM5,
		StrToWzID("LS22"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM6,
		StrToWzID("LS13"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM7,
		StrToWzID("LS21"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM8,
		StrToWzID("LS14"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM9,
		StrToWzID("LS20"), //eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM10,

		StrToWzID("T057"), //eDIALOG_CASH_ITEM_SHOP_DLG_TXT_MONEY,
		StrToWzID("BT13"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_PREV_PAGE,	
		StrToWzID("BT12"), //eDIALOG_CASH_ITEM_SHOP_DLG_BTN_NEXT_PAGE,	

		StrToWzID("ST01"), //eDIALOG_CASH_ITEM_SHOP_DLG_TXT_CURR_PAGE,	

        //위 디파인이 적용되면 UI는 공통으로 들어간다.
        //_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_ 디파인으로 else경우 HIDE기능을 켠다.
        //.......
        StrToWzID("ST12"), // eDIALOG_CASH_ITEM_SHOP_DLG_STXT_TOTAL_WCOIN
        StrToWzID("ST21"), // eDIALOG_CASH_ITEM_SHOP_DLG_STXT_WCOINP
        StrToWzID("ST20"), // eDIALOG_CASH_ITEM_SHOP_DLG_STXT_WCOINC
        StrToWzID("ST03"), // eDIALOG_CASH_ITEM_SHOP_DLG_STXT_MILEAGE
        StrToWzID("ST04"), // eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINP
        StrToWzID("ST05"), // eDIALOG_CASH_ITEM_SHOP_DLG_TXT_WCOINC
        StrToWzID("ST02"), // eDIALOG_CASH_ITEM_SHOP_DLG_TXT_MILEGE
        StrToWzID("T057"), // eDIALOG_CASH_ITEM_SHOP_DLG_TXT_TOTAL_WCOIN
        StrToWzID("PI10"), // eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOIN_BACK
        StrToWzID("PI11"), // eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOINC_BACK
        StrToWzID("PI12"), // eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOINP_BACK
        StrToWzID("PI13"), // eDIALOG_CASH_ITEM_SHOP_DLG_IMG_TOTAL_WCOIN_BACK
        StrToWzID("BT07"), // eDIALOG_CASH_ITEM_SHOP_DLG_BTN_MILEGE

        StrToWzID("B000"), // eDIALOG_CASH_ITEM_SHOP_DLG_BTN_SORTCLASS,
        StrToWzID("L001"), // eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SORTCLASS,
        StrToWzID("S000"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_SORTCLASS,
        StrToWzID("L000"), // eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SUBMENU,

        StrToWzID("S001"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE1,
        StrToWzID("S003"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE2,
        StrToWzID("S004"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE3,
        StrToWzID("S005"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE4,
        StrToWzID("S006"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE5,
        StrToWzID("S007"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE6,
        StrToWzID("S008"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE7,
        StrToWzID("S009"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE8,
        StrToWzID("S010"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE9,
        StrToWzID("S011"), // eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_PRICE10,

};


//------------------------------------------------------------------------------ 
//! static function
void uiCashItemShopDlg::Callback_ShopIconRender(CControlWZ* control_ptr, void* parameter)
{
    uiCashItemShopDlg* onwer = static_cast<uiCashItemShopDlg*>(parameter);
    if ((control_ptr != NULL) && (onwer != NULL))
    {
        CASH_ITEM_IMG* item_img = (CASH_ITEM_IMG*)(control_ptr->GetUserData());

        if ((item_img->pImage != NULL) && (item_img->dwIconCode > 0))
        {
            RECT rc = control_ptr->GetSizeRect();
            item_img->pImage->Render(&rc);
            onwer->GetManager()->renderCoverImage(*item_img, rc);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopDlg::uiCashItemShopDlg(InterfaceManager *pUIMan) :
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
	ZeroMemory(&m_CashItemImg, sizeof(m_CashItemImg));
	m_iCurMenu = -1;
	m_iCurPage = -1;
	m_iCurIndex = -1;
    m_iCurSubMenu = 0;

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
}

//------------------------------------------------------------------------------
/**
*/
uiCashItemShopDlg::~uiCashItemShopDlg()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::Init(CDrawBase* pDrawBase)
{
    uiBase::Init(pDrawBase);

    this->InitControls();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::Release()
{
	_removeAllIconImage();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::InitControls()
{
	int i=0;
	for(i=0; i<eCASH_ITEM_SHOP_DLG_CHK_MAX; ++i)
	{
		m_pBtnCheckTabs[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_CHK_CASH_ITEM+i));
		assert(m_pBtnCheckTabs[i]);
	}

#ifdef __CHINA_BLOCK_ITEM_SHOP_PRESENT
	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_MY_PAGE])
	{
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_MY_PAGE]->EnableWindowWZ(FALSE);
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_MY_PAGE]->SetCheckState(FALSE);
	}
#endif //__CHINA_BLOCK_ITEM_SHOP_PRESENT


	for(i=0; i<eCASH_ITEM_SHOP_DLG_CHK_MENU_MAX; ++i)
	{
		m_pBtnCheckMenu[i] = static_cast<CCtrlButtonCheckWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL+i));
		assert(m_pBtnCheckMenu[i]);
	}

    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_//PP카드 관련 기능
        //적용되면 숨기지 않는다
    #else
        if (m_pBtnCheckMenu[eCASH_ITEM_SHOP_DLG_CHK_BUY_WCOIN])
        {
            m_pBtnCheckMenu[eCASH_ITEM_SHOP_DLG_CHK_BUY_WCOIN]->ShowWindowWZ(WZ_HIDE);
        }
        if (m_pBtnCheckMenu[eCASH_ITEM_SHOP_DLG_CHK_PP_CARD_ONLY])
        {
            m_pBtnCheckMenu[eCASH_ITEM_SHOP_DLG_CHK_PP_CARD_ONLY]->ShowWindowWZ(WZ_HIDE);
        }
    #endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_

	for(i=0; i<eCASH_ITEM_SHOP_DLG_ITEM_MAX; ++i)
	{
		m_pImgCtrl[i] = static_cast<CCtrlImageStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM1+i));
		assert(m_pImgCtrl[i]);

		m_pListCtrlName[i] = static_cast<CCtrlListWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_ITEM1+i));
		assert(m_pListCtrlName[i]);

		m_pListCtrlMemo[i] = static_cast<CCtrlListWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_MEMO_ITEM1+i));
		assert(m_pListCtrlMemo[i]);
	}
	
	m_pTxtCtrlCurPage = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_TXT_CURR_PAGE));
	assert(m_pTxtCtrlCurPage);

	m_pTxtCtrlMoney = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_TXT_MONEY));
	assert(m_pTxtCtrlMoney);

    m_pTxtCtrlMilege = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_TXT_MILEGE));
	assert(m_pTxtCtrlMilege);

    this->m_bControlsInitialized = true;

#ifdef _K2P_EXTENSION_BLOCK_
	CCtrlButtonWZ * pExtentionBtn = (CCtrlButtonWZ *)getControl( eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION );
	if( pExtentionBtn != NULL )
		pExtentionBtn->ShowWindowWZ( WZ_HIDE );
#endif

#ifdef __LTJ_GSP_HIDE_CASHSHOP_PCCAFE_BUTTON
	m_pBtnCheckMenu[eCASH_ITEM_SHOP_DLG_CHK_PC]->ShowWindowWZ( WZ_HIDE ); 
	CCtrlPictureWZ* pPictureControl = static_cast<CCtrlPictureWZ*>(GetControlWZ(StrToWzID("PI01")));
	if (pPictureControl)
	{
		pPictureControl->ShowWindowWZ( WZ_HIDE );
	}
#endif //__LTJ_GSP_HIDE_CASHSHOP_PCCAFE_BUTTON

    //적용되면 숨기지 않는다
    for (i=eCASH_ITEM_SHOP_DLG_STXT_WCOINP; i<eCASH_ITEM_SHOP_DLG_TXT_TOTAL_WCOIN; ++i)
	{
		m_pTxtCtrlPPCards[i] = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_STXT_TOTAL_WCOIN+i));
		SUN_ASSERT(m_pTxtCtrlPPCards[i]);
    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_//PP카드 관련 기능
    //적용되면 숨기지 않는다
    #else
        if (m_pTxtCtrlPPCards[i])
	    {
		    m_pTxtCtrlPPCards[i]->ShowWindowWZ( WZ_HIDE );
	    }
    #endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
	}

    for(i=0; i<eCASH_ITEM_SHOP_DLG_IMG_TOTAL_WCOIN_BACK; ++i)
	{
        m_pPICBackImg[i] = static_cast<CCtrlPictureWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOIN_BACK+i));
	    SUN_ASSERT(m_pPICBackImg[i]);
    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_//PP카드 관련 기능
    //적용되면 숨기지 않는다
    #else
        if (m_pPICBackImg[i])
	    {
		    m_pPICBackImg[i]->ShowWindowWZ( WZ_HIDE );
	    }
    #endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
    }

#ifdef __NA_00387_20070605_SHOP_MILEAGE2    // 마일리지가 적용되었으면 보여준다.
    if (m_pTxtCtrlPPCards[eCASH_ITEM_SHOP_DLG_STXT_MILEAGE])
	{
		m_pTxtCtrlPPCards[eCASH_ITEM_SHOP_DLG_STXT_MILEAGE]->ShowWindowWZ( WZ_SHOW );
	}

    if (m_pTxtCtrlPPCards[eCASH_ITEM_SHOP_DLG_TXT_MILEAGE])
	{
		m_pTxtCtrlPPCards[eCASH_ITEM_SHOP_DLG_TXT_MILEAGE]->ShowWindowWZ( WZ_SHOW );
	}

    if (m_pPICBackImg[eCASH_ITEM_SHOP_DLG_IMG_WCOINP_BACK])
    {
        m_pPICBackImg[eCASH_ITEM_SHOP_DLG_IMG_WCOINP_BACK]->ShowWindowWZ( WZ_SHOW );
    }
#else
    CCtrlStaticWZ* control_txts;
    control_txts = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_TXT_MILEGE));
    SUN_ASSERT(control_txts);
    if(control_txts)
    {
        control_txts->ShowWindowWZ(WZ_HIDE);
    }

    control_txts = static_cast<CCtrlStaticWZ *>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_STXT_MILEAGE));
    SUN_ASSERT(control_txts);
    if(control_txts)
    {
        control_txts->ShowWindowWZ(WZ_HIDE);
    }

    CCtrlButtonWZ* icon_of_mileage = static_cast<CCtrlButtonWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_BTN_MILEGE));
    SUN_ASSERT(icon_of_mileage);
    if(icon_of_mileage)
    {
        icon_of_mileage->ShowWindowWZ(WZ_HIDE);
    }

    if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_MILEAGE])
    {
        m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_MILEAGE]->ShowWindowWZ(WZ_HIDE);
    }
#endif//__NA_00387_20070605_SHOP_MILEAGE2


    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SORTCLASS));
    GetManager()->FillSortCombo(list_control);
    if (list_control != NULL)
    {
        list_control->SetCurSel(0);
        OnSelectedCombo(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SORTCLASS, 
                        eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_SORTCLASS, 
                        list_control->GetCurSel());
    }
}

//------------------------------------------------------------------------------
void uiCashItemShopDlg::OnShowWindow( BOOL val )
{
    if (val)
    {
		 //this->MoveDefaultWindowPos();

        if (!this->m_bControlsInitialized)
        {
            this->ShowInterface(FALSE);
            return;
        }
    
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = eDoType_dialog_cash_item_shop;
        msg.wParam = InterfaceManager::DIALOG_CASH_ITEM_SHOP;
        msg.DoSomething = GlobalFunc::DoShowWindow;
        g_KeyQueueManager.PushBack(msg);

		m_bShow = true;

		_prepareShow();
	}
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_cash_item_shop);
		_removeAllIconImage();

        if (GetManager())
        {
			GetManager()->set_pp_category(false);
            GetManager()->set_cc_category(false);
            GetManager()->ShowCashItemPurchaseDlg(false);
        }

        getControl(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SORTCLASS)->ShowWindowWZ(WZ_HIDE);
    }
}


//------------------------------------------------------------------------------ 
WORD uiCashItemShopDlg::GetFilter()
{
    CCtrlListWZ* sort_list_control = static_cast<CCtrlListWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SORTCLASS));

    return GetManager()->GetFilter(m_iCurMenu, m_iCurSubMenu, sort_list_control);
}

//------------------------------------------------------------------------------
BYTE uiCashItemShopDlg::_getCategoryType(int iMenu, int iSubMenu)
{
    switch (iMenu)
    {
    case eCASH_ITEM_SHOP_DLG_CHK_TOTAL:        
        {
            return SHOP_CATEGORY_ALL;
        }
        break;
    case eCASH_ITEM_SHOP_DLG_CHK_ARMOR:
        {
            switch (iSubMenu)
            {
            case 0: return SHOP_CATEGORY_EQUIP_ALL;
            case 1: return SHOP_CATEGORY_EQUIP_HELMET;    // 헬멧
            case 2: return SHOP_CATEGORY_EQUIP_ARMOR;     // 갑옷
            case 3: return SHOP_CATEGORY_EQUIP_GLOVES;    // 장갑
            case 4: return SHOP_CATEGORY_EQUIP_PANTS;     // 바지
            case 5: return SHOP_CATEGORY_EQUIP_BOOTS;     // 부츠
            case 6: return SHOP_CATEGORY_EQUIP_PROTECTOR; // 프로텍터
            case 7: return SHOP_CATEGORY_EQUIP_BELT;      // 벨트
            case 8: return SHOP_CATEGORY_EQUIP_SHIRTS;    // 셔츠
            case 9: return SHOP_CATEGORY_EQUIP_WEAPON;    // 무기
            }
            return SHOP_CATEGORY_EQUIP_ALL;
        }
        break;

    case eCASH_ITEM_SHOP_DLG_CHK_SKIN:          
        {
            switch (iSubMenu)
            {
            case 0: return SHOP_CATEGORY_SKIN_ALL;
            case 1: return SHOP_CATEGORY_SKIN_HELMET;    // 헬멧
            case 2: return SHOP_CATEGORY_SKIN_ARMOR;     // 갑옷
            case 3: return SHOP_CATEGORY_SKIN_GLOVES;    // 장갑
            case 4: return SHOP_CATEGORY_SKIN_PENTS;     // 바지
            case 5: return SHOP_CATEGORY_SKIN_BOOTS;     // 부츠
            case 6: return SHOP_CATEGORY_SKIN_PROTECTOR; // 프로텍터
            case 7: return SHOP_CATEGORY_SKIN_BELT;      // 벨트
            case 8: return SHOP_CATEGORY_SKIN_SHIRTS;    // 셔츠
            case 9: return SHOP_CATEGORY_SKIN_WEAPON;    // 무기
            }
            return SHOP_CATEGORY_SKIN_ALL;
        }
        break;

    case eCASH_ITEM_SHOP_DLG_CHK_CUSTOMIZE:   
        {
            switch (iSubMenu)
            {
            case 0: return SHOP_CATEGORY_CUSTOMIZING_ALL;
            case 1: return SHOP_CATEGORY_CUSTOMIZING_HAIR;   // 머리
            case 2: return SHOP_CATEGORY_CUSTOMIZING_FACE;   // 얼굴
            case 3: return SHOP_CATEGORY_CUSTOMIZING_ETC;    // 기타
            }
            return SHOP_CATEGORY_CUSTOMIZING_ALL;
        }
        break;

    case eCASH_ITEM_SHOP_DLG_CHK_ITEM:        
        {
            switch (iSubMenu)
            {
            case 0: return SHOP_CATEGORY_ITEM_ALL;
            case 1: return SHOP_CATEGORY_ITEM_LUCKY;          // 행운류
            case 2: return SHOP_CATEGORY_ITEM_MATERIAL;       // 재료류
            case 3: return SHOP_CATEGORY_ITEM_GUILD;          // 길드류
            case 4: return SHOP_CATEGORY_ITEM_TICKET;         // 입장권류
            case 5: return SHOP_CATEGORY_ITEM_STRENGTHEN;     // 능력치 강화류
            case 6: return SHOP_CATEGORY_ITEM_FUNCTIONALITY;  // 기능성류
            case 7: return SHOP_CATEGORY_ITEM_CONVENIENCE;    // 편의성류
            }
            return SHOP_CATEGORY_ITEM_ALL;
        }
        break;

    case eCASH_ITEM_SHOP_DLG_CHK_PET:       
        {
            switch (iSubMenu)
            {
            case 0: return SHOP_CATEGORY_PET_ALL;
            case 1: return SHOP_CATEGORY_PET;          // 사료
            case 2: return SHOP_CATEGORY_PET_FEED;       // 머리파츠
            case 3: return SHOP_CATEGORY_PET_SKIN;          // 몸통파츠
            //case 4: return SHOP_CATEGORY_PET_LEGS;         // 다리파츠
            }
            return SHOP_CATEGORY_PET_ALL;
        }
        break;

    case eCASH_ITEM_SHOP_DLG_CHK_RIDER: 
        {
            switch (iSubMenu)
            {
            case 0: return SHOP_CATEGORY_VEHICLE_ALL;
            case 1: return SHOP_CATEGORY_VEHICLE;          // 탈것
            case 2: return SHOP_CATEGORY_VEHICLE_HEAD;       // 머리파츠
            case 3: return SHOP_CATEGORY_VEHICLE_BODY;          // 몸통파츠
            case 4: return SHOP_CATEGORY_VEHICLE_LEGS;         // 다리파츠
            }
            return SHOP_CATEGORY_VEHICLE_ALL;
        }
        break;

    case eCASH_ITEM_SHOP_DLG_CHK_PC:            
        {
            return SHOP_CATEGORY_PCROOM;
        }
        break;

    case eCASH_ITEM_SHOP_DLG_CHK_EXTENSION:     
        {
            return SHOP_CATEGORY_PERIOD_EXTEND;
        }
        break;
    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
    case eCASH_ITEM_SHOP_DLG_CHK_PP_CARD_ONLY:     
        {
            return SHOP_CATEGORY_PPCASH;
        }
        break;
    #endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
    }
    return SHOP_CATEGORY_ALL;
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_setMenu(int iMenu, int iSubMenu, bool force_change)
{
//------------------------------------------------------------------------------ 
//!

	if ((m_iCurMenu == iMenu) && (m_iCurSubMenu == iSubMenu) && (force_change == false))
    {
		return;
    }

	BYTE byCategory = _getCategoryType(iMenu, iSubMenu);


    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SUBMENU));
    if (list_control != NULL)
    {
        list_control->SetCurSel(iSubMenu);
    }

	if(byCategory == SHOP_CATEGORY_PERIOD_EXTEND)
	{
        GetManager()->OnClickExtensionButton(m_iCurPage);
	}
	else
	{
		m_iCurMenu = iMenu;
        m_iCurSubMenu = iSubMenu;
		m_iCurPage = 0;
		m_iCurIndex = 0;

        WORD filter = GetFilter();

		_updateMenu();

        GetManager()->Send_CG_SHOP_ITEM_LIST_REQ_SYN(SHOP_PARTITION_SINGLEITEM, byCategory, filter, m_iCurPage);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_updateMenu()
{
	for(int i = 0; i < eCASH_ITEM_SHOP_DLG_CHK_MENU_MAX; ++i)
    {
        if (m_pBtnCheckMenu[i] != NULL)
        {
            m_pBtnCheckMenu[i]->SetCheckState(false);
        }
    }

	if(m_pBtnCheckMenu[m_iCurMenu] != NULL)
    {
		m_pBtnCheckMenu[m_iCurMenu]->SetCheckState(true);
    }


    int start_sub_menu = 0;
    int end_sub_menu = -1;

    if (m_iCurMenu == eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL - eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL)
    {
        start_sub_menu = uiCashItemShopMan::kShopStringCode_AllItem;
        end_sub_menu = uiCashItemShopMan::kShopStringCode_Armor - 1;
    }
    else if (m_iCurMenu == eDIALOG_CASH_ITEM_SHOP_DLG_CHK_ARMOR - eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL)
    {
        //! 장비
        start_sub_menu = uiCashItemShopMan::kShopStringCode_Armor;
        end_sub_menu = uiCashItemShopMan::kShopStringCode_Skin - 1;
    }
    else if (m_iCurMenu == eDIALOG_CASH_ITEM_SHOP_DLG_CHK_SKIN - eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL)
    {
        //! 스킨
        start_sub_menu = uiCashItemShopMan::kShopStringCode_Skin;
        end_sub_menu = uiCashItemShopMan::kShopStringCode_Customize - 1;
    }
    else if (m_iCurMenu == eDIALOG_CASH_ITEM_SHOP_DLG_CHK_CUSTOMIZE - eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL)
    {
        //! 커스터마이징
        start_sub_menu = uiCashItemShopMan::kShopStringCode_Customize;
        end_sub_menu = uiCashItemShopMan::kShopStringCode_Item - 1;
    }
    else if (m_iCurMenu == eDIALOG_CASH_ITEM_SHOP_DLG_CHK_ITEM - eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL)
    {
        //! 아이템
        start_sub_menu = uiCashItemShopMan::kShopStringCode_Item;
        end_sub_menu = uiCashItemShopMan::kShopStringCode_Pet - 1;
    }
    else if (m_iCurMenu == eDIALOG_CASH_ITEM_SHOP_DLG_CHK_PET - eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL)
    {
        //! 펫
        start_sub_menu = uiCashItemShopMan::kShopStringCode_Pet;
        end_sub_menu = uiCashItemShopMan::kShopStringCode_Rider - 1;
    }
    else if (m_iCurMenu == eDIALOG_CASH_ITEM_SHOP_DLG_CHK_RIDER - eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL)
    {
        //! 탈것
        start_sub_menu = uiCashItemShopMan::kShopStringCode_Rider;
        end_sub_menu = uiCashItemShopMan::kShopStringCode_Filter_Cash - 1;
    }

    GetManager()->FillSubMenu(static_cast<CCtrlListWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SUBMENU)), start_sub_menu, end_sub_menu);

    CControlWZ* picture_control = getControl(eDIALOG_CASH_ITEM_SHOP_DLG_IMG_WCOIN_BACK);
    if (picture_control != NULL)
    {
        picture_control->ShowWindowWZ((end_sub_menu != -1) ? WZ_SHOW : WZ_HIDE);
    }
}

//------------------------------------------------------------------------------
/** 
*/
void uiCashItemShopDlg::_setIndex(int iIndex)
{
	m_iCurIndex = iIndex;
	// 해당 아이템 가격 정보가 있으면 Show, 없으면 프로토콜 요청
	if(GetManager()->m_stCashItemInfo[m_iCurIndex].stPrice[0].priceGuid > 0)
		GetManager()->ShowCashItemShopBuy(m_iCurIndex);							
	else
		GetManager()->Send_CG_SHOP_ITEM_REQ_SYN(m_iCurIndex);
}
//------------------------------------------------------------------------------
/** 아이템 페이지 변경 관련
*/
void uiCashItemShopDlg::_setPage(int iPage)
{
	if(m_iCurPage == iPage)
		return;

	//m_iCurPage = iPage;
	BYTE byCategory = _getCategoryType(m_iCurMenu, m_iCurSubMenu);

	m_iCurIndex = 0;

    WORD filter = GetFilter();

    GetManager()->Send_CG_SHOP_ITEM_LIST_REQ_SYN(SHOP_PARTITION_SINGLEITEM, 
                                                byCategory, 
                                                filter, 
                                                iPage);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_prepareShow()
{
	if (!_isEnable())
		return;

	GetManager()->PrepareShow(eCashItemShopDlgState_CashShop);

	ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
	_clearAllItem();
	
	m_iCurMenu = -1;
	m_iCurPage = -1;
	m_iCurIndex = -1;

	_setMenu(eCASH_ITEM_SHOP_DLG_CHK_TOTAL);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::SetCurMoney( void )
{
    MONEY totalmoney = 0;
    CASH normalcash = GetManager()->GetMyCash();
    CASH ppcardcash = GetManager()->GetPPCash();   
    GlobalFunc::SetMoneyText(m_pTxtCtrlPPCards[eCASH_ITEM_SHOP_DLG_TXT_WCOINP], ppcardcash, CASH_ITEM_SHOP_DEFAULT);
    GlobalFunc::SetMoneyText(m_pTxtCtrlPPCards[eCASH_ITEM_SHOP_DLG_TXT_WCOINC], normalcash, CASH_ITEM_SHOP_DEFAULT);
    totalmoney = static_cast<MONEY>(normalcash) + ppcardcash;
	GlobalFunc::SetMoneyText(m_pTxtCtrlMilege, GetManager()->GetMyMileage(), CASH_ITEM_SHOP_MILEAGE);
	if (LOGINPARAM->IsLiveTestServer())	//테섭실행모드
	{
		GlobalFunc::SetMoneyText(m_pTxtCtrlMoney, g_pHero->GetMoney(), CASH_ITEM_SHOP_HAIM);
	}
	else
	{
		GlobalFunc::SetMoneyText(m_pTxtCtrlMoney, totalmoney, CASH_ITEM_SHOP_DEFAULT);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::RefreshItemPrice()
{
#ifdef _DEBUG
	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("Category[%d], Page[%d], Index[%d] \n"),
		m_iCurMenu, m_iCurPage, m_iCurIndex);
	OutputDebugString(szDebug);
#endif
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::RefreshSingleItemList()
{
#ifdef _DEBUG
	TCHAR szDebug[INTERFACE_STRING_LENGTH] = {0,};
	Snprintf(szDebug, INTERFACE_STRING_LENGTH-1, _T("RefreshList[%d] %d Page\n"),
		m_iCurMenu, m_iCurPage);
	OutputDebugString(szDebug);
#endif
	_clearAllItem();
	SetCurPage();
	SetCurMoney();
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_clearItem(int index)
{
	this->_removeIconImage(index);

	if(m_pListCtrlName[index])
	{
		m_pListCtrlName[index]->SetUseReport(false);
		m_pListCtrlName[index]->DeleteItemAll();
	}
	if(m_pListCtrlMemo[index])
	{
		m_pListCtrlMemo[index]->SetUseReport(false);
		m_pListCtrlMemo[index]->DeleteItemAll();
	}

}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_clearAllItem()
{
	for(int i=0; i<eCASH_ITEM_SHOP_DLG_ITEM_MAX; ++i)
	{
		_clearItem(i);
	}

	GlobalFunc::SetCaption(m_pTxtCtrlCurPage, _T(""));	// 현재 페이지

}

//------------------------------------------------------------------------------
/**
*/

void uiCashItemShopDlg::SetItem(int index, CODETYPE itemCode)
{
	_setItem(index, itemCode);
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::SetCurPage()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] ={0,};
	//5796	%d페이지
	g_InterfaceManager.GetInterfaceString( 5796 , szTemp ,INTERFACE_STRING_LENGTH);
	Snprintf(szMessage, INTERFACE_STRING_LENGTH-1, szTemp, m_iCurPage + 1);
	GlobalFunc::SetCaption(m_pTxtCtrlCurPage, szMessage);	// 현재 페이지
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_setItem(int index, CODETYPE itemCode)
{
	BASE_ITEMINFO * pItemInfo = ItemInfoParser::Instance()->GetItemInfo( itemCode );

	if( pItemInfo == NULL ) 
		return;

    m_CashItemImg[index].type = GetManager()->m_stCashItemInfo[index].byRank;
	_addIconImage(index, pItemInfo->m_wVICode, itemCode);

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp2[INTERFACE_STRING_LENGTH] = {0,};
	TCHAR szTemp3[INTERFACE_STRING_LENGTH] = {0,};

	// 아이템 이름
	g_InterfaceManager.GetItemName(itemCode, szMessage, INTERFACE_STRING_LENGTH);


	if(GetManager()->m_stCashItemInfo[index].byEnchant)
	{
		Snprintf(szTemp, INTERFACE_STRING_LENGTH-1, _T("+%d"), GetManager()->m_stCashItemInfo[index].byEnchant);
		_tcscat(szMessage, szTemp);
	}

    // 캐시샵아이템의 Rank 값을 용도 변경하여 Rank를 출력 하지 않도록 수정
    // 이전부터 Rank자체는 사용되고 있지 않았음.
	//if( (pItemInfo->m_wSubType != eITEMSUBTYPE_FATE)  && pItemInfo->CanRankUpItem())
	//{
	//	int iRankIndex = eST_RANK_E_STRING_INDEX  + (GetManager()->m_stCashItemInfo[index].byRank);
	//	if (iRankIndex > eST_RANK_PS_STRING_INDEX)
	//		iRankIndex = eST_RANK_PS_STRING_INDEX;
	//	if (iRankIndex < eST_RANK_E_STRING_INDEX)
	//		iRankIndex = eST_RANK_E_STRING_INDEX;

	//	ZeroMemory(szTemp, sizeof(szTemp));
	//	ZeroMemory(szTemp2, sizeof(szTemp2));
	//	ZeroMemory(szTemp3, sizeof(szTemp3));
	//	g_InterfaceManager.GetInterfaceString(eST_RANK_STRING_INDEX,szTemp,INTERFACE_STRING_LENGTH);
	//	g_InterfaceManager.GetInterfaceString(iRankIndex,szTemp2,INTERFACE_STRING_LENGTH);	
	//	Snprintf(szTemp3,INTERFACE_STRING_LENGTH-1, szTemp, szTemp2);
	//	_tcscat(szMessage, _T("   "));
	//	_tcscat(szMessage, szTemp3);
	//}

	m_pListCtrlName[index]->DeleteItemAll();
	m_pListCtrlName[index]->SetColorSelBar(c_CashItemShopListSelBarColor);
	m_pListCtrlName[index]->SetUseReport(false);

	if( pItemInfo->m_NCode != 0 )
	{
#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
	//step:이름그릴영역에 내용을 벡터에 담아낸다.
	RECT rcName = m_pListCtrlName[index]->GetSizeRect();

	std::vector<util::_tstring> vecItemName;

	GlobalFunc::ProcessWordWrapByFontSize( &vecItemName, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), rcName.right - rcName.left - 5 );

	//step1:나뉜갯수만큼 리스트에 차곡넣어준다.
	for (size_t i=0; i<vecItemName.size(); ++i)
	{
		int idx = m_pListCtrlName[index]->InsertItemBack();
		TCHAR *pStr = const_cast<TCHAR *>(vecItemName[i].c_str());
		GlobalFunc::SetItem(
			m_pListCtrlName[index], 
			idx, 0, pStr, 
			c_wiCashItemShopFont,
			DT_VCENTER | DT_LEFT, 
			c_CashItemShopListBKColor, 
			c_CashItemShopListNameColor, 
			c_CashItemShopListNameColor);

	}
#else
		int idx = m_pListCtrlName[index]->InsertItemBack();

		GlobalFunc::SetItem(
			m_pListCtrlName[index], 
			idx, 0, szMessage, 
			c_wiCashItemShopFont,
			DT_VCENTER | DT_LEFT, 
			c_CashItemShopListBKColor, 
			c_CashItemShopListNameColor, 
			c_CashItemShopListNameColor);
#endif//_GS_MOD_GSP_ITEMSHOP_TXT
	}

	// 간단 설명
	ZeroMemory(szMessage, sizeof(szMessage));
	if(pItemInfo->m_TDNCode)
	{
        SCItemSlot item_slot;
        item_slot.SetCode(static_cast<SLOTCODE>(itemCode));
        GameFunc::GetItemDescription(szMessage, &item_slot, pItemInfo);

		if(m_pListCtrlMemo[index])
		{
			std::vector<util::_tstring> vstrMsg;
			vstrMsg.clear();

			if(!_tcslen(szMessage))
				return;

			RECT rc = m_pListCtrlMemo[index]->GetSizeRect();
#ifdef __LTJ_TEXT_OUTPUT_AT_LIST_MOD
			int iListWidth = rc.right - rc.left - 20;
#else
			int iListWidth = rc.right - rc.left - 10;
#endif	
		
			GlobalFunc::ProcessWordWrapByFontSize( &vstrMsg, szMessage, c_wiCashItemShopFont, _tcslen(szMessage), iListWidth );

			m_pListCtrlMemo[index]->DeleteItemAll();
			m_pListCtrlMemo[index]->SetColorSelBar(c_CashItemShopListSelBarColor);
			m_pListCtrlMemo[index]->SetUseReport(false);
			int iListIndex = -1;

			

#ifdef _GS_MOD_GSP_ITEMSHOP_TXT
			size_t   nColume = 0;		//리스트에 들어갈메시지갯수(col)
			BOOL	 bOverMsg= FALSE;	//메시지가 넘어가는지 flag
			int		 nDotSize= 0;

			for (size_t i=0; i<vstrMsg.size(); ++i)
			{
				//STEP:일단 한번만체크한다(캐쉬샵아템설명리스트보다 긴지 체크)
				if(i==0)
				{
					//리스트몇개가 들어가는지 얻는다.
					int nHeight = GlobalFunc::GetTextHeight(_T("A"),m_pListCtrlMemo[index]->GetFontID());
					nColume = (m_pListCtrlMemo[index]->GetSize().bottom)/(nHeight);
					if(vstrMsg.size()>1 && vstrMsg.size()>nColume)
					{
						bOverMsg = TRUE;
						nDotSize = GlobalFunc::GetTextWidth(_T("..."), m_pListCtrlMemo[index]->GetFontID());
					}
					
				}

				iListIndex = m_pListCtrlMemo[index]->InsertItemBack();
				TCHAR *pStr = const_cast<TCHAR *>(vstrMsg[i].c_str());

				//STEP2:만약 길다면 리스트 끝줄에 있는 텍스트에 (...)붙여준다..그리고 리턴해버린다.
				if(bOverMsg)
				{
					if(i!=0 && (i+1)==nColume)
					{
						std::vector<util::_tstring> vstr;

						//1.현재 텍스트의 사이즈를 체크
						int nWidth = GlobalFunc::GetTextWidth(pStr, m_pListCtrlMemo[index]->GetFontID());
						
						//2.현재텍스트사이즈에서 ...사이즈만큼 재서 짜른다.
						GlobalFunc::ProcessWordWrapByFontSize( &vstr, pStr, m_pListCtrlMemo[index]->GetFontID(), _tcslen(pStr) , nWidth-nDotSize );

						//3.짜른다음 ...을 다시 붙여준다.
						vstr[0]+=_T("...");

						pStr = const_cast<TCHAR *>(vstr[0].c_str());

						GlobalFunc::SetItem(
							m_pListCtrlMemo[index], 
							iListIndex, 0, pStr, 
							c_wiCashItemShopFont,
							DT_VCENTER | DT_LEFT, 
							c_CashItemShopListBKColor, 
							c_CashItemShopListTextColor, 
							c_CashItemShopListTextColor);
						
						return;
					}
				}
				
				//STEP3:그린다.
				GlobalFunc::SetItem(
					m_pListCtrlMemo[index], 
					iListIndex, 0, pStr, 
					c_wiCashItemShopFont,
					DT_VCENTER | DT_LEFT, 
					c_CashItemShopListBKColor, 
					c_CashItemShopListTextColor, 
					c_CashItemShopListTextColor);
			}  



			
#else
			for (size_t i=0; i<vstrMsg.size(); ++i)
			{
				iListIndex = m_pListCtrlMemo[index]->InsertItemBack();

				TCHAR *pStr = const_cast<TCHAR *>(vstrMsg[i].c_str());
				GlobalFunc::SetItem(
					m_pListCtrlMemo[index], 
					iListIndex, 0, pStr, 
					c_wiCashItemShopFont,
					DT_VCENTER | DT_LEFT, 
					c_CashItemShopListBKColor, 
					c_CashItemShopListTextColor, 
					c_CashItemShopListTextColor);
			} 
#endif//_GS_GSP_ITEMSHOP_MEMO_TXT

			  
		}
	}
}



//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_addIconImage(int iIndex, DWORD dwCode, CODETYPE itemCode)
{
	m_CashItemImg[iIndex].dwIconCode = dwCode;
	m_CashItemImg[iIndex].itemCode = itemCode;
	_removeIconImage(iIndex);
	
	if (dwCode != 0)
	{
		m_CashItemImg[iIndex].pImage  = ICONMGR()->Add(dwCode);
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_removeIconImage(int iIndex)
{
	if(m_CashItemImg[iIndex].pImage)
	{
		ICONMGR()->RemoveIcon(m_CashItemImg[iIndex].pImage);
		ZeroMemory(&(m_CashItemImg[iIndex]), sizeof(m_CashItemImg[iIndex]));
	}
}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_removeAllIconImage()
{
	for(int i=0; i<eCASH_ITEM_SHOP_DLG_ITEM_MAX;  ++i)
	{
		_removeIconImage(i);
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL uiCashItemShopDlg::_isEnable()
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
void uiCashItemShopDlg::MessageProc( SI_MESSAGE * pMessage )
{
    if (!_isEnable())
        return;

	switch (pMessage->eResultMsg) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
			int	iCtrlID = getCtrlIDToPos( pMessage->dwCtrlID );
			switch( iCtrlID )
            {
            case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_EXIT:
                {
                    m_bShow = false;
                }
                break;
			case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_MY_PAGE:
				{
					GetManager()->ShowCashItemShopPresentDlg(true);
				}
				break;
			case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_PACKAGE:
				{
					GetManager()->ShowCashItemShopPackageDlg(true);
				}
				break;
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
		    case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_MILEAGE:
			    {
				    GetManager()->ShowCashItemShopMileageDlg(true);
			    }
			    break;
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_PREV_PAGE:
				{
					int iPage = m_iCurPage;
					if( iPage > 0)
					{
						iPage--;
						_setPage(iPage);
					}
				}
				break;
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_NEXT_PAGE:
				{
                    _setPage(m_iCurPage + 1);
				}
				break;
    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
            case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_BUY_WCOIN:
                {
                    GetManager()->ExecBuyWCoin();
                }
                break;
    #endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY1:
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY2:
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY3:
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY4:
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY5:
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY6:
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY7:
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY8:
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY9:
			case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY10:
				{
					int iIndex = iCtrlID - eDIALOG_CASH_ITEM_SHOP_DLG_BTN_BUY1;
                    if(GetManager()->m_stCashItemInfo[iIndex].dwItemCode > 0)
					{
						GetManager()->SelectBuyItem(iIndex);
					}
				}
				break;

            case eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SUBMENU:
                {
                    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(getControl(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SUBMENU));
                    int index = list_control->GetCurSel();

                    if (index >= 0)
                    {
                        _setMenu(m_iCurMenu, index);
                    }
                }
                break;

			case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL:
            case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_SKIN:
            case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_CUSTOMIZE:
            case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_ITEM:
            case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_PET:
            case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_RIDER:
			case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_PC:
			case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_ARMOR:
			case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_EXTENSION:

    #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
            case eDIALOG_CASH_ITEM_SHOP_DLG_CHK_PP_CARD_ONLY:
    #endif//_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
				{
					int clickmemu = iCtrlID - eDIALOG_CASH_ITEM_SHOP_DLG_CHK_TOTAL;
					this->_setMenu(clickmemu);
                    if (GetManager())
                    {
                        if (clickmemu == eCASH_ITEM_SHOP_DLG_CHK_PP_CARD_ONLY)
                        {
                            GetManager()->set_pp_category(true);
                            GetManager()->set_cc_category(false);
                        }
                        else if(clickmemu == eCASH_ITEM_SHOP_DLG_CHK_PC)
                        {
                            GetManager()->set_cc_category(true);
                            GetManager()->set_pp_category(false);
                        }
						else if(clickmemu == eCASH_ITEM_SHOP_DLG_CHK_EXTENSION)
						{
							//extension일 경우는 pp category를 변경하지 않는다.
						}
                        else
                        {
                            GetManager()->set_cc_category(false);
                            GetManager()->set_pp_category(false);
                        }
                        GetManager()->ShowCashItemPurchaseDlg(false);
                    }
				}
				break;

            case eDIALOG_CASH_ITEM_SHOP_DLG_BTN_SORTCLASS:
            case eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SORTCLASS:
                OnClickCombo(static_cast<eDIALOG_CASH_ITEM_SHOP_DLG>(iCtrlID));
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
void uiCashItemShopDlg::OnUpdateSolarDialog()
{
	if(!m_bShow)
		this->ShowInterface(FALSE);

    if (!_isEnable())
        return;

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_CASH_ITEM])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_CASH_ITEM]->SetCheckState(true);

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_MY_PAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_MY_PAGE]->SetCheckState(false);

	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_PACKAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_PACKAGE]->SetCheckState(false);

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	if(m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_MILEAGE])
		m_pBtnCheckTabs[eCASH_ITEM_SHOP_DLG_CHK_MILEAGE]->SetCheckState(false);
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2

	_updateMenu();

}

//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::OnRenderSolarDialog()
{
	if (!_isEnable())
        return;

	_renderIconImage();
	_renderTooltip();
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_renderIconImage()
{
    for(int i = 0; i < eCASH_ITEM_SHOP_DLG_ITEM_MAX; ++i)
    {
        if(m_pImgCtrl[i] != NULL)
        {
            m_pImgCtrl[i]->SetUserData((DWORD)&(m_CashItemImg[i]));
            m_pImgCtrl[i]->SetCallbackDraw(uiCashItemShopDlg::Callback_ShopIconRender, this);
        }
    }
}


//------------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::_renderTooltip()
{
#ifdef __CN_0_20070507_SHOP_MILEAGE
	uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
	if(pUIMan)
	{
		//	155387	해방군의 훈장	//	일단 예외상황이 하나라서 일단 하드코딩 하나 더 늘어나면 모듈화 필요해연
		if(this->m_ItemCodeForTooltip.dwItemCode == 155387)
		{
			pUIMan->RegisterTooltipWithStringCode(m_ItemCodeForTooltip.dwItemCode);
			pUIMan->SetTooltipDialogID(GetDialogKey());
		}
		else if(this->m_ItemCodeForTooltip.dwItemCode > 0)
		{
			pUIMan->RegisterTooltipItemForCashShop(m_ItemCodeForTooltip.dwItemCode, m_ItemCodeForTooltip.dwCoolTime, m_ItemCodeForTooltip.SellEndDate,
				m_ItemCodeForTooltip.byEnchant, m_ItemCodeForTooltip.byRank);
			pUIMan->SetTooltipDialogID(GetDialogKey());
		}
	}
#else	//	__CN_0_20070507_SHOP_MILEAGE
	if(this->m_ItemCodeForTooltip.dwItemCode > 0)
	{
		uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
		if(pUIMan)
		{
			pUIMan->RegisterTooltipItemForCashShop(m_ItemCodeForTooltip.dwItemCode, m_ItemCodeForTooltip.dwCoolTime, m_ItemCodeForTooltip.SellEndDate,
				m_ItemCodeForTooltip.byEnchant, m_ItemCodeForTooltip.byRank);
			pUIMan->SetTooltipDialogID(GetDialogKey());
		}
	}
#endif	//	__CN_0_20070507_SHOP_MILEAGE

	int iMouseX = Mouse::Instance()->GetMouseX();
	int iMouseY = Mouse::Instance()->GetMouseY();

	// 스킵처리 
	// 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
	BOOL bMouseOver = FALSE;

#ifdef __CN_0_20070507_SHOP_MILEAGE
	for (POSTYPE a = eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM1; a <= eDIALOG_CASH_ITEM_SHOP_DLG_BTN_MILEGE; a++)
#else	//	__CN_0_20070507_SHOP_MILEAGE
	for (POSTYPE a = eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM1; a <= eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM10; a++)
#endif	//	__CN_0_20070507_SHOP_MILEAGE

	{
		CControlWZ * pControl = getControl(a);

		if(pControl)
		{
			RECT rc = pControl->GetSizeRect();

			if (rc.left <= iMouseX && rc.right >= iMouseX)
			{
				if (rc.top <= iMouseY && rc.bottom >= iMouseY)
				{
					bMouseOver = TRUE;
				}
			}
		}
	}

	if ( !bMouseOver )
	{
		ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
	}
}

//----------------------------------------------------------------------------
/**
*/
void uiCashItemShopDlg::MouseOver_MsgProc( SI_MESSAGE * pMessage )
{
	if (!_isEnable())
        return;

	switch( pMessage->eResultMsg ) 
	{
	case RT_MSG_MOUSEOVER:
		{
			POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

			if (pos >= eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM1 && pos <= eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM10)
			{
				int iIndex = pos - eDIALOG_CASH_ITEM_SHOP_DLG_IMG_ITEM1;
				if (m_CashItemImg[iIndex].pImage && m_CashItemImg[iIndex].dwIconCode)
				{
					m_ItemCodeForTooltip.dwItemCode		= m_CashItemImg[iIndex].itemCode;
					m_ItemCodeForTooltip.dwCoolTime		= GetManager()->m_stCashItemInfo[iIndex].dwCoolTime;
					m_ItemCodeForTooltip.SellEndDate	= GetManager()->m_stCashItemInfo[iIndex].SellEndDate;
					m_ItemCodeForTooltip.byEnchant		= GetManager()->m_stCashItemInfo[iIndex].byEnchant;
					m_ItemCodeForTooltip.byRank			= GetManager()->m_stCashItemInfo[iIndex].byRank;
				}
				else
				{
					ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
				}
			}
#ifdef __CN_0_20070507_SHOP_MILEAGE
			else if(pos == eDIALOG_CASH_ITEM_SHOP_DLG_BTN_MILEGE)
			{
				//	155387	해방군의 훈장
				m_ItemCodeForTooltip.dwItemCode = 155387;
			}
#endif	//	__CN_0_20070507_SHOP_MILEAGE
			else
			{
				ZeroMemory(&m_ItemCodeForTooltip, sizeof(m_ItemCodeForTooltip));
			}			
		}
		break;
	}
}

//------------------------------------------------------------------------------ 
bool uiCashItemShopDlg::OnSelectedCombo(eDIALOG_CASH_ITEM_SHOP_DLG list_control_index, 
                                        eDIALOG_CASH_ITEM_SHOP_DLG static_control_index, 
                                        int selected_index)
{
    CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(getControl(list_control_index));
    CControlWZ* staticbox_control = getControl(static_control_index);

    if (selected_index < 0)
    {
        return false;
    }

    if ((list_control != NULL) && (staticbox_control != NULL))
    {
        staticbox_control->SetTextWZ("");

        const char* text = list_control->GetItemText(selected_index, 0);
        if (text != NULL)
        {
            staticbox_control->SetTextWZ((char*)text);
        }

        list_control->ShowWindowWZ(WZ_HIDE);
    }

    return true;
}

//------------------------------------------------------------------------------ 
void uiCashItemShopDlg::OnClickCombo(eDIALOG_CASH_ITEM_SHOP_DLG e)
{
    if (e == eDIALOG_CASH_ITEM_SHOP_DLG_BTN_SORTCLASS)
    {
        CControlWZ* list_control = getControl(eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SORTCLASS);
        if (list_control != NULL)
        {
            if (list_control->GetState() == WZ_SHOW)
            {
                list_control->ShowWindowWZ(WZ_HIDE);
            }
            else
            {
                list_control->ShowWindowWZ(WZ_SHOW);
            }
        }
    }
    else if (e == eDIALOG_CASH_ITEM_SHOP_DLG_LIST_SORTCLASS)
    {
        CCtrlListWZ* list_control = static_cast<CCtrlListWZ*>(getControl(e));

        if (list_control != NULL)
        {
            if (OnSelectedCombo(e, eDIALOG_CASH_ITEM_SHOP_DLG_TEXT_SORTCLASS, list_control->GetCurSel()) == true)
            {
                _setMenu(m_iCurMenu, m_iCurSubMenu, true);
            }
        }
    }
}


//------------------------------------------------------------------------------ 
#endif //#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL //#if'N'def
//------------------------------------------------------------------------------ 

