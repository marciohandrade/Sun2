//------------------------------------------------------------------------------
//  uiVendorBuy_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiVendorBuy.h"
#include "ItemManager.h"
#include "ItemUnitRenderer.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
/** ITEM UNIT RENDERER
*/
void
uiVendorBuy::InitItemRender()
{
    SAFE_RELEASENDELETE(m_pItemUnitRenderer);

    if (NULL == m_pItemUnitRenderer )
    {
        m_pItemUnitRenderer = new ItemUnitRenderer;
        m_pItemUnitRenderer->Init( uiBase::GetDrawBase(), GetMaxSlotNum() );
    }
}

//------------------------------------------------------------------------------
/** ITEM UNIT RENDERER
*/
void
uiVendorBuy::ResetItemRenderer()
{
    SAFE_RELEASENDELETE(m_pItemUnitRenderer);

    this->InitItemRender();
}

//------------------------------------------------------------------------------
/** ITEM UNIT RENDERER
*/
ItemUnitRenderer * 
uiVendorBuy::GetItemUnitRender() const
{
    return m_pItemUnitRenderer;
}

//------------------------------------------------------------------------------
/**
    working for slot clean

    called where to Hero::ReleaseContainer().
*/
void
uiVendorBuy::FlushUnitDraw()
{
    SCSlot FromSlot;

    for (int i = 0; i < GetMaxSlotNum(); ++i)
    {
        if( !IsEmpty(i) )
            DeleteSlot(i, NULL);
    }

}

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::SendItemBuySync(POSTYPE pos , int buyNum)
{
	MSG_CG_VENDOR_BUY_SYN sendMsg;
	sendMsg.m_byCategory    = CG_VENDOR;
	sendMsg.m_byProtocol    = CG_VENDOR_BUY_SYN;
	sendMsg.m_VendorPos     = pos;

	if ( buyNum )
	{
		sendMsg.m_BuyNum = buyNum;
	}
	else
	{
		sendMsg.m_BuyNum		= m_curAmouts;
	}


	GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
}

#else
//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::SendItemBuySync(POSTYPE pos)
{
    MSG_CG_VENDOR_BUY_SYN sendMsg;
    sendMsg.m_byCategory    = CG_VENDOR;
    sendMsg.m_byProtocol    = CG_VENDOR_BUY_SYN;
    sendMsg.m_VendorPos     = pos;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
}

#endif

//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::ItemSub(POSTYPE pos)
{
    SCSlot FromSlot;
    DeleteSlot(pos, NULL);

    this->m_curAmouts = 0;
    this->m_curMoney = 0;
    this->m_curPos = 0;
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::QueryView(DWORD key)
{
    MSG_CG_VENDOR_VIEW_START_SYN sendMsg;
    sendMsg.m_byCategory    = CG_VENDOR;
    sendMsg.m_byProtocol    = CG_VENDOR_VIEW_START_SYN;
    sendMsg.m_dwPlayerKey   = key;

    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
}

//------------------------------------------------------------------------------
/**
*/
void
uiVendorBuy::QueryViewEnd()
{
    MSG_CG_VENDOR_VIEW_END_SYN sendMsg;
    GlobalFunc::SendPacket( CI_GAMESERVERIDX, &sendMsg, sizeof(sendMsg) );
}

void 
uiVendorBuy::SetBuyItemsSearchFoucs( POSTYPE pos,byte count ,LONGLONG price )
{
	m_curPos = pos;
	m_curMoney = price* count;
	m_curAmouts = count;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
