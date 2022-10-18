#include "stdafx.h"
#include "SkillCoolTimeAccessor.h"


#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE

SkillCoolTimeAccessor::SkillCoolTimeAccessor()
{
}

SkillCoolTimeAccessor::~SkillCoolTimeAccessor()
{
}

void SkillCoolTimeAccessor::ApplySkillCoolTime(Player* player, const SkillCoolTime* skill_cool_time)
{
    for (uint i = 0 ; i < skill_cool_time->count ; ++i)
    {
        const SkillCoolTime::CoolTimeInfo& info = skill_cool_time->info[i];
        util::Timer* timer = player->GetSkillCoolTimer(info.skill_code);
        if (timer)
        {
            timer->Reset();
            timer->InitCoolTime(GetTickCount() + info.remain_second);
        }
    }
}

void SkillCoolTimeAccessor::GetSkillCoolTime(Player* player, SkillCoolTime* OUT skill_cool_time)
{
    const COOL_TIME_TABLE& cool_time_table = player->get_cool_time_table();
    if (cool_time_table.empty())
    {
        skill_cool_time->count = 0;
        return;
    }

    uint idx = 0;
    COOL_TIME_TABLE::const_iterator iter = cool_time_table.begin();
    for ( ; iter != cool_time_table.end() ; ++iter)
    {
        util::Timer* timer = iter->second;
        uint remain_second = timer->GetRemainedTime();
        if (remain_second)
        {
            SkillCoolTime::CoolTimeInfo& info = skill_cool_time->info[idx];
            info.skill_code = iter->first;
            info.remain_second = remain_second;
            ++idx;
        }
    }

    skill_cool_time->count = idx;
}
#endif // _NA_0_20110308_SKILL_COOL_TIME_STORAGE