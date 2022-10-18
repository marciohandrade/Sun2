#pragma once

#include "AbilityStatus.h"

// 소환투데스 스킬에 의해 소환된 상태
class SummonStatus : public AbilityStatus
{
public:
	SummonStatus() {}
	~SummonStatus() {}

	virtual VOID		Start();
	virtual VOID		End();
};





