#include "SunClientPrecompiledHeader.h"

#include "uiVendorList/uiVendorList.h"
#include "GlobalFunc.h"


uiVendorList::uiVendorList(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
	for( int i = 0; i < SYN_MAX ; ++i)
	{
		m_bIsSend[i] = FALSE;
	}
}

uiVendorList::~uiVendorList()
{


}


void uiVendorList::Release()
{

}

void uiVendorList::Init(CDrawBase* pDrawBase )
{
	uiBase::Init(pDrawBase);
	InitControl();
	m_iPage = 1;
	m_iSearchType = PAGE_REQUIRE_KIND_ALL;
	m_iLastSearchType = PAGE_REQUIRE_KIND_ALL;
	m_iMaxPage = 1;
	
	m_bOpenVendorList = FALSE;
	GlobalFunc::SetCaption(m_pTextPage , _T("1") );

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
	item_code_ = 0;
    search_new_ = true;
    set_search_end(true);
    set_search_string("");
    item_name_info_iterator_ = ItemInfoParser::Instance()->GetItemNamesBegin();
    item_name_info_iterator_end_ = ItemInfoParser::Instance()->GetItemNamesEnd();
    search_index_ = 0;
    search_frame_per_update_time_ = PROGRAMPARAM->search_frame_per_update_time();
    number_of_searching_per_a_frame_ = PROGRAMPARAM->number_of_searching_per_a_frame();
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
}


void uiVendorList::OnRenderSolarDialog()
{
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    UpdateDialogSize();
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
}

void uiVendorList::Process(DWORD dwTick)
{
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    static DWORD delay_tick = 0;
    
    if (!search_end())   // 검색이 끝인가?
    {
	    delay_tick += dwTick;
	    if (delay_tick > search_frame_per_update_time_)
	    {
            // 하던 검식 계속 진행
            FillFindListData(search_string());
		    delay_tick = 0;
	    }
    }
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
}