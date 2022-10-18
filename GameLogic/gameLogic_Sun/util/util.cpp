// util.cpp

#include "stdafx.h"
#include <string.h>
#include <Windows.h>

#include "util.h"
#include "FileLogger.h"

void WriteToFile(char* filepath, BYTE* lpData, int nSize)
{
	HANDLE hFile = CreateFile( filepath,           // create MYFILE.TXT 
		GENERIC_WRITE,                // open for writing 
		0,                            // do not share 
		NULL,                         // no security 
		CREATE_ALWAYS,                // overwrite existing 
		FILE_ATTRIBUTE_NORMAL,
		NULL);                        // no attr. template 

	


	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		//		_LOG("Could not open file. %s", filepath);  // process error 
		return;
	} 

	DWORD dwWritten;
	WriteFile(hFile, (BYTE*)lpData, nSize, &dwWritten, NULL);
	CloseHandle(hFile);
}

CFileLogger* g_pFileLogger = 0;

//#define LOGBUFFER_SIZE		4096
#define LOGBUFFER_SIZE		8192
void _LOG(const char* szFormat, ...)
{
	char		szBuf[LOGBUFFER_SIZE] = {0};
	va_list list;
	va_start (list, szFormat);
	vsprintf (szBuf, szFormat, list);
	va_end (list);

	OutputDebugString(szBuf);
	//_debug(szBuf);

 //VC 2005 ¿Ã«œ∏∏
#if _MSC_VER < 1400
	if (g_pFileLogger)
    {
        g_pFileLogger->Log(szBuf);
    }		
#endif
}


//Removes spaces and tabs from right end of string

void trimLeft(char *TargetStr)
{
	int i, nNewStrSize;
	i = 0;

	while ((TargetStr[i] == ' ') || (TargetStr[i] == '\t'))
		i++;

	if (i > 0)
	{
		nNewStrSize = strlen(TargetStr) - i + 1;
		memmove(TargetStr, TargetStr + i, nNewStrSize);
	}
}


//Removes spaces and tabs from left end of string
void trimRight(char *TargetStr)
{

	int i;



	i = strlen(TargetStr) - 1;

	while ((i >= 0) && ((TargetStr[i] == ' ') || (TargetStr[i] == '\t')) )

		i--;

	TargetStr[i+1] = 0;

}


//Removes spaces and tabs from both ends of a string
void trim(char *TargetStr)
{
	trimRight(TargetStr);
	trimLeft(TargetStr);
}