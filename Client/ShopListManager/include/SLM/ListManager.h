
/**************************************************************************************************

�ۼ���: 2009.10.06
�ۼ���: ������

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

	// �⺻ ���� ���� - Version 3 (Old ����) - TCHAR
	void SetListManagerInfo (DownloaderType type, TCHAR* ServerIP, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, TCHAR* Delimiter = NULL);
	void SetListManagerInfo (DownloaderType type, TCHAR* ServerIP, unsigned short PortNum, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, TCHAR* Delimiter = NULL);

	// �⺻ ���� ���� - Version 6 (New ����) - TCHAR
	void SetListManagerInfoVer6 (DownloaderType type, TCHAR* ServerIP, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, TCHAR* Delimiter = NULL);
	void SetListManagerInfoVer6 (DownloaderType type, TCHAR* ServerIP, unsigned short PortNum, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, TCHAR* Delimiter = NULL);

	/* ~W() �޼ҵ�
		���� Ŭ���̾�Ʈ MBCS�̰� ��ũ��Ʈ ���� UTF-8 ������ ���
		��Ʈ�� �� ���ַ� �� ������ �ʵ��� �����ϱ� ���� �� VO Ŭ������ WCHAR ����� ���ڿ��� ����
		���ڿ� ���ڵ� ��� WCHAR�� ��������� ����ϵ��� �Ѵ�.
	*/

	// �⺻ ���� ���� - Version 3 (Old ����) - WCHAR
	void SetListManagerInfoW(DownloaderType type, WCHAR* ServerIP, WCHAR* UserID, WCHAR* Pwd, WCHAR* RemotePath, WCHAR* LocalPath, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, WCHAR* Delimiter = NULL);
	void SetListManagerInfoW(DownloaderType type, WCHAR* ServerIP, unsigned short PortNum, WCHAR* UserID, WCHAR* Pwd, WCHAR* RemotePath, WCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, WCHAR* Delimiter = NULL);

	// �⺻ ���� ���� - Version 6 (New ����) - WCHAR
	void SetListManagerInfoVer6W(DownloaderType type, WCHAR* ServerIP, WCHAR* UserID, WCHAR* Pwd, WCHAR* RemotePath, WCHAR* LocalPath, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, WCHAR* Delimiter = NULL);
	void SetListManagerInfoVer6W(DownloaderType type, WCHAR* ServerIP, unsigned short PortNum, WCHAR* UserID, WCHAR* Pwd, WCHAR* RemotePath, WCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime = 0, WCHAR* Delimiter = NULL);

	WZResult		LoadScriptList(bool bDonwLoad);

private:
	// �⺻ ���� ���� - TCHAR
	void SetListManagerInfo(DownloaderType type, TCHAR* ServerIP, unsigned short PortNum, TCHAR* UserID, TCHAR* Pwd, TCHAR* RemotePath, TCHAR* LocalPath, bool isFTPPassiveMode, CListVersionInfo& Version, DWORD dwDownloadMaxTime, TCHAR* Delimiter, int shopListManagerVersion, bool isReturnUnicode);
	// �⺻ ���� ���� - WCHAR
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

	virtual	void		RegistScriptFiles() = 0;			// �ٿ�ε��� ��ũ��Ʈ ���� ���
	virtual WZResult	LoadScript(bool bDonwLoad) = 0;

	CListManagerInfo			m_ListManagerInfo;
	std::vector<tstring>		m_vScriptFiles;
	std::vector<std::wstring>	m_vScriptFilesW;
	WZResult					m_Result;

	std::auto_ptr<CFileDownLoaderHelper>	FileDownLoaderHelper;	
	std::auto_ptr<CUtilSLM> 				UtilSLM;
};
