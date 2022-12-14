#include "SunClientPrecompiledHeader.h"
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateDrillAttack.h"
#include "Character.h"
#include <../../ProgramCommon/SkillInfoParser.h>


SkillStateDrillAttack::SkillStateDrillAttack()
{
    m_IsMove = FALSE;
    m_IsDamaged = FALSE;

}

SkillStateDrillAttack::~SkillStateDrillAttack()
{

}

STATEPROCESS::TYPE SkillStateDrillAttack::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    EnterProcess(pOwner,CurTime,dwTick);

    Character* pCharacter = (Character*)pOwner;
    Character* pTarget = (Character*)pCharacter->GetTargetObject();

    if( pTarget == NULL )
        return STATEPROCESS::END;

    // 이벤트체크
    CheckEvent();

    if ( CheckNotProcessedHitEvent(1) == TRUE )
    {
        ++m_iHitProcessed;

        SkillScriptInfo* pSkillInfo = GetSkillInfo();

        WzVector CurrentPosition = pCharacter->GetVisiblePos();
        WzVector Direction = pTarget->GetVisiblePos() - CurrentPosition;
        VectorNormalize(&Direction,&Direction);

        WzVector TargetPosition = CurrentPosition + Direction * ((float)pSkillInfo->m_wSkillRange / 10.f);

        pCharacter->Move( CurrentPosition, TargetPosition, CMS_RUN, TRUE );
        m_IsMove = TRUE;
    }

    if( m_IsMove == TRUE )
    {
        // 무지하게 빨리 지나가면서
        const float SPEED_PER_SECOND = 25.0f * SPEED_MULTIPLIER;
        ProcessMove( pCharacter, SPEED_PER_SECOND, dwTick);

        if( m_IsDamaged == FALSE )
        {
            WzVector Distance = pCharacter->GetVisiblePos() - pTarget->GetVisiblePos();
            if( VectorLength(&Distance) < 1.0f )
            {
                // 데미지 표시
                pCharacter->ProcessAttackResult_Group( pCharacter->GetCurrentAttackSerial() );
                m_IsDamaged = TRUE;
            }
        }

        // 대상의 위치에 도착
        if( FALSE == pCharacter->GetPathExplorer()->IsMoving() )
        {
            m_IsMove = FALSE;
        }
    }

    return LeaveProcess(pOwner,CurTime,dwTick);
}


