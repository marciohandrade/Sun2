#include "StdAfx.h"
#include "misc.h"
#include "DominationInfoParser.h"
#include <SolarDateTime.h>
#include <ItemOptionHeader.h>

DominationInfoParser::DominationInfoParser(void)
#ifdef _SERVER
: is_script_mode_(true)
, is_loaded_(false)
#endif
{
}

DominationInfoParser::~DominationInfoParser(void)
{
}

BOOL DominationInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch( script_code )
    {
    case SCRIPT_DOMINATIONINFO:
        return (LoadDominationInfo((is_reload == TRUE)) ? TRUE : FALSE);
    case SCRIPT_DOMINATION_EVENT_INFO:
        return (LoadEventInfo((is_reload == TRUE)) ? TRUE : FALSE);
    }
    return FALSE;
}

bool DominationInfoParser::LoadDominationInfo(const bool /*is_reload*/)
{
    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        sDOMINATION_INFO domination_info;

        GetData(row, domination_info.m_DominationMapCode, "MapCode");
        GetData(row, domination_info.m_LinkMapCode, "FieldCode");
        GetData(row, domination_info.m_SubMapCode, "SubFieldCode");
        GetData(row, domination_info.m_Index, "Index");
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        GetData(row, domination_info.m_DominationType, "DominationType");
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
        GetData(row, domination_info.m_MaxUser, "MaxUser");
        GetData64(row, domination_info.m_BaseBattingMoney, "BaseBattingMoney");
        GetData(row, domination_info.m_BattingCharge, "BattingChargeRate");

        // added by _NA_0_20100705_DOMINATION_EVENT = {
        GetData(row, domination_info.m_byUseFixEvent, "UseFixEvent");
        GetData(row, domination_info.m_wEventDayOfWeek, "EventDayOfWeek");
        GetData(row, domination_info.m_dwEventTime, "EventTime");
        GetDataWZID(row, domination_info.m_dwEventTriggerID, "EventTriggerID");
        // }
        GetDataWZID(row, domination_info.m_StartTriggerID, "StartTriggerID");
        GetDataWZID(row, domination_info.m_EndTriggerID, "EndTriggerID");

        for (int i = 0; i < sDOMINATION_INFO::GATEID_MAX; ++i)
        {
            GetData(row, domination_info.m_GateID[i], "GateID%d", i+1);
        }
            
        GetData(row, domination_info.m_GateHPTunning, "GateHPtunning");
        GetData(row, domination_info.m_RespawnDelay, "RespawnDelay");

        for (int i = 0; i < sDOMINATION_INFO::LIMIT_MAX; ++i)
        {
            GetData(row, domination_info.m_LimitSkillGroup[i], "LimitSkillGroup%d", i+1);
            GetData(row, domination_info.m_LimitItemGroup[i], "LimitItemGroup%d", i+1);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        }
        for (int i = 0; i < sDOMINATION_INFO::REWARD_OPTION_MAX; ++i)
        {
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
            AttrOptionInfo& option_info = domination_info.m_RewardOption[i];
            GetData(row, option_info.m_wOptionKind, "RewardOptionKind%d", i+1);
            GetData(row, option_info.m_byOptionType, "RewardOptionType%d", i+1);
            GetData(row, option_info.m_iOptionValue, "RewardOptionValue%d", i+1);
        }

        GetData(row, domination_info.m_RewardDropHeimRate, "RewardDropHeimRate");

        GetData(row, domination_info.m_BattingItemCode, "BattingItemCode");
#ifdef _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST
        GetData(row, domination_info.m_BattingGuildLevelLimit, "BattingGuildLevelLimit");
#endif

        if (IsValidDominationInfo(domination_info, row+1) == false)
        {
            ASSERT(false);
            continue;
        }
        
        script_infos_[domination_info.m_DominationMapCode] = domination_info;        
        
        typedef std::pair<MAPCODE, MAPCODE> MapCodePair;
        if (domination_info.m_DominationMapCode != 0)
        {
            map_codes_.insert(
                MapCodePair(domination_info.m_DominationMapCode, domination_info.m_DominationMapCode));
        }
        if (domination_info.m_LinkMapCode != 0)
        {
            map_codes_.insert(
                MapCodePair(domination_info.m_LinkMapCode, domination_info.m_DominationMapCode));
        }
        if (domination_info.m_SubMapCode != 0)
        {
            map_codes_.insert(
                MapCodePair(domination_info.m_SubMapCode, domination_info.m_DominationMapCode));
        }
    }
    is_loaded_ = true;
    return true;
}

bool DominationInfoParser::LoadEventInfo(const bool /*is_reload*/)
{
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //기존의 이벤트는 dayOfweek(요일)을 인덱스로 구분하여 저장한다.
    //몬트샤인과 관련된 수정 뿐만아니라 이벤트와 필드를 매칭시켜 다양한 시간의 점령전을 위해
    //이벤트 자체에 Index를 두고 이 Index로 구분하여 event_infos_에 저장한다.
    const char* field_name[DOMINATION_EVENT_MAX] = {
        "AuctionStart",
        "AuctionEnd", 
        "SelectAttackGuild",
        "SelectAttackGuildEnd",
        "SystemResetTime",
        "WaitingTime",
        "BattleStartingTime",
        "BattleEndTime",
        "AuctionStart2" 
    };

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        sDOMINATION_EVENT_INFO event_info;

        GetData(row, event_info.m_wEventIndex, "Index");
        GetData(row, event_info.m_wMapCode, "MapCode");
        GetData(row, event_info.m_wDayOfWeek, "DayOfWeek");

        //점령전에서 경매 시스템을 사용하지 않으므로 스크립트에서 삭제하고 아래와 같이 상수로 처리한다.
        event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START] = DOMINATION_EVENT_AUCTION_START_TIME;
        event_info.m_dwTime[DOMINATION_EVENT_AUCTION_END] = DOMINATION_EVENT_AUCTION_END_TIME;
        event_info.m_dwTime[DOMINATION_EVENT_SELECT_ATTACKGUILD] = DOMINATION_EVENT_SELECT_ATTACKGUILD_TIME;
        event_info.m_dwTime[DOMINATION_EVENT_SELECT_ATTACKGUILD_END] = DOMINATION_EVENT_SELECT_ATTACKGUILD_END_TIME;

        GetData(row, event_info.m_dwTime[DOMINATION_EVENT_SYSTEM_RESET], "SystemResetTime");
        GetData(row, event_info.m_dwTime[DOMINATION_EVENT_WAIT], "WaitingTime");
        GetData(row, event_info.m_dwTime[DOMINATION_EVENT_START], "BattleStartingTime");
        GetData(row, event_info.m_dwTime[DOMINATION_EVENT_END], "BattleEndTime");

        event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START2] = DOMINATION_EVENT_AUCTION_START2_TIME;

        if (IsValidEventInfo(event_info))
        {
            event_infos_.insert(std::make_pair(event_info.m_wEventIndex, event_info));
        }            
    }

    return true;
#else
    const char* field_name[DOMINATION_EVENT_MAX] = {
        "AuctionStart",
        "AuctionEnd", 
        "SelectAttackGuild",
        "SelectAttackGuildEnd",
        "SystemResetTime",
        "WaitingTime",
        "BattleStartingTime",
        "BattleEndTime",
        "AuctionStart2" 
    };

    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        sDOMINATION_EVENT_INFO event_info;

        GetData(row, event_info.m_wDayOfWeek, "DayOfWeek");
        for (int i = 0; i < DOMINATION_EVENT_MAX; ++i)
        {
            GetData(row, event_info.m_dwTime[i], field_name[i]);
        }
            
        if (IsValidEventInfo(event_info))
        {
            event_infos_.insert(std::make_pair(event_info.m_wDayOfWeek, event_info));
        }            
    }

    return true;
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
}

bool DominationInfoParser::IsValidDominationInfo(
    const sDOMINATION_INFO& domination_info, const int row) const
{
    if (row != domination_info.m_Index)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|입력된 인덱스가 올바르지 않습니다.|Row = %d, Index = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            row, 
            domination_info.m_Index
        );
        ASSERT(false);
        return false;
    }

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    for (int i = 0; i < sDOMINATION_INFO::REWARD_OPTION_MAX; ++i)
#else
    for (int i = 0; i < sDOMINATION_INFO::LIMIT_MAX; ++i)
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    {
        const AttrOptionInfo& option_info = domination_info.m_RewardOption[i];

        const WORD option_index = option_info.m_wOptionKind;
        const BYTE option_type = option_info.m_byOptionType;
        const int option_value = option_info.m_iOptionValue;

        if (CheckOptionValidity(option_index, option_type, option_value) == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|보상 버프 옵션이 올바르지 않습니다.|Row = %d, Index = %d, OptionIndex = %d, OptionType = %d, OptionValue = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                row, 
                domination_info.m_Index, 
                option_index, 
                option_type, 
                option_value
            );
            ASSERT(false);
            return false;
        }
    }

    return true;
}

bool DominationInfoParser::IsValidEventInfo(const sDOMINATION_EVENT_INFO& eventInfo) const
{
    if (eventInfo.m_wDayOfWeek > 6)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|%d|요일이 올바르지 않습니다.|DayOfWeek = %d|"), 
            __FUNCTION__, 
            __LINE__, 
            eventInfo.m_wDayOfWeek
        );
        ASSERT(false);
        return false;
    }

    const DWORD* times = eventInfo.m_dwTime;    
    for (int i = DOMINATION_EVENT_AUCTION_START; i < DOMINATION_EVENT_AUCTION_START2; ++i, ++times)
    {
        const DWORD start_time = *times;
        const DWORD end_time = *(times+1);

        if (((start_time + end_time) != 0) && start_time >= end_time)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|시작 시간이 종료 시간보다 큽니다.|DayOfWeek = %d, StartTime = %d, EndTime = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                eventInfo.m_wDayOfWeek, 
                start_time, 
                end_time
            );
            ASSERT(false);            
            return false;
        }

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        if (GetCurEventState(eventInfo.m_wDayOfWeek, start_time, 
                             eventInfo.m_wMapCode) != DOMINATION_EVENT_MAX)
#else
        if (GetCurEventState(eventInfo.m_wDayOfWeek, start_time) != DOMINATION_EVENT_MAX)
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|이미 존재하는 시간대 입니다.|DayOfWeek = %d, StartTime = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                eventInfo.m_wDayOfWeek, 
                start_time
            );
            ASSERT(false);
            return false;
        }
    }

    return true;
}

const sDOMINATION_INFO* DominationInfoParser::GetDominationInfo(const MAPCODE field_code) const
{
    const MAPCODE map_code = GetDominationFieldCode(field_code);
    DOMINATION_SCRIPT_INFO_MAP::const_iterator it = script_infos_.find(map_code);
    if (it != script_infos_.end())
    {
        return &(it->second);
    }        
    return NULL;
}

MAPCODE DominationInfoParser::GetDominationFieldCode(const MAPCODE field_code) const
{
    MapCodes::const_iterator it = map_codes_.find(field_code);
    if (it != map_codes_.end())
    {
        return it->second;
    }    
    return 0;
}

MAPCODE DominationInfoParser::GetDominationLinkMapCode(const MAPCODE map_code) const
{
    DOMINATION_SCRIPT_INFO_MAP::const_iterator it = script_infos_.find(map_code);
    if (it != script_infos_.end())
    {
        const sDOMINATION_INFO& domination_info = it->second;
        return domination_info.m_LinkMapCode;
    }
    return 0;
}

MONEY DominationInfoParser::GetAreaLimitMoney(const MAPCODE map_code) const
{
    DOMINATION_SCRIPT_INFO_MAP::const_iterator it = script_infos_.find(map_code);
    if (it == script_infos_.end())
    {
        return 0;
    }
    const sDOMINATION_INFO& domination_info = it->second;
    return domination_info.m_BaseBattingMoney;
}

const sDOMINATION_EVENT_INFO* DominationInfoParser::GetEventInfo(
    const WORD day_of_week, const DWORD time) const
{
    std::pair< EventInfos::const_iterator, EventInfos::const_iterator > range = 
        event_infos_.equal_range(day_of_week);
#ifdef _SERVER
    if (is_script_mode_ == false)
    {
        range = gm_event_infos_.equal_range(day_of_week);
    }
#endif

    for(EventInfos::const_iterator it = range.first; it != range.second; ++it)
    {
        const sDOMINATION_EVENT_INFO& event_info = it->second;
        const DWORD start_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START];
        const DWORD end_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START2];
        const bool is_include = Between(time, start_time, end_time);
        if (is_include)
        {
            return &event_info;
        }            
    }

    return NULL;
}

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
const sDOMINATION_EVENT_INFO* DominationInfoParser::GetGMEventInfo(const WORD day_of_week, 
                                                                   const DWORD time,
                                                                   const MAPCODE map_code) const
{
    std::pair< EventInfos::const_iterator, EventInfos::const_iterator > range;

#ifdef _SERVER
    if (is_script_mode_ == false)
    {
        range = gm_event_infos_.equal_range(day_of_week);
    }
    else
    {
        return NULL;
    }
#endif

    for(EventInfos::const_iterator it = range.first; it != range.second; ++it)
    {
        const sDOMINATION_EVENT_INFO& event_info = it->second;

        if(event_info.m_wMapCode != 0 && event_info.m_wMapCode != map_code)
        {
            continue;
        }

        const DWORD start_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START];
        const DWORD end_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START2];
        const bool is_include = Between(time, start_time, end_time);
        if (is_include)
        {
            return &event_info;
        }            
    }

    return NULL;
}

//점령전 이벤트 정보를 해당 필드에서 독립적으로 가지고 있기 위해 아래와 같은 함수를 추가하였다.
STLX_MULTIMAP<WORD, sDOMINATION_EVENT_INFO> DominationInfoParser::GetEventInfosFromMapCode(MAPCODE map_code)
{
    EventInfos::const_iterator event_infos_it =  event_infos_.begin();
    EventInfos::const_iterator event_infos_end = event_infos_.end();
    EventInfos event_infos;

    //등록되어진 이벤트를 순회한다.
    for ( ;event_infos_it != event_infos_end; ++event_infos_it)
    {
        const sDOMINATION_EVENT_INFO& event_info = event_infos_it->second;
        //해당 점령지의 맵코드와 이벤트의 맵 코드를 체크한다
        if (event_info.m_wMapCode != map_code)
        {
            continue;
        }

        event_infos.insert(std::make_pair(event_info.m_wEventIndex, event_info));
    }
    return event_infos;
}

eDOMINATION_EVENT_STATE DominationInfoParser::GetCurEventState(
    const WORD day_of_week, const DWORD time, const DWORD map_code) const
{
    EventInfos::const_iterator event_infos_it =  event_infos_.begin();
    EventInfos::const_iterator event_infos_end = event_infos_.end();
    
    //등록되어진 이벤트를 순회한다.
    for ( ;event_infos_it != event_infos_end; ++event_infos_it)
    {
        const sDOMINATION_EVENT_INFO& event_info = event_infos_it->second;
        //오늘과 같은 요일인지 체크한다.
        if (event_info.m_wDayOfWeek != day_of_week)
        {
            continue;
        }
        //해당 점령지의 코드와 이벤트의 점령전 맵 코드를 체크한다
        if (event_info.m_wMapCode != map_code)
        {
            continue;
        }
        //현재 시간이 이벤트의 시간에 포함되는지 확인한다.
        const DWORD start_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START];
        const DWORD end_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START2];
        const bool is_include = Between(time, start_time, end_time);
        if (is_include == false)
        {
            continue;
        }
        //이벤트의 시간에 포함된다면 현재 시간에 해당하는 이벤트 상태를 리턴한다
        const DWORD* times = event_info.m_dwTime;
        for (int cur_event = DOMINATION_EVENT_AUCTION_START; 
            cur_event < DOMINATION_EVENT_AUCTION_START2; ++cur_event, ++times)
        {
            const DWORD start_time = *times;
            const DWORD end_time = *(times+1);
            const bool is_include_current = Between(time, start_time, end_time);
            if (is_include_current == false)
            {
                continue;
            }
            return (eDOMINATION_EVENT_STATE)cur_event;
        }
    }
    return DOMINATION_EVENT_MAX;
}
#else
eDOMINATION_EVENT_STATE DominationInfoParser::GetCurEventState(
    const WORD day_of_week, const DWORD time) const
{
    const sDOMINATION_EVENT_INFO* const event_info = GetEventInfo(day_of_week,time);
    if (event_info != NULL)
    {
        const DWORD* times = event_info->m_dwTime;
        for (int cur_event = DOMINATION_EVENT_AUCTION_START; 
            cur_event < DOMINATION_EVENT_AUCTION_START2; ++cur_event, ++times)
        {
            const DWORD start_time = *times;
            const DWORD end_time = *(times+1);
            const bool is_include = Between(time, start_time, end_time);
            if (is_include)
            {
                return (eDOMINATION_EVENT_STATE)cur_event;
            }
        }
    }
    return DOMINATION_EVENT_MAX;
}
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

//_NA_0_20100705_DOMINATION_EVENT 
DWORD DominationInfoParser::GetRandomAuctionTime(const WORD day_of_week) const
{
    const size_t event_size = event_infos_.count(day_of_week);
    if (event_size <= 0)
    {
        return 0;
    }
        
    EventInfos::const_iterator it = event_infos_.lower_bound(day_of_week);    
    int it_index = random(0, event_size-1);
    while (it_index-- != 0)
    {
        ++it;
    }

    const sDOMINATION_EVENT_INFO& event_info = it->second;

    typedef std::pair<DWORD, DWORD> AuctionTime;
    std::vector< AuctionTime > auction_times;

    const DWORD start_time1 = 
        util::HHMMSSToSecond(event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START]);
    const DWORD end_time1 = 
        util::HHMMSSToSecond(event_info.m_dwTime[DOMINATION_EVENT_AUCTION_END]);
    if ((end_time1 - start_time1) >= 60)
    {
        auction_times.push_back(AuctionTime(start_time1, end_time1));
    }
    
    const DWORD start_time2 = 
        util::HHMMSSToSecond(event_info.m_dwTime[DOMINATION_EVENT_END]);
    const DWORD end_time2 = 
        util::HHMMSSToSecond(event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START2]);
    if ((end_time2 - start_time2) >= 60)
    {
        auction_times.push_back(AuctionTime(start_time2, end_time2));
    }
        
    if (auction_times.size() == 0)
    {
        return 0;
    }

    const int sel_index = random(1, auction_times.size())-1;
    const AuctionTime& sel_auction_time = auction_times[sel_index];
    return util::SecondToHHMMSS(random(sel_auction_time.first, sel_auction_time.second));
}

// 교전 전일경우 교전시간
// 교전 중일경우 남아 있는 교전 시간
// 교전 끝일경우 0
DWORD DominationInfoParser::GetWarTime(const WORD day_of_week, const DWORD time) const
{
    const sDOMINATION_EVENT_INFO* const event_info = GetEventInfo(day_of_week, time);
    if (event_info != NULL)
    {
        const DWORD start_time = event_info->m_dwTime[DOMINATION_EVENT_START];
        const DWORD end_time = event_info->m_dwTime[DOMINATION_EVENT_END];
        return util::CarcHHMMSS(end_time, max(start_time, time), false);
    }
    return 0;
}

// 교전 전일경우 교전 까지 남은 시간
// 교전 중일경우 0
// 교전 끝일경우 0
DWORD DominationInfoParser::GetRemindEventTime(
    const WORD day_of_week, const DWORD time, const eDOMINATION_EVENT_STATE event_state) const
{
    const sDOMINATION_EVENT_INFO* const event_info = GetEventInfo(day_of_week, time);
    if (event_info != NULL)
    {
        const DWORD start_time = event_info->m_dwTime[event_state];
        return util::CarcHHMMSS(start_time, time, false);
    }
    return 0;
}

#ifdef _SERVER
//서버전용(GM)
bool DominationInfoParser::GMInsertEventInfo(const sDOMINATION_EVENT_INFO& event_info)
{
    is_script_mode_ = false;
    gm_event_infos_.clear();

    if (IsValidEventInfo(event_info))
    {
        gm_event_infos_.insert(std::make_pair(event_info.m_wDayOfWeek, event_info));
        return true;
    }
    return false;
}

void DominationInfoParser::GMClearEventInfo()
{
    is_script_mode_ = true;
    gm_event_infos_.clear();
}
#endif  //_SERVER

#if defined (_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC) || \
    defined (_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION)
WORD DominationInfoParser::GetBattingItemCodeByMapCode(const MAPCODE field_code) const
{
    const MAPCODE map_code = GetDominationFieldCode(field_code);
    DOMINATION_SCRIPT_INFO_MAP::const_iterator it = script_infos_.find(map_code);
    if (it != script_infos_.end())
    {
        return (it->second.m_BattingItemCode);
    }
    return 0;
}
#endif

DominationInfoParser::SelectedRewardOptions DominationInfoParser::GetRewardOption(
    const DominationInfoParser::TakenMaps& taken_maps) const
{
    SelectedRewardOptions sel_options;

    for (TakenMaps::const_iterator map_it = taken_maps.begin(); map_it < taken_maps.end(); ++map_it)
    {
        const MAPCODE taken_map_code = *map_it;
        const sDOMINATION_INFO* const domination_info = GetDominationInfo(taken_map_code);
        if (domination_info == NULL)
        {
            continue;
        }
        for (int option_index = 0; 
            option_index < _countof(domination_info->m_RewardOption); 
            ++option_index)
        {
            const AttrOptionInfo& new_option_info = domination_info->m_RewardOption[option_index];
            const int option_kind = new_option_info.m_wOptionKind;
            if (option_kind == 0)
            {
                break;
            }
            SelectedRewardOptions::const_iterator option_it = sel_options.find(option_kind);
            if (option_it != sel_options.end())
            {
                // 같은 종류의 보상 옵션이 존재하면 큰값만 남긴다.
                const SelectedRewardOption& prev_option = option_it->second;
                const AttrOptionInfo* const prev_option_info = prev_option.option_info;
                if (new_option_info.m_iOptionValue > prev_option_info->m_iOptionValue)
                {
                    sel_options[option_kind] = 
                        SelectedRewardOption(domination_info, option_index, &new_option_info);
                }
            }
            else
            {
                sel_options[option_kind] = 
                    SelectedRewardOption(domination_info, option_index, &new_option_info);
            }
        }
    }
    return sel_options;
}

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
const eDOMINATION_TYPE DominationInfoParser::GetDominationTypeFromFieldCode( const MAPCODE field_code ) const
{
    const sDOMINATION_INFO* const parser_info = GetDominationInfo(field_code);
    if (parser_info == NULL)
    {
        SUNLOG(eFULL_LOG,"["__FUNCTION__"]Not found sDomination info(field code) = %d", field_code);
        return DOMINATION_TYPE_NORMAL;
    }
    return static_cast<const eDOMINATION_TYPE>(parser_info->m_DominationType);
}

const MAPCODE DominationInfoParser::GetMondScheinMapCode()
{
    DOMINATION_SCRIPT_INFO_MAP::const_iterator it = script_infos_.begin();
    for (it; it != script_infos_.end(); ++it)
    {
        sDOMINATION_INFO domi_info = it->second;
        if (domi_info.m_DominationType == DOMINATION_TYPE_MONDSCHIEN) {
            return domi_info.m_DominationMapCode;
        }
    }
    return 0;
}

sDOMINATION_EVENT_INFO DominationInfoParser::GetMondScheinEventInfo()
{
    MAPCODE mond_mapcode = GetMondScheinMapCode();
    EventInfos mond_eventinfos = GetEventInfosFromMapCode(mond_mapcode);

    sDOMINATION_EVENT_INFO event_info;

    EventInfos::iterator itr = mond_eventinfos.begin();
    if (itr != mond_eventinfos.end())
    {
        event_info = itr->second;
    }

    return event_info;
}

#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION