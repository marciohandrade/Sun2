//------------------------------------------------------------------------------
//  SkillStateHide.cpp
//  (C) 2008 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "SkillState.h"
#include "SkillInfoParser.h"
#include "skillstaterush.h"
#include "CooltimeManager.h"
#include "Character.h"

//------------------------------------------------------------------------------
/**
*/
SkillStateRush::SkillStateRush()
{

}
//------------------------------------------------------------------------------
/**
*/
SkillStateRush::~SkillStateRush(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL SkillStateRush::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	return TRUE;
}

//------------------------------------------------------------------------------
/** 러쉬는 ANI 재생금지 
*/
STATECHANGE::TYPE SkillStateRush::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pCharacter = (Character *)pOwner;
	if( pCharacter != NULL )
	{
		if( pCharacter->IsMyHero() == TRUE )
		{
			if( SendSkillPacket() == FALSE )
			{
				assert(!"스킬사용 패킷 전송실패");
				return STATECHANGE::FAILED;
			}

			// 오프라인 처리
			if( GENERALPARAM->IsNet() == FALSE )
			{
                if( util::Timer* pTimer = xGetCooltimeManager()->GetSkillCooltime( m_dwSkillCode ) )
				{
					pTimer->Reset();
				}
			}
		}

		ProcessStartAttackSerial(pCharacter);
		//ProcessStartAnimation(pCharacter);
		ProcessStartDirection(pCharacter);
		ProcessStartSkillEffect(pCharacter);

		return STATECHANGE::SUCCESS;
	}

	return STATECHANGE::FAILED;

}
//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE SkillStateRush::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{

	EnterProcess(pOwner,CurTime,dwTick);

	return STATEPROCESS::END;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE SkillStateRush::OnEnd(Object* pOwner, DWORD CurTime)
{
	return STATECHANGE::SUCCESS;
}





