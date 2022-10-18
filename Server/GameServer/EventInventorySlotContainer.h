
//=======================================================================================================================
/// EventInventorySlotContainer
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 1. 19
	@remark
			- �̺�Ʈ �������� ���� ���� ���� �κ��丮�� ��Ÿ����.
	@implement
			- �κ��丮���� �̺�Ʈ �κ��丮 ��ư�� ������, Open() �Լ��� ȣ���Ͽ� DB�κ��� �̺�Ʈ �������� �����Ѵ�.
				�̺�Ʈ �����۽�Ʈ�� Ŭ�����͸� �ް� ����(�̿��Ҽ��ִ»��·κ���) �ٸ� Ʈ�������� �����Ҽ��ִ�.
	@todo
			- �κ�Ʈ�������ڵ带 ���������� �ڵ�� �����ϴ� �۾��� �ڵ������� ó���ϰ�!
			- Ŭ���̾�Ʈ�� ���� ����� �� �ְ�
	@history
			- 
*/
//=======================================================================================================================

#pragma once

#include <scitemslotcontainer.h>
#include "ItemManager.h"

enum eEVENT_INVENTORY_STATE
{
	EVENT_INVEN_STATE_UNAVAILABLE,
	EVENT_INVEN_STATE_AVAILABLE,
};
enum eEVENT_INVENTORY_TRANSACTION
{
	EVENT_INVEN_TRANS_END					= (1<<0),

	EVENT_INVEN_TRANS_BEGIN					= (1<<1),
	EVENT_INVEN_TRANS_BEGIN_SELECT			= ( EVENT_INVEN_TRANS_BEGIN | (1<<2) ),
	EVENT_INVEN_TRANS_BEGIN_MOVE_TO_INVEN	= ( EVENT_INVEN_TRANS_BEGIN | (1<<3) ),
	
};

class EventInventorySlotContainer :
	public SCItemSlotContainer
{
public:
	EventInventorySlotContainer(void);
	virtual ~EventInventorySlotContainer(void);

	virtual eSlotType	GetSlotType() const					{ return ST_EVENT; }
	virtual SCSlot *	CreateSlot();
	virtual VOID		ClearAll();

	// custom
	VOID				InsertSlot( POSTYPE AtPos, EVENTSLOTEX & IN rSlotInfo );
public:
	// �̺�Ʈ �κ��丮�� ����
	RC::eITEM_RESULT 	Open();
	VOID				Serialize( EVENT_ITEM_TOTAL_INFO & rTotalInfo, eSERIALIZE se );
private:
	
	VOID				Close();
	inline BOOL			IsEventTransaction() { return (EVENT_INVEN_TRANS_BEGIN & m_eEventIvenTransaction);	}
	inline VOID			SetEventTransaction(eEVENT_INVENTORY_TRANSACTION stat) { m_eEventIvenTransaction=stat;	}
public:
	// �̺�Ʈ �κ��丮���� ���ΰ�ħ�� ������
	RC::eITEM_RESULT	Select();

	VOID				OnSelect( BOOL bSuccess, EVENT_ITEM_TOTAL_INFO_EX * IN pTotalInfo );
	// �̺�Ʈ �������� �κ��丮�� ������ ��
	RC::eITEM_RESULT	MoveItemToInventory( POSTYPE AtPos, DWORD & OUT ItemSeq );
	VOID				OnMoveItemToInventory( BOOL bSuccess, POSTYPE atPos, JUST_INVENTORY_TOTAL_INFO * OUT pTotalInfoOut );
	inline VOID			SetPlayer( Player * pPlayer ) { m_pPlayer = pPlayer; }
protected:
	Player *			m_pPlayer;
private:
	eEVENT_INVENTORY_STATE			m_eEventIvenState;
	eEVENT_INVENTORY_TRANSACTION	m_eEventIvenTransaction;
	static const int MAX_LOCKED_INFO_NUM = MAX_INVENTORY_SLOT_NUM;
	EVENTSLOT			m_TransactionInfo;
	LOCKED_INFO			m_TransLockedInfo[MAX_LOCKED_INFO_NUM];
	BYTE				m_LockedCount;
};
