#pragma once
#ifndef DBPROXY_CHILD_SERVER_SESSION_H
#define DBPROXY_CHILD_SERVER_SESSION_H

#include <ServerSession.h>
#include <ServerGlobal.h>

#include "QueryObjects/SolarDatabaseInDBP.h"
#include "DBUser.h"

//==================================================================================================
// CHANGES: f101118.1L, remove unused data fields and code rearrangement

class ChildServerSession : public ServerSession
                         , public ISession4StartServiceWorkNode
{
public:
    ChildServerSession();
    virtual ~ChildServerSession();

    virtual void Init();
    virtual void Release();
    virtual void Update();
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg) = 0;
    virtual void OnDisconnect();
    // NOTE: f110617.1L, don't called by root, use OnRedirect event.
    //virtual void OnAccept(DWORD index);
    //
    DWORD GetDBUnitID() const;
    void DBQuery(Query* query);
    void DBQuery(BYTE category, BYTE protocol, Query* query);
    // to support the ISession4StartServiceWorkNode interfaces
    virtual BOOL IsValidObject(DWORD check_value);
    virtual BOOL IsValidState(DWORD status_value);
    virtual BOOL SendPacket4Interface(MSG_BASE* pMsg, WORD wSize);

    // 이것은 WorkLoad에서 해당 세션에 대한 유효성을 판별하기 위해서 사용한다.
    void SetSyncKey(DWORD sync_value);
    DWORD GetSyncKey() const;

    WORK_LOAD_CONTAINER_SESSION_WORK& GetWorkLoadContainer();
    WorkLoadPacketStream& GetWorkLoadPacketStream();
    
protected:
    DWORD db_unit_id_;
private:
    DWORD sync_key_;
    WorkLoadPacketStream workload_packet_stream_;
    WORK_LOAD_CONTAINER_SESSION_WORK workload_;
};

//==================================================================================================
inline DWORD ChildServerSession::GetDBUnitID() const
{
    return db_unit_id_;
}

inline void ChildServerSession::SetSyncKey(DWORD sync_value) {
    sync_key_ = sync_value;
}

inline DWORD ChildServerSession::GetSyncKey() const {
    return sync_key_;
}

inline WORK_LOAD_CONTAINER_SESSION_WORK&
ChildServerSession::GetWorkLoadContainer() {
    return workload_;
}

inline WorkLoadPacketStream&
ChildServerSession::GetWorkLoadPacketStream() {
    return workload_packet_stream_;
}

//virtual
inline void ChildServerSession::Update() {
}

#endif //DBPROXY_CHILD_SERVER_SESSION_H