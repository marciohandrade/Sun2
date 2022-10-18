#ifndef _uiVendorList_h_
#define _uiVendorList_h_

#include "uiBase.h"
#include "uiVendorList/uiVendorList_def.h"

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
#include "ItemInfoParser.h"
#define SEARCH_FRAME_UPDATE_TIME 100
#define NUMBER_OF_SEARCHING_PER_A_FRAME 200
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

class uiVendorMan;
class uiVendorList : public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_BTN_REFRESH,
		DLGPOS_BTN_FIND,
		DLGPOS_BTN_VIEW,
		DLGPOS_BTN_EXIT,
		DLGPOS_CHECK_ID,
		DLGPOS_CHECK_ROOM_NAME,
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
		DLGPOS_CHECK_ITEM,
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
		DLGPOS_LIST,
		DLGPOS_EDIT_ID,
		DLGPOS_BTN_LEFT,
		DLGPOS_BTN_RIGHT,
		DLGPOS_BTN_ALL,
		DLGPOS_TXT_PAGE,
		DLGPOS_REPORT_BUTTON_NUMBER,
		DLGPOS_REPORT_BUTTON_VENDOR_NAME,
		DLGPOS_REPORT_BUTTON_SELLER_NAME,
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
		DLGPOS_FIND_BACKIMG,
		DLGPOS_FIND_SCROLLIMG,
		DLGPOS_FIND_LIST,
		DLGPOS_FIND_SCROLL,
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
		DLGPOS_MAX,
	};
	enum 
	{
		__DLGPOS_CHECK_ID , 
		__DLGPOS_CHECK_ROOM_NAME,
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
		__DLGPOS_CHECK_ITEM,
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
		__DLGPOS_CHECK_MAX,
	};
public:
	uiVendorList(InterfaceManager * pUIMan);
	virtual ~uiVendorList();
	virtual void Init(CDrawBase* pDrawBase);
	virtual void Release();
	virtual void Process(DWORD dwTick);
	virtual void OnRenderSolarDialog();
	virtual void OnShowWindow(BOOL val );
	virtual void MessageProc(SI_MESSAGE * pMessage );
	virtual void NetworkProc(MSG_BASE * pMsg );
public:
	void	NET_SEND_CG_VENDOR_LIST_START_SYN();
	void	NET_SEND_CG_VENDOR_LIST_END_SYN();
	void	NET_SEND_CG_VENDOR_REQUIRE_PAGE_SYN(int page, int searchKind);
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	void	NET_SEND_CG_VENDOR_SEARCH_SYN(TCHAR * pFindName, DWORD dwItemCode);
#else
	void	NET_SEND_CG_VENDOR_SEARCH_SYN(TCHAR *pFindName);
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
	void	NET_SEND_CG_VENDOR_VIEW_START_SYN(int index);
	void	InsertList( TCHAR * pTitle , TCHAR *pMasterName);
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH	
	void	InsertFindList(const char* item_name);
    bool    search_end(){ return search_end_; }
    void    set_search_end(bool search_end){ search_end_ = search_end; }
    TCHAR*  search_string(){ return search_string_; }
    void    set_search_string(TCHAR* search_string);
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

protected:
	CControlWZ *	getcontrol(POSTYPE atPos);
	POSTYPE			getControlIDToPos(WzID wzID);
	uiVendorMan *	GetManager() {return pVendorMan; }
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	void			HideFindWord();
	void			ShowFindWord();
	void			FillFindListData(char * pItemName);
	void			UpdateDialogSize();
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
#ifdef _DH_SHOW_VENDOR_LIST_POPUP
    void ShowCharacterPopupMenu(BOOL is_show, DWORD index);
#endif//_DH_SHOW_VENDOR_LIST_POPUP

private:
	void			InitControl();
	void			FillVendorListData(VENDOR_LIST_TOTAL_INFO * pListInfo);
	BOOL			IsValidPage(int page);
	void			ErrorCode(DWORD dwError);

private:
	static WzID				m_wzID[DLGPOS_MAX];
	uiVendorMan*			pVendorMan;
	CCtrlListWZ*			m_pList;
    CCtrlDxIMEEditBox*		m_pEdit;
	CCtrlButtonCheckWZ* 	m_pCheck[__DLGPOS_CHECK_MAX];
	CCtrlStaticWZ*			m_pTextPage;

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	int						find_scroll_current_value_;
	int						find_scroll_value_max_;
	CCtrlListWZ*			find_list_ptr_;
	CCtrlVScrollWZ* 		find_scroll_;
	CCtrlPictureWZ*	    	find_background_image_;
	CCtrlPictureWZ*		    find_scroll_image_;
	DWORD					item_code_;
    TCHAR                   search_string_[MAX_VENDOR_SEARCH_LENGTH+1];
    bool                    search_new_;
    bool                    search_end_;
    int                     search_index_;
    unsigned int            search_frame_per_update_time_;
    unsigned int            number_of_searching_per_a_frame_;
    ITEM_NAME_INFO::iterator item_name_info_iterator_;
    ITEM_NAME_INFO::iterator item_name_info_iterator_end_;
	nArray<_ITEM_NAME_INFO>  searched_results_;
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
	int						m_iPage;	// List Page Index
	int						m_iMaxPage; // Max List Page Index
	int						m_iSearchType;
	int						m_iLastSearchType;
	DWORD					m_dwObjectKeyAtList[COUNT_VENDOR_IN_PAGE];
	BOOL					m_bIsSend[SYN_MAX];
	BOOL					m_bOpenVendorList;	
};

inline CControlWZ * uiVendorList::getcontrol(POSTYPE atPos)
{
	assert(atPos < DLGPOS_MAX );
	return GetControlWZ(m_wzID[atPos]);
}

inline POSTYPE uiVendorList::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < DLGPOS_MAX ; ++i )
	{
		if( m_wzID[i] == wzID)
		{
			return i;
		}
	}
	return DLGPOS_MAX;
}


#endif // _uiVendorList_h_
