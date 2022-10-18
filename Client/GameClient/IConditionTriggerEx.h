#pragma once

#include "..\\..\\SolarTrigger\\IConditionTrigger.h"

#define __CONDITION_PROPERTY( name )	__PTR_PROPERTY( name##_INFO,  )

#define	__BEGIN_DECL_CONDITION( ConcreteCondition )			\
class ConcreteCondition : public ConditionTriggerEx		\
{															\
	__DECLARE_POOL( ConcreteCondition )						\
	__CONDITION_PROPERTY( ConcreteCondition )				\
public:														\
	ConcreteCondition(){OnInstanciate();}					\
	~ConcreteCondition(){}									\
	WORD TriggerType() { return e##ConcreteCondition; }

#define __END_DECL_CONDITION								\
};

class Trigger;
class TriggerManEx;

class ConditionTriggerEx : public IConditionTrigger
{
public:
	ConditionTriggerEx(void);
	virtual ~ConditionTriggerEx(void);

public:
	VOID Init( TriggerManager* pMGR, Trigger * pParent );
	BOOL OnMsg( TRIGGER_MSG * pMsg );
	BOOL GetAlwaysAction(){ return m_bAlwaysAction;}
	VOID SetAlwaysAction(BOOL bVal){m_bAlwaysAction = bVal;}

protected:
	inline TriggerManEx*	getMGR() { return (TriggerManEx*)m_pTriggerMGR;	}
	inline Trigger*			getParent() { return m_pParent;	}

private:
	Trigger*				m_pParent;
	TriggerManager*			m_pTriggerMGR;
	BOOL					m_bAlwaysAction;
	
};

