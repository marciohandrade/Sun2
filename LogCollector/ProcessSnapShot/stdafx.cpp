// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// ProcessSnapShot.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"

// TODO: �ʿ��� �߰� �����
// �� ������ �ƴ� STDAFX.H���� �����մϴ�.
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
	fprintf(pFile, "[%04d-%02d-%02d %02d�� %02d�� %02d��:%04d] %s", time.wYear, time.wMonth, time.wDay, time.wHour,time.wMinute, time.wSecond, time.wMilliseconds, szMsg);
/*
	va_list list;
	va_start(list, pszFormat);

	vsprintf(szDebug, pszFormat, list);

	vfprintf(pFile, pszFormat, list);

	va_end(list);
*/
	fclose(pFile);

}