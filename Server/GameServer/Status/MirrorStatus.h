#ifndef	__MIRRORSTATUS_H__
#define __MIRRORSTATUS_H__

#include "Skill.h"
#include "AbilityStatus.h"


// 데미지 반사 상태
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

	virtual VOID		DamageMirror( Character *pAttacker, DAMAGETYPE wDamage );		// 데미지 반사

private:
	float				m_fAbsorbRatio;
	int					m_iOption2;

	SkillInfo			m_SkillInfoData;		// 피격어빌리티 생성을 위한 임시 변수
	BASE_ABILITYINFO	m_BaseAbilityInfo;		// 피격상태 생성을 위한 임시 변수

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    // 피격시 스킬 사용 관련
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


