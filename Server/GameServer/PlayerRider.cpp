#include "stdafx.h"
#include "./PlayerRider.h"

#include <SCItemSlot.h>
#include <RidingInfoParser.h>
#include <SkillAttrCalculator.h>
//==================================================================================================

#include "PlayerWings.h"
#include "GameZone.h"
#include "GameZoneManager.h"
#include "ItemManager.h"
#include "InventorySlotContainer.h"
#include "./Status/StatusManager.h"

//==================================================================================================
//
// TODO : (091006) 몇몇 상투적으로 중복되는 부분에 대한 처리를 고민해 보자.
//==================================================================================================
//
#define REF_RIDER_DATA(var) \
    static_cast<const PlayerRider::Container&>(static_cast<const IPlayerRiderD::ContainerBase&>(var))

static const BYTE IPlayerRiderD_Null[sizeof(IPlayerRiderD)];

PlayerRider::PlayerRider()
    : delegator_((IPlayerRiderD&)IPlayerRiderD_Null) // dummy code
    , is_transaction_(0)
    , is_applied_option_(0)
{
    ClearRiderItemInfo();
    ClearGameControlInfo();
}

PlayerRider::PlayerRider(const IPlayerRiderD& r)
    : delegator_(r) // dummy code
    , is_transaction_(0)
    , is_applied_option_(0)
{
    ClearRiderItemInfo();
    ClearGameControlInfo();
}

PlayerRider::~PlayerRider()
{
    // release related resource
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    if(container.is_summoned == 0)
        return;

    GameField* game_field = container.player->GetField();
    if(game_field == 0)
        return;

    // (NOTE) current state description
    // 소환된 정보가 있다면, 

    StatusManager* pStatusManager = container.player->GetStatusManager();
    pStatusManager->ChangeInterState(eCHAR_STATE_RIDING_RIDER, false);

}

BOOLEAN
PlayerRider::GetRiderItemStream(RENDER_ITEMSTREAMEX* item_stream)
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    if(FlowControl::FCAssert(container.is_summoned != 0 && rider_item_ != 0)) {
        rider_item_->CopyOut(*item_stream);
        return false;
    }

    return true;
}

// NOTE: f110411.2L, added the interface to support spread wings control on riding
bool PlayerRider::CanSpreadWings(const SCItemSlot& riding_item) const
{
#if !defined(_NA001955_110210_WING_COSTUME)
    return false;
#endif
#ifdef _NA001955_110210_WING_COSTUME
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    if (container.is_summoned == false || rider_item_ == NULL) {
        return false;
    };
    if (rider_info_->flying_ == false) {
        return false;
    };
    if (rider_item_ != &riding_item) {
        return false;
    };

    return true;
#endif
}

bool PlayerRider::ChangeFlyingState(bool flied)
{
#if !defined(_NA001955_110210_WING_COSTUME)
    return false;
#endif
#ifdef _NA001955_110210_WING_COSTUME
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    if (container.is_summoned == false || rider_item_ == NULL) {
        return false;
    };
    if (rider_info_->flying_ == false) {
        return false;
    };

    assert(flied_ != flied);
    flied_ = flied;

    return true;
#endif

}

//==================================================================================================

PlayerRider::eResult
PlayerRider::Route(const RiderControl& req_info,
                   RiderControl* result, RiderControlData* result_data)
{
    BOOST_STATIC_ASSERT(eResult_Fail == 0);
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    assert(container.player != 0);
    eResult success = eResult_Fail;
    switch(req_info.request)
    {
    case req_info.eReq_SummonStart:
    case req_info.eReq_SummonCancel:
        success = SummonCasting(req_info, result, result_data);
        break;
    case req_info.eReq_Summon:
        success = Summon(req_info, result, result_data);
        break;
    case req_info.eReq_SummonBattleRelease: //전투를 위한 소환해제
    case req_info.eReq_SummonRelease:       //일반 소환 해제
        success = SummonRelease(req_info, result, result_data);
        break;
    case req_info.eReq_WaxGloss:
        success = WaxGloss(req_info, result, result_data, container.player);
        break;
    case req_info.eReq_EquipParts:
        success = Equip(req_info, result, result_data);
        break;
    case req_info.eReq_UnEquipParts:
        success = UnEquip(req_info, result, result_data);
        break;
    default:
        result->Clear();
        result_data->Clear();
        IPlayerRiderD::SetDefaultFailResult(req_info, result, result_data);
        success = eResult_Fail;
        break;
    }
    return success;
}

BOOLEAN
PlayerRider::CheckRule(const RiderControl& req_info,
                       RiderControl* result, RiderControlData* result_data, eResult* broad)
{
    __UNUSED(result_data);
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    assert(container.player != 0);

    *broad = eResult_Fail;

    const eCHAR_TYPE char_type = container.player->GetCharType();
    __UNUSED(char_type); // debug
    StatusManager* pStatusManager = container.player->GetStatusManager();

    BOOLEAN is_req_casting_start = req_info.request == req_info.eReq_SummonStart;
    BOOLEAN is_req_summon = is_req_casting_start == 0 &&
                            req_info.request == req_info.eReq_Summon;


    if (is_req_casting_start || is_req_summon)
    {
        // (CHANGES) (f100514.1L) add filter rule
        const StatusField& status_field = pStatusManager->Status;
        if (status_field.IsDragonTransforming() ||
            status_field.IsPlayer_TransOn() ||
            pStatusManager->CanSpecialAction() == false) // CHANGES: f110315.2L
        {
            // remove a status find code
            result->result = req_info.eRet_Failed;
            result->result_code = RC::RC_RIDER_FAILED;
            *broad = is_req_casting_start ? eResult_Fail : eResult_1toN;
            return false;
        }
    }

    return true;
}

// casting start|cancel
PlayerRider::eResult
PlayerRider::SummonCasting(const RiderControl& req_info,
                           RiderControl* result, RiderControlData* result_data)
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    assert(container.player != 0);
    result->Clear();
    result_data->Clear();

    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    if(container.is_summoned != 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_ALREADY_SUMMONED;
        return eResult_Fail;
    }

    if(container.player->IsDead()) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDSTATE;
        return eResult_Fail;
    }

    if(container.player->IsMoving()) {
        container.player->StopMoving();
    }

    eResult rule_result;
    if(CheckRule(req_info, result, result_data, &rule_result) == 0)
        return rule_result;

    BOOLEAN success = true;

    if(req_info.request == req_info.eReq_SummonStart) {
        if(container.state.state == req_info.eReq_SummonStart) {
            result->result = req_info.eRet_Failed;
            result->result_code = RC::RC_RIDER_SUMMON_CASTING;
            return eResult_Fail;
        }

        Container& container_edit = const_cast<Container&>(container);
        container_edit.state.request = req_info.eReq_SummonStart;
        container_edit.state.args = req_info.args;
        container_edit.casting_start = GetTickCount();
    }
    else if(req_info.request == req_info.eReq_SummonCancel) {
        if(container.state.state != req_info.eReq_SummonStart) {
            result->result = req_info.eRet_Failed;
            result->result_code = RC::RC_RIDER_FAILED;
            return eResult_Fail;
        }

        Container& container_edit = const_cast<Container&>(container);
        container_edit.state.state = req_info.eReq_None;
        container_edit.casting_start = 0;
    }
    else {
        success = false;
    }

    if(success) {
        result->result = req_info.eRet_Success;
        result_data->data_type = result_data->eDataType_Actor;
        RiderControlData::sActor& actor = result_data->actor;
        actor.requester = (WORD)container.player->GetObjectKey();

        ClearRiderItemInfo();
        return eResult_1toN;
    }

    return eResult_Fail;
}

PlayerRider::eResult
PlayerRider::Summon(const RiderControl& req_info,
                    RiderControl* result, RiderControlData* result_data)
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    assert(container.player != 0);
    result->Clear();
    result_data->Clear();

    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    if(container.state.state != req_info.eReq_SummonStart) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_FAILED;
        return eResult_Fail;
    }

    if(container.state.args.casting_start.slot_pos != req_info.args.summon.slot_pos) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_FAILED;
        return eResult_1toN;
    }

    // need state released
    const DWORD current_tick = GetTickCount();
    DWORD casting_start = container.casting_start;
    {
        Container& container_edit = const_cast<Container&>(container);
        container_edit.state.state = req_info.eReq_None;
        container_edit.casting_start = 0;
    }

    WORD player_key = (WORD)container.player->GetObjectKey();

    if(container.is_summoned != 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_ALREADY_SUMMONED;
        return eResult_Fail;
    }

    if(item_pos_ != INVALID_SLOT_POS || rider_item_ != 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED;
        result_data->data_type = result_data->eDataType_Actor;
        result_data->actor.requester = player_key;
        return eResult_1toN;
    }

    ePLAYER_BEHAVE_STATE behave = container.player->GetBehave();
    if(behave != PLAYER_BEHAVE_IDLE_STATE) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
        result_data->data_type = result_data->eDataType_Actor;
        result_data->actor.requester = player_key;
        return eResult_1toN;
    }

    if(container.player->IsDead()) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDSTATE;
        return eResult_1toN;
    }

    eResult rule_result;
    if(CheckRule(req_info, result, result_data, &rule_result) == 0)
        return rule_result;

    POSTYPE rider_item_pos = req_info.args.summon.slot_pos;
    // (NOTE) (100202) 변경된 제어룰. 인벤잠금 상태 고려할 것
    RiderItemQueryResult query_result = { 0, };
    RC::eITEM_RESULT item_result = GetRiderItem(rider_item_pos, &query_result);
    if(item_result != RC::RC_ITEM_SUCCESS) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = item_result;
        result_data->data_type = result_data->eDataType_Actor;
        result_data->actor.requester = player_key;
        return eResult_1toN;
    }
    SCItemSlot* rider_item = query_result.rider_item; // compatible interface
    // (NOTE) 게임서버의 제어가 벗어나 있는 상태에서 들어올 경우, 여기서 문제가 발생할 수 있다.
    // 어캐...해야 하나... 최소한 2개가 summoned로 설정되어 있다면... 음...
    if(rider_item->IsRiderSummoned() != 0) {
        if(is_transaction_) { // field move transaction by 'OnEnterField'
            // in GameServerControl
            rider_item->RiderSummonStatus(false); // no update
            is_transaction_ = false;
        }
        else if (query_result.inventory_lock) {
            result->result = req_info.eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
            return eResult_Fail;
        }
        else {
            result->result = req_info.eRet_Failed;
            result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED;
            return eResult_Fail;
        }
    }

    // check time expiration
    if(rider_item->IsExpiredWaitTime() != 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_TIME_EXPIRED;
        result_data->data_type = result_data->eDataType_Actor;
        result_data->actor.requester = player_key;
        return eResult_1toN;
    }
    if (rider_item->CanSummonStatus() == false) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_NOT_RIDER_ITEM;
        result_data->data_type = result_data->eDataType_Actor;
        result_data->actor.requester = player_key;
        return eResult_1toN;
    }

    const BASE_ITEMINFO* rider_item_info = rider_item->GetItemInfo();
    DWORD rider_info_index = rider_item_info->m_wSkillCode;

    RidingInfoParser* const pRidingInfoParser = RidingInfoParser::Instance();
    const sRiderInfo* rider_info = pRidingInfoParser->GetInfo(rider_info_index);
    if(rider_info == 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_CANT_FIND_INFO;
        result_data->data_type = result_data->eDataType_Actor;
        result_data->actor.requester = player_key;
        return eResult_1toN;
    }

    GameZone* game_zone = container.player->GetGameZonePtr();
    {   // (NOTE) game_zone이 null일 상황은 있을 수 없다. already check 'GetRiderItem'
        // (CHANGES) (WOPS:5607) 타락한 사원 내 소환 요청을 막는다.
        eZONETYPE zone_type = game_zone ? game_zone->GetZoneType() : eZONETYPE_MAX;
        if(rider_info->IsAcceptField(zone_type) == 0 ||
           !!(game_zone->Policyment.FIELD & game_zone->Policyment.ENABLE_SSQ_CONTROL))
        {
            result->result = req_info.eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_UNABLE_FUNCTION_FOR_CUR_ZONE;
            result_data->data_type = result_data->eDataType_Actor;
            result_data->actor.requester = player_key;
            return eResult_1toN;
        }
    };

    // casting 100초 후라 할 경우 약 88초부터 허용해 준다.
    casting_start += rider_info->spawn_timer_ - (rider_info->spawn_timer_ >> 3);
    if(current_tick < casting_start) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_SUMMON_CASTING;

        // 이 경우는 원래값 복원하자. 아,, 트랜젝션 이상해 질지도...
        //container_edit.casting_start = casting_start;
        //container_edit.state.state = req_info.eReq_SummonStart;
        result_data->data_type = result_data->eDataType_Actor;
        result_data->actor.requester = player_key;
        return eResult_1toN;
    }

    //-------------------------------------//
    //-------------- success --------------//
    // (WARNING) if success == 0 then
    //             need release resource
    BOOLEAN success = true;

    {// register item related info
        item_pos_ = rider_item_pos;
        rider_item_ = rider_item;
        rider_info_ = rider_info;
    };
    // (CHANGES) (WAVERIX) (091022) clear local field for game control
    this->ClearGameControlInfo();

    nsSlot::RiderUpdateBits update_bits = rider_item_->RiderSummonStatus(true);
    // (WARNING) 해당 기능은 transaction을 만족시켜야 한다. 적용 도중에 실패하면 대략 낭패
    if(success && ApplyOption(*rider_item_, update_bits) == 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED; // (NOTE) 에러를 재정의할 필요 있음.
        result_data->data_type = result_data->eDataType_Actor;
        result_data->actor.requester = player_key;

        success = false;
    }

    //---- success control ----//
    if(success) {
        // change flag
        Container& container_edit = const_cast<Container&>(container);
        container_edit.is_summoned = true;

        StatusManager* pStatusManager = container.player->GetStatusManager();
        pStatusManager->ChangeInterState(eCHAR_STATE_RIDING_RIDER, true);

        // notification
        result->result = req_info.eRet_Success;
        result_data->data_type = result_data->eDataType_ItemAction;
        RiderControlData::sItemAction& item_action = result_data->item_action;
        item_action.requester = player_key;
        item_action.update_bits = update_bits;
        rider_item_->CopyOut(item_action.item_slot);
        // change state
        //player_.SetBehave(PLAYER_BEHAVE_RIDING_STATE);

        return eResult_1toN;
    }

    //---- fail control ----//
    if(success == 0) {
        ClearRiderItemInfo();
    }

    return eResult_1toN;
}

PlayerRider::eResult
PlayerRider::SummonRelease(const RiderControl& req_info,
                           RiderControl* result, RiderControlData* result_data)
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    assert(container.player != 0);
    result->Clear();
    result_data->Clear();

    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    if(container.is_summoned == 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_ALREADY_SUMMON_RELEASED;
        return eResult_Fail;
    }

    if(item_pos_ == INVALID_SLOT_POS || rider_item_ == 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED;
        return eResult_Fail;
    }

    // ##problem##
    if(rider_item_->IsRiderSummoned() == 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_ALREADY_SUMMON_RELEASED;
        return eResult_Fail;
    }

    ePLAYER_BEHAVE_STATE behave = container.player->GetBehave();
    if(behave != PLAYER_BEHAVE_IDLE_STATE) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return eResult_Fail;
    }

    POSTYPE rider_item_pos = req_info.args.summon.slot_pos;
    if(item_pos_ != rider_item_pos) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return eResult_Fail;
    }

    GameField* pGameField = container.player->GetField();
    if(pGameField == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDSTATE;
        return eResult_Fail;
    }
    // no-check 'dead' state

    // (NOTE) 아이템 삭제 등등의 예기치 못한 이유가 발생시 // ##problem##위치에서부터
    // 문제를 야기할 수 있다. 따진다면 다른 곳에서도 유사 문제 발생할 수 있는데...
    // DeleteSlot 등에서의 문제를 고민해 보자. 일단은 정황상 캐시템을 삭제하면서까지 능력치를 올리는
    // 짱구들이 존재할 지도 모른다. 테섭이라면 특히 가능성 높다.
    // (NOTE) (100202) 변경된 제어룰. 인벤잠금 상태 고려할 것
    RiderItemQueryResult query_result = { 0, };
    RC::eITEM_RESULT item_result = GetRiderItem(rider_item_pos, &query_result);
    if(item_result != RC::RC_ITEM_SUCCESS) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = item_result;
        return eResult_Fail;
    }
    SCItemSlot* rider_item = query_result.rider_item; // compatible interface
    if(rider_item_ != rider_item) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED;
        return eResult_Fail;
    }
    //-------------------------------------//
    //-------------- success --------------//
#ifdef _NA001955_110210_WING_COSTUME
    if (this->flied_)
    {
        bool result = container.player->Wings.Command_Furl(IVehicleEvent::LinkEvent());
        assert(result);
        this->flied_ = false;
    }
#endif
    // (WARNING) 
    // 
    BOOLEAN success = true;

    nsSlot::RiderUpdateBits update_bits = rider_item_->RiderSummonStatus(false);
    // (WARNING) 해당 기능은 transaction을 만족시켜야 한다. 적용 도중에 실패하면 대략 낭패
    if(success && CancelOption(*rider_item_, update_bits) == 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED; // (NOTE) 에러를 재정의할 필요 있음.
        success = false;
        //...transaction이 보장된다는 전제에서는 그냥 에러 반환해도 될 거라 생각된다.
        return eResult_Fail;
    }

    if(success) {
        // change flag
        Container& container_edit = const_cast<Container&>(container);
        container_edit.is_summoned = false;

        StatusManager* pStatusManager = container.player->GetStatusManager();
        pStatusManager->ChangeInterState(eCHAR_STATE_RIDING_RIDER, false);

        // notification
        result->result = req_info.eRet_Success;
        result_data->data_type = result_data->eDataType_Actor;
        RiderControlData::sActor& actor = result_data->actor;
        actor.requester = (WORD)container.player->GetObjectKey();

        ClearRiderItemInfo();
        return eResult_1toN;
    }

    if(success == 0) {
        // nothing to do 'current'
    }

    return eResult_Fail;
}

PlayerRider::eResult
PlayerRider::Equip(const RiderControl& req_info,
                   RiderControl* result, RiderControlData* result_data)
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    assert(container.player != 0);
    result->Clear();
    result_data->Clear();

    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    const RiderControl::sReq_Equip& equip_info = req_info.args.equip;
    if(container.is_summoned == 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_ALREADY_SUMMON_RELEASED;
        return eResult_Fail;
    }

    ePLAYER_BEHAVE_STATE behave = container.player->GetBehave();
    if(behave != PLAYER_BEHAVE_IDLE_STATE) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return eResult_Fail;
    }

    ItemManager* item_manager = container.player->GetItemManager();
    if(item_manager->ValidState() == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDSTATE;
        return eResult_Fail;
    }

    if(item_manager->ValidPos(SI_INVENTORY, equip_info.from, true) == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return eResult_Fail;
    }

    CInventorySlotContainer* inventory =
        static_cast<CInventorySlotContainer*>(item_manager->GetItemSlotContainer(SI_INVENTORY));

    SCItemSlot& parts_item = static_cast<SCItemSlot&>(inventory->GetSlot(equip_info.from));
    if(parts_item.IsContainValidItem() == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDSTATE;
        return eResult_Fail;
    }
    if(parts_item.GetItemInfo()->IsRiderPartsItem() == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return eResult_Fail;
    }

    if(rider_item_->IsExpiredWaitTime() || parts_item.IsExpiredWaitTime()) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_TIME_EXPIRED;
        return eResult_Fail;
    }

    RC::eITEM_RESULT item_result = rider_item_->CanEquipRiderParts(parts_item);
    if(item_result != RC::RC_ITEM_SUCCESS) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = item_result;
        return eResult_Fail;
    }

    if(parts_item.OnEquip()) { // 처음 장착 여부이다.
        GAMELOG->LogItem(ITEM_CHARGE_FIRST_EQUIP, container.player, &parts_item);
    }

    SCItemSlot parts_item_out;
    if(inventory->DeleteSlot(equip_info.from, &parts_item_out) == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDSTATE;
        return eResult_Fail;
    }

    if(!FlowControl::FCAssert(rider_item_->EquipRiderParts(parts_item_out))) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED;
        return eResult_Fail;
    }

    nsSlot::RiderUpdateBits update_bits = rider_item_->GetRiderAttrChangedInfo();
    // update event
    ApplyOption(*rider_item_, update_bits);

    result->request = req_info.request;
    result->result = req_info.eRet_Success;
    result_data->data_type = result_data->eDataType_ItemAction;
    RiderControlData::sItemAction& item_action = result_data->item_action;
    item_action.requester = (WORD)container.player->GetObjectKey();
    item_action.update_bits = update_bits;
    rider_item_->CopyOut(item_action.item_slot);

    return eResult_1toN;
}

PlayerRider::eResult
PlayerRider::UnEquip(const RiderControl& req_info,
                     RiderControl* result, RiderControlData* result_data)
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    assert(container.player != 0);
    result->Clear();
    result_data->Clear();

    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    const RiderControl::sReq_UnEquip& unequip_info = req_info.args.unequip;

    // (NOTE) 장착 해제의 경우는 소환 여부와 상관 없이, 해당 라이더 아이템에서 해제할 수 있다.
    // case : 정액 만료된 라이더의 인벤토리를 열어 파츠를 추출한다.
    // can't use 'rider_item_'
    POSTYPE summoned_item_pos = INVALID_POSTYPE_VALUE;
    POSTYPE from_rider_item_pos = unequip_info.where;
    if(container.is_summoned != 0) {
        summoned_item_pos = rider_item_->GetPos();
        //POSTYPE summoned_item_pos = rider_item_->GetPos();
        //if(from_rider_item_pos != summoned_item_pos) {
        //    result->result = req_info.eRet_Failed;
        //    result->result_code = RC::RC_RIDER_MISMATCH_SUMMON_RIDER;
        //    return eResult_Fail;
        //}
    }

    if(container.player->GetField() == 0 || container.player->IsDead()) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDSTATE;
        return eResult_Fail;
    }

    ePLAYER_BEHAVE_STATE behave = container.player->GetBehave();
    if(behave != PLAYER_BEHAVE_IDLE_STATE) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return eResult_Fail;
    }

    // (NOTE) (100202) 변경된 제어룰. 인벤잠금 상태 고려할 것
    // can't use 'rider_item_'
    RiderItemQueryResult query_result = { 0, };
    RC::eITEM_RESULT item_result = GetRiderItem(from_rider_item_pos, &query_result);
    if(item_result != RC::RC_ITEM_SUCCESS) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = item_result;
        return eResult_Fail;
    }
    SCItemSlot* rider_item = query_result.rider_item; // compatible interface
    ItemManager* item_manager = container.player->GetItemManager();
    if(item_manager->ValidPos(SI_INVENTORY, unequip_info.to, false) == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return eResult_Fail;
    }

    nsSlot::RiderUpdateBits update_bits; // for sub option
    if(rider_item->CanUnEquipRiderParts(unequip_info.from, &update_bits) == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return eResult_Fail;
    }
    assert(update_bits.value == rider_item->GetRiderAttrChangedInfo().value);

    //----------------------------------------------------------------------------------------------
    // desire : must be sussess
    SCItemSlot rider_item_pre_unequiped = *rider_item;
    SCItemSlot parts_item;
    if(rider_item->UnEquipRiderParts(unequip_info.from, &parts_item) == 0) {
        ASSERT(!"Terrible");
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED;
        return eResult_Fail;
    }
    //위에서는 통과하고 하기 상황이 위반되는 경우는 기한 만료된 아이템의 경우다.
    //기한 만료는 ItemManager::Update에 의한 'Command_ItemSync'에 의해 변경되고 이미 스탯도 변경된다.
    // - 정리하면 다음과 같다. UnEquip:Success가 발생한 Rider아이템의 경우는 이미 parts가 추출된 상태
    //이기 때문에 업데이트에 문제가 생긴다. 또한 기한 만료가 발생하면서도 parts는 비활성 상태로 rider
    //인벤토리에 존재해야 하기 때문에 서로 다른 업데이트 형식이 발생하게 된다.
    // 따라서 UnEquip Process에는 두 단계의 서로 다른 UpdateBits가 발생할 수 있는 상황으로 구분해서
    //처리한다.
    // 
    //assert(update_bits.value == rider_item->GetRiderAttrChangedInfo().value);
    parts_item.OnUnEquip();

    CInventorySlotContainer* inventory =
        static_cast<CInventorySlotContainer*>(item_manager->GetItemSlotContainer(SI_INVENTORY));
    // 에러 처리 고민해 볼 것
    RC::eSLOT_INSERT_RESULT slot_result = inventory->InsertSlot(unequip_info.to, parts_item);
    if(slot_result != RC::RC_INSERT_SLOT_SUCCESS) {
        ASSERT(!"Terrible");
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;//unexpected
        return eResult_Fail;
    }

    if(summoned_item_pos == from_rider_item_pos)
        CancelOption(rider_item_pre_unequiped, update_bits);
    // desire : must be sussess
    //----------------------------------------------------------------------------------------------

    /*
    SCRiderPartsData parts_item_info;
    if(rider_item->GetRiderPartsInfo(unequip_info.from, &parts_item_info) == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return eResult_Fail;
    }

    if(parts_item_info.item_code_ == 0) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return eResult_Fail;
    }

    nsSlot::RiderUpdateBits update_bits_for_server = { 0, };
    if(summoned_item_pos == from_rider_item_pos) {
        // not cleared logic...
        BYTE parts_index = parts_item_info.parts_type_.value - parts_item_info.parts_type_.StartIndex;
        update_bits_for_server.value |= (1 << parts_index) << 5;
        CancelOption(update_bits_for_server);
    }

    nsSlot::RiderUpdateBits update_bits = rider_item->GetRiderAttrChangedInfo();

    CInventorySlotContainer* inventory =
        static_cast<CInventorySlotContainer*>(item_manager->GetItemSlotContainer(SI_INVENTORY));
    // 에러 처리 고민해 볼 것
    RC::eSLOT_INSERT_RESULT slot_result = inventory->InsertSlot(unequip_info.to, parts_item);
    if(slot_result != RC::RC_INSERT_SLOT_SUCCESS) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return eResult_Fail;
    }
    */

    result->request = req_info.request;
    result->result = req_info.eRet_Success;
    result_data->data_type = result_data->eDataType_ItemAction;
    RiderControlData::sItemAction& item_action = result_data->item_action;
    item_action.requester = (WORD)container.player->GetObjectKey();
    rider_item->CopyOut(item_action.item_slot);

    if(summoned_item_pos != from_rider_item_pos) {
        item_action.update_bits.value = 0;
        return eResult_1to1;
    }

    item_action.update_bits = update_bits;

    return eResult_1toN;
}

//==================================================================================================

PlayerRider::eResult
PlayerRider::WaxGloss(const RiderControl& req_info,
                      RiderControl* result, RiderControlData* result_data,
                      Player* player)
{
    //const Container& container = REF_RIDER_DATA(delegator_.container_);
    result->Clear();
    result_data->Clear();

    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    BOOLEAN success = true;

    if(player->GetField() == 0 || player->IsDead()) {
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDSTATE;
        return eResult_Fail;
    }

    const RiderControl::sReq_WaxGloss& wax_info = req_info.args.wax_gloss;
    ItemManager* item_manager = player->GetItemManager();
    if(success && (item_manager->ValidPos(SI_INVENTORY, wax_info.from_wax, TRUE) == 0 ||
                   item_manager->ValidPos(SI_INVENTORY, wax_info.to_parts, TRUE) == 0))
    {
        success = false;
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return eResult_Fail;
    }

    CInventorySlotContainer* inventory =
        static_cast<CInventorySlotContainer*>(item_manager->GetItemSlotContainer(SI_INVENTORY));

    SCItemSlot& from_wax_item = static_cast<SCItemSlot&>(inventory->GetSlot(wax_info.from_wax));
    SCItemSlot& to_parts_item = static_cast<SCItemSlot&>(inventory->GetSlot(wax_info.to_parts));
    const BASE_ITEMINFO* from_wax_info = from_wax_item.GetItemInfo();
    const BASE_ITEMINFO* to_parts_info = to_parts_item.GetItemInfo();
    if(success && (from_wax_info->m_wType != eITEMTYPE_WAX ||
                   to_parts_info->IsRiderPartsItem() == 0))
    {
        success = false;
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALID_TYPE;
        return eResult_Fail;
    }

    const WORD wax_enchant = from_wax_info->m_wSkillCode;
    // 정의된 값 없나?
    //if(success && (wax_enchant < 12 + 1) == 0)

    // Use이후 광택 성공시키기 위해 광택 낼 수 있는 지 선행 검사 필요함
    {
        WORD item_code_out = 0;
        RC::eITEM_RESULT ret = item_manager->Use(SI_INVENTORY, wax_info.from_wax, item_code_out);
        if(ret != RC::RC_ITEM_SUCCESS) {
            success = false;
            result->result = req_info.eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_INVALID_TYPE;
            return eResult_Fail;
        }
    };
    // ItemManager::Use에 의해 소진|삭제|이벤트 처리가 되었을 터... 나머지는 반드시 성공시켜야 하는
    // 파츠 광택 내기...
    to_parts_item.SetEnchant((BYTE)wax_enchant);    

    assert(success != 0);
    result->request = req_info.request;
    result->result = req_info.eRet_Success;
    result_data->data_type = result_data->eDataType_ItemAction;
    RiderControlData::sItemAction& item_action = result_data->item_action;
    item_action.requester = (WORD)player->GetObjectKey();
    to_parts_item.CopyOut(item_action.item_slot);

    return eResult_1to1;
}

//==================================================================================================

// (DESC) runtime check routine
BOOLEAN
PlayerRider::IsValidCurrentContainedItemSlot() const
{
    // (NOTE) 아직 어디까지 검증을 해야 확실하지 않는 관계로 가능한 정보들을 모두 추출해 둔다.
    // 확정로직일 경우 적절히 배제 시키기로 한다.
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    assert(container.player != 0);
    if(container.is_summoned == 0)
        return false;

    if(item_pos_ == INVALID_SLOT_POS || //assert(SI_INVENTORY)
       rider_item_ == 0 ||
       rider_info_ == 0)
    {
        return false;
    }

    if(rider_item_->IsContainValidItem() == 0)
        return false;

    POSTYPE item_pos = rider_item_->GetPos();
    if(item_pos != item_pos_)
        return false;

    // 이건... 시리얼 키를 받는 시점과 미묘하게 핀트가 어긋난다. 좀 더 좋은 방법을 강구할 것
    DBSERIAL serial_key = rider_item_->GetSerial();
    if(serial_key != rider_item_serial_)
        return false;

    ItemManager* item_manager = container.player->GetItemManager();
    if(item_manager->ValidPos(SI_INVENTORY, item_pos, TRUE) == 0)
        return false;

    CInventorySlotContainer* inventory =
        static_cast<CInventorySlotContainer*>(item_manager->GetItemSlotContainer(SI_INVENTORY));
    SCItemSlot& rider_item = static_cast<SCItemSlot&>(inventory->GetSlot(item_pos));
    if(&rider_item != rider_item_)
        return false;

    // no checked 'rider_item_->IsRiderSummoned()'
    if(rider_item_->IsRiderItem() == 0)
        return false;

    return true;
}


RC::eITEM_RESULT
PlayerRider::GetRiderItem(POSTYPE request_item_pos, RiderItemQueryResult* query_result)
{
    //assert player_ != 0
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    assert(query_result->rider_item == NULL);

    ItemManager* item_manager = container.player->GetItemManager();

    // (NOTE) item_manager->ValidState()는 IsDead도 검사한다. 이건 소환 해제시에는 문제 발생
    //if(item_manager->ValidState() == 0)
    //    return RC::RC_ITEM_INVALIDSTATE;
    if (container.player->GetField() == 0) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    { // validation test queried item by pos
        // to skip inventory lock
        // (NOTE) prevent inventory lock status open/close action in packet handler
        ItemManager::TransactionClosure transaction(item_manager,
            ItemManager::TransactionClosure::eTransType_RiderQuery);
        if (item_manager->ValidPos(SI_INVENTORY, request_item_pos, TRUE) == 0) {
            return RC::RC_ITEM_INVALIDPOS;
        }
        ProtectorInventory& protector = \
            container.player->GetWarehouseInventoryProtector().INVENTORY();
        if (protector.IsUsable() == false) {
            query_result->inventory_lock = true;
        }
    };

    CInventorySlotContainer* inventory = \
        static_cast<CInventorySlotContainer*>(item_manager->GetItemSlotContainer(SI_INVENTORY));

    SCItemSlot& rider_item = static_cast<SCItemSlot&>(inventory->GetSlot(request_item_pos));
    if (rider_item.IsContainValidItem() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (rider_item.IsRiderItem() == false) {
        return RC::RC_ITEM_INVALIDPOS;
    }

    const BASE_ITEMINFO* item_info = rider_item.GetItemInfo();
    RC::eITEM_RESULT result = container.player->CanUseItem(item_info);
    if (result != RC::RC_ITEM_SUCCESS) {
        return result;
    }
    query_result->rider_item = &rider_item;
    return RC::RC_ITEM_SUCCESS;
}

BOOLEAN
PlayerRider::OnChangedOption(const nsSlot::RiderUpdateBits& update_bits)
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    // 소환 
    PlayerAttributes& attributes = static_cast<PlayerAttributes&>(container.player->GetAttr());
    SkillAttrCalculator skill_calculator(attributes);
    bool is_pc_room = (container.player->CanApplyPCBangBenefit(true) != false);
    skill_calculator.ApplyRidingRider(false, is_pc_room, *rider_item_, update_bits);
    skill_calculator.ApplyRidingRider(true, is_pc_room, *rider_item_, update_bits);

    return true; // test code
}

BOOLEAN
PlayerRider::ApplyOption(const SCItemSlot& rider_item, const nsSlot::RiderUpdateBits& update_bits)
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);

    PlayerAttributes& attributes = static_cast<PlayerAttributes&>(container.player->GetAttr());
    SkillAttrCalculator skill_calculator(attributes); 
    bool is_pc_room = (container.player->CanApplyPCBangBenefit(true) != false);
    skill_calculator.ApplyRidingRider(true, is_pc_room, rider_item, update_bits);

    return true; // test code
}

BOOLEAN
PlayerRider::CancelOption(const SCItemSlot& rider_item, const nsSlot::RiderUpdateBits& update_bits)
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);

    PlayerAttributes& attributes = static_cast<PlayerAttributes&>(container.player->GetAttr());
    SkillAttrCalculator skill_calculator(attributes);
    bool is_pc_room = (container.player->CanApplyPCBangBenefit(true) != false);
    skill_calculator.ApplyRidingRider(false, is_pc_room, rider_item, update_bits);

    return true; // test code
}

//float
//PlayerRider::GetMoveSpeedRatio() const
//{
//    // TODO(WAVERIX) AttrValues를 이용한 적용이 필요하다.
//    return 1.0f;
//}

//==================================================================================================
//==================================================================================================
//==================================================================================================

IPlayerRiderD::~IPlayerRiderD()
{
    BOOST_STATIC_ASSERT(sizeof(IPlayerRiderD::ContainerExport) >= sizeof(PlayerRider::Container));
    REF_RIDER_DATA(container_); //compile-time checker
    if(container_.handler != 0)
    {
    }
}

void IPlayerRiderD::Init(Player* player)
{
    if(!FlowControl::FCAssert(player && container_.handler == 0))
        return;

    new (this) IPlayerRiderD;

    container_.handler = TAllocNew(PlayerRider);
    new (container_.handler) PlayerRider(*this);
    container_.player = player;
}

void IPlayerRiderD::Release()
{
    if(container_.handler != 0) {
        TAllocDelete(PlayerRider, container_.handler);
        container_.handler = 0;
    }

    new (this) IPlayerRiderD;
}

//==================================================================================================
// (NOTE) IPlayerRiderD::Update는 PlayerRider가 instance되지 않고 액션이 일어날 경우를 가정한 것이다.
// 많은 연산이 일어날 수 있으므로, PlayerRider.cpp에 구현해 둔다.
void IPlayerRiderD::Update(Player* player, DWORD delta_tick)
{
    __UNUSED(delta_tick);
    const DWORD MaxInterval = 20 * 1000;
    DWORD current_tick = GetTickCount();

    const PlayerRider::Container& container = REF_RIDER_DATA(container_);

    switch(container.state.state)
    {
    case container.state.eReq_SummonStart:
        assert(container.handler);
        break;
    default:
        return;
    };

    RiderControl cmd = { 0, };
    MSG_CG_ITEM_RIDER_CONTROL_BRD msgBRD;

    BOOLEAN to_client_notify = false;
    PlayerRider::eResult result = PlayerRider::eResult_Fail;
    switch(container.state.state)
    {
    case container.state.eReq_SummonStart:
        // casting 시작 후 MaxInterval(20sec)동안 소환 요청이 오지 않는다면 cancel시킨다.
        // for casting ani rendering synchronization
        if(container.casting_start + MaxInterval < current_tick) {
            cmd.request = cmd.eReq_SummonCancel;
            result = container.handler->SummonCasting(cmd, &msgBRD.header, &msgBRD.data);
            to_client_notify = true;
        }
        break;
    }

    if(to_client_notify) {
        if(result == PlayerRider::eResult_1toN)
            player->SendPacketAround(&msgBRD, msgBRD.GetSize(), true);
        else
            player->SendPacket(&msgBRD, msgBRD.GetSize());
    }
}

//--------------------------------------------------------------------------------------------------
// special commands : for field<->battle synchronization

// serializing from DBStream : first status
BOOLEAN
IPlayerRiderD::SyncPrepareRidingStatus(KEYTYPE enter_zone_key)
{
    const PlayerRider::Container& container = REF_RIDER_DATA(container_);
    if(!FlowControl::FCAssert(container.state.request == container.state.eReq_None &&
                              container.enter_state.request == container.enter_state.eReq_None &&
                              container.transaction_mode == container.eTrans_None))
    {
        return false;
    }

    PlayerRider::Container& container_edit = const_cast<PlayerRider::Container&>(container);
    container_edit.transaction_mode = container_edit.eTrans_SerializeStart;
    container_edit.transaction_zone_key = enter_zone_key;

    return true;
}

// serializing notification : OnUpdate
// (NOTE) important!!
// - 클라이언트로 보내지기 전의 서버쪽 인벤토리를 설정하는 시점에 호출되는 함수이며,
// 이곳에서 입장하는 존의 성격|라이더 정보등을 고려해 소환상태 유지|해제 등이 이뤄질 수 있도록 지원
// 한다. 아이템 스트림을 받는 시점(최초 입장|필드<->배틀|기타...)에 해당 스트림이 전송될 경우, 클라
// 이언트의 경우는 그 정보를 보고 소환 상태를 결정하고 능력치 계산이 이뤄진다.
BOOLEAN
IPlayerRiderD::SyncNotifyRiderSummon(const SCItemSlot& item_slot, Player* player)
{
    const SLOTIDX slot_idx = item_slot.GetSlotIdx();
    const POSTYPE rider_item_pos = item_slot.GetPos();
    if(slot_idx != SI_INVENTORY || rider_item_pos == INVALID_POSTYPE_VALUE)
        return false;

    const PlayerRider::Container& container = REF_RIDER_DATA(container_);
    if(container.enter_state.request == container.enter_state.eReq_Summon) {
        if(container.enter_state.args.summon.slot_pos == rider_item_pos)
            return true;
        return false;
    }

    BOOLEAN valid_check = item_slot.IsRiderItem() && item_slot.IsRiderSummoned();
    if(!FlowControl::FCAssert(valid_check != 0))
        return false;

    if(item_slot.IsExpiredWaitTime() != 0)
        return false;

    // (NOTE) 이후 로직은 입장 시점의 라이더 소환 유지 상태를 위한 것이므로 하기 조건을 이용해 필터
    // 링한다. transaction_mode != eTrans_SerializeStart인 상태는 장비 업데이트(+인벤)일 경우 발동할
    // 수 있다.
    if(container.transaction_mode != container.eTrans_SerializeStart)
        return true;

    // Check enter zone rule by RidingInfo
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    const sRiderInfo* rider_info = RidingInfoParser::Instance()->GetInfo(item_info->m_wSkillCode);
    if(!FlowControl::FCAssert(rider_info != 0))
        return false;

    GameZone* game_zone = g_GameZoneManager.GetZone(container.transaction_zone_key);
    if(game_zone == 0) // case : field released during db transaction : asserting logic
        return false;

    // if (summoned AND can't summon field) then change to summon released
    eZONETYPE zone_type = game_zone->GetZoneType();
    if(rider_info->IsAcceptField(zone_type) == 0) {
        // case study : exist two methods
        // #1: 1st enter field
        // #2: enter field events over than 1st
        ePLAYER_STATE player_state = (ePLAYER_STATE)player->GetState();
        if(player_state == PLAYER_CHARINFO_REQUESTED) {
            //irregular... 일단, serializing 상태이자 sync 명령으로 전환되었다.
            const_cast<SCItemSlot&>(item_slot).RiderSummonStatus(false);
            Command_ItemSync(player, item_slot);
        }
        return false;
    }

    assert(container.handler == 0 && this == &player->Rider);
    player->Rider.Init(player);

    PlayerRider::Container& container_edit = const_cast<PlayerRider::Container&>(container);
    container_edit.enter_state.request = container.enter_state.eReq_Summon;
    container_edit.enter_state.args.summon.slot_pos = rider_item_pos;

    // summon interface를 사용하기 위해 casting상태로 전환시킨다.
    container_edit.state.state = container.state.eReq_SummonStart;
    container_edit.state.args.casting_start.slot_pos = rider_item_pos;
    container_edit.casting_start = GetTickCount() + 60 * 60 * 1000; //update에서 취소되지 않도록...

    container_edit.transaction_mode = container_edit.eTrans_SyncRequested;

    return true;
}

// EnterField
BOOLEAN
IPlayerRiderD::SyncUpdateRidingStatus(Player* player)
{
    __UNUSED(player);
    const PlayerRider::Container& container = REF_RIDER_DATA(container_);
    //----------------------------------------------------------------------------------------------
    // call by OnEnterField
    if (container.is_summoned) {
        IVehicleEvent::OnEvent(player, IVehicleEvent::EnterField());
    }
    //----------------------------------------------------------------------------------------------
    BOOLEAN need_summon = container.enter_state.request == container.enter_state.eReq_Summon &&
                          container.state.request == container.enter_state.eReq_SummonStart;
    if(need_summon == 0)
        return true;

    PlayerRider::Container& container_edit = const_cast<PlayerRider::Container&>(container);
    container_edit.casting_start = GetTickCount() - 60 * 1000; //casting관련해서는 반드시 성공할 수 있도록

    if(!FlowControl::FCAssert(container.handler != 0))
        return false;

    // no notify to client - client has been do control 'standalone summon' (based on item stream info)
    RiderControl result;
    RiderControlData result_data;

    container.handler->is_transaction_ = true;
    PlayerRider::eResult ret = container.handler->Summon(container.enter_state, &result, &result_data);
    assert(ret != container.handler->eResult_Fail);
    if (container.is_summoned) {
        IVehicleEvent::OnEvent(player, IVehicleEvent::EnterField());
    };
    return true;
}

// assert 'IsValidState() != false'
BOOLEAN
IPlayerRiderD::Command_SummonRelease(const IVehicleEvent& event)
{
    const PlayerRider::Container& container = REF_RIDER_DATA(container_);
    if(container.is_summoned == 0)
        return false;

    if(!FlowControl::FCAssert(container_.handler != 0))
        return false;

    MSG_CG_ITEM_RIDER_CONTROL_BRD msgBRD;
    RiderControl cmd = { 0, };
    cmd.request = cmd.eReq_SummonRelease;
    cmd.args.summon.slot_pos = container.handler->item_pos_;
    PlayerRider::eResult result = container_.handler->SummonRelease(cmd, &msgBRD.header, &msgBRD.data);

    // event chain : on success control
    if(result != container.handler->eResult_Fail) {
        switch(event.event)
        {
        case event.eEvent_CancelByHit:
        case event.eEvent_CancelByDead:
            msgBRD.header.result_code = RC::RC_RIDER_SUMMON_RELEASED_BY_HIT;
            break;
        case event.eEvent_TimeExpired:
            msgBRD.header.result_code = RC::RC_RIDER_TIME_EXPIRED;
            break;
        }
    }

    Player* player = container.player;
    if(result == container.handler->eResult_1toN)
        player->SendPacketAround(&msgBRD, msgBRD.GetSize(), true);
    else
        player->SendPacket(&msgBRD, msgBRD.GetSize());

    // <- 로직이 꼬이는 것을 방지하기 위해서는 어떻게 해서든지 제거가 성공하고 싶다.
    // 조건을 좀 더 검토하고, 이것이 사용되는 위치에서 문제가 발생시 다음 턴 재시도, 또는 오류 복구
    // 가 이뤄질 수 있도록 고민해야 할 것이다.
    if(result == container.handler->eResult_Fail) {
        // NOTE: temporary log, check more detailed state
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=fails, ErrorCode=%d|"),
               __FUNCTION__, msgBRD.header.result_code);
        return false;
    }
    return true;
}

// (WARNING) ItemManager::_UpdateExpirationItem()에서 시간 만료에 의한 이벤트 (기간 연장 하지 않는
// 이상 단 한번)에 의해 호출된다. 다른 사례는 아직 찾지 못했다.
BOOLEAN
IPlayerRiderD::Command_ItemSync(Player* player, const SCItemSlot& changed_item)
{
    // 하기 opertion이 수행되면 GetRiderAttrChangedInfo의 결과값이 달라진다. 미리 얻어논다.
    nsSlot::RiderUpdateBits update_bits = changed_item.GetRiderAttrChangedInfo();
    if(!FlowControl::FCAssert(update_bits.value != 0))
        return false;

    const PlayerRider::Container& container = REF_RIDER_DATA(container_);

    BOOLEAN is_summoned_item = changed_item.IsRiderSummoned();
    POSTYPE item_pos = changed_item.GetPos();

    MSG_CG_ITEM_RIDER_CONTROL_BRD msgBRD;
    msgBRD.header.Clear();
    msgBRD.data.Clear();

    if(is_summoned_item) {
        assert(container.is_summoned != 0 && container.handler != 0);
        container.handler->CancelOption(changed_item, update_bits);
    }

    msgBRD.header.request = msgBRD.header.eCmd_SyncRider;
    msgBRD.header.args.sync.where = item_pos;
    msgBRD.data.data_type = msgBRD.data.eDataType_ItemAction;
    RiderControlData::sItemAction& item_action = msgBRD.data.item_action;
    item_action.requester = (WORD)player->GetObjectKey();
    item_action.update_bits = update_bits;
    changed_item.CopyOut(item_action.item_slot);

    // 기한 만료의 경우, 주변에 parts정보의 갱신이 필요할 지도 모른다.
    if(is_summoned_item) {
        player->SendPacketAround(&msgBRD, msgBRD.GetSize(), true);
    }
    else {
        player->SendPacket(&msgBRD, msgBRD.GetSize());
    }

    if(is_summoned_item) {
        if(update_bits.sub_rider)
            Command_SummonRelease(IVehicleEvent::TimeExpired());
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
// Battle Event
bool IPlayerRiderD::_OnEvent(const IVehicleEvent& event)
{
    const PlayerRider::Container& container = REF_RIDER_DATA(container_);
    // (NOTE) 추가 요청사항에 의해 eEvent_Hit는 두 가지로 구분된다.
    // 소환 상태 전 캐스팅 상태 | 소환된 이후 상태
    // 캐스팅 상태일 경우 캐스팅 취소
    // 소환 이후는 데미지 처리 방식에 따른 변환
    bool processed = false;
    bool released = false;
    if(event.event == event.eEvent_Hit) {
        if(container.handler == 0)
            return false;
        else if(container.state.state == container.state.eReq_SummonStart) {
            MSG_CG_ITEM_RIDER_CONTROL_BRD msgBRD;
            msgBRD.Clear();

            RiderControl req_info;
            req_info.Clear();
            req_info.request = req_info.eReq_SummonCancel;

            PlayerRider& rider = *container.handler;
            PlayerRider::eResult result = rider.Route(req_info, &msgBRD.header, &msgBRD.data);
            if(result == rider.eResult_1toN)
                container.player->SendPacketAround(&msgBRD, msgBRD.GetSize(), true);
            else
                container.player->SendPacket(&msgBRD, msgBRD.GetSize());

            return true;
        }
        else if(container.is_summoned == 0)
            return false;
        // (WARNING) 위 조건 검사 순서 중요...

        //
        // process normal hit events

        PlayerRider& rider = *container.handler;

        const DWORD current_tick = GetTickCount();
        const DWORD cacel_count_timer = rider.rider_info_->cancel_count_timer_;
        processed = true;

        //printf("|DEBUG-1st|Start=%d|CancelCount=%d|CancelTimeout=%u|\n",
        //       rider.hit_count_start_, rider.spawn_cancel_count_, rider.cancel_timeout_);

        IVehicleEvent post_effect = { post_effect.eEvent_None };
        if(rider.hit_count_start_ == 0) {
            // event start
            rider.hit_count_start_ = true;
            rider.spawn_cancel_count_ = rider.rider_info_->spawn_cancel_count_;
            rider.cancel_timeout_ = current_tick + cacel_count_timer;
        }

        if(rider.cancel_timeout_ < current_tick) {
            // timeout!
            if(rider.spawn_cancel_count_) { // < reset
                rider.spawn_cancel_count_ = rider.rider_info_->spawn_cancel_count_;
                rider.cancel_timeout_ = current_tick + cacel_count_timer;
            }
            else {
                post_effect.event = post_effect.eEvent_CancelByHit;
            }
        }
        else if(rider.spawn_cancel_count_ == 0) {
            post_effect.event = post_effect.eEvent_CancelByHit;
        }
        else if(container.player->IsDead()) {
            post_effect.event = post_effect.eEvent_CancelByDead;
        }

        //printf("|DEBUG-2nd|Start=%d|CancelCount=%d|CancelTimeout=%u|\n",
        //       rider.hit_count_start_, rider.spawn_cancel_count_, rider.cancel_timeout_);

        if(post_effect.event == post_effect.eEvent_None) {
            if(rider.spawn_cancel_count_ != 0)
                --rider.spawn_cancel_count_;
        }
        else {
            if(Command_SummonRelease(post_effect) != 0)
                released = true;
        }
    }
    else {
        // other events, except 'Hit'
        assert(container.is_summoned != 0 && container.handler != 0);

        PlayerRider& rider = *container.handler;
        switch(event.event)
        {
        case event.eEvent_EnterField:
            rider.hit_count_start_ = 0;
            rider.spawn_cancel_count_ = 0;
            rider.cancel_timeout_ = 0;
            processed = true;
            break;
        case event.eEvent_LeaveField:
            break;
        }
    }
    // if (release == true) then can't control rider

    return processed;
}

//==================================================================================================
// for GM interface
BOOLEAN
PlayerRider::ChangeToInstanceCasting() const
{
    const Container& container = REF_RIDER_DATA(delegator_.container_);
    Container& container_edit = const_cast<Container&>(container);
    container_edit.casting_start = 0;
    return true;
}


