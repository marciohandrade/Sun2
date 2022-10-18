#include "stdafx.h"
#include "./EtherealizeImpl.h"
//
#include <SCItemSlot.h>
#include <SCItemSlotContainer.h>
//
#include "ItemManager.h"

//==================================================================================================

namespace nsSlot {
;

// implemented by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_

void EtheriaImpl::SetInstance(Player* player, EtheriaControlOption* control_option)
{
    player_ = player;
    control_option_ = control_option;
}

EtheriaImpl::eResult EtheriaImpl::Casting(const EtheriaControl& req_info,
                                          EtheriaControl* const result,
                                          EtheriaControlData* const broad_result_data)
{
    assert(req_info.request == req_info.eReq_CastingStart ||
           req_info.request == req_info.eReq_CastingCancel);
    result->Clear();
    broad_result_data->Clear();
    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    const WORD player_key = static_cast<WORD>(player_->GetObjectKey());
    const DWORD tick_count = GetTickCount();
    bool pass = true;
    ;{
        ePLAYER_BEHAVE_STATE behave = player_->GetBehave();
        if (behave != PLAYER_BEHAVE_IDLE_STATE)
        {
            pass = false;
            broad_result_data->data_type = broad_result_data->eDataType_Actor;
            broad_result_data->actor.requester = player_key;
            result->result = req_info.eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
            return this->eResult_Fail;
        }
    };
    //
    if (pass == false) {
    }
    else if (req_info.request == req_info.eReq_CastingStart)
    {
        if (control_option_->doing_casting_) {
            // fault tolerant
            pass = (control_option_->timeout_ < tick_count);
        }
        if (control_option_->doing_casting_ && pass == false)
        {
            broad_result_data->data_type = broad_result_data->eDataType_Actor;
            broad_result_data->actor.requester = player_key;
            result->result = req_info.eRet_Failed;
            result->result_code = req_info.eRetCode_CastingToggleError;
            return this->eResult_1to1;
        }
        else
        {
            control_option_->doing_casting_ = true;
            control_option_->timeout_ = tick_count +
                                        (req_info.kCastingDelay - (req_info.kCastingDelay >> 2));
            broad_result_data->data_type = broad_result_data->eDataType_Actor;
            broad_result_data->actor.requester = player_key;
            result->result = req_info.eRet_Success;
            return this->eResult_1toN;
        }
    }
    else if (req_info.request == req_info.eReq_CastingCancel)
    {
        if (control_option_->doing_casting_)
        {
            control_option_->doing_casting_ = false;
            control_option_->timeout_ = 0;
            broad_result_data->data_type = broad_result_data->eDataType_Actor;
            broad_result_data->actor.requester = player_key;
            result->result = req_info.eRet_Success;
            return this->eResult_1toN;
        }
        else
        {
            broad_result_data->data_type = broad_result_data->eDataType_Actor;
            broad_result_data->actor.requester = player_key;
            result->result = req_info.eRet_Failed;
            result->result_code = req_info.eRetCode_CastingToggleError;
            return this->eResult_1to1;
        }
    }
    ;{
        broad_result_data->data_type = broad_result_data->eDataType_Actor;
        broad_result_data->actor.requester = player_key;
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_Unknown;
        return this->eResult_Fail;
    };
}

EtheriaImpl::eResult EtheriaImpl::ExtractToEtheria(const EtheriaControl& req_info,
                                                   EtheriaControl* const result,
                                                   EtheriaControlData* const result_data)
{
    //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_
    const bool kUseEtherealizer = true;
    const int kDefaultExtractOperation = 0; // BASE_ITEMINFO::m_byWasteType == 0, redirected value
    //
    assert(req_info.request == req_info.eReq_Extract);
    result->Clear();
    result_data->Clear();
    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    bool success = true;
    RC::eITEM_RESULT item_result = RC::RC_ITEM_SUCCESS;
    //...
    const EtheriaControl::Req_Extract& extract_info = req_info.args.extract;
    //
    ePLAYER_BEHAVE_STATE behave = player_->GetBehave();
    if (behave != PLAYER_BEHAVE_IDLE_STATE)
    {
        success = false;
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return this->eResult_Fail;
    }
    // (f100526.5L) add casting presentation
    if (this->control_option_->doing_casting_ == false)
    {
        success = false;
        result->result = req_info.eRet_Failed;
        result->result_code = req_info.eRetCode_ExtractNeedCasting;
        return this->eResult_Fail;
    }
    this->control_option_->doing_casting_ = false;
    //
    ItemManager* const item_manager = player_->GetItemManager();

    if (item_manager->ValidPos(SI_INVENTORY, extract_info.slot_pos, true) == false ||
        (kUseEtherealizer &&
         item_manager->ValidPos(SI_INVENTORY, extract_info.etherealizer_pos, true) == false))
    {
        success = false;
        result->result = result->eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return this->eResult_Fail;
    }
    SCSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    //
    SCItemSlot* etherealizer = NULL;
    const BASE_ITEMINFO* etherealizer_info = NULL;
    if (kUseEtherealizer)
    {
        etherealizer = static_cast<SCItemSlot*>(&inventory->GetSlot(extract_info.etherealizer_pos));
        if (etherealizer->IsContainValidItem() == false ||
            (etherealizer_info = etherealizer->GetItemInfo())->m_wType != eITEMTYPE_ETHEREALIZER)
        {
            success = false;
            result->result = result->eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_INVALID_TYPE;
            return this->eResult_Fail;
        }
        if (etherealizer->IsLocked() || etherealizer->IsBlocked())
        {
            success = false;
            result->result = result->eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_INVALIDPOS;
            return this->eResult_Fail;
        }
        // eWASTETYPE = { 0, eITEMWASTE_ETHEREALIZER_SHELL, eITEMWASTE_ETHEREALIZER_ETHERIA };
        assert(etherealizer_info);
        switch (etherealizer_info->m_byWasteType)
        {
        case kDefaultExtractOperation:
        //_NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
        case eITEMWASTE_ETHEREALIZER_SHELL:
        case eITEMWASTE_ETHEREALIZER_ETHERIA:
            {
                // passed, notthing to do
            };
            break;
        default:
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|Msg=unexpected an etherealizer's waste type|")
                   _T("|ItemCode=%d|WasteType=%d|"),
                   etherealizer_info->m_Code, etherealizer_info->m_byWasteType);
            //
            success = false;
            result->result = result->eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_INVALID_TYPE;
            return this->eResult_Fail;
        }; //end 'switch'
    };
    //
    const int extract_operation = etherealizer_info ? etherealizer_info->m_byWasteType
                                                    : kDefaultExtractOperation;
    //
    SCItemSlot& extract_item = static_cast<SCItemSlot&>(inventory->GetSlot(extract_info.slot_pos));
    if (extract_item.CanExtractToEtheria(&item_result) == false) // need result variation
    {
        success = false;
        result->result = result->eRet_ItemFailed;
        result->result_code = item_result;
        return this->eResult_Fail;
    };
    // added rules to support _NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM
    // CHANGES: f101104.3L, added a filter about 'etherealization_mode_' 
    // as represent of 'ExtractType' in item scripts
    const BASE_ITEMINFO* extract_item_info = extract_item.GetItemInfo();
    const eEtherealizeMode& etherealize_mode = extract_item_info->etherealization_mode_;
    switch (etherealize_mode.value)
    {
    case etherealize_mode.NoFilter:
        break;
    case etherealize_mode.ExtractEtheriaOnly:
        if (extract_operation != eITEMWASTE_ETHEREALIZER_ETHERIA) {
            success = false;
            result->result = result->eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_INVALID_TYPE;
            return this->eResult_Fail;
        }
        break;
    case etherealize_mode.ExtractShellOnly:
        if (extract_operation != eITEMWASTE_ETHEREALIZER_SHELL) {
            success = false;
            result->result = result->eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_INVALID_TYPE;
            return this->eResult_Fail;
        }
        break;
    case etherealize_mode.CantEtherealize:
    default:
        success = false;
        result->result = result->eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALID_TYPE;
        return this->eResult_Fail;
    }
    // NOTE, a default action require a new slot to insert an extracted etheria item.
    // CHANGES, f100901.2L, changes a check sequence and
    // add a rule that a process enables the next step when a requisite empty slot is exist
    // if an operation is a default.
    if (extract_operation == kDefaultExtractOperation)
    {  // check empty slot
        int empty_slots = item_manager->CheckInsertItem(SI_INVENTORY,
                                                        HardCode::ITEMCODE_of_ETHERIA, 1);
        if (empty_slots != 1)
        {
            success = false;
            result->result = result->eRet_ItemFailed;
            result->result_code = RC::RC_ITEM_NOSPACEININVENTORY;
            return this->eResult_Fail;
        }
    };
    //
    int log_type_for_change_item = ITEM_CHANGED_TO_SHELL;
    SCItemSlot making_item;
    switch (extract_operation)
    {
    case kDefaultExtractOperation:
        log_type_for_change_item = ITEM_CHANGED_TO_SHELL;
        item_result = extract_item.ExtractToEtheria(&making_item);
        break;
    case eITEMWASTE_ETHEREALIZER_SHELL:
        log_type_for_change_item = ITEM_CHANGED_TO_SHELL;
        if (extract_item.ChangeToShellItem() == false) {
            item_result = RC::RC_ITEM_INVALIDSTATE;
        }
        break;
    case eITEMWASTE_ETHEREALIZER_ETHERIA:
        log_type_for_change_item = ITEM_CHANGED_TO_ETHERIA;
        if (extract_item.ChangeToEtheriaItem() == false) {
            item_result = RC::RC_ITEM_INVALIDSTATE;
        }
        break;
    };
    if (item_result != RC::RC_ITEM_SUCCESS)
    {
        success = false;
        result->result = result->eRet_ItemFailed;
        result->result_code = item_result;
        return this->eResult_Fail;
    }
    // logging
    GAMELOG->LogItem(log_type_for_change_item, player_, &extract_item);
    //
    if (kUseEtherealizer)
    {
        WORD item_code_out = 0;
        RC::eITEM_RESULT item_result = \
            item_manager->Use(SI_INVENTORY, extract_info.etherealizer_pos, item_code_out);
        if (item_result != RC::RC_ITEM_SUCCESS)
        {
            success = false;
            result->result = req_info.eRet_ItemFailed;
            result->result_code = item_result;
            return eResult_Fail;
        }
    };
    // success control
    assert(success != false);
    if (extract_operation != kDefaultExtractOperation)
    {
        result->result = result->eRet_Success;
        result_data->data_type = result_data->eDataType_Changed;
        result_data->changed.is_etheria = (extract_operation == eITEMWASTE_ETHEREALIZER_ETHERIA);
        extract_item.CopyOut(result_data->changed.changed_item);
    }
    else //if (extract_operation == kDefaultExtractOperation)
    {
        // last update logic ... should be success
        INVENTORY_TOTAL_INFO inven_total_info;
        item_result = item_manager->Obtain(making_item, 1, &inven_total_info);
        if (FlowControl::FCAssert(item_result == RC::RC_ITEM_SUCCESS) == false)
        {
            success = false;
            result->result = result->eRet_ItemFailed;
            result->result_code = item_result;
            // rollback
            item_result = extract_item.CombineWithEtheria(making_item);
            ASSERT(item_result == RC::RC_ITEM_SUCCESS);
            return this->eResult_Fail;
        }

        result->result = result->eRet_Success;
        result_data->data_type = result_data->eDataType_Extract;
        extract_item.CopyOut(result_data->extract.shell_item);
        result_data->extract.etheria_item = inven_total_info.m_Slot[0];
        // logging
        POSTYPE acquied_etheria_pos = inven_total_info.m_Slot[0].m_Pos;
        const SCItemSlot& acquied_etheria = \
            static_cast<SCItemSlot&>(inventory->GetSlot(acquied_etheria_pos));
        GAMELOG->LogItem(ITEM_PICK, player_, &acquied_etheria);
    };
    return this->eResult_1to1;
}

EtheriaImpl::eResult EtheriaImpl::CombineWithEtheria(const EtheriaControl& req_info,
                                                     EtheriaControl* const result,
                                                     EtheriaControlData* const result_data)
{
    assert(req_info.request == req_info.eReq_Combine);
    result->Clear();
    result_data->Clear();
    // client passing data
    result->request = req_info.request;
    result->args = req_info.args;

    bool success = true;
    RC::eITEM_RESULT item_result = RC::RC_ITEM_SUCCESS;
    //
    const EtheriaControl::Req_Combine& combine_info = req_info.args.combine;
    //
    ePLAYER_BEHAVE_STATE behave = player_->GetBehave();
    if (behave != PLAYER_BEHAVE_IDLE_STATE)
    {
        success = false;
        result->result = req_info.eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return this->eResult_Fail;
    }
    //
    ItemManager* const item_manager = player_->GetItemManager();
    if (item_manager->ValidPos(SI_INVENTORY, combine_info.from_etheria, true) == false ||
        item_manager->ValidPos(SI_INVENTORY, combine_info.to_shell, true) == false)
    {
        success = false;
        result->result = result->eRet_ItemFailed;
        result->result_code = RC::RC_ITEM_INVALIDPOS;
        return this->eResult_Fail;
    }
    SCSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& etheria_item = \
        static_cast<SCItemSlot&>(inventory->GetSlot(combine_info.from_etheria));
    SCItemSlot& shell_item = static_cast<SCItemSlot&>(inventory->GetSlot(combine_info.to_shell));
    item_result = shell_item.CombineWithEtheria(etheria_item);
    if (item_result != RC::RC_ITEM_SUCCESS)
    {
        success = false;
        result->result = result->eRet_ItemFailed;
        result->result_code = item_result;
        return this->eResult_Fail;
    }
    GAMELOG->LogItem(ITEM_COMBINED_WITH_ETHERIA, player_, &shell_item);
    //
    SCItemSlot item_for_logging;
    success = inventory->DeleteSlot(combine_info.from_etheria, &item_for_logging) != false;
    assert(success);
    ;{
        result->result = result->eRet_Success;
        result_data->data_type = result_data->eDataType_Combine;
        shell_item.CopyOut(result_data->combine.combined_item);
        GAMELOG->LogItem(ITEM_USE, player_, &item_for_logging);
    };

    return this->eResult_1to1;
}

//==================================================================================================

void EtheriaFailHandler::SetFailResult(uint8_t result_type, uint16_t result_code, bool show_log)
{
    result_->request = req_info_.request;
    result_->args = req_info_.args;
    //
    result_->result = result_type;
    result_->result_code = result_code;
    result_data_->data_type = result_data_->eDataType_None;
    BOOST_STATIC_ASSERT(result_data_->eDataType_None == 0); //for clear memory stream
    if (show_log) {
        OutputLog();
    }
}

void EtheriaFailHandler::OutputLog()
{
    SUNLOG(eCRITICAL_LOG,
           _T("|["__FUNCTION__"]|Can't Accept Request|Request=%d|CharName=%s|"),
           req_info_.request, player_->GetCharName());
}

}; //end of namespace
