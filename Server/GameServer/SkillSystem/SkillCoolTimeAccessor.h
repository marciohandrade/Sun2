#ifndef SKILL_COOL_TIME_ACCESSOR_H_
#define SKILL_COOL_TIME_ACCESSOR_H_

#if _MSC_VER > 1000
#pragma once
#endif


#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
//===============================================================================================
//
// class SkillCoolTimeAccessor
//
//===============================================================================================
#include <Singleton.h>

class SkillCoolTimeAccessor : public util::Singleton<SkillCoolTimeAccessor>
{
public:
    SkillCoolTimeAccessor();
    virtual ~SkillCoolTimeAccessor();

    void ApplySkillCoolTime(Player* player, const SkillCoolTime* skill_cool_time);
    void GetSkillCoolTime(Player* player, SkillCoolTime* OUT skill_cool_time);
};

#endif // _NA_0_20110308_SKILL_COOL_TIME_STORAGE

#endif // SKILL_COOL_TIME_ACCESSOR_H_
