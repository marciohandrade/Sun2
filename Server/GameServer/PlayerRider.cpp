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
// TODO : (091006) ��� ���������� �ߺ��Ǵ� �κп� ���� ó���� ����� ����.
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
    // ��ȯ�� ������ �ִٸ�, 

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
    case req_info.eReq_SummonBattleRelease: //������ ���� ��ȯ����
    case req_info.eReq_SummonRelease:       //�Ϲ� ��ȯ ����
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
    // (NOTE) (100202) ����� �����. �κ���� ���� ����� ��
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
    // (NOTE) ���Ӽ����� ��� ��� �ִ� ���¿��� ���� ���, ���⼭ ������ �߻��� �� �ִ�.
    // ��ĳ...�ؾ� �ϳ�... �ּ��� 2���� summoned�� �����Ǿ� �ִٸ�... ��...
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
    {   // (NOTE) game_zone�� null�� ��Ȳ�� ���� �� ����. already check 'GetRiderItem'
        // (CHANGES) (WOPS:5607) Ÿ���� ��� �� ��ȯ ��û�� ���´�.
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

    // casting 100�� �Ķ� �� ��� �� 88�ʺ��� ����� �ش�.
    casting_start += rider_info->spawn_timer_ - (rider_info->spawn_timer_ >> 3);
    if(current_tick < casting_start) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_SUMMON_CASTING;

        // �� ���� ������ ��������. ��,, Ʈ������ �̻��� ������...
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
    // (WARNING) �ش� ����� transaction�� �������Ѿ� �Ѵ�. ���� ���߿� �����ϸ� �뷫 ����
    if(success && ApplyOption(*rider_item_, update_bits) == 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED; // (NOTE) ������ �������� �ʿ� ����.
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

    // (NOTE) ������ ���� ����� ����ġ ���� ������ �߻��� // ##problem##��ġ��������
    // ������ �߱��� �� �ִ�. �����ٸ� �ٸ� �������� ���� ���� �߻��� �� �ִµ�...
    // DeleteSlot ����� ������ ����� ����. �ϴ��� ��Ȳ�� ĳ������ �����ϸ鼭���� �ɷ�ġ�� �ø���
    // ¯������ ������ ���� �𸥴�. �׼��̶�� Ư�� ���ɼ� ����.
    // (NOTE) (100202) ����� �����. �κ���� ���� ����� ��
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
    // (WARNING) �ش� ����� transaction�� �������Ѿ� �Ѵ�. ���� ���߿� �����ϸ� �뷫 ����
    if(success && CancelOption(*rider_item_, update_bits) == 0) {
        result->result = req_info.eRet_Failed;
        result->result_code = RC::RC_RIDER_UNEXPECTED_FAILED; // (NOTE) ������ �������� �ʿ� ����.
        success = false;
        //...transaction�� ����ȴٴ� ���������� �׳� ���� ��ȯ�ص� �� �Ŷ� �����ȴ�.
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

    if(parts_item.OnEquip()) { // ó�� ���� �����̴�.
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

    // (NOTE) ���� ������ ���� ��ȯ ���ο� ��� ����, �ش� ���̴� �����ۿ��� ������ �� �ִ�.
    // case : ���� ����� ���̴��� �κ��丮�� ���� ������ �����Ѵ�.
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

    // (NOTE) (100202) ����� �����. �κ���� ���� ����� ��
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
    //�������� ����ϰ� �ϱ� ��Ȳ�� ���ݵǴ� ���� ���� ����� �������� ����.
    //���� ����� ItemManager::Update�� ���� 'Command_ItemSync'�� ���� ����ǰ� �̹� ���ȵ� ����ȴ�.
    // - �����ϸ� ������ ����. UnEquip:Success�� �߻��� Rider�������� ���� �̹� parts�� ����� ����
    //�̱� ������ ������Ʈ�� ������ �����. ���� ���� ���ᰡ �߻��ϸ鼭�� parts�� ��Ȱ�� ���·� rider
    //�κ��丮�� �����ؾ� �ϱ� ������ ���� �ٸ� ������Ʈ ������ �߻��ϰ� �ȴ�.
    // ���� UnEquip Process���� �� �ܰ��� ���� �ٸ� UpdateBits�� �߻��� �� �ִ� ��Ȳ���� �����ؼ�
    //ó���Ѵ�.
    // 
    //assert(update_bits.value == rider_item->GetRiderAttrChangedInfo().value);
    parts_item.OnUnEquip();

    CInventorySlotContainer* inventory =
        static_cast<CInventorySlotContainer*>(item_manager->GetItemSlotContainer(SI_INVENTORY));
    // ���� ó�� ����� �� ��
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
    // ���� ó�� ����� �� ��
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
    // ���ǵ� �� ����?
    //if(success && (wax_enchant < 12 + 1) == 0)

    // Use���� ���� ������Ű�� ���� ���� �� �� �ִ� �� ���� �˻� �ʿ���
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
    // ItemManager::Use�� ���� ����|����|�̺�Ʈ ó���� �Ǿ��� ��... �������� �ݵ�� �������Ѿ� �ϴ�
    // ���� ���� ����...
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
    // (NOTE) ���� ������ ������ �ؾ� Ȯ������ �ʴ� ����� ������ �������� ��� ������ �д�.
    // Ȯ�������� ��� ������ ���� ��Ű��� �Ѵ�.
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

    // �̰�... �ø��� Ű�� �޴� ������ �̹��ϰ� ��Ʈ�� ��߳���. �� �� ���� ����� ������ ��
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

    // (NOTE) item_manager->ValidState()�� IsDead�� �˻��Ѵ�. �̰� ��ȯ �����ÿ��� ���� �߻�
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
    // ��ȯ 
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
//    // TODO(WAVERIX) AttrValues�� �̿��� ������ �ʿ��ϴ�.
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
// (NOTE) IPlayerRiderD::Update�� PlayerRider�� instance���� �ʰ� �׼��� �Ͼ ��츦 ������ ���̴�.
// ���� ������ �Ͼ �� �����Ƿ�, PlayerRider.cpp�� ������ �д�.
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
        // casting ���� �� MaxInterval(20sec)���� ��ȯ ��û�� ���� �ʴ´ٸ� cancel��Ų��.
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
// - Ŭ���̾�Ʈ�� �������� ���� ������ �κ��丮�� �����ϴ� ������ ȣ��Ǵ� �Լ��̸�,
// �̰����� �����ϴ� ���� ����|���̴� �������� ����� ��ȯ���� ����|���� ���� �̷��� �� �ֵ��� ����
// �Ѵ�. ������ ��Ʈ���� �޴� ����(���� ����|�ʵ�<->��Ʋ|��Ÿ...)�� �ش� ��Ʈ���� ���۵� ���, Ŭ��
// �̾�Ʈ�� ���� �� ������ ���� ��ȯ ���¸� �����ϰ� �ɷ�ġ ����� �̷�����.
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

    // (NOTE) ���� ������ ���� ������ ���̴� ��ȯ ���� ���¸� ���� ���̹Ƿ� �ϱ� ������ �̿��� ����
    // ���Ѵ�. transaction_mode != eTrans_SerializeStart�� ���´� ��� ������Ʈ(+�κ�)�� ��� �ߵ���
    // �� �ִ�.
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
            //irregular... �ϴ�, serializing �������� sync ������� ��ȯ�Ǿ���.
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

    // summon interface�� ����ϱ� ���� casting���·� ��ȯ��Ų��.
    container_edit.state.state = container.state.eReq_SummonStart;
    container_edit.state.args.casting_start.slot_pos = rider_item_pos;
    container_edit.casting_start = GetTickCount() + 60 * 60 * 1000; //update���� ��ҵ��� �ʵ���...

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
    container_edit.casting_start = GetTickCount() - 60 * 1000; //casting�����ؼ��� �ݵ�� ������ �� �ֵ���

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

    // <- ������ ���̴� ���� �����ϱ� ���ؼ��� ��� �ؼ����� ���Ű� �����ϰ� �ʹ�.
    // ������ �� �� �����ϰ�, �̰��� ���Ǵ� ��ġ���� ������ �߻��� ���� �� ��õ�, �Ǵ� ���� ����
    // �� �̷��� �� �ֵ��� ����ؾ� �� ���̴�.
    if(result == container.handler->eResult_Fail) {
        // NOTE: temporary log, check more detailed state
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=fails, ErrorCode=%d|"),
               __FUNCTION__, msgBRD.header.result_code);
        return false;
    }
    return true;
}

// (WARNING) ItemManager::_UpdateExpirationItem()���� �ð� ���ῡ ���� �̺�Ʈ (�Ⱓ ���� ���� �ʴ�
// �̻� �� �ѹ�)�� ���� ȣ��ȴ�. �ٸ� ��ʴ� ���� ã�� ���ߴ�.
BOOLEAN
IPlayerRiderD::Command_ItemSync(Player* player, const SCItemSlot& changed_item)
{
    // �ϱ� opertion�� ����Ǹ� GetRiderAttrChangedInfo�� ������� �޶�����. �̸� �����.
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

    // ���� ������ ���, �ֺ��� parts������ ������ �ʿ��� ���� �𸥴�.
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
    // (NOTE) �߰� ��û���׿� ���� eEvent_Hit�� �� ������ ���еȴ�.
    // ��ȯ ���� �� ĳ���� ���� | ��ȯ�� ���� ����
    // ĳ���� ������ ��� ĳ���� ���
    // ��ȯ ���Ĵ� ������ ó�� ��Ŀ� ���� ��ȯ
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
        // (WARNING) �� ���� �˻� ���� �߿�...

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


