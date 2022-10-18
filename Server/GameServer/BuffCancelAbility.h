#pragma once

#include "Ability.h"


#ifdef		__NA_S00015_20080828_NEW_HOPE_SHADOW

class BuffCancelAbility : public Ability
{
public:
	BuffCancelAbility(void);
	~BuffCancelAbility(void);

	virtual BOOL	Execute( Character *pTarget, BYTE *pMsg, WORD &wMsgSize );
};

#endif
