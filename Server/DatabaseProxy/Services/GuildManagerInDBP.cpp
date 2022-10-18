#include "stdafx.h"
#include "./GuildManagerInDBP.h"
//==================================================================================================
#include <PacketStruct_DG.h>
#include <PacketStruct_DZ.h>
#include <VersionControl.h>
//#include "ServerSession.h"
#include "QueryObjects/DatabaseProxyQuery.h"
#include "QueryObjects/GuildServerQuery.h"
//
#include "QueryObjects/DBHandler.h"
#include "ProxyNodes/DBQueryProxyManager.h"

#include "PacketHandler/Handler_Guild.DbpToGuild.h"

//==================================================================================================

GuildManagerInDBP g_GuildManagerInDBP;
__IMPL_CUSTOMPOOL_PTR(GuildWarehouseInfo);

GuildManagerInDBP::GuildManagerInDBP()
    : update_frames_(0)
    , guild_warehouse_pool_(NULL)
{
}

GuildManagerInDBP::~GuildManagerInDBP()
{
    Release();
}

void GuildManagerInDBP::Init(DWORD pool_size)
{
    guild_warehouse_pool_ = new util::CMemoryPoolFactory<GuildWarehouseInfo>;
    guild_warehouse_pool_->Initialize(pool_size);
    GuildWarehouseInfo::SetPool(guild_warehouse_pool_);
}

void GuildManagerInDBP::Release()
{
    FOREACH_CONTAINER(const GUILD_WAREHOUSE_HASH::value_type& node, guild_warehouse_hash_, \
                      GUILD_WAREHOUSE_HASH)
    {
        GuildWarehouseInfo* guild_warehouse_info = node.second;
        if (guild_warehouse_info)
        {
            guild_warehouse_info->Release();
            GuildWarehouseInfo::FREE(guild_warehouse_info);
        }
    };
    guild_warehouse_hash_.clear();
    SAFE_DELETE(guild_warehouse_pool_);
}

GuildWarehouseInfo* GuildManagerInDBP::CreateGuildWarehouse(GUILDGUID guild_guid)
{
    // f101011.4L, change styles
    GUILD_WAREHOUSE_HASH::const_iterator found_it = guild_warehouse_hash_.find(guild_guid);
    if (found_it != guild_warehouse_hash_.end()) {
        return found_it->second;
    }

    GuildWarehouseInfo* const guild_warehouse_info = GuildWarehouseInfo::ALLOC();
    assert(guild_warehouse_info);
    guild_warehouse_info->Init(guild_guid);
    guild_warehouse_hash_.insert(std::make_pair(guild_guid, guild_warehouse_info));

    return guild_warehouse_info;
}


void GuildManagerInDBP::DestroyGuildWarehouse(GUILDGUID guild_guid)
{
    GUILD_WAREHOUSE_HASH::iterator found = guild_warehouse_hash_.find(guild_guid);
    if (found == guild_warehouse_hash_.end()) {
        return;
    };
    GuildWarehouseInfo* guild_warehouse_info = found->second;
    if (guild_warehouse_info) {
        GuildWarehouseInfo::FREE(guild_warehouse_info);
    }
    guild_warehouse_hash_.erase(found);
}

bool GuildManagerInDBP::Update()
{
    bool changed_events = false;
    // NOTE: f110627.1L, one updating per 128 frames
    if ((update_frames_++ & this->kUpdatePerFrames) == 0) {
        return false;
    };
    ChildServerSession* const shared_session = g_DBHandler.GetSharedSession();
    FOREACH_CONTAINER(const GUILD_WAREHOUSE_HASH::value_type& node, guild_warehouse_hash_, \
                      GUILD_WAREHOUSE_HASH)
    {
        GuildWarehouseInfo* guild_warehouse_info = node.second;
        if (guild_warehouse_info == NULL) {
            continue;
        };
        // NOTE: f110627.1L, ... do forced update to prevent a mission update probability
        guild_warehouse_info->Update(shared_session, 0, 0);
    };
    //
    if (changed_events) {
        return false;
    }
    return true;
}

void GuildManagerInDBP::OpenWarehouseError(DWORD user_key, GUILDGUID guild_guid,
                                        ChildServerSession* session,
                                        BYTE error_code, DWORD session_index)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
    // CHANGES: f110526.6L, reply session correction
    // CHANGES: f110609.7L, added versioning, database proxy server session doesn't equal
    // them of the guild server
    ;{
        ServerSession* reply_session = g_DBProxyServer.FindSession(session_index);
        if (reply_session != NULL) {
            session = static_cast<ChildServerSession*>(reply_session);
        };
    };
#endif
    MSG_DZ_GUILD_WAREHOUSE_OPEN_NAK msg_nak;
    msg_nak.m_dwKey = user_key;
    msg_nak.m_dwSessionIndex = 0; // (WAVERIX) (090903) (CHANGES) broad 계열로 처리되고 있지만, 초기화하자.
    msg_nak.m_GuildGuid = guild_guid;
    msg_nak.m_byErrorCode	= error_code;
    //msg.m_dwSessionIndex = SessionIndex;
    session->SendPacket(&msg_nak, sizeof(msg_nak));
    return;
}

void GuildManagerInDBP::DBGuildCreateResultCodeConvert(BYTE& result_error_code)
{
    const BYTE db_result_code = result_error_code;
    switch (db_result_code)
    {
    case 1:
        result_error_code = RC::RC_GUILD_CANNOT_CREAT_SAMENAME_STATE; 
        return;
    }
}

void GuildManagerInDBP::DominationReturnMoneyUpdate(ChildServerSession* pServerSession, 
                                                 const DominationAuctionInfo& actionInfo, 
                                                 eSYSTEM_MEMO_TYPE moneyType)
{
    INT flag = static_cast<int>(moneyType);

    TCHAR szQueryBuff[MAX_QUERY_LENGTH_128+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_128,
               "{?=call S_DominationInfo_ActionMoney_Update(%d,%d,%I64u,%d)}", 
               actionInfo.m_ActionInfo.m_FieldCode,
               actionInfo.m_MasterGuid,
               actionInfo.m_ActionInfo.m_TenderCost,
               flag);
    szQueryBuff[MAX_QUERY_LENGTH_128] = '\0';

    Query_Domination_ActionMoney_Update* pQuery = Query_Domination_ActionMoney_Update::ALLOC();

    pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_UPDATE_RETURNHEIM_DBR));
    pQuery->SetQuery(szQueryBuff);
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

void GuildManagerInDBP::DominationReturnMoneyDelete(ChildServerSession* pServerSession, 
                                                 const DOMINATION_INFO& dominationInfo)
{
    TCHAR szQueryBuff[MAX_QUERY_LENGTH_64+1];
    _sntprintf(szQueryBuff, MAX_QUERY_LENGTH_64, "{?=call S_DominationInfo_ActionMoney_Delete(%d,%d)}",
               dominationInfo.m_FieldCode, dominationInfo.m_disGuildInfo.m_GuildMasterGuid);
    szQueryBuff[MAX_QUERY_LENGTH_64]='\0';

    Query_Domination_ReturnMoney_Delete* pQuery = Query_Domination_ReturnMoney_Delete::ALLOC();

    pQuery->SetDominationInfo(dominationInfo);
    pQuery->SetQuery(szQueryBuff);
    pQuery->SetIndex(MAKEDWORD((WORD)DZ_GUILD, (WORD)DZ_DOMINATION_DELETE_RETURNHEIM_DBR));
    pQuery->SetVoidObject(pServerSession);
    pServerSession->DBQuery(pQuery);
}

//==================================================================================================


bool GuildManagerInDBP::RequestLoadWarehouse(const MSG_DZ_GUILD_WAREHOUSE_OPEN_SYN& recv_msg,
                                          GuildWarehouseInfo* guild_warehouse_info,
                                          ChildServerSession* server_session)
{
    const USERGUID user_guid = recv_msg.m_dwKey;
    const GUILDGUID guild_guid = recv_msg.m_GuildGuid;
    GuildWarehouseInfo::Shared* guild_shared = &guild_warehouse_info->shared_;
    // #1
    if ((guild_shared->state & guild_warehouse_info->eState_Loaded_Warehouse) == 0 &&
        (guild_shared->state & guild_warehouse_info->eTrans_Select_Warehouse) == 0)
    {
        STRING_SAFER_QUERY64 query_buffer;
        query_buffer.MakeString("{call S_GuildWarehouse_Select (?)}");

        GuildWarehouseSelectQuery* query = GuildWarehouseSelectQuery::ALLOC();
        query->SetUserKey(user_guid);
        // passing data
        query->SetSessionIndex(recv_msg.m_dwSessionIndex);
        query->SetCharGuid(recv_msg.m_OperCharGuid);
        // set query parameters
        if (GuildWarehouseSelectQuery::sQueryParam* const parameter = &query->parameters_[0])
        {
            parameter->guild_guid_ = guild_guid;
        }
        //
        query->SetQuery(query_buffer);
        //
        server_session->DBQuery(DZ_GUILD, DZ_GUILD_WAREHOUSE_OPEN_DBR, query);
        guild_warehouse_info->BeginTransaction(guild_warehouse_info->eTrans_Select_Warehouse);
    };
    // #2
    if ((guild_shared->state & guild_warehouse_info->eState_Loaded_WareItems) == 0 &&
        (guild_shared->state & guild_warehouse_info->eTrans_Select_WareItems) == 0)
    {
        STRING_SAFER_QUERY64 query_buffer;
        query_buffer.MakeString("{?=call S_GuildWarehouseItemSlotInfo_Select (?,?)}");

        Query_GuildWarehouse_ItemSlotInfo_Select* query = \
            Query_GuildWarehouse_ItemSlotInfo_Select::ALLOC();
        query->SetUserKey(user_guid);
        //
        query->session_index_ = recv_msg.m_dwSessionIndex;
        query->guild_guid_ = guild_guid;
        query->set_char_guid(recv_msg.m_OperCharGuid);
        //
        query->parameters_[0].guid_ = guild_guid;
        query->parameters_[0].slot_type_ = nsQuery::GuildWarehouseSlotUpdateTable::kSlotIndex;
        //
        query->SetQuery(query_buffer);
        //
        server_session->DBQuery(DZ_GUILD, DZ_GUILD_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR, query);
        guild_warehouse_info->BeginTransaction(guild_warehouse_info->eTrans_Select_WareItems);
    };

    //
    return true;
}

bool GuildManagerInDBP::DBResult_Warehouse_Select(const GuildWarehouseSelectQuery& query_result,
                                               ChildServerSession* server_session)
{
    const USERGUID user_guid = query_result.GetUserKey();
    const GUILDGUID guild_guid = query_result.GetGuildGuid();
    const DWORD session_index = query_result.GetSessionIndex();
    //
    GuildWarehouseInfo* guild_warehouse_info = g_GuildManagerInDBP.FindGuildWarehouse(guild_guid);
    if (!FlowControl::FCAssert(guild_warehouse_info)) {
        return false;
    }
    //
    bool success = true;
    if (success && query_result.GetResultRowNum() != 1)
    {
        g_GuildManagerInDBP.OpenWarehouseError(user_guid, guild_guid, server_session,
                                            RC::RC_GUILD_WAREHOUSE_GET_CANNOT_DB,
                                            session_index);
        success = false;
    }
    //
    if (success) {
        guild_warehouse_info->OnSetGuildWarehouseBaseInfo(query_result);
    }
    guild_warehouse_info->EndTransaction(guild_warehouse_info->eTrans_Select_Warehouse, success);
    //
    if (success == false) {
        return false;
    }
    //
    return true;
}

bool GuildManagerInDBP::DBResult_WarehouseItemSlotInfo_Select(
    GuildWarehouseInfo* guild_warehouse_info,
    const Query_GuildWarehouse_ItemSlotInfo_Select& query_result,
    ChildServerSession* server_session)
{
    bool success = guild_warehouse_info->DBResult(query_result);
    //
    guild_warehouse_info->EndTransaction(guild_warehouse_info->eTrans_Select_WareItems, success);
    //
    const USERGUID user_guid = query_result.GetUserKey();
    const CHARGUID char_guid = query_result.char_guid();
    bool all_loaded = guild_warehouse_info->IsWarehouseLoaded();
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
    // CHANGES: f110526.6L, reply session correction
    // CHANGES: f110609.7L, added versioning, database proxy server session doesn't equal
    // them of the guild server
    ;{
        ServerSession* reply_session = g_DBProxyServer.FindSession(query_result.session_index_);
        if (reply_session != NULL) {
            server_session = static_cast<ChildServerSession*>(reply_session);
        };
    };
#endif
    // NOTE: f101117.1L, if 'all_loaded' is fails, it is occurred a wrong sequence.
    if (success == false || all_loaded == false)
    {
        g_GuildManagerInDBP.OpenWarehouseError(user_guid, query_result.guild_guid_,
                                            server_session,
                                            RC::RC_GUILD_WAREHOUSE_GET_CANNOT_DB,
                                            query_result.session_index_);
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|[%u][%u]")
               _T("|Msg=can't open guild(%u) warehouse for char(%u) - addin status 'loaded(%d)'"),
               query_result.guild_guid_, char_guid, all_loaded);
        return false;
    };
    // NOTE: f101117.1L, add filters considering user life-time
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy = nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result != db_query_proxy->eFindResult_Service)
    {   // eFindResult_NotFound, eFindResult_Pending
        g_GuildManagerInDBP.OpenWarehouseError(user_guid, query_result.guild_guid_,
                                            server_session,
                                            RC::RC_GUILD_WAREHOUSE_GET_CANNOT_DB,
                                            query_result.session_index_);
        return false;
    };
    //
    ;{
        MSG_DZ_GUILD_WAREHOUSE_OPEN_ACK msg_ack;
        //msg.m_ItemInfo.m_Count = 0;
        msg_ack.m_dwKey = user_guid;
        msg_ack.m_OperCharGuid = char_guid;
        msg_ack.m_dwSessionIndex = query_result.session_index_;
        msg_ack.m_GuildGuid = query_result.guild_guid_;
        msg_ack.m_SlotTabNum = guild_warehouse_info->GetSlotTabNum();
        msg_ack.m_WarehouseMoney = guild_warehouse_info->GetWarehouseMoney();
        // (f100527.4L)
        _GUILDWAREHOUSE_TOTAL_INFO* const guild_warehouse_item_block = &msg_ack.m_ItemInfo;
        guild_warehouse_item_block->Clear();
        guild_warehouse_info->LoadItemSlotInfo(SI_GUILDWAREHOUSE,
            guild_warehouse_item_block->m_Slot, &guild_warehouse_item_block->m_Count);

        guild_warehouse_info->AcquireOpenLock(user_guid, char_guid);
    #ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION
        Handler_Guild_DbpToGuild::OnDZ_GUILD_WAREHOUSE_OPEN_ACK(
            g_DBHandler.GetSharedSession(), &msg_ack, msg_ack.GetSize());
    #else
        server_session->SendPacket(&msg_ack, msg_ack.GetSize());
    #endif
    };
    return true;
}

//==================================================================================================

