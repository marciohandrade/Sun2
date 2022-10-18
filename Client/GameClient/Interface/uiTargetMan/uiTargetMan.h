#pragma once
#include "uiBaseMan.h"

class TargetDialog;
class uiTargetZoneMarkDialog;
class uiTargetAggro;

struct IM_TARGET_MANAGER 
{
	static const DWORD TARGET_DIALOG;
    static const DWORD TARGET_ZONEMARK_DIALOG;
    static const DWORD TARGET_AGGRO;
};

class uiTargetMan :
	public uiBaseMan
{
public:
	uiTargetMan(InterfaceManager *pUIMan);


public:
	virtual void			OnInitialize();

private:
	TargetDialog*			target_dialog_;
    uiTargetZoneMarkDialog* target_zonemark_dialog_;
    uiTargetAggro* target_aggro_dialog_;
};