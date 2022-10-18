#pragma once

#define SHOPLIST_LENGTH_CATEGORYNAME		128

#define SHOPLIST_LENGTH_PACKAGENAME			512
#define SHOPLIST_LENGTH_PACKAGEDESC			2048
#define SHOPLIST_LENGTH_PACKAGECAUTION		1024
#define SHOPLIST_LENGTH_PACKAGECASHNAME		256
#define SHOPLIST_LENGTH_PACKAGEPRICEUNIT	64

#define SHOPLIST_LENGTH_PRODUCTNAME			128
#define SHOPLIST_LENGTH_PRODUCTPROPERTYNAME 128
#define SHOPLIST_LENGTH_PRODUCTVALUE		512
#define SHOPLIST_LENGTH_PRODUCTUNITNAME		64
#define SHOPLIST_LENGTH_INGAMEPACKAGEID		20
#define SHOPLIST_LENGTH_PRODUCTDESCRIPTION	2000

#define SHOPLIST_LENGTH_PROPERTYNAME		128
#define SHOPLIST_LENGTH_PROPERTYVALUE		255
#define SHOPLIST_LENGTH_PROPERTYUNITNAME	64

#define BANNER_LENGTH_NAME					50

#define ERROR_TIMEOUT_BREAK					0x01
#define ERROR_FILE_SIZE_ZERO				0x02
#define ERROR_PACKAGEMAP_OPEN_FAIL			0x03
#define ERROR_PRODUCTMAP_OPEN_FAIL			0x04
#define ERROR_CATEGORY_OPEN_FAIL			0x05
#define ERROR_PACKAGE_OPEN_FAIL				0x06
#define ERROR_PRODUCT_OPEN_FAIL				0x07
#define ERROR_PROPERTY_OPEN_FAIL			0x08
#define ERROR_BANNER_OPEN_FAIL				0x09
#define ERROR_LOAD_SCRIPT					0x0A
#define ERROR_THREAD						0x0B

#include <Windows.h>
#include <Wininet.h>

#pragma warning( push )
#pragma warning( disable : 4786 )
#include <vector>
#include <map>
#include <string>
#pragma warning( pop )

#include <tchar.h>
#include <fstream>
#include <atlbase.h> 
#include <atlconv.h>
#include <strsafe.h>
#include <time.h>
#include <process.h>
#include <stdio.h>

#include <SLM/FileDownloader/WZResult.h>
#include <SLM/FileDownloader/DownloadInfo.h>
#include <SLM/FileDownloader/FileDownloader.h>



#ifdef _UNICODE
	typedef std::wstring	tstring;
	typedef std::wifstream	tifstream;
#else
	typedef std::string		tstring;
	typedef std::ifstream	tifstream;
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
//


class CListVersionInfo
{
public:
	unsigned short Zone;
	unsigned short year;
	unsigned short yearId;
};

class CListManagerInfo
{
public:
	DownloaderType		m_DownloaderType;		// 다운로드 타입 FTP or HTTP
	tstring				m_strServerIP;			// 서버 주소
	std::wstring		m_strServerIPW;			// 서버 주소
	unsigned short		m_nPortNum;				// 서버 포트
	tstring				m_strUserID;			// 계정(아이디)
	std::wstring		m_strUserIDW;			// 계정(아이디)
	tstring				m_strPWD;				// 계정(비밀번호)
	std::wstring		m_strPWDW;				// 계정(비밀번호)
	tstring				m_strRemotePath;		// 경로 (폴더명만)
	std::wstring		m_strRemotePathW;		// 경로 (폴더명만)
	bool				IsFTPPassiveMode;		// FTP 모드
	tstring				m_strLocalPath;			// 파일 다운로드 할 경로 (폴더명만)
	std::wstring		m_strLocalPathW;		// 파일 다운로드 할 경로 (폴더명만)
	DWORD				m_dwDownloadMaxTime;	// 다운로드 받는 시간 제한
	tstring				Delimiter;				// 파일을 읽어들일때 사용할 구분자	
	std::wstring		DelimiterW;				// 파일을 읽어들일때 사용할 구분자	
	int					ShopListManagerVersion;	// 구버전(샵스크립트 3개 사용)과 신버전(샵스크립트 5개 사용)의 구분 용(3 또는 5의 값을 갖음)
	bool				IsReturnUnicode;		// true : 스크립트파일의 문자를 WCHAR 멤버변수에 세팅, false : TCHAR 멤버변수에 세팅
												// 다국어 지원을 위해 스크립트 파일이 UTF-8 BOM인데 게임 클라이언트가 MBCS인 경우를 위한 옵션

	CListVersionInfo	m_Version;
};

