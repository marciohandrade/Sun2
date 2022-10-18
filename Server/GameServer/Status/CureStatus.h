#pragma once

#include "AbilityStatus.h"

// 치료 상태
class CureStatus : public AbilityStatus
{
	enum { MAX_CURE_STATE_NUM = 4 };
public:
	CureStatus() {}
	~CureStatus() {}

	virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
	virtual VOID		Start();
	virtual VOID		Execute();

private:
	VOID				RemoveStatus();

	// m_StateValue[0] 이 0이면 1,2,3은 상태 타입이며, 0이 아니면 상태 코드이다.
	INT m_StateValue[MAX_CURE_STATE_NUM];
};




