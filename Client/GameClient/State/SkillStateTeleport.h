/**

    @author
        -����

    @since

    @remarks
        -2005.07.18 Ŭ���� �߰�   
*/

#pragma once

class StateSkill;

class SkillStateTeleport : public StateSkill
{
public:
	SkillStateTeleport() {}

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
};