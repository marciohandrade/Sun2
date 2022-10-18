#pragma once

#include "EtcStatus.h"


class StyleDown : public EtcStatus
{
public:
	StyleDown() {}
	~StyleDown() {}

	virtual VOID		Start();
	virtual VOID		End();
};


