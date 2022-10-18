//=======================================================================================================================
/// VendorPage class
/**
@author
Kim Chang Hyun< shogen@webzen.com >	
@since
2006. 9. 16~
@note 
- 
@history
-

*/
//=======================================================================================================================

#pragma once

class VendorSlotContainer;
class Player;

class VendorPage
{
public:
	VendorPage(void);
	~VendorPage(void);

	void CreatePage(Player * pOnlooker);
	void InitPage();
	void InitSearchVec();
	void DestoryPage();
	
	BOOL SetPage(eVENDOR_LIST_PAGE_KIND ekind, BYTE page, ZONEKEY zonekey);
	BOOL GetVendorListTotalInfo(VENDOR_LIST_TOTAL_INFO& VendorListTotalInfo) const;
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    BOOL Search(eVENDOR_LIST_PAGE_KIND ekind, const TCHAR* pat, ZONEKEY zonekey, SLOTCODE ItemCode = 0);
#else //_NA_0_20091208_VENDOR_ITEM_SEARCH
    BOOL Search(eVENDOR_LIST_PAGE_KIND ekind, const TCHAR* pat, ZONEKEY zonekey);
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH

private:
	BOOL MakePageInAll(BYTE byPage, ZONEKEY zonekey);
	BOOL MakePageInSearch(BYTE byPage);
	
	void SearchTitle(ZONEKEY zonekey);
	void SearchID(ZONEKEY zonekey);
#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    void SearchItem(ZONEKEY zonekey, SLOTCODE ItemCode);
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH


	void CalFailFuction();
	BOOL IsIncludePattern(const TCHAR* str);

private:
	VendorSlotContainer*					m_VendorContainerArray[COUNT_VENDOR_IN_PAGE];		//Page List	
	STLX_VECTOR<VendorSlotContainer*>		m_SearchContainerVector;
	eVENDOR_LIST_PAGE_KIND					m_eSearchedKind;
	TCHAR									m_Pattern[MAX_VENDOR_SEARCH_LENGTH+1];
	TCHAR									m_PatternFail[MAX_VENDOR_SEARCH_LENGTH+1];
	BYTE									m_PatternLength;
	Player*									m_pOnlooker;
	BYTE									m_byCountAllPage;									//��ü �������� �� - Make###�Լ����� ��������.
	BYTE									m_byCountListInPage;								//���������� ����ִ� ����Ʈ �� (Not MAX) - Make###�Լ����� ��������.
};