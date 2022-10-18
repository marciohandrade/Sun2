#pragma once
#ifndef DBPROXY_DBQUERYPROXY_BATTLERECORDS_NODE_H
#define DBPROXY_DBQUERYPROXY_BATTLERECORDS_NODE_H

class Query_BattleRecords_Select;
class Query_BattleRecords_Update;

namespace nsQuery {
;
struct BattleRecordsUpdateNode
{
    enum eRecordChange {
        eRecordChange_Default = 0,
        eRecordChange_Changed,      // update
        //eRecordChange_NewInsert,    // insert
    };
    //
    uint8_t record_changed_;
    QueryForCharTransaction* query_;
    BattleRecordsResultRecord commited_data_;
    // latest_data_ is refered to BASE_PLAYERINFO
    //
    void UpdateAndCompareData(const DBCharacter* db_character);
    //
};

typedef BattleRecordsUpdateNode BattleRecordsUpdateTable;

}; //end of namespace

#endif //DBPROXY_DBQUERYPROXY_BATTLERECORDS_NODE_H