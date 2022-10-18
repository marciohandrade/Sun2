#pragma once
#include "uiBaseMan.h"

class RewardDialog;
class uiRewardMan :
	public uiBaseMan
{
public:
	uiRewardMan(InterfaceManager *pUIMan);


public:
    virtual void            OnInitialize();


private:
	RewardDialog *			m_pRewardDlg;
};
