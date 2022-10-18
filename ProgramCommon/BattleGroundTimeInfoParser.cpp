#include "StdAfx.h"
#include ".\BattleGroundTimeInfoParser.h"

//------------------------------------------------------------------------------ 
void BattleGroundTimeInfoParser::Release()
{
    Unload();
}

BOOL BattleGroundTimeInfoParser::LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload)
{
    __UNUSED(bReload);
    switch (scriptCode)
    {
    case SCRIPT_BATTLE_GROUND_TIME_INFO:
        return _Load(false);
    }

    return false;
}
//------------------------------------------------------------------------------ 
void BattleGroundTimeInfoParser::Unload()
{
}
bool BattleGroundTimeInfoParser::_Load(bool reload)
{
    __UNUSED(reload);

    // open time event specification
    //         -5            Open               -5       closed
    //----------|--------------|-----------------|----------|

    if (!FlowControl::FCAssert(time_node_array_ == NULL && open_time_table_.empty()))
    {
        return false;
    }

    const int row_count = GetRowSize();
    number_of_time_nodes_ = \
        (row_count * kOpenTimeMaxCount * kEventTimeModeCounts) + (row_count * kOpenTimeMaxCount);

    const size_t buffer_size = sizeof(*time_node_array_) * number_of_time_nodes_;
    time_node_array_ = reinterpret_cast<BattleGroundTimeNode*>(::malloc(buffer_size));
    ZeroMemory(time_node_array_, buffer_size);
    first_event_list_.reserve(row_count * kOpenTimeMaxCount * 2);

    TCHAR field_name[64] = {0,};
    static const char* kTimeEventArray[] = 
    {
        "StartTimeStringCode1", 
        "StartTimeStringCode2", 
        "EndTimeStringCode1", 
        "EndTimeStringCode2"
    };

    BattleGroundTimeNode* time_node_it = time_node_array_;
    for (int row = 0; row < row_count; ++row) //day..
    {
        for(int col = 0; col < kOpenTimeMaxCount; ++col) // 요일별 시간
        {
            for (int i = 0; i < _countof(kTimeEventArray); ++i) 
            {
                BattleGroundTimeNode* const time_node = time_node_it++;

                // day_flag setting..
                const uint16_t day_flag = GetDataWORD("DayFlag", row);
                time_node->day_flag = day_flag;

                _sntprintf(field_name, _countof(field_name), "StartTime%d", col + 1);
                BattleGroundTimeNode::AcmTmFmt start_time = GetDataWORD(field_name, row);
                start_time = ((start_time / 100) * 60) + (start_time % 100);

                _sntprintf(field_name, _countof(field_name), "EndTime%d", col + 1);
                BattleGroundTimeNode::AcmTmFmt end_time = GetDataWORD(field_name, row);
                end_time = ((end_time / 100) * 60) + (end_time % 100);

                if (start_time == end_time) // 사용 하지 않은 시간..
                {
                    continue;
                }

                ulong string_code = GetDataDWORD(kTimeEventArray[i], row);

                _SetTimeInfo(time_node, start_time, end_time, 
                             static_cast<const BattleGroundTimeNode::TimeMode>(
                             time_node->kCountdown_5Minutes + i),string_code);

                open_time_table_.insert(OpenTimeTable::value_type(time_node->day_flag, time_node));
                if (i == 1)
                {
                    first_event_list_.push_back(time_node);

                    BattleGroundTimeNode* const empty_time_node = time_node_it++;
                    empty_time_node->day_flag = day_flag;
                    empty_time_node->start_time = end_time;
                    if (empty_time_node->start_time > k24hours)
                    {
                        empty_time_node->start_time -= k24hours;
                        empty_time_node->day_flag += 1;
                        if (empty_time_node->day_flag > 7)
                            empty_time_node->day_flag = 0;
                    }
                    first_event_list_.push_back(empty_time_node);
                }
            }
        }
    }

    const size_t event_size = first_event_list_.size();
    FirstEventList::iterator it = first_event_list_.begin(),
                             last = first_event_list_.end() - 1;
    for (; it != last; ++it)
    {
        BattleGroundTimeNode* const time_node = const_cast<BattleGroundTimeNode*>(*it);
        if (time_node->time_mode != time_node->kNone) 
        {
            continue;
        }
        BattleGroundTimeNode* const next_time_node = const_cast<BattleGroundTimeNode*>(*(it + 1));
        time_node->end_time = next_time_node->start_time;
    }   
    //마지막 노드..
    if (BattleGroundTimeNode* const last_time_node = const_cast<BattleGroundTimeNode*>(*last))
    {
        const BattleGroundTimeNode* const first_time_node = first_event_list_.front();
        last_time_node->end_time = first_time_node->start_time;
    }

    return true;
}

void BattleGroundTimeInfoParser::_SetTimeInfo(BattleGroundTimeNode* time_node,
                                              const BattleGroundTimeNode::AcmTmFmt input_open_time,
                                              const BattleGroundTimeNode::AcmTmFmt input_close_time,
                                              const BattleGroundTimeNode::TimeMode time_mode,
                                              const DWORD string_code)
{
    int start_time = 0;
    int end_time = 0;
    uint16_t day_flag = time_node->day_flag;

    switch (time_mode)
    {
    case time_node->kCountdown_5Minutes:
        {
            start_time = input_open_time - 5;
            end_time = input_open_time;
        };
        break;
    case time_node->kOpened:
        {
            start_time = input_open_time;
            end_time = input_close_time - 5;
        };
        break;
    case time_node->kClosingCountdown_5Minutes:
        {
            start_time = input_close_time - 5;
            end_time = input_close_time;
        };
        break;
    case time_node->kClosed:
        {
            start_time = input_close_time;
            end_time = input_close_time + 5; // fake
        };
        break;
    default:
        ASSERT(!"unexpected time mode|");
        start_time = end_time = 0;
        break;
    } //end 'switch'

    if (start_time < 0)
    {
        start_time += k24hours;
        end_time += k24hours;

        day_flag -= 1;
        if (day_flag < 1)
            day_flag = 7;
    }
    
    else if (start_time >= k24hours)
    {
        start_time -= k24hours;
        end_time -= k24hours;

        day_flag += 1;
        if (day_flag > 7)
            day_flag = 1;
    }

    time_node->time_mode = time_mode;   // 모드
    time_node->day_flag = day_flag; // 날짜
    time_node->start_time = static_cast<BattleGroundTimeNode::AcmTmFmt>(start_time); // 시작시간
    time_node->end_time = static_cast<BattleGroundTimeNode::AcmTmFmt>(end_time); // 종료시간
    time_node->string_code = string_code; // 스트링코드
}

bool BattleGroundTimeInfoParser::IsExistStartTimeNode(const util::TimeInfoPerFrame& time_info,
                                                      BattleGroundTimeNode* result_node) const
{
    const util::TimeInfoPerFrame::AcmTmFmt cur_time = time_info.acm_time_;
    FOREACH_CONTAINER(const FirstEventList::value_type& node, first_event_list_, FirstEventList)
    {
        const BattleGroundTimeNode* time_node = node;
        if (time_node->time_mode == time_node->kNone) 
        {
            continue;
        }

        bool exist_event = time_node->IsInRange(cur_time);
        if (exist_event)
        {
            *result_node = *time_node;
            return true;
        }
        else
            return GetCurrentTimeNode(time_info, time_node->day_flag, result_node);
    }
    ZeroMemory(result_node, sizeof(*result_node));
    return false;
}
bool BattleGroundTimeInfoParser::GetCurrentTimeNode(const util::TimeInfoPerFrame& time_info, 
                                                    uint16_t day_flag,
                                                    ns_func::BattleGroundTimeNode* result_node) const
{
    const util::TimeInfoPerFrame::AcmTmFmt cur_time = time_info.acm_time_;
    OpenTimeTable::_Paircc ranges = open_time_table_.equal_range(day_flag);
    for ( ; ranges.first != ranges.second; ++ranges.first)
    {
        const BattleGroundTimeNode* const time_node = ranges.first->second;
        if (time_node->IsInRange(cur_time))
        {
            *result_node = *time_node;
            return true;
        }
    }
    ZeroMemory(result_node, sizeof(*result_node));
    return false;
}