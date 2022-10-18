// WarehouseSlotContainer.h: interface for the CWarehouseSlotContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAREHOUSESLOTCONTAINER_H__314B6ABE_55BB_4BD6_BF5F_CDA55ACA343A__INCLUDED_)
#define AFX_WAREHOUSESLOTCONTAINER_H__314B6ABE_55BB_4BD6_BF5F_CDA55ACA343A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SCItemSlotContainer.h>

#include <ResultCode.h>

class Player;
struct _WAREHOUSE_TOTAL_INFO;

class CWarehouseSlotContainer : public SCItemSlotContainer  
{
public:
	CWarehouseSlotContainer();
	virtual ~CWarehouseSlotContainer();

	// �⺻ �Լ� ������
	/*virtual VOID			UpdateSlot( POSTYPE AtPos, eSlotType type, DBSERIAL serial, SLOTCODE code, POSTYPE quickPos, DURATYPE dura, BYTE level, eUPDATE_BIT upbits = UB_ALL );
	virtual VOID			UpdateSlot( POSTYPE AtPos, SCSlot & IN rSlot );*/

	virtual RC::eSLOT_INSERT_RESULT InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );		// ���Կ� ��ü �߰�

	virtual BOOL					DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );	
	virtual VOID			ClearAll();


	virtual void			OnInsert( SCSlot & IN rSlot );
	virtual void			OnDelete( SCSlot & IN rSlot );

	// ����
	virtual BOOL			ValidState();
	VOID					StartWarehouse();
	VOID					EndWarehouse();

	inline void				SetStarted(bool v)	{ m_bStarted = v; }
    BOOL IsStarted() const 
    { 
        return m_bStarted; 
    }
    BOOL IsChanged() const 
    { 
        return m_bChanged; 
    }

	// SERIALIZE
	inline VOID				SerializeMoney( MONEY money ) { m_Money = money;	}

	// �α� ��Ͽ����� ����
	VOID					SerializeItemInfo( BYTE * pStream, WORD wSize, eSERIALIZE eType );

    //template<class WAREHOUSE_TOTAL_INFO_TYPE>
    void SerializeItemInfo(_WAREHOUSE_TOTAL_INFO& total_info, eSERIALIZE serialize_type);

	BOOL					SaveWarehouseToDBP();

	// �� ó��
	const MONEY& GetCurMoney() const 
    { 
        return m_Money; 
    }
	BOOL					WithDrawMoney( MONEY MinusMoney );
	BOOL					DepositMoney( MONEY PlusMoney );

	inline VOID				SetPlayer( Player * pPlayer ) { m_pPlayer = pPlayer; }
	BOOL					IsEnableService();
protected:
	Player *				m_pPlayer;
private:
	MONEY					m_Money;
	bool					m_bChanged;	//< �����Ͱ� ���ߴ��� ����
	bool					m_bStarted;	//< �����Ͱ� ���ߴ��� ����
};

//==================================================================================================
// (f100527.4L) WAREHOUSE_TOTAL_INFO_TYPE = { _WAREHOUSE_TOTAL_INFO }
//template<class WAREHOUSE_TOTAL_INFO_TYPE>
//void CWarehouseSlotContainer::SerializeItemInfo(WAREHOUSE_TOTAL_INFO_TYPE& total_info,
//                                                eSERIALIZE serialize_type)

#endif // !defined(AFX_WAREHOUSESLOTCONTAINER_H__314B6ABE_55BB_4BD6_BF5F_CDA55ACA343A__INCLUDED_)
