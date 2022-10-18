#ifndef	__STUNSTATUS_H__
#define __STUNSTATUS_H__

#include "AbilityStatus.h"

// 스턴 상태
class StunStatus : public AbilityStatus
{
public:
	StunStatus() {}
	~StunStatus() {}

	virtual VOID		Start();
	virtual VOID		End();
};


#endif


