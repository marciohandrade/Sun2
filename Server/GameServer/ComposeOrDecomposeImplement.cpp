////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   ComposeOrDecomposeImplement.cpp
/// @author hukim
/// @brief  [2010/10/14 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\ComposeOrDecomposeImplement.h" // header
#include "ItemManager.h"
#include "SCItemSlotContainer.h"
#include "ImplementUtil.h"
#include "SocketSystem/SocketSystemImplement.h"
#include "CubeSystem/CubeSystemImplement.h"
#include "ItemTypeList.h"
#include "PacketStruct_DG.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

ComposeOrDecomposeImplement::ComposeOrDecomposeImplement()
{
}

ComposeOrDecomposeImplement::~ComposeOrDecomposeImplement()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Implement
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace util {
namespace internal {
// 사용 가능한 종류의 합성 분해 내역을 검사한다.
static bool CheckGetHistoryFilter(
    const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::RequestType request_type, const bool is_composition)
{
    // 큐브의 합성 내역만 사용 하도록 제한한다.
    // 제한 안해도 제네릭하게 구현되어 정상적으로 동작하지만 현재 클라이언트가 필요한건 
    // 큐브 합성 내역 뿐이라서 일단 막아둔다.
    // 나중에 클라이언트 기능도 추가되면 서버는 아래 검사만 제거하면 정상 동작한다.
    return (request_type == MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest) && 
        (is_composition == true);
    //return true;
}
} // internal
} // util


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

void ComposeOrDecomposeImplement::HandleComposeOrDecompose( 
    const MSG_BASE* const message, const MessageSize message_size) const
{
    using namespace util::internal;
    typedef MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN RequestMessage;

    // 요청 메시지를 검사한다.
    const RequestMessage* request(NULL);
    Player* requester(NULL);
    if (CheckRequestMessage(message, message_size, request, requester) == false)
    {
        return;
    }

    // 요청 종류를 검사한다.
    switch (request->request_type_)
    {
    case RequestMessage::kZardRequest:
    case RequestMessage::kCubeRequest:
        break;
    default:
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|지원하지 않는 형식의 아이템 합성 또는 분해 요청입니다.|요청종류 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               request->request_type_, 
               requester->GetCharName());
        return;
    }

    // 응답 보내기를 생성한다.
    struct ResponseSender 
    {
        explicit ResponseSender(const RequestMessage& request, Player& requester) : 
            request_(request), requester_(requester), result_code(0)
        {
            reward_items.Clear();
        }
        ~ResponseSender()
        {
            switch (request_.request_type_)
            {
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
            case RequestMessage::kZardRequest:
                {
                    MSG_CG_ITEM_ZARD_COMPOSE_ACK response;
                    response.is_composition_ = request_.is_composition_;
                    response.result_code_ = result_code;
                    response.added_inventory_items_ = reward_items;
                    requester_.SendPacket(&response, response.GetSize());
                }
                break;
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#ifdef _NA_002253_20100811_CUBE_SYSTEM
            case RequestMessage::kCubeRequest:
                {
                    MSG_CG_ITEM_CUBE_COMPOSE_OR_DECOMPOSE_ACK response;
                    response.is_composition_ = request_.is_composition_;
                    response.result_code_ = result_code;
                    response.added_inventory_items_ = reward_items;
                    requester_.SendPacket(&response, response.GetSize());
                }
                break;
#endif // _NA_002253_20100811_CUBE_SYSTEM
            }
        }
        MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_ACK::ResultCode result_code;
        INVENTORY_TOTAL_INFO reward_items;
    private:
        const RequestMessage& request_;
        Player& requester_;
        void operator=(const ResponseSender&) {}
    };
    ResponseSender response_sender(*request, *requester);

    // 요청자의 기본적인 상태를 검사한다.
    if (CheckRequesterState(requester, __FUNCTION__, response_sender.result_code) == false)
    {
        return;
    }

    // 합성 또는 분해할 재료가 적어도 하나는 필요하다.
    if (request->material_list_size_ < 1 || 
        request->material_list_size_ > _countof(request->material_list_))
    {
        response_sender.result_code = RC::RC_ITEM_INVALID_VALUE;
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|합성 또는 분해할 재료 개수가 올바르지 않습니다.|재료개수 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               request->material_list_size_, 
               requester->GetCharName());
        return;
    }

    // 인벤토리에 보상을 받을 여유 슬롯이 적어도 하나는 필요하다.
    ItemManager* const item_manager = requester->GetItemManager();
    SCItemSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    if (inventory->GetEmptySlotNum() == 0)
    {
        response_sender.result_code = RC::RC_ITEM_HAVENOTSPACE;
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|인벤토리에 보상을 받을 여유 공간이 없습니다.|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               requester->GetCharName());
        return;
    }

    // 재료 위치를 검사한다.
    util::BitSetOperation<MAX_INVENTORY_SLOT_NUM> duplication_checker;
    duplication_checker.ClearAll();
    for (RequestMessage::MaterialListSize i = 0; i < request->material_list_size_; ++i)
    {
        const POSTYPE material_position = ((request->material_list_)[i]).inventory_position_;
        if (duplication_checker.IsSet(material_position))
        {
            response_sender.result_code = RC::RC_ITEM_INVALID_VALUE;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|인벤토리의 재료 위치가 중복되었습니다.|재료위치 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   material_position, 
                   requester->GetCharName());
            return;
        }
        if (item_manager->ValidPos(SI_INVENTORY, material_position) == false)
        {
            response_sender.result_code = RC::RC_ITEM_INVALID_VALUE;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|인벤토리의 재료 위치가 옳바르지 않습니다.|재료위치 = %d, 캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   material_position, 
                   requester->GetCharName());
            return;
        }
        duplication_checker.Set(material_position);
    }

    // 보상을 초기화한다.
    ItemCompositionOrDecompositionReward reward;
    if (reward.Init(requester, ItemCompositionOrDecompositionReward::kItemCodeReward) == false)
    {
        response_sender.result_code = RC::RC_ITEM_INVALID_VALUE;
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|보상을 초기화 하는데 실패 했습니다.|캐릭터이름 = %s|"), 
               __FUNCTION__, 
               requester->GetCharName());
        return;
    }
    
    // 합성 또는 분해한다.
    int reward_log_type = ItemCompositionOrDecompositionReward::kDoNotUseLog;
    switch (request->request_type_)
    {
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    case MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kZardRequest:
        {
            SocketSystemImplement socket_system_impl;
            response_sender.result_code = 
                socket_system_impl.ZardCompose(*request, *requester, reward);
            if (response_sender.result_code != RC::RC_ITEM_SUCCESS_TO_COMPOSE_ZARD)
            {
                return;
            }
            reward_log_type = ITEM_ZARD_COMPOSE_REWARD;
        }
        break;
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#ifdef _NA_002253_20100811_CUBE_SYSTEM
    case MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kCubeRequest:
        {
            CubeSystemImplement cube_system_impl;
            response_sender.result_code = 
                cube_system_impl.CubeComposeOrDecompose(*request, *requester, reward);
            if (request->is_composition_)
            {
                if (response_sender.result_code != RC::RC_ITEM_SUCCESS_TO_COMPOSE_CUBE)
                {
                    return;
                }
                reward_log_type = ITEM_CUBE_COMPOSE_REWARD;
            }
            else
            {
                if (response_sender.result_code != RC::RC_ITEM_SUCCESS_TO_DECOMPOSE_CUBE)
                {
                    return;
                }
                reward_log_type = ITEM_CUBE_DECOMPOSE_REWARD;
            }
        }
        break;
#endif // _NA_002253_20100811_CUBE_SYSTEM
    }
    
    // 합성 또는 분해에 성공했다면 인벤토리에 보상을 받는다.
    const RC::eITEM_RESULT obtain_result = 
        reward.ObtainRewardToInventory(reward_log_type, response_sender.reward_items);
    if (obtain_result != RC::RC_ITEM_SUCCESS)
    {
        response_sender.result_code = obtain_result;
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|인벤토리에 보상 받기에 실패 했습니다.|실패코드 = %d, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               obtain_result, 
               requester->GetCharName());
        return;
    }

    // 재료 로그 형식을 설정한다.
    int material_log_type = ItemCompositionOrDecompositionReward::kDoNotUseLog;
    switch (reward_log_type)
    {
    case ITEM_ZARD_COMPOSE_REWARD:
        material_log_type = ITEM_ZARD_COMPOSE_MATERIAL;
        break;
    case ITEM_CUBE_COMPOSE_REWARD:
        material_log_type = ITEM_CUBE_COMPOSE_MATERIAL;
        break;
    case ITEM_CUBE_DECOMPOSE_REWARD:
        material_log_type = ITEM_CUBE_DECOMPOSE_MATERIAL;
        break;
    }

    // 사용한 재료들을 인벤토리에서 제거한다.
    const bool use_history = CheckGetHistoryFilter(request->request_type_, request->is_composition_);
    ItemCompositionOrDecompositionHistory history;
    MSG_CG_ITEM_LOSE_ACK item_lose_ack;
    for (RequestMessage::MaterialListSize i = 0; i < request->material_list_size_; ++i)
    {
        const MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& material = 
            (request->material_list_)[i];

        INVEN_DELETE_TOTAL_INFO::SLOT_TYPE& lose_slot = item_lose_ack.m_ItemInfo.m_Slot[i];
        lose_slot.m_Pos = material.inventory_position_;
        lose_slot.m_Num = material.selected_number_;
        SCItemSlot& inven_item = 
            static_cast<SCItemSlot&>(inventory->GetSlot(material.inventory_position_));

        // 보상 받은 아이템이 있으면 합성 분해 내역에 사용한 재료 아이템을 추가한다.
        if (use_history && reward.IsExistObtainedToInventory())
        {
            ItemCompositionOrDecompositionHistory::UsedItem used_item;
            used_item.is_material = true;
            used_item.item_code = inven_item.GetItemCode();
            used_item.number_of_item = material.selected_number_;
            used_item.ig_type = nsSlot::ItemTypeChanger::GetKindOfItemType(inven_item);
            history.AddUsedItem(used_item);
        }

        // 인벤토리에서 재료 개수를 갱신한다.
        if (material.selected_number_ < inven_item.GetNum())
        {
            GAMELOG->LogItem(
                material_log_type, 
                requester, 
                &inven_item, 
                0, 
                material.selected_number_, 
                NULL, 
                NULL, 
                _T("U")
            );
            inventory->UpdateSlot(inven_item.GetPos(), -material.selected_number_);
        }
        else
        {
            GAMELOG->LogItem(
                material_log_type, 
                requester, 
                &inven_item, 
                0, 
                material.selected_number_
            ); 
            inventory->DeleteSlot(inven_item.GetPos(), NULL);
        }
    }
    item_lose_ack.m_ItemInfo.m_Count = request->material_list_size_;
    requester->SendPacket(&item_lose_ack, item_lose_ack.GetSize());
    
    // 보상 받은 아이템이 있으면 합성 분해 내역을 디비에 기록한다.
    if (use_history && reward.IsExistObtainedToInventory())
    {
        // 내역을 설정한다.
        history.request_type = request->request_type_;
        history.is_composition = request->is_composition_;
        util::GetDateTime_YYYYMMDDHHMMSS(history.history_date_time);
        reward.AddToHistory(history);

        // 내역을 보낸다.
        MSG_DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD insert_history_cmd;
        insert_history_cmd.new_composition_or_decomposition_history = history;
        requester->SendToGameDBPServer(&insert_history_cmd, insert_history_cmd.GetSize());
    }
}

void ComposeOrDecomposeImplement::HandleGetHistories( 
    const MSG_BASE* const message, const MessageSize message_size) const
{
    // 요청 메시지를 검사한다.
    const MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN* get_histories_syn(NULL);
    Player* requester(NULL);
    if (!util::internal::CheckRequestMessage(message, message_size, get_histories_syn, requester))
    {
        return;
    }

    // 요청자의 기본적인 상태를 검사한다.
    MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK::ResultCode result_code = 0;
    if (!util::internal::CheckRequesterState(requester, __FUNCTION__, result_code))
    {
        return;
    }

    // 지원하는 요청인지 검사한다.
    if (!util::internal::CheckGetHistoryFilter(
        get_histories_syn->request_type, get_histories_syn->is_composition))
    {
        MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK response;
        response.Init();
        response.result_code = RC::RC_ITEM_FAILED_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES;
        requester->SendPacket(&response, response.GetSize());

        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|지원하지 않는 형식의 아이템 합성 또는 분해 내역 얻기 요청입니다.|요청종류 = %u, 합성분해여부 = %u, 캐릭터이름 = %s|"), 
               __FUNCTION__, 
               get_histories_syn->request_type, 
               (get_histories_syn->is_composition ? 1 : 0), 
               requester->GetCharName());
        return;
    }
#ifdef _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK
#else
    // 내역 얻기 요청을 보낸다.
    MSG_DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN select_histories_syn;
    {
        select_histories_syn.Init();
        select_histories_syn.request_type = get_histories_syn->request_type;
        select_histories_syn.is_composition = get_histories_syn->is_composition;
    }
    requester->SendToGameDBPServer(&select_histories_syn, select_histories_syn.GetSize());
#endif // _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK
}

void ComposeOrDecomposeImplement::HandleGetHistoriesResponseFromDBP( 
    const MSG_BASE* const message, const MessageSize message_size) const
{
    // 받은 메시지를 검사한다.
    const MSG_DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK* dbp_ack(NULL);
    Player* requester(NULL);
    if (!util::internal::CheckRequestMessage(
        message, message_size, dbp_ack, requester))
    {
        return;
    }

    // 요청자의 기본적인 상태를 검사한다.
    MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK::ResultCode result_code = 0;
    if (!util::internal::CheckRequesterState(requester, __FUNCTION__, result_code))
    {
        return;
    }

    // 요청자에게 응답을 보낸다.
    MSG_CG_ITEM_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK response;
    {
        response.Init();
        response.result_code = dbp_ack->result_code;
        if (response.result_code == 
            RC::RC_ITEM_SUCCESS_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES)
        {
            if (dbp_ack->history_list_size > 0)
            {
                response.history_list_size = dbp_ack->history_list_size;
                enum { kMaxResponseHistoryListSize = _countof(response.history_list) };
                if (response.history_list_size > kMaxResponseHistoryListSize)
                {
                    response.history_list_size = kMaxResponseHistoryListSize;
                }
                BOOST_STATIC_ASSERT(sizeof(dbp_ack->history_list[0]) == sizeof(response.history_list[0]));
                BOOST_STATIC_ASSERT(sizeof(dbp_ack->history_list) == sizeof(response.history_list));
                ::CopyMemory(
                    response.history_list, 
                    dbp_ack->history_list, 
                    sizeof(dbp_ack->history_list[0]) * dbp_ack->history_list_size);
            }
        }
    }
    requester->SendPacket(&response, response.GetSize());
}

void ComposeOrDecomposeImplement::HandleModifyHistories( 
    const MSG_BASE* const message, const MessageSize message_size) const
{
    // 요청 메시지를 검사한다.
    typedef MSG_CG_ITEM_MODIFY_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD ModifyHistoriesCmd;
    const ModifyHistoriesCmd* modify_histories_cmd(NULL);
    Player* requester(NULL);
    if (!util::internal::CheckRequestMessage(message, message_size, modify_histories_cmd, requester))
    {
        return;
    }

    // 요청자의 기본적인 상태를 검사한다.
    int result_code = 0;
    if (!util::internal::CheckRequesterState(requester, __FUNCTION__, result_code))
    {
        return;
    }

    // 내역 수정 요청을 보낸다.
    MSG_DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD update_histories_cmd;
    {
        update_histories_cmd.Init();

        // 받은 내역 크기를 검사한다.
        ModifyHistoriesCmd::ConditionListSize received_condition_list_size = 
            modify_histories_cmd->condition_list_size;
        enum { kMaxUpdateConditionListSize = _countof(update_histories_cmd.modify_condition_list) };
        if (received_condition_list_size > kMaxUpdateConditionListSize)
        {
            received_condition_list_size = kMaxUpdateConditionListSize;
        }

        // 받은 조건들이 유효한지 검사한다.
        for (ModifyHistoriesCmd::ConditionListSize i = 0; i < received_condition_list_size; ++i)
        {
            const ModifyHistoriesCmd::ModifyCondition& received_condition = 
                modify_histories_cmd->modify_condition_list[i];
            if (received_condition.history_id > 0)
            {
                update_histories_cmd.modify_condition_list[i] = received_condition;
                ++update_histories_cmd.condition_list_size;
            }
        }
        if (update_histories_cmd.condition_list_size < 1)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|내역 수정 조건들이 유효하지 않습니다.|캐릭터이름 = %s|"), 
                   __FUNCTION__, 
                   requester->GetCharName());
            return;
        }
    }
    requester->SendToGameDBPServer(&update_histories_cmd, update_histories_cmd.GetSize());
}