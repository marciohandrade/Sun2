/**
	@author
		-±¤¼ö

	@since
		2008.10.20 
	@remarks

*/
#pragma once

class StateSkill;

class SkillStateHide :	public StateSkill
{
public:
	SkillStateHide();
	virtual ~SkillStateHide(void);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
	void						SetHidePlayer(Object *pObj);
	
private:
	DWORD m_dwCopyTime;
	int	  m_iCloneCnt;
};
