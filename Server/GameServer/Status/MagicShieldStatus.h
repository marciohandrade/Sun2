#ifndef	__MAGICSHIELDSTATUS_H__
#define __MAGICSHIELDSTATUS_H__

#include "AbilityStatus.h"

// 마법쉴드 상태
class MagicShieldStatus : public AbilityStatus
{
public:
	MagicShieldStatus() {}
	~MagicShieldStatus() {}

	virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
	virtual VOID		Start();
	virtual VOID		Execute() {}
	virtual VOID		End();

private:
	int			m_iShieldHP;
	int         m_iDecreaseMP;
    float       m_fDecreaseMpRatio;
	float		m_fAbsorbRatio;
};


#endif


