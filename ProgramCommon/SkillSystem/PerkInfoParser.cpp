#include "stdafx.h"
#include "./PerkInfoParser.h"

#include "Attribute.h"
#include "ItemAttrCalculator.h"

//==================================================================================================

const PerkInfo* PerkGroup::GetPerkInfoByExp(EXPTYPE exp) const
{
    if (item_code_ != 0) { // cash perk (has only 1 level)
        return perk_info_array_[0];
    }
    const PerkInfo* const * perk_info_it = perk_info_array_;
    const PerkInfo* const * perk_info_end = perk_info_array_ + number_of_perk_infos_;
    for ( ; perk_info_it != perk_info_end; ++perk_info_it)
    {
        const PerkInfo* perk_info = *perk_info_it;
        assert(perk_info);
        if (exp >= perk_info->exp_value_ &&
            exp <  perk_info->next_level_exp_)
        {
            return perk_info;
        }
    }
    assert(!"unexpected exp value or corrupted perk group data");
    return NULL;
}

//==================================================================================================

PerkInfoParser* PerkInfoParser::perk_info_parser_static_ = NULL;

PerkInfoParser* PerkInfoParser::CreateInstance()
{
    // (NOTE) thread-unsafe singleton
    perk_info_parser_static_ = new PerkInfoParser;
    return perk_info_parser_static_;
}

void PerkInfoParser::DestroyInstance()
{
    SAFE_DELETE(perk_info_parser_static_);
}

//==================================================================================================

PerkInfoParser::PerkInfoParser()
    : perk_info_array_(NULL)
{
}

PerkInfoParser::~PerkInfoParser()
{
}

//==================================================================================================

// IParser interface
void PerkInfoParser::Release()
{
    Unload();
}

// IParser interface
BOOL PerkInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    __UNUSED(reloaded);
    switch (script_code)
    {
    case SCRIPT_PERKINFO:
        return _Load();
    }
    return false;
}

//==================================================================================================

void PerkInfoParser::Init(char* pack_file_name)
{
    SetPackFileName(pack_file_name);
}

void PerkInfoParser::Unload()
{
    if (perk_info_array_)
    {
        ::free(perk_info_array_);
        perk_info_array_ = NULL;
        perk_table_.clear();
    }
    group_table_.clear();
}

//==================================================================================================

bool PerkInfoParser::_BuildReference(int max_number_of_perk_infos)
{
    if (max_number_of_perk_infos < 0 || max_number_of_perk_infos >= USHRT_MAX) {
        return false;
    }
    //
    if (perk_info_array_) {
        ::free(perk_info_array_);
    }
    ;{  // perk info
        const size_t kMaxArraySize = sizeof(*perk_info_array_) * max_number_of_perk_infos;
        void* ptr = ::malloc(kMaxArraySize);
        ZeroMemory(ptr, kMaxArraySize);
        perk_info_array_ = reinterpret_cast<PerkInfo*>(ptr);
    };
    perk_table_.clear();
    // perk group
    group_table_.clear();
    return true;
}

namespace nsSkill {
;

struct NameTable {
    struct NameNode {
        char name[MAX_FIELD_NAME_SIZE];
        char type_name[MAX_FIELD_NAME_SIZE];
        char value_name[MAX_FIELD_NAME_SIZE];
    };
    NameTable(const char* prefix, const int number_of_array)
        : number_of_array_(number_of_array)
    {
        ZeroMemory(name_table, sizeof(name_table));
        for (int i = 0; i < number_of_array; ++i)
        {
            _snprintf(name_table[i].name, _countof(name_table[i].name),
                "%s%d", prefix, i + 1);
            name_table[i].name[_countof(name_table[i].name) - 1] = '\0';
            //
            _snprintf(name_table[i].type_name, _countof(name_table[i].type_name),
                "%sType%d", prefix, i + 1);
            name_table[i].type_name[_countof(name_table[i].type_name) - 1] = '\0';
            //
            _snprintf(name_table[i].value_name, _countof(name_table[i].value_name),
                "%sValue%d", prefix, i + 1);
            name_table[i].value_name[_countof(name_table[i].value_name) - 1] = '\0';
        }
    }
    //
    const int number_of_array_;
    NameNode name_table[\
        max(PerkInfo::kNumberOfBenefitOptions, PerkInfo::kNumberOfPenaltyOptions)];
    //
    __DISABLE_COPY(NameTable);
};

}; //end of namespace

bool PerkInfoParser::_Load()
{
    //------------------------------------------------------------------------------
    // build index table
    using nsSkill::NameTable;
    const NameTable effect_name("Option", PerkInfo::kNumberOfBenefitOptions),
                    penalty_2x("DoublePenalty", PerkInfo::kNumberOfPenaltyOptions),
                    penalty_3x("TriplePenalty", PerkInfo::kNumberOfPenaltyOptions),
                    penalty_4x("QuadruplePenalty", PerkInfo::kNumberOfPenaltyOptions),
                    * name_table_ref[4] = { &effect_name, &penalty_2x, &penalty_3x, &penalty_4x };
    //------------------------------------------------------------------------------
    PerkInfo perk_info;
    PerkInfo::OptionNode* value_table_bind[4] = {
        perk_info.benefit_option_, perk_info.penalty_2x_option_,
        perk_info.penalty_3x_option_, perk_info.penalty_4x_option_
    };
    SORTED_GROUP sorted_group;
    //------------------------------------------------------------------------------
    const int max_rows = this->GetRowSize();
    const bool perk_records_index_out_of_range = _BuildReference(max_rows);
    if (FlowControl::FCAssert(perk_records_index_out_of_range) == false) {
        return false;
    };
    //
    PerkInfo* it_for_array = perk_info_array_;
    for (int row = 0; row < max_rows; ++row)
    {
        ZeroMemory(&perk_info, sizeof(perk_info));
        const PerkID& perk_id = perk_info.perk_id_;
        ;{  // make uid
            ulong group = GetDataDWORD("Group", row);
            ulong level = GetDataDWORD("Level", row);
            FASSERT(group < USHRT_MAX &&
                    (level != 0 && level <= PerkGroup::kMaxNumberOfPerkInfos));
            perk_info.perk_id_.group_ = static_cast<SLOTCODE>(group);
            perk_info.perk_id_.level_ = static_cast<uint8_t>(level);
        };
        ;{
            PerkTable::const_iterator found = perk_table_.find(perk_id.uid_);
            if (found != perk_table_.end()) {
                SUNLOG(eCRITICAL_LOG, _T("|%s|Msg=duplicated perk group(%d) & level(%d)|"),
                       perk_info.perk_id_.group_, perk_info.perk_id_.level_);
                __debugbreak();
                return false;
            }
        };
        perk_info.item_code_ = GetDataWORD("ItemCode", row);
        perk_info.name_code_ = GetDataDWORD("NameCode", row);
        perk_info.icon_code_ = GetDataDWORD("IconCode", row);        
        perk_info.option_description_code_ = GetDataDWORD("OptionDescriptionCode", row);
        perk_info.penalty_description_code_ = GetDataDWORD("PenaltyDescriptionCode", row);
        perk_info.exp_value_ = GetDataInt64("ExpValue", row);
        // read benefit & penalty options
        for (int ref_index = 0; ref_index < _countof(name_table_ref); ++ref_index)
        {
            const NameTable* name_table = name_table_ref[ref_index];
            const int number_of_indexes = name_table->number_of_array_;
            for (int index = 0; index < number_of_indexes; ++index)
            {
                PerkInfo::OptionNode* data = &value_table_bind[ref_index][index];
                const NameTable::NameNode& name_node = name_table->name_table[index];
                data->item_option_ = GetDataBYTE(name_node.name, row);
                if (data->item_option_ == 0) {
                    continue;
                }
                data->value_type_ = GetDataBYTE(name_node.type_name, row);
                data->value_ = GetDataInt(name_node.value_name, row);
                // (f100812.2L) pre-calculated
            #ifdef _NA000000_VALUE_TYPE_SIMPLIFICATION
                const ulong kAcceptValueTypeBits = (1 << VALUE_TYPE_VALUE) |
                                                   (1 << VALUE_TYPE_PERCENT_PER_CUR) |
                                                   (1 << VALUE_TYPE_PERCENT_PER_MAX);
            #else
                const ulong kAcceptValueTypeBits = (1 << VALUE_TYPE_VALUE) |
                                                   (1 << VALUE_TYPE_PERCENT_PER_CUR) |
                                                   (1 << VALUE_TYPE_PERCENT_PER_MAX) |
                                                   (1 << VALUE_TYPE_RATIO_VALUE);
            #endif
                if (data->item_option_ >= _countof(g_ITEM_OPTION) ||
                    (kAcceptValueTypeBits & (1 << data->value_type_)) == 0)
                {
                    SUNLOG(eCRITICAL_LOG,
                           _T("|[%s]|violate a rule of an option value, change to an empty section|")
                           _T("Group=%d|Level=%d|Name=%s|Option=%d|ValueType=%d|Value=%d|"),
                           __FUNCTION__, perk_id.group_, perk_id.level_,
                           name_node.name, data->item_option_, data->value_type_, data->value_);
                    ZeroMemory(data, sizeof(*data));
                    continue;
                }
                data->option_ = static_cast<uint16_t>(g_ITEM_OPTION[data->item_option_]);
                uint8_t attr_kind = eATTR_KIND_ITEM;
                ItemAttrCalculator::ModifyAttrKindAndCalculatedValue(\
                    data->value_type_, &attr_kind, &data->value_);
                // change to skill attribute
                attr_kind = attr_kind == eATTR_KIND_ITEM ?       eATTR_KIND_SKILL
                          : attr_kind == eATTR_KIND_ITEM_RATIO ? eATTR_KIND_SKILL_RATIO
                          :                                      attr_kind;
                // skill value = 100.0, item value = 100
                data->attr_kind_ = attr_kind;
                //
            }
        }
        //
        ;{
            PerkInfo* perk_info_data = it_for_array++;
            *perk_info_data = perk_info;
            perk_table_.insert(std::make_pair(perk_id.uid_, perk_info_data));
            sorted_group.insert(std::make_pair(perk_id.group_, 0)); // true, false...
            if (perk_info_data->item_code_ != 0)
            {
                cash_perk_table_.insert(std::make_pair(perk_info_data->item_code_, perk_info_data));
            }
        };
    } //end 'for'

    _BuildGroupInfo(sorted_group);
    sorted_group.clear();
    //
    return true;
}

bool PerkInfoParser::_BuildGroupInfo(SORTED_GROUP& sorted_group)
{
    ;{  // renumbering
        int order = 0;
        FOREACH_CONTAINER(SORTED_GROUP::value_type& node, sorted_group, SORTED_GROUP) {
            node.second = order++;
        }
    };
    //
    FOREACH_CONTAINER(const PerkTable::value_type& perk_node, perk_table_, PerkTable)
    {
        const PerkInfo* perk_info = perk_node.second;
        const PerkID& perk_id = perk_info->perk_id_;
        GroupTable::iterator it = group_table_.find(perk_id.group_);
        const bool exist = (it != group_table_.end());
        PerkGroup* const perk_group = exist
                                    ?   const_cast<PerkGroup*>(&it->second)
                                    :   &group_table_[perk_id.group_];
        if (exist == false) {
            ZeroMemory(perk_group, sizeof(*perk_group));
        }
        ;{
            perk_group->group_code_ = perk_id.group_;
            assert(perk_group->number_of_perk_infos_ <= perk_group->kMaxNumberOfPerkInfos);
            perk_group->perk_info_array_[perk_id.level_ - 1] = perk_info;
            ++perk_group->number_of_perk_infos_;
        };
        if (exist == false)
        {
            int order = sorted_group.find(perk_id.group_)->second;
            perk_group->order_ = static_cast<uint8_t>(order);
            perk_group->item_code_ = perk_info->item_code_;
            if (perk_group->item_code_ && perk_id.level_ != 1)
            {
                SUNLOG(eCRITICAL_LOG,
                       _T("|[%s]|Msg=the perk of (Group=%d, Level=%d) is violated a perk rule. ")
                       _T("a cash perk using an item should be have '1' level|"),
                       __FUNCTION__, perk_id.group_, perk_id.level_);
                __debugbreak();
            }
            //group_table_.insert(std::make_pair(perk_id.group_, perk_group));
        }
        else
        {
            // WARNING, f100823.5L, each of perk item types should be have one group code.
            bool fail_cash_type_rule = false;
            const TCHAR* error_msg = NULL;
            if (perk_group->item_code_)
            {
                fail_cash_type_rule = true;
                error_msg = _T("each of a cash perk types, using an item, ")
                            _T("should be have one group code.");
            }
            else
            {
                if (perk_info->item_code_ != 0) {
                    error_msg = _T("you need check the field of 'ItemCode'");
                    fail_cash_type_rule = true;
                }
            };
            if (fail_cash_type_rule)
            {
                SUNLOG(eCRITICAL_LOG,
                       _T("|[%s]|Msg=%s|Group=%d, Level=%d, a ItemCode of group=%d, ")
                       _T("the field of 'ItemCode=%d' is against a cash perk rule|"),
                       __FUNCTION__, error_msg, perk_id.group_, perk_id.level_,
                       perk_group->item_code_, perk_info->item_code_);
                __debugbreak();
                return false;
            };
        }
    };
    //
    FOREACH_CONTAINER(GroupTable::value_type& node, group_table_, GroupTable)
    {   // numbering
        PerkGroup* const group = &node.second;
        const int upperbound = group->number_of_perk_infos_;
        const uint8_t order = static_cast<uint8_t>(group->order_);
        if (upperbound)
        {
            PerkInfo* perk_info_edit = \
                const_cast<PerkInfo*>(group->perk_info_array_[upperbound - 1]);
            perk_info_edit->order_ = order;
            perk_info_edit->max_level_ = true;
            perk_info_edit->next_level_exp_ = 0x7FFFFFFFFFFFFFFFLL;
        }
        PerkInfo* prev_perk_info = NULL;
        //            signed comparison, ranges=[-1, x]
        for (int i = 0; i < (upperbound - 1); ++i)
        {
            PerkInfo* perk_info_edit = const_cast<PerkInfo*>(group->perk_info_array_[i]);

            perk_info_edit->order_ = order;
            perk_info_edit->next_level_exp_ = group->perk_info_array_[i + 1]->exp_value_;
            //
            perk_info_edit->prev_perk_ = prev_perk_info;
            if (prev_perk_info) {
                prev_perk_info->next_perk_ = perk_info_edit;
            }
            prev_perk_info = perk_info_edit;
        }
        if (upperbound)
        {
            PerkInfo* perk_info_edit = \
                const_cast<PerkInfo*>(group->perk_info_array_[upperbound - 1]);
            //
            perk_info_edit->prev_perk_ = prev_perk_info;
            if (prev_perk_info) {
                prev_perk_info->next_perk_ = perk_info_edit;
            }
            prev_perk_info = perk_info_edit;
        }
    };

    return true;
}

//==================================================================================================

