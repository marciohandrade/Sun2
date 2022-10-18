// wzlogtype.h

#ifndef _PROGRAMCOMMON_WZLOGTYPE_H_
#define _PROGRAMCOMMON_WZLOGTYPE_H_

#define VC_EXTRALEAN

#include <windows.h>
#include <assert.h>

enum WZLOG_TYPE
{
    WZLGT_UNKNOWN = -1,
    WZLGT_FILE,
    WZLGT_CONSOLE,
};

enum WZLOG_LEVEL
{
    WZERR = 0,
    WZWAR,
    WZINF,
    WZDBG,
    WZTRA,
    WZNUM_LOGLVL
};

struct WzLogCreateInfo
{
    WZLOG_TYPE type;
    WZLOG_LEVEL level;
    char filePath[MAX_PATH];
};

#endif // _PROGRAMCOMMON_WZLOGTYPE_H_