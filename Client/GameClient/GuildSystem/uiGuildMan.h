#pragma once
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#include "uiBaseMan.h"

class uiGuildMan : public uiBaseMan
{
public:
    uiGuildMan(InterfaceManager* interface_manager);
    virtual ~uiGuildMan();

    virtual void OnInitialize();
    virtual void OnRelease();
    virtual void OnReset();
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void NetworkProcWorld(MSG_BASE* recv_packet);
    virtual void InitPKBooleans();

private:

private:

};

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER