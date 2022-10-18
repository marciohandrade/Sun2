#ifndef WEB_REQUEST_H
#define WEB_REQUEST_H

// 06�� 12�� ����ȣ

#include <wininet.h>

typedef struct sURL
{
	char szURL[MAX_URL];				// ������ URL
	char szFileName[MAX_FILENAME];		// �����ų �����̸�
    INTERNET_PORT wPort;				// ������ URL ��Ʈ

    sURL()
	{
		memset(this, 0, sizeof(*this) );
		wPort = 80;						// �⺻ ���ͳ� ��Ʈ
	}

}URL_INFO, *LPURL_INFO;

class CWebRequest
{
	HINTERNET m_hOpen,			// ���ͳ� �ʱ�ȭ �ڵ�
			  m_hConnect,		// ���ͳ� HTTP ���� �ڵ�
              m_hRequest;		// HTTP ������ Request ��û�� �ڵ�

	URL_INFO m_URLInfo;			// ������ URL ���� ����ü
	DWORD m_dwStateCode;		// Request �����ڵ�
	DWORD m_dwURLIndex;			// ������ URL�� ���� Index

public:

	CWebRequest() : m_hOpen(NULL), m_hConnect(NULL), m_hRequest(NULL), m_dwStateCode(0), m_dwURLIndex(0) {}
	~CWebRequest() { Clear(); }

	DWORD GetStateCode() { return m_dwStateCode; }
	DWORD GetIndex() { return m_dwURLIndex; }

	bool Init(DWORD dwIndex, char* pURL, INTERNET_PORT wPort = INTERNET_DEFAULT_HTTP_PORT);
	bool Open();
	bool Connect();
    bool RequestGet();
	void Clear();
}; 

#endif