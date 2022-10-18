#ifndef	__BLINDSTATUS_H__
#define __BLINDSTATUS_H__

#include "AbilityStatus.h"

// 블라인드(장님) 상태
class BlindStatus : public AbilityStatus
{
public:
	BlindStatus() {}
	~BlindStatus() {}

	virtual VOID		Start();
	virtual VOID		Execute() {}
	virtual VOID		End();

private:
	int			m_iPrevSightRange;
};


#endif


