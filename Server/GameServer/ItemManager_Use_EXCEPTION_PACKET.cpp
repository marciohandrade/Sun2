#include "stdafx.h"
#include "./ItemManager.h"

#include "InventorySlotContainer.h"

//==================================================================================================

// 소모성 길드 아이템일 경우(길드 마크)- 혹은 딴거일수도
// DB와 길드서버에서의 결과값을 받은 후 
// 결과값을 받은 후에 삭제를 해야 한다.
RC::eITEM_RESULT ItemManager::IsValidItem(SLOTIDX atIndex, POSTYPE atPos, eITEMTYPE Itemtype)
{
	if(!ValidPos( atIndex, atPos, TRUE ))
		return RC::RC_ITEM_INVALIDPOS;

	SCSlotContainer* pAtContainer = GetItemSlotContainer(atIndex);
	SCItemSlot & rAtSlot = (SCItemSlot &)pAtContainer->GetSlot(atPos);

	RC::eITEM_RESULT rcResult = player_->CanUseItem(rAtSlot.GetItemInfo());
	if( rcResult != RC::RC_ITEM_SUCCESS ) 
		return rcResult;

	SLOTCODE ItemCode = rAtSlot.GetItemCode();
	const BASE_ITEMINFO* const pInfo = rAtSlot.GetItemInfo();
	if(pInfo->m_wType != Itemtype) 
		return RC::RC_ITEM_FAILED;

	if(!ValidState()) 
		return RC::RC_ITEM_INVALIDSTATE;

	return RC::RC_ITEM_SUCCESS;
}





