#include "stdafx.h"
#include "./PerkManager.h"

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_

#include "ItemManager.h"

#include <PacketStruct_DG.h>
#include <SkillSystem/PerkInfoParser.h>
#include <SCItemSlot.h>
#include <SCItemSlotContainer.h>

#ifdef _NA_001990_ACSYSTEM_ADD
#include "AchievementManager.h"
#endif //_NA_001990_ACSYSTEM_ADD

//==================================================================================================

namespace nsSkill {
;

struct PerkControlMessengerInGame : public nsSkill::IPerkControlMessenger,
                                    public nsSkill::IPerkControlEvent
{
    PerkControlMessengerInGame(Player* player)
        : player_(player),
          attributes_(&player->GetAttr())
    {}
    //--------------------------------------------------------------
    // interfaces nsSkill::IPerkControlMessenger
    virtual SCPerkSlotContainer* GetPerkSlotContainer() {
        return NULL;
    }
    virtual Attributes* GetOwnerAttributes() {
        return attributes_;
    }
    virtual nsSkill::IPerkControlEvent* GetPerkControlEvent() {
        return this;
    }
    //--------------------------------------------------------------
    // interfaces for nsSkill::IPerkControlEvent
    virtual void OnChangeAttributes() {
        attributes_->UpdateEx();
    }
    virtual void OnChangeSlotContainer() {
        // nothing to do
    }
    virtual void OnLevelup() {
        // nothing to do
    }
    //

#ifdef _NA_001990_ACSYSTEM_ADD
    virtual void OnLevelup(const PerkID& perk_id)
    {
        if (AchievementManager* achievement = player_->GetACManager()) {
            achievement->ProcessPerkLevel(perk_id.group_, perk_id.level_);
        };
        // CHANGES: f110602.3L
        LEVELTYPE prev_level = (perk_id.level_ > 1) ? perk_id.level_ - 1 : perk_id.level_;
        GAMELOG->WritePerkLevelUp(player_, perk_id.group_, prev_level, perk_id.level_);
    }
#endif //

    Player* player_;
    Attributes* attributes_;
};

struct RollbackRequestByFailed
{
    RollbackRequestByFailed(const PerkControl* result_info, PerkControl* modify_info_if_failed)
        : result_info_(result_info), modify_info_if_failed_(modify_info_if_failed)
    {
    }
    ~RollbackRequestByFailed()
    {
        if (result_info_->result != result_info_->eRet_Success) {
            modify_info_if_failed_->Clear();
        }
    }
    //
    const PerkControl* result_info_;
    PerkControl* modify_info_if_failed_;
};

//--------------------------------------------------------------------------------------------------

PerkManager::PerkManager()
    : perk_data_container_(new nsSkill::PerkDataContainer)
{
    ZeroMemory(&inter_, sizeof(inter_));
}

PerkManager::~PerkManager()
{
    if (perk_data_container_) {
        delete perk_data_container_;
    }
    ZeroMemory(&inter_, sizeof(inter_));
}

void PerkManager::Reuse(Player* player)
{
    ZeroMemory(&inter_, sizeof(inter_));
    inter_.player = player;
    perk_data_container_->Clear();
    update_expiration_date_timer_.Stop();
    update_status_timer_.Stop();
}

bool PerkManager::CanProcessState(PerkControl* header, PerkControlData* data)
{
    GameField* game_field = inter_.player->GetField();
    if (game_field == NULL) {
        return false;
    };
    //
    ePLAYER_BEHAVE_STATE behave = inter_.player->GetBehave();
    if (behave != PLAYER_BEHAVE_IDLE_STATE
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        //_NA_000000_20140402_MODIFY_PERKMANAGER_POSSIBLE_TO_USE_WHILE_BATTLE_GROUND_RESERVATION
        && behave != PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE
#endif
        ) 
    {
        header->result = header->eRet_ItemFailed;
        header->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return false;
    }
    return true;
}

bool PerkManager::CanProcessState()
{
	GameField* game_field = inter_.player->GetField();
	if (game_field == NULL) {
		return false;
	};
	//
	ePLAYER_BEHAVE_STATE behave = inter_.player->GetBehave();
	if (behave != PLAYER_BEHAVE_IDLE_STATE
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
		//_NA_000000_20140402_MODIFY_PERKMANAGER_POSSIBLE_TO_USE_WHILE_BATTLE_GROUND_RESERVATION
		&& behave != PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE
#endif
		) 
	{
		return false;
	}
	return true;
}

// NOTE: the perk system experience rule, add only battle experience value.
bool PerkManager::AddExp(int acquired_exp)
{
    if (acquired_exp == 0) {
        return false;
    };
    inter_.acquired_experience += acquired_exp;
    return true;
}

void PerkManager::Update(const util::ServerTimeInfoPerFrame* time_info_per_frame)
{
    GameField* game_field = inter_.player->GetField();
    if (game_field == NULL) {
        // to support client synchronization
        return;
    };
    if (update_expiration_date_timer_.IsExpired()) {
        UpdateExpirationDate(time_info_per_frame);
    }
    //if (inter_.acquired_experience)
    if (update_status_timer_.IsExpired()) {
        UpdateStatus(time_info_per_frame);
    }
}

bool PerkManager::UpdateExpirationDate(const util::ServerTimeInfoPerFrame* time_info_per_frame)
{
    //
    PerkDataContainer::InterData* const inter_data = &perk_data_container_->inter_;
    const DWORD64& curr_time_wz = time_info_per_frame->wz_yyyymmddhhmmss_;
    const PerkDataNode** cash_it = inter_data->cash_slots_,
                      ** cash_end = &inter_data->cash_slots_[_countof(inter_data->cash_slots_)];
    const PerkDataNode* cash_node = NULL;
    for ( ; cash_it != cash_end; ++cash_it)
    {
        // NOTE: one deletion per one time
        if (*cash_it == NULL) {
            continue;
        };
        cash_node = *cash_it;
        if (curr_time_wz <= cash_node->exp_) {
            continue;
        }
        // expired perk
        break;
    }
    const bool need_deletion = (cash_it != cash_end && cash_node);
    if (need_deletion == false) {
        return true;
    }

    // delete operation
    MSG_CG_SKILL_PERK_CONTROL_CMD msg_deletion;
    //
    const PerkID deletion_perk_id = cash_node->perk_id_;
    PerkControlMessengerInGame perk_messenger(inter_.player);
    const bool success_deletion = perk_data_container_->RequestDeletion(&msg_deletion.header,
                                                                        deletion_perk_id,
                                                                        &perk_messenger);
    if (success_deletion == false) {
        return true;
    }
    ;{  // send a deletion operation to the DB proxy server
        MSG_DG_CHARINFO_SKILL_PERK_CONTROL_CMD msg_cmd;
        PerkControl* req_header = &msg_cmd.header;
        req_header->Clear();
        req_header->request = req_header->eReq_DeleteS2S;
        req_header->char_guid = inter_.player->GetCharGuid();
        req_header->args.delete_s2s.perk_uid = deletion_perk_id.uid_;
        msg_cmd.data.Clear();
        inter_.player->SendToGameDBPServer(&msg_cmd, msg_cmd.GetSize());
    };
    perk_data_container_->ProcessDelete(msg_deletion.header, &msg_deletion.data, &perk_messenger);
    inter_.player->SendPacket(&msg_deletion, msg_deletion.GetSize());

    return true;
}

// CHANGES: f101108.3L, to support GM commmand.
bool PerkManager::ChangePerkLevel(int changed_level)
{
    GameField* game_field = inter_.player->GetField();
    if (game_field == NULL) {
        // to support client synchronization
        return false;
    };
    if (changed_level == 0) {
        return true;
    };
    PerkDataContainer::TouchList touch_list;
    if (perk_data_container_->ModifyExpOrLevel(changed_level, true, &touch_list) == false) {
        return true; // true? false? hummm....
    };
    // CHANGES: f110602.3L
    for (const PerkDataContainer::TouchList::check_type* it = touch_list.touches;
         it != &touch_list.touches[_countof(touch_list.touches)];
         ++it)
    {
        if (*it != touch_list.Selected) {
            continue;
        };
        const PerkDataContainer::TouchList::ChangedInfo* changed_info = touch_list.changed_infos;
        GAMELOG->LogExpPerk(inter_.player, changed_info->perk_group,
            changed_info->next_exp, changed_info->prev_exp);
    };
    return ProcessStatus(touch_list);
}

bool PerkManager::UpdateStatus(const util::ServerTimeInfoPerFrame* time_info_per_frame)
{
    __UNUSED(time_info_per_frame);
    if (inter_.acquired_experience == 0) {
        return true;
    };
    PerkDataContainer::TouchList touch_list;
    if (perk_data_container_->ModifyExpOrLevel(inter_.acquired_experience, false,
                                               &touch_list) == false)
    {
        return true; // true? false? hummm....
    }
    // CHANGES: f110602.3L
    for (const PerkDataContainer::TouchList::check_type* it = touch_list.touches;
         it != &touch_list.touches[_countof(touch_list.touches)];
         ++it)
    {
        if (*it != touch_list.Selected) {
            continue;
        };
        const PerkDataContainer::TouchList::ChangedInfo* changed_info = touch_list.changed_infos;
        GAMELOG->LogExpPerk(inter_.player, changed_info->perk_group,
            changed_info->next_exp, changed_info->prev_exp);
    };
    return ProcessStatus(touch_list);
}

bool PerkManager::ProcessStatus(const PerkDataContainer::TouchList& touch_list)
{
    //
    MSG_CG_SKILL_PERK_STATUS_CMD msg_cmd;
    PerkControl* const header = &msg_cmd.header;
    ;{  // init data
        header->Clear();
        header->request = header->eReq_Status;
    };
    PerkStatusData* const data = &msg_cmd.data;
    data->Clear();
    data->data_type = data->eDataType_ExpUpdate;
    //
    PerkDataContainer::InterData* const inter_data = &perk_data_container_->inter_;

    PerkStatusData::StatusList* const status_list = &data->status_list;
    PerkStatusData::__Status* dest_it = status_list->nodes_;
    //
    for (int index = 0; index < _countof(touch_list.touches); ++index)
    {
        if (touch_list.touches[index] != touch_list.Selected) {
            continue;
        }
        const PerkDataNode* active_node = inter_data->active_slots_[index];
        if (active_node == NULL) {
            assert(active_node);
            continue;
        };
        ;{  // align data
            dest_it->index_ = static_cast<uint8_t>(index);
            dest_it->level_ = static_cast<uint8_t>(active_node->perk_id_.level_);
            dest_it->exp_ = active_node->exp_;
            //
            ++dest_it;
        };
    };
    //
    status_list->number_of_nodes_ = static_cast<uint8_t>(dest_it - status_list->nodes_);
    inter_.acquired_experience = 0; // flush hit events
    if (status_list->number_of_nodes_ == 0) {
        // it's possible, if any perk is not registered.
        return true;
    };
    // NOTE: this section process level up events.
    // if a client part, it will be processed in ProcessEventMessage.
    ;{  // NOTE: level up event handling
        PerkControlMessengerInGame perk_messenger(inter_.player);
        perk_data_container_->ProcessEvent(msg_cmd.header, &msg_cmd.data, &perk_messenger);
    };
    // last process
    inter_.player->SendPacket(&msg_cmd, msg_cmd.GetSize());
    //
    return true;
}

//==================================================================================================

void PerkManager::StorePerk(const PerkDataList& perk_data_list, DWORD char_key)
{
#ifdef _NA_007743_20141027_ADD_ISVAILD_CHECK
    if (obj_util::IsValid(inter_.player) == false)
    {
        return;
    }
#endif // _NA_007743_20141027_ADD_ISVAILD_CHECK

    perk_data_container_->set_pass_key(char_key);
    PerkControlMessengerInGame perk_messenger(inter_.player);
    perk_data_container_->StorePerk(perk_data_list, &perk_messenger);
    perk_data_container_->ApplyAttributes(&perk_messenger);
}

void PerkManager::LoadPerk(PerkDataList* perk_data_list)
{
    perk_data_container_->LoadPerk(perk_data_list);
}

// CHANGES: f110727.2L, fixed a missing stopped timer resume event
void nsSkill::PerkManager::OnEnterField(GameField* game_field)
{
    __TOUCHED(game_field);
    update_expiration_date_timer_.SetTimer(kUpdateExpirationDateInterval);
    update_status_timer_.SetTimer(kUpdateStatusInterval);
}

PerkManager::eRequestResult
PerkManager::ProcessRequestedMessage(const PerkControl& req_info,
                                     PerkControl* header, PerkControlData* data)
{
    header->Clear();
    data->Clear();
    *header = req_info;
    //
    bool success = false;
    switch (req_info.request)
    {
    case req_info.eReq_Register:
        return ProcessRegister(req_info, header, data);
    case req_info.eReq_Plants:
        return ProcessPlants(req_info, header, data);
    case req_info.eReq_Uproot:
        return ProcessUproot(req_info, header, data);
    case req_info.eReq_Unregister:
        return ProcessUnregister(req_info, header, data);
    //-----------------------------
    // operations on server
    case req_info.eReq_InsertS2S:
        {
            const PerkControl::Req_InsertS2S& insert_s2s = req_info.args.insert_s2s;
            insert_s2s.perk_uid;
            // TODO : this logic, currently, is not implemented at this point.
            // need more think related to a synchronous or asynchronous flow.
            // related the article : eReq_Plants
        };
        break;
    case req_info.eReq_DeleteS2S:
        {
            const PerkControl::Req_DeleteS2S& delete_s2s = req_info.args.delete_s2s;
            delete_s2s.perk_uid;
            // TODO : this logic, currently, is not implemented at this point.
            // need more think related to a synchronous or asynchronous flow.
            // related the article : eReq_Uproot
        };
        break;
    }
    //
    return this->eReqResult_None;
}

PerkManager::eRequestResult
PerkManager::ProcessRegister(const PerkControl& req_info,
                             PerkControl* header, PerkControlData* data)
{
    if (CanProcessState(header, data) == false) {
        return this->eReqResult_ReplyResult;
    }
    bool success = false;
    assert(req_info.request == req_info.eReq_Register);

    const PerkControl::Req_Register& regist = req_info.args.regist;
    regist.perk_uid;
    regist.active_slot_uid;
    PerkControlMessengerInGame perk_messenger(inter_.player);
    success = perk_data_container_->RequestRegister(\
        header, PerkID(regist.perk_uid), PerkID(regist.active_slot_uid), &perk_messenger);
    if (success == false) {
        return this->eReqResult_ReplyResult;
    }
    ASSERT(regist.perk_uid == header->args.regist.perk_uid);
    ASSERT(regist.active_slot_uid == header->args.regist.active_slot_uid);
    //
    RollbackRequestByFailed rollback_request(header, &perk_data_container_->inter_.request_info_);
    //
    success = perk_data_container_->ProcessReplyMessage(*header, data, &perk_messenger);
    ASSERT(success);
    //
    return this->eReqResult_ReplyResult;
}

PerkManager::eRequestResult
PerkManager::ProcessUnregister(const PerkControl& req_info,
                               PerkControl* header, PerkControlData* data)
{
    bool success = false;
    assert(req_info.request == req_info.eReq_Unregister);

    const PerkControl::Req_Unregister& unregist = req_info.args.unregist;
    unregist.perk_uid;
    PerkControlMessengerInGame perk_messenger(inter_.player);
    success = perk_data_container_->RequestUnregister(\
        header, PerkID(unregist.perk_uid), &perk_messenger);
    if (success == false) {
        return this->eReqResult_ReplyResult;
    }
    ASSERT(unregist.perk_uid == header->args.unregist.perk_uid);
    //
    RollbackRequestByFailed rollback_request(header, &perk_data_container_->inter_.request_info_);
    //
    success = perk_data_container_->ProcessReplyMessage(*header, data, &perk_messenger);
    ASSERT(success);

    return this->eReqResult_ReplyResult;
}

PerkManager::eRequestResult
PerkManager::ProcessPlants(const PerkControl& req_info,
                           PerkControl* header, PerkControlData* data)
{
    if (CanProcessState(header, data) == false) {
        return this->eReqResult_ReplyResult;
    }
    //
    bool success = false;
    assert(req_info.request == req_info.eReq_Plants);

    // a plant is an action that a cash item is moved to a cash perk slot
    const PerkControl::Req_Plants& plants = req_info.args.plants;
    plants.from_item_pos;
    // check the inventory of a player related to 'plants.from_item_pos'
    ItemManager* const item_manager = inter_.player->GetItemManager();
    if (item_manager->ValidPos(SI_INVENTORY, plants.from_item_pos, true) == false) {
        header->result = header->eRet_ItemFailed;
        header->result_code = RC::RC_ITEM_INVALIDPOS;
        return this->eReqResult_ReplyResult;
    };
    //
    SCSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& plants_item = static_cast<SCItemSlot&>(inventory->GetSlot(plants.from_item_pos));
    const BASE_ITEMINFO* plants_item_info = plants_item.GetItemInfo();
    if (plants_item.IsContainValidItem() == false ||
        plants_item_info->m_wType != eITEMTYPE_PERK_CASH_ITEM)
    {
        header->result = header->eRet_ItemFailed;
        header->result_code = RC::RC_ITEM_INVALID_TYPE;
        return this->eReqResult_ReplyResult;
    }
    if (plants_item.IsLocked() || plants_item.IsBlocked())
    {
        header->result = header->eRet_ItemFailed;
        header->result_code = RC::RC_ITEM_INVALIDPOS;
        return this->eReqResult_ReplyResult;
    }
    //
    const SLOTCODE perk_group_code = plants_item_info->m_wSkillCode;
    const PerkID perk_id(perk_group_code, 1);
    const PerkInfoParser* perk_parser = PerkInfoParser::Instance();
    /*
    const PerkGroup* perk_group_info = perk_parser->FindPerkGroup(perk_id);
    if (perk_group_info == NULL) {
        header->result = header->eRet_SkillFailed;
        header->result_code = RC::RC_SKILL_CANT_FIND_INFO;
        return this->eReqResult_ReplyResult;
    }
    const PerkInfo* perk_info = perk_group_info->perk_info_array_[0];
    */
    const PerkInfo* perk_info = perk_parser->FindPerkInfo(perk_id);
    assert(perk_info);
    if (perk_info == NULL || perk_info->item_code_ == 0 ||
        perk_info->perk_id_.uid_ != perk_id.uid_)
    {
        header->result = header->eRet_SkillFailed;
        header->result_code = RC::RC_SKILL_CANT_FIND_INFO;
        return this->eReqResult_ReplyResult;
    };
    const PerkDataNode* perk_data = perk_data_container_->FindPerkData(perk_id);
    if (perk_data != NULL) {
        header->result = header->eRet_SkillFailed;
        header->result_code = RC::RC_SKILL_ALREADY_EXIST_SKILL;
        return this->eReqResult_ReplyResult;
    };
    // process with data modification
    const bool is_enable_timer = plants_item.GetItemRuleInfo().IsEnableTimer();
    assert(is_enable_timer);
    if (plants_item.IsProgressTime() == false) {
        plants_item.FireUp_DateTime();
    }
    DWORD64 expiration_date = plants_item.GetDateTime();
    //
    PerkControlMessengerInGame perk_messenger(inter_.player);
    success = perk_data_container_->RequestPlants(header, plants.from_item_pos, &perk_messenger);
    if (success == false) {
        if (header->result == header->eRet_Success) {
            header->result = header->eRet_Failed;
            header->result_code = header->eRetCode_Unknown;
        }
        return this->eReqResult_ReplyResult;
    }
    ASSERT(plants.from_item_pos == header->args.plants.from_item_pos);
    //
    RollbackRequestByFailed rollback_request(header, &perk_data_container_->inter_.request_info_);
    //
    ;{
        // next task: if the result is success, do configure the 'data' variable.
        data->data_type = data->eDataType_Plants;
        data->plants.perk_uid = perk_id.uid_;
        data->plants.expiration_date = expiration_date; // f101228.2L
    };
    ;{  // use a cash perk regist item
        SLOTCODE result_item_code = 0;
        RC::eITEM_RESULT result = \
            item_manager->Use(SI_INVENTORY, plants.from_item_pos, result_item_code);
        if (result != RC::RC_ITEM_SUCCESS) {
            header->result = header->eRet_ItemFailed;
            header->result_code = RC::RC_ITEM_INVALID_TYPE;
            return this->eReqResult_ReplyResult;
        }
    };
    //
    success = perk_data_container_->ProcessReplyMessage(*header, data, &perk_messenger);
    assert(success);
#ifdef SUN_CODE_BACKUP // f101228.2L
    ;{  // RESOLVED: configure additional data using 'expiration_date'
        const nsSkill::PerkDataNode* perk_data_node = perk_data_container_->FindPerkData(perk_id);
        nsSkill::PerkDataNode* perk_data_node_edit = \
            const_cast<nsSkill::PerkDataNode*>(perk_data_node);
        perk_data_node_edit->exp_ = expiration_date;
    };
#endif
    return this->eReqResult_ReplyResult;
}

PerkManager::eRequestResult
PerkManager::ProcessUproot(const PerkControl& req_info,
                           PerkControl* header, PerkControlData* data)
{
    if (CanProcessState(header, data) == false) {
        return this->eReqResult_ReplyResult;
    }
    //
    bool success = false;
    assert(req_info.request == req_info.eReq_Uproot);

    // an uproot is an action that a cash perk slot is moved to an item on an inventory
    const PerkControl::Req_Uproot& uproot = req_info.args.uproot;
    uproot.perk_uid;
    uproot.to_item_pos;

    //
    const PerkID uproot_perk_id(uproot.perk_uid);
    const PerkInfoParser* const perk_parser = PerkInfoParser::Instance();
    const PerkInfo* perk_info = perk_parser->FindPerkInfo(uproot_perk_id);
    if (perk_info == NULL) {
        header->result = header->eRet_SkillFailed;
        header->result_code = RC::RC_SKILL_CANT_FIND_INFO;
        return this->eReqResult_ReplyResult;
    };
    //
    if (perk_info->item_code_ == 0) {
        header->result = header->eRet_ItemFailed;
        header->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return this->eReqResult_ReplyResult;
    };
    const ItemInfoParser* item_parser = ItemInfoParser::Instance();
    const BASE_ITEMINFO* perk_item_info = item_parser->GetItemInfo(perk_info->item_code_);
    if (perk_item_info == NULL || perk_item_info->m_wType != eITEMTYPE_PERK_CASH_ITEM) {
        header->result = header->eRet_ItemFailed;
        header->result_code = (perk_item_info == NULL) ? RC::RC_ITEM_NOINFO
                                                       : RC::RC_ITEM_UNUSABLE_FUNCTION;
        return this->eReqResult_ReplyResult;
    }
    // check the inventory of a player related to 'uproot.to_item_pos'
    ItemManager* const item_manager = inter_.player->GetItemManager();
    if (item_manager->ValidPos(SI_INVENTORY, uproot.to_item_pos, false) == false) {
        header->result = header->eRet_ItemFailed;
        header->result_code = RC::RC_ITEM_INVALIDPOS;
        return this->eReqResult_ReplyResult;
    };
    //
    SCSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    const SCItemSlot& empty_slot = static_cast<SCItemSlot&>(inventory->GetSlot(uproot.to_item_pos));
    if (empty_slot.GetItemCode() != 0) {    //...maybe implicit resolved by a ValidPos
        header->result = header->eRet_ItemFailed;
        header->result_code = RC::RC_ITEM_INVALIDPOS;
        return this->eReqResult_ReplyResult;
    };
    if (empty_slot.IsLocked() || empty_slot.IsBlocked()) {
        header->result = header->eRet_ItemFailed;
        header->result_code = RC::RC_ITEM_INVALIDSTATE;
        return this->eReqResult_ReplyResult;
    };
    //
    PerkControlMessengerInGame perk_messenger(inter_.player);
    //
    success = perk_data_container_->RequestUproot(header, \
        uproot_perk_id, uproot.to_item_pos, &perk_messenger);
    if (success == false) {
        return this->eReqResult_ReplyResult;
    }
    ASSERT(uproot.perk_uid == header->args.uproot.perk_uid);
    ASSERT(uproot.to_item_pos == header->args.uproot.to_item_pos);
    //
    RollbackRequestByFailed rollback_request(header, &perk_data_container_->inter_.request_info_);
    //
    // NOTE: r100914.6L, to get the expiration time of a perk node,
    // you try to access the low level data container
    nsSkill::PerkDataNode* const uprooting_node = \
        perk_data_container_->server_data_.uproot.uprooting_node;
    const DWORD64 expiration_date = uprooting_node->exp_;
    //
    // NOTE: send a perk data stream to a DBP & send a delete operation to a DBP
    success = perk_data_container_->ProcessReplyMessage(*header, data, &perk_messenger);
    assert(success);
    //
    ;{  // send a deletion operation to the DB proxy server
        MSG_DG_CHARINFO_SKILL_PERK_CONTROL_CMD msg_cmd;
        PerkControl* req_header = &msg_cmd.header;
        req_header->Clear();
        req_header->request = req_header->eReq_DeleteS2S;
        req_header->char_guid = inter_.player->GetCharGuid();
        req_header->args.delete_s2s.perk_uid = uproot_perk_id.uid_;
        msg_cmd.data.Clear();
        inter_.player->SendToGameDBPServer(&msg_cmd, msg_cmd.GetSize());
    };
    //
    SCItemSlot making_perk_item;
    making_perk_item.SetCode(perk_item_info->m_Code);
    ;{
        // NOTE: r100914.6L, get and set time information
        // if the uprooting node has the expiration date information.
        assert(making_perk_item.IsContainValidItem() &&
               making_perk_item.GetItemRuleInfo().IsEnableTimer());
        making_perk_item.SetFixedDateTime(expiration_date);
        making_perk_item.SetProgressTime(true);
    };
    FlowControl::FCAssert(RC::RC_INSERT_SLOT_SUCCESS ==
                          inventory->InsertSlot(uproot.to_item_pos, making_perk_item));
    assert(&empty_slot == &inventory->GetSlot(uproot.to_item_pos));
    SCItemSlot& perk_item_in_inventory = const_cast<SCItemSlot&>(empty_slot);
    //
    data->data_type = data->eDataType_Uproot;
    perk_item_in_inventory.CopyOut(data->uproot.maked_item);
    //
    return this->eReqResult_ReplyResult;
}

}; //end of namespace

//==================================================================================================

#endif //_NA002217_100728_EP2_PERK_SYSTEM_
