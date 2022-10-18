// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// ParfeitLogConsole.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

TCHAR g_szCurrentPath[MAX_PATH] = {0};

void PrintUserColorString(HANDLE hStdOut, WORD Attribute, TCHAR* str) {
	SetConsoleTextAttribute(hStdOut, Attribute);
	_putts(str); 
	SetConsoleTextAttribute(hStdOut, USER_WHITE);
}

//ccnet.test
