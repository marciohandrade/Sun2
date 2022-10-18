#pragma once
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "AbilityStatus.h"

class AutoCastStatus : public AbilityStatus
{
public:
    AutoCastStatus() {}
    ~AutoCastStatus() {}

    virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
    virtual VOID		Start() {}
    virtual VOID		Execute() {}

    virtual void UseSkillByAttack(Character* target);
private:
};
#endif //_NA_007667_20141001_WITCHBLADE_SKILL