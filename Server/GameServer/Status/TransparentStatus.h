#pragma once

#include "AbilityStatus.h"

class TransparentStatus : public AbilityStatus
{
public:
	TransparentStatus(void);
	~TransparentStatus(void);

public:
	virtual  VOID		Start();
	virtual  VOID		End();
};

