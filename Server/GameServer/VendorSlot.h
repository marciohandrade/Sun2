#pragma once
//=============================================================================================================================
/// VendorSlot class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2004. 12. 29
	@remark
		- SCSlot을 상속받아 개인상점 슬롯을 구현한 클래스
	@note
		- 
	@history 
		- 
*/
//=============================================================================================================================
#include <scslot.h>
#include <SCItemSlot.h>

class VendorSlot : public SCSlot
{
public:
	VendorSlot(void);
	virtual ~VendorSlot(void);

	virtual VOID			Clear(){ m_Money = 0; if(m_pItemSlot) m_pItemSlot->SetLock(FALSE);  m_pItemSlot = NULL; }
	virtual VOID			Copy( SCSlot & IN slot );
	VOID					CopyOut(VENDOR_ITEMSLOTEX & OUT ItemStream);
	VOID					Create( SCItemSlot & IN slot, const MONEY & money );//< 인벤토리 아이템의 정보를 copy
	
	virtual eSlotType		GetSlotType() const	{ return ST_VENDOR; }
	virtual DBSERIAL		GetSerial() const	{ if( !m_pItemSlot) return 0; return m_pItemSlot->GetSerial();}

	virtual DBWLDSERIAL		GetWorldSerial() const { if( !m_pItemSlot) return 0; return m_pItemSlot->GetWorldSerial();	}

	virtual SLOTCODE		GetCode() const		{ return m_pItemSlot->GetCode();}

	// 판매하고자하는 개수
	//virtual DURATYPE		GetSellNum() const	{ return m_nSellNum; }
	MONEY&					GetMoney()		{ return m_Money; }
    
    // (ADD)(20100324)(hukim)(STYLE) 위와 같은 경우 GetMoney()=xxx; 와 같은 코드가 작성 되었을 경우
    // 유효하므로 의미적으로 getter & setter이다. 따라서 읽기 전용 접근자를 별도로 추가해 줄 필요가
    // 있다.

    MONEY GetMoney() const 
    { 
        return m_Money; 
    }
    
	VOID SetLinkItem( SCItemSlot * pItem )		{ if(pItem) pItem->SetLock(TRUE); if(m_pItemSlot) m_pItemSlot->SetLock(FALSE);	m_pItemSlot = pItem; }
	SCItemSlot * GetLinkItem() const				{ return m_pItemSlot;	}

	inline BOOL				IsLocked() const		{ return m_bLocked;	}
	inline VOID				SetLock( BOOL val ) { m_bLocked = val;	}

	virtual void Copy( BitStream & IN stream );
	virtual void CopyOut( BitStream & OUT stream );
	
	virtual void Copy( BitStream & IN stream, eSLOT_SERIALIZE eType );
	virtual void CopyOut( BitStream & OUT stream, eSLOT_SERIALIZE eType );

private:
	SCItemSlot *	m_pItemSlot;	//< 인벤토리 아이템의 정보
	MONEY			m_Money;		//< 판매 가격
	//DURATYPE		m_nSellNum;		//< 판매하는 개수
	BOOL			m_bLocked;
};

