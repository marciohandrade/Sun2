#include "stdafx.h"
#include "./ItemManager_Move.h"
#include "ItemManager.h"

#include "SlotInsertResultConvert.h"
#include "SCSlotContainer.h"
#include "ItemAttrCalculator.h"

#include "AchievementManager.h" // added by _NA_001990_ACSYSTEM_ADD

//==================================================================================================
#include "GameZone.h"
#include "Status/StatusManager.h"

#include "EquipmentSlotContainer.h"
#include "GuildWarehouseSlotContainer.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemMoveImpl) == sizeof(ItemManager*));

#define SLOTIDX_FROM_TO(_from_index_, _to_index_) \
    MAKEWORD((_from_index_), (_to_index_))

}; //end of namespace

//==================================================================================================

#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
VOID nsSlot::ItemMoveImpl::CacheArg::SetData(ItemManager* item_mgr, SLOTIDX index, POSTYPE slot_pos)
{
    slot_index = index;
    container = item_mgr->GetItemSlotContainer(this->slot_index);
    pos_in_slots = slot_pos;

    item_slot = &static_cast<SCItemSlot&>(container->GetSlot(pos_in_slots));
    slot_exist = (item_slot->GetItemCode() != 0);
    item_rule_info = &item_slot->GetItemRuleInfo();
    item_info = item_slot->GetItemInfo();
}
BOOL nsSlot::ItemMoveImpl::CacheArg::DeleteItem(SCSlot* slot_clone_before_delete)
{
    if (container->DeleteSlot(pos_in_slots, slot_clone_before_delete) == FALSE) {
        return FALSE;
    }

    item_slot = NULL;
    slot_exist = false;
    item_rule_info = NULL;
    item_info = NULL;

    return TRUE;
}
 RC::eSLOT_INSERT_RESULT nsSlot::ItemMoveImpl::CacheArg::InsertItem(SCSlot& slot)
{
    RC::eSLOT_INSERT_RESULT result = container->InsertSlot(pos_in_slots, slot);

    if (result == RC::RC_INSERT_SLOT_SUCCESS)
    {
        item_slot = &static_cast<SCItemSlot&>(container->GetSlot(pos_in_slots));
        slot_exist = (item_slot->GetItemCode() != 0);
        item_rule_info = &item_slot->GetItemRuleInfo();
        item_info = item_slot->GetItemInfo();
    }

    return result;
}
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE

//==================================================================================================

// NOTE: this moving implements can only accept ItemContainer interoperability.
bool nsSlot::ItemMoveImpl::CheckMovingSlotIndexRule(
    RC::eITEM_RESULT* item_result,
    const SLOTIDX from_slot_index, const SLOTIDX to_slot_index) const
{
    // NOTE: removed slot index = { SI_TEMPINVENTORY (f100528.3L, 0903 ~ current) }
    const ulong kAcceptableField = (1 << SI_INVENTORY)
                                 | (1 << SI_EQUIPMENT)
                                 | (1 << SI_WAREHOUSE)
                                 | (1 << SI_GUILDWAREHOUSE);
    if (bool cant_acceptable_from = ((kAcceptableField & (1 << from_slot_index)) == 0)) {
        *item_result = RC::RC_ITEM_INVALIDPOS;
        return false;
    };
    if (bool cant_acceptable_to = ((kAcceptableField & (1 << to_slot_index)) == 0)) {
        *item_result = RC::RC_ITEM_INVALIDPOS;
        return false;
    };
    // SLOTIDX_FROM_TO
    const WORD moving_rule = SLOTIDX_FROM_TO(from_slot_index, to_slot_index);
    switch (moving_rule)
    {
    case SLOTIDX_FROM_TO(SI_INVENTORY, SI_INVENTORY):
    case SLOTIDX_FROM_TO(SI_INVENTORY, SI_EQUIPMENT):
    case SLOTIDX_FROM_TO(SI_INVENTORY, SI_WAREHOUSE):
    case SLOTIDX_FROM_TO(SI_INVENTORY, SI_GUILDWAREHOUSE):
        break;
    //----------------------------------------------------------------------------------------------
    case SLOTIDX_FROM_TO(SI_EQUIPMENT, SI_INVENTORY):
    case SLOTIDX_FROM_TO(SI_EQUIPMENT, SI_EQUIPMENT):
        break;
    //----------------------------------------------------------------------------------------------
    case SLOTIDX_FROM_TO(SI_WAREHOUSE, SI_INVENTORY):
    case SLOTIDX_FROM_TO(SI_WAREHOUSE, SI_WAREHOUSE):
        break;
    //----------------------------------------------------------------------------------------------
    case SLOTIDX_FROM_TO(SI_GUILDWAREHOUSE, SI_INVENTORY):
    case SLOTIDX_FROM_TO(SI_GUILDWAREHOUSE, SI_GUILDWAREHOUSE):
        break;
    //----------------------------------------------------------------------------------------------
    default:
        *item_result = RC::RC_ITEM_INVALIDPOS;
        return false;
    };
    //
    *item_result = RC::RC_ITEM_SUCCESS;
    return true;
}

//==================================================================================================
// NOTE: f110701.2L
bool nsSlot::ItemMoveImpl::CheckMovingAcceptableState(
    RC::eITEM_RESULT* item_result,
    CacheArg* const from, CacheArg* const to) const
{
    // CHANGES: change to more generic code 'IsPetLocked' & 'IsRiderLocked' ...
    if (from->item_slot->IsUserBlocked() || to->item_slot->IsUserBlocked()) {
        *item_result = RC::RC_ITEM_INVALIDPOS;
        return false;
    };
    //----------------------------------------------------------------------------------------------
    if (const_cast<SCItemSlot*>(from->item_slot)->GetLaxLock()->IsLock() ||
        const_cast<SCItemSlot*>(to->item_slot)->GetLaxLock()->IsLock())
    {
        *item_result = RC::RC_ITEM_INVALIDPOS;
        return false;
    };
    Player* const player = root_->player_;
	// no-check inputed args...
	GameField* const game_field = player->GetField();
	if ((game_field && player->GetGameZonePtr()) == false) {
		*item_result = RC::RC_ITEM_INVALIDSTATE;
        return false;
    };
    *item_result = RC::RC_ITEM_SUCCESS;
    return true;
}

//==================================================================================================

bool nsSlot::ItemMoveImpl::CheckInsertableCondition(
    RC::eITEM_RESULT* item_result,
    CacheArg* const from, CacheArg* const to) const
{
    // NOTE: f110701.2L, no check locked states. use 'CheckMovingAcceptableState'
    //----------------------------------------------------------------------------------------------
    //// pre-loading variable with debugging facility
    //const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    // CHANGES: f110713.4L, modified to validated target about the equipments rules
    const WORD moving_rule = SLOTIDX_FROM_TO(from->slot_index, to->slot_index);
    CacheArg* equip = NULL;
    CacheArg* inven = NULL;
    if (moving_rule == SLOTIDX_FROM_TO(SI_INVENTORY, SI_EQUIPMENT)) {
        equip = to; inven = from;
    }
    else if (moving_rule == SLOTIDX_FROM_TO(SI_EQUIPMENT, SI_INVENTORY)) {
        equip = from; inven = to;
    };
    // (WAVERIX) (090430) (BUG-FIX) �巡�� ���� ���� ���, ���� ��ü ���� �Ϻ� ����Ǵ� �κ� ���
    if (equip)
    {
        if (root_->player_->GetStatusManager()->Status.IsDragonTransforming()) {
            *item_result = RC::RC_ITEM_UNABLE_FUNCTION_FOR_TRANSFORMATION;
            return false;
        };
    };

	const eZONE_RULESET& zone_ruleset = root_->player_->GetGameZonePtr()->Ruleset;
	if (zone_ruleset.rules & zone_ruleset.ZONE_RULE_INDUN_CANT_EQUIP_WEAPON_AND_BLOCK)
	{
		bool cant_accept_status = (equip) && (equip->pos_in_slots == eEQUIPCONTAINER_WEAPON);
		if (cant_accept_status) {
			*item_result = RC::RC_ITEM_CANT_EQUIP_WEAPON;
            return false;
        };
	}

#ifdef _NA_20100307_BETAKEY_SYSTEM
    // ��ŸŰ�� ������ ���� ����ڴ� ��ŸŰ �������� �������� ���Ѵ�.
    if (equip)
    {
        const BetaKeyInfo* beta_key_info = root_->player_->GetBetaKeyInfo();
        if (beta_key_info->CanAbilityContent(inven->item_info->m_betakey) == false) {
            *item_result = RC::RC_ITEM_CANNOT_EQUIPMENT_BETAKEY;
            return false;
        };
    }
#endif

	*item_result = RC::RC_ITEM_SUCCESS;
    return true;
}

//==================================================================================================
// CHANGES: f110701.2L, added the rule is to prevent irregula stat calcuation
// like the dragon transformed state.
// other states do not regard because the method that is 'CheckDeletableCondition'
// is used by the user action 'Move'
bool nsSlot::ItemMoveImpl::CheckDeletableCondition(RC::eITEM_RESULT* item_result,
    CacheArg* const from, CacheArg* const to) const
{
    __TOUCHED((to));
    // NOTE: f110701.2L, no check locked states. use 'CheckMovingAcceptableState'
    // CHANGES: f110713.4L, modified to validated target about the equipments rules
    const WORD moving_rule = SLOTIDX_FROM_TO(from->slot_index, to->slot_index);
    CacheArg* equip = NULL;
    CacheArg* inven = NULL;
    if (moving_rule == SLOTIDX_FROM_TO(SI_INVENTORY, SI_EQUIPMENT)) {
        equip = to; inven = from;
    }
    else if (moving_rule == SLOTIDX_FROM_TO(SI_EQUIPMENT, SI_INVENTORY)) {
        equip = from; inven = to;
    };
    //----------------------------------------------------------------------------------------------
    if (equip) {
        if (root_->player_->GetStatusManager()->Status.IsDragonTransforming()) {
            *item_result = RC::RC_ITEM_UNABLE_FUNCTION_FOR_TRANSFORMATION;
            return false;
        };
    };
    *item_result = RC::RC_ITEM_SUCCESS;
    return true;
}

//==================================================================================================
bool nsSlot::ItemMoveImpl::CheckMovingFilterRule(RC::eITEM_RESULT* item_result,
    CacheArg* const from, CacheArg* const to) const
{
    //----------------------------------------------------------------------------------------------
    // restriction for move from slot to other slot
    if (from->item_slot->CanTradeSell(eITEM_TRADESELL_SLOT) == false ||
        (to->slot_exist && to->item_slot->CanTradeSell(eITEM_TRADESELL_SLOT) == false))
    {
        if (from->slot_index != to->slot_index) {
            *item_result = RC::RC_ITEM_CANNOT_MOVE_OTHER_SLOT_ITEM;
            return false;
        }
    };
    //----------------------------------------------------------------------------------------------
    // restirctions for TradeSell field rule
    // from  ->   to(empty)
    // from  <->  to(!empty)
    // â������
    // - �������� �̵��Ϸ��� �����̳��� â�� �ִٸ�
    // 1. �κ����� â��� �̵�: �κ��� ���Կ� â�� �̵� �Ұ��� �Ӽ��� ������ FAIL
    // 2. â���� �κ����� �̵�: �κ��� ���Կ� â�� �̵� �Ұ��� �Ӽ��� ������ FAIL
    const bool check_swapping_rule = (to->slot_exist);
    ;;;; if ((to->slot_index == SI_WAREHOUSE || to->slot_index == SI_GUILDWAREHOUSE) &&
             (from->item_slot->CanTradeSell(eITEM_TRADESELL_WAREHOUSE) == false))
    {
        *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
        return false;
    }
    else if ((check_swapping_rule) &&
             (from->slot_index == SI_WAREHOUSE || from->slot_index == SI_GUILDWAREHOUSE) &&
             (to->item_slot->CanTradeSell(eITEM_TRADESELL_WAREHOUSE) == false))
    {   // swapping rule
        *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
        return false;
    }
    //--â�� ���� �Ұ�, added rules by '_NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION'
    else if ((to->slot_index == SI_WAREHOUSE) &&
             (from->item_slot->CanTradeSell(eITEM_TRADESELL_ONLY_WAREHOUSE) == false))
    {
        *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
        return false;
    }
    //--[�κ� -> ����â��] ������ ��ȯ���� ����â�� ���� �Ұ�, _NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION
    else if ((check_swapping_rule) &&
             (from->slot_index == SI_WAREHOUSE) &&
             (to->item_slot->CanTradeSell(eITEM_TRADESELL_ONLY_WAREHOUSE) == false))
    {   // swapping rule
        *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
        return false;
    };
    //----------------------------------------------------------------------------------------------
    if ((check_swapping_rule) &&
        (to->slot_index == SI_GUILDWAREHOUSE || from->slot_index == SI_GUILDWAREHOUSE) &&
        ((to->item_info->m_wType == eITEMTYPE_AC) ||
         (to->item_info->m_wType == eITEMTYPE_QUEST) ||
         (to->item_info->m_wType == eITEMTYPE_PORTAL_QUEST_ITEM)))
    {   // swapping rule
        *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
        return false;
    }

    //----------------------------------------------------------------------------------------------

    if ((check_swapping_rule) &&
        (from->slot_index == SI_GUILDWAREHOUSE))
    {
        // (NOTE) prevent an exchange operation
        if (to->item_slot->CanTradeSell(eITEM_TRADESELL_TRADE) == false)
        {   //--�ŷ� �Ұ��� + ��� â�� �̵� ����, '_NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION'
            if (to->item_slot->CanTradeSell(eITEM_TRADESELL_ONLY_WAREHOUSE) == true)
            {
                *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
                return false;
            }
        }
        if ((to->item_slot->CanTradeSell(eITEM_TRADESELL_USED_ITEM) == false) &&
            (to->item_slot->IsProgressTime()))
        {
            *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
            return false;
        }
    };

    if (to->slot_index == SI_GUILDWAREHOUSE) 
    {
        /* �ŷ� �Ұ� / AC ������ / ĳ�� �������� �̵� �Ұ� */
        if (from->item_slot->CanTradeSell(eITEM_TRADESELL_TRADE) == false) {
            //--�ŷ� �Ұ��� + ��� â�� �̵� ����, _NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION
            if (from->item_slot->CanTradeSell(eITEM_TRADESELL_ONLY_WAREHOUSE) == true)
            {
                *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
                return false;
            }
        }
        if (from->item_slot->CanTradeSell(eITEM_TRADESELL_USED_ITEM) == false &&
            from->item_slot->IsProgressTime())
        {
            *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
            return false;
        }
        //(lst1024)(�߱�) eITEMSUBTYPE_ELITE ��� â��� �̵� �Ұ�.
        if (from->item_slot->CanTradeSell(eITEM_TRADESELL_GUILDWAREHOUSE) == false) {
            *item_result = RC::RC_ITEM_CANNOT_MOVE_WAREHOUSE_ITEM;
            return false;
        }
        /* ��� ������ ĳ���� ������ ���� ��� â�� ���� �̿����� üũ */
        *item_result = static_cast<CGuildWarehouseSlotContainer*>(to->container)->IsValidPos(
            to->pos_in_slots);
        if (*item_result != RC::RC_ITEM_SUCCESS) {
            return false;
        }
    }
    //��� â���� ���� ��� ������ �ִ��� üũ�Ѵ�.
    if ( (SI_GUILDWAREHOUSE == from->slot_index && SI_INVENTORY == to->slot_index) || //â���� �κ����� �̵�
         (SI_INVENTORY == from->slot_index && SI_GUILDWAREHOUSE == to->slot_index && check_swapping_rule ) )
         //�κ� �������� â������۰� ��ȯ �Ҷ�
    {
        *item_result = static_cast<CGuildWarehouseSlotContainer*>(\
            from->container)->IsUseRight(root_->player_, true);
        if (*item_result != RC::RC_ITEM_SUCCESS) {
            return false;
        }
    }

    if ((check_swapping_rule) &&
        (SI_INVENTORY == from->slot_index && SI_GUILDWAREHOUSE == to->slot_index))
    {
        *item_result = static_cast<CGuildWarehouseSlotContainer*>(\
            to->container)->IsUseRight(root_->player_, false);
        if (*item_result != RC::RC_ITEM_SUCCESS) {
            return false;
        }
    }

    *item_result = RC::RC_ITEM_SUCCESS;
    return true;
}


//==================================================================================================

// NOTE: f100528.3L, code arrangement and enforce an empty check rule
RC::eITEM_RESULT nsSlot::ItemMoveImpl::Move(
    const SLOTIDX from_slot_index, const POSTYPE from_pos,
    const SLOTIDX to_slot_index, const POSTYPE to_pos,
    const DURATYPE move_quantity)
{
    RC::eITEM_RESULT item_result = RC::RC_ITEM_SUCCESS;
    //----------------------------------------------------------------------------------------------
    if (CheckMovingSlotIndexRule(&item_result, from_slot_index, to_slot_index) == false) {
        return item_result;
    };
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    };
    // Validate üũ ��ƾ �ʼ�!!!!!  
    if (root_->ValidPos(from_slot_index, from_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    };
    if (root_->ValidPos(to_slot_index, to_pos, false) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    };
    //----------------------------------------------------------------------------------------------
    CacheArg from = { 0, };
#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
    from.SetData(root_, from_slot_index, from_pos);
#else
    if (CacheArg* arg = &from)
    {
        arg->slot_index = from_slot_index;
        arg->pos_in_slots = from_pos;
        arg->container = root_->GetItemSlotContainer(arg->slot_index);
        arg->item_slot = &static_cast<SCItemSlot&>(arg->container->GetSlot(arg->pos_in_slots));
        arg->slot_exist = (arg->item_slot->GetItemCode() != 0);
        arg->item_rule_info = &arg->item_slot->GetItemRuleInfo();
        arg->item_info = arg->item_slot->GetItemInfo();
    };
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE
    
    // CHANGES: protect invalid item operation
    if (from.item_rule_info->is_valid_item == false) {
        return RC::RC_ITEM_INVALIDPOS;
    };
    assert(from.slot_exist);
    //----------------------------------------------------------------------------------------------
    CacheArg to = { 0, };
#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
    to.SetData(root_, to_slot_index, to_pos);
#else
    if (CacheArg* arg = &to)
    {
        arg->slot_index = to_slot_index;
        arg->pos_in_slots = to_pos;
        arg->container = root_->GetItemSlotContainer(arg->slot_index);
        arg->item_slot = &static_cast<SCItemSlot&>(arg->container->GetSlot(arg->pos_in_slots));
        arg->slot_exist = (arg->item_slot->GetItemCode() != 0);
        arg->item_rule_info = &arg->item_slot->GetItemRuleInfo();
        arg->item_info = arg->item_slot->GetItemInfo();
    };
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE
    //----------------------------------------------------------------------------------------------
    if (this->CheckMovingAcceptableState(&item_result, &from, &to) == false) {
        return item_result;
    };
    if (this->CheckInsertableCondition(&item_result, &from, &to) == false) {
        return item_result;
    };
    // CHANGES: f110701.2L, added the rule is to prevent irregula stat calcuation
    // like the dragon transformed state.
    if (this->CheckDeletableCondition(&item_result, &from, &to) == false) {
        return item_result;
    };
    if (this->CheckMovingFilterRule(&item_result, &from, &to) == false) {
        return item_result;
    };
    //----------------------------------------------------------------------------------------------
    const bool divided_move = (move_quantity > 0 && move_quantity != from.item_slot->GetNum());
    if (divided_move) {
        if (this->MoveDivision(&item_result, &from, &to, move_quantity) == false) {
            return item_result;
        };
    }
    else {
        if (this->MoveDefault(&item_result, &from, &to) == false) {
            return item_result;
        };
    };
    //
    assert(item_result == RC::RC_ITEM_SUCCESS);
    LogOnSuccess(&from, &to, move_quantity);

    return RC::RC_ITEM_SUCCESS;
}

//==================================================================================================
// CHANGES: f110418.1L, moved logging parts from 'CG_ITEM_MOVE_SYN' handler.
bool nsSlot::ItemMoveImpl::LogOnSuccess(CacheArg* const from, CacheArg* const to,
                                        const DURATYPE move_quantity)
{
    const bool is_put_to_warehouse =
        (from->slot_index == SI_INVENTORY && to->slot_index == SI_WAREHOUSE);
    const bool is_get_from_warehouse =
        (from->slot_index == SI_WAREHOUSE && to->slot_index == SI_INVENTORY);
    const bool is_get_from_temp_inventory =
        (from->slot_index == SI_TEMPINVENTORY && to->slot_index == SI_INVENTORY);
            
    const bool is_put_to_guild_warehouse =
        (from->slot_index == SI_INVENTORY && to->slot_index == SI_GUILDWAREHOUSE);
    const bool is_get_from_guild_warehouse =
        (from->slot_index == SI_GUILDWAREHOUSE && to->slot_index == SI_INVENTORY);
    const bool is_move_guild_warehouse =
        (is_put_to_guild_warehouse || is_get_from_guild_warehouse);
            
    const bool is_move_warehouse = (is_move_guild_warehouse || 
                                    is_put_to_warehouse || 
                                    is_get_from_warehouse || 
                                    is_get_from_temp_inventory);

    // â��� �̵����� �ʾҴٸ�
    if (is_move_warehouse == false)
    {
        // â�� �α׸� ���� �ʿ䰡 ����.
        return true;
    }

    eLOG_ITEM item_log_type = ITEM_MAX;

    // â�� �־��ٸ�
    if (is_put_to_warehouse)
    {
        item_log_type = ITEM_WAREHOUSE_PUT;
    }
    // â���� ���´ٸ�
    else if (is_get_from_warehouse)
    {
        item_log_type = ITEM_WAREHOUSE_GET;
    }
    // ���� â���� ���´ٸ�
    else if (is_get_from_temp_inventory)
    {
        item_log_type = ITEM_TEMP_INVENTORY_GET;
    }
    // ��� â�� �־��ų� ���´ٸ�
    else if (is_move_guild_warehouse)
    {
        CGuildWarehouseSlotContainer* guild_warehouse_slot_container(NULL);
        GuildWarehouseLogType to_guild_warehouse_log_type;
        GuildWarehouseLogType from_guild_warehouse_log_type;

        // ��� â�� �־��ٸ�
        if (is_put_to_guild_warehouse)
        {   
            item_log_type = ITEM_GUILD_WAREHOUSE_PUT;
            to_guild_warehouse_log_type = ITEM_IN;
            from_guild_warehouse_log_type = ITEM_OUT;
            guild_warehouse_slot_container = 
                static_cast<CGuildWarehouseSlotContainer*>(to->container);
        }
        // ��� â���� ���´ٸ�
        else
        {
            assert(is_get_from_guild_warehouse);
            item_log_type = ITEM_GUILD_WAREHOUSE_GET;
            to_guild_warehouse_log_type = ITEM_OUT;
            from_guild_warehouse_log_type = ITEM_IN;
            guild_warehouse_slot_container = 
                static_cast<CGuildWarehouseSlotContainer*>(from->container);
        }

        guild_warehouse_slot_container->ItemLogDBSave(
            const_cast<SCItemSlot*>(to->item_slot), to_guild_warehouse_log_type);
                
        // ��� â���� �����۰� ��ȯ �ߴٸ�
        if (!from->container->IsEmpty(from->pos_in_slots))
        {
            guild_warehouse_slot_container->ItemLogDBSave(
                const_cast<SCItemSlot*>(from->item_slot), from_guild_warehouse_log_type);
        }
    }
    else // ����� �����ߴٸ� ������ �ִ�.
    {
        ASSERT(false);
        return false;
    }
            
    // â�� �α׸� ����Ѵ�.
    const DURATYPE actual_quantity_moved = \
        (move_quantity == 0 ? to->item_slot->GetNum() : move_quantity);
    GAMELOG->LogItemWarehouse(item_log_type,
        root_->player_, to->item_slot, actual_quantity_moved,
        from->slot_index, from->pos_in_slots,
        to->slot_index, to->pos_in_slots);

    return true;
}

//==================================================================================================
// NOTE: f110418.2L, added rule to prevent swapping fails
// when an equipment switch to other an equipment
nsSlot::ItemMoveImpl::eSwapContinualResult
nsSlot::ItemMoveImpl::CheckEnableSwapWhenSwapEquipments(
    RC::eITEM_RESULT* item_result,
    CacheArg* const from, CacheArg* const to)
{
#if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
    *item_result = RC::RC_ITEM_SUCCESS;
    return eSwap_Process;
#endif
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
    *item_result = RC::RC_ITEM_SUCCESS;
    //----------------------------------------------------------------------------------------------
    if (to->slot_exist == false) {
        return eSwap_Process;
    };
    // CHANGES: f110713.4L, modified to validated target about the equipments rules
    const WORD moving_rule = SLOTIDX_FROM_TO(from->slot_index, to->slot_index);
    CacheArg* equip = NULL;
    CacheArg* inven = NULL;
    if (moving_rule == SLOTIDX_FROM_TO(SI_INVENTORY, SI_EQUIPMENT)) {
        equip = to; inven = from;
    }
    else if (moving_rule == SLOTIDX_FROM_TO(SI_EQUIPMENT, SI_INVENTORY)) {
        equip = from; inven = to;
    };
    if (equip)
    {
        // test the equippable device when equipments swapping
        Player* const player = root_->player_;
        bool pc_room_status = (player->CanApplyPCBangBenefit(true) != false);
        ItemAttrCalculator item_calculator(player->GetPlayerAttr(),
            static_cast<CEquipmentSlotContainer*>(equip->container));
        if (item_calculator.TestSwitchableEquipments(
                inven->item_slot, equip->pos_in_slots, pc_room_status,
                true, player->GetLevel()) == false)
        {
            *item_result = RC::RC_ITEM_CAN_NOT_EQUIP;
            return eSwap_Stop;
        }
    };
    return eSwap_Process;
#endif
}

//==================================================================================================

bool nsSlot::ItemMoveImpl::MoveDefault(RC::eITEM_RESULT* item_result,
    CacheArg* const from, CacheArg* const to)
{
    // NOTE: f110418.2L
    if (CheckEnableSwapWhenSwapEquipments(item_result, from, to) == eSwap_Stop)
    {
        assert(*item_result != RC::RC_ITEM_SUCCESS);
        return false;
    };
    //
    SCItemSlot move_item_slot;

#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
    if (from->DeleteItem(&move_item_slot) == FALSE) {
        *item_result = RC::RC_ITEM_UNKNOWNERROR;
        return false;
    }
#else
    if (from->container->DeleteSlot(from->pos_in_slots, &move_item_slot) == false) {
        *item_result = RC::RC_ITEM_UNKNOWNERROR;
        return false;
    }
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    //���� ���������̳� ���� �̵� �ÿ��� ���������ʴ´�
    if (from->slot_index != to->slot_index)
    {
        PlayerSpaInfo* spa_info = root_->player_->GetPlayerSpaInfo();
        spa_info->CheckAndResetSpaTicket(from->pos_in_slots);
    }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

    if (to->slot_exist == false)
    {
#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
        // �� ���� �̵�
        RC::eSLOT_INSERT_RESULT rt = to->InsertItem(move_item_slot);
        if (RC::RC_INSERT_SLOT_SUCCESS != rt)
        {
            // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
            ItemManager::TransactionClosure transaction(root_,
                ItemManager::TransactionClosure::eTransType_ForcedEquipWithoutLimitation);
            //
            from->InsertItem(move_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(rt);
            return false;
        }
#else//-------------------------------------
        // �� ���� �̵�
        RC::eSLOT_INSERT_RESULT rt = to->container->InsertSlot(to->pos_in_slots, move_item_slot);
        if (RC::RC_INSERT_SLOT_SUCCESS != rt)
        {
            // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
            ItemManager::TransactionClosure transaction(root_,
                ItemManager::TransactionClosure::eTransType_ForcedEquipWithoutLimitation);
            //
            from->container->InsertSlot(from->pos_in_slots, move_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(rt);
            return false;
        }
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE
    }
    else
    {
        // ��ȯ
        SCItemSlot exchange_item_slot;
#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
        if (!to->DeleteItem(&exchange_item_slot))
        {
            // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
            ItemManager::TransactionClosure transaction(root_,
                ItemManager::TransactionClosure::eTransType_ForcedEquipWithoutLimitation);
            //
            from->InsertItem(move_item_slot);
            *item_result = RC::RC_ITEM_UNKNOWNERROR;
            return false;
        }
        //
        RC::eSLOT_INSERT_RESULT rt = from->InsertItem(exchange_item_slot);
        if (RC::RC_INSERT_SLOT_SUCCESS != rt)
        {
            // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
            ItemManager::TransactionClosure transaction(root_,
                ItemManager::TransactionClosure::eTransType_ForcedEquipWithoutLimitation);
            //
            from->InsertItem(move_item_slot);
            to->InsertItem(exchange_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(rt);
            return false;
        }
        //
        rt = to->InsertItem(move_item_slot);
        if (RC::RC_INSERT_SLOT_SUCCESS != rt)
        {
            // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
            ItemManager::TransactionClosure transaction(root_,
                ItemManager::TransactionClosure::eTransType_ForcedEquipWithoutLimitation);
            //
            from->DeleteItem(NULL);

            from->InsertItem(move_item_slot);
            to->InsertItem(exchange_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(rt);
            return false;
        }
#else
        if (!to->container->DeleteSlot(to->pos_in_slots, &exchange_item_slot))
        {
            // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
            ItemManager::TransactionClosure transaction(root_,
                ItemManager::TransactionClosure::eTransType_ForcedEquipWithoutLimitation);
            //
            from->container->InsertSlot(from->pos_in_slots, move_item_slot);
            *item_result = RC::RC_ITEM_UNKNOWNERROR;
            return false;
        }
        //
        RC::eSLOT_INSERT_RESULT rt = from->container->InsertSlot(from->pos_in_slots, exchange_item_slot);
        if (RC::RC_INSERT_SLOT_SUCCESS != rt)
        {
            // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
            ItemManager::TransactionClosure transaction(root_,
                ItemManager::TransactionClosure::eTransType_ForcedEquipWithoutLimitation);
            //
            from->container->InsertSlot(from->pos_in_slots, move_item_slot);
            to->container->InsertSlot(to->pos_in_slots, exchange_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(rt);
            return false;
        }
        //
        rt = to->container->InsertSlot(to->pos_in_slots, move_item_slot);
        if (RC::RC_INSERT_SLOT_SUCCESS != rt)
        {
            // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
            ItemManager::TransactionClosure transaction(root_,
                ItemManager::TransactionClosure::eTransType_ForcedEquipWithoutLimitation);
            //
            from->container->DeleteSlot(from->pos_in_slots, NULL);

            from->container->InsertSlot(from->pos_in_slots, move_item_slot);
            to->container->InsertSlot(to->pos_in_slots, exchange_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(rt);
            return false;
        }
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE
    }
    //----------------------------------------------------------------------------------------------
    // NOTE: f110429.1L, at this point, the item moving action completed.
    // (this changes are post reflection based on problem fixed sun:r53511)
    // the related state diagram is as in the following.
    //  #1. from(exist)  -> to(empty) : move
    //  #2. from(exist) <-> to(exist) : swap
    // 'current_from' is the moved 'to' information, and 'current_to' is the moved 'from' information.
    // can't use the slot information anymore like a (from/to)->item_slot.
    CacheArg* const current_from = to; // current 'to'
    CacheArg* const current_to = from; // current 'from'
    //
#ifdef _NA_001990_ACSYSTEM_ADD
    if ((from->slot_index == SI_INVENTORY && to->slot_index == SI_EQUIPMENT) ||
        (from->slot_index == SI_EQUIPMENT && to->slot_index == SI_INVENTORY))
    {
#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
        if (current_to->slot_exist)
        {
            if (WORD ac_code = current_to->item_info->m_wACCode)
            {
                root_->player_->GetACManager()->ProcessItemEquip(current_to->item_info->m_Code,
                    ac_code, current_to->item_info->m_byACReferenceID);
            }
        }
#else
        if (WORD ac_code = current_to->item_info->m_wACCode)
        {
            root_->player_->GetACManager()->ProcessItemEquip(current_to->item_info->m_Code,
                ac_code, current_to->item_info->m_byACReferenceID);
        }
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE
            
        if (current_from->slot_exist)
        {
            if (WORD ac_code = current_from->item_info->m_wACCode)
            {
                root_->player_->GetACManager()->ProcessItemEquip(current_from->item_info->m_Code,
                    ac_code, current_from->item_info->m_byACReferenceID);
            }
        }
#ifdef _NA_002935_20110704_ITEM_SCORE
        // ������ �� ������ �޼��ߴ��� �˻��Ѵ�.
        CEquipmentSlotContainer* const equip_container = static_cast<CEquipmentSlotContainer* const>(
            root_->GetItemSlotContainer(SI_EQUIPMENT));
        const ns_formula::EquipSlotsManageInfo* const equip_manage_info = 
            equip_container->GetEquipmentManageInfo(); 
        const ns_formula::EquipSlotsManageInfo::ItemScoreInfo& item_score_info = 
            equip_manage_info->item_score_info;
        AchievementManager* const ac_manager = root_->player_->GetACManager();
        ac_manager->ProcessItemScore(item_score_info.GetTotalScore());
#endif // _NA_002935_20110704_ITEM_SCORE
    }
#endif //
    //
    *item_result = RC::RC_ITEM_SUCCESS;
    return true;
}

//==================================================================================================

bool nsSlot::ItemMoveImpl::MoveDivision(RC::eITEM_RESULT* item_result,
    CacheArg* const from, CacheArg* const to,
    const DURATYPE move_quantity)
{
    // �̵� ������ �˻��Ѵ�.
    if (move_quantity > from->item_slot->GetNum())
    {
        *item_result = RC::RC_ITEM_INVALID_VALUE;
        return false;
    }

    // ���� ���� �̵����� �˻��Ѵ�.
    if (to->slot_exist == true)
    {
        // ���� �̵��ÿ��� ��ȯ�� �������� �ʴ�. ��ȯ�� ��ġ�� �̹� �������� �����ϱ� �����̴�.
        *item_result = RC::RC_ITEM_INVALIDPOS;
        return false;
    }

    // �κ��丮���� â��� ���� �̵��Ѵٸ� â���ʿ��� ������ �ø����� �߱� �޴°��� ��������
    // �κ����� �������� ���� �� â�� �����Ѵ��� ���� ������ŭ �������� �����ؼ�
    // �κ��ʿ� �����ϴ� ����� ����Ѵ�.
    const bool is_move_from_inventory_to_warehouse_all =
        ((from->slot_index == SI_INVENTORY) && 
         (to->slot_index == SI_WAREHOUSE || to->slot_index == SI_GUILDWAREHOUSE));
    if (is_move_from_inventory_to_warehouse_all) // �κ��丮���� â��� ���� �̵��Ѵٸ�
    {
        // �κ����� �������� �����Ѵ�.
        SCItemSlot move_item_slot;
#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
        if (!from->DeleteItem(&move_item_slot))
        {
            *item_result = RC::RC_ITEM_UNKNOWNERROR;
            return false;
        }
#else
        if (!from->container->DeleteSlot(from->pos_in_slots, &move_item_slot))
        {
            *item_result = RC::RC_ITEM_UNKNOWNERROR;
            return false;
        }
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE

        const DURATYPE from_quantity = move_item_slot.GetNum();
        const DURATYPE remain_quantity = from_quantity - move_quantity;
            
        // ������ �����ۿ� �̵��� ������ ������ ��
        move_item_slot.SetNum(move_quantity);

        // â���ʿ� ������ �������� �����Ѵ�.
#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
        const RC::eSLOT_INSERT_RESULT to_insert_result =
            to->InsertItem(move_item_slot);
        if (to_insert_result != RC::RC_INSERT_SLOT_SUCCESS)
        {
            // �����ϸ� �ǵ�����.
            move_item_slot.SetNum(from_quantity);
            from->InsertItem(move_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(to_insert_result);
            return false;
        }
#else
        const RC::eSLOT_INSERT_RESULT to_insert_result =
            to->container->InsertSlot(to->pos_in_slots, move_item_slot);
        if (to_insert_result != RC::RC_INSERT_SLOT_SUCCESS)
        {
            // �����ϸ� �ǵ�����.
            move_item_slot.SetNum(from_quantity);
            from->container->InsertSlot(from->pos_in_slots, move_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(to_insert_result);
            return false;
        }
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE

        // �̵� �� ���� ������ŭ �������� ����
        SCItemSlot temp_item_slot;
        temp_item_slot.Copy(move_item_slot); 
        temp_item_slot.SetNum(remain_quantity);
        temp_item_slot.SetSerial(TEMP_DBSERIAL_VALUE);
            
        // �κ��ʿ� �����Ѵ�.
#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
        const RC::eSLOT_INSERT_RESULT from_insert_result =
            from->InsertItem(temp_item_slot);
        if (from_insert_result != RC::RC_INSERT_SLOT_SUCCESS)
        {
            // �����ϸ� �ǵ�����.
            to->DeleteItem(NULL);
            move_item_slot.SetNum(from_quantity);
            from->InsertItem(move_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(from_insert_result);
            return false;
        }
#else
        const RC::eSLOT_INSERT_RESULT from_insert_result =
            from->container->InsertSlot(from->pos_in_slots, temp_item_slot);
        if (from_insert_result != RC::RC_INSERT_SLOT_SUCCESS)
        {
            // �����ϸ� �ǵ�����.
            to->container->DeleteSlot(to->pos_in_slots, NULL);
            move_item_slot.SetNum(from_quantity);
            from->container->InsertSlot(from->pos_in_slots, move_item_slot);
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(from_insert_result);
            return false;
        }
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE
    }
    else // â��� �̵��ϴ°��� �ƴ϶��
    {
        // �̵� ������ ����� �ӽ� ������ ������ ����
        SCItemSlot temp_item_slot = *from->item_slot;
        //temp_item_slot.Copy(const_cast<SCItemSlot&>(*from->item_slot)); 
        temp_item_slot.SetNum(move_quantity);
        temp_item_slot.SetSerial(TEMP_DBSERIAL_VALUE);

        // �����ϰ�
#ifdef _NA_008182_20150402_MODIFY_ITEM_MOVE
        const RC::eSLOT_INSERT_RESULT insert_result = to->InsertItem(temp_item_slot);
#else
        const RC::eSLOT_INSERT_RESULT insert_result =
            to->container->InsertSlot(to->pos_in_slots, temp_item_slot);
#endif //_NA_008182_20150402_MODIFY_ITEM_MOVE
        if (insert_result != RC::RC_INSERT_SLOT_SUCCESS)
        {
            *item_result = SlotInsertResultConverter::ConvertToeItemResult(insert_result);
            return false;
        }

        // ���������� �̵� �������� �̵� ������ŭ �ٿ��ش�.
        from->container->UpdateSlot(from->pos_in_slots, -move_quantity);
    }
    //
    *item_result = RC::RC_ITEM_SUCCESS;
    return true;
}

//==================================================================================================
