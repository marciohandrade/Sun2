#pragma once

/********************************************************************
	created:	2010/12/01
	created:	1:12:2010   14:34
	filename: 	d:\Svn\trunk\Server\Common\Pms\PMSManager.h
	file path:	d:\Svn\trunk\Server\Common\Pms
	file base:	PMSManager
	file ext:	h
	author:		youngmoon
	
	purpose:	실제 서버들이 사용할 PMS 관리 객체
*********************************************************************/

#ifdef _JP_0_20101123_PMS

#include <Singleton.h>
#include "PMSConnWrapper.h"
#include "PMSObserver.h"


class PMSManager
{
public:						
    PMSManager(void);
    virtual ~PMSManager(void);

public:
    void Run();
    void Stop();

    inline void SetCategory(DWORD category)            {pms_observer_.SetCategory(category);};
    inline void SetUserCount(DWORD user_count)         {pms_observer_.SetUserCount(user_count);};
    inline void SetSessioCount(DWORD session_count)    {pms_observer_.SetSessioCount(session_count);};
    inline void SetChannelCount(DWORD channel_count)   {pms_observer_.SetChannelCount(channel_count);};
    inline void SetRoomCount(DWORD room_count)         {pms_observer_.SetRoomCount(room_count);};

    inline DWORD GetCategory()                         {return pms_observer_.GetCategory();};
    inline DWORD GetUserCount()                        {return pms_observer_.GetUserCount();};
    inline DWORD GetSessioCount()                      {return pms_observer_.GetSessioCount();};
    inline DWORD GetChannelCount()                     {return pms_observer_.GetChannelCount();};
    inline DWORD GetRoomCount()                        {return pms_observer_.GetRoomCount();};

    inline DWORD GetStatus()                           {return pms_wrapper_.GetStatus();}

    void SetAnnounceRequest(IAnnounceRequest* announce_request)   {pms_observer_.SetAnnounceRequest(announce_request);};

    void DisplayPMSInfo();

private:
    PMSWrapper  pms_wrapper_; 
    PMSObserver pms_observer_;
};

#endif //_JP_0_20101123_PMS
