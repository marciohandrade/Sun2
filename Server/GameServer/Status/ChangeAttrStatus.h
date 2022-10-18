#pragma once

#include "AbilityStatus.h"

/************************************************************************/
/*  ChangeAttrStatus Class                                              */
// �÷��̾��� �Ӽ��� ���� ��Ű�� Status
// ���� �����ð��� 0�̸� �Ƹ��� ���� Update������ ������ ���̴�.
// ���� �����ð��� �����ϸ� �ش� �����ð� ��ŭ ������ ���̴�.
// ���� �� ���´� �����ð��� 0�̸� ��Ŷ�� �ش� ������ �� ������ �ؾ� �� ���̴�.
// history :
/************************************************************************/

class ChangeAttrStatus : public AbilityStatus
{
public:
	ChangeAttrStatus(void);
	~ChangeAttrStatus(void);

public:
	virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
	virtual VOID		End();
};

