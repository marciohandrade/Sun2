#pragma once
#ifndef GAMESERVER_INTENSIFY_SUMMON_H
#define GAMESERVER_INTENSIFY_SUMMON_H

#include <functional>
#include "AbilityStatus.h"
#include "SummonManager.h"
#include "SkillAttrCalculator.h"

//==================================================================================================

class NPC;

// 소환수 강화
class IntensifySummon : public AbilityStatus
{
public:
    struct ChangeSummonAttr;
    //
    IntensifySummon();
    virtual ~IntensifySummon();
    //
    virtual void Start();
    virtual void Execute();
    virtual void End();

    // CHANGES: f110622.4L, when the Adrenaline status is activating, it is just fail
    // to call the ApplyAdrenaline in InitSummoned
    // because at that point in time, the summoned can't find in its list.
    // the below method support attributes application for the each summoned
    // in the same reason as above.
    // the release attributes operation is invoked by this object::End()
    void ApplyAttributes(NPC* const summoned);
private:
    void ApplyAtrributesForAllSummoned(bool applied);
    // data fields...
    // NOTE: f110808.5L, the start and end timestamp of the adrenaline status have
    // mismatched summoner attributes. this data structure is used to complete stacked option changes.
    nsSkill::AdrenalineConfiguration applied_data_;
    //
    __DISABLE_COPY(IntensifySummon);
};

#endif //GAMESERVER_INTENSIFY_SUMMON_H