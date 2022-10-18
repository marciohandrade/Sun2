#include "stdafx.h"
#include "DominationManager.h"

#include <SolarDateTime.h>
#include <time.h>

#include <ResultCode.h>
#include <PacketStruct_DZ.h>
#include <PacketStruct_GZ.h>
#include <PacketStruct_WZ.h>
#include <DominationInfoParser.h>

#include "Sessions/GameServerSession.h"
#include "PacketHandler/PacketHandler.h"
#include "PacketHandler/Handler_Guild.GuildToDbp.h"
#include "QueryObjects/DBHandler.h"

#include "Services/Guild/GuildRelation.h"
#include "Services/Guild/GuildManager.h"

#ifdef _NA_003923_20120130_GUILD_RENEWAL
#include "Services/Guild/Guild.h"
#include "GuildInGameLog.h"
#endif

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
#include <MapInfoParser.h>
#include "PacketHandler//Handler_FromGuildServer.h"
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION


//==================================================================================================

DominationManager* DominationManager::instance_static_ = NULL;

DominationManager* DominationManager::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(DominationManager** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        DominationManager** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    DominationManager* provider = new DominationManager;
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

//==================================================================================================

DominationManager::DominationManager()
    : service_mode_(eServiceMode_None) // NOTE: f110521.4L
{
    service_mode_ = eServiceMode(service_mode_ | eServiceMode_Instanced);

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    continent_lord_infos_.clear();
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
}

DominationManager::~DominationManager()
{
}

bool DominationManager::Update()
{
    if (IsCompleteLoading())
    {
        return false;
    }
    const int kPreparedBaseInfo = (eServiceMode_Instanced | eServiceMode_ScriptLoaded);
    if (bool not_prepared_base_info = ((service_mode_ & kPreparedBaseInfo) != kPreparedBaseInfo))
    {
        return true;
    };
    if (bool has_transaction = ((service_mode_ & eServiceMode_InformationLoading) != 0))
    {
        bool is_complete_Load_auctions = true;
        bool is_complete_load_members = true;
        for (DominationMap::const_iterator it = domination_map_.begin(); 
            it != domination_map_.end(); ++it)
        {
            const Domination& domination = it->second;
            if (is_complete_Load_auctions && domination.is_load_auctions == false)
            {
                is_complete_Load_auctions = false;
            }
            //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
            if (is_complete_load_members && domination.is_load_members == false)
            {
                is_complete_load_members = false;
            }
        }
        if (is_complete_Load_auctions)
        {
            service_mode_ = eServiceMode(service_mode_ | eServiceMode_AuctionInfoLoaded);
            if ((service_mode_ & eServiceMode_Loaded) == eServiceMode_Loaded) 
            {
                service_mode_ = eServiceMode(service_mode_ & ~eServiceMode_InformationLoading);
            };
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|[Domination] Auction data loading is complete.|"), 
                __FUNCTION__
            );
        }            
        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        if (is_complete_load_members)
        {
            service_mode_ = eServiceMode(service_mode_ | eServiceMode_MemberInfoLoaded);
            if ((service_mode_ & eServiceMode_Loaded) == eServiceMode_Loaded) 
            {
                service_mode_ = eServiceMode(service_mode_ & ~eServiceMode_InformationLoading);
            };
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|[Domination] Member data loading is complete.|"), 
                __FUNCTION__
            );
        }
        return true;
    };
    if (bool need_request_data = ((service_mode_ & eServiceMode_AreaInfoLoaded) == 0)) 
    {
        service_mode_ = eServiceMode(service_mode_ | eServiceMode_InformationLoading);

        MSG_DZ_DOMINATION_INFO_SYN area_request;
        Handler_Guild_GuildToDbp::OnDZ_DOMINATION_INFO_SYN(
            g_DBHandler.GetSharedSession(), &area_request, sizeof(area_request));

        for (DominationMap::const_iterator it = domination_map_.begin(); 
            it != domination_map_.end(); ++it)
        {
            const Domination& domination = it->second;
            if (domination.is_load_auctions == false)
            {
                MSG_DZ_DOMINATION_AUCTION_INFO_SYN auction_request;
                auction_request.m_FieldCode = domination.field_code;
                Handler_Guild_GuildToDbp::OnDZ_DOMINATION_AUCTION_INFO_SYN(
                    g_DBHandler.GetSharedSession(), &auction_request, sizeof(auction_request));
            }
            //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
            if (domination.is_load_members == false)
            {
                MSG_DZ_DOMINATION_MEMBERLIST_REQ member_request;
                member_request.map_code_ = domination.field_code;
                Handler_Guild_GuildToDbp::OnDZ_DOMINATION_MEMBERLIST_REQ(
                    g_DBHandler.GetSharedSession(), &member_request, sizeof(member_request));
            }
        }
        return true;
    };
    return false;
}

void DominationManager::OnScriptsLoaded()
{
    domination_map_.clear();

    const DominationInfoParser::DOMINATION_SCRIPT_INFO_MAP& scripts = 
        DominationInfoParser::Instance()->GetDominationInfoMap();
    for (DominationInfoParser::DOMINATION_SCRIPT_INFO_MAP::const_iterator it = scripts.begin(); 
        it != scripts.end(); ++it)
    {
        const sDOMINATION_INFO& script = it->second;
        Domination domination;
        domination.field_code = script.m_DominationMapCode;
        domination.ClearAreaInfo();
        domination_map_[script.m_DominationMapCode] = domination;
    }
    service_mode_ = eServiceMode(service_mode_ | eServiceMode_ScriptLoaded);
    
    SUNLOG(
        eCRITICAL_LOG, 
        _T("|%s|[Domination] Script data loading is complete.|number of script = %d|"), 
        __FUNCTION__, 
        domination_map_.size()
    );
}

void DominationManager::SetCompleteLoadAreaInfo()
{
    for (DominationMap::const_iterator it = domination_map_.begin(); it != domination_map_.end(); ++it)
    {
        const Domination& domination = it->second;
        if (domination.is_load_area_info == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|[Domination] Area info is not loaded.|map code = %d|"), 
                __FUNCTION__, 
                domination.field_code
            );
        }
    }
    service_mode_ = eServiceMode(service_mode_ | eServiceMode_AreaInfoLoaded);
    if ((service_mode_ & eServiceMode_Loaded) == eServiceMode_Loaded) 
    {
        service_mode_ = eServiceMode(service_mode_ & ~eServiceMode_InformationLoading);
    };

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    UpdateContinentLordInfo(false);
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
}

WORD DominationManager::GetNumOfDominationArea(const GUILDGUID guild_guid) const
{
    size_t number_of_area = 0;
    for (DominationMap::const_iterator it = domination_map_.begin(); it != domination_map_.end(); ++it)
    {
        const Domination& domination = it->second;
        if (domination.area_info.m_inGuildInfo.m_GuildGuid == guild_guid)
        {
            ++number_of_area;
        }
    }
    if (number_of_area > USHRT_MAX)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Number of area is overflow.|guild guid = %d, number = %d|"), 
            __FUNCTION__, 
            guild_guid, 
            number_of_area
        );
        return 0;
    }
    return static_cast<WORD>(number_of_area);
}

bool DominationManager::DeleteAuction(const MAPCODE map_code, const GUILDGUID guild_guid)
{
    DominationMap::iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    Domination& domination = it->second;
    Domination::AuctionList& auctions = domination.auctions;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    Domination::AuctionList::iterator it2 = auctions.find(guild_guid);
    if (it2 != auctions.end())
    {
        auctions.erase(it2);
        return true;
    }
#else
    for (Domination::AuctionList::iterator it2 = auctions.begin(); it2 != auctions.end(); ++it2)
    {
        const DominationAuctionInfo& auction_info = it2->second;
        if (auction_info.m_ActionInfo.m_GuilGuid == guild_guid)
        {
            auctions.erase(it2);
            return true;
        }
    }
#endif
    SUNLOG(
        eCRITICAL_LOG, 
        _T("|%s|[Domination] Not found auction.|map code = %d, guild guid = %d|"), 
        __FUNCTION__, 
        map_code, 
        guild_guid
    );
    return false;
}

bool DominationManager::SetCompleteLoadAuctions(const MAPCODE map_code)
{
    DominationMap::iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    Domination& domination = it->second;
    domination.is_load_auctions = true;
    return true;
}

bool DominationManager::ClearAuctions(const MAPCODE map_code)
{
    DominationMap::iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    Domination& domination = it->second;
    domination.ClearAuctions();
    return true;
}

void DominationManager::ClearAreaInfos()
{
    for (DominationMap::iterator it = domination_map_.begin(); it != domination_map_.end(); ++it)
    {
        Domination& domination = it->second;
        domination.ClearAreaInfo();
    }
}

bool DominationManager::InsertAreaInfo(const DOMINATION_INFO& area_info)
{
    DominationMap::iterator it = domination_map_.find(area_info.m_FieldCode);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            area_info.m_FieldCode
        );
        return false;
    }
    Domination& domination = it->second;
    domination.area_info = area_info;
    domination.is_load_area_info = true;
    return true;
}

bool DominationManager::InsertAuction(
    const MAPCODE map_code, const DominationAuctionInfo& auction)
{
    DominationMap::iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    Domination& domination = it->second;
    Domination::AuctionList& auctions = domination.auctions;
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    auctions[auction.m_ActionInfo.m_GuilGuid] = auction;
#else
    for (Domination::AuctionList::iterator it = auctions.begin() ; it != auctions.end(); ++it)
    {
        DominationAuctionInfo& old_auction_info = it->second;
        if (old_auction_info.m_ActionInfo.m_GuilGuid == auction.m_ActionInfo.m_GuilGuid)
        {
            old_auction_info = auction;
            return true;
        }
    }
    auctions.insert(make_pair(auction.m_ActionInfo.m_TenderCost, auction));
#endif
    return true;
}

bool DominationManager::SendAreaInfo(
    GameServerSession* const game_server, const MAPCODE map_code) const
{
    DominationMap::const_iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    const Domination& domination = it->second;
    MSG_GZ_DOMINATION_INFO_ACK msg;
    msg.m_DominationInfo = domination.area_info;
    game_server->SendPacket(&msg, sizeof(msg));
    return true;
}

bool DominationManager::SendAuctions(
    GameServerSession* const game_server, const MAPCODE map_code) const
{
    DominationMap::const_iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    MSG_GZ_DOMINATION_AUCTION_INFO_ACK msg;
    BOOST_STATIC_ASSERT(_countof(msg.m_ActionInfo) < UCHAR_MAX);
    const Domination& domination = it->second;
    const Domination::AuctionList& auctions = domination.auctions;
    if (auctions.size() > _countof(msg.m_ActionInfo))
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Number of Auction is overflow.|map code = %d, number = %d, max number = %d|"), 
            __FUNCTION__, 
            map_code, 
            auctions.size(), 
            _countof(msg.m_ActionInfo)
        );
        return false;
    }
    int i = 0;
    for (Domination::AuctionList::const_reverse_iterator it2 = auctions.rbegin(); 
        it2 != auctions.rend(); ++it2)
    {
        const DominationAuctionInfo& auction_info = it2->second;
        msg.m_ActionInfo[i] = auction_info;
        ++i;
    }
    msg.m_MapCode = map_code;
    msg.m_DataCount = static_cast<BYTE>(i);
    game_server->SendPacket(&msg, msg.GetSize());
    return true;
}

DOMINATION_INFO* DominationManager::FindDominationInfo(const MAPCODE map_code)
{
    DominationMap::iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        return NULL;
    }
    Domination& domination = it->second;
    return &(domination.area_info);
}

bool DominationManager::SetListForReturnHeim(
    const MAPCODE map_code, 
    DOMINATION_RETURNMONEY_INFO* OUT master_info,
    const int max_size,
    int& OUT count_index) const
{
    DominationMap::const_iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    const Domination& domination = it->second;
    const Domination::AuctionList& auctions = domination.auctions;
    if (auctions.empty() || auctions.size() > max_size)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Auction list size is invalid.|map code = %d, size = %d|"), 
            __FUNCTION__, 
            map_code, 
            auctions.size()
        );
        return false;
    }
    count_index = static_cast<int>(auctions.size());
    int i = 0;
    for (Domination::AuctionList::const_iterator it = auctions.begin(); it != auctions.end(); ++it)
    {
        const DominationAuctionInfo& auction_info = it->second; 
        ::strncpy(
            master_info[i].m_tszMaster, 
            auction_info.m_tszGuildMaster, 
            _countof(master_info[i].m_tszMaster)
        );
        master_info[i].m_tszMaster[_countof(master_info[i].m_tszMaster) - 1] = '\0';
        master_info[i].m_TenderCost = auction_info.m_ActionInfo.m_TenderCost;
        master_info[i].m_CostType = SYSMEMO_DOMINATION_RETURN_MONEY;
        ++i;
    }
    return true;
}

bool DominationManager::SetCompleteLoadMembers(const MAPCODE map_code)
{
    DominationMap::iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    Domination& domination = it->second;
    domination.is_load_members = true;
    return true;
}

bool DominationManager::ClearMembers(const MAPCODE map_code)
{
    DominationMap::iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    Domination& domination = it->second;
    domination.ClearMembers();
    return true;
}

bool DominationManager::InsertMember(const MAPCODE map_code, const DominationMemberInfo& member)
{
    DominationMap::iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    Domination& domination = it->second;
    Domination::MemberList& members = domination.members;
    members[member.member_guid_] = member;
    return true;
}

bool DominationManager::DeleteMember(const MAPCODE map_code, const CHARGUID member_guid)
{
    DominationMap::iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    Domination& domination = it->second;
    Domination::MemberList& members = domination.members;
    Domination::MemberList::iterator it2 = members.find(member_guid);
    if (it2 == members.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found member.|map code = %d, member guid = %d"), 
            __FUNCTION__, 
            map_code, 
            member_guid
        );
        return false;
    }
    members.erase(it2);
    return true;
}

bool DominationManager::SendMembers(
    GameServerSession* const game_server, const MAPCODE map_code) const
{
    DominationMap::const_iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Not found map from script.|map code = %d|"), 
            __FUNCTION__, 
            map_code
        );
        return false;
    }
    const Domination& domination = it->second;
    const Domination::MemberList& members = domination.members;
    const size_t member_size = members.size();
    
    MSG_GZ_DOMINATION_MEMBERLIST_ANS msg;
    if (member_size > _countof(msg.member_list_) || member_size > UCHAR_MAX)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|[Domination] Too many members.|map code = %d, member size = %d"), 
            __FUNCTION__, 
            map_code, 
            member_size
        );
        return false;
    }
    size_t i = 0;
    for (Domination::MemberList::const_iterator it2 = members.begin(); it2 != members.end(); ++it2)
    {
        msg.member_list_[i] = it2->second;
        ++i;
    }
    msg.map_code_ = map_code;
    msg.data_count_ = static_cast<BYTE>(member_size);
    game_server->SendPacket(&msg, msg.GetSize());
    return true;
}

#ifdef _NA_003923_20120130_GUILD_RENEWAL
void
DominationManager::DominationInGameGuildLog(DOMINATION_INFO update_info,
                                            eDOMINATION_EVENT_TYPE event_type)
{
    GUILDGUID defense_guild_guid = update_info.m_inGuildInfo.m_GuildGuid;
    GUILDGUID attack_guild_guid = update_info.m_disGuildInfo.m_GuildGuid;

    Guild* defense_guild = g_GuildManager.FindGuild(defense_guild_guid);
    Guild* attack_guild = g_GuildManager.FindGuild(attack_guild_guid);
    Guild* request_guild = (defense_guild == NULL) ? attack_guild : defense_guild;
    if (request_guild == NULL)
    {
        return;
    }

    switch (event_type)
    {
    case DOMINATION_SUCCESS_DEFENCE:
        {
            request_guild->GetGuildInGameLog()->WriteGuildLog(
                GuildSuccessDomiDefense,
                update_info.m_inGuildInfo.m_GuildGuid,
                NULL,
                update_info.m_FieldCode
                );
        }
        break;

    case DOMINATION_SUCCESS_OFFENCE:
        {
            request_guild->GetGuildInGameLog()->WriteGuildLog(
                GuildSuccessDomiAttack,
                update_info.m_disGuildInfo.m_GuildGuid,
                NULL,
                update_info.m_FieldCode
                );
        }
        break;

    case DOMINATION_DISGUILD_SET:
        {
            request_guild->GetGuildInGameLog()->WriteGuildLog(
                GuildSelectedDomiAttack,
                attack_guild_guid,
                NULL,
                update_info.m_FieldCode
                );

            request_guild->GetGuildInGameLog()->WriteGuildLog(
                GuildNotifyDomiAttack,
                defense_guild_guid,
                update_info.m_disGuildInfo.m_tszGuildName,
                update_info.m_FieldCode
                );

            //입찰 실패한 길드에게 지역, 입찰 성공 갯수 로그를 보낸다.
            //step 1 : 로그를 남길 지역 정보및 해당 정보의 경매 리스트 구하기
            DominationMap::iterator it = domination_map_.find(update_info.m_FieldCode);
            if (it == domination_map_.end())
            {
                SUNLOG(
                    eCRITICAL_LOG, 
                    __FUNCTION__ " : Not found map from script : map code = %d", 
                    update_info.m_FieldCode
                    );
                return;
            }

            Domination& domination = it->second;
            Domination::AuctionList& auctions = domination.auctions;

            //step 2 : 입찰 성공 갯수 구하기
            Domination::AuctionList::iterator find_it = auctions.find(attack_guild_guid);
            if (find_it == auctions.end())
            {
                SUNLOG(
                    eCRITICAL_LOG,
                    __FUNCTION__" : Not found auction info : guild guid = %d",
                    attack_guild_guid
                    );
                return;
            }
            const DominationAuctionInfo& auction_info = find_it->second;
            ULONGLONG best_auctioin_cost = auction_info.m_ActionInfo.m_TenderCost;

            //step 3 : 입찰 실패 길드에 로그 남기기
            Domination::AuctionList::iterator begin_it = auctions.begin();
            Domination::AuctionList::iterator end_it = auctions.end();
            for (; begin_it != end_it; ++begin_it)
            {
                const DominationAuctionInfo& auction_info = begin_it->second;
                if (auction_info.m_ActionInfo.m_GuilGuid != attack_guild_guid)
                {
                    request_guild->GetGuildInGameLog()->WriteGuildLog(
                        GuildFailAuction,
                        auction_info.m_ActionInfo.m_GuilGuid,
                        NULL,
                        update_info.m_FieldCode,
                        best_auctioin_cost
                        );
                }

            }
        }
        break;
    }
}

#endif

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION

void DominationManager::SetDominationContinentLordInfo( const sDOMINATION_CONTINENT_LORD_INFO lord_info )
{
    ContinentLordInfos::iterator iter = continent_lord_infos_.find(lord_info.continent_number);
    if (iter != continent_lord_infos_.end())
    {
        continent_lord_infos_.erase(iter);
    }
    continent_lord_infos_.insert(std::make_pair<int, sDOMINATION_CONTINENT_LORD_INFO>
        (lord_info.continent_number, lord_info));
}

const sDOMINATION_CONTINENT_LORD_INFO* DominationManager::GetDominationContinentLordInfo( int continent ) const
{
    ContinentLordInfos::const_iterator iter = continent_lord_infos_.find(continent);
    if (iter != continent_lord_infos_.end())
    {
        return &iter->second;
    }
    return NULL;
}

bool DominationManager::SendDominationContinentLordInfo( const MAPCODE map_code )
{
    DominationMap::const_iterator it = domination_map_.find(map_code);
    if (it == domination_map_.end())
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found Map (field code : %d)", map_code);
        return false;
    }

    const Domination& domination = it->second;
    const MAPCODE field_code = domination.field_code;
    if (DominationInfoParser::Instance()->GetDominationTypeFromFieldCode(field_code) != DOMINATION_TYPE_MONDSCHIEN)
    {
        return false;
    }

    const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(field_code);
    if(pInfo == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found Map info (field code : %d)", field_code);
        return false;
    }

    const sDOMINATION_CONTINENT_LORD_INFO* lord_info = GetDominationContinentLordInfo(pInfo->ContienetNumber);
    if (lord_info == NULL)  //대륙의 영주가 존재하지 않는다
    {
        return false;
    }

    MSG_GZ_DOMINATION_CONTINENT_LORD_INFO_ACK msg;
    msg.m_lordInfo = *lord_info;
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &msg, sizeof(msg));

    return true;
}

void DominationManager::UpdateContinentLordInfo(const bool send_game_server/* = false*/)
{
    const DominationInfoParser* domination_info_parser = DominationInfoParser::Instance();

    for (DominationMap::iterator it = domination_map_.begin(); it != domination_map_.end(); ++it)
    {
        Domination& domination = it->second;
        MAPCODE field_code = domination.area_info.m_FieldCode;

        const eDOMINATION_TYPE domi_type = domination_info_parser->GetDominationTypeFromFieldCode(field_code);
        if (domi_type != DOMINATION_TYPE_MONDSCHIEN)
        {
            continue;
        }

        const sMAPINFO* pInfo = MapInfoParser::Instance()->FindMapInfo(field_code);
        if(pInfo == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found Map info (field code : %d)", field_code);
            continue;
        }

        GUILDGUID lord_guild_guid = domination.area_info.m_inGuildInfo.m_GuildGuid;

        MSG_DZ_CONTINENT_LORD_INFO_SYN msg;
        msg.m_dwKey = 0;
        msg.m_dwSessionIndex = 0;
        msg.lord_guild_guid = lord_guild_guid;
        msg.contienet_number = pInfo->ContienetNumber;
        msg.map_code = field_code;
        msg.send_game_server = send_game_server;
        util::GUILDNAMECOPY(msg.lord_guild_name, domination.area_info.m_inGuildInfo.m_tszGuildName);
        Handler_Guild_GuildToDbp::OnDZ_CONTINENT_LORD_INFO_SYN(
            g_DBHandler.GetSharedSession(), &msg, sizeof(msg));
    }
}

#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
