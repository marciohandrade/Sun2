#include "Stdafx.h"
#include "./SCRiderInfo.h"
#include "./SCItemSlot.h"

#include <time.h>
#include <SolarDateTime.h>
#include <ItemInfoParser.h>

//==================================================================================================
// __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__


// offset�� �� ������ ���ȴ�.
DWORD
SCRiderPartsInfo::_ConvertToDateOffset(DWORD64 base_date_time, DWORD64 parts_date_time)
{
    //              Rider Date(20091013235959)
    // ||----------|------------|------------|--------------||
    // [-30��]   Parts1                    Part2          [+30��]
    //           -10��                     +10��
    // Parts�� DateOffset�� �� ������ ����Ѵ�. (��ȣ ����)
    // ����Ǹ� Wait�� �����Ǿ� update���� �ʴ´�. ���� �� ���� 30�� �̻� ���̳� �� �־
    // DateOffset�� �������� �ʴ´�.

    // �� �������� �����ϸ� ���̰� ���� ������ ���Ѵ�.
    if((DWORD64)(base_date_time / 100) == (DWORD64)(parts_date_time / 100))
        return 0;

    time_t base_time_t = 0, offset_time_t = 0;
    {
        struct tm base_tm = { 0, };
        struct tm offset_tm = { 0, };
        SYSTEMTIME sys_time;
        SYSTEMTIME offset_time;
        util::YYYYMMDDHHMMSSToSYSTEMTIME(base_date_time, sys_time);
        util::YYYYMMDDHHMMSSToSYSTEMTIME(parts_date_time, offset_time);
        base_tm.tm_year = sys_time.wYear - 1900;
        base_tm.tm_mon  = sys_time.wMonth - 1;
        base_tm.tm_mday = sys_time.wDay;
        base_tm.tm_hour = sys_time.wHour;
        base_tm.tm_min  = sys_time.wMinute;

        offset_tm.tm_year = offset_time.wYear - 1900;
        offset_tm.tm_mon  = offset_time.wMonth - 1;
        offset_tm.tm_mday = offset_time.wDay;
        offset_tm.tm_hour = offset_time.wHour;
        offset_tm.tm_min  = offset_time.wMinute;

        base_time_t = mktime(&base_tm);
        offset_time_t = mktime(&offset_tm);
    }

    const DWORD MaxPeriod = SCRiderInfo::MaxPeriod;
    // �ð� ��� �����ϴ�...�Ф� �׳� ��/��/�и� �ջ��ϱ�� �Ѵ�.
    //                                   ��                   ��            ��    ��
    const double max_period = (double)MaxPeriod;// + 23 * (60 * 24) + 59 * (60) + 59;
    BOOST_STATIC_ASSERT(MaxPeriod == (MaxPeriod & SCRiderInfo::NumberOfBits_DateOffset_Value));
    DWORD result = 0;

    if(base_date_time < parts_date_time) {
        double result_diff = difftime(offset_time_t, base_time_t) / 60.0f;
        if(max_period < result_diff)
            result_diff = max_period;
        result = (DWORD)result_diff;
    }
    else { //if(base_date_time > parts_date_time)
        double result_diff = difftime(base_time_t, offset_time_t) / 60.0f;
        if(max_period < result_diff)
            result_diff = max_period;
        result = (DWORD)result_diff;
        result |= SCRiderInfo::NumberOfBits_DateOffset_SignBit; // sign bit (-)
    }

    // equal
    return result;
}

DWORD64
SCRiderPartsInfo::_ConvertToDateTime(DWORD64 base_date_time, DWORD parts_date_offset)
{
    const BOOLEAN is_minus = !!(parts_date_offset & SCRiderInfo::NumberOfBits_DateOffset_SignBit);
    int parts_date = parts_date_offset & ~SCRiderInfo::NumberOfBits_DateOffset_SignBit;
    if(!FlowControl::FCAssert(SCRiderInfo::MaxPeriod >= (DWORD)parts_date))
        parts_date = SCRiderInfo::MaxPeriod;

    SYSTEMTIME sys_time;
    util::YYYYMMDDHHMMSSToSYSTEMTIME(base_date_time, sys_time);

    struct tm base_tm = { 0, };
    base_tm.tm_year = sys_time.wYear - 1900;
    base_tm.tm_mon  = sys_time.wMonth - 1;
    base_tm.tm_mday = sys_time.wDay;
    base_tm.tm_hour = sys_time.wHour;
    base_tm.tm_min  = sys_time.wMinute;

    base_tm.tm_min = is_minus ? base_tm.tm_min - parts_date
                              : base_tm.tm_min + parts_date;

    if(mktime(&base_tm) != time_t(-1)) {
        // time_info_.data_time_ ��¥�� ��ü
        DWORD64 date_time = 0;
        util::GetDateTime_YYYYMMDDHHMMSS(base_tm.tm_year + 1900,
                                         base_tm.tm_mon + 1,
                                         base_tm.tm_mday,
                                         base_tm.tm_hour,
                                         base_tm.tm_min,
                                         base_tm.tm_sec,
                                         date_time);
        return date_time;
    }

    return 0;
}

void
SCRiderPartsInfo::SetInfo(nsSlot::SerializeItemOption::sRiderParts* src,
                          const BASE_ITEMINFO* item_info)
{
    Clear();
    //SLOT_BITBOUND_SET(src->Enchant,           SCRiderPartsInfo::NumberOfBits_EnchantGrade);
    SLOT_BITBOUND_SET(src->IsUnlimitedPeriod, SCRiderPartsInfo::NumberOfBits_IsUnlimitedPeriod);
    //SLOT_BITBOUND_SET(src->IsProgress,        SCRiderPartsInfo::NumberOfBits_IsProgress);
    //SLOT_BITBOUND_SET(src->IsExpiredWaitTime, SCRiderPartsInfo::NumberOfBits_IsExpiredWait);

    item_code_ = item_info->m_Code;
    //enchant_grade_ = src->Enchant;
    is_unlimited_period_ = !!src->IsUnlimitedPeriod;
    //is_progress_ = src->IsProgress;
    //is_expired_wait_ = src->IsExpiredWaitTime;
    //date_time_ = src->DateTime;
}

void
SCRiderPartsInfo::GetInfo(nsSlot::SerializeItemOption::sRiderParts* dest)
{
    dest->IsUnlimitedPeriod = !!is_unlimited_period_;
}

//void
//SCRiderPartsInfo::SetInfo(const SCRiderPartsInfo& IN parts_info, const BASE_ITEMINFO* item_info)
//{
//}
//
//void
//SCRiderPartsInfo::GetInfo(SCRiderPartsInfo* dest)
//{
//}

bool
SCRiderPartsInfo::ExtractToPartsInfo(const SCItemSlot& parts_item_slot, SCRiderPartsInfo* parts_info)
{
    parts_info->Clear();

    if(parts_item_slot.item_rule_info_.is_valid_item == 0 ||
       parts_item_slot.item_rule_info_.is_rider_parts == 0)
    {
        return false;
    }

    parts_info->parts_type_.value    = (BYTE)parts_item_slot.item_info_->m_wEqPos;
    parts_info->is_unlimited_period_ = parts_item_slot.shared_.rider_parts_info_.is_unlimited_period_;

    assert(parts_item_slot.shared_.rider_parts_info_.change_mode_.value ==
           parts_item_slot.shared_.rider_parts_info_.change_mode_.eMode_None);

    parts_info->enchant_grade_  = parts_item_slot.equip_info_.enchant_grade_;
    parts_info->item_code_      = parts_item_slot.item_code_;
    parts_info->item_tag_       = parts_item_slot.lock_info_.item_tag_; // f100910.1L
    parts_info->is_progress_    = parts_item_slot.time_info_.is_progress_;
    parts_info->is_expired_wait_= parts_item_slot.time_info_.is_expired_wait_;
    parts_info->date_time_      = parts_item_slot.time_info_.date_time_;

    parts_info->item_info_      = parts_item_slot.item_info_;

    return true;
}

bool
SCRiderPartsInfo::ExtractToItem(const SCRiderPartsInfo& parts_info, SCItemSlot* parts_item_slot)
{
    parts_item_slot->Clear();

    if(parts_info.item_code_ == 0 || parts_info.item_info_ == 0)
        return false;

    const BASE_ITEMINFO* item_info = parts_info.item_info_;

    if(parts_info.item_code_ != item_info->m_Code)
        return false;

    if(item_info->IsRiderPartsItem() == 0)
        return false;

    parts_item_slot->SetCode(parts_info.item_code_);
    if(parts_item_slot->item_info_ == 0 ||
       parts_item_slot->item_info_ != item_info ||
       parts_item_slot->item_rule_info_.is_rider_parts == 0)
    {
        parts_item_slot->Clear();
        return false; //Ŭ���� ���θ� Ȯ������.
    }

    // need verfication

    // no lock, ������ �ø��� Ű�� �߱��ϱ� ���ؼ��� �������� ���� ���� �� �ʿ� �ִ�.

    parts_item_slot->equip_info_.enchant_grade_ = parts_info.enchant_grade_;
    parts_item_slot->lock_info_.item_tag_ = parts_info.item_tag_; // f100910.1L
    parts_item_slot->shared_.rider_parts_info_.is_unlimited_period_ = parts_info.is_unlimited_period_;
    parts_item_slot->time_info_.is_progress_ = !!parts_info.is_progress_;
    parts_item_slot->time_info_.is_expired_wait_ = !!parts_info.is_expired_wait_;
    if(parts_info.is_progress_)
        parts_item_slot->time_info_.date_time_ = parts_info.date_time_;

    return true;
}



//==================================================================================================

void
SCRiderInfo::SetInfo(nsSlot::SerializeItemOption::sRider* src, const BASE_ITEMINFO* item_info)
{
    Clear();

    BOOST_STATIC_ASSERT(eRIDER_PARTS_POS::HeadPart - eRIDER_PARTS_POS::StartIndex == 0 &&
                        eRIDER_PARTS_POS::HeadPart + 2 == eRIDER_PARTS_POS::LegsPart);

    SLOT_BITBOUND_SET(src->IsUnlimitedPeriod, NumberOfBits_IsUnlimitedPeriod);
    this->is_unlimited_period_ = !!src->IsUnlimitedPeriod;
    if(item_info->m_ChargeSubType == eCHARGE_SUB_RIDER_WITH_PART) {
        assert(this->is_unlimited_period_ == 0);
        this->is_unlimited_period_ = 0;
    }

    ItemInfoParser* const pItemInfoParser = ItemInfoParser::Instance();

    for(int i = 0; i < eRIDER_PARTS_POS::Counts; ++i) {
        nsSlot::SerializeItemOption::sRider::sPart* src_part = &src->Parts[i];
        if(src_part->ItemCode == 0)
            continue;

        SLOT_BITBOUND_SET(src_part->ItemCode,         SCRiderPartsInfo::NumberOfBits_ItemCode);
        SLOT_BITBOUND_SET(src_part->EnchantGrade,     SCRiderPartsInfo::NumberOfBits_EnchantGrade);
        SLOT_BITBOUND_SET(src_part->IsUnlimitedPeriod, SCRiderPartsInfo::NumberOfBits_IsUnlimitedPeriod);
        SLOT_BITBOUND_SET(src_part->IsProgress,       SCRiderPartsInfo::NumberOfBits_IsProgress);
        SLOT_BITBOUND_SET(src_part->IsExpiredWaitTime, SCRiderPartsInfo::NumberOfBits_IsExpiredWait);
        SLOT_BITBOUND_SET(src_part->DateOffset,       NumberOfBits_DateOffset);

        SCRiderPartsInfo& dest_part = rider_parts_[i];
        dest_part.parts_type_.value = (BYTE)(i + eRIDER_PARTS_POS::StartIndex);
        dest_part.item_code_ = (SLOTCODE)src_part->ItemCode;
        dest_part.enchant_grade_ = (BYTE)src_part->EnchantGrade;
        dest_part.is_unlimited_period_ = !!src_part->IsUnlimitedPeriod;
        dest_part.is_progress_ = !!src_part->IsProgress;
        dest_part.is_expired_wait_ = !!src_part->IsExpiredWaitTime;
        if(dest_part.is_progress_) {
            dest_part.date_time_ =
                SCRiderPartsInfo::_ConvertToDateTime(src->DateTime, src_part->DateOffset);
        }

        const BASE_ITEMINFO* item_info = pItemInfoParser->GetItemInfo(dest_part.item_code_);
        ASSERT(item_info != 0);
        dest_part.item_info_ = item_info;

    } //end 'for'
}

void
SCRiderInfo::GetInfo(nsSlot::SerializeItemOption::sRider* dest, nsSlot::SCTimeInfo* base_time_info)
{
    SLOT_BITBOUND_SET(this->is_unlimited_period_, NumberOfBits_IsUnlimitedPeriod);
    dest->IsUnlimitedPeriod = !!this->is_unlimited_period_;

    for(int i = 0; i < eRIDER_PARTS_POS::Counts; ++i) {
        SCRiderPartsInfo* src_part = &rider_parts_[i];
        if(src_part->item_code_ == 0)
            continue;

        SLOT_BITBOUND_SET(src_part->item_code_,       SCRiderPartsInfo::NumberOfBits_ItemCode);
        SLOT_BITBOUND_SET(src_part->enchant_grade_,   SCRiderPartsInfo::NumberOfBits_EnchantGrade);
        SLOT_BITBOUND_SET(src_part->is_unlimited_period_, SCRiderPartsInfo::NumberOfBits_IsUnlimitedPeriod);
        SLOT_BITBOUND_SET(src_part->is_progress_,     SCRiderPartsInfo::NumberOfBits_IsProgress);
        SLOT_BITBOUND_SET(src_part->is_expired_wait_, SCRiderPartsInfo::NumberOfBits_IsExpiredWait);

        nsSlot::SerializeItemOption::sRider::sPart& dest_part = dest->Parts[i];

        dest_part.ItemCode = src_part->item_code_;
        dest_part.EnchantGrade = src_part->enchant_grade_;
        dest_part.IsUnlimitedPeriod = !!src_part->is_unlimited_period_;

        if(src_part->is_unlimited_period_ == 0) {
            dest_part.IsProgress = !!src_part->is_progress_;
            dest_part.IsExpiredWaitTime = !!src_part->is_expired_wait_;
            if(dest_part.IsProgress) {
                assert(dest_part.IsExpiredWaitTime == 0);
                dest_part.IsExpiredWaitTime = 0;
            }

            if(dest_part.IsProgress) {
                dest_part.DateOffset = SCRiderPartsInfo::_ConvertToDateOffset(
                    base_time_info->date_time_, src_part->date_time_);
            }
            else {
                dest_part.DateOffset = 0;
            }
        }
    } //end 'for'
}

RC::eITEM_RESULT
SCRiderInfo::CanEquipParts(const SCItemSlot& rider_item_slot,
                           const BASE_ITEMINFO* rider_item_info,
                           eRIDER_PARTS_POS pos,
                           const SCItemSlot& IN parts_item_slot,
                           const BASE_ITEMINFO* parts_item_info) const
{
    if(!FlowControl::FCAssert(eRIDER_PARTS_POS::StartIndex <= pos.value &&
                                                              pos.value < eRIDER_PARTS_POS::EndIndex))
    {
        return RC::RC_ITEM_INVALIDPOS;
    }

    BOOLEAN success = parts_item_info->m_wType == eITEMTYPE_RIDER_PARTS &&
                      pos.value == parts_item_info->m_wEqPos &&
                      rider_item_info->m_RidingClass.value == parts_item_info->m_RidingClass.value &&
                      parts_item_slot.item_info_ == parts_item_info &&
                      this == &rider_item_slot.shared_.rider_info_;

    if(!FlowControl::FCAssert(success != 0))
        return RC::RC_ITEM_INVALIDSTATE;

    BYTE wax_enchant = parts_item_slot.GetEnchant();
    wax_enchant; //<- need boundary check?
    //success = SLOT_BITBOUND_CHECK(parts_info.enchant_grade_,
    //    SCRiderPartsInfo::NumberOfBits_EnchantGrade);
    //if(success == 0)
    //    return RC::RC_ITEM_INVALID_VALUE;

    // �̹� ����� �������̶� ������ ���? NO!

    int parts_index = pos.value - eRIDER_PARTS_POS::StartIndex; // change array index;
    const SCRiderPartsInfo& dest = rider_parts_[parts_index];
    if(dest.item_code_ != 0) // �̹� ������ ������ ����ô� ���� �ϵ��� �Ѵ�. (���⵵ �氨 ����)
        return RC::RC_ITEM_ALREADY_EXIST;

    if(is_unlimited_period_ == 0) {
        // Rider�� �ð� ���� �ִ� �������� ����.
        // message "������ ����� Rider���� ������ �� �����ϴ�."
        if(rider_item_slot.time_info_.is_expired_wait_)
            return RC::RC_ITEM_ALREADY_TIME_EXPIRED;
    }

    const SCRiderPartsInfo& parts_info = parts_item_slot.shared_.rider_parts_info_;
    if(parts_info.is_unlimited_period_ == 0) {
        // Rider parts�� �ð� ���� �ִ� �������� ����.
        // message "������ ����� Rider Parts�� ������ �� �����ϴ�."
        if(parts_item_slot.time_info_.is_expired_wait_)
            return RC::RC_ITEM_ALREADY_TIME_EXPIRED;
    }

    return RC::RC_ITEM_SUCCESS;
}

bool
SCRiderInfo::EquipParts(const SCItemSlot& rider_item_slot,
                        const BASE_ITEMINFO* rider_item_info,
                        eRIDER_PARTS_POS pos,
                        const SCItemSlot& IN parts_item_slot,
                        const BASE_ITEMINFO* parts_item_info)
{
    // (NOTE) Rider <- Parts ������ serial key�� �������.
    //        Rider -> Parts ���� ������ serial key�� ���� ������ ��߱��� �ʿ��ϴ�.

    if(RC::RC_ITEM_SUCCESS !=
       CanEquipParts(rider_item_slot, rider_item_info, pos, parts_item_slot, parts_item_info))
    {
        return false;
    }

    int parts_index = pos.value - eRIDER_PARTS_POS::StartIndex; // change array index;
    SCRiderPartsInfo& dest = rider_parts_[parts_index];

    dest.Clear();

    dest.parts_type_ = pos;

    dest.item_code_ = parts_item_slot.item_code_;
    dest.item_info_ = parts_item_info;
    dest.enchant_grade_ = parts_item_slot.GetEnchant();
    dest.is_unlimited_period_ = parts_item_slot.shared_.rider_parts_info_.is_unlimited_period_;

    // (NOTE) Scenario equip parts
    // ���� ��ų ��, �ش� ������ ������Ų��.
    // but, set option�� ���� ���� �̺�Ʈ�� ���ɼ��� ����� ����. set�� ���� ��� �� �ص�, �߰��Ѵ�.
    // (NOTE) ���� ���� ���� �򰡵� �ʿ��ϴ�.
    update_bits_.value = 0;
    BYTE changed_bits_temp = 1 << parts_index;

    // ���� ���� ���� Parts�� �ʱ�ȭ ���·� �����ϰ�, ���� ���� �ִ� Parts�� ������ �ʿ��ϴ�.
    if(dest.is_unlimited_period_ != 0) {
        dest.date_time_ = parts_item_slot.time_info_.date_time_;
        update_bits_.value |= changed_bits_temp << 2; //sub_parts_option[n]
    }
    else {
        const nsSlot::SCTimeInfo& parts_time_info = parts_item_slot.time_info_;

        assert(parts_time_info.is_expired_wait_ == 0); // check 'CanEquipParts'
        if(parts_time_info.is_expired_wait_) { // �̹� ���� ����� �������̶�� 'abusing?'
            dest.is_progress_     = false;
            dest.is_expired_wait_ = true;
            dest.date_time_       = 0;
        }
        else {
            //������ Ÿ�̸� ���� ���� - ù ���� ����� SCItemSlot���� �����Ѵ�.
            dest.is_progress_     = parts_time_info.is_progress_;
            dest.is_expired_wait_ = parts_time_info.is_expired_wait_;
            // Rider���� �ð� ���� 30�� �̻� ���� �ȵȴ�.
            dest.date_time_       = parts_time_info.date_time_;
            update_bits_.value |= changed_bits_temp << 2; //sub_parts_option[n]
        }
    }

    dest.item_info_ = parts_item_info;

    return true;
}

// (returns) if exist then Aquire 'SCRiderPartsInfo'
//                        { exist | empty (cleared stream), check 'parts_type_' }
//           else     then return 0 { index out of range }
const SCRiderPartsInfo*
SCRiderInfo::GetPartsInfo(const SCItemSlot* rider_item_slot, eRIDER_PARTS_POS pos) const
{
    __UNUSED(rider_item_slot);
    if(!FlowControl::FCAssert(pos.StartIndex <= pos.value &&
                                                pos.value < pos.EndIndex))
    {
        return 0;
    }

    int parts_index = pos.value - pos.StartIndex; // change array index;
    const SCRiderPartsInfo& dest = rider_parts_[parts_index];
    // ...
    return &dest;
}

bool
SCRiderInfo::GetUnEquipChangedBits(const SCItemSlot& IN rider_item_slot,
                                   eRIDER_PARTS_POS pos, nsSlot::RiderUpdateBits* update_bits_ptr)
{
    __UNUSED(rider_item_slot);
    if(!FlowControl::FCAssert(pos.StartIndex <= pos.value &&
                                                pos.value < pos.EndIndex))
    {
        return false;
    }

    int parts_index = pos.value - pos.StartIndex; // change array index;
    SCRiderPartsInfo& dest = rider_parts_[parts_index];
    if(dest.item_code_ == 0)
        return false;

    // (NOTE) �̰����� ��ȯ�Ǵ� 'RiderUpdateBits'�� ���� ���� ����� ������ �� ������ ó���ȴ�.
    // �⸸ ��������� ó���� IPlayerRiderD::Command_ItemSync���� ó���Ǹ�, _UpdateRider�� ���� �ѹ�
    // ó���ȴ�.

    update_bits_.value = 0;
    BYTE changed_bits_temp = 1 << parts_index;
    if(dest.is_expired_wait_ == 0)
        update_bits_.value |= changed_bits_temp << 5; //sub_parts_option[n]
    *update_bits_ptr = update_bits_;

    return true;
}

RC::eITEM_RESULT
SCRiderInfo::UnEquipParts(const SCItemSlot& IN rider_item_slot,
                          eRIDER_PARTS_POS pos,
                          SCItemSlot* OUT parts_item_slot)
{
    __UNUSED(rider_item_slot);
    if(!FlowControl::FCAssert(pos.StartIndex <= pos.value &&
                                                pos.value < pos.EndIndex))
    {
        return RC::RC_ITEM_INVALIDPOS;
    }

    int parts_index = pos.value - pos.StartIndex; // change array index;
    SCRiderPartsInfo& dest = rider_parts_[parts_index];
    if(dest.item_code_ == 0)
        return RC::RC_ITEM_INVALIDPOS;

    update_bits_.value = 0;

    if(SCRiderPartsInfo::ExtractToItem(dest, parts_item_slot) == false)
        return RC::RC_ITEM_INVALIDSTATE;

    // (NOTE) Scenario unequip parts
    // ���� ��ų ��, �ش� ������ ���ҽ�Ų��.
    // but, set option�� ���� ���� �̺�Ʈ�� ���ɼ��� ����� ����. set�� ���� ��� �� �ص�, �߰��Ѵ�.
    BYTE changed_bits_temp = 1 << parts_index;
    update_bits_.value |= changed_bits_temp << 5; //sub_parts_option[n]

    dest.Clear();

    return RC::RC_ITEM_SUCCESS;
}

nsSlot::RiderUpdateBits
SCRiderInfo::GetUpdateEnableInfo(const SCItemSlot& rider_item, BOOLEAN summon) const
{
    nsSlot::RiderUpdateBits update_bits = { update_bits.NoOperation };

    if(rider_item.time_info_.is_expired_wait_)
        return update_bits;

    if(summon)
        update_bits.add_rider = true;
    else
        update_bits.sub_rider = true;

    const SCRiderPartsInfo* it = &rider_parts_[0];
    //     sub_parts[x]
    for(int op_index = summon ? 2 : 5;
        it != &rider_parts_[eRIDER_PARTS_POS::Counts]; ++it, ++op_index) {
        if(it->item_code_ == 0)
            continue;
        if(it->is_expired_wait_)
            continue;

        update_bits.value |= 1 << op_index;
    }

    return update_bits;
}

//                                 SCItemSlot::time_info_      on update time value
bool
SCRiderInfo::_UpdateRider(SCItemSlot* rider_item, DWORD64 curr_time_wz)
{
    update_bits_.value = 0;

    nsSlot::SCTimeInfo& rider_time_info = rider_item->time_info_;
    if(is_unlimited_period_) {
        rider_time_info.date_time_ = curr_time_wz; // ��������� ������Ʈ�� �����Ѵ�?
    }
    else if(rider_time_info.is_expired_wait_ == 0) {
        // no - control 'is_progress' in rider
        if(curr_time_wz > rider_time_info.date_time_) {
            rider_time_info.is_expired_wait_ = true;

            update_bits_.value |= (1 << 1); // sub_rider
            update_bits_.value |= (1 << 5) | (1 << 6) | (1 << 7); // sub_parts0,1,2
        }
    }

    // parts update, ��ȯ ���ο� ���� �ɷ�ġ ������ �ʿ��ϴ�.
    SCRiderPartsInfo* it = &rider_parts_[0];
    //     sub_parts[x]
    for(int sub_index = 5; it != &rider_parts_[eRIDER_PARTS_POS::Counts]; ++it, ++sub_index) {
        if(it->item_code_ == 0)
            continue;
        if(it->is_expired_wait_)
            continue;

        if(it->is_unlimited_period_) {
            it->date_time_ = curr_time_wz; // time update
            continue;
        }

        assert(it->is_progress_);
        it->is_progress_ = true; //is_expired_wait_���� tic-toc condition�� ���� �������� ���߱� ����.
        if(curr_time_wz > it->date_time_) {
            //it->change_mode_.value = it->change_mode_.eMode_ActiveToInactive;
            update_bits_.value |= (1 << sub_index); // sub_parts
            it->is_progress_ = false;
            it->is_expired_wait_ = true;
        }
    } //end 'for'

    if(update_bits_.value == 0)
        return true;

    return false;
}

