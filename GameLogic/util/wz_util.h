#ifndef __WZ_UTIL_H__
#define __WZ_UTIL_H__
// wz_util.h

#include <string>
#include <fstream>
using namespace std;

//extern FILE* logFile_;

void _LOG(const char* szFormat, ...);
void IntToChar4(int nData, char* pszBuf2);
std::string GetPrintFormat(char* pszData, int nSize);


void WriteToFile(char* filepath, char* lpData, int nSize);
void ReadFromFile(char* filepath, char* lpData, int& nSize);

#endif //__WZ_UTIL_H__