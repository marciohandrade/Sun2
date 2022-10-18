#pragma once

#ifndef PRAGRAMCOMMON_ATTENDANCE_PARSER_H
#define PRAGRAMCOMMON_ATTENDANCE_PARSER_H
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#include <Singleton.h>
#include <ScriptCode.IParser.h>
#include "Struct.h"

class AttendanceParser : public util::Singleton<AttendanceParser>, public IParser
{
public:
    AttendanceParser(void){};
    ~AttendanceParser(void){};

public:
    /** Parser **/
    virtual void Release(){};
    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

    /** 출석 정보 **/
    const ATTENDANCE* GetAttendance(DWORD year, BYTE month);

private:
    const bool IsValidAttendance(ATTENDANCE* attendance);

    typedef std::pair<DWORD, BYTE> ATTENDANCE_TAG;
    typedef std::map<ATTENDANCE_TAG, ATTENDANCE> ATTENDANCE_MAP;
    ATTENDANCE_MAP attendance_map;
};

#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#endif // PRAGRAMCOMMON_ATTENDANCE_PARSER_H