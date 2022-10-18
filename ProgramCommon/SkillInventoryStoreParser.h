////////////////////////////////////////////////////////////////////////////////////////////////////
// SkillInventoryStoreParser.h
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __SKILL_INVENTORY_STORE_H__
#define __SKILL_INVENTORY_STORE_H__

#include <Singleton.h>

#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    enum { MAX_SKILL_INVENTORY_STORE_NUM = 10 };
#else
    enum { MAX_SKILL_INVENTORY_STORE_NUM = 50 };
#endif

struct SkillInven_s
{
    typedef int Index;
    Index index;
    BYTE bClassID;
    BYTE bSkillTab1;
    BYTE bSkillTab2;
    WORD wSkillNum;
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    int iSkillID[MAX_SKILL_INVENTORY_STORE_NUM];    // 스킬 코드
#else
    CODETYPE dwSkillID[MAX_SKILL_INVENTORY_STORE_NUM];    // 스킬 코드
#endif    
    SkillInven_s()
    {
        ::ZeroMemory(this, sizeof(*this));
    }
};

class SkillInventoryStoreParser : public util::Singleton<SkillInventoryStoreParser>, public IParser
{
public:
    SkillInventoryStoreParser() {}
    ~SkillInventoryStoreParser() {}

public:
    virtual void Release() {}
    virtual	BOOL LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

public:
    const SkillInven_s* GetSkillInventoryStore(const SkillInven_s::Index index) const
    {
        SkillInvens::const_iterator it = skill_invens_.find(index);
        if (it == skill_invens_.end())
        {
            return NULL;
        }
        return &(it->second);
    }
    size_t GetSkillInventoryStoreNum() const
    {
        return skill_invens_.size();
    }

private:
    BOOL _Load(const BOOL is_reload);

private:
    typedef STLX_HASH_MAP<SkillInven_s::Index, SkillInven_s> SkillInvens;
    SkillInvens skill_invens_;
};

#endif // __SKILL_INVENTORY_STORE_H__