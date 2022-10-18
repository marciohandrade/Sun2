#ifndef	__CHAOSSTATUS_H__
#define __CHAOSSTATUS_H__

#include "AbilityStatus.h"

// 치료 상태
class ChaosStatus : public AbilityStatus
{
public:
	ChaosStatus() {}
	~ChaosStatus() {}

	virtual VOID		Start();
	virtual VOID		Execute() {}
};


#endif


