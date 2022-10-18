#pragma once

#ifndef tstring
	#ifdef _UNICODE
		#define tstring std::wstring
	#else 
		#define tstring std::string
	#endif
#endif

// VS 6.0 ���带 ���� �ڵ�
#ifndef INVALID_FILE_ATTRIBUTES
	#define INVALID_FILE_ATTRIBUTES ((DWORD)-1) 
#endif

#ifndef errno_t
	typedef int errno_t;
#endif

#ifndef _tstoi 
	#ifdef _UNICODE
		#define _tstoi _wtoi
	#else
		#define _tstoi atoi
	#endif
#endif

class Path
{
public:
	//					��� ��ü ��� ��������
	static TCHAR*		GetCurrentFullPath(TCHAR* szPath);
	//					��� ���丮 ��������
	static TCHAR*		GetCurrentDirectory(TCHAR* szPath);
	//					��� ���� �̸� ��������
	static TCHAR*		GetCurrentFileName(TCHAR* szPath);
	static WCHAR*		GetCurrentFileNameW(WCHAR* szPath);

	//					���� ���ڿ� ����� : �� �ڿ� "\\" �ٿ��ش�.
	static TCHAR*		SetDirString(TCHAR * szPath);
	//					���� ���ڿ� ����� : �� �ڿ� "\\" ����
	static TCHAR*		ClearDirString(TCHAR * szPath);
	
	//					���� ���ڿ� ����� : ���ϸ� ������ ���
	static TCHAR*		GetDirectory(TCHAR * szPath);
	static WCHAR*		GetDirectoryW(WCHAR * szPath);

	//					���� ���ڿ� ����� : �н� ������ ���� ��
	static TCHAR*		GetFileName(TCHAR * szPath);
	static WCHAR*		GetFileNameW(WCHAR * szPath);

	//					/ => \\ �� ����
	static TCHAR*		ChangeSlashToBackSlash(TCHAR * szPath);
	//					\\ => / �� ����
	static TCHAR*		ChangeBackSlashToSlash(TCHAR * szPath);

	//					���Ͽ��� ������ �� �о����
	static BOOL			ReadFileLastLine(TCHAR * szFile, TCHAR * szLastLine);
	//					���Ͽ��� ù �� �о����
	static BOOL			ReadFileFirstLine(TCHAR * szFile, TCHAR * szLine, int nTextSize = 0, BOOL bBinaryFile = FALSE);
	//					�� ���Ͽ� ���� ����
	static BOOL			WriteNewFile(TCHAR * szFile, TCHAR * szText, int nTextSize, BOOL bBinaryFile = FALSE);
	//					���� ��� ���丮 ����
	static BOOL			CreateDirectorys(TCHAR * szFilePath, BOOL bIsFile);
	static BOOL			CreateDirectorysW(const std::wstring& szFilePath, BOOL bIsFile);

	//					���丮 ����
	static void			DeleteDirectory(TCHAR* pszDirPath);

	//					���� ������
	static ULONGLONG	GetFileSize(TCHAR * szFile);


	//					���ڿ� ���� : char -> TCHAR
	static tstring		GetTstringA(char * szData);
	//					���ڿ� ���� : WCHAR -> TCHAR
	static tstring		GetTstringW(WCHAR * szData);
	//					���ڿ� ���� : TCHAR -> char
	static std::string	GetAstring(TCHAR * szData);
	//					���ڿ� ���� : TCHAR -> WCHAR
	static std::wstring	GetWstring(TCHAR * szData);


	static tstring		Wstr2Tstr(std::wstring& str);
};

