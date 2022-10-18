#include "stdafx.h"
#include "./ItemTypeDropRatio.h"

//==================================================================================================

// NOTE: f110401.1L
uint8_t ItemTypeDropRatio::kConvertNpcGradeToIndex[eNPC_GRADE_MAX] = { 0, };

//==================================================================================================

ItemTypeDropRatio::ItemTypeDropRatio(void)
    : selected_selector_(&item_type_selector_in_field_)
{
    // NOTE: f110401.1L
    MakeInitialConvertTable();
}

ItemTypeDropRatio::~ItemTypeDropRatio(void)
{
    Unload();
}

// UNUSED
void ItemTypeDropRatio::Unload()
{
    for (int i = 0; i < _countof(item_type_selector_in_field_); ++i) {
        item_type_selector_in_field_[i].Clear();
    }
    //_NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION
    for (int i = 0; i < _countof(item_type_selector_in_battle_); ++i) {
        item_type_selector_in_field_[i].Clear();
    }
}

// unused
void ItemTypeDropRatio::Reload()
{
}

void ItemTypeDropRatio::Release()
{
    Unload();
}

// NOTE: f110401.1L
void ItemTypeDropRatio::SetupDropRatioTable(bool selected_battlezone)
{
    selected_selector_ = selected_battlezone ? &item_type_selector_in_battle_
                                             : &item_type_selector_in_field_;
}

BOOL ItemTypeDropRatio::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch(script_code)
    {
    case SCRIPT_ITEMTYPEDROPRATIO:
        return _Load(reloaded != false);
    }

    return false;
}

// NOTE: f110401.1L
bool ItemTypeDropRatio::MakeInitialConvertTable()
{
    BOOST_STATIC_ASSERT(ItemTypeDropRatio::eIndex_None == 0);
    ZeroMemory(kConvertNpcGradeToIndex, sizeof(kConvertNpcGradeToIndex));

#define MAKE_LINK_INDEX_WITH_NPC_GRADE(_npc_grade_) \
    kConvertNpcGradeToIndex[_npc_grade_] = ItemTypeDropRatio::eIndex_##_npc_grade_
    ;{
        MAKE_LINK_INDEX_WITH_NPC_GRADE(eNPC_GENERAL);
        MAKE_LINK_INDEX_WITH_NPC_GRADE(eNPC_ELITE);
        MAKE_LINK_INDEX_WITH_NPC_GRADE(eNPC_LEADER);
        MAKE_LINK_INDEX_WITH_NPC_GRADE(eNPC_MIDDLEBOSS);
        MAKE_LINK_INDEX_WITH_NPC_GRADE(eNPC_BOSS);
        MAKE_LINK_INDEX_WITH_NPC_GRADE(eNPC_LUCKY_MONSTER);
    };
#undef MAKE_LINK_INDEX_WITH_NPC_GRADE
    //
    return true;
}

bool ItemTypeDropRatio::_Load(bool reloaded)
{
    __UNUSED(reloaded);

    //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    const int max_rows = GetRowSize();
    //_NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION
    BOOST_STATIC_ASSERT(MAX_ITEM_TYPE_INDEX == 90);
    FASSERT(initialized_tag_ != kInitializedTag &&
            max_rows == MAX_ITEM_TYPE_INDEX * TYPE_SELECTOR_NUM);

    for (int row = 0; row < max_rows; ++row)
    {
        const int index = GetDataInt("bIndex", row);
        const bool selected_battlezone = (GetDataBYTE("Type", row) != 0);
        ItemTypeSelectorList* const item_type_selector = \
            selected_battlezone ? &item_type_selector_in_battle_
                                : &item_type_selector_in_field_;
        const int standard_value = GetDataInt("dStandard", row);
        bool read_record_result = _LoadRecord(row, index, standard_value, *item_type_selector);
        ASSERT(read_record_result);
    };
    initialized_tag_ = kInitializedTag;
    //
    return true;
}

// NOTE: f110401.1L
bool ItemTypeDropRatio::_LoadRecord(
    const int row, const int index, const int standard_value,
    ItemTypeSelectorList& item_type_selector_list)
{
    struct ErrorLogger {
        void Logging(int index, int item_drop_rate, int standard_value) {
            SUNLOG(eCRITICAL_LOG,
                   _T("|[ItemTypeDropRatio::_Load]|")
                   _T("|Msg=invalid item_drop_ratio|Index=%d|item_drop_rate=%u|standard=%u|"),
                   index, item_drop_rate, standard_value);
        }
    } logger;

    int item_drop_rate = 0;
    BOOL result = false;
    // (NOTE) 6kinds / eNPC_GRADE_MAX(17) kinds
    if (ItemTypeSelector* selector = \
            &item_type_selector_list[kConvertNpcGradeToIndex[eNPC_GENERAL]])
    {  //eNPC_GENERAL
        item_drop_rate = GetDataInt("MGrade1", row);
        result = selector->AddRatio(index, item_drop_rate, standard_value);
        if (result == false) {
            logger.Logging(index, item_drop_rate, standard_value);
        }
    };
    if (ItemTypeSelector* selector = \
            &item_type_selector_list[kConvertNpcGradeToIndex[eNPC_ELITE]])
    {  //eNPC_ELITE
        item_drop_rate = GetDataInt("MGrade2", row);
        result = selector->AddRatio(index, item_drop_rate, standard_value);
        if (result == false) {
            logger.Logging(index, item_drop_rate, standard_value);
        }
    };
    if (ItemTypeSelector* selector = \
            &item_type_selector_list[kConvertNpcGradeToIndex[eNPC_LEADER]])
    {  //eNPC_LEADER
        item_drop_rate = GetDataInt("MGrade3", row);
        result = selector->AddRatio(index, item_drop_rate, standard_value);
        if (result == false) {
            logger.Logging(index, item_drop_rate, standard_value);
        }
    };
    if (ItemTypeSelector* selector = \
            &item_type_selector_list[kConvertNpcGradeToIndex[eNPC_MIDDLEBOSS]])
    {  //eNPC_MIDDLEBOSS
        item_drop_rate = GetDataInt("MGrade4", row);
        result = selector->AddRatio(index, item_drop_rate, standard_value);
        if (result == false) {
            logger.Logging(index, item_drop_rate, standard_value);
        }
    };
    if (ItemTypeSelector* selector = \
            &item_type_selector_list[kConvertNpcGradeToIndex[eNPC_BOSS]])
    {  //eNPC_BOSS
        item_drop_rate = GetDataInt("MGrade5", row);
        result = selector->AddRatio(index, item_drop_rate, standard_value);
        if (result == false) {
            logger.Logging(index, item_drop_rate, standard_value);
        }
    };
    if (ItemTypeSelector* selector = \
            &item_type_selector_list[kConvertNpcGradeToIndex[eNPC_LUCKY_MONSTER]])
    {  //eNPC_LUCKY_MONSTER
        item_drop_rate = GetDataInt("MGrade9", row);
        result = selector->AddRatio(index, item_drop_rate, standard_value);
        if (result == false) {
            logger.Logging(index, item_drop_rate, standard_value);
        }
    };
    //
    return true;
}

