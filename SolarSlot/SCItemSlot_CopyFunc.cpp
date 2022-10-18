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

//==================================================================================================

//==================================================================================================
//
//__NA000000_090906_SOLAR_SLOT_CODE_ARRANGEMENT__

//-------------------------------------------------------------------------------------------------------------------
// START ------------------------------------------------------------------------------------------------------------
// 내부적으로 사용할 셋팅 함수
// COPY
void
SCItemSlot::CopyPart(const ITEMPART& IN rPart)
{
    Clear();

    m_byType = rPart.byType;
    m_dwSerial = rPart.dwSerial;
    // side-effect - set code + set item info
    SetCode(rPart.wCode);

    if (WithDura()) {
        SetDura(rPart.byDura);
    }
    else {
        SetNum(rPart.byDura);
    }
}

void
SCItemSlot::CopyPart(const RENDER_ITEMPART& IN rPart)
{
    Clear();

    SetCode(rPart.wCode);

    if (WithDura()) {
        SetDura(rPart.byDura);
    }
    else {
        SetNum(rPart.byDura);
    }
}

void
SCItemSlot::CopyPart(const OPT_ITEMPART& IN rPart)
{
    Clear();

    SetCode(rPart.wCode);
}

void
SCItemSlot::CopyPart(const RENDER_OPTIONPART& IN rPart)
{
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Divine) {
        SetSet(rPart.Set);
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Enchant) {
        SetEnchant(rPart.Enchant);
    }
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Awakening) {
        SetAwakening(rPart.Awakening);
    }
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    //switch(GetItemInfo()->m_ChargeSubType)
    //{
    //case eCHARGE_SUB_COORDINATE:
    //    {
    //    }break;
    //default:
    //    {
    //        SetEnchant(rPart.Enchant);
    //    }break;
    //}
    if (item_rule_info_.is_weapon) {
        SetEquipEtherDevice(rPart.bEquipDevice);
        SetEtherBulletIndex(rPart.byBulletIndex);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 내부적으로 사용할 셋팅 함수
// COPYOUT
void
SCItemSlot::CopyOutPart(ITEMPART& OUT rPart) const
{
    rPart.byType        = GetType();
    rPart.dwSerial      = GetSerial();
    rPart.wCode         = GetItemCode();
    if (WithDura()) {
        rPart.byDura    = GetDura();
    }
    else {
        rPart.byDura    = GetNum();
    }
}

void
SCItemSlot::CopyOutPart(RENDER_ITEMPART& OUT rPart) const
{
    rPart.wCode         = GetItemCode();
    if (WithDura()) {
        rPart.byDura    = GetDura();
    }
    else {
        rPart.byDura    = GetNum();
    }
}

void
SCItemSlot::CopyOutPart(OPT_ITEMPART& OUT rPart) const
{
    rPart.wCode = image_item_code_;
}

void
SCItemSlot::CopyOutPart(RENDER_OPTIONPART& OUT rPart) const
{
    ZeroMemory(&rPart, sizeof(rPart));
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Divine) {
        rPart.Set = GetSet();
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Enchant) {
        rPart.Enchant = GetEnchant();
    }
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Awakening) {
        rPart.Awakening = GetAwakening();
    }
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    //switch(GetItemInfo()->m_ChargeSubType)
    //{
    //case eCHARGE_SUB_COORDINATE:
    //    {
    //    }break;
    //default:
    //    {
    //        rPart.Enchant = GetEnchant();
    //    }break;
    //}

    if (item_rule_info_.is_weapon) {
        rPart.bEquipDevice = equip_info_.is_equip_ether_device_;
        rPart.byBulletIndex =  equip_info_.ether_bullet_index_;
    }
}

void
SCItemSlot::Copy(SCSlot& IN slot)
{
    SCSlot::Copy(slot);
    SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(slot);
#ifdef _NA_003740_20111122_RANDOM_ITEM
    random_info_.Copy(rItemSlot.random_info_);
#endif
    CopyMemory(item_slot_point_holder_start_, rItemSlot.item_slot_point_holder_start_,
        static_cast<size_t>(&item_slot_point_holder_last_[2] - item_slot_point_holder_start_));
}

void
SCItemSlot::Copy(BitStream& IN stream, eSLOT_SERIALIZE eType)
{
    switch(eType)
    {
    case eSLOT_SERIALIZE_CLIENT_SAVE: Copy(stream); break;
    case eSLOT_SERIALIZE_RENDER_SAVE: Copy_Render(stream); break;
    }
}

void
SCItemSlot::CopyOut(BitStream& OUT stream, eSLOT_SERIALIZE eType)
{
    switch(eType)
    {
    case eSLOT_SERIALIZE_CLIENT_LOAD: CopyOut(stream); break;
    case eSLOT_SERIALIZE_RENDER_LOAD: CopyOut_Render(stream); break;
    }
}

void
SCItemSlot::Copy_Render(BitStream& IN bitstream)
{
    Clear();

    SLOTCODE item_code = 0;
    bitstream.Read(&item_code, 2*8);
    SetCode(item_code);

    if (!FlowControl::FCAssert(item_rule_info_.is_valid_item)) {
        return;
    }
    // render control
    BOOLEAN is_limited = 0;
    bitstream.Read(&is_limited, 1);
    if (is_limited != 0) {
        const_cast<nsSlot::ItemRuleInfo&>(item_rule_info_).is_limited = true;
        equip_info_.is_limited_ = true;
        assert(item_rule_info_.is_armor);
    }

    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Enchant) {
        bitstream.Read(&equip_info_.enchant_grade_, 4);
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Divine) {
        bitstream.Read(&equip_info_.is_divine_, 1);
    }
    if (item_rule_info_.is_weapon) {
        bitstream.Read(&equip_info_.is_equip_ether_device_, 1);
        if (equip_info_.is_equip_ether_device_) {
            bitstream.Read(&equip_info_.ether_bullet_index_, 4);
        }
    }
    //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    eEtheriaHistory::value_type history = lock_info_.history_.None;
    bitstream.Read(&history, 2);
    SLOTCODE image_item_code = 0;
    if (history == lock_info_.history_.Combined) {
        bitstream.Read(&image_item_code, 16);
    }
    else if (history == lock_info_.history_.Etheria) {
        image_item_code = HardCode::ITEMCODE_of_ETHERIA;
    }
    else {
        image_item_code = item_code;
    }
    ChangeImageItem(image_item_code, history);

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Awakening) {
        bitstream.Read(&equip_info_.awakening_grade_, 4);
    }
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
}

void
SCItemSlot::CopyOut_Render(BitStream& OUT bitstream)
{
    bitstream.Write(&item_code_, 2*8);
    if (!FlowControl::FCAssert(item_rule_info_.is_valid_item)) {
        return;
    }
    BOOLEAN is_limited = equip_info_.is_limited_;
    bitstream.Write(&is_limited, 1);

    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Enchant) {
        bitstream.Write(&equip_info_.enchant_grade_, 4);
    }
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Divine) {
        bitstream.Write(&equip_info_.is_divine_, 1);
    }
    if (item_rule_info_.is_weapon) {
        bitstream.Write(&equip_info_.is_equip_ether_device_, 1);
        if (equip_info_.is_equip_ether_device_) {
            bitstream.Write(&equip_info_.ether_bullet_index_, 4);
        }
    }
    //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    eEtheriaHistory::value_type history = lock_info_.history_.value;
    bitstream.Write(&history, 2);
    if (history == lock_info_.history_.Combined) {
        bitstream.Write(&image_item_code_, 16);
    }

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Awakening) {
        bitstream.Write(&equip_info_.awakening_grade_, 4);
    }
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
}

//
//[고정]
//아이템 코드(2*8)
//아이템 개수혹은내구도(1*8)
//블락여부(1)
//[가변1]
//인첸트여부(1)+인첸트값(4)/랭크값(4)+랭크옵션(6*9)/소켓개수(2)+소켓옵션(7*3)/디바인(2?)/N옵션(3)
//[가변2] : eCHARGE_SUB_TYPE 이 eCHARGE_SUB_FIXED_AMOUNT(일정기간제)
//인첸트여부(1)+인첸트값(4)/날짜(58)/소켓개수(2)+소켓옵션(7*3)/디바인(2?)/N옵션(3)
//[가변3] : eCHARGE_SUB_TYPE 이 eCHARGE_SUB_FIXED_QUANTITY(사용시 차감)
//인첸트여부(1)+인첸트값(4)/날짜(58)/소켓개수(2)+소켓옵션(7*3)/디바인(2?)/N옵션(3)
void
SCItemSlot::Copy(BitStream& IN bitstream)
{
    Clear();

    SLOTCODE item_code = 0;
    bitstream.Read(&item_code, 2*8);
    SetCode(item_code);

    DURATYPE duration_or_number = 0;
    bitstream.Read(&duration_or_number, 1*8);

    if (WithDura()) {
        SetDura(duration_or_number);
    }
    else {
        SetNum(duration_or_number);
    }

    // Read OPTIONPART
    OPTIONPART option_part;
    ZeroMemory(&option_part, sizeof(option_part));

    const int ParseTypeNumberOfBits = sizeof(option_part.ParseType) * 8;
    bitstream.Read(&option_part.ParseType, ParseTypeNumberOfBits);
    // (WARNING) 바로 위에서 미리 읽는 부분 때문에 감산이 필요하다.
    int used_bits = _SerializeUsedBits(option_part.ParseType) - ParseTypeNumberOfBits;
    bitstream.Read(option_part.Variable, used_bits);

    CopyPart(option_part);
}

void
SCItemSlot::CopyOut(BitStream& OUT bitstream)
{
    // ITEMPART
    bitstream.Write(&item_code_, 2*8);
    if (WithDura()) {
        bitstream.Write(&duration_, 1*8);
    }
    else {
        DURATYPE number = GetNum();
        bitstream.Write(&number, 1*8);
    }
    OPTIONPART option_part;
    CopyOutPart(option_part);
    //const int ParseTypeNumberOfBits = sizeof(option_part.ParseType) * 8;
    int used_bits = _SerializeUsedBits(option_part.ParseType); //읽을 때는 미리 읽는 부분 존재. 여기서는 불필요
    bitstream.Write(&option_part, used_bits);
}

void SCItemSlot::CopyPart(const OPTIONPART& option_part)
{
    if (_SerializeFromItemStream(item_code_, &option_part) == false) 
    {
        return;
    }
    
    nsSlot::ItemRuleInfo item_rule;
    if (nsSlot::ItemRuleInfo::GetRuleInfo(
        &item_rule, 
        *this,
        (option_part.ParseType == ItemParseType::kLimited),
        &(option_part.ParseType)
        ) == false)
    {
        return;
    }
    const_cast<nsSlot::ItemRuleInfo&>(item_rule_info_) = item_rule;
        
    if (item_rule.is_limited)
    {
        const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
        const nsSlot::SerializeItemOption::sLimited& root = item_option->Limited;
        lock_info_.SetInfo(root, item_info_);
        equip_info_.SetInfo(root, item_info_);
        n_option_info_.SetInfo(root, item_info_);
        socket_info_.SetInfo(root, item_info_);
        shared_.limited_info_.SetInfo(
            const_cast<nsSlot::SerializeItemOption::sLimited*>(&root), item_info_);
        //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        SLOTCODE image_item_code \
            = root.ImageCode ? static_cast<SLOTCODE>(root.ImageCode)
            : lock_info_.history_.value == lock_info_.history_.Etheria ? HardCode::ITEMCODE_of_ETHERIA
            : item_code_;
        ChangeImageItem(image_item_code, lock_info_.history_.value);
    #ifdef _NA_003740_20111122_RANDOM_ITEM
        random_info_.SetInfo(root, item_info_);
    #endif // _NA_003740_20111122_RANDOM_ITEM
    }
    else if (item_rule.is_fate)
    {
        const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
        const nsSlot::SerializeItemOption::sFate& root = item_option->Fate;
        lock_info_.SetInfo(root, item_info_);
        equip_info_.SetInfo(root, item_info_);
        time_info_.SetInfo(root, item_info_);
        n_option_info_.SetInfo(root, item_info_);
        socket_info_.SetInfo(root, item_info_);
        shared_.fate_info_.SetInfo(
            const_cast<nsSlot::SerializeItemOption::sFate*>(&root), item_info_);
        //_NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM
        SLOTCODE image_item_code \
            = root.ImageCode ? static_cast<SLOTCODE>(root.ImageCode)
            : lock_info_.history_.value == lock_info_.history_.Etheria ? HardCode::ITEMCODE_of_ETHERIA
            : item_code_;
        ChangeImageItem(image_item_code, lock_info_.history_.value);
    #ifdef _NA_003740_20111122_RANDOM_ITEM
        random_info_.SetInfo(root, item_info_);
    #endif // _NA_003740_20111122_RANDOM_ITEM
    }
    else if (item_rule.is_lottery)
    {
        const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
        const nsSlot::SerializeItemOption::sLottery& root = item_option->Lottery;
        lock_info_.SetInfo(root, item_info_);
        shared_.lotto_info_.SetInfo(
            const_cast<nsSlot::SerializeItemOption::sLottery*>(&root), item_info_);
    }
    else if (item_rule.is_pet)
    {
        const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
        const nsSlot::SerializeItemOption::sPet& root = item_option->Pet;
        lock_info_.SetInfo(root, item_info_);
        time_info_.SetInfo(root, item_info_);
        shared_.pet_info_.SetInfo(
            const_cast<nsSlot::SerializeItemOption::sPet*>(&root), item_info_);
        //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        SLOTCODE image_item_code \
            = root.ImageCode ? static_cast<SLOTCODE>(root.ImageCode)
            : lock_info_.history_.value == lock_info_.history_.Etheria ? HardCode::ITEMCODE_of_ETHERIA
            : item_code_;
        ChangeImageItem(image_item_code, lock_info_.history_.value);
    }
    else if (item_rule.is_waypoint)
    {
        const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
        const nsSlot::SerializeItemOption::sWayPoint& root = item_option->WayPoint;
        lock_info_.SetInfo(root, item_info_);
        time_info_.SetInfo(root, item_info_);
        shared_.waypoint_info_.SetInfo(
            const_cast<nsSlot::SerializeItemOption::sWayPoint*>(&root), item_info_);
    }
    else if (item_rule.is_coord)
    {
        const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
        const nsSlot::SerializeItemOption::sCoord& root = item_option->Coord;
        lock_info_.SetInfo(root, item_info_);
        shared_.coordinate_info_.SetInfo(root, item_info_);
    }
    else if (item_rule.is_rider)
    {
        // already checked.
        assert(item_rule.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART ||
               item_rule.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART_NOTIMER);

        const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
        const nsSlot::SerializeItemOption::sRider& root = item_option->Rider;
        lock_info_.SetInfo(root, item_info_);
        shared_.rider_info_.SetInfo( //시간 설정 부분 확인할 것
            const_cast<nsSlot::SerializeItemOption::sRider*>(&root), item_info_);
        time_info_.SetInfo(root, item_info_);
    }
    else if (item_rule.is_rider_parts)
    {
        // already checked.
        assert(item_rule.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART ||
               item_rule.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART_NOTIMER);

        const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
        const nsSlot::SerializeItemOption::sRiderParts& root = item_option->RiderParts;
        lock_info_.SetInfo(root, item_info_);
        equip_info_.SetInfo(root, item_info_);
        time_info_.SetInfo(root, item_info_);
        shared_.rider_parts_info_.SetInfo(
            const_cast<nsSlot::SerializeItemOption::sRiderParts*>(&root), item_info_);
    }
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    else if (item_rule.is_extend_data)
    {
        const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
        const nsSlot::SerializeItemOption::sExtendData& root = item_option->ExtendData;
        lock_info_.SetInfo(root, item_info_);
        shared_.default_ect_info_.SetInfo(root, item_info_);
    }
#endif
    else if (item_rule.charge_sub_type == eCHARGE_SUB_GENERAL ||
             item_rule.charge_sub_type == eCHARGE_PC_ROOM)
    {
        if (item_rule.is_weapon || item_rule.is_armor)
        {
            const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
            const nsSlot::SerializeItemOption::sEquip& root = item_option->Equip;
            lock_info_.SetInfo(root, item_info_);
            equip_info_.SetInfo(root, item_info_);
            n_option_info_.SetInfo(root, item_info_);
        #if !defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
            if (item_rule.func_attr.value & item_rule.func_attr.eEnable_Rank) {
                rank_info_.SetInfo(root, item_info_);
            }
        #endif
            socket_info_.SetInfo(root, item_info_);
            //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
            SLOTCODE image_item_code \
                = root.ImageCode ? static_cast<SLOTCODE>(root.ImageCode)
                : lock_info_.history_.value == lock_info_.history_.Etheria ? HardCode::ITEMCODE_of_ETHERIA
                : item_code_;
            ChangeImageItem(image_item_code, lock_info_.history_.value);
        #ifdef _NA_003740_20111122_RANDOM_ITEM
            random_info_.SetInfo(root, item_info_);
        #endif // _NA_003740_20111122_RANDOM_ITEM
        }
        else if (item_rule.is_accessory || item_rule.is_accessory_special ||
                 item_rule.is_accessory_wings) // added by _NA002823_WINGS_EQUIPMENTS_EXTENTION
        {
            const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
            const nsSlot::SerializeItemOption::sEquipEtc& root = item_option->EquipEtc;
            lock_info_.SetInfo(root, item_info_);
            equip_info_.SetInfo(root, item_info_);
            n_option_info_.SetInfo(root, item_info_);
        #if !defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
            if (item_rule.func_attr.value & item_rule.func_attr.eEnable_Rank) {
                rank_info_.SetInfo(root, item_info_);
            }
        #endif
        #ifdef _NA_003740_20111122_RANDOM_ITEM
            random_info_.SetInfo(root, item_info_);
        #endif // _NA_003740_20111122_RANDOM_ITEM
        }
        else
        {
            const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
            const nsSlot::SerializeItemOption::sDefault& root = item_option->Default;
            lock_info_.SetInfo(root, item_info_);
        }
    }
    else if (item_rule.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT ||
             item_rule.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX ||
             item_rule.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY ||
             item_rule.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_WAIT ||
             item_rule.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX2)
    {
        if (item_rule.is_weapon || item_rule.is_armor)
        {
            const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
            const nsSlot::SerializeItemOption::sEquipTime& root = item_option->EquipTime;
            lock_info_.SetInfo(root, item_info_);
            equip_info_.SetInfo(root, item_info_);
            time_info_.SetInfo(root, item_info_);
            n_option_info_.SetInfo(root, item_info_);
        #if !defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
            if (item_rule.func_attr.value & item_rule.func_attr.eEnable_Rank) {
                rank_info_.SetInfo(root, item_info_);
            }
        #endif
            socket_info_.SetInfo(root, item_info_);
        #ifdef _NA_003740_20111122_RANDOM_ITEM
            random_info_.SetInfo(root, item_info_);
        #endif // _NA_003740_20111122_RANDOM_ITEM
        }
        else if (item_rule.is_accessory || item_rule.is_accessory_special ||
                 item_rule.is_accessory_wings) // added by _NA002823_WINGS_EQUIPMENTS_EXTENTION
        {
            const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
            const nsSlot::SerializeItemOption::sEquipEtcTime& root = item_option->EquipEtcTime;
            lock_info_.SetInfo(root, item_info_);
            equip_info_.SetInfo(root, item_info_);
            time_info_.SetInfo(root, item_info_);
            n_option_info_.SetInfo(root, item_info_);
        #if !defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
            if (item_rule.func_attr.value & item_rule.func_attr.eEnable_Rank) {
                rank_info_.SetInfo(root, item_info_);
            }
        #endif
        #ifdef _NA_003740_20111122_RANDOM_ITEM
            random_info_.SetInfo(root, item_info_);
        #endif // _NA_003740_20111122_RANDOM_ITEM
        }
        else
        {
            const nsSlot::SerializeItemOption* const item_option = _GetBindingBlock();
            const nsSlot::SerializeItemOption::sTime& root = item_option->Time;
            lock_info_.SetInfo(root, item_info_);
            time_info_.SetInfo(root, item_info_);
        }
    }
    //
    // last position
    //=============================================================================================
    // NOTE: modify a runtime data by script data
    // f100910.2L, modify a locked information.
    // this rule support item of previous p-coin purchases of users.
    // reference 'f100909.1L', added by _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
    if (lock_info_.item_tag_.value == lock_info_.item_tag_.Tag_None &&
        item_info_->m_ChargeType == eCHARGE_CHARGE_PPCARD)
    {
        lock_info_.item_tag_.value = lock_info_.item_tag_.Tag_PCoinPurchase;
    }
    //=============================================================================================
} //end 'CopyPart(const OPTIONPART& IN rPart)'

void
SCItemSlot::CopyOutPart(OPTIONPART& OUT rPart) const // (090911) (ADD-IN)
{
    ZeroMemory(&rPart, sizeof(rPart));
    if (!FlowControl::FCAssert(item_info_ != 0)) {
        // occurred case: Stream, CopyOut operation by invalid info
        return;
    }
    // NOTE: f110327.1L, the parse_type field is contained SCItemSlot
    nsSlot::ItemRuleInfo item_type_info;
    bool success = nsSlot::ItemRuleInfo::GetRuleInfo(&item_type_info, *this,
        equip_info_.is_limited_ != false, NULL);
    if (success == false) {
        return;
    }
    nsSlot::SerializeItemOption* item_option = _GetBindingBlock();
    ZeroMemory(item_option, sizeof(*item_option));

    SCItemSlot* this_ = const_cast<SCItemSlot*>(this);

    if (item_type_info.is_limited)
    {
        nsSlot::SerializeItemOption::sLimited* root = &item_option->Limited;
        item_option->Default.ParseType = ItemParseType::kLimited;
        this_->lock_info_.GetInfo(root);
        this_->equip_info_.GetInfo(root);
        this_->n_option_info_.GetInfo(root);
        this_->socket_info_.GetInfo(root);
        this_->shared_.limited_info_.GetInfo(root);
        //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        if (image_item_code_ != item_code_ &&
            image_item_code_ != HardCode::ITEMCODE_of_ETHERIA)
        {
            root->ImageCode = image_item_code_;
        }
    #ifdef _DEBUG
        if (image_item_code_ == HardCode::ITEMCODE_of_ETHERIA) {
            assert(this_->lock_info_.history_.value == this_->lock_info_.history_.Etheria);
        }
    #endif
    #ifdef _NA_003740_20111122_RANDOM_ITEM
        random_info_.GetInfo(root);
    #endif // _NA_003740_20111122_RANDOM_ITEM
    }
    //else if (item_type_info.charge_sub_type == eCHARGE_SUB_FATE)
    else if (item_type_info.is_fate)
    {
        nsSlot::SerializeItemOption::sFate* root = &item_option->Fate;
        item_option->Default.ParseType = ItemParseType::kFate;
        this_->lock_info_.GetInfo(root);
        this_->equip_info_.GetInfo(root);
        this_->time_info_.GetInfo(root);
        this_->n_option_info_.GetInfo(root);
        this_->socket_info_.GetInfo(root);
        this_->shared_.fate_info_.GetInfo(root);
        // added rule to support _NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM
        if (image_item_code_ != item_code_ &&
            image_item_code_ != HardCode::ITEMCODE_of_ETHERIA)
        {
            root->ImageCode = image_item_code_;
        }
    #ifdef _DEBUG
        if (image_item_code_ == HardCode::ITEMCODE_of_ETHERIA) {
            assert(this_->lock_info_.history_.value == this_->lock_info_.history_.Etheria);
        }
    #endif
    #ifdef _NA_003740_20111122_RANDOM_ITEM
        random_info_.GetInfo(root);
    #endif // _NA_003740_20111122_RANDOM_ITEM
    }
    else if (item_type_info.is_lottery)
    {
        nsSlot::SerializeItemOption::sLottery* root = &item_option->Lottery;
        item_option->Default.ParseType = ItemParseType::kLottery;
        this_->lock_info_.GetInfo(root);
        this_->shared_.lotto_info_.GetInfo(root);
    }
    else if (item_type_info.is_pet)
    {
        nsSlot::SerializeItemOption::sPet* root = &item_option->Pet;
        item_option->Default.ParseType = ItemParseType::kPet;
        this_->lock_info_.GetInfo(root);
        this_->time_info_.GetInfo(root);
        this_->shared_.pet_info_.GetInfo(root);
        //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
        if (image_item_code_ != item_code_ &&
            image_item_code_ != HardCode::ITEMCODE_of_ETHERIA)
        {
            root->ImageCode = image_item_code_;
        }
    #ifdef _DEBUG
        if (image_item_code_ == HardCode::ITEMCODE_of_ETHERIA) {
            assert(this_->lock_info_.history_.value == this_->lock_info_.history_.Etheria);
        }
    #endif
    }
    else if (item_type_info.is_waypoint)
    {
        nsSlot::SerializeItemOption::sWayPoint* root = &item_option->WayPoint;
        item_option->Default.ParseType = ItemParseType::kWayPoint;
        this_->lock_info_.GetInfo(root);
        this_->time_info_.GetInfo(root);
        this_->shared_.waypoint_info_.GetInfo(root);
    }
    else if (item_type_info.is_coord)
    {
        nsSlot::SerializeItemOption::sCoord* root = &item_option->Coord;
        item_option->Default.ParseType = ItemParseType::kCoord;
        this_->lock_info_.GetInfo(root);
        this_->shared_.coordinate_info_.GetInfo(root);
    }
    else if (item_type_info.is_rider)
    {
        assert(item_type_info.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART ||
               item_type_info.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART_NOTIMER);

        nsSlot::SerializeItemOption::sRider* root = &item_option->Rider;
        item_option->Default.ParseType = ItemParseType::kRider;
        this_->lock_info_.GetInfo(root);
        this_->shared_.rider_info_.GetInfo(root, &this_->time_info_);
        this_->time_info_.GetInfo(root);
    }
    else if (item_type_info.is_rider_parts)
    {
        assert(item_type_info.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART ||
               item_type_info.charge_sub_type == eCHARGE_SUB_RIDER_WITH_PART_NOTIMER);

        nsSlot::SerializeItemOption::sRiderParts* root = &item_option->RiderParts;
        item_option->Default.ParseType = ItemParseType::kRiderParts;
        this_->lock_info_.GetInfo(root);
        this_->equip_info_.GetInfo(root);
        this_->time_info_.GetInfo(root);
        this_->shared_.rider_parts_info_.GetInfo(root);
    }
#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
    else if (item_type_info.is_extend_data)
    {
        nsSlot::SerializeItemOption::sExtendData* root = &item_option->ExtendData;
        item_option->Default.ParseType = ItemParseType::kExtendData;
        this_->lock_info_.GetInfo(root);
        this_->shared_.default_ect_info_.GetInfo(root);
    }
#endif
    else if (item_type_info.charge_sub_type == eCHARGE_SUB_GENERAL ||
             item_type_info.charge_sub_type == eCHARGE_PC_ROOM)
    {
        if (item_type_info.is_weapon || item_type_info.is_armor)
        {
            nsSlot::SerializeItemOption::sEquip* root = &item_option->Equip;
            item_option->Default.ParseType = ItemParseType::kEquip;
            this_->lock_info_.GetInfo(root);
            this_->equip_info_.GetInfo(root);
            this_->n_option_info_.GetInfo(root);
        #if !defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
            if (item_type_info.func_attr.value & item_type_info.func_attr.eEnable_Rank) {
                this_->rank_info_.GetInfo(root);
            }
        #endif
            this_->socket_info_.GetInfo(root);
            //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
            if (image_item_code_ != item_code_ &&
                image_item_code_ != HardCode::ITEMCODE_of_ETHERIA)
            {
                root->ImageCode = image_item_code_;
            }
        #ifdef _DEBUG
            if (image_item_code_ == HardCode::ITEMCODE_of_ETHERIA) {
                assert(this_->lock_info_.history_.value == this_->lock_info_.history_.Etheria);
            }
        #endif
        #ifdef _NA_003740_20111122_RANDOM_ITEM
            random_info_.GetInfo(root);
        #endif // _NA_003740_20111122_RANDOM_ITEM
        }
        else if (item_type_info.is_accessory || item_type_info.is_accessory_special ||
                 item_type_info.is_accessory_wings) // added by _NA002823_WINGS_EQUIPMENTS_EXTENTION
        {
            nsSlot::SerializeItemOption::sEquipEtc* root = &item_option->EquipEtc;
            item_option->Default.ParseType = ItemParseType::kEquipEtcs;
            this_->lock_info_.GetInfo(root);
            this_->equip_info_.GetInfo(root);
            this_->n_option_info_.GetInfo(root);
        #if !defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
            if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank) {
                this_->rank_info_.GetInfo(root);
            }
        #endif
        #ifdef _NA_003740_20111122_RANDOM_ITEM
            random_info_.GetInfo(root);
        #endif // _NA_003740_20111122_RANDOM_ITEM
        }
        else
        {
            nsSlot::SerializeItemOption::sDefault* root = &item_option->Default;
            item_option->Default.ParseType = ItemParseType::kDefault;
            this_->lock_info_.GetInfo(root);
        }
    }
    else if (item_type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT ||
             item_type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX ||
             item_type_info.charge_sub_type == eCHARGE_SUB_FIXED_QUANTITY ||
             item_type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_WAIT ||
             item_type_info.charge_sub_type == eCHARGE_SUB_FIXED_AMOUNT_EX2)
    {
        if (item_type_info.is_weapon || item_type_info.is_armor)
        {
            nsSlot::SerializeItemOption::sEquipTime* root = &item_option->EquipTime;
            item_option->Default.ParseType = ItemParseType::kEquipTime;
            this_->lock_info_.GetInfo(root);
            this_->equip_info_.GetInfo(root);
            this_->time_info_.GetInfo(root);
            this_->n_option_info_.GetInfo(root);
        #if !defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
            if (item_type_info.func_attr.value & item_type_info.func_attr.eEnable_Rank) {
                this_->rank_info_.GetInfo(root);
            }
        #endif
            this_->socket_info_.GetInfo(root);
        #ifdef _NA_003740_20111122_RANDOM_ITEM
            random_info_.GetInfo(root);
        #endif // _NA_003740_20111122_RANDOM_ITEM
        }
        else if (item_type_info.is_accessory || item_type_info.is_accessory_special ||
                 item_type_info.is_accessory_wings) // added by _NA002823_WINGS_EQUIPMENTS_EXTENTION
        {
            nsSlot::SerializeItemOption::sEquipEtcTime* root = &item_option->EquipEtcTime;
            item_option->Default.ParseType = ItemParseType::kEquipEtcsTime;
            this_->lock_info_.GetInfo(root);
            this_->equip_info_.GetInfo(root);
            this_->time_info_.GetInfo(root);
            this_->n_option_info_.GetInfo(root);
        #if !defined(_NA_0_20100727_SOCKET_SYSTEM_CONVERT)
            if (item_rule_info_.func_attr.value & item_rule_info_.func_attr.eEnable_Rank) {
                this_->rank_info_.GetInfo(root);
            }
        #endif
        #ifdef _NA_003740_20111122_RANDOM_ITEM
            random_info_.GetInfo(root);
        #endif // _NA_003740_20111122_RANDOM_ITEM
        }
        else
        {
            nsSlot::SerializeItemOption::sTime* root = &item_option->Time;
            item_option->Default.ParseType = ItemParseType::kDefaultTime;
            this_->lock_info_.GetInfo(root);
            this_->time_info_.GetInfo(root);
        }
    }

    _SerializeToItemStream(&rPart, ITEM_STREAM_SIZE_LAST_VERSION);
}
