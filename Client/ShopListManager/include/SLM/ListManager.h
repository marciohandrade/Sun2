
/**************************************************************************************************

작성일: 2009.10.06
작성자: 진혜진

**************************************************************************************************/

#pragma once
#include <SLM/CommonSLM.h>
#include <SLM/UtilSLM.h>

class CFileDownLoaderHelper;
class CListVersionInfo;

class CListManager  
{
public:
	CListManager();	
	virtual ~CListManager();

	// 기본 정보 설정 - Version 3 (Old 버전) - TCHAR
	void SetListManagerInfo (DownloaderType type, TCHAR* ServerIP, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, TCHAR* Delimiter = NULL);
	void SetListManagerInfo (DownloaderType type, TCHAR* ServerIP, unsigned short PortNum, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, TCHAR* Delimiter = NULL);

	// 기본 정보 설정 - Version 6 (New 버전) - TCHAR
	void SetListManagerInfoVer6 (DownloaderType type, TCHAR* ServerIP, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, TCHAR* Delimiter = NULL);
	void SetListManagerInfoVer6 (DownloaderType type, TCHAR* ServerIP, unsigned short PortNum, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, TCHAR* Delimiter = NULL);

	/* ~W() 메소드
		게임 클라이언트 MBCS이고 스크립트 파일 UTF-8 형식인 경우
		베트남 등 비주류 언어가 깨지지 않도록 지원하기 위해 각 VO 클래스의 WCHAR 멤버에 문자열을 세팅
		문자열 인자도 모두 WCHAR를 명시적으로 사용하도록 한다.
	*/

	// 기본 정보 설정 - Version 3 (Old 버전) - WCHAR
	void SetListManagerInfoW(DownloaderType type, WCHAR* ServerIP, WCHAR* UserID, WCHAR* Pwd, WCHAR* RemotePath, WCHAR* LocalPath, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, WCHAR* Delimiter = NULL);
	void SetListManagerInfoW(DownloaderType type, WCHAR* ServerIP, unsigned short PortNum, WCHAR* UserID, WCHAR* Pwd, WCHAR* RemotePath, WCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, WCHAR* Delimiter = NULL);

	// 기본 정보 설정 - Version 6 (New 버전) - WCHAR
	void SetListManagerInfoVer6W(DownloaderType type, WCHAR* ServerIP, WCHAR* UserID, WCHAR* Pwd, WCHAR* RemotePath, WCHAR* LocalPath, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, WCHAR* Delimiter = NULL);
	void SetListManagerInfoVer6W(DownloaderType type, WCHAR* ServerIP, unsigned short PortNum, WCHAR* UserID, WCHAR* Pwd, WCHAR* RemotePath, WCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, WCHAR* Delimiter = NULL);

	WZResult		LoadScriptList(bool bDonwLoad);

private:
	// 기본 정보 설정 - TCHAR
	void SetListManagerInfo(DownloaderType type, TCHAR* ServerIP, unsigned short PortNum, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime, TCHAR* Delimiter, int shopListManagerVersion, bool isReturnUnicode);
	// 기본 정보 설정 - WCHAR
	void SetListManagerInfoW(DownloaderType type, WCHAR* ServerIP, unsigned short PortNum, WCHAR* UserID, WCHAR* Pwd, WCHAR* RemotePath, WCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime, WCHAR* Delimiter, int shopListManagerVersion, bool isReturnUnicode);

	WZResult		LoadScriptListT(bool bDonwLoad);
	WZResult		LoadScriptListW(bool bDonwLoad);

protected:
	bool			IsScriptFileExist();
	bool			IsScriptFileExistW();

	tstring			GetScriptPath();
	std::wstring	GetScriptPathW();

	void			DeleteScriptFiles();
	void			DeleteScriptFilesW();

	WZResult		FileDownLoad();
	WZResult		FileDownLoadImpl();


	static unsigned int __stdcall RunFileDownLoadThread(LPVOID pParam);

	virtual	void		RegistScriptFiles() = 0;			// 다운로드할 스크립트 파일 등록
	virtual WZResult	LoadScript(bool bDonwLoad) = 0;

	CListManagerInfo			m_ListManagerInfo;
	std::vector<tstring>		m_vScriptFiles;
	std::vector<std::wstring>	m_vScriptFilesW;
	WZResult					m_Result;

	std::auto_ptr<CFileDownLoaderHelper>	FileDownLoaderHelper;	
	std::auto_ptr<CUtilSLM> 				UtilSLM;
};
