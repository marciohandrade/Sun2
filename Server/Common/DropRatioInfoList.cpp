#include "stdafx.h"
#include "./DropRatioInfoList.h"
#include <ItemInfoParser.h>
#include <LimitedItemInfoParser.h>

//==================================================================================================

DropRatioInfoList* DropRatioInfoList::drop_ratio_info_list_static_ = NULL;

DropRatioInfoList* DropRatioInfoList::CreateInstance()
{
    // (NOTE) thread-unsafe singleton
    drop_ratio_info_list_static_ = new DropRatioInfoList;
    return drop_ratio_info_list_static_;
}

void DropRatioInfoList::DestroyInstance()
{
    SAFE_DELETE(drop_ratio_info_list_static_);
}

COMPILE_ASSERTION_CHECK_METHOD_IMPL()
{
    DropItemObj item_obj;
    DropMoneyObj money_obj;
    const size_t kInterMaxSharedDropObjectSize = max(sizeof(item_obj), sizeof(money_obj));
    BOOST_STATIC_ASSERT(sizeof(item_obj) == sizeof(money_obj) &&
                        (kInterMaxSharedDropObjectSize ==
                         DropRatioInfoList::kMaxSharedDropObjectSize));

}
COMPILE_ASSERTION_CHECK_METHOD_END();

//==================================================================================================

DropRatioInfoList::DropRatioInfoList(void)
    : randomizer_array_(NULL),
      drop_object_free_chunk_(NULL)
{
    util::LList::Init(&drop_object_alloc_root_);
}

DropRatioInfoList::~DropRatioInfoList(void)
{
    Unload();
}

void
DropRatioInfoList::Release()
{
    Unload();
}

void
DropRatioInfoList::Reload()
{
}

BOOL
DropRatioInfoList::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch(script_code)
    {
    case SCRIPT_DROPRATIOINFO:
        return _Load(reloaded);
    }

    return false;
}


void
DropRatioInfoList::Unload()
{
    if (randomizer_array_ == NULL) {
        return;
    }
    SAFE_DELETE(drop_object_free_chunk_);
    while (util::LList::IsEmpty(&drop_object_alloc_root_) == false)
    {
        DropObjPoolChunk* it = drop_object_alloc_root_.next;
        util::LList::Delete(it);
        delete it;
    };
    ::free(randomizer_array_);
    randomizer_array_ = NULL;
    random_drop_type_selector_.clear();
}

// (f100612.3L) use only in loading
DropObj* DropRatioInfoList::AllocDropObject(eDROP_OBJ_TYPES drop_obj_type)
{
    if (drop_object_free_chunk_ == NULL) {
        drop_object_free_chunk_ = new DropObjPoolChunk;
    }
    if (drop_object_free_chunk_->number_of_allocations_ >=
        _countof(drop_object_free_chunk_->array_))
    {
        util::LList::AddPrev(drop_object_free_chunk_, &drop_object_alloc_root_);
        drop_object_free_chunk_ = new DropObjPoolChunk;
    }
    //
    DropObjPoolChunk::node_type* const node = \
        &drop_object_free_chunk_->array_[drop_object_free_chunk_->number_of_allocations_];
    ++drop_object_free_chunk_->number_of_allocations_;
    if (drop_obj_type == DROP_OBJ_ITEM)
    {
        return new (node) DropItemObj;
    }
    else if (drop_obj_type == DROP_OBJ_MONEY)
    {
        return new (node) DropMoneyObj;
    }
    ASSERT(!"|request invalid drop object type|");
    return NULL;
}

// (f100612.3L)
bool DropRatioInfoList::_BuildReference(int added_number_of_nodes)
{
    if (added_number_of_nodes <= 0) {
        return false;
    }
    if (randomizer_array_) {
        ::free(randomizer_array_);
    }
    const size_t randomizer_array_size = sizeof(randomizer_array_[0]) * added_number_of_nodes;
    randomizer_array_ = reinterpret_cast<RANDOMIZER*>(::malloc(randomizer_array_size));
    ZeroMemory(randomizer_array_, randomizer_array_size);
    ;{
        RANDOMIZER* it = randomizer_array_;
        RANDOMIZER* const end = randomizer_array_ + added_number_of_nodes;
        for (; it < end; ++it) {
            new (it) RANDOMIZER;
        }
    };
    return true;
}

BOOL
DropRatioInfoList::_Load(BOOL reloaded)
{
    __UNUSED(reloaded);
    //char description_for_debug[512];
    static int index_for_debug = 0;
    static int last_index_for_debug = 0;
    //
    BOOST_STATIC_ASSERT(kNumberOfMoneyColumns == 5);
    static const char* kMoneyColumns[kNumberOfMoneyColumns] = {
        "Money1", "Money2", "Money3", "Money4", "Money5"
    };
    BOOST_STATIC_ASSERT(kNumberOfItemProbabilityColumns == 30);
    static const char* kGroupColumns[kNumberOfItemProbabilityColumns] = {
        "bG1",  "bG2",  "bG3",  "bG4",  "bG5",  "bG6",  "bG7",  "bG8",  "bG9",  "bG10", 
        "bG11", "bG12", "bG13", "bG14", "bG15", "bG16", "bG17", "bG18", "bG19", "bG20", 
        "bG21", "bG22", "bG23", "bG24", "bG25", "bG26", "bG27", "bG28", "bG29", "bG30", 
    };
    static const char* kItemCodeColumns[kNumberOfItemProbabilityColumns] = {
        "IC1",  "IC2",  "IC3",  "IC4",  "IC5",  "IC6",  "IC7",  "IC8",  "IC9",  "IC10", 
        "IC11", "IC12", "IC13", "IC14", "IC15", "IC16", "IC17", "IC18", "IC19", "IC20", 
        "IC21", "IC22", "IC23", "IC24", "IC25", "IC26", "IC27", "IC28", "IC29", "IC30", 
    };
    static const char* kLimitedIndexColumns[kNumberOfItemProbabilityColumns] = {
        "LM1",  "LM2",  "LM3",  "LM4",  "LM5",  "LM6",  "LM7",  "LM8",  "LM9",  "LM10", 
        "LM11", "LM12", "LM13", "LM14", "LM15", "LM16", "LM17", "LM18", "LM19", "LM20", 
        "LM21", "LM22", "LM23", "LM24", "LM25", "LM26", "LM27", "LM28", "LM29", "LM30", 
    };
    static const char* kProbabilityColumns[kNumberOfItemProbabilityColumns] = {
        "IP1",  "IP2",  "IP3",  "IP4",  "IP5",  "IP6",  "IP7",  "IP8",  "IP9",  "IP10", 
        "IP11", "IP12", "IP13", "IP14", "IP15", "IP16", "IP17", "IP18", "IP19", "IP20", 
        "IP21", "IP22", "IP23", "IP24", "IP25", "IP26", "IP27", "IP28", "IP29", "IP30", 
    };
    static const char* kReservedColumns[kNumberOfItemProbabilityColumns] = {
        "TV1",  "TV2",  "TV3",  "TV4",  "TV5",  "TV6",  "TV7",  "TV8",  "TV9",  "TV10", 
        "TV11", "TV12", "TV13", "TV14", "TV15", "TV16", "TV17", "TV18", "TV19", "TV20", 
        "TV21", "TV22", "TV23", "TV24", "TV25", "TV26", "TV27", "TV28", "TV29", "TV30", 
    };
    //
    ItemInfoParser* const item_parser = ItemInfoParser::Instance();
    LimitedItemInfoParser* const limited_parser = LimitedItemInfoParser::Instance();
    //
    const int max_rows = GetRowSize();
    if (_BuildReference(max_rows) == false) {
        return false;
    }
    RANDOMIZER* randomizer_it = randomizer_array_;
    //
    for (int row = 0; row < max_rows; ++row)
    {
        const int drop_index = GetDataInt("Index", row);
        index_for_debug = drop_index;
        //strncpy(description_for_debug, GetDataString("Description", row), 512);

        const int standard_value = GetDataInt("P_Amount", row);
        if (standard_value <= 0)
        {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|Index=%d|Msg=Invalid standard_value(%d)|"),
                   __FUNCTION__, drop_index, standard_value);
            continue;
        }
        const bool desired_unique_drop_index_check_passed = \
            random_drop_type_selector_.find(drop_index) == random_drop_type_selector_.end();
        if (!FlowControl::FCAssert(desired_unique_drop_index_check_passed))
        {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|Index=%d|Msg=drop index duplicated|"),
                   __FUNCTION__, drop_index);
            continue;
        }
        //
        RANDOMIZER* const randomizer = randomizer_it++;
        random_drop_type_selector_[drop_index] = randomizer;

        const int min_money_seed = GetDataInt("DropMoney_Min", row);
        const int max_money_seed = GetDataInt("DropMoney_Max", row);

        // µ∑ »Æ∑¸ √≥∏Æ
        BOOST_STATIC_ASSERT(kNumberOfMoneyColumns == 5);
        for (int i = 0; i < kNumberOfMoneyColumns; ++i)
        {
            const int money_ratio = GetDataInt(kMoneyColumns[i], row);
            if (money_ratio == 0) {
                continue;
            }
            DropMoneyObj* const drop_money_obj = \
                static_cast<DropMoneyObj*>(this->AllocDropObject(DROP_OBJ_MONEY));
            //drop_list_hash_[drop_index].push_back(drop_money_obj);
            //
            DropObj::DropMoneyInfo* const drop_money_info = drop_money_obj->GetInfo();
            drop_money_info->min_money_seed = min_money_seed;
            drop_money_info->max_money_seed = max_money_seed;
            switch (i + 1)
            {
            case 1:
                drop_money_info->min_multiple = 1;
                drop_money_info->max_multiple = 1;
                break;
            case 2:
                drop_money_info->min_multiple = 450;
                drop_money_info->max_multiple = 550;
                break;
            case 3:
                drop_money_info->min_multiple = 4500;
                drop_money_info->max_multiple = 5500;
                break;
            case 4:
                drop_money_info->min_multiple = 100000;
                drop_money_info->max_multiple = 200000;
                break;
            case 5:
                drop_money_info->min_multiple = 1000000;
                drop_money_info->max_multiple = 2000000;
                break;
            }

            const bool desired_randomizer_add_ratio_success = \
                randomizer->AddRatio(drop_money_obj, money_ratio, standard_value) != false;
            if (!FlowControl::FCAssert(desired_randomizer_add_ratio_success))
            {
                SUNLOG(eCRITICAL_LOG,
                       _T("|[%s]|Index=%d|Msg=invalid money drop ratio|Action=passed|"),
                       __FUNCTION__, drop_index);
            }
        }
        BOOST_STATIC_ASSERT(kNumberOfItemProbabilityColumns == 30);
        // referenced column names
        //kGroupColumns[kNumberOfItemProbabilityColumns];
        //kItemCodeColumns[kNumberOfItemProbabilityColumns];
        //kLimitedIndexColumns[kNumberOfItemProbabilityColumns];
        //kProbabilityColumns[kNumberOfItemProbabilityColumns];
        //kReservedColumns[kNumberOfItemProbabilityColumns];
        // æ∆¿Ã≈€ »Æ∑¸ √≥∏Æ
        for (int i = 0; i < kNumberOfItemProbabilityColumns; ++i)
        {
            const DWORD item_code = GetDataInt(kItemCodeColumns[i], row);
            if (item_code == 0) {
                continue;
            }
            const DWORD probability_ratio = GetDataDWORD(kProbabilityColumns[i], row);
            if (probability_ratio == 0) {
                continue;
            }
            BYTE group = GetDataBYTE(kGroupColumns[i], row);
            const WORD limited_item_index = GetDataWORD(kLimitedIndexColumns[i], row);
            if (limited_item_index != 0)
            {
                const sLIMITED_ITEM_INFO* limited_item_info = \
                    limited_parser->GetLimitedItemInfo(limited_item_index);
                if (limited_item_info == NULL)
                {
                    SUNLOG(eCRITICAL_LOG,
                           _T("|[%s]|Index=%d|Msg=can't find a limited item index(%d) of the '%s'|"),
                           __FUNCTION__, drop_index, limited_item_index, kLimitedIndexColumns[i]);
                    continue;
                }
            }
            //--------------------------------------------------------------------
            // control flow diagram
            //
            // group  |  item  |  Condition
            //   0    |  NULL  |  FAIL
            //   0    |  !NULL |  IsOverlap  | Reserved
            //                      false    | use 'number'
            //                      true     | ItemTypeIndex in ItemTypeList.txt
            //   1    |    X   |  next drop index
            //--------------------------------------------------------------------
            // changed logic to support _NA_0_20110318_ITEM_EXPIRATION_PERIOD
            switch (group)
            {
            case 0: group = DROP_GROUP::LINK_ITEM;
                break;
            case 1: group = DROP_GROUP::LINK_DROP;
                break;
            case 2: group = DROP_GROUP::LINK_ITEM | DROP_GROUP::LIMIT_TIME;
                break;
            case 3: group = DROP_GROUP::LINK_ITEM | DROP_GROUP::LIMIT_DATE;
                break;
            case 4: group = DROP_GROUP::LINK_ITEM
                          | DROP_GROUP::LIMIT_DISTRIBUTE //_NA002479_20110401_OVERLAPPING_ITEM_DISTRIBUTION_CHANGES
                          ;
                break;
            default:
                SUNLOG(eCRITICAL_LOG, _T("Index[%d] bG value failed![%d][must have 0~4]"),
                       drop_index, group);
                continue;
            }

            const BASE_ITEMINFO* base_item_info = NULL;
            DWORD item_reserved = 0;

            if ((group & DROP_GROUP::LINK_ITEM) == DROP_GROUP::LINK_ITEM)
            {
                item_reserved = GetDataDWORD(kReservedColumns[i], row);
                base_item_info = item_parser->GetItemInfo(item_code);

                if (base_item_info == NULL)
                {
                #if !defined(_JAPAN)
                    SUNLOG(eCRITICAL_LOG,
                        _T("|[%s]|Index=%d|Msg=can't find a item(%d) of the '%s' in an item script|"),
                        __FUNCTION__, drop_index, item_code, kItemCodeColumns[i]);
                #endif
                    continue;
                }

                const DURATYPE duplicable_number = base_item_info->m_byDupNum;
                if (duplicable_number > 1) // overlapped item
                {
                    // use 'number' by item_reserved
                    if (item_reserved == 0 ||
                        ((group & DROP_GROUP::LIMIT_DISTRIBUTE) == 0 &&
                         item_reserved > duplicable_number))
                    {
                        SUNLOG(eCRITICAL_LOG,
                            _T("|[%s]|Index=%d|Msg=a (%d) value of the '%s' is violated ")
                            _T("that of a m_byDupNum(%d) value of item(%d).|Action=modified|"),
                            __FUNCTION__, drop_index,
                            item_reserved, kReservedColumns[i],
                            duplicable_number, item_code);
                        // modified by an item script information
                        item_reserved = duplicable_number;
                    }
                }
                else
                {   // assert(duplicable_number == 1)
                    // use ItemTypeIndex in ItemTypeList.txt
                }
            }
            //
            DropItemObj* const drop_item_obj = \
                static_cast<DropItemObj*>(this->AllocDropObject(DROP_OBJ_ITEM));
            //drop_list_hash_[drop_index].push_back(drop_item_obj);
            DropObj::DropItemInfo* const drop_item_info = drop_item_obj->GetInfo();
            //declared by _NA_1419_20100121_ACCUMULATE_POINT
            drop_item_info->drop_item_order = static_cast<BYTE>(i);
            drop_item_info->group_code = group;
            drop_item_info->item_code = item_code;
            drop_item_info->limited_item_index = limited_item_index;
            drop_item_info->probability_ratio = probability_ratio;
            drop_item_info->reserved = item_reserved;

            const bool desired_randomizer_add_ratio_success = \
                randomizer->AddRatio(drop_item_obj, probability_ratio, standard_value) != false;
            if (desired_randomizer_add_ratio_success == false)
            {
                SUNLOG(eCRITICAL_LOG,
                       _T("|[%s]|Index=%d|Msg=invalid item drop ratio, ")
                       _T("a probability(%d) of the '%s' by standard(%d)|Action=passed|"),
                       __FUNCTION__, drop_index,
                       probability_ratio, kProbabilityColumns[i], standard_value);
            }
        }

        last_index_for_debug = drop_index;
    }

    return true;
}


// (CHANGES) (WAVERIX) (091127) move from header to code file, changes codes for debugging factility

DropObj*
DropRatioInfoList::PopRandomItemTypeIndex(DWORD first_drop_index) 
{
    const int MaxDepth = 6;
                // backup for debugging facility
    DWORD drop_index = first_drop_index;
    int depth = 0;
    DropObj* drop_object_array[MaxDepth] = { 0, };
    do
    {
        RANDOMIZER_HASH::const_iterator it = random_drop_type_selector_.find(drop_index);
        if (it == random_drop_type_selector_.end()) {
            return NULL;
        }
        RANDOMIZER* const randomizer = it->second;
        DropObj* drop_object = drop_object_array[depth] = randomizer->RandomizedKey();
        if (drop_object == NULL) {
            return NULL;
        }
        if (drop_object->kDropObjType == DROP_OBJ_ITEM) {
            DropItemObj* drop_item_obj = static_cast<DropItemObj*>(drop_object);
            // if group_code == 0 then drop item code
            // else               then drop index (need recursively call)
            if (DROP_GROUP::LINK_ITEM & drop_item_obj->GetGroupCode()) 
            {
                return drop_item_obj;
            }
            else 
            {
                drop_index = drop_item_obj->GetItemCode();
            }
        }
        else {
            return drop_object;
        }

        ++depth;
    }
    while (depth < MaxDepth);

    SUNLOG(eCRITICAL_LOG, "|Warning|Msg=occurred too many loop!|DropIndex=%d|", first_drop_index);
    return NULL;
}

UINT DropRatioInfoList::PopDropItemCodeArray(DWORD drop_index, SLOTCODE* OUT item_code_array, UINT item_code_array_count)
{
    if (random_drop_type_selector_.empty()) {
        return 0;
    }
    RANDOMIZER_HASH::const_iterator it_find = random_drop_type_selector_.find(drop_index);
    if (it_find == random_drop_type_selector_.end()) {
        return 0;
    }
    uint32_t result_of_count = 0;
    const RANDOMIZER* randomizer = it_find->second;
    const RANDOMIZER::KeyTypeArray& drop_obj_array = randomizer->GetKeyTypeArray();
    const RANDOMIZER::key_type* it = &drop_obj_array[0];
    const RANDOMIZER::key_type* const end = &drop_obj_array[_countof(drop_obj_array)];
    for (; it != end; ++it)
    {
        const DropObj* drop_obj = *it;
        if (drop_obj == NULL) {
            continue;
        }
        if (result_of_count >= item_code_array_count) {
            break;
        }
        if (drop_obj->kDropObjType == DROP_OBJ_ITEM)
        {
            const DropItemObj* drop_item_obj = static_cast<const DropItemObj*>(drop_obj);
            DWORD item_code = drop_item_obj->GetItemCode();
            assert(SAFE_NUMERIC_TYPECAST(DWORD, item_code, SLOTCODE));
            item_code_array[result_of_count] = static_cast<SLOTCODE>(item_code);
            ++result_of_count;
        }
    }
    return result_of_count;
}
