#pragma once
#include "object.h"

class CinematicCamera :
	public Object
{
public:
	CinematicCamera(void);
	virtual ~CinematicCamera(void);

	BOOL Create( DWORD dwObjectKey, DWORD dwID );
};
