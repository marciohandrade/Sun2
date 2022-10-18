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
    // 매주 월요일 오전 6시 30분에 일정 설정
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

// 일정 시점을 나타내는 토큰 클래스
class ScheduleToken
{
public:
    ScheduleToken() : timer_(0), valid_(false) { str_[0] = '\0'; }

    int     SecondsTo() const;      // 일정까지 남은 시간을 초 단위로 리턴
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

// 규칙적으로 반복되는 일정을 위한 Schedule 클래스
class Schedule
{
public:
    // 요일 상수
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

    void SetDayOfWeek() { daysOfWeek_ = static_cast<uint8_t>(~0); }         // 모든 요일 선택
    void SetHour()      { hours_ = static_cast<DWORD32>(~0); }              // 모든 시 선택
    void SetMinute()    { minutes_ = ~((DWORD64)0); } // 모든 분 선택

    void ResetAll()
    {
        daysOfWeek_ = 0;
        hours_ = 0;
        minutes_ = (DWORD64)0;
    }

    bool FindNearestAhead(ScheduleToken& token) const;

    static time_t SystemTime2time_t(const SYSTEMTIME* st, long* ns = NULL); // Windows SYSTEMTIME을 CRT time_t로
    static time_t FileTime2time_t(const FILETIME* ft, long* ns = NULL);     // Windows FILETIME을 CRT time_t로
    static void time_t2SystemTime(time_t timer, SYSTEMTIME* st);            // CRT time_t를 Windows SYSTEMTIME으로
    static void time_t2FileTime(time_t timer, FILETIME* ft);                // CRT time_t를 Windows FILETIME으로

private:
    BYTE    daysOfWeek_;    // Bit Mask : 매 주 요일 설정 (7일, 0~6)
    DWORD32 hours_;         // Bit Mask : 시간 설정 (24시간, 0~23)   
    DWORD64 minutes_;       // Bit Mask : 분 설정 (60분, 0~59)
    // 분 단위까지만 지원
};

#endif // _SCHEDULE_H_
