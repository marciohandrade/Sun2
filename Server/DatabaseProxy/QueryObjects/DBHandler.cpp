#include "stdafx.h"
#include "./DBHandler.h"
#include "./DBUnit.h"
#include "PacketHandler/PacketHandler.h"

//==================================================================================================

DBHandler g_DBHandler;
//
ChildServerSession* DBHandler::ground_session_static = NULL;

//==================================================================================================
// CHANGES: f101118.1L, added ground session handling
namespace util { namespace internal {
;
struct GroundSession : public ChildServerSession
{
    GroundSession();
    virtual ~GroundSession();
    // ServerSession interface
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
    //
    void BindDBUnit();
    void UnbindDBUnit();

    bool binded_;
};

inline GroundSession::GroundSession()
    : binded_(false)
{
}

GroundSession::~GroundSession()
{
}

void GroundSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    __UNUSED((network_index, first_contact_msg));
}

void GroundSession::BindDBUnit()
{
    //                                               session_index
    db_unit_id_ = g_DBHandler.AllocateDBUnit(TEMP_SERVER, 0);
    assert(db_unit_id_ != 0);
    binded_ = true;
}

void GroundSession::UnbindDBUnit()
{
    // NOTE: f101118.1L, nothing to do because this object share DBHandler
}

}}; //end of namespace

//==================================================================================================

DBHandler::DBHandler()
    : initialized_(false)
{
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
}

DBHandler::~DBHandler()
{
    Release();
}

//==================================================================================================

void 
DBHandler::Init(const char* database_name, const char* user_id, const char* password)
{
    if (!FlowControl::FCAssert(initialized_ == false)) {
        __debugbreak();
        return;
    };
    //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
    id_generator_ = new DBUnitIDGenerator;
    id_generator_->Create(1, _countof(db_unit_array_));
    ZeroMemory(db_connectors_, sizeof(db_connectors_));
    ;{
        strncpy(database_name_, database_name, _countof(database_name_));
        database_name_[_countof(database_name_) - 1] = '\0';
        strncpy(user_id_, user_id, _countof(user_id_));
        user_id_[_countof(user_id_) - 1] = '\0';
        strncpy(password_, password, _countof(password_));
        password_[_countof(password_) - 1] = '\0';
    };
    //
    ground_session_static = new util::internal::GroundSession;
    ground_session_static->Init();
    initialized_ = true;
}

void 
DBHandler::Release()
{
    ;{  //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
        const nsQuery::DBConnectNode* it = db_connectors_,
                                    * end = &db_connectors_[_countof(db_connectors_)];
        for ( ; it != end; ++it)
        {
            SolarDatabaseInDBP* solar_database = it->solar_database_;
            if (solar_database == NULL) {
                continue;
            };
            solar_database->Release();
            delete solar_database;
        }
        ZeroMemory(db_connectors_, sizeof(db_connectors_));
    };
    if (ground_session_static != NULL)
    {
        ground_session_static->Release();
        SAFE_DELETE(ground_session_static);
    }
    //
    SAFE_DELETE(id_generator_);
    //
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
    //
    initialized_ = false;
}

bool 
DBHandler::DBQuery(DWORD db_unit_id, Query* query)
{
    DBUnit* const db_unit = FindDBUnit(db_unit_id);
    if (db_unit == NULL)
    {
    #ifdef __NA000000_080709_DBQUERY_FAULT_DETECTION__
        SUNLOG(eCRITICAL_LOG, _T("--Fail DBQuery key %u/tpQuery %s"), db_unit_id, query->GetQuery());
    #endif
        return false;
    };
    //
    int connector_index = db_unit->GetDBConnectorIndex();
    if (!FlowControl::FCAssert(connector_index > 0 &&
                               connector_index < _countof(db_connectors_)))
    {
        return false;
    }
    const nsQuery::DBConnectNode& db_connector = db_connectors_[connector_index];
    assert(db_connector.solar_database_);
    // DBUnit::DBQuery replacement
    ++db_unit->number_of_queries_;
    query->SetVoidObject(db_unit);
    bool query_success = db_connector.solar_database_->DBQuery(query);
    if (FlowControl::FCAssert(query_success) == false) {
        --db_unit->number_of_queries_;
    }
    return query_success;
}

bool DBHandler::AlignEmptyChannelConnectorIndex(int* connector_index)
{
    nsQuery::DBConnectNode* it = &db_connectors_[db_connectors_->eIndex_Channel1],
                          * const end = &db_connectors_[db_connectors_->eIndex_ChannelEnd];
    for ( ; it != end; ++it)
    {
        if (it->link_agent_ != false) {
            continue;
        }
        break;
    }

    if (it == end) {
        // full
        *connector_index = db_connectors_->eIndex_None;
        return false;
    };
    //
    SolarDatabaseInDBP* solar_database = NULL;
    if (AllocateSolarDatabase(&solar_database) == false)
    {
        SUNLOG(eCRITICAL_LOG, "solar_database_->Initialize()IsFailed");
        *connector_index = db_connectors_->eIndex_None;
        return false;
    }
    //
    it->index_ = static_cast<uint8_t>(it - db_connectors_); // equals enumeration value
    it->link_agent_ = true;
    it->solar_database_ = solar_database;
    *connector_index = it->index_;
    return true;
}

bool DBHandler::AlignEmptyUserConnectorIndex(int* connector_index)
{
    // find node that consider load balance
    nsQuery::DBConnectNode* it = &db_connectors_[db_connectors_->eIndex_User1],
                          * const end = &db_connectors_[db_connectors_->eIndex_Counts];
    nsQuery::DBConnectNode* lowest_node = it;
    for ( ; it != end; ++it)
    {
        if (lowest_node->number_of_linked_ > it->number_of_linked_) {
            lowest_node = it;
        }
    }
    it = lowest_node;
    //
    if (it->solar_database_ == NULL)
    {
        SolarDatabaseInDBP* solar_database = NULL;
        if (AllocateSolarDatabase(&solar_database) == false)
        {
            SUNLOG(eCRITICAL_LOG, "solar_database_->Initialize()IsFailed");
            *connector_index = db_connectors_->eIndex_None;
            return false;
        }
        it->index_ = static_cast<uint8_t>(it - db_connectors_); // equals enumeration value
        it->solar_database_ = solar_database;
    };
    //
    ++it->number_of_linked_;
    *connector_index = it->index_;
    return true;
}


bool DBHandler::AllocateSolarDatabase(SolarDatabaseInDBP** result_solar_database)
{
    SolarDatabaseInDBP* solar_database = new SolarDatabaseInDBP;
    SolarDatabaseDesc db_desc;
    ;{
        db_desc.database_module_type = DBCInterfaceType_ODBC;
        db_desc.query_pool_size      = 5000;
        db_desc.query_processor_num  = 1;
        db_desc.database_ip   = "";
        db_desc.database_name = const_cast<char*>(database_name_);
        db_desc.user_name     = const_cast<char*>(user_id_);
        db_desc.user_password = const_cast<char*>(password_);
        db_desc.fnptr_error_message    = &DBHandler::CallBackMessageSession;
        db_desc.fnptr_query_result     = &DBHandler::CallbackDBResult;
        db_desc.fnptr_error_message_ex = &DBHandler::CallBackMessageSessionEx;
    };
    //
    bool success = solar_database->Initialize(db_desc, true);
    if (success == false) {
        *result_solar_database = NULL;
        solar_database->Release();
        delete solar_database;
        return false;
    }
    *result_solar_database = solar_database;
    return true;
}

ChildServerSession* DBHandler::GetSharedSession()
{
    using util::internal::GroundSession;
    GroundSession* ground_session = static_cast<GroundSession*>(ground_session_static);
    assert(ground_session);

    if (ground_session->binded_) {
        return ground_session;
    }
    ground_session->BindDBUnit();
    return ground_session;
};

// 커넥션 연결시
DWORD DBHandler::AllocateDBUnit(eSERVER_TYPE server_type, DWORD session_index)
{
    const DWORD kNullHandleID = 0;
    //
    BOOST_STATIC_ASSERT(db_connectors_->eIndex_Counts == kNumberOfDBConnector);
    DWORD db_unit_id = id_generator_->GetKey();
    if (db_unit_id == kNullHandleID) {
        return kNullHandleID;
    };
    //
    int db_connector_index = db_connectors_->eIndex_None;
    ;{
        switch (server_type)
        {
        case AGENT_SERVER:
            if (AlignEmptyChannelConnectorIndex(&db_connector_index) == false) {
                return kNullHandleID;
            }
            break;
        case GAME_SERVER:
        case FIELD_SERVER:
        case BATTLE_SERVER:
            // associate agentserver
            break;
        default:
            {
                db_connector_index = db_connectors_->eIndex_Shared;
                if (db_connectors_[db_connector_index].solar_database_ == NULL)
                {
                    SolarDatabaseInDBP* solar_database = NULL;
                    if (AllocateSolarDatabase(&solar_database) == false)
                    {
                        SUNLOG(eCRITICAL_LOG,
                               _T("|["__FUNCTION__"]|Msg=can't allocate solardatabase"));
                        return kNullHandleID;
                    }
                    db_connectors_[db_connector_index].solar_database_ = solar_database;
                }
            };
            break;
        };
    };
    DBUnit* const db_unit = &db_unit_array_[db_unit_id - 1];
    if (db_unit->Init(db_unit_id, db_connector_index, server_type, session_index) == false) {
        return kNullHandleID;
    };
    //
    return db_unit_id;
    //
}

// 커넥션 해제시
void 
DBHandler::DeallocateDBUnit(DWORD db_unit_id)
{
    DBUnit* const db_unit = FindDBUnit(db_unit_id);
    if (!FlowControl::FCAssert(db_unit)) {
        return;
    };
    //
    const int connector_index = db_unit->db_connector_index_;
    const eSERVER_TYPE server_type = static_cast<eSERVER_TYPE>(db_unit->server_type_);
    //
    if (db_unit->RequestRelease()) {
        id_generator_->RestoreKey(db_unit_id);
    };
    // garbage data = { db_unit };
    //
    if (server_type == GAME_SERVER)
    {
        // not associated with agent
        if (connector_index == 0) {
            return;
        }
    }
    else
    {
        if (!FlowControl::FCAssert(connector_index > 0 &&
                                   connector_index < db_connectors_->eIndex_ChannelEnd))
        {
            return;
        };
    };
    //
    nsQuery::DBConnectNode* const db_connector = &db_connectors_[connector_index];
    if (server_type == AGENT_SERVER)
    {
        db_connector->link_agent_ = false;
        // will be changed related with game servers (like that field, battle) on reconnect agent
    };
    //
    return;
}

void DBHandler::UpdateDBConnectorIndex(DWORD db_unit_id, int db_connector_index)
{
    DBUnit* const db_unit = FindDBUnit(db_unit_id);
    if (!FlowControl::FCAssert(db_unit)) {
        return;
    };
    if (db_connector_index == 0) {
        return;
    };
    const int db_connector_index_in_dbunit = db_unit->db_connector_index_;
    if (db_connector_index_in_dbunit != 0 && db_connector_index_in_dbunit == db_connector_index) {
        return;
    }
    if (!FlowControl::FCAssert(db_connector_index >= db_connectors_->eIndex_Channel1 &&
                               db_connector_index < _countof(db_connectors_)))
    {
        return;
    }
    db_unit->db_connector_index_ = static_cast<uint8_t>(db_connector_index);
}

DWORD DBHandler::UpdateDBUnitForUser(DBUser* const db_user, DWORD db_unit_id)
{
    const DWORD kNullHandleID = 0;
    ChildServerSession* const server_session = db_user->GetServerSession();
    DWORD session_index = server_session ? server_session->GetSessionIndex() : 0;
    //
    enum { eUserState_Clear, eUserState_New, eUserState_Change, eUserState_Close };
    const int user_state = (session_index && db_unit_id)   ? eUserState_Change
                         : (session_index)                 ? eUserState_New
                         : (db_unit_id)                    ? eUserState_Close
                         :                                   eUserState_Clear;
    //
    if (user_state == eUserState_Clear) {
        return kNullHandleID;
    };
    //
    DBUnit* db_unit = NULL;
    //
    if (user_state == eUserState_Change)
    {
        db_unit = FindDBUnit(db_unit_id);
        db_unit->session_index_ = session_index;
        db_unit->server_type_ = static_cast<uint8_t>(server_session->GetServerType());
        return db_unit_id;
    };
    //
    int db_connector_index = db_connectors_->eIndex_None;
    if (user_state == eUserState_Close)
    {
        db_unit = FindDBUnit(db_unit_id);
        db_connector_index = db_unit->db_connector_index_;
        if (db_unit->RequestRelease()) {
            if (FlowControl::FCAssert(db_connector_index >= db_connectors_->eIndex_User1 &&
                                      db_connector_index < db_connectors_->eIndex_Counts))
            {
                if (db_connectors_[db_connector_index].number_of_linked_) {
                    --db_connectors_[db_connector_index].number_of_linked_;
                }
            }
            id_generator_->RestoreKey(db_unit_id);
        };
        return kNullHandleID;
    };
    //
    if (user_state == eUserState_New)
    {
        db_unit_id = id_generator_->GetKey();
        if (FlowControl::FCAssert(db_unit_id != kNullHandleID) == false) {
            return kNullHandleID;
        }
        db_unit = &db_unit_array_[db_unit_id - 1];
        // TODO: alloc new db connector
        if (AlignEmptyUserConnectorIndex(&db_connector_index) == false) {
            id_generator_->RestoreKey(db_unit_id);
            return kNullHandleID;
        }
        db_unit->Init(db_unit_id, db_connector_index,
                      server_session->GetServerType(), session_index);
    };
    return db_unit_id;
}

bool DBHandler::Update()
{
    bool disconnected_or_condition = false;
    const nsQuery::DBConnectNode* it = db_connectors_,
                                * end = &db_connectors_[_countof(db_connectors_)];
    for ( ; it != end; ++it)
    {
        SolarDatabaseInDBP* solar_database = it->solar_database_;
        if (solar_database == NULL) {
            continue;
        };
        solar_database->Update();
        // NOTE: f110427.5L, do shutdown process if a handler disconnected with odbc.
        if (solar_database->IsConnected() == false) {
            disconnected_or_condition = true;
        };
    }
    return (disconnected_or_condition == false);
}

void 
DBHandler::Display()
{
    SUNLOG(eCRITICAL_LOG, _T("Remains DB Unit Resources : %d"), id_generator_->GetSize());
}

DWORD 
DBHandler::GetQueuingQueryNum() const
{
    DWORD number_of_total_queueing_queries = 0;
    const nsQuery::DBConnectNode* it = db_connectors_,
                                * end = &db_connectors_[_countof(db_connectors_)];
    for ( ; it != end; ++it)
    {
        SolarDatabaseInDBP* solar_database = it->solar_database_;
        if (solar_database == NULL) {
            continue;
        };
        number_of_total_queueing_queries += solar_database->GetBufferedQueryNum();
    }
    return number_of_total_queueing_queries;
};

// NOTE: f110628.1L, added the specialied busy update query counter to support a load banalcing
DWORD DBHandler::GetNumberOfDBConnectionQueuingQueries(DBUser* const db_user) const
{
    DWORD db_unit_id = db_user->db_unit_id();
    DBUnit* const db_unit = this->FindDBUnit(db_unit_id);
    if (db_unit == NULL) {
        assert(!"invalid db unit id");
        return 0;
    };

    const int db_connector_index_in_dbunit = db_unit->db_connector_index_;
    if (!FlowControl::FCAssert(db_connector_index_in_dbunit >= db_connectors_->eIndex_User1 &&
                               db_connector_index_in_dbunit < _countof(db_connectors_)))
    {
        return 0;
    }
    int number_of_pending_queries = db_connectors_[\
        db_connector_index_in_dbunit].solar_database_->GetNumberOfPendingQueries();
    return number_of_pending_queries;
}

//==================================================================================================
// callback

void DBHandler::CallbackDBResult(DWORD protocol, QueryResult* query_result)
{
    // same context as logic thread
    DBUnit* const db_unit = reinterpret_cast<DBUnit*>(query_result->GetVoidObject());
    DBHandler* const this_ = &g_DBHandler;
    //
    ;{
        const DWORD delay_time = query_result->GetQueryTime();
        if (delay_time > 1000)
        {
            SUNLOG(eCRITICAL_LOG, "QueryHighDelay:[%u=%usec]%s",
                   delay_time, delay_time / 1000, query_result->GetQuery());
        }
    };
    //
    ChildServerSession* server_session = \
        static_cast<ChildServerSession*>(g_DBProxyServer.FindSession(db_unit->session_index_));
    //
    MSG_DBPROXY_RESULT result_msg;
    ;{
        result_msg.m_byCategory = static_cast<BYTE>(LOWORD(protocol));
        result_msg.m_byProtocol = static_cast<BYTE>(HIWORD(protocol));
        result_msg.m_pData      = query_result;
    };
    //
    if (!server_session) {
        server_session = this_->ground_session_static;
    };
    //
    PacketHandler::Instance()->ParsePacket(PACKET_HANDLER_DBR,
                                           server_session, &result_msg, sizeof(result_msg));
    //
    if (db_unit->number_of_queries_) {
        --db_unit->number_of_queries_;
    }
    if (db_unit->number_of_queries_ == 0 &&
        db_unit->unit_state_.value == db_unit->unit_state_.eState_Closing)
    {
        uint16_t db_unit_id = db_unit->db_unit_id_;
        int db_connector_index = db_unit->db_connector_index_;
        if (FlowControl::FCAssert(db_connector_index > this_->db_connectors_->eIndex_None &&
                                  db_connector_index < this_->db_connectors_->eIndex_Counts))
        {
            nsQuery::DBConnectNode* const db_connector = &this_->db_connectors_[db_connector_index];
            if (db_connector->number_of_linked_) {
                --db_connector->number_of_linked_;
            }
        }
        db_unit->RequestRelease();

        this_->id_generator_->RestoreKey(db_unit_id);
        return;
    }
}

void DBHandler::CallBackMessageSession(char* message)
{
    // diff with logic thread context
    SUNCONNLOG(eCRITICAL_LOG, "[DBMessage]:%s", message);
}

void DBHandler::CallBackMessageSessionEx(WORD db_index, QueryResult* query_result)
{
    // diff with logic thread context
    DBHandler* const this_ = &g_DBHandler;
    DBUnit* db_unit = reinterpret_cast<DBUnit*>(query_result->GetVoidObject());
    const char* sqlerror_msg = query_result->GetErrorMsg();
    const char* sqlstate_msg = query_result->GetSqlState();
    SUNCONNLOG(eCRITICAL_LOG, "[Index:%u,DBIndex:%u]:%s,SQLSTATE:%s,NativeError:%u,%s", 
               db_unit->GetSessionIndex(),
               db_index, sqlerror_msg, sqlstate_msg, query_result->GetNativeError(),
               query_result->GetQuery());
    //
    const char* found = strstr(sqlerror_msg, "[DBNETLIB]");
    if (found != NULL || (strncmp(sqlstate_msg, "08S01", 5) == 0))
    {
        SolarDatabaseInDBP* solar_database = \
            this_->db_connectors_[db_unit->db_connector_index_].solar_database_;
        solar_database->SetConnectState(false);
    }
}

