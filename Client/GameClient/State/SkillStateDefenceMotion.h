/**

	@author
		-����

	@since

	@remarks
		-2006.01.08 �ű� �߰�-���� 
*/

#pragma once

class StateSkill;

class SkillStateDefenceMotion : public StateSkill
{
public:
	SkillStateDefenceMotion();
	~SkillStateDefenceMotion();

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

#ifdef _NA_008783_20160418_DEFENCEMOTION_CANCEL_BUG
	BOOL CanTransite(Object* owner_ptr, DWORD cur_time, StateBase* to_transite_state);
#endif // _NA_008783_20160418_DEFENCEMOTION_CANCEL_BUG

protected:
	DWORD m_dwlifeTime;
	DWORD m_dwCurTime;

    void ProcessExtraCommand();

};