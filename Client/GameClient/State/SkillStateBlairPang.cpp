#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "StateBase.h"
#include "SkillState.h"
#include "Character.h"
#include "SkillStateBlairPang.h"

SkillStateBlairPang::SkillStateBlairPang(void)
{
}

SkillStateBlairPang::~SkillStateBlairPang(void)
{
}

STATECHANGE::TYPE SkillStateBlairPang::OnStart( Object* owner_ptr, DWORD cur_time )
{
    return StateSkill::OnStart( owner_ptr, cur_time );
}

STATEPROCESS::TYPE SkillStateBlairPang::OnProcess( Object* owner_ptr, DWORD cur_time, DWORD tick )
{
    Character* character_ptr = static_cast<Character*>(owner_ptr);

    EnterProcess(owner_ptr, cur_time, tick);

    WzVector start_position = character_ptr->GetVisiblePos();
    WzVector direction = character_ptr->GetDirection();
    VectorNormalize(&direction, &direction);
    const float SPEED_PER_SECOND = 5.f * SPEED_MULTIPLIER;
    if (GetAnimationRate() < 40.0f)
    {
        WzVector target_position = start_position + direction * 8.f;
        character_ptr->Move( start_position, target_position, CMS_RUN, TRUE, -1, -1);
        ProcessMove(character_ptr, SPEED_PER_SECOND, tick);
    }
    else
    {
        character_ptr->StopMove();
    }

    // 이벤트체크
    CheckEvent();

    ProcessHit(character_ptr, 1);

    return LeaveProcess( owner_ptr, cur_time, tick );
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE