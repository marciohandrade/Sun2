#ifndef	__ABSORBSTATUS_H__
#define __ABSORBSTATUS_H__

#include "AbilityStatus.h"

// ��� ����
class AbsorbStatus : public AbilityStatus
{
public:
	AbsorbStatus() {}
	~AbsorbStatus() {}

	virtual VOID		Execute() {}

	virtual VOID		AttackAbsorb( DAMAGETYPE wDamage );								// ���� ������ ���
	virtual VOID		AttackedAbsorb( eATTACK_TYPE attackType, DAMAGETYPE wDamage );	// �ǰ� ������ ���
};







#endif


