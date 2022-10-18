// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// ProcessSnapShot.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.
void LOGF(const char* pszFormat,...)
{
	char szDebug[512];
	char szPath[MAX_PATH] = {'0'};
	char szMsg[MAX_PATH] = {'0'};

	ZeroMemory(szDebug, sizeof(szDebug));

	StringCchCopy(szMsg, sizeof(szMsg), pszFormat);

	SYSTEMTIME time;
	GetLocalTime(&time);

	GetModuleFileName(GetModuleHandle(NULL), szPath, MAX_PATH);

	*(strrchr( szPath, '\\' ) ) = 0;

	StringCchPrintf(szDebug, sizeof(szDebug), _T("%s\\ProcessLog%04d%02d%02d.txt"), szPath, time.wYear, time.wMonth, time.wDay);

	FILE *pFile = fopen(szDebug, "a+");

	fseek(pFile, SEEK_END , 0);

	StringCchCat(szMsg, sizeof(szMsg), _T("\r\n"));
	fprintf(pFile, "[%04d-%02d-%02d %02d시 %02d분 %02d초:%04d] %s", time.wYear, time.wMonth, time.wDay, time.wHour,time.wMinute, time.wSecond, time.wMilliseconds, szMsg);
/*
	va_list list;
	va_start(list, pszFormat);

	vsprintf(szDebug, pszFormat, list);

	vfprintf(pFile, pszFormat, list);

	va_end(list);
*/
	fclose(pFile);

}