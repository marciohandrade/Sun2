#pragma once
#include "uiBaseMan.h"

class SummonSkillContainer;
class uiSummonSkillMan :
	public uiBaseMan
{
public:
	uiSummonSkillMan(InterfaceManager *pUIMan);


public:
    virtual void            OnInitialize();

    void MoveSummonSkillDialog(int value);

private:
	SummonSkillContainer *	m_pSummonSkillDlg;
};
