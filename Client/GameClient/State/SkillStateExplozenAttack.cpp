//------------------------------------------------------------------------------
//  SkillStateExplozenAttack.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateExplozenAttack.h"
#include "Character.h"
#include "Clone.h"
#include "CloneManager.h"
#include "Camera.h"
#include "Hero.h"
#include "SkillInfoParser.h"

#define UP_TIME 300
#define DOWN_TIME 167
#define DEFAULT_HEIGHT 2.0
#define JUMP_MIN   2.0
#define JUMP_MAX   4.0

//------------------------------------------------------------------------------
/**
*/
SkillStateExplozenAttack::SkillStateExplozenAttack()
{

}

//------------------------------------------------------------------------------
/**
*/
SkillStateExplozenAttack::~SkillStateExplozenAttack()
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateExplozenAttack::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/**
	�θ� Onstart ȣ����Ѵ�.
*/
STATECHANGE::TYPE SkillStateExplozenAttack::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;

	m_fHeight = 0.0f;
	m_fRate = 1.0f;


	WzVector StartPosition = pCharacter->GetVisiblePos();
	WzVector TargetPosition;

	if( Character* pTarget = (Character*)pCharacter->GetTargetObject() )
	{
		WzVector Direction = pTarget->GetVisiblePos() - StartPosition;
		VectorNormalize(&Direction, &Direction);

		TargetPosition = pTarget->GetVisiblePos() + Direction * 1.5f;

		m_fHeight = pTarget->GetMeshHeight();
		m_fHeight = float( max(m_fHeight,JUMP_MIN) );
		m_fHeight = float( min(m_fHeight,JUMP_MAX) );
		m_fRate = float( m_fHeight/DEFAULT_HEIGHT );

	}
	else
	{
		TargetPosition = StartPosition;
	}

	pCharacter->Move( StartPosition, TargetPosition, CMS_RUN, TRUE, -1, -1 );


	m_vStartpos = StartPosition;
	m_vTargetpos = TargetPosition;

	m_vStartpos.z += m_fHeight;

	m_dwCopyTime = 80;

	m_dwCurTime = 0;

	m_bFrontMove = FALSE;
	m_bUpMove = FALSE;
	m_bDownMove = FALSE;
	m_dwCurTime = 0;

	return StateSkill::OnStart( pOwner, CurTime );
}

//------------------------------------------------------------------------------
/**
*/

// ��ǥ������ �̵��ؼ� �Ѵ� ������ ����̴�.
STATEPROCESS::TYPE SkillStateExplozenAttack::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    EnterProcess(pOwner,CurTime,dwTick);

	Character* pCharacter = (Character*)pOwner;

	m_dwCurTime+=dwTick;

	if(m_dwCurTime>=300&&m_iHitProcessed==0)
	{
		m_bUpMove=TRUE;
	}


	// Ÿ���� üũ
	CheckEvent();
	
	if(m_bUpMove)//�ø���.
	{
		float fRate=(float)m_dwCurTime/(float)(UP_TIME*m_fRate);
		fRate=min(fRate,1.0f);
		float temp = m_fHeight * (fRate);
		pCharacter->SetHeight(temp);
	}
	else if(m_bDownMove)//������.
	{
		float fRate=(float)m_dwCurTime/(float)DOWN_TIME;
		fRate=min(fRate,1.0f);
		float temp = m_fHeight* (1.0f - fRate);
		pCharacter->SetHeight(temp);
	}

	if (CheckNotProcessedHitEvent(1))//�ö󰡴°� ������ �ð� 
	{		
		++m_iHitProcessed;
		m_bUpMove=FALSE;
	}

	if (CheckNotProcessedHitEvent(2))//�̵����� �ð� 
	{		
		
		++m_iHitProcessed;
		m_bDownMove=TRUE;
		m_dwCurTime=0;
		m_bFrontMove=TRUE;

		//const PLAYER_ACTION* pAction = pCharacter->GetCurrentAction();
		//pCharacter->Move( pAction->SKILL.vCurPos.wvPos, pAction->SKILL.vDestPos.wvPos, CMS_RUN, TRUE, pAction->SKILL.vCurPos.wTile, pAction->SKILL.vDestPos.wTile);

	}

	if (CheckNotProcessedHitEvent(3))//�̵� ���ð� 
	{		
		++m_iHitProcessed;
		m_bFrontMove=FALSE;

		g_Camera.SetVibeType(Camera::VIBE_TYPE_VERTICAL);
		g_Camera.StartVibration(40,0.03f);		
		
		if (pCharacter->ProcessAttackResult_Group(pCharacter->GetCurrentAttackSerial()))
		{
		}
	}


	//Todo:��ǥ���� �ӵ� �ȿ� ���� �ִ� ���̾�߰ڴ�.

	if(m_bFrontMove)
	{
		WzVector vDest,vTemp;
		float fAngleZ,fAngleX;

		vDest =  m_vTargetpos - m_vStartpos;
		vTemp = vDest;
		vTemp.z = 0.0f;

		float fLength;

		if (vTemp.x == 0.0f && vTemp.y == 0.0f)
		{
			fLength = 0.0f;
		}
		else
		{
			fLength = VectorLength(&vTemp);
		}

		fAngleX = -atan2(vDest.z,fLength);
		fAngleZ = Math_GetAngleFromVector(&vDest);

		float fRate=(float)m_dwCurTime/(float)DOWN_TIME;
		fRate=min(fRate,1.0f);
		float fAngleY = WZ_PI*2 * (fRate);
		pCharacter->SetAngle(fAngleX,fAngleY,fAngleZ);//��ǥ�������� ���� ��ũ�� 

		// ��ǥ������ �̵�
		ProcessMove(pCharacter,100.0f * SPEED_MULTIPLIER,dwTick);

		if( m_dwCopyTime < dwTick ) 
		{
			m_dwCopyTime=30;

			DWORD key = g_CloneManager.CreateClone(pCharacter);	
			if ( Clone* pClone = g_CloneManager.GetClone(key) )
			{
				pClone->SetColor(WzColor_RGBA(255,255,255,255));
				pClone->SetPlayAni(TRUE);
				pClone->SetDisappear(400);//���� �ȵȴ�
				pClone->SetLifeTime(400);

			}
		}
		else
		{
			m_dwCopyTime-=dwTick;
		}

	}

	
    return LeaveProcess(pOwner,CurTime,dwTick);
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateExplozenAttack::OnEnd(Object* pOwner, DWORD CurTime)
{
    //Todo:Ưȭ�� ó���� �Ѵ�.
	Character*	pCharacter = (Character*)pOwner;
	pCharacter->SetHeight(0.0f);

    return StateSkill::OnEnd(pOwner,CurTime);
}