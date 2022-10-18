#pragma once

#include "State/StateBase.h"

class Object;
class StateSkill;

class StateController
{
private:
	StateBase*		m_StateArray[STATE::MAX];
	StateBase*		m_pNextSkillState;

	STATE::TYPE		m_eCurState;
	STATE::TYPE		m_eNextState;
	STATE::TYPE		m_ePreState;

	DWORD			m_CurStateStartTime;	

protected:
	StateController();
	virtual ~StateController();

	virtual Object*		GetObject() PURE;
	virtual void		OnChangeState() PURE; 

	void				InitializeState();
    void                InitializeStateForHero();
	void				StateProcess(DWORD dwTick, DWORD CurTime);
	
public:
	STATE::TYPE			GetPreState() {return m_ePreState;	}	
	STATE::TYPE			GetCurState() {return m_eCurState;	}
	STATE::TYPE			GetNextState() {return m_eNextState;	}
	StateBase *			GetCurStateInstance() {return m_StateArray[GetCurState()];}
	DWORD				GetCurStateStartTime() {return m_CurStateStartTime;	}
    StateBase *			GetStateInstance(STATE::TYPE state);

    StateSkill*         GetCurSkillState();

	void				InitState(STATE::TYPE initState, DWORD CurTime);

	STATECHANGE::TYPE	CommitChangeState( DWORD CurTime );
	BOOL				ForceStateChange( STATE::TYPE NextState, DWORD CurTime );

	//!다음으로 변화될 상태를 지정해주는 함수
	/*!
	내부적으로 현재 상태에 CanTransite을 호출 TRUE시에만 다음 상태로 기록한다.
	*/
	BOOL				SetNextState(STATE::TYPE nextState, DWORD CurTime);
	BOOL				SetNextSkillState(StateBase* pSkillState);

#ifdef _KST_DISPLAY_STATE_CHANGE_LOG
    const char* GetStateName( STATE::TYPE type );
#endif
};
