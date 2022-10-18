#include "StdAfx.h"
#include "./DBUser.h"

#include <Struct.h>
#include <StructInPacket.h>
#include <Misc.h>
#include <SerializeStream.h>
#include <VersionControl.h>

#include <ServerStruct.h>
#include <PacketStruct_ServerCommon.h>
#include <PacketStruct_AD.h>
#include <PacketStruct_DG.h>
#include <PacketStruct_DW.h>

#include <ServerSession.h>
#include <WayPoint.h>

#include "QueryObjects/StructInQuery.h"
#include "PacketHandler/Handler_Common.h"
#include "DBProxyServerEx.h"
#include "DBUserManager.h"
#include "Services/GuildManagerInDBP.h"
#include "ProxyNodes/DBQueryProxyManager.h"
#include "QueryObjects/DBHandler.h"
#include "QueryObjects/DBUnit.h"


DBUser::DBUser(void) : is_released_(false), conn_char_guid_(0)
{
    BOOST_STATIC_ASSERT(eDB_STATE_NONE == 0);
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
    destroy_wait_timer_.Clear();
    transaction_wait_timer_.Clear();
#ifdef _NA_002050_20110216_ADD_GENDER
    char_slot_size_ = MaxCharListConfig::kDefaultSlotSize;
#endif
}

DBUser::~DBUser(void)
{
    conn_char_guid_ = 0;
    for (CHAR_HASH::const_iterator it = character_list_hash_.begin(); 
        it != character_list_hash_.end(); ++it)
    {
        DBCharacter* db_character = it->second;
        if (impl::IsValidCharacter(db_character, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }
        ReleaseChar(db_character);
    }
    is_released_ = true;
}

void DBUser::Clear()
{
#ifdef _NA_002050_20110216_ADD_GENDER
    char_slot_size_ = MaxCharListConfig::kDefaultSlotSize;
#endif
    for (CHAR_HASH::const_iterator it = character_list_hash_.begin(); 
        it != character_list_hash_.end(); ++it)
    {
        DBCharacter* db_character = it->second;
        if (impl::IsValidCharacter(db_character, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }
        ReleaseChar(db_character);
    }
    character_list_hash_.clear();
    db_query_node_.Init(NULL);
    ;{
        ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
    };
    destroy_wait_timer_.Clear();
    transaction_wait_timer_.Clear();
    // NOTE: f110429.4L, a dangling user occur possibility
    // when he moves a field server to a battle server
    UpdateLatestUpdateTick();
    conn_char_guid_ = 0;
}

void DBUser::Init()
{
    Clear();
    db_query_node_.Init(this);
    is_released_ = false;
}

void DBUser::Release()
{
    // NOTE: f101128.1L, call method to invoke a dbunitid release event
    if (server_session_) {
        SetServerSession(NULL);
    }
    Clear();
    is_released_ = true;
}

DBCharacter* DBUser::FindCharacter(const CHARGUID char_guid) const
{
    CHAR_HASH& char_list = const_cast<CHAR_HASH&>(character_list_hash_);
    CHAR_HASH::iterator it = char_list.find(char_guid);
    if (it != char_list.end()) 
    {
        DBCharacter* const db_character = it->second;
        if (impl::IsValidCharacter(db_character, __FUNCTION__, __LINE__) == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|DBCharacter is already released.|CharGuid = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                char_guid
            );
            char_list.erase(it);
            return NULL;
        }
        return db_character;
    }
    return NULL;
}

bool DBUser::Update(BOOL /*is_by_warehouse*/)
{
    if (db_state_ == eDB_STATE_DESTROY_WAIT) 
    {
        // destroy pending
        return false;
    }

    db_query_node_.Update();

    DBCharacter* const conn_char = FindCharacter(conn_char_guid_);
    if (conn_char != NULL)
    {
        conn_char->Updatea();
    }

    // NOTE: f110429.4L, a dangling user occur possibility
    // when he moves a field server to a battle server
    if (IsExpiredDanglingTimeout())
    {
        db_query_node_.CancelRequest();
        LeaveChannelTransaction();
        return false;
    }

    return true;
}

// NOTE: f101126.2L, add query handler based on user
bool DBUser::DBQuery(BYTE category, BYTE protocol, Query* query)
{
    query->SetIndex(MAKEDWORD((WORD)category, (WORD)protocol));
    //query->SetVoidObject(this);
    bool success = g_DBHandler.DBQuery(db_unit_id_, query);
    assert(success);
    return success;
}

void DBUser::SetServerSession(ChildServerSession* server_session)
{
    server_session_ = server_session;
    db_unit_id_ = g_DBHandler.UpdateDBUnitForUser(this, db_unit_id_);
}

//캐릭터가 접속해서 DB에서 캐릭터를 가져 왔을 때
DBCharacter* DBUser::OnAddChar(const sQUERY_CHARACTER* queried_char_info)
{
    if (!FlowControl::FCAssert(FindCharacter(queried_char_info->m_CharGuid) == NULL)) 
    {
        return NULL;
    }
    
    DBCharacter* db_character = g_DBUserManager.CreateCharacter();
    db_character->OnInit(this);

    if (db_character->OnSetInfo(queried_char_info) == false)
    {
        ReleaseChar(db_character);
        return db_character;
    }
    else if (user_guid_ != db_character->GetInfo()->m_UserGuid)
    {
        ReleaseChar(db_character);
        return db_character;
    }

#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
	db_character->SetGameChuUSN(GetGameChuUSN());
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM
    db_character->SetBetakey(GetBetaKey());
#endif

    character_list_hash_[queried_char_info->m_CharGuid] = db_character;

    return db_character;
}

// 캐릭터가 삭제됐을 때
bool DBUser::OnDelChar(CHARGUID char_guid)
{
    if (char_guid == conn_char_guid_)
    {
        conn_char_guid_ = 0;
    }

    DBCharacter* db_character = FindCharacter(char_guid);
    if (db_character == NULL)
    {
        return false;
    }

    if (db_character->GetLV() >= CHAR_DESTROY_LEVEL)
    {
        db_character->OnDelete();
    }
    else
    {
        character_list_hash_.erase(char_guid);
        ReleaseChar(db_character);
    }

    return true;
}

DBCharacter* DBUser::OnRecoverChar(CHARGUID char_guid)
{
    DBCharacter* db_character = FindCharacter(char_guid);
    if (!FlowControl::FCAssert(db_character != NULL)) {
        return NULL;
    }
    db_character->OnRecover();

    return db_character;
}


bool DBUser::OnConnectChar(const CHARGUID char_guid)
{
    DBCharacter* const db_char = FindCharacter(char_guid);
    if (db_char == NULL)
    {
        return false;
    }
    conn_char_guid_ = char_guid;
    db_char->OnConnect();
    return true;
}

bool DBUser::OnDisconnectChar()
{
    DBCharacter* const conn_char = FindCharacter(conn_char_guid_);
    if (conn_char == NULL)
    {
        return false;
    }
    conn_char->OnDisconnect();
    conn_char_guid_ = 0;
    return true;
}


bool DBUser::SendPacketToGameServer(MSG_BASE_FORWARD* msg, WORD msg_size)
{
    if (FindCharacter(conn_char_guid_) == NULL)
    {
        return false;
    }
    if (server_session_ == NULL)
    {
        return false;
    }
    ASSERT(server_session_->GetServerType() == GAME_SERVER);
    msg->m_dwKey = user_key_;
    return server_session_->SendPacket(msg, msg_size) != false;
}

void DBUser::SetWorldType(ITEMPART& OUT rPart)
{
    // db<->gs
    if (rPart.byType > 200) {
        rPart.byType = g_DBProxyServer.GetItemWorldID();
    }
}

void DBUser::SendAllDataToGameServer()
{
    DBCharacter* const conn_char = FindCharacter(conn_char_guid_);
    if (conn_char == NULL)
    {
        return;
    }
    conn_char->SendAllDataToGameServer();
}

void DBUser::Disconnect(BYTE channel_id)
{
    // NOTE: f110428.7L, DBUserManager::Update call this method only
    UpdateChar(eDB_UPDATE_DISCONNECT_CMD, channel_id, NULL);
    nsQuery::DBQueryProxyManager* db_query_proxy_manager = nsQuery::DBQueryProxyManager::Instance();
    db_query_proxy_manager->DisconnectUser(this, true);
}

void DBUser::RequestDisconnect(int channel_id)
{
    // NOTE: f110426.2L, this method used only forced disconnect process
    LeaveChannelTransaction();
    UpdateChar(eDB_UPDATE_DISCONNECT_CMD, channel_id, NULL);
    SetDBState(eDB_STATE_DESTROY_WAIT);
    destroy_wait_timer_.SetTimer(MAX_DESTROY_WAIT_TIME);
}

bool DBUser::CanLogin() const
{
    switch(db_state_)
    {
    case eDB_STATE_CREATING:
    case eDB_STATE_DESTROY_WAIT:
        return false;
    }
    return true;
}

bool DBUser::CanDBUpdate() const
{
    switch(db_state_)
    {
    case eDB_STATE_CREATING:
    //  case eDB_STATE_DESTROY_WAIT:
    //  Destroy를 기다리는 상태라도 GameServer로부터 뒤늦게 DBUpdate 명령이 날라 올 수 있다.
        return false;
    }
    return true;
}

void DBUser::ReleaseChar( DBCharacter*& db_character ) const
{
    db_character->OnRelease();
    g_DBUserManager.DestroyCharacter(db_character);
    db_character = NULL;
}

void DBUser::SetConnectedChar( const DBCharacter* const db_char )
{
    if (impl::IsValidCharacter(db_char, __FUNCTION__, __LINE__) == false)
    {
        conn_char_guid_ = 0;
        return;
    }
    conn_char_guid_ = db_char->GetCharGuid();
}

namespace impl {

bool impl::IsValidCharacter( 
    const DBCharacter* const db_character, const char* func_str, const int line_number )
{
    if (db_character == NULL)
    {
        return false;
    }
    if (db_character->IsReleased())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|DBCharacter is already released.|CharGuid = %d|"), 
            func_str, 
            line_number, 
            db_character->GetCharGuid()
        );
        return false;
    }
    return true;
}

} // impl
