#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "StateBase.h"
#include "SkillState.h"
#include "Character.h"
#include "SkillStateSpiningHearts.h"

SkillStateSpiningHearts::SkillStateSpiningHearts(void)
{
    hit_count_ = 0;
}

SkillStateSpiningHearts::~SkillStateSpiningHearts(void)
{
}

STATECHANGE::TYPE SkillStateSpiningHearts::OnStart( Object* owner_ptr, DWORD cur_time )
{
    return StateSkill::OnStart( owner_ptr, cur_time );
}

STATEPROCESS::TYPE SkillStateSpiningHearts::OnProcess( Object* owner_ptr, DWORD cur_time, DWORD tick )
{
    Character* character_ptr = static_cast<Character*>(owner_ptr);
    Character* target_ptr = static_cast<Character*>(character_ptr->GetTargetObject());

    if (target_ptr == NULL)
        return STATEPROCESS::END;

    EnterProcess(owner_ptr, cur_time, tick);

    // 이벤트체크
    CheckEvent();

    WzVector start_position = character_ptr->GetVisiblePos();
    WzVector direction = start_position - target_ptr->GetVisiblePos();
    VectorNormalize(&direction, &direction);
    const float SPEED_PER_SECOND = 3.0f * SPEED_MULTIPLIER;
    if (GetAnimationRate() < 30.0f)
    {        
        WzVector target_position = start_position + direction * 2.f;
        character_ptr->Move( start_position, target_position, CMS_BACKSTEP, TRUE, -1, -1);
        ProcessMove(character_ptr, SPEED_PER_SECOND, tick);
    }
    else if (GetAnimationRate() > 30.0f && GetAnimationRate() < 50.0f)
    {
        WzVector target_position = start_position + direction * 13.f;
        character_ptr->Move( start_position, target_position, CMS_BACKSTEP, TRUE, -1, -1);
        ProcessMove(character_ptr, 13.0f * SPEED_MULTIPLIER, tick);
    }
    else
    {
        character_ptr->StopMove();
    }

    if (GetAnimationRate() >= 20 && hit_count_ < 1)
    {
        ++hit_count_;
        // 데미지 처리
        ++m_iHitProcessed;
        character_ptr->ProcessAttackResult_Group( character_ptr->GetCurrentAttackSerial() );

#ifdef _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST
		return STATEPROCESS::HOLD;
#endif // _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST
    }

    if (GetAnimationRate() >= 70 && hit_count_ < 2)
    {
        ++hit_count_;
        // 데미지 처리
        ++m_iHitProcessed;
        character_ptr->ProcessAttackResult_Group( character_ptr->GetCurrentAttackSerial() );

#ifdef _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST
		return STATEPROCESS::HOLD;
#endif // _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST
    }

    return LeaveProcess( owner_ptr, cur_time, tick );
}

STATECHANGE::TYPE SkillStateSpiningHearts::OnEnd( Object* owner_ptr, DWORD cur_time )
{
    Character* character_ptr = static_cast<Character*>(owner_ptr);

    if ( character_ptr != NULL)
    {
        character_ptr->ProcessAttackResult( character_ptr->GetCurrentAttackSerial(), TRUE );
    }

    return StateBase::OnEnd(owner_ptr, cur_time);
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE