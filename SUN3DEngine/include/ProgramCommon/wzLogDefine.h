#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif


#include <Windows.h>


namespace WzLogType
{
    enum WZLOG_TYPE
    {
        WZERR = 0,
        WZWAR,
        WZINF,
        WZDBG,
        WZTRA,
        WZPKT1,
        WZPKT2,
        WZPKT3,
        WZNUM_LOGLVL
    };
};

using namespace WzLogType;
