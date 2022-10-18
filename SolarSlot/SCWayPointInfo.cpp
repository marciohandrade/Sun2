#include "Stdafx.h"
#include ".\SCWayPointInfo.h"

#include <time.h>
#include <SolarDateTime.h>
#include <BitStream.hpp>

//==================================================================================================
//
void
SCWayPointInfo::SetInfo(nsSlot::SerializeItemOption::sWayPoint* src, const BASE_ITEMINFO* item_info)
{
    Clear();

    if(!FlowControl::FCAssert(item_info->m_wType == eITEMTYPE_WAYPOINT))
        return;

    SLOT_BITBOUND_SET(src->IsExpired, NumberOfBits_DeleteEnabled);
    // 시간 관련 제약 검사는 보류 
    //LARGE_INTEGER integer;
    //integer.QuadPart = src->DateTime;
    //SLOT_BITBOUND_SET(integer.HighPart, NumberOfBits_ExpireTimeForDelete);
    //src->DateTime = integer.QuadPart;
    delete_enabled_ = !!src->WayPointDeleteEnabled;
    expire_time_for_delete_ = src->WayPointDeleteDateTime;
}

void
SCWayPointInfo::GetInfo(nsSlot::SerializeItemOption::sWayPoint* dest)
{
    dest->WayPointDeleteEnabled = !!delete_enabled_;
    dest->WayPointDeleteDateTime = expire_time_for_delete_;
}

void
SCWayPointInfo::ResetWayPointDelete()
{
    delete_enabled_ = 0;

    time_t now;
    util::TimeSync::time(&now);

    tm* local_time = localtime(&now);
#ifdef _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME
    local_time->tm_min += eDeleteTime;
#else
    local_time->tm_hour += eDeleteTime;
#endif // _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME

    if(mktime(local_time) != (time_t)-1)
    {
        // m_DateTime 날짜로 교체
        util::GetDateTime_YYYYMMDDHHMMSS(local_time->tm_year + 1900,
                                         local_time->tm_mon + 1,
                                         local_time->tm_mday,
                                         local_time->tm_hour,
                                         local_time->tm_min,
                                         local_time->tm_sec,
                                         expire_time_for_delete_);
    }
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
//


