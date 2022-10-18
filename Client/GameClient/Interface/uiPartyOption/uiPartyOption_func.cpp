//------------------------------------------------------------------------------
//  uiPartyOption_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyOption.h"

#include "uiPartyMan/uiPartyMan.h"
#include "interfacemanager.h"
//------------------------------------------------------------------------------
/**
*/
uiPartyMan*
uiPartyOption::GetManager()
{
    uiPartyMan* partyMan =
        static_cast<uiPartyMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_PARTY));
    assert (partyMan);
    return partyMan;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
