#ifndef	__SLEEPSTATUS_H__
#define __SLEEPSTATUS_H__

#include "AbilityStatus.h"

// ���� ����
class SleepStatus : public AbilityStatus
{
public:
	SleepStatus() {}
	~SleepStatus() {}

	virtual VOID		Start();

	virtual BOOL		CanRemove();
};


#endif


