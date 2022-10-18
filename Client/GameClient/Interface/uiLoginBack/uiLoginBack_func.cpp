//------------------------------------------------------------------------------
//  uiLoginBack_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginBack.h"
#include "uiLoginMan/uiLoginMan.h"

//------------------------------------------------------------------------------
/**
*/
uiLoginMan*
uiLoginBack::GetManager()
{
    uiLoginMan* loginMan =
        static_cast<uiLoginMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_LOGIN));
    assert (loginMan);
    return loginMan;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
