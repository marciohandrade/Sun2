#ifndef __GAMESERVER_EQUIP_SLOTCONTAINER_H
#define __GAMESERVER_EQUIP_SLOTCONTAINER_H
#pragma once

//==================================================================================================
#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
//==================================================================================================

#include "./EquipmentSlotContainerEP2.h"

//==================================================================================================
#else //if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
//==================================================================================================
// EquipmentSlotContainer.h: interface for the CEquipmentSlotContainer class.
//
//////////////////////////////////////////////////////////////////////
//==================================================================================================
/// CEquipmentSlotContainer class
/**
@author
    Kim Min Wook < taiyo@webzen.com >   
@since
    2004. 8. 11~
@history
    - 2006/2/15 : 아이템제한처리때문에 스텟up아이템을 내리기위해 CanUnEquip() 함수추가
                => DeleteSlot()의 리턴값(BOOL) 추가, 실패할 때 예외(nak) 처리
    - 2006/5/12 : 제한수치가 맞지 않는 아이템이면 탈착 가능하게!
*/
//==================================================================================================

#include <hash_map>
#include <SCItemSlotContainer.h>
#include <GroupCounter.h>
#include <ResultCode.h>

//==================================================================================================

class Player;
class SCItemSlot;

typedef STLX_HASH_MAP<SLOTCODE, int>        HASHMAP_EQUIP_COUNT;
typedef HASHMAP_EQUIP_COUNT::iterator       HASHMAP_EQUIP_COUNT_ITR;
typedef std::pair<SLOTCODE, int>            HASHMAP_EQUIP_COUNT_PAIR;

typedef STLX_HASH_MAP<eITEMTYPE, int>       HASHMAP_EQUIP_TYPE_COUNT;
typedef HASHMAP_EQUIP_TYPE_COUNT::iterator  HASHMAP_EQUIP_TYPE_COUNT_ITR;
typedef std::pair<eITEMTYPE, int>           HASHMAP_EQUIP_TYPE_COUNT_PAIR;

typedef STLX_HASH_MAP<BYTE, BYTE>           EQUIP_GROUP_COUNT_MAP;
typedef EQUIP_GROUP_COUNT_MAP::iterator     EQUIP_GROUP_COUNT_MAP_ITER;

//==================================================================================================

class CEquipmentSlotContainer : public SCItemSlotContainer
{
public:
    CEquipmentSlotContainer();
    virtual ~CEquipmentSlotContainer();

    //virtual void UpdateSlot(POSTYPE at_pos, eSlotType type, DBSERIAL serial, SLOTCODE code, POSTYPE quickPos, DURATYPE dura, BYTE level, eUPDATE_BIT upbits = UB_ALL);
    //virtual void UpdateSlot(POSTYPE at_pos, SCSlot& IN rSlot);

    virtual void ClearAll();

    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN rSlot);
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* pSlotOut);

    virtual void OnInsert(SCSlot& IN rSlot);
    virtual void OnDelete(SCSlot& IN rSlot);

    virtual BOOL ValidState();

    BOOL CanEquip(POSTYPE at_pos, SCSlot& IN rSlotIn, BOOL bCheckEquipState = TRUE);
    BOOL CanUnEquip(POSTYPE at_pos, SCSlot& IN rSlotIn);
    BOOL ValidLimitStr(SCItemSlot& item_slot);
    BOOL ValidLimitDex(SCItemSlot& item_slot);
    BOOL ValidLimitVit(SCItemSlot& item_slot);
    BOOL ValidLimitInt(SCItemSlot& item_slot);
    BOOL ValidLimitSpr(SCItemSlot& item_slot);
    BOOL ValidLimitLevel(SCItemSlot& item_slot);
    BOOL ValidLimitSKill1(SCItemSlot& item_slot);
    BOOL ValidLimitSKill2(SCItemSlot& item_slot);

    WORD CalcLimitStat(WORD ItemLimitStr, SCItemSlot& item_slot, BOOL IsWeapon, BOOL IsArmor);

    void SetPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }

    // 장착 개수 제한 관리
    int GetEquipCount(SLOTCODE item_code);
    void IncEquipCount(SLOTCODE item_code);
    void DecEquipCount(SLOTCODE item_code);

    int GetEquipTypeCount(eITEMTYPE item_type);
    void IncEquipTypeCount(eITEMTYPE item_type);
    void DecEquipTypeCount(eITEMTYPE item_type);

    //그룹별 장착 개수 제한 관리
    // (NOTE) it has not the usage case at this point in time (100505)
    int GetEquipGroupCount(BYTE byEquipGroup);
    void IncEquipGroupCount(BYTE byEquipGroup);
    void DecEquipGroupCount(BYTE byEquipGroup);

    void SendSetItemBrd(BOOL bFullOption);

    void CheckupActiveEquipItemAll();    
    BOOL IsRealEquip(POSTYPE at_pos)    { return m_bRealEquip[at_pos]; }
    POSTYPE GetPosEquipByItemType(eITEMTYPE Type, POSTYPE start, POSTYPE end); // _NA_0_20100722_AUTO_SYSTEM
    BOOL IsEquipEventItem(BYTE byEventCode);
    // CHANGES: f110420.5L, constant information is added whether this item is renderable equipment.
    static bool IsDefaultRenderableEquipItem(POSTYPE pos);
protected:
    inline BOOL ValidLimitAll(short player_limit, short item_limit); //internal function
    //----------------------------------------------------------------------------------------------
    Player* m_pPlayer;
    BOOL m_bRealEquip[MAX_EQUIPMENT_SLOT_NUM];

    HASHMAP_EQUIP_COUNT      m_mapEquipCount;        // 같은 아이템 장착개수 관리
    HASHMAP_EQUIP_TYPE_COUNT m_mapEquipTypeCount;    // 같은 종류 아이템 장착 개수 관리
    // (NOTE) it has not the usage case at this point in time (100505)
    EQUIP_GROUP_COUNT_MAP    m_mapEquipGroupCount;   // 같은 아이템 그룹별 장착 개수 관리
    //
    WAVERIX_TEST_BLOCK_IN_CLASS(CEquipmentSlotContainer);
};

//==================================================================================================
#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
//==================================================================================================
#endif //__GAMESERVER_EQUIP_SLOTCONTAINER_H
