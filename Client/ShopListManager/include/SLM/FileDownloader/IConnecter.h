/*******************************************************************************
*	�� �� �� : ������
*	�� �� �� : 2009.07.07
*	��    �� : Connecter Interface
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

	virtual WZResult CreateSession(HINTERNET& hSession) = 0;		// ����
	virtual WZResult CreateConnection(HINTERNET& hSession, HINTERNET& hConnection) = 0;	// Ŀ��Ʈ
	virtual WZResult OpenRemoteFile(HINTERNET& hConnection, HINTERNET& hRemoteFile, ULONGLONG& nFileLength) = 0;	// �ٿ�ε� ���� ���� & ������ ��������
	virtual WZResult ReadRemoteFile(HINTERNET& hRemoteFile, BYTE* byReadBuffer, DWORD* dwBytesRead) = 0;			// ����Ʈ ���� �б�

protected:
	
	WZResult 				m_Result;		// ���..
	DownloadServerInfo*	m_pServerInfo;	// �ٿ�ε� ���� ���� ��ü
	DownloadFileInfo*		m_pFileInfo;	// �ٿ�ε� ���� ���� ��ü
};

