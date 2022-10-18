//=======================================================================================================================
/// TradeSlotContainer class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2004. 8. 11~
	@history
			- 2006. 3. 4 : trade관련 리펙토링
							
*/
//=======================================================================================================================
#pragma once

#include "SCSlotContainer.h"

class SCItemSlot;

// 제시 수정중, 제시완료후, 수락후
enum eTRADE_STATE { TRADE_READY, TRADE_PROPOSAL, TRADE_ACCEPT, };

class TradeSlotContainer : public SCSlotContainer  
{
public:
	TradeSlotContainer();
	virtual ~TradeSlotContainer();

	VOID				Init( Player * pPlayer, Player * pCounterPlayer, DWORD key );

	BOOL				PutItem( SCItemSlot & IN slot, POSTYPE & OUT put_pos_out );
	//BOOL				PutMoney( > & IN money );
	//BOOL				GetMoney( MONEY & IN money );
	
	inline DWORD		GetKey()							{ return m_dwKey;	}
	inline VOID			SetKey( DWORD dwKey )				{ m_dwKey = dwKey;	}
	inline eTRADE_STATE	GetTradeState()						{ return m_State;	}
	inline VOID			SetTradeState( eTRADE_STATE state ) { m_State = state; }
	inline Player *		GetCounterPlayer()					{ return m_pCounterPlayer;	}
	inline Player *		GetPlayer()							{ return m_pPlayer;	}
	inline const MONEY GetMoney()							{ return m_Money;	}
	inline VOID			SetMoney( const MONEY money )		{ m_Money = money;	}

	virtual BOOL		IsEmpty( POSTYPE AtPos );
	virtual eSlotType	GetSlotType() const					{ return ST_TRADE; }

private:
	virtual SCSlot *	CreateSlot();

private:
	DWORD				m_dwKey;
	eTRADE_STATE		m_State;
	MONEY				m_Money;
	Player *			m_pCounterPlayer;
	Player *			m_pPlayer;
};

