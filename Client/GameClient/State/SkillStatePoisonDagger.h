

/**
	@author
	-광수

	@since
		2008.09
	@remarks
	   조절점이 1개인 베지어곡석기저식사용
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
