#include "stdafx.h"
#include "./SlotManager.h"

#include "SCItemSlot.h"

#include "Player.h"
#include "ItemManager.h"

#include "InventorySlotContainer.h"
#include "EquipmentSlotContainer.h"
#include "WarehouseSlotContainer.h"
#include "TempStoreSlotContainer.h"
#include "SkillSlotContainer.h"
#include "QuickSlotContainer.h"
#include "SCQuickStyleSlotContainer.h"
#include "GuildWareHouseSlotContainer.h"

//==================================================================================================

struct SlotManager::ImplData
{
    ImplData();
    ~ImplData();
    void SetPlayer(Player* player);

    CInventorySlotContainer inventory_container;
    CEquipmentSlotContainer equipments_container;
    CSkillSlotContainer skill_container;
    QuickSlotContainer quick_container;
#if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
    SCQuickStyleSlotContainer quick_style_container; // NOTE: don't use anymore after EP2
#endif
    CWarehouseSlotContainer warehouse_container;
};

SlotManager::ImplData::ImplData()
{
#if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
    quick_style_container.Init(MAX_STYLE_SLOT_NUM, SI_STYLE); //< 상속받지 않고 사용하기 위해
#endif
}

SlotManager::ImplData::~ImplData()
{
}

void SlotManager::ImplData::SetPlayer(Player* player)
{
    inventory_container.SetPlayer(player);
    equipments_container.SetPlayer(player);
    warehouse_container.SetPlayer(player);
    //SI_TEMPINVENTORY
}

//==================================================================================================

SlotManager::SlotManager()
    : player_(NULL)
    , data_(new SlotManager::ImplData)
{
    ZeroMemory(slot_container_list_, sizeof(slot_container_list_));
    slot_container_list_[SI_INVENTORY]      = &data_->inventory_container;
    //SI_TEMPINVENTORY
    slot_container_list_[SI_EQUIPMENT]      = &data_->equipments_container;
    slot_container_list_[SI_WAREHOUSE]      = &data_->warehouse_container;
    slot_container_list_[SI_SKILL]          = &data_->skill_container;
    slot_container_list_[SI_QUICK]          = &data_->quick_container;
#if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
    slot_container_list_[SI_STYLE]          = &data_->quick_style_container;
#endif
    //SI_GUILDWAREHOUSE
}

SlotManager::~SlotManager()
{
    delete data_;
}


void SlotManager::Release()
{
    slot_container_list_[SI_GUILDWAREHOUSE] = NULL;
}

void SlotManager::Init(Player* player)
{
    player_ = player;
    assert(slot_container_list_[SI_GUILDWAREHOUSE] == NULL);
    for (SCSlotContainer** it = slot_container_list_;
         it != &slot_container_list_[_countof(slot_container_list_)];
         ++it)
    {
        SCSlotContainer* slot_container = *it;
        if (*it == NULL) {
            continue;
        };
        slot_container->ClearAll();
    };

    data_->SetPlayer(player);
}

//==================================================================================================

void SlotManager::BindGuildWarehouseContainer(
    CGuildWarehouseSlotContainer* guild_warehouse_container)
{
    slot_container_list_[SI_GUILDWAREHOUSE] = guild_warehouse_container;
}

void SlotManager::UnbindGuildWarehouseContainer()
{
    slot_container_list_[SI_GUILDWAREHOUSE] = NULL;
}

//==================================================================================================

bool SlotManager::IsEmpty(SLOTIDX slot_index, POSTYPE pos_in_slot) const
{
    ASSERT(slot_index < SI_MAX);
    return (slot_container_list_[slot_index]->IsEmpty(pos_in_slot) != false);
}

SCSlotContainer* SlotManager::GetSlotContainer(SLOTIDX slot_index) const
{
    const ulong kAcceptablContainers = (1 << SI_INVENTORY)
                                     | (1 << SI_EQUIPMENT)
                                     | (1 << SI_SKILL)
                                     | (1 << SI_QUICK)
                                     | (1 << SI_WAREHOUSE)
                                #if !defined(_NA002217_100728_EP2_PERK_SYSTEM_)
                                     | (1 << SI_STYLE)
                                #endif
                                     | (1 << SI_GUILDWAREHOUSE)
                                     ;
    if (bool in_range = (((1 << slot_index) & kAcceptablContainers) != 0)) {
        SCSlotContainer* selected_container = slot_container_list_[slot_index];
        // the guild warehouse container may have not in list
        return selected_container; 
    }
    return NULL;
}

bool SlotManager::ValidPos(
    const SLOTIDX slot_index, const POSTYPE pos_in_slot, const bool check_empty) const
{
    if (slot_index >= SI_MAX) {
        return false;
    };
    //
    SCSlotContainer* const slot_container = GetSlotContainer(slot_index);
    //
    if (slot_container == NULL) {
        return false;
    };
    // derived functions.
    if (slot_container->ValidState() == false) {
        return false;
    };
    if (slot_container->ValidPos(pos_in_slot) == false) {
        return false;
    };
    // 락여부 체크
    if (slot_container->IsLocked(pos_in_slot)) {
        return false;
    };

    // 시리얼 0 체크
    // 퀵 슬롯때문에 불가능
    //  if (bEmptyCheck && slot_container->GetSlot(atPos).GetSerial() == TEMP_DBSERIAL_VALUE)
    //      return false;

    // default enabled check
    if (check_empty && slot_container->IsEmpty(pos_in_slot)) {
        return false;
    };
    return true;
}

bool SlotManager::ValidContainer(SLOTIDX slot_index) const
{
    SCSlotContainer* const slot_container = GetSlotContainer(slot_index);

    if (slot_container == NULL) {
        return false;
    };
    if (slot_container->ValidState() == false) {
        return false;
    };
    return true;
}

bool SlotManager::ValidState() const
{
    if (player_ == NULL) {
        return false;
    };
    if (player_->GetField() == NULL) {
        return false;
    };
    if (player_->IsDead()) {
        return false;
    };
    return true;
}

//==================================================================================================
// CHANGES: f110406.6L, foreach routines using functor
bool SlotManager::ForeachSlot(
    SLOTIDX slot_index, const ns_functor::ISlotForeachFunctor* functor) const
{
    SCSlotContainer* slot_container = GetSlotContainer(slot_index);
    if (FlowControl::FCAssert(slot_container != NULL) == false) {
        return false;
    };

    const POSTYPE max_number_of_slots = slot_container->GetMaxSlotNum();
    for (POSTYPE pos = 0; pos < max_number_of_slots; ++pos)
    {
        if (slot_container->IsEmpty(pos)) {
            continue;
        }
        SCSlot& slot = slot_container->GetSlot(pos);
        if (functor->operator()(slot_container, slot) == kForeachContinueCondition) {
            continue;
        };
        // stop
        return true;
    }
    return false;
}

bool SlotManager::ForeachSlot(
    SLOTIDX slot_index, const ns_functor::IItemSlotForeachFunctor* functor) const
{
    typedef SCItemSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;
    SCItemSlotContainer* slot_container =
        static_cast<SCItemSlotContainer*>(GetSlotContainer(slot_index));
    if (FlowControl::FCAssert(slot_container != NULL) == false) {
        return false;
    };
    const SLOT_LINEAR_LIST& slot_list = slot_container->GetSlotList();
    SLOT_LINEAR_LIST::const_iterator it = slot_list.begin();
    const SLOT_LINEAR_LIST::const_iterator end = slot_list.end();
    for ( ; it != end; ++it)
    {
        const SCItemSlot& item_slot = static_cast<const SCItemSlot&>(*it->slot);
        if (item_slot.GetItemCode() == 0) {
            continue;
        };
        if (functor->operator()(slot_container, item_slot) == kForeachContinueCondition) {
            continue;
        };
        // stop
        return true;
    }
    return false;
}

//==================================================================================================
