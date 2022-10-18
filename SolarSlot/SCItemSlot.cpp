#include "stdafx.h"
#include "./SCItemSlot.h"

#include <time.h>

#include <BitStream.hpp>
#include <NumericValues.h>
#include <ItemFormula.h>
#include <Misc.h>
#include <SolarDateTime.h>

//==================================================================================================
//  (Item informations)
#include <ItemInfoParser.h>
#include <FateItemInfoParser.h>
#include <LimitedItemInfoParser.h>
#include <LimitedOptionParser.h>
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
#include <SocketSystem/SocketItemRatioParser.h>
#endif // _NA_000251_20100727_SOCKET_SYSTEM

//==================================================================================================
//
//==================================================================================================
// ���� �Լ� ����
//==================================================================================================
//==================================================================================================
// NOTE: f101221.3L, change namespace
namespace ns_formula {
;

static DWORD
CalcPriceForDura(DWORD item_price, DURATYPE cur_duration, DURATYPE max_duration)
{
    if (cur_duration > max_duration) {
        return 0;
    }
    // CHANGES, f100830.3L, fix up an invalid result
    // when a value of below the decimal point occur a very small missing error
    // in the middle of a formula. for example, when a result of 'item_price * 0.03f' is 'x.49999~'.
    float temp_value = (item_price * 3.0f) / (static_cast<float>(max_duration) * 100);
    DWORD price_for_repair = util::LL_Round((max_duration - cur_duration) * temp_value);
    //DWORD price_for_repair = Round((max_duration - cur_duration) *
    //                               item_price * 0.03f /
    //                               max_duration);

    if (price_for_repair > 15000000) {
        price_for_repair = 15000000;
    }
    return price_for_repair;
}

static float
CalcItemPriceForBuy(DWORD drop_level, eRANK rank, BYTE enchant, BYTE divine)
{
    float price_weight_for_rank = GetPriceWeightForRank(rank);
    float price_weight_for_enchant = GetPriceWeightForEnchant(enchant);

    //formula
    //  item_price = rank_weight * enchant_weight * level * divine_weight;

    float item_price = price_weight_for_rank * price_weight_for_enchant;
    item_price *= drop_level;

    if (divine != 0) {
        item_price *= 4.0f;
    }
    return item_price;
}

// ��æƮ�� �Ҹ� ���� å����
static float
CalcItemPriceForUpGradeFare(DWORD drop_level, eRANK rank, BYTE enchant, BYTE divine)
{
    float price_weight_for_rank_fare = GetPriceWeightForRankFare(rank);
    float price_weight_for_enchant_fare = GetPriceWeightForEnchantFare(enchant);

    //formula
    //  item_price = rank_fare_weight * enchant_fare_weight * level * divine_weight;

    float item_price = price_weight_for_rank_fare * price_weight_for_enchant_fare;
    item_price *= drop_level;
#ifdef _NA_006589_20120415_ENCHANT_PRICE_MODIFY
#else
    if (divine != 0) {
        item_price *= 4.0f;
    }
#endif // _NA_006589_20120415_ENCHANT_PRICE_MODIFY
    return item_price;
}

inline float
CalcItemPriceForSellWithDura(LEVELTYPE item_level, DWORD drop_level,
                             eRANK rank, BYTE enchant, BYTE divine,
                             MONEY repair_fare)
{
    __UNUSED(item_level);
    float item_price_for_buy = CalcItemPriceForBuy(drop_level, rank, enchant, divine);
    float money = (item_price_for_buy / 5.0f - repair_fare);
    if (money < 0.f) {
        return 0.f;
    }
    return money;
}

inline float
CalcItemPriceForSellWithoutDura(DWORD drop_level, eRANK rank, BYTE enchant, BYTE divine)
{
    float item_price_for_buy = CalcItemPriceForBuy(drop_level, rank, enchant, divine);
    float price = item_price_for_buy / 5.0f;
    return (price < 0 ? 0 : price);
}


__forceinline static DWORD
CalcDropLVForDivine(DWORD item_drop_level)
{
    //return (ItemDropLV*28-(ItemDropLV-1)*8+4);
    return (item_drop_level + 5);
}

}; //end of namespace
//==================================================================================================
//==================================================================================================

//==================================================================================================

const SCItemSlot
SCItemSlot::Null_Slot;

const BASE_ITEMINFO
SCItemSlot::Null_ItemInfo;

//==================================================================================================

SCItemSlot::SCItemSlot(void) :
	SlotType_info_(ST_ITEM)
{
    static bool singleton_init_static = _InitializeBinding(this);

    Clear();
}

SCItemSlot::~SCItemSlot(void)
{
    // (CHANGES) (f100802.4L) remove by implementation
    // to acquire multiple list based on type and version
    //_DestroyBinding(this);
}

//==================================================================================================
//  virtual functions
/*
void
SCItemSlot::SetSlotType(eSlotType type) {
    __UNUSED(type);
}

eSlotType
SCItemSlot::GetSlotType() const {
    return ST_ITEM;
}
*/
void
SCItemSlot::SetSerial(DBSERIAL serial) {
    m_dwSerial = serial;
}
/*
DBSERIAL
SCItemSlot::GetSerial() const {
    return m_dwSerial;
}
*/
// (WAVERIX) (NOTE) ������ ������ ���ʿ� �����ؾ��ϴ� �ڵ� : �ڵ尡 �������� ���� ��å�� ��������.
// ����: SCItemSlot instance�� �Ϸ�Ǿ� �־�� �Ѵ�. (��, �ʱ�ȭ �Ǿ� �־�� �Ѵ�.)
// ����: Copy(from server stream, db stream)�� �����, SetCode�� ���ο��� ȣ��ȴ�.
void
SCItemSlot::SetCode(SLOTCODE item_code)
{ 
    item_code_ = item_code; 
    image_item_code_ = item_code;
    _SetItemInfo();

    if (WithDura()) {
        SetDura(GetMaxDura());
    }
}
/*
SLOTCODE
SCItemSlot::GetCode() const {
    return item_code_;
}
*/

//==================================================================================================
/*
void
SCItemSlot::Clear()
{
    SCSlot::Clear();

    ZeroMemory(item_slot_point_holder_start_,               // end
        static_cast<size_t>(&item_slot_point_holder_last_[2] - item_slot_point_holder_start_));
}
*/

void
SCItemSlot::SetDura(DURATYPE duration)
{
    if (WithDura() != false) {
        duration_ = duration;
    }
    else {
        duration_ = 0;
    }
}


bool
SCItemSlot::_SetItemInfo()
{
    if (item_code_ == 0) {
        image_item_info_ = item_info_ = &Null_ItemInfo;
        return false;
    }

    ItemInfoParser* const pItemInfoParser = ItemInfoParser::Instance();
    image_item_info_ = item_info_ = pItemInfoParser->GetItemInfo(item_code_);
    if (item_info_ == NULL)
    {
        //SUNLOG(eCRITICAL_LOG, "Can't find item info %d", item_code_);
        assert(item_info_ != NULL);
        image_item_info_ = item_info_ = &Null_ItemInfo;
        return false;
    }

    // �ʱ�ȭ�ÿ� �ο��ؾ� �ϴ� ��,
    // ������ �ʱ� ������ �����Ѵ�.
    if (nsSlot::ItemRuleInfo::GetRuleInfo(                       // �ܺ� ������ Limited -
        const_cast<nsSlot::ItemRuleInfo*>(&item_rule_info_), item_info_, false) == false)
    {
        return false; // ������ ������ �߸� �Ǿ��� ���ɼ��� �����ϴ�.
    }
    //
    if (!FlowControl::FCAssert(item_rule_info_.is_valid_item)) {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    // (NOTE) (WAVERIX) ������ ������ ������ Ÿ�Կ� ���� ���� ����...

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
#else
    // (NOTE) ������ ������ 2������ �����ϰ� �� ���̴�. Serialize, ItemInfoParser
    const_cast<BASE_ITEMINFO*>(item_info_)->m_SocketHole = min(item_info_->m_SocketHole, 3);

    // ���� ���� �ձ�
    if (item_info_->IsElite()) {
        SetSocketNum(BYTE(item_info_->m_SocketHole));
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM
    // (NOTE) �ϱ� ������ ���� �����... �׳� charge_sub_type������ �򰡰� �������ٵ�...
    if (item_rule_info_.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART_NOTIMER)
    {
        if (item_rule_info_.is_rider) {
            shared_.rider_info_.is_unlimited_period_ = true;
        }
        if (item_rule_info_.is_rider_parts) {
            shared_.rider_parts_info_.is_unlimited_period_ = true;
        }
    }
    //������ �齺�� ������ ������ �ʱ�ȭ ����
    if (item_rule_info_.is_waypoint) {
        ClearWayPointItem();
    }
    // f100910.2L, f101101.2L
    if (item_info_->m_ChargeType == eCHARGE_CHARGE_PPCARD) {
        lock_info_.item_tag_.value = lock_info_.item_tag_.Tag_PCoinPurchase;
    }

    return true;
}

BYTE SCItemSlot::GetMaxDura() const
{
    if (WithDura() == false) {
        return 0;
    }
    //
    if (item_rule_info_.is_limited) 
    {
        // ����Ƽ��� ��þƮ �� ������ ���ȿ�� ���� +
        // GetDuraEnchantWeightForEnchant(GetEnchant()));
        return GetLimitedDuraMax();
    }
    else if (item_rule_info_.is_fate) 
    {
        // Fate(Lucky) �������� ��þƮ �� ������ ���ȿ�� ����
        return GetFateDuraMax();
    }

    BYTE enchant = GetEnchant();
    DWORD bound_check = GetDuraEnchantWeightForEnchant(enchant);
    bound_check += item_info_->m_Dura;

    bound_check = min(UCHAR_MAX, bound_check);
    return BYTE(bound_check);
}

// static function
bool
SCItemSlot::IsOverlap(SLOTCODE item_code)
{
    const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info && item_info->m_byDupNum > 1) {
        return true;
    }
    return false;
}

//==================================================================================================
//  item multi-configuration

// (CHANGES) (f100510.1L) redirected interfaces to support the contents of an etheria
bool SCItemSlot::CanEquip() const
{
    if (item_rule_info_.is_valid_item == false ||
        (item_rule_info_.is_etheria || item_rule_info_.is_shell))
    {
        return false;
    }
    if (CanEquip(item_info_) == false) {
        return false;
    }
    // CHANGES: f110329.3L, added filter about 'lucky identification, possession rules'
    if (item_rule_info_.is_fate) {
        if (shared_.fate_info_.identify_ == false) {
            return false;
        };
    };
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Possession) {
        if (equip_info_.is_identify_possession_ == false) {
            return false;
        };
        //__NA_0_POSSESSION_ITEM_DROP
        if (equip_info_.is_destroyed_) {
            return false;
        };
    };
    // CHANGES: f110331.5L, added filter, ref:f110329.3L
    // IsExpiredTime() || IsExpiredWaitTime()
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer) {
        if (time_info_.is_expired_ || time_info_.is_expired_wait_) {
            return false;
        };
    };
    // NOTE: __NA_001062_20080623_STRENGTH_PENALTY
#ifdef _NA_007129_20140327_MODIFY_CAN_EQUIP_ZERODURA
#else
    if (WithDura()) {
        if (duration_ == 0) {
            return false;
        }
    };
#endif //_NA_007129_20140327_MODIFY_CAN_EQUIP_ZERODURA

    return true;
}

// (CHANGES) (f100510.1L) redirected interfaces to support the contents of an etheria
bool SCItemSlot::CanUseMixture(eMIXTURE_FLAG flag) const
{
    if (item_rule_info_.is_valid_item == false) {
        return false;
    }
    if (item_rule_info_.is_etheria || item_rule_info_.is_shell) {
        return false;
    }
    // (f100727.1L) add rule
    if (lock_info_.history_.value == lock_info_.history_.Combined) {
        if (flag & MIXTURE_RANKUP) {
            return false;
        }
    }
    if (CanUseMixture(item_info_, flag) == false) {
        return false;
    }
    return true;
}

// (CHANGES) (f100510.1L)                            whether the 'is_blocked' value is blocked
bool SCItemSlot::CanTradeSell(eITEM_TRADESELL_TYPE is_blocked) const
{
    if (item_rule_info_.is_valid_item == false) {
        return false;
    }
    WORD calculated_set = item_info_->trade_sell_type;
    if (item_rule_info_.is_etheria || item_rule_info_.is_shell) {
        calculated_set |= eITEM_TRADESELL_SELL;
    }
    // f100909.1L, add rule by _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
    if (lock_info_.item_tag_.value == lock_info_.item_tag_.Tag_TradeSkip) {
        calculated_set &= ~(eITEM_TRADESELL_TRADE | eITEM_TRADESELL_CASH_PRESENT);
    };
#if !defined(_SERVER)
    if (is_blocked == eITEM_TRADESELL_DOALL) {
        return calculated_set == eITEM_TRADESELL_DOALL;
    }
    if (is_blocked == eITEM_TRADESELL_DONTALL) {
        return calculated_set == eITEM_TRADESELL_DONTALL;
    }
#endif
    //!~ ���� �������� ������ ���빰 Ȯ���� ������� �ŷ�/���λ��� �Ұ� - arycoat 2013.02.13
    if (item_rule_info_.is_lottery && (GetLottoInfo()->IsIdentity() == TRUE)) {
        calculated_set |= (eITEM_TRADESELL_SELL | eITEM_TRADESELL_TRADE);
    }
    //~!

    if (calculated_set & is_blocked) {
        return false;
    }
    return true;
}

// the result is fails,
// if mount(by rider) items or pet items is changed to the shell or the etheria
bool SCItemSlot::CanSummonStatus() const
{
    if (item_rule_info_.is_valid_item == false) {
        return false;
    }
    if (item_rule_info_.is_rider == false && item_rule_info_.is_pet == false) {
        return false;
    }
    if (item_rule_info_.is_etheria || item_rule_info_.is_shell) {
        return false;
    }
    if (lock_info_.blocked_ || lock_info_.user_blocked_ || lock_info_.locked_) {
        return false;
    }
    return true;
}

//==================================================================================================
//  Time Utilities
BOOL
SCItemSlot::GetDateTimeToSYSTEMTIME(SYSTEMTIME& OUT sys_time)
{
    if (item_rule_info_.is_valid_item == false || time_info_.is_expired_) {
        return false;
    };
    // CHANGES: f110327.1L, changes charge_sub_type access field
    // to support various making item with timer
    const eCHARGE_SUB_TYPE charge_sub_type = eCHARGE_SUB_TYPE(item_rule_info_.charge_sub_type);
    switch (charge_sub_type)
    {
    case eCHARGE_SUB_FIXED_AMOUNT:
    case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
    case eCHARGE_SUB_RIDER_WITH_PART:
    case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
        {
            util::YYYYMMDDHHMMSSToSYSTEMTIME(GetDateTime(), sys_time);
            return true;
        }
        break;
    case eCHARGE_SUB_FIXED_AMOUNT_EX:
    case eCHARGE_SUB_COORDINATE_EX: //__NA001164_20081024_CHAR_WAY_POINT
        {
            if (IsProgressTime()) {
                util::YYYYMMDDHHMMSSToSYSTEMTIME(GetDateTime(), sys_time);
                return true;
            }
        }
        break;
    case eCHARGE_SUB_FATE:
        {
            if (shared_.fate_info_.identify_) {
                util::YYYYMMDDHHMMSSToSYSTEMTIME(GetDateTime(), sys_time);
                return true;
            }
        }
        break;
    }
    return false;
}

void
SCItemSlot::SetDateTime(DWORD64 d/*second*/)
{
    if (!FlowControl::FCAssert(item_rule_info_.is_valid_item != false)) {
        return;
    }
    // CHANGES: f110327.1L, changes charge_sub_type access field
    // to support various making item with timer
    const eCHARGE_SUB_TYPE charge_sub_type = eCHARGE_SUB_TYPE(item_rule_info_.charge_sub_type);
    if (charge_sub_type == eCHARGE_SUB_GENERAL ||
#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
        charge_sub_type == eCHARGE_SUB_IMMEDIATELY_USE ||
        charge_sub_type == eCHARGE_SUB_IMMEDIATELY_USE_AND_DELETE ||
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
        charge_sub_type == eCHARGE_SUB_COORDINATE)
    {
        return;
    }

    // (NOTE) rider|rider parts (no timer item...)
    if (charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART)
    {
        if (item_rule_info_.is_rider_parts) 
        {
            // parts�� ����������� �����ȴ�. rider�� ������ �����Ǵ� ��������.
            // ��� �ð� �Է�(�ʴ����� �Է�)
            time_info_.date_time_ = d; // start by 'Fireup_DateTime'
            return;
        }
    }
    else if (charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART_NOTIMER)
    {
        // (NOTE) �ϱ� ������ ������ų ��, d�� ���� 0, ��¥�� �����ؾ� �Ѵ�. parts�� �ʿ� ������...
        d = 0;
        if (item_rule_info_.is_rider) {
            shared_.rider_info_.is_unlimited_period_ = true;
        }
        if (item_rule_info_.is_rider_parts) {
            shared_.rider_parts_info_.is_unlimited_period_ = true;
        }
    }

    switch(charge_sub_type)
    {
    case eCHARGE_SUB_FIXED_AMOUNT:
    case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
    case eCHARGE_SUB_FATE:
    case eCHARGE_SUB_RIDER_WITH_PART:
    case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
        {
            time_t now;
            util::TimeSync::time(&now);
            tm* local_time = ::localtime(&now);
            //local_time->tm_hour += (int)(d / (60*60));
            local_time->tm_sec += (int)d;
            if (mktime(local_time) != (time_t)-1)
            {   // m_DateTime ��¥�� ��ü
                util::GetDateTime_YYYYMMDDHHMMSS(local_time->tm_year + 1900,
                                                 local_time->tm_mon + 1,
                                                 local_time->tm_mday,
                                                 local_time->tm_hour,
                                                 local_time->tm_min,
                                                 local_time->tm_sec, 
                                                 time_info_.date_time_);
            }
        }
        break;
    case eCHARGE_SUB_FIXED_AMOUNT_EX:
    case eCHARGE_SUB_COORDINATE_EX:
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
        {
            //----------------------------------------------------------------------
            // ����!!!!!!�ӽ� 
            // ����ȭ ��ȿ�Ⱓ ���� ������ ��ȿ �����Ű�� ���� �Ǽ�-_-;
            // 20061219000000 ������ ū �ð��� ��� �� �� ����. 
            assert(d <= 20061219000000);

            // �ð� ����(�ʴ���)
            // �ٸ� �ʴ����� ���ڴ� ���õ�!(�ð������� ����)
            // if it will be 30�ð� �̿� ������
            time_info_.date_time_ = d;
        }
        break;
    case eCHARGE_SUB_FIXED_QUANTITY:
        {
            // ��� �ð� �Է�(�ʴ����� �Է�)
            // if it will be 30�ð� �̿� ������
            time_info_.date_time_ = d;
        }
        break;
    }
}

void
SCItemSlot::SetFixedDateTime(DWORD64 d)
{
    time_info_.date_time_ = d;
}

bool
SCItemSlot::_UpdateTime()
{
    // eCHARGE_SUB_FIXED_QUANTITY�� �ش�
    assert(item_rule_info_.is_valid_item &&
           item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY);

    ASSERT(0 != time_info_.date_time_);

    if (time_info_.last_checked_ > 0)
    {
        DWORD cur_check_time = GetTickCount();
        DWORD used_time = cur_check_time - time_info_.last_checked_;
        if (used_time >= 1000) //< 1�� �̻� ���
        {
            DWORD minute = used_time / 1000;
            DWORD remainder = used_time % 1000;
            if (time_info_.date_time_ > minute)
            {
                time_info_.date_time_ -= minute;
                time_info_.last_checked_ = cur_check_time - remainder;
            }
            else
            {
                time_info_.date_time_ = 0;
                time_info_.last_checked_ = 0;
                return true;
            }
        }
    }
    return false;
}


bool
SCItemSlot::FireUp_DateTime()
{
    if (time_info_.is_progress_ != false) {
        return false;
    }
    // (NOTE) add rule, timer�� �����ϴ� rider parts�� ù ������ Ÿ�̸� �����̶�� ������ �ִ�.
    if (item_rule_info_.is_rider) {
        return false;                   // assert (false) condition
    }
    if (item_rule_info_.is_rider_parts && shared_.rider_parts_info_.is_unlimited_period_) {
        return false;
    }

    if (time_info_.date_time_ != 0)
    {
        time_t now;
        util::TimeSync::time(&now);
        tm* local_time = ::localtime(&now);
        //LocalTime->tm_hour += (int)(time_info_.date_time_/(60*60));
        //LocalTime->tm_min += (int()(time_info_.date_time_/60);
        local_time->tm_sec += (int)time_info_.date_time_;
        if (mktime(local_time) != (time_t)-1)
        {
            // time_info_.date_time_ ��¥�� ��ü
            util::GetDateTime_YYYYMMDDHHMMSS(local_time->tm_year + 1900,
                                             local_time->tm_mon + 1,
                                             local_time->tm_mday,
                                             local_time->tm_hour,
                                             local_time->tm_min,
                                             local_time->tm_sec,
                                             time_info_.date_time_);
            time_info_.is_progress_ = true;
            return true;
        }
    }
    return false;
}

// (returns)
// - eCHARGE_SUB_FIXED_AMOUNT_EX Ÿ�԰� ���� ó�� ������ ��츦 �����ؾ� �ϴ� ��� (start timer) ����
//   if (start_timer) then true
//   else            then false
BOOL
SCItemSlot::OnEquip()
{
    // <- ������ ������ �ʿ��Ѱ�?...
    if (time_info_.is_expired_) {
        return false;
    }
    // CHANGES: f110327.1L, changes charge_sub_type access field
    // to support various making item with timer
    const eCHARGE_SUB_TYPE charge_sub_type = eCHARGE_SUB_TYPE(item_rule_info_.charge_sub_type);
    switch(charge_sub_type)
    {
    case eCHARGE_SUB_FIXED_AMOUNT://< ������ : ���� �Ⱓ�� (2007-06-07 24:12:30)
    case eCHARGE_SUB_COORDINATE_EX:
    case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER: //�̰��� ���� ���� �����̹Ƿ� ���û��� �ƴ�
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
        // nothing to do
        break;
    case eCHARGE_SUB_FIXED_AMOUNT_EX://< Ȯ�������� : ����� ���� ���� �Ⱓ ���  (30�ð� -> 2007-06-07 24:12:30)
    case eCHARGE_SUB_RIDER_WITH_PART: //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
        if (FireUp_DateTime() != false) {
            return true;
        }
        break;
    case eCHARGE_SUB_FIXED_QUANTITY://< ������ : ��� �Ⱓ�� (30�ð�)
        // �������� ��
        // Ÿ�̸� ����?
        ASSERT(time_info_.date_time_ != 0);
        ASSERT(time_info_.last_checked_ == 0);
        time_info_.last_checked_ = GetTickCount();
        break;
    case eCHARGE_SUB_FATE:
        if (shared_.fate_info_.identify_) {
            return true;
        }
        break;
    }

    return false;
}

void
SCItemSlot::OnUnEquip()
{
    if (time_info_.is_expired_) {
        return;
    }
    const eCHARGE_SUB_TYPE charge_sub_type = \
        static_cast<eCHARGE_SUB_TYPE>(item_rule_info_.charge_sub_type);
    switch(charge_sub_type)
    {
    case eCHARGE_SUB_FIXED_AMOUNT://< ������ : ���� �Ⱓ�� (2007-06-07 24:12:30)
    case eCHARGE_SUB_FIXED_AMOUNT_EX://< Ȯ�������� : ����� ���� ���� �Ⱓ ���  (30�ð� -> 2007-06-07 24:12:30)
    case eCHARGE_SUB_FATE:
    case eCHARGE_SUB_COORDINATE_EX:
    case eCHARGE_SUB_RIDER_WITH_PART: //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
        {
            // nothing to do
        }
        break;
    case eCHARGE_SUB_FIXED_QUANTITY://< ������ : ��� �Ⱓ�� (30�ð�)
        {
            // ������������ ��
            // Ÿ�̸� ����?
            time_info_.is_expired_ = _UpdateTime();
            time_info_.last_checked_ = 0;
        }
        break;
    }
}

BOOL
SCItemSlot::Update(BOOL is_pcroom_user)
{
    // �ϱ� ���ǵ��̸� ���⿡ ������ �� �ؾ߰�����...
    if (item_info_ == NULL ||
        item_info_->m_Code == 0 ||
        item_rule_info_.is_valid_item == false)
    {
        return true;
    }

    if (!FlowControl::FCAssert(item_code_ == item_info_->m_Code)) {
        return false;
    }
    if (time_info_.is_expired_) {
        return false;
    }
    if (slot_updater_.SlotUpdate(this, is_pcroom_user) == false) {
        return false;
    }
    const eCHARGE_SUB_TYPE charge_sub_type = \
        static_cast<eCHARGE_SUB_TYPE>(item_rule_info_.charge_sub_type);
    //eCHARGE_PC_ROOM�� ���� Ÿ�̸� ����� �ƴϴ�...
    if (charge_sub_type == eCHARGE_PC_ROOM && is_pcroom_user == false) {
        return false;
    }
    // Ÿ�̸� �����ϴ� �������� ���, �̰��� ����Ѵ�.
    if ((item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer) == 0) {
        return true;
    }
    DWORD64 curr_time_wz = 0; // Webzen Time Format

    // �ֱ����� �˻� ����
    // eCHARGE_SUB_FIXED_AMOUNT     < ������ : ���� �Ⱓ�� (2007-06-07 24:12:30)
    // eCHARGE_SUB_FIXED_AMOUNT_EX  < Ȯ�������� : ����� ���� ���� �Ⱓ ��� (30�ð� -> 2007-06-07 24:12:30)
    // eCHARGE_SUB_FIXED_AMOUNT_WAIT< ���� ����� ������ ���, ��� �ð��� ������ ����
    // eCHARGE_SUB_FIXED_QUANTITY   < ������ : ��� �Ⱓ�� (30�ð�)
    // eCHARGE_SUB_FATE             < ��Ű ������ ����: { ������, �ð� ����, �ð� ����� ��� �Ұ� }
    // eCHARGE_SUB_COORDINATE_EX    < ���� Ÿ�̸�: ��� �Ⱓ ����� ���� + ��ϵ� ��������Ʈ ������ ���� ���� ���� ���� ����
    // eCHARGE_SUB_RIDER_WITH_PART  < ���� Ÿ�̸�:
    //                              - ���̴�: �Լ� �������� ���� �Ⱓ ��� ��, ��� �Ұ� ���·� ����
    //                              - ����: ���� �������� ���� �Ⱓ ��� ��, ��� �Ұ� ���·� ����
    // eCHARGE_SUB_RIDER_WITH_PART_NOTIMER < ���� Ÿ�̸�:
    //                              - ���̴�: �Ⱓ ����(eCHARGE_SUB_GENERAL), ������ ������ �Ⱓ ������ �� ����.
    //                              - ����: �Ⱓ ����, ���̴��� ������ �ٸ� ������ �Ⱓ ���� ����
    //                                  �������� �ʰ� ���������� ����� 'eCHARGE_SUB_GENERAL'�� ����
    switch(charge_sub_type)
    {
    case eCHARGE_SUB_FIXED_AMOUNT:
    case eCHARGE_SUB_FIXED_AMOUNT_EX2:
        { //< ������ : ���� �Ⱓ�� (2007-06-07 24:12:30)
            util::GetDateTime_YYYYMMDDHHMMSS(curr_time_wz);
            if (curr_time_wz > time_info_.date_time_) {
                time_info_.is_expired_ = true;
                return false;
            }
        }
        break;
    case eCHARGE_SUB_FIXED_AMOUNT_EX:
        { //< Ȯ�������� : ����� ���� ���� �Ⱓ ���  (30�ð� -> 2007-06-07 24:12:30)
            if (false != time_info_.is_progress_)
            {
                util::GetDateTime_YYYYMMDDHHMMSS(curr_time_wz);
                if (curr_time_wz > time_info_.date_time_)
                {
                    time_info_.is_expired_ = true;
                    time_info_.is_progress_ = false;
                    return false;
                }
            }
        }
        break;
    case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
        { //���� ����� ������ ���, ��� �ð��� ������ ����
            util::GetDateTime_YYYYMMDDHHMMSS(curr_time_wz);
            if (time_info_.is_expired_wait_)
            {
                // �������� ���ð� ���� ������ �����Ѵ�.
                if (curr_time_wz > time_info_.date_time_)
                {
                    time_info_.is_expired_ = true;
                    time_info_.is_expired_wait_ = false;
                    return false;
                }
            }
            else {
                // �Ⱓ�� �� �Ǹ� �������� ���ð��� �ش�.
                if (curr_time_wz > time_info_.date_time_)
                {
                    time_info_.is_expired_wait_ = true;
                    SetDateTime(FIXED_AMOUNT_EXPIRE_WAIT_TIME);
                    return false;
                }
            }
        }
        break;
    case eCHARGE_SUB_FIXED_QUANTITY:
        { //< ������ : ��� �Ⱓ�� (30�ð�)
            if (time_info_.is_expired_ == false)
            {
                time_info_.is_expired_ = _UpdateTime();
            }
            return !time_info_.is_expired_;
        }
        break;
    case eCHARGE_SUB_FATE:
        { //< ��Ű ������ ����: { ������, �ð� ����, �ð� ����� ��� �Ұ� }
            //������ �������� �ð� ���� ����
            if (shared_.fate_info_.period_ == INFINITE_ITEM_DAY) {
                return true;
            }
            if (time_info_.is_expired_wait_) {
                return true;
            }
            util::GetDateTime_YYYYMMDDHHMMSS(curr_time_wz);

            // �Ⱓ�� �� �Ǹ� ��� �Ұ� ���·� �����.
            if (shared_.fate_info_.identify_ && curr_time_wz > time_info_.date_time_)
            {
                time_info_.is_expired_wait_ = true; 
                return false;
            }
        }
        break;
    case eCHARGE_SUB_COORDINATE_EX:
        { //< ���� Ÿ�̸�: ��� �Ⱓ ����� ���� + ��ϵ� ��������Ʈ ������ ���� ���� ���� ���� ����
            if (false != time_info_.is_progress_)
            {
                util::GetDateTime_YYYYMMDDHHMMSS(curr_time_wz);
                SCWayPointInfo& waypoint_edit = shared_.waypoint_info_.GetEditableObject();
                if (curr_time_wz > time_info_.date_time_)
                {
                    time_info_.is_expired_ = true;
                    time_info_.is_progress_ = false;
                    waypoint_edit.Clear();
                    return false;
                }
                waypoint_edit.Update(curr_time_wz);
            }
        }
        break;
    case eCHARGE_SUB_RIDER_WITH_PART:           //[09]
    case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:   //[10]
        {
            //[09] ���� Ÿ�̸�:
            //  - ���̴�: �Լ� �������� ���� �Ⱓ ��� ��, ��� �Ұ� ���·� ����
            //  - ����: ���� �������� ���� �Ⱓ ��� ��, ��� �Ұ� ���·� ����
            //[10] ���� Ÿ�̸�:
            //  - ���̴�: �Ⱓ ����(eCHARGE_SUB_GENERAL), ������ ������ �Ⱓ ������ �� ����.
            //  - ����: �Ⱓ ����, ���̴��� ������ �ٸ� ������ �Ⱓ ���� ����
            //          �������� �ʰ� ���������� ����� 'eCHARGE_SUB_GENERAL'�� ����
            util::GetDateTime_YYYYMMDDHHMMSS(curr_time_wz);
            if (item_rule_info_.is_rider)
            {
                // (NOTE) Rider �������� ���� ����� ���� ������Ʈ�� �ʿ��ϴ�.
                // Rider ��ü�� ���� ���ᰡ �ƴ� ���� ���� ������Ʈ�ÿ��� �����ϰ� false��ȯ����
                //�̺�Ʈ �ߵ���Ų��.
                bool result = shared_.rider_info_._UpdateRider(this, curr_time_wz);
                if (result == false) {
                    // notification to PlayerRider? ���� ������ ���� ���� �ʿ�
                    return false;
                }
            }
            else if (item_rule_info_.is_rider_parts)
            {
                if (shared_.rider_parts_info_.is_unlimited_period_) {
                    time_info_.date_time_ = curr_time_wz; // ��������� ������Ʈ�� �����Ѵ�?
                    return true;
                }
                // ���� ���������� ����ÿ��� SCItemSlot�� �ð� ������ Ȱ���Ѵ�.
                if (time_info_.is_expired_wait_) {
                    return true;
                }
                // ���� �ѹ��̶� ������, is_progress_ != false
                if (time_info_.is_progress_ == false) {
                    return true;
                }
                if (curr_time_wz > time_info_.date_time_)
                {
                    time_info_.is_expired_wait_ = true;
                    time_info_.is_progress_ = false;
                    return false; // ���� �������� ���� �̺�Ʈ�� �ߵ��ؾ� �Ѵ�.
                }
            }
            else
            {
                ASSERT(!"Unexpected value");
            }
        }
        break;
    } //end 'switch'

    return true;
}

BOOL
SCItemSlot::DecreasingDura(eDecDuraType eDecType)
{
    if (item_rule_info_.is_valid_item == false) {
        return false;
    }
    //assert(item_info_ != 0);
    if (item_info_->m_dwDuraStr == 0) {
        return false;
    }
    // WithDura�� ���� �������� ���Ǵ� ������������ ������ �ʿ��� �� ������...

    int fWeaponRatio = 0, fArmorRatio = 0;
    if (eDecType == DECDURA_TYPE_ATTACK)
    {
        fWeaponRatio = 170000;
        fArmorRatio = 42500;
    }
    else
    {
        fWeaponRatio = 340000;
        fArmorRatio = 85000;
    }

    if (item_info_->IsWeapon())
    {
        int fSuccessRatio = (int)dRandom(0, fWeaponRatio);
        if (fSuccessRatio <= 100)
        {
            if (duration_ >= 1) {
                duration_ -= 1;
            }
            return true;
        }
    }
    else if (item_info_->IsArmor())
    {
        int fSuccessRatio = (int)dRandom(0, fArmorRatio);
        if (fSuccessRatio <= 100)
        {
            if (duration_ >= 1) {
                duration_ -= 1;
            }
            return true;
        }
    }

    return false;
}

BOOL
SCItemSlot::StoreDamageNDecreaseDura(const DWORD damage)
{
    if (item_rule_info_.is_valid_item == false) {
        return false;
    }
    const DWORD duration_strength = item_info_->m_dwDuraStr;
    if (duration_strength == 0) {
        return false;
    }

#ifdef _NA_0_20111130_DISTROY_WEAPON
    if ((duration_ == 0) && damage)
        return true;
#endif

    m_DamageStoreForDura += damage;

    if (m_DamageStoreForDura >= duration_strength)
    {
        if (duration_ >= 1) {
            duration_ -= 1;
        }
        m_DamageStoreForDura -= duration_strength;
        return true;
    }

    return false;
}

//==================================================================================================

MONEY
SCItemSlot::GetPriceForRepair() const
{
    const DURATYPE max_duration = GetMaxDura(); // side-effect FCAssert(WithDura())
    if (max_duration == 0) {
        return 0;
    }
    MONEY price_for_buy = GetPriceForBuy();
    duration_;

    return ns_formula::CalcPriceForDura((DWORD)price_for_buy, duration_, max_duration);
}

// (UNUSED) ??
LEVELTYPE
SCItemSlot::GetLimitLevel() const
{
    if (GetItemInfo()->IsAccessory())
    {
        return CalcLimitLVAccessory(GetItemInfo()->m_LimitEqLevel, GetRank());
    }

    return GetItemInfo()->m_LimitEqLevel;
}

#if SUN_CODE_BACKUP
// (UNUSED) ??
DWORD
SCItemSlot::GetPhyDef() const
{
    WORD PhyDef = GetItemInfo()->m_wPhyDef;
    if (GetSet() == 1)
    {
        PhyDef = CalcPhyDefForDivineArmor(PhyDef, GetItemInfo()->m_LV);
    }
    return CalcPhyDef(PhyDef, GetEnchant(), GetItemInfo()->m_LV);
}
#endif
//==================================================================================================
// Rank Option

eRANK_LEVEL
SCItemSlot::GetRankLevel() const
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank)
    {
        switch(rank_info_.current_rank_)
        {
        case RANK_F: case RANK_E: case RANK_D: case RANK_C: case RANK_B:
            return RANK_LOW;
        case RANK_MA: case RANK_A: case RANK_PA:
            return RANK_MIDDLE;
        case RANK_MS: case RANK_S: case RANK_PS: 
            return RANK_HIGH; 
        }
    }
    ASSERT(!"�߸��� ��ũ�Դϴ�.");
    return RANK_LOW;
}
//
//==================================================================================================
//

//--------------------------------------------------------------------------------------------------
// (WARNING) �ϱ� ���� �Լ����� ��Ȯ�� ����� �����ϴ� ���� �ܺ� ��� ���� ���̴�.
// �߸��� ������ ���� ��� ������ ��â, �Ĵ� ������ 0�� �ұ�? hummm...
// (091001) �ϴ��� ���� ������� �ϱ�� �Ѵ�. �� ���� �ִ� ����...

DWORD
SCItemSlot::GetDropLevel() const
{
    // TODO(WAVERIX) ���ο��� ���� ����ϴ� �Լ���.
    //�߸��� ������ ��쿡 ���� ���� �ڵ� '0'�� �������� ���� �������� �ľ��ϱ�� �Ѵ�.
    if (item_rule_info_.is_valid_item == false) {
        return 0;
    }
    // formula
    // cur_drop_level = item_info::m_DropLV + enchant_weight + rank_weight(0)

    // ��þƮ 1�ܰ� �� ���� ��� ���� 2�� ����
    DWORD enchant_weight = (DWORD)GetEnchant() << 1;
    DWORD rank_weight = 0; //GetRank() * 0; //...?

    bool is_divine = IsDivine();
    DWORD cur_drop_level = item_info_->m_DropLV + enchant_weight + rank_weight;
    if (is_divine && (item_rule_info_.is_weapon || item_rule_info_.is_armor)) {
        cur_drop_level = ns_formula::CalcDropLVForDivine(cur_drop_level);
    }
    return cur_drop_level;
}

MONEY
SCItemSlot::GetPriceForBuy() const
{
    //�߸��� ������ ��쿡 ���� ���� �ڵ� '0'�� �������� ���� �������� �ľ��ϱ�� �Ѵ�.
    if (item_rule_info_.is_valid_item == false) {
        return 0;
    }
    DWORD drop_level = GetDropLevel();
    eRANK rank = GetRank();
    BYTE enchant = GetEnchant();
    bool is_divine = IsDivine();

    MONEY item_price_for_buy =
        (MONEY)ns_formula::CalcItemPriceForBuy(drop_level, rank, enchant, is_divine);

    //                  ranges of 'GetNum'
    //                  if duplicated then [0, 256)
    //                  else then 1
    item_price_for_buy *= GetNum();
    return item_price_for_buy;
}

// ��æƮ�� �Ҹ� ���� å����
MONEY
SCItemSlot::GetPriceForUpGradeFare() const
{
    //�߸��� ������ ��쿡 ���� ���� �ڵ� '0'�� �������� ���� �������� �ľ��ϱ�� �Ѵ�.
    if (item_rule_info_.is_valid_item == false) {
        return 0;
    }
#ifdef _NA_006589_20120415_ENCHANT_PRICE_MODIFY
    DWORD drop_level = item_info_->m_DropLV;
#else
    DWORD drop_level = GetDropLevel();
#endif
    eRANK rank = GetRank();
    BYTE enchant = GetEnchant();
    bool is_divine = IsDivine();

    MONEY item_price_for_upgrade_fare =
        (MONEY)ns_formula::CalcItemPriceForUpGradeFare(drop_level, rank, enchant, is_divine);

    // enchant�� �Ͼ�� �������� ��ø�� �����Ѱ�? - assert = NO!
    // (WAVERIX) (CHANGES) GetNum�� ���� ����
    return item_price_for_upgrade_fare;
}

MONEY
SCItemSlot::GetPriceForSell() const
{
    //�߸��� ������ ��쿡 ���� ���� �ڵ� '0'�� �������� ���� �������� �ľ��ϱ�� �Ѵ�.
    if (item_rule_info_.is_valid_item == false) {
        return 0;
    }
    // default
    DWORD drop_level = GetDropLevel();
    eRANK rank = GetRank();

    MONEY item_price_for_sell = 0;
    // (WAVERIX) (CHANGES) WithDura����... is_weapon || is_armor��� assert(WithDura() != 0)
    if (item_rule_info_.is_weapon || item_rule_info_.is_armor)
    {
        BYTE enchant = GetEnchant();
        bool is_divine = IsDivine();
        MONEY item_price_for_repair = GetPriceForRepair();

        item_price_for_sell = (MONEY)ns_formula::CalcItemPriceForSellWithDura(item_info_->m_LV,
            drop_level, rank, enchant, is_divine, item_price_for_repair);
    }
    else if (item_rule_info_.is_accessory)
    {
        // (NOTE) �ŵ�Ǵ� �ǹ��ε�, special accessory�� accessory����� ���� �ʴ� ���ΰ�?
        item_price_for_sell = \
            (MONEY)ns_formula::CalcItemPriceForSellWithoutDura(drop_level, rank, 0, 0);
    }
    else
    {
        assert(rank == RANK_E);
        item_price_for_sell = \
            (MONEY)ns_formula::CalcItemPriceForSellWithoutDura(drop_level, RANK_E, 0, 0);
        item_price_for_sell *= GetNum();
    }

    return item_price_for_sell;
}

//==================================================================================================
// Socket Item Option

bool SCItemSlot::ExistEmptySocket(OUT eSOCKET& empty_socket_out) const
{
    // ������ ��Ģ�� �˻��Ѵ�.
    if ((item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Socket) == 0) {
        return false;
    }
 
    // (WARNING) GetSocketAttrIndex�� �����ϴ� ���Ͽ� ���� �����̴�.

    // (BUGFIX) (WOPS:36529) ����Ʈ �����ۿ� �Ӽ� �߰��� ���� �ʴ� ����
    // (CHANGES) �ּ� ó��
    // (NOTE) �� �ڵ� ��� �۾� ��� GetSocketNum()�� ���� ������ ��. �ܼ��� ��� ������ �ǹ��Ѵ�.
    /*
    const int number_of_sockets = GetSocketNum();
    if (number_of_sockets == SOCKET_MAX)
        return false;
    */

    // �� ������ �ִ��� �˻��Ѵ�.
    bool is_find_empty_socket = false;
    int socket_index = 0;
    for (; socket_index < SOCKET_MAX; ++socket_index) 
    {
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
        const SCSocketOptionInfo::AttrValues& socket_attr_value = 
                                              socket_info_.socket_attributes_[socket_index];
        if (socket_attr_value.first == 0) 
        {
            is_find_empty_socket = true;
            break;
        }
#else
        const SCSocketInfo::AttrValues& socket_attr_value = 
                                        socket_info_.socket_attributes_[socket_index];
        if (socket_attr_value.attr_index_ == 0) {
            is_find_empty_socket = true;
            break;        
        }
#endif // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    }

    // ��� ������ ���ϼ��� �ʰ� �ߴ��� �˻��Ѵ�.
    const int max_socket_num = GetSocketNum();
    if ((is_find_empty_socket == true) && (socket_index < max_socket_num))
    {
        empty_socket_out = static_cast<eSOCKET>(socket_index);
        return true;
    }
    
    return false;
}

// �ش� ����� ��尡 ���� �������� �˻��Ѵ�.
bool SCItemSlot::IsEnableFillSocketByGrade(const eSOCKET_LEVEL zard_grade) const
{
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    const size_t max_filled_socket_list[kZardGradeMax] = 
    {
        0, // ��� ��� ����
        5, // �ϱ� ��� ��� ����
        3, // �߱� ��� ��� ����
        2, // ��� ��� ��� ����
        1  // �ֻ�� ��� ��� ����
    };
    const size_t max_number_of_filled_socket = max_filled_socket_list[zard_grade];

    // �ش� ����� ��尡 ��� ���� �ִ��� �˻��Ѵ�.
    size_t number_of_filled_socket = 0;
    {
        const int number_of_socket = GetSocketNum();
        for (int i = 0; i < number_of_socket; ++i)
        {
            const eSOCKET socket_index = static_cast<eSOCKET>(i);
            const sSOCKET_OPTION* const socket_option = GetSocketOption(socket_index);
            if (socket_option != NULL)
            {
                if (socket_option->zard_grade == zard_grade)
                {
                    ++number_of_filled_socket;
                }
            }
        }
    }
    
    // ��� �������� ��� ���� �ִ��� �˻��Ѵ�.
    if (number_of_filled_socket >= max_number_of_filled_socket)
    {
        return false;
    }
#else
    __UNUSED(zard_grade);
#endif // _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    return true;
}

//==================================================================================================
// Limited Item Option

//--------------------------------------------------------------------------------------------------
// Limited Control
void
SCItemSlot::SetLimitedIndex(WORD limited_index) 
{
    if (item_rule_info_.is_valid_item == false ||
        limited_index == 0)
    {
        return;
    }
    //
    if (_CanChangeToLimited() == false) { // important!!
        return;
    }
    // preparing...
    // post action = on Recv Serial, change to limited item
    // desired next call : 'CalLimited();
    shared_.limited_info_.limited_index_ = limited_index; // temp backup
}

//--------------------------------------------------------------------------------------------------
// Limited Control
bool
SCItemSlot::_CanChangeToLimited() const
{
    // (WARNING) ����Ƽ�� ���� ������ ���� �� (Armor)���̴�.
    // Constraints#1 - �̹� ����Ƽ�尡 �ƴϾ�� �Ѵ�.
    // Constraints#2 - ���ΰ�?
    // Constraints#3 - ��Ÿ �ٸ� �Ӽ��� ���� ��? (��Ű ���, shared_������ �����ϴ�...)

    if (item_rule_info_.is_valid_item == false) {
        assert(!"invalid item type");
        return false;
    }

    if (item_rule_info_.is_can_overlap != false) {
        //assert(!"invalid item type : enable duplicate");
        return false;
    }

    // Constraints#1 - �̹� ����Ƽ�尡 �ƴϾ�� �Ѵ�.
    if (item_rule_info_.is_limited != false) { // check stored type
        //assert(!"already limited item");
        return false;
    }

    // Constraints#2 - ���ΰ�?
    if (item_rule_info_.is_armor == false) {
        //assert(!"must be armor");
        return false;
    }

    // Constraints#3 - ��Ÿ �ٸ� �Ӽ��� ���� ��? (��Ű ���, shared_������ �����ϴ�...)
    // (WARNING) 
    if (_CanReuseSharedBlock() == false) {
        assert(!"already another item");
        return false;
    }

    nsSlot::ItemRuleInfo item_rule_info; //                          �ܺ� ������ Limited -
    if (nsSlot::ItemRuleInfo::GetRuleInfo(&item_rule_info, *this, true, NULL) == false) {
        assert(!"other rule exist");
        return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
// Limited Control
bool
SCItemSlot::_ChangeToLimitedType()
{
    // �ʱ�ȭ�ÿ� �ο��ؾ� �ϴ� ��, ������ �ʱ� ������ �����Ѵ�.
    nsSlot::ItemRuleInfo item_rule_info; //                          �ܺ� ������ Limited -
    if (nsSlot::ItemRuleInfo::GetRuleInfo(&item_rule_info, *this, true, NULL) == false)
    {
        __debugbreak(); // i want must be true, already checked in '_CanChangeToLimited'
        return false; // ������ ������ �߸� �Ǿ��� ���ɼ��� �����ϴ�.
    }
    if (!FlowControl::FCAssert(item_rule_info.is_valid_item && item_rule_info.is_limited)) {
        return false;
    }
    const_cast<nsSlot::ItemRuleInfo&>(item_rule_info_) = item_rule_info;
    equip_info_.is_limited_ = true;
    return true;
}

//--------------------------------------------------------------------------------------------------
// Limited Control
void
SCItemSlot::CalLimited()
{
    // called state:
    //  on Recv Serial key, called by 'ItemManager::AddSerialCode'
    const WORD limited_index = shared_.limited_info_.limited_index_;
    if (limited_index == 0) {
        return;
    }
    if (_CanChangeToLimited() == false) {
        return;
    }
    //if (IsOverlap())
    //{
    //    ASSERT(FALSE);
    //    DISPMSG("Overlap Item must not CalLimited : %d \n", limited_index);
    //    return;
    //}
    if (_ChangeToLimitedType() == false) {
        return;
    }
    shared_.limited_info_.CalLimited(item_code_);

    //������
    SetDura(GetMaxDura());

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
#else
    //���� Ȯ��
    LimitedItemInfoParser* const pLimitedItemInfoParser = LimitedItemInfoParser::Instance();
    int random_value = pLimitedItemInfoParser->GetRandomkeyFromSocketRatio(limited_index);
    if (FlowControl::FCAssert(0 <= random_value && random_value <= SOCKET_MAX)) {
        SetSocketNum(static_cast<BYTE>(random_value));
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM
}

// ������ ����(for stat change) - serial key not issued, need serial key 
// (090929) (ADD-IN)
bool
SCItemSlot::CopyFromSrcForStatChange(SLOTCODE to_change_item_code, const SCItemSlot& src)
{
    if (src.item_rule_info_.is_valid_item == false) {
        return false;
    }
    if (src.IsBlocked() || src.IsLocked()) {
        return false;
    }
    //__UNUSED(const BASE_ITEMINFO* src_item_info = src.item_info_;);
    if (src.item_rule_info_.is_weapon == false && src.item_rule_info_.is_armor == false) {
        return false;
    }
    // skip! elite check
    if (src.item_rule_info_.is_fate || src.item_rule_info_.is_limited) {
        return false;
    }
    const BASE_ITEMINFO* new_item_info = ItemInfoParser::Instance()->GetItemInfo(to_change_item_code);
    if (new_item_info == NULL) {
        return false;
    }
    if (!FlowControl::FCAssert(new_item_info->m_wWICode == src.item_info_->m_wWICode &&
                               new_item_info->m_wVICode == src.item_info_->m_wVICode &&
                               new_item_info->m_NCode == src.item_info_->m_NCode))
    {
        return false;
    }

    SetCode(to_change_item_code); // buildup configurations
    if (item_rule_info_.is_valid_item == false) {
        return false;
    }
    // copy options
    CopyMemory(item_slot_point_holder_options_, src.item_slot_point_holder_options_,
        static_cast<size_t>(&item_slot_point_holder_last_[2] - item_slot_point_holder_options_));
    // (NOTE) ���� ���͸��� �ɸ��� ���� ������ ������� �ʴ´�.
    // clear shared block
    ZeroMemory(&shared_, sizeof(shared_));

    //���׼���:36567 2010-02-02 ������ 0905�̻��籹���������� �츣�޽��� ���� ���� ������ ����Ǵ� ����
    DURATYPE duration = src.GetDura();
    DURATYPE max_duration = GetMaxDura();

    DURATYPE align = duration > max_duration ? max_duration : duration;
    SetDura(align);    

    return true;
}

//--------------------------------------------------------------------------------------------------
// CHANGES: f110327.1L, changes charge_sub_type access field
// to support various making item with timer
// accetable = { sDefault -> sTime, sEquip -> sEquipTime, sEquipEtc -> sEquipEtcTime }
bool SCItemSlot::ChangeItemToUsingTimer()
{
    if (item_rule_info_.is_valid_item == false) {
        return false;
    };
    ;{  // filters to prevent the below conditions
        // can't change item that is not acceptable type like a limited, lucky(fate), etc ...
        if (item_rule_info_.is_editable == false) {
            return false;
        };
        if (item_rule_info_.is_can_overlap) {
            return false;
        };
        if (item_rule_info_.is_limited || item_rule_info_.is_fate) {
            return false;
        };
        if (item_rule_info_.is_shell) {
            return false;
        };
        bool only_accept_item_types =
            (item_rule_info_.is_normal ||
             item_rule_info_.is_armor || item_rule_info_.is_weapon ||
             item_rule_info_.is_accessory || item_rule_info_.is_accessory_special ||
             //youngmoon 2011-11-25 ����� ĳ���������� ��, Ż�� �����ϰ� �Ѵ�.
             item_rule_info_.is_pet || item_rule_info_.is_rider ||
             item_rule_info_.is_costume || item_rule_info_.is_toggle_item
#ifdef _NA_007031_20140116_MODIFIED_PANDORABOX_ITEM_POLICY
             || item_rule_info_.is_waypoint
#endif //_NA_007031_20140116_MODIFIED_PANDORABOX_ITEM_POLICY
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
             || item_rule_info_.is_deco_costume
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
             );

        if (only_accept_item_types == false) {
            return false;
        };
    };
    //
    ItemParseType::ValueType parse_type = 0;
    if (item_rule_info_.is_armor || item_rule_info_.is_weapon) {
        if (item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX) {
            return true;
        };
        if (item_rule_info_.charge_sub_type != eCHARGE_SUB_GENERAL) {
            return false; // exist already used timer
        };
        parse_type = ItemParseType::kEquipTime;
    }
    else if (item_rule_info_.is_accessory || item_rule_info_.is_accessory_special) {
        if (item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX
#ifdef _NA_008150_20150331_MODIFY_EX2_CHARGE_SUB_TYPE_USABLE_ANOTHER_FUNC
            || item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX2
#endif // _NA_008150_20150331_MODIFY_EX2_CHARGE_SUB_TYPE_USABLE_ANOTHER_FUNC
            ) {
            return true;
        };
        if (item_rule_info_.charge_sub_type != eCHARGE_SUB_GENERAL) {
            return false; // exist already used timer
        };
        parse_type = ItemParseType::kEquipEtcsTime;
    }
    //youngmoon 2011-11-25 ����� ĳ���������� ��, Ż�� �����ϰ� �Ѵ�.
    else if (item_rule_info_.is_pet) {
#ifdef _NA_008389_20150703_TIMERITEM_PET_DROP_RULE
        if (item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT ||
            item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY) {
            return true;
        };
#else
        if (item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY) {
            return true;
        };
#endif //_NA_008389_20150703_TIMERITEM_PET_DROP_RULE
        
        if (item_rule_info_.charge_sub_type != eCHARGE_SUB_GENERAL) {
            return false; // exist already used timer
        };
        parse_type = ItemParseType::kPet;
    }      
    else if (item_rule_info_.is_rider) {
        if (item_rule_info_.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART ||
            item_rule_info_.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART_NOTIMER) {
            return true;
        };
        if (item_rule_info_.charge_sub_type != eCHARGE_SUB_GENERAL) {
            return false; // exist already used timer
        };
        parse_type = ItemParseType::kRider;
    }
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
    else if (item_rule_info_.is_deco_costume) 
#else
    else if (item_rule_info_.is_costume) 
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
    {
        if (item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX) {
            return true;
        }
        if (item_rule_info_.charge_sub_type != eCHARGE_SUB_GENERAL) {
            return false; // exist already used timer
        };
        parse_type = ItemParseType::kEquipEtcsTime;
    }
    else if (item_rule_info_.is_toggle_item) {
        if (item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT ||
            item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX ||
            item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY)
        {
            return true;
        }
        if (item_rule_info_.charge_sub_type != eCHARGE_SUB_GENERAL) {
            return false; // exist already used timer
        };
        parse_type = ItemParseType::kEquipEtcsTime;
    }
    ////////
    else { //item_rule_info_.is_normal
#ifdef _NA_007031_20140116_MODIFIED_PANDORABOX_ITEM_POLICY
        if (item_rule_info_.charge_sub_type > eCHARGE_SUB_GENERAL &&
            item_rule_info_.charge_sub_type < eCHARGE_SUB_COUNTS) {
                return true;
        };
#else
        if (item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT ||
            item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX2) {
                return true;
        };
        if (item_rule_info_.charge_sub_type != eCHARGE_SUB_GENERAL) {
            return false; // exist already used timer
        };
#endif //_NA_007031_20140116_MODIFIED_PANDORABOX_ITEM_POLICY
        parse_type = ItemParseType::kDefaultTime;
    }
    //
    nsSlot::ItemRuleInfo modifying_rule;
    if (nsSlot::ItemRuleInfo::GetRuleInfo(&modifying_rule, *this,
            item_rule_info_.is_limited, &parse_type) == false)
    {
        return false;
    };

    const_cast<nsSlot::ItemRuleInfo&>(item_rule_info_) = modifying_rule;
    ZeroMemory(&time_info_, sizeof(time_info_));
    return true;
}

//--------------------------------------------------------------------------------------------------
// Limited Control
BOOL
SCItemSlot::DecreasingMaxDura()
{
    if (!IsLimited()) {
        return false;
    }
    int max_duration = shared_.limited_info_.max_duration_;
    if (max_duration <= 2) {
        return false;
    }
    max_duration -= 2;
    if (max_duration <= 0) {
        max_duration = 0;
    }
    shared_.limited_info_.max_duration_ = static_cast<DURATYPE>(max_duration);
    return true;
}

//--------------------------------------------------------------------------------------------------
// Limited Control
void
SCItemSlot::RepairMaxDura()
{
    if (item_rule_info_.is_limited == false) {
        assert(0);
        return;
    }
#ifdef __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT
    //üũ ���ϴ°ɷ� ����
#else
    if (GetLimitedRepairCount() <= 0) {
        return;
    }
#endif

    shared_.limited_info_.max_duration_ = shared_.limited_info_.max_duration_for_restore_;
#ifdef __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT
    //üũ ���ϴ°ɷ� ����
#else
    if (shared_.limited_info_.number_of_repairs_) {
        --shared_.limited_info_.number_of_repairs_;
    }
#endif
}

//--------------------------------------------------------------------------------------------------
// Limited Control
void
SCItemSlot::SetLimitValueRatio(int value)
{
    if (item_rule_info_.is_limited == false) {
        assert(0);
        return;
    }
    if (value < -100 || 400 < value) {
        return;
    }
    shared_.limited_info_.limit_value_ratio_ = value;
}

//--------------------------------------------------------------------------------------------------
// Limited Control
void
SCItemSlot::SetLimitedDuraMax(WORD value)
{
    if (item_rule_info_.is_limited == false) {
        assert(0);
        return;
    }
    if (value > 255) {
        return;
    }
    shared_.limited_info_.max_duration_ = DURATYPE(value);
}

//--------------------------------------------------------------------------------------------------
// Limited Control
void
SCItemSlot::SetLimitedDuraMMax(WORD value)
{
    if (item_rule_info_.is_limited == false) {
        assert(0);
        return;
    }
    if (value > 255) {
        return;
    }
    shared_.limited_info_.max_duration_for_restore_ = DURATYPE(value);
}

//--------------------------------------------------------------------------------------------------
// Limited Control
void
SCItemSlot::SetLimitedRepairCount(BYTE value)
{
    if (item_rule_info_.is_limited == false) {
        assert(0);
        return;
    }
    if (value > 15) {
        return;
    }
    // TODO(WAVERIX) ���� ȸ�� ������ 15�̴�... ������ ��.
    shared_.limited_info_.number_of_repairs_ = value;
}

//--------------------------------------------------------------------------------------------------
// Limited Control - ����� ��ġ
//������ ���� �ʿ俩��
BOOL
SCItemSlot::IsNeedToRepair()
{
    if (item_rule_info_.is_editable == false) {
        return false;
    }
    if (item_rule_info_.is_limited == false) {
        return false;
    }
    if (WithDura() == false) {
        return false;
    }
    const BYTE max_duration = GetMaxDura();
    if (max_duration == duration_ &&
        max_duration == GetLimitedDuraMMax())
    {
        return false;
    }

    return true;
}

//==================================================================================================
//  Fate Item Control
//
bool
SCItemSlot::CalFateInfo()
{
    if (IsFateItem() == false) {
        return false;
    }
    // (NOTE) �ϱ� ������ ���� Rule�˻簡 �̷����� ����
    //if (IsOverlap())
    //{
    //    ASSERT(FALSE);
    //    DISPMSG("Overlap Item must not CalFate : %d \n", item_code_);
    //    return FALSE;
    //}
    //ASSERT(IsFateItem());

    FateItemInfoParser* const pFateItemInfoParser = FateItemInfoParser::Instance();
    const sFATE_ITEM_INFO* pFateItemInfo =
        pFateItemInfoParser->GetFateItemInfoByRefItemCode(item_code_);

    if (!FlowControl::FCAssert(pFateItemInfo != NULL)) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|can't find lucky item info, code=%d|"),
               __FUNCTION__, item_code_);
        return false;
    }
    //

    SCFateInfo backup = shared_.fate_info_;
    if (!shared_.fate_info_.CalFate(item_code_, pFateItemInfo->m_dwIndex)) {
        return false;
    }

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
#else
    //���� Ȯ��
    WORD number_of_sockets = \
        pFateItemInfoParser->GetRandomkeyFromFateSocketRatio(pFateItemInfo->m_dwIndex);

    // (CHANGES) CalFate�� ���� Ʈ������� �����ϵ��� �����Ѵ�.
    if (!FlowControl::FCAssert(number_of_sockets <= SOCKET_MAX)) {
        // rollback
        shared_.fate_info_ = backup;
        return false;
    }

    SetSocketNum((BYTE)number_of_sockets);
#endif // _NA_000251_20100727_SOCKET_SYSTEM

    //������ - routing duration by item type
    SetDura(GetMaxDura());

    return true;
}

bool
SCItemSlot::CalFateDateTime(DWORD& OUT date_time)
{
    if (IsFateItem() == false) {
        return false;
    }
    // (NOTE) �ϱ� ������ ���� Rule�˻簡 �̷����� ����
    //if (IsOverlap())
    //{
    //    ASSERT(FALSE);
    //    DISPMSG("Overlap Item must not CalFate : %d \n", item_code_);
    //    return FALSE;
    //}
    //�̹� ���� ����.
    if (GetFateIdentify()) {
        return false;
    }
    FateItemInfoParser* const pFateItemInfoParser = FateItemInfoParser::Instance();
    const sFATE_ITEM_INFO* pFateItemInfo = \
        pFateItemInfoParser->GetFateItemInfoByRefItemCode(item_code_);
    //
    if (!FlowControl::FCAssert(pFateItemInfo != NULL)) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|can't find lucky item info, code=%d|"),
               __FUNCTION__, item_code_);
        return false;
    }
    // (NOTE) ...��ĳ ������ ��?
    date_time = pFateItemInfoParser->GetRandomkeyFromFateUsingTimeRatio(pFateItemInfo->m_dwIndex);

    return true;
}

void
SCItemSlot::SetFateAtkDefRatio(WORD value)
{
    if (item_rule_info_.is_fate == false) {
        assert(0);
        return;
    }
    if (value > 511) {
        value = 511;
    }
    shared_.fate_info_.ratio_ = value;
}

void
SCItemSlot::SetFateDuraMax(WORD value)
{
    if (item_rule_info_.is_fate == false) {
        assert(0);
        return;
    }

    const DURATYPE Max_Duration = DURATYPE(-1);
    BOOST_STATIC_ASSERT(Max_Duration == 255);
    if (value > Max_Duration) {
        value = Max_Duration;
    }
    shared_.fate_info_.max_duration_ = DURATYPE(value);
}

//  Fate Item Control
//==================================================================================================
//  NOption < ExtraStone Option
void
SCItemSlot::SetNOptionAttr(int iAttrValueIndex)
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_NOption)
    {
        if (item_info_->m_Code != 0) {
            n_option_info_.SetNOptionAttr(item_info_->m_wType, item_info_->m_LV, iAttrValueIndex);
        }
        return;
    }
    assert(0);
}
//
//==================================================================================================
//  Pet Item Control
//  (NOTE) �Ŀ�... �̸� �ʹ� Generic�ϴ�.
bool
SCItemSlot::StartQuantity()
{
    // (THINKING) Pet item�� ��, ����ϴµ�... Ÿ�̸Ӱ� ������ ���� �ְ�, �׷��� ���� ���� �ְ�...
    if (item_rule_info_.is_pet == false) {
        assert(0);
        return false;
    }

    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer)
    {
        assert(item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY);
        if (time_info_.is_expired_) {
            return false;
        }
        ASSERT(time_info_.date_time_ != 0);
        ASSERT(time_info_.last_checked_ == 0);
        time_info_.last_checked_ = GetTickCount();

        return true;
    }

    return true; // �ð� ���� ���� ���� ���
}

void
SCItemSlot::EndQuantity()
{
    if (item_rule_info_.is_pet == false) {
        assert(0);
        return;
    }

    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Timer)
    {
        assert(item_rule_info_.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY);
        if (time_info_.is_expired_) {
            return;
        }
        time_info_.is_expired_ = _UpdateTime();
        time_info_.last_checked_ = 0;
    }
}

//==================================================================================================
//  Rider & Rider Parts Item Control

//--------------------------------------------------------------------------------------------------

bool
SCItemSlot::GetRiderPartsInfo(eRIDER_PARTS_POS pos, SCRiderPartsData* OUT parts_data) const
{
    if (item_rule_info_.is_rider == false) {
        assert(0);
        return false;
    }

    // (returns) if exist then Acquire 'SCRiderPartsInfo'
    //                        { exist | empty (cleared stream), check 'parts_type_' }
    //           else     then return 0 { index out of range }
    const SCRiderPartsInfo* parts_info = shared_.rider_info_.GetPartsInfo(this, pos);
    if (parts_info == NULL) {
        return false;
    }
    *parts_data = *parts_info;
    return true;
}

bool
SCItemSlot::GetRiderPartsItem(eRIDER_PARTS_POS pos, SCItemSlot* OUT parts_item) const
{
    if (item_rule_info_.is_rider == false) {
        assert(0);
        return false;
    }

    const SCRiderPartsInfo* parts_info = shared_.rider_info_.GetPartsInfo(this, pos);
    if (parts_info == NULL) {
        return false;
    }
    if (parts_info->item_code_ == 0) {
        parts_item->Clear();
        return true;
    }

    if (SCRiderPartsInfo::ExtractToItem(*parts_info, parts_item) == false) {
        return false;
    }
    return true;
}

RC::eITEM_RESULT
SCItemSlot::CanEquipRiderParts(const SCItemSlot& rider_part_item)
{
    if (item_rule_info_.is_rider == false ||
        rider_part_item.item_rule_info_.is_valid_item == false)
    {
        assert(0);
        return RC::RC_ITEM_INVALID_TYPE;
    }

    if (rider_part_item.item_rule_info_.is_rider_parts == false) {
        return RC::RC_ITEM_INVALID_TYPE;
    }
    //     Locked                  Blocked
    if (lock_info_.locked_ || lock_info_.blocked_ ||
        lock_info_.user_blocked_ == false) // UserBlocked|RiderSummoned|PetLocked
    {
        // (NOTE) ������ �ƴϾ�� �ϰ�, ��ȯ�� ���¿��� �Ѵ�.(with Open RiderInventory)
        return RC::RC_ITEM_INVALIDPOS;
    }

    if (rider_part_item.lock_info_.locked_ || rider_part_item.lock_info_.blocked_ ||
        rider_part_item.lock_info_.user_blocked_) // UserBlocked
    {
        return RC::RC_ITEM_INVALIDPOS;
    }

    const BASE_ITEMINFO* parts_item_info = rider_part_item.item_info_;
    if (item_info_->m_RidingClass.value != parts_item_info->m_RidingClass.value) {
        return RC::RC_ITEM_CAN_NOT_EQUIP;
    }
    //SCRiderPartsInfo parts_info;
    //if (SCRiderPartsInfo::ExtractToPartsInfo(rider_part_item, &parts_info) == false)
    //    return RC::RC_ITEM_INVALID_TYPE;

    bool is_valid_pos = \
        eRIDER_PARTS_POS::StartIndex <= parts_item_info->m_wEqPos &&
                                        parts_item_info->m_wEqPos < eRIDER_PARTS_POS::EndIndex;
    if (!FlowControl::FCAssert(is_valid_pos)) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    eRIDER_PARTS_POS pos = { (BYTE)parts_item_info->m_wEqPos };
    RC::eITEM_RESULT result = shared_.rider_info_.CanEquipParts(*this, item_info_,
                                                                pos,
                                                                rider_part_item,
                                                                rider_part_item.item_info_);

    return result;
}

bool
SCItemSlot::EquipRiderParts(const SCItemSlot& rider_part_item)
{
    if (item_rule_info_.is_rider == false) {
        assert(0);
        return false;
    }

    if (rider_part_item.item_rule_info_.is_rider_parts == false)
    {
        assert(0);
        return false;
    }

    const BASE_ITEMINFO* parts_item_info = rider_part_item.item_info_;
    if (item_info_->m_RidingClass.value != parts_item_info->m_RidingClass.value) {
        return false;
    }
    //SCRiderPartsInfo parts_info;
    //if (SCRiderPartsInfo::ExtractToPartsInfo(rider_part_item, &parts_info) == false)
    //    return false;

    eRIDER_PARTS_POS pos = { (BYTE)parts_item_info->m_wEqPos };
    bool result = shared_.rider_info_.EquipParts(*this, item_info_,
        pos, rider_part_item, rider_part_item.item_info_);

    return result;
}

bool
SCItemSlot::CanUnEquipRiderParts(eRIDER_PARTS_POS pos, nsSlot::RiderUpdateBits* update_bits_ptr)
{
    if (item_rule_info_.is_rider == false) {
        assert(0);
        return false;
    }

    bool result = shared_.rider_info_.GetUnEquipChangedBits(*this, pos, update_bits_ptr);
    return result;
}

// int pos : eRIDER_PARTS_POS::eValue { 91, 92, 93 }
bool
SCItemSlot::UnEquipRiderParts(eRIDER_PARTS_POS pos, SCItemSlot* rider_part_item)
{
    // (WARNING) ���Ű� �ݵ�� �����ϱ� ���ؼ��� CanUnEquip�� �ʿ��� �Ŷ� ���δ�.
    rider_part_item->Clear();

    if (item_rule_info_.is_rider == false) {
        assert(0);
        return false;
    }

    //SCRiderPartsInfo parts_info;
    RC::eITEM_RESULT result = shared_.rider_info_.UnEquipParts(*this, pos, rider_part_item);
    if (result != RC::RC_ITEM_SUCCESS) {
        return false;
    }
    //if (SCRiderPartsInfo::ExtractToItem(parts_info, rider_part_item) == false) {
    //    assert(0);
    //    return false;
    //}

    return true;
}

nsSlot::RiderUpdateBits
SCItemSlot::RiderSummonStatus(BOOLEAN is_summoned) {
    nsSlot::RiderUpdateBits update_bits = { 0, };
    if (item_rule_info_.is_rider == false) {
        assert(0);
        return update_bits;
    }

    // DONE: update changed status
    BOOLEAN pre  = !!lock_info_.user_blocked_;
    BOOLEAN post = !!is_summoned;

    // (NOTE) update_bits(-1) {all_sub & all_add} command�� ���� ����ؼ� Ư���ϰ� ó���ұ�?
    // State diagram
    //   pre | post  | result
    //    0      0       X
    //    0      1     add_rider, add_parts(all) with filter 'timeout'
    //    1      0     sub_rider, sub_parts(all) with filter 'timeout'
    //    1      1       X
    if (pre < post) {
        update_bits = shared_.rider_info_.GetUpdateEnableInfo(*this, true);
    }
    else if (pre > post) {
        update_bits = shared_.rider_info_.GetUpdateEnableInfo(*this, false);
    }

    lock_info_.user_blocked_ = post;
    shared_.rider_info_.update_bits_ = update_bits;

    return update_bits;
}

//==================================================================================================
// implemented by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_

//--------------------------------------------------------------------------------------------------
// fields specification related to the 'Etheria'
// the below diagram show a state flow that
// a default item can change to the etheria and the shell
// - the shell is the empty item which is extracted to etheria
// (custom = { ItemCode=10001 (�ݼӾ�յ���) }
//-------------------- function : split to the etheria and the shell -------------------------------
// Axe  -|-> the shell
//       |-> the etheria
// same slot : Axe = the shell
// add slot  : the etheria
// same serial : Axe = the etheria (copied)
//                         Axe(->)        the shell              the etheria
// ItemCode                (10001)         (10001)                 (10001)
// Number                    (1)             (1)              (1) by the etheria
// ImageCode               (10001)         (10001)      (HardCode::ITEMCODE_of_ETHERIA)
// sDefault::ParseType  (eParse_Equip)  (eParse_Equip)  (eParse_Equip) same as default
// sDefault::Block          false           false            false by the etheria
// sDefault::UserBlock      false           false            false by the etheria
// sDefault::History       (None)          (Shell)                (Etheria)
//
//--------------------------- function : combined from the shell -----------------------------------
// the etheria  -|-> a combined item
// the shell    -|
// same slot : the shell = Axe
// del slot  : the etheria
//                         Axe(<-)        the shell              the etheria
// ItemCode                (10001)         (10002)                 (10001)
// Number                    (1)             (1)              (1) by the etheria
// ImageCode               (10002)         (10002)       (HardCode::ITEMCODE_of_ETHERIA)
// sDefault::ParseType  (eParse_Equip)   (eParse_Equip)  (eParse_Equip) same as default
// sDefault::Block          false           false                   false
// sDefault::UserBlock      false           false                   false
// sDefault::History      (Combined)       (Shell)                (Etheria)
//--------------------------------------------------------------------------------------------------
// the 'History' field specification, referenced by eEtheriaHistory
// value = { 0, 1, 2, 3 };
// {0} : None       - default spec.
// {1} : Combined   - the combination item, this item is combined from etheria
// {2} : Shell      - the shell item, this item is extracted options
// {3} : Etheria    - the etheria item, this item has contained all options of the item
//--------------------------------------------------------------------------------------------------
// Shared Block & normal item (no equipment)
//struct sDefault {
//    ulong ParseType;            //8
//
//    ulong Block;                //1
//    ulong UserBlock;            //1
//    ulong History;              //2
//};
// SerializeItemOption::sDefault::History
//--------------------------------------------------------------------------------------------------

// internal function in a code file.
// reset rules based on the value of a lock_info.history_.
bool SCItemSlot::ChangeImageItemRule(eEtheriaHistory::value_type history)
{
    lock_info_.history_.value = history;
    //
    nsSlot::ItemRuleInfo& item_rule_info = const_cast<nsSlot::ItemRuleInfo&>(item_rule_info_);
    item_rule_info.is_shell = false;
    item_rule_info.is_etheria = false;
    item_rule_info.is_editable = true;
    // (f100727.1L) add a rankup enable rule which is changed by a combined item
    bool exist_rankup_function = false;
    int rank_control_operation = 0; // -1:sub, 0:none, 1:add
    if (item_info_->item_rule_info.func_attr.value & item_rule_info.func_attr.eEnable_Rank) {
        exist_rankup_function = true;
    }
    switch (lock_info_.history_.value)
    {
    case lock_info_.history_.None:
        rank_control_operation = exist_rankup_function ? 1 : 0;
        break;
    case lock_info_.history_.Combined:
        // CHANGES, f100823.1L, fix up a data loss about rank information.
        // a combined item can't change a rank information, but can query it of a rank.
        rank_control_operation = exist_rankup_function ? 1 : 0;
        break;
    case lock_info_.history_.Shell:
        item_rule_info.is_shell = true;
        item_rule_info.is_editable = false;
        rank_control_operation = exist_rankup_function ? 1 : -1;
        break;
    case lock_info_.history_.Etheria:
        item_rule_info.is_etheria = true;
        item_rule_info.is_editable = false;
        rank_control_operation = exist_rankup_function ? 1 : 0;
        break;
    }
    // 0 is skipped.
    if (rank_control_operation == 0) {
    }
    else if (rank_control_operation < 0) {
        item_rule_info.func_attr.value &= ~item_rule_info.func_attr.eEnable_Rank;
    }
    else if (rank_control_operation > 0) {
        item_rule_info.func_attr.value |= item_rule_info.func_attr.eEnable_Rank;
    }
    return true;
}

bool SCItemSlot::ChangeImageItem(SLOTCODE image_item_code, eEtheriaHistory::value_type history)
{
    const BASE_ITEMINFO* image_item_info = NULL;
    if (image_item_code_ != 0 && image_item_code_ == image_item_code) {
        image_item_info = image_item_info_;
    }
    else
    {
        if (item_code_ != image_item_code) {
            ItemInfoParser* item_parser = ItemInfoParser::Instance();
            image_item_info = item_parser->GetItemInfo(image_item_code);
        }
        else {
            image_item_info = item_info_;
        }
    }
    if (image_item_info == NULL) {
        return false;
    }
    // change link
    image_item_code_ = image_item_code;
    image_item_info_ = image_item_info;
    // change rule
    // CHANGES, f100823.3L, the combined item, combining both same grade a shell and an etheria,
    // is not a combined type.
    if (history == lock_info_.history_.Combined) {
        if (item_info_ == image_item_info_) {
            history = lock_info_.history_.None;
        }
    }
    //if (history != lock_info_.history_.None) {
    ChangeImageItemRule(history);
    //}
    return true;
}

bool SCItemSlot::CanExtractToEtheria(RC::eITEM_RESULT* result) const
{
    struct ResultMaker {
        ResultMaker(RC::eITEM_RESULT* result)
            : inter_result_(RC::RC_ITEM_SUCCESS), result_(result) {}
        ~ResultMaker()
        {
            if (result_ != NULL) {
                *result_ = inter_result_;
            }
        }
        RC::eITEM_RESULT inter_result_;
        RC::eITEM_RESULT* result_;
    } result_maker(result);

    if (item_rule_info_.is_valid_item == false) {
        result_maker.inter_result_ = RC::RC_ITEM_INVALIDSTATE;
        return false;
    }
    if ((item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Etherealize) == 0) {
        result_maker.inter_result_ = RC::RC_ITEM_CANT_EXTRACT_TO_ETHERIA;
        return false;
    }
    // history control, if the shell item can't use this function
    // enabled  = eEtheriaHistory::None || eEtheriaHistory::Combined
    // disabled = eEtheriaHistory::Shell
    if (lock_info_.history_.value == lock_info_.history_.Shell) {
        result_maker.inter_result_ = RC::RC_ITEM_ALREADY_EXTRACTED_TO_ETHERIA;
        return false;
    }
    if (lock_info_.blocked_ || lock_info_.locked_ || lock_info_.user_blocked_) {
        result_maker.inter_result_ = RC::RC_ITEM_INVALIDPOS;
        return false; // passed if the case calling in the ExtractToEtheria function
    }
    // NOTE: f101112.4L, add a rule whether a lucky item is identified.
    if (item_rule_info_.is_fate) {
        if (GetFateIdentify() == false) {
            result_maker.inter_result_ = RC::RC_ITEM_CANT_EXTRACT_TO_ETHERIA;
            return false;
        }
    }
    if (this->IsDestroyItem() != false) {
        result_maker.inter_result_ = RC::RC_ITEM_CANT_EXTRACT_TO_ETHERIA;
        return false;
    }
    // debug block
    if (image_item_code_ == HardCode::ITEMCODE_of_ETHERIA) {
        assert(!"the etheria item can't convert to an another etheria item");
        result_maker.inter_result_ = RC::RC_ITEM_INVALID_TYPE;
        return false;
    }
#ifdef _DEBUG
    if (WithDura() == false)
    {
        DURATYPE number_of_items = GetNum();
        if (number_of_items != 1) {
            result_maker.inter_result_ = RC::RC_ITEM_INVALID_TYPE;
            return false;
        }
    }
#endif
    return true;
}

// this -|-> the shell item
//       |-> the etheria item (from making_item)
RC::eITEM_RESULT SCItemSlot::ExtractToEtheria(SCItemSlot* making_item)
{
    if (lock_info_.blocked_ || lock_info_.locked_ || lock_info_.user_blocked_) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    RC::eITEM_RESULT item_result;
    if (CanExtractToEtheria(&item_result) == false) {
        return item_result; // can't use this function
    }
    // if the below logic is failed, rollback process is needed
    /* RESERVED_SECTION
    struct RollbackData {
        RollbackData(eEtheriaHistory* history)
            : need_rollback_(false)
            , history_(history)
        {
            source_history_ = *history_;
        }
        ~RollbackData()
        {
            if (need_rollback_ == false) {
                return;
            }
            *history_ = source_history_;
        }
        bool need_rollback_;
        eEtheriaHistory source_history_;
        eEtheriaHistory* history_;
    } rollback_data(&this->lock_info_.history_);
    */
    const SLOTCODE item_code = item_code_;
    const SLOTCODE image_item_code = image_item_code_;
    ITEMSTREAMEX source_stream;
    ITEMPART& item_part = source_stream.Part;
    ZeroMemory(&source_stream, sizeof(source_stream));
    this->CopyOut(source_stream);
    ;{  // make clone item
        making_item->Clear();
        making_item->SetCode(item_code);
        DURATYPE duration = item_part.byDura;
        if (making_item->WithDura()) {
            making_item->SetDura(duration);
        }
        else {
            making_item->SetNum(duration);
        }
        making_item->CopyPart(source_stream.OptionPart);
    };
    ;{  // the making_item is changed to etheria type
        //const_cast<nsSlot::ItemRuleInfo&>(making_item->item_rule_info_).is_etheria = true;
        making_item->ChangeImageItem(HardCode::ITEMCODE_of_ETHERIA,
                                     making_item->lock_info_.history_.Etheria);
    };
    ;{  // make to the shell item using this object
        // if the process is failed, do roll-backed.
        item_part.wCode = image_item_code;
        item_part.byDura = 1;
        // (f100816.3L) fix up an invalid position which is occurred on change an item status.
        const SLOTIDX slot_index = this->GetSlotIdx();
        const POSTYPE slot_pos = this->GetPos();
        this->Clear();
        this->CopyPart(item_part);
        this->SetPos(slot_pos);
        this->SetSlotIdx(slot_index);
        this->ChangeImageItemRule(lock_info_.history_.Shell);
    };
    return RC::RC_ITEM_SUCCESS;
}

// this check routine should be called by the shell item.
bool SCItemSlot::CanCombineWithEtheria(RC::eITEM_RESULT* result) const
{
    struct ResultMaker {
        ResultMaker(RC::eITEM_RESULT* result)
            : inter_result_(RC::RC_ITEM_SUCCESS), result_(result) {}
        ~ResultMaker()
        {
            if (result_ != NULL) {
                *result_ = inter_result_;
            }
        }
        RC::eITEM_RESULT inter_result_;
        RC::eITEM_RESULT* result_;
    } result_maker(result);

    if (item_rule_info_.is_valid_item == false) {
        result_maker.inter_result_ = RC::RC_ITEM_INVALIDSTATE;
        return false;
    }
    if (item_rule_info_.is_shell == false ||
        (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Etherealize) == 0)
    {
        result_maker.inter_result_ = RC::RC_ITEM_CANT_COMBINED_WITH_ITEM;
        return false;
    }
    // history control, if the shell item can't use this function
    // disabled = eEtheriaHistory::None || eEtheriaHistory::Combined
    // enabled  = eEtheriaHistory::Shell
    if (lock_info_.history_.value != lock_info_.history_.Shell) {
        result_maker.inter_result_ = RC::RC_ITEM_CANT_COMBINED_WITH_ITEM;
        return false;
    }
    if (lock_info_.blocked_ || lock_info_.locked_ || lock_info_.user_blocked_) {
        result_maker.inter_result_ = RC::RC_ITEM_INVALIDPOS;
        return false; // passed if the case calling in the CombineWithEtheria function
    }
    if (this->IsDestroyItem() != false) {
        result_maker.inter_result_ = RC::RC_ITEM_CANT_COMBINED_WITH_ITEM;
        return false;
    }
    // debug block
    if (image_item_code_ == HardCode::ITEMCODE_of_ETHERIA) {
        assert(!"the etheria item can't combine with an another etheria item");
        result_maker.inter_result_ = RC::RC_ITEM_INVALID_TYPE;
        return false;
    }
#ifdef _DEBUG
    // debug block, must be passed if the process step has been reached this point
    if (WithDura() == false)
    {
        DURATYPE number_of_items = GetNum();
        if (number_of_items != 1) {
            result_maker.inter_result_ = RC::RC_ITEM_INVALID_TYPE;
            return false;
        }
    }
#endif
    return true;
}

// this check routine should be called by the etheria item.
bool SCItemSlot::CanCombineWithShell(RC::eITEM_RESULT* result) const
{
    struct ResultMaker {
        ResultMaker(RC::eITEM_RESULT* result)
            : inter_result_(RC::RC_ITEM_SUCCESS), result_(result) {}
        ~ResultMaker()
        {
            if (result_ != NULL) {
                *result_ = inter_result_;
            }
        }
        RC::eITEM_RESULT inter_result_;
        RC::eITEM_RESULT* result_;
    } result_maker(result);

    if (item_rule_info_.is_valid_item == false) {
        result_maker.inter_result_ = RC::RC_ITEM_INVALIDSTATE;
        return false;
    }
    if (item_rule_info_.is_etheria == false) {
        result_maker.inter_result_ = RC::RC_ITEM_CANT_COMBINED_WITH_ITEM;
        return false;
    }
    if (lock_info_.blocked_ || lock_info_.locked_ || lock_info_.user_blocked_) {
        result_maker.inter_result_ = RC::RC_ITEM_INVALIDPOS;
        return false; // passed if the case calling in the CombineWithEtheria function
    }
    // debug block
    if (image_item_code_ != HardCode::ITEMCODE_of_ETHERIA) {
        assert(!"the etheria item can't combine with an another etheria item");
        result_maker.inter_result_ = RC::RC_ITEM_INVALID_TYPE;
        return false;
    }
    return true;
}

// this(the shell item) -|-> the combination item
// the etheria item     -|-> post action=delete
// the etheria item should be cleared, no control in this slot to logic control
RC::eITEM_RESULT SCItemSlot::CombineWithEtheria(const SCItemSlot& etheria_item)
{
    if (lock_info_.blocked_ || lock_info_.locked_ || lock_info_.user_blocked_) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    RC::eITEM_RESULT item_result;
    if (etheria_item.CanCombineWithShell(&item_result) == false) {
        return item_result;
    }
    if (this->CanCombineWithEtheria(&item_result) == false) {
        return item_result; // can't use this function
    }
    const BASE_ITEMINFO* etheria_item_info = etheria_item.item_info_;
    if (item_info_->m_wType != etheria_item_info->m_wType) {
        return RC::RC_ITEM_CANT_COMBINED_WITH_DIFFERENT_ITEM;
    }
    const bool accept_class = \
        (item_info_->m_wEqClass1 && item_info_->m_wEqClass1 == etheria_item_info->m_wEqClass1) ||
        (item_info_->m_wEqClass2 && item_info_->m_wEqClass2 == etheria_item_info->m_wEqClass2) ||
        (item_info_->m_wEqClass3 && item_info_->m_wEqClass3 == etheria_item_info->m_wEqClass3) ||
        (item_info_->m_wEqClass4 && item_info_->m_wEqClass4 == etheria_item_info->m_wEqClass4) ||
        (item_info_->m_wEqClass5 && item_info_->m_wEqClass5 == etheria_item_info->m_wEqClass5) || 
        //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        (item_info_->m_wEqClass7 && item_info_->m_wEqClass7 == etheria_item_info->m_wEqClass7) || 
        //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        (item_info_->m_wEqClass8 && item_info_->m_wEqClass8 == etheria_item_info->m_wEqClass8) || 
        // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        (item_info_->m_wEqClass6 && item_info_->m_wEqClass6 == etheria_item_info->m_wEqClass6);
    if (accept_class == false) {
        return RC::RC_ITEM_CANT_COMBINED_WITH_DIFFERENT_ITEM;
    }
    // if the below logic is failed, rollback process is needed
    /* RESERVED_SECTION
    struct RollbackData {
        RollbackData(eEtheriaHistory* history)
            : need_rollback_(false)
            , history_(history)
        {
            source_history_ = *history_;
        }
        ~RollbackData()
        {
            if (need_rollback_ == false) {
                return;
            }
            *history_ = source_history_;
        }
        bool need_rollback_;
        eEtheriaHistory source_history_;
        eEtheriaHistory* history_;
    } rollback_data(&this->lock_info_.history_);
    */
    const SLOTCODE shell_item_code = image_item_code_;
    ITEMPART shell_item_part;
    // (f100816.3L) fix up an invalid position which is occurred on change an item status.
    const SLOTIDX slot_index = this->GetSlotIdx();
    const POSTYPE slot_pos = this->GetPos();
    ;{  // clear the shell item
        ZeroMemory(&shell_item_part, sizeof(shell_item_part));
        this->CopyOutPart(shell_item_part);
        this->Clear();
    };
    const SLOTCODE etheria_item_code = etheria_item.item_code_;
    ITEMSTREAMEX etheria_stream;
    ZeroMemory(&etheria_stream, sizeof(etheria_stream));
    ;{  // get option block & copy from the etheria item
        //OPTIONPART& etheria_option = etheria_stream.OptionPart;
        etheria_item.CopyOut(etheria_stream);
        shell_item_part.wCode = etheria_item_code;
        shell_item_part.byDura = etheria_stream.Part.byDura;
        this->CopyPart(shell_item_part);
        this->CopyPart(etheria_stream.OptionPart);
        this->SetPos(slot_pos);
        this->SetSlotIdx(slot_index);
    };
    ;{  // make to the combination item
        this->ChangeImageItem(shell_item_code, lock_info_.history_.Combined);
    };
    return RC::RC_ITEM_SUCCESS;
}

// it is operated internally. an item object with options is changed to a default item.
bool SCItemSlot::ChangeToShellItem()
{
    if (item_rule_info_.is_valid_item == false ||
        item_rule_info_.is_shell != false ||
        item_rule_info_.is_etheria)
    {
        return false;
    }
    if ((item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Etherealize) == 0) {
        return false;
    }
    if (lock_info_.blocked_ || lock_info_.locked_ || lock_info_.user_blocked_) {
        return false; // passed if the case calling in the CombineWithEtheria function
    }
    // debug block
    if (image_item_code_ == HardCode::ITEMCODE_of_ETHERIA) {
        assert(!"the etheria item can't combine with an another etheria item");
        return false;
    }
    // history control, if the shell item can't use this function
    // disabled = eEtheriaHistory::None || eEtheriaHistory::Combined
    // enabled  = eEtheriaHistory::Shell
    if (!(lock_info_.history_.value == lock_info_.history_.None ||
          lock_info_.history_.value == lock_info_.history_.Combined))
    {
        return false;
    }
#ifdef _DEBUG
    // debug block, must be passed if the process step has been reached this point
    if (WithDura() == false)
    {
        DURATYPE number_of_items = GetNum();
        if (number_of_items != 1) {
            return false;
        }
    }
#endif
    const SLOTCODE item_code = image_item_code_;
    const SLOTIDX slot_index = this->GetSlotIdx();
    const POSTYPE slot_pos = this->GetPos();
    ;{  // change to the shell item
        ITEMPART item_part;
        ZeroMemory(&item_part, sizeof(item_part));
        this->CopyOutPart(item_part);
        this->Clear();
        item_part.wCode = item_code;
        this->CopyPart(item_part);
        this->SetPos(slot_pos);
        this->SetSlotIdx(slot_index);
    };
    ;{  // change attributes
        ChangeImageItem(item_code, lock_info_.history_.Shell);
    };
    return true;
}

// it is operated internally. an item object with options is changed to an etheria item which
// is equal to the base item except an image link and an uncorrectable mode (is_editable == false)
bool SCItemSlot::ChangeToEtheriaItem()
{
    if (item_rule_info_.is_valid_item == false ||
        item_rule_info_.is_shell ||
        item_rule_info_.is_etheria != false)
    {
        return false;
    }
    if ((item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Etherealize) == 0) {
        return false;
    }
    if (lock_info_.blocked_ || lock_info_.locked_ || lock_info_.user_blocked_) {
        return false; // passed if the case calling in the CombineWithEtheria function
    }
    // debug block
    if (image_item_code_ == HardCode::ITEMCODE_of_ETHERIA) {
        assert(!"the etheria item can't combine with an another etheria item");
        return false;
    }
    // history control, if the shell item can't use this function
    // disabled = eEtheriaHistory::None || eEtheriaHistory::Combined
    // enabled  = eEtheriaHistory::Shell
    if (!(lock_info_.history_.value == lock_info_.history_.None ||
          lock_info_.history_.value == lock_info_.history_.Combined))
    {
        return false;
    }
#ifdef _DEBUG
    // debug block, must be passed if the process step has been reached this point
    if (WithDura() == false)
    {
        DURATYPE number_of_items = GetNum();
        if (number_of_items != 1) {
            return false;
        }
    }
#endif
    ;{  // change to the etheria item
        ChangeImageItem(HardCode::ITEMCODE_of_ETHERIA, lock_info_.history_.Etheria);
    };
    return true;
}

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
bool SCItemSlot::CreateSocketItem(const BYTE number_of_socket, const bool is_recreate)
{
    if (number_of_socket < 0 || number_of_socket > SOCKET_MAX)
    {
        return false;
    }

    const nsSlot::ItemRuleInfo& item_rule_info = GetItemRuleInfo();
    if (item_rule_info.IsEnableSocket() == false)
    {
        return false;
    }

    // ���� ä��Ⱑ �Ұ����ϴٸ� ������ �������� �ʴ´�.
    if (CanUseMixture(MIXTURE_SOCKET_HOLE_FILL) == false) 
    {
        if (is_recreate)
        {
            // ���� ä��Ⱑ �Ұ����ѵ� ������ ���� ������ �õ��ϴ°��� �������� �ʴ�.
            return false;
        }
        SetSocketIdentifyType(eSOCKET_FAIL_IDENTIFY_PROCESS);
        return true;
    }

    const eSOCKET_IDENTIFY_TYPE state = GetSocketIdentifyType();
    if (is_recreate == false && state != eSOCKET_IDENTIFY_NOT_PROCESS)
    {
        return false;
    }

    // ���� ������ �ԷµǾ�����
    if (number_of_socket > 0)
    {
        // �ٽ� �����Ѵٸ� ���� ���� ������ �����Ѵ�.
        if (is_recreate)
        {
            socket_info_.Clear();
        }
        // �Է��� ������ŭ ������ �����Ѵ�.
        SetSocketNum(number_of_socket);
        SetSocketIdentifyType(eSOCKET_IDENTIFIED_ITEM);
        return true;
    }

    SocketItemRatioParser* const parser = SocketItemRatioParser::Instance();
    const BASE_ITEMINFO* item_info = GetItemInfo();
    const SocketItemRatioInfo::RatioType::Value ratio_type = 
        parser->GetRatioType(item_info->m_wType, is_recreate);
    const SocketItemRatioInfo* const ratio = parser->FindSocketRatio(ratio_type, item_info->m_LV);
    if (ratio == NULL)
    {
        return false;
    }

    // �����ϰ� ���� ������ ���� �ʱ�ȭ�Ѵ�.
    socket_info_.Clear();
    
    const int seed = random(1, SocketItemRatioInfo::kMaxRatio);
    if (seed <= ratio->socket_creation_ratio_)
    {
        SetSocketIdentifyType(eSOCKET_UNIDENTIFIED_ITEM);
    }
    else
    {
        SetSocketIdentifyType(eSOCKET_FAIL_IDENTIFY_PROCESS);
    }

    return true;
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
BYTE SCItemSlot::GetDiceCastType()
{
    return dicecast_type_;
}

void SCItemSlot::SetDiceCastType(BYTE _type)
{
    dicecast_type_ = _type;
}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

bool SCItemSlot::EnableRandomize() const
{
#ifdef _NA_003740_20111122_RANDOM_ITEM
    if (item_rule_info_.IsEnableRandomize() == false)
    {
        return false;
    }
    if (item_info_ == NULL)
    {
        return false;
    }
    return random_info_.EnableRandomize(*item_info_);
#else
    return false;
#endif
}

#ifdef _NA_003740_20111122_RANDOM_ITEM
bool SCItemSlot::Randomize(const RandomValueTypeGrade::Value value_type_grade)
{
    if (EnableRandomize() == false)
    {
        return false;
    }
    if (item_info_ == NULL)
    {
        return false;
    }
    if (random_info_.Randomize(value_type_grade, *item_info_) == false)
    {
        return false;
    }
    if (random_info_.CreateItemInfo(*item_info_) == false)
    {
        return false;
    }
    return true;
}

RandomValueGrade::Value SCItemSlot::GetRandomOptionGrade(const uint8_t index) const
{
    if (EnableRandomize() == false)
    {
        return RandomValueGrade::kNone;
    }
    return random_info_.GetRandomOptionGrade(index);
}
#endif // _NA_003740_20111122_RANDOM_ITEM

//--------------------------------------------------------------------------------------------------
SCItemSlot::EditModeTransaction::EditModeTransaction(SCItemSlot* item_slot)
    : item_slot_(item_slot),
      first_value_(item_slot->item_rule_info_.is_editable != false)
{
    const_cast<nsSlot::ItemRuleInfo&>(item_slot_->item_rule_info_).is_editable = !first_value_;
}

SCItemSlot::EditModeTransaction::~EditModeTransaction()
{
    const_cast<nsSlot::ItemRuleInfo&>(item_slot_->item_rule_info_).is_editable = first_value_;
}
//const bool first_value_; // to support toggle
//SCItemSlot* const item_slot_;


//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
