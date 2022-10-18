#pragma once
#ifndef SKILL_SYSTEM_VERSION_ROUTING_INL
    #error "can't direct include this file"
#endif

#include <Singleton.h>
// below include files has implicit internal reference
// because of this factory class is used only in a skill manager
#include "Skill.h"
#include "InstantSkill.h"
#include "DelayedSkill.h"
#include "StyleSkill.h"
#include "PassiveSkill.h"
#include "NormalAttack.h"
#include "NormalAreaAttack.h"

class SkillScriptInfo;

//==================================================================================================
//  @history
//      - ~100820
//      - 100820, waverix, CHANGES, f100820.3L, add an interface frame
//                         to support skill allocation process using a shared memory
//==================================================================================================

class SkillFactory
{
public:
    struct ISkillAllocator {
        virtual ~ISkillAllocator() = 0;
        virtual Skill* Allocate() const = 0;
        virtual void Deallocate(Skill* ptr) const = 0;
    };
    template<class SkillType_>
    struct SkillAllocNode;
    //
    virtual ~SkillFactory();
    // util::Singletone interfaces
    static SkillFactory* Instance();
    //
    Skill* AllocSkill(eSKILL_TYPE skill_type,
                      const ROOT_SKILLINFO* root_skill_info, WORD skill_delay = 0) const;
    void FreeSkill(Skill* skill) const;

private:
    SkillFactory();
    static SkillFactory* CreateInstance();
    //
    static const ISkillAllocator* skill_allocators_static_[SKILL_TYPE_UPPERBOUND];
    static SkillFactory* skill_factory_static_;
};

//==================================================================================================
// implements
//
// interfaces for util::Singleton<>
inline SkillFactory* SkillFactory::Instance()
{
    if (skill_factory_static_) {
        return skill_factory_static_;
    }
    return CreateInstance();
}
