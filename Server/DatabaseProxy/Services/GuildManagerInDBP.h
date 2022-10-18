#pragma once
#include <SerializeStream.h>
#include "./ProxyNodes/DBQueryProxyGuildNode.h"

struct MSG_DZ_GUILD_WAREHOUSE_OPEN_SYN;
class Query_GuildWarehouse_ItemSlotInfo_Select;

// f101010.3L
class GuildWarehouseInfo : public nsQuery::DBQueryProxyGuildNode
{
    __DECL_CUSTOMPOOL_PTR(GuildWarehouseInfo);
};


//==================================================================================================

class GuildManagerInDBP
{
public:
    GuildManagerInDBP();
    ~GuildManagerInDBP();

    bool Update();
    void Release();
    void Init(DWORD);
    void DestroyGuildWarehouse(GUILDGUID);
    GuildWarehouseInfo* CreateGuildWarehouse(GUILDGUID guild_guid);
    GuildWarehouseInfo* FindGuildWarehouse(GUILDGUID guild_guid) const;

    void DBGuildCreateResultCodeConvert(BYTE& result_error_code);
    // side-effect: message send 'MSG_DZ_GUILD_WAREHOUSE_OPEN_NAK'
    void OpenWarehouseError(DWORD, GUILDGUID, ChildServerSession*, BYTE, DWORD);
    // NOTE: database query & reply process
    bool RequestLoadWarehouse(const MSG_DZ_GUILD_WAREHOUSE_OPEN_SYN& recv_msg,
                              GuildWarehouseInfo* guild_warehouse_info,
                              ChildServerSession* server_session);
    bool DBResult_Warehouse_Select(const GuildWarehouseSelectQuery& query_result,
                                   ChildServerSession* server_session);
    bool DBResult_WarehouseItemSlotInfo_Select(\
        GuildWarehouseInfo* guild_warehouse_info,
        const Query_GuildWarehouse_ItemSlotInfo_Select& query_result,
        ChildServerSession* server_session);
    //bool ChangeNumberOfSlotTabs(const MSG_DG_GUILD_WAREHOUSE_TAPEX_SYN& tapex_syn);
    //----------------------------------------------------------------------------------------------
    // NOTE: this below section describes contents interfaces
    //
    void GuildManagerInDBP::DominationReturnMoneyUpdate( ChildServerSession* pServerSession, 
        const DominationAuctionInfo& actionInfo, eSYSTEM_MEMO_TYPE moneyType );

    void GuildManagerInDBP::DominationReturnMoneyDelete( ChildServerSession* pServerSession, 
        const DOMINATION_INFO& DominationInfo );

private:
    static const ulong kUpdatePerFrames = 128 - 1;
    typedef STLX_HASH_MAP<GUILDGUID, GuildWarehouseInfo*> GUILD_WAREHOUSE_HASH;
    //
    ulong update_frames_;
    util::CMemoryPoolFactory<GuildWarehouseInfo>* guild_warehouse_pool_;
    GUILD_WAREHOUSE_HASH guild_warehouse_hash_;
};

inline GuildWarehouseInfo* GuildManagerInDBP::FindGuildWarehouse(GUILDGUID guild_guid) const
{
    GUILD_WAREHOUSE_HASH::const_iterator found = guild_warehouse_hash_.find(guild_guid);
    if (found == guild_warehouse_hash_.end()) {
        return NULL;
    }
    GuildWarehouseInfo* guild_warehouse_info = found->second;
    return guild_warehouse_info;
}

extern GuildManagerInDBP g_GuildManagerInDBP;

