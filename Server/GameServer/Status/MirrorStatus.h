#ifndef	__MIRRORSTATUS_H__
#define __MIRRORSTATUS_H__

#include "Skill.h"
#include "AbilityStatus.h"


// ������ �ݻ� ����
class MirrorStatus : public AbilityStatus
{
public:
	MirrorStatus() {}
	~MirrorStatus() {}

	virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
	virtual VOID		Start() {}
	virtual VOID		Execute() {}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    virtual BOOL        Update(DWORD curTime);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	virtual VOID		DamageMirror( Character *pAttacker, DAMAGETYPE wDamage );		// ������ �ݻ�

private:
	float				m_fAbsorbRatio;
	int					m_iOption2;

	SkillInfo			m_SkillInfoData;		// �ǰݾ����Ƽ ������ ���� �ӽ� ����
	BASE_ABILITYINFO	m_BaseAbilityInfo;		// �ǰݻ��� ������ ���� �ӽ� ����

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    // �ǰݽ� ��ų ��� ����
    struct LinkSkill
    {
        Character*   target;
        DWORD        execute_time;
    };
    typedef STLX_MAP<Character*, LinkSkill> LinkSkillMap;
    void _RegistLinkSkill(Character *pAttacker);
    LinkSkillMap m_LinkSkillMap;
    DWORD        m_LinkSkillCoolTime;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
};







#endif


