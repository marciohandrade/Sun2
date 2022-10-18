#ifndef __UTIL_DEBUG_H__
#define __UTIL_DEBUG_H__

// util_debug.h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DEBUG
//#define mprintf(args) outwnd_printf2 args
//#define nprintf(args) outwnd_printf args
#define mprintf(args) _LOG args
#define nprintf(args) _LOG2 args

#else
#define mprintf(args)
#define nprintf(args)
#endif

extern BOOL enable_debugLog;

void _LOG(const char* psz_format, ...);
void _LOG2(const char* psz, const char* psz_format, ...);


#ifdef __cplusplus
}
#endif

#endif //__UTIL_DEBUG_H__