#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionCustomization) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use_EXCEPTION_PACKET.cpp
//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
RC::eITEM_RESULT nsSlot::ItemFunctionCustomization::UseChangeCharacterAppearanceItem(
    const POSTYPE item_position, 
    CharacterAppearanceInfo& INOUT character_appearance) // 캐릭터 외모 변경 아이템을 사용한다.
{
    // 캐릭터가 아이템을 사용할 수 있는 상태인지 검사한다.
    if (!root_->ValidState())
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }

    // 인벤토리의 아이템 위치가 유효한지 검사한다.
    if (!root_->ValidPos(SI_INVENTORY, item_position))
    {
        return RC::RC_ITEM_INVALIDPOS;	
    }

    SCSlotContainer* const item_slot_container = root_->GetItemSlotContainer(SI_INVENTORY);
    const SCItemSlot& item_slot = 
        static_cast<const SCItemSlot&>(item_slot_container->GetSlot(item_position));

    // 아이템 정보를 찾을수 있는지 검사한다.
    const BASE_ITEMINFO* const item = item_slot.GetItemInfo();
    if (item == NULL)
    {
        return RC::RC_ITEM_NOINFO;
    }

    // 아이템이 소모성인지 검사한다. (기획 명세상 1회 소모성으로 정의됨)
    if (item->IsMaterialWaste() == false)
    {
        return RC::RC_ITEM_INVALID_TYPE;
    }

    // 아이템이 사용가능한 상태인지 검사한다.
    const RC::eITEM_RESULT can_use_result = root_->player_->CanUseItem(item);
    if (can_use_result != RC::RC_ITEM_SUCCESS)
    {
        return can_use_result;
    }

    // 모두 변경 요청을 검사한다.
    if (item->m_wType == eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL)
    {
        if (ChangeCharacterAppearanceTypeConst::IsChangeAll(
                character_appearance.appearance_type_) == false)
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }
    // 얼굴 변경 요청을 검사한다.
    else if (item->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE) 
    {
        if (ChangeCharacterAppearanceTypeConst::IsChangeFace(
                character_appearance.appearance_type_) == false)
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
        if ((item->m_byWasteType != 0) && (item->m_byWasteType != character_appearance.face_))
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }
    // 헤어 변경 요청을 검사한다.
    else if (item->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR) 
    {
        if (ChangeCharacterAppearanceTypeConst::IsChangeHair(
                character_appearance.appearance_type_) == false)
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
        if ((item->m_byWasteType != 0) && (item->m_byWasteType != character_appearance.hair_))
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }
    // 키 변경 요청을 검사한다.
    else if (item->m_wType == eITEMTYPE_CHANGE_CHARACTER_HEIGHT) 
    {
        if (ChangeCharacterAppearanceTypeConst::IsChangeHeight(
                character_appearance.appearance_type_) == false)
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
        if ((item->m_byWasteType != 0) && (item->m_byWasteType != character_appearance.height_))
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }
#ifdef _NA_002050_20110216_ADD_GENDER
    // 성별 변경 요청을 검사한다.
    else if (item->m_wType == eITEMTYPE_CHANGE_CHARACTER_GENDER) 
    {
        if (ChangeCharacterAppearanceTypeConst::IsChangeGender(
                character_appearance.appearance_type_) == false)
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
        if ((item->m_byWasteType != 0) && (item->m_byWasteType != character_appearance.gender))
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }
#endif
    else
    {
        return RC::RC_ITEM_INVALID_TYPE;
    }

    // 아이템 수량을 검사한다.
    if (item_slot.GetNum() < 1)
    {
        return RC::RC_ITEM_LESS_FOR_USE;
    }

    // 추가 로그 정보를 생성한다.
    TCHAR log_add_info[MAX_ADDINFO_SIZE];
    {
        int message_index = 0;
        int number_of_written = 0;
        const BASE_PLAYERINFO* char_info = root_->player_->GetCharInfo();
        typedef ChangeCharacterAppearanceTypeConst TypeChanger;
        if (TypeChanger::IsChangeHair(character_appearance.appearance_type_) && 
            (char_info->m_byHair != character_appearance.hair_))
        {
            number_of_written = ::_sntprintf(
                &log_add_info[message_index], _countof(log_add_info) - message_index,
                _T("h:%u:%u"), char_info->m_byHair, character_appearance.hair_);
            message_index += number_of_written;
        }
        assert(message_index < _countof(log_add_info));
        if (TypeChanger::IsChangeFace(character_appearance.appearance_type_) && 
            (char_info->m_byFace != character_appearance.face_))
        {
            if (message_index != 0) {
                log_add_info[message_index++] = _T(',');
            };
            number_of_written = ::_sntprintf(
                &log_add_info[message_index], _countof(log_add_info) - message_index,
                _T("f:%u:%u"), char_info->m_byFace, character_appearance.face_);
            message_index += number_of_written;
        }
        assert(message_index < _countof(log_add_info));
        if (TypeChanger::IsChangeHeight(character_appearance.appearance_type_) &&
            (char_info->m_byHeight != character_appearance.height_))
        {
            if (message_index != 0) {
                log_add_info[message_index++] = _T(',');
            };
            number_of_written = ::_sntprintf(
                &log_add_info[message_index], _countof(log_add_info) - message_index,
                _T("t:%u:%u"), char_info->m_byHeight, character_appearance.height_);
            message_index += number_of_written;
        }
#ifdef _NA_002050_20110216_ADD_GENDER
        if (TypeChanger::IsChangeGender(character_appearance.appearance_type_) &&
            (char_info->gender != character_appearance.gender))
        {
            if (message_index != 0) {
                log_add_info[message_index++] = _T(',');
            };
            number_of_written = ::_sntprintf(
                &log_add_info[message_index], _countof(log_add_info) - message_index,
                _T("g:%u:%u"), char_info->gender, character_appearance.gender);
        }
#endif
        assert(message_index < _countof(log_add_info));
        log_add_info[_countof(log_add_info) - 1] = _T('\0');
    }
    
    // 캐릭터 외모를 변경한다.
    const RC::eCHANGE_CHARACTER_APPEARANCE_RESULT change_result = 
        root_->player_->ChangeCharacterAppearance(character_appearance);
    if (RC::ChangeCharacterAppearanceResultUtil::IsFail(change_result))
    {
        return RC::RC_ITEM_FAIL_TO_CHANGE_CHARACTER_APPEARANCE;
    }

    // 로그를 기록한다.
    GAMELOG->LogItem(ITEM_USE, root_->player_, &item_slot, 0, 1, NULL, NULL, log_add_info);

    // 소모성 아이템이면 삭제한다.
    if (item->IsMaterialWaste())
    {
        if (item_slot.GetNum() <= 1)
        {
            if (!item_slot_container->DeleteSlot(item_position, NULL))
            {
                ::MessageOut(eCRITICAL_LOG, _T("|%s|")
                    _T("Fail to delete item.|ItemnPosition:%d|ItemType:%d|NumOfItem:%d|"),
                    __FUNCTION__, item_position, item->m_wType, item_slot.GetNum());
            }
        }
        else
        {
            item_slot_container->UpdateSlot(item_position, -1);
        }
    }

    // 쿨타임을 갱신한다.
    root_->player_->ResetUseItem(item->m_wCoolTimeType);

    return RC::RC_ITEM_SUCCESS;
}

