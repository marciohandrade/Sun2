#pragma once
#include <SLM/CommonSLM.h>


class CListVersionInfo;

class CFileDownLoaderHelper
{
public:
	CFileDownLoaderHelper();
	virtual ~CFileDownLoaderHelper();

public:
	WZResult DownLoadFiles(DownloaderType				type, 
						   tstring&						strServerIP, 
						   unsigned short				PortNum, 
						   tstring&						strUserName, 
						   tstring&						strPWD, 
						   tstring&						strRemotepath, 
						   tstring&						strlocalpath, 
						   bool							bPassiveMode, 
						   CListVersionInfo&			Version, 
						   std::vector<tstring>&		vScriptFiles);

	WZResult DownLoadFilesW(DownloaderType				type, 
							std::wstring&				strServerIP, 
							unsigned short				PortNum, 
							std::wstring&				strUserName, 
							std::wstring&				strPWD, 
							std::wstring&				strRemotepath, 
							std::wstring&				strlocalpath, 
							bool						bPassiveMode, 
							CListVersionInfo&			Version, 
							std::vector<std::wstring>&	vScriptFiles);

	void	Break();

private:

	std::wstring T2Wstr(tstring& str);

	BOOL CreateFolder(tstring strFilePath);
	BOOL CreateFolderW(std::wstring strFilePath);
	BOOL m_Break;
	std::auto_ptr<FileDownloader>	Downloader;
};
