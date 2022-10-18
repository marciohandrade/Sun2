#ifndef _SOLAR_SLOT_SCLOTTOINFO_H
#define _SOLAR_SLOT_SCLOTTOINFO_H
#pragma once

// define
#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"
//class 
class SCItemSlot;


//==================================================================================================
//
namespace nsSlot {

struct LottoInfoSerializeStruct;

};

// 외부 제어가 심한 객체라서 현 시점 쉽게 변경하질 못하고 있다.
class SCLottoInfo
{
public:
    struct MiniSlot {
        BOOLEAN Random;     // is random item
        BOOLEAN ExistItem;  // Is exist item
        BYTE InfoPos;       // part position of script
    };

    //__forceinline SCLottoInfo() {}
    //SCLottoInfo(const SCLottoInfo& rhs);
    //SCLottoInfo& operator=(const SCLottoInfo& rhs);

    void Clear();

    //void Encode(BitStream& IN bitstream) const;
    //void Decode(BitStream& OUT bitstream);
    //void Encode(OPTIONPART_VER5* OUT dest) const;
    //void Decode(const OPTIONPART_VER5* IN src);

    void SetInfo(nsSlot::SerializeItemOption::sLottery* src, const BASE_ITEMINFO* item_info);
    void GetInfo(nsSlot::SerializeItemOption::sLottery* dest);

    BOOLEAN Identity();
    BOOLEAN IsIdentity() const;
    BOOLEAN ValidPosition(POSTYPE val) const;
    BOOLEAN ExistItem(POSTYPE pos) const;
    void Move(POSTYPE from, POSTYPE to);
    void ClearMiniSlot(POSTYPE pos);
    void SetMiniSlot(POSTYPE pos, const MiniSlot& slot);
    const MiniSlot& GetMiniSlot(POSTYPE pos) const;

    // corresponding interface : result by __PROPERTY
    WORD GetLottoIndex() const;
    BOOLEAN GetIdentityStatus() const;
    BOOLEAN GetOpenStatus() const;
    int GetCount() const;
    // corresponding interface : result by __PROPERTY
    // external controller, ...
    void /*DEPRECATE_RESERVED*/ SetLottoIndex(DWORD lottery_index);
    void /*DEPRECATE_RESERVED*/ SetIdentityStatus(BOOL identity_status);
    void /*DEPRECATE_RESERVED*/ SetOpenStatus(BOOL open_status);
    void /*DEPRECATE_RESERVED*/ SetCount(int count);

private:
    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_LotteryIndex      = 16;
    static const DWORD NumberOfBits_IdentityStatus    =  8;
    static const DWORD NumberOfBits_NumberOfMiniSlots =  8;

    static const DWORD NumberOfBits_MiniSlot_IsRandom  =  1;
    static const DWORD NumberOfBits_MiniSlot_ExistItem =  1;
    static const DWORD NumberOfBits_MiniSlot_InfoPos   =  4;
    static const DWORD NumberOfBits_MiniSlot_Reserve   =  2; //...?
    //----------------------------------------------------------------------------------------------
    WORD lottery_index_;
    BOOLEAN identity_status_;
    BOOLEAN open_status_;
    uint8_t number_of_minislots_;
    MiniSlot mini_slots_[MAX_PANDORA_SLOT_NUM];
    static const MiniSlot NullMiniSlot;

    friend class SCItemSlot;
    friend struct nsSlot::LottoInfoSerializeStruct;
};

//--------------------------------------------------------------------------------------------------
typedef SCLottoInfo::MiniSlot   MiniSlot; // corresponding type


inline void
SCLottoInfo::Clear() {
    ZeroMemory(this, sizeof(*this));
}

inline BOOLEAN
SCLottoInfo::IsIdentity() const {
    return identity_status_;
}

inline BOOLEAN
SCLottoInfo::ValidPosition(POSTYPE val) const {
    return 0 <= val && val < MAX_PANDORA_SLOT_NUM;
}

inline BOOLEAN
SCLottoInfo::ExistItem(POSTYPE pos) const {
    if(FlowControl::FCAssert(0 <= pos && pos < MAX_PANDORA_SLOT_NUM)) {
        return mini_slots_[pos].ExistItem;
    }
    return false;
}

inline void
SCLottoInfo::SetMiniSlot(POSTYPE pos, const MiniSlot& slot)
{
    // TODO(WAVERIX) (WARNING) 에러 검증 방법 개선할 것.
    if(FlowControl::FCAssert(0 <= pos && pos < MAX_PANDORA_SLOT_NUM)) {
        mini_slots_[pos] = slot;
    }
}

inline const MiniSlot&
SCLottoInfo::GetMiniSlot(POSTYPE pos) const
{
    if(FlowControl::FCAssert(0 <= pos && pos < MAX_PANDORA_SLOT_NUM)) {
        return mini_slots_[pos];
    }
    return NullMiniSlot;
}

// corresponding interface : result by __PROPERTY
inline WORD
SCLottoInfo::GetLottoIndex() const {
    return lottery_index_;
}

inline BOOLEAN
SCLottoInfo::GetIdentityStatus() const {
    return identity_status_;
}

inline BOOLEAN
SCLottoInfo::GetOpenStatus() const {
    return open_status_;
}

inline int
SCLottoInfo::GetCount() const {
    return number_of_minislots_;
}
// corresponding interface : result by __PROPERTY
// external controller, ...
inline void DEPRECATE_RESERVED
SCLottoInfo::SetLottoIndex(DWORD lottery_index) {
    if(FlowControl::FCAssert((lottery_index & USHRT_MAX) == lottery_index))
        const_cast<SCLottoInfo*>(this)->lottery_index_ = WORD(lottery_index);
}

inline void DEPRECATE_RESERVED
SCLottoInfo::SetIdentityStatus(BOOL identity_status) {
    const_cast<SCLottoInfo*>(this)->identity_status_ = !!identity_status;
}

inline void DEPRECATE_RESERVED
SCLottoInfo::SetOpenStatus(BOOL open_status) {
    const_cast<SCLottoInfo*>(this)->open_status_ = !!open_status;
}

inline void DEPRECATE_RESERVED
SCLottoInfo::SetCount(int count) {
    assert(count >= 0 && count <= MAX_PANDORA_SLOT_NUM);
    const_cast<SCLottoInfo*>(this)->number_of_minislots_ = uint8_t(count);
}


//
//==================================================================================================



#endif //_SOLAR_SLOT_SCLOTTOINFO_H
