#pragma once
#ifndef DBPROXY_DBQUERYPROXY_MISSIONNODE_H
#define DBPROXY_DBQUERYPROXY_MISSIONNODE_H

class Query_MissionInfo_Select;
class Query_MissionInfo_Insert;
class Query_MissionInfo_Update;

namespace nsQuery {
;
// temporary value
//#define MAX_MISSION_NUM_EX 50
struct MissionUpdateNode
{
    enum eRecordChange {
        eRecordChange_Default = 0,
        eRecordChange_Changed,      // update
        eRecordChange_NewInsert,    // insert
    };
    //
    uint8_t record_changed_;
    QueryForCharTransaction* query_;
    MissionResultRecord commited_data_;
    MissionResultRecord latest_data_;
    //
    void UpdateAndCompareData();
};

// specification of the MSG_DG_CHARINFO_QUEST_CMD::m_pMissionStream by f101130.2L
//               [00]     [01]     [02]   ...   [31]
// ~f101130.2L  UNUSED |MISSION1|MISSION2|...|MISSION31|
// f101130.2L~ MISSION1|MISSION2|MISSION3|...|MISSION32|
struct MissionUpdateTable
{
    MissionUpdateNode records_[MAX_MISSION_NUM];
};

}; //end of namespace

#endif //DBPROXY_DBQUERYPROXY_MISSIONNODE_H