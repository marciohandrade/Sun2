#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"
#include "GuildWareHouseSlotContainer.h"
#include "ChaoState.h"
#include "GameGuildManager.h"
#include "Status/StatusManager.h"

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "PacketStruct_DZ.h"
#include "GameGuild.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionPlayerProperty) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionPlayerProperty::InitStatItemUse(
    POSTYPE ItemPos, WORD&  OUT rDecreaseValue)
{
#if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
    //��� ��� ���� �Ͽ��°�?
    if (!root_->IsUnEquipAll())
        return RC::RC_ITEM_NOT_UNEQUIP_ALL;
#endif
    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (!root_->ValidPos(SI_INVENTORY, ItemPos))
        return RC::RC_ITEM_INVALIDPOS;  
    // CHANGES: f110701.2L, added the rule is to prevent irregula stat calcuation
    // like the dragon transformed state.
    if (root_->player_->GetStatusManager()->Status.IsDragonTransforming()) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_TRANSFORMATION;
    };

    SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(ItemPos);
    if (!rItemSlot.GetItemInfo())
        return RC::RC_ITEM_NOINFO;

    //��Ÿ�� üũ
    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rItemSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;

    //�ʱ�ȭ �������ΰ�?
    if (rItemSlot.GetItemInfo()->m_wType != eITEMTYPE_STAT_RESET)
        return RC::RC_ITEM_INVALID_TYPE;

    //��ü �ʱ�ȭ �������� ������ ����� �� ����.
    if ((rItemSlot.GetItemInfo()->m_byWasteType == eItemWASTE_INIT_ALL ||
        rItemSlot.GetItemInfo()->m_byWasteType == eItemWASTE_INIT_BASIC_STAT_ALL ||
        rItemSlot.GetItemInfo()->m_byWasteType == eItemWASTE_INIT_EXPERTY_ALL ||
        rItemSlot.GetItemInfo()->m_byWasteType == eItemWASTE_INIT_PASSIVE_ALL)      
        && rDecreaseValue != 1)
    {
        return RC::RC_ITEM_CANNOT_USE_MULTI_ALL_ITEM;
    }

    //�ش� ���� ��ŭ �����ϴ°�?
    if (rDecreaseValue > rItemSlot.GetNum())
        return RC::RC_ITEM_LESS_FOR_USE;    

    //���� �ʱ�ȭ
    if (!(root_->player_->InitStat(rItemSlot.GetItemInfo()->m_byWasteType, rDecreaseValue)))
        return RC::RC_ITEM_NOT_MORE_DECREASE;

    GAMELOG->LogItem(ITEM_USE, root_->player_, &rItemSlot);

    WORD wCoolTimeType = rItemSlot.GetItemInfo()->m_wCoolTimeType;

    //������ ����
    if (rDecreaseValue >= rItemSlot.GetNum())
    {
        BOOL rt = pContainer->DeleteSlot(ItemPos, NULL);
        ASSERT(rt && "User():���� ���� �����̳ʿ��� ������ �� ���� �������Դϴ�.\n");
    }
    else
    {
        pContainer->UpdateSlot(rItemSlot.GetPos(), -rDecreaseValue);
    }

    root_->player_->ResetUseItem(wCoolTimeType);

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//Use��Ŷ
RC::eITEM_RESULT nsSlot::ItemFunctionPlayerProperty::ExtendCashSlot(SCItemSlot & rItemSlot)
{
    BASE_PLAYERINFO * pInfo = root_->player_->GetCharInfo();
    if (pInfo->m_ExtraEquipCashSlotCount >= MAX_CHARGE_EQUIPMENT_SLOT_NUM)
    {
        return RC::RC_ITEM_CANNOT_EXTEND_CASH_SLOT;
    }
    else
    {
        pInfo->m_ExtraEquipCashSlotCount = min(pInfo->m_ExtraEquipCashSlotCount+2,
                                               MAX_CHARGE_EQUIPMENT_SLOT_NUM);

        MSG_CG_ITEM_EXTEND_CASH_SLOT_CMD cmd;
        cmd.m_ExtraEquipCashSlotCount = pInfo->m_ExtraEquipCashSlotCount;
        root_->player_->SendPacket(&cmd, sizeof(cmd));
    }

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//Use ��Ŷ
RC::eITEM_RESULT nsSlot::ItemFunctionPlayerProperty::ExtendGuildWarehouseCashSlot(
    SLOTIDX atIndex, POSTYPE atPos)
{
    CGuildWarehouseSlotContainer* pGuildWareHouse =
        g_GameGuildManager.GetGuildWarehouse(root_->player_->GetGuildGuid());
    if (!pGuildWareHouse) return RC::RC_ITEM_GUILDWAREHOUSE_NONE_USENOT_CASHITEM;

    RC::eITEM_RESULT result = pGuildWareHouse->ExtendWarehouseTabCount(root_->player_);
    if (RC::RC_ITEM_SUCCESS == result) 
    {
        MSG_DG_GUILD_WAREHOUSE_TAPEX_SYN msg;
        msg.m_SlotPos = atPos;
        msg.m_SlotIndex = atIndex;
        msg.m_TabCount = 2;
        msg.m_dwKey = root_->player_->GetUserGuid();
        msg.m_GuildGuid = root_->player_->GetGuildGuid();
        msg.m_CharGuid = root_->player_->GetCharGuid();
        root_->player_->SendToGameDBPServer(&msg, sizeof(msg));
        return result;
    }

    MSG_CG_GUILD_WAREHOUSE_TAPEX_NAK msg;
    msg.m_bErrorCode = result;
    root_->player_->SendPacket(&msg, sizeof(msg));
    return result;
}

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
//{__NA_1323_CHAO_TIME_DECREASE_ITEM
RC::eITEM_RESULT nsSlot::ItemFunctionPlayerProperty::UseChaoDecItem(const BASE_ITEMINFO* pInfo)
{
    if (root_->player_->GetChaoState().GetChaoState() != eCHAO_STATE_PREV_CHAO &&
        root_->player_->GetChaoState().GetChaoState() != eCHAO_STATE_CHAO)
    {
        return RC::RC_ITEM_CAN_USE_ONLY_CHAO;
    };
    DWORD dwDecreaseTime = pInfo->m_byChaoTimeDec * 60 * 60;
    if (root_->player_->GetChaoState().GetChaoEndTime() > dwDecreaseTime)
    {
        root_->player_->GetChaoState().SetChaoEndTime(
            root_->player_->GetChaoState().GetChaoEndTime() - dwDecreaseTime);
    }
    else
    {
        time_t cur_time;    util::TimeSync::time(&cur_time);
        root_->player_->GetChaoState().SetChaoEndTime(cur_time);
    }

    return RC::RC_ITEM_SUCCESS;
}
//}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
RC::eITEM_RESULT nsSlot::ItemFunctionPlayerProperty::InitGuildFacility( POSTYPE ItemPos, WORD decrease_value )
{
    SCSlotContainer * pContainer = root_->GetItemSlotContainer(SI_INVENTORY);

    if (!root_->ValidState()) 
        return RC::RC_ITEM_INVALIDSTATE;

    if (!root_->ValidPos(SI_INVENTORY, ItemPos))
        return RC::RC_ITEM_INVALIDPOS;

    SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(ItemPos);
    if (!rItemSlot.GetItemInfo())
        return RC::RC_ITEM_NOINFO;

    //��Ÿ�� üũ
    RC::eITEM_RESULT rcResult = root_->player_->CanUseItem(rItemSlot.GetItemInfo());
    if (rcResult != RC::RC_ITEM_SUCCESS)
        return rcResult;

    //��� �ü� �ʱ�ȭ �������ΰ�?
    if (rItemSlot.GetItemInfo()->m_wType != eITEMTYPE_GUILDFACILITY_RESET)
        return RC::RC_ITEM_INVALID_TYPE;

    //�ش� ���� ��ŭ �����ϴ°�?
    if (decrease_value > rItemSlot.GetNum())
        return RC::RC_ITEM_LESS_FOR_USE;    

    GameGuild* game_guild = g_GameGuildManager.FindGuild(root_->player_->GetGuildGuid());
    if (game_guild == NULL)
    {
        return RC::RC_ITEM_GUILDFACILITY_NOT_GUILD_MEMBER;
    }

    //���� üũ
    Player* master = game_guild->GetGuildMaster();
    if (master == NULL)
    {
        return RC::RC_ITEM_GUILDFACILITY_NONE_USE_RIGHT;
    }
    if (master->GetCharGuid() != root_->player_->GetCharGuid())
    {
        return RC::RC_ITEM_GUILDFACILITY_NONE_USE_RIGHT;
    }

    WORD wCoolTimeType = rItemSlot.GetItemInfo()->m_wCoolTimeType;

    //������ ����
    if (decrease_value >= rItemSlot.GetNum())
    {
        if (pContainer->DeleteSlot(ItemPos, NULL) == false)
        {
            return RC::RC_ITEM_LESS_FOR_USE;    
        }
    }
    else
    {
        pContainer->UpdateSlot(rItemSlot.GetPos(), -decrease_value);
    }

    root_->player_->ResetUseItem(wCoolTimeType);

    MSG_DZ_GUILD_FACILITY_INIT_SYN syn_msg;
    syn_msg.guild_guid = root_->player_->GetGuildGuid();
    root_->player_->SendToGameDBPServer(&syn_msg, sizeof(syn_msg));

    return RC::RC_ITEM_SUCCESS;
}
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION