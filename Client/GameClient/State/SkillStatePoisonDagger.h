

/**
	@author
	-����

	@since
		2008.09
	@remarks
	   �������� 1���� �����������Ļ��
*/

#pragma once

class StateSkill;


class SkillStatePoisonDagger :	public StateSkill
{
public:
	SkillStatePoisonDagger();
	virtual ~SkillStatePoisonDagger(void);

	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
	DWORD	m_dwDaggerKey;
	DWORD	m_bAniStop;

};
