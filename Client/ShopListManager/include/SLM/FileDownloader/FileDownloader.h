/*******************************************************************************
*	�� �� �� : ������
*	�� �� �� : 2009.06.10
*	��    �� : FileDownloader
*				File ���� �ٿ�ε� ��� ����
*******************************************************************************/

#pragma once

#include <SLM/FileDownloader/IConnecter.h>
#include <SLM/FileDownloader/IDownloaderStateEvent.h>

class FileDownloader
{
public:
	FileDownloader(IDownloaderStateEvent* pStateEvent, DownloadServerInfo* pServerInfo, DownloadFileInfo* pFileInfo);
	~FileDownloader();

	void				Break();		// �ٿ�ε� ����
	WZResult			DownloadFile();	// ������ ���� �ٿ�ε� ���� : ����, Ŀ��Ʈ, ���� ���� ��� ó��

private:
	BOOL				CanBeContinue();	// ���� ����
	void				Release();			// ������
	IConnecter*			CreateConnecter();	// Ŀ���� ����
	WZResult 			CreateConnection();	// ���� ó��
	WZResult 			Connection();
	static unsigned int __stdcall RunConnectThread(LPVOID pParam);

	
	WZResult 			TransferRemoteFile();	// ���� ó��
	WZResult 			CreateLocalFile();		// ���� ���� ����
	WZResult 			ReadRemoteFile(BYTE* byReadBuffer, DWORD* dwBytesRead);		// �ٿ�ε� ���� �б�
	WZResult 			WriteLocalFile(BYTE* byReadBuffer, DWORD dwBytesRead);		// ���� ���� ����
	void				SendStartedDownloadFileEvent(ULONGLONG nFileLength);		// �ٿ�ε� ���� �̺�Ʈ ������
	void				SendCompletedDownloadFileEvent(WZResult wzResult);			// �ٿ�ε� �Ϸ� �̺�Ʈ ������
	void				SendProgressDownloadFileEvent(ULONGLONG nTotalBytesRead);	// �ٿ�ε� ���� ��Ȳ �̺�Ʈ ������ : ��Ŷ ����

	
	volatile BOOL				m_bBreak;		// �ٿ�ε� ���� �÷���
	WZResult 					m_Result;		// ���..
	IDownloaderStateEvent*		m_pStateEvent;	// �ٿ�ε� ���� �̺�Ʈ ���� ��ü
	DownloadServerInfo*			m_pServerInfo;	// �ٿ�ε� ���� ���� ��ü
	DownloadFileInfo*			m_pFileInfo;	// �ٿ�ε� ���� ���� ��ü
	IConnecter*					m_pConnecter;	// Ŀ����
	HINTERNET					m_hSession;		// WinINet ���� �ڵ�
	HINTERNET					m_hConnection;	// WinINet Ŀ���� �ڵ�
	HINTERNET					m_hRemoteFile;	// ���� ���� �ڵ�
	HANDLE						m_hLocalFile;	// ���� ���� �ڵ�
	ULONGLONG					m_nFileLength;	// ���� ������
};

