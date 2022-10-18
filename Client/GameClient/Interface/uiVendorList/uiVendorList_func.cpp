#include "SunClientPrecompiledHeader.h"

#include "uiVendorList/uiVendorList.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "ResultCode.h"
#include "Hero.h"

using namespace RC;
void uiVendorList::InitControl()
{
	m_pList = static_cast<CCtrlListWZ *> ( getcontrol(DLGPOS_LIST) );
	assert(m_pList);
	m_pList->SetHideReportRender(TRUE);
	m_pList->SetSellHeight(21);

	CCtrlStaticWZ * pNumberStaitc = (CCtrlStaticWZ*)getcontrol( DLGPOS_REPORT_BUTTON_NUMBER );
	CCtrlStaticWZ * pVendorNameStaitc = (CCtrlStaticWZ*)getcontrol( DLGPOS_REPORT_BUTTON_VENDOR_NAME );
	CCtrlStaticWZ * pSellerNameStaitc = (CCtrlStaticWZ*)getcontrol( DLGPOS_REPORT_BUTTON_SELLER_NAME );

#ifdef _INTERNATIONAL_UI
	GlobalFunc::SetReportButtonInfo( m_pList, 0, pNumberStaitc->GetSize().right+3, NULL );
	GlobalFunc::SetReportButtonInfo( m_pList, 1, pVendorNameStaitc->GetSize().right+3, NULL );
	GlobalFunc::SetReportButtonInfo( m_pList, 2, pSellerNameStaitc->GetSize().right+3, NULL );
#else
	GlobalFunc::SetReportButtonInfo( m_pList, 0, pNumberStaitc->GetSize().right, NULL );
	GlobalFunc::SetReportButtonInfo( m_pList, 1, pVendorNameStaitc->GetSize().right, NULL );
	GlobalFunc::SetReportButtonInfo( m_pList, 2, pSellerNameStaitc->GetSize().right, NULL );
#endif//_INTERNATIONAL_UI

	m_pEdit = static_cast<CCtrlDxIMEEditBox *> ( getcontrol(DLGPOS_EDIT_ID) );
	assert(m_pEdit);

	m_pCheck[__DLGPOS_CHECK_ID] = static_cast<CCtrlButtonCheckWZ * > 
									(getcontrol(DLGPOS_CHECK_ID) );
	assert(m_pCheck[__DLGPOS_CHECK_ID]);

	m_pCheck[__DLGPOS_CHECK_ROOM_NAME] = static_cast <CCtrlButtonCheckWZ *>
											(getcontrol(DLGPOS_CHECK_ROOM_NAME) );
	assert(m_pCheck[__DLGPOS_CHECK_ROOM_NAME]);

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	m_pCheck[__DLGPOS_CHECK_ITEM] = static_cast <CCtrlButtonCheckWZ *>
											(getcontrol(DLGPOS_CHECK_ITEM) );
	SUN_ASSERT(m_pCheck[__DLGPOS_CHECK_ITEM]);
	m_pCheck[__DLGPOS_CHECK_ITEM]->SetCheckState(FALSE);
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

	m_pCheck[__DLGPOS_CHECK_ROOM_NAME]->SetCheckState(TRUE);
	m_pCheck[__DLGPOS_CHECK_ID]->SetCheckState(FALSE);

	m_pTextPage = static_cast<CCtrlStaticWZ *> (getcontrol(DLGPOS_TXT_PAGE) );

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	find_list_ptr_ = static_cast<CCtrlListWZ *> ( getcontrol(DLGPOS_FIND_LIST) );
	SUN_ASSERT(find_list_ptr_);

	find_scroll_ = static_cast<CCtrlVScrollWZ *> ( getcontrol(DLGPOS_FIND_SCROLL) );
	SUN_ASSERT(find_scroll_);

	find_background_image_ = static_cast<CCtrlPictureWZ *> ( getcontrol(DLGPOS_FIND_BACKIMG) );
	SUN_ASSERT(find_background_image_);

	find_scroll_image_ = static_cast<CCtrlPictureWZ *> ( getcontrol(DLGPOS_FIND_SCROLLIMG) );
	SUN_ASSERT(find_scroll_image_);

	HideFindWord();
    UpdateDialogSize();
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

#ifdef _DH_SHOW_VENDOR_LIST_POPUP
    if (m_pList)
    {
        m_pList->SetUseRButtonSelect(true);
    }
#endif//_DH_SHOW_VENDOR_LIST_POPUP

	assert(m_pTextPage);
}	

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
void uiVendorList::UpdateDialogSize()
{
    // TODO: UI변경에 따른 적용 보류
    RECT disable_rectangle;
	GetDialogWZ()->GetClientRect(&disable_rectangle);

    if(find_background_image_)
    {
	    disable_rectangle.top = find_background_image_->GetSizeRect().top + 30;
	    if( find_background_image_->GetState() == WZ_SHOW )
	    {
		   disable_rectangle.left = find_list_ptr_->GetSizeRect().right + 30;
	    }
    }    
    DisableInterSectRect(disable_rectangle);
}

void uiVendorList::HideFindWord()	// 추천어 자동완성
{
    item_code_ = 0;
    set_search_string("");
	if (find_list_ptr_)
	{
		find_list_ptr_->ShowWindowWZ(WZ_HIDE);
        find_list_ptr_->SetCurSel(-1);
	}
	if (find_scroll_)
	{
		find_scroll_->ShowWindowWZ(WZ_HIDE);
	}
	if (find_background_image_)
	{
		find_background_image_->ShowWindowWZ(WZ_HIDE);
	}
	if (find_scroll_image_)
	{
		find_scroll_image_->ShowWindowWZ(WZ_HIDE);
	}
}

void uiVendorList::ShowFindWord()	// 추천어 자동완성
{
	if (find_list_ptr_)
	{
		find_list_ptr_->ShowWindowWZ(WZ_SHOW);
	}
	if (find_scroll_)
	{
		find_scroll_->ShowWindowWZ(WZ_SHOW);
	}
	if (find_background_image_)
	{
		find_background_image_->ShowWindowWZ(WZ_SHOW);
	}
	if (find_scroll_image_)
	{
		find_scroll_image_->ShowWindowWZ(WZ_SHOW);
	}
}

void uiVendorList::InsertFindList(const char* item_name)
{
	int index = find_list_ptr_->InsertItemBack();

#ifdef _INTERNATIONAL_UI
	DWORD dwListFont = StrToWzID("st10");
#else
	DWORD dwListFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI

	TCHAR szTitle[MAX_VENDOR_TITLE_LENGTH+1] = {0,};
	Snprintf(szTitle , MAX_VENDOR_TITLE_LENGTH , item_name);
	GlobalFunc::SetItem(find_list_ptr_ , index , 1,  szTitle ,dwListFont ,DT_VCENTER| DT_CENTER ,
		RGBA(0,0,0,0), 
		RGBA(180,180,180,255), 
		RGBA(100,100,180,255));
}

void uiVendorList::FillFindListData(TCHAR* pItemName)
{    
    unsigned int search_count = 0;
    _ITEM_NAME_INFO search_buffer;
    
    if (search_new_)  // 검색이 처음이라면
    {   // 새로 검색
	    find_list_ptr_->DeleteItemAll(); // 기존에 리스트 모두 지워버린다.
        item_name_info_iterator_ = ItemInfoParser::Instance()->GetItemNamesBegin();
        search_index_ = 0;
        search_new_ = false;
        ShowFindWord();
        searched_results_.Clear();
    }
    
    while (item_name_info_iterator_ != item_name_info_iterator_end_)
    {
        search_buffer.m_dwItemCode = item_name_info_iterator_->m_dwItemCode;
		search_buffer.m_stName = item_name_info_iterator_->m_stName.c_str();
        // 검색 알고리즘 적용
		if (search_buffer.m_stName.find(pItemName, 0) != -1)
		{
            searched_results_.PushBack(search_buffer);
			InsertFindList(search_buffer.m_stName.c_str());
            find_list_ptr_->SetItemData(search_index_, &(searched_results_[search_index_]));
			++search_index_;
		}
        // 검색 알고리즘 종료
        ++search_count;
        ++item_name_info_iterator_;
        if (search_count >= number_of_searching_per_a_frame_)
        {
            break;
        }        
    }

    if (item_name_info_iterator_ == item_name_info_iterator_end_)
    {
        set_search_end(true);
        search_new_ = true;     //검색 시작 초기화
        if (search_index_ > 0 && find_list_ptr_ && m_pEdit)
        {
            _ITEM_NAME_INFO select_item_name_info = searched_results_[0];
            TCHAR select_item_name[MAX_VENDOR_SEARCH_LENGTH+1] = "";
            find_list_ptr_->SetCurSel(0);
            item_code_ = select_item_name_info.m_dwItemCode;
            strncpy(select_item_name, select_item_name_info.m_stName.c_str(), MAX_VENDOR_SEARCH_LENGTH);
			GlobalFunc::SetText(m_pEdit, select_item_name);
			m_pEdit->SetTextWZ(select_item_name);
        }
        else
        {
            HideFindWord();
        }
    }

    find_scroll_current_value_ = 0;
	find_scroll_value_max_ = search_index_ - MAX_VISIBLE_FINDLIST_NUM;
	// 스크롤 범위 지정.
	find_scroll_->SetScrollPosWZ(find_scroll_current_value_);
	find_scroll_->SetScrollRangeWZ(0,find_scroll_value_max_);
	find_list_ptr_->SetBeginDrawIndex(find_scroll_current_value_);
}

void uiVendorList::set_search_string(TCHAR *search_string)
{ 
    strncpy(search_string_, search_string, MAX_VENDOR_SEARCH_LENGTH);    
    search_string_[MAX_VENDOR_SEARCH_LENGTH] = '\0';     
}
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

void uiVendorList::InsertList(TCHAR * pTitle , TCHAR *pMasterName)
{
	int index = m_pList->InsertItemBack();
	TCHAR szIndex[5] = {0,};
	Snprintf(szIndex , 4 , _T("%d") , index+1);

#ifdef _INTERNATIONAL_UI
	DWORD dwListFont = StrToWzID("st10");
#else
	DWORD dwListFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI

	GlobalFunc::SetItem(m_pList , index , 0,  szIndex ,dwListFont ,DT_VCENTER| DT_CENTER ,
		RGBA(0,0,0,0), 
		RGBA(180,180,180,255), 
		RGBA(100,100,180,255) );

	TCHAR szTitle[MAX_VENDOR_TITLE_LENGTH+1] = {0,};
	Snprintf(szTitle , MAX_VENDOR_TITLE_LENGTH , pTitle);
	GlobalFunc::SetItem(m_pList , index , 1,  szTitle ,dwListFont ,DT_VCENTER| DT_CENTER ,
		RGBA(0,0,0,0), 
		RGBA(180,180,180,255), 
		RGBA(100,100,180,255));

	TCHAR szName[ID_MAX_LEN+1] = {0,};
	Snprintf(szName , ID_MAX_LEN , pMasterName);
	GlobalFunc::SetItem(m_pList , index , 2,  szName ,dwListFont ,DT_VCENTER| DT_CENTER ,
		RGBA(0,0,0,0), 
		RGBA(180,180,180,255), 
		RGBA(100,100,180,255) );


}

void uiVendorList::FillVendorListData(VENDOR_LIST_TOTAL_INFO * pListInfo)
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH];
	// 기존에 리스트 모두 지워버린다.
	m_pList->DeleteItemAll();

	m_iMaxPage = pListInfo->m_AllPage;
	// 총 페이지수 보다 요청한 페이지가 더 크다면 작은걸로 셋팅해준다.
	m_iPage = ( m_iPage < m_iMaxPage ) ?  m_iPage : m_iMaxPage;
	Sprintf(szMessage , _T("%d") , m_iPage);
	GlobalFunc::SetCaption(m_pTextPage , szMessage);
	
	for( int i = 0; i < pListInfo->m_Count ; i ++ )
	{
		InsertList(pListInfo->m_List[i].m_pszTitle , pListInfo->m_List[i].m_pszID );
		
		m_dwObjectKeyAtList[i] = pListInfo->m_List[i].m_dwPlayerKey;
	}

	m_pList->SetBeginDrawIndex(0);

}

BOOL uiVendorList::IsValidPage(int page)
{
	if(page > 0 && page <= m_iMaxPage)
		return TRUE;
	else
		return FALSE;
}

void uiVendorList::ErrorCode(DWORD dwError)
{	
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	switch ( dwError)
	{
	case RC_VENDOR_LIST_INVALID_STATE:
		{
			//5830	잘못된 상태입니다.
			g_InterfaceManager.GetInterfaceString(5830 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_LIST_UNABLE_FUNCTION_FOR_CHAOSTATE:
		{
			// 5275 
			g_InterfaceManager.GetInterfaceString(eST_NOT_AVALIBLE_CHAO_STATE , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_LIST_SEARCH_FAILED:
		{
			//5808	개인상점 리스트 검색에 실패했습니다.
			g_InterfaceManager.GetInterfaceString(5808 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;

	case RC_VENDOR_LIST_ALREADY:
		{
			//5837	이미 개인상점 리스트가 열려 있습니다.
			g_InterfaceManager.GetInterfaceString(5837 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;
	default:
		{
			//5836	개인 상점 리스트와 관련한 이상 현상이 발생되었습니다.
			g_InterfaceManager.GetInterfaceString( 5836 , szMessage ,INTERFACE_STRING_LENGTH);
		}
		break;
	}
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szMessage);

}
