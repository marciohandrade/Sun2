//------------------------------------------------------------------------------
//  uiItemDivMan_main.cpp
//  (C) 2006 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "globalfunc.h"

#include "uiItemDivMan.h"
#include "uiItemDiv/uiItemDiv.h"
#include "uiItemShopDiv/uiItemShopDiv.h"
//------------------------------------------------------------------------------
/**
*/
uiItemDivMan::uiItemDivMan(InterfaceManager *pUIMan) :
    uiBaseMan(pUIMan)
{
}

//------------------------------------------------------------------------------
/**
*/
void
uiItemDivMan::OnInitialize()
{
    _LoadUI();
}

//------------------------------------------------------------------------------
/**
*/
void
uiItemDivMan::NET_SEND_CG_ITEM_DIVIDE_SYN(POSTYPE fromPos, POSTYPE toPos, DURATYPE fromNum, DURATYPE toNum)
{
    MSG_CG_ITEM_DIVIDE_SYN  sync;
    sync.m_fromPos = fromPos;
    sync.m_toPos = toPos;
    sync.m_fromNum = fromNum;
    sync.m_toNum = toNum;

    GlobalFunc::SendPacketToGameServer(&sync, sizeof (sync));
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
