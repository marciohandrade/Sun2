#include "SunClientPrecompiledHeader.h"
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateRisingSerpent.h"
#include "Character.h"
#include "Hero.h"
#include "SkillEffectFunc.h"
#include "SkillInfoParser.h"
#include "GlobalFunc.h"


SkillStateRisingSerpent::SkillStateRisingSerpent()
{
}

SkillStateRisingSerpent::~SkillStateRisingSerpent()
{
}

STATECHANGE::TYPE SkillStateRisingSerpent::OnStart(Object* pOwner, DWORD CurTime)
{
	// 이동 위치를 먼저 구한다
	Character* pCharacter = (Character*)pOwner;
	Character* pTarget = (Character*)pCharacter->GetTargetObject();

	WzVector CurrentPosition = pCharacter->GetVisiblePos();
	WzVector Direction = pTarget ? pTarget->GetVisiblePos() - CurrentPosition : pCharacter->GetDirection();
	VectorNormalize(&Direction,&Direction);

	static float s_MoveLength = 3.0f;
	WzVector TargetPosition = CurrentPosition + Direction * s_MoveLength;

	pCharacter->Move( CurrentPosition, TargetPosition, CMS_RUN, TRUE );

	return StateSkill::OnStart(pOwner, CurTime );
}


STATEPROCESS::TYPE SkillStateRisingSerpent::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    STATEPROCESS::TYPE Result = StateSkill::OnProcess(pOwner, CurTime, dwTick );

    Character* pCharacter = (Character*)pOwner;

    if( GetAnimationRate() <= 50.0f )
    {
        static float s_MoveSpeed = 8.0f;
        const float SPEED_PER_SECOND = s_MoveSpeed * SPEED_MULTIPLIER;
        ProcessMove( pCharacter, SPEED_PER_SECOND, dwTick);
    }
    else
    {
        pCharacter->StopMove();
    }

    return Result;
}


