// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// ParfeitLogConsole.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

#include "stdafx.h"

TCHAR g_szCurrentPath[MAX_PATH] = {0};

void PrintUserColorString(HANDLE hStdOut, WORD Attribute, TCHAR* str) {
	SetConsoleTextAttribute(hStdOut, Attribute);
	_putts(str); 
	SetConsoleTextAttribute(hStdOut, USER_WHITE);
}

//ccnet.test
