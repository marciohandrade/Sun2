/**

	@author
		-����

	@since

	@remarks
		-2006.01.08 �ű� �߰�-���� 
*/
#pragma once

class StateSkill;

class SkillStateViperBlade : public StateSkill
{
public:

	SkillStateViperBlade() {}

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
};