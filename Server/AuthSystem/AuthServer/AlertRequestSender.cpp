#include "stdafx.h"

#include "AlertRequestSender.h"

//#ifdef _PAAS_ALERT
//
//CAlertRequestSender::CAlertRequestSender() : m_hOpen(NULL), m_hConnect(NULL), m_hRequest(NULL),
//m_eventKill(INVALID_HANDLE_VALUE), m_hThread(INVALID_HANDLE_VALUE)
//{
//}
//
//CAlertRequestSender::~CAlertRequestSender()
//{
//    ::SetEvent(m_eventKill);
//    ::WaitForSingleObject(m_hThread, INFINITE);
//
//    CloseHandle(m_eventKill);
//    CloseHandle(m_hThread);
//    m_eventKill = m_hThread = INVALID_HANDLE_VALUE;
//
//    if(!m_Pool.empty())
//    {
//        ALERTLIST::iterator it = m_Pool.begin(),
//                            end = m_Pool.end();
//        for( ; it != end; ++it)
//        {
//            AlertInfo* pAlertInfo = *it;
//            if(pAlertInfo)
//                delete pAlertInfo;
//        }
//        m_Pool.clear();
//    }
//
//    if(!m_AlertList.empty())
//    {
//        ALERTLIST::iterator it = m_Pool.begin(),
//                            end = m_Pool.end();
//        for( ; it != end; ++it)
//        {
//            AlertInfo* pAlertInfo = *it;
//            if(pAlertInfo)
//                delete pAlertInfo;
//            m_AlertList.clear();
//        }
//
//    }
//
//    Clear();
//    SUNLOG(eCRITICAL_LOG, "[CAlertRequestSender::~CAlertRequestSender] Return WaitForSingleObject\n");
//}
//
//
//bool CAlertRequestSender::Open()
//{
//    m_hOpen = InternetOpen(DEFAULT_APP_NAME, INTERNET_OPEN_TYPE_PRECONFIG, "", "", 0);
//    if(m_hOpen == NULL)
//    {
//        Clear();
//        return false;
//    }
//
//    return true;
//}
//
//bool CAlertRequestSender::Connect()
//{
//    m_hConnect = InternetConnect(m_hOpen, m_szURL, INTERNET_DEFAULT_HTTP_PORT,
//                                 "", "", INTERNET_SERVICE_HTTP, INTERNET_FLAG_NO_CACHE_WRITE, 0);
//    if(m_hConnect == NULL)
//    {
//        Clear();
//        return false;
//    }
//
//    return true;
//}
//
//void CAlertRequestSender::Clear()
//{
//    if(m_hRequest)
//    {
//        InternetCloseHandle(m_hRequest);
//        m_hRequest = NULL;
//    }
//
//    if(m_hConnect)
//    {
//        InternetCloseHandle(m_hConnect);
//        m_hConnect = NULL;
//    }
//
//    if(m_hOpen)
//    {
//        InternetCloseHandle(m_hOpen);
//        m_hOpen = NULL;
//    }
//}
//
//bool CAlertRequestSender::InsertAlertInfo(char* pAccount, char* pClientIP, SYSTEMTIME* pLoginTime)
//{
//    AlertInfo* pAlertInfo = NULL;
//    ALERTLIST::iterator iter = m_Pool.begin();
//
//    if(iter == m_Pool.end())
//    {
//        pAlertInfo = new AlertInfo;
//    }
//    else
//    {
//        pAlertInfo = *iter;
//        m_Pool.erase(iter);
//    }
//
//    pAlertInfo->Reset();		// 초기화
//
//    memcpy(&pAlertInfo->szAccount[0], pAccount, MAX_ACCOUNT);
//    memcpy(&pAlertInfo->szClientIP[0], pClientIP, MAX_IP);
//    pAlertInfo->dwLoginTickCount = ::GetTickCount();
//    memcpy(&pAlertInfo->LoginTime, &pLoginTime, sizeof(SYSTEMTIME));
//
//    m_AlertList.push_back(pAlertInfo);
//
//    return true;
//}
//
//void CAlertRequestSender::Create(const char * pDescFileName)
//{
//    memset(&m_szPart[0], 0, MAX_PARTITION);
//    memset(&m_szURL[0], 0, MAX_URL);
//    memset(&m_szURLPath[0], 0, MAX_URL_PATH);
//
//    ::GetPrivateProfileString("ALERT_URL", "PART", NULL, m_szPart, MAX_PARTITION, pDescFileName);
//    ::GetPrivateProfileString("ALERT_URL", "URL", NULL, m_szURL, MAX_URL, pDescFileName);
//    ::GetPrivateProfileString("ALERT_URL", "PATH", NULL, m_szURLPath, MAX_PATH, pDescFileName);
//
//    if((NULL != m_szURL) && (NULL != m_szURLPath) && (NULL != m_szPart))
//    {
//        m_eventKill = ::CreateEvent(NULL, false, false, NULL);
//        unsigned int uiThreadID = 0;
//        m_hThread = (HANDLE)_beginthreadex(0, 0, Runner, this, 0, &uiThreadID);
//
//        SUNLOG(eCRITICAL_LOG, "[CAlertRequestSender::Create] Runner thread Create, Target URL : %s\n", m_szURL);
//    }
//    else
//    {
//        SUNLOG(eCRITICAL_LOG, "[CAlertRequestSender::Create] URL or PATH or PARTITION is NULL, Initialize Failed\n");
//    }
//}
//
//bool CAlertRequestSender::SendAlert(char* pAccount, char* pClientIP, SYSTEMTIME* pLoginTime, bool bRetry)
//{
//    if((NULL == pAccount) || (NULL == pClientIP) || (NULL == pLoginTime))
//    {
//        SUNLOG(eFULL_LOG, "[CAlertRequestSender::SendAlert()] Invalid Parameter, Account : %s, ClientIP : %s\n", pAccount, pClientIP);
//        return false;
//    }
//
//    bool bConnect = true;
//
//    if(!Open())
//    {
//        SUNLOG(eFULL_LOG, "[CAlertRequestSender::SendAlert()] Error, Not InternetOpen\n");
//        bConnect = false;
//    }
//
//    if(!Connect())
//    {
//        SUNLOG(eFULL_LOG, "[CAlertRequestSender::SendAlert()] Error, Not InternetConnect\n");
//        bConnect = false;
//    }
//
//    if(!bConnect)
//    {
//        if(!bRetry)
//        {
//            InsertAlertInfo(pAccount, pClientIP, pLoginTime);
//
//            SUNLOG(eFULL_LOG, "[CAlertRequestSender::SendAlert()] HttpSendRequest Connect Failed, Insert Alert List, Account : %s, ClientIP : %s\n", pAccount, pClientIP);
//        }
//        return false;
//    }
//
//    char szTempParameter[MAX_URL_PATH] = {0, };
//    char szTempFullPath[MAX_URL_PATH + MAX_URL_PATH] = {0, };
//
//    //	sprintf(&szTemp[0], "?Tickcount=%d", ::GetTickCount());	// Request 요청 시간이 같으면 Page Fault가 일어나더라도 항상
//    // OK를 리턴하므로 요청할때마다 시간을 갱신한다
//
//    sprintf(&szTempParameter[0], "?game=SUN&site=%s&accountname=%s&clientIP=%s&loginTime=%02d-%02d-%04d%%20%02d:%02d:%02d", m_szPart, pAccount, pClientIP,
//        pLoginTime->wMonth, pLoginTime->wDay, pLoginTime->wYear, pLoginTime->wHour, pLoginTime->wMinute, pLoginTime->wSecond);
//
//    strcpy(szTempFullPath, m_szURLPath);	// 대상 경로 복사
//    strcat(szTempFullPath, szTempParameter);		// 매개변수 복사
//
//    m_hRequest = HttpOpenRequest(m_hConnect, "GET", szTempFullPath, NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE, 0);
//
//    DWORD dwStateCode = 0;
//
//    if(::HttpSendRequest(m_hRequest, NULL, 0, NULL, 0))
//    {
//        char szTemp[10] = {0, };
//        DWORD dwSize = 10;
//
//        HttpQueryInfo(m_hRequest, HTTP_QUERY_STATUS_CODE, &szTemp, &dwSize, NULL);
//
//        dwStateCode = static_cast<DWORD>(atoi(szTemp));
//    }
//    else
//    {
//        dwStateCode = GetLastError();
//    }
//
//    if(HTTP_STATUS_OK != dwStateCode)		// 어떤 이유로든 성공이 아니면...
//    {
//        if(!bRetry)
//        {
//            InsertAlertInfo(pAccount, pClientIP, pLoginTime);
//
//            // 어쨋든 전송 실패이므로 컨테이너에 저장한다음 정해진 시도만큼 Send시킨다
//            SUNLOG(eFULL_LOG, "[CAlertRequestSender::SendAlert()] HttpSendRequest Failed, Insert Alert List, Account : %s, ClientIP : %s, GetLastError : %d, URL : %s\n", pAccount, pClientIP, dwStateCode, szTempFullPath);
//        }
//        else
//        {
//            SUNLOG(eFULL_LOG, "[CAlertRequestSender::SendAlert()] HttpSendRequest Retry Failed, Account : %s, ClientIP : %s, GetLastError : %d, , URL : %s\n", pAccount, pClientIP, dwStateCode, szTempFullPath);
//        }
//        return false;
//    }
//
//    return true;
//}
//
//unsigned int __stdcall CAlertRequestSender::Runner(void* lParam)
//{
//    CAlertRequestSender* pAlertRequestSender = (CAlertRequestSender*) lParam;
//
//    while(1)
//    {
//        if(WaitForSingleObject(pAlertRequestSender->m_eventKill, 60000) == WAIT_OBJECT_0)
//        {
//            SUNLOG(eCRITICAL_LOG, "[CAlertRequestSender::Runner] escape Ruuner Thread!!\n");
//            break;
//        }
//
//        if(pAlertRequestSender->m_Pool.empty())
//        {
//            continue;
//        }
//
//        CAlertRequestSender::ALERTLIST::iterator iter = pAlertRequestSender->m_AlertList.begin();
//        AlertInfo* pAlertInfo = NULL;
//        DWORD dwIntervalTickCount = 0;
//        DWORD dwNowTickCount = ::GetTickCount();
//
//        while(1)
//        {
//            pAlertInfo = *iter;
//            dwIntervalTickCount = dwNowTickCount - pAlertInfo->dwLoginTickCount;
//
//            if(dwIntervalTickCount < 1800000)
//            {
//                break;
//            }
//
//            if((0 != pAlertInfo->dwRetryTickCount) && (pAlertInfo->dwRetryTickCount < 1800000))
//            {
//                ++iter;
//                continue;
//            }
//
//            if(pAlertRequestSender->SendAlert(pAlertInfo->szAccount, pAlertInfo->szClientIP, &pAlertInfo->LoginTime, true))
//            {
//                pAlertInfo->Reset();
//                pAlertRequestSender->m_Pool.push_back(pAlertInfo);
//                pAlertRequestSender->m_AlertList.erase(iter++);
//                SUNLOG(eFULL_LOG, "[CAlertRequestSender::Runner] Send Request, Account : %s, ClientIP : %s\n",
//                    pAlertInfo->szAccount, pAlertInfo->szClientIP);
//            }
//            else
//            {
//                if(dwIntervalTickCount > 3600000)	// 1시간이 경과했으면 삭제한다
//                {
//                    pAlertInfo->Reset();
//                    pAlertRequestSender->m_Pool.push_back(pAlertInfo);
//                    pAlertRequestSender->m_AlertList.erase(iter++);
//                    SUNLOG(eFULL_LOG, "[CAlertRequestSender::Runner] TimeOut, Delete Alert List,  Account : %s, ClientIP : %s\n",
//                        pAlertInfo->szAccount, pAlertInfo->szClientIP);
//                }
//                else
//                {
//                    pAlertInfo->dwRetryTickCount = dwNowTickCount;
//                    ++iter;
//                }
//            }
//        }
//    }
//
//    _endthreadex(0);
//    return 1;
//}
//
//#endif // _PAAS_ALERT

