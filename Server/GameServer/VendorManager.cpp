#include "StdAfx.h"
#include ".\vendormanager.h"
#include ".\VendorSlotContainer.h"
#include ".\Player.h"
#include ".\VendorSlot.h"
#include ".\ItemManager.h"
#include ".\SCItemSlotContainer.h"
#include ".\GameServerEx.h"
#include ".\FunctionalRestrictManager.h"
#include "ChaoState.h"
#include "GameZone.h"
#include "VendorPageManager.h"
#include <ProhibitionWordParser.h>

VendorManager g_VendorManager;

VendorManager::VendorManager(void)
{
    m_pVendorPool.Initialize(0, MAX_PERSONAL_VENDOR_SLOT_NUM, "VendorSlotContainer");
}

VendorManager::~VendorManager(void)
{
	m_pVendorPool.Release();
}

void VendorManager::Initialize()
{
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    vendorsearch_system_.initialize();
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
}

void VendorManager::Destroy()
{
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    vendorsearch_system_.close();
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
}

void VendorManager::Update()
{
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    vendor_search_recv();
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
}

BOOL VendorManager::IsEnableService()
{
	return g_FunctionalRestrictMgr.IsPossibleFunction( eFUNCTIONALRESTRICT_USE_VENDOR );
}
BOOL VendorManager::ValidEstablisherState( Player * pPlayer )
{
	/*if( !pPlayer->GetField() )
		return FALSE;*/

	if( pPlayer->GetBehave() != PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE )
		return FALSE;

	return TRUE;
}

BOOL VendorManager::ValidPlayer( Player * pPlayer )
{
	// Player Release()�ÿ��� Field�� NULL
	if( !pPlayer->GetField() )
		return FALSE;
	return TRUE;
}

BOOL VendorManager::ValidObserverState( Player * pPlayer )
{
	/*
	// Player Release()�ÿ��� Field�� NULL
	if( !pPlayer->GetField() )
		return FALSE;
	*/

	if( pPlayer->GetBehave() != PLAYER_BEHAVE_VENDOR_OBSERVER_STATE )
		return FALSE;

	return TRUE;
}

RC::eVENDOR_RESULT VendorManager::StartVendor(Player * pEstablishedPlayer, 
                                              const util::StrArrayForVendorTitle& title,
                                              const ESTABLISHER_VENDOR_TOTAL_INFO& TotalInfo)
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_NOT_AVAILABLE_SERVICE;

	if( !ValidPlayer(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pEstablishedPlayer->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE;
	
	if( pEstablishedPlayer->GetBehave() != PLAYER_BEHAVE_IDLE_STATE || !pEstablishedPlayer->GetField() )
		return RC::RC_VENDOR_INVALID_STATE;

	if( !ProhibitionWordParser::Instance()->CheckFilterName(title))
		return RC::RC_VENDOR_TITLE_BAN_WORD;

	
	if( !(pEstablishedPlayer->GetGameZonePtr()) )
		return RC::RC_VENDOR_INVALID_STATE;

	VendorSlotContainer * pContainer = _Alloc();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::StartVendor] VendorSlotContainer is Null" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

    if(!pContainer->CreateVendor(pEstablishedPlayer, title, TotalInfo))
	{
		pContainer->DestroyVendor();
		if( !_Free( pContainer ) )
		{
			SUNLOG( eCRITICAL_LOG, "�����ȯ����[%s]", pEstablishedPlayer->GetCharName() );
		}

		return RC::RC_VENDOR_INCLUDE_INVALID_ITEM_TO_START;
	}

	//Add to Container - shogen
	if( m_VendorSlotContainerHashMap.find(pEstablishedPlayer->GetObjectKey()) != m_VendorSlotContainerHashMap.end())
	{
		SUNLOG( eCRITICAL_LOG, "Already exist VendorContainer in Hash_map[%s:%u]", pEstablishedPlayer->GetCharName(), pEstablishedPlayer->GetBehave());
	}	
	m_VendorSlotContainerHashMap[pEstablishedPlayer->GetObjectKey()].pVendorSlotContainer	= pContainer;	
	m_VendorSlotContainerHashMap[pEstablishedPlayer->GetObjectKey()].it
		= g_VendorPageManager.AddContainer(pEstablishedPlayer->GetGameZonePtr()->GetKey(), pContainer);
	m_VendorSlotContainerHashMap[pEstablishedPlayer->GetObjectKey()].ZoneKey = pEstablishedPlayer->GetGameZonePtr()->GetKey();

	pEstablishedPlayer->SetVendorContainer(pContainer);
	pEstablishedPlayer->SetBehave(PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE);

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    vendor_search_insert(pEstablishedPlayer, TotalInfo);
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

	return RC::RC_VENDOR_SUCCESS;
}

// ȣ���ϱ� ���� �ֺ� �÷��̾�� ������ �����Ѵٰ� �˸���.
RC::eVENDOR_RESULT VendorManager::EndVendor( Player * pEstablishedPlayer )
{
	if( !ValidEstablisherState(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	VendorSlotContainer * pContainer = pEstablishedPlayer->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::EndVendor] pContainer is NULL!" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

	STLX_HASH_MAP<DWORD, st_VendorContainerPTR>::iterator it = m_VendorSlotContainerHashMap.find(pEstablishedPlayer->GetObjectKey());
	if(it == m_VendorSlotContainerHashMap.end())
	{
		SUNLOG( eCRITICAL_LOG, "Not exist VendorContainer from Hash_map[%s:%u]", pEstablishedPlayer->GetCharName(), pEstablishedPlayer->GetBehave());
	}	
	else
	{
		g_VendorPageManager.DelContainer( (it->second).ZoneKey, (it->second).it );
		m_VendorSlotContainerHashMap.erase(it);
	}

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    for(POSTYPE pos = 0; pos < pContainer->GetMaxSlotNum(); ++pos)
    {
        if (pContainer->IsEmpty(pos) == false) {
            vendor_search_delete(pEstablishedPlayer, pos);
        }
    }
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

	pContainer->DestroyVendor();
	if( !_Free( pContainer ) )
	{
		SUNLOG( eCRITICAL_LOG, "�����ȯ����[%s]", pEstablishedPlayer->GetCharName() );
	}

	pEstablishedPlayer->SetVendorContainer(NULL);
	pEstablishedPlayer->SetBehave(PLAYER_BEHAVE_IDLE_STATE);

	return RC::RC_VENDOR_SUCCESS;
}

RC::eVENDOR_RESULT VendorManager::ModifyRequest( Player * pEstablishedPlayer, POSTYPE VendorPos )
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_NOT_AVAILABLE_SERVICE;

	if( !ValidPlayer(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pEstablishedPlayer->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidEstablisherState(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	VendorSlotContainer * pContainer = pEstablishedPlayer->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::ModifyRequest] pContainer is NULL!" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

	if( !pContainer->ValidPos(VendorPos)  || pContainer->IsEmpty(VendorPos) || pContainer->IsLocked(VendorPos) )
	{
		return RC::RC_VENDOR_INVALID_VENDORSLOT;
	}

	// �� ��ġ�� ���� �������� ���� �Ǵ�.
	pContainer->SetLock( VendorPos, TRUE );

	return RC::RC_VENDOR_SUCCESS;
}

RC::eVENDOR_RESULT VendorManager::ModifyCancel( Player * pEstablishedPlayer, POSTYPE VendorPos )
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_NOT_AVAILABLE_SERVICE;

	if( !ValidPlayer(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pEstablishedPlayer->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidEstablisherState(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	VendorSlotContainer * pContainer = pEstablishedPlayer->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::ModifyCancel] pContainer is NULL!" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

	if( !pContainer->ValidPos(VendorPos) || pContainer->IsEmpty(VendorPos) || !pContainer->IsLocked(VendorPos) )
	{
		return RC::RC_VENDOR_INVALID_VENDORSLOT;
	}

	pContainer->SetLock( VendorPos, FALSE );

	return RC::RC_VENDOR_SUCCESS;
}

// ����
RC::eVENDOR_RESULT VendorManager::ModifyApply( Player * pEstablishedPlayer, const VENDOR_ITEMSLOT & IN SlotInfo, VENDOR_ITEMSLOTEX & OUT ModifiedSlotInfo )
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_NOT_AVAILABLE_SERVICE;

	if( !ValidPlayer(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pEstablishedPlayer->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidEstablisherState(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	VendorSlotContainer * pContainer = pEstablishedPlayer->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::ModifyApply] pContainer is NULL!" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

	if( !pContainer->ValidPos(SlotInfo.m_VendorPos) || 
		pContainer->IsEmpty(SlotInfo.m_VendorPos) || 
		!pContainer->IsLocked(SlotInfo.m_VendorPos) )
	{
		return RC::RC_VENDOR_INVALID_VENDORSLOT;
	}

	if( !pContainer->ModifySlot( SlotInfo.m_VendorPos, SlotInfo ) )
	{
		return RC::RC_VENDOR_INVALID_VENDORSLOT;
	}

	pContainer->SetLock( SlotInfo.m_VendorPos, FALSE );

	VendorSlot & rSlot = (VendorSlot &)pContainer->GetSlot( SlotInfo.m_VendorPos );
	rSlot.CopyOut( ModifiedSlotInfo );

	return RC::RC_VENDOR_SUCCESS;
}
// �߰�
RC::eVENDOR_RESULT VendorManager::Insert( Player * pEstablishedPlayer, const VENDOR_ITEMSLOT & IN SlotInfo, VENDOR_ITEMSLOTEX & OUT ModifiedSlotInfo )
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_NOT_AVAILABLE_SERVICE;

	if( !ValidPlayer(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pEstablishedPlayer->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidEstablisherState(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	VendorSlotContainer * pContainer = pEstablishedPlayer->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::Insert] pContainer is NULL!" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

	if( !pContainer->ValidPos(SlotInfo.m_VendorPos) || !pContainer->IsEmpty(SlotInfo.m_VendorPos) )
	{
		return RC::RC_VENDOR_INVALID_VENDORSLOT;
	}

	if( !pContainer->ModifySlot( SlotInfo.m_VendorPos, SlotInfo ) )
	{
		return RC::RC_VENDOR_INVALID_VENDORSLOT;
	}

	VendorSlot & rSlot = (VendorSlot &)pContainer->GetSlot(SlotInfo.m_VendorPos);
	rSlot.CopyOut( ModifiedSlotInfo );

	return RC::RC_VENDOR_SUCCESS;
}


RC::eVENDOR_RESULT VendorManager::Delete( Player * pEstablishedPlayer, POSTYPE VendorPos )
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_NOT_AVAILABLE_SERVICE;

	if( !ValidPlayer(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pEstablishedPlayer->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidEstablisherState(pEstablishedPlayer) )
		return RC::RC_VENDOR_INVALID_STATE;

	VendorSlotContainer * pContainer = pEstablishedPlayer->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::Delete] pContainer is NULL!" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

	// ���� �ɸ����� ���� �� �ִ�!
	if( !pContainer->ValidPos(VendorPos)  || pContainer->IsEmpty(VendorPos) || pContainer->IsLocked(VendorPos) )
	{
		return RC::RC_VENDOR_INVALID_VENDORSLOT;
	}

    SCSlot org_item_slot;
	pContainer->DeleteSlot( VendorPos, &org_item_slot );

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    vendor_search_delete(pEstablishedPlayer, org_item_slot.GetPos());
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM

	return RC::RC_VENDOR_SUCCESS;
}

RC::eVENDOR_RESULT VendorManager::ModifyTitle(
    Player* pEstablisher, const util::StrArrayForVendorTitle& title)
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_NOT_AVAILABLE_SERVICE;

	if( !ValidPlayer(pEstablisher) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pEstablisher->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidEstablisherState(pEstablisher) )
		return RC::RC_VENDOR_INVALID_STATE;

	VendorSlotContainer * pContainer = pEstablisher->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::ModifyTitle] pContainer is NULL!" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

    pContainer->SetTitle(title);

	return RC::RC_VENDOR_SUCCESS;
}

RC::eVENDOR_RESULT VendorManager::StartViewVendor( Player * pEstablisher, Player * pObserver )
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_NOT_AVAILABLE_SERVICE;

	if( !ValidPlayer(pEstablisher) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pEstablisher->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidEstablisherState(pEstablisher) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pObserver->GetBehave() != PLAYER_BEHAVE_IDLE_STATE || !pObserver->GetField() )
		return RC::RC_VENDOR_INVALID_STATE;

#ifdef  _NA_008570_20151116_MODIFY_VENDOR_SEARCH
#else
    if( pObserver->GetGameZonePtr()->GetMapCode() != pEstablisher->GetGameZonePtr()->GetMapCode())
        return RC::RC_VENDOR_DIFFERENT_ZONE;
#endif //_NA_008570_20151116_MODIFY_VENDOR_SEARCH

	//SUNLOG( eFULL_LOG, "��������[%s]", pObserver->GetCharName() );
	VendorSlotContainer * pContainer = pEstablisher->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::StartViewVendor] VendorSlotContainer is null, Establisher = %s, pObserver = %s", 
			pEstablisher->GetCharName(), pObserver->GetCharName() );
		return RC::RC_VENDOR_INVALID_STATE;
	}

	//memcpy( tszVenderTitle, pContainer->GetTitle(), sizeof(TCHAR)*MAX_VENDOR_TITLE_LENGTH );
	//pContainer->GetTotalInfo(VendorTotalInfo);
	pContainer->AddObserver( pObserver );

	pObserver->SetVendorContainer(pContainer);
	pObserver->SetBehave(PLAYER_BEHAVE_VENDOR_OBSERVER_STATE);

	return RC::RC_VENDOR_SUCCESS;
}

RC::eVENDOR_RESULT VendorManager::EndViewVendor( Player * pObserver )
{
	if( !ValidObserverState(pObserver) )
		return RC::RC_VENDOR_INVALID_STATE;

	//SUNLOG( eFULL_LOG, "���������[%s]", pObserver->GetCharName() );

	VendorSlotContainer * pContainer = pObserver->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::EndViewVendor] pContainer is NULL!" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

	pContainer->RemoveObserver( pObserver );

	pObserver->SetVendorContainer(NULL);
	pObserver->SetBehave(PLAYER_BEHAVE_IDLE_STATE);

	return RC::RC_VENDOR_SUCCESS;
}

//#define _DEV_TAIYO_APPLY_COMMISSION

RC::eVENDOR_RESULT VendorManager::Buy( Player * pObserver, POSTYPE VendorPos, BYTE BuyNum, INVENTORY_TOTAL_INFO & OUT TotalInfo )
{
	if( !IsEnableService() )
		return RC::RC_VENDOR_NOT_AVAILABLE_SERVICE;

	if( !ValidPlayer(pObserver) )
		return RC::RC_VENDOR_INVALID_STATE;

	if( pObserver->GetChaoState().IsChaoState() )
		return RC::RC_VENDOR_UNABLE_FUNCTION_FOR_CHAOSTATE;

	if( !ValidObserverState(pObserver) )
		return RC::RC_VENDOR_INVALID_STATE;

	VendorSlotContainer * pContainer = pObserver->GetVendorContainer();
	ASSERT( pContainer );
	if( !pContainer )
	{
		SUNLOG( eCRITICAL_LOG, "[VendorManager::Buy] pContainer is NULL!" );
		return RC::RC_VENDOR_INVALID_STATE;
	}

	// ���� �ɸ����� ������ �� ����.
	if( !pContainer->ValidPos(VendorPos) || pContainer->IsEmpty(VendorPos) || pContainer->IsLocked(VendorPos) )
	{
		return RC::RC_VENDOR_INVALID_VENDORSLOT;
	}

	
	VendorSlot & rVendorSlot = (VendorSlot &)pContainer->GetSlot(VendorPos);
	BYTE SellNum = rVendorSlot.GetNum();

	// �ӽ� �Ҵ� : Ŭ���̾�Ʈ �۾� �Ϸ�Ǳ�������
	BuyNum = SellNum;

	// ����� ���� üũ
	if( 0 == BuyNum || BuyNum > SellNum )
		return RC::RC_VENDOR_INVALID_VENDORSLOT;

	// �� ���� �����ϴ� �� üũ
	if( pObserver->GetMoney() < rVendorSlot.GetMoney()*BuyNum )
		return RC::RC_VENDOR_HAVE_NO_MONEY;

	Player * pEstablisher = pContainer->GetEstablisher();
    if (pEstablisher == NULL)
    {
        return RC::RC_VENDOR_FAILED;
    }

#ifdef _NA_008570_20151116_MODIFY_VENDOR_SEARCH
#else
    if( pObserver->GetGameZonePtr()->GetMapCode() != pEstablisher->GetGameZonePtr()->GetMapCode())
        return RC::RC_VENDOR_DIFFERENT_ZONE;
#endif //_NA_008570_20151116_MODIFY_VENDOR_SEARCH

	if( !pEstablisher->CanPlusMoney( rVendorSlot.GetMoney()*BuyNum ) )
		return RC::RC_VENDOR_FAILED;

	// üũ
	ItemManager * pMGR = pObserver->GetItemManager();
	SCSlotContainer * pInventory = pMGR->GetItemSlotContainer( SI_INVENTORY );
#if SUN_CODE_BACKUP
	SCSlotContainer * pTmpInventory = pMGR->GetItemSlotContainer( SI_TEMPINVENTORY );
#endif
	if( rVendorSlot.GetLinkItem()->IsBlocked() )
	{
		return RC::RC_VENDOR_BLOCKED_ITEM;
	}

	//�� ��ȯ���� �������� �ŷ��Ұ�
    // (CHANGES) change to more generic code 'IsPetLocked' & 'IsRiderLocked' ...
    if(rVendorSlot.GetLinkItem()->IsUserBlocked())
        return RC::RC_VENDOR_BLOCKED_ITEM;
	if( rVendorSlot.GetLinkItem()->GetLaxLock()->IsLock() )
		return RC::RC_VENDOR_BLOCKED_ITEM;

    if (rVendorSlot.GetLinkItem()->IsPossessionItem() &&
        rVendorSlot.GetLinkItem()->GetIdentifyPossession() &&
        (!rVendorSlot.GetLinkItem()->IsDestroyItem()))
    {
        return RC::RC_VENDOR_CANNOT_TRADE_ITEM;
    }
    ;{
        const SCItemSlot* linked_item = rVendorSlot.GetLinkItem();
        if (linked_item->CanTradeSell(eITEM_TRADESELL_TRADE) == false) {
            return RC::RC_VENDOR_CANNOT_TRADE_ITEM;
        }
        if (linked_item->CanTradeSell(eITEM_TRADESELL_USED_ITEM) == false &&
            linked_item->IsProgressTime())
        {
            return RC::RC_VENDOR_CANNOT_TRADE_ITEM;
        }
    };

	// ������ ������?
	if( rVendorSlot.GetLinkItem()->GetNum() != SellNum )
	{
		return RC::RC_VENDOR_CANNOT_TRADE_ITEM;
	}

	SCItemSlot ItemSlot;
	ItemSlot.Copy( *rVendorSlot.GetLinkItem() );
	ItemSlot.SetLock(FALSE);
	ItemSlot.SetNum(1);


	RC::eITEM_RESULT rt = pMGR->Obtain( ItemSlot, BuyNum, &TotalInfo );
	if( rt != RC::RC_ITEM_SUCCESS )
	{
		return RC::RC_VENDOR_NOSPACEININVENTORY;
	}
	
	// �� ���
	MONEY price = rVendorSlot.GetMoney()*BuyNum;

	if( SellNum > BuyNum )
	{
		// ���� ����
		pContainer->UpdateSlot( rVendorSlot.GetPos(), -BuyNum );

		// �������� ���� �����۵� ����
		SCSlotContainer * pEstInventory = pEstablisher->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
		SCSlot & rOrgSlot = pEstInventory->GetSlot( rVendorSlot.GetLinkItem()->GetPos() );

		pEstInventory->UpdateSlot( rOrgSlot.GetPos(), -BuyNum );
	}
	else
	{
		// ���� ���� ����
		VendorSlot DeletedVendorSlot;
		pContainer->DeleteSlot( VendorPos, &DeletedVendorSlot );

		// �������� ���� �����۵� ����
		SCSlotContainer * pEstInventory = pEstablisher->GetItemManager()->GetItemSlotContainer( SI_INVENTORY );
		BOOL rt2 = pEstInventory->DeleteSlot( DeletedVendorSlot.GetLinkItem()->GetPos(), NULL );
		ASSERT( rt2 && "[Error]TradeItem Destroy is failed!" );

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
        vendor_search_delete(pEstablisher, DeletedVendorSlot.GetPos());
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
	}

#ifdef _DEV_TAIYO_APPLY_COMMISSION
	//������ ���
	MONEY Commission = (MONEY)(price*TRADE_COMMISSION_RATE);
#else
	MONEY Commission = (MONEY)(price*0);
#endif
	MONEY SellPrice = (price-Commission);
	pObserver->MinusMoney( price, Player::eMONEY_CIRCULATING );		// SetMoney�� �� ���� �κ� ���� MinusMoney()�� ��ü��(��â)
	pEstablisher->PlusMoney( SellPrice, Player::eMONEY_CIRCULATING );	// SetMoney�� �� �Դ� �κ� ���� PlusMoney()�� ��ü��(��â)


	//-------------------------------�α� �����---------------------------------------------------------------------------------
	// ���λ����� �������� �ѹ��� �Ѱ��� �ۿ� ���� ���Ѵ�.
	// ���� ��� 5���� �ö�� �־, ����ڴ� �ѹ��� �ϳ��� ���.
	//ITEMLOGINFO logInfo;
	//logInfo.nItemCode = ItemSlot.GetItemCode();
	//logInfo.nItemSerial = ItemSlot.GetSerial();
	//logInfo.nItemCount = SellNum;
	// ���λ���(���)
	GAMELOG->LogItem( ITEM_VENDOR_BUY, pObserver, &ItemSlot, price, BuyNum, pEstablisher->GetUserID(), pEstablisher->GetCharName() );
	// ���λ���(�ȱ�)
	GAMELOG->LogItem( ITEM_VENDOR_SELL, pEstablisher, &ItemSlot, SellPrice, BuyNum, pObserver->GetUserID(), pObserver->GetCharName() );

	GAMELOG->LogMoney( MONEY_VENDOR_BUY, pObserver, price, 0, pEstablisher->GetUserID(), pEstablisher->GetCharName() );
	GAMELOG->LogMoney( MONEY_VENDOR_SELL, pEstablisher, SellPrice, Commission, pObserver->GetUserID(), pObserver->GetCharName() );
	//-------------------------------�α� �����---------------------------------------------------------------------------------

	return RC::RC_VENDOR_SUCCESS;

}

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
void VendorManager::vendor_search_insert( \
    Player* player, const ESTABLISHER_VENDOR_TOTAL_INFO& TotalInfo )
{
    for (const ESTABLISHER_VENDOR_TOTAL_INFO::SELL_ITEM* it = &TotalInfo.sell_items_[0]; \
        it != &TotalInfo.sell_items_[TotalInfo.m_Count]; ++it) 
    {
        VENDOR_SEARCH_INSERT msg( \
            player->GetObjectKey(), it->szName, it->m_Slot.m_VendorPos, it->m_Slot.m_Money);

        vendorsearch_system_.SendToVendorSearchSystem(&msg, sizeof(msg));
    }
}

void VendorManager::vendor_search_delete( Player* player, POSTYPE pos )
{
    VENDOR_SEARCH_DELETE msg(player->GetObjectKey(), pos);
    vendorsearch_system_.SendToVendorSearchSystem(&msg, sizeof(msg));
}

void VendorManager::vendor_search_find( Player* player, const char* search_name, int page )
{
    VENDOR_SEARCH_SELECT msg(player->GetObjectKey(), search_name, page);
    vendorsearch_system_.SendToVendorSearchSystem(&msg, sizeof(msg));
}

void VendorManager::vendor_search_recv()
{
    static VENDOR_SEARCH_BUFFER buffer;
    while(vendorsearch_system_.RecvFromVendorSearchSystem(&buffer))
    {
        VENDOR_SEARCH_ACK* recv_packet = reinterpret_cast<VENDOR_SEARCH_ACK*>(&buffer);
        switch(recv_packet->packet_type_)
        {
        case eVendorSearchSelectAck:
            {
                VENDOR_SEARCH_SELECT_ACK* recv_ack = \
                    static_cast<VENDOR_SEARCH_SELECT_ACK*>(recv_packet);
                vendor_search_selected(recv_ack->guid, recv_ack->result);
            } break;
        }
    }
}

void VendorManager::vendor_search_selected(DWORD guid, SEARCH_RESULT& search_result)
{
    Player* const player = PlayerManager::Instance()->FindPlayerByObjKey(guid);
    if (!player)
        return;

    MSG_CG_VENDOR_ENHANCED_SEARCH_ACK msg;
    msg.clear();

    ENHANCED_VENDOR_SEARCH_RESULT& result = msg.search_result;
    result.current_page_ = search_result.current_page;
    result.total_page_   = search_result.total_page;
    result.item_count_   = 0;

    for (int i = 0; i < search_result.row_count; ++i) {
        SEARCH_RESULT_ROW& search_result_item = search_result.result_row[i];
        
        Player* const vendor_player = \
            PlayerManager::Instance()->FindPlayerByObjKey(search_result_item.guid);
        if (vendor_player == NULL || vendor_player->GetGameZonePtr() == NULL) {
            continue;
        }

        POSTYPE vendor_pos = search_result_item.pos;
        VendorSlotContainer* vendor_container = vendor_player->GetVendorContainer();
        if (!vendor_container 
            || !vendor_container->ValidPos(vendor_pos) 
            || vendor_container->IsEmpty(vendor_pos) 
            || vendor_container->IsLocked(vendor_pos)) {
            continue;
        }

        VENDOR_SEARCH_ITEM_INFO& vendor_items = result.vendor_items[result.item_count_];
        VENDOR_LIST_INFO& vendor_info = vendor_items.vendor_info_;
        {
            _tcsncpy(vendor_info.m_pszTitle, vendor_container->GetTitle(), MAX_VENDOR_TITLE_LENGTH);
            CHECK_ID_LENGTH_BUFFER(vendor_info.m_pszID);
		    _tcsncpy(vendor_info.m_pszID, vendor_player->GetCharName(), _countof(vendor_info.m_pszID));
            vendor_info.m_pszID[_countof(vendor_info.m_pszID) - 1] = '\0';
		    vendor_info.m_dwPlayerKey = vendor_player->GetObjectKey();
            vendor_info.zone_key_     = vendor_player->GetGameZonePtr()->GetMapCode();
        };

        VENDOR_ITEMSLOTEX& vendor_item = vendor_items.item_info_ex_;
        {
            VendorSlot& vendor_slot = (VendorSlot&)vendor_container->GetSlot(vendor_pos);
            vendor_slot.CopyOut(vendor_item);
        };

        result.item_count_++;
    }

    player->SendPacket(&msg, sizeof(msg));
}
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM