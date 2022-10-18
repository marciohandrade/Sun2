////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   MaxCharListParser.cpp
/// @author hukim
/// @email  silped@gmail.com
/// @brief  [2011/05/04 by hukim] This is created.\n
///
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\MaxCharListParser.h" // header

#ifdef _USE_MAX_CHAR_LIST_PARSER

////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

MaxCharListParser::MaxCharListParser() : 
    default_slot_size_(MaxCharListConfig::kDefaultSlotSize), 
    max_slot_size_(MaxCharListConfig::kMaxSlotSize), 
    use_extension_((MaxCharListConfig::kUseExtension == 1)), 
    slot_size_per_extension_(MaxCharListConfig::kSlotSizePerExtension)
{
}

MaxCharListParser::~MaxCharListParser()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL MaxCharListParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    if (script_code == SCRIPT_MAX_CHAR_LIST)
    {
        if (Load(is_reload))
        {
            return TRUE;
        }
    }
    FASSERT(false && _T("스크립트 데이터에 오류가 있습니다."));
    return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Private
////////////////////////////////////////////////////////////////////////////////////////////////////

bool MaxCharListParser::Load(const bool is_reload)
{
    if (is_reload)
    {
        default_slot_size_ = 0;
        max_slot_size_ = 0;
        use_extension_ = false;
        slot_size_per_extension_ = 0;
    }

    BOOST_STATIC_ASSERT(MAX_CHARACTER_LIST_NUM == MAX_CHARACTER_SLOT_NUM);
    enum { kMinSlotSize = 6, kMaxSlotSize = MAX_CHARACTER_SLOT_NUM };
    BOOST_STATIC_ASSERT(kMinSlotSize <= kMaxSlotSize);

    const int row_size = GetRowSize();
    if (row_size != 1)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|줄 개수가 1개가 아닙니다.|줄 개수 = %u|"), 
               __FUNCTION__, 
               row_size);
        return false;
    }

    const int cur_row = 0;

    const CharSlotSize default_slot_size = GetDataBYTE(_T("DefaultSlotSize"), cur_row);
    if (default_slot_size < kMinSlotSize || default_slot_size > kMaxSlotSize)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|기본 캐릭터 슬롯 개수가 올바르지 않습니다.|기본 슬롯 개수 = %u|"), 
               __FUNCTION__, 
               default_slot_size);
        return false;
    }

    const CharSlotSize max_slot_size = GetDataBYTE(_T("MaxSlotSize"), cur_row);
    if (max_slot_size < kMinSlotSize || max_slot_size > kMaxSlotSize)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|최대 캐릭터 슬롯 개수가 올바르지 않습니다.|최대 슬롯 개수 = %u|"), 
               __FUNCTION__, 
               max_slot_size);
        return false;
    }

    if (default_slot_size > max_slot_size)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|기본 캐릭터 슬롯 개수가 최대 개수보다 많습니다.|기본 슬롯 개수 = &u, 최대 슬롯 개수 = %u|"), 
               __FUNCTION__, 
               default_slot_size, 
               max_slot_size);
        return false;
    }

    const BYTE use_extension = GetDataBYTE(_T("UseExtension"), cur_row);
    if (use_extension != 0 && use_extension != 1)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|캐릭터 슬롯 확장 사용 여부가 올바르지 않습니다.|슬롯 확장 사용 여부 = %u|"), 
               __FUNCTION__, 
               use_extension);
        return false;
    }

    if (use_extension == 0 && default_slot_size != max_slot_size)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|캐릭터 슬롯 확장을 사용하지 않는다면 기본 슬롯 개수와 최대 슬롯 개수는 같아야 합니다."), 
               __FUNCTION__);
        return false;
    }

    const CharSlotSize slot_size_per_extension = GetDataBYTE(_T("SlotSizePerExtension"), cur_row);
    if (slot_size_per_extension < 1 || 
        slot_size_per_extension > (max_slot_size-default_slot_size))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|확장 시도시 늘어나는 캐릭터 슬롯 개수가 올바르지 않습니다.|늘어나는 슬롯 개수 = %u|"), 
               __FUNCTION__, 
               slot_size_per_extension);
        return false;
    }

    default_slot_size_ = default_slot_size;
    max_slot_size_ = max_slot_size;
    use_extension_ = (use_extension == 1);
    slot_size_per_extension_ = slot_size_per_extension;

    return true;
}

#endif // _USE_MAX_CHAR_LIST_PARSER
