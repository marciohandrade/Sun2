#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

//=======================================================================================================================
// 아이템의 공통적인 처리를 담당하는 메니져 클래스
/**
	@author
			유 재 영( y2jinc@webzen.co.kr ) 
	@since
			2005 1. 2
	@remarks
			 - 각종 아이템 이동처리, 사용처리, 서버로의 메세지 처리
			 - 네트워크 패킷 파싱 처리
			 - ItemUseTransaction 추가
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
	EVENT,	// 이벤트 인벤토리에서 가져왔다.
	QUEST_OBTAIN,
	PRESENT,
    POINT_REWARD,   // 접속포인트 보상 //_NA_1419_20100121_ACCUMULATE_POINT
    CUBE,   //큐브
    SOCKET_COMPOSTE, //쟈드합성

    SHOP_STORAGE,   //!< 통합샵

	NONE,	//메시지없음
};

enum eDELETETOTALTYPE
{
	DELETE_NONE = -1,
	QUEST_DELETE = 0,
	COMPOSE_USE,
	DELETE_STAT_INIT_ITEM,	// 스텟 초기화 아이템을 사용.
};


enum eSLOT_SOUND_TYPE
{
	eSLOT_SOUND_TYPE_PICK,		// 집을 때
	eSLOT_SOUND_TYPE_PUT,		// 놓을 때
	eSLOT_SOUND_TYPE_EQUIP,		// 장착
	eSLOT_SOUND_TYPE_UNEQUIP,	// 탈착
	eSLOT_SOUND_TYPE_LINK,		// 링크
	eSLOT_SOUND_TYPE_UNLINK,	// 언링크
	eSLOT_SOUND_TYPE_DUMP,		// 버릴때

	eSLOT_SOUND_TYPE_NUM,
};

enum eKIND_OF_EQUIPED_SETITEM
{
	eKIND_OF_EQUIPED_SETITEM_NONE = -1,
	eKIND_OF_EQUIPED_SETITEM_ELITE = 0,	 // 엘리트 아이템
	eKIND_OF_EQUIPED_SETITEM_GENERAL = 1,  // 일반
	eKIND_OF_EQUIPED_SETITEM_DIVINE = 2, // 디바인
	eKIND_OF_EQUIPED_SETITEM_UNIQUE = 3, // 유니크
	eKIND_OF_EQUIPED_SETITEM_LUCKY  = 4, // 럭키
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
    // Returns :NUMTYPE : 갯수
    // Qualifier : 해당 컨테이너에 있는 관련 아이템 갯수 구함
    // Parameter : SLOTIDX AtSlotIdx : 컨테이너 인덱스
    // Parameter : eITEMTYPE item_type : 아이템 타입
    // Parameter : eWASTETYPE waste_type : 아이템 서브 타입
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
    
	// 클라이언트 테스트를 위한 함수
	BOOL								Test_InsertItemAtInventory( CODETYPE ItemCode, int ItemNum );

	// 내구도 처리 관련 함수
	VOID								RepairItem( SLOTIDX AtSlotIdx, POSTYPE pos );
	VOID								RepairItemAll( SLOTIDX AtSlotIdx );
	MONEY                               GetPriceRepairItemAll( SLOTIDX AtSlotIdx );
	BOOL	                            IsExistRepairItem( SLOTIDX AtSlotIdx );
    bool	                            IsExistRepairItem(SLOTIDX AtSlotIdx, DURATYPE less_dura);
	BOOL	                            IsExistLimitedItem( SLOTIDX AtSlotIdx );

	
	// 패킷 처리 부분 
	VOID								ParsePacket( MSG_BASE * pMsg );
	VOID								OnRecvItemSync(MSG_BASE * pMsg );
	VOID								OnrecvItemComposite(MSG_BASE * pMsg);
	VOID								OnrecvItemEnchant(MSG_BASE * pMsg);
	VOID								OnRecvStyleLink(MSG_BASE * pMsg );

	/// Item처리 부분
    VOID                                MoveItem(SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos, DURATYPE itemCount);
    VOID								MoveItemWithDiv();

    //! 자동 사양 컨테이너에 아이템 링크
    void AutobotLinkItem(SLOTIDX from_container_index, POSTYPE from_pos, POSTYPE to_pos, SLOTCODE org_code = 0);
    void AutobotLinkSkill(POSTYPE to_pos, SLOTCODE org_code);

	// QuickSlot 처리
	VOID								QuickLinkItem( SLOTIDX atIndex, POSTYPE OrgPos, POSTYPE toQuickPos, SLOTCODE OrgCode = 0);
	VOID								QuickLinkSkill( SLOTIDX atIndex, SLOTCODE OrgCode, POSTYPE toQuickPos ); 
	// StyleSlot 처리
	VOID								StyleLink(SLOTCODE OrgCode, POSTYPE toQuickPos ); 
	// Quick/Style 공동 처리
	VOID								SimplexMoveLinkItem( SLOTIDX AtIndex, POSTYPE fromPos, POSTYPE toQuickPos );

	// Dumm Item 처리 부분
	VOID								LinkDummyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos );								


	VOID								TradeBuyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos );
	VOID								TradeSellItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos );

	VOID								UseInventoryItem( SLOTIDX AtIndex, POSTYPE AtPos );
	VOID								UseEquipmentItem( SLOTIDX AtIndex, POSTYPE AtPos );


    void IncItemCount( SLOTIDX AtIndex, POSTYPE AtPos, int Count );

	VOID								CopyItem( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos );

	// 아이템 composite 관련 처리
	BOOL								SpendCompositeMaterials(
											eITEMCOMPOSITECODE eItemCompositeType, 
											SLOTIDX	fromSlotIndex
										);

	BOOL								AutoRegisterCompositeMaterials(
											eITEMCOMPOSITECODE eItemCompositeType, 
											SLOTIDX	atSlotIndex = SI_ITEM_ETC_COMPOSE,
											SLOTIDX	fromSlotIndex = SI_INVENTORY
											);	

	// Rank 관련
	eRANK_LEVEL							GetCompRankLevel(SCItemSlot & rItemSlot );
	// Enchant 관련
	eENCHANTLEVEL						GetCompEnchantLevel(SCItemSlot & rItemSlot );
	// Item 조건 처리

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

	//BOOL								CanTradeItem( BASE_ITEMINFO * pInfo, int iAction ) const;	// 클라이언트에서는 아이템 거래 타입에 따른 체크 안함

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
	
	// Item Composite 조건 
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
	
	// 퀵컨테이너 관련
	int									GetOverlapItemCount( CODETYPE code, SLOTIDX AtIndex = SI_INVENTORY);
	BOOL								FindFirstItem( CODETYPE code, POSTYPE & OUT rOutPos, SLOTIDX AtIndex = SI_INVENTORY);
	BOOL								FindFirstItem( eITEMTYPE eItemTypeInfo, POSTYPE & OUT rOutPos, SLOTIDX AtIndex = SI_INVENTORY);
    VOID UpdateQuickInfo(eSlotContainerIndex container_index = SI_QUICK);
    //------------------------------------------------------------------------------
    // Returns :SCItemSlot*
    // Qualifier : 
    // Parameter : SLOTIDX AtSlotIdx : 컨테이너 인덱스
    // Parameter : eITEMTYPE item_type : 아이템 타입
    // Parameter : eWASTETYPE waste_type : 아이템 서브 타입
    // Parameter : SLOTCODE item_code : 아이템 코드
    // Parameter : NUMTYPE less_count : 해당 갯수 보다 작은 슬롯 ( 뒤에서 부터 검색한다)
    //------------------------------------------------------------------------------
    SCItemSlot* FindFirstItemSlot(SLOTIDX AtSlotIdx, eITEMTYPE item_type);
    SCItemSlot* FindFirstItemSlot(SLOTIDX AtSlotIdx, eITEMTYPE item_type, eWASTETYPE waste_type);
    SCItemSlot* FindFirstItemSlot(SLOTIDX AtSlotIdx, SLOTCODE item_code);
    SCItemSlot* FindFirstItemSlot(SLOTIDX AtSlotIdx, SLOTCODE item_code, NUMTYPE less_count);

	BOOL								FindActiveChargeItem(eITEMTYPE eItemTypeInfo, POSTYPE & OUT rOutPos, SLOTIDX AtIndex = SI_INVENTORY);

	//--
	// 아래 함수는 사라질 것이다.
	VOID								UpdateQuickForEquipItemAll();
	VOID								UpdateValidQuickItem();
	//--

public:
	//////////////////////////////////////////////////////////////////////////
	// 패킷 센드 관련
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 필드 아이템 관련
	VOID								SendPickupItem(DWORD ItemObjectKey);
	VOID								SendPickupMoney(DWORD ItemObjectKey);
    void                                SendPickUpItemObject(Item* item_object);
	VOID								SendDropItemMsg( SLOTIDX FromIdx, POSTYPE FromPos);
	
	//////////////////////////////////////////////////////////////////////////
	// 아이템 스탯 체인지 아이템
	VOID								SetIndexPosInStatChangePkt(SLOTIDX Idx, POSTYPE Pos);
	VOID								SetChangeItemPosInStatChangePkt(POSTYPE ChangeItemPos);
	VOID								SendStatChangeItem();

private:
	MSG_CG_ITEM_STAT_CHANGE_USE_SYN		m_Item_Stat_Change_Use_Syn;

    SLOTIDX                             m_verify_dropFromIdx;
    POSTYPE                             m_verify_dropFromPos;

public:
    VOID                                VerifiedSendDropItemMsg();

	// 아이템 삭제
	VOID								SendDeleteItemMsg( POSTYPE FromPos );

	
	BOOL								AddFieldItem( const ITEM_RENDER_INFO * pItemInfo );
	void								AddItemForSingleMode( DWORD dwObjectKey, DWORD dwID );
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 아이템 거래 관련
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
	// 아이템 이동
    VOID								SendItemMoveMsg( SLOTIDX fromIdx, SLOTIDX toIdx, POSTYPE fromPos, POSTYPE toPos, DURATYPE ItemCount = 0);
private:
    // 임시 코드 정리할 것
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
	// 아이템 수리
	VOID								SendItemRepairMsg( SLOTIDX AtIdx, POSTYPE Pos, bool bAllRepair = false );
	//////////////////////////////////////////////////////////////////////////
	// 퀵슬롯 관련.
	VOID								SendQuickItemLinkMsg(POSTYPE OrgPos, POSTYPE ToPos);
	VOID								SendQuickSkillLinkMsg(SLOTCODE OrgCode, POSTYPE ToPos);
	VOID								SendSimplexMoveLinkMsg(POSTYPE fromPos, POSTYPE ToQuickPos);
	VOID								SendQuickItemRemoveMsg(POSTYPE AtPos );
	// 스타일 관련.
	VOID								SendQuickStyleLinkMsg(SLOTCODE OrgCode, POSTYPE ToPos);
	VOID								SendQuickStyleMoveLinkMsg(POSTYPE fromPos, POSTYPE ToPos);
	VOID								SendQuickStyleRemoveMsg(POSTYPE AtPos );

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 창고 관련.
	VOID								SendWareHouseItemAllSyn();
	//////////////////////////////////////////////////////////////////////////
	// 아이템 사용
	VOID								SendItemUseMsg(SLOTIDX	atIndex, POSTYPE atPos);
	//////////////////////////////////////////////////////////////////////////
	// 아이템 쌓기
    VOID                                SendItemCombineMsg(SLOTIDX fromIndex, SLOTIDX toIndex, POSTYPE fromPos, POSTYPE toPos, DURATYPE itemCount = 0);
	//////////////////////////////////////////////////////////////////////////
	// 저장가능 포탈 아이템 관련.
	VOID								SendItemPortalMsg( POSTYPE CoordPos, POSTYPE PortalPos );


	bool								SetItemLock(SLOTIDX slotIndex, POSTYPE atPos, bool bLock);

	//////////////////////////////////////////////////////////////////////////
	// 아이템 스텟 초기화 
	VOID								SendStatInitItem( POSTYPE FromPos, int iNum);



	void								SendTopOfTrialItemMsg( POSTYPE FromPos );



	//////////////////////////////////////////////////////////////////////////
	// NAK 관련된 ITEM ERROR 이유를 보여주기
	VOID								NoticeReasonItemErrorCode(DWORD dwErrCode);

	//////////////////////////////////////////////////////////////////////////
	// 유료화 관련해서 아이템시간 갱신 관련한 함수 모음.

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
    void                                UpdateCanGetACItem(SLOTCODE item_code);  //완료된 AC아이템 갱신
    void                                UpdateMyItem();
	void								MessageforItem(POSTYPE ItemPos);	
	
    //////////////////////////////////////////////////////////////////////////
    void SerialCharCustomize(void* recv_packet);

    //////////////////////////////////////////////////////////////////////////
    // 에테리아 보내는패킷(C2S)
    bool IsInvalidEtheriaExtract(POSTYPE slot_pos, POSTYPE etherealizer_pos);
    void SendEtheriaCastingStart(POSTYPE slot_pos, POSTYPE etherealizer_pos);
    void SendEtheriaExtractPacket(POSTYPE slot_pos, POSTYPE etherealizer_pos);
    void SendEtheriaCombinationPacket(POSTYPE to_pos, POSTYPE from_pos);
    void SerialEtheriaCastingStart(EtheriaControl* header,EtheriaControlData* data);
    void SerialEtheriaCastingCancle(EtheriaControl* header,EtheriaControlData* data);
    //////////////////////////////////////////////////////////////////////////
    // 에테리아 받는패킷(S2C)
    void SerialEtheriaExtract(EtheriaControl* header,EtheriaControlData* data);
    void SerialEtheriaCombination(EtheriaControl* header,EtheriaControlData* data);
    bool IsInvalidSocketIdentify(POSTYPE slot_pos, POSTYPE socket_item_pos);
    void                                CheckRiderSummon();

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
    // 아이템 기간연장
    void SendExtendItemDate(POSTYPE source_pos, POSTYPE target_pos );
#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM
    void SendRandomizeItem(POSTYPE source_pos, POSTYPE target_pos );
#endif//_NA_003740_20111122_RANDOM_ITEM


private:
    //////////////////////////////////////////////////////////////////////////
    // 라이더관련 소환패킷처리(소환,소환해제,캐스팅시작,취소,패킷실패NAK)
    void                                SerialRiderSummon(RiderControl* header,RiderControlData* data);
    void                                SerialRiderSummonRelease(RiderControl* header,RiderControlData* data);
    void                                SerialRiderSyncCommand(RiderControl* header,RiderControlData* data);
    void                                SerialRiderCastingStart(RiderControl* header,RiderControlData* data);
    void                                SerialRiderCastingCancle(RiderControl* header,RiderControlData* data);
    //////////////////////////////////////////////////////////////////////////
    // 라이더관련 파츠패킷처리(파츠장착,탈착.광택)
    void                                SerialRiderWaxGloss(RiderControl* header,RiderControlData* data);
    void                                SerialRiderPartEquip(RiderControl* header,RiderControlData* data);
    void                                SerialRiderPartUnEquip(RiderControl* header,RiderControlData* data);
    void                                SerialRiderPartBrdFail(RiderControl* header,RiderControlData* data);
    //////////////////////////////////////////////////////////////////////////
    // 라이더관련 실패처리
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
    std::vector<DWORD> update_owner_list_;    //소유권 갱신할 리스트
};

#define ITEMMGR()	ItemManager::Instance()	


#endif // __ITEM_MANAGER_H__