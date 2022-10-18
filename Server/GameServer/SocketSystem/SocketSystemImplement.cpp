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
    // 요청 메시지를 검사한다.
    const MSG_CG_ITEM_IDENTIFY_SOCKET_CMD* identify_socket_cmd(NULL);
    Player* requester(NULL);
    if (!util::internal::CheckRequestMessage(message, message_size, identify_socket_cmd, requester))
    {
        return;
    }
    
    MSG_CG_ITEM_IDENTIFY_SOCKET_NAK::ErrorCode error_code = 0;
    do 
    {
        // 요청자의 기본적인 상태를 검사한다.
        if (!util::internal::CheckRequesterState(requester, __FUNCTION__, error_code))
        {
            break;
        }
        
        // 소켓을 확인할 아이템 위치를 검사한다.
        ItemManager* const item_manager = requester->GetItemManager();
        if (!item_manager->ValidPos(SI_INVENTORY, identify_socket_cmd->unidentified_item_pos_)) 
        {
            error_code = RC::RC_ITEM_INVALIDPOS;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|소켓 미확인 아이템 슬롯 위치가 옳바르지 않습니다.|아이템 슬롯 위치 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   identify_socket_cmd->unidentified_item_pos_, 
                   requester->GetCharName());
            break;
        }

        SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
        SCItemSlot& unidentified_item_slot = 
            static_cast<SCItemSlot&>(inventory->GetSlot(identify_socket_cmd->unidentified_item_pos_));
        
        // 소켓 미확인 아이템인지 검사한다.
        const eSOCKET_IDENTIFY_TYPE identify_state = 
            static_cast<eSOCKET_IDENTIFY_TYPE>(unidentified_item_slot.GetSocketIdentifyType());
        if (identify_state != eSOCKET_UNIDENTIFIED_ITEM)
        {
            error_code = RC::RC_ITEM_NOT_UNIDENTIFIED_SOCKET;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|소켓 미확인 아이템이 아닙니다.|확인상태 = %d, 아이템 슬롯 위치 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   identify_state, 
                   identify_socket_cmd->unidentified_item_pos_, 
                   requester->GetCharName());
            break;
        }

        // 소켓 생성 확률 정보를 찾을 수 있는지 검사한다.
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
                   _T("|%s|소켓 생성 확률 정보가 없습니다.|아이템 타입 = %d, 아이템 레벨 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   unidentified_item->m_wType, 
                   unidentified_item->m_LV, 
                   requester->GetCharName());
            break;
        }

        // 여기까지 오면 소켓 확인은 성공하기 때문에 소켓 확인 아이템을 사용했을 경우를 처리한다.
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

        // 소켓 개수를 설정한다.
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

        // 로그를 기록한다.
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
    // 쟈드 합성인지 검사한다.
    if (!request.is_composition_)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|쟈드 합성이 아닙니다.|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
    }

    // 합성할 쟈드 개수를 검사한다.
    if (request.material_list_size_ != ZardComposeInfoParser::kMaterialSize)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|합성할 쟈드 개수가 옳바르지 않습니다.|쟈드개수 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               request.material_list_size_, 
               requester.GetCharName());
        return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
    }

    // 합성할 쟈드 위치를 얻는다.
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
                       _T("|%s|합성할 쟈드 정보가 옳바르지 않습니다.|SelectedNumber = %d, MatchedIndex = %d, 캐릭터이름 = %s|"), 
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
               _T("|%s|쟈드 성공 확률을 얻는데 실패했습니다.|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               requester.GetCharName());
        return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
    }

    // 첨가제를 사용했을 경우를 처리한다.
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
        // 보상은 확률이기 때문에 실패했지만 합성 시도 자체는 성공했기 때문에 성공으로 처리한다.
        SUNLOG(eDEV_LOG, 
               _T("|%s|쟈드 합성에 실패 했습니다.|성공확률 = %d, 실패확률 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               success_ratio, 
               fail_ratio, 
               requester.GetCharName());
        return RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD;
    }
    
#ifdef _NA_007973_20150120_ADD_MULTIOPTION_ZARD
    // 합성에 성공했을 경우 보상받을 쟈드에 대한 정보를 얻는다.
    SOCKETINDEX reward_zard_option_index=0;
    CODETYPE composed_result_zard_code=0;
    RC::eITEM_RESULT result;

    result = _GetZardComposeResultIndex(requester, inventory, zard_position_list, &reward_zard_option_index);
    if (result == RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD) {
        result = _GetZardItemCodeByIndex(requester, reward_zard_option_index, &composed_result_zard_code);
    }

    if (result != RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD) {
        //실패하면 결과리턴
        return result;
    }

    // 보상을 설정한다.
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
                _T("|%s|쟈드 아이템을 보상에 추가하는데 실패 했습니다.|쟈드아이템코드 = %d, 쟈드옵션인덱스 = %d, 캐릭터이름 = %s|"), 
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
        
        // 보상받을 쟈드 옵션을 얻는다.
        const SocketOptionParser* const zard_option_parser = SocketOptionParser::Instance();
        reward_zard_option_index = zard_option_parser->GetRandomZardOptionByGrade(
            reward_zard_grade, selected_zard_info->zard_item_option_kind);
        if (reward_zard_option_index == SocketOptionParser::kInvalidSocketOptionIndex)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|보상받을 쟈드 옵션을 얻는데 실패 했습니다.|쟈드등급 = %d, 쟈드옵션종류 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   reward_zard_grade, 
                   selected_zard_info->zard_item_option_kind, 
                   requester.GetCharName());
            return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
        }

        // 보상받을 쟈드 코드를 얻는다.
        const sSOCKET_OPTION* const reward_zard_option = 
            zard_option_parser->FindSocketOption(reward_zard_option_index);
        if (reward_zard_option == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|보상받을 쟈드 옵션을 찾는데 실패 했습니다.|쟈드옵션인덱스 = %d, 캐릭터이름 = %s|"), 
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
                   _T("|%s|보상받을 쟈드를 찾는데 실패 했습니다.|쟈드아이템코드 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   reward_zard_code, 
                   requester.GetCharName());
            return RC::RC_ITEM_FAILED_TO_COMPOSE_ZARD;
        }
    }

    // 보상을 설정한다.
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
                _T("|%s|쟈드 아이템을 보상에 추가하는데 실패 했습니다.|쟈드아이템코드 = %d, 쟈드옵션인덱스 = %d, 캐릭터이름 = %s|"), 
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

    // 요청을 검사한다.
    const MSG_CG_ITEM_RECREATE_SOCKET_SYN* request(NULL);
    Player* requester(NULL);
    if (CheckRequestMessage(message, message_size, request, requester) == false)
    {
        return;
    }

    MSG_CG_ITEM_RECREATE_SOCKET_ACK::ResultCode result_code = RC::RC_ITEM_FAILED;
    do 
    {
        // 요청자의 기본적인 상태를 검사한다.
        if (CheckRequesterState(requester, __FUNCTION__, result_code) == false)
        {
            break;
        }

        // 소켓 다시 생성 요청 아이템 위치를 검사한다.
        ItemManager* const item_manager = requester->GetItemManager();
        if (!item_manager->ValidPos(SI_INVENTORY, request->request_item_pos)) 
        {
            result_code = RC::RC_ITEM_INVALIDPOS;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|소켓 다시 생성 요청 아이템 슬롯 위치가 옳바르지 않습니다.|아이템 슬롯 위치 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   request->request_item_pos, 
                   requester->GetCharName());
            break;
        }

        // 소켓 다시 생성 요청 아이템인지 검사한다.
        SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
        const SCItemSlot& request_item_slot = 
            static_cast<const SCItemSlot&>(inventory->GetSlot(request->request_item_pos));
        const BASE_ITEMINFO* const request_item_info = request_item_slot.GetItemInfo();
        if (request_item_info->m_wType != eITEMTYPE_STAT_CHANGE) 
        {
            // UseItemByType() 안에서도 검사하는데 여긴 사전 검사의 의미를 가진다.
            // 빨리 발견하면 더이상 처리를 진행하지 않으니까.
            result_code = RC::RC_ITEM_INVALID_TYPE;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|아이템 종류가 옳바르지 않습니다.|아이템코드 = %u, 아이템종류 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   request_item_info->m_Code, 
                   request_item_info->m_wType, 
                   requester->GetCharName());
            break;
        }
            
        // 설정할 소켓 개수를 얻는다.
        const BYTE number_of_socket = static_cast<BYTE>(request_item_info->m_ExchangedItem);
        if (number_of_socket < 0 || number_of_socket > SOCKET_MAX)
        {
            // CreateSocketItem() 안에서도 검사하는데 여긴 사전 검사의 의미를 가진다.
            // 빨리 발견하면 불필요한 처리를 중지한다는 의미도 있고 CreateSocketItem()은 
            // 여러곳에서 쓰이기 때문에 여러 경우의 수를 고려한 검사 로직이 들어가 있어서
            // 외부의 검사 로직과 중첩되는 부분이 있을 수 있다.
            result_code = RC::RC_ITEM_INVALID_VALUE;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|설정할 소켓 개수가 옳바르지 않습니다.|소켓 개수 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   number_of_socket, 
                   requester->GetCharName());
            break;
        }

        // 소켓을 다시 생성할 아이템 위치를 검사한다.
        if (!item_manager->ValidPos(SI_INVENTORY, request->target_item_pos)) 
        {
            result_code = RC::RC_ITEM_INVALIDPOS;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|소켓을 다시 생성할 아이템 슬롯 위치가 옳바르지 않습니다.|아이템 슬롯 위치 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   request->target_item_pos, 
                   requester->GetCharName());
            break;
        }

        // 소켓을 다시 생성 가능한 아이템인지 검사한다.
        SCItemSlot& target_slot = 
            static_cast<SCItemSlot&>(inventory->GetSlot(request->target_item_pos));
        if (target_slot.CanUseMixture(MIXTURE_SOCKET_HOLE_FILL) == false)
        {
            result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|소켓 다시 생성이 가능한 아이템이 아닙니다.|아이템 슬롯 위치 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   request->target_item_pos, 
                   requester->GetCharName());
            break;
        }

        // 소켓 생성 확률 정보를 찾을 수 있는지 검사한다.
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
                   _T("|%s|소켓 생성 확률 정보가 없습니다.|아이템 타입 = %d, 아이템 레벨 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   target_info->m_wType, 
                   target_info->m_LV, 
                   requester->GetCharName());
            break;
        }

        // 여기까지 오면 소켓 다시 생성은 성공하기 때문에 소켓 다시 생성 요청 아이템을 사용한다.
        // 모든 검사를 통과한 시점에 사용하면 트랜잭션 실패시 사용된 아이템을 되살릴 필요가 없다.
        result_code = item_manager->UseItemByType(
            eITEMTYPE_STAT_CHANGE, SI_INVENTORY, request->request_item_pos, true, __FUNCTION__);
        if (result_code != RC::RC_ITEM_SUCCESS)
        {
            break;
        }
        
        // 소켓을 다시 생성한다.
        const BYTE prev_socket_num = target_slot.GetSocketNum();
        if (target_slot.CreateSocketItem(number_of_socket, true) == false)
        {
            result_code = RC::RC_ITEM_INVALID_VALUE;
            break;
        }
        
        // 소켓 다시 생성 시도는 성공했는데 시도 결과로 소켓이 생성되지 않았다.
        if (target_slot.GetSocketIdentifyType() == eSOCKET_FAIL_IDENTIFY_PROCESS)
        {
            result_code = RC::RC_ITEM_SOCKET_FAILED_TO_EXTRACT_BY_RATIO;
        }
        else if (target_slot.GetSocketIdentifyType() == eSOCKET_UNIDENTIFIED_ITEM)
        {
            // 소켓을 생성하는데 성공했으면 개수를 결정한다.
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

        // 다시 생성된 소켓 상태 변경을 알린다.
        item_manager->SendItemChange(target_slot);

        // 로그를 기록한다.
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

    // 응답을 보낸다.
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
    // 합성에 성공했을 경우 보상받을 쟈드에 대한 정보를 얻는다.
    SOCKETINDEX reward_zard_option_index = SocketOptionParser::kInvalidSocketOptionIndex;
    CODETYPE reward_zard_code = 0;

    const int selected_zard_index = random(0, ZardComposeInfoParser::kMaterialSize-1);
    
    const POSTYPE selected_zard_position = zard_position_list[selected_zard_index];
    const SCItemSlot& selected_zard_slot = 
        static_cast<const SCItemSlot&>(inventory->GetSlot(selected_zard_position));
    const BASE_ITEMINFO* selected_zard_info = selected_zard_slot.GetItemInfo();
    const BASE_ITEMINFO::ZardGrade reward_zard_grade = selected_zard_info->zard_grade+1;
    
    // 보상받을 쟈드 옵션을 얻는다.
    const SocketOptionParser* const zard_option_parser = SocketOptionParser::Instance();
    reward_zard_option_index = zard_option_parser->GetRandomZardOptionByGrade(
        reward_zard_grade, selected_zard_info->zard_item_option_kind);
    if (reward_zard_option_index == SocketOptionParser::kInvalidSocketOptionIndex)
    {
        SUNLOG(eCRITICAL_LOG, 
            _T("|%s|보상받을 쟈드 옵션을 얻는데 실패 했습니다.|쟈드등급 = %d, 쟈드옵션종류 = %d, 캐릭터이름 = %s|"), 
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

    // 보상받을 쟈드 코드를 얻는다.
    const sSOCKET_OPTION* const reward_zard_option = 
        zard_option_parser->FindSocketOption(socket_index);
    if (reward_zard_option == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
            _T("|%s|보상받을 쟈드 옵션을 찾는데 실패 했습니다.|쟈드옵션인덱스 = %d, 캐릭터이름 = %s|"), 
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
            _T("|%s|보상받을 쟈드를 찾는데 실패 했습니다.|쟈드아이템코드 = %d, 캐릭터이름 = %s|"), 
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