#pragma once

#include "AbilityStatus.h"

// ġ�� ����
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

	// m_StateValue[0] �� 0�̸� 1,2,3�� ���� Ÿ���̸�, 0�� �ƴϸ� ���� �ڵ��̴�.
	INT m_StateValue[MAX_CURE_STATE_NUM];
};




