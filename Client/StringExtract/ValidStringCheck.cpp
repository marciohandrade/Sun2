// ValidStringCheck.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


//TODO : 주석처리된 문자열은 걸러 내도록 할것.

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
		MessageBox(NULL,"검색 완료" ,"알림" , MB_OK);
	}
	fclose(pFile);

	return 0;
}


