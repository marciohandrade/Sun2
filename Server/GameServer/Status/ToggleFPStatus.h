#pragma once
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "AbilityStatus.h"

class ToggleFPStatus : public AbilityStatus
{
public:
    ToggleFPStatus() : recover_fp_(0.0f),
                       connected_skill_info_(NULL),
                       owner_player_(NULL) {}
    ~ToggleFPStatus() {}

    virtual void Init(Character* owner, Character* attacker, Ability* ability);
    virtual void Start() {}
    virtual void Execute();
    virtual void End();

    virtual bool IsToggleFPStatus() const {return true;}
private:
    void _ReduceFP();
    BOOL _UseSkill();
private:
    PLAYERFP recover_fp_;

    const SkillScriptInfo* connected_skill_info_;
    Player* owner_player_;
};

#endif //_NA_007667_20141001_WITCHBLADE_SKILL
