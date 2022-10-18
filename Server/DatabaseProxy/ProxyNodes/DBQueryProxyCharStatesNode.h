#pragma once
#ifndef DBPROXY_DBQUERYPROXY_CHARSTATE_NODE_H
#define DBPROXY_DBQUERYPROXY_CHARSTATE_NODE_H

class Query_Char_State_Select;
class Query_Char_State_Insert;
class Query_Char_State_Delete;
class Query_Char_State_Update;
//STATE_DETAIL_TOTAL_INFO;

namespace nsQuery {
;

struct CharStateUpdateNode
{
    enum eRecordChange {
        eRecordChange_Default = 0,
        eRecordChange_Changed,      // update
        eRecordChange_NewInsert,    // insert
        eRecordChange_NeedDelete,   // delete
    };
    //
    uint8_t record_changed_;
    uint32_t store_touch_; // added a temporary field to check a deletion state and that.
    QueryForCharTransaction* query_;
    CharStateResultRecord commited_data_;
    CharStateResultRecord latest_data_;
    void ChangeData(const STATE_DETAIL_INFO& inputed_data);
    void UpdateAndCompareData();
};

}; //end of namespace

#endif //DBPROXY_DBQUERYPROXY_CHARSTATE_NODE_H