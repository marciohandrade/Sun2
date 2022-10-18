#include "Stdafx.h"
#include "VendorPageManager.h"
#include "VendorPage.h"
#include ".\Player.h"
#include "ChaoState.h"
#include ".\FunctionalRestrictManager.h"
#include "GameZone.h"

VendorPageManager g_VendorPageManager;

VendorPageManager::VendorPageManager()
{
	m_VendorPagePool.Initialize( MAX_ONLOOKER_VENDOR_NUM, 0, "VendorPage", AssertionLogger4Pool_Server );
}

VendorPageManager::~VendorPageManager()
{
	m_VendorPagePool.Release();
}

RC::eVENDOR_LIST_RESULT VendorPageManager::StartVendorList( Player * pOnlooker)
{
	
	if( !IsEnableService() )
		return RC::RC_VENDOR_LIST_NOT_AVAILABLE_SERVICE;

	if( !IsValidPlayer(pOnlooker) )
		return RC::RC_VENDOR_LIST_INVALID_STATE;

	if( pOnlooker->GetBehave() != PLAYER_BEHAVE_IDLE_STATE)
		return RC::RC_VENDOR_LIST_INVALID_STATE;

	if( pOnlooker->GetVendorPage() != NULL )
		return RC::RC_VENDOR_LIST_ALREADY;

	VendorPage* pVendorPage = _Alloc();
	ASSERT( pVendorPage );
	pVendorPage->CreatePage(pOnlooker);
	
	pOnlooker->SetVendorPage(pVendorPage);	
	
	return RC::RC_VENDOR_LIST_SUCCESS;
}

RC::eVENDOR_LIST_RESULT VendorPageManager::RequirePage( Player * pOnlooker, eVENDOR_LIST_PAGE_KIND ePageKind, BYTE Page)
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_LIST_NOT_AVAILABLE_SERVICE;

	if( !IsValidPlayer(pOnlooker) )
		return RC::RC_VENDOR_LIST_INVALID_STATE;

	/*
	if( !IsValidOnlookerState(pOnlooker) )
		return RC::RC_VENDOR_LIST_INVALID_STATE;
	*/

	VendorPage* pPage = pOnlooker->GetVendorPage();
	if( pPage == NULL)
	{
		SUNLOG(eCRITICAL_LOG, "NULL Page refrence [Require] - [%s]", pOnlooker->GetCharName());		
		return RC::RC_VENDOR_LIST_REFRENCE_NULL;
	}

	if( !pPage->SetPage(ePageKind, Page, pOnlooker->GetGameZonePtr()->GetKey()) )
	{
		return RC::RC_VENDOR_LIST_NONE_PAGE;
	}
	
	return RC::RC_VENDOR_LIST_SUCCESS;
}

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    RC::eVENDOR_LIST_RESULT VendorPageManager::Search(Player * pOnlooker, eVENDOR_LIST_PAGE_KIND eKind, const TCHAR* Pattern, SLOTCODE ItemCode)
#else //_NA_0_20091208_VENDOR_ITEM_SEARCH
    RC::eVENDOR_LIST_RESULT VendorPageManager::Search(Player * pOnlooker, eVENDOR_LIST_PAGE_KIND eKind, const TCHAR* Pattern)
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH
{
	if( !IsEnableService())
		return RC::RC_VENDOR_LIST_NOT_AVAILABLE_SERVICE;

	if ( pOnlooker->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_LIST_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !IsValidPlayer( pOnlooker ) )
		return RC::RC_VENDOR_LIST_INVALID_STATE;
/*
	if( !IsValidOnlookerState( pOnlooker ) )
		return RC::RC_VENDOR_LIST_INVALID_STATE;
*/
	VendorPage* pPage = pOnlooker->GetVendorPage();
	if( pPage == NULL)
	{
		SUNLOG(eCRITICAL_LOG, "NULL Page refrence [Require] - [%s]", pOnlooker->GetCharName());		
		return RC::RC_VENDOR_LIST_REFRENCE_NULL;
	}

#ifdef _NA_0_20091208_VENDOR_ITEM_SEARCH
    if( !pPage->Search(eKind, Pattern, pOnlooker->GetGameZonePtr()->GetKey(), ItemCode) )
#else //_NA_0_20091208_VENDOR_ITEM_SEARCH
    if( !pPage->Search(eKind, Pattern, pOnlooker->GetGameZonePtr()->GetKey()) )
#endif //_NA_0_20091208_VENDOR_ITEM_SEARCH
	{
		return RC::RC_VENDOR_LIST_SEARCH_FAILED;
	}

	return RC::RC_VENDOR_LIST_SUCCESS;
}

RC::eVENDOR_LIST_RESULT VendorPageManager::EndVendorList( Player * pOnlooker)
{	
/*
	if( !IsValidOnlookerState(pOnlooker) )
		return RC::RC_VENDOR_LIST_INVALID_STATE;
*/
	
	VendorPage* pVendorPage = pOnlooker->GetVendorPage();
	//ASSERT( pVendorPage );
	if( pVendorPage == NULL )
	{
		return RC::RC_VENDOR_LIST_REFRENCE_NULL;
	}

	pVendorPage->DestoryPage();
	if( !_Free( pVendorPage ) )
	{
		SUNLOG( eCRITICAL_LOG, "Page 반환오류[%s]", pOnlooker->GetCharName() );
	}

	pOnlooker->SetVendorPage(NULL);

	return RC::RC_VENDOR_LIST_SUCCESS;
}

VENDOR_SLOT_CONTAINER_LIST_IT	VendorPageManager::AddContainer(ZONEKEY zonekey, VendorSlotContainer *pContainer)
{
	m_VendorZoneHash[zonekey].push_front(pContainer);

	return m_VendorZoneHash[zonekey].begin();
}

void VendorPageManager::DelContainer(ZONEKEY zonekey, VENDOR_SLOT_CONTAINER_LIST_IT it)
{
	m_VendorZoneHash[zonekey].erase( it );
}

BOOL VendorPageManager::IsValidPlayer( Player * pPlayer )
{
	// Player Release()시에는 Field가 NULL
	if( !pPlayer->GetField() )
		return FALSE;
	return TRUE;
}


BOOL VendorPageManager::IsEnableService()
{
	return g_FunctionalRestrictMgr.IsPossibleFunction( eFUNCTIONALRESTRICT_VENDOR_LIST_NPC );
}