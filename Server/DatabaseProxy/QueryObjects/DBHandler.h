#pragma once
#ifndef DBPROXY_DB_HANDLER_H
#define DBPROXY_DB_HANDLER_H

//==================================================================================================
//
#include <ObjKeyGenerator.h>

#include "./DBUnit.h"
class SolarDatabaseInDBP;
class ChildServerSession;

// _NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
namespace nsQuery {
;

struct DBConnectNode
{
    enum eIndex {
        eIndex_None = 0,
        eIndex_Shared,
        eIndex_Channel1, eIndex_Channel2, eIndex_Channel3, eIndex_Channel4,
        eIndex_Channel5, eIndex_Channel6, eIndex_Channel7, eIndex_Channel8, 
        eIndex_ChannelEnd,
        eIndex_User1 = eIndex_ChannelEnd,
        eIndex_User2, eIndex_User3, eIndex_User4,
        eIndex_User5,  eIndex_User6,  eIndex_User7,  eIndex_User8,
        // NOTE: f110628.1L, expand database connectors to support a load balancing int high-load
        eIndex_User9,  eIndex_User10, eIndex_User11, eIndex_User12,
        eIndex_User13, eIndex_User14, eIndex_User15, eIndex_User16,
        eIndex_Counts
    };
    uint8_t index_; // 1,2,...9
    bool link_agent_; //
    int number_of_linked_;
    SolarDatabaseInDBP* solar_database_;
};

}; //end of namespace

//==================================================================================================
//
class DBHandler
{
public:
    static const int kMaxNumberOfSpecializedConcurrentBusyUpdateQueries = 8000;
    //
    DBHandler();
    ~DBHandler();

    void Init(const char* database_name, const char* user_id, const char* password);
    void Release();

    bool DBQuery(DWORD db_unit_id, Query* query);
    bool Update();
    //
    ChildServerSession* GetSharedSession();

    DWORD AllocateDBUnit(eSERVER_TYPE server_type, DWORD session_index);
    void DeallocateDBUnit(DWORD db_unit_id);
    DWORD UpdateDBUnitForUser(DBUser* const db_user, DWORD db_unit_id);
    DBUnit* FindDBUnit(DWORD db_unit_id) const;
    //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
    void UpdateDBConnectorIndex(DWORD db_unit_id, int db_connector_index);
    // NOTE: f110628.1L, added the specialied busy update query counter to support a load banalcing
    // list = {
    //  S_ItemSlotInfo_Update, S_Char_State_(Update, Insert, Delete),
    //  S_Perk_(Update, Insert, Delete), S_SlotInfo_(Update, Insert),
    //  S_MissionInfo_(Update, Insert), S_ACSystem_Update
    // }
    // NOTE: currently, the db connection of a server session do not invoke too many queries.
    DWORD GetNumberOfDBConnectionQueuingQueries(DBUser* const db_user) const;

    void Display();
    DWORD GetQueuingQueryNum() const;

private:
    typedef util::CObjKeyGenerator<ulong> DBUnitIDGenerator;
    static const ulong kMaxNumberOfUnits = 30000;
    // 0:nothing, 1: shared:, 2~9:channel base, 10~25:user base
    static const int kNumberOfDBConnector = nsQuery::DBConnectNode::eIndex_Counts;
    static const int MAX_NAME_LEN = 128;
    //
    bool AlignEmptyChannelConnectorIndex(int* connector_index);
    bool AlignEmptyUserConnectorIndex(int* connector_index);
    bool AllocateSolarDatabase(SolarDatabaseInDBP** solar_database);
    static void CallbackDBResult(DWORD protocol, QueryResult* query_result);
    static void CallBackMessageSession(char* message);
    static void CallBackMessageSessionEx(WORD db_index, QueryResult* query_result);
    //
    bool initialized_;
    //
    char pod_field_start_[1];
    //
    DBUnitIDGenerator* id_generator_;
    nsQuery::DBConnectNode db_connectors_[kNumberOfDBConnector];
    //
    char database_name_[MAX_NAME_LEN];
    char user_id_[MAX_NAME_LEN];
    char password_[MAX_NAME_LEN];
    //
    DBUnit db_unit_array_[kMaxNumberOfUnits];
    //
    char pod_field_end_[1];
    //
    static ChildServerSession* ground_session_static;
};

//==================================================================================================

extern DBHandler g_DBHandler;

//==================================================================================================

inline DBUnit* DBHandler::FindDBUnit(DWORD db_unit_id) const
{
    if (db_unit_id == 0 || db_unit_id > _countof(db_unit_array_)) {
        return NULL;
    }
    const DBUnit* db_unit = &db_unit_array_[db_unit_id - 1];
    if (db_unit->db_unit_id_ != db_unit_id) {
        return NULL;
    }
    return const_cast<DBUnit*>(db_unit);
}

#endif //DBPROXY_DB_HANDLER_H