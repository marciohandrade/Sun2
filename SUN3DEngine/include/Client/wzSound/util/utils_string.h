#ifndef __UTILS_STRING_H__
#define __UTILS_STRING_H__
// utils_string.h

//#ifdef __cplusplus
//extern "C" {
//#endif

int hexVal(char c);
void trimLeft(char *TargetStr);
void trimInet(char *TargetStr);
void trimRight(char *TargetStr);
void trim(char *TargetStr);
void getWord(char *DestStr, const char *SourceStr, const int Start, int &End);
void getLastWord(char *DestString, const char *SourceStr, int &Start);
void splitPath(char *Path, char *Dir, char *FileName);
void getExtension(char *Path, char *Extention);
void translateEscapeString(char *TargetStr);

//#ifdef __cplusplus
//}
//#endif

#endif //__UTILS_STRING_H__