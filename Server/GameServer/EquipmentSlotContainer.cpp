// EquipmentSlotContainer.cpp: implementation of the CEquipmentSlotContainer class.
//
//==================================================================================================
#include "stdafx.h"
#include "EquipmentSlotContainer.h"

//==================================================================================================
#if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
//==================================================================================================
#include "Player.h"
#include "GameField.h"
#include <Protocol_CG.h>
#include <PacketStruct_CG.h>
#include <ItemInfoParser.h>
#include "ItemAttrCalculator.h"
#include "SkillSlotManager.h"
#include <SCItemSlot.h>
#include <ItemFormula.h>
#include "QuickManager.h"
#include "QuickSlotContainer.h"
#include <SCQuickSlot.h>
#include "ItemManager.h"
#include "StatusManager.h"
#include <BitStream.hpp>
#include <float.h>
#include <EnchantParser.h>

//==================================================================================================
// Construction/Destruction

CEquipmentSlotContainer::CEquipmentSlotContainer()
    : m_pPlayer(NULL)
{
    Init(MAX_EQUIPMENT_SLOT_NUM, SI_EQUIPMENT);
    ZeroMemory(m_bRealEquip, sizeof(m_bRealEquip));
}

CEquipmentSlotContainer::~CEquipmentSlotContainer()
{
}

void
CEquipmentSlotContainer::ClearAll()
{
    SCItemSlotContainer::ClearAll();

    m_mapEquipCount.clear();
    m_mapEquipTypeCount.clear();
    m_mapEquipGroupCount.clear();

    ZeroMemory(m_bRealEquip, sizeof(m_bRealEquip));
}


RC::eSLOT_INSERT_RESULT
CEquipmentSlotContainer::InsertSlot(POSTYPE at_pos, SCSlot& IN rSlot)
{
    SASSERT(m_pPlayer != NULL, "[CEquipmentSlotContainer::InsertSlot] m_pPlayer == NULL");

    if(CanEquip(at_pos, rSlot) == 0)
        return RC::RC_INSERT_SLOT_CAN_NOT_EQUIP;

    RC::eSLOT_INSERT_RESULT rt = SCSlotContainer::InsertSlot(at_pos, rSlot);
    if(rt != RC::RC_INSERT_SLOT_SUCCESS)
        return rt;

    SCItemSlot& rCurSlot = static_cast<SCItemSlot&>(GetSlot(at_pos));

    if(rCurSlot.OnEquip())
    {
        GAMELOG->LogItem(ITEM_CHARGE_FIRST_EQUIP, m_pPlayer, &rCurSlot);
    }

    SCQuickSlotContainer* quick_slot_container = m_pPlayer->GetQuickManager()->GetQuickSlotContainer();
    quick_slot_container->UpdateQuickSlot(rSlot.GetSlotIdx(), rSlot.GetPos(), rCurSlot.GetItemCode(), 
                                          SI_EQUIPMENT, at_pos, rCurSlot.GetItemCode());

    //  m_pPlayer->GetQuickManager()->GetQuickSlotContainer()->UpdateQuickSlot(rCurSlot.GetSlotIdx(), rCurSlot.GetPos(), rCurSlot.GetItemCode(), 
    //      SI_EQUIPMENT, at_pos, rCurSlot.GetItemCode());

    //장착했으므로 비활성화이면 활성화로 수정해주자.
    if(m_bRealEquip[at_pos] == false)
    {
        m_bRealEquip[at_pos] = true;
    }

    // Equip AttrChange
    ItemAttrCalculator itemCalc(m_pPlayer->GetPlayerAttr(), *this);

    BOOL bChangeFullOption = false;
    itemCalc.Equip(rCurSlot, m_pPlayer->CanApplyPCBangBenefit(true), true, bChangeFullOption); 

    // 무기가 변경되었을 경우 스타일 셋팅과 패시브 적용이 달라진다.
    if(at_pos == eEQUIPCONTAINER_WEAPON)
    {
        m_pPlayer->GetSkillSlotManager()->AddWeapon(rCurSlot.GetItemCode());
    }

    if(m_pPlayer->IsRenderEquipItem(at_pos))
    {
        // 자신을 제외한 모든 사람에게 날린다.
        MSG_CG_CHARINFO_APPREARANCE_BRD msg;
        msg.m_PlayerKey = (PLAYERKEY)m_pPlayer->GetObjectKey();
        msg.m_EquipPos  = at_pos;
        rCurSlot.CopyOut(msg.m_OptItemStream);
        m_pPlayer->SendPacketAround(&msg, sizeof(msg), false);
    }
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->AddSerialCode(eSERIAL_EQUIPMENT, GetSlotIdx(), rCurSlot.GetPos(),
            rCurSlot.GetWorldSerial());
    }

    static_cast<PlayerAttributes&>(m_pPlayer->GetAttr()).Display();

    return rt;
}

BOOL
CEquipmentSlotContainer::DeleteSlot(POSTYPE at_pos, SCSlot* pSlotOut)
{
    SASSERT(m_pPlayer != NULL, "[CEquipmentSlotContainer::DeleteSlot] m_pPlayer == NULL");

    BOOL bChangeFullOption = false;

    SCSlot& at_pos_slot = GetSlot(at_pos);
    if(CanUnEquip(at_pos, at_pos_slot) == 0)
        return false;

    SCItemSlot temp_delete_item_slot;

    if(SCSlotContainer::DeleteSlot(at_pos, &temp_delete_item_slot) == 0)
        return false;

    if(pSlotOut) {
        static_cast<SCItemSlot*>(pSlotOut)->Copy(temp_delete_item_slot);
    }

    temp_delete_item_slot.OnUnEquip();

    SASSERT(pSlotOut != NULL, "[CEquipmentSlotContainer::DeleteSlot] pSlotOut == NULL");

    // UnEquip AttrChange
    ItemAttrCalculator itemCalc(m_pPlayer->GetPlayerAttr(), *this);

    //활성화 이면 Unequip 
    if(m_bRealEquip[at_pos] != false) {
        BOOLEAN apply_pc_benefit = m_pPlayer->CanApplyPCBangBenefit(true);
        itemCalc.UnEquip(&temp_delete_item_slot, apply_pc_benefit, true, bChangeFullOption);
    }

    // 무기가 변경되었을 경우 스타일 셋팅과 패시브 적용이 달라진다.
    if(at_pos == eEQUIPCONTAINER_WEAPON)
        m_pPlayer->GetSkillSlotManager()->DelWeapon();

    if(m_pPlayer->IsRenderEquipItem(at_pos))
    {
        MSG_CG_CHARINFO_DISAPPREARANCE_BRD msg;
        msg.m_PlayerKey = (PLAYERKEY)m_pPlayer->GetObjectKey();
        msg.m_EquipPos  = at_pos;
        m_pPlayer->SendPacketAround(&msg, sizeof(msg), false);
    }
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->RemoveSerialCode(GetSlotIdx(), at_pos,
            temp_delete_item_slot.GetWorldSerial());
    };
    static_cast<PlayerAttributes&>(m_pPlayer->GetAttr()).Display();
    //벗은 아이템이 버프이벤트 아이템일 경우면
    BYTE byEventCode = temp_delete_item_slot.GetItemInfo()->m_byEventCode;
    if(byEventCode != 0)
    {
        //같은 종류의 이벤트 아이템을 입고 있지 않다면 상태 해제
        if(!IsEquipEventItem(byEventCode))
        {
            m_pPlayer->GetStatusManager()->RemoveEventStatus(byEventCode);
        }
    }

    return true;
}

BOOL
CEquipmentSlotContainer::ValidState()
{   // 예) PK시 드랍되는 ITEM이 장비일 경우... CanUnEquip에서 사용하면 안된다.
    // 패킷에서 사용할 수 있도록 하자.

    // (BUGFIX) (091202) (WOPS:5334) 인벤잠금 상태에서 아이템 제거 가능한 특수 처리(PK드랍)
    ItemManager::TransactionClosure::value_type transaction =
        m_pPlayer->GetItemManager()->GetCurrentTransactionAction();
    if(transaction == ItemManager::TransactionClosure::eTransType_None) {
        ProtectorInventory& rPROTECT = m_pPlayer->GetWarehouseInventoryProtector().INVENTORY();
        if(!rPROTECT.IsUsable())
            return false;
    }
    return SCItemSlotContainer::ValidState();
}

BOOL
CEquipmentSlotContainer::CanUnEquip(POSTYPE at_pos, SCSlot& IN rSlotIn)
{
    //비활성화 아이템이면 UnEquip가능
    if (m_bRealEquip[at_pos] == false)
        return true;

    ItemAttrCalculator itemCalc(m_pPlayer->GetPlayerAttr(), *this);
    itemCalc.Clear();

    m_pPlayer->GetItemManager()->UpdateAttrInventory();

    itemCalc.SetCalcTypeSetItemCount(true);
    BOOL bFullOption = false;

    m_mapEquipCount.clear();
    m_mapEquipTypeCount.clear();
    m_mapEquipGroupCount.clear();

    // (CHANGES) ...초기화 관련
    const BOOLEAN need_apply_pc_benefit = m_pPlayer->CanApplyPCBangBenefit(true);
    BOOL bEquipTable[MAX_EQUIPMENT_SLOT_NUM] = { 0, };

    const POSTYPE maxSlotNum = GetMaxSlotNum();
    POSTYPE itr_pos = 0;
    //POSTYPE stacked_inc = 0;
    //POSTYPE stacked_equip[MAX_EQUIPMENT_SLOT_NUM];
    //memset(stacked_equip, -1, sizeof(stacked_equip));

    while(itr_pos < maxSlotNum)
    {
        //현재 장착하고 있는 아이템중 장착 가능한 아이템의 능력치를 모두 적용 시킨다. 
        //현재 벗을 아이템때문에 장착 못하는 아이템은 bEquipTable[itr_pos] 로 구분한다.
        if(bEquipTable[itr_pos] ||
           itr_pos == at_pos ||
           m_bRealEquip[itr_pos] == false ||
           IsEmpty(itr_pos)) // (CHANGES) change check condition sequence : more cheap
        {
            ++itr_pos;
            continue;
        }

        SCItemSlot& itr_slot = static_cast<SCItemSlot &>(GetSlot(itr_pos));
        if(CanEquip(itr_pos, itr_slot, false))
        {
            bEquipTable[itr_pos] = true;
            //stacked_equip[stacked_inc++] = itr_pos;

            itemCalc.Equip(itr_slot, need_apply_pc_benefit, true, bFullOption);
            itr_pos = 0;
            continue;
        }

        ++itr_pos;
    }

    BOOL rt = true;

    for(POSTYPE itr_pos = 0; itr_pos < maxSlotNum ; ++itr_pos)
    {
        if(IsEmpty(itr_pos))
            continue;

        SCItemSlot& itr_slot = static_cast<SCItemSlot&>(GetSlot(itr_pos));

        IncEquipCount(itr_slot.GetItemCode());

        //현재 벗을려는 아이템 이거나, 착용하지 않은 아이템이거나, 비활성화 된 아이템이면 스킵
        if(itr_pos == at_pos || m_bRealEquip[itr_pos] == false)
            continue;

        if(bEquipTable[itr_pos] == false)
        {
            // 여기에 오는 경우는 현재 장착은 하고 있으나,
            // 현재 벗은 아이템 때문에 다시 Equip하지 못하는 아이템인 경우이다.
            // 이경우 false 라도 다시 다 입혀야 하기 때문에 break 하면 안됨
            itemCalc.Equip(itr_slot, need_apply_pc_benefit, false, bFullOption);
            rt = false;
        }
    }

    itemCalc.Equip(rSlotIn, need_apply_pc_benefit, false, bFullOption);

    return rt;
}

BOOL
CEquipmentSlotContainer::CanEquip(POSTYPE at_pos, SCSlot& IN rSlotIn, BOOL bCheckEquipState)
{
    if(bCheckEquipState != false)
    {
        // 드래곤 변신상태에서는 장착슬롯을 이용할 수 없다.
        if(m_pPlayer->GetStatusManager()->Status.IsDragonTransforming())
        {
            return false;
        }
    }

    SCItemSlot& item_slot = static_cast<SCItemSlot&>(rSlotIn);

    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    // (CHANGES) use BASE_ITEMINFO interface|
    // ref eITEM_MATERIAL_TYPE + for rider info
    if (item_slot.CanEquip() == false) {
        return false;
    }
    // 중복장착 개수 체크(0이면 무제한)
    if(item_info->m_byDupEquipNum)
    {
        int iCount = GetEquipCount(item_slot.GetItemCode());

        if(iCount >= item_info->m_byDupEquipNum)
            return false;
    }

    // 중복타입 장착 개수 체크
    //나중에 스크립트로 빼야지!!!
    if(item_info->m_byDupEquipTypeNum)
    {
        int iCount = GetEquipTypeCount((eITEMTYPE)item_info->m_wType);
        if(iCount >= 1)
            return false;
    }
    // 아이템 그룹별 개수 제한 체크
    if(item_info->m_byDupEquipGroup)
    {
        int iCount = GetEquipGroupCount(item_info->m_byDupEquipGroup);
        if(iCount >= item_info->m_byDupEquipGroupNum)
            return false;
    }
#ifdef _NA_002050_20110216_ADD_GENDER
    // 장착 가능한 성별인지 검사한다.
    if (item_info->IsCanUseGender((m_pPlayer->GetCharInfo())->gender) == false)
    {
        return false;
    }
#endif
    // class별 장착 여부
    switch(m_pPlayer->GetCharType())
    {
    case eCHAR_BERSERKER    : if(0 == item_info->m_wEqClass1) return false; break;
    case eCHAR_DRAGON       : if(0 == item_info->m_wEqClass2) return false; break;
    case eCHAR_SHADOW       : if(0 == item_info->m_wEqClass3) return false; break;
    case eCHAR_VALKYRIE     : if(0 == item_info->m_wEqClass4) return false; break;
    case eCHAR_ELEMENTALIST : if(0 == item_info->m_wEqClass5) return false; break;
    // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eCHAR_MYSTIC:      : if (item_info->can_equip_class6 == false) return false; break;
    // _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eCHAR_HELLROID     : if(item_info->can_equip_class7 == false) return false; break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_WITCHBLADE     : if(item_info->can_equip_class8 == false) return false; break;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    default:
        SUNLOG(eCRITICAL_LOG, "[CEquipmentSlotContainer::CanEquip] Wrong Char_Type in CanEquip()");
        return false;
    }

    switch(item_info->m_wEqPos)
    {
    case eEQUIPCONTAINER_RING1:
    case eEQUIPCONTAINER_RING2:
        if(at_pos != eEQUIPCONTAINER_RING1 &&
           at_pos != eEQUIPCONTAINER_RING2)
        {
            return false;
        }

        break;

    case eEQUIPCONTAINER_SACCESSORY1:
    case eEQUIPCONTAINER_SACCESSORY2:
    case eEQUIPCONTAINER_SACCESSORY3:
        if(at_pos != eEQUIPCONTAINER_SACCESSORY1 &&
           at_pos != eEQUIPCONTAINER_SACCESSORY2 &&
           at_pos != eEQUIPCONTAINER_SACCESSORY3)
        {
            return false;
        }

    #ifdef _NA001955_110210_WING_COSTUME_ACCESSORY_EXTENTION
    #else
        if((at_pos == eEQUIPCONTAINER_SACCESSORY2 || at_pos == eEQUIPCONTAINER_SACCESSORY3) &&
           m_pPlayer->GetCharType() != eCHAR_ELEMENTALIST)
        {
            return false;
        }
    #endif
        break;

    case eEQUIPCONTAINER_CHARGE1:
    case eEQUIPCONTAINER_CHARGE2:
    case eEQUIPCONTAINER_CHARGE3:
    case eEQUIPCONTAINER_CHARGE4:
    case eEQUIPCONTAINER_CHARGE5:
        if(at_pos != eEQUIPCONTAINER_CHARGE1 &&
           at_pos != eEQUIPCONTAINER_CHARGE2 &&
           at_pos != eEQUIPCONTAINER_CHARGE3 &&
           at_pos != eEQUIPCONTAINER_CHARGE4 &&
           at_pos != eEQUIPCONTAINER_CHARGE5)
        {
                return false;
        }

        if(at_pos == eEQUIPCONTAINER_CHARGE4 &&
           m_pPlayer->GetCharInfo()->m_ExtraEquipCashSlotCount < 1)
        {
            return false;
        }

        if(at_pos == eEQUIPCONTAINER_CHARGE5 &&
           m_pPlayer->GetCharInfo()->m_ExtraEquipCashSlotCount < 2)
        {
            return false;
        }

        break;

    case eEQUIPCONTAINER_PC_ROOM1:
    case eEQUIPCONTAINER_PC_ROOM2:
    case eEQUIPCONTAINER_PC_ROOM3:  
        if(at_pos != eEQUIPCONTAINER_PC_ROOM1 &&
           at_pos != eEQUIPCONTAINER_PC_ROOM2 &&
           at_pos != eEQUIPCONTAINER_PC_ROOM3)
        {
            return false;
        }

        if(m_pPlayer->CanApplyPCBangBenefit(true) == false)
            return false;

        break;

    default:
        if(item_info->m_wEqPos != at_pos)
            return false;
        break;
    }

    if(!ValidLimitStr(item_slot))    return false;
    if(!ValidLimitDex(item_slot))    return false;
    if(!ValidLimitVit(item_slot))    return false;
    if(!ValidLimitInt(item_slot))    return false;
    if(!ValidLimitSpr(item_slot))    return false;
    if(!ValidLimitLevel(item_slot))  return false;

    if(!ValidLimitSKill1(item_slot)) return false;
    if(!ValidLimitSKill2(item_slot)) return false;

    if(item_slot.IsFateItem())
    {
        if(!(item_slot.GetFateIdentify()))
            return false;
    }

    if(item_slot.IsPossessionItem() &&
       item_slot.GetIdentifyPossession() == 0)
    {
        return false;
    }
    if(item_slot.IsDestroyItem())
        return false;

    return true;
}

//--------------------------------------------------------------------------------------------------
// internal function
inline BOOL
CEquipmentSlotContainer::ValidLimitAll(short player_limit, short item_limit)
{
    // 스탯 제한
    short decrease_limit_stat = m_pPlayer->GetDecreaseLimitStat();
    if(player_limit + decrease_limit_stat < item_limit)
        return false;

    return true;
}

// 스크립트에서 착용 요구치 조정값 적용(CalcLimitStatWeapon/Armor래핑)
WORD
CEquipmentSlotContainer::CalcLimitStat(WORD ItemLimitStr, SCItemSlot& item_slot,
                                       BOOL is_weapon, BOOL is_armor)
{
    WORD (*CalcLimitStatRouteFunc)(WORD,BYTE,eRANK,LEVELTYPE,float);

    if(is_weapon) {
        CalcLimitStatRouteFunc = CalcLimitStatWeapon;
    }
    else if(is_armor) {
        CalcLimitStatRouteFunc = CalcLimitStatArmor;
    }
    else
    {
        DEBUG_CODE(if(ItemLimitStr!=0) SUNLOG(eFULL_LOG, "[CEquipmentSlotContainer::CalcLimitStat] 스크립트오류!장비(%s)가아닌데0이아니다", item_slot.GetItemInfo()->m_pszName););
        return ItemLimitStr;
    }

    BYTE byEnchant = item_slot.GetEnchant();
    LEVELTYPE lv   = item_slot.GetItemInfo()->m_LV;
    float fLimitRatio  =
        EnchantParser::Instance()->GetLimitRatio(lv, byEnchant, is_weapon, item_slot.IsLimited());

    return (WORD)CalcLimitStatRouteFunc(ItemLimitStr,byEnchant,item_slot.GetRank(),lv,fLimitRatio);
}

BOOL
CEquipmentSlotContainer::ValidLimitStr(SCItemSlot& item_slot)
{
    BASE_ITEMINFO* pInfo = item_slot.GetItemInfo();

    WORD LimitStr = pInfo->m_wLimitStr;
    if(0 == LimitStr) return true;

    //디바인?
    if(item_slot.IsDivine())
    {
        //LimitStr = CalcLimitStrForDivineWeapon(LimitStr, m_pPlayer->GetCharType(), (eITEMTYPE)pInfo->m_wType);
        LimitStr = pInfo->m_wDivLimitStr;
    }

    //리미티드?
    if(item_slot.IsLimited())
    {
        INT Ratio = 0;
        Ratio = item_slot.GetLimitValueRatio();
        float fValue = 0;

        if(Ratio >= 0)
            fValue = 0.5f + 0.001f;//FLT_EPSILON;           
        else
            fValue = -0.5f - 0.001f;//FLT_EPSILON;

        fValue = LimitStr + ((float)Ratio * (float)LimitStr) / 100.0f + fValue;
        LimitStr = (WORD)fValue;
    }

    LimitStr = CalcLimitStat(LimitStr, item_slot, pInfo->IsWeapon(), pInfo->IsArmor());
    // 스탯 제한
    return ValidLimitAll(m_pPlayer->GetLimitStrength(), LimitStr);
}

BOOL
CEquipmentSlotContainer::ValidLimitDex(SCItemSlot& item_slot)
{
    const BASE_ITEMINFO* pInfo = item_slot.GetItemInfo();

    WORD LimitDex = pInfo->m_wLimitDex;
    if(0 == LimitDex) return true;

    //디바인?
    if(item_slot.IsDivine())
    {
        //LimitDex = CalcLimitDexForDivineWeapon(LimitDex, m_pPlayer->GetCharType(), (eITEMTYPE)pInfo->m_wType);
        LimitDex = pInfo->m_wDivLimitDex;
    }

    //리미티드?
    if(item_slot.IsLimited())
    {       
        INT Ratio = 0;
        Ratio = item_slot.GetLimitValueRatio();
        float fValue = 0;

        if(Ratio >= 0)
            fValue = 0.5f + 0.001f;//FLT_EPSILON;           
        else
            fValue = -0.5f - 0.001f;//FLT_EPSILON;

        fValue = LimitDex + ((float)Ratio * (float)LimitDex) / 100.0f + fValue;
        LimitDex = (WORD)fValue;
    }

    LimitDex = CalcLimitStat(LimitDex, item_slot, pInfo->IsWeapon(), pInfo->IsArmor());

    // 스탯 제한
    return ValidLimitAll(m_pPlayer->GetLimitDexterity(), LimitDex);
    //if(m_pPlayer->GetLimitDexterity() < LimitDex) return false;
    //return true;
}

BOOL
CEquipmentSlotContainer::ValidLimitVit(SCItemSlot& item_slot)
{
    const BASE_ITEMINFO* pInfo = item_slot.GetItemInfo();

    WORD LimitVit = pInfo->m_wLimitVit;
    if(0 == LimitVit) return true;

    //디바인?
    if(item_slot.IsDivine())
    {
        //LimitInt = CalcLimitIntForDivineWeapon(LimitVit, m_pPlayer->GetCharType(), (eITEMTYPE)pInfo->m_wType);
        LimitVit = pInfo->m_wDivLimitVit;
    }

    //리미티드?
    if(item_slot.IsLimited())
    {       
        INT Ratio = 0;
        Ratio = item_slot.GetLimitValueRatio();
        float fValue = 0;

        if(Ratio >= 0)
            fValue = 0.5f + 0.001f;//FLT_EPSILON;           
        else
            fValue = -0.5f - 0.001f;//FLT_EPSILON;

        fValue = LimitVit + ((float)Ratio * (float)LimitVit) / 100.0f + fValue;
        LimitVit = (WORD)fValue;
    }

    LimitVit = CalcLimitStat(LimitVit, item_slot, pInfo->IsWeapon(), pInfo->IsArmor());

    // 스탯 제한
    return ValidLimitAll(m_pPlayer->GetLimitVitality(), LimitVit);
    //if(m_pPlayer->GetLimitVitality() < LimitVit) return false;
    //return true;
}

BOOL
CEquipmentSlotContainer::ValidLimitInt(SCItemSlot& item_slot)
{
    const BASE_ITEMINFO* pInfo = item_slot.GetItemInfo();

    WORD LimitInt = pInfo->m_wLimitInt;
    if(0 == LimitInt) return true;

    //디바인?
    if(item_slot.IsDivine())
    {
        //LimitInt = CalcLimitIntForDivineWeapon(LimitInt, m_pPlayer->GetCharType(), (eITEMTYPE)pInfo->m_wType);
        LimitInt = pInfo->m_wDivLimitInt;
    }


    //리미티드?
    if(item_slot.IsLimited())
    {
        INT Ratio = 0;
        Ratio = item_slot.GetLimitValueRatio();
        float fValue = 0;

        if(Ratio >= 0)
            fValue = 0.5f + 0.001f;//FLT_EPSILON;
        else
            fValue = -0.5f - 0.001f;//FLT_EPSILON;

        fValue = LimitInt + ((float)Ratio * (float)LimitInt) / 100.0f + fValue;
        LimitInt = (WORD)fValue;
    }

    LimitInt = CalcLimitStat(LimitInt, item_slot, pInfo->IsWeapon(), pInfo->IsArmor());

    // 스탯 제한
    return ValidLimitAll(m_pPlayer->GetLimitInteligence(), LimitInt);
    //if(m_pPlayer->GetLimitInteligence() < LimitInt) return false;
    //return true;
}

BOOL
CEquipmentSlotContainer::ValidLimitSpr(SCItemSlot& item_slot)
{
    BASE_ITEMINFO* pInfo = item_slot.GetItemInfo();

    WORD LimitSpr = pInfo->m_wLimitSpr;
    if(0 == LimitSpr) return true;

    //디바인?
    if(item_slot.IsDivine())
    {
        //LimitSpr = CalcLimitIntForDivineWeapon(LimitSpr, m_pPlayer->GetCharType(), (eITEMTYPE)pInfo->m_wType);
        LimitSpr = pInfo->m_wDivLimitSpr;
    }

    //리미티드?
    if(item_slot.IsLimited())
    {       
        INT Ratio = 0;
        Ratio = item_slot.GetLimitValueRatio();
        float fValue = 0;

        if(Ratio >= 0)
            fValue = 0.5f + 0.001f;//FLT_EPSILON;
        else
            fValue = -0.5f - 0.001f;//FLT_EPSILON;

        fValue = LimitSpr + ((float)Ratio * (float)LimitSpr) / 100.0f + fValue;
        LimitSpr = (WORD)fValue;
    }

    LimitSpr = CalcLimitStat(LimitSpr, item_slot, pInfo->IsWeapon(), pInfo->IsArmor());

    // 스탯 제한
    return ValidLimitAll(m_pPlayer->GetLimitSpirit(), LimitSpr);
    //if(m_pPlayer->GetLimitSpirit() < LimitSpr) return false;
    //return true;
}

BOOL
CEquipmentSlotContainer::ValidLimitLevel(SCItemSlot& item_slot)
{
    BASE_ITEMINFO* pInfo = item_slot.GetItemInfo();

    WORD LimitLV = pInfo->m_LimitEqLevel;
    if(0 == LimitLV) return true;

    //if(item_slot.GetItemInfo()->IsAccessory())
    //{
    //  LimitLV = CalcLimitLVAccessory(LimitLV, item_slot.GetRank());
    //}

    if(item_slot.IsLimited())
    {       
        INT Ratio = 0;
        Ratio = item_slot.GetLimitValueRatio();
        float fValue = 0;

        if(Ratio >= 0)
            fValue = 0.5f + 0.001f;//FLT_EPSILON;
        else
            fValue = -0.5f - 0.001f;//FLT_EPSILON;

        fValue = LimitLV + ((float)Ratio * (float)LimitLV) / 100.0f + fValue;
        LimitLV = (WORD)fValue;
    }   

    // 레벨 제한
    if(m_pPlayer->GetLevel() < LimitLV) return false;

    return true;
}

BOOL
CEquipmentSlotContainer::ValidLimitSKill1(SCItemSlot& item_slot)
{
    BASE_ITEMINFO* pInfo = item_slot.GetItemInfo();

    WORD LimitSkill = pInfo->m_wLimitSkil1;
    if(0 == LimitSkill) return true;

    //디바인?
    if(item_slot.IsDivine())
    {
        LimitSkill = pInfo->m_wDivLimitSkill1;
    }

    //리미티드?
    if(item_slot.IsLimited())
    {       
        INT Ratio = 0;
        Ratio = item_slot.GetLimitValueRatio();
        float fValue = 0;

        if(Ratio >= 0)
            fValue = 0.5f + 0.001f;//FLT_EPSILON;
        else
            fValue = -0.5f - 0.001f;//FLT_EPSILON;

        fValue = LimitSkill + ((float)Ratio * (float)LimitSkill) / 100.0f + fValue;
        LimitSkill = (WORD)fValue;
    }

    LimitSkill = CalcLimitStat(LimitSkill, item_slot, pInfo->IsWeapon(), pInfo->IsArmor());

    // 스탯 제한
    return ValidLimitAll(m_pPlayer->GetLimitExperty1(), LimitSkill);
    //if(m_pPlayer->GetLimitExperty1() < LimitSkill) return false;
    //return true;
}

BOOL
CEquipmentSlotContainer::ValidLimitSKill2(SCItemSlot& item_slot)
{
    BASE_ITEMINFO* pInfo = item_slot.GetItemInfo();

    WORD LimitSkill = pInfo->m_wLimitSkil2;
    if(0 == LimitSkill) return true;

    //디바인?
    if(item_slot.IsDivine())
    {
        LimitSkill = pInfo->m_wDivLimitSkill2;
    }

    //리미티드?
    if(item_slot.IsLimited())
    {       
        INT Ratio = 0;
        Ratio = item_slot.GetLimitValueRatio();
        float fValue = 0;

        if(Ratio >= 0)
            fValue = 0.5f + 0.001f;//FLT_EPSILON;
        else
            fValue = -0.5f - 0.001f;//FLT_EPSILON;

        fValue = LimitSkill + ((float)Ratio * (float)LimitSkill) / 100.0f + fValue;
        LimitSkill = (WORD)fValue;
    }

    LimitSkill = CalcLimitStat(LimitSkill, item_slot, pInfo->IsWeapon(), pInfo->IsArmor());

    // 스탯 제한
    return ValidLimitAll(m_pPlayer->GetLimitExperty2(), LimitSkill);
    //if(m_pPlayer->GetLimitExperty2() < LimitSkill) return false;
    //return true;
}

VOID
CEquipmentSlotContainer::CheckupActiveEquipItemAll()
{
    // (CHANGES) initalized
    ZeroMemory(m_bRealEquip, sizeof(m_bRealEquip));

    ItemAttrCalculator itemCalc(m_pPlayer->GetPlayerAttr(), *this);
    itemCalc.Clear();

    m_pPlayer->GetItemManager()->UpdateAttrInventory();

    itemCalc.SetCalcTypeSetItemCount(true);
    m_mapEquipCount.clear();
    m_mapEquipTypeCount.clear();
    m_mapEquipGroupCount.clear();

    BOOLEAN need_apply_pc_benefit = m_pPlayer->CanApplyPCBangBenefit(true);
    POSTYPE itr_pos = 0;
    const POSTYPE maxSlotNum = GetMaxSlotNum();
    while(itr_pos < maxSlotNum)
    {
        if(m_bRealEquip[itr_pos] || IsEmpty(itr_pos))
        {
            ++itr_pos;
            continue;
        }

        SCItemSlot& itr_slot = static_cast<SCItemSlot &>(GetSlot(itr_pos));

        // 실제 착용하고 있으면서 능력치도 적용되는 놈
        if(CanEquip(itr_pos, itr_slot, false))
        {
            m_bRealEquip[itr_pos] = true;
            BOOL bFullOption = false;
            itemCalc.Equip(itr_slot, need_apply_pc_benefit, true, bFullOption);
            itr_pos = 0;
            continue;
        }

        ++itr_pos;
    }   

    // 실제 착용은 하고 있으나 능력치는 적용 안되는 놈
    for(POSTYPE itr_pos = 0; itr_pos < maxSlotNum; ++itr_pos)
    {
        if(!IsEmpty(itr_pos))
        {
            SCItemSlot& itr_slot = static_cast<SCItemSlot &>(GetSlot(itr_pos));
            IncEquipCount(itr_slot.GetItemCode());
        }
    }
}

void
CEquipmentSlotContainer::OnInsert(SCSlot& IN rSlot)
{
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->AddExpirationItem(static_cast<SCItemSlot&>(rSlot));
    }
    // 장착 개수 증가
    IncEquipCount(static_cast<SCItemSlot&>(rSlot).GetItemCode());
}

void
CEquipmentSlotContainer::OnDelete(SCSlot& IN rSlot)
{
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
            m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->RemoveExpirationItem(static_cast<SCItemSlot&>(rSlot));
    }
    // 장착 개수 감소
    DecEquipCount(static_cast<SCItemSlot&>(rSlot).GetItemCode());
}

int
CEquipmentSlotContainer::GetEquipCount(SLOTCODE item_code)
{
    HASHMAP_EQUIP_COUNT_ITR it = m_mapEquipCount.find(item_code);
    if(it != m_mapEquipCount.end())
        return it->second;

    return 0;
}

VOID
CEquipmentSlotContainer::IncEquipCount(SLOTCODE item_code)
{
    HASHMAP_EQUIP_COUNT_ITR it = m_mapEquipCount.find(item_code);
    if(it != m_mapEquipCount.end())
    {
        it->second += 1;
    }
    else
    {
        m_mapEquipCount.insert(HASHMAP_EQUIP_COUNT_PAIR(item_code, 1));
    }

    const BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if(pItemInfo == NULL) {
        ASSERT(false);
        return;
    }
    IncEquipTypeCount((eITEMTYPE)pItemInfo->m_wType);
    IncEquipGroupCount((eITEMTYPE)pItemInfo->m_byDupEquipGroup);
}

VOID
CEquipmentSlotContainer::DecEquipCount(SLOTCODE item_code)
{
    HASHMAP_EQUIP_COUNT_ITR it = m_mapEquipCount.find(item_code);
    if(it != m_mapEquipCount.end())
    {
        if(it->second <= 1)
            m_mapEquipCount.erase(item_code);
        else
            it->second -= 1;
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] item_code[%d] Can't Remove EquipCount",
               item_code);
    }

    const BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if(pItemInfo == NULL)
    {
        ASSERT(false);
        return;
    }

    DecEquipTypeCount((eITEMTYPE)pItemInfo->m_wType);
    DecEquipGroupCount((eITEMTYPE)pItemInfo->m_byDupEquipGroup);
}

int
CEquipmentSlotContainer::GetEquipTypeCount(eITEMTYPE item_type)
{
    HASHMAP_EQUIP_TYPE_COUNT_ITR it = m_mapEquipTypeCount.find(item_type);
    if(it != m_mapEquipTypeCount.end())
        return it->second;

    return 0;
}

VOID
CEquipmentSlotContainer::IncEquipTypeCount(eITEMTYPE item_type)
{
    HASHMAP_EQUIP_TYPE_COUNT_ITR itr = m_mapEquipTypeCount.find(item_type);
    if(itr != m_mapEquipTypeCount.end())
    {
        itr->second += 1;
    }
    else
    {
        m_mapEquipTypeCount.insert(HASHMAP_EQUIP_TYPE_COUNT_PAIR(item_type, 1));
    }
}

VOID
CEquipmentSlotContainer::DecEquipTypeCount(eITEMTYPE item_type)
{
    HASHMAP_EQUIP_TYPE_COUNT_ITR itr = m_mapEquipTypeCount.find(item_type);
    if(itr != m_mapEquipTypeCount.end())
    {
        if(itr->second <= 1)
            m_mapEquipTypeCount.erase(item_type);
        else
            itr->second -= 1;
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] item_code[%d] Can't Remove EquipTypeCount",
               item_type);
    }
}


//그룹별 장착 개수 제한 관리
// (NOTE) it has not the usage case at this point in time (100505)
int
CEquipmentSlotContainer::GetEquipGroupCount(BYTE byEquipGroup)
{
    EQUIP_GROUP_COUNT_MAP_ITER it = m_mapEquipGroupCount.find(byEquipGroup);
    if(it != m_mapEquipGroupCount.end())
        return it->second;

    return 0;
}

VOID
CEquipmentSlotContainer::IncEquipGroupCount(BYTE byEquipGroup)
{
    EQUIP_GROUP_COUNT_MAP_ITER itr = m_mapEquipGroupCount.find(byEquipGroup);
    if(itr != m_mapEquipGroupCount.end())
    {
        itr->second += 1;
    }
    else
    {
        m_mapEquipGroupCount.insert(std::make_pair(byEquipGroup, 1));
    }
}

VOID
CEquipmentSlotContainer::DecEquipGroupCount(BYTE byEquipGroup)
{
    EQUIP_GROUP_COUNT_MAP_ITER itr = m_mapEquipGroupCount.find(byEquipGroup);
    if(itr != m_mapEquipGroupCount.end())
    {
        if(itr->second <= 1)
            m_mapEquipGroupCount.erase(byEquipGroup);
        else
            itr->second -= 1;
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] GroupCode[%d] Can't Remove EquipGroupCount",
               byEquipGroup);
    }

}

VOID
CEquipmentSlotContainer::SendSetItemBrd(BOOL bFullOption)
{   
    if(!m_pPlayer)
        return;

    MSG_CG_ITEM_SETITEM_CHANGE_BRD  Brd;
    Brd.m_PlayerKey = (PLAYERKEY)m_pPlayer->GetObjectKey();
    Brd.m_bFull = bFullOption;
    m_pPlayer->SendPacketAround(&Brd, sizeof(Brd), false);
}

//_NA_0_20100722_AUTO_SYSTEM
POSTYPE 
CEquipmentSlotContainer::GetPosEquipByItemType(eITEMTYPE type, POSTYPE start, POSTYPE end)
{
    ASSERT(start < end);
    ASSERT(end <= eEQUIPCONTAINER_MAX);

    const POSTYPE maxSlotNum = GetMaxSlotNum();
    for(POSTYPE itr_pos = start; itr_pos < end ; ++itr_pos)
    {
        if(!IsEmpty(itr_pos))
        {
            SCItemSlot& item_slot = static_cast<SCItemSlot&>(GetSlot(itr_pos));
            if (type == item_slot.GetItemInfo()->m_wType)
                return itr_pos;
        }
    }

    return eEQUIPCONTAINER_INVALID;
}

BOOL
CEquipmentSlotContainer::IsEquipEventItem(BYTE byEventCode)
{
    const POSTYPE maxSlotNum = GetMaxSlotNum();
    for(POSTYPE itr_pos = 0; itr_pos < maxSlotNum ; ++itr_pos)
    {
        if(!IsEmpty(itr_pos))
        {
            SCItemSlot& item_slot = static_cast<SCItemSlot&>(GetSlot(itr_pos));
            if(byEventCode == item_slot.GetItemInfo()->m_byEventCode)
                return TRUE;
        }
    }

    return FALSE;
}

//==================================================================================================
// CHANGES: f110420.5L, constant information is added whether this item is renderable equipment.
bool CEquipmentSlotContainer::IsDefaultRenderableEquipItem(POSTYPE pos)
{
    switch (pos)
    {
    case eEQUIPCONTAINER_WEAPON:
    case eEQUIPCONTAINER_ARMOR:
    case eEQUIPCONTAINER_PROTECTOR: //< 엘리트 아이템 풀셋일 경우 효과 추가 (2007.08.03 - 김창현, 전보환)
    case eEQUIPCONTAINER_HELMET:
    case eEQUIPCONTAINER_PANTS:
    case eEQUIPCONTAINER_BOOTS:
    case eEQUIPCONTAINER_GLOVE:
    case eEQUIPCONTAINER_SACCESSORY1:
    case eEQUIPCONTAINER_SACCESSORY2:
    case eEQUIPCONTAINER_SACCESSORY3:
    case eEQUIPCONTAINER_BELT:      //< 엘리트 아이템 풀셋일 경우 효과 추가
    case eEQUIPCONTAINER_SHIRTS:    //< 엘리트 아이템 풀셋일 경우 효과 추가
    //case eEQUIPCONTAINER_CAPE:    //< 현재 추가 안됨
    case eEQUIPCONTAINER_CHARGE1:
    case eEQUIPCONTAINER_CHARGE2:
    case eEQUIPCONTAINER_CHARGE3:
    case eEQUIPCONTAINER_CHARGE4:
    case eEQUIPCONTAINER_CHARGE5:
        return true;
    //case eEQUIPCONTAINER_PROTECTOR:
    //case eEQUIPCONTAINER_BELT:
    //case eEQUIPCONTAINER_SHIRTS:
    case eEQUIPCONTAINER_RING1:
    case eEQUIPCONTAINER_RING2:
    case eEQUIPCONTAINER_NECKLACE:

    case eEQUIPCONTAINER_PC_ROOM1:
    case eEQUIPCONTAINER_PC_ROOM2:
    case eEQUIPCONTAINER_PC_ROOM3:
        return false;
    };

    return false;
}

//==================================================================================================

#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
