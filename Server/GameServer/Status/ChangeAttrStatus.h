#pragma once

#include "AbilityStatus.h"

/************************************************************************/
/*  ChangeAttrStatus Class                                              */
// 플레이어의 속성을 변경 시키는 Status
// 만일 유지시간이 0이면 아마도 다음 Update까지만 유지될 것이다.
// 만일 유지시간이 존재하면 해당 유지시간 만큼 존재할 것이다.
// 또한 이 상태는 유지시간이 0이면 패킷에 해당 내용이 안 가도록 해야 할 것이다.
// history :
/************************************************************************/

class ChangeAttrStatus : public AbilityStatus
{
public:
	ChangeAttrStatus(void);
	~ChangeAttrStatus(void);

public:
	virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
	virtual VOID		End();
};

