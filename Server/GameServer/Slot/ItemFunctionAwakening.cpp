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
nsSlot::ItemFunctionAwakening::ChkBlockAwakening() const
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
nsSlot::ItemFunctionAwakening::ChkInvaild(const POSTYPE equip_item_pos, 
                                          const POSTYPE material_item_pos, 
                                          const POSTYPE additive_item_pos) const
{
    if (root_->ValidPos(SI_INVENTORY, equip_item_pos) == false) 
    {
        return RC::RC_ITEM_INVALIDPOS;
    }

    if (root_->ValidPos(SI_INVENTORY, material_item_pos) == false) 
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
nsSlot::ItemFunctionAwakening::ChkSlotAwakening(const SCItemSlot& equip_item, 
                                                const SCItemSlot& material_item, 
                                                const SCItemSlot& additive_item) const
{
    return AwakeningInfoParser::Instance()->ResultAwakeningCheck(equip_item, material_item, additive_item);
}

RC::eITEM_RESULT 
nsSlot::ItemFunctionAwakening::DeleteMaterialAndAdditive(SCItemSlot& equip_item, 
                                                         SCItemSlot& material_item, 
                                                         SCItemSlot& additive_item)
{
    BYTE result_awakening = 
        AwakeningInfoParser::Instance()->GetResultAwakening(equip_item, material_item.GetCode(), material_item.GetAwakening());
    const AwakeningInfo* awakening_info = 
        AwakeningInfoParser::Instance()->FindData(equip_item.GetCode());
    if ( awakening_info == NULL )
    {
        return RC::RC_EQUIPMENT_AWAKENING_INVALID_EQUIP_INFO;
    }

    int need_additive_number = awakening_info->additive_count;
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	int need_material_number = AwakeningInfoParser::Instance()->GetAllRoundMaterialNum(equip_item.GetCode(), material_item.GetCode());
	if (need_material_number == 0)
	{
		return RC::RC_EQUIPMENT_AWAKENING_DOES_NOT_MATCH_MATERIAL;
	}
	std::list<AllRoundMaterialInfo> allround_material_list = awakening_info->allround_material_info_list;

	RC::eITEM_RESULT result_code = RC::RC_ITEM_SUCCESS;
	result_code = AwakeningInfoParser::Instance()->IsAllroundMaterial(equip_item.GetCode(), material_item.GetCode(), need_material_number);
	if (result_code != RC::RC_ITEM_SUCCESS)
	{
		return result_code;
	}
	
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD

    // 첨가제 삭제
    for (int count = 0; count < need_additive_number; ++count)
    {
        const RC::eITEM_RESULT check_delete_additive = 
            root_->DeleteWastedItem(SI_INVENTORY, additive_item.GetPos());
        if (RC::RC_ITEM_SUCCESS != check_delete_additive) 
        {
            SUNLOG(eCRITICAL_LOG, "|[%s]| Additives deletes failed(%d, %d, %d)",
                __FUNCTION__, root_->player_->GetCharGuid(), additive_item.GetCode(), need_additive_number);
            return check_delete_additive;
        }
    }

    // 재료 삭제
    const POSTYPE material_pos = material_item.GetPos();
#ifdef _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD
	for (int count = 0; count < need_material_number; ++count)
	{
		const RC::eITEM_RESULT check_delete_material = root_->Delete(SI_INVENTORY, 
			material_item.GetPos(), 1, ITEM_AWAKENING_DELETE_MATERIAL);
		if (RC::RC_ITEM_SUCCESS != check_delete_material) 
		{
			SUNLOG(eCRITICAL_LOG, "|[%s]| Materials deletes failed(%d, %d)",
				__FUNCTION__, root_->player_->GetCharGuid(), material_item.GetCode());
			return check_delete_material;
		}

		MSG_CG_ITEM_DELETE_CMD CmdMsg;
		CmdMsg.m_byReason = RC::RC_ITEM_DELETE_SUCCESS;
		CmdMsg.m_atIndex = SI_INVENTORY;
		CmdMsg.m_atPos = material_pos;
		root_->player_->SendPacket(&CmdMsg, sizeof(CmdMsg));
	}	
#else
    const RC::eITEM_RESULT check_delete_material = root_->Delete(SI_INVENTORY, 
        material_item.GetPos(), 1, ITEM_AWAKENING_DELETE_MATERIAL);
    if (RC::RC_ITEM_SUCCESS != check_delete_material) 
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| Materials deletes failed(%d, %d)",
            __FUNCTION__, root_->player_->GetCharGuid(), material_item.GetCode());
        return check_delete_material;
    }

    MSG_CG_ITEM_DELETE_CMD CmdMsg;
    CmdMsg.m_byReason = RC::RC_ITEM_DELETE_SUCCESS;
    CmdMsg.m_atIndex = SI_INVENTORY;
    CmdMsg.m_atPos = material_pos;
    root_->player_->SendPacket(&CmdMsg, sizeof(CmdMsg));
#endif // _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT
nsSlot::ItemFunctionAwakening::Awakening(const POSTYPE equip_item_pos, 
                                         const POSTYPE material_item_pos, 
                                         const POSTYPE additive_item_pos)
{   
    const RC::eITEM_RESULT check_block_awakening = ChkBlockAwakening();
    if (RC::RC_ITEM_SUCCESS != check_block_awakening) 
    {
        return check_block_awakening;
    }

    const RC::eITEM_RESULT check_invalid = ChkInvaild(equip_item_pos, material_item_pos, additive_item_pos);
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
    SCItemSlot& material_item_slot = static_cast<SCItemSlot&>(inventory->GetSlot(material_item_pos));    
    SCItemSlot& additive_item_slot = static_cast<SCItemSlot&>(inventory->GetSlot(additive_item_pos));    
    const RC::eITEM_RESULT check_slot_awakening = ChkSlotAwakening(equip_item_slot, material_item_slot, additive_item_slot);
    if (check_slot_awakening != RC::RC_ITEM_SUCCESS) 
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| CheckFunction failed(%d)",
            __FUNCTION__, root_->player_->GetCharGuid());
        return check_slot_awakening;
    }

    const CODETYPE additive_item_code = additive_item_slot.GetCode();
    const CODETYPE material_item_code = material_item_slot.GetCode();
    const BYTE material_awakening = material_item_slot.GetAwakening();
    const RC::eITEM_RESULT check_delete = DeleteMaterialAndAdditive(
        equip_item_slot, material_item_slot, additive_item_slot);
    if (check_delete != RC::RC_ITEM_SUCCESS) 
    {
        SUNLOG(eCRITICAL_LOG, "|[%s]| DeleteMaterialAndAdditive failed(%d)",
            __FUNCTION__, root_->player_->GetCharGuid());
        return check_delete;
    }

    bool success = GetResult(equip_item_slot, material_item_code, material_awakening);
    return ApplyAwakening(success, equip_item_slot, material_item_code, material_awakening, additive_item_code);
}

RC::eITEM_RESULT 
nsSlot::ItemFunctionAwakening::ApplyAwakening(bool success,
                                              SCItemSlot& equip_item_slot, 
                                              const CODETYPE material_item_code,
                                              const BYTE material_awakening,
                                              const CODETYPE additive_item_code)
{
    BYTE prev_awakening = equip_item_slot.GetAwakening();

    if (success == true)
    {
        BYTE result_awakening = 
            AwakeningInfoParser::Instance()->GetResultAwakening(equip_item_slot, material_item_code, material_awakening);
        equip_item_slot.SetAwakening(result_awakening);
        root_->SendItemChange(equip_item_slot);

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        if (NotificationInfoParser::Instance()->IsNotificationItem(
            equip_item_slot, eNotificationAwakening) == true)
        {
            MSG_DG_NOTIFICATION_ITEM_SYN msg;
            msg.info.SetInfo(root_->player_->GetCharName(),
                (root_->player_->GetGameZonePtr() != NULL) ? root_->player_->GetGameZonePtr()->GetMapCode() : 0,
                GetTickCount(), eNotificationAwakening, equip_item_slot.GetItemCode(), equip_item_slot.GetAwakening());
            g_pGameServer->SendToSessionType(GAME_DBPROXY, &msg, sizeof(MSG_DG_NOTIFICATION_ITEM_SYN));
        }
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    }

    GAMELOG->LogAwakening(success ? ITEM_AWAKENING_SUCCESS : ITEM_AWAKENING_FAILED, 
        root_->player_, equip_item_slot, material_item_code, material_awakening, 
        equip_item_slot.GetAwakening(), prev_awakening, additive_item_code);

    return success ? RC::RC_EQUIPMENT_AWAKENING_SUCCESS : RC::RC_EQUIPMENT_AWAKENING_FAIL;
}

const bool 
nsSlot::ItemFunctionAwakening::GetResult(const SCItemSlot& equip_item_slot, 
                                         const CODETYPE material_item_code, 
                                         const BYTE material_awakening) const
{
    int succeed_ratio = 
        AwakeningInfoParser::Instance()->GetSuccessProbability(
        equip_item_slot, material_item_code, material_awakening);
    RandomManager* const random_manager = root_->player_->GetRandomManager();
    int random_result = random_manager->randomizer_awakening_.Rand();
    succeed_ratio += root_->player_->GetAttr().GetAwakeningProbability();
    return (random_result <= succeed_ratio) ? true : false;
}

#endif // _NA_008124_20150313_AWAKENING_SYSTEM