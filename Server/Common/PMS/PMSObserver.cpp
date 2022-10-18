#include "StdAfx.h"
#include ".\PMSObserver.h"

#ifdef _JP_0_20101123_PMS

PMSObserver::PMSObserver(void) : category_(0), 
                                 user_count_(0),
                                 session_couont_(0),
                                 channel_count_(0),
                                 room_count_(0),
                                 announce_request_(NULL)
{
    
}

PMSObserver::~PMSObserver(void)
{
}

BOOL PMSObserver::OnHeartbeatReq(LONG lIndex)
{
    //_tprintf(_T("OnHeartbeatReq Seq:%d\n"), lIndex); 

    return TRUE;
}

BOOL PMSObserver::OnRegionInfoReq(IPMSRegionInfoList* plstRegionInfo)
{
    //_tprintf(_T("OnRegionInfoReq\n"));

    /*
    not using now
    mplement dummy interface
    */

    return FALSE;
}

BOOL PMSObserver::OnRegionInfoPCReq(IPMSRegionInfoListPC* plstRegionInfoPC)
{
    //_tprintf(_T("OnRegionInfoPCReq\n"));

    /*
    not using now
    mplement dummy interface
    */

    return FALSE;
}

BOOL PMSObserver::OnStatInfoReq(IPMSStatInfoList* plstStatInfo)
{ 
    //_tprintf(_T("OnStatInfoReq\n"));

    plstStatInfo->AddStatInfoList(pms_ssn_, category_, user_count_, session_couont_, channel_count_, room_count_, NULL);

    return TRUE;
}

BOOL PMSObserver::OnStatInfoPCReq(IPMSStatInfoListPC* plstStatInfoPC)
{ 
    //_tprintf(_T("OnStatInfoPCReq\n"));

    plstStatInfoPC->AddStatInfoListPC(pms_ssn_, category_, user_count_, session_couont_, channel_count_, room_count_, NULL);

    return TRUE;
}

BOOL PMSObserver::OnPerformInfoReq(IPMSPerformanceInfo *pPerformanceInfo)
{ 
    //_tprintf(_T("OnPerformInfoReq\n"));

    /*
    not using now
    implement dummy interface
    */

    return FALSE;
}

BOOL PMSObserver::OnAnnounceReq(DWORD dwSSN, DWORD dwCategoryID, LPCSTR lpszMsg)
{
    //if (lpszMsg)
     //   printf("OnAnnounceReq SSN:%u CategoryID:%u lpszMsg:%s\n", dwSSN, dwCategoryID, lpszMsg);

//    AfxMessageBox(lpszMsg);
    if(announce_request_ == NULL)
        return FALSE;

    return announce_request_->OnAnnounceReq(lpszMsg);
}

BOOL PMSObserver::OnOrderReq(LPCSTR lpszCmdName, LPCSTR lpszCtlVal, LPSTR lpResult, LONG *lpResultLen, DWORD dwSSN, DWORD dwCategoryID)
{
    //if (lpszCmdName&&lpszCtlVal)
      //  _tprintf(_T("OnAnnounceReq SSN:%u CategoryID:%u lpszlpszCmdName:%s lpszCtlVal:%s\n"), dwSSN, dwCategoryID, lpszCmdName, lpszCtlVal);

    /*
    Custom Action that PMS Administrator need
    :psuedo:
    objCommand = CommandAction::generate(lpszCmdName)
    lpResult = objCommand->do_action(lpszCtlVal)
    :psuedo:
    */

    return FALSE;
}




#endif //_JP_0_20101123_PMS