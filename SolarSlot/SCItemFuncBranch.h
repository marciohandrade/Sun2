#ifndef _SOLAR_SLOT_SCITEMFUNCBRANCH_H
#define _SOLAR_SLOT_SCITEMFUNCBRANCH_H

#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"

//==================================================================================================
//
namespace nsSlot {
;

struct SCItemLockInfo
{
    struct BitSize
    {
        enum { kBlocked = 1 };
        enum { kUserBlocked = 1 };
        enum { kItemTag = 2 };
        enum { kHistory = 2 };
    };

    bool blocked_;
    bool user_blocked_; //<- 펫 소환된 정보임 1이면 펫이 소환된 아이템
    bool locked_; // serial transaction?
    KindOfItemTag item_tag_; // f100909.1L
    eEtheriaHistory history_;

    // Default
    void SetInfo(
        const nsSlot::SerializeItemOption::sDefault& src, const BASE_ITEMINFO* const item_info) 
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sDefault* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // Time
    void SetInfo(
        const nsSlot::SerializeItemOption::sTime& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sTime* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // ExtendData
    void SetInfo(
        const nsSlot::SerializeItemOption::sExtendData& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sExtendData* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // EquipTime
    void SetInfo(
        const nsSlot::SerializeItemOption::sEquipTime& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sEquipTime* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // EquipEtc
    void SetInfo(
        const nsSlot::SerializeItemOption::sEquipEtc& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sEquipEtc* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // EquipEtcTime
    void SetInfo(
        const nsSlot::SerializeItemOption::sEquipEtcTime& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // Coord
    void SetInfo(
        const nsSlot::SerializeItemOption::sCoord& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sCoord* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // Lottery
    void SetInfo(
        const nsSlot::SerializeItemOption::sLottery& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sLottery* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // WayPoint
    void SetInfo(
        const nsSlot::SerializeItemOption::sWayPoint& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sWayPoint* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // Rider
    void SetInfo(
        const nsSlot::SerializeItemOption::sRider& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sRider* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // RiderParts
    void SetInfo(
        const nsSlot::SerializeItemOption::sRiderParts& src, const BASE_ITEMINFO* const item_info)
    {
        SetInfoByNotHistory(src, item_info);
    }
    void GetInfo(nsSlot::SerializeItemOption::sRiderParts* const dest) const
    {
        GetInfoByNotHistory(dest);
    }

    // Default
    template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
    void SetInfo(const SERIALIZE_ITEM_OPTION_TYPE_T& src, const BASE_ITEMINFO* const item_info)
    {
        __UNUSED(item_info);
        blocked_ = !!src.Block;
        user_blocked_ = !!src.UserBlock;
        item_tag_.value = static_cast<KindOfItemTag::value_type>(src.Tag);
        history_.value = static_cast<eEtheriaHistory::value_type>(src.History);
    }
    template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
    void GetInfo(SERIALIZE_ITEM_OPTION_TYPE_T* const dest) const
    {
        dest->Block = !!blocked_;
        dest->UserBlock = !!user_blocked_;
        dest->Tag = item_tag_.value;
        dest->History = history_.value;
    }

private:
    template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
    void SetInfoByNotHistory(
        const SERIALIZE_ITEM_OPTION_TYPE_T& src, const BASE_ITEMINFO* const item_info)
    {
        __UNUSED(item_info);
        blocked_ = !!src.Block;
        user_blocked_ = !!src.UserBlock;
        item_tag_.value = static_cast<KindOfItemTag::value_type>(src.Tag);
    }
    template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
    void GetInfoByNotHistory(SERIALIZE_ITEM_OPTION_TYPE_T* const dest) const
    {
        dest->Block = !!blocked_;
        dest->UserBlock = !!user_blocked_;
        dest->Tag = item_tag_.value;
    }
}; // SCItemLockInfo

//_NA_0_20100727_SOKET_SYSTEM_CONVERT
struct SCDefaultEtcInfo
{
    WORD data_;

    void SetInfo(const nsSlot::SerializeItemOption::sExtendData& src, const BASE_ITEMINFO* item_info);
    void GetInfo(nsSlot::SerializeItemOption::sExtendData* dest) const;
};

struct SCCoordinateInfo
{
    MAPCODE map_code_;
    short x_, y_, z_;

    void SetInfo(const nsSlot::SerializeItemOption::sCoord& src, const BASE_ITEMINFO* item_info);
    void GetInfo(nsSlot::SerializeItemOption::sCoord* dest) const;
};

struct SCTimeInfo
{
    struct BitSize
    {
        enum { kIsExpired = 1 };
        enum { kIsProgress = 1 };
        enum { kIsExpiredWait = 1 };
        enum { kDateTime = 45 };
    };

    DWORD last_checked_;
    bool is_expired_;
    bool is_progress_;
    bool is_expired_wait_; // 기간이 만료된 후 소멸되기 전까지 대기상태
    DWORD64 date_time_;

    // Fate
    void SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* const item_info)
    {
        __UNUSED(item_info);
        is_expired_ = !!src.IsExpired;
        is_expired_wait_ = !!src.IsExpiredWaitTime;
        date_time_ = src.DateTime;
    }
    void GetInfo(nsSlot::SerializeItemOption::sFate* const dest) const
    {
        dest->IsExpired = is_expired_;
        dest->IsExpiredWaitTime = is_expired_wait_;
        dest->DateTime = date_time_;
    }

    // WayPoint
    void SetInfo(const nsSlot::SerializeItemOption::sWayPoint& src, const BASE_ITEMINFO* const item_info)
    {
        __UNUSED(item_info);
        is_expired_ = !!src.IsExpired;
        is_progress_ = !!src.IsProgress;
        date_time_ = src.DateTime;
    }
    void GetInfo(nsSlot::SerializeItemOption::sWayPoint* const dest) const
    {
        dest->IsExpired = is_expired_;
        dest->IsProgress = is_progress_;
        dest->DateTime = date_time_;
    }

    // Pet
    void SetInfo(const nsSlot::SerializeItemOption::sPet& src, const BASE_ITEMINFO* const item_info)
    {
        __UNUSED(item_info);
        is_expired_ = !!src.IsExpired;
        date_time_ = src.DateTime;
    }
    void GetInfo(nsSlot::SerializeItemOption::sPet* const dest) const
    {
        dest->IsExpired = is_expired_;
        dest->DateTime = date_time_;
    }

    // Rider
    void SetInfo(const nsSlot::SerializeItemOption::sRider& src, const BASE_ITEMINFO* const item_info)
    {
        __UNUSED(item_info);
        is_expired_wait_ = !!src.IsExpiredWaitTime;
        date_time_ = src.DateTime;
    }
    void GetInfo(nsSlot::SerializeItemOption::sRider* const dest) const
    {
        dest->IsExpiredWaitTime = is_expired_wait_;
        dest->DateTime = date_time_;
    }

    // RiderParts
    void SetInfo(const nsSlot::SerializeItemOption::sRiderParts& src, const BASE_ITEMINFO* const item_info)
    {
        __UNUSED(item_info);
        is_expired_wait_ = !!src.IsExpiredWaitTime;
        is_progress_ = !!src.IsProgress;
        date_time_ = src.DateTime;
    }
    void GetInfo(nsSlot::SerializeItemOption::sRiderParts* const dest) const
    {
        dest->IsExpiredWaitTime = is_expired_wait_;
        dest->IsProgress = is_progress_;
        dest->DateTime = date_time_;
    }

    // Default
    template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
    void SetInfo(const SERIALIZE_ITEM_OPTION_TYPE_T& src, const BASE_ITEMINFO* const item_info)
    {
        __UNUSED(item_info);
        is_expired_ = !!src.IsExpired;
        is_expired_wait_ = !!src.IsExpiredWaitTime;
        is_progress_ = !!src.IsProgressTime;
        date_time_ = src.DateTime;
    }
    template < typename SERIALIZE_ITEM_OPTION_TYPE_T >
    void GetInfo(SERIALIZE_ITEM_OPTION_TYPE_T* const dest) const
    {
        dest->IsExpired = is_expired_;
        dest->IsExpiredWaitTime = is_expired_wait_;
        dest->IsProgressTime = is_progress_;
        dest->DateTime = date_time_;
    }
}; // SCTimeInfo

struct SCEquipInfo
{
    BOOLEAN is_limited_;
    BOOLEAN is_divine_;
    BYTE enchant_grade_;    // 인첸트 정보 [0, 12]
    BOOLEAN is_identify_possession_; // 귀속 인증
    BOOLEAN is_destroyed_;           // 해당 아이템 파괴 상태 (귀속 인증)
    BOOLEAN is_equip_ether_device_;  // 에테르 장비 장착 여부
    BYTE ether_bullet_index_;        // 에테르 장비 장착시 탄환 정보
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    BYTE awakening_grade_;  // 각성 정보 [0, 9]
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

    void SetInfo(const nsSlot::SerializeItemOption::sEquip& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipEtc& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipEtcTime& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sLimited& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sRiderParts& src, const BASE_ITEMINFO* item_info);

    void GetInfo(nsSlot::SerializeItemOption::sEquip* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sLimited* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sFate* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sRiderParts* dest) const;

    //void SetInfo(const nsSlot::SerializeItemOption::sRider& src, const BASE_ITEMINFO* item_info);
    //void GetInfo(nsSlot::SerializeItemOption::sRider* dest) const;
};


//==================================================================================================
//==================================================================================================
//==================================================================================================

//////////////

inline void
SCDefaultEtcInfo::SetInfo(const nsSlot::SerializeItemOption::sExtendData& src,
                          const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    data_ = WORD(src.data);
}

inline void
SCDefaultEtcInfo::GetInfo(nsSlot::SerializeItemOption::sExtendData* dest) const
{
    dest->data = this->data_;
}

//////////////

inline void
SCCoordinateInfo::SetInfo(const nsSlot::SerializeItemOption::sCoord& src,
                          const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    map_code_ = WORD(src.MapCode);
    x_ = short(src.X);
    y_ = short(src.Y);
    z_ = short(src.Z);
}

inline void
SCCoordinateInfo::GetInfo(nsSlot::SerializeItemOption::sCoord* dest) const
{
    dest->MapCode = this->map_code_;
    dest->X = WORD(this->x_);
    dest->Y = WORD(this->y_);
    dest->Z = WORD(this->z_);
}

//////////////

inline void
SCEquipInfo::SetInfo(const nsSlot::SerializeItemOption::sEquip& src, const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    is_divine_ = !!src.IsDivine;
    is_limited_ = 0;
    enchant_grade_ = (BYTE)src.EnchantGrade;
    is_identify_possession_ = (BOOLEAN)src.IsIdentifyPossession;
    is_destroyed_ = (BOOLEAN)src.IsDestroyed;
    is_equip_ether_device_ = (BOOLEAN)src.IsEquipEtherDevice;
    ether_bullet_index_ = (BYTE)src.EtherBulletIndex;
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    awakening_grade_ = static_cast<BYTE>(src.AwakeningGrade);
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
}

inline void
SCEquipInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src,
                     const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    is_divine_ = !!src.IsDivine;
    is_limited_ = 0;
    enchant_grade_ = (BYTE)src.EnchantGrade;
    is_identify_possession_ = (BOOLEAN)src.IsIdentifyPossession;
    is_destroyed_ = (BOOLEAN)src.IsDestroyed;
    is_equip_ether_device_ = (BOOLEAN)src.IsEquipEtherDevice;
    ether_bullet_index_ = (BYTE)src.EtherBulletIndex;
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    awakening_grade_ = static_cast<BYTE>(src.AwakeningGrade);
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
}

inline void
SCEquipInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipEtc& src,
                     const BASE_ITEMINFO* item_info)
{
    // CHANGES: f110711.5L, 'is_divine and enchant_grade' are restored
    // by _NA002823_WINGS_EQUIPMENTS_EXTENTION
    __UNUSED(item_info);
    is_divine_ = !!src.IsDivine;
    is_limited_ = 0;
    enchant_grade_ = (BYTE)src.EnchantGrade;
    is_identify_possession_ = (BOOLEAN)src.IsIdentifyPossession;
    is_destroyed_ = (BOOLEAN)src.IsDestroyed;
    //is_equip_ether_device_ = (BOOLEAN)src.IsEquipEtherDevice;
    //ether_bullet_index_ = (BYTE)src.EtherBulletIndex;
}

inline void
SCEquipInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipEtcTime& src,
                     const BASE_ITEMINFO* item_info)
{
    // CHANGES: f110711.5L, 'is_divine and enchant_grade' are restored
    // by _NA002823_WINGS_EQUIPMENTS_EXTENTION
    __UNUSED(item_info);
    is_divine_ = !!src.IsDivine;
    is_limited_ = 0;
    enchant_grade_ = (BYTE)src.EnchantGrade;
    is_identify_possession_ = (BOOLEAN)src.IsIdentifyPossession;
    is_destroyed_ = (BOOLEAN)src.IsDestroyed;
    //is_equip_ether_device_ = (BOOLEAN)src.IsEquipEtherDevice;
    //ether_bullet_index_ = (BYTE)src.EtherBulletIndex;
}

//inline void
//SCEquipInfo::SetInfo(const nsSlot::SerializeItemOption::sCoord& src, const BASE_ITEMINFO* item_info) {
//    is_divine_ = src.IsDivine;
//    is_limited_ = 0;
//    enchant_grade_ = src.EnchantGrade;
//    is_identify_possession_ = src.IsIdentifyPossession;
//    is_destroyed_ = src.IsDestroyed;
//    is_equip_ether_device_ = src.IsEquipEtherDevice;
//    ether_bullet_index_ = src.EtherBulletIndex;
//}

inline void
SCEquipInfo::SetInfo(const nsSlot::SerializeItemOption::sLimited& src,
                     const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    is_divine_ = !!src.IsDivine;
    is_limited_ = 1;
    enchant_grade_ = (BYTE)src.EnchantGrade;
    is_identify_possession_ = (BOOLEAN)src.IsIdentifyPossession;
    is_destroyed_ = (BOOLEAN)src.IsDestroyed;
    //is_equip_ether_device_ = (BOOLEAN)src.IsEquipEtherDevice;
    //ether_bullet_index_ = (BYTE)src.EtherBulletIndex;
}

inline void
SCEquipInfo::SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    //is_divine_ = !!src.IsDivine;
    is_limited_ = 0;
    enchant_grade_ = (BYTE)src.EnchantGrade;
    is_identify_possession_ = (BOOLEAN)src.IsIdentifyPossession;
    is_destroyed_ = (BOOLEAN)src.IsDestroyed;
    //is_equip_ether_device_ = (BOOLEAN)src.IsEquipEtherDevice; // f101105.3L, unused
    //ether_bullet_index_ = (BYTE)src.EtherBulletIndex; // f101105.3L, unused
}

inline void
SCEquipInfo::GetInfo(nsSlot::SerializeItemOption::sEquip* dest) const
{
    dest->IsDivine = is_divine_;
    //is_limited_ = 0;
    dest->EnchantGrade = enchant_grade_;
    dest->IsIdentifyPossession = is_identify_possession_;
    dest->IsDestroyed = is_destroyed_;
    dest->IsEquipEtherDevice = is_equip_ether_device_;
    dest->EtherBulletIndex = ether_bullet_index_;
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    dest->AwakeningGrade = awakening_grade_;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
}

inline void
SCEquipInfo::GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest) const
{
    dest->IsDivine = is_divine_;
    //is_limited_ = 0;
    dest->EnchantGrade = enchant_grade_;
    dest->IsIdentifyPossession = is_identify_possession_;
    dest->IsDestroyed = is_destroyed_;
    dest->IsEquipEtherDevice = is_equip_ether_device_;
    dest->EtherBulletIndex = ether_bullet_index_;
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    dest->AwakeningGrade = awakening_grade_;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
}

inline void
SCEquipInfo::GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest) const
{
    // CHANGES: f110711.5L, 'is_divine and enchant_grade' are restored
    // by _NA002823_WINGS_EQUIPMENTS_EXTENTION
    dest->IsDivine = is_divine_;
    //is_limited_ = 0;
    dest->EnchantGrade = enchant_grade_;
    dest->IsIdentifyPossession = is_identify_possession_;
    dest->IsDestroyed = is_destroyed_;
    //dest->IsEquipEtherDevice = is_equip_ether_device_;
    //dest->EtherBulletIndex = ether_bullet_index_;
}

inline void
SCEquipInfo::GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest) const
{
    // CHANGES: f110711.5L, 'is_divine and enchant_grade' are restored
    // by _NA002823_WINGS_EQUIPMENTS_EXTENTION
    dest->IsDivine = is_divine_;
    //is_limited_ = 0;
    dest->EnchantGrade = enchant_grade_;
    dest->IsIdentifyPossession = is_identify_possession_;
    dest->IsDestroyed = is_destroyed_;
    //dest->IsEquipEtherDevice = is_equip_ether_device_;
    //dest->EtherBulletIndex = ether_bullet_index_;
}

inline void
SCEquipInfo::GetInfo(nsSlot::SerializeItemOption::sLimited* dest) const
{
    dest->IsDivine = is_divine_;
    //is_limited_ = 0;
    dest->EnchantGrade = enchant_grade_;
    dest->IsIdentifyPossession = is_identify_possession_;
    dest->IsDestroyed = is_destroyed_;
    //dest->IsEquipEtherDevice = is_equip_ether_device_;
    //dest->EtherBulletIndex = ether_bullet_index_;
}

inline void
SCEquipInfo::GetInfo(nsSlot::SerializeItemOption::sFate* dest) const
{
    //dest->IsDivine = is_divine_;
    //is_limited_ = 0;
    dest->EnchantGrade = enchant_grade_;
    dest->IsIdentifyPossession = is_identify_possession_;
    dest->IsDestroyed = is_destroyed_;
    //dest->IsEquipEtherDevice = is_equip_ether_device_; // f101105.3L, unused
    //dest->EtherBulletIndex = ether_bullet_index_; // f101105.3L, unused
}

inline void
SCEquipInfo::SetInfo(const nsSlot::SerializeItemOption::sRiderParts& src,
                     const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    //is_divine_ = !!src.IsDivine;
    is_limited_ = 0;
    enchant_grade_ = (BYTE)src.EnchantGrade;
    //is_identify_possession_ = (BOOLEAN)src.IsIdentifyPossession;
    //is_destroyed_ = (BOOLEAN)src.IsDestroyed;
    //is_equip_ether_device_ = (BOOLEAN)src.IsEquipEtherDevice;
    //ether_bullet_index_ = (BYTE)src.EtherBulletIndex;
}

inline void
SCEquipInfo::GetInfo(nsSlot::SerializeItemOption::sRiderParts* dest) const
{
    //dest->IsDivine = is_divine_;
    //is_limited_ = 0;
    dest->EnchantGrade = enchant_grade_;
    //dest->IsIdentifyPossession = is_identify_possession_;
    //dest->IsDestroyed = is_destroyed_;
    //dest->IsEquipEtherDevice = is_equip_ether_device_;
    //dest->EtherBulletIndex = ether_bullet_index_;
}

}; //end of namespace

#endif //_SOLAR_SLOT_SCITEMFUNCBRANCH_H

