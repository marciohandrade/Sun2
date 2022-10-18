#pragma once
#ifndef DBPROXY_DBQUERYPROXY_QUESTNODE_H
#define DBPROXY_DBQUERYPROXY_QUESTNODE_H

struct MSG_DG_CHARINFO_QUEST_CMD;

class Query_Quest_Select;
class Query_Quest_Update;

namespace nsQuery {
;
struct QuestUpdateNode
{
    enum eRecordChange {
        eRecordChange_Default = 0,
        eRecordChange_Changed,      // update
        //eRecordChange_NewInsert,    // insert
    };
    //
    uint8_t record_changed_;
    QueryForCharTransaction* query_;
    QuestResultRecord commited_data_;
    QuestResultRecord latest_data_;
    //
    void UpdateAndCompareData();
};

typedef QuestUpdateNode QuestUpdateTable;

}; //end of namespace

#endif //DBPROXY_DBQUERYPROXY_QUESTNODE_H