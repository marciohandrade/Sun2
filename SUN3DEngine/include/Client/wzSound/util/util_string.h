#ifndef __UTIL_STRING_H__
#define __UTIL_STRING_H__

#ifdef __cplusplus
extern "C" {
#endif

char   *str_trim(char *psz);
char   *str_delete_substr(char *strbuf, char *strtodel);

int hexVal(char c);
void trimLeft(char *TargetStr);
void trimInet(char *TargetStr);
void trimRight(char *TargetStr);
void trim(char *TargetStr);
void trimDirSeparator(char *TargetStr);
//void getWord(char *DestStr, const char *SourceStr, const int Start, int &End);
//void getLastWord(char *DestString, const char *SourceStr, int &Start);
void splitPath(char *Path, char *Dir, char *FileName);
void getExtension(char *Path, char *Extention);
void translateEscapeString(char *TargetStr);
void translateFilepath(char *TargetStr);

////////////////////////////////////////////////////////////////////////////////
//	Some basic string utility stuff
void STR_AllocSetString(char** ppcDest, const char* pcSource, const BOOL bFreeExisting);

#ifdef __cplusplus
}
#endif

#endif //__UTIL_STRING_H__