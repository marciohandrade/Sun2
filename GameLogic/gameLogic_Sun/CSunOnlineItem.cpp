#include "stdafx.h"
#include "./CSunOnlineItem.h"

#ifdef __NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

#include <strsafe.h>
#include <string.h>
#include <SolarDateTime.h>
#include "./util/util.h"
#include "./logic/logic.h"

//==================================================================================================

struct XmlElemStack
{
    XmlElemStack(BOOLEAN is_decode, CMarkupArchive& xml, const char* name)
        : xml_(xml)
    {
        if(is_decode)
            xml_.AddChildElem(name);
        xml_.IntoElem();
    }

    ~XmlElemStack() {
        xml_.OutOfElem();
    }

    CMarkupArchive& xml_;
};

//==================================================================================================
CSunOnlineItem::CSunOnlineItem(CCSunOnline* root)
: root_(root)
{
}

void
CSunOnlineItem::_Serialize_EtherWeapon(BOOLEAN is_decode, CMarkupArchive& xml,
                                       const nsSlot::ItemRuleInfo& item_rule_info,
                                       SCItemSlot& item_slot)
{
    BOOLEAN is_ether_weapon = false;
    BYTE ether_bullet_index = 0;

    if(is_decode) {
        if(item_rule_info.is_weapon && (is_ether_weapon = item_slot.IsEquipEtherDevice()))
            ether_bullet_index = item_slot.GetEtherBulletIndex();

        xml.AddChildElemEx("IsEquipEtherDevice", is_ether_weapon);
        xml.AddChildElemEx("EtherBulletIndex", ether_bullet_index);
    }
    else {
        xml.FindGetChildData("IsEquipEtherDevice", is_ether_weapon);
        if(is_ether_weapon != 0) {
            item_slot.SetEtherBulletIndex(true);
            xml.FindGetChildData("EtherBulletIndex", ether_bullet_index);//무기에 탄환 장착
            item_slot.SetEtherBulletIndex(ether_bullet_index);
        }
    }
}

void
CSunOnlineItem::_Serialize_PetItem(BOOLEAN is_decode, CMarkupArchive& xml,
                                   const nsSlot::ItemRuleInfo& item_rule_info,
                                   SCItemSlot& item_slot)
{
    BOOLEAN is_pet = false;
    BYTE fullness = 0;
    BYTE intimacy = 0;
    BOOLEAN exist_name = false;
    BOOLEAN is_summoned = false;
    char pet_name[MAX_PET_NAME_LENGTH + 1];

    if(is_decode) {
        if(item_rule_info.is_pet)
            is_pet = true;

        xml.AddChildElemEx("IsPetItem", is_pet);
        const SCPetInfo* pet_info = 0;
        if(is_pet && (pet_info = item_slot.GetPetInfo())) {
            fullness = pet_info->GetFullness();
            intimacy = pet_info->GetIntimacy();
            exist_name = !!pet_info->IsSetPetName();

            xml.AddChildElemEx("Fullness", fullness);
            xml.AddChildElemEx("Initimacy", intimacy);
            xml.AddChildElemEx("IsPetName", exist_name);

            const char* src_name = pet_info->GetPetName();
            StringCchCopy(pet_name, _countof(pet_name), src_name);
            pet_name[_countof(pet_name) - 1] = '\0';
            xml.AddChildElemEx("PetName", pet_name);

            is_summoned = !!item_slot.IsPetLocked();
            xml.AddChildElemEx("IsPetLocked", is_summoned);
        }
        else {
            _LOG(__FUNCTION__"|PetInfo=NULL|");
        }

    }
    else {
        xml.FindGetChildData("IsPetItem", is_pet);
        if(is_pet != 0 && item_rule_info.is_pet == 0) {
            _LOG(__FUNCTION__"|Msg=Invalid Pet|ItemCode=%d|", item_slot.GetCode());
        }
        else if(is_pet != 0) {
            SCPetInfo* pet_info = item_slot.GetPetInfo();
            //assert(pet_info != 0)
            xml.FindGetChildData("Fullness", fullness);//포만감 
            pet_info->SetFullness(fullness);
            xml.FindGetChildData("Initimacy", intimacy);//친밀도
            pet_info->SetIntimacy(intimacy);
            xml.FindGetChildData("IsPetName", exist_name);//
            if(exist_name != 0)
            {
                std::string strName;
                xml.FindGetChildData("PetName", strName);//
                pet_info->SetPetName(const_cast<char*>(strName.c_str()));
            }			
        }
    }
}

void
CSunOnlineItem::_Serialize_FateItem(BOOLEAN is_decode, CMarkupArchive& xml,
                                    const nsSlot::ItemRuleInfo& item_rule_info,
                                    SCItemSlot& item_slot,
                                    char time_string_inout[32])
{
    DWORD64 date_time = 0;
    WORD atk_def_ratio = 0;
    BOOLEAN is_identify = false;
    DURATYPE max_duration = 0;
    WORD period = 0;

    char buffer[256];

    if(item_rule_info.is_fate == 0) {
        _LOG(__FUNCTION__"|Unexpected logic|");
        __asm { int 3 };
    }

    if(is_decode) {
        date_time = item_slot.GetDateTime();
        _snprintf(time_string_inout, 32, "%I64d", date_time);

        atk_def_ratio = item_slot.GetFateAtkDefRatio();
        is_identify = item_slot.GetFateIdentify();
        max_duration = item_slot.GetFateDuraMax();
        period = item_slot.GetFatePeriod();

        //item_slot.IsFateItem()
        _snprintf(buffer, _countof(buffer), "[%u:%d:%d:%u]",
                  atk_def_ratio, is_identify, max_duration, period);
        xml.AddChildElem("Lucky", buffer);
    }
    else {
        date_time = static_cast<DWORD64>(_atoi64(time_string_inout));
        SYSTEMTIME systime;
        util::YYYYMMDDHHMMSSToSYSTEMTIME(date_time, systime);
        _snprintf(buffer, _countof(buffer), "%04d%02d%02d%02d%02d%02d",
                  systime.wYear, systime.wMonth, systime.wDay,
                  systime.wHour, systime.wMinute, systime.wSecond);

        // 시간을 셋팅(초 단위)
        BOOST_STATIC_ASSERT(_countof(buffer) >= 32);
        if(strncmp(buffer, time_string_inout, 32) == 0) {
        #ifdef _FIXED_DATETIIME_METHOD_
            item_slot.SetFixedDateTime(date_time);
        #endif
        }
        else {
            item_slot.SetDateTime(date_time);
        }

        std::string stl_fate_string;
        if(xml.FindGetChildData("Lucky", stl_fate_string) == 0) {
            _LOG(__FUNCTION__"|Unexpected logic|");
            __asm { int 3 };
        }

        std::string::size_type length = stl_fate_string.length();
        if(length == 0 || _countof(buffer) <= length)
            return;

        strncpy(buffer, stl_fate_string.c_str(), _countof(buffer));
        buffer[_countof(buffer) - 1] = '\0';
        const char* token = strtok(buffer, "[ ]");
        int count = 0;
        while(token != 0) {
            ++count;
            strncpy(buffer, token, _countof(buffer));
            token = strtok(NULL, "[ ]");
        }

        if(count != 1) {
            _LOG(__FUNCTION__"|Msg=invalid format lucky option|");
            __asm { int 3 };
        }

        count = 0;
        token = strtok(buffer, ":");
        atk_def_ratio = (WORD)atoi(token);
        ++count;

        token = strtok(NULL, ":");
        is_identify = !!atoi(token);
        ++count;

        token = strtok(NULL, ":");
        max_duration = (DURATYPE)atoi(token);
        ++count;

        token = strtok(NULL, ":");
        period = (WORD)atoi(token);
        ++count;

        item_slot.SetFateAtkDefRatio(atk_def_ratio);
        item_slot.SetFateIdentify(is_identify);
        item_slot.SetFateDuraMax(max_duration);
        item_slot.SetFatePeriod(period);
    }
}

void
CSunOnlineItem::_Serialize_LimitedItem(BOOLEAN is_decode, CMarkupArchive& xml,
                                       const nsSlot::ItemRuleInfo& item_rule_info,
                                       SCItemSlot& item_slot)
{
    WORD limited_index = 0;
    int damage_ratio = 0;
    int limit_value_ratio = 0;
    DURATYPE max_duration = 0;
    DURATYPE max_duration_for_restore = 0;
    BYTE number_of_repairs = 0;

    char buffer[256];

    if(is_decode) {
        if(item_rule_info.is_limited == 0 || item_slot.IsLimited() == 0) {
            _LOG(__FUNCTION__"|Unexpected logic|");
            __asm { int 3 };
        }

        limited_index = item_slot.GetLimitedIndex();
        damage_ratio = item_slot.GetDamageRatio();
        limit_value_ratio = item_slot.GetLimitValueRatio();
        max_duration = item_slot.GetLimitedDuraMax();
        max_duration_for_restore = item_slot.GetLimitedDuraMMax();
        number_of_repairs = item_slot.GetLimitedRepairCount();

        _snprintf(buffer, _countof(buffer), "[%u:%d:%d:%d:%d:%d]",
                  limited_index, damage_ratio, limit_value_ratio,
                  max_duration, max_duration_for_restore, number_of_repairs);
        xml.AddChildElem("Limited", buffer);
    }
    else {
        std::string stl_limited;
        if(xml.FindGetChildData("Limited", stl_limited) == 0)
            return;

        std::string::size_type length = stl_limited.length();
        if(length == 0 || _countof(buffer) <= length)
            return;

        strncpy(buffer, stl_limited.c_str(), _countof(buffer));
        buffer[_countof(buffer) - 1] = '\0';
        const char* token = strtok(buffer, "[ ]");
        int count = 0;
        while(token != 0) {
            ++count;
            strncpy(buffer, token, _countof(buffer));
            token = strtok(NULL, "[ ]");
        }

        if(count != 1) {
            _LOG(__FUNCTION__"|Msg=invalid format limited option|");
            __asm { int 3 };
        }

        count = 0;
        token = strtok(buffer, ":");
        limited_index = (WORD)atoi(token);
        ++count;

        token = strtok(NULL, ":");
        damage_ratio = atoi(token);
        ++count;

        token = strtok(NULL, ":");
        limit_value_ratio = atoi(token);
        ++count;

        token = strtok(NULL, ":");
        max_duration = (DURATYPE)atoi(token);
        ++count;

        token = strtok(NULL, ":");
        max_duration_for_restore = (DURATYPE)atoi(token);
        ++count;

        token = strtok(NULL, ":");
        number_of_repairs = (BYTE)atoi(token);
        ++count;

        item_slot.SetLimitedIndex(limited_index);
        item_slot.SetDamageRatio(damage_ratio);
        item_slot.SetLimitValueRatio(limit_value_ratio);
        item_slot.SetLimitedDuraMax(max_duration);
        item_slot.SetLimitedDuraMMax(max_duration_for_restore);
        item_slot.SetLimitedRepairCount(number_of_repairs);
    }
}

//==================================================================================================

BOOLEAN
CSunOnlineItem::DecodeItem(CMarkupArchive& xml, const SCItemSlot& item_slot, POSTYPE item_pos)
{
    XmlElemStack stack_item(true, xml, "ITEM");

    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if(item_rule_info.is_valid_item == 0)
        return false;

    //-------------------------------
    // default environment
    static std::string stl_temp;
    //

    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();

    const SLOTCODE item_code = item_slot.GetCode();
    xml.AddChildElemEx("Code", item_code); //코드번호
    xml.AddChildElemEx("Pos", item_pos); //인벤상 위치.

    {   // item name setup...
        const char* item_name = STRTBL_PARSER1->GetStringByCode(item_info->m_NCode);
        stl_temp.resize(0);
        if(item_name != 0)
            stl_temp = item_name;

        if(item_info->m_SCNCode != 0) { //2nd 네임코드
            item_name = STRTBL_PARSER1->GetStringByCode(item_info->m_SCNCode);
            if(item_name != 0)
                stl_temp += item_name;
        }

        item_name = stl_temp.c_str();
        xml.AddChildElem("Name", item_name); //아이템명
    };

    // lock infos...
    xml.AddChildElemEx("Block", item_slot.IsBlocked()); //아이템 Block여부

    //BUILD_KOREA_061211--
    {
        xml.AddChildElemEx("Type", item_info->m_wType); //타입
        xml.AddChildElemEx("SubType", item_info->m_wSubType); //서브타입
        xml.AddChildElemEx("ChargeType", item_info->m_ChargeType); //유료화타입
        xml.AddChildElemEx("ChargeSubType", item_info->m_ChargeSubType); //유료화서브타입
    };

    {
        xml.AddChildElemEx("DBType", item_slot.GetType()); //서버군타입
        xml.AddChildElemEx("Serial", item_slot.GetSerial()); //시리얼
        xml.AddChildElemEx("WorldSerial", item_slot.GetWorldSerial()); //월드 시리얼

        if(item_slot.CanEquip())
            xml.AddChildElemEx("EqPos", item_info->m_wEqPos); //장비위치
        else
            xml.AddChildElemEx("EqPos", -1); //장비위치

        // item_slot.WithDura() : 내구도를 가지는 아이템인가? true=Yes
        if(item_slot.WithDura() != 0) {
            xml.AddChildElemEx("IsDura", 1);
            xml.AddChildElemEx("Dura", item_slot.GetDura());
        }
        else {
            xml.AddChildElemEx("IsDura", 0);
            xml.AddChildElemEx("Dura", item_slot.GetNum());
        }
    };

#ifdef __NA_1247_POSSESSION_ITEM
    if(item_slot.IsPossessionItem()) {
        xml.AddChildElemEx("IsPossessionType", 1);
        xml.AddChildElemEx("IsPossession", item_slot.GetIdentifyPossession());
    }
    else {
        xml.AddChildElemEx(_T("IsPossessionType"), 0);
    }
  #ifdef __NA_0_POSSESSION_ITEM_DROP  
    xml.AddChildElemEx("IsDestroyItem", item_slot.IsDestroyItem() ? 1 : 0); //디바인여부
  #endif
#endif

    // (NOTE) element가 존재해야 하는 부분이 있는지라 filtering을 수행하지 않는다.
    _Serialize_EtherWeapon(true, xml, item_rule_info, const_cast<SCItemSlot&>(item_slot));
    _Serialize_PetItem(true, xml, item_rule_info, const_cast<SCItemSlot&>(item_slot));

    { // process time facilities
        BOOLEAN is_progress = item_slot.IsProgressTime();
        BOOLEAN is_expired = item_slot.IsExpiredTime();
        BOOLEAN is_wait_time = item_slot.IsExpiredWaitTime();

        xml.AddChildElemEx("ChargeOnUse", is_progress); // 장착시 타이머 동작 아이템일 경우 그 여부
        xml.AddChildElemEx("IsExpiredTime", is_expired); // 만료 여부
        //#ifdef __NA_000493_20070712_SHOP_WEAPON_ARMOR_ITEM
        xml.AddChildElemEx("IsExpiredWaitTime", is_wait_time); //만료 이후 대기 상태 여부
    };

    BOOLEAN with_timer = item_rule_info.IsEnableTimer();
    char time_buffer[32] = { 0, };

    // need re-arrangement
    // (OLD) 11 == item_info->m_wSubType
    if(item_rule_info.is_fate) {
        _Serialize_FateItem(true, xml, item_rule_info, const_cast<SCItemSlot&>(item_slot), time_buffer);
    }
    else {
        //GetItemChargeExpireTime(item_info->m_ChargeSubType, item_slot, xml, pszTime); 
        BOOST_STATIC_WARNING(0);
    }

    xml.AddChildElem("ChargeExpireTime", time_buffer); // 사용만료 시간
    //--BUILD_KOREA_061211

    {   //divine setup
        // same field 'divine' and 'set'
        xml.AddChildElemEx("IsDivine", item_slot.IsDivine()); //디바인여부
        xml.AddChildElemEx("SET", item_slot.GetSet()); //set
    };
    {   //enchant option
        xml.AddChildElemEx("Enchant", item_slot.GetEnchant()); //인첸트옵션정보
    };
    {   // rank options
        stl_temp.resize(0);
        stl_temp = root_->ConvertRankType(item_slot.GetRank());
        xml.AddChildElem("RankType", stl_temp.c_str()); //현재 랭크 
    };

    // extra options
    int extra_option_index = item_slot.GetNOptionValueIndex();
    xml.AddChildElemEx("NOptionValue", extra_option_index);
    if(extra_option_index != 0)
    {
        ExtraStoneOptionInfoParser* pExtraStoneParser = ExtraStoneOptionInfoParser::Instance();
        const sEXTRASTONE_OPT_INFO* pInfo =
            pExtraStoneParser->GetExtraStoneOptionInfo(item_info->m_wType
                                                       /*, item_slot.GetItemInfo()->m_LV*/);
        if(pInfo)
            xml.AddChildElemEx("NOptionNCode", pInfo->m_dwOptionNCode);
    }

    {   // rank options...
        int attr_index = 0;
        BYTE attr_value = 0;
        eATTR_TYPE attr_type = eATTR_TYPE_INVALID;
        int number_of_options = 0;
        int upperbound = 0;

        char temp_buffer[256];

        int buffer_offset = 0;
        char buffer[1024];
        buffer[0] = '\0';
        if(item_rule_info.IsEnableRank() == 0) {
            xml.AddChildElem("RANK", buffer);
        }
        else {
            buffer_offset += _snprintf(&buffer[buffer_offset], _countof(buffer) - buffer_offset,
                                       "[ ");

            upperbound = item_slot.GetRank() + 1;
            for(int rank_inc = RANK_D; rank_inc < upperbound; ++rank_inc) {
                eRANK rank = (eRANK)rank_inc;
                attr_value = item_slot.GetRankAttrValue(rank);
                const sRANK_OPTION* option = item_slot.GetRankAttrDesc(rank);
                if(option == 0)
                    continue;
                if(option->m_OptionNCode == 0)
                    continue;

                attr_index = option->m_iOptionIndex;
                GetStringByCode(option->m_OptionNCode, temp_buffer, _countof(temp_buffer));

                const char* rank_string = root_->ConvertRankType(rank).c_str();
                //랭크명, 텍스트, 밸류, 인덱스 순으로 넘겨준다.
                buffer_offset += _snprintf(&buffer[buffer_offset], _countof(buffer) - buffer_offset,
                                           "%s:%s:%d:%d ",
                                           rank_string, temp_buffer, attr_value, attr_index);

                ++number_of_options;
            }

            buffer_offset += _snprintf(&buffer[buffer_offset], _countof(buffer) - buffer_offset,
                                       "]");

            if(number_of_options != 0)
                buffer[0] = '\0';

            xml.AddChildElem("RANK", buffer); //위에서 만든 스트링값[즉 랭크옵션스트링]을 넣는다.
        }
    };

    {   // socket options...
        if(item_rule_info.IsEnableSocket())
            xml.AddChildElemEx("SocketNum", item_slot.GetSocketNum()); //소켓넘버
        else
            xml.AddChildElemEx("SocketNum", 0);
    };
    {   // socket options detailed...
        int attr_index = 0;
        BYTE attr_value = 0;
        eATTR_TYPE attr_type = eATTR_TYPE_INVALID;
        int number_of_options = 0;
        BYTE upperbound = 0;

        eSOCKET_LEVEL level = SOCKETLV_LOW;

        char temp_buffer[256];

        int buffer_offset = 0;
        char buffer[1024];
        buffer[0] = '\0';
        if(item_rule_info.IsEnableSocket() == 0) {
            xml.AddChildElem("SOCKET", buffer);
        }
        else {
            buffer_offset += _snprintf(&buffer[buffer_offset], _countof(buffer) - buffer_offset,
                                       "[ ");

            upperbound = (BYTE)item_slot.GetSocketNum();
            for(int socket_inc = SOCKET_1; socket_inc < upperbound; ++socket_inc) {
                eSOCKET sock = (eSOCKET)socket_inc;

                attr_type = item_slot.GetSocketAttr(sock);
                if(attr_type == eATTR_TYPE_INVALID)  
                    continue;

                level = item_slot.GetSocketAttrLevel(sock);
                attr_value = item_slot.GetSocketAttrValue(sock);
                const sSOCKET_OPTION* option = item_slot.GetSocketAttrDesc(sock);

                if(option == 0)
                    continue;

                GetStringByCode(option->m_NCode, temp_buffer, _countof(temp_buffer));
                attr_index = option->m_AttrIndex;

                const char* socket_string = root_->ConvertSocketType(sock).c_str();

                //소켓, 텍스트, 벨류, 인덱스,레벨
                buffer_offset += _snprintf(&buffer[buffer_offset], _countof(buffer) - buffer_offset,
                                           "%s:%s:%d:%d:%d ",
                                           socket_string, temp_buffer, attr_value, attr_index, level);

                ++number_of_options;
            }

            buffer_offset += _snprintf(&buffer[buffer_offset], _countof(buffer) - buffer_offset,
                                       "]");

            if(number_of_options != 0)
                buffer[0] = '\0';

            xml.AddChildElem("SOCKET", buffer); //위에서 만든 스트링값[즉 랭크옵션스트링]을 넣는다.
        }
    };

    _Serialize_LimitedItem(true, xml, item_rule_info, const_cast<SCItemSlot&>(item_slot));
    return true;
}

BOOLEAN
CSunOnlineItem::EncodeItem(CMarkupArchive& xml, SCItemSlot& item_slot, POSTYPE* item_pos_out)
{
    XmlElemStack stack_item(false, xml, "ITEM");
    static std::string stl_temp;
    char buffer[1024];

    SLOTCODE item_code = 0;
    if(xml.FindGetChildData("Code", item_code) == false)
        return false;
    if(item_code == 0)
        return false;

    item_slot.Clear();
    item_slot.SetCode(item_code);

    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if(item_rule_info.is_valid_item == 0)
        return false;

    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();

    POSTYPE item_pos = INVALID_POSTYPE_VALUE;
    if(xml.FindGetChildData("Pos", item_pos) == false)
        return false;

    *item_pos_out = item_pos;
    item_slot.SetPos(item_pos);

    // skip name
    BOOLEAN blocked = 0;
    if(xml.FindGetChildData("Block", blocked) == false)
        return false;

    //BUILD_KOREA_061211--
    eCHARGE_SUB_TYPE charge_sub_type = eCHARGE_SUB_GENERAL;
    {
        //WORD item_type = 0;
        //WORD sub_type = 0;
        //BYTE charge_type = 0;
        BYTE charge_sub_type_temp = 0;

        //if(xml.FindGetChildData("Type", item_type) == false)
        //    return false;
        //if(xml.FindGetChildData("SubType", sub_type) == false)
        //    return false;
        //if(xml.FindGetChildData("ChargeType", charge_type) == false)
        //    return false;
        if(xml.FindGetChildData("ChargeSubType", charge_sub_type_temp) == false)
            return false;
        charge_sub_type = (eCHARGE_SUB_TYPE)charge_sub_type_temp;

        //SetItemChargeExpireTime( ChargeSubType, slot, xml, sTemp.c_str() );
    };
    {
        BYTE db_type;
        if(xml.FindGetChildData("DBType", db_type) == false)
            return false;
        item_slot.SetType(db_type);

        DBSERIAL dbserial;
        if(xml.FindGetChildData("Serial", dbserial) == false)
            return false;
        item_slot.SetSerial(dbserial);

        BOOLEAN with_duration = 0;
        if(xml.FindGetChildData("IsDura", with_duration) == false)
            return false;

        if(item_slot.WithDura() != !!with_duration)
            return false;

        BYTE temp_value = 0;
        if(xml.FindGetChildData("Dura", temp_value) == false)
            return false;

        if(with_duration)
            item_slot.SetDura(temp_value);
        else
            item_slot.SetNum(temp_value);
    }

#ifdef __NA_1247_POSSESSION_ITEM
    {
        BOOLEAN is_possession_type = 0;
        if(xml.FindGetChildData("IsPossessionType", is_possession_type) == false)
            return false;
        if(is_possession_type) {
            BOOLEAN is_possession = 0;
            if(xml.FindGetChildData("IsPossession", is_possession) == false)
                return false;
            if(is_possession)
                item_slot.SetIdentifyPossession(true);
        }
    #ifdef __NA_0_POSSESSION_ITEM_DROP
        BOOLEAN is_destroyed = false;
        if(xml.FindGetChildData("IsDestroyItem", is_destroyed) == false)
            return false;

        if(is_destroyed)
            item_slot.SetDestroyItem(true);
    #endif
    }
#endif

    // (NOTE) element가 존재해야 하는 부분이 있는지라 filtering을 수행하지 않는다.
    _Serialize_EtherWeapon(false, xml, item_rule_info, const_cast<SCItemSlot&>(item_slot));
    _Serialize_PetItem(false, xml, item_rule_info, const_cast<SCItemSlot&>(item_slot));

    if(item_rule_info.IsEnableTimer()) {
        stl_temp.resize(0);
        if(xml.FindGetChildData("ChargeExpireTime", stl_temp) == false)
            return false;

        BOOLEAN is_progress = 0;
        BOOLEAN is_expired = 0;
        BOOLEAN is_wait_time = 0;
        if(xml.FindGetChildData("ChargeOnUse", is_progress) == false)
            return false;
        if(xml.FindGetChildData("IsExpiredTime", is_expired) == false)
            return false;
        if(xml.FindGetChildData("IsExpiredWaitTime", is_wait_time) == false)
            return false;

        if(is_progress)
            item_slot.SetProgressTime(!!is_progress);
        if(is_expired)
            item_slot.SetExpiredTime(!!is_expired);
        if(is_wait_time)
            item_slot.SetExpiredWaitTime(!!is_wait_time);

        BOOST_STATIC_WARNING(0);
        //SetItemChargeExpireTime(charge_sub_type, item_slot, xml, stl_temp.c_str());

        char time_string[32];
        strncpy(time_string, stl_temp.c_str(), _countof(time_string));
        time_string[_countof(time_string) - 1] = '\0';
        if(item_rule_info.is_fate)
            _Serialize_FateItem(false, xml, item_rule_info, item_slot, time_string);
    }

    {   //divine
        BOOLEAN is_divine = false;
        if(xml.FindGetChildData("IsDivine", is_divine) == false)
            return false;

        if(is_divine)
            item_slot.SetSet(is_divine);
    };

    if(item_rule_info.IsEnableEnchant())
    {   //enchant
        BYTE enchant = 0;
        if(xml.FindGetChildData("Enchant", enchant) == false)
            return false;
        if(enchant)
            item_slot.SetEnchant(enchant);
    };
    if(item_rule_info.IsEnableExtraOption())
    {   //extra option
        ExtraStoneOptionInfoParser* pExtraStoneParser = ExtraStoneOptionInfoParser::Instance();
        const sEXTRASTONE_OPT_INFO* pInfo =
            pExtraStoneParser->GetExtraStoneOptionInfo(item_info->m_wType
                                                       /*, item_slot.GetItemInfo()->m_LV*/);
        if(pInfo) {
            int extra_option_index = 0;
            if(xml.FindGetChildData("NOptionValue", extra_option_index) == false)
                return false;
            item_slot.SetNOptionAttr(extra_option_index);
        }
    };

    eRANK rank = RANK_E;
    {
        stl_temp.resize(0);
        if(xml.FindGetChildData("RankType", stl_temp) == false)
            return false;

        rank = root_->ConvertRankType(stl_temp);
        item_slot.SetRank(rank);
    };
    if(rank != RANK_E)
    {
        stl_temp.resize(0);
        if(xml.FindGetChildData("RANK", stl_temp) == false)
            return false;

        std::string::size_type length = stl_temp.length();
        if(length == 0 || _countof(buffer) <= length)
            return false;

        strncpy(buffer, stl_temp.c_str(), _countof(buffer));
        buffer[_countof(buffer) - 1] = '\0';

        char* token = strtok(buffer, "[]");
        int count = 0;
        while(token != 0) {
            ++count;
            strncpy(buffer, token, _countof(buffer));
            token = strtok(NULL, "[]");
        }

        if(count > 1) {
            _LOG(__FUNCTION__"|Msg=invalid format rank option|");
            __asm { int 3 };
            return false;
        }

        typedef char* Infos[4]; //rank option args "Rank:Name:val:val"
        typedef Infos Tokens[RANK_MAX];

        Tokens tokens = { 0, };
        count = 0;
        token = strtok(buffer, " ");
        while(token != 0) {
            tokens[count++][0] = token;
            token = strtok(NULL, " ");
        }

        // stack overrun
        if(_countof(tokens) < count) {
            _LOG(__FUNCTION__"|Msg=invalid format rank option numbers|");
            __asm { int 3 };
            return false;
        }

        int upperbound = count;
        if(upperbound != 0) { // rank가 있는데, count==0이면???
            int offset = 0;
            for(count = 0 ; count < upperbound; ++upperbound) {
                Infos& rank_infos = tokens[count];
                token = strtok(rank_infos[count], ":");
                while(token != 0) {
                    rank_infos[offset++] = token;
                    token = strtok(NULL, ":");
                }

                if(_countof(rank_infos) < offset) {
                    _LOG(__FUNCTION__"|Msg=invalid format rank separated option numbers|");
                    __asm { int 3 };
                    return false;
                }

                eRANK current_rank = root_->ConvertRankType(rank_infos[0]);
                int string_index = STRTBL_PARSER1->GetCodeByString(rank_infos[1]);
                int attr_value = atoi(rank_infos[2]);
                int attr_index = atoi(rank_infos[3]);
                if(_countof(g_ITEM_OPTION) <= attr_index) {
                    _LOG(__FUNCTION__"|Msg=invalid format rank attributes index|index=%d|", attr_index);
                    __asm { int 3 };
                    return false;
                }

                RankOptionParser* pRankOptionParser = RankOptionParser::Instance();
                sRANK_OPTION* option = pRankOptionParser->GetRankOption(item_info->m_wType, attr_index);
                BOOST_STATIC_WARNING(0); //파서의 값을 설정중이다... 정상인 건가?
                if(option) {
                    option->m_iOptionIndex = attr_index;
                    if(string_index > 0)
                        option->m_OptionNCode = string_index;
                    option->m_Value[current_rank] = attr_value;

                    item_slot.SetRank(current_rank);
                    item_slot.SetRankAttr(current_rank, option);
                }
            } //end 'for'
        } //end 'if exist ranks'
    } //end 'if exist rank'

    BYTE number_of_sockets = 0;
    if(item_rule_info.IsEnableSocket())
    {
        if(xml.FindGetChildData("SocketNum", number_of_sockets) == false)
            return false;
        if(number_of_sockets)
            item_slot.SetSocketNum(number_of_sockets);
    }

    if(number_of_sockets)
    {
        stl_temp.resize(0);
        if(xml.FindGetChildData("SOCKET", stl_temp) == false)
            return false;

        char buffer[1024];

        std::string::size_type length = stl_temp.length();
        if(length == 0 || _countof(buffer) <= length)
            return false;

        strncpy(buffer, stl_temp.c_str(), _countof(buffer));
        buffer[_countof(buffer) - 1] = '\0';

        char* token = strtok(buffer, "[]");
        int count = 0;
        while(token != 0) {
            ++count;
            strncpy(buffer, token, _countof(buffer));
            token = strtok(NULL, "[]");
        }

        if(count > 1) {
            _LOG(__FUNCTION__"|Msg=invalid format rank option|");
            __asm { int 3 };
            return false;
        }

        typedef char* Infos[5]; //socket option args "SocketNo:Name:value:index:level"
        typedef Infos Tokens[SOCKET_MAX];

        Tokens tokens = { 0, };
        count = 0;
        token = strtok(buffer, " ");
        while(token != 0) {
            tokens[count++][0] = token;
            token = strtok(NULL, " ");
        }

        // stack overrun
        if(_countof(tokens) < count) {
            _LOG(__FUNCTION__"|Msg=invalid format socket option numbers|");
            __asm { int 3 };
            return false;
        }

        int upperbound = count;
        if(upperbound != 0) { // rank가 있는데, count==0이면???
            int offset = 0;
            for(count = 0 ; count < upperbound; ++upperbound) {
                Infos& socket_infos = tokens[count];
                token = strtok(socket_infos[count], ":");
                while(token != 0) {
                    socket_infos[offset++] = token;
                    token = strtok(NULL, ":");
                }

                if(_countof(socket_infos) < offset) {
                    _LOG(__FUNCTION__"|Msg=invalid format socket separated option numbers|");
                    __asm { int 3 };
                    return false;
                }

                eSOCKET current_sock = root_->ConvertSocketType(socket_infos[0]);
                int string_index = STRTBL_PARSER1->GetCodeByString(socket_infos[1]);
                int attr_value = atoi(socket_infos[2]);
                int attr_index = atoi(socket_infos[3]);
                int sock_level = atoi(socket_infos[4]);
                if(_countof(g_ITEM_OPTION) <= attr_index) {
                    _LOG(__FUNCTION__"|Msg=invalid format socket attributes index|index=%d|", attr_index);
                    __asm { int 3 };
                    return false;
                }

                const sSOCKET_OPTION* option_c = SocketOptionParser::Instance()->GetSocketOption(attr_index);
                sSOCKET_OPTION* option = const_cast<sSOCKET_OPTION*>(option_c);
                BOOST_STATIC_WARNING(0); //파서의 값을 설정중이다... 정상인 건가?
                if(option) {
                    option->m_AttrIndex = attr_index;
                    if(string_index > 0)
                        option->m_NCode = string_index;
                    option->m_iValue[current_sock] = attr_value;
                    item_slot.SetSocketAttr(current_sock, (eSOCKET_LEVEL)sock_level, option);
                }
            } //end 'for'
        } //end 'if exist sockets'
    }

    _Serialize_LimitedItem(false, xml, item_rule_info, item_slot);

    return true;
}

#endif //__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__

