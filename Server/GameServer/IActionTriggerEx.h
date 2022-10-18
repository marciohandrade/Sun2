#pragma once

#include "..\\..\\SolarTrigger\\IActionTrigger.h"

#define __ACTION_PROPERTY( name )		__PTR_PROPERTY( name##_INFO,  )

#define	__BEGIN_DECL_ACTION( ConcreteAction )				\
class ConcreteAction : public IActionTriggerEx			\
{														\
	__DECLARE_POOL( ConcreteAction )					\
	__ACTION_PROPERTY( ConcreteAction )					\
public:													\
	ConcreteAction(){}									\
	~ConcreteAction(){}									\
	WORD TriggerType() { return e##ConcreteAction; }

#define __END_DECL_ACTION									\
};


class Trigger;
class TriggerManagerEx;

class IActionTriggerEx : public IActionTrigger
{
public:
	IActionTriggerEx(void);
	~IActionTriggerEx(void);

public:
	VOID Init( TriggerManager* pMGR, Trigger* pParent );

protected:
	inline TriggerManagerEx* getMGR() { return (TriggerManagerEx*)m_pTriggerMGR;	}
	inline Trigger* getParent() { return m_pParent;	}

private:
	Trigger*			m_pParent;
	TriggerManager*		m_pTriggerMGR;
};

