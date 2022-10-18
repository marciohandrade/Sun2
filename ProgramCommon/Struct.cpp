#include "stdafx.h"
#include "./Struct.h"

//==================================================================================================
namespace util { namespace internal {
;

struct ZoneTypeCheckNode
{
    uint8_t zone_type;
    bool    acceptabled;
};

}}; //end of namespace


// CHANGES: f110504.2L, internal check utility
bool sMAPINFO_BASE::CheckValidMapKind() const
{
    static const util::internal::ZoneTypeCheckNode kZoneTypes[] =
    {
        { eZONETYPE_LOBBY      , true  }, //  0,
        { eZONETYPE_VILLAGE    , true  }, //  1,
        { eZONETYPE_CHARSELECT , true  }, //  2,
        { eZONETYPE_MISSION    , true  }, //  3,
        { eZONETYPE_HUNTING    , false }, //  4,
        { eZONETYPE_QUEST      , false }, //  5,
        { eZONETYPE_PVP        , true  }, //  6,
        { eZONETYPE_EVENT      , false }, //  7,
        { eZONETYPE_GUILD      , false }, //  8,
        { eZONETYPE_SIEGE      , false }, //  9,
        { eZONETYPE_FIELD      , true  }, // 10,
        { eZONETYPE_CHUNTING   , true  }, // 11,
        { eZONETYPE_TUTORIAL_FIELD    , true  }, // 12,
        { eZONETYPE_INSTANCE          , true  }, // 13,
        { eZONETYPE_DOMINATION_FIELD  , true  }, // 14,
        { eZONETYPE_AIRSHIP_FIELD     , true  }, // 15,
        { eZONETYPE_SPA_FIELD         , true  }, // 16,}
        //-----------------------------------
        //eZONETYPE_MAX: 17,
    };
    BOOST_STATIC_ASSERT(_countof(kZoneTypes) == eZONETYPE_MAX);
    //
    ;;;; if (_countof(kZoneTypes) <= this->byMKind) {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
               _T("Msg=index out of ranges, invalid 'MKind(%d)' of the map_code(%d) in world.txt"),
               this->byMKind, this->MapCode);
        return false;
    }
    else if (bool blocked_type = (kZoneTypes[this->byMKind].acceptabled == false)) {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
               _T("Msg=invalid 'MKind(%d)' of the map_code(%d) in world.txt"),
               this->byMKind, this->MapCode);
        return false;
    }
    else if (this->byMKind == eZONETYPE_LOBBY && this->MapKind != 11001) {
        return false;
    }
    else if (this->byMKind == eZONETYPE_CHARSELECT && (this->MapKind != 11002 && this->MapKind != 11000)) {
        return false;
    }
    else if (this->byMKind == eZONETYPE_MISSION && this->MapCode >= MAX_MISSION_NUM)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
               _T("Msg=invalid mission map code (%d) in world.txt|"),
               this->MapCode);
        return false;
    }

    return true;
}

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

AttendanceInfo::AttendanceInfo()
{
    init(0, 0, eATTENDANCE_SUPERPOSITION, eATTENDANCE_INFO_NOT_ATTEND);
}

void AttendanceInfo::init( DWORD year, BYTE month, eATTENDANCE_TYPE type, eATTENDANCE_INFO info )
{
    for (int i = 0; i < kRecordMaxCount; ++i)
    {
        m_info[i] = info;
    }
    m_month = month;
    m_year = year;
    m_type = type;
}

eATTENDANCE_INFO* AttendanceInfo::GetAttendance( int day )
{
    if (day < 0 || kRecordMaxCount <= day)
    {
        return NULL;
    }

    return &m_info[day];
}

bool AttendanceInfo::SetAttendance( int day, eATTENDANCE_INFO info )
{
    if (day < 0 || kRecordMaxCount <= day)
    {
        return false;
    }

    m_info[day] = info;
    return true;
}

bool AttendanceInfo::IsEqualAttendanceInfo( int day, eATTENDANCE_INFO type )
{
    eATTENDANCE_INFO* attendance_info = GetAttendance(day);
    if (attendance_info != NULL && *attendance_info == type)
    {
        return true;
    }

    return false;
}

int AttendanceInfo::GetAttendanceInfoCount( eATTENDANCE_INFO type )
{
    int count = 0;
    for (eATTENDANCE_INFO* it = begin(); it != end(); ++it)
    {
        if (*it == type)
        {
            ++count;
        }
    }
    return count;
}

#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
void NotificationInfo::SetInfo( const TCHAR* char_name, MAPCODE map_code, DWORD time, NotificationType notification_type, SLOTCODE item_code, int option_value1 /*= 0*/ )
{
    _tcsncpy(m_char_name, char_name, _countof(m_char_name));
    m_map_code = map_code;
    m_item_code = item_code;
    m_time = time;
    m_notification_type = notification_type;
    m_optione_value1 = option_value1;
}

void NotificationInfo::Clear()
{
    memset(m_char_name, 0, _countof(m_char_name));
    m_map_code = 0;
    m_item_code = 0;
    m_time = 0;
    m_notification_type = eNotificationNone;
    m_optione_value1 = 0;
}
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM