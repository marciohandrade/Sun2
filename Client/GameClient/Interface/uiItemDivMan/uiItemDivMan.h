#ifndef     __USER_INTERFACE_MANAGER_ITEM_DIV_H__
#define     __USER_INTERFACE_MANAGER_ITEM_DIV_H__
//------------------------------------------------------------------------------
/**
    @class uiItemDivMan

    (C) 2006 ckbang
*/
#include "uiItemDivMan_def.h"
#include "uibaseman.h"

class uiItemDiv;
class uiItemShopDiv;


class uiItemDivMan :
    public uiBaseMan
{
public:
    uiItemDivMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
	virtual void            OnInitialize();

private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    NET_SEND_CG_ITEM_DIVIDE_SYN(POSTYPE fromPos, POSTYPE toPos, DURATYPE fromNum, DURATYPE toNum);

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiItemDiv*			    m_pUIItemDiv;
    uiItemShopDiv*		    m_pUIItemShopDiv;
};

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_MANAGER_ITEM_DIV_H__
