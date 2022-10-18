#ifndef _SOLAR_SLOT_SCWAYPOINTINFO_H
#define _SOLAR_SLOT_SCWAYPOINTINFO_H
#pragma once

#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"

class BitStream;

//==================================================================================================
//
class SCWayPointInfo
{
    // changed value by _NA_002474_20110330_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME
public:
#ifdef _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME
    enum { eDeleteTime = 10, };
#else
    enum { eDeleteTime = 1, };
#endif // _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME

private:    
    SCWayPointInfo& GetEditableObject() const { return const_cast<SCWayPointInfo&>(*this); }
    void Clear();

    //void Encode(BitStream& IN bitstream) const;
    //void Decode(const WORD item_type, BitStream& OUT bitstream);

    //void Encode(OPTIONPART_VER5* OUT dest) const;
    //void Decode(const WORD item_type, const OPTIONPART_VER5* IN src);
    void ResetWayPointDelete();
    //BOOL CanWayPointDelete() { return delete_enabled_; }

    void SetInfo(nsSlot::SerializeItemOption::sWayPoint* src, const BASE_ITEMINFO* item_info);
    void GetInfo(nsSlot::SerializeItemOption::sWayPoint* dest);

    void Update(const DWORD64& current_time);

    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_DeleteEnabled = 1;
    static const DWORD NumberOfBits_ExpireTimeForDelete = 45;
    //----------------------------------------------------------------------------------------------
    BOOLEAN delete_enabled_; // 
    DWORD64 expire_time_for_delete_;

    friend class SCItemSlot;
};

//--------------------------------------------------------------------------------------------------

inline void
SCWayPointInfo::Clear() {
    ZeroMemory(this, sizeof(*this));
    // (WARNING) 초기값은 true이다. delete_disabled는 부정형이라서리...
    delete_enabled_ = true;
}

inline void
SCWayPointInfo::Update(const DWORD64& current_time) {
    if(delete_enabled_)
        return;
    if(current_time > expire_time_for_delete_)
        delete_enabled_ = true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
//

#endif //_SOLAR_SLOT_SCWAYPOINTINFO_H
