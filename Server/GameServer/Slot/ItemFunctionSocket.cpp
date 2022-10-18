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
//  09/07/16|shogen|enchant facility uniformity|<- 적용하긴 하는 걸까?
//  09/10/22|namka199|rank facility improvement|(WARNING) 인첸트 일원화 작업이 일부 누락된 듯.
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
//                  |현웅씨, 다시 한 번 재검토 해보시구랴. 물론 경고를 무시하는 것은 나쁜 거지만,
//                  warning level 4라는 녀석의 함정에 빠져 무리한 변경을 수행한 걸로 생각됩니다.
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
    // 쟈드 옵션을 검사한다.
    const SOCKETINDEX zard_option_index = zard_slot.GetExtendData();
    const sSOCKET_OPTION* const socket_option =
        socket_option_parser->FindSocketOption(zard_option_index);
    if (socket_option == NULL) 
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|삽입하려는 옵션 정보를 찾을 수 없습니다.|옵션 = %u, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               zard_option_index, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_INVALID_CONDITION;
    }

    // 쟈드 등급을 검사한다.
    if (target_slot.IsEnableFillSocketByGrade(socket_option->zard_grade) == false)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|해당 쟈드 등급은 더 이상 삽입할 수 없습니다.|쟈드등급 = %u, 캐릭터이름 = %s|"), 
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
// 보석과 재료는 ItemCompositeParser에서 찾기
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
    // 소켓옵션을 제거할 옵션이 있는가?
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

    // 재료가 있는지에 대한 체크
    // 개수 체크, 돈 체크
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
    // 재료 없애기
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

    // 돈차감
    BOOL ret = root_->player_->MinusMoney(composite->m_Money);

    // 로그 남기기
    //GAMELOG->LogItemExtrackSocket(root_->player_, target_slot, pCInfo->m_byMatCount, materials);
    GAMELOG->LogMoney(MONEY_SOCKETOPT_EXTRACT, root_->player_, composite->m_Money);

    // 성공 확률
    {
        int seed = random(0, 99);
        if (seed < 75) // 성공 확률
        {
            // 최근 발생한 옵션 제거
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
// 소켓 구멍 뚫기
// 기획적인 고려사항이 있어서 우선 보류!
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
// 소멸의 룬을 사용하여 소켓에 박힌 쟈드를 제거한다.
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

    // 쟈드 추출 아이템인지 검사한다.
    if ((extract_item->m_wType != eITEMTYPE_SOCKETEXTRACT) || (extract_item_slot.GetNum() == 0))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|쟈드 추출 아이템이 아닙니다.|아이템타입 = %u, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               extract_item->m_wType, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_SOKET_NOT_RUNE_ITEM;
    }

    // 쟈드를 추출할 소켓 번호를 검사한다.
    if (socket_number >= zard_owner_slot.GetSocketNum())
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|유효한 소켓번호가 아닙니다.|소켓번호 = %u, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               socket_number, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_INVALID_SOCKET_NUM;
    }
    const eSOCKET extract_socket = static_cast<eSOCKET>(socket_number);
    
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    // 추출할 쟈드가 존재하는지 검사한다.
    const sSOCKET_OPTION* const socket_option = zard_owner_slot.GetSocketOption(extract_socket);
    if (socket_option == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|추출할 쟈드가 소켓에 존재하지 않습니다.|소켓번호 = %u, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               extract_socket, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_SOCKET_HAS_NOT_OPTION;
    }

    // 추출한 쟈드가 파괴되지 않았으면 추출한 쟈드를 얻는다.
    enum { kMaxDestroyRatio = 1000 };
    const int destroy_ratio = random(1, kMaxDestroyRatio);
    if (extract_item->zard_success_per >= destroy_ratio) 
    {
        typedef util::internal::ItemCompositionOrDecompositionReward ZardRewards;

        ZardRewards zard_rewards;
        if (zard_rewards.Init(root_->player_, ZardRewards::kItemCodeReward) == false)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|추출한 쟈드 보상을 초기화 하는데 실패 했습니다.|캐릭터이름 = %s|"), 
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
                   _T("|%s|추출한 쟈드 보상을 추가하는데 실패 했습니다.|캐릭터이름 = %s|"), 
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
                   _T("|%s|추출한 쟈드를 얻는데 실패 했습니다.|캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   root_->player_->GetCharName());
            return obtain_result;
        }

        MSG_CG_ITEM_OBTAIN_ITEM_CMD zard_obtain_cmd;
        zard_obtain_cmd.m_tagInventory = modified_inven;
        root_->player_->SendPacket(&zard_obtain_cmd, zard_obtain_cmd.GetSize());
    }
#else
    // 추출할 쟈드가 존재하는지 검사한다.
    if (zard_owner_slot.GetSocketAttr(extract_socket) == eATTR_TYPE_INVALID)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|추출할 쟈드가 소켓에 존재하지 않습니다.|소켓번호 = %u, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               extract_socket, 
               root_->player_->GetCharName());
        return RC::RC_ITEM_SOCKET_HAS_NOT_OPTION;
    }

    // 추출할 쟈드 등급이 일치하는지 검사한다.
    if (extract_item->m_byWasteType != zard_owner_slot.GetSocketAttrLevel(extract_socket))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|추출할 쟈드 등급이 일치하지 않습니다.")
               _T("|추출아이템등급 = %u, 소켓등급 = &u, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               extract_item->m_byWasteType, 
               zard_owner_slot.GetSocketAttrLevel(extract_socket), 
               root_->player_->GetCharName());
        return RC::RC_ITEM_SOCKET_DIFFERENT_LEVEL;
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM    
        
    // 로그를 기록한다.
    GAMELOG->LogItemExtrackSocket(
        root_->player_, zard_owner_slot, extract_item->m_wType, extract_item_slot.GetNum());
    GAMELOG->LogItem(ITEM_USE, root_->player_, &extract_item_slot);

    // 쿨타임을 설정한다.
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

