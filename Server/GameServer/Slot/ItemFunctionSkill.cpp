#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"
#include "EquipmentSlotContainer.h"

#include "SkillManager.h"

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionSkill) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
bool nsSlot::ItemFunctionSkill::SkillExecute(
    SLOTCODE skillCode,
    SkillInfo* inputed_skill_info_data,
    const SkillOption& addOption,
    BYTE byAttackPropensity, // reserved replace
    BYTE byEventCode) // reserved replace
{
    if (byEventCode > 0)
    {
        CEquipmentSlotContainer* pContainer =
            static_cast<CEquipmentSlotContainer*>(root_->GetItemSlotContainer(SI_EQUIPMENT));
        if (pContainer->IsEquipEventItem(byEventCode) == 0)
            return false;
    }

    if (inputed_skill_info_data != 0)
    {
        // 스킬 정보가 있는 아이템은 신규 추가된 디버프나 데미지를 주는 아이템이다.
        // 스킬 사용 가능 체크 들어감(봉쇄 등)
        inputed_skill_info_data->skill_code_ = skillCode;
        inputed_skill_info_data->event_code_ = byEventCode; // f100831.4L, added
        inputed_skill_info_data->AlignSkillInfo(skillCode); // to bind a root_skill_info_
        RC::eSKILL_RESULT rc = root_->player_->UseSkill(inputed_skill_info_data, byAttackPropensity);
        if (rc != RC::RC_SKILL_SUCCESS)
            return false;
    }
    else
    {
        // 기존 아이템
        const WzVector* cur_pos = root_->player_->GetPosPtr();
        SkillInfo skill_info_data(root_->player_, skillCode, 0);
        skill_info_data.SetArgsObjectInfo(0, cur_pos, cur_pos, cur_pos);
        skill_info_data.SetArgsSkillExt(skill_info_data.kDefaultSequence,
                                        skill_info_data.kDefaultSkillEffect,
                                        skill_info_data.kDefaultSkillDelay,
                                        SKILL_FACTOR_ITEM,
                                        static_cast<eATTACK_PROPENSITY>(byAttackPropensity),
                                        byEventCode);
        // (WAVERIX) (090709) (NOTE) __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__는 일단 버프용이
        //므로 이곳에서만 처리하도록 한다. 이후 일반적으로 확장하기로 한다. 
        skill_info_data.skill_option_.ApplyOption(addOption);
        root_->player_->GetSkillManager()->RegisterSkill(SKILL_TYPE_ACTIVE, &skill_info_data);
    }

    return true;
}
