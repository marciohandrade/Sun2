#pragma once


#include "StateBase.h"
#include "SkillState.h"


class SkillStateCustom : public StateSkill
{
public:

	SkillStateCustom();

private:
	BOOL        m_bCreateProjectile;	// �߻�ü�� �������� �ʾҴٸ� OnEnd �ÿ� ��������� �����Ѵ�.

	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	STATECHANGE::TYPE OnEnd(Object* pOwner, DWORD CurTime);

	//STATEPROCESS::TYPE	_OnProcessSacredFire(Object* pOwner, DWORD CurTime,DWORD dwTick);
	STATEPROCESS::TYPE	_OnProcessInQuiry(Object* pOwner, DWORD CurTime,DWORD dwTick);
	STATEPROCESS::TYPE	_OnProcessDragonTransForm(Object* pOwner, DWORD CurTime,DWORD dwTick);
	STATEPROCESS::TYPE	_OnProcessDragonCler(Object* pOwner, DWORD CurTime,DWORD dwTick);

	const WzVector& GetTargetAreaPosition( Character* pCharacter );
};
