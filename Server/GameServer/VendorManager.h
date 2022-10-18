#pragma once

#include <MemoryPoolFactory.h>
#include <SafeStringBufferControl.h>
#include "VendorSlotContainer.h"
#include "VendorSearchSystem/VendorSearchSystem.h"

//=======================================================================================================================
/// VendorManager class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2004. 8. 11~
	@note 
			- 
	@history
			- 2006. 3. 7 : vendor���� �����丵, ������ �� �Ǵ� ���� fixed
			
*/
//=======================================================================================================================


struct VENDOR_ITEMSLOTEX;

typedef STLX_LIST<VendorSlotContainer*>			VENDOR_SLOT_CONTAINER_LIST;
typedef VENDOR_SLOT_CONTAINER_LIST::iterator	VENDOR_SLOT_CONTAINER_LIST_IT;

struct st_VendorContainerPTR
{
	VendorSlotContainer*			pVendorSlotContainer;
	VENDOR_SLOT_CONTAINER_LIST_IT	it;						//������ list�� �ٷ� �����ϱ� ���ؼ�...
	ZONEKEY							ZoneKey;				//������������ ��Ű...(���⺸�� �����̳ʰ� �����°� ������...)
};

class VendorManager
{
public:
	VendorManager(void);
	~VendorManager(void);

    void Initialize();
    void Destroy();
    void Update();

	// ������ �޼���
	RC::eVENDOR_RESULT StartVendor(Player * pEstablishedPlayer, 
                                   const util::StrArrayForVendorTitle& title,
                                   const ESTABLISHER_VENDOR_TOTAL_INFO& TotalInfo);
	RC::eVENDOR_RESULT EndVendor( Player * pEstablishedPlayer );

	RC::eVENDOR_RESULT ModifyRequest( Player * pEstablishedPlayer, POSTYPE VendorPos );
	RC::eVENDOR_RESULT ModifyCancel( Player * pEstablishedPlayer, POSTYPE VendorPos );

	RC::eVENDOR_RESULT ModifyApply( Player * pEstablishedPlayer, const VENDOR_ITEMSLOT & IN SlotInfo, VENDOR_ITEMSLOTEX & OUT ModifiedSlotInfo );
	RC::eVENDOR_RESULT Insert( Player * pEstablishedPlayer, const VENDOR_ITEMSLOT & IN SlotInfo, VENDOR_ITEMSLOTEX & OUT ModifiedSlotInfo );
	RC::eVENDOR_RESULT Delete( Player * pEstablishedPlayer, POSTYPE VendorPos );

    RC::eVENDOR_RESULT ModifyTitle(Player* pEstablishedPlayer,
        const util::StrArrayForVendorTitle& title);

	// ������ �޼���
	RC::eVENDOR_RESULT StartViewVendor( Player * pEstablisher, Player * pObserver );
	RC::eVENDOR_RESULT EndViewVendor( Player * pObserver );
	

	RC::eVENDOR_RESULT Buy( Player * pObserver, POSTYPE VendorPos, BYTE BuyNum, INVENTORY_TOTAL_INFO & OUT TotalInfo );

	BOOL ValidEstablisherState( Player * pPlayer );
	BOOL ValidObserverState( Player * pPlayer );
	BOOL ValidPlayer( Player * pPlayer );

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    void vendor_search_insert( Player* player, const ESTABLISHER_VENDOR_TOTAL_INFO& TotalInfo);
    void vendor_search_delete( Player* player, POSTYPE pos );
    void vendor_search_find( Player* player, const char* search_name, int page);
    void vendor_search_recv();
    void vendor_search_selected(DWORD guid, SEARCH_RESULT& search_result);
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

private:
	BOOL IsEnableService();
	VendorSlotContainer * _Alloc() { return (VendorSlotContainer *)m_pVendorPool.Alloc();	}
	BOOL _Free( VendorSlotContainer * po ) { return m_pVendorPool.Free(po);	}
private:
	util::CMemoryPoolFactory<VendorSlotContainer>	m_pVendorPool;

	STLX_HASH_MAP<DWORD, st_VendorContainerPTR>		m_VendorSlotContainerHashMap;

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    VendorSearchSystem vendorsearch_system_;
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
};

extern VendorManager g_VendorManager;