//------------------------------------------------------------------------------
//  SkillStateTackle.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"


#include "statebase.h"
#include "SkillState.h"
#include "SkillStateTackle.h"
#include "Hero.h"
#include "Character.h"
#include "Clone.h"
#include "CloneManager.h"
#include "GlobalFunc.h"
#include "Camera.h"
#include "SkillEffectFunc.h"

// ��ų DashStun�� ����

SkillStateTackle::SkillStateTackle()
: m_bStartMove( FALSE )
, m_dwCloneKey( 0 )
, m_dwCrushTime( 0 )
{

}

STATECHANGE::TYPE SkillStateTackle::OnStart(Object* pOwner, DWORD CurTime)
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
    }

    return StateSkill::OnStart( pOwner, CurTime );
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateTackle::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    EnterProcess(pOwner,CurTime,dwTick);

    Character* pCharacter = (Character*)pOwner;

    // �̺�Ʈüũ
    CheckEvent();

    if ( CheckNotProcessedHitEvent(1) == TRUE )
    {
        m_bStartMove = TRUE;
        m_dwCrushTime = 0;

        ++m_iHitProcessed;


        // �ܻ�� Ŭ�� ����
        m_dwCloneKey = g_CloneManager.CreateClone(pCharacter);

        if( Clone *pClone = g_CloneManager.GetClone(m_dwCloneKey) )
        {
            pClone->SetPlayAni(FALSE);
            pClone->SetAlphaBlend(AB_ALPHA);
            pClone->SetAnimation( pCharacter->GetCurrentAnimation() );
            pClone->SetColor(WzColor_RGBA(255,255,255,100));		
            pClone->SetLifeTime( 2000 );	
            //			pClone->SetAppear( 1000 );
        }
    }


    if( m_bStartMove == TRUE )
    {
        // �����ϰ� ���� �������鼭
        const float SPEED_PER_SECOND = 20.0f * SPEED_MULTIPLIER;
        ProcessMove( pCharacter, SPEED_PER_SECOND, dwTick);
    }

    if( m_bStartMove == TRUE )
    {
        if( m_dwCloneKey != 0 )
        {
            // Ŭ���� ������ ��ġ�� �ִϸ��̼� �������� ���� �����ش�.
            if( Clone *pClone = g_CloneManager.GetClone(m_dwCloneKey) )
            {
                WzVector wvPos = pCharacter->GetPosition() - (pCharacter->GetDirection() * 0.3f);

                pClone->SetPosition( wvPos );
                pClone->SetCurrentFrame( pCharacter->GetCurrentFrame() );

            }
        }

        // ����� ��ġ�� ����
        if( FALSE == pCharacter->GetPathExplorer()->IsMoving() )
        {

            if( m_dwCrushTime == 0 )
            {
                // ���� �浹�� ��Ʈ ����Ʈ
                _AddHitEffect(pCharacter);

                // ������ ǥ��
                pCharacter->ProcessAttackResult_Group( pCharacter->GetCurrentAttackSerial() );


                if( pCharacter == g_pHero )
                {
                    // ī�޶� ��¦ ���� �ش�
                    g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
                    g_Camera.StartVibration(100,0.2f);
                }

                m_dwCrushTime = CurTime;
            }

            // �浹 �� 0.15�� ���� ���� �Ŀ�
            if( m_dwCrushTime + 150 < CurTime )
            {
                if( m_dwCloneKey != 0 )
                {
                    g_CloneManager.Delete( m_dwCloneKey );
                    m_dwCloneKey = 0;
                }


                // ���¸� �����Ѵ�.
                if (pCharacter->ProcessNextAction())
                {		
                    return STATEPROCESS::SUSPEND;		
                }
                else
                {
                    return STATEPROCESS::END;		
                }

                return STATEPROCESS::END;		
            }
        }
    }

    return LeaveProcess(pOwner,CurTime,dwTick);
}


void SkillStateTackle::_AddHitEffect( Character* pCharacter )
{
    // Ÿ�� ����Ʈ
    if( Object* pObject = pCharacter->GetTargetObject() )
    {
        if( pObject->IsKindOfObject(CHARACTER_OBJECT) )
        {
            Character* pTarget = (Character*)pObject;

            HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption( StrToWzID("K956"), pCharacter->GetPosition(), pCharacter->GetScale(), pCharacter->GetObjectKey(), pCharacter->GetTargetID() );
            if( hHandle != INVALID_HANDLE_VALUE )
            {
                if( CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(hHandle) )
                {
                    WzVector vRot;

                    vRot.y = 0.0f;
                    vRot.z = pCharacter->GetAngle(); 
                    vRot.x = pCharacter->GetAngleX();

                    WzVector vScale;

                    vScale.x = 1.0f;
                    vScale.y = 1.0f;
                    vScale.z = 1.0f;

                    pBody->SetResultMatrix( pCharacter->GetVisiblePos(), vRot, vScale);
                }
            }
        }
    }
}