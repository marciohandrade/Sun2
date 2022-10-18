#include "UtilityCommon.h"
#include ".\SolarDateTime.h"
#include <time.h>
#include "../ProgramCommon/CountryCode.h"

//==================================================================================================

namespace util {
;
//==================================================================================================
// (f100609.2L) add a client / server time synchronization facility
// reference : (f100415.1L)
TimeSync* TimeSync::time_sync_static_ = NULL;
// NOTE: f110928.1L, the heap static memory region initialization is worked by internal - exec.
// therefore, the context of '::GetCurrentThreadId()' calling is the main thread or dll main.
// this data field is only used to avoid 'CalculateTime' process by another threads.
static DWORD interchecker_main_thread_id_static = ::GetCurrentThreadId();

TimeSync::TimeSync()
{
    ZeroMemory(this, sizeof(*this));
}

inline TimeSync* TimeSync::Instance()
{
    if (time_sync_static_) {
        return time_sync_static_;
    }
    return CreateInstance();
}

TimeSync* TimeSync::CreateInstance()
{
    // thread un-safe
    static TimeSync time_sync_static;
    time_sync_static_ = &time_sync_static;
    InitializeQueryCounters(time_sync_static_);
    return time_sync_static_;
}

void TimeSync::InitializeQueryCounters(TimeSync* time_sync)
{
    time_sync->latest_tick_ = GetTickCount();
    time_sync->lifetime_start_ = time_sync->latest_tick_;
    time_sync->time_value_start_ = 0;
    time_sync->time_value_latest_ = 0;
    time_sync->delta_tick_ = 0;
    time_sync->sum_of_delta_tick_ = 0;
}


void TimeSync::CalculateTime()
{
    // CHANGES: f110928.2L,
    if (util::interchecker_main_thread_id_static != ::GetCurrentThreadId()) {
        return;
    };
    // (CHANGES) (f100415.1L) (WAVERIX) fix up delta tick error related to degree of accuracy
    const ULONGLONG current_tick = GetTickCount();
    ;     ULONGLONG calc_tick = current_tick;
    if (latest_tick_ > current_tick) {
        calc_tick += ULONG_MAX;
    };
    ::GetCurrentThreadId();
    delta_tick_ = static_cast<DWORD>(calc_tick - latest_tick_);
    latest_tick_ = current_tick;

    ULONGLONG lifetime_offset = (calc_tick - lifetime_start_) / 1000;
    time_value_latest_ = time_value_start_ + lifetime_offset;
#if defined(_SERVER)
    sum_of_delta_tick_ += delta_tick_;
    if (sum_of_delta_tick_ > (60 * 1000))
    {   // (f100612.1L) update time synchronization
        // because of a virtual timer has a loss of time on the software emulated pc
        Command_Standalone();
    }
#endif
}

// redirected interfaces
// to support a client/server time synchronization using the facade pattern
void TimeSync::GetLocalTime(LPSYSTEMTIME system_time)
{
    TimeSync* const this_ = TimeSync::Instance();
    if (this_->config_node_ != this_->eConfig_UseCSSync) {
        ::GetLocalTime(system_time);
        return;
    }
    //
    this_->CalculateTime();
    ZeroMemory(system_time, sizeof(*system_time));
    const tm* local_tm = ::_localtime64(&this_->time_value_latest_);
    if (!local_tm) {
        return;
    }
    system_time->wYear = static_cast<WORD>(local_tm->tm_year + 1900);
    system_time->wMonth = static_cast<WORD>(local_tm->tm_mon + 1);
    system_time->wDayOfWeek = static_cast<WORD>(local_tm->tm_wday);
    system_time->wDay = static_cast<WORD>(local_tm->tm_mday);
    system_time->wHour = static_cast<WORD>(local_tm->tm_hour);
    system_time->wMinute = static_cast<WORD>(local_tm->tm_min);
    system_time->wSecond = static_cast<WORD>(local_tm->tm_sec);
    system_time->wMilliseconds = static_cast<WORD>(this_->delta_tick_ % 1000);
}

void TimeSync::GetSystemTime(LPSYSTEMTIME system_time)
{
    TimeSync* const this_ = TimeSync::Instance();
    if (this_->config_node_ != this_->eConfig_UseCSSync) {
        ::GetSystemTime(system_time);
        return;
    }
    //
    this_->CalculateTime();
    ZeroMemory(system_time, sizeof(*system_time));
    const tm* local_tm = ::_gmtime64(&this_->time_value_latest_);
    if (!local_tm) {
        return;
    }
    system_time->wYear = static_cast<WORD>(local_tm->tm_year + 1900);
    system_time->wMonth = static_cast<WORD>(local_tm->tm_mon + 1);
    system_time->wDayOfWeek = static_cast<WORD>(local_tm->tm_wday);
    system_time->wDay = static_cast<WORD>(local_tm->tm_mday);
    system_time->wHour = static_cast<WORD>(local_tm->tm_hour);
    system_time->wMinute = static_cast<WORD>(local_tm->tm_min);
    system_time->wSecond = static_cast<WORD>(local_tm->tm_sec);
    system_time->wMilliseconds = static_cast<WORD>(this_->delta_tick_ % 1000);
}

#if !defined(_WIN64)
// WARNING : 값의 증가로 인해 오버플로 발생.
time_t TimeSync::time(time_t* _time)
{
    TimeSync* const this_ = TimeSync::Instance();
    if (this_->config_node_ != this_->eConfig_UseCSSync) {
        return ::time(_time);
    }
    this_->CalculateTime();
    // WARNING : 값의 증가로 인해 오버플로 발생.
    time_t time_value = static_cast<time_t>(this_->time_value_latest_);
    if (_time) {
        *_time = time_value;
    }
    return time_value;
}
#endif

__time64_t TimeSync::time(__time64_t* _time)
{
    TimeSync* const this_ = TimeSync::Instance();
    if (this_->config_node_ != this_->eConfig_UseCSSync) {
        return ::_time64(_time);
    }
    this_->CalculateTime();
    __time64_t time_value = this_->time_value_latest_;
    if (_time) {
        *_time = time_value;
    }
    return time_value;
}

// desc. : the server's utc time, __time64_t format
// (WARNING) don't support a 64bit extention time format, ranges=[x, 2038-01-18]
__time64_t TimeSync::_time64(__time64_t* _time64) // UTC
{
    TimeSync* const this_ = TimeSync::Instance();
    if (this_->config_node_ != this_->eConfig_UseCSSync) {
        return ::_time64(_time64);
    }
    this_->CalculateTime();
    __time64_t time_value = this_->time_value_latest_;
    if (_time64) {
        *_time64 = time_value;
    }
    return time_value;
}

// standalone mode
void TimeSync::Command_Standalone()
{
    util::TimeSyncNode node;
    ZeroMemory(&node, sizeof(node));
    Command_Clear();
    Command_ServerSend(&node);
    Command_ClientRecv(node);
}

// client / server time synchronization flow controllers
void TimeSync::Command_Clear()
{
    TimeSync* const this_ = TimeSync::Instance();
    ZeroMemory(this_, sizeof(*this_));
    InitializeQueryCounters(this_);
}

bool TimeSync::Command_ClientSend(TimeSyncNode* making_send_info)
{
    TimeSync* const this_ = TimeSync::Instance();
    ZeroMemory(making_send_info, sizeof(*making_send_info));
    if (this_->config_node_ != this_->eConfig_None) {
        return false;
    }
    InitializeQueryCounters(this_);
    this_->config_node_ = this_->eConfig_DoingTrans;
    making_send_info->time_stamp_client_1st = GetTickCount();
    return true;
}

// side-effect : time synchronization
bool TimeSync::Command_ServerRecv(TimeSyncNode* recv_info)
{
    //TimeSync* const this_ = TimeSync::Instance();
    recv_info->time_stamp_server_1st = GetTickCount();
    return true;
}

// clear a stream if a server-side send only
bool TimeSync::Command_ServerSend(TimeSyncNode* making_send_info)
{
    //TimeSync* const this_ = TimeSync::Instance();
    making_send_info->time_stamp_server_2nd = GetTickCount();
    TimeSync::GetLocalTime(&making_send_info->server_time);

    // CHANGES, f100830.2L, apply a daylight of a server time zone

    //_NA_008483_20150910_REMOVE_DAYLIGHT_TIME
#if defined (TRUE)
    making_send_info->daylight_bias = 0;
#else
    TIME_ZONE_INFORMATION time_zone_info;
    ::GetTimeZoneInformation(&time_zone_info);
    making_send_info->daylight_bias = time_zone_info.DaylightBias;
#endif

    return true;
}

// side-effect : time synchronization
bool TimeSync::Command_ClientRecv(const TimeSyncNode& send_info)
{
    TimeSync* const this_ = TimeSync::Instance();
    // "making_send_info->time_stamp_client_1st == 0"
    // - server side time synchronization command
    const bool duplex_sync = (send_info.time_stamp_client_1st != 0);
    if (duplex_sync)
    {
        if (this_->config_node_ != this_->eConfig_DoingTrans) {
            return false;
        }
    }
    DWORD time_stamp_client_2nd = GetTickCount();
    if (duplex_sync)
    {
        if (!FlowControl::FCAssert(
            send_info.time_stamp_client_1st <= time_stamp_client_2nd &&
            send_info.time_stamp_server_1st <= send_info.time_stamp_server_2nd))
        {
            return false;
        }
    }
    // reference :
    //  H. Kopetz, W. Ochsenreiter, Clock synchronization in distributed real-time systems,
    //  IEEE Transactions on Computers, v.36, i.8, p.933-940, August 1987
    // premise :
    //  must consider a network delay only to support a real-time synchronization.
    int simplex_network_delay_ticks = 0;
    if (duplex_sync)
    {
        simplex_network_delay_ticks = \
            (int(time_stamp_client_2nd - send_info.time_stamp_client_1st) -
             int(send_info.time_stamp_server_2nd - send_info.time_stamp_server_1st)) >> 1;
        //
        if (!FlowControl::FCAssert(simplex_network_delay_ticks >= 0)) {
            return false;
        }
    }
    //
    InitializeQueryCounters(this_);
    tm local_time = { 0, };
    ;{
#if !defined(_SERVER)
        //_NA_008483_20150910_REMOVE_DAYLIGHT_TIME
#if defined (_KOREA)
        long local_daylight = 0;
#else
        TIME_ZONE_INFORMATION time_zone_info;
        ::GetTimeZoneInformation(&time_zone_info);
        long local_daylight = time_zone_info.DaylightBias;
#endif
#endif
        local_time.tm_year = send_info.server_time.wYear - 1900;
        local_time.tm_mon  = send_info.server_time.wMonth - 1;
        local_time.tm_mday = send_info.server_time.wDay;
        local_time.tm_hour = send_info.server_time.wHour;
#ifdef _SERVER
        // CHANGES, f100830.2L, apply a daylight of a server time zone
        local_time.tm_min  = send_info.server_time.wMinute + send_info.daylight_bias;
#else
        local_time.tm_min  = send_info.server_time.wMinute;
        if (send_info.daylight_bias == local_daylight) {
            local_time.tm_min += send_info.daylight_bias;
        }
#endif
        local_time.tm_sec  = send_info.server_time.wSecond;
    };
#if !defined(_SERVER)
    ;{
        // (f100609.2L) (f100610.2L) the game logic has a complex condition
        // besides a network delay. on the contrary,
        // change to a more simple algorithm than a deep consideration.
        const int round_value = ((simplex_network_delay_ticks % 1000) > 500) ? 1 : 0;
        const int delayed_seconds = (simplex_network_delay_ticks / 1000) + round_value;
        local_time.tm_sec -= delayed_seconds;
    };
#endif
    ;{  // (NOTE) modification time value
        this_->time_value_start_ = ::_mktime64(&local_time);
        this_->time_value_latest_ = this_->time_value_start_;
    };
    this_->CalculateTime();
    this_->config_node_ = this_->eConfig_UseCSSync;

    return true;
}

//==================================================================================================
// NOTE: f100920.3L, busy access time inforamtion in SUN specification
// this structure is moved from SSQTimeControl
// sample
//  2010-09-20 14:48:30
//  WzTimeYYYYMMDDHHMMSS = 20100920144830
//  WzTime               = 1448     { h * 100 + m }
//  AcmTime              =  888     { h * 60 + m }
void TimeSync::UpdateTimeInfoPerFrame(TimeInfoPerFrame* OUT time_info)
{
    ZeroMemory(time_info, sizeof(*time_info));
    time_info->current_tick_ = GetTickCount();
    //time_info->delta_tick_
    SYSTEMTIME* const sys_time = &time_info->system_time;
    util::TimeSync::GetLocalTime(sys_time);
    time_info->wz_hhmm_  = sys_time->wHour * 100 + sys_time->wMinute;
    time_info->acm_time_ = sys_time->wHour *  60 + sys_time->wMinute;
    time_info->wz_yyyymmddhhmmss_ = (sys_time->wYear * 10000000000ULL)
                                  + (sys_time->wMonth *  100000000ULL)
                                  + (sys_time->wDay *      1000000ULL)
                                  + (sys_time->wHour *       10000ULL)
                                  + (sys_time->wMinute *       100ULL)
                                  + (sys_time->wSecond *         1ULL);
}

//==================================================================================================
// solar date time facilities
//

// 2006-12-26 24:00:60
const DWORD64& GetDateTime_YYYYMMDDHHMMSS(const int year IN, const int month IN, const int day IN,
                                          const int hour IN, const int minute IN,
                                          const int second IN,
                                          DWORD64& OUT result_of_wztime64)
{
    result_of_wztime64 = ((DWORD64)year * 10000000000)
                       + ((DWORD64)month * 100000000)
                       + ((DWORD64)day * 1000000)
                       + ((DWORD64)hour * 10000)
                       + (minute) * 100
                       + (second);
    return result_of_wztime64;
}

// 2006-12-26
const DWORD& GetDate_YYYYMMDD(DWORD& OUT result_of_wztime)
{
    SYSTEMTIME local_time;
    util::TimeSync::GetLocalTime(&local_time);
    result_of_wztime = ((DWORD)local_time.wYear * 10000)
                     + (local_time.wMonth * 100)
                     + (local_time.wDay);
    return result_of_wztime;
}

// 2006-12-26 24:00
const DWORD64& GetDateTime_YYYYMMDDHHMM(DWORD64& OUT result_of_wztime64)
{
    SYSTEMTIME local_time;
    util::TimeSync::GetLocalTime(&local_time);
    result_of_wztime64 = ((DWORD64)local_time.wYear * 100000000)
                       + ((DWORD64)local_time.wMonth * 1000000)
                       + ((DWORD64)local_time.wDay * 10000)
                       + (local_time.wHour * 100)
                       + (local_time.wMinute);
    return result_of_wztime64;
}

// 2006-12-26 24:00:01
const DWORD64& GetDateTime_YYYYMMDDHHMMSS(DWORD64& OUT result_of_wztime64)
{
    SYSTEMTIME local_time;
    util::TimeSync::GetLocalTime(&local_time);
    result_of_wztime64 = ((DWORD64)local_time.wYear * 10000000000)
                       + ((DWORD64)local_time.wMonth * 100000000)
                       + ((DWORD64)local_time.wDay * 1000000)
                       + ((DWORD64)local_time.wHour * 10000)
                       + (local_time.wMinute * 100)
                       + (local_time.wSecond);
    return result_of_wztime64;
}

// 2006-12-26 24:00:01.123
const DWORD64& GetDateTime_YYYYMMDDHHMMSSNNN(DWORD64& OUT result_of_wztime64)
{
    SYSTEMTIME local_time;
    util::TimeSync::GetLocalTime(&local_time);
    result_of_wztime64 = ((DWORD64)local_time.wYear * 10000000000000)
                       + ((DWORD64)local_time.wMonth * 100000000000)
                       + ((DWORD64)local_time.wDay * 1000000000)
                       + ((DWORD64)local_time.wHour * 10000000)
                       + ((DWORD64)local_time.wMinute * 100000)
                       + (local_time.wSecond * 1000)
                       + (local_time.wMilliseconds);
    return result_of_wztime64;
}

// convert '__time64_t' to 'YYYYMMDDHHMMSS'
DWORD64 GetDateTime_YYYYMMDDHHMMSS(__time64_t datetime)
{
    SYSTEMTIME system_time;
    ZeroMemory(&system_time, sizeof(system_time));
    const tm* local_tm = ::_localtime64(&datetime);
    system_time.wYear   = static_cast<WORD>(local_tm->tm_year + 1900);
    system_time.wMonth  = static_cast<WORD>(local_tm->tm_mon + 1);
    //system_time.wDayOfWeek = static_cast<WORD>(local_tm->tm_wday);
    system_time.wDay    = static_cast<WORD>(local_tm->tm_mday);
    system_time.wHour   = static_cast<WORD>(local_tm->tm_hour);
    system_time.wMinute = static_cast<WORD>(local_tm->tm_min);
    system_time.wSecond = static_cast<WORD>(local_tm->tm_sec);

    DWORD64 datetime64;

    return GetDateTime_YYYYMMDDHHMMSS(
        system_time.wYear, 
        system_time.wMonth, 
        system_time.wDay, 
        system_time.wHour, 
        system_time.wMinute, 
        system_time.wSecond, datetime64);
}

// 06-12-26
const int kMaxYearStringLength = 4;
const DWORD& GetDate_YYMMDD(DWORD& OUT result_of_wztime)
{
    SYSTEMTIME local_time;
    util::TimeSync::GetLocalTime(&local_time);

    char szYY[kMaxYearStringLength + 1];
    GetDateFormat(NULL, 0, &local_time, "yy", szYY, kMaxYearStringLength);
    szYY[kMaxYearStringLength] = '\0';

    result_of_wztime = (atoi(szYY) * 10000)
                     + (local_time.wMonth * 100)
                     + (local_time.wDay);
    return result_of_wztime;
}

// 06-12-26 24:00
const DWORD& GetDateTime_YYMMDDHHMM(DWORD& OUT result_of_wztime)
{
    SYSTEMTIME local_time;
    util::TimeSync::GetLocalTime(&local_time);

    char szYY[kMaxYearStringLength + 1];
    GetDateFormat(NULL, 0, &local_time, "yy", szYY, kMaxYearStringLength);
    szYY[kMaxYearStringLength] = '\0';

    result_of_wztime = ((DWORD)atoi(szYY) * 100000000)
                     + ((DWORD)local_time.wMonth * 1000000)
                     + ((DWORD)local_time.wDay * 10000)
                     + (local_time.wHour * 100)
                     + (local_time.wMinute);
    return result_of_wztime;
}

// 06-12-26 24:00:01
const DWORD64& GetDateTime_YYMMDDHHMMSS(DWORD64& OUT wztime64_value)
{
    SYSTEMTIME local_time;
    util::TimeSync::GetLocalTime(&local_time);

    char szYY[kMaxYearStringLength + 1];
    GetDateFormat(NULL, 0, &local_time, "yy", szYY, kMaxYearStringLength);
    szYY[kMaxYearStringLength] = '\0';

    wztime64_value = ((DWORD64)atoi(szYY) * 10000000000)
                   + ((DWORD64)local_time.wMonth * 100000000)
                   + ((DWORD64)local_time.wDay * 1000000)
                   + ((DWORD64)local_time.wHour * 10000)
                   + (local_time.wMinute * 100)
                   + (local_time.wSecond);
    return wztime64_value;
}

// (f100611.1L) add a convert function for 'tm'
void YYYYMMDDHHMMSSToTMStructure(DWORD64 wztime64_value IN, tm* result_tm OUT)
{
    ZeroMemory(result_tm, sizeof(*result_tm));
    result_tm->tm_year = static_cast<int>(wztime64_value / 10000000000ULL);
    wztime64_value %= 10000000000ULL;
    result_tm->tm_mon  = static_cast<int>(wztime64_value / 100000000ULL);
    wztime64_value %= 100000000ULL;
    result_tm->tm_mday = static_cast<int>(wztime64_value / 1000000ULL);
    wztime64_value %= 1000000ULL;
    result_tm->tm_hour = static_cast<int>(wztime64_value / 10000ULL);
    wztime64_value %= 10000ULL;
    result_tm->tm_min  = static_cast<int>(wztime64_value / 100ULL);
    wztime64_value %= 100ULL;
    result_tm->tm_sec  = static_cast<int>(wztime64_value);
}

// for displaying client
//wztime64_value == "20061023241200"
void YYYYMMDDHHMMSSToSYSTEMTIME(DWORD64 wztime64_value IN, SYSTEMTIME& system_time OUT)
{
    ZeroMemory(&system_time, sizeof(system_time));
    system_time.wYear   = static_cast<WORD>(wztime64_value / 10000000000ULL);
    wztime64_value %= 10000000000ULL;
    system_time.wMonth  = static_cast<WORD>(wztime64_value / 100000000ULL);
    wztime64_value %= 100000000ULL;
    system_time.wDay    = static_cast<WORD>(wztime64_value / 1000000ULL);
    wztime64_value %= 1000000ULL;
    system_time.wHour   = static_cast<WORD>(wztime64_value / 10000ULL);
    wztime64_value %= 10000ULL;
    system_time.wMinute = static_cast<WORD>(wztime64_value / 100ULL);
    wztime64_value %= 100ULL;
    system_time.wSecond = static_cast<WORD>(wztime64_value);
}

#define _MAX_SMALLDATETIME_SIZE     (16)
#define _MAX_DATETIME_SIZE          (23)

//"2006-10-23 10:00" : strlen = 16
// sql : smalldatetime string
void GetSQLDateTime_YYYYMMDDHHMM(DWORD64 wztime64_value IN,
                                 char* result_datetime_string OUT/*[MAX_SMALLDATETIME_SIZE+1=17]*/)
{
#ifdef _DEBUG
    assert(!IsBadWritePtr(result_datetime_string, _MAX_SMALLDATETIME_SIZE));
    assert(!IsBadReadPtr(result_datetime_string, _MAX_SMALLDATETIME_SIZE));
#endif
    DWORD64 num=0;
    for (int i = (_MAX_SMALLDATETIME_SIZE - 1); i >= 0; --i)
    {
        switch (i)
        {
        case 13: result_datetime_string[i] = ':'; break;
        case 10: result_datetime_string[i] = ' '; break;
        case  7:
        case  4: result_datetime_string[i] = '-'; break;
        default:
            {
                num = wztime64_value % 10;
                wztime64_value = wztime64_value / 10;
                result_datetime_string[i] = '0' + (char)num;
            }
            break;
        }
    }
    result_datetime_string[_MAX_SMALLDATETIME_SIZE] = '\0';
}

//"12345678901234567890123" : strlen = 23
//"2006-10-23 10:00:00.123"
// sql : datetime string
void GetSQLDateTime_YYYYMMDDHHMMSSNNN(DWORD64 wztime64_value IN,
                                      char* result_datetime_string OUT/*[MAX_DATETIME_SIZE+1=24]*/)
{
#ifdef _DEBUG
    assert(!IsBadWritePtr(result_datetime_string, _MAX_DATETIME_SIZE));
    assert(!IsBadReadPtr(result_datetime_string, _MAX_DATETIME_SIZE));
#endif
    DWORD64 num=0;
    for (int i = (_MAX_DATETIME_SIZE - 1)/*16+6*/; i >= 0; --i)
    {
        switch (i)
        {
        case 19: result_datetime_string[i] = '.'; break;
        case 16:
        case 13: result_datetime_string[i] = ':'; break;
        case 10: result_datetime_string[i] = ' '; break;
        case  7:
        case  4: result_datetime_string[i] = '-'; break;
        default:
            {
                num = wztime64_value % 10;
                wztime64_value = wztime64_value / 10;
                result_datetime_string[i] = '0'+(char)num;
            }
            break;
        }
    }
    result_datetime_string[_MAX_DATETIME_SIZE] = '\0';
}

// _NA001385_20090924_DOMINATION_EVENT_CONTROLL
WORD GetDateTime_Week_HHMMSS(DWORD& OUT result_of_wztime)
{
    SYSTEMTIME local_time;
    util::TimeSync::GetLocalTime(&local_time);

    result_of_wztime = (local_time.wHour * 10000)
                     + (local_time.wMinute * 100)
                     + (local_time.wSecond);
    return local_time.wDayOfWeek;
}

DWORD HHMMSSToSecond(DWORD wztime_value)
{
    const DWORD dwTime_h = (wztime_value / 10000);
    const DWORD dwTime_m = (wztime_value / 100)%100;

    return (dwTime_h * 3600) + (dwTime_m * 60) + (wztime_value % 100);
}

DWORD SecondToHHMMSS(DWORD wztime_value)
{
    const DWORD wzTime_h = (wztime_value / 3600);
    const DWORD wzTime_m = (wztime_value / 60) % 60;
    const DWORD wzTime_s = (wztime_value % 60);

    return wzTime_h * 10000 + wzTime_m * 100 + wzTime_s;
}

DWORD CarcHHMMSS(DWORD wztime_value_l, DWORD wztime_value_r, bool plus_operation)
{
    const DWORD second_l = HHMMSSToSecond(wztime_value_l);
    const DWORD second_r = HHMMSSToSecond(wztime_value_r);

    DWORD wzTime_s = second_l;
    if (!plus_operation)
    {
        if (second_l <= second_r) {
            return 0;
        }
        wzTime_s -= second_r; 
    }
    else
    {
        wzTime_s += second_r; 
    }
    return SecondToHHMMSS(wzTime_s);
}

DWORD64 AddMinuteToYYYYDDHHMMSS(DWORD64 source_time, DWORD add_minute)
{
    tm sorce_tm = {0,};
    __time64_t sorce_time_t;
    __time64_t result_time_t;

    YYYYMMDDHHMMSSToTMStructure(source_time, &sorce_tm);
    sorce_tm.tm_year -= 1900;
    sorce_tm.tm_mon -= 1;
    sorce_time_t = mktime(&sorce_tm);
    result_time_t = sorce_time_t + (add_minute * 60);

    return GetDateTime_YYYYMMDDHHMMSS(result_time_t);
}

}; //end of namespace
