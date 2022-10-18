/*******************************************************************************
*	작 성 자 : 진혜진
*	작 성 일 : 2009.06.10
*	내    용 : FileDownloader
*				File 단위 다운로드 기능 제공
*******************************************************************************/

#pragma once

#include <SLM/FileDownloader/IConnecter.h>
#include <SLM/FileDownloader/IDownloaderStateEvent.h>

class FileDownloader
{
public:
	FileDownloader(IDownloaderStateEvent* pStateEvent, DownloadServerInfo* pServerInfo, DownloadFileInfo* pFileInfo);
	~FileDownloader();

	void				Break();		// 다운로드 중지
	WZResult			DownloadFile();	// 지정한 파일 다운로드 실행 : 세션, 커낵트, 파일 오픈 모두 처리

private:
	BOOL				CanBeContinue();	// 진행 여부
	void				Release();			// 릴리즈
	IConnecter*			CreateConnecter();	// 커넥터 생성
	WZResult 			CreateConnection();	// 접속 처리
	WZResult 			Connection();
	static unsigned int __stdcall RunConnectThread(LPVOID pParam);

	
	WZResult 			TransferRemoteFile();	// 전송 처리
	WZResult 			CreateLocalFile();		// 로컬 파일 생성
	WZResult 			ReadRemoteFile(BYTE* byReadBuffer, DWORD* dwBytesRead);		// 다운로드 파일 읽기
	WZResult 			WriteLocalFile(BYTE* byReadBuffer, DWORD dwBytesRead);		// 로컬 파일 쓰기
	void				SendStartedDownloadFileEvent(ULONGLONG nFileLength);		// 다운로드 시작 이벤트 보내기
	void				SendCompletedDownloadFileEvent(WZResult wzResult);			// 다운로드 완료 이벤트 보내기
	void				SendProgressDownloadFileEvent(ULONGLONG nTotalBytesRead);	// 다운로드 진행 상황 이벤트 보내기 : 패킷 단위

	
	volatile BOOL				m_bBreak;		// 다운로드 중지 플래그
	WZResult 					m_Result;		// 결과..
	IDownloaderStateEvent*		m_pStateEvent;	// 다운로드 상태 이벤트 받을 객체
	DownloadServerInfo*			m_pServerInfo;	// 다운로드 서버 정보 객체
	DownloadFileInfo*			m_pFileInfo;	// 다운로드 파일 정보 객체
	IConnecter*					m_pConnecter;	// 커넥터
	HINTERNET					m_hSession;		// WinINet 세션 핸들
	HINTERNET					m_hConnection;	// WinINet 커낵션 핸들
	HINTERNET					m_hRemoteFile;	// 서버 파일 핸들
	HANDLE						m_hLocalFile;	// 로컬 파일 핸들
	ULONGLONG					m_nFileLength;	// 파일 사이즈
};

