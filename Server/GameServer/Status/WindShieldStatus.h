#ifndef	__WINDSHIELDSTATUS_H__
#define __WINDSHIELDSTATUS_H__

#include "AbilityStatus.h"

// 주변에 주기적으로 데미지를 가하는 상태
class WindShieldStatus : public AbilityStatus
{
public:
	WindShieldStatus() {}
	~WindShieldStatus() {}

	virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
	virtual VOID		Start() {}
	virtual VOID		Execute();

private:
	eATTACK_TYPE	m_eAttackType;		// 공격타입
	float			m_fAttackRange;		// 데미지를 주는 사거리
	DAMAGETYPE		m_wPeriodicDamage;	// 주기 데미지 수치
    // NOTE: f110217.5L, added
    int base_skill_power_application_ratio_;

#ifdef _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK
    Player* owner_player_;
    bool is_force_attack_;
#endif //_NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK
};


#endif


