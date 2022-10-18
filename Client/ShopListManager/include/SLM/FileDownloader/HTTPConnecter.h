/*******************************************************************************
*	�� �� �� : ������
*	�� �� �� : 2009.07.07
*	��    �� : HTTP Connecter
*******************************************************************************/

#pragma once

#include <SLM/FileDownloader/IConnecter.h>

class HTTPConnecter : public IConnecter
{
public:
	HTTPConnecter(DownloadServerInfo* pServerInfo, DownloadFileInfo* pFileInfo);
	~HTTPConnecter();

	virtual WZResult CreateSession(HINTERNET& hSession);	// ����
	virtual WZResult CreateConnection(HINTERNET& hSession, HINTERNET& hConnection); // Ŀ��Ʈ
	virtual WZResult OpenRemoteFile(HINTERNET& hConnection, HINTERNET& hRemoteFile, ULONGLONG& nFileLength);	// �ٿ�ε� ���� ���� & ������ ��������
	virtual WZResult ReadRemoteFile(HINTERNET& hRemoteFile, BYTE* byReadBuffer, DWORD* dwBytesRead);	// ����Ʈ ���� �б�
};

