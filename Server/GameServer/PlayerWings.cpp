#include "stdafx.h"
#include "./PlayerWings.h"

#include "Player.h"
#include "PlayerRider.h" // f110411.2L
#include "StatusManager.h"
#include "ItemManager.h"
#include "EquipmentSlotContainer.h"

//==================================================================================================

namespace ns_object {
;

static uint8_t IPlayerWingsD_NullStream[sizeof(IPlayerWingsD)] = { 0, };
#define IPlayerWingsD_NullRef() \
    reinterpret_cast<IPlayerWingsD&>(ns_object::IPlayerWingsD_NullStream)


}; //end of namespace

//==================================================================================================

void IPlayerWingsD::Init(Player* player)
{
#ifdef _NA001955_110210_WING_COSTUME
    if (player != NULL)
    {
        if (!FlowControl::FCAssert(container_.handler == NULL)) {
            return;
        }
        container_.handler = TAllocNew(ns_object::PlayerWings);
        new (container_.handler) ns_object::PlayerWings(*this, player);
    }
    else
    {
        if (container_.handler) {
            TAllocDelete(ns_object::PlayerWings, container_.handler);
        };
        new (this) IPlayerWingsD;
    }
#else
    new (this) IPlayerWingsD;
#endif
}

void IPlayerWingsD::Release()
{
#ifdef _NA001955_110210_WING_COSTUME
    if (container_.handler != NULL) {
        TAllocDelete(ns_object::PlayerWings, container_.handler);
    }
#endif
    new (this) IPlayerWingsD;
}

void IPlayerWingsD::Update(Player* player, const util::ServerTimeInfoPerFrame& time_info)
{
    __UNUSED((player, time_info));
}

bool IPlayerWingsD::_OnEvent(const IVehicleEvent& event)
{
    assert(container_.handler != NULL);
#ifdef _NA001955_110210_WING_COSTUME
    ns_object::PlayerWings* const wings = container_.handler;
    // CHANGES: f110525.8L, added the on dead event when player is hit.
    if (event.event == event.eEvent_Hit)
    {
        if (wings == NULL) {
            return false;
        }
        else if (wings->data_.spread_wings == false) {
            return false;
        };
        if (wings->data_.player->IsDead()) {
            IVehicleEvent post_effect = { post_effect.eEvent_None };
            return Command_Furl(event);
        };
    };
    // CHANGES: f110518.3L, removed the furl event when player is hit by others. planner's request
    if (event.event == event.eEvent_LinkEvent)
    {
        if (wings == NULL) {
            return false;
        }
        else if (wings->data_.spread_wings == false) {
            return false;
        };
        return Command_Furl(event);
    };
    // CHANGES: f110520.3L, the changes of equipments may occur wings controls
    if (event.event == event.eEvent_ChangeEquips)
    {
        if (wings == NULL) {
            return false;
        }
        else if (wings->data_.spread_wings == false) {
            return false;
        }
        else if (wings->data_.slot_index != SI_EQUIPMENT) {
            return false;
        };
        //------------------------------------------------------------------------------------------
        const SCItemSlot* known_wings_item = wings->data_.wings_item;
        const nsSlot::ItemRuleInfo& item_rule_info = known_wings_item->GetItemRuleInfo();
        const BASE_ITEMINFO* wings_item_info = known_wings_item->GetItemInfo();
        if (bool changed_to_others = (item_rule_info.is_valid_item == false ||
                                      wings_item_info->m_Code == 0 ||
                                      wings_item_info->m_wType != eITEMTYPE_WINGS))
        {   //       or empty
            Command_Furl(event);
            return true;
        };

        CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer*>(
            wings->data_.player->GetItemManager()->GetItemSlotContainer(SI_EQUIPMENT));
        if (bool inactivated = !equip_container->IsActivated(wings->data_.item_pos))
        {
            Command_Furl(event);
            return true;
        };
    };
#endif
    return true;
}

bool IPlayerWingsD::Command_Furl(const IVehicleEvent& event)
{
    using ns_object::PlayerWings;
    if (container_.handler == NULL) {
        return false;
    };
#ifdef _NA001955_110210_WING_COSTUME
    PlayerWings* const wings = container_.handler;
    Player* const player = wings->data_.player;
    if (wings->data_.spread_wings == false || player == NULL) {
        return false;
    }

    MSG_CG_SYNC_WINGS_CONTROL_CMD msg_cmd;
    WingControl header = { 0, };
    ;{
        header.request = header.eReq_FurlWings;
        header.args.furl.wings_pos = wings->data_.item_pos;
        header.args.furl.wings_container = wings->data_.slot_index;
    };

    PlayerWings::eResult notify_result = PlayerWings::eResult_Fail;
    bool success = wings->FurlWings(player,
        header, &msg_cmd.header, &msg_cmd.data, &notify_result);

    if (notify_result != PlayerWings::eResult_Fail)
    {
        switch (event.event)
        {
        case event.eEvent_Hit:
            msg_cmd.header.result = header.eRet_Success;
            msg_cmd.header.result_code = header.eRetCode_FurledByHit;
            break;
        case event.eEvent_TimeExpired:
            msg_cmd.header.result = header.eRet_Success;
            msg_cmd.header.result_code = header.eRetCode_TimeExpired;
            break;
        }
    }

    if (notify_result != ns_object::PlayerWings::eResult_1toN) {
        player->SendPacket(&msg_cmd, msg_cmd.GetSize());
    }
    else {
        player->SendPacketAround(&msg_cmd, msg_cmd.GetSize());
    }
#endif
    return true;
}

//==================================================================================================
#ifdef _NA001955_110210_WING_COSTUME

namespace ns_object {
;

PlayerWings::PlayerWings()
    : base_(IPlayerWingsD_NullRef())
{
    ZeroMemory(&data_, sizeof(data_));
}

PlayerWings::PlayerWings(const IPlayerWingsD& rhs, Player* player)
    : base_(rhs)
{
    ZeroMemory(&data_, sizeof(data_));
    data_.player = player;
}

PlayerWings::~PlayerWings()
{
    if (&IPlayerWingsD_NullRef() == &base_ ||
        data_.spread_wings == false ||
        data_.player == NULL)
    {
        return;
    };

    if (StatusManager* status_manager = data_.player->GetStatusManager()) {
        status_manager->ChangeInterState(eCHAR_STATE_SPREAD_WINGS, false);
    }
}

bool PlayerWings::SpreadWings(Player* player,
    const WingControl& req_info,
    WingControl* const result, WingControlData* const broad_result_data,
    eResult* const notify_result)
{
    result->Clear();
    broad_result_data->Clear();
    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    // CHANGES: f110323.2L, all results have actor that is requested by client
    const uint16_t player_key_16 = static_cast<uint16_t>(player->GetObjectKey());

    if (FlowControl::FCAssert(player != NULL) == false) {
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_Unknown;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_Fail;
        return false;
    }

    IPlayerWingsD& wings = player->Wings;
    bool validated = wings.IsValidState();
    if (validated == false) {
        wings.Init(player);
    }
    return wings.container_.handler->Spread(req_info, result, broad_result_data, notify_result);
}

bool PlayerWings::FurlWings(Player* player,
    const WingControl& req_info,
    WingControl* const result, WingControlData* const broad_result_data,
    eResult* const notify_result)
{
    result->Clear();
    broad_result_data->Clear();
    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    // CHANGES: f110323.2L, all results has actor that is requested by client
    const uint16_t player_key_16 = static_cast<uint16_t>(player->GetObjectKey());

    if (FlowControl::FCAssert(player != NULL) == false) {
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_Unknown;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    }

    IPlayerWingsD& wings = player->Wings;
    bool validated = wings.IsValidState();
    if (validated == false) {
        result->result = req_info.eRet_Success;
        result->result_code = req_info.eRetCode_AlreadyFurled;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    }
    bool success = wings.container_.handler->Furl(req_info, result, broad_result_data, notify_result);
    if (success) {
        wings.Init(NULL);
    }
    return success;
}


bool PlayerWings::Spread(const WingControl& req_info,
    WingControl* const result, WingControlData* const broad_result_data,
    eResult* const notify_result)
{
    assert(this != NULL && (&IPlayerWingsD_NullRef() != &base_));
    // CHANGES: f110323.2L, all results has actor that is requested by client
    const uint16_t player_key_16 = static_cast<uint16_t>(data_.player->GetObjectKey());

    if (data_.spread_wings != false)
    {
        result->result = req_info.eRet_Success;
        result->result_code = req_info.eRetCode_AlreadySpread;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    };
    if (data_.player->IsDead())
    {
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_InvalidCharState;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    }
    //----------------------------------------------------------------------------------------------
    enum { eSlot_Unknown = 0, eSlot_UseAccessory, eSlot_UseSummonedRidingItem };
    int check_type = eSlot_Unknown;
    ;{
        // CHANGES: f110411.2L, some of riding items have a function
        // that is enabled spread wings like the kalbus riding.
        const SLOTIDX slot_index = req_info.args.spread.wings_container;
        if (slot_index == SI_EQUIPMENT) {
            check_type = eSlot_UseAccessory;
        }
        else if (slot_index == SI_INVENTORY) {
            check_type = eSlot_UseSummonedRidingItem;
        };
        if (check_type == eSlot_Unknown)
        {
            result->result = req_info.eRet_Failed;
            result->result_code = req_info.eRetCode_Unknown;;
            broad_result_data->data_type = broad_result_data->eDataType_Actor;
            broad_result_data->actor.requester = player_key_16;
            *notify_result = eResult_1to1;
            return false;
        };
    };
    //
    SCItemSlot* flying_item = NULL;
    if (check_type == eSlot_UseAccessory) {
        if (SpreadDefault(req_info, result, broad_result_data,
                          notify_result, &flying_item) == false)
        {
            return false;
        };
    }
    else if (check_type == eSlot_UseSummonedRidingItem) {
        if (SpreadOnRiding(req_info, result, broad_result_data,
                           notify_result, &flying_item) == false)
        {
            return false;
        };
    };
    // at this point, do brutely success process. in case of SpreadOnRiding,
    // already set flying to riding object
    ;{  // success process, change states
        data_.spread_wings = true;
        data_.slot_index = req_info.args.spread.wings_container;
        data_.item_pos = req_info.args.spread.wings_pos;
        data_.wings_item = flying_item;

        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        if (StatusManager* const status_manager = data_.player->GetStatusManager()) {
            status_manager->ChangeInterState(eCHAR_STATE_SPREAD_WINGS, true);
        };
    };

    *notify_result = eResult_1toN;
    return true;
}

bool PlayerWings::SpreadDefault(const WingControl& req_info,
    WingControl* const result, WingControlData* const broad_result_data,
    eResult* const notify_result, SCItemSlot** flying_item)
{
    const uint16_t player_key_16 = static_cast<uint16_t>(data_.player->GetObjectKey());

    CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer*>(
        data_.player->GetItemManager()->GetItemSlotContainer(req_info.args.spread.wings_container));
    ;{
        // TODO: use transaction
        if (equip_container->ValidState() == false ||
            equip_container->ValidPos(req_info.args.spread.wings_pos) == false)
        {
            assert(false);
            result->result = req_info.eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_INVALIDPOS;
            broad_result_data->data_type = broad_result_data->eDataType_Actor;
            broad_result_data->actor.requester = player_key_16;
            *notify_result = eResult_1to1;
            return false;
        }
    };

    StatusManager* const status_manager = data_.player->GetStatusManager();
    // use wings item
    if (status_manager->CanSpecialAction() == false)
    {
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_CantDuplicateAction;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    }

    // NOTE: f110411.2L, the selected item is changed process selection
    // whether the item in equipments container or it in inventory queried since f110411.2L.
    // so that the variable of 'wings_item' change to 'flying_item'.
    SCItemSlot& wings_item = static_cast<SCItemSlot&>(
        equip_container->GetSlot(req_info.args.spread.wings_pos));
    // use wings item
    // NOTE: check item whether is wings
    const nsSlot::ItemRuleInfo& item_rule_info = wings_item.GetItemRuleInfo();
    const BASE_ITEMINFO* wings_item_info = wings_item.GetItemInfo();
    if (item_rule_info.is_valid_item == false ||
        wings_item_info->m_wType != eITEMTYPE_WINGS)
    {
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_NotWingsItem;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    };
    // CHANGES: f110520.3L
    if (equip_container->IsActivated(req_info.args.spread.wings_pos) == false)
    {
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_Inactivated;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    }
    //
    wings_item.SetUserBlocked(true);
    *flying_item = &wings_item;
    //
    return true;
}

bool PlayerWings::SpreadOnRiding(const WingControl& req_info,
    WingControl* const result, WingControlData* const broad_result_data,
    eResult* const notify_result, SCItemSlot** flying_item)
{
    const uint16_t player_key_16 = static_cast<uint16_t>(data_.player->GetObjectKey());

    CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer*>(
        data_.player->GetItemManager()->GetItemSlotContainer(req_info.args.spread.wings_container));
    ;{
        // TODO: use transaction
        if (equip_container->ValidState() == false ||
            equip_container->ValidPos(req_info.args.spread.wings_pos) == false)
        {
            assert(false);
            result->result = req_info.eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_INVALIDPOS;
            broad_result_data->data_type = broad_result_data->eDataType_Actor;
            broad_result_data->actor.requester = player_key_16;
            *notify_result = eResult_1to1;
            return false;
        }
    };

    StatusManager* const status_manager = data_.player->GetStatusManager();
    // use riding item that enabled to fly
    if (status_manager->CanSpecialAction(eCHAR_STATE_RIDING_RIDER) == false)
    {
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_CantDuplicateAction;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    }

    // NOTE: f110411.2L, the selected item is changed process selection
    // whether the item in equipments container or it in inventory queried since f110411.2L.
    // so that the variable of 'wings_item' change to 'flying_item'.
    SCItemSlot& riding_item = static_cast<SCItemSlot&>(
        equip_container->GetSlot(req_info.args.spread.wings_pos));
    // use riding item that enabled to fly
    if (data_.player->Rider.IsValidState() == false) {
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_InvalidCharState;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    };
    //
    PlayerRider& player_rider = data_.player->Rider.Get();
    if (player_rider.CanSpreadWings(riding_item) == false) {
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_NotToFlyingItem;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    };
    // success
    player_rider.ChangeFlyingState(true);
    *flying_item = &riding_item;
    //
    return true;
}

bool PlayerWings::Furl(const WingControl& req_info,
    WingControl* const result, WingControlData* const broad_result_data,
    eResult* const notify_result)
{
    assert(this != NULL && (&IPlayerWingsD_NullRef() != &base_));
    // CHANGES: f110323.2L, all results has actor that is requested by client
    const uint16_t player_key_16 = static_cast<uint16_t>(data_.player->GetObjectKey());

    if (data_.spread_wings == false) {
        result->result = req_info.eRet_Success;
        result->result_code = req_info.eRetCode_AlreadyFurled;
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
        *notify_result = eResult_1to1;
        return false;
    };

    // TODO: check player state
    // NOTE: f110411.2L,
    if (data_.slot_index == SI_INVENTORY) // using riding item
    {
        if (FlowControl::FCAssert(data_.player->Rider.IsValidState()))
        {
            PlayerRider& player_rider = data_.player->Rider.Get();
            player_rider.ChangeFlyingState(false);
        }
    }
    else if (FlowControl::FCAssert(data_.slot_index == SI_EQUIPMENT))
    {   // NOTE: f110520.3L
        const nsSlot::ItemRuleInfo& item_rule_info = data_.wings_item->GetItemRuleInfo();
        const BASE_ITEMINFO* wings_item_info = data_.wings_item->GetItemInfo();
        if (bool expected_wings_item = ((item_rule_info.is_valid_item != false) &&
                                        (wings_item_info->m_Code != 0) ||
                                        (wings_item_info->m_wType == eITEMTYPE_WINGS)))
        {
            data_.wings_item->SetUserBlocked(false);
        };
    };

    ;{
        data_.spread_wings = false;

        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key_16;
    };
    if (StatusManager* status_manager = data_.player->GetStatusManager()) {
        status_manager->ChangeInterState(eCHAR_STATE_SPREAD_WINGS, false);
    }

    *notify_result = eResult_1toN;
    return true;
}

}; //end of namespace

#endif //_NA001955_110210_WING_COSTUME
//==================================================================================================
