#pragma once
#include "uiBaseMan.h"

class BattleLobby;
class uiChangeRoomName;

class uiBattleLobbyMan :
	public uiBaseMan
{
public:
	uiBattleLobbyMan(InterfaceManager *pUIMan);


public:
    virtual void            OnInitialize();


private:

	BattleLobby *			m_pBattleLobby;

	uiChangeRoomName* m_pChangeRoomName;
};
