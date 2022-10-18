#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"
#include "InventorySlotContainer.h"
#include "AwakeningInfoParser.h"
#include "SlotManager.h"

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"
#include "GameZone.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM

RC::eITEM_RESULT 
nsSlot::ItemFunctionEvolution::ChkBlockEvolution() const
{
    if (root_->player_->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING) {
        return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    }
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT 
nsSlot::ItemFunctionEvolution::ChkSlotEvolution(const SCItemSlot& equip_item,
                                                const SCItemSlot& additive_item) const
{
    return AwakeningInfoParser::Instance()->ResultEvolutionCheck(equip_item, additive_item);
}

RC::eITEM_RESULT 
nsSlot::ItemFunctionEvolution::DeleteAdditive(const CODETYPE& equip_item_code,
                                              SCItemSlot& additive_item)
{
    int need_additive_number = 
        AwakeningInfoParser::Instance()->GetEvolutionAdditiveCount(equip_item_code);

    // √∑∞°¡¶ ªË¡¶
    for (int count = 0; count < need_additive_number; ++count)
    {
        const RC::eITEM_RESULT check_delete_additive = 
            root_->DeleteWastedItem(SI_INVENTORY, additive_item.GetPos());
        if (RC::RC_ITEM_SUCCESS != check_delete_additive) 
        {
            SUNLOG(eCRITICAL_LOG, "|[%s]| Additives deletes failed(%d, %d, %d, %d)",
                __FUNCTION__, root_->player_->GetCharGuid(),
                additive_item.GetCode(), need_additive_number, count);
            return check_delete_additive;
        }
    }

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT
nsSlot::ItemFunctionEvolution::DeleteEquipment(const POSTYPE equip_item_pos)
{
    const RC::eITEM_RESULT check_delete_equip = root_->Delete(SI_INVENTORY, 
        equip_item_pos, 1, ITEM_EVOLUTION_DELETE_EQUIPMENT);
    if (RC::RC_ITEM_SUCCESS != check_delete_equip) 
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| EquipItem deletes failed(%d, %d)",
            __FUNCTION__, root_->player_->GetCharGuid(), equip_item_pos);
        return check_delete_equip;
    }

    MSG_CG_ITEM_DELETE_CMD CmdMsg;
    CmdMsg.m_byReason = RC::RC_ITEM_DELETE_SUCCESS;
    CmdMsg.m_atIndex = SI_INVENTORY;
    CmdMsg.m_atPos = equip_item_pos;
    root_->player_->SendPacket(&CmdMsg, sizeof(CmdMsg));

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT 
nsSlot::ItemFunctionEvolution::ChkInvaild(const POSTYPE equip_item_pos, 
                                          const POSTYPE additive_item_pos) const
{
    if (root_->ValidPos(SI_INVENTORY, equip_item_pos) == false) 
    {
        return RC::RC_ITEM_INVALIDPOS;
    }

    if (root_->ValidPos(SI_INVENTORY, additive_item_pos) == false) 
    {
        return RC::RC_ITEM_INVALIDPOS;
    }
    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT
nsSlot::ItemFunctionEvolution::Evolution(const POSTYPE equip_item_pos, 
                                         const POSTYPE additive_item_pos)
{   
    const RC::eITEM_RESULT check_block_evolution = ChkBlockEvolution();
    if (RC::RC_ITEM_SUCCESS != check_block_evolution) 
    {
        return check_block_evolution;
    }

    const RC::eITEM_RESULT check_invalid = ChkInvaild(equip_item_pos, additive_item_pos);
    if (RC::RC_ITEM_SUCCESS != check_invalid) 
    {
        return check_invalid;
    }

    SCSlotContainer* const inventory = root_->GetItemSlotContainer(SI_INVENTORY);
    if (inventory == NULL)
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }

    SCItemSlot& equip_item_slot = static_cast<SCItemSlot&>(inventory->GetSlot(equip_item_pos));    
    SCItemSlot& additive_item_slot = static_cast<SCItemSlot&>(inventory->GetSlot(additive_item_pos));    
    const RC::eITEM_RESULT check_slot_evolution = ChkSlotEvolution(equip_item_slot, additive_item_slot);
    if (check_slot_evolution != RC::RC_ITEM_SUCCESS) 
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| CheckFunction failed(%d)",
            __FUNCTION__, root_->player_->GetCharGuid());
        return check_slot_evolution;
    }

    const CODETYPE additive_item_code = additive_item_slot.GetCode();
    const RC::eITEM_RESULT check_delete = 
        DeleteAdditive(equip_item_slot.GetCode(), additive_item_slot);
    if (check_delete != RC::RC_ITEM_SUCCESS) 
    {
        return check_delete;
    }

    return ApplyEvolution(GetResult(equip_item_slot.GetCode()), equip_item_slot, additive_item_code);
}

RC::eITEM_RESULT 
nsSlot::ItemFunctionEvolution::ApplyEvolution(bool success,
                                              SCItemSlot& equip_item_slot, 
                                              const CODETYPE additive_item_code)
{
    const CODETYPE result_item_code = 
        AwakeningInfoParser::Instance()->GetResultEvolutionItemCode(equip_item_slot.GetCode());
    const CODETYPE prev_item = equip_item_slot.GetCode();
    const BYTE prev_enchant = equip_item_slot.GetEnchant();

    if (success == true)
    {
        const POSTYPE equip_item_pos = equip_item_slot.GetPos();
        const RC::eITEM_RESULT check_delete_equip = DeleteEquipment(equip_item_pos);
        if (RC::RC_ITEM_SUCCESS != check_delete_equip) 
        {
            return check_delete_equip;
        }

        const RC::eITEM_RESULT check_obtain = 
            ObtainItem(GetResultEnchant(prev_enchant), result_item_code);
        if (RC::RC_ITEM_SUCCESS != check_obtain) 
        {
            return check_obtain;
        }
    }

    GAMELOG->LogEvolution(success ? ITEM_EVOLUTION_SUCCESS : ITEM_EVOLUTION_FAILED, 
        root_->player_, result_item_code, prev_item, additive_item_code);

    return success ? RC::RC_EQUIPMENT_EVOLUTION_SUCCESS : RC::RC_EQUIPMENT_EVOLUTION_FAIL;
}

RC::eITEM_RESULT
nsSlot::ItemFunctionEvolution::ObtainItem(const BYTE result_enchant, const CODETYPE result_item_code)
{
    ItemType item_type;
    item_type.SetEnchant(result_enchant);
    item_type.SetItemType(KindOfItemType::Default);
    item_type.SetRank(0);
    SCItemSlot obtain_item_slot;
    if (nsSlot::ItemTypeChanger::MakeItemSlotByType(
        &obtain_item_slot, result_item_code, 1, &item_type) == false)
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| EvolutionItem create failed(%d, %d)",
            __FUNCTION__, root_->player_->GetCharGuid(), result_item_code);
        return RC::RC_ITEM_INVALIDSTATE;
    }

    MSG_CG_ITEM_OBTAIN_ITEM_CMD send_msg;
    RC::eITEM_RESULT obtain_check = root_->Obtain(obtain_item_slot, 1, &send_msg.m_tagInventory);
    if (obtain_check != RC::RC_ITEM_SUCCESS) {
        SUNLOG(eCRITICAL_LOG, "|[%s]| EvolutionItem obtains failed(%d, %d)",
            __FUNCTION__, root_->player_->GetCharGuid(), result_item_code);
        return obtain_check;
    }

    root_->player_->SendPacket(&send_msg, send_msg.GetSize());

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    if (NotificationInfoParser::Instance()->IsNotificationItem(
        obtain_item_slot, eNotificationEvolution) == true)
    {
        MSG_DG_NOTIFICATION_ITEM_SYN msg;
        msg.info.SetInfo(root_->player_->GetCharName(),
            (root_->player_->GetGameZonePtr() != NULL) ? root_->player_->GetGameZonePtr()->GetMapCode() : 0,
            GetTickCount(), eNotificationEvolution, obtain_item_slot.GetItemCode(), result_enchant);
        g_pGameServer->SendToSessionType(GAME_DBPROXY, &msg, sizeof(MSG_DG_NOTIFICATION_ITEM_SYN));
    }
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

    return RC::RC_ITEM_SUCCESS;
}

const BYTE 
nsSlot::ItemFunctionEvolution::GetResultEnchant(const BYTE prev_enchant) const
{
    BYTE result_enchant = 0;
    switch(prev_enchant)
    {
    case 8: case 9: case 10: case 11: case 12:
        result_enchant = 7;
        break;
    case 13:
        result_enchant = 11;
        break;
    case 14:
        result_enchant = 12;
        break;
    case 15:
        result_enchant = 13;
        break;
    }

    return result_enchant;
}

const bool 
nsSlot::ItemFunctionEvolution::GetResult(const CODETYPE& equip_item_code) const
{
    int succeed_ratio = 
        AwakeningInfoParser::Instance()->GetEvolutionSuccessProbability(equip_item_code);
    RandomManager* const random_manager = root_->player_->GetRandomManager();
    int random_result = random_manager->randomizer_evolution_.Rand();

    return (random_result <= succeed_ratio) ? true : false;
}

#endif // _NA_008124_20150313_AWAKENING_SYSTEM