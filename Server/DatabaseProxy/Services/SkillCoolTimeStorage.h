#ifndef SKILL_COOL_TIME_STORAGE_H_
#define SKILL_COOL_TIME_STORAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif


#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
//===============================================================================================
//
// class SkillCoolTimeStorage
//
//===============================================================================================
#include <Singleton.h>

class SkillCoolTimeStorage : public util::Singleton<SkillCoolTimeStorage>
{
public:
    SkillCoolTimeStorage();
    virtual ~SkillCoolTimeStorage();

    void SaveSkillCoolTime(uint guid, const SkillCoolTime* skill_cool_time);
    void LoadSkillCoolTime(uint guid, SkillCoolTime* OUT skill_cool_time);

private:
    //                            CHARGUID
    SkillCoolTime* find_cool_time(uint guid);

    typedef STLX_HASH_MAP<uint, SkillCoolTime> CoolTimeHashMap;
    CoolTimeHashMap cool_time_hashmap_;
};

//--------------------------------------------------------------------------
inline SkillCoolTime* SkillCoolTimeStorage::find_cool_time(uint guid)
{
    CoolTimeHashMap::iterator iter = cool_time_hashmap_.find(guid);
    if (iter != cool_time_hashmap_.end()) {
        return &(iter->second);
    }
    return NULL;
}

#endif // _NA_0_20110308_SKILL_COOL_TIME_STORAGE

#endif // SKILL_COOL_TIME_STORAGE_H_
