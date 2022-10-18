#include "StdAfx.h"
#include ".\dropcore.h"
#include <SolarFileReader.h>
#include <SCItemSlot.h>
#include <ItemTypeList.h>
#include "DropRatioInfoList.h"
#include "FormulaRatioParser.h"

DropCore::DropCore(void)
    : selected_battlezone_(false)
{
    m_pItemTypeList = ItemTypeList::Instance();
    m_pDropRatioInfoList = DropRatioInfoList::Instance();
}

DropCore::~DropCore(void)
{
    // 반드시 Unload()를 호출해줘라!
    m_pDropRatioInfoList = NULL;
    m_pItemTypeList = NULL;
}

void
DropCore::Unload()
{
    m_pDropRatioInfoList->Unload();
    m_ItemTypeDropRatioForMonster.Unload();
    m_pItemTypeList->Unload();
}

void
DropCore::Reload()
{
    m_pDropRatioInfoList->Reload();
    m_ItemTypeDropRatioForMonster.Reload();
    m_pItemTypeList->Reload();
}

void
DropCore::Release()
{
    // nothing to do
}

// NOTE: f110401.1L, added by _NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION
void DropCore::SetupDropRatioTable(bool selected_battlezone)
{
    selected_battlezone_ = selected_battlezone;
    m_ItemTypeDropRatioForMonster.SetupDropRatioTable(selected_battlezone_);
}

//--------------------------------------------------------------------------------------------------
// 돈 드랍
//--------------------------------------------------------------------------------------------------
BOOL DropCore::DropMoneyFunc(DropMoneyObj* drop_money_ptr, eZONETYPE zone_type, float bonus_percent, 
    eNPC_GRADE npc_grade, OUT MONEY& dropped_money_out, OUT DWORD& decided_seed_out)
{
    if (drop_money_ptr->GetMaxMoneySeed() == 0)
    {
        return false;
    }
    
    // 추가 배율
    RATIO_TYPE ratio_type = RATIO_TYPE_DROP_FIELD;
    switch (zone_type)
    {
    case eZONETYPE_FIELD:
    case eZONETYPE_VILLAGE:
        ratio_type = RATIO_TYPE_DROP_FIELD;
        break;
    case eZONETYPE_CHUNTING:
    case eZONETYPE_HUNTING:
        ratio_type = RATIO_TYPE_DROP_HUNTING;
        break;
    case eZONETYPE_MISSION:
        ratio_type = RATIO_TYPE_DROP_MISSION;
        break;
    }

    assert(SAFE_NUMERIC_TYPECAST(RATIO_TYPE, ratio_type, WORD));
    const WORD drop_ratio_type = static_cast<WORD>(ratio_type);
    
    FormulaRatioParser* const formula_ratio_parser_ptr = FormulaRatioParser::Instance();
    BASE_FORMULA_RATIOINFO* formula_ratio_info_ptr = formula_ratio_parser_ptr->GetFormulaRatio(
        drop_ratio_type);

    // Percentage calculation
    const float weight_by_script = (formula_ratio_info_ptr != NULL) ? 
                                    formula_ratio_info_ptr->m_fRatio : 1.0f;
    const float weight_by_bonus = bonus_percent;

    // 등급에 따른 가중치
    float weight_by_grade = 1.0f;
    switch (npc_grade)
    {
    case eNPC_GENERAL:       
        weight_by_grade = 1.0f; 
        break;
    case eNPC_LEADER:        
        weight_by_grade = 1.1f; 
        break;
    case eNPC_ELITE:         
        weight_by_grade = 1.3f; 
        break;
    case eNPC_MIDDLEBOSS:    
        weight_by_grade = 1.5f; 
        break;
    case eNPC_BOSS:          
        weight_by_grade = 2.0f; 
        break;
    case eNPC_LUCKY_MONSTER: 
        weight_by_grade = 2.0f; 
        break;
    case eNPC_MAPOBJECT_NPC: 
        weight_by_grade = 1.0f; 
        break;
    case eNPC_SUMMON_NPC:    
        weight_by_grade = 1.0f; 
        break;
    default:                
        {
            SUNLOG(eCRITICAL_LOG, TEXT("[DropMoneyFunc] Invalid MonsterGrade[%d]!!"), npc_grade);
        }
        break;
    }

    const DWORD seed_min = drop_money_ptr->GetMinMoneySeed();
    const DWORD seed_max = drop_money_ptr->GetMaxMoneySeed();

    // 돈 액수 계산
    MONEY calced_money = static_cast<MONEY>(dRandom(seed_min, seed_max));
    if (calced_money == 0)
    {
        return false;
    }
    
    // 씨드 배수에 따른 DROP
    const DWORD multi_seed_min = drop_money_ptr->GetMinMultiple();
    const DWORD multi_seed_max = drop_money_ptr->GetMaxMultiple();

    decided_seed_out = static_cast<DWORD>(dRandom(multi_seed_min, multi_seed_max));

    // formula
    // weight = rDecided_seed * weight_by_grade * weight_by_bonus * weight_by_script
    // result_money = calced_money * weight

    double weight = weight_by_script;
    {
        weight *= weight_by_bonus;
        weight *= weight_by_grade;
        weight *= decided_seed_out;
    };

    dropped_money_out = MONEY(double(calced_money) * weight);

    return true;

/*
    FormulaRatioParser* const pFormulaRatioParser = FormulaRatioParser::Instance();
    BASE_FORMULA_RATIOINFO* formula_ratio_info_ptr = NULL;
    switch(zone_type)
    {
    case eZONETYPE_FIELD:
    case eZONETYPE_VILLAGE:
        formula_ratio_info_ptr = pFormulaRatioParser->GetFormulaRatio(RATIO_TYPE_DROP_FIELD);
        break;
    case eZONETYPE_CHUNTING:
    case eZONETYPE_HUNTING:
        formula_ratio_info_ptr = pFormulaRatioParser->GetFormulaRatio(RATIO_TYPE_DROP_HUNTING);
        break;
    case eZONETYPE_MISSION:
        formula_ratio_info_ptr = pFormulaRatioParser->GetFormulaRatio(RATIO_TYPE_DROP_MISSION);
        break;
    default:
        formula_ratio_info_ptr = pFormulaRatioParser->GetFormulaRatio(RATIO_TYPE_DROP_FIELD);
        break;
    }
    float PlusScriptPercent = 1.0f;
    if(formula_ratio_info_ptr) PlusScriptPercent = formula_ratio_info_ptr->m_fRatio;

    float PlusBonusPercent = fBonusPercent;
    float PlusGradePercent = 1.0f;

    // 등급에 따른 가중치
    switch(npc_grade)
    {
    case eNPC_GENERAL:          PlusGradePercent = 1.0f; break;
    case eNPC_LEADER:           PlusGradePercent = 1.1f; break;
    case eNPC_ELITE:            PlusGradePercent = 1.3f; break;
    case eNPC_MIDDLEBOSS:       PlusGradePercent = 1.5f; break;
    case eNPC_BOSS:             PlusGradePercent = 2.0f; break;
    case eNPC_LUCKY_MONSTER:    PlusGradePercent = 2.0f; break;
    case eNPC_MAPOBJECT_NPC:    PlusGradePercent = 1.0f; break;
    case eNPC_SUMMON_NPC:       PlusGradePercent = 1.0f; break;
    default:                
        { 
            SUNLOG(eCRITICAL_LOG, "[DropMoneyFunc] Invalid MonsterGrade[%d]!!", npc_grade);
            PlusGradePercent = 1.0f;
        }
    }

    // 돈 액수 계산
    MONEY RealMoney = (MONEY)dRandom(pDropObj->GetMinMoneySeed(), pDropObj->GetMaxMoneySeed());
    if(RealMoney == 0)  return false;

    // 씨드 배수에 따른 DROP
    decided_seed_out = (DWORD)dRandom(pDropObj->GetMinMultiple(), pDropObj->GetMaxMultiple());

    RealMoney = (MONEY)(RealMoney * (decided_seed_out*PlusGradePercent*PlusBonusPercent*PlusScriptPercent));
    dropped_money_out = RealMoney;

    return true;
*/
} // BOOL DropCore::DropMoneyFunc(...

BOOL
DropCore::Drop(eZONETYPE zone_type, DWORD drop_index, eNPC_GRADE npc_grade, float fBonusPercent,
               eDROP_TYPE_& OUT drop_type_out,
               MONEY& OUT dropped_money_out, SCItemSlot& OUT dropped_item_out,
               DWORD& OUT decided_seed_out,
               int* result_number_of_drop_items)
{
    DropObj* pDropObj = m_pDropRatioInfoList->PopRandomItemTypeIndex(drop_index);
    if(pDropObj == 0)
        return false;

    switch(pDropObj->GetType())
    {
    case DROP_OBJ_ITEM:
        drop_type_out = DROP_TYPE_ITEM;
        return DropItemFunc(static_cast<DropItemObj*>(pDropObj),
                            npc_grade, dropped_item_out, result_number_of_drop_items);
    case DROP_OBJ_MONEY:
        drop_type_out = DROP_TYPE_MONEY;
        *result_number_of_drop_items = 1;
        return DropMoneyFunc(static_cast<DropMoneyObj *>(pDropObj),
                             zone_type, fBonusPercent, npc_grade,
                             dropped_money_out, decided_seed_out);
    }
    assert(0);
    return false;
}

//--------------------------------------------------------------------------------------------------
// 기본 몬스터 아이템 드랍
//--------------------------------------------------------------------------------------------------
BOOL DropCore::DropItemFunc(DropItemObj* drop_item_ptr, eNPC_GRADE npc_grade, 
                            OUT SCItemSlot& dropped_item_out, int* result_number_of_drop_items)
{
    assert(drop_item_ptr != NULL && result_number_of_drop_items != NULL);
    // NOTE: f110401.3L, set the default value (1) of the number of separately paid items
    *result_number_of_drop_items = 1;
    dropped_item_out.Clear();
    
    // dword, drop_index일 수도 있으므로...
    const CODETYPE drop_item_code = drop_item_ptr->GetItemCode();
    if (SAFE_NUMERIC_TYPECAST(CODETYPE, drop_item_code, SLOTCODE) == false) 
    {
        SUNLOG(eCRITICAL_LOG, TEXT("ItemCode|%u|drop info corrupt: invalid item code"), drop_item_code);
        return false;
    }
    dropped_item_out.SetCode(static_cast<SLOTCODE>(drop_item_code));

    // 유효한 아이템인지 여기서 먼저 검사한다.
    const nsSlot::ItemRuleInfo& drop_item_rule_info = dropped_item_out.GetItemRuleInfo();
    if (drop_item_rule_info.is_valid_item == 0) 
    {
        SUNLOG(eCRITICAL_LOG, TEXT("ItemCode|%u|Try to invalid item drop"), drop_item_code);
        return false;
    }
    
    // (DESC) drop_item_ptr->GetReserved()
    // - drop_item_ptr->GetGroupCode() == 0 이고 ItemCode가 스택아이템이면 개수
    // - drop_item_ptr->GetGroupCode() == 0 이고 ItemCode가 장비아이템이면 ItemTypeIndex

    DWORD drop_item_reserved = drop_item_ptr->GetReserved();
    const BYTE group_code = drop_item_ptr->GetGroupCode();
    //----------------------------------------------------------------------------------------------
    if (dropped_item_out.IsOverlap())
    {
        const DURATYPE drop_item_max_num = dropped_item_out.GetMaxNum();
        //default : GetNum == 1
        // added rule to support _NA002479_20110401_OVERLAPPING_ITEM_DISTRIBUTION_CHANGES
        if (group_code & DROP_GROUP::LIMIT_DISTRIBUTE)
        {
            // NOTE: f110401.3L, the 'drop_item_reserved' is external controlled.
            // TODO: changes generally drop processing.
            dropped_item_out.SetNum(1);
            *result_number_of_drop_items = drop_item_reserved;
        }
        else
        {
            if (drop_item_max_num < drop_item_reserved)
            {
                dropped_item_out.SetNum(drop_item_max_num);
            }
            else if (drop_item_reserved != 0)
            {
                assert(SAFE_NUMERIC_TYPECAST(DWORD, drop_item_reserved, DURATYPE));
                dropped_item_out.SetNum(static_cast<DURATYPE>(drop_item_reserved));
            }
        };
        return true;
    };
    //----------------------------------------------------------------------------------------------
    // changed by _NA_0_20110318_ITEM_EXPIRATION_PERIOD
    const bool apply_time = ((group_code & (DROP_GROUP::LIMIT_DATE|DROP_GROUP::LIMIT_TIME)) != 0);
    if (apply_time)
    {
        if (dropped_item_out.ChangeItemToUsingTimer() == false)
        {
            SUNLOG(eCRITICAL_LOG, _T("ItemCode|%u|failed apply timer to drop item"),
                   drop_item_code);
            return false;
        };

        switch (drop_item_rule_info.charge_sub_type)
        {
        case eCHARGE_SUB_FIXED_AMOUNT:
        case eCHARGE_SUB_FIXED_AMOUNT_EX:
        case eCHARGE_SUB_FIXED_QUANTITY:
        case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
        case eCHARGE_SUB_FATE:
        case eCHARGE_SUB_COORDINATE_EX:
        case eCHARGE_SUB_RIDER_WITH_PART:
        case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
        case eCHARGE_SUB_FIXED_AMOUNT_EX2:
            if (group_code & DROP_GROUP::LIMIT_TIME)
            {
                // 분단위 적용
                dropped_item_out.SetDateTime(drop_item_reserved * 60);
            }
            else if (group_code & DROP_GROUP::LIMIT_DATE)
            {
                //날짜 YYYYMMDDHHMMSS 값을 맞추기 위해 1000000을 곱한다.
                DWORD64 d = (DWORD64)drop_item_reserved*1000000;
                dropped_item_out.SetFixedDateTime(d);
            };
            // CHANGES: f110603.4L, set default value about drop_item_reserved
            drop_item_reserved = 0;
            break;
        }
    }; // apply time
    //----------------------------------------------------------------------------------------------
    // if (dropped_item_out.IsOverlap() == false)
    if (drop_item_rule_info.is_weapon || drop_item_rule_info.is_armor) 
    {
        if (drop_item_reserved == 0)
        {
            drop_item_reserved = m_ItemTypeDropRatioForMonster.PopRandomItemTypeIndex(npc_grade);
        }
    }
    else
    {
        // (NOTE) 이후 다른 룰이 추가되었을 지도 모른다. 걸리면 확인 후 변경/제거하기로 한다.
        // (ADD by hukim) 위에서 이미 아이템 유효성을 먼저 검사하므로 이곳은 일부 퀘스트 아이템인 
        // 경우에 적중할 것이다.
    }
    //----------------------------------------------------------------------------------------------
    const ItemType* drop_item_type_ptr = (drop_item_reserved != 0) ?
        m_pItemTypeList->FindItemType(drop_item_reserved) : NULL;
    if (drop_item_type_ptr != NULL)
    {
        if (drop_item_rule_info.IsEnableEnchant())
        {
            const BYTE enchant = drop_item_type_ptr->GetEnchant();
            dropped_item_out.SetEnchant(enchant);
        }            
    #ifdef _NA_000251_20100727_SOCKET_SYSTEM
        // EP2에서는 아이템 타입 정보의 랭크 항목을 소켓 개수로 사용한다.
        const BYTE number_of_socket = drop_item_type_ptr->GetRank();
        if (number_of_socket != 0 && drop_item_rule_info.IsEnableSocket())
        {
            if (dropped_item_out.CreateSocketItem(number_of_socket, false) == false)
            {
                SUNLOG(eCRITICAL_LOG, 
                        _T("|%s|소켓 개수 설정에 실패했습니다.|아이템코드 = %u, 소켓개수 = &u"), 
                        __FUNCTION__, 
                        drop_item_code, 
                        number_of_socket);
            }
        }
    #else
        // (CHANGES) (WAVERIX) itemslot에서 엘리트 룰이 제거되었다.
        const BASE_ITEMINFO* drop_item_info_ptr = dropped_item_out.GetItemInfo();
        assert(drop_item_info_ptr != NULL);
        if (drop_item_info_ptr->IsElite() == 0 && drop_item_rule_info.IsEnableRank())
        {
            const eRANK item_rank = static_cast<eRANK>(drop_item_type_ptr->GetRank());
            dropped_item_out.SetRank(item_rank);
        }
    #endif // _NA_000251_20100727_SOCKET_SYSTEM
            
        nsSlot::ItemTypeChanger::ChangeItemByType(&dropped_item_out,
                                                  drop_item_type_ptr->GetItemType());
    }

    return true;
    //----------------------------------------------------------------------------------------------

} // BOOL DropCore::DropItemFunc(DropItemObj* drop_item_ptr, eNPC_GRADE npc_grade, OUT SCItemSlot& dropped_item_out)

BOOL
DropCore::Load(eSCRIPT_CODE script_code, const TCHAR* pszFileName, BOOL bBigData,
               eSEPERATOR_MODE mode, const TCHAR* pszPackFileName, BOOL bReload)
{
    switch(script_code)
    {
    case SCRIPT_ITEMTYPELIST:
        return m_pItemTypeList->Load(script_code, pszFileName, bBigData,
                                     mode, pszPackFileName, bReload);
    case SCRIPT_ITEMTYPEDROPRATIO:
        return m_ItemTypeDropRatioForMonster.Load(script_code, pszFileName, bBigData,
                                                  mode, pszPackFileName, bReload);
    case SCRIPT_DROPRATIOINFO:
        return m_pDropRatioInfoList->Load(script_code, pszFileName, bBigData, mode,
                                          pszPackFileName, bReload);
    }

    return false;
}

BOOL
DropCore::SolarLoad(eSCRIPT_CODE script_code, BYTE* stream, DWORD dwSize, BOOL bBigData)
{
    const char* pszFileName = FindFileName(script_code);

    switch(script_code)
    {
    case SCRIPT_ITEMTYPELIST:
        m_pItemTypeList->InsertFileName(script_code,  pszFileName);
        return m_pItemTypeList->SolarLoad(script_code, stream, dwSize, bBigData);
    case SCRIPT_ITEMTYPEDROPRATIO:
        m_ItemTypeDropRatioForMonster.InsertFileName(script_code,  pszFileName);
        return m_ItemTypeDropRatioForMonster.SolarLoad(script_code, stream, dwSize, bBigData);
    case SCRIPT_DROPRATIOINFO:
        m_pDropRatioInfoList->InsertFileName(script_code,  pszFileName);
        return m_pDropRatioInfoList->SolarLoad(script_code, stream, dwSize, bBigData);
    }

    return false;
}





