#include "WorldServerPch.hxx"
#include "./Schedule.h"

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

static const __int64 SECS_BETWEEN_EPOCHS = 11644473600;
static const __int64 NUM_100NS_PER_SEC   = 10000000;    // 10^(9-2)
static const __int32 SECS_PER_MINUTE     = 60;
static const __int32 SECS_PER_HOUR       = SECS_PER_MINUTE * 60;
static const __int32 SECS_PER_DAY        = SECS_PER_HOUR * 24;

// 32-bit time_t는 19:14:07, January 18, 2038, UTC까지만 유효하나, 현재 서비스에는 충분

int ScheduleToken::SecondsTo() const
{
    // time(NULL)은 UTC 기반으로 리턴하므로
    // 비교 대상과 동일하게 로컬 SYSTEMTIME에서 변환해야 한다
    SYSTEMTIME st;
    util::TimeSync::GetLocalTime(&st);
    return SecondsTo(&st);
}

int ScheduleToken::SecondsTo(const SYSTEMTIME* st) const
{
    time_t now = Schedule::SystemTime2time_t(st);
    double diff = difftime(timer_, now);
    return (int)ceil(diff);
}

void ScheduleToken::SetTimer(time_t timer)
{
    timer_ = timer;
    valid_ = true;

    // 문자열 표현 캐싱
    SYSTEMTIME st;
    Schedule::time_t2SystemTime(timer, &st);
    _sntprintf(str_, ScheduleToken::MAX_TIME_FORMAT_LEN, "%02u-%02u-%02u %02u:%02u:%02u", \
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    str_[ScheduleToken::MAX_TIME_FORMAT_LEN - 1] = '\0';
}

bool Schedule::FindNearestAhead(ScheduleToken& token) const
{
    SYSTEMTIME st;
    util::TimeSync::GetLocalTime(&st);

    time_t diff = 0; // 초 단위의, 다음 일정까지 남은 시간
    for (int i = 0; i <= Schedule::NUM_DAYS_OF_WEEK; ++i) // 다음 주 같은 요일까지 순환 검사
    {
        int j = (st.wDayOfWeek + i);
        if (j >= Schedule::NUM_DAYS_OF_WEEK) { j %= Schedule::NUM_DAYS_OF_WEEK; }
        if ((daysOfWeek_ & (1 << j)) != 0) // 요일 일치
        {
            for (int k = (i == 0 ? st.wHour : 0); k < 24; ++k) // 당일이면 현재 시간부터 검사
            {
                if ((hours_ & (1 << k)) != 0) // 시 일치
                {
                    // 초 단위는 지원하지 않으므로, 당일의 같은 시간이면 현재의 다음 분부터 검사
                    for (int l = ((i == 0 && k == st.wHour) ? (st.wMinute + 1) : 0); l < 60; ++l)
                    {
                        if ( i == 0 && k == st.wHour && l == (st.wMinute + 1) ) {
                            diff += (SECS_PER_MINUTE - st.wSecond);
                        }

                        if ((minutes_ & ((DWORD64)1 << l)) != 0) // 분 일치
                        {
                            // Found nearest schedule ahead
                            time_t timer = SystemTime2time_t(&st);
                            timer += diff;
                            token.SetTimer(timer);
                            return true;
                        }
                        else {
                            diff += SECS_PER_MINUTE;
                        }
                    }
                }
                else
                {
                    if (i == 0 && k == st.wHour) { 
                        diff += SECS_PER_HOUR - (st.wMinute * SECS_PER_MINUTE + st.wSecond); 
                    }
                    else { 
                        diff += SECS_PER_HOUR; 
                    }
                }
            }
        }
        else
        {
            if (i == 0) {
                diff += SECS_PER_DAY - (st.wHour * SECS_PER_HOUR + st.wMinute * SECS_PER_MINUTE + st.wSecond);
            }
            else {
                diff += SECS_PER_DAY;
            }
        }
    }
    token.Invalidate();
    return false;
}

time_t Schedule::SystemTime2time_t(const SYSTEMTIME* st, long* ns)
{
    FILETIME ft;
    SystemTimeToFileTime(st, &ft);
    return FileTime2time_t(&ft, ns);
}

time_t Schedule::FileTime2time_t(const FILETIME* ft, long* ns)
{
    __int64 i64;
    i64 = ((__int64)ft->dwHighDateTime << 32) + ft->dwLowDateTime;
    i64 -= (SECS_BETWEEN_EPOCHS * NUM_100NS_PER_SEC);
    if (ns != NULL) {
        *ns = (long)((i64 % NUM_100NS_PER_SEC) * 100);
    }
    i64 /= NUM_100NS_PER_SEC;
    ASSERT((time_t)i64 == i64); // time_t overflow
    return (time_t)i64;
}

void Schedule::time_t2SystemTime(time_t timer, SYSTEMTIME* st)
{
    FILETIME ft;
    time_t2FileTime(timer, &ft);
    FileTimeToSystemTime(&ft, st);
}

void Schedule::time_t2FileTime(time_t timer, FILETIME* ft)
{
    // MSDN KB 167296
    __int64 i64;
    i64 = Int32x32To64(timer, NUM_100NS_PER_SEC);
    i64 += (SECS_BETWEEN_EPOCHS * NUM_100NS_PER_SEC);
    ft->dwLowDateTime  = (DWORD)i64;
    ft->dwHighDateTime = (DWORD)(i64 >> 32);
}
