#include "stdafx.h"

__EMPTY_FILE_TAG();

#if SUN_CODE_BACKUP

#include ".\tempstoreslotcontainer.h"
#include ".\ItemManager.h"
#include ".\Player.h"

CTempStoreSlotContainer::CTempStoreSlotContainer()
{
	Init( MAX_TEMPINVENTORY_SLOT_NUM, SI_TEMPINVENTORY );
}

CTempStoreSlotContainer::~CTempStoreSlotContainer(void)
{
}

RC::eSLOT_INSERT_RESULT CTempStoreSlotContainer::InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot )
{
	RC::eSLOT_INSERT_RESULT rt = SCSlotContainer::InsertSlot( AtPos, rSlot );
	if( rt == RC::RC_INSERT_SLOT_SUCCESS )
	{
		SCItemSlot & rCurSlot = (SCItemSlot &)GetSlot(AtPos);
		m_pPlayer->GetItemManager()->AddSerialCode( eSERIAL_TEMPINVENTORY, GetSlotIdx(), rCurSlot.GetPos(), rCurSlot.GetWorldSerial() );		
	}
	
	return rt;
}

BOOL CTempStoreSlotContainer::DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut )
{
	SCItemSlot TmpDeleteItemSlot;
	BOOL rt = SCSlotContainer::DeleteSlot( AtPos, &TmpDeleteItemSlot );
	if( rt )
	{
		if( pSlotOut ) 
		{
			((SCItemSlot*)pSlotOut)->Copy(TmpDeleteItemSlot);
		}
		m_pPlayer->GetItemManager()->RemoveSerialCode( GetSlotIdx(), AtPos, TmpDeleteItemSlot.GetWorldSerial() );
	}
	return rt;
}


BOOL CTempStoreSlotContainer::ValidState()
{ 
	ProtectorInventory& rPROTECT = m_pPlayer->GetWarehouseInventoryProtector().INVENTORY();
	if( !rPROTECT.IsUsable() )
		return FALSE;
	else
		return SCItemSlotContainer::ValidState();
}

#endif //SUN_CODE_BACKUP