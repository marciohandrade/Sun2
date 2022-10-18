#ifndef WEB_REQUEST_H
#define WEB_REQUEST_H

// 06년 12월 김종호

#include <wininet.h>

typedef struct sURL
{
	char szURL[MAX_URL];				// 감시할 URL
	char szFileName[MAX_FILENAME];		// 실행시킬 파일이름
    INTERNET_PORT wPort;				// 접속할 URL 포트

    sURL()
	{
		memset(this, 0, sizeof(*this) );
		wPort = 80;						// 기본 인터넷 포트
	}

}URL_INFO, *LPURL_INFO;

class CWebRequest
{
	HINTERNET m_hOpen,			// 인터넷 초기화 핸들
			  m_hConnect,		// 인터넷 HTTP 세션 핸들
              m_hRequest;		// HTTP 서버에 Request 요청할 핸들

	URL_INFO m_URLInfo;			// 감시할 URL 정보 구조체
	DWORD m_dwStateCode;		// Request 상태코드
	DWORD m_dwURLIndex;			// 감시할 URL의 고유 Index

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