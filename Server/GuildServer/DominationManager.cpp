#include "StdAfx.h"

#include <SolarDateTime.h>
#include <time.h>
#include <GuildServer.h>
#include <Sessions/DBProxyServerSession.h>
#include <Sessions/GameServerSession.h>
#include <PacketStruct_DZ.h>
#include <Guild/GuildRelation.h>
#include <Guild/GuildManager.h>

#include <PacketStruct_DZ.h>
#include <PacketStruct_GZ.h>
#include <PacketStruct_WZ.h>
#include <ResultCode.h>

#include "DominationManager.h"
#include "DominationInfoParser.h"

DominationManager::DominationManager()
:is_load_domination_area_info_(false)
,is_load_domination_auction_info_(false)
,is_init_(false)
#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
,is_load_domination_member_info_(false)
#endif
{
    LoadingTimerSet();
}

DominationManager::~DominationManager()
{
}

bool DominationManager::SendToDBProxy(MSG_BASE* msg, WORD size)const
{
    if (!GuildServer::GetMainFrame()->ConnectSessions.SendToDBProxy(msg, size))
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not connect DB");
        return false;
    }
    return true;
}

#ifdef _NA_20100604_BETTER_DOMINATION_MANAGER 
WORD 
DominationManager::NumberOfDominationAreaByGuild(const GUILDGUID guild_guid)
{
    AreaInfoMap::iterator begin_iterator = domination_area_info_map_.begin(),
                                    end_iterator = domination_area_info_map_.end();

    WORD number_of_domination_map = 0;

    for(;begin_iterator != end_iterator; ++begin_iterator)
    {
        const DOMINATION_INFO& info = begin_iterator->second;

        if (info.m_inGuildInfo.m_GuildGuid == guild_guid)
        {
            ++number_of_domination_map;
        }
    }

    return number_of_domination_map;
}
#endif//_NA_20100604_BETTER_DOMINATION_MANAGER

void 
DominationManager::DominationInfoStatusProc()
{
    DominationLoadingStatus loading_status = GetDominationInfoStatus();

    switch (loading_status)
    {
    case DOMINATION_INFO_EMPTY:
        SetDominationLoadingStatus(DOMINATION_INFO_LOADING);
        RequestDominationAreaInfo();
        RequestDominationAuctionInfo();
    #ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        RequestDominationMemberInfo();
    #endif
        break;

    case DOMINATION_INFO_LOADING:
        CheckCompleteLoadAuctionData();
    #ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        CheckCompleteLoadMemberData();
    #endif
        if (CheckLoadingStatus() == TRUE)
        {
            SetDominationLoadingStatus(DOMINATION_INFO_SUCCESS);
        }
        break;

    case DOMINATION_INFO_SUCCESS:
        LoadingTimerDisable();
        break;

    default:
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Invalid Status Value");
    }
}


//1. 현재 점령전 정보를 모두 로드 하였는지 체크 한다.
//  - 로드 하지 못했다면 DBP로 점령전 정보 로드 루틴을 실행한다.
//  - 로드를 완료 하였다면 점령전 완료상태를 체크 한다.
//2. 현재 GameServer에서 정보로드를 완료 하였는지 체크 한다.
//  - GameServer에서 정보를 로드 완료 하였는지를 알고 있어야 한다.
//  - GameServer에서 정보를 요청하게 되면 정보 로드 셋팅을 바꾼다.
//  - Transaction 상황을 체크하여 적용 한다.
void 
DominationManager::LoadDominationInfo()
{
    if ((loading_timer_.IsExpired()) == false)
    {
        return;
    }

    ScriptInfoMap& script_domination_area_info_map = 
        DominationInfoParser::Instance( )->GetDominationInfoMap( );

    int row_size = static_cast<int>(script_domination_area_info_map.size());

    if (row_size > 0)
    {
        Initialize();
    }

    if (GetIsinit() == true)
    {
        DominationInfoStatusProc();
    }
}



void 
DominationManager::Initialize()
{
    if (GetIsinit() == false)
    {
        SetCheckDominationActionMap();
        SetEmptyDominationInfo();
#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        SetCheckDominationMemberMap();
#endif
        loading_status_ = DOMINATION_INFO_EMPTY;
        SetIsInit(true);
    }
}

bool 
DominationManager::DeleteActionList(MAPCODE map_code, GUILDGUID guild_guid)
{
    AllAreaAuctionInfoMap::iterator find_iterator = all_area_auction_info_map_.find(map_code);

    if (find_iterator == all_area_auction_info_map_.end())
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__"Not found map : %d", map_code);
        return false;
    }

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    DominationAuctionInfoList& action_list =  find_iterator->second;

    DominationAuctionInfoList::iterator begin_iterator = action_list.begin(),
                                        end_iterator = action_list.end();
#else
    DominationAuctionInfoMap& auction_map = find_iterator->second;
    
    DominationAuctionInfoMap::iterator begin_iterator = auction_map.begin(),
                                       end_iterator = auction_map.end();
#endif

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
    #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        const DominationAuctionInfo& auction_info = *begin_iterator;
        if (auction_info.m_ActionInfo.m_GuilGuid == guild_guid)
        {
            action_list.erase(begin_iterator);
            return true;
        }
    #else
        const DominationAuctionInfo& auction_info = begin_iterator->second;
        if (auction_info.m_ActionInfo.m_GuilGuid == guild_guid)
        {
            auction_map.erase(begin_iterator);
            return true;
        }
    #endif
    }

    find_iterator->second.clear();
    return false;
}

void 
DominationManager::SetCheckActionInfo(MAPCODE map_code)
{
    AuctionInfoLoadingCheckMap::iterator find_iterator = 
        auction_info_loading_check_map_.find(map_code);

    if (find_iterator != auction_info_loading_check_map_.end())
    {
        find_iterator->second = true;
    }
}

void 
DominationManager::SetCheckDominationActionMap()
{
    ScriptInfoMap& script_info_map = 
        DominationInfoParser::Instance()->GetDominationInfoMap();

    ScriptInfoIterator begin_iterator = script_info_map.begin(),
                       end_iterator = script_info_map.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const sDOMINATION_INFO& script_info = begin_iterator->second;
        auction_info_loading_check_map_.insert(std::make_pair(script_info.m_DominationMapCode, false));
    }
}


void 
DominationManager::SetEmptyDominationInfo()
{
    ScriptInfoMap& script_info_map = 
        DominationInfoParser::Instance( )->GetDominationInfoMap( );

    ScriptInfoIterator begin_iterator = script_info_map.begin(),
                       end_iterator = script_info_map.end();

    for (;begin_iterator != end_iterator; ++begin_iterator )
    {
        const sDOMINATION_INFO& script_info = begin_iterator->second;

        if (FindDominationInfo(script_info.m_DominationMapCode) == NULL)
        {
            DOMINATION_INFO domination_info;
            ZeroMemory(&domination_info, sizeof(domination_info));

            domination_info.m_FieldCode = script_info.m_DominationMapCode;
            domination_info.m_TotalPoint = DOMINATION_DEFFENSWALL_POINT;

            InsertDominationInfo(domination_info);
        }
    }
}


bool 
DominationManager::CheckLoadingStatus()
{
    if (GetLoadDominationInfo() == false)
    {
        RequestDominationAreaInfo();
    }

    if (GetLoadDominationAuctionInfo() == false)
    {
        RequestDominationAuctionInfo();
    }

#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    if (GetLoadDominationMemberInfo() == false)
    {
        RequestDominationMemberInfo();
    }
#endif

    return IsCompleteLoad();
}

bool 
DominationManager::IsCompleteLoad()
{
#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    bool result = (is_load_domination_area_info_ && 
                   is_load_domination_auction_info_ && 
                   is_load_domination_member_info_ ) ? true : false;
#else
    bool result = (is_load_domination_area_info_ && 
                   is_load_domination_auction_info_) ? true : false;
#endif
    return result;
}

void 
DominationManager::RequestDominationAreaInfo()
{
    MSG_DZ_DOMINATION_INFO_SYN send_msg;

    SendToDBProxy(&send_msg, sizeof(send_msg));
}

void 
DominationManager::RequestDominationAuctionInfo()
{
    AuctionInfoLoadingCheckMap::iterator begin_iterator = auction_info_loading_check_map_.begin(),
                                         end_iterator = auction_info_loading_check_map_.end();

    for(; begin_iterator != end_iterator; ++begin_iterator)
    {
        const bool& is_load = begin_iterator->second;

        if (is_load == false)
        {
            MAPCODE map_code = begin_iterator->first;
            MSG_DZ_DOMINATION_AUCTION_INFO_SYN send_msg;
            send_msg.m_FieldCode = map_code;
            if (SendToDBProxy(&send_msg, sizeof(send_msg)) == true)
            {
                InsertDominationMapInfo(send_msg.m_FieldCode);
            }
        }
    }
}

void
DominationManager::CheckCompleteLoadAuctionData()
{
    AuctionInfoLoadingCheckMap::iterator begin_iterator = auction_info_loading_check_map_.begin(),
                                         end_iterator = auction_info_loading_check_map_.end();

    for(; begin_iterator != end_iterator; ++begin_iterator)
    {
        const bool& is_load = begin_iterator->second;

        if (is_load == false)
        {
            return;
        }
    }

    SUNLOG(eCRITICAL_LOG,"Complete auction data load");
    SetLoadDominationAuctionInfo(true);
}

bool 
DominationManager::InsertDominationMapInfo(MAPCODE map_code)
{
    AllAreaAuctionInfoMap::iterator auction_iterator = all_area_auction_info_map_.find(map_code);

    if (auction_iterator == all_area_auction_info_map_.end())
    {
    #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        DominationAuctionInfoList auction_list;
        all_area_auction_info_map_.insert(std::make_pair(map_code, auction_list));
    #else
        DominationAuctionInfoMap auction_map;
        all_area_auction_info_map_.insert(std::make_pair(map_code, auction_map));
    #endif
    }
    return true;
}

void 
DominationManager::InsertDominationInfo(const DOMINATION_INFO& domination_info)
{
    AreaInfoMap::iterator find_iterator = 
        domination_area_info_map_.find(domination_info.m_FieldCode);

    if (find_iterator == domination_area_info_map_.end())
    {
        //insert
        domination_area_info_map_.insert(std::make_pair(domination_info.m_FieldCode, domination_info));
    }
    else
    {
        //update
        find_iterator->second = domination_info;
    }
}

bool 
DominationManager::InsertActionInfo(MAPCODE map_code, const DominationAuctionInfo& db_auction_info)
{
    AllAreaAuctionInfoMap::iterator find_iterator = all_area_auction_info_map_.find(map_code);

    if (find_iterator == all_area_auction_info_map_.end())
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Not found auction info : %d", map_code);
        return false;
    }

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    DominationAuctionInfoList& auction_list = find_iterator->second;
    DominationAuctionInfoList::iterator begin_iterator = auction_list.begin(),
                                        end_iterator = auction_list.end();
#else
    DominationAuctionInfoMap& auction_map = find_iterator->second;
    DominationAuctionInfoMap::iterator begin_iterator = auction_map.begin(),
                                        end_iterator = auction_map.end();
#endif
 
    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
    #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        DominationAuctionInfo& current_db_auction_info = *begin_iterator;
    #else
        DominationAuctionInfo& current_db_auction_info = begin_iterator->second;
    #endif

        DOMINATION_ACTION_INFO& current_auction_info = current_db_auction_info.m_ActionInfo;
        if (current_auction_info.m_GuilGuid == db_auction_info.m_ActionInfo.m_GuilGuid)
        {
            //update
            current_auction_info.m_TenderCost = db_auction_info.m_ActionInfo.m_TenderCost;
            return true;
        }
    }

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    //insert
    auction_list.push_back(db_auction_info);
#else
    //insert
    MONEY key_value = db_auction_info.m_ActionInfo.m_TenderCost;
    auction_map.insert(make_pair(key_value, db_auction_info));
#endif

    return true;
}

void 
DominationManager::SendDominationInfo(GameServerSession* pServerSession, MAPCODE map_code)
{
    AreaInfoMap::iterator find_iterator = domination_area_info_map_.find(map_code);
    DOMINATION_INFO domination_info = find_iterator->second;

    MSG_GZ_DOMINATION_INFO_ACK send_msg;
    send_msg.m_DominationInfo = domination_info;
    pServerSession->SendPacket(&send_msg, sizeof(send_msg));
}

void 
DominationManager::SendActionList(GameServerSession* server_session, MAPCODE map_code)
{
    MSG_GZ_DOMINATION_AUCTION_INFO_ACK send_msg;

    AllAreaAuctionInfoMap::iterator find_iterator = all_area_auction_info_map_.find(map_code);

    if (find_iterator == all_area_auction_info_map_.end())
    {
        send_msg.m_MapCode = map_code;
        send_msg.m_DataCount = 0;
    }
    else
    {
    #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        DominationAuctionInfoList auction_list = find_iterator->second;
        typedef DominationAuctionInfoList::const_reverse_iterator ReverseIterator;
        ReverseIterator reverse_it = auction_list.rbegin(),
                        rend = auction_list.rend();
    #else
        DominationAuctionInfoMap& auction_map = find_iterator->second;
        typedef DominationAuctionInfoMap::const_reverse_iterator ReverseIterator;
        ReverseIterator reverse_it = auction_map.rbegin(),
                        rend = auction_map.rend();
    #endif

        int index = 0;
        for (; index < _countof(send_msg.m_ActionInfo) && reverse_it != rend; ++index, ++reverse_it)
        {
        #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
            const DominationAuctionInfo& db_auction_info = *reverse_it;
        #else
            const DominationAuctionInfo& db_auction_info = reverse_it->second; 
        #endif
            send_msg.m_ActionInfo[index] = db_auction_info;
        }

        send_msg.m_MapCode = map_code;
        send_msg.m_DataCount = index;
    }
    server_session->SendPacket(&send_msg, send_msg.GetSize());
}

DOMINATION_INFO* 
DominationManager::FindDominationInfo(MAPCODE map_code)
{
    AreaInfoMap::iterator find_iterator = domination_area_info_map_.find(map_code);

    if (find_iterator == domination_area_info_map_.end())
    {
        return NULL;
    }

    return &(find_iterator->second);
}

void 
DominationManager::ClearDominationActionInfo(MAPCODE map_code)
{
    AllAreaAuctionInfoMap::iterator find_iterator = all_area_auction_info_map_.find(map_code);

    if (find_iterator == all_area_auction_info_map_.end())
    {
        SUNLOG( eCRITICAL_LOG, __FUNCTION__":Not found map : %d", map_code );
        return;
    }

    find_iterator->second.clear();
}

bool 
DominationManager::SetListForReturnHeim(MAPCODE map_code, 
                                        DOMINATION_RETURNMONEY_INFO* OUT master_info,
                                        DWORD max_index,
                                        int& OUT count_index)
{
    AllAreaAuctionInfoMap::iterator find_iterator = all_area_auction_info_map_.find(map_code);
    if (find_iterator == all_area_auction_info_map_.end())
    {
        SUNLOG( eCRITICAL_LOG, __FUNCTION__":Not found map : %d", map_code );
        return false;
    }

#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    DominationAuctionInfoList& auction_list = find_iterator->second;
    DominationAuctionInfoList::iterator begin_iterator = auction_list.begin(),
                                        end_iterator = auction_list.end();
    if( auction_list.size() > max_index )
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Data count is over : %d, %d", map_code, auction_list.size());
        return false;
    }

    count_index = static_cast<int>(auction_list.size());
#else
    DominationAuctionInfoMap& auction_map = find_iterator->second;
    DominationAuctionInfoMap::iterator begin_iterator = auction_map.begin(),
                                       end_iterator = auction_map.end();
    if( auction_map.size() > max_index )
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Data count is over : %d, %d", map_code, auction_map.size());
        return false;
    }

    count_index = static_cast<int>(auction_map.size());
#endif

    if (begin_iterator == end_iterator)
    {
        return false;
    }

    for(int i=0; i<count_index; ++i, ++begin_iterator)
    {
    #ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        const DominationAuctionInfo& db_auction_info = *begin_iterator; 
    #else
        const DominationAuctionInfo& db_auction_info = begin_iterator->second; 
    #endif
        strncpy(master_info[i].m_tszMaster, db_auction_info.m_tszGuildMaster, 
                _countof(master_info[i].m_tszMaster));
        master_info[i].m_tszMaster[_countof(master_info[i].m_tszMaster) - 1] = '\0';
        master_info[i].m_TenderCost = db_auction_info.m_ActionInfo.m_TenderCost;
        master_info[i].m_CostType = SYSMEMO_DOMINATION_RETURN_MONEY;
    }
    return true;
}

#ifdef _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
void
DominationManager::RequestDominationMemberInfo()
{
    
    MemberLoadCheckMap::iterator begin_iterator = member_info_loading_check_map_.begin(),
                                 end_iterator = member_info_loading_check_map_.end();

    for(; begin_iterator != end_iterator; ++begin_iterator)
    {
        bool is_load = begin_iterator->second;

        if (is_load == false)
        {
            MAPCODE map_code = begin_iterator->first;

            MSG_DZ_DOMINATION_MEMBERLIST_REQ send_msg;
            send_msg.map_code_ = map_code;
            SendToDBProxy(&send_msg, sizeof(send_msg));
        }
    }
    SetLoadDominationMemberInfo(true);
}

void 
DominationManager::SetCheckMemberInfo(MAPCODE map_code)
{
    MemberLoadCheckMap::iterator find_iterator = 
        member_info_loading_check_map_.find(map_code);

    if (find_iterator != member_info_loading_check_map_.end())
    {
        find_iterator->second = true;
    }
}

void 
DominationManager::SetCheckDominationMemberMap()
{
    ScriptInfoMap& script_info_map = 
        DominationInfoParser::Instance()->GetDominationInfoMap();

    ScriptInfoIterator begin_iterator = script_info_map.begin(),
                       end_iterator = script_info_map.end();

    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const sDOMINATION_INFO& script_info = begin_iterator->second;
        member_info_loading_check_map_.insert(std::make_pair(script_info.m_DominationMapCode, false));
    }
}

void
DominationManager::InsertDominationMemberInfo(MAPCODE map_code, const DominationMemberInfo& member_info)
{
    //gamelulu_dev_note : 중복된 멤버 데이터가 존재하는건 비정상적인 상황이다.
    //만약에 이러한 비정상적인 상황을 미리 대비하여 예외 처리를 하여 insert를 하지 않는다면
    //이러한 비정상 적인 상황을 모르고 지나칠수 있다. 일단 예외 처리를 하지 않고 입력을 받는다.
    member_info_map_.insert(std::make_pair(map_code, member_info));
}

void 
DominationManager::DeleteDominationMemberInfo(MAPCODE map_code, CHARGUID member_guid)
{
    MemberMap::_Pairii range = member_info_map_.equal_range(map_code);
    MemberMap::iterator begin_iterator = range.first,
                        end_iterator = range.second;

    for (;begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationMemberInfo& member_info = begin_iterator->second;
        if (member_info.member_guid_ == member_guid)
        {
            member_info_map_.erase(begin_iterator);
            return;
        }
    }
}

void 
DominationManager::SendMemberList(GameServerSession* server_session, MAPCODE map_code)const
{
    MemberMap::_Paircc range = member_info_map_.equal_range(map_code);
    MemberMap::const_iterator begin_iterator = range.first,
                              end_iterator = range.second;

    MSG_GZ_DOMINATION_MEMBERLIST_ANS send_msg;

    BYTE index = 0;
    const BYTE MaxList = _countof(send_msg.member_list_);
    for (; begin_iterator != end_iterator; ++index, ++begin_iterator)
    {
        if (index > MaxList)
        {
            //gamelulu_dev_note : 만약 라이브 상에서 이와 같이 리스트 최대치를 넘길
            //경우라도 서비스는 계속 진행 되어야 하기 때문에 아래와 같이 처리한다.
            --index;
            assert(!"Member count is over");
            SUNLOG(eCRITICAL_LOG, __FUNCTION__":Member count is over : %d", index);
            break;
        }
        const DominationMemberInfo& src_info = begin_iterator->second;
        DominationMemberInfo& dest_info = send_msg.member_list_[index];
        dest_info = src_info;
    }

    send_msg.map_code_ = map_code;
    send_msg.data_count_ = index;
    server_session->SendPacket(&send_msg, send_msg.GetSize());
}

void 
DominationManager::CheckCompleteLoadMemberData()
{
    MemberLoadCheckMap::iterator begin_iterator = member_info_loading_check_map_.begin(),
                                 end_iterator = member_info_loading_check_map_.end();

    for(; begin_iterator != end_iterator; ++begin_iterator)
    {
        const bool& is_load = begin_iterator->second;

        if (is_load == false)
        {
            return;
        }
    }

    SUNLOG(eCRITICAL_LOG,"Complete member data load");
    SetLoadDominationMemberInfo(true);

}
#endif//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN