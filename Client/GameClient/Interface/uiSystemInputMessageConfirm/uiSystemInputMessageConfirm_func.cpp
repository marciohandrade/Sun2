//------------------------------------------------------------------------------
// #include "uiSystemInputMessageConfirm.h"
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiSystemInputMessageConfirm.h"
#include "globalfunc.h"

void
uiSystemInputMessageConfirm::SetFocus( void )
{
	GlobalFunc::SetFocus( this, ( CCtrlDxEditBox * )getControl( DLGPOS_EDIT ) );
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
