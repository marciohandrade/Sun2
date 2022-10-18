#pragma once

#include <stdio.h>
#include <vector>
#include <Windows.h>



#define MAX_STRING_LENGTH 2048

class StringCheck
{
public:
	StringCheck(void);
	~StringCheck(void);
	BOOL	ExtractString(FILE * pFile, TCHAR * pszDirectory);

protected:
	BOOL	Load(FILE * Output , TCHAR * pszFileName);
	BOOL	IsHangul(TCHAR ch);
	BOOL	ReadLine();
private:
	FILE *	m_pFile;
	FILE *	m_pWirteFile;
	TCHAR	szString[MAX_STRING_LENGTH];
};
