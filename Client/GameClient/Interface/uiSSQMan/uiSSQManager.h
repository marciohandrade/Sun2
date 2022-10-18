#pragma once

#include "uiBaseMan.h"
#include "uiSSQSystemtime/uiSSQSystemWindow.h"
#include "uiSSQEnterWindow/uiSSQEnterWindow.h"
#include "uiSSQRewardWindow/uiSSQRewardWindow.h"
#include "uiSSQDiceWindow/uiSSQDiceWindow.h"

struct	IM_SSQ_MANAGER
{
	static const DWORD SSQ_SYSTEMTIME;			
	static const DWORD SSQ_ENTER_WINDOW;		
	static const DWORD SSQ_REWARD_WINDOW;
	static const DWORD SSQ_DICE_WINDOW;
};


class uiSSQManager : public uiBaseMan
{
public:
	uiSSQManager( InterfaceManager * pUIMAN );

private:
	virtual void            OnInitialize();
};
