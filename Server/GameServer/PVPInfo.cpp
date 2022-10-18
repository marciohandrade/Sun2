#include "stdafx.h"
#include ".\pvpinfo.h"

#include <Global.h>
#include ".\PVPFormula.h"

//==================================================================================================

PVPInfo::PVPInfo(Player* player)
    : player_(player)
{
    SetPVPState(ePVP_USER_NONE);
}

PVPInfo::~PVPInfo()
{
}

void PVPInfo::Init()
{
    Clear();
    SetPVPState(ePVP_USER_NONE);
    SetTeam(0);
    StopTimer();
}

DWORD PVPInfo::CalcPoint()
{
    BASE_PLAYERINFO* pInfo = player_->GetCharInfo();

    pInfo->m_iPVPMaxSeries = BOUND_CHECK(0, max_series_, INT_MAX);
    pInfo->m_iPVPMaxKill = BOUND_CHECK(0, number_of_kills_, INT_MAX);
    pInfo->m_iPVPMaxDie = BOUND_CHECK(0, number_of_dies_, INT_MAX);

    pInfo->m_iPVPTotalKill = BOUND_CHECK(0, pInfo->m_iPVPTotalKill + number_of_kills_, INT_MAX);
    pInfo->m_iPVPTotalDie = BOUND_CHECK(0, pInfo->m_iPVPTotalDie + number_of_dies_, INT_MAX);

    DWORD plusPoint = CalcPVPPoint(max_series_, number_of_kills_, number_of_dies_);

    pInfo->m_iPVPPoint = BOUND_CHECK(0, pInfo->m_iPVPPoint + plusPoint, INT_MAX);
    return plusPoint;
}

DWORD PVPInfo::CalcScore(LEVELTYPE my_level, LEVELTYPE other_level)
{
    BASE_PLAYERINFO* pInfo = player_->GetCharInfo();
    DWORD plusScore = CalcPVPScore(my_level, other_level - my_level);
    pInfo->m_iPVPScore = BOUND_CHECK(0, pInfo->m_iPVPScore+plusScore, INT_MAX);
    return plusScore;
}

void PVPInfo::PlusPVPPoint(WORD plus_point)
{
    BASE_PLAYERINFO* pInfo = player_->GetCharInfo();
    pInfo->m_iPVPPoint = BOUND_CHECK(0, pInfo->m_iPVPPoint + plus_point, INT_MAX);
}


void PVPInfo::MinusPVPPoint(WORD minus_point)
{
    BASE_PLAYERINFO* pInfo = player_->GetCharInfo();
    pInfo->m_iPVPPoint = BOUND_CHECK(0, pInfo->m_iPVPPoint - minus_point, INT_MAX);
}

