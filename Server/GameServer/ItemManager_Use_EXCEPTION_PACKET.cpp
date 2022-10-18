#include "stdafx.h"
#include "./ItemManager.h"

#include "InventorySlotContainer.h"

//==================================================================================================

// �Ҹ� ��� �������� ���(��� ��ũ)- Ȥ�� �����ϼ���
// DB�� ��弭�������� ������� ���� �� 
// ������� ���� �Ŀ� ������ �ؾ� �Ѵ�.
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





