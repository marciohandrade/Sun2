/*******************************************************************************
*	�� �� �� : ������
*	�� �� �� : 2009.06.10
*	��    �� : Download�� �ʿ��� ���� ����
*******************************************************************************/

#pragma once
#include <Windows.h>
#include <Wininet.h>
#include <string>
#include <crtdbg.h>
#include <tchar.h>
#include <strsafe.h>

#ifndef DL_DEFAULT_BUFFER_SIZE
	#define DL_DEFAULT_BUFFER_SIZE			4096
#endif

typedef enum _DownloaderType
{
	FTP,
	HTTP,
} DownloaderType;



class DownloadFileInfo
{
public:
	DownloadFileInfo();
	~DownloadFileInfo();

	DownloadFileInfo(std::wstring& fileName, std::wstring& localFilePath, std::wstring& remoteFilePath, std::wstring& targetDirPath);

	// ���� ��� ��������
	std::wstring	GetFileName();
	std::wstring	GetLocalFilePath();
	std::wstring	GetRemoteFilePath();
	std::wstring	GetTargetDirPath();
	ULONGLONG		GetFileLength();

	// ���� ��� ����
	void			SetFilePath(std::wstring& fileName, std::wstring& localFilePath, std::wstring& remoteFilePath, std::wstring& targetDirPath);
	void			SetFileLength(ULONGLONG fileLength);

private:
	std::wstring	FileName;			// ���� �̸�
	std::wstring	LocalFilePath;		// ���� ��ü ���
	std::wstring	RemoteFilePath;		// ����Ʈ ��ü ���
	std::wstring	TargetDirPath;		// ��ġ Ǯ� ���� ���	
	ULONGLONG		FileLength;			// ���� ������
};

class DownloadServerInfo
{
public:
	DownloadServerInfo();
	~DownloadServerInfo();
	
	std::wstring	GetServerURL();			// ���� �ּ� ��������
	std::wstring	GetUserID();			// ���� ���� ���� ��������
	std::wstring	GetPassword();			// ���� ���� ���� ��� ��������
	INTERNET_PORT	GetPort();				// ��Ʈ ��ȣ ��������
	DownloaderType	GetDownloaderType();	// Ÿ��ε� Ÿ�� ��������
	DWORD			GetReadBufferSize();	// ���� ������ ��������
	DWORD			GetConnectTimeout();	// Ŀ��Ʈ Ÿ�Ӿƿ� ��������
	BOOL			IsOverWrite();			// ����� ����
	BOOL			IsPassive();			// �нú� ��� ����

	
	void			SetServerInfo(std::wstring& serverURL, INTERNET_PORT port, std::wstring& userID, std::wstring& password);	// ���� ���� ����
	void			SetDownloaderType(DownloaderType downloaderType);		// �ٿ�ε� Ÿ�� ����
	void			SetReadBufferSize(DWORD readBufferSize);				// ���� ������ ����
	void			SetOverWrite(BOOL overWrite);							// ���� ���� ���� �� ����� ����
	void			SetPassiveMode(BOOL passive);							// �нú� ��� ����
	void			SetConnectTimeout(DWORD connectTimeout);				// Ŀ��Ʈ Ÿ�Ӿƿ� ����

private:
	std::wstring	ServerURL;				// Server �ּ�
	std::wstring	UserID;					// ���� ���� ��
	std::wstring	Password;				// ���� ���� Password
	INTERNET_PORT	Port;					// ���� ��Ʈ default = INTERNET_DEFAULT_FTP_PORT (21)
	DownloaderType	DownloaderTypeValue;	// �ٿ�δ� Ÿ�� - ��������
	DWORD			ReadBufferSize;			// �ٿ�ε� ��Ŷ ������ ���� default = 4096
	BOOL			OverWrite;				// Local File ������ ��� ����� ���� default = TRUE
	BOOL			Passive;				// Passive ���� default = FALSE
	DWORD			ConnectTimeout;			// Ŀ��Ʈ Ÿ�Ӿƿ�
};

