#pragma once

/********************************************************************
	created:	2010/11/23
	created:	23:11:2010   18:23
	filename: 	d:\Svn\trunk\Server\Common\Pms\PMSObserver.h
	file path:	d:\Svn\trunk\Server\Common\Pms
	file base:	PMSObserver
	file ext:	h
	author:		youngmoon
	
	purpose:	PMS 이벤트를 받고 값을 보낸다.
*********************************************************************/

#ifdef _JP_0_20101123_PMS

#include "IPMSObject.h"

/********************************************************************
공지를 보내는 작업은 라이브러리 단이 아니라 서버단에서 이루워진다.
그렇기 때문에 공지를 보내는 서버에서 IAnnounceRequest 추상 클래스를 상속받는 객체를 만들어
공지 전송을 처리 하도록 한다.*/
class IAnnounceRequest
{
public:
    virtual BOOL OnAnnounceReq(LPCSTR lpszMsg) = 0;
};
/*********************************************************************/


class PMSObserver : public IPMSObject
{
public:						
    PMSObserver(void);
    virtual ~PMSObserver(void);

public:	
    virtual BOOL OnAnnounceReq(DWORD dwSSN, DWORD dwCategoryID, LPCSTR lpszMsg);
    virtual BOOL OnHeartbeatReq(LONG lIndex);
    virtual BOOL OnRegionInfoReq(IPMSRegionInfoList* plstRegionInfo);
    virtual BOOL OnStatInfoReq(IPMSStatInfoList* plstStatInfo);
    virtual BOOL OnPerformInfoReq(IPMSPerformanceInfo *pPerformanceInfo);
    virtual BOOL OnOrderReq(LPCSTR lpszCmdName, LPCSTR lpszCtlVal, LPSTR lpResult, LONG *lpResultLen, DWORD dwSSN, DWORD dwCategoryID);
    virtual BOOL OnRegionInfoPCReq(IPMSRegionInfoListPC* plstRegionInfoPC);
    virtual BOOL OnStatInfoPCReq(IPMSStatInfoListPC* plstStatInfoPC);

public:
    typedef std::vector<LONG> vecRegion;

    inline void SetCategory(DWORD category)            {category_ = category;};
    inline void SetUserCount(DWORD user_count)         {user_count_ = user_count;};
    inline void SetSessioCount(DWORD session_count)    {session_couont_ = session_count;};
    inline void SetChannelCount(DWORD channel_count)   {channel_count_ = channel_count;};
    inline void SetRoomCount(DWORD room_count)         {room_count_ = room_count;};

    inline DWORD GetCategory()                         {return category_;};
    inline DWORD GetUserCount()                        {return user_count_;};
    inline DWORD GetSessioCount()                      {return session_couont_;};
    inline DWORD GetChannelCount()                     {return channel_count_;};
    inline DWORD GetRoomCount()                        {return room_count_;};

    void SetAnnounceRequest(IAnnounceRequest* announce_request)   {announce_request_ = announce_request;};

private:    
    static const DWORD pms_ssn_     = 715;
    
    DWORD category_;
    DWORD user_count_;
    DWORD session_couont_;	
    DWORD channel_count_;
    DWORD room_count_;

    IAnnounceRequest* announce_request_;    
};

#endif //_JP_0_20101123_PMS
