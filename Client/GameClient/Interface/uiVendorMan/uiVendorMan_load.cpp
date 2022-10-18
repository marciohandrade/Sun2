//------------------------------------------------------------------------------
//  uiVendorMan_main.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorMan.h"
#include "uiVendorBuy/uiVendorBuy.h"
#include "uiVendorSell/uiVendorSell.h"
#include "uiVendorPopup/uiVendorPopup.h"
#include "uiVendorTitlePopup/uiVendorTitlePopup.h"
#include "interfacemanager.h"
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
#include "uiVendorBuyPopup.h"
#endif
#include "uiVendorList/uiVendorList.h"
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
#include "uiVendorMan/uiVendorSearch/uiVendorSearch.h"
#endif//_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

const DWORD IM_VENDOR_MANAGER::VENDOR_SELL			= StrToWzID("0480");
const DWORD IM_VENDOR_MANAGER::VENDOR_BUY			= StrToWzID("0481");
const DWORD IM_VENDOR_MANAGER::VENDOR_POPUP			= StrToWzID("0880");
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
const DWORD IM_VENDOR_MANAGER::VENDOR_BUY_POPUP		= StrToWzID("0486");
#endif
const DWORD IM_VENDOR_MANAGER::VENDOR_LIST			= StrToWzID("0485");
const DWORD IM_VENDOR_MANAGER::VENDOR_TITLE_POPUP	= StrToWzID("0483");
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
const DWORD IM_VENDOR_MANAGER::VENDOR_SEARCH = StrToWzID("0487");
#endif//_NA_005988_20120901_VENDOR_SEARCH_SYSTEM500

//------------------------------------------------------------------------------
/**
*/
BOOL uiVendorMan::_LoadUI()
{
	m_pUIVendorSell			= CreateUIDialog<uiVendorSell>(
								IM_VENDOR_MANAGER::VENDOR_SELL,
								"Data\\Interface\\48_1_Personal_Vendor_Window(sell).iwz", 
								this, TRUE);
	m_pUIVendorBuy			= CreateUIDialog<uiVendorBuy>(
								IM_VENDOR_MANAGER::VENDOR_BUY, 
								"Data\\Interface\\48_2_Personal_Vendor_Window(Buy).iwz", 
								this, TRUE);
	m_pUIVendorPopup		= CreateUIDialog<uiVendorPopup>(
								IM_VENDOR_MANAGER::VENDOR_POPUP, 
								"Data\\Interface\\88_personal_popup.iwz", 
								this, TRUE);
	m_pUIVendorList			= CreateUIDialog<uiVendorList>(
								IM_VENDOR_MANAGER::VENDOR_LIST, 
								"Data\\interface\\48_5_Personal_Vendor_List.iwz", 
								this, TRUE);
	m_pUIVendorTitlePopup	= CreateUIDialog<uiVendorTitlePopup>(
								IM_VENDOR_MANAGER::VENDOR_TITLE_POPUP, 
								"Data\\Interface\\48_3_Input_Message.iwz", 
								this, TRUE);
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
	m_pUIVendorBuyPopup		= CreateUIDialog<uiVendorBuyPopup>(
		IM_VENDOR_MANAGER::VENDOR_BUY_POPUP, 
		"Data\\Interface\\48_6_seperatebuy_popup.iwz", 
		this);
#endif

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    ui_vendor_search_ptr_	= CreateUIDialog<uiVendorSearch>(
        IM_VENDOR_MANAGER::VENDOR_SEARCH, 
        "Data\\Interface\\48_7_Personal_Vendor_Search.iwz", 
        this, TRUE);
#endif//_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

	return TRUE;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------