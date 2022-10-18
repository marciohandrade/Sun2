#pragma once

#include "uiBaseMan.h"

struct IM_SOCKET_MANAGER
{
    static const DWORD SOCKET_COMPOSITE;
};

class uiSocketComposite;
class uiSocketMan: public uiBaseMan
{
public:
	uiSocketMan(InterfaceManager* ui_manager);

public:
	virtual void OnInitialize();

private:
	uiSocketComposite* ui_socket_composite_;
};
