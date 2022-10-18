#ifndef	__ABSORBSTATUS_H__
#define __ABSORBSTATUS_H__

#include "AbilityStatus.h"

// Èí¼ö »óÅÂ
class AbsorbStatus : public AbilityStatus
{
public:
	AbsorbStatus() {}
	~AbsorbStatus() {}

	virtual VOID		Execute() {}

	virtual VOID		AttackAbsorb( DAMAGETYPE wDamage );								// °ø°Ý µ¥¹ÌÁö Èí¼ö
	virtual VOID		AttackedAbsorb( eATTACK_TYPE attackType, DAMAGETYPE wDamage );	// ÇÇ°Ý µ¥¹ÌÁö Èí¼ö
};







#endif


