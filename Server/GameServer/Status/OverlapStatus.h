#pragma once
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "AbilityStatus.h"


class OverlapStatus : public AbilityStatus
{
public:
    OverlapStatus() : current_overlap_count_(0), max_overlap_count_(0) {}
    ~OverlapStatus() {}

    virtual void Init(Character* owner, Character* attacker, Ability* ability);

    virtual bool IsOverlapStatus() const { return true; }
    virtual bool StatusOverlap();   // ÁßÃ¸ +1
    virtual WORD GetStatusOverlapCount() { return current_overlap_count_; }

private:
    WORD current_overlap_count_;
    WORD max_overlap_count_;
};

#endif //_NA_007667_20141001_WITCHBLADE_SKILL