#ifndef	__CHAOSSTATUS_H__
#define __CHAOSSTATUS_H__

#include "AbilityStatus.h"

// ġ�� ����
class ChaosStatus : public AbilityStatus
{
public:
	ChaosStatus() {}
	~ChaosStatus() {}

	virtual VOID		Start();
	virtual VOID		Execute() {}
};


#endif


