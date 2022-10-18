#include "stdafx.h"
#include "./MissionManager.h"

//==================================================================================================
BOOST_STATIC_ASSERT(MAX_MISSION_NUM < static_cast<MissionManager::MissionCode_t>(-1));

void MissionManager::SerializeStream(MISSION_TOTAL_INFO& IN total_info, eSERIALIZE serialize_type)
{
    BOOST_STATIC_ASSERT(MAX_MISSION_NUM == MISSION_TOTAL_INFO::MAX_SLOT_NUM);
    // 수정되지 않았다로 초기화!!
    modified_ = false;

    if (serialize_type == SERIALIZE_STORE)
    {
        Clear();
        int number_of_nodes = total_info.m_Count;
        if (FlowControl::FCAssert(number_of_nodes <= _countof(mission_parts_)) == false) {
            number_of_nodes = _countof(mission_parts_);
        };
        const MISSION_TOTAL_INFO::SLOT_TYPE* slot_it = total_info.m_Slot,
            * slot_end = &total_info.m_Slot[number_of_nodes];
        for ( ; slot_it != slot_end; ++slot_it)
        {
            const MissionCode_t mission_code = slot_it->m_MissionCode;
            if (!FlowControl::FCAssert(mission_code && mission_code <= _countof(mission_parts_))) {
                continue;
            }
            int offset = mission_code - 1;
            mission_parts_[offset] = *slot_it;
        }

        CheckEnableWindowOpen();
    }
    else if (serialize_type == SERIALIZE_LOAD)
    {
        typedef MISSION_TOTAL_INFO::SLOT_TYPE SLOT_TYPE;
        int count = 0;
        const MISSIONPART* mission_it = mission_parts_,
                         * mission_end = &mission_parts_[_countof(mission_parts_)];
        for ( ; mission_it != mission_end; ++mission_it)
        {
            if (mission_it->Point > 0)
            {
                SLOT_TYPE* const mission_slot = &total_info.m_Slot[count];
                // (CHANGES) another assert check, fix up compile error in vs2003 and
                // fit in coding rules abount output type
                typedef mpl::if_c<sizeof(mission_slot->m_MissionCode) == sizeof(uint8_t),
                    MissionCode_t, void>::type  SafeConversionCheck;

                ZeroMemory(mission_slot, sizeof(*mission_slot));
                static_cast<MISSIONPART&>(*mission_slot) = *mission_it;
                MissionCode_t mission_code = \
                    static_cast<SafeConversionCheck>(mission_it - mission_parts_) + 1;
                mission_slot->m_MissionCode = mission_code;
                ++count;
            }
        }
        total_info.m_Count = count;
    }
}

// for server routine
void MissionManager::SerializeStream(BYTE* stream, WORD stream_size, eSERIALIZE serialize_type)
{
    if (serialize_type == SERIALIZE_LOAD) {
        ZeroMemory(stream, stream_size);
    }
    BOOST_STATIC_ASSERT(MAX_MISSIONSTREAM_SIZE == MAX_MISSION_NUM * sizeof(mission_parts_[0]) &&
                        MAX_MISSIONSTREAM_SIZE == sizeof(mission_parts_));
    if (FlowControl::FCAssert(stream_size == MAX_MISSIONSTREAM_SIZE) == false) {
        stream_size = (stream_size < MAX_MISSIONSTREAM_SIZE)
                    ?   stream_size : MAX_MISSIONSTREAM_SIZE;
    };
    //
    modified_ = false;

    if (serialize_type == SERIALIZE_STORE)
    {
        need_reward_ = false;
        ZeroMemory(mission_parts_, sizeof(mission_parts_));
        CopyMemory(mission_parts_, stream, stream_size);
        // need_reward_ 체크
        CheckEnableWindowOpen();
    }
    else if (serialize_type == SERIALIZE_LOAD)
    {
        ASSERT(serialize_type == SERIALIZE_LOAD);
        CopyMemory(stream, mission_parts_, stream_size);
    }
}

void MissionManager::CheckEnableWindowOpen()
{
    const MISSIONPART* mission_it = mission_parts_,
        * mission_end = &mission_parts_[_countof(mission_parts_)];
    for ( ; mission_it != mission_end; ++mission_it)
    {
        const MissionCode_t mission_code = //   offset
            static_cast<MissionCode_t>(mission_it - mission_parts_) + 1;
        // 'GetMissionState(mission_code)'
        if (mission_it->State == MISSION_REWARD)
        {
            need_reward_ = true;
            to_receive_reward_mission_code_ = mission_code;
            break;
        }
    };
}

void MissionManager::SetMissionState(MissionCode_t mission_code, eMISSION_STATE state)
{
    if (mission_code == 0 || mission_code > _countof(mission_parts_)) {
        assert(!"index out of range");
        return;
    }

    const int offset = mission_code - 1;
    MISSIONPART& mission_part = mission_parts_[offset];
    //type rule check
    ASSERT(mission_part.State != static_cast<UINT>(state));
    mission_part.State = state;
    if (state == MISSION_REWARD)
    {
        need_reward_ = true;
        to_receive_reward_mission_code_ = mission_code;
    }
    else if (need_reward_ && to_receive_reward_mission_code_ == mission_code)
    {
        need_reward_ = false;
    }

    modified_ = true;
}

void MissionManager::SetMissionPoint(MissionCode_t mission_code, int point, BOOL check_same_point)
{
    if (mission_code == 0 || mission_code > _countof(mission_parts_)) {
        assert(!"index out of range");
        return;
    }

    const int offset = mission_code - 1;
    MISSIONPART& mission_part = mission_parts_[offset];
    if (check_same_point) {
        ASSERT(mission_part.Point != point);
    }
    if (point > 511) {
        point = 511;
    }
    mission_part.Point = max(point, 0);
    modified_ = true;
}


bool MissionManager::IsClearHiddenMission(MissionCode_t mission_code,
                                          MissionCode_t hidden_mission_code) const
{
    if (mission_code == 0 || mission_code > _countof(mission_parts_)) {
        assert(!"index out of range");
        return false;
    }

    const int offset = mission_code - 1;
    const MISSIONPART& mission_part = mission_parts_[offset];
    assert(hidden_mission_code == 0); // NOTE: currently, this value is not used.
    return (mission_part.Hidden & (1 << hidden_mission_code)) != 0;
}
