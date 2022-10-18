//------------------------------------------------------------------------------
//  StateOwner.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "StateController.h"
#include "Object.h"

#include "State/StateBase.h"
#include "State/StateIdle.h"
#include "State/StateSpawn.h"
#include "State/StateMove.h"
#include "State/StateKeyboardMove.h"
#include "State/StateAttack.h"
#include "State/StateJump.h"
#include "State/StateDeath.h"
#include "State/StateAnimation.h"
#include "State/StateDamage.h"
#include "State/StateAir.h"
#include "State/StateDown.h"
#include "State/StateGetup.h"
#include "State/StateEventJump.h"
#include "State/StateSit.h"
#include "State/StateSpecialMove.h"
#include "State/StateEmoticon.h"
#include "State/StateAttackJump.h"
#include "State/StateRunAway.h"
#include "State/StateKeyboardJump.h"

#ifdef _YJ_FISHING_SYSTEM_000933
#include "State/StateFishing.h"
#endif //_YJ_FISHING_SYSTEM_000933
#include "State/StateKnockBack2.h"
#include "State/StateFireCracker.h"
#include "State/StateCollect.h"
#include "State/StateSummonRider.h"
#include "State/StateRiderTurn.h"
#include "State/StateEtheriaExtract.h"
#include "State/StateTriggerAnimationControl.h"

#include "State/StateRevive.h"
#include "State/StateTransForm.h"
#include "State/StateEarthQuake.h"
#include "State/StateFallDown.h"
#include "State/StateAirFallDown.h"

#include "State/StateNPCSpecialAction_Loop.h"
#include "State/StateMove.h"
#include "State/stateIdle.h"
#include "State/statenpcswing.h"
#include "State/statenpcresponse.h"
#include "State/stateNPCIdle.h"
#include "State/StateSpecialAction.h"

#include "State/StateMove.h"
#include "State/stateIdle.h"
#include "State/StatePetEatAction.h"
#include "State/StatePetIdle.h"
#include "State/StateMapIdle.h"
#include "State/StateTrigger.h"
#include "State/StateEarthQuake.h"
#include "State/StateMapIdle.h"
#include "state/StateTrigger.h"
#include "State/StateItemDrop.h"
#include "State/StateHeroMove.h"
#include "State/StateHeroAttack.h"
#include "State/StateWingsSpread.h"
#include "State/StateWingsFurl.h"
#include "State/StateRiderSpread.h"
#include "State/StateRiderFurl.h"

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "State/StateStatueStand.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

#ifdef _KST_DISPLAY_STATE_CHANGE_LOG
#include "Hero.h"
#include "GlobalFunc.h"
static char s_StateName[][32] = {
        //"NONE = -1",		///<NextState�� ����ִٴ� �� ǥ���ϴ� ����.CurState�� �� ���� �� ���� ����.
        "IDLE",		///<����(������ ���ִ�)�� ����
        "SPAWN",			///<�� ó�� ������� ����
        "MOVE",			///<�̵����� ����
        "KEYBOARDMOVE",	/// Ű���� �̵�
        "ATTACK",			///<���ݽ��� ����", Skill�����Ͽ� �ٲ� ���ɼ� ����
        "JUMP",			///<Jump!
        "DEATH",			///<��������
        "ANIMATION",		///<�̸�Ƽ�� ���ۿ�( ����� ���� Ȯ�� �����ε� ���̰� ����)
        "DAMAGE",			///<������ ����
        "AIR",			///<����
        "DOWN",			///<�ں�����
        "SKILL",			///<��ų
        "GETUP",			///<�Ͼ��
        "EVENTJUMP",
        "SIT",
        "SPECIALMOVE",
        "REVIVE",
        "KEYBOARDJUMP",
        "EMOTICON",
        "ATTACKJUMP",    ///<��������
        "RUNAWAY",       ///<(��Ű����)����ģ��

        //pet
        "PET_EAT_ACTION_STATE",	//Pet�Դ� �ִ�
        "PET_IDLE",				//�� ������

        /// nonPcState
        "TRIGGER_STATE",	///<�ʿ�����Ʈ Ʈ���� ���� 
        "ITEM_DROP_STATE",///<������ ���
        // << npc
        "NPC_IDLE_STATE",		///<NPC�� ����(������ ���ִ�)�� ����
        "NPC_SWING_STATE",	///<NPC�� ���� �ϴ���
        "NPC_RESPONSE_STATE", ///<NPC�� ���	
        "NPC_SPECIAL_ACT_STATE", //NPC�� Ư���� �ൿ(�ൿ ������ Ŭ�󿡼� ���Ѵ�)
        "NPC_SPECIAL_ACT_LOOP_STATE", //NPC�� Ư���� �ൿ(�ൿ ������ Ŭ�󿡼� ���Ѵ�)(����)

        "TRANSFORM",
        "EARTH_QUAKE", //���� ������ 
        "FALL_DOWN",
        "AIR_FALL_DOWN",
#ifdef _YJ_FISHING_SYSTEM_000933
        "FISHING_THROW",
        "FISHING_STAND",
        "FISHING_BITE",
        "FISHING_SUCCESS",
        "FISHING_FAILED",
#endif //_YJ_FISHING_SYSTEM_000933

		"KNOCKBACK",
        "KNOCKBACK2",
		"KNOCKBACK3",
        "FIRECRACKER",

        "COLLECT",

        "SUMMON_RIDER",
        "RIDER_TURN",

        "ETHERIA_EXTRACT",
        "TRIGGER_ANIMATION_CONTROL",

        //! 2011.2.28 / i4u4me / ����
        "WINGS_SPREAD",
        "WINGS_FURL",
        "RIDER_SPREAD",
        "RIDER_FURL",
        "MAX",
};


const char* StateController::GetStateName( STATE::TYPE type )
{
    if( type == STATE::NONE )
    {
        static char s_StateName2[] = "NONE";
        return s_StateName2;
    }
    if( type >= 0 && type <= STATE::MAX )
    {
        return s_StateName[type];
    }

    static char s_StateName3[] = "UNKNOWN";
    return s_StateName3;
}

#endif// _KST_DISPLAY_STATE_CHANGE_LOG




//------------------------------------------------------------------------------
/**
*/
StateController::StateController()
{
	for(int i=0;i<STATE::MAX;i++)
	{
		m_StateArray[i]=NULL;
	}

	m_pNextSkillState = NULL;

	m_eCurState = STATE::NONE;
	m_eNextState = STATE::NONE;
	m_ePreState = STATE::NONE;

	m_CurStateStartTime = 0;
}

//------------------------------------------------------------------------------
/**
*/
StateController::~StateController()
{
	if(m_StateArray[STATE::SKILL])
	{
#ifndef _K2P_SKILL_REFACTORYING
		delete m_StateArray[STATE::SKILL];
#endif
		m_StateArray[STATE::SKILL] = NULL;
	}

	if(m_pNextSkillState)
	{
#ifndef _K2P_SKILL_REFACTORYING
		delete m_pNextSkillState;
#endif
		m_pNextSkillState = NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
void StateController::InitializeState()
{
	Object* pObject = GetObject();
	if( pObject == NULL )
		return;

	eOBJECT_TYPE ObjectType = pObject->GetObjectType();

	switch( ObjectType )
	{
	case CHARACTER_OBJECT:
	case PET_OBJECT:
	case PLAYER_OBJECT:
	case MONSTER_OBJECT:
	case SSQMONSTER_OBJECT:
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	case STATUE_PLAYER_OBJECT:
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		

		m_StateArray[STATE::IDLE]	        = StateIdle::GetInstance();
		m_StateArray[STATE::SPAWN]	        = StateSpawn::GetInstance();
		m_StateArray[STATE::MOVE]	        = StateMove::GetInstance();
		m_StateArray[STATE::KEYBOARDMOVE]	= StateKeyboardMove::GetInstance();
		m_StateArray[STATE::ATTACK]	        = StateAttack::GetInstance();
		m_StateArray[STATE::JUMP]	        = StateJump::GetInstance();
		m_StateArray[STATE::DEATH]	        = StateDeath::GetInstance();
		m_StateArray[STATE::ANIMATION]      = StateAnimation::GetInstance();
		m_StateArray[STATE::DAMAGE]	        = StateDamage::GetInstance();
		m_StateArray[STATE::AIR]	        = StateAir::GetInstance();
		m_StateArray[STATE::DOWN]	        = StateDown::GetInstance();
		m_StateArray[STATE::GETUP]          = StateGetup::GetInstance();
		m_StateArray[STATE::SPECIALMOVE]    = StateSpecialMove::GetInstance();
		m_StateArray[STATE::EVENTJUMP]      = StateEventJump::GetInstance();
		m_StateArray[STATE::SIT]			= StateSit::GetInstance();
		m_StateArray[STATE::EMOTICON]		= StateEmoticon::GetInstance();
		m_StateArray[STATE::KEYBOARDJUMP]   = StateKeyboardJump::GetInstance();
		m_StateArray[STATE::ATTACKJUMP]     = StateAttackJump::GetInstance();
		m_StateArray[STATE::RUNAWAY]        = StateRunAway::GetInstance();
		m_StateArray[STATE::REVIVE]         = StateRevive::GetInstance();
		m_StateArray[STATE::TRANSFORM]		= StateTransForm::GetInstance();
		m_StateArray[STATE::FALL_DOWN]		= StateFallDown::GetInstance();
		m_StateArray[STATE::AIR_FALL_DOWN]	= StateAirFallDown::GetInstance();


#ifdef _YJ_FISHING_SYSTEM_000933
		m_StateArray[STATE::FISHING_THROW]		= StateFishing_Throw::GetInstance();
		m_StateArray[STATE::FISHING_STAND]		= StateFishing_Stand::GetInstance();
		m_StateArray[STATE::FISHING_BITE]		= StateFishing_Bite::GetInstance();
		m_StateArray[STATE::FISHING_SUCCESS]	= StateFishing_Success::GetInstance();
		m_StateArray[STATE::FISHING_FAILED]		= StateFishing_Failed::GetInstance();
#endif //_YJ_FISHING_SYSTEM_000933

		m_StateArray[STATE::KNOCKBACK]			= StateKnockBack::GetInstance();
		m_StateArray[STATE::KNOCKBACK2]			= StateKnockBack2::GetInstance();
		m_StateArray[STATE::KNOCKBACK3]			= StateKnockBack3::GetInstance();
		m_StateArray[STATE::FIRECRACKER]		= StateFireCracker::GetInstance();

		m_StateArray[STATE::COLLECT]			= StateCollect::GetInstance();

		m_StateArray[STATE::SUMMON_RIDER]		= StateSummonRider::GetInstance();
		m_StateArray[STATE::RIDER_TURN]			= StateRiderTurn::GetInstance();

		m_StateArray[STATE::ETHERIA_EXTRACT] = StateEtheriaExtract::GetInstance();

		m_StateArray[STATE::TRIGGER_ANIMATION_CONTROL] = StateTriggerAnimationControl::GetInstance();

        m_StateArray[STATE::WINGS_SPREAD] = StateWingsSpread::GetInstance();
        m_StateArray[STATE::WINGS_FURL] = StateWingsFurl::GetInstance();
        m_StateArray[STATE::RIDER_SPREAD] = StateRiderSpread::GetInstance();
        m_StateArray[STATE::RIDER_FURL] = StateRiderFurl::GetInstance();
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		m_StateArray[STATE::STATUE_STAND]   = StateStatueStand::GetInstance();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

		m_StateArray[STATE::SKILL]          = NULL;
		m_StateArray[STATE::TRIGGER_STATE]  = NULL;
		m_pNextSkillState					= NULL;
		break;


	case NPC_OBJECT:
	case LOTTO_NPC_OBJECT:
		m_StateArray[STATE::IDLE]				= StateIdle::GetInstance();
		m_StateArray[STATE::MOVE]				= StateMove::GetInstance();
		m_StateArray[STATE::NPC_IDLE_STATE]		= StateNPCIdle::GetInstance();
		m_StateArray[STATE::NPC_SWING_STATE]	= StateNPCSwing::GetInstance();
		m_StateArray[STATE::NPC_RESPONSE_STATE] = StateNPCResponse::GetInstance();
		m_StateArray[STATE::NPC_SPECIAL_ACT_STATE] = StateSpecialAction::GetInstance();
		m_StateArray[STATE::NPC_SPECIAL_ACT_LOOP_STATE] = StateNPCSpecialAction_Loop::GetInstance();

		m_pNextSkillState						= NULL;
		break;

	case MAP_OBJECT:
		m_StateArray[STATE::IDLE]			= StateMapIdle::GetInstance();
		m_StateArray[STATE::TRIGGER_STATE]  = StateTrigger::GetInstance();

		// MAP_OBJECT Ÿ���� TerrainObject�� ��쿡�� 
		// TRIGGER_STATE ��� EARTH_QUAKE �� ��������� �Ѵ� �����صд�.
		m_StateArray[STATE::EARTH_QUAKE]  = StateEarthQuake::GetInstance();

		m_pNextSkillState					 = NULL;
		break;

	case ITEM_OBJECT:
		m_StateArray[STATE::ITEM_DROP_STATE] = StateItemDrop::GetInstance();

		m_pNextSkillState					 = NULL;

		break;
	}


	// ���� �߰� ����
	if( ObjectType == PET_OBJECT )
	{
		m_StateArray[STATE::IDLE]					= StateIdle::GetInstance();
		m_StateArray[STATE::MOVE]					= StateMove::GetInstance();
		m_StateArray[STATE::PET_IDLE]				= StatePetIdle::GetInstance();
		m_StateArray[STATE::PET_EAT_ACTION_STATE]	= StatePetEatAction::GetInstance();
	}


}

//------------------------------------------------------------------------------
/**
*/
void StateController::InitializeStateForHero()
{
    // Hero�϶� ����ϴ� ���·� ����
    m_StateArray[STATE::MOVE]	= StateHeroMove::GetInstance();
    m_StateArray[STATE::ATTACK]	= StateHeroAttack::GetInstance();
}

//------------------------------------------------------------------------------
/**
*/
void StateController::InitState( STATE::TYPE initState, DWORD CurTime )
{
	m_eCurState = initState;
	m_eNextState = STATE::NONE;
	m_ePreState  = STATE::NONE;
	m_CurStateStartTime = CurTime;

	GetCurStateInstance()->Initialize( GetObject() );

	STATECHANGE::TYPE startResult = GetCurStateInstance()->OnStart( GetObject(), CurTime );
	assert(startResult == 0);
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateController::SetNextState(STATE::TYPE nextState, DWORD CurTime)
{
	assert(nextState != STATE::NONE);
	assert(nextState < STATE::MAX);


	// ���� ������ �ʿ�.
	if (nextState == STATE::SKILL)
	{
		if((NULL == m_pNextSkillState) || (FALSE == GetCurStateInstance()->CanTransite( GetObject(), CurTime, m_pNextSkillState)))
		{
			return FALSE;;
		}
	}
	else 
	{
		if((NULL == m_StateArray[nextState]) || (FALSE == GetCurStateInstance()->CanTransite( GetObject(), CurTime, m_StateArray[nextState])))
		{
			return FALSE;
		}
	}

	m_eNextState = nextState;
	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
void StateController::StateProcess(DWORD CurTime, DWORD dwTick)
{
	STATEPROCESS::TYPE processResult = STATEPROCESS::END;

	processResult = GetCurStateInstance()->OnProcess( GetObject(), CurTime, dwTick);
	
	//��� ������ 
	if(processResult == STATEPROCESS::HOLD)
		return;

	BOOL bCommitStateChange = FALSE;
	if(processResult == STATEPROCESS::END)//������ �������� 
	{
		if(GetNextState() == STATE::NONE)// ������ ���ٸ� �⺻ ������ �������� 
		{
			m_eNextState = STATE::IDLE;
		}
		
		bCommitStateChange = TRUE;
	}
	else if(processResult == STATEPROCESS::SUSPEND)//���� ���̸� 
	{
		if(GetNextState() != STATE::NONE)
			bCommitStateChange = TRUE;
	}
	else
	{
		assert(0); // to prevent mistake
	}

	//YH2DO	�� ����� ����� ã�ƶ�! -0-
	if(bCommitStateChange)//������ ���ؾ� �ϴ� ���� �̸� 
	{
		STATECHANGE::TYPE commitResult = CommitChangeState(CurTime);//���� �ٲٱ� 
		if(commitResult == STATECHANGE::FAILED)//�����ϸ� �⺻ �������� 
		{
			m_eNextState = STATE::IDLE;
			STATECHANGE::TYPE rt = CommitChangeState(CurTime);
			assert(rt == STATECHANGE::SUCCESS);
		}		
	}
}

//------------------------------------------------------------------------------
/**
*/
STATECHANGE::TYPE StateController::CommitChangeState(DWORD CurTime)
{
    if ( m_eNextState == STATE::NONE )
    {
        return STATECHANGE::FAILED;
    }

	StateBase *pCurState = NULL;
	StateBase *pNewState = NULL;

	pCurState = GetCurStateInstance();

	assert(pCurState);

	// ������ ���¸� �����Ѵ�.
	pCurState->Finalize( GetObject() );
	STATECHANGE::TYPE endResult = pCurState->OnEnd( GetObject(), CurTime );
	assert(endResult == 0);


	if (m_eCurState == STATE::SKILL)
	{
#ifndef _K2P_SKILL_REFACTORYING
		delete m_StateArray[STATE::SKILL];
#endif
		m_StateArray[STATE::SKILL] = NULL;
	}

	if (m_eNextState == STATE::SKILL)
	{
		assert(m_pNextSkillState);

		m_StateArray[STATE::SKILL] = m_pNextSkillState;
		m_pNextSkillState = NULL;
	}

	// ���¸� ����
	m_ePreState = m_eCurState;
	m_eCurState = m_eNextState;
	m_eNextState = STATE::NONE;
	m_CurStateStartTime = CurTime;

	pNewState = GetCurStateInstance();
	assert(pNewState);

	if ( m_eCurState == m_ePreState)
	{
		// ���°� ���� �Ǿ������� ó��
		OnChangeState();
	}



#ifdef _KST_DISPLAY_STATE_CHANGE_LOG
	{
		if( g_pHero == GetObject() )
		{
			char temp[1024];
			wsprintf( temp, "STATE Start [%s]", GetStateName( m_eCurState ) );
			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, temp );
		}
	}
#endif


	// ���ο� ���¸� ����
	pNewState->Initialize( GetObject() );
	return pNewState->OnStart( GetObject(), CurTime );
}

//------------------------------------------------------------------------------
/**
*/
BOOL StateController::SetNextSkillState(StateBase *pState)
{
	// change�Ǳ����� �ٸ� skill state�� ������ �׳� �������ش�
	if (m_pNextSkillState)
	{
		m_pNextSkillState = NULL;
	}

	m_pNextSkillState = pState;
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/

BOOL StateController::ForceStateChange( STATE::TYPE NextState, DWORD CurTime)
{
	if( SetNextState( NextState, CurTime ) == FALSE )
		return FALSE;

	CommitChangeState( CurTime );

	return TRUE;
}
//------------------------------------------------------------------------------ 
StateBase * StateController::GetStateInstance(STATE::TYPE state)
{
    if ((state >= STATE::IDLE) && (state < STATE::MAX))
    {
        return m_StateArray[state];
    }

    return NULL;
}

//------------------------------------------------------------------------------ 
StateSkill* StateController::GetCurSkillState()
{
    STATE::TYPE CurrentState = GetCurState();
    if( CurrentState == STATE::SKILL )
    {
        return (StateSkill*)m_StateArray[STATE::SKILL];
    }

    return NULL;
}
