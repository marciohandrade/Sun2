#include "stdafx.h"
#include "SkillCoolTimeStorage.h"


#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE

SkillCoolTimeStorage::SkillCoolTimeStorage()
{
}

SkillCoolTimeStorage::~SkillCoolTimeStorage()
{
}

void SkillCoolTimeStorage::SaveSkillCoolTime(uint guid, const SkillCoolTime* skill_cool_time)
{
    SkillCoolTime* saved_cool_time = find_cool_time(guid);
    if (saved_cool_time == NULL) {
        saved_cool_time = &cool_time_hashmap_[guid];
    }
    *saved_cool_time = *skill_cool_time;
}

void SkillCoolTimeStorage::LoadSkillCoolTime(uint guid, SkillCoolTime* OUT skill_cool_time)
{
    SkillCoolTime* saved_cool_time = find_cool_time(guid);
    if (saved_cool_time == NULL)
    {
        skill_cool_time->count = 0;
        return;
    }
    *skill_cool_time = *saved_cool_time;
}


#endif // _NA_0_20110308_SKILL_COOL_TIME_STORAGE