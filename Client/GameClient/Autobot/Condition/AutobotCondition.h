#ifndef _AutobotCondition_H_
#define _AutobotCondition_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <ProgramCommon/wzvector.h>

#include "Singleton.h"

struct BASE_ITEMINFO;
class Item;
class QuickSlot;

class AutobotCondition
{
public:
    enum ItemType
    {
        kItem_All = 0,
        kItem_Normal,
        kItem_Quest,
        kItem_Stone,
        kItem_Weapon,
        kItem_Armor,
        kItem_AC,
        kItem_Heim,
        kItem_Max
    };

    //! 2011.9.15 / i4u4me / 버전업 : 마을로 돌아와서 다시 필드로 나갈건지 플레그 
    //static const int kAutobotFileVersion = 100;
    static const int kAutobotFileVersion = 110;

public:
    AutobotCondition(){}
    ~AutobotCondition(){}

    virtual void Clear() = 0;
    virtual void Save(FILE* fp) = 0;
    virtual void Load(FILE* fp, int file_version) = 0;
    virtual void SaveToBuffer(ArchiveOfStream& ar) = 0;
    virtual void LoadFromBuffer(ArchiveOfStream& ar, int file_version) = 0;
    virtual void CheckValid(SLOTCODE item_code) = 0;

    virtual ItemType ConvertPickupItemType(const BASE_ITEMINFO* item_info);
    virtual ItemType ConvertPickupItemType(Item& item);
    virtual ItemType ConvertPickupItemType(eAUTO_HUNTING_TYPE hunting_type);

};

//------------------------------------------------------------------------------
class AutobotCondition_Pickup : public AutobotCondition
{
public:
    typedef std::set<AutobotCondition::ItemType> ConditionPickupItemSet;

public:
    AutobotCondition_Pickup() { pickupitem_set_.clear(); }

    void Clear();
    void Save(FILE* fp);
    void Load(FILE* fp, int file_version);
    void SaveToBuffer(ArchiveOfStream& ar);
    void LoadFromBuffer(ArchiveOfStream& ar, int file_version);
    void CheckValid(SLOTCODE item_code);

    bool Insert(AutobotCondition::ItemType item_type);
    void Remove(AutobotCondition::ItemType item_type);

    bool IsPossiblePickup(Item& item);
    bool IsPossiblePickup(ItemType item_type);
    bool HasItemType(ItemType item_type);

    ConditionPickupItemSet& pickupitem_set() { return pickupitem_set_; }

    bool SendCheck(AutobotCondition_Pickup& dummy);
    void CopyData(AutobotCondition_Pickup& sorce);

private:
    ConditionPickupItemSet pickupitem_set_;

};
//------------------------------------------------------------------------------
//! AutobotCondition_Recall
//------------------------------------------------------------------------------
class AutobotCondition_Recall : public AutobotCondition
{

public:
    AutobotCondition_Recall() { Clear(); }

    void Clear();
    void Save(FILE* fp);
    void Load(FILE* fp, int file_version);
    void SaveToBuffer(ArchiveOfStream& ar);
    void LoadFromBuffer(ArchiveOfStream& ar, int file_version);
    void CheckValid(SLOTCODE item_code);

    bool IsReturnVillage();

    float inventory_full_percent() const { return inventory_full_percent_; }
    void set_inventory_full_percent(float val) { inventory_full_percent_ = val; }
    DURATYPE warning_dura() const { return warning_dura_; }
    void set_warning_dura(DURATYPE val) { warning_dura_ = val; }
    NUMTYPE heal_item_num() const { return heal_item_num_; }
    void set_heal_item_num(NUMTYPE val) { heal_item_num_ = val; }
    NUMTYPE feed_item_num() const { return feed_item_num_; }
    void set_feed_item_num(NUMTYPE val) { feed_item_num_ = val; }
    bool is_return_field() const { return is_return_field_; }
    void set_return_field(bool val) { is_return_field_ = val; }

    bool SendCheck(AutobotCondition_Recall& dummy);
    void CopyData(AutobotCondition_Recall& sorce);

private:
    float inventory_full_percent_;
    DURATYPE warning_dura_;
    NUMTYPE heal_item_num_;
    NUMTYPE feed_item_num_;

    //! 귀환 했다가 다시 필드로 나갈건지 여부
    bool is_return_field_;
};

//------------------------------------------------------------------------------
//!AutobotCondition_Recovery
//------------------------------------------------------------------------------
class AutobotCondition_Recovery : public AutobotCondition
{
public:
    typedef std::vector< std::pair<float /*hp percent*/, SLOTCODE /*item code*/> > ConditionPotionVector;

public:
    AutobotCondition_Recovery() { Clear(); }

    void Clear();
    void Save(FILE* fp);
    void Load(FILE* fp, int file_version);
    void SaveToBuffer(ArchiveOfStream& ar);
    void LoadFromBuffer(ArchiveOfStream& ar, int file_version);
    void CheckValid(SLOTCODE item_code);

    bool IsRelax();

    bool check_hp_potion();
    bool check_immediate_potion();
    bool check_mp_potion();
    bool check_feed_potion();

    float hp_potion_percent() const { return hp_potion_percent_; }
    float immediate_potion_percent() const { return immediate_potion_percent_; }
    float mp_potion_percent() const { return mp_potion_percent_; }
    float feed_potion_percent() const { return feed_potion_percent_; }
    float relax_hp_percent() const { return relax_hp_percent_; }
    float relax_mp_percent() const { return relax_mp_percent_; }

    void set_hp_potion_percent(float val) { hp_potion_percent_ = val; }
    void set_mp_potion_percent(float val) { mp_potion_percent_ = val; }
    void set_feed_potion_percent(float val) { feed_potion_percent_ = val; }
    void set_immediate_potion_percent(float val) { immediate_potion_percent_ = val; }
    void set_relax_hp_percent(float val) { relax_hp_percent_ = val; }
    void set_relax_mp_percent(float val) { relax_mp_percent_ = val; }

    bool SendCheck(AutobotCondition_Recovery& dummy);
    void CopyData(AutobotCondition_Recovery& sorce);

private:
    float hp_potion_percent_;
    float immediate_potion_percent_;
    float mp_potion_percent_;
    float feed_potion_percent_;
    float relax_hp_percent_;
    float relax_mp_percent_;

};

//------------------------------------------------------------------------------
//! AutobotCondition_Skill
//------------------------------------------------------------------------------
class AutobotCondition_Skill : public AutobotCondition
{
public:
    struct Slot
    {
        eSlotContainerIndex container_index;
        eSlotType slot_type;
        SLOTCODE slot_code;

        //!보조 스킬 설정시에만 사용
        DWORD interval_millisecond;
        DWORD last_use_millisecond;

        Slot()
        { 
            Reset();
        }
        void Reset()
        {
            container_index = SI_MAX_TOTAL;
            slot_type = ST_NONE; 
            slot_code = 0;
            interval_millisecond = 0; 
            last_use_millisecond = 0; 
        }
        bool IsValid()
        {
            return ((container_index != SI_MAX_TOTAL) && 
                    (slot_type != ST_NONE) && 
                    (slot_type != ST_MAX) && 
                    (slot_code > 0) && 
                    (interval_millisecond > 0));
        }
    };

    typedef std::vector<Slot>   ConditionAttackSkillVector;
    typedef std::vector<Slot>   ConditionIntervalSkillVector;

    // 스킬 등록할수 있는 슬롯 갯수
    enum { kSlotCount = 12 };

public:
    AutobotCondition_Skill();
    void Clear();
    void Save(FILE* fp);
    void Load(FILE* fp, int file_version);
    void SaveToBuffer(ArchiveOfStream& ar);
    void LoadFromBuffer(ArchiveOfStream& ar, int file_version);
    void CheckValid(SLOTCODE item_code);

    SLOTCODE GetAttackSkill();
    AutobotCondition_Skill::Slot* GetIntervalSkill();
    int GetAttackCount();

    bool InsertAttackSkill(Slot& slot);
    bool InsertAttackSkill(eSlotContainerIndex container_index, 
                            eSlotType slot_type, 
                            SLOTCODE slot_code);
    bool InsertIntervalSkill(Slot& slot);
    bool InsertIntervalSkill(eSlotContainerIndex container_index, 
                            eSlotType slot_type, 
                            SLOTCODE slot_code, 
                            DWORD interval_millisecond);

    ConditionAttackSkillVector& attack_skill_vector() { return attack_skill_vector_; }
    ConditionIntervalSkillVector& interval_skill_vector() { return interval_skill_vector_; }

    bool SendCheck(AutobotCondition_Skill& dummy);
    void CopyData(AutobotCondition_Skill& sorce);

private:
    ConditionAttackSkillVector attack_skill_vector_;
    ConditionIntervalSkillVector interval_skill_vector_;
};

//------------------------------------------------------------------------------
//! AutobotCondition_Revival
//------------------------------------------------------------------------------
class AutobotCondition_Revival : public AutobotCondition
{
public:
    enum RevivalType
    {
        kRevival_Stay = 0,
        kRevival_Resurrection, //!< 제자리부활
        kRevival_Village,
        kRevival_Max
    };

public:
    AutobotCondition_Revival() { Clear(); }
    void Clear();
    void Save(FILE* fp);
    void Load(FILE* fp, int file_version);
    void SaveToBuffer(ArchiveOfStream& ar);
    void LoadFromBuffer(ArchiveOfStream& ar, int file_version);
    void CheckValid(SLOTCODE item_code);

    RevivalType revival_type() const { return revival_type_; }
    void set_revival_type(RevivalType val) { revival_type_ = val; }

    bool SendCheck(AutobotCondition_Revival& dummy);
    void CopyData(AutobotCondition_Revival& sorce);

private:
    RevivalType revival_type_;
};

//------------------------------------------------------------------------------
//! AutobotCondition_Shop
//------------------------------------------------------------------------------
class AutobotCondition_Shop : public AutobotCondition
{
public:
    typedef std::set<AutobotCondition::ItemType> ConditionSellItemSet;
    typedef std::map<SLOTCODE /*아이템코드*/, NUMTYPE /*필요갯수*/> ConditionBuyItemMap;

public:
    AutobotCondition_Shop() { Clear(); }
    void Clear();
    void Save(FILE* fp);
    void Load(FILE* fp, int file_version);
    void SaveToBuffer(ArchiveOfStream& ar);
    void LoadFromBuffer(ArchiveOfStream& ar, int file_version);
    void CheckValid(SLOTCODE item_code);

    bool IsSellItem(AutobotCondition::ItemType item_type);
    bool IsSellItem(SCItemSlot* inventory_item_slot);
    bool IsBuyItem(SCItemSlot* shop_item_slot);
    bool IsRepair() const { return repair_; }
    NUMTYPE GetBuyItemCount(SLOTCODE item_code);

    void SetRepair(bool val) { repair_ = val; }
    bool InsertSellItem(AutobotCondition::ItemType item_type);
    bool InsertBuyItem(SLOTCODE slot_code, NUMTYPE count);
    void EraseBuyItemByWasteType(eWASTETYPE waste_type);

    ConditionSellItemSet& sellitem_set() { return sellitem_set_; }
    ConditionBuyItemMap& buyitem_map() { return buyitem_map_; }

    bool SendCheck(AutobotCondition_Shop& dummy);
    void CopyData(AutobotCondition_Shop& sorce);
    
private:
    ConditionSellItemSet sellitem_set_;
    ConditionBuyItemMap buyitem_map_;
    bool repair_;
};

//------------------------------------------------------------------------------
//! AutobotCondition_Hunting
//------------------------------------------------------------------------------
class AutobotCondition_Hunting : public AutobotCondition
{
public:
    enum HuntingType
    {
        kHuntingType_Free = 0,
        kHuntingType_Party,
        kHuntingType_Area,
        kHuntingType_Max
    };

    typedef std::set<eNPC_GRADE> ConditionAvoidMonsterSet;

public:
    AutobotCondition_Hunting();

    void Clear();
    void Save(FILE* fp);
    void Load(FILE* fp, int file_version);
    void SaveToBuffer(ArchiveOfStream& ar);
    void LoadFromBuffer(ArchiveOfStream& ar, int file_version);
    void CheckValid(SLOTCODE item_code);

    DWORD GetFollowPlayer();
    bool GetFollowPlayerPosition(OUT WzVector& pos);
    bool NextFollowPlayer(DWORD without_object_key);
    bool IsAvoidMonster(DWORD object_key);
    bool IsAvoidMonster(eNPC_GRADE npc_grade);
    bool IsMonsterInValidArea(DWORD object_key);
    bool IsValidArea(WzVector& pos);

    float target_range() const { return target_range_; }
    HuntingType hunting_type() const { return hunting_type_; }
    int party_follow_no() const { return party_follow_no_; }
    WzVector area_position() const { return area_position_; }
    float area_range() const { return area_range_; }

    void set_target_range(float val) { target_range_ = val; }
    void set_hunting_type(HuntingType val) { hunting_type_ = val; }
    void set_party_follow_no(int val) { party_follow_no_ = val; }
    void set_area_position(WzVector val) { area_position_ = val; }
    void set_area_range(float val) { area_range_ = val; }

    ConditionAvoidMonsterSet& avoid_monster_set() { return avoid_monster_set_; }

    bool SendCheck(AutobotCondition_Hunting& dummy);
    void CopyData(AutobotCondition_Hunting& sorce);

private:
    ConditionAvoidMonsterSet avoid_monster_set_;

    //! 타겟팅 반경
    float target_range_;                // 타겟팅을 하기위한 몬스터와의 거리, 최대 사정거리보다 좁으면 안됨

    //! 사냥 종류
    HuntingType hunting_type_;

    //! 파티원 사냥 일때만 사용
    int party_follow_no_;

    //! 특정 영역 사냥일때만 사용
    WzVector area_position_;
    float area_range_;
};
//------------------------------------------------------------------------------
//! AutobotCondition_Community
//------------------------------------------------------------------------------
class AutobotCondition_Community : public AutobotCondition
{
public:
    enum ShoutingType
    {
        kShouting_None = 0,
        kShouting_Normal,
        kShouting_World,
        kShouting_Max
    };
    enum PartyType
    {
        kParty_Stay = 0,
        kParty_Agree,
        kParty_Ignore,
        kParty_Max
    };

public:
    AutobotCondition_Community() { Clear(); }
    void Clear();
    void Save(FILE* fp);
    void Load(FILE* fp, int file_version);
    void SaveToBuffer(ArchiveOfStream& ar);
    void LoadFromBuffer(ArchiveOfStream& ar, int file_version);
    void CheckValid(SLOTCODE item_code);

    bool IsShouting();
    bool IsChannel();

    bool is_save_chat_message() const { return save_chat_message_; }
    PartyType party_type() const { return party_type_; }

    TCHAR* channel_level() { return channel_level_; }
    TCHAR* channel_name() { return channel_name_; }
    ShoutingType shouting_type() const { return shouting_type_; }
    int shouting_second() const { return shouting_second_; }
    TCHAR* shouting_message() { return shouting_message_; }

    void set_channel(TCHAR* level, TCHAR* name);
    void set_shouting(ShoutingType shout_type, int second, TCHAR* message);
    void set_party_type(AutobotCondition_Community::PartyType val) { party_type_ = val; }
    void set_save_chat_message(bool val) { save_chat_message_ = val; }

    bool SendCheck(AutobotCondition_Community& dummy);
    void CopyData(AutobotCondition_Community& sorce);

private:
    //자동채널
    TCHAR channel_level_[MAX_ROOMTITLE_LENGTH];
    TCHAR channel_name_[MAX_ROOMTITLE_LENGTH];

    //! 외치기
    ShoutingType shouting_type_;
    int shouting_second_;
    TCHAR shouting_message_[MAX_CHAT_LEN];

    //! 파티수락
    PartyType party_type_;

    //!채팅저장
    bool save_chat_message_;
};

#endif
