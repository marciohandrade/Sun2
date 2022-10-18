#include "stdafx.h"
#include "./ChildServerSession.h"

#include <Protocol_ServerCommon.h>
#include <PacketStruct_ServerCommon.h>
#include <FileParser.h>

#include "QueryObjects/QueryPoolFactory.h"
#include "DBProxyServerEx.h"
#include "DBUserManager.h"
#include "QueryObjects/DBHandler.h"
#include "QueryObjects/DBUnit.h"

//==================================================================================================

ChildServerSession::ChildServerSession()
    : db_unit_id_(0)
    , sync_key_(0)
{
    // 세션 풀을 설정할 때만 수행된다.
    workload_packet_stream_.Initialize(WorkLoadPacketStream::c_DefaultUpdateTick,
                                       WorkLoadPacketStream::c_DefaultLimitTransferSize);
    workload_.Initialize();
}

ChildServerSession::~ChildServerSession()
{
    // 아마 세션 풀을 해제할 경우만 수행될 듯.
    workload_packet_stream_.Initialize(WorkLoadPacketStream::c_DefaultUpdateTick,
                                       WorkLoadPacketStream::c_DefaultLimitTransferSize);
    workload_.Release(true);
}

void ChildServerSession::Init()
{
    ServerSession::Init();
    SetSyncKey(TEMP_SERVER);
}

void ChildServerSession::Release()
{
    ServerSession::Release();
    SetSyncKey(TEMP_SERVER);
}

void ChildServerSession::OnDisconnect()
{
    ServerSession::OnDisconnect();

    g_DBHandler.DeallocateDBUnit(db_unit_id_);
    db_unit_id_ = 0;

    SetSyncKey(TEMP_SERVER);
    workload_packet_stream_.Initialize(WorkLoadPacketStream::c_DefaultUpdateTick,
                                       WorkLoadPacketStream::c_DefaultLimitTransferSize);
    // 이것은 모든 Work들을 제거하는 것이므로 올바른 위치에 사용되지 않은 것이다.
    //WorkLoadManager::ReleaseTaskAll();
    workload_.Release(true);
}

#if SUN_CODE_BACKUP
// NOTE: f110617.1L, don't called by root, use OnRedirect event.
void ChildServerSession::OnAccept(DWORD index)
{
    ServerSession::OnAccept(index);
    SUNLOG(eCRITICAL_LOG, _T("[OnAccept][%u] Server Accept(%s)"),
           index, GetServerType2String(GetServerType()));
}
#endif

void ChildServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(network_index, first_contact_msg);

    db_unit_id_ = g_DBHandler.AllocateDBUnit(GetServerType(), GetSessionIndex());
    ASSERT(0 != db_unit_id_);
    DBUnit* db_unit = g_DBHandler.FindDBUnit(db_unit_id_);
    ASSERT(db_unit);
    if(!db_unit)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%u:%s:%s] Can't allocate DB connection"),
               network_index, GetServerType2String(GetServerType()), this->GetConnectIP().c_str());
        NetworkObject::Disconnect(true);
        return;
    }
    ASSERT(g_DBHandler.FindDBUnit(db_unit_id_)->GetSessionIndex() == GetSessionIndex());

    SUNLOG(eCRITICAL_LOG, "[Redirect][%u] Server Redirect(%s)",
           GetSessionIndex(), GetServerType2String(GetServerType()));
}

void ChildServerSession::DBQuery(BYTE category, BYTE protocol, Query* query) 
{ 
    query->SetIndex(MAKEDWORD((WORD)category, (WORD)protocol));
    query->SetVoidObject(this);
    g_DBHandler.DBQuery(db_unit_id_, query);
}

void ChildServerSession::DBQuery(Query* query) 
{ 
    query->SetVoidObject(this);
    g_DBHandler.DBQuery(db_unit_id_, query);
}

BOOL ChildServerSession::IsValidObject(DWORD check_value)
{
    return sync_key_ == check_value;
}

BOOL ChildServerSession::IsValidState(DWORD status_value)
{
    DWORD status = (DWORD)g_DBProxyServer.GetPhaseStatus();
    return status == status_value;
}

BOOL ChildServerSession::SendPacket4Interface(MSG_BASE* pMsg, WORD wSize)
{
    return SendPacket(pMsg, wSize);
}

