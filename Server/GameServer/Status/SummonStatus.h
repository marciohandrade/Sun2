#pragma once

#include "AbilityStatus.h"

// ��ȯ������ ��ų�� ���� ��ȯ�� ����
class SummonStatus : public AbilityStatus
{
public:
	SummonStatus() {}
	~SummonStatus() {}

	virtual VOID		Start();
	virtual VOID		End();
};





