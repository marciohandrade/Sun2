#include "StdAfx.h"
#include ".\setitemoptioninfoparser.h"
#include <ItemOptionHeader.h>
#include "Struct.h"

//==================================================================================================
// @history
//  - ~~~
//  - 09/10/24|waverix|code arrangement, remove warnings

SetItemOptionInfoParser::SetItemOptionInfoParser(void)
{
}

SetItemOptionInfoParser::~SetItemOptionInfoParser(void)
{
}


void
SetItemOptionInfoParser::Release()
{
    SETITEM_OPTION_INFO_MAP::iterator it = m_mapSetItemOptionInfo.begin(),
                                      end = m_mapSetItemOptionInfo.end();
    for( ; it != end; ++it) {
        SETITEM_OPTION_INFO* option_info = it->second;
        delete option_info;
    }
    m_mapSetItemOptionInfo.clear();
}

SETITEM_OPTION_INFO*
SetItemOptionInfoParser::Find(DWORD set_group_code)
{
    SETITEM_OPTION_INFO_MAP::const_iterator it = m_mapSetItemOptionInfo.find(set_group_code);
    if(it != m_mapSetItemOptionInfo.end()) {
        SETITEM_OPTION_INFO* option_info = it->second;
        return option_info;
    }
    return NULL;
}

eSETITEM_OPTION_LEVEL
SetItemOptionInfoParser::GetSetItemOptionLevel(int number_of_same_set_items) const
{
    // (CHANGES) (WAVERIX) change binary-condition to unary-condition
    if (number_of_same_set_items < 0 ||
        number_of_same_set_items > MAX_SETITEM_ACTIVE_EQUIP_OPTION)
    {
    #ifdef _DEBUG
        // (NOTE) 여기서 문제가 발생하면 SetItem관련 로직에 문제가 발생한 사례일 것이다.
        // 반드시 해결 할 것.
        __debugbreak();
    #endif
        number_of_same_set_items = (number_of_same_set_items < 0) ?
            0 : MAX_SETITEM_ACTIVE_EQUIP_OPTION;
    };

    eSETITEM_OPTION_LEVEL level
        = (number_of_same_set_items < 2) ?  SETITEM_OPTION_LEVEL_NONE
        : (number_of_same_set_items < 4) ?  SETITEM_OPTION_LEVEL_FIRST
        : (number_of_same_set_items < 6) ?  SETITEM_OPTION_LEVEL_SECOND
        :                                   SETITEM_OPTION_LEVEL_THIRD;
    //
    return level;
}

// NOTE: f110325.3L, added interface to support fast calculation
const SET_ITEM_OPTION* SetItemOptionInfoParser::GetCurrentSetItemOption(
    const SETITEM_OPTION_INFO* set_item_option_info,
    POSTYPE pos, eSETITEM_OPTION_LEVEL level) const
{
    if (bool out_of_ranges = !(pos < MAX_EQUIPMENT_SLOT_NUM)) {
        return NULL;
    }

    const EQUIP_ACTIVE_OPTION* it = set_item_option_info->m_EquipActiveOption,
        * const end = &set_item_option_info->m_EquipActiveOption[
                _countof(set_item_option_info->m_EquipActiveOption)];
    for ( ; it != end; ++it)
    {
        if (it->m_EqPos != pos) {
            continue;
        };
        return &it->m_ActiveOption[level - 1];
    }

    return NULL;
}

//풀 옵션이 아닌 일반 장착 아이템 옵션을 구한다.
SET_ITEM_OPTION*
SetItemOptionInfoParser::GetSetItemOption(DWORD set_group_code,
                                          POSTYPE pos, eSETITEM_OPTION_LEVEL level)
{
    // (CHANGES) (WAVERIX) move to first condition
    // 장착장 위치 확인
    if(pos >= MAX_EQUIPMENT_SLOT_NUM)
        return NULL;

    SETITEM_OPTION_INFO* option_info = Find(set_group_code);
    if(option_info == NULL)
        return NULL;

    for(int i = 0; i < MAX_SETITEM_ACTIVE_EQUIP_OPTION; ++i)
    {
        EQUIP_ACTIVE_OPTION& active_option = option_info->m_EquipActiveOption[i];
        if(active_option.m_EqPos == pos)
            return &active_option.m_ActiveOption[level - 1];
    }

    return NULL;
}

BYTE
SetItemOptionInfoParser::GetSetItemFullOption(DWORD set_group_code, OUT SET_ITEM_OPTION** ppOption)
{
    SETITEM_OPTION_INFO* option_info = Find(set_group_code);
    if(option_info == 0)
        return 0;

    BYTE count = 0;

    for(int i = 0; i < MAX_SETITEM_FULL_OPTION; i++)
    {
        ppOption[i] = &option_info->m_FullOption[i];
        ++count;
    }

    return count;
}

BOOL
SetItemOptionInfoParser::IsFullOption(DWORD set_group_code, BYTE num_of_set_items)
{
    SETITEM_OPTION_INFO* option_info = Find(set_group_code);
    if(option_info == 0)
        return false;

    //풀 옵션인가?
    if(option_info->m_byActiveFullOptionNum == num_of_set_items) 
        return true;

    return false;
}


BOOL
SetItemOptionInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL bReload)
{
    switch(script_code)
    {
    case SCRIPT_SETITEM_OPTION_INFO:
        return _Load(bReload);
    }

    return false;
}

BOOL
SetItemOptionInfoParser::_Load(BOOL bReload)
{
    int nRowSize = GetRowSize();
    for(int nRow = 0; nRow < nRowSize; ++nRow)
    {
        // (CHANGES) change type to word|exist implicit check in TableData
        WORD dwCode = GetDataWORD("wSetGCode", nRow);
        SETITEM_OPTION_INFO* pInfo = Find(dwCode);
        if(pInfo == NULL)
        {
            pInfo = new SETITEM_OPTION_INFO;
            ZeroMemory(pInfo, sizeof(*pInfo)); // (CHANGES) (WAVERIX) add clear routine
            m_mapSetItemOptionInfo.insert(std::make_pair(dwCode, pInfo));
        }
        else
        {
            if(false == bReload)
            {
                SUNLOG(eCRITICAL_LOG, "SetItemOptionInfoParser Error ID = %d", dwCode);
                FASSERT(!"데이터에 오류가 있습니다.");
            }
        }

        pInfo->m_wSetGroupCode = dwCode;
        pInfo->m_byItemType = GetDataBYTE("bysubType", nRow);
        pInfo->m_byActiveFullOptionNum = GetDataBYTE("bFulloptionNum", nRow);

        //ActiveOption
        // (CHANGES) buffer size...
        char szTemp[MAX_FIELD_NAME_SIZE]; // (REMOVE) = { 0, };

        BOOST_STATIC_ASSERT(_countof(pInfo->m_EquipActiveOption) == MAX_SETITEM_ACTIVE_EQUIP_OPTION);
        for(int i = 0; i < _countof(pInfo->m_EquipActiveOption); ++i)
        {
            EQUIP_ACTIVE_OPTION& active_option = pInfo->m_EquipActiveOption[i];

            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "EqPos%d", i + 1);
            active_option.m_EqPos = GetDataBYTE(szTemp, nRow);

            BOOST_STATIC_ASSERT(_countof(active_option.m_ActiveOption) == MAX_SETITEM_OPTION_LEVEL);
            for(int j = 0; j < _countof(active_option.m_ActiveOption); ++j)
            {
                SET_ITEM_OPTION& set_item_option = active_option.m_ActiveOption[j];

                _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "OptionKind%d_%d", j + 1, i + 1);
                set_item_option.m_byOptionKind = GetDataBYTE(szTemp, nRow);
                _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "useType%d_%d", j + 1, i + 1);
                set_item_option.m_byUseType = GetDataBYTE(szTemp, nRow);
                _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "Option%d_%d", j + 1, i + 1);
                set_item_option.m_nOption = GetDataInt(szTemp, nRow);
            }
        }

        BOOST_STATIC_ASSERT(_countof(pInfo->m_FullOption) == MAX_SETITEM_FULL_OPTION);
        for(int i = 0; i < _countof(pInfo->m_FullOption); i++)
        {
            SET_ITEM_OPTION& full_option = pInfo->m_FullOption[i];
            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "FullOptionKind%d", i+1);
            full_option.m_byOptionKind = GetDataBYTE(szTemp, nRow);
            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "useType%d", i+1);
            full_option.m_byUseType = GetDataBYTE(szTemp, nRow);
            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "FullOption%d", i+1);
            full_option.m_nOption = GetDataInt(szTemp, nRow);
        }

#ifdef _YMS_FULLSETITEM_RESOURCE_DIVISION_
    #ifdef _YMS_GENDER_FULLSETITEM
        pInfo->change_armor_resource[GenderType::kMale] = GetDataDWORD("FullsetChangeArmorResource", nRow);
        pInfo->change_armor_resource[GenderType::kFemale] = GetDataDWORD("FullsetChangeArmorResource2", nRow);
    #else
        pInfo->change_armor_resource = GetDataDWORD("FullsetChangeArmorResource", nRow);
    #endif //_YMS_GENDER_FULLSETITEM
#else
        pInfo->m_FullSetChangeItem = GetDataWORD("FullSetChangeItemCode", nRow);
        pInfo->change_armor_resource = 0;
#endif //_YMS_FULLSETITEM_RESOURCE_DIVISION_

    
        pInfo->m_SpecialEquipOptionItem.m_wItemCode = GetDataWORD("SpecialItemCode", nRow);
        pInfo->m_SpecialEquipOptionItem.m_EqPos = GetDataBYTE("SpecialPos", nRow);

        BOOST_STATIC_ASSERT(_countof(pInfo->m_SpecialEquipOptionItem.m_ActiveOption) ==
                            MAX_SETITEM_SPECIAL_OPTION);

        for(int i = 0; i < _countof(pInfo->m_SpecialEquipOptionItem.m_ActiveOption); ++i)
        {
            SET_ITEM_OPTION& special_option = pInfo->m_SpecialEquipOptionItem.m_ActiveOption[i];

            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "SpecialOptionKind%d", i+1);
            special_option.m_byOptionKind = GetDataBYTE(szTemp, nRow);
            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "SpecialuseType%d", i+1);
            special_option.m_byUseType = GetDataBYTE(szTemp, nRow);
            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "SpecialOption%d", i+1);
            special_option.m_nOption = GetDataInt(szTemp, nRow);
        }
    
#ifdef _YMS_GENDER_FULLSETITEM
        WzID null_wzid = StrToWzID("Null");
        WzID effect_wzid =0;

        //!< 남
        effect_wzid = StrToWzID(GetDataString("sEffect1", nRow));
        pInfo->m_dwEffectCode[0][1] = (effect_wzid == null_wzid) ? 0 : effect_wzid;
        effect_wzid = StrToWzID(GetDataString("sEffect2", nRow));
        pInfo->m_dwEffectCode[1][1] = (effect_wzid == null_wzid) ? 0 : effect_wzid;
        effect_wzid = StrToWzID(GetDataString("sEffect3", nRow)); 
        pInfo->m_dwEffectCode[2][1] = (effect_wzid == null_wzid) ? 0 : effect_wzid;
        effect_wzid = StrToWzID(GetDataString("sEffect4", nRow));
        pInfo->m_dwEffectCode[3][1] = (effect_wzid == null_wzid) ? 0 : effect_wzid;

        //!< 여
        effect_wzid = StrToWzID(GetDataString("sEffect5", nRow));
        pInfo->m_dwEffectCode[0][2] = (effect_wzid == null_wzid) ? 0 : effect_wzid;
        effect_wzid = StrToWzID(GetDataString("sEffect6", nRow));
        pInfo->m_dwEffectCode[1][2] = (effect_wzid == null_wzid) ? 0 : effect_wzid;
        effect_wzid = StrToWzID(GetDataString("sEffect7", nRow));
        pInfo->m_dwEffectCode[2][2] = (effect_wzid == null_wzid) ? 0 : effect_wzid;
        effect_wzid = StrToWzID(GetDataString("sEffect8", nRow));
        pInfo->m_dwEffectCode[3][2] = (effect_wzid == null_wzid) ? 0 : effect_wzid;

        //!< 남
        pInfo->m_btBoneIdx[0][1] = GetDataBYTE("btBoneIdx1", nRow); 
        pInfo->m_btBoneIdx[1][1] = GetDataBYTE("btBoneIdx2", nRow);
        pInfo->m_btBoneIdx[2][1] = GetDataBYTE("btBoneIdx3", nRow);
        pInfo->m_btBoneIdx[3][1] = GetDataBYTE("btBoneIdx4", nRow);
         //!< 여
        pInfo->m_btBoneIdx[0][2] = GetDataBYTE("btBoneIdx5", nRow);
        pInfo->m_btBoneIdx[1][2] = GetDataBYTE("btBoneIdx6", nRow);
        pInfo->m_btBoneIdx[2][2] = GetDataBYTE("btBoneIdx7", nRow);
        pInfo->m_btBoneIdx[3][2] = GetDataBYTE("btBoneIdx8", nRow);
#else
        for(int i = 0; i < MAX_SETITEM_EFFECT_NUM; ++i)
        {
            // (CHANGES) remove buffer clear|sizeof -> _countof
            char szTemp2[10]; // (REMOVE) = { 0, };

            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "sEffect%d", i+1);
            strncpy(szTemp2, GetDataString(szTemp, nRow), _countof(szTemp2));
            szTemp2[4] = '\0';
            pInfo->m_dwEffectCode[i] = (strcmp(szTemp2, "Null") == 0) ?  0 : StrToWzID(szTemp2);

            _snprintf(szTemp, MAX_FIELD_NAME_SIZE, "btBoneIdx%d", i+1);
            pInfo->m_btBoneIdx[i] = GetDataBYTE(szTemp, nRow);
        }
#endif //_YMS_GENDER_FULLSETITEM

        ValidCheck(pInfo);

        // (NOTE) delete reserved section
    #ifdef SHAKETAKE_DATA_CHECK
        char pszAreaID[128] = {0,}; WzIDToString(pszAreaID, pInfo->m_dwEffectCode[0]);pszAreaID[4] = '\0'; 
        char pszAreaID2[128]= {0,}; WzIDToString(pszAreaID2, pInfo->m_dwEffectCode[1]);pszAreaID2[4] = '\0'; 
        char pszAreaID3[128]= {0,}; WzIDToString(pszAreaID3, pInfo->m_dwEffectCode[2]);pszAreaID3[4] = '\0'; 


        SUNLOG(eCRITICAL_LOG,
               "%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"
               "%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"
               "%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"
               "%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"
               "%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"
               "%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"
               "%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t"
               "%u\t%u\t%u\t%u\t%u\t%u\t%u\t%s\t%u\t%s\t%u\t%s\t%u\t",
               pInfo->m_wSetGroupCode,
               pInfo->m_byItemType,
               pInfo->m_byActiveFullOptionNum, 
               pInfo->m_EquipActiveOption[0].m_EqPos,
               pInfo->m_EquipActiveOption[0].m_ActiveOption[0].m_byOptionKind,
               pInfo->m_EquipActiveOption[0].m_ActiveOption[0].m_byUseType,
               pInfo->m_EquipActiveOption[0].m_ActiveOption[0].m_nOption,
               pInfo->m_EquipActiveOption[0].m_ActiveOption[1].m_byOptionKind,
               pInfo->m_EquipActiveOption[0].m_ActiveOption[1].m_byUseType,
               pInfo->m_EquipActiveOption[0].m_ActiveOption[1].m_nOption,
               pInfo->m_EquipActiveOption[0].m_ActiveOption[2].m_byOptionKind,
               pInfo->m_EquipActiveOption[0].m_ActiveOption[2].m_byUseType,
               pInfo->m_EquipActiveOption[0].m_ActiveOption[2].m_nOption,

               pInfo->m_EquipActiveOption[1].m_EqPos,
               pInfo->m_EquipActiveOption[1].m_ActiveOption[0].m_byOptionKind,
               pInfo->m_EquipActiveOption[1].m_ActiveOption[0].m_byUseType,
               pInfo->m_EquipActiveOption[1].m_ActiveOption[0].m_nOption,
               pInfo->m_EquipActiveOption[1].m_ActiveOption[1].m_byOptionKind,
               pInfo->m_EquipActiveOption[1].m_ActiveOption[1].m_byUseType,
               pInfo->m_EquipActiveOption[1].m_ActiveOption[1].m_nOption,
               pInfo->m_EquipActiveOption[1].m_ActiveOption[2].m_byOptionKind,
               pInfo->m_EquipActiveOption[1].m_ActiveOption[2].m_byUseType,
               pInfo->m_EquipActiveOption[1].m_ActiveOption[2].m_nOption,

               pInfo->m_EquipActiveOption[2].m_EqPos,
               pInfo->m_EquipActiveOption[2].m_ActiveOption[0].m_byOptionKind,
               pInfo->m_EquipActiveOption[2].m_ActiveOption[0].m_byUseType,
               pInfo->m_EquipActiveOption[2].m_ActiveOption[0].m_nOption,
               pInfo->m_EquipActiveOption[2].m_ActiveOption[1].m_byOptionKind,
               pInfo->m_EquipActiveOption[2].m_ActiveOption[1].m_byUseType,
               pInfo->m_EquipActiveOption[2].m_ActiveOption[1].m_nOption,
               pInfo->m_EquipActiveOption[2].m_ActiveOption[2].m_byOptionKind,
               pInfo->m_EquipActiveOption[2].m_ActiveOption[2].m_byUseType,
               pInfo->m_EquipActiveOption[2].m_ActiveOption[2].m_nOption,

               pInfo->m_EquipActiveOption[3].m_EqPos,
               pInfo->m_EquipActiveOption[3].m_ActiveOption[0].m_byOptionKind,
               pInfo->m_EquipActiveOption[3].m_ActiveOption[0].m_byUseType,
               pInfo->m_EquipActiveOption[3].m_ActiveOption[0].m_nOption,
               pInfo->m_EquipActiveOption[3].m_ActiveOption[1].m_byOptionKind,
               pInfo->m_EquipActiveOption[3].m_ActiveOption[1].m_byUseType,
               pInfo->m_EquipActiveOption[3].m_ActiveOption[1].m_nOption,
               pInfo->m_EquipActiveOption[3].m_ActiveOption[2].m_byOptionKind,
               pInfo->m_EquipActiveOption[3].m_ActiveOption[2].m_byUseType,
               pInfo->m_EquipActiveOption[3].m_ActiveOption[2].m_nOption,

               pInfo->m_EquipActiveOption[4].m_EqPos,
               pInfo->m_EquipActiveOption[4].m_ActiveOption[0].m_byOptionKind,
               pInfo->m_EquipActiveOption[4].m_ActiveOption[0].m_byUseType,
               pInfo->m_EquipActiveOption[4].m_ActiveOption[0].m_nOption,
               pInfo->m_EquipActiveOption[4].m_ActiveOption[1].m_byOptionKind,
               pInfo->m_EquipActiveOption[4].m_ActiveOption[1].m_byUseType,
               pInfo->m_EquipActiveOption[4].m_ActiveOption[1].m_nOption,
               pInfo->m_EquipActiveOption[4].m_ActiveOption[2].m_byOptionKind,
               pInfo->m_EquipActiveOption[4].m_ActiveOption[2].m_byUseType,
               pInfo->m_EquipActiveOption[4].m_ActiveOption[2].m_nOption,

               pInfo->m_EquipActiveOption[5].m_EqPos,
               pInfo->m_EquipActiveOption[5].m_ActiveOption[0].m_byOptionKind,
               pInfo->m_EquipActiveOption[5].m_ActiveOption[0].m_byUseType,
               pInfo->m_EquipActiveOption[5].m_ActiveOption[0].m_nOption,
               pInfo->m_EquipActiveOption[5].m_ActiveOption[1].m_byOptionKind,
               pInfo->m_EquipActiveOption[5].m_ActiveOption[1].m_byUseType,
               pInfo->m_EquipActiveOption[5].m_ActiveOption[1].m_nOption,
               pInfo->m_EquipActiveOption[5].m_ActiveOption[2].m_byOptionKind,
               pInfo->m_EquipActiveOption[5].m_ActiveOption[2].m_byUseType,
               pInfo->m_EquipActiveOption[5].m_ActiveOption[2].m_nOption,

               pInfo->m_EquipActiveOption[6].m_EqPos,
               pInfo->m_EquipActiveOption[6].m_ActiveOption[0].m_byOptionKind,
               pInfo->m_EquipActiveOption[6].m_ActiveOption[0].m_byUseType,
               pInfo->m_EquipActiveOption[6].m_ActiveOption[0].m_nOption,
               pInfo->m_EquipActiveOption[6].m_ActiveOption[1].m_byOptionKind,
               pInfo->m_EquipActiveOption[6].m_ActiveOption[1].m_byUseType,
               pInfo->m_EquipActiveOption[6].m_ActiveOption[1].m_nOption,
               pInfo->m_EquipActiveOption[6].m_ActiveOption[2].m_byOptionKind,
               pInfo->m_EquipActiveOption[6].m_ActiveOption[2].m_byUseType,
               pInfo->m_EquipActiveOption[6].m_ActiveOption[2].m_nOption,

               pInfo->m_EquipActiveOption[7].m_EqPos,
               pInfo->m_EquipActiveOption[7].m_ActiveOption[0].m_byOptionKind,
               pInfo->m_EquipActiveOption[7].m_ActiveOption[0].m_byUseType,
               pInfo->m_EquipActiveOption[7].m_ActiveOption[0].m_nOption,
               pInfo->m_EquipActiveOption[7].m_ActiveOption[1].m_byOptionKind,
               pInfo->m_EquipActiveOption[7].m_ActiveOption[1].m_byUseType,
               pInfo->m_EquipActiveOption[7].m_ActiveOption[1].m_nOption,
               pInfo->m_EquipActiveOption[7].m_ActiveOption[2].m_byOptionKind,
               pInfo->m_EquipActiveOption[7].m_ActiveOption[2].m_byUseType,
               pInfo->m_EquipActiveOption[7].m_ActiveOption[2].m_nOption,

               pInfo->m_FullOption[0].m_byOptionKind,
               pInfo->m_FullOption[0].m_byUseType,
               pInfo->m_FullOption[0].m_nOption,
               pInfo->m_FullOption[1].m_byOptionKind,
               pInfo->m_FullOption[1].m_byUseType,
               pInfo->m_FullOption[1].m_nOption,
               pInfo->m_FullOption[2].m_byOptionKind,
               pInfo->m_FullOption[2].m_byUseType,
               pInfo->m_FullOption[2].m_nOption,
               pInfo->m_FullOption[3].m_byOptionKind,
               pInfo->m_FullOption[3].m_byUseType,
               pInfo->m_FullOption[3].m_nOption,
               pInfo->m_FullOption[4].m_byOptionKind,
               pInfo->m_FullOption[4].m_byUseType,
               pInfo->m_FullOption[4].m_nOption,
               pszAreaID,
               pInfo->m_btBoneIdx[0],
               pszAreaID2,
               pInfo->m_btBoneIdx[1],
               pszAreaID3,
               pInfo->m_btBoneIdx[2]);
    #endif //SHAKETAKE_DATA_CHECK
    }

    return true;
}


void SetItemOptionInfoParser::ValidCheck(SETITEM_OPTION_INFO* pInfo)
{
    for(int i = 0; i < MAX_SETITEM_ACTIVE_EQUIP_OPTION; ++i)
    {
        EQUIP_ACTIVE_OPTION& option = pInfo->m_EquipActiveOption[i];
        for(int j = 0; j < MAX_SETITEM_OPTION_LEVEL; ++j)
        {
            SET_ITEM_OPTION& setOption = option.m_ActiveOption[j];
            const BYTE byOptionKind = setOption.m_byOptionKind; 
            const BYTE byUseType = setOption.m_byUseType;    
            const int  nOption = setOption.m_nOption;

            if(!CheckOptionValidity(byOptionKind, byUseType, nOption))
            {
                SUNLOG(eCRITICAL_LOG,
                       "[SetItemOption]|Invalid Equip Option|"
                       "SetGroupCode=%d|OptionIndex=%d|OptionType=%d|OptionValue=%d|",
                       pInfo->m_wSetGroupCode, byOptionKind, byUseType, nOption);
            }
       }
    }

    for(int k = 0; k < MAX_SETITEM_FULL_OPTION; ++k)
    {
        SET_ITEM_OPTION& setOption = pInfo->m_FullOption[k];
        const BYTE byOptionKind = setOption.m_byOptionKind; 
        const BYTE byUseType = setOption.m_byUseType;    
        const int  nOption = setOption.m_nOption;

        if(!CheckOptionValidity(byOptionKind, byUseType, nOption))
        {
            SUNLOG(eCRITICAL_LOG,
                   "[SetItemOption]|Invalid Full Option|"
                   "SetGroupCode=%d|OptionIndex=%d|OptionType=%d|OptionValue=%d|",
                   pInfo->m_wSetGroupCode, byOptionKind, byUseType, nOption);
        }
    }

    SPECIAL_EQUIP_ACTIVE_OPTION& specialOption = pInfo->m_SpecialEquipOptionItem;
    for(int l = 0; l < MAX_SETITEM_SPECIAL_OPTION; ++l)
    {
        SET_ITEM_OPTION& setOption = specialOption.m_ActiveOption[l];
        const BYTE byOptionKind = setOption.m_byOptionKind; 
        const BYTE byUseType = setOption.m_byUseType;    
        const int  nOption = setOption.m_nOption;

        if(!CheckOptionValidity(byOptionKind, byUseType, nOption))
        {
            SUNLOG(eCRITICAL_LOG,
                   "[SetItemOption]|Invalid Special Option|"
                   "SetGroupCode=%d|OptionIndex=%d|OptionType=%d|OptionValue=%d|",
                   pInfo->m_wSetGroupCode, byOptionKind, byUseType, nOption);
        }
    }
}
