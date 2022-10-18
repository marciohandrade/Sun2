#include "StdAfx.h"
#include ".\ZardComposeInfoParser.h"
#include "PacketStruct_CG.h"
#include "SCItemSlot.h"
#include "SCItemSlotContainer.h"

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Constructor & Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////

ZardComposeInfoParser::ZardComposeInfoParser()
{
}

ZardComposeInfoParser::~ZardComposeInfoParser()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Implement
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace util {
namespace internal {
static bool IsValidPosition(const SCItemSlotContainer& inventory, const POSTYPE position)
{
    return (position >= 0 && position < inventory.GetMaxSlotNum());
}

static bool IsValidItemState(const SCItemSlot& inventory_item)
{
#ifdef _SERVER
    return (!inventory_item.IsLocked() && 
        !inventory_item.IsBlocked() && 
        !inventory_item.IsUserBlocked() && 
        inventory_item.IsContainValidItem());
#else
    return (!inventory_item.IsBlocked() && 
        !inventory_item.IsUserBlocked() && 
        inventory_item.IsContainValidItem());
#endif // _SERVER
}
    
static bool IsValidZardGrade(const ZardComposeInfo::ZardGrade zard_grade)
{
    return (zard_grade >= kZardGrade1 && zard_grade < kZardGradeMax);
}

static bool IsValidZardRatio(const ZardComposeInfo::ZardRatio zard_ratio)
{
    return (zard_ratio >= 1 && zard_ratio <= ZardComposeInfo::kMaxRatio);
}

static bool IsValidItem(
    SCItemSlotContainer& inventory, 
    const POSTYPE zard_position_list[ZardComposeInfoParser::kMaterialSize], 
    const POSTYPE additive_position)
{
    ZardComposeInfo::ZardGrade zard_grade = kZardGradeUnused;
    for (int i = 0; i < ZardComposeInfoParser::kMaterialSize; ++i)
    {
        // 아이템 위치를 검사한다.
        if (!IsValidPosition(inventory, zard_position_list[i]))
        {
            return false;
        }

        // 아이템 상태를 검사한다.
        const SCItemSlot& zard_item = 
            static_cast<const SCItemSlot&>(inventory.GetSlot(zard_position_list[i]));
        if (!IsValidItemState(zard_item))
        {
            return false;
        }

        // 쟈드 아이템인지 검사한다.
        if (!zard_item.IsZardItem())
        {
            return false;
        }

        // 쟈드 등급을 검사한다.
        const ZardComposeInfo::ZardGrade new_zard_grade = (zard_item.GetItemInfo())->zard_grade;
        if (!IsValidZardGrade(new_zard_grade))
        {
            return false;
        }

        // 재료로 사용할 쟈드의 등급이 모두 같은지 검사한다.
        if (i == 0)
        {
            zard_grade = new_zard_grade;
        }
        else
        {
            if (zard_grade != new_zard_grade)
            {
                return false;
            }
        }
    }

    // 쟈드 합성 첨가제를 사용 했다면 
    if (additive_position != INVALID_POSTYPE_VALUE)
    {
        // 아이템 위치를 검사한다.
        if (!IsValidPosition(inventory, additive_position))
        {
            return false;
        }

        // 아이템 상태를 검사한다.
        const SCItemSlot& additive_item = 
            static_cast<const SCItemSlot&>(inventory.GetSlot(additive_position));
        if (!IsValidItemState(additive_item))
        {
            return false;
        }

        // 쟈드 합성 첨가제인지 검사한다.
        if (!additive_item.IsZardAdditiveItem())
        {
            return false;
        }

        // 추가 합성 성공률을 검사한다.
        if (!IsValidZardRatio((additive_item.GetItemInfo())->zard_success_per))
        {
            return false;
        }
    }

    return true;
}
} // internal
} // util


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Public
////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL ZardComposeInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch (script_code)
    {
    case SCRIPT_ZARD_COMPOSE_INFO:
        return (LoadZardCompose(!!is_reload) ? TRUE : FALSE);
    }
    return FALSE;
}

const ZardComposeInfo* ZardComposeInfoParser::FindZardCompose(
    const ZardComposeInfo::ZardGrade zard_grade) const 
{
    if (!util::internal::IsValidZardGrade(zard_grade))
    {
        return NULL;
    }
    return &(zard_compose_list_[zard_grade-1]);
}

ZardComposeInfo::ZardRatio ZardComposeInfoParser::GetZardCompositionSuccessRatio( 
    SCItemSlotContainer& inventory, 
    const POSTYPE zard_position_list[kMaterialSize], 
    const POSTYPE additive_position, 
    int fail_number) const
{
    // 합성 실패 회수를 검사한다.
    if (fail_number < 0)
    {
        return 0;
    }
    else if (fail_number >= ZardComposeInfo::kSuccessRatioCount)
    {
        fail_number = ZardComposeInfo::kSuccessRatioCount - 1;
    }

    // 아이템들이 유효한지 검사한다.
    if (!util::internal::IsValidItem(inventory, zard_position_list, additive_position))
    {
        return 0;
    }

    // 성공 확률을 얻는다.
    const SCItemSlot& zard_item = 
        static_cast<const SCItemSlot&>(inventory.GetSlot(zard_position_list[0]));
    const ZardComposeInfo::ZardGrade zard_grade = (zard_item.GetItemInfo())->zard_grade;    
    const ZardComposeInfo* const zard_composition = FindZardCompose(zard_grade);
    if (zard_composition == NULL)
    {
        return 0;
    }
    ZardComposeInfo::ZardRatio success_ratio = zard_composition->success_ratio_[fail_number];
    if (!util::internal::IsValidZardRatio(success_ratio))
    {
        return 0;
    }
    
    // 첨가제를 사용했다면 추가 성공 확률을 더한다.
    if (additive_position != INVALID_POSTYPE_VALUE)
    {
        const SCItemSlot& additive_item = 
            static_cast<const SCItemSlot&>(inventory.GetSlot(additive_position));
        const ZardComposeInfo::ZardRatio zard_success_per = 
            (additive_item.GetItemInfo())->zard_success_per;
        success_ratio += zard_success_per;
    }
    
    return success_ratio;
}

bool ZardComposeInfoParser::GetZardCompositionRequest( 
    SCItemSlotContainer& inventory, 
    const POSTYPE zard_position_list[kMaterialSize], 
    const POSTYPE additive_position, 
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN& OUT request) const
{
    // 인벤토리에 여유 공간이 없으면 시도할 필요가 없다.
    if (inventory.GetEmptySlotNum() == 0)
    {
        return false;
    }

    // 아이템들이 유효한지 검사한다.
    if (!util::internal::IsValidItem(inventory, zard_position_list, additive_position))
    {
        return false;
    }

    // 아이템이 중복인지 검사한다.
    for (POSTYPE pos_index = 0; pos_index < (kMaterialSize-1); ++pos_index)
    {
        for (POSTYPE dup_index = (pos_index+1); dup_index < kMaterialSize; ++dup_index)
        {
            if (zard_position_list[pos_index] == zard_position_list[dup_index])
            {
                return false;
            }
        }
    }

    request.request_type_ = MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kZardRequest;
    request.is_composition_ = true;
    request.reserved_code_ = additive_position;
    request.using_item_position_ = 0;

    // 합성 재료로 사용할 쟈드를 설정한다.
    BOOST_STATIC_ASSERT(kMaterialSize <= MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::kMaxMaterialSize);
    MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialListSize material_index = 0;
    while (material_index < kMaterialSize)
    {
        MSG_CG_ITEM_COMPOSE_OR_DECOMPOSE_SYN::MaterialInfo& zard_material = 
            request.material_list_[material_index];
        zard_material.inventory_position_ = zard_position_list[material_index];
        zard_material.selected_number_ = 1;
        zard_material.matched_index_ = 0;
        ++material_index;
    }
    request.material_list_size_ = material_index;
    
    return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// REGION : Private
////////////////////////////////////////////////////////////////////////////////////////////////////

bool ZardComposeInfoParser::IsValid(const ZardComposeInfo& zard_compose) const 
{
    if (!util::internal::IsValidZardGrade(zard_compose.zard_grade_))
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|조합 불가능한 쟈드 등급 입니다.|ZardGrade = %d|"), 
               __FUNCTION__, 
               zard_compose.zard_grade_); 
        return false;
    }

    for (int i = 0; i < ZardComposeInfo::kSuccessRatioCount; ++i)
    {
        if (!util::internal::IsValidZardRatio(zard_compose.success_ratio_[i]))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|성공 확률이 허용할 수 없는 값입니다.|ZardGrade = %d, SuccessRatio = %d|"), 
                   __FUNCTION__, 
                   zard_compose.zard_grade_, 
                   zard_compose.success_ratio_[i]);
            return false;
        }
    }

    return true;
}

bool ZardComposeInfoParser::LoadZardCompose(const bool is_reload)
{
    __UNUSED(is_reload);

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const ZardComposeInfo::ZardGrade zard_grade = GetDataInt(_T("ZardGrade"), row);
        if (!util::internal::IsValidZardGrade(zard_grade))
        {
            continue;
        }

        ZardComposeInfo& zard_compose = zard_compose_list_[zard_grade-1];
        {
            zard_compose.zard_grade_ = zard_grade;
            for (int i = 0; i < ZardComposeInfo::kSuccessRatioCount; ++i)
            {
                GetData(row, zard_compose.success_ratio_[i], _T("SuccesRatio%d"), i+1);
            }
        }
        
        if (!IsValid(zard_compose))
        {
            continue;
        }
    }

    return true;
}
#endif //_NA_000251_20100727_SOCKET_SYSTEM