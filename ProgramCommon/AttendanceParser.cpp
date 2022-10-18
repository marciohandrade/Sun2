#include "StdAfx.h"

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "AttendanceParser.h"

BOOL AttendanceParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    __UNUSED((script_code, is_reload));

    int row_size = GetRowSize();
    for (int script_row = 0; script_row < row_size; ++script_row)
    {
        ATTENDANCE attendance;

        GetData(script_row, attendance.year, "Index1");
        GetData(script_row, attendance.month, "Index2");

        int temp_type = 0;
        GetData(script_row, temp_type, "Type");
        if (temp_type < eATTENDANCE_CALENDAR || temp_type >= eATTENDANCE_MAX)
        {
            SUNLOG(eCRITICAL_LOG, _T("[%s] Invalid Attendance Type (%d)"),
                __FUNCTION__, attendance.type);
            ASSERT(false);
        }
        attendance.type = static_cast<eATTENDANCE_TYPE>(temp_type);

        GetData(script_row, attendance.title, "TitleString");
        GetData(script_row, attendance.explanation, "ExplanationString");

        char temp_buffer[32];
        ZeroMemory(&temp_buffer, sizeof(temp_buffer));
        for (int i = 0; i < ATTENDANCE::kDayMaxCount; ++i)
        {
            _snprintf(temp_buffer, _countof(temp_buffer), "Emphasis%d", i+1);
            GetData(script_row, attendance.day_emphasis[i], temp_buffer);

            _snprintf(temp_buffer, _countof(temp_buffer), "RewardCode%d", i+1);
            GetData(script_row, attendance.day_reward_index[i], temp_buffer);
        }

        if (IsValidAttendance(&attendance) == false)
        {
            continue;
        }

        ATTENDANCE_TAG attendance_tag = std::make_pair(attendance.year, attendance.month);
        attendance_map.insert(std::make_pair(attendance_tag, attendance));
    }

    return TRUE;
}

const bool AttendanceParser::IsValidAttendance(ATTENDANCE* attendance)
{
    if (attendance == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%s] Attendance is null"), __FUNCTION__);
        ASSERT(false);
        return false;
    }

    if (attendance->type < eATTENDANCE_CALENDAR || attendance->type >= eATTENDANCE_MAX)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%s] Invalid Attendance Type (%d)"),
            __FUNCTION__, attendance->type);
        ASSERT(false);      
        return false;
    }

    const ATTENDANCE* prev_attendance = GetAttendance(attendance->year, attendance->month);
    if (prev_attendance)
    {
        SUNLOG(eCRITICAL_LOG, _T("[%s] There is such attendance (%d, %d)"),
            __FUNCTION__, attendance->year, attendance->month);
        ASSERT(false);
        return false;
    }

    return true;
}

const ATTENDANCE* AttendanceParser::GetAttendance( DWORD year, BYTE month )
{
    ATTENDANCE_TAG attendance_tag = std::make_pair(year, month);
    ATTENDANCE_MAP::iterator find_iter = attendance_map.find(attendance_tag);
    if (find_iter == attendance_map.end())
    {
        return NULL;
    }

    return &(find_iter->second);
}

#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
