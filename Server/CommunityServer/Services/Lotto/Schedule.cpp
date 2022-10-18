#include "WorldServerPch.hxx"
#include "./Schedule.h"

// implemented by __NA_001359_20090619_HEIM_LOTTO_SYSTEM

static const __int64 SECS_BETWEEN_EPOCHS = 11644473600;
static const __int64 NUM_100NS_PER_SEC   = 10000000;    // 10^(9-2)
static const __int32 SECS_PER_MINUTE     = 60;
static const __int32 SECS_PER_HOUR       = SECS_PER_MINUTE * 60;
static const __int32 SECS_PER_DAY        = SECS_PER_HOUR * 24;

// 32-bit time_t�� 19:14:07, January 18, 2038, UTC������ ��ȿ�ϳ�, ���� ���񽺿��� ���

int ScheduleToken::SecondsTo() const
{
    // time(NULL)�� UTC ������� �����ϹǷ�
    // �� ���� �����ϰ� ���� SYSTEMTIME���� ��ȯ�ؾ� �Ѵ�
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

    // ���ڿ� ǥ�� ĳ��
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

    time_t diff = 0; // �� ������, ���� �������� ���� �ð�
    for (int i = 0; i <= Schedule::NUM_DAYS_OF_WEEK; ++i) // ���� �� ���� ���ϱ��� ��ȯ �˻�
    {
        int j = (st.wDayOfWeek + i);
        if (j >= Schedule::NUM_DAYS_OF_WEEK) { j %= Schedule::NUM_DAYS_OF_WEEK; }
        if ((daysOfWeek_ & (1 << j)) != 0) // ���� ��ġ
        {
            for (int k = (i == 0 ? st.wHour : 0); k < 24; ++k) // �����̸� ���� �ð����� �˻�
            {
                if ((hours_ & (1 << k)) != 0) // �� ��ġ
                {
                    // �� ������ �������� �����Ƿ�, ������ ���� �ð��̸� ������ ���� �к��� �˻�
                    for (int l = ((i == 0 && k == st.wHour) ? (st.wMinute + 1) : 0); l < 60; ++l)
                    {
                        if ( i == 0 && k == st.wHour && l == (st.wMinute + 1) ) {
                            diff += (SECS_PER_MINUTE - st.wSecond);
                        }

                        if ((minutes_ & ((DWORD64)1 << l)) != 0) // �� ��ġ
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
