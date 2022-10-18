#include "SunClientPrecompiledHeader.h"
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateFlyingCharging.h"
#include "Character.h"
#include <../../ProgramCommon/SkillInfoParser.h>


SkillStateFlyingCharging::SkillStateFlyingCharging()
{
    m_IsMove = FALSE;
    m_IsDamaged = FALSE;
    m_IsStarted = FALSE;
}

SkillStateFlyingCharging::~SkillStateFlyingCharging()
{

}

STATEPROCESS::TYPE SkillStateFlyingCharging::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    EnterProcess(pOwner,CurTime,dwTick);

    Character* pCharacter = (Character*)pOwner;
    Character* pTarget = (Character*)pCharacter->GetTargetObject();

    if( pTarget == NULL )
        return STATEPROCESS::END;

    // 이벤트체크
    CheckEvent();

    static float s_rate1 = 15.0f;
    static float s_rate2 = 80.f;

    if( m_IsStarted == FALSE && GetAnimationRate() >= s_rate1 )
    {
        WzVector CurrentPosition = pCharacter->GetVisiblePos();
        WzVector TargetPosition = pTarget->GetVisiblePos();

        pCharacter->Move( CurrentPosition, TargetPosition, CMS_RUN, TRUE );

        WzVector Distance = TargetPosition - CurrentPosition;
        m_fRange = VectorLength(&Distance);

        m_IsStarted = TRUE;
        m_IsMove = TRUE;
    }

    if( GetAnimationRate() >= s_rate1 && GetAnimationRate() < s_rate2 )
    {
        if( m_IsMove == TRUE )
        {
            static float s_time_multiplier = 5.0f;

            ProcessMove( pCharacter, m_fRange * SPEED_MULTIPLIER * s_time_multiplier, dwTick);    

            // 대상의 위치에 도착
            if( FALSE == pCharacter->GetPathExplorer()->IsMoving() )
            {
                m_IsMove = FALSE;
            }
        }
    }

    if( GetAnimationRate() >= s_rate2 && m_IsDamaged == FALSE )
    {
        // 데미지 표시
        pCharacter->ProcessAttackResult_Group( pCharacter->GetCurrentAttackSerial() );

        m_IsDamaged = TRUE;
    }

    return LeaveProcess(pOwner,CurTime,dwTick);
}


