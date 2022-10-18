#include "stdafx.h"
#include "CharacterInfoExManager.h"
#include "ItemManager.h"

CharacterInfoExManager::CharacterInfoExManager()
{

}

CharacterInfoExManager::~CharacterInfoExManager()
{

}

void CharacterInfoExManager::Init( Player* _player )
{
    player_ = _player;

    previous_daily_quest_reset_time = 0;
}

ToggleItemInfos& CharacterInfoExManager::GetToggleItemInfos()
{
    return toggle_item_infos_;
}

nsSlot::TOGGLE_ITEMS* CharacterInfoExManager::GetToggleItemInfo( const ToggleItemType toggle_item_type )
{
    if((toggle_item_type >= kToggleItemMax) || (toggle_item_type < 0))
        return NULL;

    return &toggle_item_infos_.toggle_item_info_[toggle_item_type];
}

const BattleGroundRecordInfos& CharacterInfoExManager::GetBattleGroundRecordInfos() const
{
    return battle_ground_record_infos_;
}

void CharacterInfoExManager::SetBattleGroundRecordInfos(const BattleGroundRecordInfos& battle_ground_record_infos)
{
    memcpy(&battle_ground_record_infos_, 
           &battle_ground_record_infos, 
           sizeof(battle_ground_record_infos));
}

BattleGroundRecord* CharacterInfoExManager::GetBattleGroundRecord(const int index)
{
    ASSERT((index < BattleGroundRecordInfos::kRecordMaxCount) && (index >= 0));
    return &(battle_ground_record_infos_.battle_ground_record[index]);
}

void CharacterInfoExManager::OnEnterField()
{
#ifdef _NA_006372_20130108_BADGE_ITEM
    TurnOnOffToggleItems(true); // turn on badge items...

    nsSlot::TOGGLE_ITEMS* toggle_item_info = GetToggleItemInfo(kToggleItem_PCBang);
    if (toggle_item_info != NULL && toggle_item_info->is_equiped_) {
        player_->GetItemManager()->UpdateItemAttributes(true, false);
    }
#endif //_NA_006372_20130108_BADGE_ITEM
}

void CharacterInfoExManager::OnLeaveField()
{
#ifdef _NA_006372_20130108_BADGE_ITEM
    //backup
    ToggleItemInfos toggle_item_info;
    ToggleItemInfos& toggle_item_info_container = GetToggleItemInfos();
    memcpy(&toggle_item_info, &toggle_item_info_container, sizeof(toggle_item_info));

    TurnOnOffToggleItems(false); // turn off badge items...

    //restore
    memcpy(&toggle_item_info_container, &toggle_item_info, sizeof(toggle_item_info));
#endif //_NA_006372_20130108_BADGE_ITEM
}

// ÀüÃ¼¸¦ 
// ÄÓ¶§´Â ÇÇ¾¾¹æ ÄÑ°í ¹îÁö ÄÑ°í
// ²ø¶§´Â ¹îÁö ²ô°í, ÇÇ¾¾¹æ ²ô°í...
ToggleItemType turn_priority[] = {
    kToggleItemMax, // guard data
    kToggleItem_PCBang,
    kToggleItem_Badge,
    kToggleItemMax, // guard data
};
const int size_of_turn_priority = sizeof(turn_priority) / sizeof(turn_priority[0]);
const ToggleItemType* front = &turn_priority[0];
const ToggleItemType* back  = &turn_priority[size_of_turn_priority-1];

struct TOGGLE_ITEM_PRIORITY
{
    const ToggleItemType* begin_;
    const ToggleItemType* end_;
    bool direction_;

    const ToggleItemType* begin() const { return begin_; }
    const ToggleItemType* end() const { return end_; };
    const ToggleItemType* next(const ToggleItemType* it) { 
        return (direction_) ? ++it : --it; 
    };
    
    TOGGLE_ITEM_PRIORITY(bool direction)
    {
        direction_ = direction;
        if (direction == true) {
            begin_ = front+1, end_ = back;
        } else {
            begin_ = back-1, end_ = front;
        }
    }
};

void CharacterInfoExManager::TurnOnOffToggleItems(bool turn_on)
{
    TOGGLE_ITEM_PRIORITY toggle_item(turn_on);

#ifdef _NA_006372_20130108_BADGE_ITEM
    // activate turned on items
    for (const ToggleItemType* it = toggle_item.begin(); it != toggle_item.end(); it = toggle_item.next(it))
    {
        nsSlot::TOGGLE_ITEMS* toggle_item_info = GetToggleItemInfo(*it);
        if (toggle_item_info == NULL)
            continue;

#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
        if (toggle_item_info->toggle_item_type_ == kToggleItem_PCBang) {
            if (!player_->GetPCRoomSts() && player_->GetReservedValue()) {
                toggle_item_info->clear();
                continue;
            }
        }
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM

        if (toggle_item_info->is_equiped_)
        {
            POSTYPE item_pos = toggle_item_info->item_pos_;
            nsSlot::ItemFunctionToggle& toggle_item_handler = nsSlot::ItemFunctionToggle( \
                player_->GetItemManager(), toggle_item_info->toggle_item_type_);
            if (toggle_item_handler.Equip(item_pos, turn_on) == false) {
                toggle_item_info->clear();
            }
        }
    }
#endif //_NA_006372_20130108_BADGE_ITEM
}

//!~ _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
CTime CharacterInfoExManager::GetDailyQuestResetUseTime()
{
    return previous_daily_quest_reset_time;
}

void CharacterInfoExManager::SetDailyQuestResetUseTime( CTime daily_quest_reset_time )
{
    previous_daily_quest_reset_time = daily_quest_reset_time;
}

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
void CharacterInfoExManager::SetGoldRushRecordInfos(const GoldRushRecordInfos& goldrush_record_infos)
{
    memcpy(&goldrush_record_infos_, &goldrush_record_infos,
        sizeof(goldrush_record_infos));
}

GoldRushRecord* CharacterInfoExManager::GetGoldRushRecord( const int index )
{
    ASSERT((index < GoldRushRecordInfos::kRecordMaxCount) && (index >= 0));
    if (index > GoldRushRecordInfos::kRecordMaxCount || index < 0) {
        return NULL;
    }
    return &(goldrush_record_infos_.goldrush_record[index]);
}   
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
//~! _NA_000000_20130521_DAILY_QUEST_RESET_ITEM

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

void CharacterInfoExManager::SetAttendanceInfos( const AttendanceInfo& attendance_infos )
{
    attendance_manager.SetAttendanceInfos(attendance_infos);
}

#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#ifdef _NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE
void CharacterInfoExManager::TurnOnOffToggleItems( const eITEMTYPE item_type, bool turn_on )
{
    nsSlot::TOGGLE_ITEMS* toggle_item_info = NULL;

    switch (item_type)
    {
    case eITEMTYPE_PCBANG:
        toggle_item_info = GetToggleItemInfo(kToggleItem_PCBang);
        break;
    case eITEMTYPE_RENDER_EFFECT_ITEM:
        toggle_item_info = GetToggleItemInfo(kToggleItem_Badge);
        break;
    }
    
    if (toggle_item_info == NULL)
        return;

    if (toggle_item_info->toggle_item_type_ == kToggleItem_PCBang) {
        if (!player_->GetPCRoomSts() && player_->GetReservedValue()) {
            toggle_item_info->clear();
            return;
        }
    }

    //PC¹îÁö¸¦ ÀåÂø/ÇØÁ¦ÇÒ¶©
    //ÀÏ¹Ý ¹îÁö¸¦ ´Ù½Ã ²°´Ù°¡ ²¸Áà¾ß ÇÑ´Ù
    POSTYPE badge_item_pos = 0;
    if (item_type == eITEMTYPE_PCBANG) {        
        badge_item_pos = UnEquipBadge();
    }

    if (toggle_item_info->is_equiped_)
    {
        POSTYPE item_pos = toggle_item_info->item_pos_;
        nsSlot::ItemFunctionToggle& toggle_item_handler = nsSlot::ItemFunctionToggle( \
            player_->GetItemManager(), toggle_item_info->toggle_item_type_);
        if (toggle_item_handler.Equip(item_pos, turn_on) == false) {
            toggle_item_info->clear();
        }
    }
    
    if (item_type == eITEMTYPE_PCBANG && badge_item_pos != 0) {
        EquipBadge(badge_item_pos);
    }
}

const POSTYPE CharacterInfoExManager::UnEquipBadge()
{
    POSTYPE badge_item_pos;
    nsSlot::TOGGLE_ITEMS* const badge_info = \
        player_->GetExtraInfoManager().GetToggleItemInfo(kToggleItem_Badge);
    
    if (badge_info == NULL) {
        return 0;
    }
    if (badge_info->is_equiped_ == false) {
        return 0;
    }

    badge_item_pos = badge_info->item_pos_;

    nsSlot::ItemFunctionToggle& badge_toggle_item_handler = \
        nsSlot::ItemFunctionToggle(player_->GetItemManager(), kToggleItem_Badge);

    if (badge_toggle_item_handler.Equip(badge_item_pos, false) == false)
    {
        badge_info->clear();
        return 0;
    }

    return badge_item_pos;
}

VOID CharacterInfoExManager::EquipBadge(POSTYPE item_pos)
{
    if (item_pos == 0) {
        return;
    }

    nsSlot::ItemFunctionToggle& badge_toggle_item_handler = \
        nsSlot::ItemFunctionToggle(player_->GetItemManager(), kToggleItem_Badge);

    if (badge_toggle_item_handler.Equip(item_pos, true) == false) {

        nsSlot::TOGGLE_ITEMS* const badge_info = \
            player_->GetExtraInfoManager().GetToggleItemInfo(kToggleItem_Badge);
        if (badge_info != NULL && badge_info->is_equiped_ == false) {
            badge_info->clear();
        }
    }
}
#endif //_NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
void CharacterInfoExManager::SetMiniGameBingoInfos( MiniGameBingoInfos& minigame_bingo_infos )
{
    minigame_manager.SetMiniGameBingoInfos(minigame_bingo_infos);
}
#endif // _NA_008404_20150712_MINIGAME_SYSTEM