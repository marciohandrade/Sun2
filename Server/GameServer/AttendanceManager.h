#ifndef SERVER_GAMESERVER_ATTENDANCEMANAGER_H
#define SERVER_GAMESERVER_ATTENDANCEMANAGER_H

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#include <atltime.h>

class Player;
class AttendanceReward;

class AttendanceManager
{
public:
    AttendanceManager();

    const AttendanceInfo& GetAttendanceInfos() {return m_attendance_infos; }
    void SetAttendanceInfos(const AttendanceInfo& attendance_infos);

    bool ToAttend(Player* player);
    bool ProcessReward(Player* player);
    bool Attend(int year, int month, int day, Player* player);

private:
    AttendanceReward* _GetAttendanceReward(eATTENDANCE_TYPE attendace_type);

    AttendanceInfo m_attendance_infos;
};

#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#endif // SERVER_GAMESERVER_ATTENDANCEMANAGER_H