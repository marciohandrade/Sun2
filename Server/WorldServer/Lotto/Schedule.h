/*
 * Schedule.h
 */
#ifndef _SCHEDULE_H_
#define _SCHEDULE_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <time.h>

/*  [Example]

    Schedule schedule;
    // ���� ������ ���� 6�� 30�п� ���� ����
    schedule.SetDayOfWeek(Schedule::MONDAY);
    schedule.SetHour(6);
    schedule.SetMinute(30);
    
    ScheduleToken token;

    if (schedule.FindNearestAhead(token)) {
        int secondsTo = token.SecondsTo();
        if (secondsTo > 0) {
            printf("%d second(s) left\n", secondsTo);
        }
        else {
            printf("It's the time now!\n");
        }
    }
 */

// ���� ������ ��Ÿ���� ��ū Ŭ����
class ScheduleToken
{
public:
    ScheduleToken() : timer_(0), valid_(false) { str_[0] = '\0'; }

    int     SecondsTo() const;      // �������� ���� �ð��� �� ������ ����
    int     SecondsTo(const SYSTEMTIME* st) const;

    void    SetTimer(time_t timer);
    time_t  GetTimer() const        { return timer_; }

    void    Invalidate()            { valid_ = false; }
    bool    IsValid() const         { return valid_; }

    const TCHAR* ToString() const   { return str_; }
private:
    static const int MAX_TIME_FORMAT_LEN = 51;

    time_t timer_;
    bool   valid_;
    TCHAR  str_[MAX_TIME_FORMAT_LEN];
};

// ��Ģ������ �ݺ��Ǵ� ������ ���� Schedule Ŭ����
class Schedule
{
public:
    // ���� ���
    enum DayOfWeek {
        SUNDAY = 0
      , MONDAY
      , TUESDAY
      , WEDNESDAY
      , THURSDAY
      , FRIDAY
      , SATURDAY
      , NUM_DAYS_OF_WEEK
    };

    Schedule() : daysOfWeek_(0), hours_(0), minutes_(0) {}
    ~Schedule() {}

    void SetDayOfWeek(Schedule::DayOfWeek dayOfWeek) {daysOfWeek_ |= (1 << dayOfWeek); }
    void SetHour(int hour)                           { hours_ |= (1 << hour); }
    void SetMinute(int minute)                       { minutes_ |= ((DWORD64)1 << minute); }

    void SetDayOfWeek() { daysOfWeek_ = static_cast<uint8_t>(~0); }         // ��� ���� ����
    void SetHour()      { hours_ = static_cast<DWORD32>(~0); }              // ��� �� ����
    void SetMinute()    { minutes_ = ~((DWORD64)0); } // ��� �� ����

    void ResetAll()
    {
        daysOfWeek_ = 0;
        hours_ = 0;
        minutes_ = (DWORD64)0;
    }

    bool FindNearestAhead(ScheduleToken& token) const;

    static time_t SystemTime2time_t(const SYSTEMTIME* st, long* ns = NULL); // Windows SYSTEMTIME�� CRT time_t��
    static time_t FileTime2time_t(const FILETIME* ft, long* ns = NULL);     // Windows FILETIME�� CRT time_t��
    static void time_t2SystemTime(time_t timer, SYSTEMTIME* st);            // CRT time_t�� Windows SYSTEMTIME����
    static void time_t2FileTime(time_t timer, FILETIME* ft);                // CRT time_t�� Windows FILETIME����

private:
    BYTE    daysOfWeek_;    // Bit Mask : �� �� ���� ���� (7��, 0~6)
    DWORD32 hours_;         // Bit Mask : �ð� ���� (24�ð�, 0~23)   
    DWORD64 minutes_;       // Bit Mask : �� ���� (60��, 0~59)
    // �� ���������� ����
};

#endif // _SCHEDULE_H_
