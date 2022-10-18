#include "stdafx.h"
#include "SkillFactory.h"
//
#include <SkillInfoParser.h>

//==================================================================================================

SkillFactory::ISkillAllocator::~ISkillAllocator()
{
}

template<class SkillType_>
struct SkillFactory::SkillAllocNode : public SkillFactory::ISkillAllocator
{
    typedef SkillType_ SkillType;
    static const size_t kSkillTypeSize = sizeof(SkillType);

    virtual ~SkillAllocNode()
    {
    }

    virtual Skill* Allocate() const
    {
        void* ptr = TAllocMalloc(kSkillTypeSize);
        ZeroMemory(ptr, kSkillTypeSize);
        return new (ptr) SkillType;
    }
    //
    virtual void Deallocate(Skill* ptr) const
    {
        SkillType* object = static_cast<SkillType*>(ptr);
        object->~SkillType();
        TAllocFree(object);
    }
};

template<>
struct SkillFactory::SkillAllocNode<void> : public SkillFactory::ISkillAllocator
{
    virtual ~SkillAllocNode()
    {
    }

    virtual Skill* Allocate() const
    {
        SUNLOG(eCRITICAL_LOG, _T("|Msg=request an allocation operation about an unknown skill type|"));
        return NULL;
    }
    //
    virtual void Deallocate(Skill* ptr) const
    {
        SUNLOG(eCRITICAL_LOG, _T("|Msg=request an free operation about an unknown skill type|"));
    }
};

namespace nsSkill {
;

static const SkillFactory::SkillAllocNode<void> unknown_skill_static_;

};

SkillFactory* SkillFactory::skill_factory_static_ = NULL;
const SkillFactory::ISkillAllocator* SkillFactory::skill_allocators_static_[SKILL_TYPE_UPPERBOUND];

//==================================================================================================

SkillFactory* SkillFactory::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(SkillFactory** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        SkillFactory** static_table_;
    } auto_closure(&skill_factory_static_);
    //
    if (skill_factory_static_ != NULL) {
        return skill_factory_static_;
    }
    //
    SkillFactory* provider = new SkillFactory;
    // initialization...
    skill_factory_static_ = provider;
    //
    return skill_factory_static_;
}

//==================================================================================================

SkillFactory::SkillFactory()
{
    ZeroMemory(skill_allocators_static_, sizeof(skill_allocators_static_));
    const ISkillAllocator** it = skill_allocators_static_,
                         ** end = &skill_allocators_static_[_countof(skill_allocators_static_)];
    for (; it != end; ++it) {
        *it = &nsSkill::unknown_skill_static_;
    }
    //
    skill_allocators_static_[SKILL_TYPE_PASSIVE] = new SkillAllocNode<PassiveSkill>;
    skill_allocators_static_[SKILL_TYPE_ACTIVE_INSTANT] = new SkillAllocNode<InstantSkill>;
    skill_allocators_static_[SKILL_TYPE_ACTIVE_DELAYED] = new SkillAllocNode<DelayedSkill>;
    skill_allocators_static_[SKILL_TYPE_STYLE] = new SkillAllocNode<StyleSkill>;
    skill_allocators_static_[SKILL_TYPE_NORMAL] = new SkillAllocNode<NormalAttack>;
    skill_allocators_static_[SKILL_TYPE_NORMAL_AREA] = new SkillAllocNode<NormalAreaAttack>;
}

SkillFactory::~SkillFactory()
{
    const ISkillAllocator** it = skill_allocators_static_,
                         ** end = &skill_allocators_static_[_countof(skill_allocators_static_)];
    for (; it != end; ++it)
    {
        const ISkillAllocator* allocator = *it;
        if (allocator == &nsSkill::unknown_skill_static_) {
            continue;
        }
        delete allocator;
    }
    ZeroMemory(skill_allocators_static_, sizeof(skill_allocators_static_));
}

Skill* SkillFactory::AllocSkill(eSKILL_TYPE skill_type,
                                const ROOT_SKILLINFO* root_skill_info, WORD skill_delay) const
{
    const SkillScriptInfo* const base_skill_info \
        = root_skill_info->IsSkill() ?  static_cast<const SkillScriptInfo*>(root_skill_info)
        :                               NULL;

    if (base_skill_info)
    {
        if (base_skill_info->m_bySkillType == SKILL_TYPE_PASSIVE)
        {
            skill_type = SKILL_TYPE_PASSIVE;
        }
        else if (skill_delay ||
                 base_skill_info->m_wSkillCasting ||
                 base_skill_info->m_wFlyingLifeTime)
        {
            skill_type = SKILL_TYPE_ACTIVE_DELAYED;
        }
        else
        {
            skill_type = SKILL_TYPE_ACTIVE_INSTANT;
        }
    };
    //
    if (FlowControl::FCAssert(skill_type < _countof(skill_allocators_static_)) == false) {
        return NULL;
    }
    //
    const ISkillAllocator* allocator = skill_allocators_static_[skill_type];
    assert(allocator);
    Skill* skill = allocator->Allocate();
    return skill;
}


void SkillFactory::FreeSkill(Skill* skill) const
{
    const eSKILL_TYPE skill_type = skill->GetSkillType();
    const ISkillAllocator* allocator = skill_allocators_static_[skill_type];
    assert(allocator);
    allocator->Deallocate(skill);
}

