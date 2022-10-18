#pragma once

#include "..\\..\\SolarTrigger\\IConditionTrigger.h"

#define __CONDITION_PROPERTY( name )	__PTR_PROPERTY( name##_INFO,  )

#define	__BEGIN_DECL_CONDITION( ConcreteCondition )			\
class ConcreteCondition : public IConditionTriggerEx		\
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
class TriggerManagerEx;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//	<IConditionTriggerEx>
class IConditionTriggerEx : public IConditionTrigger
{
public:		IConditionTriggerEx();
			virtual ~IConditionTriggerEx();

public:		VOID						Init( TriggerManager* pMGR, Trigger* pParent );

protected:	inline TriggerManagerEx*	getMGR() { return (TriggerManagerEx*)m_pTriggerMGR;	}
			inline Trigger*				getParent() { return m_pParent;	}

private:	TriggerManager*			m_pTriggerMGR;
			Trigger*				m_pParent;
};

