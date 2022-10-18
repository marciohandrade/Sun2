/*******************************************************************************
*	�� �� �� : ������
*	�� �� �� : 2009.06.10
*	��    �� : Interface - Download State Event ���� Ŭ����
*				�ٿ�ε� ���¸� ���� ���� ��ü
*				���̺귯���� ����� ���α׷����� �������Ͽ� ���
*******************************************************************************/

#pragma once

class IDownloaderStateEvent
{
public:
	IDownloaderStateEvent(){};
	virtual ~IDownloaderStateEvent(){};


	virtual void	OnStartedDownloadFile(TCHAR* szFileName, ULONGLONG uFileLength) = 0;			// �ٿ�ε� ���� �̺�Ʈ �ڵ鷯
	virtual void	OnProgressDownloadFile(TCHAR* szFileName, ULONGLONG uDownloadFileLength) = 0;	// �ٿ�ε� ���� ��Ȳ �̺�Ʈ �ڵ鷯 : ��Ŷ ����
	virtual void	OnCompletedDownloadFile(WZResult wzResult) = 0;									// �ٿ�ε� ���� �̺�Ʈ �ڵ鷯
};

