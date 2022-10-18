//------------------------------------------------------------------------------
//  uiGuildMan_net.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

#else

#include "uiGuildMan.h"

//------------------------------------------------------------------------------
/**
*/
void
uiGuildMan::InitPKBooleans()
{
    this->m_vPKBool.clear();

    for (int i=ePKBOOL_GUILD_BASE; i<ePKBOOL_GUILD_MAX; ++i)
    {
        uicPacketControl packetControl;
        this->m_vPKBool.push_back(packetControl);
    }
}

#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
