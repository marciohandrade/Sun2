#include "SunClientPrecompiledHeader.h"

#include "AutobotCondition.h"
#include "Autobot/Autobot.h"

#include "SCItemSlot.h"
#include "Hero.h"
#include "Pet.h"
#include "Map.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "SkillInfoParser.h"
#include "SkillSlot.h"
#include "BattleScene.h"
#include "ItemManager.h"
#include "Item.h"
#include "Interface/uiPartyMan/uiPartyMan.h"
#include "InventoryDialog.h"
#include "AutomaticInfoParser.h"
#include "ItemInfoParser.h"

bool CanTradeSell(SCItemSlot* pItemSlot, const BASE_ITEMINFO *pItemInfo, eITEM_TRADESELL_TYPE trade_sell_type);

//------------------------------------------------------------------------------
//! AutobotCondition
//------------------------------------------------------------------------------
AutobotCondition::ItemType AutobotCondition::ConvertPickupItemType(const BASE_ITEMINFO* item_info)
{
    if (item_info == NULL)
    {
        return kItem_All;
    }

    if (item_info->IsWeapon() == TRUE)
    {
        return kItem_Weapon;
    }
    else if (item_info->IsArmor() == TRUE)
    {
        return kItem_Armor;
    }
    else if (item_info->m_wType == eITEMTYPE_AC)
    {
        return kItem_AC;
    }
    else if (item_info->m_wType == eITEMTYPE_QUEST)
    {
        return kItem_Quest;
    }
    else if ((item_info->m_wType == eITEMTYPE_ENCHANT) || 
            (item_info->m_wType == eITEMTYPE_CRYSTAL) ||
            (item_info->m_wType == eITEMTYPE_SOCKET_ZARD)
            )
    {
        return kItem_Stone;
    }

    return kItem_Normal;
}
//------------------------------------------------------------------------------ 
AutobotCondition::ItemType AutobotCondition::ConvertPickupItemType(Item& item)
{
    if (item.IsMoney() == TRUE)
    {
        return kItem_Heim;
    }

    return ConvertPickupItemType(item.GetItemSlot().GetItemInfo());
}
//------------------------------------------------------------------------------ 
AutobotCondition::ItemType AutobotCondition::ConvertPickupItemType(eAUTO_HUNTING_TYPE hunting_type)
{
    switch (hunting_type)
    {
        case eAUTO_PICKUP_ALL:       return kItem_All;
        case eAUTO_PICKUP_NORMAL:    return kItem_Normal;
        case eAUTO_PICKUP_QUEST:     return kItem_Quest;
        case eAUTO_PICKUP_STONE:     return kItem_Stone;
        case eAUTO_PICKUP_WEAPON:    return kItem_Weapon;
        case eAUTO_PICKUP_ARMOR:     return kItem_Armor;
        case eAUTO_PICKUP_AC:        return kItem_AC;
        case eAUTO_PICKUP_HEIM:      return kItem_Heim;
        //case eAUTO_PICKUP_ITEMTYPE:        
        case eAUTO_SELL_ALL:         return kItem_All;
        case eAUTO_SELL_NORMAL:      return kItem_Normal;
        case eAUTO_SELL_QUEST:       return kItem_Quest;
        case eAUTO_SELL_STONE:       return kItem_Stone;
        case eAUTO_SELL_WEAPON:      return kItem_Weapon;
        case eAUTO_SELL_ARMOR:       return kItem_Armor;
        case eAUTO_SELL_AC:          return kItem_AC;
    }

    return kItem_Max;
}
//------------------------------------------------------------------------------
//! AutobotCondition_Pickup
//------------------------------------------------------------------------------
void AutobotCondition_Pickup::Clear()
{
    pickupitem_set().clear();
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Pickup::Insert(AutobotCondition::ItemType item_type)
{
    pickupitem_set().insert(item_type);

    return true;
}

//------------------------------------------------------------------------------
void AutobotCondition_Pickup::Remove(AutobotCondition::ItemType item_type)
{
    ConditionPickupItemSet::iterator iter = pickupitem_set().find(item_type);

    if (iter != pickupitem_set().end())
    {
        pickupitem_set().erase(iter);
    }
}
//------------------------------------------------------------------------------
bool AutobotCondition_Pickup::IsPossiblePickup(Item& item)
{
    return IsPossiblePickup(ConvertPickupItemType(item));
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Pickup::IsPossiblePickup(ItemType item_type)
{
    if (pickupitem_set().find(kItem_All) != pickupitem_set().end())
    {
        return true;
    }

    return HasItemType(item_type);
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Pickup::HasItemType(ItemType item_type)
{
    return (pickupitem_set().find(item_type) != pickupitem_set().end());
}

//------------------------------------------------------------------------------ 
void AutobotCondition_Pickup::Save(FILE* fp)
{
    int count = pickupitem_set().size();
    ConditionPickupItemSet::iterator begin = pickupitem_set().begin();
    ConditionPickupItemSet::iterator end = pickupitem_set().end();

    fwrite(&count, sizeof(int), 1, fp);

    for (; begin != end; ++begin)
    {
        AutobotCondition::ItemType item_type = *begin;
        fwrite(&item_type, sizeof(item_type), 1, fp);
    }
}

//------------------------------------------------------------------------------ 
void AutobotCondition_Pickup::Load(FILE* fp, int file_version)
{
    int count = 0;
    fread(&count, sizeof(int), 1, fp);

    AutobotCondition::ItemType item_type = kItem_Max;
    for (int i = 0; i < count; ++i)
    {
        fread(&item_type, sizeof(item_type), 1, fp);
        Insert(item_type);
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Pickup::CheckValid(SLOTCODE item_code)
{
    for (int i = static_cast<int>(eAUTO_PICKUP_ALL); i <= static_cast<int>(eAUTO_PICKUP_HEIM); ++i)
    {
        eAUTO_HUNTING_TYPE hunting_type =  static_cast<eAUTO_HUNTING_TYPE>(i);
        if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(hunting_type, item_code) == false)
        {
            ItemType item_type = ConvertPickupItemType(hunting_type);
            pickupitem_set().erase(item_type);
        }
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Pickup::SaveToBuffer(ArchiveOfStream& ar)
{
    BYTE count = static_cast<BYTE>(pickupitem_set().size());
    ar << count;

    ConditionPickupItemSet::iterator begin = pickupitem_set().begin();
    ConditionPickupItemSet::iterator end = pickupitem_set().end();
    for (; begin != end; ++begin)
    {
        BYTE item_type = static_cast<BYTE>(*begin);

        ar << item_type;
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Pickup::LoadFromBuffer(ArchiveOfStream& ar, int file_version)
{
    BYTE count = 0;
    ar >> count;

    BYTE item_type = 0;
    for (int i = 0; i < static_cast<int>(count); ++i)
    {
        ar >> item_type;

        Insert(static_cast<ItemType>(item_type));
    }
}
//------------------------------------------------------------------------------
bool AutobotCondition_Pickup::SendCheck(AutobotCondition_Pickup& dummy)
{
    if (pickupitem_set().size() != dummy.pickupitem_set().size())
    {
        return true;
    }
    else
    {
        ConditionPickupItemSet::iterator begin = pickupitem_set().begin();
        ConditionPickupItemSet::iterator end = pickupitem_set().end();
        ConditionPickupItemSet::iterator dummy_begin = dummy.pickupitem_set().begin();
        ConditionPickupItemSet::iterator dummy_end = dummy.pickupitem_set().end();
        for (; begin != end; ++begin, ++dummy_begin)
        {
            if (*begin != *dummy_begin)
            {
                return true;
            }
        }
    }

    return false;
}
void AutobotCondition_Pickup::CopyData(AutobotCondition_Pickup& sorce)
{
    Clear();
    ConditionPickupItemSet::iterator begin = sorce.pickupitem_set().begin();
    ConditionPickupItemSet::iterator end = sorce.pickupitem_set().end();
    for (; begin != end; ++begin)
    {
        Insert(*begin);
    }
}
//------------------------------------------------------------------------------
//!AutobotCondition_Recall
//------------------------------------------------------------------------------
void AutobotCondition_Recall::Clear()
{
    inventory_full_percent_ = 0.0f;
    warning_dura_ = 0;
    heal_item_num_ = 0;
    feed_item_num_ = 0;
    is_return_field_ = true;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Recall::IsReturnVillage()
{
    if ((warning_dura() > 0) && 
        ItemManager::Instance()->IsExistRepairItem(SI_EQUIPMENT, warning_dura()))
    {
        return true;
    }

    SCSlotContainer* slot_container = ItemManager::Instance()->GetContainer(SI_INVENTORY);

    if (slot_container != NULL)
    {
        float full_percent = 100.0f - (static_cast<float>(slot_container->GetEmptySlotNum()) / static_cast<float>(slot_container->GetMaxSlotNum()) * 100.0f);
        if ((inventory_full_percent() > 0.0f) && 
            (inventory_full_percent() <= full_percent))
        {
            return true;
        }

        if ((heal_item_num() > 0) && 
            (heal_item_num() > ItemManager::Instance()->GetItemCount(SI_INVENTORY, eITEMTYPE_WASTE, eITEMWASTE_HPPOTION)))
        {
            return true;
        }

        if ((feed_item_num() > 0) && 
            (feed_item_num() > ItemManager::Instance()->GetItemCount(SI_INVENTORY, eITEMTYPE_PET_FEED, eITEMWASTE_PET_FEED)))
        {
            return true;
        }
    }

    return false;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recall::Save(FILE* fp)
{
    fwrite(&inventory_full_percent_, sizeof(float), 1, fp);
    fwrite(&warning_dura_, sizeof(DURATYPE), 1, fp);
    fwrite(&heal_item_num_, sizeof(NUMTYPE), 1, fp);
    fwrite(&feed_item_num_, sizeof(NUMTYPE), 1, fp);
    fwrite(&is_return_field_, sizeof(is_return_field_), 1, fp);
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recall::Load(FILE* fp, int file_version)
{
    fread(&inventory_full_percent_, sizeof(float), 1, fp);
    fread(&warning_dura_, sizeof(DURATYPE), 1, fp);
    fread(&heal_item_num_, sizeof(NUMTYPE), 1, fp);
    fread(&feed_item_num_, sizeof(NUMTYPE), 1, fp);
    if (AutobotCondition::kAutobotFileVersion >= 110)
    {
        fread(&is_return_field_, sizeof(is_return_field_), 1, fp);
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recall::CheckValid(SLOTCODE item_code)
{
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_RETURNSCROLL_INVEN, item_code) == false)
    {
        inventory_full_percent_ = 0.0f;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_RETURNSCROLL_POTION, item_code) == false)
    {
        heal_item_num_ = 0;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_RETURNSCROLL_PET_FEED, item_code) == false)
    {
        feed_item_num_ = 0;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_RETURNSCROLL_DURATION, item_code) == false)
    {
        warning_dura_ = 0;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_RETURNSCROLL_RETURNFIELD, item_code) == false)
    {
        //! AutobotCondition::kAutobotFileVersion >= 110
        //! 추가된변수 기본이 돌아가는것임
        is_return_field_ = true;
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recall::SaveToBuffer(ArchiveOfStream& ar)
{
    BYTE inventory_full_percent = static_cast<BYTE>(inventory_full_percent_);
    DURATYPE warning_dura = warning_dura_;
    NUMTYPE heal_item_num = heal_item_num_;
    NUMTYPE feed_item_num = feed_item_num_;

    ar << inventory_full_percent;
    ar << warning_dura;
    ar << heal_item_num;
    ar << feed_item_num;
    ar << is_return_field();
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recall::LoadFromBuffer(ArchiveOfStream& ar, int file_version)
{
    BYTE inventory_full_percent = 0;
    DURATYPE warning_dura = 0;
    NUMTYPE heal_item_num = 0;
    NUMTYPE feed_item_num = 0;

    ar >> inventory_full_percent;
    ar >> warning_dura;
    ar >> heal_item_num;
    ar >> feed_item_num;

    //! AutobotCondition::kAutobotFileVersion >= 110
    if (AutobotCondition::kAutobotFileVersion >= 110)
    {
        ar >> is_return_field_;
    }

    inventory_full_percent_ = static_cast<float>(inventory_full_percent);
    warning_dura_ = warning_dura;
    heal_item_num_ = heal_item_num;
    feed_item_num_ = feed_item_num;
}
//------------------------------------------------------------------------------
bool AutobotCondition_Recall::SendCheck(AutobotCondition_Recall& dummy)
{
    if ((inventory_full_percent() != dummy.inventory_full_percent()) ||
        (warning_dura() != dummy.warning_dura()) ||
        (heal_item_num() != dummy.heal_item_num()) ||
        (feed_item_num() != dummy.feed_item_num()) ||
        (is_return_field() != dummy.is_return_field()))
    {
        return true;
    }

    return false;
}
void AutobotCondition_Recall::CopyData(AutobotCondition_Recall& sorce)
{
    Clear();
    set_inventory_full_percent(sorce.inventory_full_percent());
    set_warning_dura(sorce.warning_dura());
    set_heal_item_num(sorce.heal_item_num());
    set_feed_item_num(sorce.feed_item_num());

    if (AutobotCondition::kAutobotFileVersion >= 110)
    {
        set_return_field(sorce.is_return_field());
    }
}
//------------------------------------------------------------------------------
//!AutobotCondition_Recovery
//------------------------------------------------------------------------------ 
void AutobotCondition_Recovery::Clear()
{
    hp_potion_percent_ = 0.0f;
    mp_potion_percent_ = 0.0f;
    feed_potion_percent_ = 0.0f;
    immediate_potion_percent_ = 0.0f;
    relax_hp_percent_ = 0.0f;
    relax_mp_percent_ = 0.0f;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Recovery::IsRelax()
{
    return ((g_pHero->GetHP_Percent() < relax_hp_percent_) || (g_pHero->GetMP_Percent() < relax_mp_percent_));
}
//------------------------------------------------------------------------------
bool AutobotCondition_Recovery::check_hp_potion()
{
    return (g_pHero->GetHP_Percent() < hp_potion_percent_);
}
//------------------------------------------------------------------------------
bool AutobotCondition_Recovery::check_mp_potion()
{
    return (g_pHero->GetMP_Percent() < mp_potion_percent_);
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Recovery::check_immediate_potion()
{
    return (g_pHero->GetHP_Percent() < immediate_potion_percent_);
}
//------------------------------------------------------------------------------
bool AutobotCondition_Recovery::check_feed_potion()
{
    Pet* pet = static_cast<Pet*>(g_ObjectManager.GetObject(g_pHero->GetPetObjectKey()));

    if ((pet != NULL) && (pet->IsKindOfObject(PET_OBJECT) == TRUE))
    {
        float percent = static_cast<float>(pet->GetPet()->byFullness) / static_cast<float>(C_PET_MAX_GAUGE) * 100.0f;

        return (percent < feed_potion_percent_);
    }

    return false;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recovery::Save(FILE* fp)
{
    fwrite(&hp_potion_percent_, sizeof(float), 1, fp);
    fwrite(&immediate_potion_percent_, sizeof(float), 1, fp);
    fwrite(&mp_potion_percent_, sizeof(float), 1, fp);
    fwrite(&feed_potion_percent_, sizeof(float), 1, fp);
    fwrite(&relax_hp_percent_, sizeof(float), 1, fp);
    fwrite(&relax_mp_percent_, sizeof(float), 1, fp);
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recovery::Load(FILE* fp, int file_version)
{
    fread(&hp_potion_percent_, sizeof(float), 1, fp);
    fread(&immediate_potion_percent_, sizeof(float), 1, fp);
    fread(&mp_potion_percent_, sizeof(float), 1, fp);
    fread(&feed_potion_percent_, sizeof(float), 1, fp);
    fread(&relax_hp_percent_, sizeof(float), 1, fp);
    fread(&relax_mp_percent_, sizeof(float), 1, fp);
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recovery::CheckValid(SLOTCODE item_code)
{
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_CURE_HP, item_code) == false)
    {
        hp_potion_percent_ = 0.0f;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_CURE_MPSP, item_code) == false)
    {
        mp_potion_percent_ = 0.0f;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_CURE_INSTANT, item_code) == false)
    {
        feed_potion_percent_ = 0.0f;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_CURE_PET_FEED, item_code) == false)
    {
        immediate_potion_percent_ = 0.0f;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_CURE_SITDOWN_HP, item_code) == false)
    {
        relax_hp_percent_ = 0.0f;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_CURE_SITDOWN_MP, item_code) == false)
    {
        relax_mp_percent_ = 0.0f;
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recovery::SaveToBuffer(ArchiveOfStream& ar)
{
    BYTE hp_potion_percent = static_cast<BYTE>(hp_potion_percent_);
    BYTE immediate_potion_percent = static_cast<BYTE>(immediate_potion_percent_);
    BYTE mp_potion_percent = static_cast<BYTE>(mp_potion_percent_);
    BYTE feed_potion_percent = static_cast<BYTE>(feed_potion_percent_);
    BYTE relax_hp_percent = static_cast<BYTE>(relax_hp_percent_);
    BYTE relax_mp_percent = static_cast<BYTE>(relax_mp_percent_);

    ar << hp_potion_percent;
    ar << immediate_potion_percent;
    ar << mp_potion_percent;
    ar << feed_potion_percent;
    ar << relax_hp_percent;
    ar << relax_mp_percent;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Recovery::LoadFromBuffer(ArchiveOfStream& ar, int file_version)
{
    BYTE hp_potion_percent = 0;
    BYTE immediate_potion_percent = 0;
    BYTE mp_potion_percent = 0;
    BYTE feed_potion_percent = 0;
    BYTE relax_hp_percent = 0;
    BYTE relax_mp_percent = 0;

    ar >> hp_potion_percent;
    ar >> immediate_potion_percent;
    ar >> mp_potion_percent;
    ar >> feed_potion_percent;
    ar >> relax_hp_percent;
    ar >> relax_mp_percent;

    hp_potion_percent_ = static_cast<float>(hp_potion_percent);
    immediate_potion_percent_ = static_cast<float>(immediate_potion_percent);
    mp_potion_percent_ = static_cast<float>(mp_potion_percent);
    feed_potion_percent_ = static_cast<float>(feed_potion_percent);
    relax_hp_percent_ = static_cast<float>(relax_hp_percent);
    relax_mp_percent_ = static_cast<float>(relax_mp_percent);
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Recovery::SendCheck(AutobotCondition_Recovery& dummy)
{
    if ((hp_potion_percent_ != dummy.hp_potion_percent()) ||
        (immediate_potion_percent_ != dummy.immediate_potion_percent()) ||
        (mp_potion_percent_ != dummy.mp_potion_percent()) ||
        (feed_potion_percent_ != dummy.feed_potion_percent()) ||
        (relax_hp_percent_ != dummy.relax_hp_percent()) ||
        (relax_mp_percent_ != dummy.relax_mp_percent()))
    {
        return true;
    }

    return false;
}
void AutobotCondition_Recovery::CopyData(AutobotCondition_Recovery& sorce)
{
    Clear();
    hp_potion_percent_          = sorce.hp_potion_percent();
    immediate_potion_percent_   = sorce.immediate_potion_percent();
    mp_potion_percent_          = sorce.mp_potion_percent();
    feed_potion_percent_        = sorce.feed_potion_percent();
    relax_hp_percent_           = sorce.relax_hp_percent();
    relax_mp_percent_           = sorce.relax_mp_percent();
}
//------------------------------------------------------------------------------
//! AutobotCondition_Skill
//------------------------------------------------------------------------------
AutobotCondition_Skill::AutobotCondition_Skill()
{
    Clear();
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Skill::Clear()
{
    attack_skill_vector_.clear();
    interval_skill_vector_.clear();
}
//------------------------------------------------------------------------------
int AutobotCondition_Skill::GetAttackCount()
{
    int skill_count = 0;

    ConditionAttackSkillVector::iterator begin = attack_skill_vector().begin();
    ConditionAttackSkillVector::iterator end = attack_skill_vector().end();
    for (; begin != end; ++begin)
    {
        if (begin->slot_type == ST_SKILL)
        {
            ++skill_count;
        }
    }
    return skill_count;
}
//------------------------------------------------------------------------------
SLOTCODE AutobotCondition_Skill::GetAttackSkill()
{
    ConditionAttackSkillVector::iterator begin = attack_skill_vector().begin();
    ConditionAttackSkillVector::iterator end = attack_skill_vector().end();
    for (; begin != end; ++begin)
    {
        Slot* slot = &(*begin);

        if (slot->slot_type == ST_SKILL)
        {
            if (slot->slot_code == eACTION_START_SKILL_CODE)
            {
                return slot->slot_code;
            }
            else if (g_pHero->CanUseSkill(slot->slot_code, FALSE) == TRUE)
            {
                return slot->slot_code;
            }
        }
    }

    return 0;
}
//------------------------------------------------------------------------------
AutobotCondition_Skill::Slot* AutobotCondition_Skill::GetIntervalSkill()
{
    DWORD current_time = clock_function::GetTickCount();

    ConditionIntervalSkillVector::iterator begin = interval_skill_vector().begin();
    ConditionIntervalSkillVector::iterator end = interval_skill_vector().end();

    for (; begin != end; ++begin)
    {
        Slot* slot = &(*begin);

        if (slot->IsValid() == false)
        {
            continue;
        }
        
        if (current_time > (slot->last_use_millisecond + slot->interval_millisecond))
        {
            if (slot->slot_type == ST_ITEM)
            {
                SCItemSlot* item_slot = ItemManager::Instance()->FindFirstItemSlot(SI_INVENTORY, slot->slot_code);
                if (item_slot == NULL)
                {
                    slot->Reset();
                    continue;
                }
                if (item_slot->GetNum() <= 0)
                {
                    slot->Reset();
                    continue;
                }
                if (g_pHero->CanUseItem(item_slot->GetItemInfo(), FALSE) == FALSE)
                {
                    continue;
                }

                return slot;
            }
            else if (slot->slot_type == ST_SKILL)
            {
                if (g_pHero->CanUseSkill(slot->slot_code, FALSE) == FALSE)
                {
                    continue;
                }

                return slot;
            }
        }
    }

    return NULL;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Skill::InsertAttackSkill(eSlotContainerIndex container_index, 
                                               eSlotType slot_type, 
                                               SLOTCODE slot_code)
{
    Slot slot;
    slot.container_index = container_index;
    slot.slot_type = slot_type;
    slot.slot_code = slot_code;
    slot.interval_millisecond = 0;
    slot.last_use_millisecond = 0;

    return InsertAttackSkill(slot);
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Skill::InsertAttackSkill(Slot& slot)
{
    attack_skill_vector().push_back(slot);
    return true;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Skill::InsertIntervalSkill(eSlotContainerIndex container_index, 
                                                 eSlotType slot_type, 
                                                 SLOTCODE slot_code, 
                                                 DWORD interval_millisecond)
{
    Slot slot;
    slot.container_index = container_index;
    slot.slot_type = slot_type;
    slot.slot_code = slot_code;
    slot.interval_millisecond = interval_millisecond;
    slot.last_use_millisecond = 0;

    return InsertIntervalSkill(slot);
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Skill::InsertIntervalSkill(Slot& slot)
{
    interval_skill_vector().push_back(slot);
    return true;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Skill::Save(FILE* fp)
{
    {
        int count = attack_skill_vector().size();
        ConditionAttackSkillVector::iterator begin = attack_skill_vector().begin();
        ConditionAttackSkillVector::iterator end = attack_skill_vector().end();

        fwrite(&count, sizeof(int), 1, fp);

        for (; begin != end; ++begin)
        {
            Slot slot = *begin;

            fwrite(&slot, sizeof(slot), 1, fp);
        }
    }
    {
        int count = interval_skill_vector().size();
        ConditionIntervalSkillVector::iterator begin = interval_skill_vector().begin();
        ConditionIntervalSkillVector::iterator end = interval_skill_vector().end();

        fwrite(&count, sizeof(int), 1, fp);

        for (; begin != end; ++begin)
        {
            Slot slot = *begin;

            fwrite(&slot, sizeof(slot), 1, fp);
        }
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Skill::Load(FILE* fp, int file_version)
{
    {
        int count = 0;
        fread(&count, sizeof(int), 1, fp);

        Slot slot;
        for (int i = 0; i < count; ++i)
        {
            fread(&slot, sizeof(slot), 1, fp);
            InsertAttackSkill(slot);
        }
    }
    {
        int count = 0;
        fread(&count, sizeof(int), 1, fp);

        Slot slot;
        for (int i = 0; i < count; ++i)
        {
            fread(&slot, sizeof(slot), 1, fp);
            InsertIntervalSkill(slot);
        }
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Skill::CheckValid(SLOTCODE item_code)
{
    // 
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Skill::SaveToBuffer(ArchiveOfStream& ar)
{
    BYTE count = 0;
    BYTE container_index = 0;
    BYTE slot_type = 0;
    SLOTCODE slot_code = 0;
    DWORD interval_millisecond = 0;

    {
        count = static_cast<BYTE>(attack_skill_vector().size());
        ar << count;

        ConditionAttackSkillVector::iterator begin = attack_skill_vector().begin();
        ConditionAttackSkillVector::iterator end = attack_skill_vector().end();
        for (; begin != end; ++begin)
        {
            container_index = static_cast<BYTE>(begin->container_index);
            slot_type = static_cast<BYTE>(begin->slot_type);
            slot_code = begin->slot_code;

            ar << container_index;
            ar << slot_type;
            ar << slot_code;
        }
    }

    {
        count = static_cast<BYTE>(interval_skill_vector().size());
        ar << count;

        ConditionIntervalSkillVector::iterator begin = interval_skill_vector().begin();
        ConditionIntervalSkillVector::iterator end = interval_skill_vector().end();
        for (; begin != end; ++begin)
        {
            container_index = static_cast<BYTE>(begin->container_index);
            slot_type = static_cast<BYTE>(begin->slot_type);
            slot_code = begin->slot_code;
            interval_millisecond = begin->interval_millisecond;

            ar << container_index;
            ar << slot_type;
            ar << slot_code;
            ar << interval_millisecond;
        }
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Skill::LoadFromBuffer(ArchiveOfStream& ar, int file_version)
{
    BYTE count = 0;
    BYTE container_index = 0;
    BYTE slot_type = 0;
    SLOTCODE slot_code = 0;
    DWORD interval_millisecond = 0;

    {
        ar >> count;
        for (int i = 0; i < static_cast<int>(count); ++i)
        {
            ar >> container_index;
            ar >> slot_type;
            ar >> slot_code;

            if ((container_index > SI_TEMPINVENTORY) && (container_index < SI_MAX_TOTAL))
            {
                InsertAttackSkill(static_cast<eSlotContainerIndex>(container_index), 
                                static_cast<eSlotType>(slot_type), 
                                slot_code);
            }
        }
    }

    {
        ar >> count;
        for (int i = 0; i < static_cast<int>(count); ++i)
        {
            ar >> container_index;
            ar >> slot_type;
            ar >> slot_code;
            ar >> interval_millisecond;

            if ((container_index > SI_TEMPINVENTORY) && (container_index < SI_MAX_TOTAL))
            {
                InsertIntervalSkill(static_cast<eSlotContainerIndex>(container_index), 
                                    static_cast<eSlotType>(slot_type), 
                                    slot_code, 
                                    interval_millisecond);
            }
        }
    }
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Skill::SendCheck(AutobotCondition_Skill& dummy)
{
    if (attack_skill_vector().size() != dummy.attack_skill_vector().size())
    {
        return true;
    }
    else
    {
        ConditionAttackSkillVector::iterator begin = attack_skill_vector().begin();
        ConditionAttackSkillVector::iterator end = attack_skill_vector().end();
        ConditionAttackSkillVector::iterator dummy_begin = dummy.attack_skill_vector().begin();
        ConditionAttackSkillVector::iterator dummy_end = dummy.attack_skill_vector().end();
        for (; begin != end; ++begin, ++dummy_begin)
        {
            if ((begin->container_index != dummy_begin->container_index) ||
                (begin->slot_type != dummy_begin->slot_type) ||
                (begin->slot_code != dummy_begin->slot_code))
            {
                return true;
            }
        }
    }

    if (interval_skill_vector().size() != dummy.interval_skill_vector().size())
    {
        return true;
    }
    else
    {
        ConditionIntervalSkillVector::iterator begin = interval_skill_vector().begin();
        ConditionIntervalSkillVector::iterator end = interval_skill_vector().end();
        ConditionIntervalSkillVector::iterator dummy_begin = dummy.interval_skill_vector().begin();
        ConditionIntervalSkillVector::iterator dummy_end = dummy.interval_skill_vector().end();
        for (; begin != end; ++begin, ++dummy_begin)
        {
            if ((begin->container_index != dummy_begin->container_index) ||
                (begin->slot_type != dummy_begin->slot_type) ||
                (begin->slot_code != dummy_begin->slot_code))
            {
                return true;
            }
        }
    }

    return false;
}
void AutobotCondition_Skill::CopyData(AutobotCondition_Skill& sorce)
{
    Clear();
    ConditionAttackSkillVector::iterator attack_begin = sorce.attack_skill_vector().begin();
    ConditionAttackSkillVector::iterator attack_end = sorce.attack_skill_vector().end();

    for (; attack_begin != attack_end; ++attack_begin)
    {
        InsertAttackSkill(*attack_begin);
    }

    ConditionIntervalSkillVector::iterator interval_begin = sorce.interval_skill_vector().begin();
    ConditionIntervalSkillVector::iterator interval_end = sorce.interval_skill_vector().end();

    for (; interval_begin != interval_end; ++interval_begin)
    {
        InsertIntervalSkill(*interval_begin);
    }
}
//------------------------------------------------------------------------------ 
//! AutobotCondition_Revival
//------------------------------------------------------------------------------ 
void AutobotCondition_Revival::Clear()
{
    revival_type_ = kRevival_Stay;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Revival::Save(FILE* fp)
{
    fwrite(&revival_type_, sizeof(revival_type_), 1, fp);

}
//------------------------------------------------------------------------------ 
void AutobotCondition_Revival::Load(FILE* fp, int file_version)
{
    fread(&revival_type_, sizeof(revival_type_), 1, fp);
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Revival::CheckValid(SLOTCODE item_code)
{
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_REBIRTH_VILIAGE, item_code) == false)
    {
        if (revival_type_ == kRevival_Village)
        {
            revival_type_ = kRevival_Stay;
        }
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_REBIRTH_WAITING, item_code) == false)
    {
        revival_type_ = kRevival_Stay;
        //if (revival_type_ == kRevival_Stay)
        //{
        //    revival_type_ = kRevival_Max;
        //}        
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_REBIRTH, item_code) == false)
    {
        if (revival_type_ == kRevival_Resurrection)
        {
            revival_type_ = kRevival_Stay;
        }        
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Revival::SaveToBuffer(ArchiveOfStream& ar)
{
    BYTE revival = static_cast<BYTE>(revival_type());
    ar << revival;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Revival::LoadFromBuffer(ArchiveOfStream& ar, int file_version)
{
    BYTE revival = 0;
    ar >> revival;

    set_revival_type(static_cast<RevivalType>(revival));
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Revival::SendCheck(AutobotCondition_Revival& dummy)
{
    if (revival_type() != dummy.revival_type())
    {
        return true;
    }

    return false;
}
void AutobotCondition_Revival::CopyData(AutobotCondition_Revival& sorce)
{
    Clear();
    set_revival_type(sorce.revival_type());
}
//------------------------------------------------------------------------------
//!AutobotCondition_Shop
//------------------------------------------------------------------------------
void AutobotCondition_Shop::Clear()
{
    buyitem_map_.clear(); 
    sellitem_set_.clear();
    repair_ = false;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Shop::IsSellItem(SCItemSlot* inventory_item_slot)
{
    if ((inventory_item_slot == NULL) || (inventory_item_slot->GetItemInfo() == NULL))
    {
        return false;
    }

    // 귀환아이템은 판매하지 않는다
    if( inventory_item_slot->GetCode() == 7241 )
    {
        return false;
    }

    AutobotCondition::ItemType item_type = ConvertPickupItemType(inventory_item_slot->GetItemInfo());

    ConditionSellItemSet::iterator end = sellitem_set().end();
    if ((sellitem_set().find(kItem_All) != end) ||
        (sellitem_set().find(item_type) != end))
    {
        if (CanTradeSell(inventory_item_slot, inventory_item_slot->GetItemInfo(), eITEM_TRADESELL_SELL) == true)
        {
            return true;
        }
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Shop::IsSellItem(AutobotCondition::ItemType item_type)
{
    ConditionSellItemSet::iterator iter = sellitem_set().find(item_type);
    if (iter != sellitem_set().end())
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool AutobotCondition_Shop::IsBuyItem(SCItemSlot* shop_item_slot)
{
    if ((shop_item_slot == NULL) || (shop_item_slot->GetItemInfo() == NULL))
    {
        return false;
    }

    ConditionBuyItemMap::iterator iter = buyitem_map().find(shop_item_slot->GetItemInfo()->m_Code);
    if (iter != buyitem_map().end())
    {
        return (iter->second > shop_item_slot->GetNum());
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Shop::InsertSellItem(AutobotCondition::ItemType item_type)
{
    sellitem_set().insert(item_type);
    return true;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Shop::InsertBuyItem(SLOTCODE slot_code, NUMTYPE count)
{
    buyitem_map().insert(std::make_pair(slot_code, count));
    return true;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Shop::Save(FILE* fp)
{
    {
        int count = sellitem_set().size();
        ConditionSellItemSet::iterator begin = sellitem_set().begin();
        ConditionSellItemSet::iterator end = sellitem_set().end();

        fwrite(&count, sizeof(int), 1, fp);

        for (; begin != end; ++begin)
        {
            AutobotCondition::ItemType item_type = *begin;

            fwrite(&item_type, sizeof(item_type), 1, fp);
        }
    }
    {
        int count = buyitem_map().size();
        ConditionBuyItemMap::iterator begin = buyitem_map().begin();
        ConditionBuyItemMap::iterator end = buyitem_map().end();

        fwrite(&count, sizeof(int), 1, fp);

        for (; begin != end; ++begin)
        {
            SLOTCODE slot_code = begin->first;
            NUMTYPE slot_num = begin->second;

            fwrite(&slot_code, sizeof(slot_code), 1, fp);
            fwrite(&slot_num, sizeof(slot_num), 1, fp);
        }
    }
    fwrite(&repair_, sizeof(repair_), 1, fp);
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Shop::Load(FILE* fp, int file_version)
{
    {
        int count = 0;
        fread(&count, sizeof(int), 1, fp);

        AutobotCondition::ItemType item_type = kItem_Max;
        for (int i = 0; i < count; ++i)
        {
            fread(&item_type, sizeof(item_type), 1, fp);
            InsertSellItem(item_type);
        }
    }
    {
        int count = 0;
        fread(&count, sizeof(int), 1, fp);

        SLOTCODE slot_code = 0;
        NUMTYPE slot_num = 0;
        for (int i = 0; i < count; ++i)
        {

            fread(&slot_code, sizeof(slot_code), 1, fp);
            fread(&slot_num, sizeof(slot_num), 1, fp);
            InsertBuyItem(slot_code, slot_num);
        }
    }
    fread(&repair_, sizeof(repair_), 1, fp);
}
//------------------------------------------------------------------------------ 
NUMTYPE AutobotCondition_Shop::GetBuyItemCount(SLOTCODE item_code)
{
    ConditionBuyItemMap::iterator iter = buyitem_map().find(item_code);
    if (iter != buyitem_map().end())
    {
        return iter->second;
    }
    return 0;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Shop::EraseBuyItemByWasteType(eWASTETYPE waste_type)
{
    ConditionBuyItemMap::iterator begin = buyitem_map().begin();
    ConditionBuyItemMap::iterator end = buyitem_map().end();
    for (; begin != end; ++begin)
    {
        const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(begin->first);
        if ((item_info != NULL) && (item_info->m_byWasteType == waste_type))
        {
            begin = buyitem_map().erase(begin);
        }
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Shop::CheckValid(SLOTCODE item_code)
{
    // 팔기
    for (int i = static_cast<int>(eAUTO_SELL_ALL); i <= static_cast<int>(eAUTO_SELL_AC); ++i)
    {
        eAUTO_HUNTING_TYPE hunting_type =  static_cast<eAUTO_HUNTING_TYPE>(i);
        if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(hunting_type, item_code) == false)
        {
            ItemType item_type = ConvertPickupItemType(hunting_type);
            sellitem_set().erase(item_type);
        }
    }
    // 사기
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_BUY_RETURNSCROLL, item_code) == false)
    {
        EraseBuyItemByWasteType(eITEMWASTE_RETURNSCROLL);
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_BUY_PET_FEED, item_code) == false)
    {
        EraseBuyItemByWasteType(eITEMWASTE_PET_FEED);
    }
    if ((AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_BUY_POTION_LEVEL, item_code) == false) || 
        (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_BUY_POTION_COUNT, item_code) == false))
    {
        EraseBuyItemByWasteType(eITEMWASTE_HPPOTION);
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_BUY_DURATION, item_code) == false)
    {
        repair_ = false;
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Shop::SaveToBuffer(ArchiveOfStream& ar)
{
    {
        BYTE count = static_cast<BYTE>(sellitem_set().size());
        ar << count;

        ConditionSellItemSet::iterator begin = sellitem_set().begin();
        ConditionSellItemSet::iterator end = sellitem_set().end();
        for (; begin != end; ++begin)
        {
            BYTE item_type = static_cast<BYTE>(*begin);
            ar << item_type;
        }
    }

    {
        BYTE count = static_cast<BYTE>(buyitem_map().size());
        ar << count;

        ConditionBuyItemMap::iterator begin = buyitem_map().begin();
        ConditionBuyItemMap::iterator end = buyitem_map().end();
        for (; begin != end; ++begin)
        {
            SLOTCODE slot_code = begin->first;
            NUMTYPE num_type = begin->second;
            ar << slot_code;
            ar << num_type;
        }
    }

    ar << repair_;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Shop::LoadFromBuffer(ArchiveOfStream& ar, int file_version)
{
    {
        BYTE count = 0;
        ar >> count;

        BYTE item_type = 0;
        for (int i = 0; i < static_cast<int>(count); ++i)
        {
            ar >> item_type;
            InsertSellItem(static_cast<ItemType>(item_type));
        }
    }

    {
        BYTE count = 0;
        ar >> count;

        SLOTCODE slot_code = 0;
        NUMTYPE num_type = 0;
        for (int i = 0; i < static_cast<int>(count); ++i)
        {
            ar >> slot_code;
            ar >> num_type;
            InsertBuyItem(slot_code, num_type);
        }
    }

    ar >> repair_;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Shop::SendCheck(AutobotCondition_Shop& dummy)
{
    if (sellitem_set().size() != dummy.sellitem_set().size())
    {
        return true;
    }
    else
    {
        ConditionSellItemSet::iterator begin = sellitem_set().begin();
        ConditionSellItemSet::iterator end = sellitem_set().end();
        ConditionSellItemSet::iterator dummy_begin = dummy.sellitem_set().begin();
        ConditionSellItemSet::iterator dummy_end = dummy.sellitem_set().end();
        for (; begin != end; ++begin, ++dummy_begin)
        {
            if (*begin != *dummy_begin)
            {
                return true;
            }
        }
    }

    if (buyitem_map().size() != dummy.buyitem_map().size())
    {
        return true;
    }
    else
    {
        ConditionBuyItemMap::iterator begin = buyitem_map().begin();
        ConditionBuyItemMap::iterator end = buyitem_map().end();
        ConditionBuyItemMap::iterator dummy_begin = dummy.buyitem_map().begin();
        ConditionBuyItemMap::iterator dummy_end = dummy.buyitem_map().end();
        for (; begin != end; ++begin, ++dummy_begin)
        {
            if ((begin->first != dummy_begin->first) ||
                (begin->second != dummy_begin->second))
            {
                return true;
            }
        }
    }

    if (IsRepair() != dummy.IsRepair())
    {
        return true;
    }

    return false;
}
void AutobotCondition_Shop::CopyData(AutobotCondition_Shop& sorce)
{
    Clear();
    ConditionSellItemSet::iterator sell_begin = sorce.sellitem_set().begin();
    ConditionSellItemSet::iterator sell_end = sorce.sellitem_set().end();

    for (; sell_begin != sell_end; ++sell_begin)
    {
        InsertSellItem(*sell_begin);
    }

    ConditionBuyItemMap::iterator buy_begin = sorce.buyitem_map().begin();
    ConditionBuyItemMap::iterator buy_end = sorce.buyitem_map().end();
    for (; buy_begin != buy_end; ++buy_begin)
    {
        InsertBuyItem(buy_begin->first, buy_begin->second);
    }

    SetRepair(sorce.IsRepair());
}
//------------------------------------------------------------------------------ 
//! AutobotCondition_Hunting
//------------------------------------------------------------------------------ 

AutobotCondition_Hunting::AutobotCondition_Hunting()
{
    Clear();
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Hunting::Clear()
{
    hunting_type_ = kHuntingType_Max;
    target_range_ = 23.0f;
    party_follow_no_ = 0;

    area_range_ = 10;
    area_position_.x = 0.0f;
    area_position_.y = 0.0f;
    area_position_.z = 0.0f;

    avoid_monster_set_.clear();
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Hunting::NextFollowPlayer(DWORD without_object_key)
{
    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (party_manager != NULL)
    {
        STLX_VECTOR<uicPartyMemberInfoPlus>::iterator begin = party_manager->GetPartyListRef().begin();
        STLX_VECTOR<uicPartyMemberInfoPlus>::iterator end = party_manager->GetPartyListRef().end();

        int follow_no = 0;
        for (; begin != end; ++begin)
        {
            if ((begin->Get().m_dwMemberKey != g_pHero->GetObjectKey()) &&
                (begin->Get().m_dwMemberKey != without_object_key))
            {
                set_party_follow_no(follow_no);
                return true;
            }

            ++follow_no;
        }
    }

    return false;
}
//------------------------------------------------------------------------------
DWORD AutobotCondition_Hunting::GetFollowPlayer()
{
    if (hunting_type() != kHuntingType_Party)
    {
        return 0;
    }

    int index = party_follow_no(); 

    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if ((party_manager == NULL) || 
        (index < 0) || 
        (index >= party_manager->GetPartyNum()))
    {
        return 0;
    }

    return party_manager->GetParty(index).Get().m_dwMemberKey;
}
//------------------------------------------------------------------------------
bool AutobotCondition_Hunting::GetFollowPlayerPosition(OUT WzVector& pos)
{
    if (hunting_type() != kHuntingType_Party)
    {
        return 0;
    }

    int index = party_follow_no();

    uiPartyMan* party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if ((party_manager == NULL) || 
        (index < 0) || 
        (index >= party_manager->GetPartyNum()))
    {
        return 0;
    }

    PartyNaviInfo* party_navi_info = party_manager->GetPartyNaviInfo();
    DWORD object_key = party_manager->GetParty(index).Get().m_dwMemberKey;
    NAVINODE NaviNode;
    if ((party_navi_info->GetNaviInfo(object_key, &NaviNode) == true) && 
        (g_pMap->GetCurrentFieldID() == NaviNode.fieldCode))
    {
        pos.x = NaviNode.posX;
        pos.y = NaviNode.posY;
        pos.z = NaviNode.posZ; 
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
bool AutobotCondition_Hunting::IsAvoidMonster(DWORD object_key)
{
    Object* object = g_ObjectManager.GetObject(object_key);
    if ((object == NULL) || 
        (object->IsKindOfObject(MONSTER_OBJECT) == FALSE))
    {
        return false;
    }

    eNPC_GRADE monster_grade = static_cast<eNPC_GRADE>(static_cast<Monster*>(object)->GetMonsterInfo()->m_byGrade);

    return IsAvoidMonster(monster_grade);
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Hunting::IsAvoidMonster(eNPC_GRADE npc_grade)
{
    if (avoid_monster_set().find(npc_grade) != avoid_monster_set().end())
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool AutobotCondition_Hunting::IsMonsterInValidArea(DWORD object_key)
{
    if (hunting_type() != kHuntingType_Area)
    {
        return true;
    }
    
    Object* object = g_ObjectManager.GetObject(object_key);
    if ((object == NULL) || 
        (object->IsKindOfObject(MONSTER_OBJECT) == FALSE))
    {
        return false;
    }

    return IsValidArea(object->GetPosition());
}
//------------------------------------------------------------------------------
bool AutobotCondition_Hunting::IsValidArea(WzVector& pos)
{
    if (hunting_type() != kHuntingType_Area)
    {
        return true;
    }

    WzVector gab = area_position() - pos;
    gab.y = 0.0f;

    return (VectorLength(&gab) <= area_range());
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Hunting::Save(FILE* fp)
{
    int count = avoid_monster_set().size();
    ConditionAvoidMonsterSet::iterator begin = avoid_monster_set().begin();
    ConditionAvoidMonsterSet::iterator end = avoid_monster_set().end();

    fwrite(&count, sizeof(int), 1, fp);

    for (; begin != end; ++begin)
    {
        eNPC_GRADE npc_grade = (*begin);

        fwrite(&npc_grade, sizeof(npc_grade), 1, fp);
    }

    fwrite(&target_range_, sizeof(target_range_), 1, fp);
    fwrite(&hunting_type_, sizeof(hunting_type_), 1, fp);
    fwrite(&party_follow_no_, sizeof(party_follow_no_), 1, fp);
    fwrite(&area_position_, sizeof(area_position_), 1, fp);
    fwrite(&area_range_, sizeof(area_range_), 1, fp);
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Hunting::Load(FILE* fp, int file_version)
{
    int count = 0;
    fread(&count, sizeof(int), 1, fp);

    eNPC_GRADE npc_grade = eNPC_GRADE_MAX;
    for (int i = 0; i < count; ++i)
    {
        fread(&npc_grade, sizeof(npc_grade), 1, fp);

        avoid_monster_set().insert(npc_grade);
    }

    fread(&target_range_, sizeof(target_range_), 1, fp);
    fread(&hunting_type_, sizeof(hunting_type_), 1, fp);
    fread(&party_follow_no_, sizeof(party_follow_no_), 1, fp);
    fread(&area_position_, sizeof(area_position_), 1, fp);
    fread(&area_range_, sizeof(area_range_), 1, fp);

}
//------------------------------------------------------------------------------ 
void AutobotCondition_Hunting::CheckValid(SLOTCODE item_code)
{
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_RANGE_FIELD, item_code) == false)
    {
        if (hunting_type_ == kHuntingType_Free)
        {
            hunting_type_ = kHuntingType_Max;
        }
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_RANGE_PARTY, item_code) == false)
    {
        if (hunting_type_ == kHuntingType_Party)
        {
            hunting_type_ = kHuntingType_Max;
        }
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_RANGE_USER, item_code) == false)
    {
        if (hunting_type_ == kHuntingType_Area)
        {
            hunting_type_ = kHuntingType_Max;
        }
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_RANGE_USER, item_code) == false)
    {
        avoid_monster_set_.clear();
    }
 }
//------------------------------------------------------------------------------ 
void AutobotCondition_Hunting::SaveToBuffer(ArchiveOfStream& ar)
{
    BYTE count = static_cast<BYTE>(avoid_monster_set().size());
    ar << count;

    ConditionAvoidMonsterSet::iterator begin = avoid_monster_set().begin();
    ConditionAvoidMonsterSet::iterator end = avoid_monster_set().end();
    for (; begin != end; ++begin)
    {
        BYTE npc_grade = static_cast<BYTE>(*begin);
        ar << npc_grade;
    }

    BYTE hunting = static_cast<BYTE>(hunting_type());
    BYTE area = static_cast<BYTE>(area_range());
    ar << hunting;
    ar << area;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Hunting::LoadFromBuffer(ArchiveOfStream& ar, int file_version)
{
    BYTE count = 0;
    ar >> count;

    BYTE npc_grade = 0;
    for (int i = 0; i < static_cast<int>(count); ++i)
    {
        ar >> npc_grade;

        avoid_monster_set().insert(static_cast<eNPC_GRADE>(npc_grade));
    }

    BYTE hunting = 0;
    BYTE area  = 0;
    ar >> hunting;
    ar >> area;

    set_hunting_type(static_cast<HuntingType>(hunting));
    set_area_range(static_cast<float>(area));
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Hunting::SendCheck(AutobotCondition_Hunting& dummy)
{
    if (avoid_monster_set().size() != dummy.avoid_monster_set().size())
    {
        return true;
    }
    else
    {
        ConditionAvoidMonsterSet::iterator begin = avoid_monster_set().begin();
        ConditionAvoidMonsterSet::iterator end = avoid_monster_set().end();
        ConditionAvoidMonsterSet::iterator dummy_begin = dummy.avoid_monster_set().begin();
        ConditionAvoidMonsterSet::iterator dummy_end = dummy.avoid_monster_set().end();
        
        for (; begin != end; ++begin, ++dummy_begin)
        {
            if (*begin != *dummy_begin)
            {
                return true;
            }
        }
    }
    
    if ((hunting_type() != dummy.hunting_type()) ||
        (area_range() != dummy.area_range()))
    {
        return true;
    }

    return false;
}
void AutobotCondition_Hunting::CopyData(AutobotCondition_Hunting& sorce)
{
    Clear();
    ConditionAvoidMonsterSet::iterator begin = sorce.avoid_monster_set().begin();
    ConditionAvoidMonsterSet::iterator end = sorce.avoid_monster_set().end();
    for (; begin != end; ++begin)
    {
        avoid_monster_set().insert(*begin);
    }

    set_hunting_type(sorce.hunting_type());
    set_area_range(sorce.area_range());
}
//------------------------------------------------------------------------------
//! AutobotCondition_Community
//------------------------------------------------------------------------------ 
void AutobotCondition_Community::Clear()
{
    ZeroMemory(channel_level_, sizeof(channel_level_));
    ZeroMemory(channel_name_, sizeof(channel_name_));

    shouting_type_ = kShouting_Max;
    shouting_second_ = 0;
    ZeroMemory(shouting_message_, sizeof(shouting_second_));

    party_type_ = kParty_Stay;

    save_chat_message_ = false;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Community::set_channel(TCHAR* level, TCHAR* name)
{
    if (level == NULL || name == NULL)
    {
        return;
    }

    strncpy(channel_level_, level, MAX_ROOMTITLE_LENGTH);
    strncpy(channel_name_, name, MAX_ROOMTITLE_LENGTH);
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Community::set_shouting(ShoutingType shout_type, int second, TCHAR* message)
{
    if (message == NULL)
    {
        return;
    }
    shouting_type_ = shout_type;
    shouting_second_ = second;

    strncpy(shouting_message_, message, MAX_CHAT_LEN);
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Community::Save(FILE* fp)
{
    fwrite(&channel_level_, sizeof(channel_level_), 1, fp);
    fwrite(&channel_name_, sizeof(channel_name_), 1, fp);

    fwrite(&shouting_type_, sizeof(shouting_type_), 1, fp);
    fwrite(&shouting_second_, sizeof(shouting_second_), 1, fp);
    fwrite(&shouting_message_, sizeof(shouting_message_), 1, fp);

    fwrite(&party_type_, sizeof(party_type_), 1, fp);
    fwrite(&save_chat_message_, sizeof(save_chat_message_), 1, fp);
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Community::Load(FILE* fp, int file_version)
{
    fread(&channel_level_, sizeof(channel_level_), 1, fp);
    fread(&channel_name_, sizeof(channel_name_), 1, fp);

    fread(&shouting_type_, sizeof(shouting_type_), 1, fp);
    fread(&shouting_second_, sizeof(shouting_second_), 1, fp);
    fread(&shouting_message_, sizeof(shouting_message_), 1, fp);

    fread(&party_type_, sizeof(party_type_), 1, fp);
    fread(&save_chat_message_, sizeof(save_chat_message_), 1, fp);
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Community::IsShouting()
{
    if ((shouting_type() == kShouting_Normal) ||
        (shouting_type() == kShouting_World))
    {
        if ((shouting_second() > 0) && 
            (_tcslen(shouting_message()) > 0))        
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Community::IsChannel()
{
    if ((_tcslen(channel_level()) > 0) && 
        (_tcslen(channel_name()) > 0))
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Community::CheckValid(SLOTCODE item_code)
{
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_COMMUNITY_PARTY_JOIN, item_code) == false)
    {
        if (party_type_ == kParty_Agree)
        {
            party_type_ = kParty_Stay;
        }
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_COMMUNITY_PARTY_BLOCK, item_code) == false)
    {
        if (party_type_ == kParty_Ignore)
        {
            party_type_ = kParty_Stay;
        }
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_COMMUNITY_SAVE_CHAT, item_code) == false)
    {
        save_chat_message_ = false;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_COMMUNITY_SHOUT, item_code) == false)
    {
        shouting_type_ = kShouting_None;
    }
    if (AutomaticInfoParser::Instance()->CheckItemCodeInAutomaticInfo(eAUTO_COMMUNITY_CHANNEL, item_code) == false)
    {
        ZeroMemory(channel_level_, sizeof(channel_level_));
        ZeroMemory(channel_name_, sizeof(channel_name_));
    }
}
//------------------------------------------------------------------------------ 
void AutobotCondition_Community::SaveToBuffer(ArchiveOfStream& ar)
{
    BYTE len = 0;

    len = (BYTE)strlen(channel_level());
    ar << len;
    if (len > 0)
    {
        ar.Write(channel_level(), len);
    }

    len = (BYTE)strlen(channel_name());
    ar << len;
    if (len > 0)
    {
        ar.Write(channel_name(), len);
    }

    ar << shouting_type();
    ar << shouting_second();

    len = strlen(shouting_message());
    ar << len;
    if (len > 0)
    {
        ar.Write(shouting_message(), len);
    }

    ar << party_type();
    ar << is_save_chat_message();

}
//------------------------------------------------------------------------------ 
void AutobotCondition_Community::LoadFromBuffer(ArchiveOfStream& ar, int file_version)
{
    BYTE len = 0;

    ar >> len;
    if (len > 0)
    {
        ar.Read(channel_level_, len);
    }

    ar >> len;
    if (len > 0)
    {
        ar.Read(channel_name_, len);
    }

    ar >> shouting_type_;
    ar >> shouting_second_;
    
    ar >> len;
    if (len > 0)
    {
        ar.Read(shouting_message_, len);
    }
    if (len + 1 < MAX_CHAT_LEN)
    {
        shouting_message_[len + 1] = 0;
    }

    ar >> party_type_;
    ar >> save_chat_message_;
}
//------------------------------------------------------------------------------ 
bool AutobotCondition_Community::SendCheck(AutobotCondition_Community& dummy)
{
    if ((strcmp(channel_level(), dummy.channel_level()) != 0) ||
        (strcmp(channel_name(), dummy.channel_name()) != 0) ||
        (shouting_type() != dummy.shouting_type()) ||
        (shouting_second() != dummy.shouting_second()) ||
        (strcmp(shouting_message(), dummy.shouting_message()) != 0) ||
        (party_type() != dummy.party_type()) ||
        (is_save_chat_message() != dummy.is_save_chat_message()))
    {
        return true;
    }

    return false;
}
void AutobotCondition_Community::CopyData(AutobotCondition_Community& sorce)
{
    Clear();
    strncpy(channel_level(), sorce.channel_level(), MAX_ROOMTITLE_LENGTH);
    strncpy(channel_name(), sorce.channel_name(), MAX_ROOMTITLE_LENGTH);
    
    set_shouting(sorce.shouting_type(), sorce.shouting_second(), sorce.shouting_message());

    set_party_type(sorce.party_type());
    set_save_chat_message(sorce.is_save_chat_message());
}