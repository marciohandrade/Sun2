//------------------------------------------------------------------------------
//  SkillStateMonsterTackle.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "skillstateMonstertackle.h"
#include "Character.h"
#include "Hero.h"
#include "GameFunc.h"
#include "ProjectileManagerEx.h"
#include "Clone.h"
#include "CloneManager.h"
#include "BattleScene.h"
#include "MissionScene.h"
#include "ObjectManager.h"
#include "ProjectileManagerEx.h"
#include "NumericValues.h"
#include "GlobalFunc.h"

#define INT_DUST_TIME 50

#define INT_CLONE_TIME 700

//------------------------------------------------------------------------------
// �� ��ų�� �÷��̾� ��ų�� �ƴϰ�, Ÿ���� ����� npc�� ����ϵ��� �����Ǿ��ִµ��ѵ� (���°͵� ������)
// �ϴ� ����
/**
*/
STATECHANGE::TYPE SkillStateMonsterTackle::OnStart(Object* pOwner, DWORD CurTime)
{
    m_dwDustTime = INT_DUST_TIME;
    m_dwCopyTime = INT_DUST_TIME;
    m_bCreateEffect = FALSE;

    Character* pCharacter = (Character*)pOwner;


    WzVector StartPosition = pCharacter->GetPosition();
    WzVector TargetPosition = GetTargetNearPosition( StartPosition, (Character*)pCharacter->GetTargetObject(), 1.5f );

    pCharacter->Move( StartPosition, TargetPosition, CMS_RUN, TRUE, -1, -1 );

    return StateSkill::OnStart( pOwner, CurTime );
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateMonsterTackle::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    EnterProcess(pOwner,CurTime,dwTick);

    Character* pCharacter = (Character*)pOwner;

    //m_dwCurTime += dwTick;

    //Todo:��ų ó�� 

    ProcessMove( pCharacter, 12.0f * SPEED_MULTIPLIER, dwTick);

    CheckEvent();


    if( pCharacter->IsMove() == FALSE )
    {
        if (m_iHitProcessed == 0 && pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial()))
        {
            if( m_bCreateEffect == FALSE )
            {
                m_bCreateEffect = TRUE;

                ++m_iHitProcessed;


                cTargetPosition Position;
                Position.SetTargetVisiblePosition( pCharacter->GetObjectKey(), TRUE );
                Position.SetSyncTargetAngle( TRUE );

                g_ProjectileManagerEx.CreateEffect( pCharacter->GetObjectKey(), StrToWzID("F010"), 2000, Position );
            }
        }
    }

    if( (DWORD)m_dwDustTime < dwTick ) 
    {
        WzVector vPos = pCharacter->GetVisiblePos();

        GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A030"),vPos,pCharacter->GetObjectKey());	

        m_dwDustTime=INT_DUST_TIME;
    }
    else
    {
        m_dwDustTime-=dwTick;
    }

    if( m_dwCopyTime < dwTick ) 
    {
        m_dwCopyTime=INT_DUST_TIME;
        DWORD key = g_CloneManager.CreateClone(pCharacter);	
        if (Clone* pClone = g_CloneManager.GetClone(key) )
        {
            pClone->SetAlphaBlend(AB_ALPHA);
            pClone->SetColor(WzColor_RGBA(64,64,64,128));
            pClone->SetPlayAni(TRUE);
            pClone->SetDisappear(128);//���� �ȵȴ�
            pClone->SetLifeTime(228);

        }
    }
    else
    {
        m_dwCopyTime-=dwTick;
    }

    return LeaveProcess( pOwner, CurTime, dwTick );

    //// �̵��� �ٳ�������
    //if( FALSE == pCharacter->GetPathExplorer()->IsMoving() )
    //{		
    //	// ���� �ൿ��!
    //	if (pCharacter->ProcessNextAction())
    //	{		
    //		return STATEPROCESS::SUSPEND;		
    //	}
    //	else
    //	{
    //		return STATEPROCESS::END;		
    //	}
    //}

    //return STATEPROCESS::HOLD;
}