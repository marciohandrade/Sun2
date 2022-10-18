#ifndef     __UIDEF_MANAGER_VENDOR_H__
#define     __UIDEF_MANAGER_VENDOR_H__

// vendor userinterface resource info

struct IM_VENDOR_MANAGER
{
	static const DWORD VENDOR_SELL;
	static const DWORD VENDOR_BUY;
	static const DWORD VENDOR_POPUP;
#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
	static const DWORD VENDOR_BUY_POPUP;
#endif
	static const DWORD VENDOR_LIST;
	static const DWORD VENDOR_TITLE_POPUP;
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    static const DWORD VENDOR_SEARCH;
#endif//_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
};

#endif
