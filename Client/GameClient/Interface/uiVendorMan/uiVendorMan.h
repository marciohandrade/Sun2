#ifndef     __USER_INTERFACE_MANAGER_VENDOR_H__
#define     __USER_INTERFACE_MANAGER_VENDOR_H__
//------------------------------------------------------------------------------
/**
    @class uiVendorMan

    interface manager for a vendor

    (C) 2005 ckbang
*/
#include "uiVendorMan_def.h"
#include "uibaseman.h"

class uiVendorBuy;
class uiVendorSell;
class uiVendorPopup;
class uiVendorTitlePopup;
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
class uiVendorBuyPopup;
#endif
class uiVendorList;
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
class uiVendorSearch;
#endif//_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

class uiVendorMan :
    public uiBaseMan
{
public:
    uiVendorMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnInitialize();
	DWORD					GetPendingSaler() { return m_dwSalerKey;}
	void					SetPendingSaler(DWORD dwKey) { m_dwSalerKey = dwKey;}

private:
    BOOL                    _LoadUI();

public:
	void					SetWinPos(POINT pt){ m_ptWinPos = pt; }
	POINT					GetWinPos(){ return m_ptWinPos; }

    //--------------------------------------------------------------------------
    // functions
    //
public:

    //--------------------------------------------------------------------------
    // procedure
    //
public:
	void					__errorcode(int iError);

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiVendorBuy*            m_pUIVendorBuy;
    uiVendorSell*           m_pUIVendorSell;
    uiVendorPopup*          m_pUIVendorPopup;
    uiVendorTitlePopup*     m_pUIVendorTitlePopup;
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
	uiVendorBuyPopup *		m_pUIVendorBuyPopup;
#endif
	uiVendorList *			m_pUIVendorList;
	DWORD					m_dwSalerKey;
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    uiVendorSearch* ui_vendor_search_ptr_;
#endif//_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

	POINT					m_ptWinPos;

};

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_MANAGER_VENDOR_H__
