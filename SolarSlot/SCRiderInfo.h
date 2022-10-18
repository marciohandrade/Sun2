#ifndef _SOLAR_SLOT_SCRIDERINFO_H
#define _SOLAR_SLOT_SCRIDERINFO_H

//==================================================================================================
/// ������ Rider & RiderParts ���� Ŭ����
/**
    @author waverix
    @since  2009/09/18
    @remark
    - Ż �� �� Ż �� ������ ���� ����
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

// Rider Parts�� �����Ǿ� �ִ� ��� Ȱ�� ���
struct eRiderAttrChangeMode {
    enum {
        eMode_None              = 0,
        eMode_ActiveToInactive  ,
        eMode_InactiveToActive  , // rule������ �ʿ� ���� ���� �ִ�.
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
    eRIDER_PARTS_POS parts_type_; // ���� ��ġ (by Script, ���� None�̸�, ������)
    BOOLEAN is_unlimited_period_;
    // �ϱ� ������ Rider�� �����Ǿ��� �� ���Ǵ� ������...
    eRiderAttrChangeMode change_mode_; // change_status : (����<->������)
    BYTE enchant_grade_;      // WARNING: ���� ���������� ����� ��� ����, SCItemSlot���� ���)
    SLOTCODE item_code_;      // ��
    KindOfItemTag item_tag_;  // ��, added by 
    BOOLEAN is_progress_;     // ��
    BOOLEAN is_expired_wait_; // ��
    DWORD64 date_time_;       // ��
    const BASE_ITEMINFO* item_info_;// (WARNING) ��
};

//==================================================================================================

class SCRiderPartsInfo : private SCRiderPartsData
{
private:
    void Clear();

    eRiderAttrChangeMode GetActiveMode() const;
    void Updated(); // eRiderAttrChangeMode�� ���� ���� �������� �ݿ��� �� ȣ���ؾ� �Ѵ�.

    static bool ExtractToPartsInfo(const SCItemSlot& parts_item, SCRiderPartsInfo* parts_info);
    static bool ExtractToItem(const SCRiderPartsInfo& parts_info, SCItemSlot* parts_item);

    // ���� ���������� ����� ����Ѵ�.
    void SetInfo(nsSlot::SerializeItemOption::sRiderParts* src, const BASE_ITEMINFO* item_info);
    void GetInfo(nsSlot::SerializeItemOption::sRiderParts* dest);

    //void SetInfo(const SCRiderPartsInfo& IN parts_info, const BASE_ITEMINFO* item_info);
    //void GetInfo(SCRiderPartsInfo* dest);

    //----------------------------------------------------------------------------------------------
    // Rider�� ������ ����� ���ȴ�.
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

    // (WARNING) �ϱ� ������ ����Ǵ� ��� �ٲ�� ������Ʈ �������̴�.
    // NoChanges status:
    //  - RiderSummonStatus : ���� ������ ���� �������� ���� ���� ����� �������� ��ȯ�Ѵ�.
    //      'GetLatestChangedInfo()'�� ���� �� �ִ� ���� �ٲ��� �ʴ´�.
    //  -   call by SCItemSlot::GetUpdateEnableInfo
    //  - ex) ó�� ��ȯ�� �������̶� ���� �������� Ŭ���̾�Ʈ���� ��� �����ؾ� �ϴ°�?
    //        SCItemSlot::RiderSummonStatus(false);
    //        update_bits = SCItemSlot::RiderSummonStatus(true); <�� ���� ��� ������ ������ ������
    //        attribute_calculator.ApplyRidingRider(true,...update_bits...);
    //  - ex) ��ȯ ������ ���, ������ �ش� ��ü ��Ʈ���� �������� �ʴ´�.
    //        ����) ���� ���ᰡ �߻��� ��� ItemSync������� ���� calc������ �̷��� ���̴�.
    //        ��ȯ ����) update_bits = SCItemSlot::RiderSummonStatus(false);
    //        attribute_calculator.ApplyRidingRider(false,...update_bits...);
    // Changes status:
    //  - EquipParts : ���� ���� ������ ����/��ȯ���� �ʴ� �ٴ� ���� �Ͽ�, ��ȭ�� ��
    //  -   call by SCItemSlot::EquipRiderParts
    //  - GetUnEquipChangedBits : ���� ���� ����� ��ȭ�� ��
    //  -   call by SCItemSlot::CanUnEquipRiderParts
    //  - UnEquipParts : ���� ���� ������� �ʰ� ��ȭ�� ��
    //  -   call by SCItemSlot::UnEquipRiderParts
    //  - _UpdateRider : call by ItemManager::Update : ���� ����� ��ȭ�� ��
    nsSlot::RiderUpdateBits GetLatestChangedInfo() const;
    //add_rider, add_parts(all) with filter 'timeout'
    nsSlot::RiderUpdateBits GetUpdateEnableInfo(const SCItemSlot& rider_item, BOOLEAN summon) const;
    void Updated(); // eRiderAttrChangeMode�� ���� ���� �������� �ݿ��� �� ȣ���ؾ� �Ѵ�.

    void SetInfo(nsSlot::SerializeItemOption::sRider* src, const BASE_ITEMINFO* item_info);
    //                                                      SCItemSlot::time_info_
    void GetInfo(nsSlot::SerializeItemOption::sRider* dest, nsSlot::SCTimeInfo* base_time_info);

    //                           SCItemSlot      on update time value
    bool _UpdateRider(SCItemSlot* rider_item, DWORD64 curr_time_wz);
    ////                           SCItemSlot::time_info_        on update time value
    //bool _UpdatePartsTime(nsSlot::SCTimeInfo* rider_time_info, DWORD64 curr_time_wz,
    //                         nsSlot::RiderUpdateBits* update_bits);

    //----------------------------------------------------------------------------------------------
    // RiderParts�� DateTime�� ������ ���, Rider������ ���� ��� �ɼ����� ����ȴ�.
    static const DWORD NumberOfBits_IsUnlimitedPeriod = 1;
    static const DWORD NumberOfBits_DateOffset = 17;
    static const DWORD NumberOfBits_DateOffset_SignBit = 1 << 16;
    static const DWORD NumberOfBits_DateOffset_Value = NumberOfBits_DateOffset_SignBit - 1;
    static const DWORD MaxPeriod = 30 /*day*/ * (24 /*hour*/ * 60 /*minute*/);
    //----------------------------------------------------------------------------------------------
    // ��ȯ ���δ� SCItemSlot::UserBlocked�� ��
    eRiderAttrChangeMode change_mode_; // change_status : (����<->������)
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
