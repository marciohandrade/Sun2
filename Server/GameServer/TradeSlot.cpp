#include "StdAfx.h"
#include ".\tradeslot.h"

TradeSlot::TradeSlot(void):
	m_pItemSlot(NULL)
{
}

TradeSlot::~TradeSlot(void)
{
}

VOID TradeSlot::Copy( SCSlot & IN slot )
{
	SCSlot::Copy(slot);
	m_pItemSlot = ((TradeSlot &)slot).GetLinkItem();
}
