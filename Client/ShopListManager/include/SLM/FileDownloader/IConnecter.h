/*******************************************************************************
*	작 성 자 : 진혜진
*	작 성 일 : 2009.07.07
*	내    용 : Connecter Interface
*******************************************************************************/

#pragma once

#include <SLM/FileDownloader/WZResult.h>
#include <SLM/FileDownloader/DownloadInfo.h>

class IConnecter
{
public:
	IConnecter(DownloadServerInfo*	pServerInfo, DownloadFileInfo *	pFileInfo)
			   : m_pServerInfo(pServerInfo), m_pFileInfo(pFileInfo)
	{};
	~IConnecter(){};

	virtual WZResult CreateSession(HINTERNET& hSession) = 0;		// 세션
	virtual WZResult CreateConnection(HINTERNET& hSession, HINTERNET& hConnection) = 0;	// 커낵트
	virtual WZResult OpenRemoteFile(HINTERNET& hConnection, HINTERNET& hRemoteFile, ULONGLONG& nFileLength) = 0;	// 다운로드 파일 오픈 & 사이즈 가져오기
	virtual WZResult ReadRemoteFile(HINTERNET& hRemoteFile, BYTE* byReadBuffer, DWORD* dwBytesRead) = 0;			// 리모트 파일 읽기

protected:
	
	WZResult 				m_Result;		// 결과..
	DownloadServerInfo*	m_pServerInfo;	// 다운로드 서버 정보 객체
	DownloadFileInfo*		m_pFileInfo;	// 다운로드 파일 정보 객체
};

