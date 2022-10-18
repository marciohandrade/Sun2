/*******************************************************************************
*	작 성 자 : 진혜진
*	작 성 일 : 2009.06.10
*	내    용 : Download에 필요한 정보 설정
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

	// 파일 경로 가져오기
	std::wstring	GetFileName();
	std::wstring	GetLocalFilePath();
	std::wstring	GetRemoteFilePath();
	std::wstring	GetTargetDirPath();
	ULONGLONG		GetFileLength();

	// 파일 경로 설정
	void			SetFilePath(std::wstring& fileName, std::wstring& localFilePath, std::wstring& remoteFilePath, std::wstring& targetDirPath);
	void			SetFileLength(ULONGLONG fileLength);

private:
	std::wstring	FileName;			// 파일 이름
	std::wstring	LocalFilePath;		// 로컬 전체 경로
	std::wstring	RemoteFilePath;		// 리모트 전체 경로
	std::wstring	TargetDirPath;		// 패치 풀어낼 폴더 경로	
	ULONGLONG		FileLength;			// 파일 사이즈
};

class DownloadServerInfo
{
public:
	DownloadServerInfo();
	~DownloadServerInfo();
	
	std::wstring	GetServerURL();			// 서버 주소 가져오기
	std::wstring	GetUserID();			// 서버 접속 계정 가져오기
	std::wstring	GetPassword();			// 서버 접속 계정 비번 가져오기
	INTERNET_PORT	GetPort();				// 포트 번호 가져오기
	DownloaderType	GetDownloaderType();	// 타운로드 타입 가져오기
	DWORD			GetReadBufferSize();	// 버퍼 사이즈 가져오기
	DWORD			GetConnectTimeout();	// 커낵트 타임아웃 가져오기
	BOOL			IsOverWrite();			// 덮어쓰기 유무
	BOOL			IsPassive();			// 패시브 모드 유무

	
	void			SetServerInfo(std::wstring& serverURL, INTERNET_PORT port, std::wstring& userID, std::wstring& password);	// 서버 정보 설정
	void			SetDownloaderType(DownloaderType downloaderType);		// 다운로드 타입 설정
	void			SetReadBufferSize(DWORD readBufferSize);				// 버퍼 사이즈 설정
	void			SetOverWrite(BOOL overWrite);							// 로컬 파일 존재 시 덮어쓰기 설정
	void			SetPassiveMode(BOOL passive);							// 패시브 모드 설정
	void			SetConnectTimeout(DWORD connectTimeout);				// 커낵트 타임아웃 설정

private:
	std::wstring	ServerURL;				// Server 주소
	std::wstring	UserID;					// 접속 계정 명
	std::wstring	Password;				// 접속 계정 Password
	INTERNET_PORT	Port;					// 접속 포트 default = INTERNET_DEFAULT_FTP_PORT (21)
	DownloaderType	DownloaderTypeValue;	// 다운로더 타입 - 프로토콜
	DWORD			ReadBufferSize;			// 다운로드 패킷 사이즈 제한 default = 4096
	BOOL			OverWrite;				// Local File 존재할 경우 덮어쓰기 설정 default = TRUE
	BOOL			Passive;				// Passive 설정 default = FALSE
	DWORD			ConnectTimeout;			// 커낵트 타임아웃
};

