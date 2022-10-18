#ifndef _SOLAR_SLOT_SCITEMSLOT_INL
    #error "Can't direct include header file"
#endif

//--------------------------------------------------------------------------------------------------
//

 
inline
SCItemSlot::SCItemSlot(const SCItemSlot& r)
{
    // all copy ok (090929) current
    CopyMemory(this, &r, sizeof(*this));
    random_info_.item_info_ = NULL;
    random_info_.Copy(r.random_info_);
}

inline SCItemSlot&
SCItemSlot::operator=(const SCItemSlot& r)
{
    // all copy ok (090929) current
    CopyMemory(this, &r, sizeof(*this));
    random_info_.item_info_ = NULL;
    random_info_.Copy(r.random_info_);
    return *this;
}
/* (CHANGES) (f100520.1L) remove an unused section
inline
SCItemSlot::SCItemSlot(const ITEMSTREAM& item_stream)
{
    Copy(item_stream);
}
*/
inline
SCItemSlot::SCItemSlot(const ITEMSTREAMEX& item_stream)
{
    Copy(item_stream);
}
/* (CHANGES) (f100520.1L) remove an unused section
inline
SCItemSlot::SCItemSlot(const RENDER_ITEMSTREAM& item_stream)
{
    Copy(item_stream);
}
*/
inline
SCItemSlot::SCItemSlot(const RENDER_ITEMSTREAMEX& item_stream)
{
    Copy(item_stream);
}

inline
SCItemSlot::SCItemSlot(const OPT_ITEMSTREAMEX& item_stream)
{
    Copy(item_stream);
}

/* (CHANGES) (f100520.1L) remove an unused section
inline void
SCItemSlot::Copy(const ITEMSTREAM& item_stream)
{   
    CopyPart(item_stream.Part);
}
*/
inline void
SCItemSlot::Copy(const ITEMSTREAMEX& item_stream)
{
    CopyPart(item_stream.Part);
    CopyPart(item_stream.OptionPart);
}
/* (CHANGES) (f100520.1L) remove an unused section
inline void
SCItemSlot::Copy(const RENDER_ITEMSTREAM& IN item_stream)
{
    CopyPart(item_stream.Part);
}
*/
inline void
SCItemSlot::Copy(const RENDER_ITEMSTREAMEX& IN item_stream)
{
    CopyPart(item_stream.Part);
    CopyPart(item_stream.OptionPart);
}

// 2006/5/16 : 추가
inline void
SCItemSlot::Copy(const OPT_ITEMSTREAMEX& IN item_stream)
{
    CopyPart(item_stream.Part);
    CopyPart(item_stream.OptionPart);
}
/* (CHANGES) (f100520.1L) remove an unused section
inline void
SCItemSlot::CopyOut(ITEMSTREAM& OUT item_stream) const
{
    CopyOutPart(item_stream.Part);
}
*/
inline void
SCItemSlot::CopyOut(ITEMSTREAMEX& OUT item_stream) const
{
    CopyOutPart(item_stream.Part);
    CopyOutPart(item_stream.OptionPart);
}

inline void
SCItemSlot::CopyOut(RENDER_ITEMSTREAMEX& OUT item_stream) const
{
    CopyOutPart(item_stream.Part);
    CopyOutPart(item_stream.OptionPart);
}
/* (CHANGES) (f100520.1L) remove an unused section
inline void
SCItemSlot::CopyOut(RENDER_ITEMSTREAM& OUT item_stream) const //< 시리얼+옵션 제외
{
    CopyOutPart(item_stream.Part);
}
*/
// 2005/5/16 : 추가
inline void
SCItemSlot::CopyOut(OPT_ITEMSTREAMEX& OUT item_stream) const
{
    CopyOutPart(item_stream.Part);
    CopyOutPart(item_stream.OptionPart);
}


//--------------------------------------------------------------------------------------------------
// (CHANGES) (091212) (WAVERIX) change light-weight properties to inline functions
// |LIST=(Get/Set)Type, GetWorldSerial, (Get/Set)Code, (Get/Set)SlotType, GetSerial, GetDura, Clear|
//
inline BYTE
SCItemSlot::GetType() const {
    return m_byType;
}

inline void
SCItemSlot::SetType(BYTE type) {
    m_byType = type;
}

inline DBWLDSERIAL
SCItemSlot::GetWorldSerial() const {
    return m_WorldSerial;
}

inline SLOTCODE
SCItemSlot::GetCode() const {
    return item_code_;
}

// NOTE: f110422.3L, add default accessability to easy acquisition
inline SLOTCODE
SCItemSlot::GetItemCode() const {
    return item_code_;
};

inline SLOTCODE
SCItemSlot::GetImageCode() const {
    return image_item_code_;
}

inline void
SCItemSlot::SetSlotType(eSlotType type) {
    SlotType_info_ = type;
}

inline eSlotType
SCItemSlot::GetSlotType() const {
    return SlotType_info_;
}

inline eSlotType
CSVendorItem::GetSlotType() const {
	return ST_VENDORSEARCH;
}


inline DBSERIAL
SCItemSlot::GetSerial() const {
    return m_dwSerial;
}

inline bool
SCItemSlot::WithDura() const
{
    // pseudo
    // if 내구도로 사용하는가?
    //   (true)
    // elif 개수로 사용하는가?
    //   (false)
    if (item_rule_info_.is_valid_item == false) {
        return false;
    }
    // 무기, 방어구
    if (item_rule_info_.is_weapon == false &&
        item_rule_info_.is_armor == false)
    {
        return false;
    }
    // 최초 내구도가 없는 경우... 이것은 스크립트 유효성 검사에서 선결되어야 하는 문제이긴 한데...
    if (item_info_->m_Dura == 0) {
        return false;
    }
    return true;
}

inline BYTE
SCItemSlot::GetDura() const {
    assert(WithDura());
    return duration_;
}

inline void
SCItemSlot::Clear()
{
    SCSlot::Clear();
#ifdef _NA_003740_20111122_RANDOM_ITEM
    random_info_.Clear();
#endif // _NA_003740_20111122_RANDOM_ITEM
    ZeroMemory(item_slot_point_holder_start_,                      // end
               static_cast<size_t>(&item_slot_point_holder_last_[2] - item_slot_point_holder_start_));
}

//  (WAVERIX) (CHANGES) support for client, 올바른 아이템 검증을 위해 추가 - no define work
//  (NOTE) 해당 ItemSlot은 어떠한 연산을 수행하기 위해 올바른 정보를 가지고 있는가?
//         예외: empty slot의 경우는 SetCode등을 필두로 IO는 가능하다. 이 함수는 있다는 전제하에
//              처리되는 일련의 연산을 검증하기 위한 수단
//  (returns) false : invalid item | empty item
//            true  : valid item
inline bool
SCItemSlot::IsContainValidItem() const {
    // empty item slot
    if (item_code_ == 0) {
        return false;
    }
    // invalid item (exist code but, can't find info in item script)
    if (item_info_ == NULL || item_info_->m_Code == 0) {
        return false;
    }
    return item_rule_info_.is_valid_item;
}

//inline BASE_ITEMINFO*
//SCItemSlot::GetItemInfo()
//{
//    // (NOTE) item_info_ != &Null_ItemInfo -- 정상
//    //        item_info_ == &Null_ItemInfo -- 비정상
//    if (item_info_ != NULL) {
//        return const_cast<BASE_ITEMINFO*>(item_info_); //부디 외부에서 제어 안되길...
//    }
//    // 비어 있는 슬롯에 대한 연산시 발생하게 될 것이다. 또는 연산 사용 순서를 확인할 필요 있음. 
//    // 초기화된 슬롯에 GetItemInfo()를 호출할 경우에도 발생
//    // (NOTE) 너무 일반화 되어 있는 관계로 assert제거
//    //assert(!"check valid operation: not initialized");
//    return const_cast<BASE_ITEMINFO*>(&Null_ItemInfo);
//}

inline const BASE_ITEMINFO* SCItemSlot::GetItemInfo() const
{
#ifdef _NA_003740_20111122_RANDOM_ITEM
    const BASE_ITEMINFO* const rand_item_info = random_info_.GetItemInfo();
    if (rand_item_info != NULL)
    {
        return rand_item_info;
    }
#endif
    // (NOTE) item_info_ != &Null_ItemInfo -- 정상
    //        item_info_ == &Null_ItemInfo -- 비정상
    if (item_info_ != NULL) {
        return item_info_; //부디 외부에서 제어 안되길...
    }
    // 비어 있는 슬롯에 대한 연산시 발생하게 될 것이다. 또는 연산 사용 순서를 확인할 필요 있음. 
    // 초기화된 슬롯에 GetItemInfo()를 호출할 경우에도 발생
    // (NOTE) 너무 일반화 되어 있는 관계로 assert제거
    //assert(!"check valid operation: not initialized");
    return &Null_ItemInfo;
}

inline const BASE_ITEMINFO*
SCItemSlot::GetImageItemInfo() const
{
    if (image_item_info_ != NULL) {
        return image_item_info_;
    }
    return NULL;
}

inline const nsSlot::ItemRuleInfo&
SCItemSlot::GetItemRuleInfo() const {
    return item_rule_info_;
}

//--------------------------------------------------------------------------------------------------
//  Lock/Block
inline bool
SCItemSlot::IsLocked() const {
    return !!lock_info_.locked_;
}

inline void
SCItemSlot::SetLock(bool val) {
    lock_info_.locked_ = !!val;
}

inline bool
SCItemSlot::IsBlocked() const {
    //return (lock_info_.blocked_ == 1);
    return lock_info_.blocked_ != false;
}

inline void
SCItemSlot::SetBlocked(BOOL val) {
    lock_info_.blocked_ = !!val;
}

// Game issue에 의해 락을 걸어야 하는 경우, ex) 펫/라이더 소환 상태
// same interface 'IsPetLocked()', 'IsRiderLocked()'
inline bool
SCItemSlot::IsUserBlocked() const {
    return lock_info_.user_blocked_ != false;
}

inline void
SCItemSlot::SetUserBlocked(BOOL val) {
    lock_info_.user_blocked_ = !!val;
}

// f100909.1L
inline void SCItemSlot::SetOptionTag(const KindOfItemTag& tag) {
    lock_info_.item_tag_ = tag;
};

inline const KindOfItemTag& SCItemSlot::GetOptionTag() const {
    return lock_info_.item_tag_;
};

// f100910.1L, (for inheritable tag option) related "f100909.1L' and "(Get/Set)OptionTag()"
// WARNING: this method is not support a stat change operation.
// NOTE: usage case = { make a clone item, making item from a lottery item, and that. }
// added by issues of _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
inline bool SCItemSlot::CopyFromInheritableOption(const SCItemSlot& src)
{
    lock_info_.item_tag_ = src.lock_info_.item_tag_;
    return true;
}

//--------------------------------------------------------------------------------------------------
// 개수
inline DURATYPE
SCItemSlot::GetMaxNum() const {
    if (item_info_) {
        return item_info_->m_byDupNum;
    }
    assert(0);
    return 0;
}

inline bool
SCItemSlot::IsOverlap() const
{
    if (item_rule_info_.is_can_overlap) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------------------
// 공유 정보를 재 정의해서 사용할 수 있는가? (아이템 타입별 설정 가부가 존재한다.)
inline bool
SCItemSlot::_CanReuseSharedBlock() const {
    return item_rule_info_.is_valid_item && item_rule_info_.is_use_shared == false;
}

//--------------------------------------------------------------------------------------------------
// Divine Option (old comment) Set 정보
// same interface 'GetSet'
inline bool
SCItemSlot::IsDivine() const {
    //return (m_Set == 1); // set자체 1bit사용한다.
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Divine) {
        return !!equip_info_.is_divine_;
    }
    return 0;
}

inline void
SCItemSlot::SetSet(BYTE set)
{
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Divine) {
        // (WAVERIX) (090930) (NOTE) 현 시점 논리 판단 상, 디바인 설정은 lucky제외 모두 설정
        // 얻을 때는 accessory제외? -> 일단 디바인은 accessory계열은 없다고 확인을 받은 사항이긴 하다.
        equip_info_.is_divine_ = !!set;
    }
}

inline bool
SCItemSlot::GetSet() const
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Divine) {
        return !!equip_info_.is_divine_;
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------
//  Enchant Option

inline BYTE
SCItemSlot::GetEnchant() const
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Enchant) {
        return equip_info_.enchant_grade_;
    }
    return 0;
}

inline void
SCItemSlot::SetEnchant(BYTE enchant)
{
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Enchant) {
        if (enchant <= MAX_ENCHANT_GRADE) {
            equip_info_.enchant_grade_ = enchant;
        }
    }

    // logging?
}

inline eENCHANTLEVEL
SCItemSlot::GetEnchantLV() const
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Enchant) {
        switch(equip_info_.enchant_grade_)
        {
        case 0: case 1: case 2:
        case 3: case 4: case 5:
        case 6:
            return ENCHANT_LOW;
        case 7: case 8: case 9:
            return ENCHANT_MIDDLE;
        case 10: case 11: case 12:
            return ENCHANT_HIGH;
#ifdef _NA_003966_20111227_ADD_ENCHANT
        case 13: case 14: case 15:
            return ENCHANT_HIGHEST;
#endif
        }
    }
    return ENCHANT_LOW;
}

//(lst1024)(080918)(2447)인챈트시 내구도를 다시 셋팅 해준다.
inline void
SCItemSlot::SetEnchantUpdateDura(BYTE enchant) {
    if (item_rule_info_.is_editable == false) {
        return;
    }
    SetEnchant(enchant);
    SetDura(GetMaxDura());
}

//--------------------------------------------------------------------------------------------------
// Awakening Option
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
inline void SCItemSlot::SetAwakening( BYTE awakening )
{
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Awakening) {
        if (awakening <= MAX_AWAKENING_GRADE) {
            equip_info_.awakening_grade_ = awakening;
        }
    }
}

inline BYTE SCItemSlot::GetAwakening() const
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Awakening) {
        return equip_info_.awakening_grade_;
    }
    return 0;
}

inline bool SCItemSlot::EnableAwakening() const
{
    if (item_rule_info_.IsEnableAwakening() == false)
    {
        return false;
    }

    if (item_info_ == NULL)
    {
        return false;
    }

    return true;
}

#endif // _NA_008124_20150313_AWAKENING_SYSTEM
//--------------------------------------------------------------------------------------------------
// Rank Option
inline eRANK
SCItemSlot::GetRank() const
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank) {
        if (item_rule_info_.is_weapon || item_rule_info_.is_armor) {
            return static_cast<eRANK>(rank_info_.current_rank_);
        }
        else if (item_rule_info_.is_accessory) {
            return static_cast<eRANK>(rank_info_.current_rank_);
        }
        // special accessory는?
    }
    return RANK_E;
}

inline int
SCItemSlot::GetRankAttrIndex(eRANK rank) const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank) {
        return rank_info_.GetRankAttrBlock(rank).attr_index_;
    }
    // 여기 좀 더 확인할 것.
    return 0;
}

inline eATTR_TYPE
SCItemSlot::GetRankAttr(eRANK rank) const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank) {
        return rank_info_.GetRankAttrBlock(rank).attr_type_;
    }
    return eATTR_TYPE_INVALID;
}

inline int
SCItemSlot::GetRankAttrValue(eRANK rank) const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank) {
        return rank_info_.GetRankAttrBlock(rank).attr_value_;
    }
    return 0;
}

inline const sRANK_OPTION*
SCItemSlot::GetRankAttrDesc(eRANK rank) const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank) {
        return rank_info_.GetRankAttrBlock(rank).rank_option_;
    }
    return 0;
}

#if DEPRECATE_CODE_DELETE_RESERVED //<- delete reserved section
// 랭크 옵션 정보
DEPRECATE_RESERVED inline void
SCItemSlot::SetRank(eRANK rank) {
    // _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    if (item_rule_info_.is_editable == false ||
        lock_info_.history_.value == lock_info_.history_.Combined)
    {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank)
    {
        if (RANK_E <= rank && rank < RANK_MAX) {
            rank_info_.current_rank_ = static_cast<BYTE>(rank);
        }
        return;
    }
    assert(0);
}

//void SetRankAttr(eRANK rank, int iAttrIndex, int iValue) {
//rank_info_.SetRankAttr(rank,iAttrIndex,iValue); }

DEPRECATE_RESERVED inline void
SCItemSlot::SetRankAttr(eRANK rank, const sRANK_OPTION* rank_option) {
    // _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    if (item_rule_info_.is_editable == false ||
        lock_info_.history_.value == lock_info_.history_.Combined)
    {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank) {
        rank_info_.SetRankAttr(rank, rank_option);
        return;
    }
    assert(0);
}

DEPRECATE_RESERVED inline void
SCItemSlot::DelRankAttr(eRANK rank) {
    // _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    if (item_rule_info_.is_editable == false ||
        lock_info_.history_.value == lock_info_.history_.Combined)
    {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank) {
        rank_info_.DelRankAttr(rank);
        return;
    }
    assert(0);
}

#endif //WAVERIX_DELETE_RESERVED<- delete reserved section


//--------------------------------------------------------------------------------------------------
//  Fate Option

inline bool
SCItemSlot::IsFateItem() const
{
    if (item_rule_info_.is_fate) {
        return true;
    }
    return false;
}


inline WORD
SCItemSlot::GetFateAtkDefRatio() const {
    if (item_rule_info_.is_fate) {
        return shared_.fate_info_.ratio_;
    }
    assert(0);
    return 0;
}

inline bool
SCItemSlot::GetFateIdentify() const {
    if (item_rule_info_.is_fate) {
        return (shared_.fate_info_.identify_ != false);
    }
    assert(0);
    return 0;
}

inline DURATYPE
SCItemSlot::GetFateDuraMax() const {
    if (item_rule_info_.is_fate) {
        return shared_.fate_info_.max_duration_;
    }
    assert(0);
    return 0;
}

inline WORD
SCItemSlot::GetFatePeriod() const {
    if (item_rule_info_.is_fate) {
        return shared_.fate_info_.period_;
    }
    assert(0);
    return 0;
}

//inline void
//SCItemSlot::SetFateAtkDefRatio(WORD value);
inline void
SCItemSlot::SetFateIdentify(BOOL identify) {
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.is_fate) {
        shared_.fate_info_.identify_ = !!identify;
        return;
    }
    assert(0);
}
//inline void
//SCItemSlot::SetFateDuraMax(WORD value);
inline void
SCItemSlot::SetFatePeriod(WORD period) {
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.is_fate) {
        shared_.fate_info_.period_ = period;
        return;
    }
    assert(0);
}

//----------------------------------------------------------------------------------------------
// Socket Option information
inline BYTE
SCItemSlot::GetSocketNum() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        return socket_info_.number_of_sockets_;
    }
    //assert(0); // (NOTE) 기존에 워낙 그냥 호출하는 사례들이 많아서 assert제거
    return 0;
}

inline void DEPRECATE_RESERVED
SCItemSlot::SetSocketNum(BYTE num) {
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        if (num <= SOCKET_MAX) {
            socket_info_.number_of_sockets_ = num;
            return;
        }
    }
    assert(0);
}

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
inline eSOCKET_IDENTIFY_TYPE SCItemSlot::GetSocketIdentifyType() const
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) 
    {
        return static_cast<eSOCKET_IDENTIFY_TYPE>(socket_info_.socket_identify_type_);
    }
    return eSOCKET_IDENTIFY_NOT_PROCESS;
}

inline void SCItemSlot::SetSocketIdentifyType(const eSOCKET_IDENTIFY_TYPE socket_identify_type)
{
    if (item_rule_info_.is_editable == false) 
    {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) 
    {
        socket_info_.socket_identify_type_ = static_cast<BYTE>(socket_identify_type);
        return;
    }
    assert(false);
}

inline void
SCItemSlot::SetSocketOption(eSOCKET sock, SOCKETINDEX index) 
{
    if (item_rule_info_.is_editable == false) 
        return;

    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) 
    {
        socket_info_.SetSocketOption(sock, index);
        return;
    }
    assert(0);
}

inline const sSOCKET_OPTION*
SCItemSlot::GetSocketOption(eSOCKET sock) const
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        return socket_info_.GetSocketOption(sock);
    }
    //assert(0);
    return NULL;
}

inline void
SCItemSlot::CopySocketInfo(const SCSocketOptionInfo& sock_info)
{
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        socket_info_.CopySocketInfo(sock_info);
        return;
    }
    assert(0);
    return;
}

inline const SCSocketOptionInfo&
SCItemSlot::GetSocketInfo()
{
    if (!(item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket)) {
        assert(0); //
    }
    return socket_info_;
}
#else // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
inline void
SCItemSlot::SetSocketAttr(eSOCKET sock, eSOCKET_LEVEL level, const sSOCKET_OPTION* socket_option) {
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        socket_info_.SetSocketAttr(sock, level , socket_option);
        return;
    }
    assert(0);
}

inline eATTR_TYPE
SCItemSlot::GetSocketAttr(eSOCKET sock) const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        return socket_info_.GetSocketAttrBlock(sock).attr_type_;
    }
    //assert(0);
    return eATTR_TYPE_INVALID;
}

inline int
SCItemSlot::GetSocketAttrValue(eSOCKET sock) const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        return socket_info_.GetSocketAttrBlock(sock).attr_value_;
    }
    assert(0);
    return 0;
}

inline eSOCKET_LEVEL
SCItemSlot::GetSocketAttrLevel(eSOCKET sock) const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        return socket_info_.GetSocketAttrBlock(sock).socket_level_;
    }
    assert(0);
    return SOCKETLV_LOW;
}

inline int
SCItemSlot::GetSocketAttrIndex(eSOCKET sock) const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        return socket_info_.GetSocketAttrBlock(sock).attr_index_;
    }
    assert(0);
    return 0;
}

inline const sSOCKET_OPTION*
SCItemSlot::GetSocketAttrDesc(eSOCKET sock) const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        return socket_info_.GetSocketAttrBlock(sock).socket_option_;
    }
    //assert(0);
    return 0;
}

inline void
SCItemSlot::CopySocketInfo(const SCSocketInfo& sock_info)
{
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) {
        socket_info_.CopySocketInfo(sock_info);
        return;
    }
    assert(0);
    return;
}

inline const SCSocketInfo&
SCItemSlot::GetSocketInfo() {
    if (!(item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket)) {
        assert(0); //
    }
    return socket_info_;
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM

//inline bool
//SCItemSlot::ExistEmptySocket(eSOCKET& OUT EmptySocket) const;

//--------------------------------------------------------------------------------------------------
//  N Option - (ExtraStone Option)

inline eATTR_TYPE
SCItemSlot::GetNOptionAttr() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_NOption) {
        return n_option_info_.attr_type_;
    }
    assert(0);
    return eATTR_TYPE_INVALID;
}

inline int
SCItemSlot::GetNOptionAttrValue() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_NOption) {
        return n_option_info_.attr_value_;
    }
    assert(0);
    return 0;
}

inline int
SCItemSlot::GetNOptionAttrIndex() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_NOption) {
        return n_option_info_.attr_index_;
    }
    assert(0);
    return 0;
}

inline const sEXTRASTONE_OPT_INFO*
SCItemSlot::GetNOptionAttrDesc() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_NOption) {
        return n_option_info_.n_option_;
    }
    assert(0);
    return 0;
}

inline int
SCItemSlot::GetNOptionValueIndex() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_NOption) {
        return n_option_info_.attr_value_index_;
    }
    assert(0);
    return 0;
}

//--------------------------------------------------------------------------------------------------
//  Lottery Info
inline bool
SCItemSlot::IsLottoItem() const {
    if (item_rule_info_.is_lottery) {
        assert(item_info_->m_wType == eITEMTYPE_LOTTO);
        return true;
    }
    return false;
}

inline bool
SCItemSlot::IsPandoraOpen() const {
    if (item_rule_info_.is_lottery) {
        return shared_.lotto_info_.open_status_ != false;
    }
    assert(0);
    return false;
}

inline SCLottoInfo*
SCItemSlot::GetLottoInfo() const {
    if (item_rule_info_.is_lottery) {
        return const_cast<SCLottoInfo*>(&shared_.lotto_info_);
    }
    assert(0);
    return 0; // new add-in
}

//--------------------------------------------------------------------------------------------------
//  PetInfo
inline bool
SCItemSlot::IsPetItem() const {
    if (item_rule_info_.is_pet) {
        assert(item_info_->m_wType == eITEMTYPE_PET);
        return true;
    }
    return false;
}

inline bool
SCItemSlot::IsPetLocked() const {
    //return (lock_info_.user_blocked_ == 1); // (NOTE) 사례가 좋지 못하다.
    if (item_rule_info_.is_pet) {
        return lock_info_.user_blocked_ != false;
    }
    assert(0);
    return false;
}

inline /*const*/ SCPetInfo*
SCItemSlot::GetPetInfo() const {
    if (item_rule_info_.is_pet) {
        return const_cast<SCPetInfo*>(&shared_.pet_info_);
    }
    assert(0);
    return 0;
}

//----------------------------------------------------------------------------------------------
// Coordinate Option : 좌표

inline bool
SCItemSlot::IsCoordinateItem() const {
    return item_rule_info_.is_coord;
}

inline WORD
SCItemSlot::GetMapCode() const {
    if (item_rule_info_.is_coord) {
        return shared_.coordinate_info_.map_code_;
    }
    assert(0);
    return 0;
}

inline short
SCItemSlot::GetX() const {
    if (item_rule_info_.is_coord) {
        return shared_.coordinate_info_.x_;
    }
    assert(0);
    return 0;
}

inline short
SCItemSlot::GetY() const {
    if (item_rule_info_.is_coord) {
        return shared_.coordinate_info_.y_;
    }
    assert(0);
    return 0;
}

inline short
SCItemSlot::GetZ() const {
    if (item_rule_info_.is_coord) {
        return shared_.coordinate_info_.z_;
    }
    assert(0);
    return 0;
}

inline void
SCItemSlot::SetMapCode(WORD v) {
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.is_coord) {
        shared_.coordinate_info_.map_code_ = v;
        return;
    }
    assert(0);
}

inline void
SCItemSlot::SetX(short v) 
{
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.is_coord) {
        shared_.coordinate_info_.x_ = v;
        return;
    }
    assert(0);
}

inline void
SCItemSlot::SetY(short v) 
{
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.is_coord) {
        shared_.coordinate_info_.y_ = v;
        return;
    }
    assert(0);
}

inline void
SCItemSlot::SetZ(short v) 
{
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.is_coord) {
        shared_.coordinate_info_.z_ = v;
        return;
    }
    assert(0);
}


//--------------------------------------------------------------------------------------------------
// Limited Item Option

inline bool
SCItemSlot::IsLimited() const
{
    return equip_info_.is_limited_ != false;
}

inline WORD
SCItemSlot::GetLimitedIndex() {
    if (item_rule_info_.is_limited) {
        return shared_.limited_info_.limited_index_;
    }
    assert(0);
    return 0;
}

inline int
SCItemSlot::GetDamageRatio() const {
    // (NOTE) limited == armor : change name damage -> defense
    if (item_rule_info_.is_limited) {
        return shared_.limited_info_.defence_ratio_;
    }
    assert(0);
    return 0;
}

inline int
SCItemSlot::GetLimitValueRatio() const {
    if (item_rule_info_.is_limited) {
        return shared_.limited_info_.limit_value_ratio_;
    }
    assert(0);
    return 0;
}

inline DURATYPE
SCItemSlot::GetLimitedDuraMax() const {
    if (item_rule_info_.is_limited) {
        return shared_.limited_info_.max_duration_;
    }
    assert(0);
    return 0;
}

inline DURATYPE
SCItemSlot::GetLimitedDuraMMax() const {
    if (item_rule_info_.is_limited) {
        return shared_.limited_info_.max_duration_for_restore_;
    }
    assert(0);
    return 0;
}

inline BYTE
SCItemSlot::GetLimitedRepairCount() const {
    if (item_rule_info_.is_limited) {
        return shared_.limited_info_.number_of_repairs_;
    }
    assert(0);
    return 0;
}

//--------------------------------------------------------------------------------------------------
// Limited Control
inline void
SCItemSlot::SetDamageRatio(int value)
{
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.is_limited == false) {
        assert(0);
        return;
    }
    // (WAVERIX) (NOTE) 이것을 보니까... 각 옵션의 상한/하한값을 상수값으로 만들 필요가 있을 듯.
    if (value < -100 || 400 < value) {
        return;
    }
    // (NOTE) limited == armor : change name damage -> defense
    shared_.limited_info_.defence_ratio_ = value;
}

//----------------------------------------------------------------------------------------------
// (ISlotUpdater)
inline void
SCItemSlot::RegisterISlotUpdater(ISlotUpdater* pISlotUpdater) {
    slot_updater_ = ISlotUpdaterD(pISlotUpdater);
}

inline void
SCItemSlot::UnRegisterISlotUpdater() {
    slot_updater_ = ISlotUpdaterD(NULL);
}

//----------------------------------------------------------------------------------------------
//  Ether Info
inline bool
SCItemSlot::IsEquipEtherDevice() const {
    if (item_rule_info_.is_valid_item == false ||
        item_rule_info_.is_shell) // CHANGES: f101021.1L, passed an etheria item
    {
        return false;
    }
    if (item_rule_info_.is_weapon) {
        return (equip_info_.is_equip_ether_device_ != false);
    }
    return false;
}

inline void
SCItemSlot::SetEquipEtherDevice(BOOL bValue) {
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.is_weapon) {
        equip_info_.is_equip_ether_device_ = !!bValue;
        return;
    }
    assert(0);
    return;
}

inline BYTE
SCItemSlot::GetEtherBulletIndex() const {
    if (item_rule_info_.is_weapon) {
        return equip_info_.ether_bullet_index_;
    }
    assert(0);
    return 0;
}

inline void
SCItemSlot::SetEtherBulletIndex(BYTE byIndex) {
    if (item_rule_info_.is_weapon) {
        equip_info_.ether_bullet_index_ = byIndex;
        return;
    }
    assert(0);
    return;
}

inline SCLaxLock<BYTE>*
SCItemSlot::GetLaxLock() {
    return &m_LaxLock;
}

//----------------------------------------------------------------------------------------------
//  Date|Time Control : 날짜
inline bool
SCItemSlot::IsProgressTime() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer) {
        return (time_info_.is_progress_ != false);
    }
    // NOTE: f110426.4L, removes assertion because of too many direct used cases
    //assert(0);
    return false;
}

inline void
SCItemSlot::SetProgressTime(bool bFlag) {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer) {
        time_info_.is_progress_ = !!bFlag;
        return;
    }
    assert(0);
}

inline DWORD64
SCItemSlot::GetDateTime() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer) {
        return time_info_.date_time_;
    };
    assert(0);
    return 0;
}

inline bool
SCItemSlot::IsExpiredWaitTime() const {
     if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer) {
        return (time_info_.is_expired_wait_ != false);
     };
     return false;
}

inline void
SCItemSlot::SetExpiredWaitTime(bool bFlag) {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer) {
        time_info_.is_expired_wait_ = !!bFlag;
        return;
    }
    assert(0);
}

inline bool
SCItemSlot::IsExpiredTime() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer) {
        return (time_info_.is_expired_ != false);
    };
    return false;
}

inline void
SCItemSlot::SetExpiredTime(bool bFlag) {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer) {
        time_info_.is_expired_ = !!bFlag;
        return;
    }
    assert(0);
}

//--------------------------------------------------------------------------------------------------
//  WayPointInfo

// for GM command
inline void
SCItemSlot::ClearWayPointItem() {
    if (item_rule_info_.is_waypoint) {
        shared_.waypoint_info_.Clear();
        return;
    }
    assert(0);
}

inline bool
SCItemSlot::IsWayPointItem() const {
    if (item_rule_info_.is_waypoint) {
        assert(item_info_->m_wType == eITEMTYPE_WAYPOINT);
        return true;
    }
    return false;
}

inline bool
SCItemSlot::CanWayPointDelete() const {
    if (item_rule_info_.is_waypoint) {
        return (shared_.waypoint_info_.delete_enabled_ != false);
    };
    assert(0);
    return false;
}

inline void
SCItemSlot::ResetWayPointDelete() {
    if (item_rule_info_.is_waypoint) {
        shared_.waypoint_info_.ResetWayPointDelete();
        return;
    }
    assert(0);
}



//--------------------------------------------------------------------------------------------------
//  PossesionInfo
// implemented by __NA_1247_POSSESSION_ITEM
// (WARNING) 해당 아이템이 귀속 가능한지의 여부이며, 아이템의 귀속 여부가 아님.
inline bool
SCItemSlot::IsPossessionItem() const
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Possession) {
        return true;
    }
    return false;
}

inline void
SCItemSlot::SetIdentifyPossession(BOOLEAN value) {
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Possession) {
        equip_info_.is_identify_possession_ = !!value;
        return;
    }
    assert(0);
}

inline bool
SCItemSlot::GetIdentifyPossession() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Possession) {
        return (equip_info_.is_identify_possession_ != false);
    }
    return false;
}

// __NA_0_POSSESSION_ITEM_DROP = {
inline void
SCItemSlot::SetDestroyItem(BOOL value) {
    if (item_rule_info_.is_editable == false) {
        return;
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Possession) {
        equip_info_.is_destroyed_ = !!value;
        return;
    }
    assert(0);
}

inline bool
SCItemSlot::IsDestroyItem() const {
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Possession) {
        return (equip_info_.is_destroyed_ != false);
    };
    return false;
}
// };

//--------------------------------------------------------------------------------------------------
inline bool SCItemSlot::IsWingItem() const
{
    if ((item_rule_info_.is_accessory_wings) &&
        (item_rule_info_.is_accessory_special))
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------------------
//  Rider & Rider Parts Info
inline bool
SCItemSlot::IsRiderItem() const {
    if (item_rule_info_.is_rider) {
        assert(item_info_->m_wType == eITEMTYPE_RIDER);
        return true;
    }
    return false;
}

inline bool
SCItemSlot::IsRiderSummoned() const { // is rider summoned? UserBlock(t/f)
    if (item_rule_info_.is_rider) {
        return lock_info_.user_blocked_ != false;
    };
    assert(0);
    return false;
}

//inline void
//SCItemSlot::RiderSummonStatus(BOOLEAN is_summoned) {
//    if (item_rule_info_.is_rider) {
//        lock_info_.user_blocked_ = !!is_summoned;
//        return;
//    }
//    assert(0);
//}

inline nsSlot::RiderUpdateBits
SCItemSlot::GetRiderAttrChangedInfo() const {
    if (item_rule_info_.is_rider) {
        return shared_.rider_info_.GetLatestChangedInfo();
    }

    assert(0);
    nsSlot::RiderUpdateBits bits = { 0, };
    return bits;
}

// (f100727.1L) Shared interface for the Etheria control
inline eEtheriaHistory SCItemSlot::GetEtheriaHistory() const
{
    return lock_info_.history_;
}

// (CHANGES) (f100510.1L) redirected interfaces to support the contents of an etheria
inline /*static*/ bool SCItemSlot::CanEquip(const BASE_ITEMINFO* item_info)
{
    if (item_info->m_RidingClass.value != item_info->m_RidingClass.None) {
        return false;
    }

    const eITEM_MATERIAL_TYPE material_type = \
        static_cast<eITEM_MATERIAL_TYPE>(item_info->m_byMaterialType);
    return material_type == eITEM_MATERIAL_TYPE_CAN_EQUIP ||
           material_type == eITEM_MATERIAL_TYPE_CAN_BOTH;
}

// (CHANGES) (f100510.1L) redirected interfaces to support the contents of an etheria
inline /*static*/ bool
SCItemSlot::CanUseMixture(const BASE_ITEMINFO* item_info, eMIXTURE_FLAG flag)
{
    if (item_info->m_Code == 0) {
        return false;
    }
    // (WAVERIX) (NOTE) 꽤나 혼란스러운 설정 값이다. 0과 255의 역할이 역전된 느낌.
    // 0x00 : all accept
    // 0xFF : disable all
    // [0x00, 0xFF) : enable_function
    const int mixture = item_info->m_byMixture;
    if (mixture == 0) {
        return true;
    }
    return mixture != MIXTURE_EXCEPTALL ? !!(mixture & flag)
                                        : false;
}

// (CHANGES) (f100510.1L)                              whether the 'is_blocked' value is blocked
inline bool
SCItemSlot::CanTradeSell(const BASE_ITEMINFO* item_info, eITEM_TRADESELL_TYPE is_blocked)
{
    if (item_info->m_Code == 0) {
        return false;
    }
#if !defined(_SERVER)
    if (is_blocked == eITEM_TRADESELL_DOALL) {
        return item_info->trade_sell_type == eITEM_TRADESELL_DOALL;
    }
    if (is_blocked == eITEM_TRADESELL_DONTALL) {
        return item_info->trade_sell_type == eITEM_TRADESELL_DONTALL;
    }
#endif
    if (item_info->trade_sell_type & is_blocked) {
        return false;
    }
    return true;
}

inline bool SCItemSlot::IsEtheriaItem() const {
    //assert(lock_info_.history_.value == lock_info_.history_.Etheria);
    return item_rule_info_.is_etheria != false;
}

inline bool SCItemSlot::IsShellItem() const {
    //assert(lock_info_.history_.value == lock_info_.history_.Shell);
    return item_rule_info_.is_shell != false;
}

//--------------------------------------------------------------------------------------------------
//  DefaultEtc Info

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
inline WORD SCItemSlot::GetExtendData() const 
{
    if (item_rule_info_.is_extend_data) 
    {
        return shared_.default_ect_info_.data_;
    }
    assert(false);
    return 0;
}

inline void SCItemSlot::SetExtendData(WORD v) 
{
    if (item_rule_info_.is_extend_data == false) 
    {
        return;
    }
    
    if (item_rule_info_.is_extend_data) 
    {
        shared_.default_ect_info_.data_ = v;
        return;
    }
    
    assert(false);
}

inline bool SCItemSlot::IsZardItem() const 
{
    if (item_rule_info_.is_extend_data) 
    {
        assert(item_info_->m_wType == eITEMTYPE_SOCKET_ZARD);
        return true;
    }
    return false;
}

inline bool SCItemSlot::IsZardAdditiveItem() const
{
    if (item_info_ != NULL)
    {
        return item_info_->m_wType == eITEMTYPE_SOCKET_COMPOSE_ADDITIVE;
    }
    return false;
}
#endif // _NA_0_20100727_SOCKET_SYSTEM_CONVERT