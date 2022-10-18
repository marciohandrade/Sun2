#include "stdafx.h"
#include "ItemInfoParser.h"

#include "./Define.h"
#include "./Struct.h"

#include <ItemOptionHeader.h>
#include <SCItemSlot.h>

//==================================================================================================

ItemInfoParser* ItemInfoParser::item_info_parser_static_ = NULL;

ItemInfoParser* ItemInfoParser::CreateInstance()
{
    // (NOTE) thread-unsafe singleton
    item_info_parser_static_ = new ItemInfoParser;
    return item_info_parser_static_;
}

void ItemInfoParser::DestroyInstance()
{
    SAFE_DELETE(item_info_parser_static_);
}

//==================================================================================================

#ifdef _SERVER
static void _ItemLoggingForDuplicatedViolation(CODETYPE item_code, int duplicable_num,
                                               const char* category)
{
    SUNLOG(eCRITICAL_LOG,
           "ItemCode|%u|DupNum(%d) != 1|'%s' DupNum must be 1, can't duplicable",
           item_code, duplicable_num, category);
}

static void _ItemLoggingForFunctionViolation(CODETYPE item_code,
                                             const char* category,
                                             const char* func_attribute)
{
    SUNLOG(eCRITICAL_LOG, "ItemCode|%u|'%s' can't use '%s'", item_code, category, func_attribute);
}

#define LOG_ITEM_VIOLATION_DUPLICATE(arg0, arg1, arg2) \
    _ItemLoggingForDuplicatedViolation(arg0, arg1, arg2)
#define LOG_ITEM_VIOLATION_FUCTION(arg0, arg1, arg2) \
    _ItemLoggingForFunctionViolation(arg0, arg1, arg2)

#else //!_SERVER

#define LOG_ITEM_VIOLATION_DUPLICATE(arg0, arg1, arg2)
#define LOG_ITEM_VIOLATION_FUCTION(arg0, arg1, arg2)

#endif //!_SERVER

//==================================================================================================

ItemInfoParser::ItemInfoParser()
    : RENDER_EFFECT_ITEMS(render_effect_item_list_)
{
}

ItemInfoParser::~ItemInfoParser()
{
}

void
ItemInfoParser::Release()
{
    Unload();
}

void
ItemInfoParser::Init(DWORD unused_arg)
{
    __UNUSED(unused_arg);
    // (CHANGES) (f100505.1L) changes data structure related to the BASE_ITEMINFO
    loaded_ = false;
    cool_time_table_.clear();
#if defined (_NA_0_20091208_VENDOR_ITEM_SEARCH) || (defined(_DH_GM_ITEM_WINDOW) && !defined(_SERVER))
	item_name_code_table_.clear();
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
    render_effect_item_list_.clear();
    ZeroMemory(&item_table_, sizeof(item_table_));
    ZeroMemory(file_name_, sizeof(file_name_));
}

void
ItemInfoParser::Init(DWORD unused_arg, char* pack_file_name)
{
    Init(unused_arg);
    SetPackFileName(pack_file_name);
}


void
ItemInfoParser::Unload()
{
    // (CHANGES) (f100505.1L) changes data structure related to the BASE_ITEMINFO
    if (loaded_ == false) {
        return;
    }
    ;{ // release all resource in a table
        this->SetFirst();
        while (BASE_ITEMINFO* item_info = this->GetNext()) {
            delete item_info;
        };
        if (item_table_.seq_total_indexes_) {
            ::free(const_cast<uint16_t*>(item_table_.seq_total_indexes_));
        };
        if (item_table_.client_checksum_array_) {
            ::free(item_table_.client_checksum_array_);
        };
        ZeroMemory(&item_table_, sizeof(item_table_));
    };
    //
    render_effect_item_list_.clear();
    ;{  // (NOTE) it has not the usage case at this point in time (100505)
        DUP_EQUIP_GROUP_INFO_MAP::const_iterator it = dup_item_group_.begin();
        const DUP_EQUIP_GROUP_INFO_MAP::const_iterator end = dup_item_group_.end();
        for ( ; it != end; ++it) {
            delete it->second;
        }
        dup_item_group_.clear();
    };
#if defined (_NA_0_20091208_VENDOR_ITEM_SEARCH) || (defined(_DH_GM_ITEM_WINDOW) && !defined(_SERVER))
	item_name_code_table_.clear();
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH
    //
    // must be settled last position
    loaded_ = false;
}

// (CHANGES) (f100505.1L) add the item access facility which is supported an O(1) time complexity
bool ItemInfoParser::_AddItemInfo(CODETYPE item_code, BASE_ITEMINFO* item_info)
{
    if (item_code == 0 || item_code > USHRT_MAX || item_info == NULL) {
        return false;
    }
    --item_code;
    item_table_.array_[item_code] = item_info;
    return true;
}

// (CHANGES) (f100505.1L) add the item access facility which is supported an O(1) time complexity
bool ItemInfoParser::_BuildReference(int added_number_of_items)
{
    if (added_number_of_items <= 0) {
        return false;
    }
    const int resize_number = item_table_.number_of_total_items_ + added_number_of_items;
    if (item_table_.seq_total_indexes_) {
        ::free(const_cast<uint16_t*>(item_table_.seq_total_indexes_));
    }
    const int chunk_size = sizeof(uint16_t) * resize_number;
    ;{
        void* ptr = ::malloc(chunk_size);
        ZeroMemory(ptr, chunk_size);
        item_table_.seq_total_indexes_ = reinterpret_cast<uint16_t*>(ptr);
    };
    uint16_t* dest_it = const_cast<uint16_t*>(item_table_.seq_total_indexes_);
    //
    const BASE_ITEMINFO* const * it = item_table_.array_;
    const BASE_ITEMINFO* const * const end = &item_table_.array_[_countof(item_table_.array_)];
    while (true)
    {
        while (it < end && (*it) == NULL) {
            ++it;
        }
        if (it >= end) {
            break;
        }
        *dest_it = static_cast<uint16_t>(it - item_table_.array_);
        ++dest_it;
        ++it;
    }
    item_table_.number_of_total_items_ = resize_number;
    return true;
}

//==================================================================================================

BOOL
ItemInfoParser::_Load(BOOL reloaded)
{
    CODETYPE previous_item_code_for_debugging = 0;
    if (item_table_.client_checksum_array_ == NULL) {
        void* ptr = ::malloc(sizeof(ulong) * USHRT_MAX);
        ZeroMemory(ptr, (sizeof(ulong) * USHRT_MAX));
        item_table_.client_checksum_array_ = reinterpret_cast<ulong*>(ptr);
    };
    const int max_rows = GetRowSize();
    for (int row_no = 0; row_no < max_rows; ++row_no)
    {
        const CODETYPE item_code = GetDataDWORD("Code", row_no);
        if (!FlowControl::FCAssert(item_code != 0 && item_code <= USHRT_MAX))
        {
            SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"]|invalid item code spec.|Code=%d|File={%s}|"),
                   item_code, GetCurFileName());
            FASSERT(!"["__FUNCTION__"]|invalid item code spec.|");
        };

        BASE_ITEMINFO* item_info = GetItemInfo(item_code);
        if (item_info == NULL) 
        {
            item_info = new BASE_ITEMINFO;
            ZeroMemory(item_info, sizeof(*item_info));
            _AddItemInfo(item_code, item_info);
        }
        else
        {
            if (false == reloaded)
            {
                SUNLOG(eCRITICAL_LOG,
                       _T("["__FUNCTION__"]|the item code was already exists|Code=%d|File={%s}|"),
                       item_code, GetCurFileName());
                FASSERT(!"["__FUNCTION__"]|the item code was already exists|");
            }
        }

        item_info->m_Code = SLOTCODE(item_code);
        item_info->m_ChargeType = GetDataBYTE("ChargeType", row_no);
        item_info->m_ChargeSubType = GetDataBYTE("ChargeSubType", row_no);
        strncpy(item_info->m_pszName, GetDataString("Name", row_no), _countof(item_info->m_pszName));
        item_info->m_pszName[_countof(item_info->m_pszName) - 1] = '\0';
        item_info->m_NCode = GetDataDWORD("NameCode", row_no);

        item_info->m_bUse = (GetDataBYTE("NameColorType", row_no) != 0);
        // (NOTE) boundary...
        strncpy(item_info->m_szColor, GetDataString("NameColorCode", row_no),
                item_info->MAX_COLOR_LENGTH);

        item_info->m_SCNCode = GetDataDWORD("NameCode2", row_no);
        item_info->m_TDNCode = GetDataDWORD("DNCode3", row_no);
        item_info->m_wWICode = GetDataDWORD("WearImageCode", row_no);
#ifdef _NA_002050_20110216_ADD_GENDER
        // m_wWICode : 남성캐릭터에 사용될 그래픽리소스코드
        item_info->m_wWICode2 = GetDataDWORD("WearImageCode2", row_no); //여성캐릭터에 사용될 그래픽리소스코드
#endif //_NA_002050_20110216_ADD_GENDER        
        item_info->m_wVICode = GetDataDWORD("InvenImageCode", row_no);
        item_info->m_wDICode = GetDataDWORD("DropImageCode", row_no);
        item_info->m_wType   = GetDataWORD("Type", row_no);
        item_info->m_byEventCode = GetDataBYTE("Etype", row_no);
        item_info->m_nSoundIndex = GetDataInt("ItemSoundIndex", row_no);
    
        // (CHANGES) (091202) (WAVERIX) change type
        item_info->m_byEffectPosition = GetDataBYTE("EffectPosition", row_no);
        item_info->m_dwEffectCode     = StrToWzID(GetDataString("EffectCode", row_no));
        // (CHANGES) (091202) (WAVERIX) change type
        item_info->m_bEfffectBoneRot  = GetDataBYTE("RotBone", row_no);
        item_info->m_dwEquipBoneID = StrToWzID(GetDataString("BoneName", row_no));
        item_info->m_bEquipRot     = (GetDataBYTE("SABoneType", row_no) != 0);

        // (REF) eITEMSUBTYPE
        item_info->m_wSubType = GetDataWORD("SubType", row_no);
#ifdef _NA_002050_20110216_ADD_GENDER
        item_info->gender_type = GetDataBYTE(_T("GenderType"), row_no);
        if (item_info->gender_type != GenderType::kAll && 
            item_info->gender_type != GenderType::kMale && 
            item_info->gender_type != GenderType::kFemale)
        {
            item_info->gender_type = GenderType::kAll;
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|성별 설정이 옳바르지 않아서 0으로 초기화 합니다.|ItemCode = %u, 줄번호 = %u|"), 
                   __FUNCTION__, 
                   item_info->m_Code, 
                   row_no);
        }
#endif // _NA_002050_20110216_ADD_GENDER
    
        // (REF) BASE_ITEMINFO::eSetItemOption
        item_info->m_bySetOptionType = GetDataBYTE("SetOptionType", row_no);    

        item_info->set_group_code = GetDataWORD("SetType", row_no);
        item_info->m_LV = GetDataWORD("Level", row_no);
        SASSERT(item_info->m_LV > 0, "아이템의 레벨이 0이상이어야 한다. 데이터 오류!");

        item_info->m_Dura = GetDataBYTE("Dura", row_no);
        item_info->m_dwDuraStr = GetDataDWORD("DuraStrenth", row_no);
        if (item_info->m_dwDuraStr == 0)
        {
            // 기획에서 일부러 0으로 셋팅할 수 있음
            //          if (pInfo->IsWeapon() || pInfo->IsArmor())
            //              SUNLOG("[경고:%u]:DuraStrength가 0입니다.\n", pInfo->m_NCode);
        }

    #ifdef __NA_001062_20080623_STRENGTH_PENALTY
        item_info->m_dwStrengthPenaltyIdx = GetDataBYTE("StrengthPenaltyIndex", row_no);
    #endif

        item_info->m_DropLV = GetDataDWORD("ItemSellMoney", row_no);

        item_info->m_byPopup = GetDataBYTE("Popup", row_no);

        item_info->m_byMaterialType = GetDataBYTE("MaterialType", row_no);
        item_info->trade_sell_type = GetDataWORD("TradeSellType", row_no);
        item_info->m_byMixture = GetDataBYTE("Mixture", row_no);
        item_info->m_bPossessionItem = GetDataBYTE("ReversionType", row_no);
        //_NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_
        item_info->etherealization_mode_.value = GetDataBYTE("ExtractType", row_no);
        item_info->m_LimitEqLevel = GetDataWORD("LimitEqLevel", row_no);
        //__CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK
        item_info->m_LimitUseLevel = GetDataWORD("LimitUseLevel", row_no);
        item_info->m_wLimitStr = GetDataWORD("LimitStr", row_no);
        item_info->m_wLimitDex = GetDataWORD("LimitDex", row_no);
        item_info->m_wLimitVit = GetDataWORD("LimitVit", row_no);
        item_info->m_wLimitInt = GetDataWORD("LimitInt", row_no);
        item_info->m_wLimitSpr = GetDataWORD("LimitSpr", row_no);
        item_info->m_wLimitSkil1  = GetDataWORD("LimitSkill1", row_no);
        item_info->m_wLimitSkil2  = GetDataWORD("LimitSkill2", row_no);
        item_info->m_wMinDamage   = GetDataWORD("MinDamage", row_no);
        item_info->m_wMaxDamage   = GetDataWORD("MaxDamage", row_no);
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        item_info->m_wPhyAttRate = static_cast<uint16_t>(
            util::LL_Round(GetDataNumber("PhyAttRate", row_no) * 100.0f));
        item_info->m_wPhyAvoid = static_cast<uint16_t>(
            util::LL_Round(GetDataNumber("PhyAvoid", row_no) * 100.0f));
    #else
        item_info->m_wPhyAttRate  = GetDataWORD("PhyAttRate", row_no);
        item_info->m_wPhyAvoid = GetDataWORD("PhyAvoid", row_no);
    #endif
        item_info->m_wPhyAttSpeed = GetDataWORD("PhyAttSpeed", row_no);
        item_info->m_wPhyDef   = GetDataWORD("PhyDef", row_no);

        item_info->m_byBaseAttType = GetDataBYTE("BaseAttType", row_no);
        item_info->m_byMagicalAttType = GetDataBYTE("MagicalAttType", row_no);
        item_info->m_byMeleeType = GetDataBYTE("MeleeType", row_no);

        item_info->m_fAttRange   = GetDataFloat("AttRange", row_no);
        item_info->m_byArmorType = GetDataBYTE("ArmorType", row_no);
        item_info->m_wMagicAttMinDamgage = GetDataWORD("MagicAttMinDamgage", row_no);
        item_info->m_wMagicAttMaxDamgage = GetDataWORD("MagicAttMaxDamgage", row_no);
        item_info->m_wMagicAttSpeed = GetDataWORD("MagicAttSpeed", row_no);
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        item_info->m_wMagicDef = GetDataWORD("MagicDef", row_no);
    #endif
        item_info->m_wSpeed    = GetDataWORD("Speed", row_no);
        ;{
            // NOTE: f110408.2L, added composed bit field = { (1 << eCHAR_BERSERKER) | ... }
            item_info->m_wEqClass1 = GetDataWORD("EqClass1", row_no);
            item_info->m_wEqClass2 = GetDataWORD("EqClass2", row_no);
            item_info->m_wEqClass3 = GetDataWORD("EqClass3", row_no);
            item_info->m_wEqClass4 = GetDataWORD("EqClass4", row_no);
            item_info->m_wEqClass5 = GetDataWORD("EqClass5", row_no);            
            // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
            item_info->m_wEqClass6 = GetDataWORD("EqClass6", row_no);      
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            item_info->m_wEqClass7 = GetDataWORD("EqClass7", row_no);
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            item_info->m_wEqClass8 = GetDataWORD("EqClass8", row_no);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            
            uint16_t composed = 0;
            if (item_info->m_wEqClass1) { composed |= (1 << eCHAR_BERSERKER); }
            if (item_info->m_wEqClass2) { composed |= (1 << eCHAR_DRAGON); }
            if (item_info->m_wEqClass3) { composed |= (1 << eCHAR_SHADOW); }
            if (item_info->m_wEqClass4) { composed |= (1 << eCHAR_VALKYRIE); }
            if (item_info->m_wEqClass5) { composed |= (1 << eCHAR_ELEMENTALIST); }            
            // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
            if (item_info->m_wEqClass6) { composed |= (1 << eCHAR_MYSTIC); }
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            if (item_info->m_wEqClass7) { composed |= (1 << eCHAR_HELLROID); }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            if (item_info->m_wEqClass8) { composed |= (1 << eCHAR_WITCHBLADE); }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE



            item_info->equip_class_bits = composed;
        };
        // (REF) {eEQUIPCONTAINER_POS, eRIDER_PARTS_POS} 장비 위치 + 라이더 파츠 장착 위치
        item_info->m_wEqPos    = GetDataBYTE("EqPos", row_no);

    #ifndef _NA_000251_20100727_SOCKET_SYSTEM
        item_info->m_wMaxRank   = GetDataWORD("MaxRank", row_no);// 최대 랭크??
        item_info->m_SocketHole = GetDataWORD("SocketUse", row_no);
        ASSERT(SAFE_NUMERIC_TYPECAST(WORD, item_info->m_SocketHole, BYTE));
        item_info->m_SocketNum =GetDataWORD("SocketNum", row_no);
    #endif // _NA_000251_20100727_SOCKET_SYSTEM
        char szName[32];

        for (int op = 0; op < _countof(item_info->m_byOptionExEffect); ++op)
        {
            BYTE& rExcerciseEffect = item_info->m_byOptionExEffect[op];
            _snprintf(szName, 32, "ExerciseEffect%d", op + 1);
            rExcerciseEffect = GetDataBYTE(szName, row_no);
            switch (rExcerciseEffect)
            {
            case eITEM_EFFECT_NONE: case eITEM_EFFECT_ON_EQUIP: case eITEM_EFFECT_ON_INVEN: break;
            default:
                SUNLOG(eCRITICAL_LOG,
                       "[%u] OptionExcerciseEffect Value (%u) is Invalid, fixed to NONE",
                       item_info->m_Code, rExcerciseEffect);
                rExcerciseEffect = eITEM_EFFECT_NONE;
                break;
            }

            _snprintf(szName, 32, "OptionKind%d", op + 1);
            item_info->m_wOptionIndex[op] = GetDataWORD(szName, row_no);
            _snprintf(szName, 32, "OptionType%d", op + 1);
            item_info->m_byOptionType[op] = GetDataBYTE(szName, row_no);

            _snprintf(szName, 32, "OptionValue%d", op + 1);
            item_info->m_iOptionValue[op] = GetDataInt(szName, row_no);
            if (item_info->m_wOptionIndex[op] >= _countof(g_ITEM_OPTION))
            {
                SUNLOG(eCRITICAL_LOG,
                       "ItemCode|%u|OptionIndex : OptionValue = %u : %u|out of ranges",
                       item_info->m_Code,
                       item_info->m_wOptionIndex[op], item_info->m_iOptionValue[op]);
            }
        }

        for (int i = 0; i < _countof(item_info->m_byPCOptionExEffect); ++i)
        {
            BYTE& rExcerciseEffect = item_info->m_byPCOptionExEffect[i];
            _snprintf(szName, 32, "PCExerciseEffect%d", i+1);
            rExcerciseEffect = GetDataBYTE(szName, row_no);
            switch (rExcerciseEffect)
            {
            case eITEM_EFFECT_NONE: case eITEM_EFFECT_ON_EQUIP: case eITEM_EFFECT_ON_INVEN: break;
            default:
                SUNLOG(eCRITICAL_LOG,
                       "ItemCode|%u|OptionExcerciseEffect value (%u) is invalid, fixed to NONE",
                       item_info->m_Code, rExcerciseEffect);
                rExcerciseEffect = eITEM_EFFECT_NONE;
                break;
            }

            _snprintf(szName, 32, "PCOptionKind%d", i+1);
            item_info->m_wPCOptionIndex[i] = GetDataWORD(szName, row_no);
            _snprintf(szName, 32, "PCOptionType%d", i+1);
            item_info->m_byPCOptionType[i] = GetDataBYTE(szName, row_no);

            _snprintf(szName, 32, "PCOptionValue%d", i+1);
            item_info->m_iPCOptionValue[i] = GetDataInt(szName, row_no);
            if (item_info->m_wPCOptionIndex[i] >= _countof(g_ITEM_OPTION))
            {
                SUNLOG(eCRITICAL_LOG,
                       "ItemCode|%u|OptionIndex : OptionValue = %u : %u|out of ranges",
                       item_info->m_Code,
                       item_info->m_wPCOptionIndex[i], item_info->m_iPCOptionValue[i]);
            }
        }

        item_info->m_wMissile = GetDataWORD("Missile", row_no);
        item_info->m_bHeadType = GetDataBYTE("HeadType", row_no);

        if (item_info->m_bHeadType)
            --item_info->m_bHeadType;// (NOTE) what is it?

        // - 해당 값이 1이면 얼굴과 머리 카락을 그리지 않는다.
        // (헬멧 사용시 머리카락 삐져나오는 경우를 위한 값)
        // - 추가: 해당 값이 2이면 머리카락을 그리지 않는다.
        // (기존 스크립트와의 호환성을 위해.. 새로 값을 추가함.)
        item_info->m_wCustomize = GetDataWORD("Customize", row_no);
#ifdef _NA_002050_20110216_ADD_GENDER
        item_info->m_bHeadType1 = GetDataBYTE("HeadType2", row_no);
        if (item_info->m_bHeadType1)
            --item_info->m_bHeadType1;
        item_info->m_wCustomize1 = GetDataWORD("Customize2", row_no);
#endif //_NA_002050_20110216_ADD_GENDER

        item_info->m_byDupNum = GetDataBYTE("DupNum", row_no);
        if (item_info->m_byDupNum == 0)
        {
            SUNLOG(eCRITICAL_LOG, "ItemCode|%u|m_byDupNum = 0", item_info->m_Code);
            item_info->m_byDupNum = 1;
        }

        item_info->m_byDupEquipNum     = GetDataBYTE("DupEquipNum", row_no);
        item_info->m_byDupEquipTypeNum = GetDataBYTE("DupEquipTypeNum", row_no);

        item_info->m_byDupEquipGroup    = GetDataBYTE("DupEquipGroup", row_no);
        item_info->m_byDupEquipGroupNum = GetDataBYTE("DupEquipGroupNum", row_no);
        DUP_EQUIP_GROUP_INFO_MAP::const_iterator find_dup_equip = \
            dup_item_group_.find(item_info->m_byDupEquipGroup);
        // 존재하면..
        if (find_dup_equip != dup_item_group_.end())
        {
            // 존재하면.. 현재 값을 체크한다.
            const DUP_EQUIP_GROUP_INFO* equip_group_info = find_dup_equip->second;
            if (equip_group_info->m_byDupEquipGroupNum != item_info->m_byDupEquipGroupNum) {
                SUNLOG(eCRITICAL_LOG,
                       "["__FUNCTION__"]|ItemCode|%d|GroupNum|%d|Invalid DupEquipGroupNum",
                       item_info->m_Code, item_info->m_byDupEquipGroupNum);
            }
        }
        else
        {
            //존재 안하면 메모리에 넣는다.
            DUP_EQUIP_GROUP_INFO* pGroupInfo = new DUP_EQUIP_GROUP_INFO;
            pGroupInfo->m_byDupEquipGroup    = item_info->m_byDupEquipGroup;
            pGroupInfo->m_byDupEquipGroupNum = item_info->m_byDupEquipGroupNum;
            dup_item_group_.insert(std::make_pair(pGroupInfo->m_byDupEquipGroup, pGroupInfo));
        }

        item_info->m_byWasteType   = GetDataBYTE("WasteType", row_no);
        item_info->m_wCoolTimeType = GetDataWORD("CoolTimeType", row_no);

        item_info->m_wHealHP       = GetDataWORD("HealHP", row_no);
        item_info->m_dwExp       = GetDataDWORD64("ExpHP", row_no);// 경험치 물약을 위해서... 후우...
        item_info->m_wTimes      = GetDataWORD("Times", row_no);
        item_info->m_dCoolTime   = GetDataDWORD("CoolTime", row_no);
        item_info->m_wHealHPTime = GetDataDWORD("HealTime", row_no);

        if (0 != item_info->m_wCoolTimeType)
        {
            ITEM_COOLTIME_ITER it = cool_time_table_.find(item_info->m_wCoolTimeType);
            if (it == cool_time_table_.end())
            {
                ITEM_COOLTIME_INFO item_cooltime_info;
                item_cooltime_info.m_wCoolTimeType = item_info->m_wCoolTimeType;
                item_cooltime_info.m_dwCoolTime = item_info->m_dCoolTime;
                cool_time_table_.insert(\
                    std::make_pair(item_info->m_wCoolTimeType, item_cooltime_info));
            }
            else
            {
                it->second.m_wCoolTimeType =  item_info->m_wCoolTimeType;
                it->second.m_dwCoolTime = item_info->m_dCoolTime;
            }
        }

        //__NA_1323_CHAO_TIME_DECREASE_ITEM
        item_info->m_byChaoTimeDec = GetDataBYTE("ChaoTimeDec", row_no);
        // m_wSpecialEffect -> m_wQuestCode 로 바뀜 (2006. 8. 7.) 
        item_info->m_wQuestCode     = GetDataWORD("QuestCode", row_no);
        // (REF) Lottery index group
        item_info->m_MaterialCode   = GetDataWORD("MaterialCode", row_no);
        // (WARNING) 다목적으로 사용되는 항목이다.
        // 아이템에 연결된 (스킬/MAPCODE/Wax Level)인 경우, 등등 아이템 타입에 따라 달리 처리된다.
        item_info->m_wSkillCode     = GetDataWORD("SkillCode", row_no);

    #ifdef _NA_000251_20100727_SOCKET_SYSTEM
        item_info->zard_grade = GetDataBYTE(_T("ZardGrade"), row_no);
        item_info->zard_item_option_kind = GetDataBYTE(_T("ZardOptionKind"), row_no);
        item_info->zard_success_per = GetDataWORD(_T("ZardSuccesPer"), row_no);
    #endif // _NA_000251_20100727_SOCKET_SYSTEM
        
        item_info->m_ExchangedItem = GetDataWORD("ExchangedItem", row_no);
        item_info->m_RidingClass.value = GetDataBYTE("EqRidingClass", row_no);
        //------------------------------------------------------------------------------------------
        // 2006/3/20 : 디바인 관련 
        item_info->m_wDivLimitStr = GetDataWORD("DivLimitStr", row_no);
        item_info->m_wDivLimitDex = GetDataWORD("DivLimitDex", row_no);
        item_info->m_wDivLimitVit = GetDataWORD("DivLimitVit", row_no);
        item_info->m_wDivLimitInt = GetDataWORD("DivLimitInt", row_no);
        item_info->m_wDivLimitSpr = GetDataWORD("DivLimitSpr", row_no);
        item_info->m_wDivLimitSkill1    = GetDataWORD("DivLimitSkill1", row_no);
        item_info->m_wDivLimitSkill2    = GetDataWORD("DivLimitSkill2", row_no);
        item_info->m_wDivPhyMinDamage   = GetDataWORD("DivPhyMinDamage", row_no);
        item_info->m_wDivPhyMaxDamage   = GetDataWORD("DivPhyMaxDamage", row_no);
        item_info->m_wDivMagicMinDamage = GetDataWORD("DivMagicMinDamage", row_no);
        item_info->m_wDivMagicMaxDamage = GetDataWORD("DivMagicMaxDamage", row_no);
        item_info->m_wDivPhyDef   = GetDataWORD("DivPhyDef", row_no);
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        item_info->m_wDivMagicDef = GetDataWORD("DivMagicDef", row_no);
    #endif
        //------------------------------------------------------------------------------------------
        // Area Conquest Section
        // 'm_wACCode' changes to smalltype after the ep2 AC system
        item_info->m_wACCode         = GetDataBYTE("ACCode", row_no);
        item_info->m_byACReferenceID = GetDataBYTE("ACReferenceID", row_no);

        if (item_info->m_wACCode == 0 && item_info->m_byACReferenceID != 0)
        {
            SUNLOG(eCRITICAL_LOG,
                   "ItemCode|%u|m_wACCode|%d|m_byACReferenceID|%d|"
                   "if ref_id != 0 then must be ACcode != 0",
                   item_info->m_Code, item_info->m_wACCode, item_info->m_byACReferenceID);
        }
        //------------------------------------------------------------------------------------------
        // Elite Item Option
        item_info->m_wElitePhyMinDamage = GetDataWORD("ElitePhyMinDamage", row_no);
        item_info->m_wElitePhyMaxDamage = GetDataWORD("ElitePhyMaxDamage", row_no);
        item_info->m_wEliteMagicMinDamage = GetDataWORD("EliteMagicMinDamage", row_no);
        item_info->m_wEliteMagicMaxDamage = GetDataWORD("EliteMagicMaxDamage", row_no);
        item_info->m_wElitePhyDef   = GetDataWORD("ElitePhyDef", row_no);
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        item_info->m_wEliteMagicDef = GetDataWORD("EliteMagicDef", row_no);
    #endif
        //------------------------------------------------------------------------------------------
        // Unique Item Option
        item_info->m_wUniPhyMinDamage = GetDataWORD("UniPhyMinDamage", row_no);
        item_info->m_wUniPhyMaxDamage = GetDataWORD("UniPhyMaxDamage", row_no);
        item_info->m_wUniMagicMinDamage = GetDataWORD("UniMagicMinDamage", row_no);
        item_info->m_wUniMagicMaxDamage = GetDataWORD("UniMagicMaxDamage", row_no);
        item_info->m_wUniPhyDef   = GetDataWORD("UniPhyDef", row_no);
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        item_info->m_wUniMagicDef = GetDataWORD("UniMagicDef", row_no);
    #endif
    #if SUN_CODE_BACKUP
        // 클라이언트용 : 아이템 종류 선정
        item_info->m_wItemListType = GetDataWORD("ItemListType", row_no);
    #endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
        item_info->m_betakey = GetDataInt("BetaKeyCode", row_no);
#endif

        


        //------------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------------
        // Rule check section
        //

        //------------------------------------------------------------------------------------------
        // Very important data
        // Rule: 겹치는 아이템은 유효기간을 가지지 않는다.
        switch (item_info->m_ChargeSubType)
        {
#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
        case eCHARGE_SUB_IMMEDIATELY_USE:
        case eCHARGE_SUB_IMMEDIATELY_USE_AND_DELETE:
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
        case eCHARGE_SUB_FIXED_AMOUNT:
        case eCHARGE_SUB_FIXED_AMOUNT_EX:
        case eCHARGE_SUB_FIXED_QUANTITY:
        case eCHARGE_SUB_COORDINATE: // (WAVERIX) 이것 역시 중첩 될 수 없다.
        case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
        case eCHARGE_SUB_FATE:
        case eCHARGE_SUB_COORDINATE_EX:
        case eCHARGE_SUB_RIDER_WITH_PART:
        case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER: //유효기간은 없지만, 동일 타입인 관계로 여기에서 검사
        case eCHARGE_SUB_FIXED_AMOUNT_EX2:
            //assert(item_info->m_byDupNum == 1);
            if (item_info->m_byDupNum != 1) {
                LOG_ITEM_VIOLATION_DUPLICATE(item_info->m_Code, item_info->m_byDupNum, "ChargeItem");
                item_info->m_byDupNum = 1;
            }
            break;
        }

        switch (item_info->m_wType)
        {
        case eITEMTYPE_TWOHANDAXE:
        case eITEMTYPE_TWOHANDSWORD:
        case eITEMTYPE_ONEHANDSWORD:
        case eITEMTYPE_SPEAR:
        case eITEMTYPE_WHIP:
        case eITEMTYPE_DAGGER:
        case eITEMTYPE_ONEHANDCROSSBOW:
        case eITEMTYPE_ETHERWEAPON:
        case eITEMTYPE_SCIMITAR:
        case eITEMTYPE_STAFF:
        case eITEMTYPE_ORB:
        case eITEMTYPE_GUNTLET: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eITEMTYPE_POLEARM: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eITEMTYPE_HELLROID_1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eITEMTYPE_ARCBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            
            assert(item_info->m_byDupNum == 1);
            if (item_info->m_byDupNum != 1) {
                LOG_ITEM_VIOLATION_DUPLICATE(item_info->m_Code, item_info->m_byDupNum, "WeaponItem");
                item_info->m_byDupNum = 1;
            }
            break;
        // 방어구
        case eITEMTYPE_ARMOR:
        case eITEMTYPE_PROTECTOR:
        case eITEMTYPE_HELMET:
        case eITEMTYPE_PANTS:
        case eITEMTYPE_BOOTS:
        case eITEMTYPE_GLOVE:
        case eITEMTYPE_BELT:
        case eITEMTYPE_SHIRTS:
            assert(item_info->m_byDupNum == 1);
            if (item_info->m_byDupNum != 1) {
                LOG_ITEM_VIOLATION_DUPLICATE(item_info->m_Code, item_info->m_byDupNum, "ArmorItem");
                item_info->m_byDupNum = 1;
            }
            break;
            // 악세서리
        case eITEMTYPE_RING:
        case eITEMTYPE_NECKLACE:
            assert(item_info->m_byDupNum == 1);
            if (item_info->m_byDupNum != 1) {
                LOG_ITEM_VIOLATION_DUPLICATE(item_info->m_Code, item_info->m_byDupNum, "Accessory");
                item_info->m_byDupNum = 1;
            }
            break;
        case eITEMTYPE_BERSERKER_SACCESSORY :
        case eITEMTYPE_DRAGON_SACCESSORY:
        case eITEMTYPE_VALKYRIE_SACCESSORY:
        case eITEMTYPE_SHADOW_SACCESSORY:
        case eITEMTYPE_ELEMENTALIST_SACCESSORY:
        case eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY1: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eITEMTYPE_HELLROID_SPECIAL_ACCESSORY1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY1: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eITEMTYPE_ELEMENTALIST_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_ALLCLASS_CHAOSEYE:             //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_BERSERKER_KALVUS_ACCESSORY:    //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_DRAGON_KALVUS_ACCESSORY:       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_VALKYRIE_SHADOW_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_MYSTIC_KALVUS_ACCESSORY:       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_HELLROID_WITCHBLADE_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
            assert(item_info->m_byDupNum == 1);
            if (item_info->m_byDupNum != 1) {
                LOG_ITEM_VIOLATION_DUPLICATE(item_info->m_Code, item_info->m_byDupNum, "SpecialAccessory");
                item_info->m_byDupNum = 1;
            }
            break;
        }; //end 'switch' for modify duplicable item


        switch (item_info->m_wType)
        {
        case eITEMTYPE_TWOHANDAXE:
        case eITEMTYPE_TWOHANDSWORD:
        case eITEMTYPE_ONEHANDSWORD:
        case eITEMTYPE_SPEAR:
        case eITEMTYPE_WHIP:
        case eITEMTYPE_DAGGER:
        case eITEMTYPE_ONEHANDCROSSBOW:
        case eITEMTYPE_ETHERWEAPON:
        case eITEMTYPE_SCIMITAR:
        case eITEMTYPE_STAFF:
        case eITEMTYPE_ORB:
        case eITEMTYPE_GUNTLET: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eITEMTYPE_POLEARM: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eITEMTYPE_HELLROID_1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eITEMTYPE_ARCBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_WEAPON);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_WEAPON)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_WEAPON);
            }
            break;
            // 방어구
        case eITEMTYPE_ARMOR:
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_ARMOR);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_ARMOR)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_ARMOR);
            }
            break;
        case eITEMTYPE_PROTECTOR:
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_PROTECTOR);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_PROTECTOR)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_PROTECTOR);
            }
            break;
        case eITEMTYPE_HELMET:
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_HELMET);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_HELMET)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_HELMET);
            }
            break;
        case eITEMTYPE_PANTS:
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_PANTS);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_PANTS)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_PANTS);
            }
            break;
        case eITEMTYPE_BOOTS:
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_BOOTS);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_BOOTS)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_BOOTS);
            }
            break;
        case eITEMTYPE_GLOVE:
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_GLOVE);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_GLOVE)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_GLOVE);
            }
            break;
        case eITEMTYPE_BELT:
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_BELT);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_BELT)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_BELT);
            }
            break;
        case eITEMTYPE_SHIRTS:
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_SHIRTS);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_SHIRTS)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_SHIRTS);
            }
            break;
            // 악세서리
        case eITEMTYPE_RING:
            if (item_info->m_wEqPos != eEQUIPCONTAINER_RING1 &&
                item_info->m_wEqPos != eEQUIPCONTAINER_CHARGE1 &&
                item_info->m_wEqPos != eEQUIPCONTAINER_CHARGE2 &&
                item_info->m_wEqPos != eEQUIPCONTAINER_CHARGE3 &&
                item_info->m_wEqPos != eEQUIPCONTAINER_CHARGE4 &&
                item_info->m_wEqPos != eEQUIPCONTAINER_CHARGE5 &&
                item_info->m_wEqPos != eEQUIPCONTAINER_PC_ROOM1 &&
                item_info->m_wEqPos != eEQUIPCONTAINER_PC_ROOM2 &&
                item_info->m_wEqPos != eEQUIPCONTAINER_PC_ROOM3)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_RING1);
            }
            break;
        case eITEMTYPE_NECKLACE: 
            assert(item_info->m_wEqPos == eEQUIPCONTAINER_NECKLACE);
            if (item_info->m_wEqPos != eEQUIPCONTAINER_NECKLACE)
            {
                SUNLOG(eCRITICAL_LOG, "ItemCode|%u|EqPos:%u != value:%u\n",
                       item_info->m_Code, item_info->m_wEqPos, eEQUIPCONTAINER_NECKLACE);
            }
            break;
        case eITEMTYPE_BERSERKER_SACCESSORY:
        case eITEMTYPE_DRAGON_SACCESSORY:
        case eITEMTYPE_VALKYRIE_SACCESSORY:
        case eITEMTYPE_SHADOW_SACCESSORY:
        case eITEMTYPE_ELEMENTALIST_SACCESSORY:
        case eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY1: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eITEMTYPE_HELLROID_SPECIAL_ACCESSORY1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY1: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eITEMTYPE_ALLCLASS_CHAOSEYE:             //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_ELEMENTALIST_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_BERSERKER_KALVUS_ACCESSORY:    //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_DRAGON_KALVUS_ACCESSORY:       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_VALKYRIE_SHADOW_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_MYSTIC_KALVUS_ACCESSORY:       //_NA_007827_20141117_ADD_ACCESSORY_TYPE
        case eITEMTYPE_HELLROID_WITCHBLADE_KALVUS_ACCESSORY: //_NA_007827_20141117_ADD_ACCESSORY_TYPE
            ASSERT(item_info->m_wEqPos == eEQUIPCONTAINER_SACCESSORY1);
            break;
        case eITEMTYPE_RIDER_PARTS:
            {
                const eRIDER_CLASS& rider = item_info->m_RidingClass;
                FASSERT(rider.value == rider.Charger ||
                        rider.value == rider.Leopard ||
                        rider.value == rider.OrcPig || 
                        rider.value == rider.Kalvus );
                FASSERT(item_info->m_wEqPos == eRIDER_PARTS_POS::HeadPart ||
                        item_info->m_wEqPos == eRIDER_PARTS_POS::BodyPart ||
                        item_info->m_wEqPos == eRIDER_PARTS_POS::LegsPart);
            }
            break;
        }
        // 아이템 분배
        item_info->m_bApplyItemDistribution = (GetDataBYTE("UseItemDistribution", row_no) != 0);

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
        item_info->dice_cast_type = GetDataBYTE("DiceCast", row_no);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

        //------------------------------------------------------------------------------------------
        // CHANGES: f110414.1L, internal configuration. calcuate 'SubTypeDetailed' value
        item_info->_UpdateSubTypeDetailedValue();
        //------------------------------------------------------------------------------------------
        //  Event Handling
        //
        switch (item_info->m_wType)
        {
        case eITEMTYPE_RENDER_EFFECT_ITEM:
            render_effect_item_list_.push_back((SLOTCODE)item_info->m_Code);
            break;
        }

    #ifdef _DEBUG
        static BASE_ITEMINFO* pPreInfo = 0;
        pPreInfo = item_info;//스크립트 뻑날때 이전 아이템 정보를 알기 위함!!
    #endif

        // 유효성을 검증한다.
        CheckValidity(item_info);

        previous_item_code_for_debugging = item_code;
        // CHANGES: f110802.3L
        item_table_.client_checksum_array_[item_code - 1] =
            util::StreamHashKey::GetMemoryHashKey(item_info, sizeof(*item_info));
    }

    // (CHANGES) (f100505.1L) add the item access facility which is supported an O(1) time complexity
    if (!FlowControl::FCAssert(_BuildReference(max_rows))) {
        return false;
    }
    loaded_ = true;

    return true;
}

//==================================================================================================

void
ItemInfoParser::CheckValidity(BASE_ITEMINFO* item_info)
{
    const SLOTCODE item_code = item_info->m_Code;
    const eCHARGE_SUB_TYPE charge_sub_type = eCHARGE_SUB_TYPE(item_info->m_ChargeSubType);

    //Enchant
    if (SCItemSlot::CanUseMixture(item_info, MIXTURE_ENCHANT))
    {
        if (item_info->IsWeapon() == 0 &&
            item_info->IsArmor() == 0 &&
            item_info->m_wType != eITEMTYPE_WINGS) // CHANGES: f111007.1L, added rule
        { // + rider parts 명시적인 인챈트 기능은 아니다.
            LOG_ITEM_VIOLATION_FUCTION(item_code, "isn't Weapon, isn't Armor, isn't Wings", "Enchant");
        }
    }

    //RankUp
    if (SCItemSlot::CanUseMixture(item_info, MIXTURE_RANKUP))
    {
        if (item_info->IsChargeItem()) {
            if (charge_sub_type != eCHARGE_SUB_FIXED_AMOUNT_WAIT) {
                LOG_ITEM_VIOLATION_FUCTION(item_code, "Charge item", "Rankup");
            }
        }
        if (item_info->IsElite()) {
            LOG_ITEM_VIOLATION_FUCTION(item_code, "Elite item", "Rankup");
        }
        if (item_info->IsUnique()) {
            LOG_ITEM_VIOLATION_FUCTION(item_code, "Unique item", "Rankup");
        }

        if (item_info->IsWeapon() == 0 &&
           item_info->IsArmor() == 0 &&
           item_info->IsAccessory() == 0)
        {
            LOG_ITEM_VIOLATION_FUCTION(item_code, \
                                       "isn't Weapon, isn't Armor, isn't Accessory", "Rankup");
        }

        if (item_info->m_wSubType ==  eITEMSUBTYPE_FATE)
        {
            LOG_ITEM_VIOLATION_FUCTION(item_code, "Lucky item", "Rankup");
            FASSERT(0);
        }
    }

    //Crystalize
    if (SCItemSlot::CanUseMixture(item_info, MIXTURE_CRYSTALIZE))
    {
        if (item_info->IsChargeItem() && (item_info->m_wSubType !=  eITEMSUBTYPE_FATE))
        {
            if (item_info->m_ChargeSubType != eCHARGE_SUB_FIXED_AMOUNT_WAIT) {
                LOG_ITEM_VIOLATION_FUCTION(item_code, "Charge item", "Crystalize");
            }
        }

        // [10/26/2009 namka199]결정화 확장 작업으로 엘리트 아이템의 결정화가 가능하게 되었음.
        // 유니크 아이템은 불가
        if (item_info->IsUnique())
        {
            LOG_ITEM_VIOLATION_FUCTION(item_code, "Unique item", "Crystalize");
        }

        if (item_info->IsWeapon() == 0 &&
           item_info->IsArmor() == 0 &&
           item_info->IsAccessory() == 0)
        {
            LOG_ITEM_VIOLATION_FUCTION(item_code, \
                                       "isn't Weapon, isn't Armor, isn't Accessory", "Crystalize");
        }
    }

    //Compose
    if (SCItemSlot::CanUseMixture(item_info, MIXTURE_COMPOSE))
    {
        // noting to do
    };

    //ExtraStone
    if (SCItemSlot::CanUseMixture(item_info, MIXTURE_EXTRASTONE))
    {
        // 무기, 방어구, 악세서리, 특수 악세서리
        if (item_info->IsWeapon() == 0 && 
           item_info->IsArmor() == 0 &&
           item_info->IsAccessory() == 0 &&
           item_info->IsSpecialAccessory() == 0)
        {
            LOG_ITEM_VIOLATION_FUCTION(item_code, \
                "isn't Weapon, isn't Armor, isn't Accessory, isn't SpecialAccessory", "Extra Stone");
        }
    }

    // 아이템 계열값을 검사한다.
    /* SubType에따라 LV값을 다양하게 사용하므로 이런식으로 단순 검증하는것은 의미 없다. - arycoat 2012.07.20
    if (item_info->GetSubTypeDetail() == ItemSubTypeDetail::kInvalid)
    {
        switch(item_info->m_wSubType)
        {
            case eITEMSUBTYPE_ELITE_SSQ:
            case eITEMSUBTYPE_ELITE:
            case eITEMSUBTYPE_UNIQUE:
            case eITEMSUBTYPE_FATE:
            case eITEMSUBTYPE_RARE:
            case eITEMSUBTYPE_GENERAL:
            {
                SUNLOG(eCRITICAL_LOG, 
                    _T( "|아이템 계열값이 레벨과 일치하지 않습니다."
                        "|ItemCode = %u, SubType = %d, ItemLevel = %u|"), 
                        item_code, item_info->m_wSubType, item_info->m_LV);
            } break;
            default:
                break;
        }
    }*/

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    // 무기 공격타입 체크
    if (item_info->IsWeapon())
    {
        if (item_info->m_byMeleeType == 0 || item_info->m_byMeleeType >= eMELEE_TYPE_MAX)
        {
            SUNLOG(eCRITICAL_LOG, "ItemCode|%u|Invalid m_byMeleeType(%d)!",
                   item_code, item_info->m_byMeleeType);
            item_info->m_byMeleeType = 1;
        }
    }

    // 방어구 방어타입 체크
    if (item_info->IsArmor())
    {
        if (item_info->m_byArmorType == 0 || item_info->m_byArmorType >= eARMOR_TYPE_MAX)
        {
            SUNLOG(eCRITICAL_LOG, "ItemCode|%u|Invalid m_byArmorType(%d)!",
                   item_code, item_info->m_byArmorType);
            item_info->m_byArmorType = 1;
        }
    }
#endif

    // CHANGES: f110329.5L, added rules, forced execlusive option
    if (item_info->IsExistInvenOptions() && item_info->IsExistEquipOptions())
    {
        SUNLOG(eCRITICAL_LOG,
               _T("ItemCode|%u|default options application has only one thing. ")
               _T("ex: Do apply options on equip? or Do apply options on inven?"),
               item_code);
        FASSERT("forced stop");
    }

    // 비율타입 옵션 체크
    for (int op = 0; op < _countof(item_info->m_wOptionIndex); ++op)
    {
        if (!CheckOptionValidity(item_info->m_wOptionIndex[op],
                                item_info->m_byOptionType[op],
                                item_info->m_iOptionValue[op]))
        {
            SUNLOG(eCRITICAL_LOG,
                   "|ItemCode=%u|Invalid Option|OptionIndex=%d|OptionType=%d|OptionValue=%d|", 
                   item_code,
                   item_info->m_wOptionIndex[op],
                   item_info->m_byOptionType[op],
                   item_info->m_iOptionValue[op]);
        }
    }

    if (item_info->m_wType == eITEMTYPE_PET)
    {
#ifdef _NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE
        if (charge_sub_type != eCHARGE_SUB_GENERAL &&
            charge_sub_type != eCHARGE_SUB_FIXED_QUANTITY &&
            charge_sub_type != eCHARGE_SUB_FIXED_AMOUNT)
        {
            SUNLOG(eCRITICAL_LOG, "ItemCode|%u|Invalid Pet SubType(%u)", 
                item_code, charge_sub_type);
            FASSERT(0);
        }
#else
        if (charge_sub_type != eCHARGE_SUB_GENERAL &&
           charge_sub_type != eCHARGE_SUB_FIXED_QUANTITY)
        {
            SUNLOG(eCRITICAL_LOG, "ItemCode|%u|Invalid Pet SubType(%u)", 
                   item_code, charge_sub_type);
            FASSERT(0);
        }
#endif //_NA_000000_20141127_MODIFY_PET_CHARGE_SUBTYPE
    }

    if (item_info->m_wType == eITEMTYPE_RIDER || item_info->m_wType == eITEMTYPE_RIDER_PARTS)
    {
        if (charge_sub_type != eCHARGE_SUB_RIDER_WITH_PART &&
           charge_sub_type != eCHARGE_SUB_RIDER_WITH_PART_NOTIMER)
        {
            SUNLOG(eCRITICAL_LOG, "ItemCode|%u|Invalid ChargeSubType(%u)",
                   item_code, charge_sub_type);
            FASSERT(0); //시간 설정 문제로 크리티컬이다. 반드시 해결하고 넘어가야 한다.
        }
    }
    if (item_info->m_bPossessionItem != false)
    {
        if (item_info->m_byMaterialType == 0)
        {
            SUNLOG(eCRITICAL_LOG, "ItemCode|%u|Invalid MaterialType(%u) \n", 
                   item_code, item_info->m_byMaterialType);
        }

        if (item_info->m_byPopup == 0)
        {
            SUNLOG(eCRITICAL_LOG, "ItemCode|%u|Invalid Popup(%u) \n", 
                   item_code, item_info->m_byPopup);
        }
    }

    if (SCItemSlot::CanTradeSell(item_info, eITEM_TRADESELL_TRADE) == false &&
        SCItemSlot::CanTradeSell(item_info, eITEM_TRADESELL_DROP) == true)
    {
        // 거래가능 && 드랍가능 => OK
        // 거래불가 && 드랍불가 => OK
        // 거래가능 && 드랍불가 => OK(거래가능한 아이템을 바닥에 떨어뜨리는 경우를 방지하기 위해 설정할 수 있음)
        // 거래불가 && 드랍가능 => Not OK
        SUNLOG(eCRITICAL_LOG, "ItemCode|%u|Invalid TradeSellType(%u) \n", 
            item_code, item_info->trade_sell_type);
    }
#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
    if (item_info->m_wType == eITEMTYPE_PERK_CASH_ITEM)
    {   // f100915.3L, add a rule about a cash perk regist item.
        const bool is_desirable_charge_sub_type = \
            item_info->m_ChargeSubType == eCHARGE_SUB_FIXED_AMOUNT_EX;
        assert(is_desirable_charge_sub_type);
        if (item_info->m_byDupNum != 1 || 
            (item_info->IsMaterialWaste() == false))
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|ItemCode=%d|Msg=violate a perk item rule, forced changes|")
                   _T("|DupNum=1<-(%d), MaterialType=2<-(%d)|ChargeSubType=2<-(%d)|"),
                   item_code, item_info->m_byDupNum,
                   item_info->m_byMaterialType, item_info->m_ChargeSubType);
            //
            item_info->m_ChargeSubType = eCHARGE_SUB_FIXED_AMOUNT_EX;
            item_info->m_byDupNum = 1;
            item_info->m_byMaterialType = eITEM_MATERIAL_TYPE_CAN_USE;
        }
    };
#endif
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    // 쟈드 아이템을 검사한다.
    if (item_info->m_wType == eITEMTYPE_SOCKET_ZARD)
    {
        if (item_info->zard_grade >= kZardGradeMax)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|쟈드 등급의 범위가 옳바르지 않아서 0으로 초기화 합니다.|ItemCode = %u, 등급 = %d|"), 
                   __FUNCTION__, 
                   item_info->m_Code, 
                   item_info->zard_grade);
            item_info->zard_grade = 0;
        }
        if (item_info->zard_item_option_kind >= _countof(g_ITEM_OPTION))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|쟈드 아이템 옵션의 범위가 옳바르지 않아서 0으로 초기화 합니다.|ItemCode = %u, 옵션 = %d|"), 
                   __FUNCTION__, 
                   item_info->m_Code, 
                   item_info->zard_item_option_kind);
            item_info->zard_item_option_kind = 0;
        }
    }
    // 쟈드 합성 첨가제를 검사한다.
    if (item_info->m_wType == eITEMTYPE_SOCKET_COMPOSE_ADDITIVE)
    {
        enum { kMaxZardSuccessPer = 1000 };
        if (item_info->zard_success_per > kMaxZardSuccessPer)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|쟈드 합성 추가 성공률 범위가 옳바르지 않아서 0으로 초기화 합니다.|ItemCode = %u, 추가성공률 = %d|"), 
                   __FUNCTION__, 
                   item_info->m_Code, 
                   item_info->zard_success_per);
            item_info->zard_success_per = 0;
        }
        if (item_info->m_byDupNum > 1 && (item_info->IsMaterialWaste() == false))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|쟈드 합성 첨가제가 겹치기가 가능한데 소모성이 아니라서 소모성으로 초기화 합니다.|ItemCode = %u, 겹침개수 = %u|"), 
                   __FUNCTION__, 
                   item_info->m_Code, 
                   item_info->m_byDupNum);
            item_info->m_byMaterialType = eITEM_MATERIAL_TYPE_CAN_USE;
        }
    }
    // 소켓 개수 재설정 아이템을 검사한다.
    if (item_info->m_wType == eITEMTYPE_STAT_CHANGE)
    {
        if (item_info->m_ExchangedItem < 0 || item_info->m_ExchangedItem > SOCKET_MAX)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|소켓 개수 설정이 옳바르지 않아서 0으로 초기화 합니다.|ItemCode = %u, 소켓개수 = %d|"), 
                   __FUNCTION__, 
                   item_info->m_Code, 
                   item_info->m_ExchangedItem);
            item_info->m_ExchangedItem = 0;
        }
        enum { kMaxSocketRatioType = 255 };
        const WORD fixed_ratio_type = item_info->m_wSubType;
        if (fixed_ratio_type < 0 || fixed_ratio_type >= kMaxSocketRatioType)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|소켓 생성 확률 종류가 올바르지 않아서 0으로 초기화 합니다.|ItemCode = %u, 확률 종류 = %u|"), 
                   __FUNCTION__, 
                   item_info->m_Code, 
                   item_info->m_wSubType);
            item_info->m_wSubType = 0;
        }
        if (item_info->m_byDupNum > 1 && (item_info->IsMaterialWaste() == false))
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|소켓 개수 재설정 아이템이 겹치기가 가능한데 소모성이 아니라서 소모성으로 초기화 합니다.|ItemCode = %u, 겹침개수 = %u|"), 
                   __FUNCTION__, 
                   item_info->m_Code, 
                   item_info->m_byDupNum);
            item_info->m_byMaterialType = eITEM_MATERIAL_TYPE_CAN_USE;
        }
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM

//#ifdef _NA_004035_20120227_COSTUME_ITEM
//    // 코스튬 아이템 속성이 소모성일 경우..
//    if (item_info->m_wType == eITEMTYPE_COSTUME)
//    {
//        if (item_info->IsMaterialWaste())
//        {
//            SUNLOG(eCRITICAL_LOG, 
//                _T("|%s|%d|코스튬 아이템 설정이 소모성이라서 삭제 불가능하도록 초기화 합니다.|ItemCode = %u, MaterialType = %u|"), 
//                __FUNCTION__, 
//                __LINE__, 
//                item_info->m_Code, 
//                item_info->m_byMaterialType);
//            //setting
//            item_info->m_byMaterialType = eITEM_MATERIAL_TYPE_CAN_USE_NOT_DELETE;
//        }
//    }
//#endif  // _NA_004035_20120227_COSTUME_ITEM

    // last order.
    FlowControl::FCAssert(\
        nsSlot::ItemRuleInfo::PrefetchRuleInfo(&item_info->item_rule_info, item_info));
}

//==================================================================================================
// CHANGES: f110408.3L, added interface to support external control like a event. _SERVER contents
void ItemInfoParser::ChangeToTimeExpirationItem(CODETYPE item_code)
{
    // (CHANGES) (f100505.1L) changes a search algorithm + added const version
    if (item_code == 0 || item_code > USHRT_MAX) {
        assert(!"index out of ranges");
        return;
    }
    --item_code;
    BASE_ITEMINFO* item_info = item_table_.array_[item_code];
    if (item_info == NULL) {
        assert(!"can't find item");
        return;
    };
    item_info->time_expiration_item = true;
}

//==================================================================================================

BOOL
ItemInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_ACCESSORYITEMINFO:
    case SCRIPT_ARMORITEMINFO:
    case SCRIPT_WASTEITEMINFO:
    case SCRIPT_WEAPONITEMINFO:
    case SCRIPT_CHARGEITEMINFO:
        return _Load(reloaded);
    }

    return false;
}

#if defined (_NA_0_20091208_VENDOR_ITEM_SEARCH) || (defined(_DH_GM_ITEM_WINDOW) && !defined(_SERVER))
void ItemInfoParser::PushItemName(TCHAR* stName, DWORD ItemCode, TCHAR* description_string /*=NULL*/)
{
    _ITEM_NAME_INFO temp;
    temp.m_stName = stName;
    temp.m_dwItemCode = ItemCode;
    temp.description_string = description_string;

    item_name_code_table_.push_back(temp);
}

DWORD ItemInfoParser::GetItemNameforCode(char *stName)
{
	ITEM_NAME_INFO::iterator itr;

	for (itr = item_name_code_table_.begin(); itr != item_name_code_table_.end(); ++itr)
	{
		//sInfo = itr;
		//if (itr->m_stName.IndexOf(nStName, 0) != -1)
        if (itr->m_stName.find(stName, 0) != std::string::npos)
		{
			strncpy(stName, itr->m_stName.c_str(), MAX_VENDOR_SEARCH_LENGTH);
			return itr->m_dwItemCode;
		}
	}
	return 0;
}

ITEM_NAME_INFO::iterator ItemInfoParser::GetItemNamesBegin()
{
	return item_name_code_table_.begin();
}

ITEM_NAME_INFO::iterator ItemInfoParser::GetItemNamesEnd()
{
	return item_name_code_table_.end();
}

DWORD ItemInfoParser::GetItemAllCount()
{
    return item_name_code_table_.size();
}
#endif//_NA_0_20091208_VENDOR_ITEM_SEARCH

// CHANGES: f110802.3L, _NA000000_SPEED_HACK_PREVENTION_SUPPORT
bool ItemInfoParser::CheckChecksum(CODETYPE item_code) const
{
    if (item_code == 0 || item_code > USHRT_MAX) {
        return NULL;
    }
    --item_code;
    const BASE_ITEMINFO* item_info = item_table_.array_[item_code];
    if (item_info == NULL) {
        return true;
    };
    ulong checksum = item_table_.client_checksum_array_[item_code];
    ulong runtime_checksum = util::StreamHashKey::GetMemoryHashKey(item_info, sizeof(*item_info));
    if (checksum != runtime_checksum) {
        return false;
    }
    return true;
}