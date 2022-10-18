#include "stdafx.h"
#include "DBQueryProxyNode.h"
#include "QueryObjects/DBHandler.h"


//==================================================================================================
#pragma warning(push, 4)
namespace nsQuery {
;

// namespace nsQuery utility
// format = slot_index|pos|code
//              00    |000|00000
inline void QuickConvertToInt32(const QUICKPART& quick_part, int* result_value)
{
    int making_slot_index = quick_part.SlotIdx;
    int making_slot_pos = quick_part.Pos;
    int making_code = quick_part.Code;
    // format = slot_index|pos|code
    //              00    |000|00000
    int making_value = (making_slot_index * 100000000) +
                       (making_slot_pos   *    100000) +
                       (making_code);
    *result_value = making_value;
};

inline void QuickConvertToQuickSlot(const int converted_value, QUICKPART* quick_part)
{
    int target_value = converted_value;
    int making_code = target_value % 100000;
    ;   target_value /= 100000;
    int making_slot_pos = target_value % 1000;
    ;   target_value /= 1000;
    int making_slot_index = target_value;
    quick_part->SlotIdx = static_cast<SLOTIDX>(making_slot_index);
    quick_part->Pos = static_cast<POSTYPE>(making_slot_pos);
    quick_part->Code = static_cast<SLOTCODE>(making_code);
}

//==================================================================================================

void SlotUpdateNode::UpdateAndCompareData()
{
    if (record_changed_ != this->eRecordChange_Default) {
        return;
    };
    
    const bool new_insert = commited_data_.slot_index_ != latest_data_.slot_index_ ||
                            commited_data_.tab_order_ != latest_data_.tab_order_;
    if (new_insert) {
        record_changed_ = this->eRecordChange_NewInsert;
        return;
    }
    bool changed = false;
    const int* db_data_it = commited_data_.slot_array_;
    const int* latest_data_it = latest_data_.slot_array_,
             * const leatest_data_end = &latest_data_.slot_array_[_countof(latest_data_.slot_array_)];
    for ( ; latest_data_it != leatest_data_end; ++latest_data_it, ++db_data_it)
    {
        if (*latest_data_it != *db_data_it) {
            changed = true;
            break;
        }
    }

    if (changed) {
        record_changed_ = this->eRecordChange_Changed;
        return;
    }
    ; // no changed
    return;
}

bool DBQueryProxyNode::CharacterNode::SetupDefaultSkillInfo(eCHAR_TYPE char_type)
{
    const SkillSlotUpdateTable& default_table = _GetDefaultSkillTable(char_type);
    skill_table_ = default_table;
    return true;
}

bool DBQueryProxyNode::CharacterNode::SetupDefaultQuickInfo()
{
    const QuickSlotUpdateTable& default_table = _GetDefaultQuickTable();
    quick_table_ = default_table;
    return true;
}

//static
const QuickSlotUpdateTable&
DBQueryProxyNode::CharacterNode::_GetDefaultQuickTable()
{
    static QuickSlotUpdateTable* default_table_static_ = NULL;
    static struct AutoClosure {
        AutoClosure(QuickSlotUpdateTable** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        QuickSlotUpdateTable** static_table_;
    } auto_closure(&default_table_static_);
    //
    if (default_table_static_ != NULL) {
        return *default_table_static_;
    }
    // setup default perk information
    default_table_static_ = new QuickSlotUpdateTable;
    QuickSlotUpdateTable& default_table = *default_table_static_; // alias
    ZeroMemory(&default_table, sizeof(default_table));
    for (SlotUpdateNode* record_it = default_table.records_;
         record_it != &default_table.records_[_countof(default_table.records_)];
         ++record_it)
    {
        SlotResultRecord* latest_info = &record_it->latest_data_;
        latest_info->slot_index_ = default_table.kSlotIndex;
        latest_info->tab_order_ = static_cast<uint8_t>(record_it - default_table.records_);
    }

    QUICKPART quick_part;
    ZeroMemory(&quick_part, sizeof(quick_part));
    if (SlotUpdateNode* record_it = &default_table.records_[0])
    {
        SlotResultRecord* latest_info = &record_it->latest_data_;
        quick_part.SlotIdx = SI_SKILL;
        quick_part.Pos = 0;
        quick_part.Code = 35000; // action: attack
        QuickConvertToInt32(quick_part, &latest_info->slot01_);
    }
    if (SlotUpdateNode* record_it = &default_table.records_[1])
    {
        SlotResultRecord* latest_info = &record_it->latest_data_;
        quick_part.SlotIdx = SI_SKILL;
        quick_part.Pos = 0;
        quick_part.Code = 35001; // action: pickup
        QuickConvertToInt32(quick_part, &latest_info->slot03_);
        quick_part.Code = 35003; // action: stand/sit
        QuickConvertToInt32(quick_part, &latest_info->slot04_);
        // CHANGES: f110222.5L
        quick_part.Code = 35021; // action: identify unknown socket
        QuickConvertToInt32(quick_part, &latest_info->slot05_);
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
        quick_part.Code = 35023; // action: perk_group_change
        QuickConvertToInt32(quick_part, &latest_info->slot06_);
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    }
    return *default_table_static_;
}

//==================================================================================================

struct SkillSlotUpdateTableForCharacters
{
    SkillSlotUpdateTable tables_[eCHAR_TYPE_MAX];
};

//static
const SkillSlotUpdateTable&
DBQueryProxyNode::CharacterNode::_GetDefaultSkillTable(eCHAR_TYPE char_type)
{
    static SkillSlotUpdateTableForCharacters* default_table_static_ = NULL;
    static struct AutoClosure {
        AutoClosure(SkillSlotUpdateTableForCharacters** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        SkillSlotUpdateTableForCharacters** static_table_;
    } auto_closure(&default_table_static_);
    //
    if (default_table_static_ != NULL)
    {
        switch (char_type)
        {
        case eCHAR_BERSERKER:
        case eCHAR_DRAGON:
        case eCHAR_SHADOW:
        case eCHAR_VALKYRIE:
        case eCHAR_ELEMENTALIST:
        case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            return default_table_static_->tables_[char_type];
        default:
            assert(!"invalid char_type");
            return default_table_static_->tables_[0];
        }
    };
    // setup default perk information
    default_table_static_ = new SkillSlotUpdateTableForCharacters;
    ZeroMemory(default_table_static_, sizeof(*default_table_static_));
    //
    switch (char_type)
    {
    case eCHAR_BERSERKER:
        {
            SkillSlotUpdateTable& default_table = \
                default_table_static_->tables_[char_type];
            for (SlotUpdateNode* record_it = default_table.records_;
                 record_it != &default_table.records_[_countof(default_table.records_)];
                 ++record_it)
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot_index_ = default_table.kSlotIndex;
                latest_info->tab_order_ = static_cast<uint8_t>(record_it - default_table.records_);
            }
            if (SlotUpdateNode* record_it = &default_table.records_[0])
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot01_ = eSTYLE_BERSERKER_PUNCH; //Punch
                latest_info->slot02_ = eSTYLE_TWOHANDSWORD_NORMAL; //Defender Normal
                latest_info->slot03_ = eSTYLE_TWOHANDAXE_NORMAL; //Berserk Normal
            }
        };
        break;
    case eCHAR_DRAGON:
        {
            SkillSlotUpdateTable& default_table = \
                default_table_static_->tables_[char_type];
            for (SlotUpdateNode* record_it = default_table.records_;
                 record_it != &default_table.records_[_countof(default_table.records_)];
                 ++record_it)
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot_index_ = default_table.kSlotIndex;
                latest_info->tab_order_ = static_cast<uint8_t>(record_it - default_table.records_);
            }
            if (SlotUpdateNode* record_it = &default_table.records_[0])
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot01_ = eSTYLE_DRAGON_PUNCH; //Punch
                latest_info->slot02_ = eSTYLE_ONEHANDSWORD_NORMAL; //Dual Sword Normal
                latest_info->slot03_ = eSTYLE_SPEAR_NORMAL; //Spear Normal
            }
        };
        break;
    case eCHAR_SHADOW:
        {
            SkillSlotUpdateTable& default_table = \
                default_table_static_->tables_[char_type];
            for (SlotUpdateNode* record_it = default_table.records_;
                 record_it != &default_table.records_[_countof(default_table.records_)];
                 ++record_it)
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot_index_ = default_table.kSlotIndex;
                latest_info->tab_order_ = static_cast<uint8_t>(record_it - default_table.records_);
            }
            if (SlotUpdateNode* record_it = &default_table.records_[0])
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot01_ = eSTYLE_SHADOW_PUNCH; //Punch
                latest_info->slot02_ = eSTYLE_SHADOW_DAGGER_NORMAL; //Daggar Normal
                latest_info->slot03_ = eSTYLE_SHADOW_WHIP_NORMAL; //Whip Normal
            }
        };
        break;
    case eCHAR_VALKYRIE:
        {
            SkillSlotUpdateTable& default_table = \
                default_table_static_->tables_[char_type];
            for (SlotUpdateNode* record_it = default_table.records_;
                 record_it != &default_table.records_[_countof(default_table.records_)];
                 ++record_it)
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot_index_ = default_table.kSlotIndex;
                latest_info->tab_order_ = static_cast<uint8_t>(record_it - default_table.records_);
            }
            if (SlotUpdateNode* record_it = &default_table.records_[0])
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot01_ = eSTYLE_VALKYRIE_PUNCH; //Punch
                latest_info->slot02_ = eSTYLE_ONEHANDCROSSBOW_NORMAL; //Crossbow Normal
                latest_info->slot03_ = eSTYLE_ETHER_NORMAL; //Ether weapon Normal
            }
        };
        break;
    case eCHAR_ELEMENTALIST:
        {
            SkillSlotUpdateTable& default_table = \
                default_table_static_->tables_[char_type];
            for (SlotUpdateNode* record_it = default_table.records_;
                 record_it != &default_table.records_[_countof(default_table.records_)];
                 ++record_it)
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot_index_ = default_table.kSlotIndex;
                latest_info->tab_order_ = static_cast<uint8_t>(record_it - default_table.records_);
            }
            if (SlotUpdateNode* record_it = &default_table.records_[0])
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot01_ = eSTYLE_MAGICIAN_PUNCH; //Punch
                latest_info->slot02_ = eSTYLE_STAFF_NORMAL; //Staff Normal
                latest_info->slot03_ = eSTYLE_ORB_NORMAL; //Orb Normal
            }
        };
        break;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {
            SkillSlotUpdateTable& default_table = default_table_static_->tables_[char_type];
            for (SlotUpdateNode* record_it = default_table.records_;
                record_it != &default_table.records_[_countof(default_table.records_)];
                ++record_it)
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot_index_ = default_table.kSlotIndex;
                latest_info->tab_order_ = static_cast<uint8_t>(record_it - default_table.records_);
            }
            if (SlotUpdateNode* record_it = &default_table.records_[0])
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot01_ = eSTYLE_MYSTIC_PUNCH;
                latest_info->slot02_ = eSTYLE_MYSTIC_GUNTLET_NORMAL;
                latest_info->slot03_ = eSTYLE_MYSTIC_POLEARM_NORMAL;
            }
        };
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        {
            SkillSlotUpdateTable& default_table = default_table_static_->tables_[char_type];
            for (SlotUpdateNode* record_it = default_table.records_;
                record_it != &default_table.records_[_countof(default_table.records_)];
                ++record_it)
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot_index_ = default_table.kSlotIndex;
                latest_info->tab_order_ = static_cast<uint8_t>(record_it - default_table.records_);
            }
            if (SlotUpdateNode* record_it = &default_table.records_[0])
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot01_ = eSTYLE_HELLROID_PUNCH;
                latest_info->slot02_ = eSTYLE_HELLROID_WEAPON1;
            }
        };
        break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAR_WITCHBLADE:
        {
            SkillSlotUpdateTable& default_table = default_table_static_->tables_[char_type];
            for (SlotUpdateNode* record_it = default_table.records_;
                record_it != &default_table.records_[_countof(default_table.records_)];
                ++record_it)
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot_index_ = default_table.kSlotIndex;
                latest_info->tab_order_ = static_cast<uint8_t>(record_it - default_table.records_);
            }
            if (SlotUpdateNode* record_it = &default_table.records_[0])
            {
                SlotResultRecord* latest_info = &record_it->latest_data_;
                latest_info->slot01_ = eSTYLE_WITCHBLADE_PUNCH;
                latest_info->slot02_ = eSTYLE_WITCHBLADE_ARCBLADE;
            }
        }
        break;
#endif ////_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    default:
        assert(!"invalid char_type");
        return default_table_static_->tables_[0];
    }
    return default_table_static_->tables_[char_type];
}

// G->D update stream
void DBQueryProxyNode::CharacterNode::StoreSkill(const SKILL_TOTAL_INFO& data_list)
{
    const int number_of_slots = data_list.m_Count <= _countof(data_list.m_Slot)
                              ? data_list.m_Count : _countof(data_list.m_Slot);
    assert(data_list.m_Count <= MAX_SKILL_SLOT_NUM);
    ;{  // clear latest information
        SlotUpdateNode* update_it = skill_table_.records_,
                      * const update_end = &skill_table_.records_[_countof(skill_table_.records_)];
        for (int tab_order = 0; update_it != update_end; ++update_it, ++tab_order) {
            ZeroMemory(&update_it->latest_data_, sizeof(update_it->latest_data_));
            update_it->latest_data_.slot_index_ = skill_table_.kSlotIndex;
            update_it->latest_data_.tab_order_ = static_cast<uint8_t>(tab_order);
        }
    };
    const SKILL_TOTAL_INFO::SLOT_TYPE* slot_it = data_list.m_Slot,
                                     * slot_end = &data_list.m_Slot[number_of_slots];
    BOOST_STATIC_ASSERT(SlotResultRecord::kMaxNumberOfSlots == 8); // 2^n
    for (; slot_it != slot_end; ++slot_it)
    {
        const int position = slot_it->m_Pos;
        if (position >= _countof(data_list.m_Slot)) {
            continue;
        }
        const int tab_order = position >> 3;
        const int index_in_tab = position & (SlotResultRecord::kMaxNumberOfSlots - 1);
        SlotUpdateNode* const update_node = &skill_table_.records_[tab_order];
        nsQuery::SlotResultRecord* const latest_info = &update_node->latest_data_;
        ;{
            int* offset_value = &latest_info->slot_array_[index_in_tab];
            *offset_value = slot_it->m_Stream.Part.wCode;
        };
    }
    ;{  // update check
        SlotUpdateNode* update_it = skill_table_.records_,
                      * const update_end = &skill_table_.records_[_countof(skill_table_.records_)];
        for ( ; update_it != update_end; ++update_it) {
            update_it->UpdateAndCompareData();
        }
    };
}

// D->G send stream
void DBQueryProxyNode::CharacterNode::LoadSkill(SKILL_TOTAL_INFO* data_list) const
{
    BOOST_STATIC_ASSERT(MAX_SKILL_SLOT_NUM == _countof(data_list->m_Slot));
    const int number_of_max_slots = _countof(data_list->m_Slot);
    //
    data_list->m_Count = 0;
    ZeroMemory(data_list->m_Slot, sizeof(data_list->m_Slot));
    //
    int position = 0, number_of_align = 0;
    for (const SlotUpdateNode* record_it = skill_table_.records_;
         position < number_of_max_slots &&
         record_it != &skill_table_.records_[_countof(skill_table_.records_)];
         ++record_it)
    {
        //const int tab_order = static_cast<int>(record_it - skill_table_.records_);
        const nsQuery::SlotResultRecord* const latest_info = &record_it->latest_data_;
        for (const int* value_it = latest_info->slot_array_;
             position < number_of_max_slots &&
             value_it != &latest_info->slot_array_[_countof(latest_info->slot_array_)];
             ++value_it, ++position)
        {
            if (*value_it == 0) {
                continue;
            }
            SKILL_TOTAL_INFO::SLOT_TYPE* slot_it = &data_list->m_Slot[number_of_align];
            slot_it->m_Pos = static_cast<POSTYPE>(position);
            slot_it->m_Stream.Part.wCode = static_cast<SLOTCODE>(*value_it);
            ++number_of_align;
        }
    }
    data_list->m_Count = static_cast<POSTYPE>(number_of_align);
}

// 
// G->D update stream
void DBQueryProxyNode::CharacterNode::StoreQuick(const QUICK_TOTAL_INFO& data_list)
{
    const int number_of_slots = data_list.m_Count <= _countof(data_list.m_Slot)
                              ? data_list.m_Count : _countof(data_list.m_Slot);
    assert(data_list.m_Count <= MAX_SKILL_SLOT_NUM);
    ;{  // clear latest information
        SlotUpdateNode* update_it = quick_table_.records_,
                      * const update_end = &quick_table_.records_[_countof(quick_table_.records_)];
        for (int tab_order = 0; update_it != update_end; ++update_it, ++tab_order) {
            ZeroMemory(&update_it->latest_data_, sizeof(update_it->latest_data_));
            update_it->latest_data_.slot_index_ = quick_table_.kSlotIndex;
            update_it->latest_data_.tab_order_ = static_cast<uint8_t>(tab_order);
        }
    };
    const QUICK_TOTAL_INFO::SLOT_TYPE* slot_it = data_list.m_Slot,
                                     * slot_end = &data_list.m_Slot[number_of_slots];
    BOOST_STATIC_ASSERT(SlotResultRecord::kMaxNumberOfSlots == 8); // 2^n
    for (; slot_it != slot_end; ++slot_it)
    {
        const int position = slot_it->m_Pos;
        if (position >= _countof(data_list.m_Slot)) {
            continue;
        }
        const int tab_order = position >> 3;
        const int index_in_tab = position & (SlotResultRecord::kMaxNumberOfSlots - 1);
        SlotUpdateNode* const update_node = &quick_table_.records_[tab_order];
        nsQuery::SlotResultRecord* const latest_info = &update_node->latest_data_;
        ;{
            int* offset_value = &latest_info->slot_array_[index_in_tab];
            QuickConvertToInt32(slot_it->m_Stream.Part, offset_value);
        };
    }
    ;{  // update check
        SlotUpdateNode* update_it = quick_table_.records_,
                      * const update_end = &quick_table_.records_[_countof(quick_table_.records_)];
        for ( ; update_it != update_end; ++update_it) {
            update_it->UpdateAndCompareData();
        }
    };
}

// D->G send stream
void DBQueryProxyNode::CharacterNode::LoadQuick(QUICK_TOTAL_INFO* data_list) const
{
    BOOST_STATIC_ASSERT(MAX_QUICK_SLOT_NUM == _countof(data_list->m_Slot));
    const int number_of_max_slots = _countof(data_list->m_Slot);
    //
    data_list->m_Count = 0;
    ZeroMemory(data_list->m_Slot, sizeof(data_list->m_Slot));
    //
    int position = 0, number_of_align = 0;
    for (const SlotUpdateNode* record_it = quick_table_.records_;
         position < number_of_max_slots &&
         record_it != &quick_table_.records_[_countof(quick_table_.records_)];
         ++record_it)
    {
        //const int tab_order = static_cast<int>(record_it - quick_table_.records_);
        const nsQuery::SlotResultRecord* const latest_info = &record_it->latest_data_;
        for (const int* value_it = latest_info->slot_array_;
             position < number_of_max_slots &&
             value_it != &latest_info->slot_array_[_countof(latest_info->slot_array_)];
             ++value_it, ++position)
        {
            if (*value_it == 0) {
                continue;
            }
            QUICK_TOTAL_INFO::SLOT_TYPE* slot_it = &data_list->m_Slot[number_of_align];
            slot_it->m_Pos = static_cast<POSTYPE>(position);
            // format = slot_index|pos|code
            //              00    |000|00000
            QuickConvertToQuickSlot(*value_it, &slot_it->m_Stream.Part);
            ++number_of_align;
        }
    }
    data_list->m_Count = static_cast<POSTYPE>(number_of_align);
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_SlotInfo_Select& query_result)
{
    // 1st load
    const int number_of_rows = query_result.GetResultRowNum();
    const Query_SlotInfo_Select::sQueryParam& parameters = query_result.parameters_[0];
    if (false)
        ;
    else if (parameters.slot_index_ == SkillSlotUpdateTable::kSlotIndex)
    {
        const eCHAR_TYPE char_type = static_cast<eCHAR_TYPE>(db_character_->GetInfo()->m_byClassCode);
        for (int i = 0; i < number_of_rows; ++i)
        {
            const Query_SlotInfo_Select::sQueryResult& record = query_result.result_records_[i];
            SlotUpdateNode* const update_node = &skill_table_.records_[record.tab_order_];
            update_node->latest_data_ = record;
            update_node->commited_data_ = record;
        }
        if (number_of_rows == 0) {
            SetupDefaultSkillInfo(char_type);
        }
    }
    else if (parameters.slot_index_ == QuickSlotUpdateTable::kSlotIndex)
    {
        for (int i = 0; i < number_of_rows; ++i)
        {
            const Query_SlotInfo_Select::sQueryResult& record = query_result.result_records_[i];
            SlotUpdateNode* const update_node = &quick_table_.records_[record.tab_order_];
            update_node->latest_data_ = record;
            update_node->commited_data_ = record;
        }
        if (number_of_rows == 0) {
            SetupDefaultQuickInfo();
        }
    }
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_SlotInfo_Insert& query_result)
{
    const Query_SlotInfo_Insert::sQueryParam& parameters = query_result.parameters_[0];
    SlotUpdateNode* update_node = NULL;
    if (false)
        ;
    else if (parameters.slot_index_ == SkillSlotUpdateTable::kSlotIndex) {
        update_node = &skill_table_.records_[parameters.tab_order_];
    }
    else if (parameters.slot_index_ == QuickSlotUpdateTable::kSlotIndex) {
        update_node = &quick_table_.records_[parameters.tab_order_];
    };
    if (update_node == NULL) {
        return false;
    }

    ASSERT(update_node->query_ == &query_result &&
           update_node->record_changed_ == update_node->eRecordChange_NewInsert);
    update_node->record_changed_ = update_node->eRecordChange_Default;
    update_node->query_ = NULL; // external delete control
    update_node->commited_data_ = parameters;
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_SlotInfo_Update& query_result)
{
    const Query_SlotInfo_Update::sQueryParam& parameters = query_result.parameters_[0];
    SlotUpdateNode* update_node = NULL;
    if (false)
        ;
    else if (parameters.slot_index_ == SkillSlotUpdateTable::kSlotIndex) {
        update_node = &skill_table_.records_[parameters.tab_order_];
    }
    else if (parameters.slot_index_ == QuickSlotUpdateTable::kSlotIndex) {
        update_node = &quick_table_.records_[parameters.tab_order_];
    };
    if (update_node == NULL) {
        return false;
    }

    ASSERT(update_node->query_ == &query_result &&
           update_node->record_changed_ == update_node->eRecordChange_Changed);
    update_node->record_changed_ = update_node->eRecordChange_Default;
    update_node->query_ = NULL; // external delete control
    update_node->commited_data_ = parameters;
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::UpdateSlotInfo()
{
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL)
    {
        return true;    // °­Á¦..transaction success..
    }
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    const bool loaded_skill = (shared_.state & this->eState_Loaded_SkillInfo) != 0;
    const bool loaded_quick = (shared_.state & this->eState_Loaded_QuickInfo) != 0;
    if (loaded_skill == false && loaded_quick == false) {
        return true;
    };
    // CHANGES: f110628.1L, the load balance updating implemented. updater='db_user->DBQuery'
    const int number_of_pending_queries = \
        g_DBHandler.GetNumberOfDBConnectionQueuingQueries(db_user);
    if (number_of_pending_queries > g_DBHandler.kMaxNumberOfSpecializedConcurrentBusyUpdateQueries) {
        return false; // pendings
    };
    //
    bool changed_events = false;
    STRING_SAFER_QUERY64 query_buffer;
    SlotUpdateNode* update_nodes[2] = {
        loaded_skill ? skill_table_.records_ : NULL,
        loaded_quick ? quick_table_.records_ : NULL
    };
    SlotUpdateNode* update_nodes_end[2] = {
        loaded_skill ? &skill_table_.records_[_countof(skill_table_.records_)] : NULL,
        loaded_quick ? &quick_table_.records_[_countof(quick_table_.records_)] : NULL
    };
    //
    for (int update_node_index = 0; update_node_index < _countof(update_nodes); ++update_node_index)
    {
        SlotUpdateNode* update_node_it = update_nodes[update_node_index],
                      * const update_node_end = update_nodes_end[update_node_index];
        if (update_node_it == NULL) {
            continue; //not loaded
        };
        for ( ; update_node_it != update_node_end; ++update_node_it)
        {
            if (update_node_it->query_ != NULL) {
                changed_events = true;
                continue; //in transaction
            }
            update_node_it->UpdateAndCompareData();
            if (update_node_it->record_changed_ == update_node_it->eRecordChange_Default) {
                continue;
            }
            //
            switch (update_node_it->record_changed_)
            {
            case update_node_it->eRecordChange_Changed: // update
                {
                    query_buffer.MakeString(_T("{?=call S_SlotInfo_Update (?,?,?,?,?,?,?,?,?,?,?)}"));
                    Query_SlotInfo_Update* const query = Query_SlotInfo_Update::ALLOC();
                    query->SetQuery(query_buffer);
                    query->SetUserKey(user_guid);
                    query->set_char_guid(char_guid);
                    // setup parameters
                    Query_SlotInfo_Update::sQueryParam* parameters = &query->parameters_[0];
                    static_cast<SlotResultRecord&>(*parameters) = update_node_it->latest_data_;
                    parameters->char_guid_ = char_guid;
                    //
                    update_node_it->query_ = query;
                };
                break;
            case update_node_it->eRecordChange_NewInsert: // insert
                {
                    query_buffer.MakeString(_T("{?=call S_SlotInfo_Insert (?,?,?,?,?,?,?,?,?,?,?)}"));
                    Query_SlotInfo_Insert* const query = Query_SlotInfo_Insert::ALLOC();
                    query->SetQuery(query_buffer);
                    query->SetUserKey(user_guid);
                    query->set_char_guid(char_guid);
                    // setup parameters
                    Query_SlotInfo_Insert::sQueryParam* parameters = &query->parameters_[0];
                    static_cast<SlotResultRecord&>(*parameters) = update_node_it->latest_data_;
                    parameters->char_guid_ = char_guid;
                    //
                    update_node_it->query_ = query;
                    //
                };
                break;
            }
            if (changed_events == false && update_node_it->query_) {
                changed_events = true;
            }
            if (update_node_it->query_)
            {
                db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_SLOTINFO_DBR,
                                 update_node_it->query_);
            }
        };
    }
    if (changed_events) {
        return false;
    }

    return true;
}

//==================================================================================================

}; //end of namespace


