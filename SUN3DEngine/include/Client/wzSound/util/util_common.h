#ifndef __UTIL_COMMON_H__
#define __UTIL_COMMON_H__
// util_common.h

BOOL GetHostInfo(char* hostName, char* hostAddr);
LPCTSTR GenerateDmpName();
void CrashResult( LPCTSTR stackWalkLog, LPCTSTR dumpFileName );


#endif //__UTIL_COMMON_H__