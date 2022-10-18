/*******************************************************************************
*	�� �� �� : ������
*	�� �� �� : 2009.07.07
*	��    �� : FTP Connecter
*******************************************************************************/

#pragma once
#include <SLM/FileDownloader/IConnecter.h>

class FTPConnecter : public IConnecter
{
public:
	FTPConnecter(DownloadServerInfo* pServerInfo, DownloadFileInfo* pFileInfo);
	~FTPConnecter();
	
	virtual WZResult CreateSession(HINTERNET& hSession);								// ����
	virtual WZResult CreateConnection(HINTERNET& hSession, HINTERNET& hConnection);		// Ŀ��Ʈ
	virtual WZResult OpenRemoteFile(HINTERNET& hConnection, HINTERNET& hRemoteFile, ULONGLONG& nFileLength);	// �ٿ�ε� ���� ���� & ������ ��������
	virtual WZResult ReadRemoteFile(HINTERNET& hRemoteFile, BYTE* byReadBuffer, DWORD* dwBytesRead);			// ����Ʈ ���� �б�
};

