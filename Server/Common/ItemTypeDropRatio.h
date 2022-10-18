#ifndef _COMMON_ITEM_TYPE_DROP_RATIO_H
#define _COMMON_ITEM_TYPE_DROP_RATIO_H
#pragma once
//==================================================================================================
/// ItemTypeDropRatio class
/**
    @author
        Kim Min Wook < taiyo@webzen.com >
    @since
        2006. 4. 5 (식목일 왜 휴일이 아니냥$_$)
    @remarks
    -   몬스터 등급의 기준에 따라 확률 처리하여 ItemType의 인덱스를 넘겨주는 역할을 하는 클래스
    -   PopRandomItemTypeIndex()의 리턴값은 ItemTypeList의 FindItemType(RetVal)로 ItemType을 찾을 때 사용
    @note
    -   
    @history
    -   091003|waverix|code arrangement
    -   100401|waverix|added 'kConvertNpcGradeToIndex'
    -                  working '_NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION'
*/
//==================================================================================================

#include <ReliableRatioRandomizer.h>

//==================================================================================================

class ItemTypeDropRatio : public IParser
{
public:
    ItemTypeDropRatio(void);
    virtual ~ItemTypeDropRatio(void);

    // interfaces of IParser
    virtual BOOL LoadScript(eSCRIPT_CODE script_code, BOOL reloaded);
    virtual void Release();
    // NOTE: f110401.1L, added by _NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION
    void SetupDropRatioTable(bool selected_battlezone);
    //
    DWORD PopRandomItemTypeIndex(eNPC_GRADE npc_grade);

    void Unload();
    void Reload(); //unused
private:
    static const ulong kInitializedTag = 0xAADAF190;
    // reference : MAX_ITEM_TYPE_INDEX, "ItemTypeDropRatio.txt" number of rows
    enum { MAX_ITEM_TYPE_INDEX = 90, TYPE_SELECTOR_NUM = 2 };
    enum eIndices {
        eIndex_None,
        //
        eIndex_eNPC_GENERAL,
        eIndex_eNPC_ELITE,
        eIndex_eNPC_LEADER,
        eIndex_eNPC_MIDDLEBOSS,
        eIndex_eNPC_BOSS,
        eIndex_eNPC_LUCKY_MONSTER,
        //
        eIndex_Counts
    };
    typedef ReliableRatioRandomizer<MAX_ITEM_TYPE_INDEX> ItemTypeSelector;
    typedef ItemTypeSelector ItemTypeSelectorList[eIndex_Counts];
    //
    bool _Load(bool reloaded);
    // NOTE: f110401.1L
    bool _LoadRecord(const int row, const int index, const int standard_value,
                     ItemTypeSelectorList& item_type_selector_list);
    bool MakeInitialConvertTable();
    // fields...
    ulong initialized_tag_;
    bool selected_battlezone_drop_ratio_table_;
    ItemTypeSelectorList* selected_selector_;
    // NOTE: 'kConvertNpcGradeToIndex' convert the inputed npc grade to the selector index.
    static uint8_t kConvertNpcGradeToIndex[eNPC_GRADE_MAX];
    ItemTypeSelectorList item_type_selector_in_field_;
    ItemTypeSelectorList item_type_selector_in_battle_;
    //
    __DISABLE_COPY(ItemTypeDropRatio);
};

//==================================================================================================

inline DWORD
ItemTypeDropRatio::PopRandomItemTypeIndex(eNPC_GRADE npc_grade)
{
    const int selected_index \
        = (npc_grade < 0 || npc_grade >= eNPC_GRADE_MAX)
        ?   eIndex_None
        :   kConvertNpcGradeToIndex[npc_grade];
    //
    return (*selected_selector_)[selected_index].RandomizedKey();
}

//==================================================================================================

#endif //_COMMON_ITEM_TYPE_DROP_RATIO_H
