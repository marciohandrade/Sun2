#ifndef _GAMESERVER_INVENTORY_SLOTCONTAINER_H
#define _GAMESERVER_INVENTORY_SLOTCONTAINER_H
#pragma once
// InventorySlotContainer.h: interface for the CInventorySlotContainer class.
//
//////////////////////////////////////////////////////////////////////
#include <ResultCode.h>
#include <SCItemSlotContainer.h>

//==================================================================================================
namespace nsSlot {
;
// f100904.4L, same priority tabs require two phase searching like non-empty if overlapping item,
// otherwise empty slot.
// iterators of a sorted list that have ranges save to iterator array of the below container
// used in an inversed select operation of the ItemQuery.
struct SearchOffset
{
    enum eSearchMode {
        eSearch_1st_FindOverlappingSlot = 0,
        eSearch_2nd_FindEmptySlot,
        eSearch_3rd_FindOverlappingSlot,
        eSearch_4th_FindEmptySlot,
        eSearch_5th_FindOverlappingSlot,
        eSearch_6th_FindEmptySlot,
        //
        eSearch_Mode_Counts
    };
    SCItemSlotContainer::SLOT_LINEAR_LIST::const_iterator search_it[eSearch_Mode_Counts];
    SCItemSlotContainer::SLOT_LINEAR_LIST::const_iterator search_end[eSearch_Mode_Counts];
};
class ItemFunctionToggle;
}; //end of namespace

//==================================================================================================
class CInventorySlotContainer : public SCItemSlotContainer
{
private:
    typedef STLX_MAP<SLOTCODE, INT>         OPTION_EFFECT_ITEMS;
    typedef OPTION_EFFECT_ITEMS::iterator   OPTION_EFFECT_ITEMS_IT;
    typedef std::pair<SLOTCODE, INT>        OPTION_EFFECT_ITEMS_INFO;
public:
    CInventorySlotContainer();
    virtual ~CInventorySlotContainer();

    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN rSlot); // 슬롯에 객체 추가
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* pSlotOut);
    virtual void ClearAll();

    void UpdateAttr(); //ItemManager::UpdateAttrForEquipItem시에만...
    BOOL CheckEnableInsert(const SLOTCODE itemCode);

    virtual void OnInsert(SCSlot& IN rSlot);
    virtual void OnDelete(SCSlot& IN rSlot);
    virtual void OnUpdate(SCSlot& IN rSlot, int AddNum);

    void SetPlayer(Player* pPlayer) { m_pPlayer = pPlayer; }

    template< class STREAM_TYPE >
        BOOL GetTabTotalInfo(BYTE tab_index, JUST_INVENTORY_TOTAL_INFO* OUT pTotalInfo);

    //인벤토리락
    virtual BOOL ValidState();
    virtual void SetValidState(BOOL v);
    //// The interface to get sorted slot list by configurations
    //const SLOT_LINEAR_LIST& GetConfiguredSlotList(const nsSlot::InventoryConfig::eInsertRule) const;
    // f100904.4L, The interface to get sorted slot list start/end offset by configurations
    void GetConfiguredSlotList(const nsSlot::InventoryConfig::eInsertRule insert_rule,
                               nsSlot::SearchOffset* result_offset_info) const;
    // update all inventory insert rule configures
    void UpdateInventoryConfiguration(const nsSlot::InventoryConfig& configure);
    void ClearConfiguredSlotList();

private:
    BOOL CheckPrevInsert(SCItemSlot& rInsertSlot);
    void ApplyEffect(SCItemSlot& item_slot,
        const int changed_num, const int changed_total_count);

protected:
    Player* m_pPlayer;
    BYTE    m_TabNum;

private:
    OPTION_EFFECT_ITEMS m_OptionEffectItems; // 인벤토리에 존재하면서 장착한 효과 나타내는 item
    SLOT_LINEAR_LIST* m_InsertRuleBaseSlotList[nsSlot::InventoryConfig::kMaxNumberOfInsertRules];
    nsSlot::SearchOffset insert_rule_list_offset_[nsSlot::InventoryConfig::kMaxNumberOfInsertRules];

public:
    struct OptionEffectOperator;
    friend  struct OptionEffectOperator;
    friend class nsSlot::ItemFunctionToggle;
};

//--------------------------------------------------------------------------------------------------

//  <NOT USED> - ... 아 그랬지...
template< class STREAM_TYPE >
BOOL
CInventorySlotContainer::GetTabTotalInfo(BYTE tab_index, JUST_INVENTORY_TOTAL_INFO* OUT pTotalInfo)
{
    if(tab_index >= m_TabNum)
        return FALSE;

    pTotalInfo->m_Count = 0;

    JUST_INVENTORY_TOTAL_INFO::SLOT_TYPE* pSlot = pTotalInfo->m_Slot;
    POSTYPE start_pos = tab_index * MAX_INVENTORY_SLOT_PER_TAB;
    const POSTYPE upperbound = start_pos + MAX_INVENTORY_SLOT_PER_TAB;
    // type overflow 발생할 수 있는 처리지만, 어지간히 망가지지 않는 한 문제 발생 가능성이 낮으므로,
    // 넘어가기로 한다.

    BYTE count = 0;
    for(POSTYPE i = start_pos; i < upperbound; ++i)
    {
        if(!IsEmpty(i)) {
            JUST_INVENTORY_TOTAL_INFO::SLOT_TYPE& rSlot = pSlot[count];
            rSlot.m_Pos = i;
            SerializeItemStream(rSlot.m_Pos, &rSlot.m_Stream, SERIALIZE_LOAD);
            ++count;
        }
    }

    pTotalInfo->m_Count = count;
}

#endif //_GAMESERVER_INVENTORY_SLOTCONTAINER_H

