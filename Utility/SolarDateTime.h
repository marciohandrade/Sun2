#pragma once
#ifndef UTILITY_SOLARDATETIME_H
#define UTILITY_SOLARDATETIME_H

//==================================================================================================
/// 시간 관련 조작 함수들
/**
    @author
        Kim Min Wook < taiyo@webzen.co.kr > 
    @since
        2006. 4. 27
    @remarks
    - 
    @note
    - 
    @history
    - ~2010.06.09
    - 2010.06.09|waverix|code arrangment and
                         add time sync facility
*/
//==================================================================================================

namespace util {
;

struct TimeInfoPerFrame;

//==================================================================================================
// (f100609.2L) add a client / server time synchronization facility
// related contents : _NA000000_100609_CS_TIME_SYNCHRONIZATION_
// reference : (f100415.1L)
class TimeSync
{
public:
    // redirected interfaces
    // to support a client/server time synchronization using the facade pattern
    // desc. : the server's local time, windows format
    static void GetLocalTime(LPSYSTEMTIME system_time);
    // desc. : the server's utc time, windows format
    static void GetSystemTime(LPSYSTEMTIME system_time); // UTC
#if !defined(_WIN64)
    // desc. : the server's utc time, time_t format
    static time_t time(time_t* _time); // UTC
#endif
    // desc. : the server's utc time, __time64_t format
    static __time64_t time(__time64_t* _time); // UTC
    // desc. : the server's utc time, __time64_t format
    static __time64_t _time64(__time64_t* _time64); // UTC
    //
    // standalone mode
    static void Command_Standalone();
    // client / server time synchronization flow controllers
    static void Command_Clear();
    static bool Command_ClientSend(TimeSyncNode* making_send_info);
    // side-effect : time synchronization
    static bool Command_ServerRecv(TimeSyncNode* recv_info);
    // clear a stream if a server-side send only
    static bool Command_ServerSend(TimeSyncNode* making_send_info);
    // side-effect : time synchronization
    static bool Command_ClientRecv(const TimeSyncNode& send_info);
    //
    // f100920.3L, busy access time inforamtion in SUN specification
    static void UpdateTimeInfoPerFrame(TimeInfoPerFrame* OUT result_time_info);
private:
    enum eConfig {
        eConfig_None       = 0x00,
        eConfig_UseCSSync  = 0x01,
        eConfig_DoingTrans = 0x02,
    };
    TimeSync();
    static TimeSync* Instance();
    static TimeSync* CreateInstance();
    static void InitializeQueryCounters(TimeSync* time_sync);
    void CalculateTime();
    //
    BYTE config_node_;
    ULONGLONG latest_tick_;
    ULONGLONG lifetime_start_;
    __time64_t time_value_start_;
    __time64_t time_value_latest_;
    DWORD delta_tick_; // low level degree of accuracy
    DWORD sum_of_delta_tick_; // virtual timer sync updater in server
    //
    static TimeSync* time_sync_static_;
};


//==================================================================================================
// NOTE: f100920.3L, busy access time inforamtion in SUN specification
// this structure is moved from SSQTimeControl in the agent server
// sample
//  2010-09-20 14:48:30
//  WzTimeYYYYMMDDHHMMSS = 20100920144830
//  WzTime               = 1448     { h * 100 + m }
//  AcmTime              =  888     { h * 60 + m }
struct TimeInfoPerFrame
{
    typedef uint16_t WzTmFmt;
    typedef uint16_t AcmTmFmt;
    //
    DWORD       current_tick_;
    DWORD       delta_tick_; // not set by 'UpdateTimeValue', user defined value
    WzTmFmt     wz_hhmm_;  // Webzen Time Format
    AcmTmFmt    acm_time_; // Accumulated Time Format
    DWORD64     wz_yyyymmddhhmmss_;
    SYSTEMTIME  system_time;
};

//==================================================================================================
// solar date time facilities

const DWORD64& GetDateTime_YYYYMMDDHHMMSS(const int year IN, const int month IN, const int day IN,
                                          const int hour IN, const int minute IN,
                                          const int second IN,
                                          DWORD64& OUT result_of_wztime64);
const DWORD& GetDate_YYYYMMDD(DWORD& OUT result_of_wztime);
const DWORD64& GetDateTime_YYYYMMDDHHMM(DWORD64& OUT result_of_wztime64);
const DWORD64& GetDateTime_YYYYMMDDHHMMSS(DWORD64& OUT result_of_wztime64);
const DWORD64& GetDateTime_YYYYMMDDHHMMSSNNN(DWORD64& OUT result_of_wztime64);

const DWORD& GetDate_YYMMDD(DWORD& OUT result_of_wztime);
const DWORD& GetDateTime_YYMMDDHHMM(DWORD& OUT result_of_wztime);
const DWORD64& GetDateTime_YYMMDDHHMMSS(DWORD64& OUT result_of_wztime);

// (f100611.1L) add a convert function for 'tm'
void YYYYMMDDHHMMSSToTMStructure(DWORD64 wztime64_value IN, tm* result_tm OUT);
void YYYYMMDDHHMMSSToSYSTEMTIME(DWORD64 wztime64_value IN, SYSTEMTIME& system_time OUT);

void GetSQLDateTime_YYYYMMDDHHMM(DWORD64 wztime64_value IN,
                                 char* result_datetime_string OUT/*[MAX_SMALLDATETIME_SIZE+1=17]*/);
void GetSQLDateTime_YYYYMMDDHHMMSSNNN(DWORD64 wztime64_value IN,
                                      char* result_datetime_string OUT/*[MAX_DATETIME_SIZE+1=24]*/);
DWORD64 GetDateTime_YYYYMMDDHHMMSS(__time64_t datetime); // convert '__time64_t' to 'YYYYMMDDHHMMSS'

// _NA001385_20090924_DOMINATION_EVENT_CONTROLL
WORD GetDateTime_Week_HHMMSS(DWORD& OUT result_of_wztime);
DWORD HHMMSSToSecond(DWORD wztime_value);
DWORD SecondToHHMMSS(DWORD wztime_value);
DWORD CarcHHMMSS(DWORD wztime_value_l, DWORD wztime_value_r, bool plus_operation = true);

DWORD64 AddMinuteToYYYYDDHHMMSS(DWORD64 source_time, DWORD add_minute);
}; //end of namespace

#endif //UTILITY_SOLARDATETIME_H