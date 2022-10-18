#pragma once
#include "uiBaseMan.h"

struct IM_SUMMON_TARGET_MANAGER
{
	static const DWORD SUMMON_TARGET_DIALOG;
};

class SummonTargetDialog;
class uiSummonTargetMan :
	public uiBaseMan
{
public:
	uiSummonTargetMan(InterfaceManager *pUIMan);

public:
    virtual void            OnInitialize();


    void MoveSummonTargetDialog(int value);


private:
	SummonTargetDialog *	m_pSummonTargetDlg;
};
