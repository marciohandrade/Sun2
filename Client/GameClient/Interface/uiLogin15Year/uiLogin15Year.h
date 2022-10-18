#ifndef     __USER_INTERFACE_LOGIN_15_H__
#define     __USER_INTERFACE_LOGIN_15_H__
//------------------------------------------------------------------------------
/**
*/
#include "uiLogin15Year_def.h"
#include "uibase.h"
#include "interfacemanager.h"

class uiLogin15Year :
    public uiBase
{
public:
    uiLogin15Year(InterfaceManager *pUIMan);
    virtual ~uiLogin15Year();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:

    //--------------------------------------------------------------------------
    // functions
    //
public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );

protected:

	//--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
    //virtual void            UpdatePosition();

    //--------------------------------------------------------------------------
    // member vars
    //
private:
};

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_LOGIN_15_H__
