// wz_util.cpp

#include "../StdAfx.h"
#include "wz_util.h"

//FILE*  logFile_ = 0;

#define LOGBUFFER_SIZE	2048*2
void _LOG(const char* szFormat, ...)
{
//	return;

	char		szBuf[LOGBUFFER_SIZE] = {0};
	va_list list;
	va_start (list, szFormat);
	vsprintf (szBuf, szFormat, list);
	va_end (list);

	OutputDebugString(szBuf);
	
//	strcat(szBuf, "\n");
//	int numwritten = fwrite(szBuf, sizeof(char), strlen(szBuf), logFile_);
	
//	printf("%s\n", szBuf);
}


void IntToChar4(int nData, char* pszBuf2)
{
	char pszBuf1[4+1] = {0};
	_itoa(nData, pszBuf1, 10);

	int nBuf1Len = strlen(pszBuf1);
	int nDiff = 4- nBuf1Len;
	if( nDiff != 0)
	{
		for(int n = 0; n < nDiff; n++)
		{
			pszBuf2[n] = '0';
		}

		int m = 0;
		for( n = nDiff; n < 4; n++, m++)
		{
			pszBuf2[n] = pszBuf1[m];
		}
	}
	else
	{
		strcpy(pszBuf2, pszBuf1);
	}
}

std::string GetPrintFormat(char* pszData, int nSize)
{
//	char* pszBuf = new char[nSize + 1];

	char pszBuf[1024] = {0};
//	memset(pszBuf, 0, nSize + 1);
	_snprintf(pszBuf, nSize, "%s", pszData);
//	pszBuf[nSize] = '\0';
	std::string strBuf = pszBuf;
	return strBuf;
}

void WriteToFile(char* filepath, char* lpData, int nSize)
{
	HANDLE hFile; 
	
	hFile = CreateFile(filepath,           // create MYFILE.TXT 
		GENERIC_WRITE,                // open for writing 
		FILE_SHARE_READ,                            // do not share 
		NULL,                         // no security 
		CREATE_ALWAYS,                // overwrite existing 
		FILE_ATTRIBUTE_NORMAL,      // normal file 
		NULL);                        // no attr. template 
	
	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		_LOG("Could not open file. %s", filepath);  // process error 
		return;
	} 

	DWORD dwWritten;
	WriteFile(hFile, (BYTE*)lpData, nSize, &dwWritten, NULL);

	CloseHandle(hFile);
}


void ReadFromFile(char* filepath, char* lpData, int& nSize)
{
	HANDLE hFile; 
	
	hFile = CreateFile(filepath,           // create MYFILE.TXT 
		GENERIC_READ,                // open for writing 
		FILE_SHARE_READ,                            // do not share 
		NULL,                         // no security 
		OPEN_EXISTING,                // overwrite existing 
		FILE_ATTRIBUTE_NORMAL,      // normal file 
		NULL);                        // no attr. template 
	
	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		_LOG("Could not open file. %s", filepath);  // process error 
		return;
	} 

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	DWORD dwRead;
	ReadFile(hFile, (BYTE*)lpData, dwFileSize, &dwRead, NULL);

	nSize = dwRead;
	CloseHandle(hFile);

}