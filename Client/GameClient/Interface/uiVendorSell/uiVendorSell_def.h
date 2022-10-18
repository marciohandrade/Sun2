#ifndef     __UIVENDORSELL_DEFINE_H__
#define     __UIVENDORSELL_DEFINE_H__
//------------------------------------------------------------------------------
const int MAX_UIVENDORSELLDIALOG_SLOTS      = 24;   // 개인상점 판매창 슬롯수

//------------------------------------------------------------------------------
/**
    @class uiSCItemSlot

    SCItemSlot to extended class with money and fromPos infos

    (C) 2005 ckbang
*/
#include "SCItemSlot.h"
#include "BitStream.hpp"


enum 
{
	eVENDOR_END_STATE_TERMINATE,
	eVENDOR_END_STATE_PAUSE,
};


class uiSCItemSlot : public SCItemSlot
{
public:
    uiSCItemSlot(void) {m_Money=m_FromPos=0;}
	uiSCItemSlot( SCItemSlot & rItemSlot ) 
	{		
		ITEMSTREAMEX Stream;
		rItemSlot.CopyOut( Stream );
		SCItemSlot::Copy( Stream );
		m_Money=m_FromPos=0;
	}

    virtual ~uiSCItemSlot(void) {}

	MONEY	GetMoneyPerOne(); // 개당 가격.
    MONEY   GetMoney();
    void    SetMoney(MONEY m);
	

    int     GetFromPos();
    void    SetFromPos(POSTYPE pos);

	virtual void Copy( BitStream & IN bitstream )
	{
		bool lLock = false;
		bitstream.Read(&m_Money, 8*8);
		bitstream.Read(&lLock, 1);
		SCItemSlot::Copy(bitstream);	

		// 초기화
		m_FromPos = 0;
	}

	virtual void CopyOut( BitStream & OUT bitstream )
	{
		// 클라안씀
		bitstream.Write(&m_Money, 8*8);
        bool locked = IsLocked();
		bitstream.Write(&locked, 1);	
		SCItemSlot::CopyOut(bitstream);	
	}

private:
    MONEY   m_Money;
    POSTYPE m_FromPos;
};

//-------------------------------------------------------------------------------------------
/**
	개당 가격.
*/
inline
MONEY	
uiSCItemSlot::GetMoneyPerOne()
{
	MONEY Money = 0;
	if ( IsOverlap() )
	{
		Money = GetMoney() / GetNum();		
	}
	else
	{
		Money = GetMoney();
	}

	return Money;
}

//------------------------------------------------------------------------------
/**
*/
inline
MONEY
uiSCItemSlot::GetMoney()
{
	MONEY Money = 0;
	if ( IsOverlap() )
	{
		Money = m_Money * GetNum();		
	}
	else
	{
		Money = m_Money;
	}

	return Money;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSCItemSlot::SetMoney(MONEY m)
{
    this->m_Money = m;
}

//------------------------------------------------------------------------------
/**
*/
inline
int
uiSCItemSlot::GetFromPos()
{
    return this->m_FromPos;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSCItemSlot::SetFromPos(POSTYPE pos)
{
    this->m_FromPos = pos;
}

//------------------------------------------------------------------------------


#endif  //  __UIVENDORSELL_DEFINE_H__
