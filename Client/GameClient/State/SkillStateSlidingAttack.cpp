#include "SunClientPrecompiledHeader.h"
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateSlidingAttack.h"
#include "Character.h"
#include <../../ProgramCommon/SkillInfoParser.h>

#include "Hero.h"
#include "Camera.h"


SkillStateSlidingAttack::SkillStateSlidingAttack()
: m_bStartMove( FALSE )
, m_bHit(FALSE)
{
}

SkillStateSlidingAttack::~SkillStateSlidingAttack()
{

}

STATECHANGE::TYPE SkillStateSlidingAttack::OnStart(Object* pOwner, DWORD CurTime)
{
    Character* pCharacter = (Character*)pOwner;

    WzVector StartPosition = pCharacter->GetVisiblePos();
    WzVector TargetPosition;

    if( Character* pTarget = (Character *)pCharacter->GetTargetObject() )
    {
        TargetPosition = pTarget->GetPosition();
    }
    else
    {
        TargetPosition = StartPosition;
    }

    {
        pCharacter->SetDirection( TargetPosition );

        WzVector vDistance = TargetPosition - StartPosition;
        float fDistance = VectorLength(&vDistance);

        if( fDistance < 1.3f )
        {
            TargetPosition = StartPosition;
        }
        else
        {
            WzVector Direction;
            VectorNormalize(&Direction,&vDistance);

            TargetPosition = TargetPosition + (Direction * -1.2f);
        }

        // StartSkillMove
        pCharacter->Move( StartPosition, TargetPosition, CMS_RUN, TRUE, -1, -1 );

        m_bHit = FALSE;
    }

    return StateSkill::OnStart( pOwner, CurTime );
}


STATEPROCESS::TYPE SkillStateSlidingAttack::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    EnterProcess(pOwner,CurTime,dwTick);

    Character* pCharacter = (Character*)pOwner;

    // �̺�Ʈüũ
    CheckEvent();

    if( GetAnimationRate() >=  0.05f )
    {
        if( m_bHit = FALSE )
        {
            if( pCharacter->GetPathExplorer()->IsMoving() == FALSE )
            {

                // ������ ǥ��
                pCharacter->ProcessAttackResult_Group( pCharacter->GetCurrentAttackSerial() );

                if( pCharacter == g_pHero )
                {
                    // ī�޶� ��¦ ���� �ش�
                    g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
                    g_Camera.StartVibration(100,0.2f);
                }

                m_bHit = TRUE;
            }
        }

        if( m_bHit == FALSE )
        {
            // �����ϰ� ���� �������鼭
            const float SPEED_PER_SECOND = 20.0f * SPEED_MULTIPLIER;
            ProcessMove( pCharacter, SPEED_PER_SECOND, dwTick);
        }
    }

    return LeaveProcess(pOwner,CurTime,dwTick);
}