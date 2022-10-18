//------------------------------------------------------------------------------
//  uiBattleZonePassword_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBattleZonePassword.h"

#include "uiBattleZoneMan/uiBattleZoneMan.h"
#include "interfacemanager.h"

//------------------------------------------------------------------------------
/**
*/
uiBattleZoneMan*
uiBattleZonePassword::GetManager()
{
    uiBattleZoneMan* battleZoneMan =
        static_cast<uiBattleZoneMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_BATTLEZONE));
    assert (battleZoneMan);
    return battleZoneMan;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
