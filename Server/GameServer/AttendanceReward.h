#ifndef SERVER_GAMESERVER_ATTENDANCEREWARD_H
#define SERVER_GAMESERVER_ATTENDANCEREWARD_H

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

class Player;

class AttendanceReward
{
public:
    void SetAttendanceInfo(AttendanceInfo* attendance_infos){ m_attendance_info = attendance_infos; };
    bool ObtainReward(Player* player, DWORD reword_info_index);

    virtual bool IsSatisfyCondition() = 0;
    virtual void RewardComplete(Player* player) = 0;

protected:
    AttendanceInfo* m_attendance_info;
};

class AttendanceRewardCalendar : public AttendanceReward
{
public:
    virtual bool IsSatisfyCondition();
    virtual void RewardComplete(Player* player);
};

class AttendanceRewardSuperposition : public AttendanceReward
{
public:
    virtual bool IsSatisfyCondition();
    virtual void RewardComplete(Player* player);
};

#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#endif // SERVER_GAMESERVER_ATTENDANCEREWARD_H