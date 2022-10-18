#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"
#include "InventorySlotContainer.h"
#include "SkillAttrCalculator.h"

// eITEMTYPE_RENDER_EFFECT_ITEM : 뱃지
// eITEMTYPE_PCBANG : PC방효과

#ifdef _NA_006372_20130108_BADGE_ITEM
bool nsSlot::ItemFunctionToggle::Use( const SLOTCODE item_code, const POSTYPE item_pos )
{
    CInventorySlotContainer* const inventory_container = \
        static_cast<CInventorySlotContainer*>(root_->GetItemSlotContainer(SI_INVENTORY));    
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(inventory_container->GetSlot(item_pos));
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();

    if ( item_info == NULL || CheckItemType(item_info) == false) {
        return false;
    }

    // 배찌 클래스 검증..
    const eCHAR_TYPE char_type = static_cast<eCHAR_TYPE>(root_->player_->GetCharInfo()->m_byClassCode);
    if (item_slot.GetItemInfo()->CheckValidEquipClass(char_type) == false)
    {
        return false;
    }

    nsSlot::TOGGLE_ITEMS* const _toggle_item_info = \
        root_->player_->GetExtraInfoManager().GetToggleItemInfo(toggle_item_type_);

    if (_toggle_item_info == NULL) {
        return false;
    }
    //
    TOGGLE_ITEMS& toggle_item_info = (*_toggle_item_info);

    if (toggle_item_info.is_equiped_ == true)
    {
        // 이미 착용 중..
        POSTYPE previous_item_pos = toggle_item_info.item_pos_;
        SCItemSlot& previous_item_slot = \
            static_cast<SCItemSlot&>(inventory_container->GetSlot(previous_item_pos));
        const BASE_ITEMINFO* previous_item_info = previous_item_slot.GetItemInfo();
        if (previous_item_info == NULL || CheckItemType(previous_item_info) == false)
        {
            toggle_item_info.clear();
            return false;
        }

        // 착용 해제..
        Equip(previous_item_slot, false);
        
        if (previous_item_pos == item_pos) {
            return true;    
        }
    };

    Equip(item_slot, true);

    return true;
}

bool nsSlot::ItemFunctionToggle::Equip( POSTYPE item_pos, const bool is_equipped)
{
    CInventorySlotContainer* const inventory_container = \
        static_cast<CInventorySlotContainer*>(root_->GetItemSlotContainer(SI_INVENTORY));    
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(inventory_container->GetSlot(item_pos));
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();

    if (item_info == NULL || CheckItemType(item_info) == false) {
        return false;
    }

    return Equip(item_slot, is_equipped);
}

bool nsSlot::ItemFunctionToggle::Equip( SCItemSlot& item_slot, const bool is_equipped )
{
    CInventorySlotContainer* const inventory_container = \
        static_cast<CInventorySlotContainer*>(root_->GetItemSlotContainer(SI_INVENTORY));  

    if (is_equipped) {
        inventory_container->ApplyEffect(item_slot, 1, 1);
        item_slot.SetUserBlocked(TRUE);
        item_slot.OnEquip();
    }
    else {
        inventory_container->ApplyEffect(item_slot, -1, 0);
        item_slot.SetUserBlocked(FALSE);
        item_slot.OnUnEquip();
    }

    TOGGLE_ITEMS* const _toggle_item_info = \
        root_->player_->GetExtraInfoManager().GetToggleItemInfo(toggle_item_type_);

    if (_toggle_item_info == NULL) {
        return false;
    }
    //
    TOGGLE_ITEMS& toggle_item_info = (*_toggle_item_info);

    if(is_equipped) {
        toggle_item_info.is_equiped_ = true;
        toggle_item_info.item_pos_   = item_slot.GetPos();
        toggle_item_info.item_code_  = item_slot.GetItemCode();
        toggle_item_info.toggle_item_type_ = toggle_item_type_;
    }
    else {
        toggle_item_info.clear();
    }

    ;{
        // 클라전송..
        MSG_CG_ITEM_USE_TOGGLE_ACK ack_msg;
        ack_msg.toggle_item_type_ = toggle_item_type_;
        ack_msg.is_equipped_      = toggle_item_info.is_equiped_;
        ack_msg.item_pos_         = toggle_item_info.item_pos_;
        root_->player_->SendPacket(&ack_msg, sizeof(ack_msg));
    };
    
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info->m_wType == eITEMTYPE_RENDER_EFFECT_ITEM)
    {
        Player* pPlayer = root_->player_;

        VarMsg2* const pVarMsgSRender = &(Player::m_VarMsg2[Player::SENDINFO_PLAYER_SPECIAL_RENDER]);
        MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD*	pSpecialRender = \
            pVarMsgSRender->StartTypePtr(Param2Class<MSG_CG_STATUS_RENDER_EFFECT_ITEM_LIST_BRD>());

        pVarMsgSRender->SetSize( pSpecialRender->GetSize() );
        pSpecialRender->m_Count = 1;	// 1명 대상
        sMSG_RENDER_EFFECT_ITEM_LIST* pEffectNode = \
            (sMSG_RENDER_EFFECT_ITEM_LIST*)pVarMsgSRender->NextPtr();
        pEffectNode->m_PlayerKey = pPlayer->GetObjectKey(); // _NA_007810_20141112_MODIFY_MSG_RENDER_EFFECT_ITEM_LIST_PACKET
        const DWORD alignedCount = pPlayer->RENDER_EFFECT.SerializeRenderInfo( \
            pEffectNode->m_BitRender, pEffectNode->m_ItemCodes, \
            sMSG_RENDER_EFFECT_ITEM_LIST::MAX_EFFECT_ITEMS );
        pEffectNode->m_ItemCount = (BYTE)alignedCount;
        pVarMsgSRender->SetSize( pEffectNode->GetSize() );

        pPlayer->SendExPacketAround( pVarMsgSRender->GetMsgCount(), \
            pVarMsgSRender->GetMsgPPtr(), pVarMsgSRender->GetMsgSizePtr(), FALSE );
    };
//!~ _NA_006413_20130201_PREMIUMSERVICE_ITEM
    if (item_info->m_wType == eITEMTYPE_PCBANG)
    {
        Player* pPlayer = root_->player_;

        MSG_AG_CHARINFO_PREMIUMSERVICE_BILL_CMD cmd;
        cmd.m_dwKey = pPlayer->GetUserGuid();
        cmd.premiumservice_ = pPlayer->GetPlayerAttr().GetAttrValue(eATTR_PREMIUMSERVICE_PCBANG);
        g_pGameServer->SendToServer(AGENT_SERVER, &cmd, sizeof(cmd));
    };
//~!_NA_006413_20130201_PREMIUMSERVICE_ITEM

    // 로그를 기록한다.
    ;{
        TCHAR add_info[MAX_ADDINFO_SIZE] = { 0 };
        ::_sntprintf(add_info, MAX_ADDINFO_SIZE, _T("ACTIVE:%u"), (is_equipped ? 1 : 0));
        add_info[MAX_ADDINFO_SIZE-1] = _T('\0');
        GAMELOG->LogItem(ITEM_USE_TOGGLE, root_->player_, &item_slot, 0, 1, NULL, NULL, add_info);
    };

    return true;
}

bool nsSlot::ItemFunctionToggle::CheckItemType( const BASE_ITEMINFO* item_info )
{
    if (item_info == NULL) {
        return false;
    }

    switch(toggle_item_type_)
    {
        case kToggleItem_Badge:
            {
                if (item_info->m_wType == eITEMTYPE_RENDER_EFFECT_ITEM)
                    return true;
            } break;
        case kToggleItem_PCBang:
            {
                if (item_info->m_wType == eITEMTYPE_PCBANG)
                    return true;
            }
    }

    return false;
}
#endif //_NA_006372_20130108_BADGE_ITEM