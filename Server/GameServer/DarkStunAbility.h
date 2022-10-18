#pragma once

#ifdef __NA_S00015_20080828_NEW_HOPE_SHADOW

#include "BaseStatusAbility.h"

class DarkStunAbility : public BaseStatusAbility
{
public:
	DarkStunAbility(void);
	~DarkStunAbility(void);

public:
	virtual BOOL	IsValidStateID();
	virtual BOOL	Execute( Character *pTarget, BYTE *pMsg, WORD &wMsgSize );
};

#endif 