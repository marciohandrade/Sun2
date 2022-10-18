#pragma once
#ifndef DBPROXY_DBQUERYPROXY_SLOTNODE_H
#define DBPROXY_DBQUERYPROXY_SLOTNODE_H

class Query_SlotInfo_Select;
class Query_SlotInfo_Insert;
class Query_SlotInfo_Update;

namespace nsQuery {
;
struct SlotUpdateNode
{
    enum eRecordChange {
        eRecordChange_Default = 0,
        eRecordChange_Changed,      // update
        eRecordChange_NewInsert,    // insert
    };
    //
    uint8_t record_changed_;
    QueryForCharTransaction* query_;
    SlotResultRecord commited_data_;
    SlotResultRecord latest_data_;
    //
    void UpdateAndCompareData();
};

struct SkillSlotUpdateTable
{
    static const SLOTIDX kSlotIndex = SI_SKILL;
    static const int kNumberOfTabs = (MAX_SKILL_SLOT_NUM / SlotResultRecord::kMaxNumberOfSlots) +
        ((MAX_SKILL_SLOT_NUM & (SlotResultRecord::kMaxNumberOfSlots - 1)) ? 1 : 0);
    //
    SlotUpdateNode records_[kNumberOfTabs];
};

struct QuickSlotUpdateTable
{
    static const SLOTIDX kSlotIndex = SI_QUICK;
    static const int kNumberOfTabs = (MAX_QUICK_SLOT_NUM / SlotResultRecord::kMaxNumberOfSlots) +
        ((MAX_QUICK_SLOT_NUM & (SlotResultRecord::kMaxNumberOfSlots - 1)) ? 1 : 0);
    //
    SlotUpdateNode records_[kNumberOfTabs];
};

}; //end of namespace

#endif //DBPROXY_DBQUERYPROXY_SLOTNODE_H