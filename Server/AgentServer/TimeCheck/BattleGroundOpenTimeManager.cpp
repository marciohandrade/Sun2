#include "stdafx.h"
#include "./BattleGroundOpenTimeManager.h"

#include <time.h>
#include <BattleGroundTimeInfoParser.h>
#include <UserSessions/UserManager.h>

BattleGroundOpenTimeManager::BattleGroundOpenTimeManager()
{
    loaded_ = false;
    update_timer_.SetTimer(kUpdateInterval);
    ZeroMemory(&current_mode_, sizeof(current_mode_));
}

BattleGroundOpenTimeManager::~BattleGroundOpenTimeManager()
{
}

bool BattleGroundOpenTimeManager::IsOpenBattleGround()
{
    switch (current_mode_.time_mode)
    {
    case current_mode_.kOpened:
    case current_mode_.kClosingCountdown_5Minutes:
        return true;
    }
    return false;
}

void BattleGroundOpenTimeManager::Update(const util::TimeInfoPerFrame& time_info)
{
    if (update_timer_.IsExpired() == false) 
    {
        return;
    }
    
    // 현재 이벤트 진행중..
    if (current_mode_.day_flag != 0 && current_mode_.IsInRange(time_info.acm_time_) == true) 
    {
        return;
    };

    const BattleGroundTimeInfoParser* parser = BattleGroundTimeInfoParser::Instance();
    if (current_mode_.time_mode == ns_func::BattleGroundTimeNode::kNone)
    {
        if (parser->GetCurrentTimeNode(time_info, time_info.system_time.wDayOfWeek + 1, &current_mode_) == false) 
        {
            return;
        }
    }

    // 현재 이벤트 시간이 지났다면.. 다음 타임 찾기..
    if (current_mode_.IsInRange(time_info.acm_time_) == false)
    {
        if (parser->GetCurrentTimeNode(time_info, current_mode_.day_flag, &current_mode_) == false) 
        {
            return;
        }
    }
    
    if (current_mode_.check_event == false) 
    {
        // first event
        ExecuteNotifier();
    }
}

void BattleGroundOpenTimeManager::ExecuteNotifier()
{
    assert(current_mode_.check_event == false);
    MSG_CG_ETC_BATTLE_OPEN_TIME_NOTICE_BY_CODE_BRD msg_brd;
    msg_brd.m_NoticeCode = current_mode_.string_code;
    UserManager::Instance()->SendMessageToAllUsers(&msg_brd, sizeof(msg_brd));

    current_mode_.check_event = true;
}

bool BattleGroundOpenTimeManager::SetupMessage(const util::TimeInfoPerFrame& time_info, 
                                    MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK* const msg_ack) const
{
    typedef BattleGroundTimeInfoParser::FirstEventList FirstEventList;
    const ns_func::BattleGroundTimeNode::AcmTmFmt k24hours = \
        BattleGroundTimeInfoParser::k24hours;
    const BattleGroundTimeInfoParser* parser = BattleGroundTimeInfoParser::Instance();
    const FirstEventList& first_event_list = parser->GetFirstEventList();
    FirstEventList::const_iterator it = first_event_list.begin(),
                                   end = first_event_list.end();
    int cur_time = time_info.acm_time_;
    WORD cur_day = time_info.system_time.wDayOfWeek + 1;
    
    bool last_event = false;
    const ns_func::BattleGroundTimeNode* time_node = *it;
    for ( ; it != end; ++it)
    {
        const ns_func::BattleGroundTimeNode* time_node_in_loop = *it;
        const bool delimeter = (time_node_in_loop->time_mode == time_node_in_loop->kNone); // 대기 이벤트

        const int open_time = time_node_in_loop->start_time;
        const int close_time = delimeter ? 
                  time_node_in_loop->end_time: // 대기 이벤트..
                  time_node_in_loop->end_time + 5; // 시작 이벤트..

        WORD yesterday = cur_day-1;
        if (yesterday < 1)
            yesterday = 7;

        if (cur_day == time_node_in_loop->day_flag &&
            (cur_time >= open_time && cur_time < close_time))
        {
            time_node = time_node_in_loop;
            break;
        }

        else if (yesterday == time_node_in_loop->day_flag &&
                 close_time < open_time &&
                 cur_time < open_time && cur_time < open_time
                 )
        {
            time_node = time_node_in_loop;
            break;
        }

        else if (yesterday == time_node_in_loop->day_flag &&
            cur_time + k24hours >= open_time && cur_time + k24hours < close_time)
        {
            time_node = time_node_in_loop;
            break;
        }
    }

    if (time_node->time_mode == time_node->kNone) 
    {
        if (it == end || it == end - 1) 
        {
            last_event = true;
            time_node = first_event_list.front();
        }
        else
            time_node = *(++it);
    }
    else 
        time_node = *it;

    int open_time = time_node->start_time;
    int close_time = time_node->end_time + 5;
    
    if (cur_time >= open_time && cur_time < close_time || 
        cur_time + k24hours >= open_time && cur_time + k24hours < close_time)
    {
        msg_ack->join_state = kJoinStateAdmission;
    }
    else 
    {
        msg_ack->join_state = kJoinStateNonadmission;
    };

    if (open_time > k24hours)
        open_time -= k24hours;
    if (close_time > k24hours)
        close_time -= k24hours;

    msg_ack->join_time = 0;

    struct tm cur_time_struct;
    util::YYYYMMDDHHMMSSToTMStructure(time_info.wz_yyyymmddhhmmss_, &cur_time_struct);
    cur_time_struct.tm_year -= 1900;
    cur_time_struct.tm_mon -= 1;
    struct tm next_time_struct = cur_time_struct;
    // test value {
    cur_time_struct.tm_hour = cur_time / 60;
    cur_time_struct.tm_min = cur_time % 60;
    // }
    time_t cur_time_val = mktime(&cur_time_struct);
    //
    if (msg_ack->join_state == kJoinStateAdmission)
    {
        next_time_struct.tm_hour = close_time / 60;
        next_time_struct.tm_min = close_time % 60;
    }
    else if (msg_ack->join_state == kJoinStateNonadmission)
    {
        next_time_struct.tm_hour = open_time / 60;
        next_time_struct.tm_min = open_time % 60;
    };
    time_t open_time_val = mktime(&next_time_struct);
    double diff_val = difftime(open_time_val, cur_time_val);
    msg_ack->join_time = static_cast<DWORD>(diff_val) * 1000;

    return true;
}
