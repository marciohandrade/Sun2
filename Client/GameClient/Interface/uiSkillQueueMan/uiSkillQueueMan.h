#pragma once
#include "uiBaseMan.h"

class SkillQueueDialog;

struct IM_SKILL_QUEUE_MANAGER
{
	static const DWORD SKILL_QUEUE;
};

class uiSkillQueueMan :
	public uiBaseMan
{
public:
	uiSkillQueueMan(InterfaceManager *pUIMan);

public:
	virtual void            OnInitialize();

    void MoveSkillQueueDialog(int value);


private:
	SkillQueueDialog *		m_pSkillQueueDialog;
};
