//------------------------------------------------------------------------------
//  SkillStateBerserkMode.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillStateBerserkMode.h"
#include "Character.h"
#include "Clone.h"
#include "CloneManager.h"


#define  SCALE_TIME_SKILL       1000
#define  SPEED_SCALE_SKILL      0.0f
#define  ACELL_UP_SKILL         0.00004f
#define  ACELL_DOWN_SKILL      -0.00001f
#define  ACELL_INIT_SKILL       0.0000001f
#define  SCALE_INIT_SKILL       0.0f 
#define  SCALE_MAX_SKILL        1.3f


//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateBerserkMode::OnStart(Object* pOwner, DWORD CurTime)
{
	STATECHANGE::TYPE Result = StateSkill::OnStart(pOwner,CurTime);

	m_dwCloneLifeTime = SCALE_TIME_SKILL;
	m_speedScale = SPEED_SCALE_SKILL;
	m_accelUp = ACELL_UP_SKILL;			
	m_accelDown = ACELL_DOWN_SKILL;		
	m_accel = ACELL_INIT_SKILL;
	m_CurScale = SCALE_INIT_SKILL;

	m_dwCloneKey = g_CloneManager.CreateClone(pOwner);
    if ( Clone * pClone = g_CloneManager.GetClone(m_dwCloneKey) )
    {
        pClone->SetAlphaBlend(AB_LIGHTING);
        pClone->SetColor(WzColor_RGBA(255,255,255,255));
        pClone->SetLifeTime(m_dwCloneLifeTime);
        pClone->SetPlayAni(TRUE);
        pClone->SetDisappear(m_dwCloneLifeTime);//알파 안된다
    }
	
	return Result;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateBerserkMode::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
    EnterProcess(pOwner,CurTime,dwTick);


	Character* pCharacter = (Character*)pOwner;

    if( m_dwCloneLifeTime < dwTick ) 
    {
        m_dwCloneLifeTime=0;
    }
    else
    {

        if ( Clone *pClone = g_CloneManager.GetClone(m_dwCloneKey) )
        {

            // 남은 시간이 2분의 1보다 클때 
            if(m_dwCloneLifeTime>SCALE_TIME_SKILL/2)
                m_accel += m_accelUp;
            else
                m_accel += m_accelDown;

            // 스피드 계산
            float speedplus = m_accel * dwTick;
            m_speedScale += speedplus;

            // 스케일 수정
            float scaleplus = m_speedScale * dwTick;
            m_CurScale += scaleplus;
            if(m_CurScale>=1.00f)
                m_CurScale=1.00f;

            pClone->SetScale(m_CurScale);
        }

        m_dwCloneLifeTime -= dwTick;
    }

    return LeaveProcess(pOwner,CurTime,dwTick);
}
