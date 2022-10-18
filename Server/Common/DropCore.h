#ifndef __COMMON_DROP_CORE_H
#define __COMMON_DROP_CORE_H
#pragma once

//==================================================================================================
/// DropCore class
/**
    @author
        Kim Min Wook < taiyo@webzen.com >
    @since
        2006. 4. 5 (식목일 왜 휴일이 아니냥$_$)
    @remarks
    -   
    @purpose
    -   아이템, 돈 드랍의 단순화
    -   밸런스를 컨트롤하기 쉬움
    @history
    -   091003|waverix|code arrangement
*/
//==================================================================================================

#include "ItemTypeDropRatio.h"
#include <SolarFileReader.h>

class DropItemObj;
class DropMoneyObj;
class SCItemSlot;
class ItemTypeList;
class DropRatioInfoList;

enum eDROP_TYPE_
{
    DROP_TYPE_NONE,
    DROP_TYPE_MONEY,
    DROP_TYPE_ITEM,
};

class DropCore : public IParser
{
public:
    DropCore(void);
    virtual ~DropCore(void);
    // CHANGES: f110401.3L, the 'result_number_of_drop_items' variable means
    // dropped_item_out * result_number_of_drop_items. basically the result of value is 1.
    BOOL Drop(eZONETYPE zone_type, DWORD drop_index, eNPC_GRADE npc_grade, float fBonusPercent,
              eDROP_TYPE_& OUT drop_type_out, MONEY& OUT dropped_money_out,
              SCItemSlot& OUT dropped_item_out, DWORD& OUT decided_seed_out,
              int* result_number_of_drop_items);

    void Reload();
    void Unload();

    virtual BOOL Load(eSCRIPT_CODE script_code, const TCHAR* file_name, BOOL bBigData,
                      eSEPERATOR_MODE mode, const TCHAR* pack_file_name, BOOL bReload);
    virtual BOOL SolarLoad(eSCRIPT_CODE script_code, BYTE* stream, DWORD dwSize, BOOL bBigData);
    virtual BOOL LoadScript(eSCRIPT_CODE /*script_code*/, BOOL /*bReload*/) { return FALSE; }
    virtual void Release();

    // NOTE: f110401.1L, added by _NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION
    void SetupDropRatioTable(bool selected_battlezone);
    //
    inline DropRatioInfoList* GetDropRatioInfoList() { return m_pDropRatioInfoList; }
    // CHANGES: f110401.3L, the 'result_number_of_drop_items' variable means
    // dropped_item_out * result_number_of_drop_items. basically the result of value is 1.
    BOOL DropItemFunc(DropItemObj* pObj, eNPC_GRADE npc_grade,
                      SCItemSlot& OUT dropped_item_out, int* result_number_of_drop_items);
    BOOL DropMoneyFunc(DropMoneyObj* pDropObj, eZONETYPE zone_type, float fBonusPercent,
                       eNPC_GRADE npc_grade, MONEY& OUT dropped_money_out, DWORD& OUT decided_seed_out);
private:
    bool selected_battlezone_;
    //
    ItemTypeList* m_pItemTypeList;
    DropRatioInfoList* m_pDropRatioInfoList;
    // 아이템타입 결정
    ItemTypeDropRatio m_ItemTypeDropRatioForMonster;
};

#endif //__COMMON_DROP_CORE_H
