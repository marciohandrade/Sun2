#ifndef	__WINDSHIELDSTATUS_H__
#define __WINDSHIELDSTATUS_H__

#include "AbilityStatus.h"

// �ֺ��� �ֱ������� �������� ���ϴ� ����
class WindShieldStatus : public AbilityStatus
{
public:
	WindShieldStatus() {}
	~WindShieldStatus() {}

	virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
	virtual VOID		Start() {}
	virtual VOID		Execute();

private:
	eATTACK_TYPE	m_eAttackType;		// ����Ÿ��
	float			m_fAttackRange;		// �������� �ִ� ��Ÿ�
	DAMAGETYPE		m_wPeriodicDamage;	// �ֱ� ������ ��ġ
    // NOTE: f110217.5L, added
    int base_skill_power_application_ratio_;

#ifdef _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK
    Player* owner_player_;
    bool is_force_attack_;
#endif //_NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK
};


#endif


