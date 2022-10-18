#pragma once
#include <scskillslotcontainer.h>

// 클라이언트에서만 사용하는 함수 추가를 위해
// TAIYO
class SkillSlotContainer : public SCSkillSlotContainer
{
public:
	SkillSlotContainer(void);
	virtual ~SkillSlotContainer(void);

	virtual SCSlot * SkillSlotContainer::CreateSlot();
	virtual VOID	Init( POSTYPE MaxSlotSize );

	//BOOL			InsertSlot( POSTYPE AtPos, DBSERIAL Serial, SKILLSTREAM & stream );
};
