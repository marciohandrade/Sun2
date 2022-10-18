// TradeSlotContainer.cpp: implementation of the CTradeSlotContainer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TradeSlotContainer.h"
#include "TradeSlot.h"
#include "SCItemSlot.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TradeSlotContainer::TradeSlotContainer()
{
	SCSlotContainer::Init( MAX_TRADE_SLOT_NUM/2, SI_TRADE );
}

TradeSlotContainer::~TradeSlotContainer()
{
	
}


SCSlot * TradeSlotContainer::CreateSlot()
{
	return new TradeSlot;
}

VOID TradeSlotContainer::Init( Player * pPlayer, Player * pCounterPlayer, DWORD key )
{
	m_pPlayer = pPlayer;
	m_pCounterPlayer = pCounterPlayer;
	m_dwKey = key;
	m_Money = 0;
	m_State = TRADE_READY;
	ClearAll();
}

BOOL TradeSlotContainer::PutItem( SCItemSlot & IN slot, POSTYPE & OUT put_pos_out )
{
	if( !GetEmptyPos(put_pos_out) ) return FALSE;

	TradeSlot tradeSlot;
	tradeSlot.SetLinkItem( &slot );
	tradeSlot.SetNum(slot.GetNum());

	RC::eSLOT_INSERT_RESULT rt = SCSlotContainer::InsertSlot( put_pos_out, tradeSlot );
	if( rt == RC::RC_INSERT_SLOT_SUCCESS)
		return TRUE;
	else
		return FALSE;
}

BOOL TradeSlotContainer::IsEmpty( POSTYPE AtPos )
{
	return ( GetSlot(AtPos).GetSerial() == 0 );
}









