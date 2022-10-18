#pragma once
//=============================================================================================================================
/// VendorSlot class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2004. 12. 29
	@remark
		- SCSlot�� ��ӹ޾� ���λ��� ������ ������ Ŭ����
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
	VOID					Create( SCItemSlot & IN slot, const MONEY & money );//< �κ��丮 �������� ������ copy
	
	virtual eSlotType		GetSlotType() const	{ return ST_VENDOR; }
	virtual DBSERIAL		GetSerial() const	{ if( !m_pItemSlot) return 0; return m_pItemSlot->GetSerial();}

	virtual DBWLDSERIAL		GetWorldSerial() const { if( !m_pItemSlot) return 0; return m_pItemSlot->GetWorldSerial();	}

	virtual SLOTCODE		GetCode() const		{ return m_pItemSlot->GetCode();}

	// �Ǹ��ϰ����ϴ� ����
	//virtual DURATYPE		GetSellNum() const	{ return m_nSellNum; }
	MONEY&					GetMoney()		{ return m_Money; }
    
    // (ADD)(20100324)(hukim)(STYLE) ���� ���� ��� GetMoney()=xxx; �� ���� �ڵ尡 �ۼ� �Ǿ��� ���
    // ��ȿ�ϹǷ� �ǹ������� getter & setter�̴�. ���� �б� ���� �����ڸ� ������ �߰��� �� �ʿ䰡
    // �ִ�.

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
	SCItemSlot *	m_pItemSlot;	//< �κ��丮 �������� ����
	MONEY			m_Money;		//< �Ǹ� ����
	//DURATYPE		m_nSellNum;		//< �Ǹ��ϴ� ����
	BOOL			m_bLocked;
};

