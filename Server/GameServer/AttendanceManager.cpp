#include "stdafx.h"

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "AttendanceManager.h"
#include "AttendanceReward.h"
#include "AttendanceParser.h"

AttendanceManager::AttendanceManager()
{
}

void AttendanceManager::SetAttendanceInfos( const AttendanceInfo& attendance_infos )
{
    memcpy(&m_attendance_infos, &attendance_infos, sizeof(attendance_infos));
}

bool AttendanceManager::ToAttend(Player* player)
{
    time_t t;
    util::TimeSync::time(&t);

    struct tm* attend_time = localtime(&t);
    if (attend_time == NULL)
    {
        return false;
    }
    int year = attend_time->tm_year + 1900;
    int month = attend_time->tm_mon + 1;
    int day = attend_time->tm_mday - 1;

    return Attend(year, month, day, player);
}

bool AttendanceManager::ProcessReward( Player* player )
{
    ToAttend(player);

    AttendanceReward* attendance_reward = 
        _GetAttendanceReward(m_attendance_infos.GetType());
    if (attendance_reward == NULL)
    {
        return false;
    }

    attendance_reward->SetAttendanceInfo(&m_attendance_infos);

    if (attendance_reward->IsSatisfyCondition() == false)
    {
        return false;
    }

    attendance_reward->RewardComplete(player);
    return true;
}

bool AttendanceManager::Attend( int year, int month, int day, Player* player )
{
    if (m_attendance_infos.GetMonth() != month 
        || m_attendance_infos.GetYear() != year)
    {
        const ATTENDANCE* attendance = AttendanceParser::Instance()->GetAttendance(year, month);
        if (attendance == NULL)
        {
            m_attendance_infos.init(year, month, 
                eATTENDANCE_SUPERPOSITION, eATTENDANCE_INFO_NOT_ATTEND);
            return false;
        }

        m_attendance_infos.init(attendance->year, attendance->month,
            attendance->type, eATTENDANCE_INFO_NOT_ATTEND);
    }

    if (m_attendance_infos.IsEqualAttendanceInfo(day, eATTENDANCE_INFO_NOT_ATTEND) == false)
    {
        return false;
    }

    m_attendance_infos.SetAttendance(day, eATTENDANCE_INFO_ATTENDED);

    GAMELOG->LogSimpleAction(ACT_ATTENDANCE_ATTENDED, player);
    return true;
}

AttendanceReward* AttendanceManager::_GetAttendanceReward( eATTENDANCE_TYPE attendace_type )
{
    AttendanceReward* attendance_reward = NULL;

    switch (attendace_type)
    {
    case eATTENDANCE_CALENDAR:
        {
            static AttendanceRewardCalendar attendance_reward_calendar;
            attendance_reward = &attendance_reward_calendar;
        } break;
    case eATTENDANCE_SUPERPOSITION:
        {
            static AttendanceRewardSuperposition attendance_reward_superposition;
            attendance_reward = &attendance_reward_superposition;
        } break;
    default:
        {
            SUNLOG(eCRITICAL_LOG, "[%s] Invalid Attendance Type (%d)", 
                __FUNCTION__, attendace_type);
        } break;
    }
    return attendance_reward;
}

#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM