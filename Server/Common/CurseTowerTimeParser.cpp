#include "stdafx.h"
#include "./CurseTowerTimeParser.h"

#if __ITIMERBASE_VERSION_CRTL >= 0x0120
//==================================================================================================

#include <algorithm>

CurseTowerTimeParser::CurseTowerTimeParser()
    : loaded_(false)
    , number_of_time_nodes_(0)
    , time_node_array_(NULL)
{
}

CurseTowerTimeParser::~CurseTowerTimeParser()
{
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)

BOOL CurseTowerTimeParser::LoadScript(eSCRIPT_CODE script_code, BOOL reload)
{
    __UNUSED(reload);
    switch (script_code)
    {
    case SCRIPT_CURSE_TOWER_TIME:
        return _Load(false);
    }

    return false;
}


void CurseTowerTimeParser::Release()
{
    open_time_table_.clear();
    if (time_node_array_) {
        ::free(time_node_array_);
        time_node_array_ = NULL;
    };
}

void CurseTowerTimeParser::SetTimeInfo(BattleTimeNode* time_node,
                                       const BattleTimeNode::AcmTmFmt input_open_time,
                                       const BattleTimeNode::mode_t time_mode,
                                       const DWORD string_code)
{
    int open_time = input_open_time;
    int start_time = open_time;
    int end_time = open_time;
    //
    switch (time_mode)
    {
    case time_node->eCountdown_5Minutes:
        {
            start_time -= 5;
            end_time -= 1;
        };
        break;
    case time_node->eCountdown_1Minutes:
        {
            start_time -= 1;
            //end_time
        };
        break;
    case time_node->eOpened:
        {
            //start_time
            end_time += 7;
        };
        break;
    case time_node->eClosingCountdown_3Minutes:
        {
            start_time += 7;
            end_time += 9;
        };
        break;
    case time_node->eClosingCountdown_1Minutes:
        {
            start_time += 9;
            end_time += 10;
        };
        break;
    case time_node->eClosed:
        {
            start_time += 10;
            end_time += 15; // fake
        };
        break;
    default:
        ASSERT(!"unexpected time mode|");
        start_time = end_time = 0;
        break;
    } //end 'switch'
    //
    if (start_time < 0) {
        start_time += k24hours;
        end_time += k24hours;
    }
    else if (start_time >= k24hours) {
        start_time -= k24hours;
        end_time -= k24hours;
    };

    time_node->time_mode_ = time_mode;
    time_node->start_tm_ = static_cast<BattleTimeNode::AcmTmFmt>(start_time);
    time_node->end_tm_ = static_cast<BattleTimeNode::AcmTmFmt>(end_time);
    time_node->string_code_ = string_code;
}

bool CurseTowerTimeParser::_Load(bool reload)
{
    // open time event specification
    //         -5         -1         Open        +7        +9      +10(close)
    //----------|----------|----------|----------|----------|----------|
    __UNUSED(reload);
    //
    if (!FlowControl::FCAssert(loaded_ == false &&
                               time_node_array_ == NULL &&
                               open_time_table_.empty()))
    {
        return false;
    };
    const int max_rows = GetRowSize();
    ;{
        number_of_time_nodes_ = (kNumberOfEventsPerNode * max_rows) + max_rows;
        const size_t buffer_size = sizeof(*time_node_array_) * number_of_time_nodes_;
                                   // <- add first event mediators
        time_node_array_ = reinterpret_cast<BattleTimeNode*>(::malloc(buffer_size));
        ZeroMemory(time_node_array_, buffer_size);
        first_event_list_.reserve(max_rows * 2);
    };

    static const char* kTimeEventArray[kNumberOfEventsPerNode] = {
        "wStart5StringCode", "wStart1StringCode", "wStartStringCode",
        "wEnd3StringCode", "wEnd1StringCode", "wEndStringCode"
    };
    BattleTimeNode* time_node_it = time_node_array_;
    for (int row = 0; row < max_rows; ++row)
    {
        const uint16_t index = GetDataWORD("wTimeManCode", row);
        
        uint16_t hour = GetDataBYTE("wHour", row);
        uint16_t minute = GetDataBYTE("wMinute", row);
        const BattleTimeNode::AcmTmFmt acm_open_tm = (hour * 60 + minute);
        if (!FlowControl::FCAssert(acm_open_tm >= kLowerbound &&
                                   (acm_open_tm < (23 * 60 + 50) || acm_open_tm == k24hours)))
        {
            continue;
        }
        //
        //first_event_list_.push_back(time_node_it++);
        for (int i = 0; i < kNumberOfEventsPerNode; ++i)
        {
            BattleTimeNode* const time_node = time_node_it++;
            time_node->index_ = index;
            ulong string_code = GetDataDWORD(kTimeEventArray[i], row);
            SetTimeInfo(time_node, acm_open_tm,
                static_cast<const BattleTimeNode::mode_t>(time_node->eCountdown_5Minutes + i),
                string_code);
            //
            open_time_table_.insert(std::make_pair(index, time_node));
            // first event
            if (i == 0) {
                first_event_list_.push_back(time_node);
                first_event_list_.push_back(time_node_it++);
            }
        };
    }; //end operation 'read records'
    /*
    ;{  // sort
        struct InterFirstEventListComparor :
            public std::binary_function<const BattleTimeNode*, const BattleTimeNode*, bool>
        {
            bool operator()(const BattleTimeNode*& lhs, const BattleTimeNode*& rhs) const
            {
                const BattleTimeNode::AcmTmFmt k24hours = CurseTowerTimeParser::k24hours;
                BattleTimeNode::AcmTmFmt ltime = lhs->start_tm_ + 5;
                BattleTimeNode::AcmTmFmt rtime = rhs->start_tm_ + 5;
                if (ltime >= k24hours) {
                    ltime -= k24hours;
                }
                if (rtime >= k24hours) {
                    rtime -= k24hours;
                }
                //
                return (ltime < rtime);
            };
        } comparor;
        //
        std::sort(first_event_list_.begin(), first_event_list_.end(), comparor);
    };
    */
    ;{
        const size_t numbers = first_event_list_.size();
        assert(numbers >= 3UL);
        FirstEventList::iterator it = first_event_list_.begin(),
                                 last = first_event_list_.end() - 1;
        for (int start_tm = 0; it != last; ++it)
        {
            BattleTimeNode* const time_node = const_cast<BattleTimeNode*>(*it);
            if (time_node->time_mode_ != time_node->eNone) {
                start_tm = time_node->start_tm_ + 15;
                continue;
            };
            //
            time_node->start_tm_ = static_cast<BattleTimeNode::AcmTmFmt>(start_tm);
            BattleTimeNode* const next_time_node = const_cast<BattleTimeNode*>(*(it + 1));
            time_node->end_tm_ = next_time_node->start_tm_;
        }
        if (BattleTimeNode* const last_time_node = const_cast<BattleTimeNode*>(*last))
        {
            assert(last_time_node->time_mode_ == last_time_node->eNone);
            const BattleTimeNode* const prev_time_node = *(last - 1);
            last_time_node->start_tm_ = prev_time_node->start_tm_ + 15;
            const BattleTimeNode* const first_time_node = first_event_list_.front();
            last_time_node->end_tm_ = first_time_node->start_tm_ + 15 + k24hours;
        }
    };
    //
    loaded_ = true;

    return true;
}

bool CurseTowerTimeParser::IsExistStartTimeNode(const util::TimeInfoPerFrame& time_info,
                                                BattleTimeNode* result_node) const
{
    const util::TimeInfoPerFrame::AcmTmFmt cur_time = time_info.acm_time_;
    FOREACH_CONTAINER(const FirstEventList::value_type& node, first_event_list_,
                      FirstEventList)
    {
        const BattleTimeNode* time_node = node;
        if (time_node->time_mode_ == time_node->eNone) {
            continue;
        }
        int mod_time = time_node->start_tm_ + 15;
        bool exist_event = time_node->IsInRange(cur_time);
        if (exist_event)
        {
            *result_node = *time_node;
            return true;
        }
        if (exist_event == false && mod_time >= k24hours)
        {   // dangling...
            exist_event = int(cur_time) >= int(time_node->start_tm_) - k24hours &&
                          int(cur_time) < mod_time - k24hours;
        }
        if (exist_event == false) {
            exist_event = cur_time >= time_node->start_tm_ &&
                          cur_time < mod_time;
        }
        if (exist_event) {
            return GetCurrentTimeNode(time_info, time_node->index_, result_node);
        }
    }
    ZeroMemory(result_node, sizeof(*result_node));
    return false;
}

bool CurseTowerTimeParser::GetCurrentTimeNode(const util::TimeInfoPerFrame& time_info, uint16_t index,
                                              ns_func::BattleTimeNode* result_node) const
{
    // NOTE: partial sequential searching. because table size is small and
    // a server start time is unforeseeable. in addition, a query owner enable result_node check.
    const util::TimeInfoPerFrame::AcmTmFmt cur_time = time_info.acm_time_;
    OpenTimeTable::_Paircc ranges = open_time_table_.equal_range(index);
    for ( ; ranges.first != ranges.second; ++ranges.first)
    {
        const BattleTimeNode* time_node = ranges.first->second;
        if (time_node->IsInRange(cur_time))
        {
            *result_node = *time_node;
            return true;
        }
    }
    ZeroMemory(result_node, sizeof(*result_node));
    return false;
}

//==================================================================================================
#else // __ITIMERBASE_VERSION_CRTL < 0x0120
//==================================================================================================


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
st_BattleOpenTimeNode::st_BattleOpenTimeNode()
{
	OpenState = STATE_CLOSE;
}

VOID st_BattleOpenTimeNode::CalTick(BYTE Hour, BYTE Min)
{
	DWORD		TickCount	= GetTickCount();	
	SYSTEMTIME	SysNowTime;
	util::TimeSync::GetLocalTime(&SysNowTime);

	INT		iNowTime				= (SysNowTime.wHour * 3600000) + (SysNowTime.wMinute * 60000) + (SysNowTime.wSecond * 1000);
	INT		iBeforeOpen5MinTime		= (( Hour * 3600000 ) + ( Min * 60000 )) - 300000;  // 오픈 5분전 (5*60*1000)

#ifdef _JAPAN
	SUNLOG( eCRITICAL_LOG, "[st_BattleOpenTimeNode::CalTick] Input Hour = %d, Min = %d, CurLocalTime Hour = %d, Min = %d, Second = %d",
		Hour, Min, SysNowTime.wHour, SysNowTime.wMinute, SysNowTime.wSecond );
#endif

	if( iBeforeOpen5MinTime < 0 )
	{
		SUNLOG( eCRITICAL_LOG, "[st_BattleOpenTimeNode::CalTick] iBeforeOpen5MinTime[%d] is minus value!! Hour[%d] Min[%d] SystemTime[%d %d]", 
			iBeforeOpen5MinTime, Hour, Min, SysNowTime.wHour, SysNowTime.wMinute );
	}

	//이미 지났으면 24시간 추가
	if( iNowTime > iBeforeOpen5MinTime)
	{
		iBeforeOpen5MinTime += 86400000;  //24시간 추가 (24 * 60 * 60 * 1000)
	}
	ASSERT( 0 <= iBeforeOpen5MinTime );
	ASSERT( 0 <= iNowTime );
	DWORD dwNowTime				= (DWORD)iNowTime;
	DWORD dwBeforeOpen5MinTime	= (DWORD)iBeforeOpen5MinTime;
    
	//등록 시간 기준 (등록을 Open 5분전에 하므로)
	if( dwBeforeOpen5MinTime >= dwNowTime )
	{
		dwBeforeOpen5MinTick	= ( dwBeforeOpen5MinTime - dwNowTime ) + TickCount;	
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "[st_BattleOpenTimeNode::CalTick] [dwBeforeOpen5MinTime < dwNowTime] iBeforeOpen5MinTime[%u] iNowTime[%d] SystemTime[%d %d]", 
			iBeforeOpen5MinTime, iNowTime, SysNowTime.wHour, SysNowTime.wMinute );
		dwBeforeOpen5MinTick	= TickCount;	
	}

	dwBeforeOpen1MinTick	= dwBeforeOpen5MinTick + (4 * 60 * 1000);
	dwOpenTick				= dwBeforeOpen5MinTick + (5 * 60 * 1000);
	dwBeforeClose3MinTick	= dwOpenTick + (7 * 60 * 1000);
	dwBeforeClose1MinTick	= dwOpenTick + (9 * 60 * 1000);
	dwCloseTick				= dwOpenTick + (10 * 60 * 1000);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CurseTowerTimeParser::CurseTowerTimeParser()
{
}

CurseTowerTimeParser::~CurseTowerTimeParser()
{
}

//#pragma warning ( push )
//#pragma warning ( disable : 4244)

BOOL CurseTowerTimeParser::LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload)
{
	switch( ScriptCode )
	{
	case SCRIPT_CURSE_TOWER_TIME:
		return _Load( bReload);
	}

	return FALSE;
}

BOOL CurseTowerTimeParser::_Load(BOOL bReload)
{
    __UNUSED(bReload);
	BYTE byHour;
	BYTE byMin;
	int nRowSize = GetRowSize();
	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		CODETYPE MainCode = GetDataBYTE( "wTimeManCode", nRow);
		st_BattleOpenTimeList* pList = NULL;
		if( m_BattleOpenTimeHash.find(MainCode) == m_BattleOpenTimeHash.end() )
		{
			pList = new st_BattleOpenTimeList;
			m_BattleOpenTimeHash[MainCode] = pList;
		}
		else
		{
			pList = m_BattleOpenTimeHash[MainCode];
		}

		st_BattleOpenTimeNode *pNode = new st_BattleOpenTimeNode;
		pList->m_List.push_back( pNode );		

		pNode->wIndexCode = MainCode;
		byHour							= GetDataBYTE( "wHour", nRow);
		byMin							= GetDataBYTE( "wMinute", nRow);
        pNode->dwStrCodeBeforeOpen5Min	= GetDataDWORD( "wStart5StringCode", nRow);
		pNode->dwStrCodeBeforeOpen1Min	= GetDataDWORD( "wStart1StringCode", nRow);
		pNode->dwStrCodeOpen			= GetDataDWORD( "wStartStringCode", nRow);
		pNode->dwStrCodeBeforeClose3Min	= GetDataDWORD( "wEnd3StringCode", nRow);
		pNode->dwStrCodeBeforeClose1Min	= GetDataDWORD( "wEnd1StringCode", nRow);
		pNode->dwStrCodeClose			= GetDataDWORD( "wEndStringCode", nRow);

		pNode->CalTick(byHour, byMin);
	}

	return TRUE;
}

VOID CurseTowerTimeParser::Release()
{
	m_RegisterNodeList.clear();

	BATTLE_OPEN_TIME_LIST_HASH::iterator	HashIt = m_BattleOpenTimeHash.begin();
	BATTLE_OPEN_TIME_LIST::iterator			ListIt;	

	while(HashIt != m_BattleOpenTimeHash.end())
	{
		ListIt = (HashIt->second)->m_List.begin();

		while(ListIt != (HashIt->second)->m_List.end() )
		{
			SAFE_DELETE((*ListIt));
			ListIt++;
		}
		(HashIt->second)->m_List.clear();
		SAFE_DELETE( HashIt->second );
		HashIt++;
	}
	m_BattleOpenTimeHash.clear();
}

//#pragma warning ( pop )

//==================================================================================================
#endif //__ITIMERBASE_VERSION_CRTL
