#include "stdafx.h"
#include "DominationAuction.h"
#include "PacketStruct_GZ.h"

#ifdef _NA001385_20090924_DOMINATION

DominationAuction::DominationAuction(MAPCODE map_code)
:is_load_data_(false)
,map_code_(false)
{
    map_code_ = map_code;
}

DominationAuction::~DominationAuction()
{
}

bool
DominationAuction::IsApplyToGuild(GUILDGUID guild_guid)const
{
    return (_GetInfo(guild_guid) == NULL) ? false : true;
}

void 
DominationAuction::ClearAuctionInfo()
{
    auction_info_map_.clear();
}

void 
DominationAuction::DeleteAuctionInfo(GUILDGUID guild_guid)
{
    AuctionInfoIterator find_iterator = auction_info_map_.find(guild_guid);

    if (find_iterator == auction_info_map_.end())
    {
        return;
    }

    auction_info_map_.erase(find_iterator);
}

BYTE 
DominationAuction::GetAuctionList(DOMINATION_ACTION_INFO* OUT auction_list, BYTE max_count)const
{
    if (max_count > MAX_DOMINATION_ACTIONLIST)
    {
        assert(!"Incorrect Request auction list count");
        SUNLOG(eCRITICAL_LOG,__FUNCTION__"Incorrect Request auction list count : %d", max_count);
        return 0;
    }

    if (auction_info_map_.size() > MAX_DOMINATION_AUCTION)
    {
        assert(!"Incorrect data count");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__"Incorrect data count : %d", auction_info_map_.size());
        return 0;
    }

    ConstAuctionInfoIterator begin_iterator = auction_info_map_.begin(),
                             end_iterator = auction_info_map_.end();
    
    //1.멀티맵에 정렬 : 동일한 금액<KEY>이 존재 가능(버전별)
    typedef STLX_MULTIMAP<MONEY, const DOMINATION_ACTION_INFO*> SortMap;
    SortMap sort_map;
    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationAuctionInfo& auction_info = begin_iterator->second;
        const DOMINATION_ACTION_INFO& insert_info = auction_info.m_ActionInfo;
        sort_map.insert(std::make_pair(insert_info.m_TenderCost, &insert_info));
    }
    
    //2.리스트 작성 : 높은 금액 부터 리스트 작성
    SortMap::reverse_iterator rbegin_iterator = sort_map.rbegin(),
                              rend_iterator = sort_map.rend();
    int index = 0;
    for (; index < max_count && rbegin_iterator != rend_iterator; ++index, ++rbegin_iterator)
    {
        const DOMINATION_ACTION_INFO* list_info = rbegin_iterator->second;
        auction_list[index] = *list_info;
#ifdef _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST
        auction_list[index].m_TenderCost = 0;
#endif
    }
    
    return index;
}

void 
DominationAuction::SetInfo(const DominationAuctionInfo* auction_info, int data_count)
{
    if (data_count < 0 || data_count > MAX_DOMINATION_AUCTION)
    {
        assert(!"Auction data count is incorrect");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Data count is incorrect[%d]", data_count);
        return;
    }

    for (int i=0; i<data_count; ++i)
    {
        _InsertData(auction_info[i]);
    }

    _IsLoadData(true);
}

void 
DominationAuction::RequestData()const
{
    MSG_GZ_DOMINATION_AUCTION_INFO_SYN send_msg;
    send_msg.m_MapCode = GetMapCode();
    g_pGameServer->SendToServer(GUILD_SERVER, &send_msg, sizeof(send_msg));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_log_trace_domination",
            _T("|["__FUNCTION__"]|{mapcode}="), send_msg.m_MapCode)));
}

eDOMINATION_TENDER_TYPE
DominationAuction::EntryAuction(const DominationAuctionInfo& auction_info)
{
    const DominationAuctionInfo* find_info = _GetInfo(auction_info.m_ActionInfo.m_GuilGuid);

    if (find_info == NULL)
    {
        //신규
        _InsertData(auction_info);
        return NEW_TENDER_COST;
    }
    else
    {
        //중복
        _ChangeData(auction_info);
        return DUP_TENDER_COST;
    }
}

MONEY 
DominationAuction::GetTenderCost(GUILDGUID guild_guid)const
{
    const DominationAuctionInfo* auction_info = _GetInfo(guild_guid);
    if (auction_info == NULL)
    {
        return 0;
    }

    return auction_info->m_ActionInfo.m_TenderCost;
}

MONEY 
DominationAuction::GetBestTenderCost()const
{
    const DominationAuctionInfo* auction_info = _FindBestCostGuild();

    if (auction_info == NULL)
    {
        return 0;
    }

    return auction_info->m_ActionInfo.m_TenderCost;
}

const DominationAuctionInfo* 
DominationAuction::SelectAttackGuild()const
{
#ifdef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
    return _ProbabilityFindGuild();
#else
    return _FindBestCostGuild();
#endif
}

namespace util {
;

struct CalculateAuctionPoint
{
    GUILDGUID guild_guid_;
    double token_value_;//길드의 포인트
    double calculation_point_;//선정 영역 포인트
};

}; //end of namespace

#if defined (_NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST) || defined (_RUSSIA)
const DominationAuctionInfo* DominationAuction::_ProbabilityFindGuild() const
{
    const DominationAuctionInfo* best_domination_auction_info = NULL;

    for (AuctionInfoMap::const_iterator it = auction_info_map_.begin(); 
        it != auction_info_map_.end(); ++it)
    {
        // 첫 번째 항목을 선택한다.
        const DominationAuctionInfo& domination_auction_info = it->second;
        const DOMINATION_ACTION_INFO& auction_info = domination_auction_info.m_ActionInfo;
        if (best_domination_auction_info == NULL)
        {
            best_domination_auction_info = &domination_auction_info;
            continue;
        }
        
        // 입찰 개수가 큰쪽을 선택한다.
        const DOMINATION_ACTION_INFO& best_auction_info = best_domination_auction_info->m_ActionInfo;
        if (auction_info.m_TenderCost > best_auction_info.m_TenderCost)
        {
            best_domination_auction_info = &domination_auction_info;
            continue;
        }

        // 입찰 개수가 같을 경우 입찰 시간을 비교하여 먼저 입찰한 쪽을 선택한다.
        if (auction_info.m_TenderCost == best_auction_info.m_TenderCost)
        {
            const std::string time(
                auction_info.m_tszActionApplyTime, _countof(auction_info.m_tszActionApplyTime));

            const std::string best_time(
                best_auction_info.m_tszActionApplyTime, _countof(best_auction_info.m_tszActionApplyTime));
            
            if (time < best_time)
            {
                best_domination_auction_info = &domination_auction_info;
                continue;
            }
        }
    }
    return best_domination_auction_info;
}
#else
const DominationAuctionInfo* 
DominationAuction::_ProbabilityFindGuild()const
{
    typedef STLX_LIST<util::CalculateAuctionPoint> AuctionPointList;
    typedef AuctionPointList::iterator AuctionPointListIterator;

    ConstAuctionInfoIterator begin_iterator = auction_info_map_.begin(),
                             end_iterator = auction_info_map_.end();

    if (begin_iterator == end_iterator)
    {
        return NULL;
    }

    AuctionPointList auction_point_list;
    double add_point = 0;

    //1. 입찰 아이템 갯수의 재곱(또는 3제곱)으로 포인트 산정
    for (; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationAuctionInfo& db_auction_info =  begin_iterator->second;
        const DOMINATION_ACTION_INFO& auction_info = db_auction_info.m_ActionInfo;
        double calculate_point = static_cast<double>(auction_info.m_TenderCost);

        util::CalculateAuctionPoint auction_point;

        auction_point.guild_guid_ = db_auction_info.m_ActionInfo.m_GuilGuid;

#if defined (_CHINA) || defined (_RUSSIA) || defined (_KOREA)
        auction_point.token_value_ = pow(calculate_point, 3);
#else
        auction_point.token_value_ = pow(calculate_point, 2);
#endif

        auction_point.calculation_point_ = auction_point.token_value_+add_point;
        add_point = auction_point.calculation_point_;

        auction_point_list.push_back(auction_point);
    }

    //2. 1/10000 렌덤값 선택후 선택값에 맞게 적용
    const int max = 10000;
    double select_value = static_cast<double>(random(1, max));
    select_value *= (add_point / max);

    //3. 선택값에 맞는 길드 선택
    AuctionPointListIterator begin_point_iterator = auction_point_list.begin(),
                             end_point_iterator = auction_point_list.end();

    for (;begin_point_iterator != end_point_iterator; ++begin_point_iterator)
    {
        const util::CalculateAuctionPoint& auction_point = *(begin_point_iterator);
        double token_min = (auction_point.calculation_point_) - (auction_point.token_value_);
        double token_max = auction_point.calculation_point_;

        if (token_min < select_value && token_max >= select_value)
        {
            return _GetInfo(auction_point.guild_guid_);
        }
    }

    return NULL;
}
#endif

const DominationAuctionInfo*
DominationAuction::_FindBestCostGuild()const
{
    ConstAuctionInfoIterator begin_iterator = auction_info_map_.begin(),
                             end_iterator = auction_info_map_.end();

    if (begin_iterator == end_iterator)
    {
        return NULL;
    }

    const DominationAuctionInfo* best_data = &(begin_iterator->second);

    for(++begin_iterator; begin_iterator != end_iterator; ++begin_iterator)
    {
        const DominationAuctionInfo* comp_data = &(begin_iterator->second);

        if(_Comp(best_data,comp_data) == false)
        {
            //비교 데이터가 크다
            best_data = comp_data;
        }
    }
    
    return best_data;
}

bool DominationAuction::_Comp(
    const DominationAuctionInfo* first_data, const DominationAuctionInfo* second_data) const
{
    const DOMINATION_ACTION_INFO& first_auction_info = first_data->m_ActionInfo;
    const MONEY first_cost = first_auction_info.m_TenderCost;

    const DOMINATION_ACTION_INFO& second_auction_info = second_data->m_ActionInfo;
    const MONEY second_cost = second_auction_info.m_TenderCost;
    
    if (first_cost == second_cost)
    {
        // 입찰 개수가 같을 경우 입찰 시간을 비교하여 먼저 입찰한 쪽이 우선권을 갖는다.
        const std::string first_time(first_auction_info.m_tszActionApplyTime, _countof(first_auction_info.m_tszActionApplyTime));
        const std::string second_time(second_auction_info.m_tszActionApplyTime, _countof(second_auction_info.m_tszActionApplyTime));
        return (first_time < second_time);        
    }
    return (first_cost > second_cost);
}

void 
DominationAuction::_InsertData(const DominationAuctionInfo& auction_info)
{
    GUILDGUID guild_guid = auction_info.m_ActionInfo.m_GuilGuid;
    auction_info_map_.insert(std::make_pair(guild_guid, auction_info));
}

void
DominationAuction::_ChangeData(const DominationAuctionInfo& auction_info)
{
    GUILDGUID guild_guid = auction_info.m_ActionInfo.m_GuilGuid;
    AuctionInfoIterator find_iterator = auction_info_map_.find(guild_guid);

    auction_info_map_.erase(find_iterator);
    auction_info_map_.insert(std::make_pair(guild_guid, auction_info));
}

const DominationAuctionInfo*
DominationAuction::_GetInfo(GUILDGUID guild_guid)const
{
    ConstAuctionInfoIterator find_iterator =  auction_info_map_.find(guild_guid);

    if (find_iterator == auction_info_map_.end())
    {
        return NULL;
    }

    return &(find_iterator->second);
}

DominationAuctionInfo*
DominationAuction::_AccessInfo(GUILDGUID guild_guid)
{
    AuctionInfoIterator find_iterator =  auction_info_map_.find(guild_guid);

    if (find_iterator == auction_info_map_.end())
    {
        return NULL;
    }

    return &(find_iterator->second);
}

void 
DominationAuction::SendRewardMemo(SLOTCODE item_code, eSYSTEM_MEMO_TYPE memo_type)const
{
    SystemMemoInfoOfDomination request_memo_info[MAX_SYSTEM_MEMO_LIST];

    ConstAuctionInfoIterator begin_iterator = auction_info_map_.begin(),
                             end_iterator = auction_info_map_.end();
    DWORD count = 0;
    for(; count <= MAX_SYSTEM_MEMO_LIST, begin_iterator!= end_iterator; ++count, ++begin_iterator)
    {
        const DominationAuctionInfo& auction_info = begin_iterator->second;

        SystemMemoInfoOfDomination& memo_info = request_memo_info[count];

        memo_info.map_code_ = auction_info.m_ActionInfo.m_FieldCode;
        memo_info.amount_cost_ = auction_info.m_ActionInfo.m_TenderCost;
        memo_info.recv_item_code_ = item_code;
        memo_info.system_memo_type_ = memo_type;

        strncpy(memo_info.guild_name_, auction_info.m_ActionInfo.m_tszGuildName, 
            _countof(memo_info.guild_name_));
        memo_info.guild_name_[_countof(memo_info.guild_name_)-1] = '\0';

        strncpy(memo_info.recv_user_name_, auction_info.m_tszGuildMaster, 
            _countof(memo_info.recv_user_name_));
        memo_info.recv_user_name_[_countof(memo_info.recv_user_name_) - 1] = '\0';
    }

    if (count > 0)
    {
        SystemMemo& system_memo = const_cast<SystemMemo&>(system_memo_);
        system_memo.SendSystemMemo(request_memo_info, count, memo_type);
    }
}

BYTE
DominationAuction::CountAuctionList()const
{
    return static_cast<BYTE>(auction_info_map_.size());
}

#endif//_NA001385_20090924_DOMINATION