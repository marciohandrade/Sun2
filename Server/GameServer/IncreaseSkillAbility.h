#pragma once

#ifdef __NA_S00015_20080828_NEW_HOPE_SHADOW

#include "BaseStatusAbility.h"

//스킬능력증가 어빌리티
class IncreaseSkillAbility : public BaseStatusAbility
{
public:
	IncreaseSkillAbility(void);
	~IncreaseSkillAbility(void);

public:
	virtual BOOL	IsValidStateID();
};

#endif //__NA_S00015_20080828_NEW_HOPE_SHADOW
