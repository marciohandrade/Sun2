//------------------------------------------------------------------------------
//  uiBase_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiBase.h"
#include "Mouse.h"
#include "GlobalFunc.h"
//------------------------------------------------------------------------------
/**
*/
BYTE
uiBase::GetMouseWheelStatus(const RECT* rc /* =NULL */)
{
    if (rc == NULL)
    {
        return g_Input.GetMouseWheel();
    }

    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    if ((rc->left <= iMouseX && rc->right >= iMouseX) && 
        (rc->top <= iMouseY && rc->bottom >= iMouseY))
    {
        return g_Input.GetMouseWheel();
    }

    return 0;
}
//------------------------------------------------------------------------------ 
bool uiBase::IsOverControl(CControlWZ* control)
{
    if (control == NULL)
    {
        return false;
    }
    RECT rc = control->GetSizeRect();
    if (rc.left <= Mouse::Instance()->GetMouseX() && rc.right >= Mouse::Instance()->GetMouseX() && 
        rc.top <= Mouse::Instance()->GetMouseY()  && rc.bottom >= Mouse::Instance()->GetMouseY())
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
