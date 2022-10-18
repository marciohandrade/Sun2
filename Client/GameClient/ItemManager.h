#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

//=======================================================================================================================
// �������� �������� ó���� ����ϴ� �޴��� Ŭ����
/**
	@author
			�� �� ��( y2jinc@webzen.co.kr ) 
	@since
			2005 1. 2
	@remarks
			 - ���� ������ �̵�ó��, ���ó��, �������� �޼��� ó��
			 - ��Ʈ��ũ ��Ŷ �Ľ� ó��
			 - ItemUseTransaction �߰�
			 - Itemcombine
			 - Item Equip
			 - Item move
			 - Item drop
			 - Item pick up
			 - Quick slot Link / Unlink 
			 - Style Quick slot Link / Unlink
*/
//=======================================================================================================================

#pragma once


#include <Singleton.h>
#include <SolarHashTable.h>
#include <ItemOptionHeader.h>
#include <PlayerAttributes.h>
#include <SCSlotHeader.h>
#include "ResultCode.h"

using namespace RC;

struct MSG_BASE;
class SCSlotContainer;
class SolarDialog;
class SCSkillSlot;
class SCItemSlot;
class ItemType;
class PlayerAttributes;
class Item;

#define __DECLARE_GET_ITEM_ATTR_VALUE( attr, type, PlayerAttr )	\
	public:													    \
		type		Get##attr(PlayerAttr * pPlayerAttr) const	\
  		{                                                       \
			if ( pPlayerAttr )	                                \
				return pPlayerAttr->GetBase##attr() + pPlayerAttr->GetItem##attr();	\
			else	                                            \
				return 0;	                                    \
		} 

enum eINSERTTOTALTYPE
{
	BUY,
	PICK,
	CREATE,
	REWARD,
	EVENT,	// �̺�Ʈ �κ��丮���� �����Դ�.
	QUEST_OBTAIN,
	PRESENT,
    POINT_REWARD,   // ��������Ʈ ���� //_NA_1419_20100121_ACCUMULATE_POINT
    CUBE,   //ť��
    SOCKET_COMPOSTE, //����ռ�

    SHOP_STORAGE,   //!< ���ռ�

	NONE,	//�޽�������
};

enum eDELETETOTALTYPE
{
	DELETE_NONE = -1,
	QUEST_DELETE = 0,
	COMPOSE_USE,
	DELETE_STAT_INIT_ITEM,	// ���� �ʱ�ȭ �������� ���.
};


enum eSLOT_SOUND_TYPE
{
	eSLOT_SOUND_TYPE_PICK,		// ���� ��
	eSLOT_SOUND_TYPE_PUT,		// ���� ��
	eSLOT_SOUND_TYPE_EQUIP,		// ����
	eSLOT_SOUND_TYPE_UNEQUIP,	// Ż��
	eSLOT_SOUND_TYPE_LINK,		// ��ũ
	eSLOT_SOUND_TYPE_UNLINK,	// ��ũ
	eSLOT_SOUND_TYPE_DUMP,		// ������

	eSLOT_SOUND_TYPE_NUM,
};

enum eKIND_OF_EQUIPED_SETITEM
{
	eKIND_OF_EQUIPED_SETITEM_NONE = -1,
	eKIND_OF_EQUIPED_SETITEM_ELITE = 0,	 // ����Ʈ ������
	eKIND_OF_EQUIPED_SETITEM_GENERAL = 1,  // �Ϲ�
	eKIND_OF_EQUIPED_SETITEM_DIVINE = 2, // �����
	eKIND_OF_EQUIPED_SETITEM_UNIQUE = 3, // ����ũ
	eKIND_OF_EQUIPED_SETITEM_LUCKY  = 4, // ��Ű
	eKIND_OF_EQUIPED_SETITEM_SET,
};


class ItemManager : public util::Singleton<ItemManager>
{
public:
	ItemManager();
	~ItemManager();

	BOOL								Init();
	VOID								Release();
	VOID								Render();
	VOID								Update();

	// Get Funcs
    SCItemSlot* GetInventoryItem( POSTYPE slot_index );
    
	SCSlotContainer *					GetContainer( SLOTIDX ContainerIndex );
	SolarDialog *						GetItemContainerDialog(SLOTIDX ContainerIndex);
    const SCSkillSlot *                 GetSkillSlotByCode(CODETYPE code);
	BOOL								GetEmptySlotPos(SLOTIDX AtSlotIdx, POSTYPE & OUT rPosOut);
	BOOL								GetEquipPosition(SCSlot & rSlot, POSTYPE & OUT rEqPosOut);

	DWORD								GetCompositeNameID(eITEMCOMPOSITECODE eCode);

	WORD								GetItemCount(SLOTIDX AtSlotIdx, CODETYPE ItemCode);
    //------------------------------------------------------------------------------
    // Returns :NUMTYPE : ����
    // Qualifier : �ش� �����̳ʿ� �ִ� ���� ������ ���� ����
    // Parameter : SLOTIDX AtSlotIdx : �����̳� �ε���
    // Parameter : eITEMTYPE item_type : ������ Ÿ��
    // Parameter : eWASTETYPE waste_type : ������ ���� Ÿ��
    //------------------------------------------------------------------------------
    WORD                                GetItemCount(SLOTIDX AtSlotIdx, eITEMTYPE item_type, eWASTETYPE waste_type);

	BOOL								IsEmptySlotPos(SLOTIDX AtSlotIdx, POSTYPE Pos);

	// sound	
	DWORD								GetItemSound(eSLOT_SOUND_TYPE eSoundType, SCItemSlot * pItemSlot);
	DWORD								GetSkillSound(eSLOT_SOUND_TYPE eSoundType, SCSkillSlot * pSkillSlot);
	VOID								PlaySlotSound( eSLOT_SOUND_TYPE eSoundType, SCSlot * pSlot );

	// 
	CODETYPE							InsertInventoryTotalInfo ( eINSERTTOTALTYPE eType, INVENTORY_TOTAL_INFO & rTotalInfo);
	BOOL								InsertJustInventory(eINSERTTOTALTYPE eType, JUST_INVENTORY_TOTAL_INFO & rTotalInfo);
	BOOL								DeleteInventoryTotalInfo ( eDELETETOTALTYPE eType, SLOTIDX AtSlotIdx, INVEN_DELETE_TOTAL_INFO & rDeleteTotalInfo);
	
	BOOL								InsertEventTotalInfo( EVENT_ITEM_TOTAL_INFO & rTotalInfo);	
	BOOL								IsExistEmptySlotFromInventoryTotal();
    
	// Ŭ���̾�Ʈ �׽�Ʈ�� ���� �Լ�
	BOOL								Test_InsertItemAtInventory( CODETYPE ItemCode, int ItemNum );

	// ������ ó�� ���� �Լ�
	VOID								RepairItem( SLOTIDX AtSlotIdx, POSTYPE pos );
	VOID								RepairItemAll( SLOTIDX AtSlotIdx );
	MONEY                               GetPriceRepairItemAll( SLOTIDX AtSlotIdx );
	BOOL	                            IsExistRepairItem( SLOTIDX AtSlotIdx );
    bool	                            IsExistRepairItem(SLOTIDX AtSlotIdx, DURATYPE less_dura);
	BOOL	                            IsExistLimitedItem( SLOTIDX AtSlotIdx );

	
	// ��Ŷ ó�� �κ� 
	VOID								ParsePacket( MSG_BASE * pMsg );
	VOID								OnRecvItemSync(MSG_BASE * pMsg );
	VOID								OnrecvItemComposite(MSG_BASE * pMsg);
	VOID								OnrecvItemEnchant(MSG_BASE * pMsg);
	VOID								OnRecvStyleLink(MSG_BASE * pMsg );

	/// Itemó�� �κ�
    VOID                                MoveItem(SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos, DURATYPE itemCount);
    VOID								MoveItemWithDiv();

    //! �ڵ� ��� �����̳ʿ� ������ ��ũ
    void AutobotLinkItem(SLOTIDX from_container_index, POSTYPE from_pos, POSTYPE to_pos, SLOTCODE org_code = 0);
    void AutobotLinkSkill(POSTYPE to_pos, SLOTCODE org_code);

	// QuickSlot ó��
	VOID								QuickLinkItem( SLOTIDX atIndex, POSTYPE OrgPos, POSTYPE toQuickPos, SLOTCODE OrgCode = 0);
	VOID								QuickLinkSkill( SLOTIDX atIndex, SLOTCODE OrgCode, POSTYPE toQuickPos ); 
	// StyleSlot ó��
	VOID								StyleLink(SLOTCODE OrgCode, POSTYPE toQuickPos ); 
	// Quick/Style ���� ó��
	VOID								SimplexMoveLinkItem( SLOTIDX AtIndex, POSTYPE fromPos, POSTYPE toQuickPos );

	// Dumm Item ó�� �κ�
	VOID								LinkDummyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos );								


	VOID								TradeBuyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos );
	VOID								TradeSellItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos );

	VOID								UseInventoryItem( SLOTIDX AtIndex, POSTYPE AtPos );
	VOID								UseEquipmentItem( SLOTIDX AtIndex, POSTYPE AtPos );


    void IncItemCount( SLOTIDX AtIndex, POSTYPE AtPos, int Count );

	VOID								CopyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos );

	// ������ composite ���� ó��
	BOOL								SpendCompositeMaterials(
											eITEMCOMPOSITECODE eItemCompositeType, 
											SLOTIDX	fromSlotIndex
										);

	BOOL								AutoRegisterCompositeMaterials(
											eITEMCOMPOSITECODE eItemCompositeType, 
											SLOTIDX	atSlotIndex = SI_ITEM_ETC_COMPOSE,
											SLOTIDX	fromSlotIndex = SI_INVENTORY
											);	

	// Rank ����
	eRANK_LEVEL							GetCompRankLevel(SCItemSlot & rItemSlot );
	// Enchant ����
	eENCHANTLEVEL						GetCompEnchantLevel(SCItemSlot & rItemSlot );
	// Item ���� ó��

	BOOL								CanMoveEquipItem( SLOTIDX AtIndex, POSTYPE AtPos, BOOL bOutputMsg = FALSE );
	BOOL								CanMoveUnEquipItem( SLOTIDX AtIndex, POSTYPE AtPos, BOOL bOutputMsg = FALSE );
	BOOL								CanUnEquipItem( POSTYPE ToPos, BOOL bOutputMsg);

	BOOL								CanEquip( SLOTIDX AtIndex, POSTYPE AtPos, POSTYPE ToPos, BOOL bOutputMsg = TRUE, BOOL bTransformCheck = TRUE );
	BOOL								CanEquipClass( Player * pPlayer, const BASE_ITEMINFO * pInfo ) const;
	BOOL								CanEquipLimit( SCItemSlot & rItemSlot, BOOL bOuputMsg);
    BOOL                                CanEquipByGender(Player* pPlayer, const BASE_ITEMINFO* pInfo);

    BOOL                                ValidLimitAll( SHORT Player_Limit, SHORT Item_Limit ) const;
	BOOL								ValidLimitStr( SCItemSlot & ItemSlot ) const;
	BOOL								ValidLimitDex( SCItemSlot & ItemSlot ) const;
	BOOL								ValidLimitInt( SCItemSlot & ItemSlot ) const;
	BOOL								ValidLimitLevel( SCItemSlot & ItemSlot ) const;
	BOOL								ValidLimitSkillExp1( SCItemSlot & ItemSlot ) const;
	BOOL								ValidLimitSkillExp2( SCItemSlot & ItemSlot ) const;
	BOOL								ValidLimitVit( SCItemSlot & ItemSlot ) const;
	BOOL								ValidLimitSpr( SCItemSlot & ItemSlot ) const;

	//BOOL								CanTradeItem( BASE_ITEMINFO * pInfo, int iAction ) const;	// Ŭ���̾�Ʈ������ ������ �ŷ� Ÿ�Կ� ���� üũ ����

	__DECLARE_GET_ITEM_ATTR_VALUE(STR, int, PlayerAttributes);
	__DECLARE_GET_ITEM_ATTR_VALUE(DEX, int, PlayerAttributes);
	__DECLARE_GET_ITEM_ATTR_VALUE(INT, int, PlayerAttributes);
	__DECLARE_GET_ITEM_ATTR_VALUE(Experty1, int, PlayerAttributes);
	__DECLARE_GET_ITEM_ATTR_VALUE(Experty2, int, PlayerAttributes);
	__DECLARE_GET_ITEM_ATTR_VALUE(VIT, int, PlayerAttributes);
	__DECLARE_GET_ITEM_ATTR_VALUE(SPR, int, PlayerAttributes);	

	BOOL								CanLinkQuick( SLOTIDX AtIndex, POSTYPE ToPos, const SCSlot * IN pSlot);
	BOOL								CanLinkStyleQuick( SLOTIDX AtIndex, POSTYPE ToPos, const SCSlot * IN pSlot);

	//BOOL								CanUse( SLOTIDX AtIndex, POSTYPE AtPos);
	
	// Item Composite ���� 
	BOOL								IsExistCompositeMaterials(
										eITEMCOMPOSITECODE eItemCompositeType,
										SLOTIDX fromSlotIndex);
    BOOL								IsExistItem(SLOTIDX from_slot_index, 
                                                    CODETYPE item_code, 
                                                    int item_num, 
                                                    ItemType* item_type_ptr = NULL, 
                                                    DWORD item_type_index = 0);
    int                                 GetSlotIndex(SLOTIDX FromSlotIdx, eWASTETYPE type);

    BOOL								SpendItem(SLOTIDX from_slot_index, 
                                                  CODETYPE item_code, 
                                                  int item_num, 
                                                  ItemType* item_type_ptr = NULL, 
                                                  DWORD item_type_index = 0);
	BOOL								DeleteItem(SLOTIDX FromSlotIdx, POSTYPE FromPos, int iItemNum, eDELETETOTALTYPE eDeleteType = DELETE_NONE);
	BOOL								DeleteItemCmd(SLOTIDX FromSlotIdx, POSTYPE FromPos, int iItemNum, eITEM_DELETE_RESULT eDeleteType);
	
	// �������̳� ����
	int									GetOverlapItemCount( CODETYPE code, SLOTIDX AtIndex = SI_INVENTORY);
	BOOL								FindFirstItem( CODETYPE code, POSTYPE & OUT rOutPos, SLOTIDX AtIndex = SI_INVENTORY);
	BOOL								FindFirstItem( eITEMTYPE eItemTypeInfo, POSTYPE & OUT rOutPos, SLOTIDX AtIndex = SI_INVENTORY);
    VOID UpdateQuickInfo(eSlotContainerIndex container_index = SI_QUICK);
    //------------------------------------------------------------------------------
    // Returns :SCItemSlot*
    // Qualifier : 
    // Parameter : SLOTIDX AtSlotIdx : �����̳� �ε���
    // Parameter : eITEMTYPE item_type : ������ Ÿ��
    // Parameter : eWASTETYPE waste_type : ������ ���� Ÿ��
    // Parameter : SLOTCODE item_code : ������ �ڵ�
    // Parameter : NUMTYPE less_count : �ش� ���� ���� ���� ���� ( �ڿ��� ���� �˻��Ѵ�)
    //------------------------------------------------------------------------------
    SCItemSlot* FindFirstItemSlot(SLOTIDX AtSlotIdx, eITEMTYPE item_type);
    SCItemSlot* FindFirstItemSlot(SLOTIDX AtSlotIdx, eITEMTYPE item_type, eWASTETYPE waste_type);
    SCItemSlot* FindFirstItemSlot(SLOTIDX AtSlotIdx, SLOTCODE item_code);
    SCItemSlot* FindFirstItemSlot(SLOTIDX AtSlotIdx, SLOTCODE item_code, NUMTYPE less_count);

	BOOL								FindActiveChargeItem(eITEMTYPE eItemTypeInfo, POSTYPE & OUT rOutPos, SLOTIDX AtIndex = SI_INVENTORY);

	//--
	// �Ʒ� �Լ��� ����� ���̴�.
	VOID								UpdateQuickForEquipItemAll();
	VOID								UpdateValidQuickItem();
	//--

public:
	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ ���� ����
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �ʵ� ������ ����
	VOID								SendPickupItem(DWORD ItemObjectKey);
	VOID								SendPickupMoney(DWORD ItemObjectKey);
    void                                SendPickUpItemObject(Item* item_object);
	VOID								SendDropItemMsg( SLOTIDX FromIdx, POSTYPE FromPos);
	
	//////////////////////////////////////////////////////////////////////////
	// ������ ���� ü���� ������
	VOID								SetIndexPosInStatChangePkt(SLOTIDX Idx, POSTYPE Pos);
	VOID								SetChangeItemPosInStatChangePkt(POSTYPE ChangeItemPos);
	VOID								SendStatChangeItem();

private:
	MSG_CG_ITEM_STAT_CHANGE_USE_SYN		m_Item_Stat_Change_Use_Syn;

    SLOTIDX                             m_verify_dropFromIdx;
    POSTYPE                             m_verify_dropFromPos;

public:
    VOID                                VerifiedSendDropItemMsg();

	// ������ ����
	VOID								SendDeleteItemMsg( POSTYPE FromPos );

	
	BOOL								AddFieldItem( const ITEM_RENDER_INFO * pItemInfo );
	void								AddItemForSingleMode( DWORD dwObjectKey, DWORD dwID );
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// ������ �ŷ� ����
    VOID								SendSellMsg(SLOTIDX atIndex, 
                                                    POSTYPE atPos, 
                                                    DURATYPE ItemCount = 0, 
                                                    eENCHANTLEVEL popup_enchant_level = ENCHANT_MIDDLE);
    VOID								SendBuyMsg(DWORD dwShopListID, 
                                                    SLOTCODE ItemCode, 
                                                    BYTE byTabIndex, 
                                                    POSTYPE ItemPos, 
                                                    DURATYPE ItemCount = 0);
	//////////////////////////////////////////////////////////////////////////
	// ������ �̵�
    VOID								SendItemMoveMsg( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos, DURATYPE ItemCount = 0);
private:
    // �ӽ� �ڵ� ������ ��
    SLOTIDX     div_fromIdx;
    SLOTIDX     div_toIdx;
    POSTYPE     div_fromPos;
    POSTYPE     div_toPos;
    DURATYPE    div_fromNum;
    DURATYPE    div_toNum;
public:
    VOID                                SendItemMoveMsgWithDiv(SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos, DURATYPE fromNum, DURATYPE toNum );

    //////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// ������ ����
	VOID								SendItemRepairMsg( SLOTIDX AtIdx, POSTYPE Pos, bool bAllRepair = false );
	//////////////////////////////////////////////////////////////////////////
	// ������ ����.
	VOID								SendQuickItemLinkMsg(POSTYPE OrgPos, POSTYPE ToPos);
	VOID								SendQuickSkillLinkMsg(SLOTCODE OrgCode, POSTYPE ToPos);
	VOID								SendSimplexMoveLinkMsg(POSTYPE fromPos, POSTYPE ToQuickPos);
	VOID								SendQuickItemRemoveMsg(POSTYPE AtPos );
	// ��Ÿ�� ����.
	VOID								SendQuickStyleLinkMsg(SLOTCODE OrgCode, POSTYPE ToPos);
	VOID								SendQuickStyleMoveLinkMsg(POSTYPE fromPos, POSTYPE ToPos);
	VOID								SendQuickStyleRemoveMsg(POSTYPE AtPos );

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// â�� ����.
	VOID								SendWareHouseItemAllSyn();
	//////////////////////////////////////////////////////////////////////////
	// ������ ���
	VOID								SendItemUseMsg(SLOTIDX	atIndex, POSTYPE atPos);
	//////////////////////////////////////////////////////////////////////////
	// ������ �ױ�
    VOID                                SendItemCombineMsg(SLOTIDX fromIndex, SLOTIDX toIndex, POSTYPE fromPos, POSTYPE toPos, DURATYPE itemCount = 0);
	//////////////////////////////////////////////////////////////////////////
	// ���尡�� ��Ż ������ ����.
	VOID								SendItemPortalMsg( POSTYPE CoordPos, POSTYPE PortalPos );


	bool								SetItemLock(SLOTIDX slotIndex, POSTYPE atPos, bool bLock);

	//////////////////////////////////////////////////////////////////////////
	// ������ ���� �ʱ�ȭ 
	VOID								SendStatInitItem( POSTYPE FromPos, int iNum);



	void								SendTopOfTrialItemMsg( POSTYPE FromPos );



	//////////////////////////////////////////////////////////////////////////
	// NAK ���õ� ITEM ERROR ������ �����ֱ�
	VOID								NoticeReasonItemErrorCode(DWORD dwErrCode);

	//////////////////////////////////////////////////////////////////////////
	// ����ȭ �����ؼ� �����۽ð� ���� ������ �Լ� ����.

public:
	VOID								AddUpdateItemSlot( SCSlot * pItemSlot );
	VOID								RemoveUpdateItemSlot( SCSlot * pItemSlot );
	VOID								UpdateItemSlot();
	VOID								ClearUpdateItemSlot();

    static bool                         CanPeriodExtend(SCItemSlot* item_slot);
    static bool                         IsChargeItem(SCItemSlot* item_slot);

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    bool                                CheckDrawDiceItem();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
    void                                UpdateOwnerList();
    void                                AddOwnerList(DWORD item_key);
    void                                ClearOwnerList();
    void                                UpdateCanGetACItem(SLOTCODE item_code);  //�Ϸ�� AC������ ����
    void                                UpdateMyItem();
	void								MessageforItem(POSTYPE ItemPos);	
	
    //////////////////////////////////////////////////////////////////////////
    void SerialCharCustomize(void* recv_packet);

    //////////////////////////////////////////////////////////////////////////
    // ���׸��� ��������Ŷ(C2S)
    bool IsInvalidEtheriaExtract(POSTYPE slot_pos, POSTYPE etherealizer_pos);
    void SendEtheriaCastingStart(POSTYPE slot_pos, POSTYPE etherealizer_pos);
    void SendEtheriaExtractPacket(POSTYPE slot_pos, POSTYPE etherealizer_pos);
    void SendEtheriaCombinationPacket(POSTYPE to_pos, POSTYPE from_pos);
    void SerialEtheriaCastingStart(EtheriaControl* header,EtheriaControlData* data);
    void SerialEtheriaCastingCancle(EtheriaControl* header,EtheriaControlData* data);
    //////////////////////////////////////////////////////////////////////////
    // ���׸��� �޴���Ŷ(S2C)
    void SerialEtheriaExtract(EtheriaControl* header,EtheriaControlData* data);
    void SerialEtheriaCombination(EtheriaControl* header,EtheriaControlData* data);
    bool IsInvalidSocketIdentify(POSTYPE slot_pos, POSTYPE socket_item_pos);
    void                                CheckRiderSummon();

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
    // ������ �Ⱓ����
    void SendExtendItemDate(POSTYPE source_pos, POSTYPE target_pos );
#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM
    void SendRandomizeItem(POSTYPE source_pos, POSTYPE target_pos );
#endif//_NA_003740_20111122_RANDOM_ITEM


private:
    //////////////////////////////////////////////////////////////////////////
    // ���̴����� ��ȯ��Ŷó��(��ȯ,��ȯ����,ĳ���ý���,���,��Ŷ����NAK)
    void                                SerialRiderSummon(RiderControl* header,RiderControlData* data);
    void                                SerialRiderSummonRelease(RiderControl* header,RiderControlData* data);
    void                                SerialRiderSyncCommand(RiderControl* header,RiderControlData* data);
    void                                SerialRiderCastingStart(RiderControl* header,RiderControlData* data);
    void                                SerialRiderCastingCancle(RiderControl* header,RiderControlData* data);
    //////////////////////////////////////////////////////////////////////////
    // ���̴����� ������Ŷó��(��������,Ż��.����)
    void                                SerialRiderWaxGloss(RiderControl* header,RiderControlData* data);
    void                                SerialRiderPartEquip(RiderControl* header,RiderControlData* data);
    void                                SerialRiderPartUnEquip(RiderControl* header,RiderControlData* data);
    void                                SerialRiderPartBrdFail(RiderControl* header,RiderControlData* data);
    //////////////////////////////////////////////////////////////////////////
    // ���̴����� ����ó��
    void                                SerialRiderFail(RiderControl* header,RiderControlData* data);
    void                                ResultRiderMessage(WORD wCode);


protected:
	VOID								moveLinkItem( SLOTIDX AtIndex, POSTYPE fromQuickPos, POSTYPE toQuickPos );
	VOID								moveLinkStyle( SLOTIDX AtIndex, POSTYPE fromQuickPos, POSTYPE toQuickPos );
	BOOL								IsItemDeleteAvalibeState(POSTYPE pos );

private:
	SLOTIDX								m_PendingExchangeContainIdx;
	POSTYPE								m_PendingExchangeItemPos;

	__PROPERTY(POSTYPE, PendingDeleteItemPos);
	__PROPERTY(POSTYPE, PendingUseItemSlotIdx);
	__PROPERTY(POSTYPE, PendingUseItemPos);

	TYPEDEF_MAP(UPDATE_ITEMSLOT, VOID *, SCSlot *);
	UPDATE_ITEMSLOT_MAP	m_UpdateItemSlotMap;
    std::vector<DWORD> update_owner_list_;    //������ ������ ����Ʈ
};

#define ITEMMGR()	ItemManager::Instance()	


#endif // __ITEM_MANAGER_H__