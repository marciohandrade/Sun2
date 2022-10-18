#pragma once
#ifndef DBPROXY_DB_UNIT_H
#define DBPROXY_DB_UNIT_H

class QueryResult;
class DBHandler;
class SolarDatabaseInDBP;

//==================================================================================================

class DBUnit
{
public:
    struct eState {
        enum {
            eState_None, eState_Init, eState_Closing, eState_Counts
        };
        typedef uint8_t value_type;
        value_type value;
    };
    DBUnit();
    ~DBUnit();

    bool Init(DWORD db_unit_id, int db_connector_index,
              eSERVER_TYPE server_type, DWORD session_index);
    bool RequestRelease();
    int GetDBConnectorIndex() const;
    DWORD GetKey() const;
    DWORD GetSessionIndex() const;
    void  SetSessionIndex(DWORD session_index);

    eSERVER_TYPE GetServerType() const;
    void  SetServerType(eSERVER_TYPE server_type);
private:
    char pod_field_start_[1];
    //
    eState unit_state_;
    uint16_t db_unit_id_;
    uint8_t db_connector_index_; //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
    uint8_t server_type_;
    DWORD session_index_;
    int number_of_queries_;
    //
    long db_connected_;
    //
    char pod_field_end_[1];
    //
    friend class DBHandler;
};

//==================================================================================================

inline int DBUnit::GetDBConnectorIndex() const
{
    return db_connector_index_;
}

inline DWORD DBUnit::GetKey() const
{
    return db_unit_id_;
}

inline DWORD DBUnit::GetSessionIndex() const
{
    return session_index_;
}

inline void DBUnit::SetSessionIndex(DWORD session_index)
{
    session_index_ = session_index;
}

inline eSERVER_TYPE DBUnit::GetServerType() const
{
    return static_cast<eSERVER_TYPE>(server_type_);
}

inline void DBUnit::SetServerType(eSERVER_TYPE server_type)
{
    server_type_ = static_cast<uint8_t>(server_type);
}

#endif //DBPROXY_DB_UNIT_H