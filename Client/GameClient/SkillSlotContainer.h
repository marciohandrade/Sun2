#pragma once
#include <scskillslotcontainer.h>

// Ŭ���̾�Ʈ������ ����ϴ� �Լ� �߰��� ����
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
