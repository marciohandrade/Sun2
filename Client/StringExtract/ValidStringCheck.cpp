// ValidStringCheck.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//


//TODO : �ּ�ó���� ���ڿ��� �ɷ� ������ �Ұ�.

#include "stdafx.h"
#include "StringCheck.h"
#include  <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	StringCheck check;
	FILE * pFile;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	TCHAR outputName[512];
	sprintf(outputName , "SRQ_%d_%d_%d_%d.txt" ,  sysTime.wMonth , sysTime.wDay,sysTime.wHour , sysTime.wMinute);
	pFile = fopen(outputName , "w+");
	BOOL bRet = check.ExtractString(pFile ,"gameclient");
	if( bRet)
	{
		MessageBox(NULL,"�˻� �Ϸ�" ,"�˸�" , MB_OK);
	}
	fclose(pFile);

	return 0;
}


