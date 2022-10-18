#include "SunClientPrecompiledHeader.h"
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateRollingPalm.h"
#include "Character.h"


SkillStateRollingPalm::SkillStateRollingPalm()
{
    m_IsMove = FALSE;
}

SkillStateRollingPalm::~SkillStateRollingPalm()
{

}

STATEPROCESS::TYPE SkillStateRollingPalm::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    STATEPROCESS::TYPE Result = StateSkill::OnProcess(pOwner, CurTime, dwTick );

    Character* pCharacter = (Character*)pOwner;
    Character* pTarget = (Character*)pCharacter->GetTargetObject();

    if( m_iHitCount == 1 )
    {
        WzVector CurrentPosition = pCharacter->GetVisiblePos();
        WzVector Direction = pTarget ? pTarget->GetVisiblePos() - CurrentPosition : pCharacter->GetDirection();
        VectorNormalize(&Direction,&Direction);

        static float s_MoveLength = -4.0f;
        WzVector TargetPosition = CurrentPosition + Direction * s_MoveLength;

        pCharacter->Move( CurrentPosition, TargetPosition, CMS_RUN, TRUE );

        m_IsMove = TRUE;
    }

    if( m_IsMove == TRUE )
    {
        static float s_StopRate = 80.f;
        if( GetAnimationRate() >= s_StopRate )
        {
            m_IsMove = FALSE;
            pCharacter->StopMove();
        }
    }

    if( m_IsMove == TRUE )
    {
        static float s_MoveSpeed = 7.0f;

        // 무지하게 빨리 지나가면서
        const float SPEED_PER_SECOND = s_MoveSpeed * SPEED_MULTIPLIER;
        ProcessMove( pCharacter, SPEED_PER_SECOND, dwTick);

        // 대상의 위치에 도착
        //if( pCharacter->GetPathExplorer()->IsMoving() )
        //{
        //    m_IsMove = FALSE;
        //}
    }

    if( Result == STATEPROCESS::END )
    {
        m_IsMove = FALSE;
        pCharacter->StopMove();

    }
         

    return Result;
}


