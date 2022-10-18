#pragma once
//=============================================================================================================================
/// MissionManager class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 12. 29
    @remark
    - 미션 정보에 대한 바이너리를 가지고 있는 조작 클래스
    @note
    - 
    @history 
    - 2010/11/30, waverix, code arrangement and recalculate the mission code index (f101130.2L)
                         , this class is derived by client class 'MissionManager_Concrete'
                         however it has no derived interfaces from the base
                         so that I changes easy modification
    - 2010/12/20, waverix, CHANGES: f101220.2L, change size control
                         based on '_NA_1388_20101207_EP2_INCREASED_MISSION_MAX' version 1101
                         accept mission ranges are [1, 32) under the 1101
                         accept mission ranges are [1, 32 (or 50)] over the 1101 +
                            _NA_1388_20101207_EP2_INCREASED_MISSION_MAX
*/
//=============================================================================================================================
#include "StructInPacket.h"

enum eMISSION_STATE
{
    MISSION_NONE   = 0,
    MISSION_REWARD = 1,
};

//==================================================================================================

class MissionManager
{
public:
    // CHANGES: f101130.2L, array index equal to 'mission_code - 1'
    // changes specification about the mission code
    //               [00]     [01]     [02]   ...   [31]
    // ~f101130.2L  UNUSED |MISSION1|MISSION2|...|MISSION31|
    // f101130.2L~ MISSION1|MISSION2|MISSION3|...|MISSION32|
    typedef uint8_t MissionCode_t; // (NOTE) (100220) (WAVERIX) add type to support mission code
    // NOTE: changes specification about 1101
    //               [00]     [01]     [02]   ...   [30]      [31]
    //  < 1101     MISSION1|MISSION2|MISSION3|...|MISSION31| (none)  |
    // >= 1101     MISSION1|MISSION2|MISSION3|...|MISSION31|MISSION32|
    // 
    static const uint8_t kNumberOfMissions = MAX_MISSION_NUM;
    MissionManager();
    ~MissionManager();

    void Clear();

    eMISSION_STATE GetMissionState(MissionCode_t mission_code) const;
    void SetMissionState(MissionCode_t mission_code, eMISSION_STATE state);
    uint16_t GetMissionPoint(MissionCode_t mission_code) const;
    void SetMissionPoint(MissionCode_t mission_code, int point, BOOL check_same_point = true);

    bool IsClearHiddenMission(MissionCode_t mission_code, MissionCode_t hidden_mission_code) const;
    void CheckEnableWindowOpen();
    bool EnableWindowOpen() const;
    uint8_t ReservedMissionNum() const;

    bool IsModified() const;

    // _NA_20081121_ABUMAN_NIJOT_EVENT
    void SetGuildEvent(bool evented);
    bool IsGuildEvent() const;
    //
    void SerializeStream(BYTE* stream, WORD stream_size, eSERIALIZE serialize_type);
    void SerializeStream(MISSION_TOTAL_INFO& IN total_info, eSERIALIZE serialize_type);

private:
    bool modified_;
    bool guild_event_; // _NA_20081121_ABUMAN_NIJOT_EVENT (KOR)
    bool need_reward_; // next Action: open the mission reward window;
    MissionCode_t to_receive_reward_mission_code_;
    //
    MISSIONPART mission_parts_[kNumberOfMissions];
    //
};

//==================================================================================================

inline MissionManager::MissionManager()
{
    // WARNING: f101130.2L, this class have no virtual interfaces,
    // I want to be forced clear countinuously.
    ZeroMemory(this, sizeof(*this));
}

inline MissionManager::~MissionManager()
{
}

inline void MissionManager::Clear() {
    ZeroMemory(this, sizeof(*this));
}

inline bool MissionManager::EnableWindowOpen() const {
    return need_reward_;
}

inline uint8_t MissionManager::ReservedMissionNum() const {
    return to_receive_reward_mission_code_;
}

inline bool MissionManager::IsModified() const {
    return modified_;
}

// _NA_20081121_ABUMAN_NIJOT_EVENT
inline void MissionManager::SetGuildEvent(bool evented) {
    guild_event_ = evented;
}

// _NA_20081121_ABUMAN_NIJOT_EVENT
inline bool MissionManager::IsGuildEvent() const {
    return guild_event_;
}

inline eMISSION_STATE MissionManager::GetMissionState(MissionCode_t mission_code) const
{
    if (mission_code && mission_code <= _countof(mission_parts_)) {
        // CHANGES: f101130.2L, array index equal to 'mission_code - 1'
        const int offset = mission_code - 1;
        return static_cast<eMISSION_STATE>(mission_parts_[offset].State);
    }

    assert(!"index out of range");
    return MISSION_NONE;
}

inline uint16_t MissionManager::GetMissionPoint(MissionCode_t mission_code) const
{
    if (mission_code && mission_code <= _countof(mission_parts_)) {
        const int offset = mission_code - 1;
        // the 'Point' field is less than 512. ranges = [0, 512)
        return static_cast<uint16_t>(mission_parts_[offset].Point);
    }
    assert(!"index out of range");
    return 0;
}

