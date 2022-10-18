#pragma once
#ifndef GAMESERVER_PVP_INFO_H
#define GAMESERVER_PVP_INFO_H

#include "./PVPCommon.h"
//enum ePVP_USER_STATE <- moved to PVPCommon.h

//==================================================================================================

// (NOTE) the data container for the PvP info of a player, no virtual
class PVPInfo
{
public:
    PVPInfo(Player* player);
    ~PVPInfo();

    void Init();
    void Clear(); // (WARNING) clear score fields only

    ePVP_USER_STATE GetPVPState() const;
    void SetPVPState(ePVP_USER_STATE sts);

    BYTE GetTeam() const;
    DWORD GetTotalKillNum() const;
    DWORD GetTotalDieNum() const;
    DWORD GetMaxSeries() const;

    void IncKill();
    void IncDie();

    void PlusPVPPoint(WORD plus_point);
    void MinusPVPPoint(WORD minus_point);

private:
    void SetTeam(BYTE team);

    bool IsExpired();
    void StartTimer(DWORD delay);
    void StopTimer();

    void IncDraw();

    DWORD CalcPoint();
    DWORD CalcScore(LEVELTYPE my_level, LEVELTYPE other_level);     //< 획득한 Score 리턴

    //
    ePVP_USER_STATE user_state_;
    // 현재 존에서의 정보
    BYTE my_team_no_;
    DWORD series_;
    DWORD max_series_;
    DWORD number_of_kills_;
    DWORD number_of_dies_;
    // BASE_PLAYERINFO의 정보
    Player* player_;
    util::Timer timeout_timer_;
    friend class PVPManager;
};

//==================================================================================================

inline void PVPInfo::Clear()
{
    series_ = max_series_ = number_of_kills_ = number_of_dies_ = 0;
}

inline ePVP_USER_STATE PVPInfo::GetPVPState() const
{
    return user_state_;
}

inline void PVPInfo::SetPVPState(ePVP_USER_STATE sts)
{
    user_state_ = sts;
}

inline BYTE PVPInfo::GetTeam() const
{
    return my_team_no_;
}

inline DWORD PVPInfo::GetTotalKillNum() const
{
    return number_of_kills_;
}

inline DWORD PVPInfo::GetTotalDieNum() const
{
    return number_of_dies_;
}

inline DWORD PVPInfo::GetMaxSeries() const
{
    return max_series_;
}

inline void PVPInfo::IncKill()
{
    ++number_of_kills_;
    ++series_;
}

inline void PVPInfo::IncDie()
{
    ++number_of_dies_;
    if (max_series_ < series_) {
        max_series_ = series_;
    }
    series_=0;
}

inline void PVPInfo::SetTeam(BYTE team)
{
    my_team_no_ = team;
}

inline bool PVPInfo::IsExpired()
{
    return timeout_timer_.IsExpiredManual(false) != false;
}

inline void PVPInfo::StartTimer(DWORD delay)
{
    return timeout_timer_.SetTimer(delay);
}

inline void PVPInfo::StopTimer()
{
    timeout_timer_.DisableCheckTime();
}

inline void PVPInfo::IncDraw()
{
    BASE_PLAYERINFO* char_info = player_->GetCharInfo();
    ++(char_info->m_iPVPTotalDraw);
}

#endif //GAMESERVER_PVP_INFO_H
