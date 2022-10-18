#pragma once

#include "uiBaseMan.h"

class MissionRelay;
class uiMissionRelayMan : public uiBaseMan
{
public:
	uiMissionRelayMan( InterfaceManager * pUIMAN);

public:
	virtual void            OnInitialize();

private:
	MissionRelay * m_pUIMissionRelay;
};