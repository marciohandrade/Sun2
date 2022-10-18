#include "stdafx.h"
#include "./LotteryInfoParser.h"

//==================================================================================================

#include "ItemInfoParser.h"

#ifdef _SERVER
    #include "../Server/Common/DropRatioInfoList.h"
#endif

//==================================================================================================
//==================================================================================================
//==================================================================================================
/*
LotteryIndexGroup::LotteryIndexGroup(SLOTCODE group_index, int count)
    : group_index_(group_index)
    , creatable_number_(static_cast<uint8_t>(count))
    , number_of_minislots_(0)
{
    assert(count > 0 && count <= MAX_PANDORA_SLOT_NUM);
    ZeroMemory(mini_slots_, sizeof(mini_slots_));
}
*/

bool LotteryIndexGroup::AddMiniSlotInfo(const MiniSlotInfo* mini_slot)
{
    if (FlowControl::FCAssert(number_of_minislots_ < _countof(mini_slots_)) == false) {
        return false;
    };
    assert(mini_slot);
    mini_slots_[number_of_minislots_++] = mini_slot;
    return true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

LotteryInfoParser* LotteryInfoParser::instance_static_ = NULL;

LotteryInfoParser* LotteryInfoParser::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(LotteryInfoParser** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        LotteryInfoParser** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    LotteryInfoParser* provider = new LotteryInfoParser;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

// old util::Singleton support interface
void LotteryInfoParser::DestroyInstance()
{
    SAFE_DELETE(instance_static_);
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

LotteryInfoParser::LotteryInfoParser()
    : lottery_group_array_(NULL)
    , mini_slot_info_array_(NULL)
{
}

LotteryInfoParser::~LotteryInfoParser()
{
    Unload();
}

void LotteryInfoParser::Release()
{
    Unload();
}

BOOL LotteryInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_LOTTO_INFO:
        return _Load(reloaded != false);
    }

    return false;
}

//void LotteryInfoParser::Init(char * pszPackFileName)
//{
//    SetPackFileName(pszPackFileName);
//}

void LotteryInfoParser::Unload()
{
    if (lottery_group_array_ == NULL) {
        return;
    };

    if (lottery_group_array_) {
        ::free(lottery_group_array_);
        lottery_group_array_ = NULL;
    };
    if (mini_slot_info_array_) {
        ::free(mini_slot_info_array_);
        mini_slot_info_array_ = NULL;
    };
    lottery_table_.clear();
}

//==================================================================================================

bool LotteryInfoParser::_Load(bool reloaded)
{
    __UNUSED(reloaded);
    const int max_rows = GetRowSize();
    if (FlowControl::FCAssert(max_rows != 0 && lottery_group_array_ == NULL) == false) {
        return false;
    };
    // prepare table
    ;{
        lottery_table_.Initialize(max_rows);
        //
        size_t size_of_lotteries = sizeof(*lottery_group_array_) * max_rows;
        lottery_group_array_ = reinterpret_cast<LotteryIndexGroup*>(::malloc(size_of_lotteries));
        ZeroMemory(lottery_group_array_, size_of_lotteries);
        //
        BOOST_STATIC_ASSERT(_countof(((LotteryIndexGroup*)NULL)->mini_slots_) ==
                            MAX_PANDORA_SLOT_NUM);
        size_t size_of_minislots = sizeof(*mini_slot_info_array_) * max_rows * 
                                   _countof(((LotteryIndexGroup*)NULL)->mini_slots_);
        mini_slot_info_array_ = reinterpret_cast<MiniSlotInfo*>(::malloc(size_of_minislots));
        ZeroMemory(mini_slot_info_array_, size_of_minislots);
    };
    //
    LotteryIndexGroup* group_it = lottery_group_array_;
    MiniSlotInfo* mini_slot_it = mini_slot_info_array_;
    //
    for (int row = 0; row < max_rows; ++row)
    {
        SLOTCODE lottery_index = 0;
        ;{
            ulong _lottery_index = GetDataDWORD("LotteryID", row);
            if (bool out_of_ranges = !(_lottery_index != 0 && _lottery_index < USHRT_MAX))
            {
                SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=index out of ranges, ")
                       _T("LotteryID must less than %u, but inputed %u"),
                       __FUNCTION__, USHRT_MAX, _lottery_index);
                continue;
            };

            lottery_index = static_cast<SLOTCODE>(_lottery_index);
        };
        if (const LotteryIndexGroup* found_it = lottery_table_.GetData(lottery_index))
        {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=LotteryID(%u) is already exist"),
                   __FUNCTION__, lottery_index);
            continue;
        };
        //
        group_it->group_index_ = lottery_index;
        group_it->number_of_minislots_; //
        //
        // NOTE: f110610.2L, don't read "ItemNum".
        //
        static const char* column_random_flags[MAX_PANDORA_SLOT_NUM] = {
            "Random1", "Random2", "Random3", "Random4",
            "Random5", "Random6", "Random7", "Random8",
        };
        static const char* column_item_indexes[MAX_PANDORA_SLOT_NUM] = {
            "ItemCode1", "ItemCode2", "ItemCode3", "ItemCode4",
            "ItemCode5", "ItemCode6", "ItemCode7", "ItemCode8",
        };
        static const char* column_item_type_indices[MAX_PANDORA_SLOT_NUM] = {
            "IGType1", "IGType2", "IGType3", "IGType4",
            "IGType5", "IGType6", "IGType7", "IGType8",
        };
        //
        ItemInfoParser* const item_parser = ItemInfoParser::Instance();
    #ifdef _SERVER
        DropRatioInfoList* const drop_list = DropRatioInfoList::Instance();
    #endif
        for (int index = 0; index < MAX_PANDORA_SLOT_NUM; ++index)
        {
            mini_slot_it->Random = (GetDataBYTE(column_random_flags[index], row) != 0);
            mini_slot_it->Index = GetDataDWORD(column_item_indexes[index], row);
            mini_slot_it->ItemTypeIndex = GetDataDWORD(column_item_type_indices[index], row);
            if (mini_slot_it->Random == 0 &&
                mini_slot_it->Index == 0 &&
                mini_slot_it->ItemTypeIndex == 0)
            {
                // CHANGES: f110610.2L, empty, changes to enable countinual tab configuring.
                continue;
            };
            // NOTE: if 'Random' is true, 'ItemCode#n' field is used by the item type drop index.
            // otherwise, it is used by the item id.
            if (mini_slot_it->Random)
            {
            #ifdef _SERVER
                if (drop_list->PopRandomItemTypeIndex(mini_slot_it->Index) == NULL)
                {
                    SUNLOG(eCRITICAL_LOG,
                        _T("|[%s]|Msg=can't find drop index = %u ")
                        _T("in '%s' column of the LotteryID(%u)"),
                        __FUNCTION__, mini_slot_it->Index, column_item_indexes[index],
                        group_it->group_index_);
                    continue;
                };
            #endif
            }
            else
            {
                const BASE_ITEMINFO* item_info = item_parser->GetItemInfo(mini_slot_it->Index);
                if (item_info == NULL)
                {
                    SUNLOG(eCRITICAL_LOG,
                        _T("|[%s]|Msg=can't find item code = %u ")
                        _T("in '%s' column of the LotteryID(%u)"),
                        __FUNCTION__, mini_slot_it->Index, column_item_indexes[index],
                        group_it->group_index_);
                    continue;
                }
            };
            //
            
            // Drop이면 TypeIndex는 0 이어야 한다.
            if (mini_slot_it->Random &&
                mini_slot_it->ItemTypeIndex != 0)
            {
                SUNLOG(eCRITICAL_LOG,
                    _T("|[%s]|Msg=invalid ItemTypeIndex = %u")
                    _T("in '%s' column of the LotteryID(%u)"),
                    __FUNCTION__, mini_slot_it->ItemTypeIndex, column_item_type_indices[index],
                    group_it->group_index_);
                continue;
            };
            //
            if (ValidCheck(*group_it, mini_slot_it) == false) {
                continue;
            };
            //
            group_it->AddMiniSlotInfo(mini_slot_it);
            // select next 'mini_slot_it'
            ++mini_slot_it;
        }; //end 'foreach record in table'
        // select next 'group_it'
        ASSERT(group_it->number_of_minislots_);
        lottery_table_.insert(group_it->group_index_, group_it);
        ++group_it;
    };
    WAVERIX_WRAPPER((WAVERIX_LC_CALL0("waverix_log_lottery")));
    return true;
}

//==================================================================================================

bool LotteryInfoParser::ValidCheck(const LotteryIndexGroup& lottery_info,
                                   MiniSlotInfo* const mini_slot_info)
{
    if (mini_slot_info->Index == 0)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|invalid minislot.index=%d in lottery(%d)|"),
               mini_slot_info->Index, lottery_info.group_index_);
        return false;
    }
#if SUN_CODE_BACKUP
    // NOTE: f110610.2L, if this slot is pet item, this has not equipments and not overlapped item.
    // ItemTypeIndex can be zero.
    if (mini_slot_info->Random == false)
    {
        // assertion(mini_slot_info->Index)
        if (mini_slot_info->ItemTypeIndex == 0) {
            return false;
        };
    }
#endif
    return true;
}

