#pragma once
#ifndef DBPROXY_DBQUERYPROXY_PERKNODE_H
#define DBPROXY_DBQUERYPROXY_PERKNODE_H

class Query_Perk_Select;
class Query_Perk_Insert;
class Query_Perk_Delete;
class Query_Perk_Update;

namespace nsQuery {
;

struct PerkUpdateNode
{
    enum eRecordChange {
        eRecordChange_Default = 0,
        eRecordChange_Changed,      // update
        eRecordChange_NewInsert,    // insert
        eRecordChange_NeedDelete,   // delete
    };
    //
    uint8_t record_changed_;
    QueryForCharTransaction* query_;
    PerkResultRecord commited_data_;
    PerkResultRecord latest_data_;
    void ChangeData(const PerkData& inputed_data);
    void UpdateAndCompareData();
};


}; //end of namespace

#endif //DBPROXY_DBQUERYPROXY_PERKNODE_H