#include "WorldServerPch.hxx"

#include <crtdbg.h>

namespace ns_world {
;

//WorldLog* g_pLOG = NULL;
CSunLog* count_log_global = NULL;

extern bool MessageCount(eSUN_LOG_LEVEL log_level, const char* meta_format, ...);

}; //end of namespace


bool ns_world::MessageCount(eSUN_LOG_LEVEL log_level, const char* meta_format, ...)
{
    bool result = false;
    va_list args; 
    va_start(args, meta_format);

    if (count_log_global)
    {
        result = (CSunLog::LOGGINGwithARG(
            count_log_global,
            CSunLog::cOUTPUT_BOTH,
            (CSunLog::eLOGLEVEL)log_level,
            CSunLog::cCATE_CONNECTION,
            meta_format,
            args
            ) != false);
    }
    else
    {
        char message[1024];
        _tcsncpy(message, SUNLOG_ENUM_TO_STRING(cCATE_CONNECTION), 100);
        int written = (int)_tcslen(message);
        result = (_vsntprintf(message+written, _countof(message) - written,
                    meta_format, args) >= 0);
    }
    va_end(args);

    return result;
}
