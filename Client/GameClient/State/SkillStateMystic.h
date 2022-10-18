#pragma once

class StateSkill;

class SkillStateMystic : public StateSkill
{
public:
	SkillStateMystic();

private:
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
};

class SkillStateMysticSmash : public StateSkill
{
public:
	SkillStateMysticSmash();

private:
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

	WzVector m_TargetPosition1;
	WzVector m_TargetPosition2;

};

class SkillStateMystyBlink : public StateSkill
{
public:
	SkillStateMystyBlink(){}

private:
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

	WzVector m_TargetPosition;

};