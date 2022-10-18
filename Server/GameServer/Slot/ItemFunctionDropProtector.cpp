#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "PKScriptParser.h"
//
#include "InventorySlotContainer.h"
#include "ChaoState.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionDropProtector) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
bool nsSlot::ItemFunctionDropProtector::ProtectDrop(Player *pKiller)
{
    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    PKOptionInfo::ePoessessionDropOption DropOption =
        PKScriptParser::Instance()->GetPKOptionInfo().m_ePossessionDropOption;

    SCSlotContainer * pEquipmentContainer = root_->GetItemSlotContainer(SI_EQUIPMENT);
    BOOL HadItemPossibleDrop = false;
    if (0 == pEquipmentContainer->GetSlotNum())
    {
        for (POSTYPE SlotPos = 0; SlotPos < pContainer->GetMaxSlotNum(); ++SlotPos)
        {
            if (pContainer->IsEmpty(SlotPos))   continue;

            // ������ üũ
            if (pContainer->IsLocked(SlotPos))
                continue;

            // ��� �Ұ����� �������ΰ�? (����Ʈ ������)
            SCItemSlot & rSlot = (SCItemSlot &)pContainer->GetSlot(SlotPos);
            if (rSlot.CanTradeSell(eITEM_TRADESELL_DROP) == false) {
                // _NA_006052_20120904_CANNOT_DROP_CAN_PKDROP
                if (rSlot.CanTradeSell(eITEM_TRADESEEL_DROP_CAN_PKDROP) == true) {
                    continue;
                }
            }
            if (rSlot.IsPossessionItem() && rSlot.GetIdentifyPossession())
            {
                if (DropOption == PKOptionInfo::ENUM_POSSESSION_DROP_NO)
                {
                    if (!rSlot.IsDestroyItem())
                        continue;
                }
            }
            //��� ������ �������� ����
            HadItemPossibleDrop = true;
            break;
        }

        //��� ������ �������� ������ ������ �� üũ ���ϰ� ����
        if (HadItemPossibleDrop == false)
            return true;
    }

    if (root_->player_->GetChaoState().GetChaoState() <= pKiller->GetChaoState().GetChaoState())
    {
        // ������ ��2�� �ִ��� ã�´�.
        for (POSTYPE i = 0 ; i < pContainer->GetMaxSlotNum() ; ++i)
        {
            if (pContainer->IsEmpty(i)) continue;

            SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(i); 

            if (rItemSlot.GetItemInfo()->m_wType == eITEMTYPE_HOLDDROP2) 
            {
                // �������� �����Ѵ�.
                RC::eITEM_RESULT rcResult = root_->Delete(\
                    SI_INVENTORY, i, 1, ITEM_DELETE_PROTECT_PKDROP);
                if (rcResult == RC::RC_ITEM_SUCCESS)
                {
                    MSG_CG_ITEM_DELETE_CMD msg;
                    msg.m_byReason = RC::RC_ITEM_DELETE_PROTECT_DROP;
                    msg.m_atIndex = SI_INVENTORY;
                    msg.m_atPos = i;
                    root_->player_->SendPacket(&msg, sizeof(msg));

                    return true;
                }
            }
        }
    }

    // ������� �������� �ִ��� ã�´�.
    for (POSTYPE i = 0 ; i < pContainer->GetMaxSlotNum() ; ++i)
    {
        if (pContainer->IsEmpty(i)) continue;

        SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(i); 

        if (rItemSlot.GetItemInfo()->m_wType == eITEMTYPE_HOLDDROP) 
        {
            // �������� �����Ѵ�.
            RC::eITEM_RESULT rcResult = root_->Delete(\
                SI_INVENTORY, i, 1, ITEM_DELETE_PROTECT_PKDROP);
            if (rcResult == RC::RC_ITEM_SUCCESS)
            {
                MSG_CG_ITEM_DELETE_CMD msg;
                msg.m_byReason = RC::RC_ITEM_DELETE_PROTECT_DROP;
                msg.m_atIndex = SI_INVENTORY;
                msg.m_atPos = i;
                root_->player_->SendPacket(&msg, sizeof(msg));

                return true;
            }
        }
    }

    return false;
}
