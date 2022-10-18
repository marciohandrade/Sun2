
//=======================================================================================================================
/// EventInventorySlotContainer
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2006. 1. 19
	@remark
			- 이벤트 아이템을 가져 오기 위한 인벤토리를 나타낸다.
	@implement
			- 인벤토리에서 이벤트 인벤토리 버튼을 누르면, Open() 함수를 호출하여 DB로부터 이벤트 아이템을 쿼리한다.
				이벤트 아이템스트림 클러스터를 받고 나서(이용할수있는상태로변경) 다른 트랜젝션을 수행할수있다.
	@todo
			- 인벤트아이템코드를 실제아이템 코드로 변경하는 작업을 자동적으로 처리하게!
			- 클라이언트와 같이 사용할 수 있게
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
	// 이벤트 인벤토리를 열때
	RC::eITEM_RESULT 	Open();
	VOID				Serialize( EVENT_ITEM_TOTAL_INFO & rTotalInfo, eSERIALIZE se );
private:
	
	VOID				Close();
	inline BOOL			IsEventTransaction() { return (EVENT_INVEN_TRANS_BEGIN & m_eEventIvenTransaction);	}
	inline VOID			SetEventTransaction(eEVENT_INVENTORY_TRANSACTION stat) { m_eEventIvenTransaction=stat;	}
public:
	// 이벤트 인벤토리에서 새로고침을 누를때
	RC::eITEM_RESULT	Select();

	VOID				OnSelect( BOOL bSuccess, EVENT_ITEM_TOTAL_INFO_EX * IN pTotalInfo );
	// 이벤트 아이템을 인벤토리로 가져올 때
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
