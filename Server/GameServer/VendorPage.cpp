#include "Stdafx.h"
#include "VendorPage.h"
#include "VendorPageManager.h"
#include "VendorSlotContainer.h"
#include ".\Player.h"
#include "GameZone.h"

VendorPage::VendorPage()
{
}

VendorPage::~VendorPage()
{
}

void VendorPage::CreatePage(Player * pOnlooker)
{	
	m_SearchContainerVector.reserve(COUNT_SEARCH_CONTAINER_INIT);
	
	m_byCountAllPage	= 0;
	m_byCountListInPage = 0;
		
	m_pOnlooker = pOnlooker;

	InitPage();
	InitSearchVec();
}

void VendorPage::DestoryPage()
{
	m_pOnlooker = NULL;
}


void VendorPage::InitPage()
{
	for(int i = 0 ; i < COUNT_VENDOR_IN_PAGE; i++)
		m_VendorContainerArray[i] = NULL;
}

void VendorPage::InitSearchVec()
{
	m_SearchContainerVector.resize(0);
	m_PatternLength = 0;

	memset(m_Pattern, 0, MAX_VENDOR_SEARCH_LENGTH+1);
	memset(m_PatternFail, 0, MAX_VENDOR_SEARCH_LENGTH+1);
}

BOOL VendorPage::SetPage(eVENDOR_LIST_PAGE_KIND ekind, BYTE page, ZONEKEY zonekey)
{
	switch(ekind)
	{
	case PAGE_REQUIRE_KIND_ALL:
		m_eSearchedKind = PAGE_REQUIRE_KIND_ALL;
		return MakePageInAll(page, zonekey);
		break;
	case PAGE_REQUIRE_KIND_TITLE:		
	case PAGE_REQUIRE_KIND_ID:
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    case PAGE_REQUIRE_KIND_ITEM:
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH
		if(ekind != m_eSearchedKind)
			return FALSE;		
		return MakePageInSearch(page);
		break;
	default:
		SUNLOG( eCRITICAL_LOG, "Require wrong PAGE_KIND(SetPage) [%d]", ekind);
		return FALSE;
		break;
	}
}

BOOL VendorPage::GetVendorListTotalInfo(VENDOR_LIST_TOTAL_INFO& vendor_list_total_info) const
{
    BYTE vendor_list_index = 0;

	for (BYTE i = 0 ; i < m_byCountListInPage; ++i)
	{
        VendorSlotContainer* const vendor_container = m_VendorContainerArray[i];
		if (vendor_container == NULL)
        {
            return FALSE;
        }        
        const Player* const establisher = vendor_container->GetEstablisher();
        if (establisher == NULL)
        {
            continue;
        }

        VENDOR_LIST_INFO& vendor_list_info = vendor_list_total_info.m_List[vendor_list_index];
		_tcsncpy(
            vendor_list_info.m_pszTitle, vendor_container->GetTitle(), MAX_VENDOR_TITLE_LENGTH);
        CHECK_ID_LENGTH_BUFFER(vendor_list_info.m_pszID);
		_tcsncpy(
            vendor_list_info.m_pszID, establisher->GetCharName(), _countof(vendor_list_info.m_pszID));
        vendor_list_info.m_pszID[_countof(vendor_list_info.m_pszID) - 1] = '\0';
		vendor_list_info.m_dwPlayerKey = establisher->GetObjectKey();
        ++vendor_list_index;
	}

	vendor_list_total_info.m_AllPage	= m_byCountAllPage;
    vendor_list_total_info.m_Count = vendor_list_index;

	return TRUE;	
}

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    BOOL VendorPage::Search(eVENDOR_LIST_PAGE_KIND ekind, const TCHAR* pat, ZONEKEY zonekey, SLOTCODE ItemCode)
#else //_NA_0_20091208_VENDOR_ITEM_SEARCH
    BOOL VendorPage::Search(eVENDOR_LIST_PAGE_KIND ekind, const TCHAR* pat, ZONEKEY zonekey)
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH
{
	InitSearchVec();

	_tcsncpy(m_Pattern, pat, MAX_VENDOR_SEARCH_LENGTH);
	m_Pattern[MAX_VENDOR_SEARCH_LENGTH] = '\0';
	m_PatternLength = _tcslen(m_Pattern);

	//실패함수 작성	
	CalFailFuction();

	switch(ekind)
	{
	case PAGE_REQUIRE_KIND_TITLE:
		SearchTitle(zonekey);
		return TRUE;
		break;
	case PAGE_REQUIRE_KIND_ID:
		SearchID(zonekey);
		return TRUE;
		break;

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    case PAGE_REQUIRE_KIND_ITEM:
        SearchItem(zonekey, ItemCode);
        return TRUE;
        break;
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH

	default:
		SUNLOG( eCRITICAL_LOG, "Require wrong PAGE_KIND(SetPage) [%d]", ekind);		
		return FALSE;
		break;
	}
}

BOOL VendorPage::MakePageInAll(BYTE byPage, ZONEKEY zonekey)
	{
	//Vendor 매니져로 부터 리스트 가져옴
	VENDOR_SLOT_CONTAINER_LIST& VendorList = g_VendorPageManager.GetVondorContainerList(zonekey);
	DWORD VendorListSize = (DWORD)VendorList.size();
	
	//페이지에 넣을 리스트가 없음
	if(VendorListSize == 0)
	{
		m_byCountAllPage = 0;
		m_byCountListInPage = 0;
		return FALSE;
	}
	
	//페이지 비움
	InitPage();

	//총 몇 페이지인지 계산	
	if( (VendorListSize / COUNT_VENDOR_IN_PAGE) > 255)
		return FALSE;

	m_byCountAllPage =  (BYTE)( ((VendorListSize - 1) / COUNT_VENDOR_IN_PAGE ) + 1);
	//요청페이지가 총 페이지 보다 크면 젤 끝 페이지 전송
	if(byPage > m_byCountAllPage)
		byPage = m_byCountAllPage;
	
	m_byCountListInPage = 0;
	DWORD		dwCount = 0;
	VENDOR_SLOT_CONTAINER_LIST_IT it = VendorList.begin();
	while(it != VendorList.end())
	{
		//페이지가 요청보다 작으면 skip
		if(dwCount < (DWORD)((byPage - 1) * COUNT_VENDOR_IN_PAGE))
		{
			dwCount++;
			it++;

			continue;
		}

		//페이지가 요청보다 크면 멈추자
		if(dwCount >= (DWORD)((byPage * COUNT_VENDOR_IN_PAGE)))
		{
			break;
		}

		//삽입
		m_VendorContainerArray[m_byCountListInPage] = (*it);
		m_byCountListInPage++;

		dwCount++;
		it++;
	}
	return TRUE;
}

BOOL VendorPage::MakePageInSearch(BYTE byPage)
{
	DWORD VendorSearchListSize = (DWORD)m_SearchContainerVector.size();
	if(VendorSearchListSize == 0)
	{
		m_byCountAllPage = 0;
		m_byCountListInPage = 0;
		return FALSE;
	}
	
	//페이지 비움
	InitPage();

	//총 몇 페이지인지 계산	
	if( (VendorSearchListSize / COUNT_VENDOR_IN_PAGE) > 255)
		return FALSE;

	m_byCountAllPage =  (BYTE)( ((VendorSearchListSize - 1) / COUNT_VENDOR_IN_PAGE ) + 1);
	if(byPage > m_byCountAllPage)
		return FALSE;
	
	m_byCountListInPage = 0;	
	for(DWORD i = (byPage - 1) * COUNT_VENDOR_IN_PAGE
		; i < ( (VendorSearchListSize < DWORD(byPage * COUNT_VENDOR_IN_PAGE)) ? VendorSearchListSize : DWORD(byPage * COUNT_VENDOR_IN_PAGE) )
		; i++ )
	{		
		m_VendorContainerArray[m_byCountListInPage] = m_SearchContainerVector[i];
		m_byCountListInPage++;
	}

	return TRUE;
}

void VendorPage::SearchTitle(ZONEKEY zonekey)
{
	m_eSearchedKind = PAGE_REQUIRE_KIND_TITLE;	

	//검색해서 패턴이 있으면 추가
	VENDOR_SLOT_CONTAINER_LIST& VendorList = g_VendorPageManager.GetVondorContainerList(zonekey);
    VENDOR_SLOT_CONTAINER_LIST_IT it = VendorList.begin();

	while(it != VendorList.end())
	{
		
		if(IsIncludePattern((*it)->GetTitle()))
			m_SearchContainerVector.push_back((*it));	

		it++;
	}
}

void VendorPage::SearchID(ZONEKEY zonekey)
{
	m_eSearchedKind = PAGE_REQUIRE_KIND_ID;	

	//검색해서 패턴이 있으면 추가
	VENDOR_SLOT_CONTAINER_LIST& vendor_list = g_VendorPageManager.GetVondorContainerList(zonekey);
    VENDOR_SLOT_CONTAINER_LIST::const_iterator it = vendor_list.begin();
	while (it != vendor_list.end())
	{
        VendorSlotContainer* const vendor_conationer = *it;
        if (vendor_conationer != NULL)
        {
            const Player* const establisher = vendor_conationer->GetEstablisher();
            if (establisher != NULL)
            {
                if (IsIncludePattern(establisher->GetCharName()))
                {
                    m_SearchContainerVector.push_back(vendor_conationer);
                }
            }
        }
		++it;
	}
}

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
void VendorPage::SearchItem(ZONEKEY zonekey, SLOTCODE ItemCode)
{
    m_eSearchedKind = PAGE_REQUIRE_KIND_ITEM;	

    //검색해서 패턴이 있으면 추가
    VENDOR_SLOT_CONTAINER_LIST& VendorList = g_VendorPageManager.GetVondorContainerList(zonekey);
    VENDOR_SLOT_CONTAINER_LIST_IT it = VendorList.begin();

    while(it != VendorList.end())
    {		
        if((*it)->HasItem(ItemCode))
            m_SearchContainerVector.push_back((*it));		

        it++;
    }
}
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH

void VendorPage::CalFailFuction()
{	
	m_PatternFail[0] = -1;
	for(int j = 1; j < m_PatternLength; j++)
	{
		int i = m_PatternFail[j-1];
		while((m_Pattern[j] != m_Pattern[i+1]) && (i >= 0))
			i=m_PatternFail[i];
		if(m_Pattern[j] == m_Pattern[i+1])
			m_PatternFail[j] = i+1;
		else
			m_PatternFail[j] = -1;
	}
}

BOOL VendorPage::IsIncludePattern(const TCHAR* str)
{
	DWORD PosP = 0, PosS = 0;	
	int LengthS = _tcslen(str);

	while( (PosP < m_PatternLength) && (PosS < (DWORD)LengthS))
	{
		if(m_Pattern[PosP] == str[PosS])
		{
			PosP++;
			PosS++;
		}
		else
		{
			if(PosP == 0)
				PosS++;
			else
				PosP = m_PatternFail[PosP-1]+1;
		}		
	}

	if(PosP<m_PatternLength)
	{
		return FALSE;
	}

	else
	{
		return TRUE;
	}
}