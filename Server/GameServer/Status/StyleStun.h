#pragma once

#include "EtcStatus.h"


class StyleStun : public EtcStatus
{
public:
	StyleStun() {}
	~StyleStun() {}

	virtual VOID		Start();
	virtual VOID		End();
};


