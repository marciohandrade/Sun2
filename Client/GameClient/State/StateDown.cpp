//------------------------------------------------------------------------------
//  StateDown.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "statebase.h"
#include "StateDown.h"
#include "Character.h"
#include "NumericValues.h"

//------------------------------------------------------------------------------
/**
*/
StateDown::StateDown(void)
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
: state_type_(eCHAR_STATE_INVALID)
#endif // _NA_008472_20150903_IMPROVED_BALANCE
{
}

//------------------------------------------------------------------------------
/**
*/
StateDown::~StateDown(void)
{
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateDown::CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState)
{
	switch(pToTransiteState->GetStateKind())
	{
	case 0:
		break;
	}
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateDown::OnStart(Object* pOwner, DWORD CurTime)
{
	Character* pChr = (Character*)pOwner;

	switch( pChr->GetWeaponAnimationCode() )
	{
	case '0':	pChr->SetAnimation( "F001", FALSE); break;
	case '1':	pChr->SetAnimation( "F101", FALSE); break;
	case '2':	pChr->SetAnimation( "F201", FALSE); break;
	case '3':	pChr->SetAnimation( "F301", FALSE); break;
	}

	pChr->m_DownAnimationState = Character::DOWN_ANIMATION_STATE_START;

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	state_type_ = eCHAR_STATE_INVALID;
	if ( SKILL_EFFECT* skill_effect = pChr->FindSkillEffect(eCHAR_STATE_DOWN) )
	{
		state_type_ = eCHAR_STATE_DOWN;
	}
	else
	{
		// 그 외의 type은 eCHAR_STATE_INVALID로 넣는다.
		// eCHAR_STATE_DOWN판별을 위한 것이다.
		state_type_ = eCHAR_STATE_INVALID;
	}
#endif // _NA_008472_20150903_IMPROVED_BALANCE

	return STATECHANGE::SUCCESS;
}

//------------------------------------------------------------------------------
/**
*/
STATEPROCESS::TYPE StateDown::OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick)
{
	Character* pChr = (Character*)pOwner;

	float fMoveSpeed = GetBaseMoveSpeedAsState(CMS_KNOCKBACK_DOWN) * (float)dwTick;
    g_pSunTerrain->PathExp_Progress(pChr->GetPathExplorer(), fMoveSpeed, NULL);
	pChr->ProcessInterpolation(dwTick);

	float fRet = pChr->ProcessAnimation(dwTick);

	if (pChr->IsDead())
	{
		pChr->SetNextState(STATE::DEATH,g_CurTime);
		return STATEPROCESS::SUSPEND;		
	}

	if( pChr->m_DownAnimationState == Character::DOWN_ANIMATION_STATE_START && fRet == 100.0f )
	{
		switch( pChr->GetWeaponAnimationCode() )
		{
		case '0':	pChr->SetAnimation( "F002", TRUE); break;
		case '1':	pChr->SetAnimation( "F102", TRUE); break;
		case '2':	pChr->SetAnimation( "F202", TRUE); break;
		case '3':	pChr->SetAnimation( "F302", TRUE); break;
		}
		pChr->m_DownAnimationState = Character::DOWN_ANIMATION_STATE_DOWN;
	}
	else if( pChr->m_DownAnimationState == Character::DOWN_ANIMATION_STATE_DOWN && CurTime >= pChr->m_DownTime )
	{
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		if ( state_type_ == eCHAR_STATE_DOWN )
		{
			if ( pChr->FindSkillEffect(eCHAR_STATE_DOWN) == NULL )
			{
				return STATEPROCESS::END;
			}
		}
		else
		{
			switch( pChr->GetWeaponAnimationCode() )
			{
			case '0':	pChr->SetAnimation( "F003", FALSE); break;
			case '1':	pChr->SetAnimation( "F103", FALSE); break;
			case '2':	pChr->SetAnimation( "F203", FALSE); break;
			case '3':	pChr->SetAnimation( "F303", FALSE); break;
			}
			pChr->m_DownAnimationState = Character::DOWN_ANIMATION_STATE_GETUP;
		}
#else
		switch( pChr->GetWeaponAnimationCode() )
		{
		case '0':	pChr->SetAnimation( "F003", FALSE); break;
		case '1':	pChr->SetAnimation( "F103", FALSE); break;
		case '2':	pChr->SetAnimation( "F203", FALSE); break;
		case '3':	pChr->SetAnimation( "F303", FALSE); break;
		}
		pChr->m_DownAnimationState = Character::DOWN_ANIMATION_STATE_GETUP;
#endif // _NA_008472_20150903_IMPROVED_BALANCE
	}
	else if( pChr->m_DownAnimationState == Character::DOWN_ANIMATION_STATE_GETUP && fRet == 100.0f )
	{
		return STATEPROCESS::END;
	}

	return STATEPROCESS::HOLD;
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateDown::OnEnd(Object* pOwner, DWORD CurTime)
{
	Character* pChr = (Character*)pOwner;
	pChr->m_DownAnimationState = Character::DOWN_ANIMATION_STATE_NONE;

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	state_type_ = eCHAR_STATE_INVALID;
#endif // _NA_008472_20150903_IMPROVED_BALANCE
	
	return STATECHANGE::SUCCESS;
}
