////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   SocketSystemImplement.cpp
/// @author hukim
/// @brief  [2010/10/06 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\SocketSystemImplement.h" // header
#include "SocketSystem/SocketItemRatioParser.h"
#include "SocketSystem/ZardComposeInfoParser.h"
#include "SCItemSlotContainer.h"
#include "ImplementUtil.h"

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

SocketSystemImplement::SocketSystemImplement()
{
}

SocketSystemImplement::~SocketSystemImplement()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

void SocketSystemImplement::HandleIdentifySocketMessage( 
    const MSG_BASE* const message, 
    const MessageSize message_size) const
{    
    // ��û �޽����� �˻��Ѵ�.
    const MSG_CG_ITEM_IDENTIFY_SOCKET_CMD* identify_socket_cmd(NULL);
    Player* requester(NULL);
    if (!util::internal::CheckRequestMessage(message, message_size, identify_socket_cmd, requester))
    {
        return;
    }
    
    MSG_CG_ITEM_IDENTIFY_SOCKET_NAK::ErrorCode error_code = 0;
    do 
    {
        // ��û���� �⺻���� ���¸� �˻��Ѵ�.
        if (!util::internal::CheckRequesterState(requester, __FUNCTION__, error_code))
        {
            break;
        }
        
        // ������ Ȯ���� ������ ��ġ�� �˻��Ѵ�.
        ItemManager* const item_manager = requester->GetItemManager();
        if (!item_manager->ValidPos(SI_INVENTORY, identify_socket_cmd->unidentified_item_pos_)) 
        {
            error_code = RC::RC_ITEM_INVALIDPOS;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���� ��Ȯ�� ������ ���� ��ġ�� �ǹٸ��� �ʽ��ϴ�.|������ ���� ��ġ = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   identify_socket_cmd->unidentified_item_pos_, 
                   requester->GetCharName());
            break;
        }

        SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
        SCItemSlot& unidentified_item_slot = 
            static_cast<SCItemSlot&>(inventory->GetSlot(identify_socket_cmd->unidentified_item_pos_));
        
        // ���� ��Ȯ�� ���������� �˻��Ѵ�.
        const eSOCKET_IDENTIFY_TYPE identify_state = 
            static_cast<eSOCKET_IDENTIFY_TYPE>(unidentified_item_slot.GetSocketIdentifyType());
        if (identify_state != eSOCKET_UNIDENTIFIED_ITEM)
        {
            error_code = RC::RC_ITEM_NOT_UNIDENTIFIED_SOCKET;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���� ��Ȯ�� �������� �ƴմϴ�.|Ȯ�λ��� = %d, ������ ���� ��ġ = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   identify_state, 
                   identify_socket_cmd->unidentified_item_pos_, 
                   requester->GetCharName());
            break;
        }

        // ���� ���� Ȯ�� ������ ã�� �� �ִ��� �˻��Ѵ�.
        const SocketItemRatioParser* const socket_ratio_parser = SocketItemRatioParser::Instance();
        const BASE_ITEMINFO* const unidentified_item = unidentified_item_slot.GetItemInfo();
        const SocketItemRatioInfo::RatioType::Value ratio_type = 
            socket_ratio_parser->GetRatioType(unidentified_item->m_wType, false);
        const SocketItemRatioInfo* const socket_ratio = 
            socket_ratio_parser->FindSocketRatio(ratio_type, unidentified_item->m_LV);
        if (!socket_ratio)
        {
            error_code = RC::RC_ITEM_NOINFO;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���� ���� Ȯ�� ������ �����ϴ�.|������ Ÿ�� = %d, ������ ���� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   unidentified_item->m_wType, 
                   unidentified_item->m_LV, 
                   requester->GetCharName());
            break;
        }

        // ������� ���� ���� Ȯ���� �����ϱ� ������ ���� Ȯ�� �������� ������� ��츦 ó���Ѵ�.
        if (identify_socket_cmd->use_identify_item_)
        {
            error_code = item_manager->UseItemByType(
                eITEMTYPE_SOCKET_IDENTIFY, 
                SI_INVENTORY, 
                identify_socket_cmd->identify_item_pos_, 
                true, 
                __FUNCTION__);
            if (error_code != RC::RC_ITEM_SUCCESS)
            {
                break;
            }
        }

        // ���� ������ �����Ѵ�.
        {
            const int seed = random(1, SocketItemRatioInfo::kMaxRatio);
            int accumulation_ratio = 0;
            for (BYTE i = SOCKET_MAX; i > SOCKET_1; --i)
            {
                accumulation_ratio += socket_ratio->socket_ratio_[i-1];
                if (seed <= accumulation_ratio)
                {
                    unidentified_item_slot.SetSocketNum(i);
                    break;
                }
            }
            unidentified_item_slot.SetSocketIdentifyType(eSOCKET_IDENTIFIED_ITEM);
            item_manager->SendItemChange(unidentified_item_slot);
        }

        // �α׸� ����Ѵ�.
        {
            TCHAR add_info[MAX_ADDINFO_SIZE];
            ::_sntprintf(
                add_info, _countof(add_info), _T("S:%u"), unidentified_item_slot.GetSocketNum());
            add_info[_countof(add_info) - 1] = _T('\0');
            GAMELOG->LogItem(
                ITEM_SOCKET_ADD, requester, &unidentified_item_slot, 0, 1, NULL, NULL, add_info);
        }
        return;
    } while (false);

    MSG_CG_ITEM_IDENTIFY_SOCKET_NAK identify_socket_nak;
    identify_socket_nak.error_code_ = error_code;
    requester->SendPacket(&identify_socket_nak, identify_socket_nak.GetSize());
}

RC::eITEM_RESULT SocketSystemImplement::ZardCompose( 
    const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& request, 
    Player& OUT requester, 
    util::internal::ItemCompositionOrDecompositionReward& OUT reward)
{
    // ��� �ռ����� �˻��Ѵ�.
    if (!request.is_composition_)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|��� �ռ��� �ƴմϴ�.|ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
    }

    // �ռ��� ��� ������ �˻��Ѵ�.
    if (request.material_list_size_ != ZardComposeInfoParser::kMaterialSize)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|�ռ��� ��� ������ �ǹٸ��� �ʽ��ϴ�.|��尳�� = %d, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               request.material_list_size_, 
               requester.GetCharName());
        return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
    }

    // �ռ��� ��� ��ġ�� ��´�.
    POSTYPE zard_position_list[ZardComposeInfoParser::kMaterialSize];
    {
        BOOST_STATIC_ASSERT(ZardComposeInfoParser::kMaterialSize <= MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kMaxMaterialSize);
        
        for (MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialListSize material_index = 0; 
            material_index < ZardComposeInfoParser::kMaterialSize; 
            ++material_index)
        {
            const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& zard_material = 
                request.material_list_[material_index];
            if (zard_material.selected_number_ != 1 || zard_material.matched_index_ != 0)
            {
                SUNLOG(eCRITICAL_LOG, 
                       _T("|%s|�ռ��� ��� ������ �ǹٸ��� �ʽ��ϴ�.|SelectedNumber = %d, MatchedIndex = %d, ĳ�����̸� = %s|"), 
                       __FUNCTION__, 
                       zard_material.selected_number_, 
                       zard_material.matched_index_, 
                       requester.GetCharName());
                return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
            }
            zard_position_list[material_index] = zard_material.inventory_position_;
        }
    }
    
    ItemManager* const item_manager = requester.GetItemManager();
    SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    const POSTYPE additive_position = static_cast<POSTYPE>(request.reserved_code_);

    const ZardComposeInfoParser* const zard_compose_parser = ZardComposeInfoParser::Instance();
    const ZardComposeInfo::ZardRatio success_ratio = zard_compose_parser->GetZardCompositionSuccessRatio(
        *inventory, zard_position_list, additive_position, 0);
    if (success_ratio == 0)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|��� ���� Ȯ���� ��µ� �����߽��ϴ�.|ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
    }

    // ÷������ ������� ��츦 ó���Ѵ�.
    if (additive_position != INVALID_POSTYPE_VALUE)
    {
        const RC::eITEM_RESULT additive_using_result = item_manager->UseItemByType(
            eITEMTYPE_SOCKET_COMPOSE_ADDITIVE, SI_INVENTORY, additive_position, true, __FUNCTION__);
        if (additive_using_result != RC::RC_ITEM_SUCCESS)
        {
            return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
        }
    }

    const int fail_ratio = random(1, ZardComposeInfo::kMaxRatio);
    if (fail_ratio > success_ratio)
    {
        // ������ Ȯ���̱� ������ ���������� �ռ� �õ� ��ü�� �����߱� ������ �������� ó���Ѵ�.
        SUNLOG(eDEV_LOG, 
               _T("|%s|��� �ռ��� ���� �߽��ϴ�.|����Ȯ�� = %d, ����Ȯ�� = %d, ĳ�����̸� = %s|"), 
               __FUNCTION__, 
               success_ratio, 
               fail_ratio, 
               requester.GetCharName());
        return RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD;
    }
    
#ifdef _NA_007973_20150120_ADD_MULTIOPTION_ZARD
    // �ռ��� �������� ��� ������� ��忡 ���� ������ ��´�.
    SOCKETINDEX reward_zard_option_index=0;
    CODETYPE composed_result_zard_code=0;
    RC::eITEM_RESULT result;

    result = _GetZardComposeResultIndex(requester, inventory, zard_position_list, &reward_zard_option_index);
    if (result == RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD) {
        result = _GetZardItemCodeByIndex(requester, reward_zard_option_index, &composed_result_zard_code);
    }

    if (result != RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD) {
        //�����ϸ� �������
        return result;
    }

    // ������ �����Ѵ�.
    {
        typedef util::internal::ItemCompositionOrDecompositionReward ZardCompositionReward;
        ZardCompositionReward::ItemReward reward_zard;
        {
            reward_zard.item_code = static_cast<ZardCompositionReward::RewardItemCode>(composed_result_zard_code);
            reward_zard.number_of_item = 1;
            reward_zard.zard_option = reward_zard_option_index;
        }
        if (!reward.AddItemReward(reward_zard))
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|��� �������� ���� �߰��ϴµ� ���� �߽��ϴ�.|���������ڵ� = %d, ���ɼ��ε��� = %d, ĳ�����̸� = %s|"), 
                __FUNCTION__, 
                composed_result_zard_code, 
                reward_zard_option_index, 
                requester.GetCharName());
            return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
        }
    }
#else
    SOCKETINDEX reward_zard_option_index = SocketOptionParser::kInvalidSocketOptionIndex;
    CODETYPE reward_zard_code = 0;
    {
        const int selected_zard_index = random(0, ZardComposeInfoParser::kMaterialSize-1);
        
        const POSTYPE selected_zard_position = zard_position_list[selected_zard_index];
        const SCItemSlot& selected_zard_slot = 
            static_cast<const SCItemSlot&>(inventory->GetSlot(selected_zard_position));
        const BASE_ITEMINFO* selected_zard_info = selected_zard_slot.GetItemInfo();
        const BASE_ITEMINFO::ZardGrade reward_zard_grade = selected_zard_info->zard_grade+1;
        
        // ������� ��� �ɼ��� ��´�.
        const SocketOptionParser* const zard_option_parser = SocketOptionParser::Instance();
        reward_zard_option_index = zard_option_parser->GetRandomZardOptionByGrade(
            reward_zard_grade, selected_zard_info->zard_item_option_kind);
        if (reward_zard_option_index == SocketOptionParser::kInvalidSocketOptionIndex)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������� ��� �ɼ��� ��µ� ���� �߽��ϴ�.|����� = %d, ���ɼ����� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   reward_zard_grade, 
                   selected_zard_info->zard_item_option_kind, 
                   requester.GetCharName());
            return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
        }

        // ������� ��� �ڵ带 ��´�.
        const sSOCKET_OPTION* const reward_zard_option = 
            zard_option_parser->FindSocketOption(reward_zard_option_index);
        if (reward_zard_option == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������� ��� �ɼ��� ã�µ� ���� �߽��ϴ�.|���ɼ��ε��� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   reward_zard_option_index, 
                   requester.GetCharName());
            return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
        }
        reward_zard_code = reward_zard_option->item_code;

        const ItemInfoParser* const item_info_parser = ItemInfoParser::Instance();
        if (item_info_parser->GetItemInfo(reward_zard_code) == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������� ��带 ã�µ� ���� �߽��ϴ�.|���������ڵ� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   reward_zard_code, 
                   requester.GetCharName());
            return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
        }
    }

    // ������ �����Ѵ�.
    {
        typedef util::internal::ItemCompositionOrDecompositionReward ZardCompositionReward;
        ZardCompositionReward::ItemReward reward_zard;
        {
            reward_zard.item_code = static_cast<ZardCompositionReward::RewardItemCode>(reward_zard_code);
            reward_zard.number_of_item = 1;
            reward_zard.zard_option = reward_zard_option_index;
        }
        if (!reward.AddItemReward(reward_zard))
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|��� �������� ���� �߰��ϴµ� ���� �߽��ϴ�.|���������ڵ� = %d, ���ɼ��ε��� = %d, ĳ�����̸� = %s|"), 
                __FUNCTION__, 
                reward_zard_code, 
                reward_zard_option_index, 
                requester.GetCharName());
            return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
        }
    }
#endif //_NA_007973_20150120_ADD_MULTIOPTION_ZARD

    return RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD;
}

void SocketSystemImplement::HandleRecreateSocket( 
    const MSG_BASE* const message, const MessageSize message_size) const
{
    using namespace util::internal;

    // ��û�� �˻��Ѵ�.
    const MSG_CG_ITEM_RECREATE_SOCKET_SYN* request(NULL);
    Player* requester(NULL);
    if (CheckRequestMessage(message, message_size, request, requester) == false)
    {
        return;
    }

    MSG_CG_ITEM_RECREATE_SOCKET_ACK::ResultCode result_code = RC::RC_ITEM_FAILED;
    do 
    {
        // ��û���� �⺻���� ���¸� �˻��Ѵ�.
        if (CheckRequesterState(requester, __FUNCTION__, result_code) == false)
        {
            break;
        }

        // ���� �ٽ� ���� ��û ������ ��ġ�� �˻��Ѵ�.
        ItemManager* const item_manager = requester->GetItemManager();
        if (!item_manager->ValidPos(SI_INVENTORY, request->request_item_pos)) 
        {
            result_code = RC::RC_ITEM_INVALIDPOS;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���� �ٽ� ���� ��û ������ ���� ��ġ�� �ǹٸ��� �ʽ��ϴ�.|������ ���� ��ġ = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   request->request_item_pos, 
                   requester->GetCharName());
            break;
        }

        // ���� �ٽ� ���� ��û ���������� �˻��Ѵ�.
        SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
        const SCItemSlot& request_item_slot = 
            static_cast<const SCItemSlot&>(inventory->GetSlot(request->request_item_pos));
        const BASE_ITEMINFO* const request_item_info = request_item_slot.GetItemInfo();
        if (request_item_info->m_wType != eITEMTYPE_STAT_CHANGE) 
        {
            // UseItemByType() �ȿ����� �˻��ϴµ� ���� ���� �˻��� �ǹ̸� ������.
            // ���� �߰��ϸ� ���̻� ó���� �������� �����ϱ�.
            result_code = RC::RC_ITEM_INVALID_TYPE;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������ ������ �ǹٸ��� �ʽ��ϴ�.|�������ڵ� = %u, ���������� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   request_item_info->m_Code, 
                   request_item_info->m_wType, 
                   requester->GetCharName());
            break;
        }
            
        // ������ ���� ������ ��´�.
        const BYTE number_of_socket = static_cast<BYTE>(request_item_info->m_ExchangedItem);
        if (number_of_socket < 0 || number_of_socket > SOCKET_MAX)
        {
            // CreateSocketItem() �ȿ����� �˻��ϴµ� ���� ���� �˻��� �ǹ̸� ������.
            // ���� �߰��ϸ� ���ʿ��� ó���� �����Ѵٴ� �ǹ̵� �ְ� CreateSocketItem()�� 
            // ���������� ���̱� ������ ���� ����� ���� ����� �˻� ������ �� �־
            // �ܺ��� �˻� ������ ��ø�Ǵ� �κ��� ���� �� �ִ�.
            result_code = RC::RC_ITEM_INVALID_VALUE;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������ ���� ������ �ǹٸ��� �ʽ��ϴ�.|���� ���� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   number_of_socket, 
                   requester->GetCharName());
            break;
        }

        // ������ �ٽ� ������ ������ ��ġ�� �˻��Ѵ�.
        if (!item_manager->ValidPos(SI_INVENTORY, request->target_item_pos)) 
        {
            result_code = RC::RC_ITEM_INVALIDPOS;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|������ �ٽ� ������ ������ ���� ��ġ�� �ǹٸ��� �ʽ��ϴ�.|������ ���� ��ġ = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   request->target_item_pos, 
                   requester->GetCharName());
            break;
        }

        // ������ �ٽ� ���� ������ ���������� �˻��Ѵ�.
        SCItemSlot& target_slot = 
            static_cast<SCItemSlot&>(inventory->GetSlot(request->target_item_pos));
        if (target_slot.CanUseMixture(MIXTURE_SOCKET_HOLE_FILL) == false)
        {
            result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���� �ٽ� ������ ������ �������� �ƴմϴ�.|������ ���� ��ġ = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   request->target_item_pos, 
                   requester->GetCharName());
            break;
        }

        // ���� ���� Ȯ�� ������ ã�� �� �ִ��� �˻��Ѵ�.
        const SocketItemRatioParser* const ratio_parser = SocketItemRatioParser::Instance();
        const BASE_ITEMINFO* const target_info = target_slot.GetItemInfo();
        SocketItemRatioInfo::RatioType::Value ratio_type = 
            ratio_parser->GetRatioType(target_info->m_wType, true);
        const SocketItemRatioInfo::RatioType::Value fixed_ratio_type = request_item_info->m_wSubType;
        if (fixed_ratio_type != 0)
        {
            ratio_type = fixed_ratio_type;
            if (target_info->IsArmor())
            {
                ++ratio_type;
            }
        }
        const SocketItemRatioInfo* const target_ratio = 
            ratio_parser->FindSocketRatio(ratio_type, target_info->m_LV);
        if (target_ratio == NULL)
        {
            result_code = RC::RC_ITEM_NOINFO;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|���� ���� Ȯ�� ������ �����ϴ�.|������ Ÿ�� = %d, ������ ���� = %d, ĳ�����̸� = %s|"), 
                   __FUNCTION__, 
                   target_info->m_wType, 
                   target_info->m_LV, 
                   requester->GetCharName());
            break;
        }

        // ������� ���� ���� �ٽ� ������ �����ϱ� ������ ���� �ٽ� ���� ��û �������� ����Ѵ�.
        // ��� �˻縦 ����� ������ ����ϸ� Ʈ����� ���н� ���� �������� �ǻ츱 �ʿ䰡 ����.
        result_code = item_manager->UseItemByType(
            eITEMTYPE_STAT_CHANGE, SI_INVENTORY, request->request_item_pos, true, __FUNCTION__);
        if (result_code != RC::RC_ITEM_SUCCESS)
        {
            break;
        }
        
        // ������ �ٽ� �����Ѵ�.
        const BYTE prev_socket_num = target_slot.GetSocketNum();
        if (target_slot.CreateSocketItem(number_of_socket, true) == false)
        {
            result_code = RC::RC_ITEM_INVALID_VALUE;
            break;
        }
        
        // ���� �ٽ� ���� �õ��� �����ߴµ� �õ� ����� ������ �������� �ʾҴ�.
        if (target_slot.GetSocketIdentifyType() == eSOCKET_FAIL_IDENTIFY_PROCESS)
        {
            result_code = RC::RC_ITEM_SOCKET_FAILED_TO_EXTRACT_BY_RATIO;
        }
        else if (target_slot.GetSocketIdentifyType() == eSOCKET_UNIDENTIFIED_ITEM)
        {
            // ������ �����ϴµ� ���������� ������ �����Ѵ�.
            const int seed = random(1, SocketItemRatioInfo::kMaxRatio);
            int accumulation_ratio = 0;
            for (BYTE i = SOCKET_MAX; i > SOCKET_1; --i)
            {
                accumulation_ratio += target_ratio->socket_ratio_[i-1];
                if (seed <= accumulation_ratio)
                {
                    target_slot.SetSocketNum(i);
                    break;
                }
            }
            target_slot.SetSocketIdentifyType(eSOCKET_IDENTIFIED_ITEM);
        }

        // �ٽ� ������ ���� ���� ������ �˸���.
        item_manager->SendItemChange(target_slot);

        // �α׸� ����Ѵ�.
        {
            TCHAR add_info[MAX_ADDINFO_SIZE];
            ::_sntprintf(
                add_info, 
                _countof(add_info), 
                _T("S:%u, %u"), 
                prev_socket_num, 
                target_slot.GetSocketNum());
            add_info[_countof(add_info) - 1] = _T('\0');
            GAMELOG->LogItem(
                ITEM_SOCKET_ADD, requester, &target_slot, 0, 1, NULL, NULL, add_info);
        }
    } while (false);

    // ������ ������.
    MSG_CG_ITEM_RECREATE_SOCKET_ACK response;
    {
        response.Init();
        response.result_code = result_code;
    }
    requester->SendPacket(&response, response.GetSize());
}

#ifdef _NA_007973_20150120_ADD_MULTIOPTION_ZARD
RC::eITEM_RESULT SocketSystemImplement::_GetZardComposeResultIndex(Player& requester, SCItemSlotContainer* const inventory, 
                                                                   const POSTYPE* zard_position_list, SOCKETINDEX* socket_index)
{
    // �ռ��� �������� ��� ������� ��忡 ���� ������ ��´�.
    SOCKETINDEX reward_zard_option_index = SocketOptionParser::kInvalidSocketOptionIndex;
    CODETYPE reward_zard_code = 0;

    const int selected_zard_index = random(0, ZardComposeInfoParser::kMaterialSize-1);
    
    const POSTYPE selected_zard_position = zard_position_list[selected_zard_index];
    const SCItemSlot& selected_zard_slot = 
        static_cast<const SCItemSlot&>(inventory->GetSlot(selected_zard_position));
    const BASE_ITEMINFO* selected_zard_info = selected_zard_slot.GetItemInfo();
    const BASE_ITEMINFO::ZardGrade reward_zard_grade = selected_zard_info->zard_grade+1;
    
    // ������� ��� �ɼ��� ��´�.
    const SocketOptionParser* const zard_option_parser = SocketOptionParser::Instance();
    reward_zard_option_index = zard_option_parser->GetRandomZardOptionByGrade(
        reward_zard_grade, selected_zard_info->zard_item_option_kind);
    if (reward_zard_option_index == SocketOptionParser::kInvalidSocketOptionIndex)
    {
        SUNLOG(eCRITICAL_LOG, 
            _T("|%s|������� ��� �ɼ��� ��µ� ���� �߽��ϴ�.|����� = %d, ���ɼ����� = %d, ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            reward_zard_grade, 
            selected_zard_info->zard_item_option_kind, 
            requester.GetCharName());
        return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
    }

    *socket_index = reward_zard_option_index;
    return RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD;
}

RC::eITEM_RESULT SocketSystemImplement::_GetZardItemCodeByIndex( Player& requester, SOCKETINDEX socket_index, 
                                                                 CODETYPE* OUT zard_code )
{
    const SocketOptionParser* const zard_option_parser = SocketOptionParser::Instance();

    // ������� ��� �ڵ带 ��´�.
    const sSOCKET_OPTION* const reward_zard_option = 
        zard_option_parser->FindSocketOption(socket_index);
    if (reward_zard_option == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
            _T("|%s|������� ��� �ɼ��� ã�µ� ���� �߽��ϴ�.|���ɼ��ε��� = %d, ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            socket_index, 
            requester.GetCharName());
        return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
    }
    CODETYPE composed_result_zard = reward_zard_option->item_code;

    const ItemInfoParser* const item_info_parser = ItemInfoParser::Instance();
    if (item_info_parser->GetItemInfo(composed_result_zard) == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
            _T("|%s|������� ��带 ã�µ� ���� �߽��ϴ�.|���������ڵ� = %d, ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            composed_result_zard, 
            requester.GetCharName());
        return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
    }

    *zard_code = composed_result_zard;
    return RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD;
}   

#endif //_NA_007973_20150120_ADD_MULTIOPTION_ZARD

#endif // _NA_000251_20100727_SOCKET_SYSTEM