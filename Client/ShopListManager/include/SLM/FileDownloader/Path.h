#pragma once

#ifndef tstring
	#ifdef _UNICODE
		#define tstring std::wstring
	#else 
		#define tstring std::string
	#endif
#endif

// VS 6.0 빌드를 위한 코드
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
	//					모듈 전체 경로 가져오기
	static TCHAR*		GetCurrentFullPath(TCHAR* szPath);
	//					모듈 디렉토리 가져오기
	static TCHAR*		GetCurrentDirectory(TCHAR* szPath);
	//					모듈 파일 이름 가져오기
	static TCHAR*		GetCurrentFileName(TCHAR* szPath);
	static WCHAR*		GetCurrentFileNameW(WCHAR* szPath);

	//					폴더 문자열 만들기 : 맨 뒤에 "\\" 붙여준다.
	static TCHAR*		SetDirString(TCHAR * szPath);
	//					폴더 문자열 만들기 : 맨 뒤에 "\\" 제거
	static TCHAR*		ClearDirString(TCHAR * szPath);
	
	//					폴더 문자열 만들기 : 파일명 제거한 경로
	static TCHAR*		GetDirectory(TCHAR * szPath);
	static WCHAR*		GetDirectoryW(WCHAR * szPath);

	//					파일 문자열 만들기 : 패스 제거한 파일 명
	static TCHAR*		GetFileName(TCHAR * szPath);
	static WCHAR*		GetFileNameW(WCHAR * szPath);

	//					/ => \\ 로 변경
	static TCHAR*		ChangeSlashToBackSlash(TCHAR * szPath);
	//					\\ => / 로 변경
	static TCHAR*		ChangeBackSlashToSlash(TCHAR * szPath);

	//					파일에서 마지막 줄 읽어오기
	static BOOL			ReadFileLastLine(TCHAR * szFile, TCHAR * szLastLine);
	//					파일에서 첫 줄 읽어오기
	static BOOL			ReadFileFirstLine(TCHAR * szFile, TCHAR * szLine, int nTextSize = 0, BOOL bBinaryFile = FALSE);
	//					새 파일에 한줄 쓰기
	static BOOL			WriteNewFile(TCHAR * szFile, TCHAR * szText, int nTextSize, BOOL bBinaryFile = FALSE);
	//					파일 경로 디렉토리 생성
	static BOOL			CreateDirectorys(TCHAR * szFilePath, BOOL bIsFile);
	static BOOL			CreateDirectorysW(const std::wstring& szFilePath, BOOL bIsFile);

	//					디렉토리 삭제
	static void			DeleteDirectory(TCHAR* pszDirPath);

	//					파일 사이즈
	static ULONGLONG	GetFileSize(TCHAR * szFile);


	//					문자열 변경 : char -> TCHAR
	static tstring		GetTstringA(char * szData);
	//					문자열 변경 : WCHAR -> TCHAR
	static tstring		GetTstringW(WCHAR * szData);
	//					문자열 변경 : TCHAR -> char
	static std::string	GetAstring(TCHAR * szData);
	//					문자열 변경 : TCHAR -> WCHAR
	static std::wstring	GetWstring(TCHAR * szData);


	static tstring		Wstr2Tstr(std::wstring& str);
};

