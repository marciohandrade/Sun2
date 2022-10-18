#include "stdafx.h"

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
#include "PlayerSpaInfo.h"

#include "ItemInfoParser.h"
#include "InventorySlotContainer.h"
#include "ItemManager.h"

BOOL PlayerSpaInfo::_SetSpaValueInfo( CODETYPE item_code)
{
    _SetSpaTicketCode(item_code);

    return TRUE;
}

RC::eITEM_RESULT PlayerSpaInfo::SetSpaInfo( SLOTIDX slotcontainer_type, POSTYPE item_pos )
{
    ItemManager* item_manager = owner_->GetItemManager();
    //�κ��丮
    SCItemSlotContainer* inventory = item_manager->GetItemSlotContainer(slotcontainer_type);
    if (inventory == NULL) {
        return RC::RC_ITEM_FAILED;
    }

    //��ǥ�� �����۰�������
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(inventory->GetSlot(item_pos));
    RC::eITEM_RESULT use_result = owner_->CanUseItem(item_slot.GetItemInfo());
    if (use_result != RC::RC_ITEM_SUCCESS) {
        return use_result;
    }

    const BASE_ITEMINFO* const item_info = item_slot.GetItemInfo();
    //��õƼ������ �˻�
    const eITEMTYPE item_type = eITEMTYPE(item_info->m_wType);
    if (item_type != eITEMTYPE_SPA_TICKET) {
        return RC::RC_ITEM_SPA_COSTUME_INVALID_ITEM;
    }
    //��õƼ�� �ڵ� ����
    BOOL set_result = _SetSpaValueInfo(item_slot.GetItemCode());
    if (set_result == TRUE) {
        return RC::RC_ITEM_SUCCESS;
    }
}

VOID PlayerSpaInfo::ResetSpaInfo()
{
    _SetSpaTicketCode(0);
}

VOID PlayerSpaInfo::CheckAndResetSpaTicket(POSTYPE item_pos)
{
    if (GetSpaTicketCode() == 0) {
        return ;
    }

    ItemManager* item_manager = owner_->GetItemManager();
    //�κ��丮
    SCItemSlotContainer* inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    if (inventory == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Cannot Find Inventory!");
        
        ResetSpaInfo();
        return;
    }

    //��ǥ�� �´� ���� ��������
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(inventory->GetSlot(item_pos));
    const BASE_ITEMINFO* const item_info = item_slot.GetItemInfo();
    // ��õƼ������ Ÿ�԰˻�
    const eITEMTYPE item_type = eITEMTYPE(item_info->m_wType);
    if (item_type != eITEMTYPE_SPA_TICKET) {
        return;
    }

    //Ƽ���ڵ�˻�
    if (GetSpaTicketCode() != item_info->m_Code) {
        return;
    }

    //��õ��������
    ResetSpaInfo();
}

#endif //_NA_008069_20150210_ADD_SPA_SYSTEM