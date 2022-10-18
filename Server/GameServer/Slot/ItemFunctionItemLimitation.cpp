#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionItemLimitation) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
// true:�ٲ� ������ �ڽ� ����, false:�׳� ����
RC::eITEM_RESULT nsSlot::ItemFunctionItemLimitation::ChangeItemStat(
    CODETYPE ItemCode, BOOLEAN bClear)
{
    if (!root_->item_switch_handle_)
        return RC::RC_ITEM_FAILED;

    CInventorySlotContainer *pContainer = static_cast<CInventorySlotContainer*>(\
        root_->item_switch_handle_->slot_container_);
    if (pContainer == NULL)
        return RC::RC_ITEM_FAILED;

    RC::eITEM_RESULT result = RC::RC_ITEM_SUCCESS;
    if (bClear)
    {
        do
        {
            // (NOTE) need precheck ItemCode
            if (!root_->item_switch_handle_->CreateSameItem((SLOTCODE)ItemCode))
            {
                SUNLOG(eCRITICAL_LOG,
                    "[AEORAGIE] ChangeItemStat() CreateSameItem Fail! ItemCode[%d]",
                    ItemCode);
                result =  RC::RC_ITEM_FAILED;
                break;
            }

            SCItemSlot *target = root_->item_switch_handle_->target_item_slot_;
            SCItemSlot temp; temp.Copy(*target);
            POSTYPE pos = target->GetPos();
            if ((result = root_->Delete(pos)) !=  RC::RC_ITEM_SUCCESS) 
            {   // �ٲ� ������ ����
                SUNLOG(eCRITICAL_LOG,
                    "[AEORAGIE] ChangeItemStat() DeleteSlot Fail! ItemCode[%d]",
                    target->GetItemCode());
                break;
            }

            SCItemSlot *created = &root_->item_switch_handle_->making_item_slot_;
            if (pContainer->InsertSlot(pos,*created) != RC::RC_INSERT_SLOT_SUCCESS) 
            {   
                SUNLOG(eCRITICAL_LOG,
                    "ItemManager::ChangeItemStat() Obtain Fail! ItemCode[%d]",
                    created->GetItemCode());
                break;
            }

            SCItemSlot *used = root_->item_switch_handle_->use_item_slot_;
            SCItemSlot temp2; temp2.Copy(*used);
            POSTYPE pos2 = used->GetPos();

            // �츣�޽����Ǽ��� ������ ��� �α�
            GAMELOG->LogItem(ITEM_USE, root_->player_, used);

            if ((result = root_->DeleteWastedItem(SI_INVENTORY, pos2)) != RC::RC_ITEM_SUCCESS)
            {    // �츣�޽� ����
                SUNLOG(eCRITICAL_LOG,
                    "[AEORAGIE] ChangeItemStat() DeleteSlot Fail! ItemCode[%d]",
                    used->GetItemCode());
                break;
            }

        } while (false);
    }

    SAFE_DELETE(root_->item_switch_handle_);
    root_->player_->SetBehave(PLAYER_BEHAVE_IDLE_STATE);

    return result;
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
// ������ ����
RC::eITEM_RESULT nsSlot::ItemFunctionItemLimitation::ChangeItemStat_SelectItem(
    POSTYPE ConIndex, POSTYPE SlotPos, POSTYPE TargetSlotPos,
    MSG_CG_ITEM_STAT_CHANGE_USE_ACK* pPacket)
{
    // ConIndex�� ������ �κ��丮���� �Ѵ�. ���� Ŭ��κ��� �޴� ������ �ƴѵ� �۾��ڰ� �Ǽ��ϰ� ������ -_-;;
    ConIndex = SI_INVENTORY;

    if (!root_->ValidPos(ConIndex, SlotPos, true) ||
        !root_->ValidPos(ConIndex, TargetSlotPos, true))
    {
        return RC::RC_ITEM_INVALIDPOS;
    };
    SCItemSlotContainer* pAtContainer = root_->GetItemSlotContainer(ConIndex);
    SCItemSlot& rUsedSlot = (SCItemSlot &)pAtContainer->GetSlot(SlotPos);
    SCItemSlot& rTargetSlot = (SCItemSlot &)pAtContainer->GetSlot(TargetSlotPos);

    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rUsedSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;


    const BASE_ITEMINFO* const pItemInfo = rTargetSlot.GetItemInfo();
    // NOTE: f110611.1L, ...-.-;;;
    if ((rUsedSlot.GetItemInfo()->m_wType != eITEMTYPE_STAT_CHANGE) ||  // �츣�޸��� ����
        !pItemInfo || // BASE_ITEMINFO�� ����
        !(pItemInfo->IsArmor() || pItemInfo->IsWeapon()) || // ���͹���
        (pItemInfo->IsElite() || rTargetSlot.IsFateItem()) ||
         pItemInfo->IsUnique() || rTargetSlot.IsLimited()) 
        // ����Ƽ�� ����Ʈ�� ��Ű������ ����ũ�� �ƴϾ���� //(����Ʈ �߰�: youngmoon)
    {
        return RC::RC_ITEM_NOT_MIXTURE;
    }

    // CHANGES:f100915.3L, prevent changes of a combined item using 'The Bible of Hermesism'.
    // added by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    if (bool is_combined_item = (rTargetSlot.GetEtheriaHistory().value != eEtheriaHistory::None)) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }
    else if (!FlowControl::FCAssert(rTargetSlot.GetItemCode() == rTargetSlot.GetImageCode())) {
        // TODO: test log. remove this check routine after the 1005 version is stabled.
        return RC::RC_ITEM_NOT_MIXTURE;
    }

    if (root_->item_switch_handle_ == NULL)
    {
        root_->player_->SetBehave(PLAYER_BEHAVE_DIALOG_OPEN_STATE);
        root_->item_switch_handle_ = new ItemStatSwitch(&rTargetSlot, &rUsedSlot, pAtContainer);
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "[AEORAGIE] ItemManager::ChangeItemStat_SelectItem() Already Doing");
        return RC::RC_ITEM_WINDOW_ALREADY_OENDEDSTATE;
    }

    if (root_->item_switch_handle_ && pPacket)
    {
        rcResult = root_->item_switch_handle_->GetItemCodeList(pPacket) ?
            RC::RC_ITEM_SUCCESS : RC::RC_ITEM_FAILED;
    };

    if (pPacket->m_ItemCount <= 1)
    {
        SAFE_DELETE(root_->item_switch_handle_);
        root_->player_->SetBehave(PLAYER_BEHAVE_IDLE_STATE);
        return RC::RC_ITEM_NOT_MIXTURE;
    }

    return rcResult;
}
