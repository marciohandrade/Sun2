#include "stdafx.h"

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "AttendanceReward.h"
#include "AttendanceParser.h"
#include "RewardManager.h"

bool AttendanceReward::ObtainReward( Player* player, DWORD reword_info_index )
{
    // 보상이 없는 날은 보상을 받은것으로 한다
    if (reword_info_index == 0)
    {
        return true;
    }

    MSG_CG_ITEM_OBTAIN_ITEM_CMD cmd_msg;
    cmd_msg.m_tagInventory.m_InvenCount = 0;
    cmd_msg.m_tagInventory.m_TmpInvenCount = 0;
    RC::eREWARD_RESULT result = g_RewardManager.RewardItem(RewardManager::eREWARD_ATTENDANCE, 
        reword_info_index, 0, player, cmd_msg.m_tagInventory);
    if( result != RC::RC_REWARD_SUCCESS )
    {
        MSG_CG_ITEM_OBTAIN_ITEM_NAK nak_msg;
        nak_msg.m_byErrorCode = result;
        player->SendPacket(&nak_msg, sizeof(nak_msg));
        return false;
    }

    player->SendPacket(&cmd_msg, cmd_msg.GetSize());
    return true;
}

bool AttendanceRewardCalendar::IsSatisfyCondition()
{
    if (m_attendance_info == NULL)
    {
        return false;
    }

    return (m_attendance_info->GetAttendanceInfoCount(eATTENDANCE_INFO_ATTENDED) > 0);
}

void AttendanceRewardCalendar::RewardComplete( Player* player )
{
    const ATTENDANCE* attendance = AttendanceParser::Instance()->GetAttendance(
        m_attendance_info->GetYear(), m_attendance_info->GetMonth());
    if (attendance == NULL)
    {
        return;
    }

    int day = 0;
    for (eATTENDANCE_INFO* it = m_attendance_info->begin();
        it != m_attendance_info->end(); ++it, ++day)
    {
        if (*it != eATTENDANCE_INFO_ATTENDED)
        {
            continue;
        }

        if (day < 0 || ATTENDANCE::kDayMaxCount <= day)
        {
            continue;
        }

        if (ObtainReward(player, attendance->day_reward_index[day]) == false)
        {
            continue;
        }

        *it = eATTENDANCE_INFO_ALEADY_GOT_REWARD;

        TCHAR tszAddInfo[MAX_ADDINFO_SIZE + 1] = { 0, };
        _snprintf(tszAddInfo, MAX_ADDINFO_SIZE, "Year:%d,Month:%d,day:%d,reward:%d", 
            m_attendance_info->GetYear(), m_attendance_info->GetMonth(), day, 
            attendance->day_reward_index[day]);
        GAMELOG->LogSimpleAction(ACT_ATTENDANCE_GOT_REWARD, player, tszAddInfo);
    }
}

bool AttendanceRewardSuperposition::IsSatisfyCondition()
{
    if (m_attendance_info == NULL)
    {
        return false;
    }

    return  (m_attendance_info->GetAttendanceInfoCount(eATTENDANCE_INFO_ATTENDED) > 0);
}

void AttendanceRewardSuperposition::RewardComplete( Player* player )
{
    const ATTENDANCE* attendance = AttendanceParser::Instance()->GetAttendance(
        m_attendance_info->GetYear(), m_attendance_info->GetMonth());
    if (attendance == NULL)
    {
        return;
    }

    int count_got_reward = m_attendance_info->GetAttendanceInfoCount(eATTENDANCE_INFO_ALEADY_GOT_REWARD);
    for (eATTENDANCE_INFO* it = m_attendance_info->begin();
        it != m_attendance_info->end(); ++it)
    {
        if (*it != eATTENDANCE_INFO_ATTENDED)
        {
            continue;
        }

        if (count_got_reward < 0 || count_got_reward >= ATTENDANCE::kDayMaxCount)
        {
            continue;
        }

        if (ObtainReward(player, attendance->day_reward_index[++count_got_reward - 1]) == false)
        {
            return;
        }

        *it = eATTENDANCE_INFO_ALEADY_GOT_REWARD;

        TCHAR tszAddInfo[MAX_ADDINFO_SIZE + 1] = { 0, };
        _snprintf(tszAddInfo, MAX_ADDINFO_SIZE, "Year:%d,Month:%d,day:%d,reward:%d", 
            m_attendance_info->GetYear(), m_attendance_info->GetMonth(), 
            count_got_reward, attendance->day_reward_index[count_got_reward - 1]);
        GAMELOG->LogSimpleAction(ACT_ATTENDANCE_GOT_REWARD, player, tszAddInfo);
    }
}

#endif _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM