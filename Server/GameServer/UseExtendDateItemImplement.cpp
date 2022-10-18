#include "StdAfx.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

#include "UseExtendDateItemImplement.h"
#include <SCItemSlotContainer.h>
#include <ItemTypeList.h>
#include ".\ItemManager.h"
#include ".\Player.h"


UseExtendDateItemImplement::UseExtendDateItemImplement()
{
}

UseExtendDateItemImplement::~UseExtendDateItemImplement()
{
}

void UseExtendDateItemImplement::Implement(MSG_CG_ITEM_USE_EXTEND_DATE_SYN* recv_message)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(player == NULL) 
        return ;

    if(!player->GetField())
    {
        SUNLOG( eCRITICAL_LOG, "[Handler_CG_ITEM_IMPL( CG_ITEM_USE_EXTEND_DATE_SYN )] Not Exist GameField" );
        return;
    }

    RC::eITEM_RESULT result = Use_Extend_Date_Applier(player, 
                                                      recv_message->extend_item_position_,
                                                      recv_message->target_item_position_);

    if (result != RC::RC_ITEM_SUCCESS)
    {
        MSG_CG_ITEM_USE_EXTEND_DATE_NAK nak_message;
        nak_message.error_code_ = result;
        player->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }
}


RC::eITEM_RESULT UseExtendDateItemImplement::Use_Extend_Date_Applier(Player* player, 
                                                                     POSTYPE applier_position, 
                                                                     POSTYPE target_position)
{
    ItemManager* item_manager = player->GetItemManager();
    if (!item_manager->ValidPos(SI_INVENTORY, applier_position, true))
        return RC::RC_ITEM_INVALIDPOS;

    if (!item_manager->ValidPos(SI_INVENTORY, target_position, true))
        return RC::RC_ITEM_INVALIDPOS;

    SCSlotContainer* slot_container = item_manager->GetItemSlotContainer(SI_INVENTORY);
    if (!slot_container)
        return RC::RC_ITEM_INVALIDPOS;

    SCItemSlot& target_item_slot = (SCItemSlot&)slot_container->GetSlot(target_position);
    const BASE_ITEMINFO* target_item_info = target_item_slot.GetItemInfo();
    if (!target_item_info)
        return RC::RC_ITEM_FAILED;

    SCItemSlot& applier_item_slot = (SCItemSlot&)slot_container->GetSlot(applier_position);
    const BASE_ITEMINFO* applier_item_info = applier_item_slot.GetItemInfo();
    if (!applier_item_info)
        return RC::RC_ITEM_FAILED;

    if (applier_item_info->m_wType != eITEMTYPE_WASTE)
        return RC::RC_ITEM_INVALID_TYPE;

    if (target_item_slot.CanUseMixture(MIXTURE_PERIOD_EXTEND) == false)
        return RC::RC_ITEM_IMPOSSIBLE_PERIOD_EXTEND;

    // 정액제(대기) 타입인지 체크
    if (!target_item_info->CanPeriodExtend()) 
        return RC::RC_ITEM_CANNOT_EXTEND_DATE;

    // 기간이 다된 아이템만 기간 연장을 할 수 있다.
    if (!target_item_slot.IsExpiredWaitTime()) 
        return RC::RC_ITEM_CANNOT_EXTEND_DATE;

    // BASE_ITEMINFO::m_MaterialCode 에 연장할 날짜가 설정되어있다.
    DWORD64 day = applier_item_info->m_MaterialCode;
    if (day == 0)
    {
        SUNLOG(eCRITICAL_LOG, "Use_Extend_Date_Applier : day => 0");
        return RC::RC_ITEM_CANNOT_EXTEND_DATE;
    }


    bool extend_date_flag = false;
    switch (applier_item_info->m_byWasteType)
    {
    case eITEMWASTE_EXTEND_DATE_WEAPON:
        {
            if (!target_item_info->IsWeapon())
                break;

            if (target_item_info->m_wEqPos == eEQUIPCONTAINER_WEAPON) 
                extend_date_flag = true;
        }
        break;
    case eITEMWASTE_EXTEND_DATE_ARMOR:
        {
            if (!target_item_info->IsArmor())
                break;

            if (target_item_info->m_wEqPos >= eEQUIPCONTAINER_ARMOR &&
                target_item_info->m_wEqPos <= eEQUIPCONTAINER_SACCESSORY3)
                extend_date_flag = true;
        }
        break;
    case eITEMWASTE_EXTEND_DATE_RIDER:
        {
            if (!target_item_info->IsRiderItem())
                break;
            extend_date_flag = true;
        }
        break;
    case eITEMWASTE_EXTEND_DATE_RIDER_PARTS:
        {
            if (!target_item_info->IsRiderPartsItem())
                break;

            if (eRIDER_PARTS_POS::StartIndex <= target_item_info->m_wEqPos &&
                target_item_info->m_wEqPos < eRIDER_PARTS_POS::EndIndex)
                extend_date_flag = true;
        }
        break;
    default:
        break;
    }

    if (extend_date_flag == false)
        return RC::RC_ITEM_INVALID_TYPE;


    GAMELOG->LogItem(ITEM_USE, player, 1, &applier_item_slot, &target_item_slot);

    // 기간 연장 아이템 감소 또는 삭제
    if( applier_item_slot.IsOverlap() && ( applier_item_slot.GetNum() > 1 ) )
        slot_container->UpdateSlot(applier_position, -1);
    else
    {
        // 아이템 삭제 로그
        GAMELOG->LogItem(ITEM_DELETE, player, &applier_item_slot, 0, applier_item_slot.GetNum());
        slot_container->DeleteSlot(applier_position, NULL);
    }

    // 기간을 연장한다.
    DWORD64 extend_date = 86400 * day; // (86400 = 60*60*24, 1일을 초단위로 환산)
    target_item_slot.SetDateTime(extend_date);
    target_item_slot.SetExpiredWaitTime(false);


    // 아이템 슬롯정보 변화를 알린다.
    MSG_CG_ITEM_USE_EXTEND_DATE_ACK ack_message;
    ack_message.extend_item_position_ = applier_position;
    ack_message.target_item_position_ = target_item_slot.GetPos();
    target_item_slot.CopyOut(ack_message.target_item_stream_);
    player->SendPacket(&ack_message, sizeof(ack_message));

    return RC::RC_ITEM_SUCCESS;
}
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
