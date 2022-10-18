#ifndef _uiVendorList_def_h_
#define _uiVendorList_def_h_

// 스크롤 범위 지정.

#define MAX_VISIBLE_LIST_NUM	10

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	#define MAX_VISIBLE_FINDLIST_NUM	6
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

enum
{
	IS_CG_VENDOR_LIST_START_SYN = 0 ,
	IS_CG_VENDOR_LIST_END_SYN,
	IS_CG_VENDOR_REQUIRE_PAGE_SYN,
	IS_CG_VENDOR_SEARCH_SYN,
	SYN_MAX,
};

#endif // _uiVendorList_def_h_
