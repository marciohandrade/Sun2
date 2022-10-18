#ifndef _SOLAR_SLOT_SCRIDERINFO_H
#define _SOLAR_SLOT_SCRIDERINFO_H

//==================================================================================================
/// 아이템 Rider & RiderParts 정보 클래스
/**
    @author waverix
    @since  2009/09/18
    @remark
    - 탈 것 및 탈 것 아이템 정보 관리
    @history 
    - 09.09.18 | waverix | created
*/
//==================================================================================================

#include <ResultCode.h>
#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"
#include "SCItemFuncBranch.h"

//==================================================================================================
// __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__

class SCRiderInfo;

// Rider Parts로 장착되어 있는 경우 활성 모드
struct eRiderAttrChangeMode {
    enum {
        eMode_None              = 0,
        eMode_ActiveToInactive  ,
        eMode_InactiveToActive  , // rule조율시 필요 없을 수도 있다.
    };
    BYTE value;
};

//==================================================================================================

struct SCRiderPartsData
{
    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_ItemCode = 16;
    // (CHANGES) (f100803.2L) change used bits range from 4 bits to 3 bits [0,...5]
    // to support the Global version need 'Tag' field about a rider parts in a rider
    static const DWORD NumberOfBits_EnchantGrade = 3;
    static const DWORD NumberOfBits_Tag = 1; // reserved for tag, this field is not used at this point
    static const DWORD NumberOfBits_IsUnlimitedPeriod = 1;
    static const DWORD NumberOfBits_IsProgress = 1;
    static const DWORD NumberOfBits_IsExpiredWait = 1;
    static const DWORD NumberOfBits_DateTime = 45;
    //----------------------------------------------------------------------------------------------
    eRIDER_PARTS_POS parts_type_; // 장착 위치 (by Script, 만약 None이면, 미장착)
    BOOLEAN is_unlimited_period_;
    // 하기 내용은 Rider에 장착되었을 때 사용되는 변수들...
    eRiderAttrChangeMode change_mode_; // change_status : (적용<->미적용)
    BYTE enchant_grade_;      // WARNING: 독립 아이템으로 존재시 사용 안함, SCItemSlot정보 사용)
    SLOTCODE item_code_;      // 상동
    KindOfItemTag item_tag_;  // 상동, added by 
    BOOLEAN is_progress_;     // 상동
    BOOLEAN is_expired_wait_; // 상동
    DWORD64 date_time_;       // 상동
    const BASE_ITEMINFO* item_info_;// (WARNING) 상동
};

//==================================================================================================

class SCRiderPartsInfo : private SCRiderPartsData
{
private:
    void Clear();

    eRiderAttrChangeMode GetActiveMode() const;
    void Updated(); // eRiderAttrChangeMode로 변한 값을 로직에서 반영한 뒤 호출해야 한다.

    static bool ExtractToPartsInfo(const SCItemSlot& parts_item, SCRiderPartsInfo* parts_info);
    static bool ExtractToItem(const SCRiderPartsInfo& parts_info, SCItemSlot* parts_item);

    // 독립 아이템으로 존재시 사용한다.
    void SetInfo(nsSlot::SerializeItemOption::sRiderParts* src, const BASE_ITEMINFO* item_info);
    void GetInfo(nsSlot::SerializeItemOption::sRiderParts* dest);

    //void SetInfo(const SCRiderPartsInfo& IN parts_info, const BASE_ITEMINFO* item_info);
    //void GetInfo(SCRiderPartsInfo* dest);

    //----------------------------------------------------------------------------------------------
    // Rider의 파츠로 존재시 사용된다.
    static DWORD _ConvertToDateOffset(DWORD64 base_date_time, DWORD64 parts_date_time);
    static DWORD64 _ConvertToDateTime(DWORD64 base_date_time, DWORD parts_date_offset);

    friend class SCRiderInfo;
    friend class SCItemSlot;
};

//==================================================================================================

class SCRiderInfo
{
private:
    void Clear();

    RC::eITEM_RESULT CanEquipParts(const SCItemSlot& rider_item_slot,
                                   const BASE_ITEMINFO* rider_item_info,
                                   eRIDER_PARTS_POS pos,
                                   const SCItemSlot& IN parts_item_slot,
                                   const BASE_ITEMINFO* parts_item_info) const;

    bool EquipParts(const SCItemSlot& rider_item_slot,
                    const BASE_ITEMINFO* rider_item_info,
                    eRIDER_PARTS_POS pos,
                    const SCItemSlot& IN parts_item_slot,
                    const BASE_ITEMINFO* parts_item_info);

    bool GetUnEquipChangedBits(const SCItemSlot& IN rider_item_slot,
                               eRIDER_PARTS_POS pos, nsSlot::RiderUpdateBits* update_bits);
    RC::eITEM_RESULT UnEquipParts(const SCItemSlot& IN rider_item_slot,
                                  eRIDER_PARTS_POS pos,
                                  SCItemSlot* OUT parts_item_slot);

    // (returns) if exist then Aquire 'SCRiderPartsInfo'
    //                        { exist | empty (cleared stream), check 'parts_type_' }
    //           else     then return 0 { index out of range }
    const SCRiderPartsInfo* GetPartsInfo(const SCItemSlot* rider_item_slot,
                                         eRIDER_PARTS_POS pos) const;

    // (WARNING) 하기 연산이 수행되는 경우 바뀌는 업데이트 설정값이다.
    // NoChanges status:
    //  - RiderSummonStatus : 현재 아이템 상태 기준으로 기한 만료 고려된 설정값을 반환한다.
    //      'GetLatestChangedInfo()'로 얻을 수 있는 값을 바꾸지 않는다.
    //  -   call by SCItemSlot::GetUpdateEnableInfo
    //  - ex) 처음 소환된 아이템이라 받은 아이템을 클라이언트에서 어떻게 연산해야 하는가?
    //        SCItemSlot::RiderSummonStatus(false);
    //        update_bits = SCItemSlot::RiderSummonStatus(true); <현 시점 사용 가능한 아이템 정보들
    //        attribute_calculator.ApplyRidingRider(true,...update_bits...);
    //  - ex) 소환 해제된 경우, 서버는 해당 전체 스트림을 보내주지 않는다.
    //        전제) 기한 만료가 발생한 경우 ItemSync명령으로 먼저 calc변경이 이뤄질 것이다.
    //        소환 해제) update_bits = SCItemSlot::RiderSummonStatus(false);
    //        attribute_calculator.ApplyRidingRider(false,...update_bits...);
    // Changes status:
    //  - EquipParts : 기한 만료 아이템 장착/소환되지 않는 다는 전제 하에, 변화된 값
    //  -   call by SCItemSlot::EquipRiderParts
    //  - GetUnEquipChangedBits : 기한 만료 고려해 변화된 값
    //  -   call by SCItemSlot::CanUnEquipRiderParts
    //  - UnEquipParts : 기한 만료 고려되지 않고 변화된 값
    //  -   call by SCItemSlot::UnEquipRiderParts
    //  - _UpdateRider : call by ItemManager::Update : 기한 만료시 변화된 값
    nsSlot::RiderUpdateBits GetLatestChangedInfo() const;
    //add_rider, add_parts(all) with filter 'timeout'
    nsSlot::RiderUpdateBits GetUpdateEnableInfo(const SCItemSlot& rider_item, BOOLEAN summon) const;
    void Updated(); // eRiderAttrChangeMode로 변한 값을 로직에서 반영한 뒤 호출해야 한다.

    void SetInfo(nsSlot::SerializeItemOption::sRider* src, const BASE_ITEMINFO* item_info);
    //                                                      SCItemSlot::time_info_
    void GetInfo(nsSlot::SerializeItemOption::sRider* dest, nsSlot::SCTimeInfo* base_time_info);

    //                           SCItemSlot      on update time value
    bool _UpdateRider(SCItemSlot* rider_item, DWORD64 curr_time_wz);
    ////                           SCItemSlot::time_info_        on update time value
    //bool _UpdatePartsTime(nsSlot::SCTimeInfo* rider_time_info, DWORD64 curr_time_wz,
    //                         nsSlot::RiderUpdateBits* update_bits);

    //----------------------------------------------------------------------------------------------
    // RiderParts의 DateTime이 장착될 경우, Rider아이템 기준 상대 옵셋으로 저장된다.
    static const DWORD NumberOfBits_IsUnlimitedPeriod = 1;
    static const DWORD NumberOfBits_DateOffset = 17;
    static const DWORD NumberOfBits_DateOffset_SignBit = 1 << 16;
    static const DWORD NumberOfBits_DateOffset_Value = NumberOfBits_DateOffset_SignBit - 1;
    static const DWORD MaxPeriod = 30 /*day*/ * (24 /*hour*/ * 60 /*minute*/);
    //----------------------------------------------------------------------------------------------
    // 소환 여부는 SCItemSlot::UserBlocked로 평가
    eRiderAttrChangeMode change_mode_; // change_status : (적용<->미적용)
    nsSlot::RiderUpdateBits update_bits_;
    BOOLEAN is_unlimited_period_;
    SCRiderPartsInfo rider_parts_[eRIDER_PARTS_POS::Counts];

    friend class SCItemSlot;
    friend class SCRiderPartsInfo;
};

//==================================================================================================
//

inline void
SCRiderPartsInfo::Clear() {
    ZeroMemory(this, sizeof(*this));
}

inline eRiderAttrChangeMode
SCRiderPartsInfo::GetActiveMode() const {
    if(parts_type_.value != parts_type_.None)
        return change_mode_;
    eRiderAttrChangeMode mode = { change_mode_.eMode_None };
    return mode;
}

inline void
SCRiderPartsInfo::Updated() {
    change_mode_.value = change_mode_.eMode_None;
}



inline void
SCRiderInfo::Clear() {
    ZeroMemory(this, sizeof(*this));
}

inline nsSlot::RiderUpdateBits
SCRiderInfo::GetLatestChangedInfo() const {
    return update_bits_;
}

inline void
SCRiderInfo::Updated() {
    update_bits_.value = 0;
}

#endif //_SOLAR_SLOT_SCRIDERINFO_H
