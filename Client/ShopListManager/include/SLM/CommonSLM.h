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
	DownloaderType		m_DownloaderType;		// �ٿ�ε� Ÿ�� FTP or HTTP
	tstring				m_strServerIP;			// ���� �ּ�
	std::wstring		m_strServerIPW;			// ���� �ּ�
	unsigned short		m_nPortNum;				// ���� ��Ʈ
	tstring				m_strUserID;			// ����(���̵�)
	std::wstring		m_strUserIDW;			// ����(���̵�)
	tstring				m_strPWD;				// ����(��й�ȣ)
	std::wstring		m_strPWDW;				// ����(��й�ȣ)
	tstring				m_strRemotePath;		// ��� (������)
	std::wstring		m_strRemotePathW;		// ��� (������)
	bool				IsFTPPassiveMode;		// FTP ���
	tstring				m_strLocalPath;			// ���� �ٿ�ε� �� ��� (������)
	std::wstring		m_strLocalPathW;		// ���� �ٿ�ε� �� ��� (������)
	DWORD				m_dwDownloadMaxTime;	// �ٿ�ε� �޴� �ð� ����
	tstring				Delimiter;				// ������ �о���϶� ����� ������	
	std::wstring		DelimiterW;				// ������ �о���϶� ����� ������	
	int					ShopListManagerVersion;	// ������(����ũ��Ʈ 3�� ���)�� �Ź���(����ũ��Ʈ 5�� ���)�� ���� ��(3 �Ǵ� 5�� ���� ����)
	bool				IsReturnUnicode;		// true : ��ũ��Ʈ������ ���ڸ� WCHAR ��������� ����, false : TCHAR ��������� ����
												// �ٱ��� ������ ���� ��ũ��Ʈ ������ UTF-8 BOM�ε� ���� Ŭ���̾�Ʈ�� MBCS�� ��츦 ���� �ɼ�

	CListVersionInfo	m_Version;
};

