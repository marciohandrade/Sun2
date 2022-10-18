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

	// ���� Ŭ���������� ���̴� �Լ�
	virtual VOID				SetDamage( eATTACK_TYPE eType, DAMAGETYPE wDamage ) { __UNUSED((eType, wDamage)); }
	virtual VOID				SetSkillPercentDamage( float fSkillPercentDamage ) { __UNUSED(fSkillPercentDamage); }

protected:
	SLOTCODE					m_SkillCode;		// ����Ʈ�� �߻���Ų ��ų�ڵ�

	GameField*					m_pField;			// Effect�� ������ �ʵ�
	DWORD						m_dwSectorIndex;	// Effect�� ������ ���� �ε���

	WzVector					m_vCurPos;			// ������ġ
	float						m_fRadius;			// ������
};


#endif


