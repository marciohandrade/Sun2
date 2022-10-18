#include "SunClientPrecompiledHeader.h"
#include "StateBase.h"
#include "SkillState.h"
#include "SkillStateMystic.h"

#include "Character.h"
#include "Hero.h"
#include "GlobalFunc.h"
#include "SkillInfoParser.h"
#include "PublicMath.h"


//------------------------------------------------------------------------------
/**
*/
SkillStateMystic::SkillStateMystic()
{
}

//------------------------------------------------------------------------------
/**
	부모 Onstart 호출안한다.
*/
STATECHANGE::TYPE SkillStateMystic::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;

    WzVector StartPosition;
    WzVector TargetPosition;

    StartPosition = pCharacter->GetPosition();

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
// 목표점까지 이동해서 한대 때리는 기술이다.
STATEPROCESS::TYPE SkillStateMystic::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE Result = StateSkill::OnProcess( pOwner, CurTime, dwTick );

	Character*	pCharacter = (Character*)pOwner;

	SkillScriptInfo* pSkillInfo = GetSkillInfo();

	if( pSkillInfo != NULL )
	{
	    float MoveDelay = 0.0f;
        if( pSkillInfo->m_SkillClassCode == eSKILL_MYSTIC_901 )
        {
            static float s_SmashDelay = 50.0f;
            MoveDelay = s_SmashDelay;
        }
		else if( pSkillInfo->m_SkillClassCode == eSKILL_MYSTIC_906 )
		{
			static float s_SmashDelay = 15.0f;
			MoveDelay = s_SmashDelay;
		}
		else if( pSkillInfo->m_SkillClassCode == eSKILL_MYSTIC_914 )
		{
			static float s_SmashDelay = 25.0f;
			MoveDelay = s_SmashDelay;
		}

		if( MoveDelay <= GetAnimationRate() )
		{
			float MoveSpeed = 15.0f;
			// 목표점까지 이동
			ProcessMove(pCharacter, MoveSpeed * SPEED_MULTIPLIER,dwTick);
		}

	}
	return Result;
}


/////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/**
*/
SkillStateMysticSmash::SkillStateMysticSmash()
{
}

//------------------------------------------------------------------------------
/**
부모 Onstart 호출안한다.
*/
STATECHANGE::TYPE SkillStateMysticSmash::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;

	WzVector StartPosition = pCharacter->GetPosition();

	// 중심위치를 구함
	WzVector BasePosition = pCharacter->GetPosition() * 2.0f;

	if( Character* pTarget = (Character *)pCharacter->GetTargetObject() )
	{
		BasePosition = pTarget->GetPosition();
	}

	// 방향을 구함
	WzVector Direction = pCharacter->GetDirection();
	Direction = BasePosition - StartPosition;
	VectorNormalize(&Direction, &Direction );


	static float s_Offset = 1.0f;

	// 2개의 위치를 먼저 구한다
	WzVector NewDirection;
	Math_ZRotateVector( &Direction, NewDirection, -135 );
	m_TargetPosition1 = BasePosition + NewDirection * s_Offset;
	Math_ZRotateVector( &Direction, NewDirection, -215 );
	m_TargetPosition2 = BasePosition + NewDirection * s_Offset;

	// 첫번째의 위치로 이동 시작
	pCharacter->SetDirection( m_TargetPosition1, pCharacter->GetPosition() );
	pCharacter->Move( StartPosition, m_TargetPosition1, CMS_RUN, TRUE, -1, -1 );

	if( g_pHero->GetObjectKey() == pCharacter->GetObjectKey() )
	{
		SendSkillPacket();
	}
	ProcessStartAttackSerial(pCharacter);
	ProcessStartAnimation(pCharacter);
	ProcessStartSkillEffect(pCharacter);

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
// 목표점까지 이동해서 한대 때리는 기술이다.
STATEPROCESS::TYPE SkillStateMysticSmash::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE Result = StateSkill::OnProcess( pOwner, CurTime, dwTick );

	Character*	pCharacter = (Character*)pOwner;

	// 첫번째 위치로 이동후 멈추면 2번째 위치로 이동
	if( pCharacter->IsMove() == FALSE )
	{
		if( VectorLength( &( pCharacter->GetPosition() - m_TargetPosition1 ) ) < 0.5f )
		{
			pCharacter->SetDirection( m_TargetPosition2, pCharacter->GetPosition() );
            pCharacter->Move( pCharacter->GetPosition(), m_TargetPosition2, CMS_RUN, TRUE, -1, -1 );
		}
	}

	if( pCharacter->IsMove() == TRUE )
	{
		static float MoveSpeed = 15.0f;
		ProcessMove(pCharacter, MoveSpeed * SPEED_MULTIPLIER,dwTick);
	}

	return Result;
}


STATECHANGE::TYPE SkillStateMystyBlink::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character*)pOwner;

	WzVector StartPosition = pCharacter->GetPosition();
	WzVector TargetPosition = pCharacter->GetPosition();

	// 지정한 위치로 이동
	if( PLAYER_ACTION* pAction = pCharacter->GetCurrentAction() )
	{
		if( pAction->ActionID == ACTION_SKILL )
		{
			TargetPosition = pAction->SKILL.vTargePos;
		}
	}

	pCharacter->SetDirection( TargetPosition, StartPosition );
	pCharacter->Move( StartPosition, TargetPosition, CMS_RUN, TRUE, -1, -1 );

	if( g_pHero->GetObjectKey() == pCharacter->GetObjectKey() )
	{
		SendSkillPacket();
	}
	ProcessStartAttackSerial(pCharacter);
	ProcessStartAnimation(pCharacter);
	ProcessStartSkillEffect(pCharacter);

	return STATECHANGE::SUCCESS;
}

STATEPROCESS::TYPE SkillStateMystyBlink::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	STATEPROCESS::TYPE Result = StateSkill::OnProcess( pOwner, CurTime, dwTick );


	Character* pCharacter = (Character*)pOwner;

	if( pCharacter->IsMove() == TRUE )
	{
		static float MoveSpeed = 50.0f;
		ProcessMove(pCharacter, MoveSpeed * SPEED_MULTIPLIER, dwTick);
	}

	return Result;
}


