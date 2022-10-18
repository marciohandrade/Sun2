#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__
#pragma once

//=======================================================================================================================
/// ItemManager
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 8. 11~
	@history
		- 2004. 12. 28 : Drop, Pick, Link, Unlink 구현
		- 2006/1/17 : 랭크의 기능성 함수 분리 (ItemRankUp(), GetRankOptionItemType(), CreateRankItemSocketHole())
						외부에서 사용하기 위해 static처리 
		- 2006/2/14(초코렛day) : divide()함수 추가 : 초코렛도 나눠먹자는 취지로다가..=_=
		- 2006/3/14(whiteday) : 조합관련 Compose(), crystalize()함수 구현 - 유연함과 확장성 추구
		- 2006/3/15(taiyo's birthday) : ItemManger_Util.cpp추가 각종 조작함수들 추가 및 리팩토링(Obtain,Delete,Check관련함수)
        - ~~ 2010/3/25 || messages after history is the bottom this line
        - 2010/3/25(WAVERIX) : add a file, the ItemManager_Types.h, and move type definitions to it
        - continued (~27) (WAVERIX)
            > 
        - 2010/5/25(WAVERIX) : change a divine field to compatible the content of the item customizing
        - 2010/4/01~, waverix : the ItemManager that is a mammoth object is distributed to sub-implements
                                decentering complex facilities
*/
//=======================================================================================================================

#include <ResultCode.h>
#include <ItemInfoParser.h>

#include <SCSlotHeader.h>
#include <SCItemSlot.h>
#include <SCSkillSlot.h>
#include "ShopInfoParser.h"

#include <ServerStructInPacket.h>

#include "./FatigueManager.h"

#include "./ItemManager_Types.h"
//==================================================================================================
// sub-implements
#include "./Slot/ItemManager_Durability.h"
#include "./Slot/ItemManager_Move.h"
#include "./Slot/ItemManager_Trading.h"
#include "./Slot/ItemManager_Updater.h"
#include "./Slot/ItemManager_ItemFunction.h"

class CGuildWarehouseSlotContainer;
//
//==================================================================================================
//
//
class ItemManager
{
public:
    //----------------------------------------------------------------------------------------------
    ItemManager();
    ~ItemManager();

    void Init(SlotManager* pMGR);
    void Release();
    //----------------------------------------------------------------------------------------------
    //
    SCItemSlotContainer* GetItemSlotContainer(const SLOTIDX Index) const;
    bool ValidPos(
        const SLOTIDX slot_index, const POSTYPE pos_in_slots, const bool check_empty = true) const;
    bool ValidState() const;
    bool ValidContainer(SLOTIDX slot_index);
    bool IsEmpty(SLOTIDX slot_index, POSTYPE pos_in_slots);
    //
    bool IsRestrictFunction(eFUNCTIONALRESTRICT_TYPE type) const;
    // special methods... need renewal with access privilege
    void UnbindGuildWarehouseContainer();
    void BindGuildWarehouseContainer(CGuildWarehouseSlotContainer* guild_warehouse_container);

    //----------------------------------------------------------------------------------------------
    //
    template <class FunctorInterface>
    bool ForeachSlot(SLOTIDX slot_index, const FunctorInterface* functor) const;

    //----------------------------------------------------------------------------------------------
    // NOTE: 각종 아이템 상태 업데이트
    void Update();
    // NOTE: this method is the root to support various item operations like Insert, Update and Delete
    nsSlot::ItemQueryResult Query(nsSlot::ItemQuery* item_query);
    //----------------------------------------------------------------------------------------------
    // CHANGES: f110404.2L, the sub-implements to decenter complex facilities
    nsSlot::ItemManagerUpdateImpl* GetUpdaterImpl();
    nsSlot::DurabilityImpl* GetDurabilityImpl();
    nsSlot::ItemMoveImpl* GetItemMoveImpl();
    nsSlot::ItemTradingImpl* GetTradingImpl();
    //
    nsSlot::ItemFunctionDropProtector       GetItemFunctionDropProtector();
    nsSlot::ItemFunctionCoordinate          GetItemFunctionCoordinate();
    nsSlot::ItemFunctionSummon              GetItemFunctionSummon();
    nsSlot::ItemFunctionBattlezoneEnterance GetItemFunctionBattlezoneEnterance();
    nsSlot::ItemFunctionPlayerProperty      GetItemFunctionPlayerProperty();
    nsSlot::ItemFunctionItemProperty        GetItemFunctionItemProperty();
    nsSlot::ItemFunctionEnchant             GetItemFunctionEnchant();
    nsSlot::ItemFunctionPet                 GetItemFunctionPet();
    nsSlot::ItemFunctionItemLimitation      GetItemFunctionItemLimitation();
    nsSlot::ItemFunctionLottery             GetItemFunctionLottery();
    nsSlot::ItemFunctionEtherWeapon         GetItemFunctionEtherWeapon();
    nsSlot::ItemFunctionIdentity            GetItemFunctionIdentity();
    nsSlot::ItemFunctionCustomization       GetItemFunctionCustomization();
    nsSlot::ItemFunctionSkill               GetItemFunctionSkill();
    nsSlot::ItemFunctionSocket              GetItemFunctionSocket();
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    nsSlot::ItemFunctionAwakening           GetItemFunctionAwakening();
    nsSlot::ItemFunctionEvolution           GetItemFunctionEvolution();
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    //----------------------------------------------------------------------------------------------
    // NOTE: f091202.x, WOPS:5334, 인벤잠금 상태에서 아이템 제거 가능한 특수 처리(PK드랍)
    struct TransactionClosure { // single transaction, not support duplicated operation
        enum eTransType;
        typedef eTransType value_type;
        ItemManager* const item_manager_;
        TransactionClosure(ItemManager* ptr, value_type action);
        ~TransactionClosure();
        __DISABLE_COPY(TransactionClosure);
    };
    TransactionClosure::value_type GetCurrentTransactionAction() const;
    //----------------------------------------------------------------------------------------------
    //    --- 각종 아이템 처리 인터페이스 함수들 ---
    // CHANGES: f110418.1L, Move : use GetItemMoveImpl()->Move(...)
    RC::eITEM_RESULT Drop(SLOTIDX atIndex, POSTYPE atPos, DWORD dwOwnerKey=0); // Slot -> GameField 버리기
    RC::eITEM_RESULT Pick(DWORD dwItemObjectKey, INVENTORY_TOTAL_INFO* OUT pTotalInfo);
    RC::eITEM_RESULT Pick(POSTYPE NumberOfItems, SCItemSlot& rItemSlot,
                          INVENTORY_TOTAL_INFO* OUT pTotalInfo);
#ifdef _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE
    RC::eITEM_RESULT PickParty( DWORD dwItemObjectKey, const SERVER_COMMON_OPTION& ServerOption );
#endif //_NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE

    RC::eITEM_RESULT Combine(const SLOTIDX from_slot_type, 
                             const POSTYPE from_slot_position, 
                             const SLOTIDX to_slot_type, 
                             const POSTYPE to_slot_position, 
                             DURATYPE& INOUT combine_quantity); // 소모용 아이템 합치기
    RC::eITEM_RESULT Divide(POSTYPE fromPos, POSTYPE toPos, DURATYPE fromNum, DURATYPE toNum); // 소모용 아이템 나누기

    // 기본적인 아이템 사용 요청이 아니라 별도의 패킷에서 처리되어야 하는 아이템 인지 검사한다.
    bool IsExtraPacketUse(
        const SLOTIDX item_slot_type, const POSTYPE item_slot_position) const;

    RC::eITEM_RESULT Use(SLOTIDX atIndex, POSTYPE atPos, SLOTCODE& OUT ItemCode,
                         SkillInfo* pSkillInfoData = NULL,
                         BYTE byAttackPropensity = ATTACK_PROPENSITY_NORMAL); // 소모용 아이템 사용하기

    // 위 Use() 함수를 사용하는 특정 아이템 타입 검사와 아이템 사용 응답 기능이 추가된 유틸리티 함수
    RC::eITEM_RESULT UseItemByType(
        const eITEMTYPE item_type, 
        const SLOTIDX item_container_type, 
        const POSTYPE item_position, 
        const bool use_response, 
        const TCHAR* called_function);

    RC::eITEM_RESULT Delete(POSTYPE atPos);
    RC::eITEM_RESULT Delete(SLOTIDX fromIndex, POSTYPE atPos, POSTYPE num, int iLogType);
    RC::eITEM_RESULT DeleteWastedItem(SLOTIDX atIndex, POSTYPE atPos);
    RC::eITEM_RESULT IsValidItem(SLOTIDX atIndex, POSTYPE atPos, eITEMTYPE Itemtype);
    //----------------------------------------------------------------------------------------------
    //  옵션처리
	void             SendItemChange(SCItemSlot& rTargetSlot);
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    RC::eITEM_RESULT Compose(SLOTCODE ComposeListCode, BYTE byMakingNum,
        INVENTORY_TOTAL_INFO* OUT pTotalInfo, BYTE &bySuccessNum, BYTE &byPreventNum);
#else  // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    RC::eITEM_RESULT Compose(SLOTCODE ComposeListCode, BYTE byMakingNum,
        INVENTORY_TOTAL_INFO* OUT pTotalInfo, BYTE &bySuccessNum);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
#else
    RC::eITEM_RESULT Crystalize(SLOTCODE CrystalizeListCode, POSTYPE TargetPos,
        INVENTORY_TOTAL_INFO* OUT pTotalInfo);
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE
    //----------------------------------------------------------------------------------------------
    //
    RC::eITEM_RESULT Lose(SLOTCODE ItemCode, BOOL bAllOfItem);
    void             Lose(SCSlotContainer* pContainer, POSTYPE pos, int num);
    RC::eITEM_RESULT DropMoney(MONEY Money);
#ifdef _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
    void UpdateItemAttributes(bool recalced_equip_items, bool recalced_inven_option_items);
    // CHANGES: f110422.2L, refresh CEquipmentSlotContainer::equip_existences_
    void RefreshEquipItemsCache();
#else
    void UpdateAttrForEquipItem();
    void UpdateAttrInventory(); // Inventory에 대해서만 UpdateAttr수행
#endif
    // (f100527.4L)
    //template< class ITEM_TOTAL_INFO_TYPE >
    //void             SerializeItemTotalInfo(ITEM_TOTAL_INFO_TYPE& ItemInfo, eSERIALIZE eType);
    // KINDS_OF_INVENTORY_TOTAL_INFO = { _INVENTORY_TOTAL_INFO, INVENTORY_TOTAL_INFO }
    template<class KINDS_OF_INVENTORY_TOTAL_INFO>
    void SerializeInventoryTotalInfo(KINDS_OF_INVENTORY_TOTAL_INFO& total_info, eSERIALIZE);
    void SerializeItemTotalInfo(_ITEM_TOTAL_INFO& total_info, eSERIALIZE serialize_type);
    //template<class ITEMSTREAM_TYPE>
    //void SerializeItemStream(SLOTIDX slot_index, POSTYPE pos, ITEMSTREAM_TYPE* IN stream,
    //                         eSERIALIZE serialize_type = SERIALIZE_LOAD);
    void SerializeItemStream(SLOTIDX slot_index, POSTYPE pos, OPT_ITEMSTREAMEX* IN stream,
                             eSERIALIZE serialize_type = SERIALIZE_LOAD);

    //----------------------------------------------------------------------------------------------
    //    조합의 기능성 메서드
    //    랭크시 사용
#if !defined(_NA_000251_20100727_SOCKET_SYSTEM)
    RC::eITEM_RESULT RankUp(POSTYPE TargetPos1, POSTYPE TargetPos2,
                            POSTYPE& OUT RankUpItemPos, POSTYPE& OUT DeletedItemPos,
                            MONEY& OUT RankUpFare, BOOL& OUT bCreatedSocket);
    static BOOL      ItemRankUp(SCItemSlot* pItemSlot);     
    static BOOL      ItemRankDown(SCItemSlot* pItemSlot);   
    RC::eITEM_RESULT RankDown(POSTYPE RankDownItemPos);     
    static BOOL      CreateRankItemSocketHole(SCItemSlot* pItemSlot, BYTE byCurRank);   
#endif // _NA_000251_20100727_SOCKET_SYSTEM

    //----------------------------------------------------------------------------------------------
    //    ItemManger_Util.cpp
    //
    //    특정 아이템 개수가 존재하는지 여부 체크
    // (CHANGES) (f100327.2L) added by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_ \
    // and other 'CheckInsertItem's are removed argument initial value
    // <returns> assert(result >= 0 &&
    //                  result <= number_of_items)
    //  number_of_items == result : succeed
    //  number_of_items >  result : failed, the empty slot requirement is insufficient.
    //  number_of_items <  result : X, violate the assertion
    int CheckInsertItem(SLOTIDX slot_index, SLOTCODE item_code, int number_of_items);
    //----------------------------------------------------------------------------------------------
    //    Deletes
    RC::eITEM_RESULT DeleteAllKindOfItem(POSTYPE KindsNum,
                                         const SLOTCODE* IN pItemCode, const POSTYPE* IN pItemNum,
                                         INVEN_DELETE_TOTAL_INFO* OUT pTotalInfo,
                                         SCItemSlot* const deleted_item_slot = NULL);

    RC::eITEM_RESULT DeleteOneKindOfItem(const SLOTCODE del_item_code, 
                                         const WORD number_of_del_item,
                                         INVEN_DELETE_TOTAL_INFO* OUT inven_delete_total_info);
    // 인벤 전체 삭제
    BOOL             DeleteAllInvenItem(INVEN_DELETE_TOTAL_INFO* OUT pTotalInfo, bool bDelPartial = true);
    void             Delete(SCSlotContainer* pContainer,
                            const POS_INFO* IN p_pos_info, POSTYPE IN pos_info_num);
    void             Delete(SCSlotContainer* pContainer,
                            const POS_INFO_EX* IN p_pos_info, POSTYPE IN pos_info_num);
    //----------------------------------------------------------------------------------------------
    //    한 종류의 아이템을 1개 이상을 획득 할 수 있는 함수
    RC::eITEM_RESULT Obtain(SLOTCODE ItemCode, int ItemNum,
                            INVENTORY_TOTAL_INFO* OUT pTotalInfo);
    RC::eITEM_RESULT Obtain(const SCItemSlot& IN rItemSlot, int ItemNum,
                            INVENTORY_TOTAL_INFO* OUT pTotalInfo);
    RC::eITEM_RESULT Obtain(const SCItemSlot& IN rItemSlot, int ItemNum,
                            JUST_INVENTORY_TOTAL_INFO* OUT pTotalInfo);
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    // NOTE: f111006.3L, changes the method specification.
    // this method support single item purchase action for GameShop service
    RC::eITEM_RESULT ObtainAtEmptySlot(
        const KindOfItemTag item_tag, 
        const SLOTCODE item_code, 
        const POSTYPE item_num,
        const BYTE enchant_count,
        const BYTE item_type,
        const BYTE socket_count,
        const DWORD delete_time,
        INVENTORY_TOTAL_INFO* OUT inven_total_info);
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL

    // 아이템 종류, 개수별 획득 함수들
    // !!! AllKindOf 계열의 함수 주의사항 : ItemCode array에 반드시 다른 ItemCode가 들어가야 한다.
    RC::eITEM_RESULT ObtainAllKindOfItem(POSTYPE KindsNum,    // (pItemCode, pItemNum, options) array number
                                         const SLOTCODE* IN pItemCode, const POSTYPE* IN pItemNum,
                                         INVENTORY_TOTAL_INFO* OUT pTotalInfo);
    RC::eITEM_RESULT ObtainAllKindOfItem(POSTYPE KindsNum,    // (pItemCode, pItemNum, options) array number
                                         const SLOTCODE* IN pItemCode, const POSTYPE* IN pItemNum,
                                         JUST_INVENTORY_TOTAL_INFO* OUT pTotalInfo);
    // (pItemCode, pItemNum, options) array number
    // (CHANGES) (f100525.1L) item_type_array = change from a divine to a kind of item
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    RC::eITEM_RESULT ObtainAllKindOfItem(POSTYPE number_of_kinds,
        const SLOTCODE* IN item_code_array,
        const POSTYPE* IN item_num_array, 
        const BYTE* IN enchant_array,
        const BYTE* IN item_type_array,
        const BYTE* IN rank_array, 
        INVENTORY_TOTAL_INFO* OUT inven_total_info,
        const DWORD* IN datetime_array = NULL);
#else
    RC::eITEM_RESULT ObtainAllKindOfItem(POSTYPE number_of_kinds,
        const SLOTCODE* IN item_code_array,
        const POSTYPE* IN item_num_array, 
        const BYTE* IN enchant_array,
        const BYTE* IN item_type_array,
        const BYTE* IN rank_array, 
        INVENTORY_TOTAL_INFO* OUT inven_total_info);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
    //----------------------------------------------------------------------------------------------
    // (CHANGES) (f100325.2L) added by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_ \
    // and other 'ExistItem's are removed argument initial value
    int ExistItem(SLOTIDX slot_index, SLOTCODE item_code, int item_num);

    // (NOTE) the interface for shop items purchase
    int              ReservedSpace(SCSlotContainer* pContainer,
                                   const SLOTCODE ItemCode IN, const POSTYPE ItemNum IN,
                                   IN    POSTYPE      LockedInfoMaxSize,
                                   INOUT LOCKED_INFO* pLockedInfo,
                                   INOUT POSTYPE*     pUsedLockedCount);
    // (NOTE) the interface for shop items purchase
    // (CHANGES) (f100526.1L) change from the name of a 'Reserved~' to the name of a 'Reserve~'
    RC::eITEM_RESULT ReserveSpaceAllKindOfItem(POSTYPE number_of_kinds,
                                               const SLOTCODE* item_code_array IN,
                                               const POSTYPE* item_num_array IN,
                                               const DWORD* datetime_array IN,
                                               const BYTE* enchant_array IN,
                                               const BYTE* item_type_array IN, // (f100525.1L)
                                               const BYTE* rank_array IN,
                                               POSTYPE number_of_locked_info IN,
                                               LOCKED_INFO* locked_info OUT,
                                               POSTYPE* number_of_used_locked_info OUT);
    // (NOTE) the interface for extend shop items purchase
    RC::eITEM_RESULT ReservedExtendDateItem(const SLOTCODE* pItemCode IN,
                                            SLOTIDX SlotIndex, POSTYPE SlotPos,
                                            OUT LOCKED_INFO* pLockedInfo,
                                            OUT POSTYPE*     pUsedLockedCount);
    // (NOTE) the interface for extend shop items purchase
    RC::eITEM_RESULT ExtendDateOfItem(POSTYPE KindsNum, const SLOTCODE* pItemCode,
                                      const DWORD* pDateTime,
                                      SLOTIDX SlotIndex, POSTYPE ExtendSlotPos);
#if !defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
    // (NOTE) the interface for shop items purchase
    // (CHANGES) (f100525.4L) change the name,
    // and the inven_total_info value will be a possibility of equal to null by the content of
    // a _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
    RC::eITEM_RESULT ObtainAllKindOfItemForShop(POSTYPE number_of_kinds,
                                                const KindOfItemTag all_apply_option_tag,
                                                const SLOTCODE* item_code_array IN,
                                                const POSTYPE* item_num_array IN,
                                                const DWORD* datetime_array IN, 
                                                const BYTE* enchant_array IN,
                                                const BYTE* item_type_array IN,
                                                const BYTE* rank_array IN,
                                                JUST_INVENTORY_TOTAL_INFO* inven_total_info OUT);
#endif
    // (NOTE) the interface for shop items purchase or event inventory
    void             ObtainReservedAllKindOfItem(const KindOfItemTag all_apply_option_tag,
                                                 LOCKED_INFO* locked_info IN,
                                                 POSTYPE number_of_locked_info IN,
                                                 JUST_INVENTORY_TOTAL_INFO* inven_total_info OUT);
    // (NOTE) the interface for shop items purchase or event inventory(does not use currently)
    void             RollbackReservedAllKindOfItem(LOCKED_INFO* pLockedInfo IN,
                                                   POSTYPE UsedLockedCount IN);

    BOOL             IsUnEquipAll();
    void             SendSetItemBrd(BOOL bFullOption);

    //----------------------------------------------------------------------------------------------

    RC::eITEM_RESULT CanPick(Item* pItem);

    // The interface to update query
    bool ObtainAllKindOfItemByQuery(const nsSlot::ItemQuery& query);
    // (CHANGES) (f100525.1L) item_type_array = change from a divine to a kind of item
    // (바뀐점) (2010/11/01) _NA_000251_20100727_SOCKET_SYSTEM
    // 1. zard_option_array 추가, rank_array -> zard_option_array로 대체 예정이나 
    // 이전 버전의 코드 수정을 피하기 위해 남겨둠
    // 2. private -> public, ItemManager의 메서드가 늘어나는것을 피하기 위해 쟈드 아이템 전용 획득
    // 구현을 SocketSystemImplement로 옮김, 그 안에서 내부 구현 로직을 공통화 하기 위해
    // ObtainAllKindOfItemShared()을 호출하기 때문에 스코프를 변경함
    RC::eITEM_RESULT ObtainAllKindOfItemShared(nsSlot::ItemQuery* const obtain_query,
                                               POSTYPE number_of_kinds,
                                               const SLOTCODE* item_code_array,
                                               const POSTYPE* item_num_array,
                                               const BYTE* enchant_array,
                                               const BYTE* item_type_array,
                                               const BYTE* rank_array,
                                               const DWORD* datetime_array, 
                                               const WORD* zard_option_array);
private:
    //----------------------------------------------------------------------------------------------
    //
    typedef STLX_HASH_SET<SCItemSlot*> EXPIRATIONITEM_HASH;
    typedef STLX_HASH_MAP<DWORD64, DWORD> ABUSESERIAL_HASH; // use nsSlot::AbuseSlotNode
    typedef STLX_DEQUE<RepurchaseSlot> SALE_ITEM_DEQ;
    //----------------------------------------------------------------------------------------------
    int _ExistItem(SCSlotContainer* item_container,
                   SLOTCODE item_code, int item_num,
                   IN    POSTYPE      max_pos_info_range,
                   OUT   POS_INFO_EX* p_pos_info,
                   INOUT POSTYPE*     p_pos_info_count);
    int _ExistItem(SLOTIDX slot_index,
                   SLOTCODE item_code, int item_num,
                   IN    POSTYPE      max_pos_info_range,
                   OUT   POS_INFO_EX* p_pos_info,
                   INOUT POSTYPE*     p_pos_info_count);
    //----------------------------------------------------------------------------------------------
    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
    // The interface to find targets which are items with number
    bool Query_Select(nsSlot::ItemQueryResult* result, nsSlot::ItemQuery* query);
    // The interface to find empty slots or remains of stacked slots \
    // which are considered items with number
    bool Query_InverseSelect(nsSlot::ItemQueryResult* result, nsSlot::ItemQuery* query);
    // The interface to find empty slots or remains of stacked slots \
    // which are considered items with number and inventory sorted insert configuration
    bool Query_InverseSelectSort(nsSlot::ItemQueryResult* result, nsSlot::ItemQuery* query);
    // CHANGES, f100903.4L, the algorithm that a duplicable item must find an exist slot first has
    // six phase depth complexity. so that a below method is split from a Query_InverseSelectSort
    // to support a mode control like not-empty, empty, and that. considering a tab sort priority.
    bool Query_InverseSelectSortInternal(nsSlot::ItemQueryResult* result, nsSlot::ItemQuery* query);
    //----------------------------------------------------------------------------------------------
#ifdef _NA_006689_20130503_CANNOTUSE_BUFFITEM
    RC::eITEM_RESULT CanUseSkillItem(const BASE_ITEMINFO *const pInfo);
#endif //_NA_006689_20130503_CANNOTUSE_BUFFITEM
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
    BOOL DeleteComposeMaterials(ns_functor::DeleteComposeMaterialItem* functor, INVEN_DELETE_TOTAL_INFO* OUT delete_item_info);
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
    //----------------------------------------------------------------------------------------------
    Player* player_;
    SlotManager* slot_manager_;
    SCSlotContainer** slot_containers_;
    // CHANGES: f091202.x, WOPS:5334, 인벤잠금 상태에서 아이템 제거 가능한 특수 처리(PK드랍)
    TransactionClosure::value_type current_special_transaction_; // ref) eLOG_ITEM
    nsSlot::DurabilityImpl durability_impl_;
    nsSlot::ItemMoveImpl item_move_impl_;
    nsSlot::ItemTradingImpl trading_impl_;
    nsSlot::ItemManagerUpdateImpl updater_impl_;
    //
    ABUSESERIAL_HASH serial_check_table_;
    EXPIRATIONITEM_HASH expiration_item_table_;
#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
    std::set<SCItemSlot*> immediately_use_item_table_;
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE

    SALE_ITEM_DEQ saled_item_list_;
    ItemStatSwitch* item_switch_handle_;
    util::Timer item_update_timer_;
    //----------------------------------------------------------------------------------------------
    //
    friend struct TransactionClosure;
    friend class nsSlot::DurabilityImpl;
    friend class nsSlot::ItemMoveImpl;
    friend class nsSlot::ItemTradingImpl;
    friend class nsSlot::ItemManagerUpdateImpl;
    //
    friend class nsSlot::ItemFunctionDropProtector;
    friend class nsSlot::ItemFunctionCoordinate;
    friend class nsSlot::ItemFunctionSummon;
    friend class nsSlot::ItemFunctionBattlezoneEnterance;
    friend class nsSlot::ItemFunctionPlayerProperty;
    friend class nsSlot::ItemFunctionItemProperty;
    friend class nsSlot::ItemFunctionEnchant;
    friend class nsSlot::ItemFunctionPet;
    friend class nsSlot::ItemFunctionItemLimitation;
    friend class nsSlot::ItemFunctionLottery;
    friend class nsSlot::ItemFunctionEtherWeapon;
    friend class nsSlot::ItemFunctionIdentity;
    friend class nsSlot::ItemFunctionCustomization;
    friend class nsSlot::ItemFunctionSkill;
    friend class nsSlot::ItemFunctionSocket;
    friend class nsSlot::ItemFunctionToggle; //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    friend class nsSlot::ItemFunctionAwakening;
    friend class nsSlot::ItemFunctionEvolution;
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    //
    __DISABLE_COPY(ItemManager);
};


//==================================================================================================
//
//
template <class FunctorInterface>
bool ItemManager::ForeachSlot(SLOTIDX slot_index, const FunctorInterface* functor) const
{
    return slot_manager_->ForeachSlot(slot_index, functor);
}

//==================================================================================================
// (BUGFIX) (091202) (WOPS:5334) 인벤잠금 상태에서 아이템 제거 가능한 특수 처리(PK드랍)
//
// (CHANGES) (100202) Inventory 제어시 잠금상태에서 제어되는 경우, 적절한 상태제어가 되지 못하는 사
// 례들이 존재한다. 다양한 사례가 있을 수 있으며, 로그타입 제어 (PK드랍) -> 명시적 정의로 전환
enum ItemManager::TransactionClosure::eTransType {
    eTransType_None,
    eTransType_PKDrop,     // PK 드랍시 인벤 잠금 여부와 상관없이 제거할 수 있어야 한다.
    eTransType_RiderQuery, // 탈 것 아이템 정보는 얻는 과정... 잠금 상태는 별도로 필요한 경우 얻을 것.
    eTransType_ChangeDura, // to skip inventory lock on decrease durability
    // CHANGES: f110713.4L, when slot container rollback, this flag will expect success
    eTransType_ForcedEquipWithoutLimitation,
};

inline ItemManager::TransactionClosure::TransactionClosure(ItemManager* item_manager,
                                                           value_type action)
    : item_manager_(item_manager)
{
    assert(item_manager_->current_special_transaction_ == this->eTransType_None);
    item_manager_->current_special_transaction_ = action;
}

inline ItemManager::TransactionClosure::~TransactionClosure()
{
    item_manager_->current_special_transaction_ = this->eTransType_None;
};

inline ItemManager::TransactionClosure::value_type
ItemManager::GetCurrentTransactionAction() const
{
    return current_special_transaction_;
}

//==================================================================================================
// CHANGES: f110404.2L, the sub-implements to decenter complex facilities
inline nsSlot::ItemManagerUpdateImpl* ItemManager::GetUpdaterImpl() {
    return &updater_impl_;
};

inline nsSlot::DurabilityImpl* ItemManager::GetDurabilityImpl() {
    return &durability_impl_;
};

inline nsSlot::ItemMoveImpl* ItemManager::GetItemMoveImpl() {
    return &item_move_impl_;
};

inline nsSlot::ItemTradingImpl* ItemManager::GetTradingImpl() {
    return &trading_impl_;
};

//==================================================================================================

inline nsSlot::ItemFunctionDropProtector
ItemManager::GetItemFunctionDropProtector()
{
    return nsSlot::ItemFunctionDropProtector(this);
}

inline nsSlot::ItemFunctionCoordinate
ItemManager::GetItemFunctionCoordinate()
{
    return nsSlot::ItemFunctionCoordinate(this);
}

inline nsSlot::ItemFunctionSummon
ItemManager::GetItemFunctionSummon()
{
    return nsSlot::ItemFunctionSummon(this);
}

inline nsSlot::ItemFunctionBattlezoneEnterance
ItemManager::GetItemFunctionBattlezoneEnterance()
{
    return nsSlot::ItemFunctionBattlezoneEnterance(this);
}

inline nsSlot::ItemFunctionPlayerProperty
ItemManager::GetItemFunctionPlayerProperty()
{
    return nsSlot::ItemFunctionPlayerProperty(this);
}

inline nsSlot::ItemFunctionItemProperty
ItemManager::GetItemFunctionItemProperty()
{
    return nsSlot::ItemFunctionItemProperty(this);
}

inline nsSlot::ItemFunctionEnchant
ItemManager::GetItemFunctionEnchant()
{
    return nsSlot::ItemFunctionEnchant(this);
}

inline nsSlot::ItemFunctionPet
ItemManager::GetItemFunctionPet()
{
    return nsSlot::ItemFunctionPet(this);
}

inline nsSlot::ItemFunctionItemLimitation
ItemManager::GetItemFunctionItemLimitation()
{
    return nsSlot::ItemFunctionItemLimitation(this);
}

inline nsSlot::ItemFunctionLottery
ItemManager::GetItemFunctionLottery()
{
    return nsSlot::ItemFunctionLottery(this);
}

inline nsSlot::ItemFunctionEtherWeapon
ItemManager::GetItemFunctionEtherWeapon()
{
    return nsSlot::ItemFunctionEtherWeapon(this);
}

inline nsSlot::ItemFunctionIdentity
ItemManager::GetItemFunctionIdentity()
{
    return nsSlot::ItemFunctionIdentity(this);
}

inline nsSlot::ItemFunctionCustomization
ItemManager::GetItemFunctionCustomization()
{
    return nsSlot::ItemFunctionCustomization(this);
}

inline nsSlot::ItemFunctionSkill
ItemManager::GetItemFunctionSkill()
{
    return nsSlot::ItemFunctionSkill(this);
}

inline nsSlot::ItemFunctionSocket
ItemManager::GetItemFunctionSocket()
{
    return nsSlot::ItemFunctionSocket(this);
}
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
inline nsSlot::ItemFunctionAwakening
ItemManager::GetItemFunctionAwakening()
{
    return nsSlot::ItemFunctionAwakening(this);
}
inline nsSlot::ItemFunctionEvolution
ItemManager::GetItemFunctionEvolution()
{
    return nsSlot::ItemFunctionEvolution(this);
}
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
//==================================================================================================
//
// the support utility for item manager
#include "ItemManager_Util.h"

#define GAMESERVER_ITEM_MANAGER_INTER_DECLARATION
#include "./Slot/ItemManager_Functor.hxx"
// stream관련 template 구현부
#include "ItemManager_Stream.hpp"
#undef GAMESERVER_ITEM_MANAGER_INTER_DECLARATION
//
//==================================================================================================


#endif // __ITEM_MANAGER_H__

