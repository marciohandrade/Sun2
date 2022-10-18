//=======================================================================================================================
/// VendorPageManager class
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

#include <MemoryPoolFactory.h>
#include "VendorPage.h"

typedef STLX_LIST<VendorSlotContainer*>					VENDOR_SLOT_CONTAINER_LIST;
typedef VENDOR_SLOT_CONTAINER_LIST::iterator			VENDOR_SLOT_CONTAINER_LIST_IT;

class VendorPageManager
{
public:
	VendorPageManager(void);
	~VendorPageManager(void);
	
	RC::eVENDOR_LIST_RESULT StartVendorList( Player * pOnlooker);
	RC::eVENDOR_LIST_RESULT RequirePage( Player * pOnlooker, eVENDOR_LIST_PAGE_KIND ePageKind, BYTE Page);	

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    RC::eVENDOR_LIST_RESULT Search(Player * pOnlooker, eVENDOR_LIST_PAGE_KIND eKind, const TCHAR* Pattern, SLOTCODE ItemCode = 0);
#else //_NA_0_20091208_VENDOR_ITEM_SEARCH
    RC::eVENDOR_LIST_RESULT Search(Player * pOnlooker, eVENDOR_LIST_PAGE_KIND eKind, const TCHAR* Pattern);
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH

	RC::eVENDOR_LIST_RESULT EndVendorList( Player * pOnlooker);

	VENDOR_SLOT_CONTAINER_LIST_IT				AddContainer(ZONEKEY zonekey, VendorSlotContainer *pContainer);
	void										DelContainer(ZONEKEY zonekey, VENDOR_SLOT_CONTAINER_LIST_IT it);
	
	BOOL IsValidPlayer( Player * pPlayer );
	
	VENDOR_SLOT_CONTAINER_LIST&	GetVondorContainerList( ZONEKEY zonekey ) { return m_VendorZoneHash[zonekey]; }
	
private:
	BOOL IsEnableService();
	VendorPage * _Alloc() { return (VendorPage *)m_VendorPagePool.Alloc();	}
	BOOL _Free( VendorPage * po ) { return m_VendorPagePool.Free(po);	}

private:
	STLX_HASH_MAP<ZONEKEY, VENDOR_SLOT_CONTAINER_LIST>		m_VendorZoneHash;
	util::CMemoryPoolFactory<VendorPage>					m_VendorPagePool;
};

extern VendorPageManager g_VendorPageManager;