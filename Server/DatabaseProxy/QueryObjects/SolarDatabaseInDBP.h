#pragma once
#ifndef DBPROXY_SOLARDATABASE_H
#define DBPROXY_SOLARDATABASE_H

#include <SolarDatabaseDefine.h>
#include <SolarDatabase.h>
#include <QueryResult.h>

//==================================================================================================
// this class is implemented to support life-time management of the SolarDatabase module
// TODO: f101021.2L, need implement.

class SolarDatabaseInDBP
{
public:
    // CHANGES: f111011.1L, extend secondary pending queue bandwith
    // to solve bombing querying in China version.
    static const ulong kPendingPoolSize = 200000;
    //
    SolarDatabaseInDBP();
    ~SolarDatabaseInDBP();

    bool Initialize(const SolarDatabaseDesc& desc, bool use_pending_pool);
    void Update();
    void Release();

    bool DBQuery(Query* query);

    bool IsRemainsQueries() const;
    int GetBufferedQueryNum() const;
    // NOTE: f110628.1L, added the specialied busy update query counter to support a load banalcing
    int GetNumberOfPendingQueries() const;

    // work in critical section
    void SetConnectState(bool is_connected);
    bool IsConnected() const;
    //bool ReConnect(WORD db_index);
    //
private:
    bool UpdateInterQuery();
    //
    bool requested_release_;
    util::WxRingBuffer* query_list_; // dual buffer
    SolarDatabase solar_database_;
    //
    bool initialized_;
    long connected_; // critical section
};

//void SolarDatabaseInDBP::Release();
//inline bool SolarDatabaseInDBP::DBQuery(Query* query)

inline bool SolarDatabaseInDBP::IsRemainsQueries() const
{
    return solar_database_.IsRemainsQueries();
}

inline int SolarDatabaseInDBP::GetBufferedQueryNum() const
{
    return solar_database_.GetBufferedQueryNum();
}

// work in critical section
inline void SolarDatabaseInDBP::SetConnectState(bool is_connected)
{
    long connect_state = is_connected;
    InterlockedExchange(&connected_, connect_state);
}

inline bool SolarDatabaseInDBP::IsConnected() const
{
    long connected = connected_;
    return (connected != false);
}

#endif //DBPROXY_SOLARDATABASE_H