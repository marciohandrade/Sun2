#ifndef __MOUSE_HANDLER_H__
#define __MOUSE_HANDLER_H__

//=============================================================================================================================
///  ���콺 ó�� ��� Ŭ����
/**
	@author	Kim Min Wook < taiyo@webzen.co.kr >
	@since	2004. 1. 14
	@note
		- �������� ������ ����, ������ �巡�� & ��� ó��
*/
//=============================================================================================================================

#pragma once

#include <Singleton.h>
#include <SCSlotHeader.h>
#include "ItemUnitRenderer.h"

class SCSlot;
class ItemUnitRenderer;

#define MOUSE_ITEM_MAX	1
#define DEFAULT_MOUSE_ITEM_SERIAL	0xff

class MouseHandler : public util::Singleton<MouseHandler>
{
public:
	enum MOUSE_STATE
	{
		NONE = -1, 
		BUY = 0,		// BUY
		SELL,			// SELL
		REPAIR,			// REPAIR
		WITHDRAW,		// ���
		DEPOSIT,		// �԰�
		ENCHANT,		// ��, �� ��þƮ
		SOCKET_FILL,	// ���� ä���
		EXTRASTONE_USE,	// ����Ʈ�� ���� ���
		THROW_WASTE_BASKET, // ������ ������ (�Ұ�)
		SOCKET_REMOVE,
#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
		ADD_RANDOM_OPTION,
#endif

		RANK_DOWN,

		REPAIR_LIMITED,

		GUILD_WITHDRAW, //���â�� ���
		GUILD_DEPOSIT,  //���â�� �԰�

		ETHER_DEVICE_FILL,

		ITEM_STAT_CHANGE,

        RIDER_WAX, 

        ETHERIA_EXTRACT,
        ETHERIA_EXTRACT_CASTING,
        ETHERIA_COMBINATION,
        SOCKET_IDENTIFY,
        SOCKET_RECREATE,

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
        EXTEND_ITEMDATE,
#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM
        RANDOMIZE_ITEM,
#endif//_NA_003740_20111122_RANDOM_ITEM
        CRYSTALIZATION,
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		AWAKENING,
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM

		MAX,
	};

	class MouseItem
	{
	public:
		SLOTIDX							m_fromSlotIdx;
		POSTYPE							m_fromPos;
        SLOTIDX                         m_toSlotIdx;
        POSTYPE                         m_toPos;
        eSlotType						m_slotType;
		ItemUnitRenderer::RenderUnit *	m_pRenderUnit;
		RECT							m_OldRect;
		SCSlot *						m_pFromSlot;						// �̵��� Item slot
		const BASE_ITEMINFO *					m_pFromBaseItemInfo;				// �̵��� Item base ���� 
	};

public:

	MouseHandler();
	~MouseHandler();

	VOID				InitUnitRenderer( CDrawBase * pDraw );
	VOID				ReleaseUnitRenderer();

	VOID				InitConfirmState( void );

	VOID				Update();
	VOID				Render();

	BOOL				IsExistItemAtHand() { return m_MouseItem.m_pRenderUnit == NULL ? FALSE : TRUE ;	}
	MouseItem *			GetMouseItem()		{ return &m_MouseItem; }

	VOID				RollbackItemAtHand();
	VOID				AddItemAtHand( SLOTIDX fromSlotIdx, POSTYPE fromPos, eSlotType slotType, SLOTCODE code, ItemUnitRenderer::RenderUnit *	pRenderUnit, RECT * pRect );
    VOID				AddItemAtHandWithItemDiv( DURATYPE divNum,
                                                  SLOTIDX fromSlotIdx,
                                                  POSTYPE fromPos,
                                                  eSlotType slotType,
                                                  SLOTCODE code,
                                                  ItemUnitRenderer::RenderUnit *	pRenderUnit,
                                                  RECT * pRect );
	VOID				RemoveItemAtHand();

	// ������ �̵� / ��� ó�� ����
	BOOL				ItemTransaction( SLOTIDX SlotIdx, POSTYPE pos, RECT * pRect);
	BOOL				ItemUseTransaction( SLOTIDX SlotIdx, POSTYPE AtPos);
    BOOL				ItemUseTransactionFromInventory( SLOTIDX SlotIdx, POSTYPE AtPos );
    BOOL				ItemUseTransactionFromInventoryByItemTpye( SLOTIDX SlotIdx, POSTYPE AtPos );

protected:
	BOOL				_canUseTransaction( SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				ItemUseTransactionFromInventoryByChargeStyle( SLOTIDX SlotIdx, POSTYPE AtPos );
    BOOL				ItemUseTransactionFromInventoryByRiderItemParts( SLOTIDX SlotIdx, POSTYPE AtPos );
	BOOL				_useTransactionItemTopOfTrial(SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				UseTransationItemSSQ(SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				_useTransactionItemPortalQuest(SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				_useTransactionChargeItemPersonalWaypoint(SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				_useTransactionItemDuraRepair(SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				_useTransactionChargeItemPortalScroll(SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				_useTransactionChargeItemStatInit(SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				_useTransactionChargeItemTransact(SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				_useTransactionFateItemIdentify(SCItemSlot& rItemSlot, const BASE_ITEMINFO* pItemInfo, POSTYPE AtPos);

	BOOL				ItemUseTransactionFromQuick(SLOTIDX SlotIdx, POSTYPE AtPos);
	BOOL				ItemUseTransactionFromSkill(SLOTCODE SlotCode);
	BOOL				ItemUseTransactionFromStyle(SLOTIDX SlotIdx, POSTYPE AtPos);		

public:	
	// ������ ���հ��� 
	BOOL				ItemCompositeNormalEnchant(SLOTIDX SlotIdx, POSTYPE TargetPos);
#ifdef __NA_001358_ENCHANT_UNIFICATION	
#else //__NA_001358_ENCHANT_UNIFICATION
	BOOL				ItemCompositeNewEnchant(SLOTIDX SlotIdx, POSTYPE AtPos);
#endif //__NA_001358_ENCHANT_UNIFICATION

	BOOL				ItemCompositeRankUPTransction( SLOTIDX SlotIdx, POSTYPE AtPos, RECT * pRect);
	BOOL				ItemCompositeSocketFill(SLOTIDX SlotIdx, POSTYPE TargetPos);
	BOOL				ItemCompositeSocketRemove(SLOTIDX SlotIdx, POSTYPE TargetPos);
    BOOL				ItemCompositeExtraStoneUse(SLOTIDX SlotIdx, POSTYPE TargetPos);
    bool ItemCubeTransection(SLOTIDX slot_idx, POSTYPE to_position, RECT* pRect);
    bool ItemCubeInventoryRightClick(const SLOTIDX from_container_index, const SLOTIDX to_container_index, const POSTYPE from_postype);

    bool MouseHandler::ItemSocketCompositeTransection(SLOTIDX slot_idx, POSTYPE to_position, RECT* pRect);

    bool ItemCompositeNewEnchant_InventoryRightClick(const SLOTIDX from_container_index, 
                                                    const SLOTIDX to_container_index, 
                                                    const POSTYPE from_position_in_container, 
                                                    const POSTYPE to_position_in_container);
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	bool ItemEquipmentAwakening_InventoryRightClick(const SLOTIDX from_container_index,
		                                             const SLOTIDX to_container_index,
		                                             const POSTYPE from_position_in_container,
		                                             const POSTYPE to_position_in_container);
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	bool ItemEquipmentEvolution_InventoryRightClick(const SLOTIDX from_container_index, 
		                                             const SLOTIDX to_container_index,
		                                             const POSTYPE from_position_in_container,
		                                             const POSTYPE to_position_in_container);
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM

#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
	BOOL				ItemCompositeAddRandomOption(SLOTIDX SlotIdx, POSTYPE TargetPos);
#endif

	BOOL				ItemExtension(SLOTIDX SlotIdx, POSTYPE AtPos, RECT * pRect);
	BOOL				ItemRepairLimitedTransction(SLOTIDX SlotIdx, POSTYPE TargetPos);
	BOOL				ItemEtherDeviceFillTransction(SLOTIDX SlotIdx, POSTYPE AtPos);
    BOOL				ItemRiderWaxChangeTransction(SLOTIDX SlotIdx, POSTYPE AtPos);

    bool ItemEtheriaCombinationTransction(SLOTIDX slot_index, POSTYPE to_pos);
    bool ItemEtheriaExtractTransction(POSTYPE slot_pos);
    void SetEtheriaExtractState();


    void SetCharCustomizeDlg(WORD item_type, POSTYPE slot_pos);
	BOOL				ItemCompositeEnchants(SLOTIDX SlotIdx, POSTYPE AtPos, RECT * pRect);
	BOOL				CheckBlockedItem( SCItemSlot * pSlot);		

//	BOOL				DummyTransaction( SLOTIDX SlotIdx, POSTYPE pos, RECT * pRect);

	// ��þƮ ����
	VOID				SetCrystalPos(POSTYPE CrystalPos) { m_CrystalPos = CrystalPos; }
	POSTYPE				GetCrystalPos() { return m_CrystalPos; }
	VOID				SetPendingTargetPos(POSTYPE TargetPos) { m_PendingTargetPos = TargetPos; }
	POSTYPE				GetPendingTargetPos() { return m_PendingTargetPos; }

	// ������ ����
	VOID				LockWasteDeleteItem() { m_bPendingLockDeleteItem = TRUE; }
	VOID				UnLockWasteDeleteItem() { m_bPendingLockDeleteItem = FALSE; }
	BOOL				IsLockWasteDeleteItem() { return m_bPendingLockDeleteItem; }

	MOUSE_STATE			GetMouseState() const;
	VOID				SetMouseState(MOUSE_STATE eShop);
	VOID				RemoveMouseState(MOUSE_STATE eShop);
    bool ItemSocketRecreateTransction(POSTYPE slot_pos);
    void SetRecreateSocketState();
    bool ItemSocketIndentifyTransction(POSTYPE TargetPos);
    void SetIdentifyPosition(POSTYPE identify_position) { identify_position_ = identify_position_; }
    POSTYPE GetIdentifyPosition() { return identify_position_; }
    void SetSocketIdentifyState();
    bool SocketCompositeInventoryRightClick(const SLOTIDX from_container_index, const SLOTIDX to_container_index, const POSTYPE from_postype);
    void SetCrystalizationState();
    bool ItemCrystalizationTransction(POSTYPE TargetPos);

	int					GetSocketRemoveType() { return m_iSocketRemoveType; }
	void				SetSocketRemoveType(int val) { m_iSocketRemoveType = val; }
	void				SetPendingSocketItemPos(POSTYPE pos) { m_iSocketItemPos = pos; }
	POSTYPE				GetPendingSocketItemPos() { return m_iSocketItemPos;}
	void				SetPendingSkillItemPos(POSTYPE pos) { m_iSkillItemPos = pos; }
	POSTYPE				GetPendingSkillItemPos() { return m_iSkillItemPos; }
    BOOL                GetPendingCtrlKeyDown() { return m_bCtlKeyDown; }


	SLOTIDX				GetSendSlotIdx() const { return m_SendSlotIdx; }
	POSTYPE				GetSendAtPos() const { return m_SendAtPos; } 	 	
	void				GetPossessionItemPOS(POSTYPE& FromPos, POSTYPE& ToPos) { FromPos = m_PossessionItemFromPos; ToPos = m_PossessionItemToPos; }
	void				SetPossessionItemPOS(POSTYPE FromPos, POSTYPE ToPos) { m_PossessionItemFromPos = FromPos; m_PossessionItemToPos = ToPos; }
	void				SetPossessionWin(bool bIsOpen){ m_bIsProssessionWin = bIsOpen; }
	bool				GetPossessionWin(void){ return m_bIsProssessionWin; }


	void				GetRandomItemPOS(POSTYPE& FromPos) { FromPos = m_RandomItemFromPos; }
	void				SetRandomItemPOS(POSTYPE FromPos) { m_RandomItemFromPos = FromPos; }
	void				SetRandomWin(bool bIsOpen){ m_bIsRandomWin = bIsOpen; }
	bool				GetRandomWin(void){ return m_bIsRandomWin; }

	void SetDivideItemOff();

protected:
	VOID				initMouseItem();

	VOID				refreshContainer(SLOTIDX SlotIdx);

private:
    MouseItem           m_MouseItem;

	MOUSE_STATE			m_eTradeState;
	POSTYPE				m_CrystalPos;	// ��� ��þƮ�� �ʿ��� ���� ��ġ
    POSTYPE				identify_position_;	// ���� Ȯ�� ������ ��ġ
	POSTYPE				m_PendingTargetPos;
	ItemUnitRenderer*	m_pItemUnitRender;
	BOOL				m_bPendingLockDeleteItem;
	WzID				m_WasteBasketDialogID;
	int					m_iSocketRemoveType;
	POSTYPE				m_iSocketItemPos;	
	POSTYPE				m_PossessionItemFromPos;
	POSTYPE				m_PossessionItemToPos;
	bool				m_bIsProssessionWin;

	POSTYPE				m_RandomItemFromPos;
	bool				m_bIsRandomWin;
	POSTYPE				m_iSkillItemPos;
    BOOL                m_bCtlKeyDown;

	SLOTIDX				m_SendSlotIdx;	// ����� �������� �ε��� �˾��ÿ� ����� �ӽ� �����
	POSTYPE				m_SendAtPos;	 	

	__PROPERTY( BOOL, PendingRepairAll );
	__PROPERTY( POSTYPE, PendingRepairSlotIdx );
	__PROPERTY( POSTYPE, PendingRepairPos );

	__PROPERTY( POSTYPE, PendingSavePositionPos ); // ��ġ ������ ������ ��ġ ( �κ��丮 )
	__PROPERTY( BOOL, SavingPosition);
	__PROPERTY( BOOL, PendingUseCoordPortalPos);
	__PROPERTY( BOOL, TryPortal);

	__PROPERTY( POSTYPE, PendingSocketRemovePos );
	__PROPERTY( BOOL, TryRemoveSocket);
#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
	__PROPERTY( POSTYPE, PendingAddRandomOptionPos );
	__PROPERTY( BOOL, TryAddRandomOption);
#endif

	__PROPERTY( POSTYPE, PendingUseChargeItemPos );	// ����� ������ ��ġ.
	__PROPERTY( BOOL, TryUseChageItem);				// ����Ϸ��� ������ ��ġ.
	

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
private:
    POSTYPE m_UseWasteItemPosition;
public:
    void ExtendItemDateTransction( const POSTYPE& TargetPosition );
    void SetUseExtendItemDateMode( const POSTYPE& ItemPosition );
#endif // _NA_0_20111012_ITEM_DATE_EXTEND

    private:
        POSTYPE m_RandomizeItemPosition;
public:
    void RandomizeTransction( const POSTYPE& TargetPosition );
    void SetRandomizeItemMode( const POSTYPE& ItemPosition );
};

#endif // __MOUSE_HANDLER_H__