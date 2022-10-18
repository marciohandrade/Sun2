#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"
#include "ChaoState.h"

#include "ImplementUtil.h"

//==================================================================================================
namespace nsSlot {
;
//--------------------------------------------------------------------------------------------------
// @history
//  09/06/25|~~~
//  09/07/16|shogen|enchant facility uniformity|<- �����ϱ� �ϴ� �ɱ�?
//  09/10/22|namka199|rank facility improvement|(WARNING) ��þƮ �Ͽ�ȭ �۾��� �Ϻ� ������ ��.
//  10/01/28|waverix|code arrangement
//                  |delete macros  '__NA_676_PK_SYSTEM_IMPROVEMENT'
//                  |               '__NA_977_PREVENTION_PARTY_RETURN_ABUSE'
//                  |               '__NA_1339_ENCHANT_RENEWAL_EX'
//                  |               '__NA_1168_ADD_CONTENTS_ADD_LOG'
//                  |               '__BUG_35162_COMPOSE_BUG_BY_FULL_IVEN'
//                  |               '__CN_1194__ELITE_1_5_ITEM'
//                  |               '__MOD_CRYSTALIZE_MAX_CREATE_ITEM_NUM'
//  10/02/23|hukim|remove warnings level 4... etc...
//                |(waverix: i think this changes have fragmentary knowledge)
//  10/02/24|waverix|rollback to revision 46727|
//                  |������, �ٽ� �� �� ����� �غ��ñ���. ���� ��� �����ϴ� ���� ���� ������,
//                  warning level 4��� �༮�� ������ ���� ������ ������ ������ �ɷ� �����˴ϴ�.
//  11/06/11|waverix|show the below description about the changes
//--------------------------------------------------------------------------------------------------

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionSocket) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionSocket::FillSocket(
    const POSTYPE zard_position, const POSTYPE target_position)
{
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_FILLSOCKET)) 
    {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    if (root_->player_->GetChaoState().IsChaoState()) 
    {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    if (root_->ValidState() == false) 
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (root_->ValidPos(SI_INVENTORY, zard_position) == false) 
    {
        return RC::RC_ITEM_INVALIDPOS;
    }
    if (root_->ValidPos(SI_INVENTORY, target_position) == false) 
    {
        return RC::RC_ITEM_INVALIDPOS;
    }

    SCSlotContainer* const inventory = root_->GetItemSlotContainer(SI_INVENTORY);
    const SCItemSlot& zard_slot = static_cast<const SCItemSlot&>(inventory->GetSlot(zard_position));
    SCItemSlot& target_slot = static_cast<SCItemSlot&>(inventory->GetSlot(target_position));

    const BASE_ITEMINFO* const zard_item = zard_slot.GetItemInfo();
    const BASE_ITEMINFO* const target_item = target_slot.GetItemInfo();

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    if (zard_item->m_wType != eITEMTYPE_SOCKET_ZARD) 
    {
#else
    if (zard_item->m_wType != eITEMTYPE_SOCKET) 
    {
#endif // _NA_000251_20100727_SOCKET_SYSTEM
        return RC::RC_ITEM_INVALID_CONDITION;
    }
    if (target_slot.CanUseMixture(MIXTURE_SOCKET_HOLE_FILL) == false) 
    {
        return RC::RC_ITEM_NOT_MIXTURE;
    }
    if ((target_item->IsWeapon() || target_item->IsArmor()) == false) 
    {
        return RC::RC_ITEM_INVALID_CONDITION;
    }

    eSOCKET found_empty_socket = SOCKET_1;
    if (target_slot.ExistEmptySocket(found_empty_socket) == false) 
    {
        return RC::RC_ITEM_FULLSOCKET;
    }
    
    SocketOptionParser* const socket_option_parser = SocketOptionParser::Instance();
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    // ��� �ɼ��� �˻��Ѵ�.
    const SOCKETINDEX zard_option_index = zard_slot.GetExtendData();
    const sSOCKET_OPTION* const socket_option =
        socket_option_parser->FindSocketOption(zard_option_index);
    if (socket_option == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|�����Ϸ��� �ɼ� ������ ã�� �� �����ϴ�.|�ɼ� = %u, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               zard_option_index, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_INVALID_CONDITION;
    }

    // ��� ����� �˻��Ѵ�.
    if (target_slot.IsEnableFillSocketByGrade(socket_option->zard_grade) == false)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|�ش� ��� ����� �� �̻� ������ �� �����ϴ�.|����� = %u, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               socket_option->zard_grade, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_SOCKET_FAILED_TO_FILL_BY_GRADE;
    }
    target_slot.SetSocketOption(found_empty_socket, zard_option_index);

    GAMELOG->LogFillSocket(
        root_->player_, 
        zard_slot, 
        target_slot, 
        static_cast<BYTE>(socket_option->zard_grade), 
        socket_option->item_option_kind, 
        socket_option->option_value);
    root_->Lose(inventory, zard_position, 1);
#else // _NA_000251_20100727_SOCKET_SYSTEM
    const sSOCKETITEM_OPTION* zard_option = 
        socket_option_parser->GetSocketItemOption(zard_slot.GetItemCode());
    if (zard_option == NULL) 
    {
        return RC::RC_ITEM_INVALID_CONDITION;
    }
    const sSOCKET_OPTION* socket_option = 
        socket_option_parser->GetSocketOption(zard_option->m_AttrIndex);
    if (socket_option == NULL) 
    {
        return RC::RC_ITEM_INVALID_CONDITION;
    }
    target_slot.SetSocketAttr(found_empty_socket, zard_option->m_SocketLevel, socket_option);

    GAMELOG->LogFillSocket(
        root_->player_, 
        zard_slot, 
        target_slot,
        zard_option->m_SocketLevel,
        socket_option->m_AttrIndex,
        socket_option->m_iValue[zard_option->m_SocketLevel]);
    root_->Lose(inventory, zard_position, 1);
#endif // _NA_000251_20100727_SOCKET_SYSTEM

    return RC::RC_ITEM_SUCCESS;
}

//--------------------------------------------------------------------------------------------------
// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
// ������ ���� ItemCompositeParser���� ã��
#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
RC::eITEM_RESULT
nsSlot::ItemFunctionSocket::ExtractSocket(POSTYPE target_pos)
{
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_EXTRACTSOCKET)) {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    if (root_->player_->GetChaoState().IsChaoState()) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (root_->ValidPos(SI_INVENTORY, target_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    }

    SCSlotContainer* inventory = root_->GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& target_slot = (SCItemSlot &)inventory->GetSlot(target_pos);
    const BASE_ITEMINFO* target_item_info = target_slot.GetItemInfo();
    if (target_item_info->IsChargeItem()) {
        return RC::RC_ITEM_CANNOT_DO_THIS_FOR_CHARGEITEM;
    }
    // ���Ͽɼ��� ������ �ɼ��� �ִ°�?
    {
        BOOLEAN exist_erasable_option = false;
        // accept minus op, need rechecking invalid status
        for (int i = static_cast<int>(target_slot.GetSocketNum()) - 1; i >= 0; --i)
        {
            eATTR_TYPE attr_type = target_slot.GetSocketAttr(static_cast<eSOCKET>(i));
            if (attr_type != eATTR_TYPE_INVALID) {
                exist_erasable_option = true;
                break;
            }
        }
        if (exist_erasable_option == false) {
            return RC::RC_ITEM_INVALID_CONDITION;
        }
    };

    // ��ᰡ �ִ����� ���� üũ
    // ���� üũ, �� üũ
    const sITEMCOMPOSITE* composite = NULL;
    {
        ItemCompositeParser* composite_parser = ItemCompositeParser::Instance();
        composite = composite_parser->GetCompositeInfo(eITEMCOMPOSITE_EXTRACT_SOCKET);
        ASSERT(composite != NULL);
    };

    if (root_->player_->GetMoney() < composite->m_Money) {
        return RC::RC_ITEM_HAVENOTMONEY;
    }

    const sITEMCOMPOSITEMATERIAL* materials = composite->m_sCompositeMaterials;
    for (POSTYPE i = 0; i < composite->m_byMatCount; ++i)
    {
        const sITEMCOMPOSITEMATERIAL& material = materials[i];
        if (material.m_dwCompositeMaterialCode == 0) {
            continue;
        }

        ns_functor::EmptyItemCheck empty_check(material.m_dwCompositeMaterialCode,
                                               material.m_bySpendMaterialNum);
        if (root_->ForeachSlot(SI_INVENTORY, &empty_check) == false) {
            return RC::RC_ITEM_ENCHANT_HAVENOTMETERIALS;
        }
    }
    // ��� ���ֱ�
    for (POSTYPE i = 0; i < composite->m_byMatCount; ++i)
    {
        const sITEMCOMPOSITEMATERIAL& material = materials[i];
        if (material.m_dwCompositeMaterialCode == 0) {
            continue;
        }

        ns_functor::DeleteItem delete_operator(material.m_dwCompositeMaterialCode,
                                               material.m_bySpendMaterialNum);
        BOOL rt = root_->ForeachSlot(SI_INVENTORY, &delete_operator);
        ASSERT(rt);
    }

    // ������
    BOOL ret = root_->player_->MinusMoney(composite->m_Money);

    // �α� �����
    //GAMELOG->LogItemExtrackSocket(root_->player_, target_slot, pCInfo->m_byMatCount, materials);
    GAMELOG->LogMoney(MONEY_SOCKETOPT_EXTRACT, root_->player_, composite->m_Money);

    // ���� Ȯ��
    {
        int seed = random(0, 99);
        if (seed < 75) // ���� Ȯ��
        {
            // �ֱ� �߻��� �ɼ� ����
            for (int i = static_cast<int>(target_slot.GetSocketNum()) - 1; i >= 0; --i)
            {
                eATTR_TYPE attr_type = target_slot.GetSocketAttr(static_cast<eSOCKET>(i));
                if (attr_type != eATTR_TYPE_INVALID)
                {
                    //target_slot.SetSocketAttr((eSOCKET)i, SOCKETLV_LOW, 0, 0);
                    target_slot.SetSocketAttr(static_cast<eSOCKET>(i), SOCKETLV_LOW, NULL);
                    return RC::RC_ITEM_EXTRACTSOCKET_SUCCESS;
                }
            }
        }
    };

    return RC::RC_ITEM_EXTRACTSOCKET_FAILED;
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM

//--------------------------------------------------------------------------------------------------
// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
// ���� ���� �ձ�
// ��ȹ���� ��������� �־ �켱 ����!
RC::eITEM_RESULT
nsSlot::ItemFunctionSocket::AddSocket(POSTYPE target_pos)
{
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_ADDSOCKET)) {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    if (root_->player_->GetChaoState().IsChaoState()) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    return RC::RC_ITEM_SUCCESS; 
}

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
// �Ҹ��� ���� ����Ͽ� ���Ͽ� ���� ��带 �����Ѵ�.
RC::eITEM_RESULT nsSlot::ItemFunctionSocket::ExtractSocket(
    const POSTYPE extract_item_position,
    const POSTYPE zard_owner_position,
    const BYTE socket_number)
{
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_EXTRACTSOCKET))
    {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    if (root_->player_->GetChaoState().IsChaoState())
    {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    if (!root_->ValidState())
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (!root_->ValidPos(SI_INVENTORY, extract_item_position))
    {
        return RC::RC_ITEM_NOT_INVENTORY;
    }
    if (!root_->ValidPos(SI_INVENTORY, zard_owner_position))
    {
        return RC::RC_ITEM_NOT_INVENTORY;
    }
        
    SCSlotContainer* const inventory = root_->GetItemSlotContainer(SI_INVENTORY);
    
    const SCItemSlot& extract_item_slot = 
        static_cast<const SCItemSlot&>(inventory->GetSlot(extract_item_position));
    const BASE_ITEMINFO* const extract_item = extract_item_slot.GetItemInfo();
    const RC::eITEM_RESULT can_use_result = root_->player_->CanUseItem(extract_item);
    if (can_use_result != RC::RC_ITEM_SUCCESS)
    {
        return can_use_result;
    }
    
    SCItemSlot& zard_owner_slot = 
        static_cast<SCItemSlot&>(inventory->GetSlot(zard_owner_position));
    if (zard_owner_slot.CanUseMixture(MIXTURE_SOCKET_EXTRACT) == false) 
    {
        return RC::RC_ITEM_NOT_MIXTURE;
    }

    // ��� ���� ���������� �˻��Ѵ�.
    if ((extract_item->m_wType != eITEMTYPE_SOCKETEXTRACT) || (extract_item_slot.GetNum() == 0))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|��� ���� �������� �ƴմϴ�.|������Ÿ�� = %u, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               extract_item->m_wType, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_SOKET_NOT_RUNE_ITEM;
    }

    // ��带 ������ ���� ��ȣ�� �˻��Ѵ�.
    if (socket_number >= zard_owner_slot.GetSocketNum())
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|��ȿ�� ���Ϲ�ȣ�� �ƴմϴ�.|���Ϲ�ȣ = %u, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               socket_number, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_INVALID_SOCKET_NUM;
    }
    const eSOCKET extract_socket = static_cast<eSOCKET>(socket_number);
    
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    // ������ ��尡 �����ϴ��� �˻��Ѵ�.
    const sSOCKET_OPTION* const socket_option = zard_owner_slot.GetSocketOption(extract_socket);
    if (socket_option == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|������ ��尡 ���Ͽ� �������� �ʽ��ϴ�.|���Ϲ�ȣ = %u, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               extract_socket, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_SOCKET_HAS_NOT_OPTION;
    }

    // ������ ��尡 �ı����� �ʾ����� ������ ��带 ��´�.
    enum { kMaxDestroyRatio = 1000 };
    const int destroy_ratio = random(1, kMaxDestroyRatio);
    if (extract_item->zard_success_per >= destroy_ratio) 
    {
        typedef util::internal::ItemCompositionOrDecompositionReward ZardRewards;

        ZardRewards zard_rewards;
        if (zard_rewards.Init(root_->player_, ZardRewards::kItemCodeReward) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������ ��� ������ �ʱ�ȭ �ϴµ� ���� �߽��ϴ�.|ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   root_->player_->GetCharName());
            return RC::RC_ITEM_INVALIDSTATE;
        }

        ZardRewards::ItemReward zard_reward;
        {
            zard_reward.item_code = socket_option->item_code;
            zard_reward.number_of_item = 1;
            zard_reward.zard_option = socket_option->index;
        }
        if (zard_rewards.AddItemReward(zard_reward) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������ ��� ������ �߰��ϴµ� ���� �߽��ϴ�.|ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   root_->player_->GetCharName());
            RC::RC_ITEM_INVALIDSTATE;
        }

        INVENTORY_TOTAL_INFO modified_inven;
        {
            modified_inven.m_InvenCount = 0;
            modified_inven.m_TmpInvenCount = 0;
        }
        const RC::eITEM_RESULT obtain_result = 
            zard_rewards.ObtainRewardToInventory(ZardRewards::kDoNotUseLog, modified_inven);
        if (obtain_result != RC::RC_ITEM_SUCCESS)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������ ��带 ��µ� ���� �߽��ϴ�.|ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   root_->player_->GetCharName());
            return obtain_result;
        }

        MSG_CG_ITEM_OBTAIN_ITEM_CMD zard_obtain_cmd;
        zard_obtain_cmd.m_tagInventory = modified_inven;
        root_->player_->SendPacket(&zard_obtain_cmd, zard_obtain_cmd.GetSize());
    }
#else
    // ������ ��尡 �����ϴ��� �˻��Ѵ�.
    if (zard_owner_slot.GetSocketAttr(extract_socket) == eATTR_TYPE_INVALID)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|������ ��尡 ���Ͽ� �������� �ʽ��ϴ�.|���Ϲ�ȣ = %u, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               extract_socket, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_SOCKET_HAS_NOT_OPTION;
    }

    // ������ ��� ����� ��ġ�ϴ��� �˻��Ѵ�.
    if (extract_item->m_byWasteType != zard_owner_slot.GetSocketAttrLevel(extract_socket))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|������ ��� ����� ��ġ���� �ʽ��ϴ�.")
               _T("|��������۵�� = %u, ���ϵ�� = &u, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               extract_item->m_byWasteType, 
               zard_owner_slot.GetSocketAttrLevel(extract_socket), 
               root_->player_->GetCharName());
        return RC::RC_ITEM_SOCKET_DIFFERENT_LEVEL;
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM    
        
    // �α׸� ����Ѵ�.
    GAMELOG->LogItemExtrackSocket(
        root_->player_, zard_owner_slot, extract_item->m_wType, extract_item_slot.GetNum());
    GAMELOG->LogItem(ITEM_USE, root_->player_, &extract_item_slot);

    // ��Ÿ���� �����Ѵ�.
    const WORD cool_time = extract_item->m_wCoolTimeType;
    root_->Lose(inventory, extract_item_position, 1);
    root_->player_->ResetUseItem(cool_time);

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    zard_owner_slot.SetSocketOption(extract_socket, SocketOptionParser::kEmptySocketOptionIndex);
#else
    zard_owner_slot.SetSocketAttr(extract_socket, SOCKETLV_LOW, NULL);
#endif // _NA_000251_20100727_SOCKET_SYSTEM

    return RC::RC_ITEM_EXTRACTSOCKET_SUCCESS;
}

