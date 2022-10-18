#ifndef	__BASEEFFECT_H__
#define __BASEEFFECT_H__

#include "BaseStatus.h"


class BaseEffect : public BaseStatus
{
public:
	BaseEffect() {}
	~BaseEffect() {}

	VOID						Init( SLOTCODE SkillCode, int iApplicationTime, int iPeriod, Character *pAttacker, WzVector vCurPos, float fRadius );

	virtual VOID				Start() {}
	virtual VOID				Execute();
	virtual VOID				End() {}
#ifdef _NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME
    virtual BOOL Update(DWORD cur_time);
#endif //_NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME

	// 하위 클래스에서만 쓰이는 함수
	virtual VOID				SetDamage( eATTACK_TYPE eType, DAMAGETYPE wDamage ) { __UNUSED((eType, wDamage)); }
	virtual VOID				SetSkillPercentDamage( float fSkillPercentDamage ) { __UNUSED(fSkillPercentDamage); }

protected:
	SLOTCODE					m_SkillCode;		// 이펙트를 발생시킨 스킬코드

	GameField*					m_pField;			// Effect가 시전된 필드
	DWORD						m_dwSectorIndex;	// Effect가 시전된 섹터 인덱스

	WzVector					m_vCurPos;			// 현재위치
	float						m_fRadius;			// 반지름
};


#endif


