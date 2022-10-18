//#ifdef _PAAS_ALERT
//
//#ifndef ALERT_REQUEST_SENDER_H
//#define ALERT_REQUEST_SENDER_H
//
//#include <list>
//#include <eXtreme.h>
//#include <threading/Mutex.h>
//#include <Singleton.h>
//
//#define DEFAULT_APP_NAME "AuthServer"		// Send Alert을 실행하는 응용 프로그램의 이름
//#define MAX_ACCOUNT		20
//#define MAX_IP			16
//#define MAX_PARTITION	10
//#define MAX_URL			128
//#define MAX_URL_PATH	1024
//
//struct AlertInfo
//{
//    char szAccount[MAX_ACCOUNT];
//    char szClientIP[MAX_IP];
//    DWORD dwLoginTickCount;
//    DWORD dwRetryTickCount;
//    SYSTEMTIME LoginTime;
//
//    AlertInfo()
//    {
//        Reset();
//    }
//    void Reset()
//    {
//        memset(this, 0, sizeof(*this));
//    }
//
//};
//
//class CAlertRequestSender : public Singleton<CAlertRequestSender>
//{
//    typedef std::list<AlertInfo*> ALERTLIST;
//
//    ALERTLIST m_Pool;
//    ALERTLIST m_AlertList;
//
//    char m_szPart[MAX_PARTITION];		// 파티션 정보
//    char m_szURL[MAX_URL];				// Alert 리스트를 전송할 URL
//    char m_szURLPath[MAX_URL_PATH];		// URL의 경로
//
//    HINTERNET m_hOpen,					// 인터넷 초기화 핸들
//        m_hConnect,				// 인터넷 HTTP 세션 핸들
//        m_hRequest;				// HTTP 서버에 Request 요청할 핸들
//
//    HANDLE m_eventKill;
//    HANDLE m_hThread;
//
//public:
//    CAlertRequestSender();
//    ~CAlertRequestSender();
//
//    void Create( const char * pDescFileName );
//    bool SendAlert(char* pAccount, char* pClientIP, SYSTEMTIME* pLoginTime, bool bRetry = false);
//
//    static unsigned int __stdcall Runner(void* lParam);
//
//private:
//
//    bool Open();
//    bool Connect();
//    void Clear();
//    bool InsertAlertInfo(char* pAccount, char* pClientIP, SYSTEMTIME* pLoginTime);
//};
//
//
//
//#endif	// ALERT_REQUEST_SENDER_H
//
//#endif // _PAAS_ALERT
//
