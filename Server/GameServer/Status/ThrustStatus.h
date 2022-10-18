#ifndef	__THRUSTSTATUS_H__
#define __THRUSTSTATUS_H__

#include "AbilityStatus.h"

// 밀리기 상태
class ThrustStatus : public AbilityStatus
{
public:
	ThrustStatus() {}
	~ThrustStatus() {}

	virtual VOID		Start();
	virtual VOID		Execute();
};


#endif


