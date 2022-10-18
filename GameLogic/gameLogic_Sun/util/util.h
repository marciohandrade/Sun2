#ifndef __UTIL_H__
#define __UTIL_H__
// util.h

//#include <log4cxx/tinylogger.h>

class CFileLogger;

extern CFileLogger* g_pFileLogger;

void trim(char *TargetStr);
void trimLeft(char *TargetStr);
void trimRight(char *TargetStr);

void _LOG(const char* szFormat, ...);
void WriteToFile(char* filepath, BYTE* lpData, int nSize);

#endif //__UTIL_H__