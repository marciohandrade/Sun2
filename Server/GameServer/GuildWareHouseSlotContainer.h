// WarehouseSlotContainer.h: interface for the CWarehouseSlotContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDWAREHOUSESLOTCONTAINER_H_)
#define AFX_GUILDWAREHOUSESLOTCONTAINER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SCItemSlotContainer.h>
#include <ResultCode.h>


#include <boost/pool/object_pool.hpp>

class Player;
class GameGuild;
struct _GUILDWAREHOUSE_TOTAL_INFO;

enum UPDATE_RESULT
{
	UPDATE_BY_CHNAGE_SUCCESS,
	UPDATE_NO_CHANGE,
	UPADTE_PLAYER_NONE,
	UPDATE_LONG_OPEN_STATE,
	UPDATE_ON_CLOSE_PALYER_DIFF,
	UPDATE_CANNOT_CONNECT_DBP,
};

enum WAREHOUSE_STATE
{
	WAREHOUSE_OPEN,
	WAREHOUSE_CLOSE,
};

class CGuildWarehouseSlotContainer : public SCItemSlotContainer
{
	
	static const int INVALID_AUTO_CLOSETIME = 0;

#ifdef _DEBUG
	enum { MAX_AUTO_CLOSE_TIMELIMIT = 3*60*1000 };			/* 1분 */
#else
	enum { MAX_AUTO_CLOSE_TIMELIMIT = 10*60*1000 };		/* 10분 */
#endif

public:
	CGuildWarehouseSlotContainer ();
	virtual ~CGuildWarehouseSlotContainer ();

	virtual RC::eSLOT_INSERT_RESULT InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );

	virtual void ClearAll();
	virtual BOOL	ValidState();
	virtual BOOL DeleteSlot( POSTYPE, SCSlot * );	
    // CHANGES: f101120.1L, fixed guild warehouse update problem
    // which is not applied on quantity changes by the item division move
    virtual void UpdateSlot(POSTYPE at_pos, int changed_num);
	virtual void OnInsert(SCSlot & IN);
	virtual void OnDelete(SCSlot & IN );

	
	void SerializeMoney(MONEY money) { m_Money = money; }
	void SetOwnerGuild(GameGuild* pGuild) { m_pOwnerGuild = pGuild; }
	
	BYTE GetSlotTabNum() { return m_SlotTabNum; }

	void Release();
	void CloseWarehouse();
	void SetSlotTabNum(BYTE);
	void OpenWarehouse();
	void OpenInit(Player*, BYTE, BOOL);
	void MoneyLogDBSave(MONEY, BYTE);
	void ItemLogDBSave(SCItemSlot*, BYTE);
	void SerializeItemInfo( BYTE* , WORD, eSERIALIZE); // 로그 기록용

	BOOL DepositMoney(MONEY);
	BOOL WithDrawMoney(MONEY);
	UPDATE_RESULT SaveWarehouseToDBP(Player*, eGUILDWAREHOUSE_UPDATE_FACTOR);

	BOOL IsOpened()	{ return m_bOpened; }
	BOOL IsChanged() { return m_bChanged; }
    // CHANGES: f110627.2L, prevent unauthorized invader's request.
	RC::eITEM_RESULT IsUseRight(Player* actor, bool withdrawal);
	RC::eITEM_RESULT IsValidPos(POSTYPE);
	RC::eITEM_RESULT ExtendWarehouseTabCount(Player*);
	RC::eGUILD_RESULT IsEnableService(GameGuild*);

	Player* GetUsedPlayer() { return m_pPlayer; }
	const MONEY&	GetCurMoney() { return m_Money; }

    // (f100527.4L)
	//template< class WAREHOUSE_TOTAL_INFO_TYPE >
	//void SerializeItemInfo(WAREHOUSE_TOTAL_INFO_TYPE & ItemInfo, eSERIALIZE eType);
    void SerializeItemInfo(_GUILDWAREHOUSE_TOTAL_INFO& ItemInfo, eSERIALIZE eType);

private:
	Player* m_pPlayer;
	GameGuild* m_pOwnerGuild;

	MONEY	m_Money;
	BOOL m_bOpened;
	BOOL m_bChanged;
	BYTE m_SlotTabNum;
	DWORD m_AutoCloseTime;
};

//==================================================================================================
// (f100527.4L) WAREHOUSE_TOTAL_INFO_TYPE = { _GUILDWAREHOUSE_TOTAL_INFO }
//template< class WAREHOUSE_TOTAL_INFO_TYPE >
//void CGuildWarehouseSlotContainer::SerializeItemInfo(WAREHOUSE_TOTAL_INFO_TYPE & ItemInfo, eSERIALIZE eType)


#endif // !defined(AFX_GUILDWAREHOUSESLOTCONTAINER_H_)
